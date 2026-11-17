/* port/gfx: the synthetic frame (synth.h). The command words follow the forms
 * 1080's emitters store (plan 11.1): F3DEX 1 G_VTX / G_TRI1, G_MTX LOAD with
 * 0x40 length, G_MOVEMEM VIEWPORT 0x10, the 15-command RDP init shape, a fill
 * clear of the colour and Z buffers, a LOADBLOCK texture through a shared
 * sub-list called twice with different G_SETTIMG, a CI8 TLUT, and a G_DL
 * branch (PolyShape_clipRows' NOPUSH form) into a tail list. */

#include "synth.h"

#include <string.h>

#define ARENA_SIZE (256 * 1024)
static unsigned int sArenaAddr; /* game address of arena[0] */

static unsigned char sArena[ARENA_SIZE] __attribute__((aligned(64)));
static size_t sTop;
static SynthFrame sFrame;

static void *alloc(size_t n) {
    void *p;
    sTop = (sTop + 63) & ~(size_t)63;
    p = &sArena[sTop];
    sTop += n;
    return p;
}

static unsigned int ga(const void *p) {
    return (unsigned int)((const unsigned char *)p - sArena) + sArenaAddr;
}

typedef struct { unsigned int w0, w1; } G;

static G *sDl;
static size_t sN;
static void cmd(unsigned int w0, unsigned int w1) {
    sDl[sN].w0 = w0;
    sDl[sN].w1 = w1;
    sN++;
}

/* N64 fixed-point Mtx from a float row-major matrix (guMtxF2L). */
static void mtx_f2l(unsigned int *m, const float f[4][4]) {
    int i;
    for (i = 0; i < 8; i++) {
        int a = (int)(f[(2 * i) / 4][(2 * i) % 4] * 65536.0f);
        int b = (int)(f[(2 * i + 1) / 4][(2 * i + 1) % 4] * 65536.0f);
        m[i] = ((unsigned int)a & 0xFFFF0000u) | (((unsigned int)b >> 16) & 0xFFFFu);
        m[i + 8] = ((unsigned int)a << 16) | ((unsigned int)b & 0xFFFFu);
    }
}

typedef struct {
    short ob[3];
    unsigned short flag;
    short tc[2];
    unsigned char cn[4];
} SVtx;

static void vtx(SVtx *v, int x, int y, int z, int r, int g, int b) {
    memset(v, 0, sizeof(*v));
    v->ob[0] = (short)x;
    v->ob[1] = (short)y;
    v->ob[2] = (short)z;
    v->cn[0] = (unsigned char)r;
    v->cn[1] = (unsigned char)g;
    v->cn[2] = (unsigned char)b;
    v->cn[3] = 255;
}

