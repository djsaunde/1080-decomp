/*
 * port/audio/tools/usf/usfrender: render a USF (the 1080 rip, port/audio/tools/usf/fetch-usf.sh) with
 * lazyusf2, RSP HLE off by default: the game's own CPU code and 1080's aspMain on the cxd4 RSP, i.e. a
 * real-game reference for port/audio (docs/1080_PC_PORT_PLAN.md 15.11, 15.12).  Built by build-lazyusf2.sh.
 *
 *   usfrender [-t SECONDS] [--hle] [--alist FILE] [--ram FILE@TASK] [--cmp] IN.miniusf OUT.wav
 *
 *   -t SECONDS    length (default 30), at the emulator's native rate (32006 Hz for 1080)
 *   --hle         lazyusf2's HLE audio instead of the RSP (not a reference: for comparison only)
 *   --alist FILE  every audio task's command list: per task u32 n, then n u32 words (host order)
 *   --ram F@N     the whole RDRAM (byte a at a ^ 3) after audio task N
 *   --cmp         run every task again on port/audio/tools/rsp_lle.c and compare all of RDRAM with cxd4's
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdint.h>
#include "usf/usf.h"
#include <psflib.h>
#include "rsp_lle.h"

extern void (*g_alist_pre)(void *rdram, size_t size, void *spmem);
extern void (*g_alist_post)(void *rdram, size_t size, void *spmem);

static unsigned char *state;
static void *f_open(void *c, const char *p) { (void)c; return fopen(p, "rb"); }
static size_t f_read(void *p, size_t s, size_t n, void *f) { return fread(p, s, n, (FILE *)f); }
static int f_seek(void *f, int64_t o, int w) { return fseek((FILE *)f, (long)o, w); }
static int f_close(void *f) { return fclose((FILE *)f); }
static long f_tell(void *f) { return ftell((FILE *)f); }
static psf_file_callbacks cb = {"\\/:", NULL, f_open, f_read, f_seek, f_close, f_tell};
static int loader(void *c, const uint8_t *exe, size_t es, const uint8_t *res, size_t rs) {
    (void)c;
    if (exe && es > 0) return -1;
    return usf_upload_section(state, res, rs);
}
static int cmp_, fifo_;
static int info(void *c, const char *n, const char *v) {
    (void)c;
    if (!strcasecmp(n, "_enablecompare") && *v) cmp_ = 1;
    if (!strcasecmp(n, "_enablefifofull") && *v) fifo_ = 1;
    return 0;
}
static void msg(void *u, const char *m) { (void)u; fputs(m, stderr); }
static void put32(unsigned char *p, uint32_t v) { p[0] = v; p[1] = v >> 8; p[2] = v >> 16; p[3] = v >> 24; }

static FILE *alist_f, *ram_f;
static unsigned long ram_task, tasks;
static int do_cmp;
static uint8_t *copy, imem[0x1000], dmem[0x1000];
static unsigned long difftasks, diffs, maxd;

static void pre(void *rdram, size_t size, void *spmem) {
    const uint32_t *sp = (const uint32_t *)spmem, *d32 = (const uint32_t *)rdram;
    if (alist_f != NULL) { /* OSTask at DMEM 0xFC0: data_ptr 0xFF0, data_size 0xFF4 */
        uint32_t ptr = sp[0xFF0 / 4] & 0xFFFFFF, n = sp[0xFF4 / 4] / 4;
        if (ptr + n * 4 <= size) {
            fwrite(&n, 4, 1, alist_f);
            fwrite(&d32[ptr / 4], 4, n, alist_f);
        }
    }
    if (do_cmp) {
        if (copy == NULL) copy = malloc(size);
        memcpy(copy, rdram, size);
        memcpy(dmem, spmem, 0x1000);
        memcpy(imem, (const uint8_t *)spmem + 0x1000, 0x1000);
    }
}
static void post(void *rdram, size_t size, void *spmem) {
    (void)spmem;
    tasks++;
    if (do_cmp) {
        size_t i;
        int d = 0;
        rsp_lle_run_raw(copy, (uint32_t)size, imem, dmem, 3);
        for (i = 0; i < size; i += 2) { /* big-endian s16, byte a at a ^ 3 */
            int16_t a = (int16_t)((copy[i ^ 3] << 8) | copy[(i + 1) ^ 3]);
            int16_t b = (int16_t)((((uint8_t *)rdram)[i ^ 3] << 8) | ((uint8_t *)rdram)[(i + 1) ^ 3]);
            if (a != b) {
                unsigned long dd = (unsigned long)abs(a - b);
                d = 1; diffs++;
                if (dd > maxd) maxd = dd;
            }
        }
        difftasks += d;
    }
    if (ram_f != NULL && tasks == ram_task) {
        fwrite(rdram, 1, size, ram_f);
        fclose(ram_f);
        ram_f = NULL;
    }
}

