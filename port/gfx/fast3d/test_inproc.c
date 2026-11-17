/* port/gfx/fast3d: test of the in-process renderer (port_fast3d.cpp, plan 19.7) on the
 * 64-bit host. For each DLMP given, the dump's memory image is rebuilt at its original
 * addresses (as f3dreplay does), so the lists sit in memory exactly as the game left them,
 * and port_fast3d_task renders the root the way port_gfx_task will: low-mapping
 * registration, per-task list widening (64-bit), ucode, viewport. With PORT_GFX_PNG set it
 * writes frame_NNNNNN.png per task; fast3d/golden.sh compares them with f3dreplay's PNGs.
 *
 *   test_inproc a.dlmp [b.dlmp ...]
 *   test_inproc -s a.dlmp [b.dlmp ...]   one game step: one memory image (every dump's chunks copied
 *                                        first), then the tasks rendered in the order given; with the
 *                                        offscreen task before the one that samples it, this shows
 *                                        render to texture (run 859) in process
 *
 * Each dump gets fresh mappings (the previous one's are unmapped first, and the texture cache
 * over them invalidated with port_fast3d_invalidate, as the game's CPU texture writers will),
 * so one process renders every scene through one interpreter, frame after frame. */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include "../dlump.h"
#include "port_fast3d.h"

#define MAXMAP 4096
static struct { unsigned long a, b; } sMaps[MAXMAP];
static int sNMaps;

static int cmp_range(const void *x, const void *y) {
    unsigned long a = *(const unsigned long *)x, b = *(const unsigned long *)y;
    return a < b ? -1 : a > b;
}

/* map the merged page ranges r[0..n) (pairs, sorted here) */
static int map_ranges(unsigned long (*r)[2], int n) {
    int i;
    qsort(r, (size_t)n, sizeof(r[0]), cmp_range);
    for (i = 0; i < n; i++) {
        if (sNMaps > 0 && r[i][0] <= sMaps[sNMaps - 1].b) {
            if (r[i][1] > sMaps[sNMaps - 1].b) {
                sMaps[sNMaps - 1].b = r[i][1];
            }
        } else {
            sMaps[sNMaps].a = r[i][0];
            sMaps[sNMaps].b = r[i][1];
            sNMaps++;
        }
    }
    for (i = 0; i < sNMaps; i++) {
        void *p = mmap((void *)sMaps[i].a, sMaps[i].b - sMaps[i].a, PROT_READ | PROT_WRITE,
                       MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE, -1, 0);
        if (p != (void *)sMaps[i].a) {
            fprintf(stderr, "test_inproc: cannot map 0x%lx..0x%lx\n", sMaps[i].a, sMaps[i].b);
            sNMaps = i;
            return -1;
        }
    }
    return 0;
}

static int sStep;              /* -s: the dumps are one step */
static unsigned char *sBufs[64];
static int sNBufs;

static int run(const char *path) {
    FILE *f = fopen(path, "rb");
    unsigned char *buf;
    long n;
    const DlumpHeader *h;
    size_t off;
    unsigned int i;
    int r, nr;
    static unsigned long ranges[MAXMAP][2];
    if (f == NULL) {
        fprintf(stderr, "test_inproc: cannot open %s\n", path);
        return -1;
    }
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    fseek(f, 0, SEEK_SET);
    buf = malloc((size_t)n);
    if (fread(buf, 1, (size_t)n, f) != (size_t)n) {
        fclose(f);
        return -1;
    }
    fclose(f);
    h = (const DlumpHeader *)buf;
    if (memcmp(h->magic, DLUMP_MAGIC, 8) != 0 || h->byteOrderMark != DLUMP_BYTE_ORDER_MARK) {
        fprintf(stderr, "test_inproc: %s: not a host-order DLMP\n", path);
        return -1;
    }
    if (sStep && sNBufs < 64) { /* keep it: mapped and copied by run_step */
        sBufs[sNBufs++] = buf;
        return 0;
    }
    for (i = 0; i < (unsigned)sNMaps; i++) { /* the next dump reuses these addresses for other texels */
        port_fast3d_invalidate((void *)sMaps[i].a, sMaps[i].b - sMaps[i].a);
        munmap((void *)sMaps[i].a, sMaps[i].b - sMaps[i].a);
    }
    sNMaps = 0;
    nr = 0;
    for (off = h->headerSize, i = 0; i < h->chunkCount && nr < MAXMAP; i++) {
        const DlumpChunk *c = (const DlumpChunk *)(buf + off);
        if (c->size != 0 && c->kind != DLUMP_ERROR && c->kind != DLUMP_IMAGE && c->kind != DLUMP_END) {
            ranges[nr][0] = c->addr & ~4095ul;
            ranges[nr][1] = ((unsigned long)c->addr + c->size + 4095) & ~4095ul;
            nr++;
        }
        off += sizeof(*c) + ((c->size + 3) & ~3u);
    }
    if (map_ranges(ranges, nr) != 0) {
        return -1;
    }
    for (off = h->headerSize, i = 0; i < h->chunkCount; i++) {
        const DlumpChunk *c = (const DlumpChunk *)(buf + off);
        if (c->size != 0 && c->kind != DLUMP_ERROR && c->kind != DLUMP_IMAGE && c->kind != DLUMP_END) {
            memcpy((void *)(unsigned long)c->addr, c + 1, c->size);
        }
        off += sizeof(*c) + ((c->size + 3) & ~3u);
    }
    r = port_fast3d_task(h->rootAddr);
    free(buf);
    return r;
}

