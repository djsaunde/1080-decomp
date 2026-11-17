/* port/gfx: the DLMP writer (format in dlump.h).
 *
 * Interprets the F3DEX 1.x control flow and the commands that reference memory,
 * collects every referenced range, and writes them as chunks at the end. The
 * memory is not modified while the dump is taken, so a range is copied once
 * however often it is referenced; a list is still RE-WALKED on every call,
 * because what a shared sub-list loads depends on the G_SETTIMG in force. */

#include "dlump.h"
#include "../port_libc.h"

#ifdef PORT_LIBC_DECLS_ONLY
void *realloc(void *p, size_t n);
void qsort(void *base, size_t n, size_t sz, int (*cmp)(const void *, const void *));
#endif

/* F3DEX (GBI 1) opcodes that move control or reference memory. */
#define OP_MTX       0x01
#define OP_MOVEMEM   0x03
#define OP_VTX       0x04
#define OP_DL        0x06
#define OP_ENDDL     0xB8
#define OP_MOVEWORD  0xBC
#define OP_LOADTLUT  0xF0
#define OP_LOADBLOCK 0xF3
#define OP_LOADTILE  0xF4
#define OP_SETTIMG   0xFD
#define OP_SETZIMG   0xFE
#define OP_SETCIMG   0xFF
#define MW_SEGMENT   0x06

#define MAX_ERRORS 64

typedef struct Range {
    dlump_u32 kind, addr, size, aux;
} Range;

typedef struct Walk {
    const DlumpOptions *opt;
    Range *ranges;
    size_t nRanges, capRanges;
    /* open-addressing set of range keys (kind, addr, size), for dedup */
    Range *set;
    size_t setCap, setCount;
    /* visited command addresses (DL coverage) */
    dlump_u32 *seen;
    size_t seenCap, seenCount;
    char errText[MAX_ERRORS][120];
    dlump_u32 errAddr[MAX_ERRORS];
    dlump_u32 nErrors;
    dlump_u32 seg[16];
    dlump_u32 timgW0, timgAddr;
    dlump_u32 cmdCount, flags;
    int oom;
    size_t lastPage;
} Walk;

static void *gptr(const Walk *w, dlump_u32 addr) {
    return (void *)(w->opt->arenaBase + (size_t)addr);
}

static int readable(Walk *w, dlump_u32 addr, size_t n) {
    if (addr < 0x1000u || (unsigned long long)addr + n > 0x100000000ull) {
        return 0;
    }
    if (w->opt->readable != NULL) {
        return w->opt->readable(gptr(w, addr), n);
    }
    return 1;
}

static void error(Walk *w, dlump_u32 at, const char *fmt, dlump_u32 a, dlump_u32 b) {
    if (w->nErrors >= MAX_ERRORS) {
        w->nErrors++;
        return;
    }
    snprintf(w->errText[w->nErrors], sizeof(w->errText[0]), fmt, a, b);
    w->errAddr[w->nErrors] = at;
    w->nErrors++;
}

static size_t hash3(dlump_u32 a, dlump_u32 b, dlump_u32 c) {
    unsigned long long h = (unsigned long long)a * 0x9E3779B97F4A7C15ull;
    h ^= (unsigned long long)b * 0xC2B2AE3D27D4EB4Full;
    h ^= (unsigned long long)c * 0x165667B19E3779F9ull;
    return (size_t)(h ^ (h >> 29));
}

/* returns 1 if (kind, addr, size) is new (and inserts it) */
static int set_insert(Walk *w, const Range *r) {
    size_t i;
    if ((w->setCount + 1) * 2 > w->setCap) {
        size_t ncap = w->setCap ? w->setCap * 2 : 1024, j;
        Range *ns = calloc(ncap, sizeof(Range));
        if (ns == NULL) {
            w->oom = 1;
            return 0;
        }
        for (j = 0; j < w->setCap; j++) {
            if (w->set[j].kind != DLUMP_END) {
                i = hash3(w->set[j].kind, w->set[j].addr, w->set[j].size) & (ncap - 1);
                while (ns[i].kind != DLUMP_END) {
                    i = (i + 1) & (ncap - 1);
                }
                ns[i] = w->set[j];
            }
        }
        free(w->set);
        w->set = ns;
        w->setCap = ncap;
    }
    i = hash3(r->kind, r->addr, r->size) & (w->setCap - 1);
    while (w->set[i].kind != DLUMP_END) {
        if (w->set[i].kind == r->kind && w->set[i].addr == r->addr && w->set[i].size == r->size) {
            return 0;
        }
        i = (i + 1) & (w->setCap - 1);
    }
    w->set[i] = *r;
    w->setCount++;
    return 1;
}