int main(int argc, char **argv) {
    FILE *f;
    int32_t sr = 0;
    double secs = 30;
    long total = 0, want;
    int16_t buf[2048];
    unsigned char h[44];
    int hle = 0, i;
    const char *in = NULL, *out = NULL;
    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-t") && i + 1 < argc) secs = atof(argv[++i]);
        else if (!strcmp(argv[i], "--hle")) hle = 1;
        else if (!strcmp(argv[i], "--cmp")) do_cmp = 1;
        else if (!strcmp(argv[i], "--alist") && i + 1 < argc) {
            if ((alist_f = fopen(argv[++i], "wb")) == NULL) { perror(argv[i]); return 1; }
        } else if (!strcmp(argv[i], "--ram") && i + 1 < argc) {
            char *at = strrchr(argv[++i], '@');
            if (at == NULL) { fprintf(stderr, "--ram FILE@TASK\n"); return 2; }
            *at = 0;
            ram_task = strtoul(at + 1, NULL, 0);
            if ((ram_f = fopen(argv[i], "wb")) == NULL) { perror(argv[i]); return 1; }
        } else if (in == NULL) in = argv[i];
        else if (out == NULL) out = argv[i];
        else { fprintf(stderr, "usage: usfrender [-t S] [--hle] [--alist F] [--ram F@N] [--cmp] IN OUT.wav\n"); return 2; }
    }
    if (in == NULL || out == NULL) { fprintf(stderr, "usage: usfrender [-t S] [--hle] [--alist F] [--ram F@N] [--cmp] IN OUT.wav\n"); return 2; }
    state = malloc(usf_get_state_size());
    usf_clear(state);
    if (psf_load(in, &cb, 0x21, loader, 0, info, 0, 1, msg, 0) <= 0) { fprintf(stderr, "usfrender: cannot load %s\n", in); return 1; }
    usf_set_compare(state, cmp_);
    usf_set_fifo_full(state, fifo_);
    usf_set_hle_audio(state, hle);
    if (alist_f != NULL || ram_f != NULL || do_cmp) {
        if (hle) { fprintf(stderr, "usfrender: --alist / --ram / --cmp need the RSP (no --hle)\n"); return 2; }
        g_alist_pre = pre;
        g_alist_post = post;
    }
    if ((f = fopen(out, "wb")) == NULL) { perror(out); return 1; }
    memset(h, 0, sizeof h);
    fwrite(h, 1, 44, f);
    usf_render(state, 0, 0, &sr);
    want = (long)(secs * sr);
    while (total < want) {
        const char *e = usf_render(state, buf, 1024, &sr);
        if (e) { fprintf(stderr, "usfrender: %s\n", e); break; }
        fwrite(buf, 4, 1024, f);
        total += 1024;
    }
    memcpy(h, "RIFF", 4); put32(h + 4, 36 + total * 4); memcpy(h + 8, "WAVEfmt ", 8); put32(h + 16, 16);
    put32(h + 20, 1 | (2u << 16)); put32(h + 24, sr); put32(h + 28, sr * 4); put32(h + 32, 4 | (16u << 16));
    memcpy(h + 36, "data", 4); put32(h + 40, total * 4);
    fseek(f, 0, SEEK_SET);
    fwrite(h, 1, 44, f);
    fclose(f);
    if (alist_f != NULL) fclose(alist_f);
    if (ram_f != NULL) { fprintf(stderr, "usfrender: only %lu audio tasks, no RAM dump\n", tasks); fclose(ram_f); }
    fprintf(stderr, "usfrender: %ld frames at %d Hz, %lu audio tasks\n", total, sr, tasks);
    if (do_cmp)
        fprintf(stderr, "usfrender: rsp_lle vs cxd4: %lu / %lu tasks differ, %lu s16 differ (max %lu); lle insns %lu bad %lu unmapped %lu\n",
                difftasks, tasks, diffs, maxd, rsp_lle_stats.insns, rsp_lle_stats.bad_insn, rsp_lle_stats.dma_unmapped);
    usf_shutdown(state);
    return do_cmp && difftasks ? 3 : 0;
}
