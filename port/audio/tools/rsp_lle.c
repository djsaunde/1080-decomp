/*
 * port/audio/tools/rsp_lle.c: a minimal RSP interpreter that runs 1080's own audio microcode
 * (aspMain: text ROM 0xE16EDC, 0xF70 bytes at IMEM 0x1000; data ROM 0xDBF730, 0x2E0 bytes at
 * DMEM 0) on the CPU, as a reference for port/audio/mixer.c (docs/1080_PC_PORT_PLAN.md 15.11).
 *
 * Only what this ucode uses is implemented: the scalar subset (no mult / div), MTC2, the vector
 * ops VMULF VMUDL VMUDM VMUDN VMUDH VMACF VMADM VMADN VMADH VADD VSUB VADDC VSAR VAND VXOR, the
 * loads LSV LDV LQV LRV LPV and the stores SSV SLV SDV SQV, and the SP DMA / semaphore / status
 * registers.  Anything else stops the task and is counted (rsp_lle_stats.bad_insn).
 *
 * Memory model.  The host keeps the audio data in mixed byte order (plan 15.1): s16 buffers
 * (AI buffers, reverb rings, ADPCM / resample / filter states, codebooks, filter coefficients)
 * in host order, the command list as host u32 words, VADPCM / S8 sample bytes in ROM order.
 * RSP DMEM is big-endian, so each DMA converts by what it moves: the command list word-swapped,
 * sample bytes (a LOADBUFF whose DMEM data feeds an ADPCM / S8DEC input) as they are, everything
 * else s16-swapped.  The DMA's 24-bit DRAM address gets its top byte from the current command's
 * w1 (the address the builder wrote), as the RDRAM view of a host pointer.
 */
#include "rsp_lle.h"

#include <stdio.h>
#include <string.h>

RspLleStats rsp_lle_stats;
void (*rsp_lle_write_log)(uint8_t *host, uint32_t len, uint32_t op);

static uint8_t sImem[0x1000];
static uint8_t sDmemInit[0x1000];
static uint32_t sDataSize;
static int sLoaded;

int rsp_lle_load(const uint8_t *rom, uint32_t romSize) {
    if (romSize < 0xE16EDC + 0xF70) {
        return 0;
    }
    memset(sImem, 0, sizeof(sImem));
    memcpy(sImem, rom + 0xE16EDC, 0xF70);
    memset(sDmemInit, 0, sizeof(sDmemInit));
    memcpy(sDmemInit, rom + 0xDBF730, 0x2E0);
    sDataSize = 0x2E0;
    sLoaded = 1;
    return 1;
}

/* ---------------------------------------------------------------- state */

typedef struct {
    uint32_t r[32];
    int16_t v[32][8]; /* lane 0 = DMEM bytes 0-1 of a quad (big-endian element 0) */
    int64_t acc[8];   /* 48-bit, sign-extended */
    uint16_t vco;     /* bit i = carry lane i, bit 8 + i = not-equal lane i */
    uint8_t dmem[0x1000];
    uint32_t memAddr, dramAddr;
    uint32_t pc, nextPc;
    int halted;
} Rsp;

static Rsp sRsp;

/* task / command context for the DMA conversions */
static const uint32_t *sCmds;
static uint32_t sNumCmds;
static const uint8_t *sSampleLoad; /* per command: 1 = a LOADBUFF of sample bytes */
static uint32_t sCurW0, sCurW1;
static int sInTask; /* 0 until the first command is dispatched */
static uint32_t sLoopW1;
static RspLleTranslate sTranslate;
static uint8_t *sRaw; /* raw mode (rsp_lle_run_raw) */
static uint32_t sRawSize, sRawSwz;

static uint8_t vbyte(const int16_t *v, int b) {
    uint16_t x = (uint16_t)v[(b >> 1) & 7];
    return (b & 1) ? (uint8_t)x : (uint8_t)(x >> 8);
}

static void vsetbyte(int16_t *v, int b, uint8_t val) {
    uint16_t x = (uint16_t)v[(b >> 1) & 7];
    if (b & 1) {
        x = (uint16_t)((x & 0xFF00) | val);
    } else {
        x = (uint16_t)((x & 0x00FF) | (val << 8));
    }
    v[(b >> 1) & 7] = (int16_t)x;
}

#define DM(a) (sRsp.dmem[(a) & 0xFFF])