/* -s: map the union of every dump's ranges, copy every dump's chunks, then render each root */
static int run_step(void) {
    static unsigned long ranges[MAXMAP][2];
    int nr = 0, k, bad = 0;
    unsigned int i;
    size_t off;
    for (k = 0; k < sNBufs; k++) {
        const DlumpHeader *h = (const DlumpHeader *)sBufs[k];
        for (off = h->headerSize, i = 0; i < h->chunkCount && nr < MAXMAP; i++) {
            const DlumpChunk *c = (const DlumpChunk *)(sBufs[k] + off);
            if (c->size != 0 && c->kind != DLUMP_ERROR && c->kind != DLUMP_IMAGE && c->kind != DLUMP_END) {
                ranges[nr][0] = c->addr & ~4095ul;
                ranges[nr][1] = ((unsigned long)c->addr + c->size + 4095) & ~4095ul;
                nr++;
            }
            off += sizeof(*c) + ((c->size + 3) & ~3u);
        }
    }
    if (map_ranges(ranges, nr) != 0) {
        return -1;
    }
    for (k = 0; k < sNBufs; k++) {
        const DlumpHeader *h = (const DlumpHeader *)sBufs[k];
        for (off = h->headerSize, i = 0; i < h->chunkCount; i++) {
            const DlumpChunk *c = (const DlumpChunk *)(sBufs[k] + off);
            if (c->size != 0 && c->kind != DLUMP_ERROR && c->kind != DLUMP_IMAGE && c->kind != DLUMP_END) {
                memcpy((void *)(unsigned long)c->addr, c + 1, c->size);
            }
            off += sizeof(*c) + ((c->size + 3) & ~3u);
        }
    }
    for (k = 0; k < sNBufs; k++) {
        bad += port_fast3d_task(((const DlumpHeader *)sBufs[k])->rootAddr) != 0;
    }
    return bad;
}

int main(int argc, char **argv) {
    int i, bad = 0;
    if (argc > 1 && strcmp(argv[1], "-s") == 0) {
        sStep = 1;
        argv++;
        argc--;
    }
    if (argc < 2) {
        fprintf(stderr, "usage: test_inproc a.dlmp [b.dlmp ...]\n");
        return 2;
    }
    if (port_fast3d_init(320, 240) != 0) {
        return 1;
    }
    for (i = 1; i < argc; i++) {
        int r = run(argv[i]);
        if (r != 0) {
            fprintf(stderr, "test_inproc: %s: %d\n", argv[i], r);
            bad++;
        }
    }
    if (sStep && sNBufs > 0) {
        bad += run_step();
    }
    {
        static unsigned char px[320 * 240 * 4];
        unsigned long sum = 0;
        size_t k;
        port_fast3d_read_rgba(px);
        for (k = 0; k < sizeof(px); k++) {
            sum += px[k];
        }
        printf("test_inproc: %d dumps rendered in process, %d failed; last frame checksum %lu\n", argc - 1, bad, sum);
    }
    return bad != 0;
}
