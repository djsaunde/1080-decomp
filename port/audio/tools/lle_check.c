/*
 * port/audio/tools/lle_check.c: seqrender's SEQRENDER_LLE mode (docs/1080_PC_PORT_PLAN.md 15.11).
 *
 *   SEQRENDER_LLE=1    every audio task runs on 1080's own aspMain under rsp_lle.c instead of the
 *                      CPU mixer: the WAV is the reference render.
 *   SEQRENDER_LLE=cmp  as 1, and per task the CPU mixer runs the same list on the same inputs: the
 *                      LLE's DRAM writes are logged and undone, the mixer runs, its result at each
 *                      written region is compared with the LLE's (s16), and the LLE's result is put
 *                      back (so the render stays the reference and differences never accumulate).
 *                      The report counts differing s16 per opcode that wrote them.
 *   LLE_ALIST_DUMP=f   (with SEQRENDER_LLE) every task's command list: per task a u32 word count and the
 *                      words, the record a lazyusf2 / mupen64plus dump of the real game writes too.
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rsp_lle.h"
#include "../mixer.h"
#include "../port_audio.h"

typedef struct {
    uint8_t *host;
    uint32_t len;
    uint32_t op;     /* the command that wrote it */
    uint32_t oldOff; /* into sArena: the bytes before, then the LLE's final bytes */
} WriteRec;

#define MAX_REC 8192
static WriteRec sRec[MAX_REC];
static uint32_t sNumRec;
static uint8_t *sArena;
static uint32_t sArenaUsed, sArenaSize;
static int sCmp;
static Mixer sShadow;
static uint8_t sFlags[4096];

static unsigned long sDiff[32], sTotal[32], sDiffMax[32], sDiffTasks, sTasks, sOverflow;
static unsigned long sDiffWords16[32][4]; /* |d| = 1, 2..15, 16..255, >= 256 */

static void *xlate(uint32_t a) {
    return port_audio_translate(a);
}

static void *mixer_xlate(uint32_t a, void *u) {
    (void)u;
    return port_audio_translate(a);
}

static void lle_write_log(uint8_t *host, uint32_t len, uint32_t op) {
    if (!sCmp) {
        return;
    }
    if (sNumRec >= MAX_REC || sArenaUsed + 2 * len > sArenaSize) {
        sOverflow++;
        return;
    }
    sRec[sNumRec].host = host;
    sRec[sNumRec].len = len;
    sRec[sNumRec].op = op;
    sRec[sNumRec].oldOff = sArenaUsed;
    memcpy(sArena + sArenaUsed, host, len);
    sArenaUsed += 2 * len;
    sNumRec++;
}

/* 1 for each LOADBUFF whose DMEM data is an ADPCM / S8DEC input (sample bytes, not s16) */
static void sample_flags(const uint32_t *w, uint32_t n) {
    uint32_t i, j;
    memset(sFlags, 0, n < sizeof(sFlags) ? n : sizeof(sFlags));
    for (i = 0; i < n && i < sizeof(sFlags); i++) {
        uint32_t d, sz;
        if ((w[2 * i] >> 24) != 20) {
            continue;
        }
        d = w[2 * i] & 0xFFFF;
        sz = (w[2 * i] >> 12) & 0xFF0;
        for (j = i + 1; j < n && j < i + 24; j++) {
            uint32_t op = w[2 * j] >> 24;
            if (op == 20 && (w[2 * j] & 0xFFFF) == d) {
                break; /* reloaded first */
            }
            if (op == 8) {
                uint32_t in = w[2 * j] & 0xFFFF;
                if (in >= d && in < d + sz && j + 1 < n) {
                    uint32_t nx = w[2 * j + 2] >> 24;
                    if (nx == 1 || nx == 23) {
                        sFlags[i] = 1;
                    }
                    break;
                }
            }
        }
    }
}