static uint32_t ld32(uint32_t a) {
    return ((uint32_t)DM(a) << 24) | ((uint32_t)DM(a + 1) << 16) | ((uint32_t)DM(a + 2) << 8) | DM(a + 3);
}

static uint32_t ld32_imem(uint32_t a) {
    a &= 0xFFC;
    return ((uint32_t)sImem[a] << 24) | ((uint32_t)sImem[a + 1] << 16) | ((uint32_t)sImem[a + 2] << 8) | sImem[a + 3];
}

static uint16_t ld16(uint32_t a) {
    return (uint16_t)((DM(a) << 8) | DM(a + 1));
}

/* ---------------------------------------------------------------- DMA */

static void *host_addr(uint32_t a24, uint32_t len) {
    uint32_t cands[4];
    int n = 0, i;
    cands[n++] = sCurW1;
    cands[n++] = sLoopW1;
    cands[n++] = (uint32_t)(uintptr_t)sCmds;
    for (i = 0; i < n; i++) {
        uint32_t full = (cands[i] & 0xFF000000u) | a24;
        void *p;
        if (((cands[i] ^ a24) & 0x00FF0000u) != 0 && ((cands[i] - a24) & 0xFFFFFFu) > 0x20000u &&
            ((a24 - cands[i]) & 0xFFFFFFu) > 0x20000u) {
            continue; /* not near this candidate */
        }
        p = sTranslate(full);
        if (p != NULL) {
            void *q = len > 1 ? sTranslate(full + len - 1) : p;
            if (q != NULL) {
                return p;
            }
        }
    }
    rsp_lle_stats.dma_unmapped++;
    return NULL;
}

static void do_dma(int write, uint32_t lenReg) {
    uint32_t len = ((lenReg & 0xFFF) | 7) + 1;
    uint32_t count = ((lenReg >> 12) & 0xFF) + 1;
    uint32_t skip = (lenReg >> 20) & 0xFFF;
    uint32_t mem = sRsp.memAddr & 0x1FF8;
    uint32_t dram = sRsp.dramAddr & 0xFFFFF8;
    uint32_t c, i;
    int imem = (mem & 0x1000) != 0;

    if (sRsp.dramAddr & 7) {
        rsp_lle_stats.dma_misaligned++;
    }
    if (sRaw != NULL) { /* raw mode: a big-endian RDRAM image (byte a at a ^ sRawSwz) */
        for (c = 0; c < count; c++) {
            uint32_t d = dram + c * (len + skip);
            for (i = 0; i < len; i++) {
                uint32_t a = (d + i) & 0xFFFFFF;
                if (a >= sRawSize) {
                    rsp_lle_stats.dma_unmapped++;
                    continue;
                }
                if (write) {
                    sRaw[a ^ sRawSwz] = DM(mem + i);
                } else {
                    DM(mem + i) = sRaw[a ^ sRawSwz];
                }
            }
            mem += len;
            rsp_lle_stats.dma_bytes += len;
        }
        return;
    }
    for (c = 0; c < count; c++) {
        uint32_t d24 = (dram + c * (len + skip)) & 0xFFFFFF;
        uint8_t *host;
        int mode; /* 0 = s16 swap, 1 = raw bytes, 2 = u32 swap, 3 = ucode data from the ROM */
        if (!write && imem) {
            rsp_lle_stats.bad_insn++;
            sRsp.halted = 1;
            return;
        }
        /* the command list */
        if (!write && sCmds != NULL && d24 >= (((uint32_t)(uintptr_t)sCmds) & 0xFFFFFF) &&
            d24 < ((((uint32_t)(uintptr_t)sCmds) + sNumCmds * 8) & 0xFFFFFF)) {
            host = (uint8_t *)sCmds + (d24 - (((uint32_t)(uintptr_t)sCmds) & 0xFFFFFF));
            mode = 2;
        } else if (!write && mem == 0 && !sInTask) {
            host = NULL;
            mode = 3;
        } else {
            host = (uint8_t *)host_addr(d24 & ~7u, len);
            mode = (!write && sSampleLoad != NULL && (sCurW0 >> 24) == 20 && sSampleLoad[0]) ? 1 : 0;
        }
        if (mode == 3) {
            for (i = 0; i < len; i++) {
                DM(mem + i) = i < sDataSize ? sDmemInit[i] : 0;
            }
            mem += len;
            continue;
        }
        if (host == NULL) {
            if (!write) {
                for (i = 0; i < len; i++) {
                    DM(mem + i) = 0;
                }
            }
            mem += len;
            continue;
        }
        rsp_lle_stats.dma_bytes += len;
        if (write && rsp_lle_write_log != NULL) {
            rsp_lle_write_log(host, len, sCurW0 >> 24);
        }
        for (i = 0; i < len; i++) {
            uint32_t hi; /* host index */
            switch (mode) {
                case 0: hi = i ^ 1; break;
                case 2: hi = i ^ 3; break;
                default: hi = i; break;
            }
            if (write) {
                host[hi] = DM(mem + i);
            } else {
                DM(mem + i) = host[hi];
            }
        }
        mem += len;
    }
}