/* returns 1 if the command address is new (and inserts it) */
static int seen_insert(Walk *w, dlump_u32 addr) {
    size_t i;
    if ((w->seenCount + 1) * 2 > w->seenCap) {
        size_t ncap = w->seenCap ? w->seenCap * 2 : 4096, j;
        dlump_u32 *ns = calloc(ncap, sizeof(dlump_u32));
        if (ns == NULL) {
            w->oom = 1;
            return 0;
        }
        for (j = 0; j < w->seenCap; j++) {
            if (w->seen[j] != 0) {
                i = hash3(w->seen[j], 0, 0) & (ncap - 1);
                while (ns[i] != 0) {
                    i = (i + 1) & (ncap - 1);
                }
                ns[i] = w->seen[j];
            }
        }
        free(w->seen);
        w->seen = ns;
        w->seenCap = ncap;
    }
    i = hash3(addr, 0, 0) & (w->seenCap - 1);
    while (w->seen[i] != 0) {
        if (w->seen[i] == addr) {
            return 0;
        }
        i = (i + 1) & (w->seenCap - 1);
    }
    w->seen[i] = addr;
    w->seenCount++;
    return 1;
}

static void add_range(Walk *w, dlump_u32 kind, dlump_u32 addr, dlump_u32 size, dlump_u32 aux) {
    Range r;
    r.kind = kind;
    r.addr = addr;
    r.size = size;
    r.aux = aux;
    if (!set_insert(w, &r)) {
        return;
    }
    if (w->nRanges == w->capRanges) {
        size_t ncap = w->capRanges ? w->capRanges * 2 : 256;
        Range *nr = realloc(w->ranges, ncap * sizeof(Range));
        if (nr == NULL) {
            w->oom = 1;
            return;
        }
        w->ranges = nr;
        w->capRanges = ncap;
    }
    w->ranges[w->nRanges++] = r;
}

/* A data reference: check and record it. */
static void capture(Walk *w, dlump_u32 at, dlump_u32 kind, dlump_u32 addr, dlump_u32 size, dlump_u32 aux) {
    if (size == 0) {
        return;
    }
    if (size > DLUMP_MAX_RANGE) {
        error(w, at, "range of 0x%x bytes at 0x%08x is over the limit", size, addr);
        return;
    }
    if (!readable(w, addr, size)) {
        error(w, at, "unreadable range 0x%08x + 0x%x", addr, size);
        return;
    }
    add_range(w, kind, addr, size, aux);
}

static dlump_u32 resolve(Walk *w, dlump_u32 a) {
    dlump_u32 base = w->seg[(a >> 24) & 15];
    if (base != 0) {
        w->flags |= DLUMP_FLAG_SEGMENTS;
        return base + (a & 0xFFFFFFu);
    }
    return a;
}

/* bytes of n texels of size siz (0 = 4b .. 3 = 32b), rounded up */
static dlump_u32 texel_bytes(dlump_u32 n, dlump_u32 siz) {
    return siz == 0 ? (n + 1) / 2 : n << (siz - 1);
}

static int read_cmd(Walk *w, dlump_u32 pc, dlump_u32 *w0, dlump_u32 *w1) {
    size_t page = (size_t)pc >> 12;
    if (page != w->lastPage) {
        if (!readable(w, pc, 8)) {
            return 0;
        }
        w->lastPage = page;
    }
    memcpy(w0, gptr(w, pc), 4);
    memcpy(w1, gptr(w, pc + 4), 4);
    return 1;
}