static int run_hook(const void *cmds, unsigned int bytes) {
    const uint32_t *w = (const uint32_t *)cmds;
    uint32_t n = bytes / 8, i, k;
    int diffTask = 0;

    if (getenv("LLE_ALIST_DUMP") != NULL) {
        static FILE *df;
        if (df == NULL) df = fopen(getenv("LLE_ALIST_DUMP"), "wb");
        {   /* one record per task: u32 word count, then the words (host order) */
            uint32_t nw = bytes / 4;
            fwrite(&nw, 4, 1, df);
            fwrite(w, 4, nw, df);
        }
        fflush(df);
    }
    sample_flags(w, n);
    rsp_lle_sample_flags = sFlags;
    sNumRec = 0;
    sArenaUsed = 0;
    if (!rsp_lle_run(w, n, xlate)) {
        fprintf(stderr, "lle: task %lu did not end cleanly\n", sTasks);
    }
    sTasks++;
    if (!sCmp) {
        return 1;
    }
    /* the LLE's final bytes, then undo in reverse order */
    for (i = 0; i < sNumRec; i++) {
        memcpy(sArena + sRec[i].oldOff + sRec[i].len, sRec[i].host, sRec[i].len);
    }
    for (i = sNumRec; i-- > 0;) {
        memcpy(sRec[i].host, sArena + sRec[i].oldOff, sRec[i].len);
    }
    audio_abi_run(&sShadow, w, (int)n);
    for (i = 0; i < sNumRec; i++) {
        const uint8_t *ref = sArena + sRec[i].oldOff + sRec[i].len;
        uint32_t op = sRec[i].op & 31;
        for (k = 0; k + 1 < sRec[i].len; k += 2) {
            int16_t a, b;
            int d;
            memcpy(&a, ref + k, 2);
            memcpy(&b, sRec[i].host + k, 2);
            sTotal[op]++;
            if (a != b) {
                d = a - b;
                if (d < 0) d = -d;
                sDiff[op]++;
                if ((unsigned long)d > sDiffMax[op]) sDiffMax[op] = d;
                sDiffWords16[op][d == 1 ? 0 : d < 16 ? 1 : d < 256 ? 2 : 3]++;
                diffTask = 1;
            }
        }
    }
    for (i = 0; i < sNumRec; i++) {
        memcpy(sRec[i].host, sArena + sRec[i].oldOff + sRec[i].len, sRec[i].len);
    }
    sDiffTasks += diffTask;
    return 1;
}

static void report(void) {
    int op;
    fprintf(stderr, "lle: %lu tasks, %lu cmds, %lu insns, dma %lu bytes, unmapped %lu, misaligned %lu, bad insn %lu\n",
            rsp_lle_stats.tasks, rsp_lle_stats.cmds, rsp_lle_stats.insns, rsp_lle_stats.dma_bytes,
            rsp_lle_stats.dma_unmapped, rsp_lle_stats.dma_misaligned, rsp_lle_stats.bad_insn);
    if (!sCmp) {
        return;
    }
    fprintf(stderr, "lle cmp: %lu of %lu tasks differ; log overflow %lu\n", sDiffTasks, sTasks, sOverflow);
    for (op = 0; op < 32; op++) {
        if (sTotal[op] != 0) {
            fprintf(stderr, "lle cmp: op %2d wrote %9lu s16, %9lu differ (|d|=1 %lu, 2-15 %lu, 16-255 %lu, >=256 %lu), max |d| %lu\n",
                    op, sTotal[op], sDiff[op], sDiffWords16[op][0], sDiffWords16[op][1], sDiffWords16[op][2],
                    sDiffWords16[op][3], sDiffMax[op]);
        }
    }
}

int lle_check_install(const uint8_t *rom, uint32_t romSize) {
    const char *e = getenv("SEQRENDER_LLE");
    if (e == NULL || !rsp_lle_load(rom, romSize)) {
        return 0;
    }
    sCmp = strcmp(e, "cmp") == 0;
    if (sCmp) {
        sArenaSize = 16u << 20;
        sArena = (uint8_t *)malloc(sArenaSize);
        mixer_init(&sShadow, mixer_xlate, NULL);
        rsp_lle_write_log = lle_write_log;
    }
    port_audio_run_hook = run_hook;
    atexit(report);
    return 1;
}