/* ---------------------------------------------------------------- vector unit */

static int16_t sat_mid(int64_t a) {
    int64_t h = a >> 16;
    if (h > 32767) {
        return 0x7FFF;
    }
    if (h < -32768) {
        return (int16_t)0x8000;
    }
    return (int16_t)h;
}

static int16_t sat_low(int64_t a) {
    int64_t h = a >> 16;
    if (h > 32767) {
        return (int16_t)0xFFFF;
    }
    if (h < -32768) {
        return 0;
    }
    return (int16_t)(uint16_t)(a & 0xFFFF);
}

static int64_t wrap48(int64_t a) {
    return (int64_t)((uint64_t)a << 16) >> 16;
}

static int sel(int e, int i) {
    if (e <= 1) {
        return i;
    }
    if (e <= 3) {
        return (i & ~1) | (e & 1);
    }
    if (e <= 7) {
        return (i & ~3) | (e & 3);
    }
    return e & 7;
}

static int16_t clamp16i(int32_t x) {
    return (int16_t)(x > 32767 ? 32767 : x < -32768 ? -32768 : x);
}

static void set_acc_low(int i, uint16_t lo) {
    sRsp.acc[i] = wrap48((sRsp.acc[i] & ~(int64_t)0xFFFF) | lo);
}

static int vector_op(uint32_t w) {
    int funct = w & 63;
    int vd = (w >> 6) & 31, vs = (w >> 11) & 31, vt = (w >> 16) & 31, e = (w >> 21) & 15;
    int16_t s[8], t[8], r[8];
    int i;
    for (i = 0; i < 8; i++) {
        s[i] = sRsp.v[vs][i];
        t[i] = sRsp.v[vt][sel(e, i)];
    }
    for (i = 0; i < 8; i++) {
        int64_t *a = &sRsp.acc[i];
        int64_t ss = s[i], tt = t[i];
        int64_t us = (uint16_t)s[i], ut = (uint16_t)t[i];
        switch (funct) {
            case 0x00: /* VMULF */
                *a = wrap48(ss * tt * 2 + 0x8000);
                r[i] = sat_mid(*a);
                break;
            case 0x04: /* VMUDL */
                *a = wrap48((us * ut) >> 16);
                r[i] = sat_low(*a);
                break;
            case 0x05: /* VMUDM: signed vs x unsigned vt */
                *a = wrap48(ss * ut);
                r[i] = sat_mid(*a);
                break;
            case 0x06: /* VMUDN: unsigned vs x signed vt */
                *a = wrap48(us * tt);
                r[i] = sat_low(*a);
                break;
            case 0x07: /* VMUDH */
                *a = wrap48((ss * tt) << 16);
                r[i] = sat_mid(*a);
                break;
            case 0x08: /* VMACF */
                *a = wrap48(*a + ss * tt * 2);
                r[i] = sat_mid(*a);
                break;
            case 0x0D: /* VMADM */
                *a = wrap48(*a + ss * ut);
                r[i] = sat_mid(*a);
                break;
            case 0x0E: /* VMADN */
                *a = wrap48(*a + us * tt);
                r[i] = sat_low(*a);
                break;
            case 0x0F: /* VMADH */
                *a = wrap48(*a + ((ss * tt) << 16));
                r[i] = sat_mid(*a);
                break;
            case 0x10: { /* VADD */
                int32_t x = (int32_t)ss + (int32_t)tt + ((sRsp.vco >> i) & 1);
                set_acc_low(i, (uint16_t)x);
                r[i] = clamp16i(x);
                break;
            }
            case 0x11: { /* VSUB */
                int32_t x = (int32_t)ss - (int32_t)tt - ((sRsp.vco >> i) & 1);
                set_acc_low(i, (uint16_t)x);
                r[i] = clamp16i(x);
                break;
            }
            case 0x14: { /* VADDC */
                uint32_t x = (uint32_t)us + (uint32_t)ut;
                set_acc_low(i, (uint16_t)x);
                r[i] = (int16_t)(uint16_t)x;
                break;
            }
            case 0x1D: /* VSAR */
                switch (e & 7) {
                    case 0: r[i] = (int16_t)(*a >> 32); break;
                    case 1: r[i] = (int16_t)(*a >> 16); break;
                    case 2: r[i] = (int16_t)*a; break;
                    default: r[i] = 0; break;
                }
                break;
            case 0x28: /* VAND */
                r[i] = (int16_t)(s[i] & t[i]);
                set_acc_low(i, (uint16_t)r[i]);
                break;
            case 0x2C: /* VXOR */
                r[i] = (int16_t)(s[i] ^ t[i]);
                set_acc_low(i, (uint16_t)r[i]);
                break;
            default:
                return 0;
        }
    }
    if (funct == 0x10 || funct == 0x11) {
        sRsp.vco = 0;
    } else if (funct == 0x14) {
        uint16_t c = 0;
        for (i = 0; i < 8; i++) {
            if (((uint32_t)(uint16_t)s[i] + (uint16_t)t[i]) >> 16) {
                c |= (uint16_t)(1u << i);
            }
        }
        sRsp.vco = c;
    }
    memcpy(sRsp.v[vd], r, sizeof(r));
    return 1;
}