static void walk(Walk *w, dlump_u32 root) {
    dlump_u32 stack[DLUMP_DL_STACK];
    int sp = 0;
    dlump_u32 pc = root;
    dlump_u32 runStart = 0, runEnd = 0, runDepth = 0;

    for (;;) {
        dlump_u32 w0, w1, op;
        if (w->cmdCount >= DLUMP_MAX_COMMANDS) {
            w->flags |= DLUMP_FLAG_TRUNCATED;
            error(w, pc, "command limit (%u) reached", DLUMP_MAX_COMMANDS, 0);
            break;
        }
        if ((pc & 7) != 0 || !read_cmd(w, pc, &w0, &w1)) {
            error(w, pc, "bad display-list address 0x%08x (depth %u)", pc, (dlump_u32)sp);
            break;
        }
        w->cmdCount++;
        if (seen_insert(w, pc)) {
            if (runEnd == pc && runEnd != 0) {
                runEnd += 8;
            } else {
                if (runEnd != 0) {
                    add_range(w, DLUMP_DL, runStart, runEnd - runStart, runDepth);
                }
                runStart = pc;
                runEnd = pc + 8;
                runDepth = (dlump_u32)sp;
            }
        }
        op = w0 >> 24;
        pc += 8;
        switch (op) {
            case OP_DL: {
                dlump_u32 target = resolve(w, w1);
                if (((w0 >> 16) & 0xFF) == 0) { /* push */
                    if (sp >= DLUMP_DL_STACK) {
                        error(w, pc - 8, "G_DL stack overflow (depth %u) calling 0x%08x", (dlump_u32)sp, target);
                        goto done;
                    }
                    stack[sp++] = pc;
                }
                pc = target;
                break;
            }
            case OP_ENDDL:
                if (sp == 0) {
                    goto done;
                }
                pc = stack[--sp];
                break;
            case OP_MOVEWORD:
                if ((w0 & 0xFF) == MW_SEGMENT) {
                    w->seg[((w0 >> 8) & 0xFFFF) / 4 & 15] = w1 & 0xFFFFFFFFu;
                }
                break;
            case OP_MTX:
                capture(w, pc - 8, DLUMP_MTX, resolve(w, w1), (w0 & 0xFFFF) ? (w0 & 0xFFFF) : 0x40,
                        (w0 >> 16) & 0xFF);
                break;
            case OP_MOVEMEM:
                capture(w, pc - 8, DLUMP_MOVEMEM, resolve(w, w1), w0 & 0xFFFF, (w0 >> 16) & 0xFF);
                break;
            case OP_VTX: /* F3DEX 1: 04 | v0*2 << 16 | n << 10 | n*16 - 1 */
                capture(w, pc - 8, DLUMP_VTX, resolve(w, w1), (w0 & 0x3FF) + 1, (w0 >> 10) & 0x3F);
                break;
            case OP_SETTIMG:
                w->timgW0 = w0;
                w->timgAddr = resolve(w, w1);
                break;
            case OP_LOADBLOCK: {
                dlump_u32 siz = (w->timgW0 >> 19) & 3, width = (w->timgW0 & 0xFFF) + 1;
                dlump_u32 uls = (w0 >> 12) & 0xFFF, ult = w0 & 0xFFF, lrs = (w1 >> 12) & 0xFFF;
                dlump_u32 start = w->timgAddr + texel_bytes(ult * width + uls, siz);
                capture(w, pc - 8, DLUMP_TEXTURE, start, lrs >= uls ? texel_bytes(lrs - uls + 1, siz) : 0,
                        w->timgW0);
                break;
            }
            case OP_LOADTILE: {
                dlump_u32 siz = (w->timgW0 >> 19) & 3, width = (w->timgW0 & 0xFFF) + 1;
                dlump_u32 uls = ((w0 >> 12) & 0xFFF) >> 2, ult = (w0 & 0xFFF) >> 2;
                dlump_u32 lrs = ((w1 >> 12) & 0xFFF) >> 2, lrt = (w1 & 0xFFF) >> 2;
                if (lrs >= uls && lrt >= ult) {
                    dlump_u32 s = texel_bytes(ult * width + uls, siz) & ~0u;
                    dlump_u32 e = texel_bytes(lrt * width + lrs + 1, siz);
                    capture(w, pc - 8, DLUMP_TEXTURE, w->timgAddr + s, e - s, w->timgW0);
                }
                break;
            }
            case OP_LOADTLUT: { /* w1 = tile << 24 | (count - 1) << 14; w0 uls in the same field */
                dlump_u32 uls = (w0 >> 14) & 0x3FF, lrs = (w1 >> 14) & 0x3FF;
                if (lrs >= uls) {
                    capture(w, pc - 8, DLUMP_TLUT, w->timgAddr + uls * 2, (lrs - uls + 1) * 2, w->timgW0);
                }
                break;
            }
            case OP_SETCIMG:
            case OP_SETZIMG:
                add_range(w, DLUMP_IMAGE, resolve(w, w1), 0, w0);
                break;
            default:
                break;
        }
    }
done:
    if (runEnd != 0) {
        add_range(w, DLUMP_DL, runStart, runEnd - runStart, runDepth);
    }
}

static void put(DlumpBuf *b, const void *p, size_t n) {
    if (b->oom) {
        return;
    }
    if (b->size + n > b->cap) {
        size_t ncap = b->cap ? b->cap : 65536;
        dlump_u8 *nd;
        while (ncap < b->size + n) {
            ncap *= 2;
        }
        nd = realloc(b->data, ncap);
        if (nd == NULL) {
            b->oom = 1;
            return;
        }
        b->data = nd;
        b->cap = ncap;
    }
    if (p != NULL) {
        memcpy(b->data + b->size, p, n);
    } else {
        memset(b->data + b->size, 0, n);
    }
    b->size += n;
}