const SynthFrame *synth_build(int realPointers) {
    SynthFrame *fr = &sFrame;
    unsigned int *task, *proj, *mv;
    short *vp;
    SVtx *v;
    unsigned short *tex0, *tex1, *tlut, *cimg, *zimg;
    G *shared, *tail, *clear;
    float p[4][4], id[4][4];
    int i;

    memset(sArena, 0, sizeof(sArena));
    sTop = 0;
    memset(fr, 0, sizeof(*fr));
    /* realPointers (32-bit hosts only): game address = host pointer, arenaBase 0 */
    sArenaAddr = (realPointers && sizeof(void *) == 4) ? (unsigned int)(size_t)sArena : 0x00100000u;
    fr->arenaBase = (size_t)sArena - sArenaAddr;

    cimg = alloc(320 * 240 * 2);
    zimg = alloc(64); /* only referenced, never read by the dumper */
    tex0 = alloc(16 * 16 * 2);
    tex1 = alloc(16 * 16 * 2);
    tlut = alloc(256 * 2);
    for (i = 0; i < 256; i++) {
        tex0[i] = (unsigned short)(0xF801 + i);
        tex1[i] = (unsigned short)(0x07C1 + i);
        tlut[i] = (unsigned short)i;
    }

    memset(p, 0, sizeof(p));
    memset(id, 0, sizeof(id));
    for (i = 0; i < 4; i++) {
        id[i][i] = 1.0f;
    }
    p[0][0] = 1.0f / 160.0f;
    p[1][1] = 1.0f / 120.0f;
    p[2][2] = -1.0f / 1000.0f;
    p[3][3] = 1.0f;
    proj = alloc(64);
    mv = alloc(64);
    mtx_f2l(proj, p);
    mtx_f2l(mv, id);

    vp = alloc(16); /* quarter pixels: scale 160x120, centre (160, 120) */
    vp[0] = 640; vp[1] = 480; vp[2] = 511; vp[3] = 0;
    vp[4] = 640; vp[5] = 480; vp[6] = 511; vp[7] = 0;

    v = alloc(4 * sizeof(SVtx));
    vtx(&v[0], -120, -90, 0, 255, 0, 0);
    vtx(&v[1], 120, -90, 0, 0, 255, 0);
    vtx(&v[2], 0, 100, 0, 0, 0, 255);
    vtx(&v[3], 120, 100, 0, 255, 255, 255);

    /* shared texture-load sub-list: depends on the caller's G_SETTIMG */
    sDl = shared = alloc(8 * 8);
    sN = 0;
    cmd(0xF5100000u, 0x07000000u);            /* SETTILE load tile 7, 16b */
    cmd(0xE6000000u, 0);                      /* LOADSYNC */
    cmd(0xF3000000u, 0x070FF100u);            /* LOADBLOCK tile 7, lrs 255, dxt 0x100 */
    cmd(0xE7000000u, 0);                      /* PIPESYNC */
    cmd(0xB8000000u, 0);                      /* ENDDL */

    /* the fill clears (colour, then Z: CIMG pointed at the Z buffer) */
    sDl = clear = alloc(16 * 8);
    sN = 0;
    cmd(0xBA001402u, 0x00300000u);            /* SETOTHERMODE_H CYCLETYPE = FILL */
    cmd(0xF7000000u, 0x18C318C3u);            /* SETFILLCOLOR dark grey 5551 */
    cmd(0xF64FC3BCu, 0x00000000u);            /* FILLRECT (0,0)-(319,239) */
    cmd(0xFF100000u | 319, ga(zimg));         /* SETCIMG = Z buffer */
    cmd(0xF7000000u, 0xFFFCFFFCu);
    cmd(0xF64FC3BCu, 0x00000000u);
    cmd(0xFF100000u | 319, ga(cimg));         /* back to the colour buffer */
    cmd(0xBA001402u, 0x00000000u);            /* CYCLETYPE = 1CYC */
    cmd(0xB8000000u, 0);

    sDl = tail = alloc(8 * 8);
    sN = 0;
    cmd(0xBF000000u, (4u << 16) | (2u << 8) | 6u); /* TRI1 2 1 3 (indices * 2) */
    cmd(0xB8000000u, 0);

    sDl = alloc(64 * 8);
    sN = 0;
    fr->rootAddr = ga(sDl);
    cmd(0xBC000006u, 0);                      /* MOVEWORD SEGMENT 0 = 0 */
    cmd(0xFF100000u | 319, ga(cimg));         /* SETCIMG RGBA16 320 */
    cmd(0xFE000000u, ga(zimg));               /* SETZIMG */
    cmd(0xE7000000u, 0);
    cmd(0x06000000u, ga(clear));              /* G_DL push */
    cmd(0x03800010u, ga(vp));                 /* MOVEMEM VIEWPORT */
    cmd(0x01030040u, ga(proj));               /* MTX projection LOAD */
    cmd(0x01020040u, ga(mv));                 /* MTX modelview LOAD */
    cmd(0xB6000000u, 0xFFFFFFFFu);            /* CLEARGEOMETRYMODE all */
    cmd(0xB7000000u, 0x00000205u);            /* SETGEOMETRYMODE ZBUFFER | SHADE | SHADING_SMOOTH */
    cmd(0xFC121824u, 0xFF33FFFFu);            /* SETCOMBINE shade (1-cycle) */
    cmd(0xB900031Du, 0x00442478u);            /* RENDERMODE AA_ZB_OPA_INTER */
    cmd(0xFD100000u | 15, ga(tex0));          /* SETTIMG RGBA16 w16 */
    cmd(0x06000000u, ga(shared));
    cmd(0xFD100000u | 15, ga(tex1));
    cmd(0x06000000u, ga(shared));             /* same sub-list, other texture */
    cmd(0xFD100000u, ga(tlut));               /* SETTIMG for the TLUT */
    cmd(0xF0000000u, 0x073FC000u);            /* LOADTLUT tile 7, 256 entries */
    cmd(0x04000000u | (4u << 10) | (4u * 16 - 1), ga(v)); /* VTX v0 = 0, n = 4 */
    cmd(0xBF000000u, (0u << 16) | (2u << 8) | 4u); /* TRI1 0 1 2 */
    cmd(0x06010000u, ga(tail));               /* G_DL branch (no push) */
    cmd(0xB8000000u, 0);                      /* never reached */
    fr->rootSize = (unsigned int)(sN * 8);

    task = alloc(64);
    task[0] = 1; /* M_GFXTASK */
    task[12] = fr->rootAddr;
    task[13] = fr->rootSize;
    fr->taskAddr = ga(task);

    fr->texAddr[0] = ga(tex0);
    fr->texAddr[1] = ga(tex1);
    fr->texSize[0] = fr->texSize[1] = 512;
    fr->tlutAddr = ga(tlut);
    fr->tlutSize = 512;
    fr->vtxAddr = ga(v);
    fr->vtxSize = 64;
    fr->sharedDl = ga(shared);
    fr->branchDl = ga(tail);
    fr->vpAddr = ga(vp);
    fr->cimgAddr = ga(cimg);
    fr->zimgAddr = ga(zimg);
    return fr;
}