static int lwc2(uint32_t w, int store) {
    int base = (w >> 21) & 31, vt = (w >> 16) & 31, op = (w >> 11) & 31, e = (w >> 7) & 15;
    int32_t off = (int32_t)((w & 0x7F) << 25) >> 25;
    int16_t *v = sRsp.v[vt];
    uint32_t a;
    int i, size;
    switch (op) {
        case 1: size = 2; break;
        case 2: size = 4; break;
        case 3: size = 8; break;
        case 4: case 5: size = 16; break;
        case 6: size = 8; break;
        default: return 0;
    }
    a = sRsp.r[base] + (uint32_t)(off * size);
    if (!store) {
        switch (op) {
            case 1: case 2: case 3:
                for (i = 0; i < size && e + i < 16; i++) {
                    vsetbyte(v, e + i, DM(a + i));
                }
                return 1;
            case 4: {
                int n = 16 - (int)(a & 15);
                for (i = 0; i < n && e + i < 16; i++) {
                    vsetbyte(v, e + i, DM(a + i));
                }
                return 1;
            }
            case 5: {
                int n = (int)(a & 15);
                uint32_t st = a & ~15u;
                for (i = 0; i < n; i++) {
                    int idx = e + (16 - n) + i;
                    if (idx < 16) {
                        vsetbyte(v, idx, DM(st + i));
                    }
                }
                return 1;
            }
            case 6: { /* LPV */
                int index = (int)(a & 7) - e;
                uint32_t st = a & ~7u;
                for (i = 0; i < 8; i++) {
                    v[i] = (int16_t)(uint16_t)(DM(st + ((index + i) & 15)) << 8);
                }
                return 1;
            }
        }
        return 0;
    }
    switch (op) {
        case 1: case 2: case 3:
            for (i = 0; i < size; i++) {
                DM(a + i) = vbyte(v, (e + i) & 15);
            }
            return 1;
        case 4: {
            int n = 16 - (int)(a & 15);
            for (i = 0; i < n; i++) {
                DM(a + i) = vbyte(v, (e + i) & 15);
            }
            return 1;
        }
    }
    return 0;
}

/* ---------------------------------------------------------------- scalar */

const uint8_t *rsp_lle_sample_flags;

static void cmd_context(void) {
    /* IMEM 0x1054 / 0x1058 load the command (k0, t9); at 0x105C both are in place */
    uint32_t idx;
    sInTask = 1;
    sCurW0 = sRsp.r[26];
    sCurW1 = sRsp.r[25];
    rsp_lle_stats.cmds++;
    if ((sCurW0 >> 24) == 15) {
        sLoopW1 = sCurW1;
    }
    /* which command of the list: the ucode reads them in order */
    idx = rsp_lle_stats.task_cmd++;
    sSampleLoad = (idx < sNumCmds && rsp_lle_sample_flags != NULL) ? &rsp_lle_sample_flags[idx] : NULL;
}

static void branch(int cond, int32_t off) {
    if (cond) {
        sRsp.nextPc = (sRsp.pc + (uint32_t)(off << 2)) & 0xFFC; /* sRsp.pc = the delay slot */
    }
}