static void put_chunk(DlumpBuf *b, dlump_u32 kind, dlump_u32 addr, dlump_u32 size, dlump_u32 aux, const void *data) {
    DlumpChunk c;
    c.kind = kind;
    c.addr = addr;
    c.size = size;
    c.aux = aux;
    put(b, &c, sizeof(c));
    put(b, data, size);
    put(b, NULL, (4 - (size & 3)) & 3);
}

static int cmp_range(const void *a, const void *b) {
    const Range *x = a, *y = b;
    if (x->kind != y->kind) {
        return x->kind < y->kind ? -1 : 1;
    }
    if (x->addr != y->addr) {
        return x->addr < y->addr ? -1 : 1;
    }
    return 0;
}

int dlump_task(DlumpBuf *out, const DlumpOptions *opt, dlump_u32 seq, dlump_u32 taskAddr,
               dlump_u32 taskType, dlump_u32 rootAddr, dlump_u32 rootSize) {
    static const DlumpOptions defOpt = { 0, NULL, NULL, 0 };
    Walk w;
    DlumpHeader h;
    size_t hdrAt, i, nOut = 0;
    dlump_u32 dataBytes = 0;

    memset(&w, 0, sizeof(w));
    w.opt = opt != NULL ? opt : &defOpt;
    w.lastPage = (size_t)-1;
    walk(&w, rootAddr);

    /* coalesce adjacent / overlapping DL runs (sorted by kind, then address) */
    qsort(w.ranges, w.nRanges, sizeof(Range), cmp_range);
    for (i = 0; i < w.nRanges; i++) {
        Range r = w.ranges[i];
        if (nOut > 0 && r.kind == DLUMP_DL && w.ranges[nOut - 1].kind == DLUMP_DL &&
            r.addr <= w.ranges[nOut - 1].addr + w.ranges[nOut - 1].size) {
            Range *p = &w.ranges[nOut - 1];
            dlump_u32 end = r.addr + r.size;
            if (end > p->addr + p->size) {
                p->size = end - p->addr;
            }
            if (r.aux < p->aux) {
                p->aux = r.aux;
            }
            continue;
        }
        w.ranges[nOut++] = r;
    }

    memset(&h, 0, sizeof(h));
    memcpy(h.magic, DLUMP_MAGIC, 8);
    h.version = DLUMP_VERSION;
    h.headerSize = sizeof(DlumpHeader);
    h.byteOrderMark = DLUMP_BYTE_ORDER_MARK;
    h.ptrSize = sizeof(void *);
    h.seq = seq;
    h.rootAddr = rootAddr;
    h.rootSize = rootSize;
    hdrAt = out->size;
    put(out, &h, sizeof(h));

    h.chunkCount = 0;
    if (taskAddr != 0 && readable(&w, taskAddr, 0x40)) {
        put_chunk(out, DLUMP_TASK, taskAddr, 0x40, taskType, gptr(&w, taskAddr));
        h.chunkCount++;
        dataBytes += 0x40;
    }
    for (i = 0; i < nOut; i++) {
        const Range *r = &w.ranges[i];
        put_chunk(out, r->kind, r->addr, r->size, r->aux, r->size ? gptr(&w, r->addr) : NULL);
        h.chunkCount++;
        dataBytes += r->size;
    }
    for (i = 0; i < w.opt->nExtra; i++) {
        const DlumpExtra *e = &w.opt->extra[i];
        put_chunk(out, e->kind, e->addr, e->size, e->aux, e->size ? e->data : NULL);
        h.chunkCount++;
        dataBytes += e->size;
    }
    for (i = 0; i < w.nErrors && i < MAX_ERRORS; i++) {
        put_chunk(out, DLUMP_ERROR, w.errAddr[i], (dlump_u32)strlen(w.errText[i]) + 1, 0, w.errText[i]);
        h.chunkCount++;
    }
    put_chunk(out, DLUMP_END, 0, 0, 0, NULL);

    h.cmdCount = w.cmdCount;
    h.errorCount = w.nErrors;
    h.flags = w.flags;
    h.dataBytes = dataBytes;
    if (!out->oom) {
        memcpy(out->data + hdrAt, &h, sizeof(h));
    }
    free(w.ranges);
    free(w.set);
    free(w.seen);
    if (out->oom || w.oom) {
        return -1;
    }
    return (int)w.nErrors;
}

void dlump_buf_free(DlumpBuf *b) {
    free(b->data);
    b->data = NULL;
    b->size = b->cap = 0;
    b->oom = 0;
}