static int step(void) {
    uint32_t pc = sRsp.pc;
    uint32_t w = ld32_imem(pc);
    uint32_t op = w >> 26;
    int rs = (w >> 21) & 31, rt = (w >> 16) & 31, rd = (w >> 11) & 31, sa = (w >> 6) & 31;
    int32_t imm = (int16_t)w;
    uint32_t uimm = w & 0xFFFF;
    uint32_t *R = sRsp.r;
    uint32_t np = sRsp.nextPc;

    if (pc == 0x05C) {
        cmd_context();
    }
    sRsp.pc = np;
    sRsp.nextPc = (np + 4) & 0xFFC;

    switch (op) {
        case 0x00:
            switch (w & 63) {
                case 0x00: R[rd] = R[rt] << sa; break;
                case 0x02: R[rd] = R[rt] >> sa; break;
                case 0x06: R[rd] = R[rt] >> (R[rs] & 31); break;
                case 0x08: sRsp.nextPc = R[rs] & 0xFFC; break;
                case 0x0D: sRsp.halted = 1; break;
                case 0x20: case 0x21: R[rd] = R[rs] + R[rt]; break;
                case 0x22: case 0x23: R[rd] = R[rs] - R[rt]; break;
                case 0x24: R[rd] = R[rs] & R[rt]; break;
                case 0x25: R[rd] = R[rs] | R[rt]; break;
                case 0x26: R[rd] = R[rs] ^ R[rt]; break;
                case 0x2A: R[rd] = (int32_t)R[rs] < (int32_t)R[rt]; break;
                default: return 0;
            }
            break;
        case 0x02: sRsp.nextPc = (w << 2) & 0xFFC; break;
        case 0x03: R[31] = 0x1000 | ((pc + 8) & 0xFFC); sRsp.nextPc = (w << 2) & 0xFFC; break;
        case 0x04: branch(R[rs] == R[rt], imm); break;
        case 0x05: branch(R[rs] != R[rt], imm); break;
        case 0x06: branch((int32_t)R[rs] <= 0, imm); break;
        case 0x07: branch((int32_t)R[rs] > 0, imm); break;
        case 0x08: case 0x09: R[rt] = R[rs] + (uint32_t)imm; break;
        case 0x0A: R[rt] = (int32_t)R[rs] < imm; break;
        case 0x0C: R[rt] = R[rs] & uimm; break;
        case 0x0D: R[rt] = R[rs] | uimm; break;
        case 0x0E: R[rt] = R[rs] ^ uimm; break;
        case 0x0F: R[rt] = uimm << 16; break;
        case 0x10: /* COP0 */
            if (rs == 0) { /* mfc0 */
                R[rt] = 0; /* status, DMA full / busy, semaphore, DPC status */
            } else if (rs == 4) { /* mtc0 */
                switch (rd) {
                    case 0: sRsp.memAddr = R[rt]; break;
                    case 1: sRsp.dramAddr = R[rt]; break;
                    case 2: do_dma(0, R[rt]); break;
                    case 3: do_dma(1, R[rt]); break;
                    default: break;
                }
            } else {
                return 0;
            }
            break;
        case 0x12: /* COP2 */
            if (w & (1u << 25)) {
                if (!vector_op(w)) {
                    return 0;
                }
            } else if (rs == 4) { /* mtc2 rt, vd[e] */
                int e = (w >> 7) & 15;
                vsetbyte(sRsp.v[rd], e, (uint8_t)(R[rt] >> 8));
                if (e + 1 < 16) {
                    vsetbyte(sRsp.v[rd], e + 1, (uint8_t)R[rt]);
                }
            } else {
                return 0;
            }
            break;
        case 0x20: R[rt] = (uint32_t)(int32_t)(int8_t)DM(R[rs] + imm); break;
        case 0x21: R[rt] = (uint32_t)(int32_t)(int16_t)ld16(R[rs] + imm); break;
        case 0x23: R[rt] = ld32(R[rs] + imm); break;
        case 0x24: R[rt] = DM(R[rs] + imm); break;
        case 0x25: R[rt] = ld16(R[rs] + imm); break;
        case 0x28: DM(R[rs] + imm) = (uint8_t)R[rt]; break;
        case 0x29: DM(R[rs] + imm) = (uint8_t)(R[rt] >> 8); DM(R[rs] + imm + 1) = (uint8_t)R[rt]; break;
        case 0x2B:
            DM(R[rs] + imm) = (uint8_t)(R[rt] >> 24);
            DM(R[rs] + imm + 1) = (uint8_t)(R[rt] >> 16);
            DM(R[rs] + imm + 2) = (uint8_t)(R[rt] >> 8);
            DM(R[rs] + imm + 3) = (uint8_t)R[rt];
            break;
        case 0x32: if (!lwc2(w, 0)) return 0; break;
        case 0x3A: if (!lwc2(w, 1)) return 0; break;
        default: return 0;
    }
    R[0] = 0;
    return 1;
}

int rsp_lle_run(const uint32_t *cmds, uint32_t ncmds, RspLleTranslate translate) {
    uint32_t n;
    uint8_t *t = &sRsp.dmem[0xFC0];
    uint32_t dp = (uint32_t)(uintptr_t)cmds;
    if (!sLoaded) {
        return 0;
    }
    sCmds = cmds;
    sNumCmds = ncmds;
    sTranslate = translate;
    sCurW0 = sCurW1 = 0;
    sInTask = 0;
    rsp_lle_stats.task_cmd = 0;
    memset(t, 0, 0x40);
    t[0x03] = 2; /* M_AUDTASK */
    t[0x1C] = (uint8_t)(sDataSize >> 24); t[0x1D] = (uint8_t)(sDataSize >> 16);
    t[0x1E] = (uint8_t)(sDataSize >> 8); t[0x1F] = (uint8_t)sDataSize;
    t[0x30] = (uint8_t)(dp >> 24); t[0x31] = (uint8_t)(dp >> 16); t[0x32] = (uint8_t)(dp >> 8); t[0x33] = (uint8_t)dp;
    n = ncmds * 8;
    t[0x34] = (uint8_t)(n >> 24); t[0x35] = (uint8_t)(n >> 16); t[0x36] = (uint8_t)(n >> 8); t[0x37] = (uint8_t)n;
    memset(sRsp.r, 0, sizeof(sRsp.r));
    sRsp.pc = 0;
    sRsp.nextPc = 4;
    sRsp.halted = 0;
    for (n = 0; n < 2000000u && !sRsp.halted; n++) {
        if (!step()) {
            rsp_lle_stats.bad_insn++;
            fprintf(stderr, "rsp_lle: unhandled insn %08x at IMEM %03x\n", ld32_imem(sRsp.pc - 4), (sRsp.pc - 4) & 0xFFF);
            return 0;
        }
    }
    if (!sRsp.halted) {
        fprintf(stderr, "rsp_lle: no break after %u insns: pc %03x, command %lu of %u (w0 %08x w1 %08x), k1 %d gp %08x sp %03x\n",
                n, sRsp.pc, rsp_lle_stats.task_cmd, ncmds, sCurW0, sCurW1, (int)sRsp.r[27], sRsp.r[28], sRsp.r[29]);
    }
    rsp_lle_stats.tasks++;
    rsp_lle_stats.insns += n;
    return sRsp.halted;
}

/* An emulator-style run: rdram is the console's RDRAM (byte a at rdram[a ^ swz]), imem / dmem the
 * RSP memories at the task start (byte a at [a ^ swz] too, the OSTask at DMEM 0xFC0). */
int rsp_lle_run_raw(uint8_t *rdram, uint32_t size, const uint8_t *imem, const uint8_t *dmem, uint32_t swz) {
    uint32_t n, i;
    for (i = 0; i < 0x1000; i++) {
        sImem[i] = imem[i ^ swz];
        sRsp.dmem[i] = dmem[i ^ swz];
    }
    sRaw = rdram;
    sRawSize = size;
    sRawSwz = swz;
    sCmds = NULL;
    sNumCmds = 0;
    sInTask = 0;
    sCurW0 = sCurW1 = 0;
    rsp_lle_stats.task_cmd = 0;
    memset(sRsp.r, 0, sizeof(sRsp.r));
    sRsp.pc = 0;
    sRsp.nextPc = 4;
    sRsp.halted = 0;
    for (n = 0; n < 2000000u && !sRsp.halted; n++) {
        if (!step()) {
            rsp_lle_stats.bad_insn++;
            fprintf(stderr, "rsp_lle: unhandled insn %08x at IMEM %03x\n", ld32_imem(sRsp.pc - 4), (sRsp.pc - 4) & 0xFFF);
            break;
        }
    }
    sRaw = NULL;
    rsp_lle_stats.tasks++;
    rsp_lle_stats.insns += n;
    return sRsp.halted;
}
