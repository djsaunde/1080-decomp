/* port/gfx: the golden-image scenes (PC port plan 19.6). Each scene is a small
 * synthetic F3DEX 1.x frame built from generator code (no ROM data), dumped as
 * DLMP through the real dumper, then rendered by fast3d/f3dreplay on llvmpipe
 * and compared by PNG hash (golden/golden.sha1, `make -C port/gfx test`).
 *
 *   gold OUTDIR            write OUTDIR/<scene>.dlmp for every scene
 *   gold --list            print the scene names
 *
 * The command words are encoded here by hand in the GBI 1 forms 1080's emitters
 * store (plan 11.1): G_VTX / G_TRI1 / G_LINE3D with index * 2, BA / B9 other
 * modes as shift + length, the three-command F3DEX 1 TEXRECT (E4, B4, B3), and
 * render modes in 1080's habit of setting the cycle 1 blender bits only. Every
 * scene starts from the same prologue: segment 0, colour and depth image, a
 * full-screen scissor, a FILL-mode colour clear and Z clear, the viewport, and an
 * orthographic projection mapping object x / y straight to native pixels around
 * the centre (x right, y up, z towards the eye, depth = -z / 1000).
 *
 * Run 828 (plan 19.7) adds seven scenes copied from named emitters: light
 * (gfx_emitLights / gfx_emitOneLight), texgen (Mat_draw's LookAt +
 * gfx_setupMatTextures), loadtile (gfx_loadTexture's TexImage LoadTile and
 * LoadBlock dxt arms), sprite (Sprite_ctor / Sprite_display: G_ZS_PRIM +
 * G_SETPRIMDEPTH TEXRECTs), cull (CULL_FRONT, U64BGWall's G_CULLDL), splitz
 * (gfx_clearScreen per window) and fader (Fader_display's list verbatim). */

#include "dlump.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------------- arena (game addresses = offset + ARENA_ADDR) ---------------- */

#define ARENA_SIZE (1024 * 1024)
#define ARENA_ADDR 0x00200000u
static unsigned char sArena[ARENA_SIZE] __attribute__((aligned(64)));
static size_t sTop;

static void *alloc(size_t n) {
    void *p;
    sTop = (sTop + 63) & ~(size_t)63;
    if (sTop + n > ARENA_SIZE) {
        fprintf(stderr, "gold: arena full\n");
        exit(1);
    }
    p = &sArena[sTop];
    sTop += n;
    return p;
}

static unsigned int ga(const void *p) {
    return (unsigned int)((const unsigned char *)p - sArena) + ARENA_ADDR;
}

/* ---------------- display-list building ---------------- */

typedef struct { unsigned int w0, w1; } G;
static G *sDl;
static size_t sN, sCap;

static void dl_begin(size_t cap) {
    sDl = alloc(cap * 8);
    sN = 0;
    sCap = cap;
}
/* a sub-list for G_DL: sub_begin switches the builder to a new list, sub_end closes it
 * (G_ENDDL) and returns to the caller's list; one level deep is enough here */
static G *sSavedDl, *sSub;
static size_t sSavedN, sSavedCap;
static void cmd(unsigned int w0, unsigned int w1);
static void sub_begin(size_t cap) {
    sSavedDl = sDl;
    sSavedN = sN;
    sSavedCap = sCap;
    dl_begin(cap);
    sSub = sDl;
}
static G *sub_end(void) {
    G *sub = sSub;
    cmd(0xB8000000u, 0);
    sDl = sSavedDl;
    sN = sSavedN;
    sCap = sSavedCap;
    return sub;
}
static void cmd(unsigned int w0, unsigned int w1) {
    if (sN == sCap) {
        fprintf(stderr, "gold: list full\n");
        exit(1);
    }
    sDl[sN].w0 = w0;
    sDl[sN].w1 = w1;
    sN++;
}

/* RSP, GBI 1 / F3DEX 1.x */
#define G_ZBUFFER 0x1u
#define G_SHADE 0x4u
#define G_SHADING_SMOOTH 0x200u
#define G_CULL_FRONT 0x1000u
#define G_CULL_BACK 0x2000u
#define G_FOG 0x10000u
#define G_LIGHTING 0x20000u
#define G_TEXTURE_GEN 0x40000u
#define G_TEXTURE_GEN_LINEAR 0x80000u

static void sp_vtx(const void *v, int v0, int n) {
    cmd(0x04000000u | ((unsigned)v0 * 2u << 16) | ((unsigned)n << 10) | ((unsigned)n * 16u - 1u), ga(v));
}
static void sp_tri(int a, int b, int c) {
    cmd(0xBF000000u, ((unsigned)a * 2u << 16) | ((unsigned)b * 2u << 8) | (unsigned)c * 2u);
}
static void sp_line(int a, int b, int wd) {
    cmd(0xB5000000u, ((unsigned)a * 2u << 16) | ((unsigned)b * 2u << 8) | (unsigned)wd);
}
static void sp_geom(unsigned int set) {
    cmd(0xB6000000u, 0xFFFFFFFFu);
    cmd(0xB7000000u, set);
}
static void sp_texture(int on, int tile, unsigned int s, unsigned int t) {
    cmd(0xBB000000u | ((unsigned)tile << 8) | (unsigned)on, (s << 16) | t);
}
static void sp_fog(int fmin, int fmax) { /* gSPFogPosition */
    int fm = 128000 / (fmax - fmin), fo = (500 - fmin) * 256 / (fmax - fmin);
    cmd(0xBC000008u, ((unsigned)(fm & 0xFFFF) << 16) | (unsigned)(fo & 0xFFFF));
}

/* RDP */
#define CYC_1 0u
#define CYC_2 (1u << 20)
#define CYC_COPY (2u << 20)
#define CYC_FILL (3u << 20)

static void dp_othermode_h(int shift, int len, unsigned int v) {
    cmd(0xBA000000u | ((unsigned)shift << 8) | (unsigned)len, v);
}
static void dp_cycle(unsigned int c) { dp_othermode_h(20, 2, c); }
static void dp_tlut(int on) { dp_othermode_h(14, 2, on ? (2u << 14) : 0); }  /* TT_NONE / TT_RGBA16 */
static void dp_filter(int bilerp) { dp_othermode_h(12, 2, bilerp ? (2u << 12) : 0); }
static void dp_rendermode(unsigned int rm) { cmd(0xB900031Du, rm); }
static void dp_combine(unsigned int w0, unsigned int w1) { cmd(w0, w1); }
static void dp_scissor(int x0, int y0, int x1, int y1) {
    cmd(0xED000000u | ((unsigned)x0 * 4u << 12) | (unsigned)y0 * 4u, ((unsigned)x1 * 4u << 12) | (unsigned)y1 * 4u);
}
static void dp_fillcolor(unsigned int c) { cmd(0xF7000000u, c); }
static void dp_fillrect(int x0, int y0, int x1, int y1) { /* FILL mode: inclusive lower right */
    cmd(0xF6000000u | ((unsigned)x1 << 14) | ((unsigned)y1 << 2), ((unsigned)x0 << 14) | ((unsigned)y0 << 2));
}
static void dp_prim(unsigned int rgba) { cmd(0xFA000000u, rgba); }
static void dp_env(unsigned int rgba) { cmd(0xFB000000u, rgba); }
static void dp_fogcolor(unsigned int rgba) { cmd(0xF8000000u, rgba); }
static void dp_pipesync(void) { cmd(0xE7000000u, 0); }

/* the combiner: gsDPSetCombineLERP */
enum { C_COMB = 0, C_T0 = 1, C_T1 = 2, C_PRIM = 3, C_SHADE = 4, C_ENV = 5, C_ONE = 6, C_ZERO_A = 15, C_ZERO_C = 31,
       C_ZERO_D = 7, C_T0A = 8, C_PRIMA = 10, C_SHADEA = 11, A_ZERO = 7, A_ONE = 6 };
static void cc(int a0, int b0, int c0, int d0, int Aa0, int Ab0, int Ac0, int Ad0,
               int a1, int b1, int c1, int d1, int Aa1, int Ab1, int Ac1, int Ad1) {
    dp_combine(0xFC000000u | ((unsigned)(a0 & 15) << 20) | ((unsigned)(c0 & 31) << 15) | ((unsigned)(Aa0 & 7) << 12) |
                   ((unsigned)(Ac0 & 7) << 9) | ((unsigned)(a1 & 15) << 5) | (unsigned)(c1 & 31),
               ((unsigned)(b0 & 15) << 28) | ((unsigned)(b1 & 15) << 24) | ((unsigned)(Aa1 & 7) << 21) |
                   ((unsigned)(Ac1 & 7) << 18) | ((unsigned)(d0 & 7) << 15) | ((unsigned)(Ab0 & 7) << 12) |
                   ((unsigned)(Ad0 & 7) << 9) | ((unsigned)(d1 & 7) << 6) | ((unsigned)(Ab1 & 7) << 3) | (unsigned)(Ad1 & 7));
}
/* one-cycle "out = d" helpers: (0 - 0) * 0 + d, both cycles the same (1080's habit) */
#define CC1(c, a) cc(C_ZERO_A, C_ZERO_A, C_ZERO_C, c, A_ZERO, A_ZERO, A_ZERO, a, C_ZERO_A, C_ZERO_A, C_ZERO_C, c, A_ZERO, A_ZERO, A_ZERO, a)

/* render modes, cycle 1 blender only (1080's 1-cycle words) */
#define RM_Z_CMP 0x10u
#define RM_Z_UPD 0x20u
#define RM_IM_RD 0x40u
#define RM_CVG_FULL 0x200u
#define RM_ZMODE_XLU 0x800u
#define RM_ALPHA_CVG_SEL 0x2000u
#define RM_FORCE_BL 0x4000u
#define GBL_C1(p, a, m, b) (((unsigned)(p) << 30) | ((unsigned)(a) << 26) | ((unsigned)(m) << 22) | ((unsigned)(b) << 18))
#define GBL_C2(p, a, m, b) (((unsigned)(p) << 28) | ((unsigned)(a) << 24) | ((unsigned)(m) << 20) | ((unsigned)(b) << 16))
#define RM_OPA (RM_FORCE_BL | GBL_C1(0, 3, 0, 2))                                              /* OPA_SURF */
#define RM_ZB_OPA (RM_Z_CMP | RM_Z_UPD | RM_CVG_FULL | RM_ALPHA_CVG_SEL | GBL_C1(0, 3, 0, 2))  /* ZB_OPA_SURF */
#define RM_XLU (RM_IM_RD | RM_CVG_FULL | RM_FORCE_BL | GBL_C1(0, 0, 1, 0))                     /* XLU_SURF 0x404240 */
#define RM_ZB_XLU (RM_Z_CMP | RM_IM_RD | RM_CVG_FULL | RM_ZMODE_XLU | RM_FORCE_BL | GBL_C1(0, 0, 1, 0))
#define RM_FOG_SHADE_A GBL_C1(3, 2, 0, 0)                                                        /* 0xC8000000 */
#define RM_ZB_OPA2 (RM_Z_CMP | RM_Z_UPD | RM_CVG_FULL | RM_ALPHA_CVG_SEL | GBL_C2(0, 3, 0, 2))
#define RM_OPA2 (RM_FORCE_BL | GBL_C2(0, 3, 0, 2))
#define RM_PASS GBL_C1(0, 3, 0, 2)                                                               /* 0x0C080000 */

/* textures */
enum { FMT_RGBA = 0, FMT_CI = 2, FMT_IA = 3, FMT_I = 4 };
enum { SIZ_4 = 0, SIZ_8 = 1, SIZ_16 = 2, SIZ_32 = 3 };

static void dp_settimg(int fmt, int siz, int width, const void *p) {
    cmd(0xFD000000u | ((unsigned)fmt << 21) | ((unsigned)siz << 19) | (unsigned)(width - 1), ga(p));
}
static int sMask; /* log2 of the wrap period for the next render tiles (0: no mask) */
static void dp_settile(int fmt, int siz, int line, int tmem, int tile, int pal) {
    cmd(0xF5000000u | ((unsigned)fmt << 21) | ((unsigned)siz << 19) | ((unsigned)line << 9) | (unsigned)tmem,
        ((unsigned)tile << 24) | ((unsigned)pal << 20) | ((unsigned)sMask << 14) | ((unsigned)sMask << 4)); /* wrap */
}
static void dp_tilesize(int tile, int w, int h) {
    cmd(0xF2000000u, ((unsigned)tile << 24) | ((unsigned)(w - 1) * 4u << 12) | (unsigned)(h - 1) * 4u);
}

/* gsDPLoadTextureBlock(_4b) into TMEM 0 for render tile `tile` (LoadBlock as 16b, the
 * way gfx_loadTexture does it), then the render tile's descriptor and size */
static void load_block(const void *tex, int fmt, int siz, int w, int h, int tmem, int tile, int pal) {
    int bits = 4 << siz, words, lrs, dxt, line;
    words = w * bits / 64;           /* 64-bit TMEM words per row */
    if (words < 1) {
        words = 1;
    }
    dxt = (2048 + words - 1) / words;
    lrs = siz == SIZ_32 ? w * h - 1 : w * h * bits / 16 - 1; /* in 16-bit (or 32-bit) texels */
    line = siz == SIZ_32 ? (w * 2 + 7) / 8 : (w * bits / 8 + 7) / 8; /* 32b: the split RG / BA halves */
    dp_settimg(fmt, siz == SIZ_32 ? SIZ_32 : SIZ_16, 1, tex);
    {
        int m = sMask;
        sMask = 0;
        dp_settile(fmt, siz == SIZ_32 ? SIZ_32 : SIZ_16, 0, tmem, 7, 0);
        sMask = m;
    }
    cmd(0xE6000000u, 0); /* LOADSYNC */
    cmd(0xF3000000u, (7u << 24) | ((unsigned)lrs << 12) | (unsigned)dxt);
    dp_pipesync();
    dp_settile(fmt, siz, line, tmem, tile, pal);
    dp_tilesize(tile, w, h);
}

static void load_tlut(const void *tlut, int count, int tmem) {
    dp_settimg(FMT_RGBA, SIZ_16, 1, tlut);
    dp_settile(0, 0, 0, tmem, 7, 0);
    cmd(0xE6000000u, 0);
    cmd(0xF0000000u, (7u << 24) | ((unsigned)(count - 1) << 14));
    dp_pipesync();
}

/* ---------------- vertices ---------------- */

typedef struct {
    short ob[3];
    unsigned short flag;
    short tc[2];
    unsigned char cn[4];
} Vtx1;

static Vtx1 *vbuf(int n) { return alloc((size_t)n * sizeof(Vtx1)); }
static void vset(Vtx1 *v, int x, int y, int z, int s, int t, unsigned int rgba) {
    memset(v, 0, sizeof(*v));
    v->ob[0] = (short)x;
    v->ob[1] = (short)y;
    v->ob[2] = (short)z;
    v->tc[0] = (short)(s * 32);
    v->tc[1] = (short)(t * 32);
    v->cn[0] = (unsigned char)(rgba >> 24);
    v->cn[1] = (unsigned char)(rgba >> 16);
    v->cn[2] = (unsigned char)(rgba >> 8);
    v->cn[3] = (unsigned char)rgba;
}
/* an axis-aligned quad at depth z: 4 vertices (TL, TR, BL, BR), texture w x h over it */
static Vtx1 *quad(int x0, int y0, int x1, int y1, int z, int tw, int th, const unsigned int col[4]) {
    Vtx1 *v = vbuf(4);
    vset(&v[0], x0, y1, z, 0, 0, col[0]);
    vset(&v[1], x1, y1, z, tw, 0, col[1]);
    vset(&v[2], x0, y0, z, 0, th, col[2]);
    vset(&v[3], x1, y0, z, tw, th, col[3]);
    return v;
}
static void draw_quad(const Vtx1 *v) {
    sp_vtx(v, 0, 4);
    sp_tri(0, 2, 1);
    sp_tri(1, 2, 3);
}
static const unsigned int kWhite4[4] = { 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu };
static const unsigned int kRgbw[4] = { 0xFF0000FFu, 0x00FF00FFu, 0x0000FFFFu, 0xFFFFFFFFu };

/* ---------------- texels (N64 big-endian image) ---------------- */

static void be16(unsigned char *p, unsigned int v) {
    p[0] = (unsigned char)(v >> 8);
    p[1] = (unsigned char)v;
}
static unsigned int rgba5551(int r, int g, int b, int a) {
    return ((unsigned)(r >> 3) << 11) | ((unsigned)(g >> 3) << 6) | ((unsigned)(b >> 3) << 1) | (a ? 1u : 0u);
}

/* 16x16 RGBA16: a 4x4 checker of red / yellow cells with a blue diagonal */
static void *tex_rgba16(void) {
    unsigned char *t = alloc(16 * 16 * 2);
    int x, y;
    for (y = 0; y < 16; y++) {
        for (x = 0; x < 16; x++) {
            unsigned int c = ((x / 4 + y / 4) & 1) ? rgba5551(255, 224, 0, 1) : rgba5551(208, 0, 0, 1);
            if (x == y) {
                c = rgba5551(0, 64, 255, 1);
            }
            be16(t + (y * 16 + x) * 2, c);
        }
    }
    return t;
}
/* 16x16 CI4: index = (x + y) & 15, with a 16-entry palette */
static void *tex_ci4(void) {
    unsigned char *t = alloc(16 * 16 / 2);
    int x, y;
    for (y = 0; y < 16; y++) {
        for (x = 0; x < 16; x += 2) {
            t[(y * 16 + x) / 2] = (unsigned char)((((x + y) & 15) << 4) | ((x + 1 + y) & 15));
        }
    }
    return t;
}
/* 16x16 CI8: index = y * 16 + x, with a 256-entry palette */
static void *tex_ci8(void) {
    unsigned char *t = alloc(16 * 16);
    int i;
    for (i = 0; i < 256; i++) {
        t[i] = (unsigned char)i;
    }
    return t;
}
static void *tlut_ramp(int n) { /* a hue-ish ramp, alpha 1 */
    unsigned char *p = alloc((size_t)n * 2);
    int i;
    for (i = 0; i < n; i++) {
        int k = i * 256 / n;
        be16(p + i * 2, rgba5551(k, 255 - k, (k * 3) & 255, 1));
    }
    return p;
}
/* 16x16 IA8: intensity = x * 16, alpha = 15 except a transparent ring */
static void *tex_ia8(void) {
    unsigned char *t = alloc(16 * 16);
    int x, y;
    for (y = 0; y < 16; y++) {
        for (x = 0; x < 16; x++) {
            int dx = 2 * x - 15, dy = 2 * y - 15, r2 = dx * dx + dy * dy;
            int a = (r2 > 100 && r2 < 200) ? 0 : 15;
            t[y * 16 + x] = (unsigned char)(((x & 15) << 4) | a);
        }
    }
    return t;
}
/* 16x16 I4: intensity = y */
static void *tex_i4(void) {
    unsigned char *t = alloc(16 * 16 / 2);
    int x, y;
    for (y = 0; y < 16; y++) {
        for (x = 0; x < 16; x += 2) {
            int a = (y + (x >> 2)) & 15, b = (y + ((x + 1) >> 2)) & 15;
            t[(y * 16 + x) / 2] = (unsigned char)((a << 4) | b);
        }
    }
    return t;
}
/* 16x16 I8 mask for the 2-cycle multi-texture: a horizontal ramp */
static void *tex_i8(void) {
    unsigned char *t = alloc(16 * 16);
    int x, y;
    for (y = 0; y < 16; y++) {
        for (x = 0; x < 16; x++) {
            t[y * 16 + x] = (unsigned char)(x * 17);
        }
    }
    return t;
}

/* ---------------- the frame skeleton ---------------- */

static unsigned short *sCimg, *sZimg;
static unsigned int sTaskAddr;

static void mtx_f2l(unsigned int *m, const float f[4][4]) { /* guMtxF2L */
    int i;
    for (i = 0; i < 8; i++) {
        int a = (int)(f[(2 * i) / 4][(2 * i) % 4] * 65536.0f);
        int b = (int)(f[(2 * i + 1) / 4][(2 * i + 1) % 4] * 65536.0f);
        m[i] = ((unsigned int)a & 0xFFFF0000u) | (((unsigned int)b >> 16) & 0xFFFFu);
        m[i + 8] = ((unsigned int)a << 16) | ((unsigned int)b & 0xFFFFu);
    }
}

/* starts the root list (sDl); the scene appends to it, and frame_end closes it */
static void frame_begin(unsigned int clearColor) {
    unsigned int *proj, *mv;
    short *vp;
    float p[4][4], id[4][4];
    int i;

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
    vp = alloc(16);
    vp[0] = 640; vp[1] = 480; vp[2] = 511; vp[3] = 0;
    vp[4] = 640; vp[5] = 480; vp[6] = 511; vp[7] = 0;

    dl_begin(512);
    cmd(0xBC000006u, 0);                       /* MOVEWORD SEGMENT 0 = 0 */
    cmd(0xFF100000u | 319, ga(sCimg));         /* SETCIMG RGBA16 320 */
    cmd(0xFE000000u, ga(sZimg));               /* SETZIMG */
    dp_pipesync();
    dp_scissor(0, 0, 320, 240);
    dp_cycle(CYC_FILL);
    dp_rendermode(0);
    dp_fillcolor(0xFFFCFFFCu);                 /* Z clear: CIMG = the Z buffer */
    cmd(0xFF100000u | 319, ga(sZimg));
    dp_fillrect(0, 0, 319, 239);
    dp_pipesync();
    cmd(0xFF100000u | 319, ga(sCimg));
    dp_fillcolor(clearColor);
    dp_fillrect(0, 0, 319, 239);
    dp_pipesync();
    cmd(0x03800010u, ga(vp));                  /* MOVEMEM VIEWPORT */
    cmd(0x01030040u, ga(proj));                /* MTX projection LOAD */
    cmd(0x01020040u, ga(mv));                  /* MTX modelview LOAD */
    dp_cycle(CYC_1);
    dp_othermode_h(19, 1, 1u << 19);           /* TEXTPERSP on */
    dp_filter(0);                              /* point sampling: exact texels */
    dp_tlut(0);
}

static unsigned int frame_end(void) {
    cmd(0xE9000000u, 0);                       /* FULLSYNC */
    cmd(0xB8000000u, 0);                       /* ENDDL */
    return ga(sDl);
}

/* ---------------- scenes ---------------- */

#define FILL2(c) (((c) << 16) | (c))

static void scene_fill(void) { /* FILL-mode rects, a scissored one, a partial Z clear */
    frame_begin(FILL2(rgba5551(0, 0, 96, 1)));
    dp_cycle(CYC_FILL);
    dp_fillcolor(FILL2(rgba5551(255, 0, 0, 1)));
    dp_fillrect(10, 10, 109, 69);
    dp_pipesync();
    dp_fillcolor(FILL2(rgba5551(0, 255, 0, 1)));
    dp_fillrect(60, 40, 199, 139);
    dp_pipesync();
    dp_scissor(160, 120, 320, 240);            /* split-screen style window */
    dp_fillcolor(FILL2(rgba5551(255, 255, 255, 1)));
    dp_fillrect(100, 100, 299, 219);
    dp_pipesync();
    dp_scissor(0, 0, 320, 240);
}

static void scene_shade(void) { /* Gouraud, back-face culling, Z buffer (two interpenetrating tris) */
    Vtx1 *v = vbuf(9);
    frame_begin(FILL2(rgba5551(32, 32, 32, 1)));
    dp_rendermode(RM_ZB_OPA);
    CC1(C_SHADE, A_ONE);
    sp_geom(G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK);
    /* red: flat at z = 0; green: z = 2.5 x, so it is behind red on the left, in front on the right */
    vset(&v[0], -140, -100, 0, 0, 0, 0xFF0000FFu);
    vset(&v[1], 100, -100, 0, 0, 0, 0xFF0000FFu);
    vset(&v[2], -20, 100, 0, 0, 0, 0xFF8080FFu);
    vset(&v[3], -120, -80, -300, 0, 0, 0x00FF00FFu);
    vset(&v[4], 140, -80, 350, 0, 0, 0x00FF00FFu);
    vset(&v[5], 20, 110, 50, 0, 0, 0x80FF80FFu);
    vset(&v[6], -150, 110, 0, 0, 0, 0xFFFF00FFu); /* clockwise on screen: culled */
    vset(&v[7], -110, 110, 0, 0, 0, 0xFFFF00FFu);
    vset(&v[8], -130, 80, 0, 0, 0, 0xFFFF00FFu);
    sp_vtx(v, 0, 9);
    sp_tri(0, 1, 2);
    sp_tri(3, 4, 5);
    sp_tri(6, 7, 8);
}

static void textured_quad(int x0, int y0, int x1, int y1, int tw, int th) {
    draw_quad(quad(x0, y0, x1, y1, 0, tw, th, kWhite4));
}

static void tex_begin(unsigned int clear) {
    frame_begin(clear);
    dp_rendermode(RM_OPA);
    sp_geom(G_SHADE | G_SHADING_SMOOTH);
    sp_texture(1, 0, 0xFFFF, 0xFFFF);
    CC1(C_T0, A_ONE);
}

static void scene_tex_rgba16(void) {
    void *t = tex_rgba16();
    tex_begin(FILL2(rgba5551(0, 0, 0, 1)));
    load_block(t, FMT_RGBA, SIZ_16, 16, 16, 0, 0, 0);
    textured_quad(-150, -110, -10, 30, 16, 16);   /* one repeat, 8.75x magnified */
    dp_pipesync();
    sMask = 4;                                    /* masks / maskt = 4: wrap every 16 texels */
    load_block(t, FMT_RGBA, SIZ_16, 16, 16, 0, 0, 0);
    sMask = 0;
    textured_quad(10, -110, 150, 30, 32, 32);     /* wrapped twice each way */
    dp_pipesync();
    CC1(C_SHADE, A_ONE);                          /* untextured reference strip */
    draw_quad(quad(-150, 50, 150, 110, 0, 0, 0, kRgbw));
}

static void scene_tex_ci(void) { /* CI4 (palette 0 at TMEM 256) and CI8 (256 entries) */
    void *t4 = tex_ci4(), *t8 = tex_ci8(), *p16 = tlut_ramp(16), *p256 = tlut_ramp(256);
    tex_begin(FILL2(rgba5551(0, 0, 0, 1)));
    dp_tlut(1);
    load_tlut(p16, 16, 256);
    load_block(t4, FMT_CI, SIZ_4, 16, 16, 0, 0, 0);
    textured_quad(-150, -100, -10, 100, 16, 16);
    dp_pipesync();
    load_tlut(p256, 256, 256);
    load_block(t8, FMT_CI, SIZ_8, 16, 16, 0, 0, 0);
    textured_quad(10, -100, 150, 100, 16, 16);
}

static void scene_tex_ia_i(void) { /* IA8 with alpha through the XLU blender, I4 opaque */
    void *ia = tex_ia8(), *i4 = tex_i4();
    tex_begin(FILL2(rgba5551(0, 96, 0, 1)));
    load_block(i4, FMT_I, SIZ_4, 16, 16, 0, 0, 0);
    textured_quad(10, -100, 150, 100, 16, 16);
    dp_pipesync();
    load_block(ia, FMT_IA, SIZ_8, 16, 16, 0, 0, 0);
    dp_rendermode(RM_XLU);
    CC1(C_T0, C_T0);                              /* colour T0, alpha T0 */
    textured_quad(-150, -100, -10, 100, 16, 16);
}

static void scene_combine(void) { /* PRIM, ENV, T0 * SHADE, lerp(ENV, PRIM, T0) in 1 cycle */
    void *t = tex_rgba16();
    tex_begin(FILL2(rgba5551(0, 0, 0, 1)));
    dp_prim(0x2080FFFFu);
    dp_env(0xFFC000FFu);
    load_block(t, FMT_RGBA, SIZ_16, 16, 16, 0, 0, 0);
    CC1(C_PRIM, A_ONE);
    draw_quad(quad(-150, 10, -10, 110, 0, 0, 0, kWhite4));
    CC1(C_ENV, A_ONE);
    draw_quad(quad(10, 10, 150, 110, 0, 0, 0, kWhite4));
    /* T0 * SHADE: (T0 - 0) * SHADE + 0 */
    cc(C_T0, C_ZERO_A, C_SHADE, C_ZERO_D, A_ZERO, A_ZERO, A_ZERO, A_ONE,
       C_T0, C_ZERO_A, C_SHADE, C_ZERO_D, A_ZERO, A_ZERO, A_ZERO, A_ONE);
    draw_quad(quad(-150, -110, -10, -10, 0, 16, 16, kRgbw));
    /* (PRIM - ENV) * T0 + ENV */
    cc(C_PRIM, C_ENV, C_T0, C_ENV, A_ZERO, A_ZERO, A_ZERO, A_ONE,
       C_PRIM, C_ENV, C_T0, C_ENV, A_ZERO, A_ZERO, A_ZERO, A_ONE);
    draw_quad(quad(10, -110, 150, -10, 0, 16, 16, kWhite4));
}

static void scene_multitex(void) { /* 2-cycle: lerp(T0, T1, PRIM_A), then * SHADE (1080's FC257E04 shape) */
    void *a = tex_rgba16();
    tex_begin(FILL2(rgba5551(0, 0, 0, 1)));
    dp_cycle(CYC_2);
    dp_rendermode(RM_PASS | RM_OPA2);
    load_block(a, FMT_RGBA, SIZ_16, 16, 16, 0, 0, 0);
    load_block(tex_i8(), FMT_I, SIZ_8, 16, 16, 256, 1, 0);
    dp_prim(0x00000080u);                               /* PRIM_A = 0.5 */
    cc(C_T1, C_T0, C_PRIMA, C_T0, A_ZERO, A_ZERO, A_ZERO, A_ONE,
       C_COMB, C_ZERO_A, C_SHADE, C_ZERO_D, A_ZERO, A_ZERO, A_ZERO, A_ONE);
    draw_quad(quad(-150, -110, 150, 110, 0, 16, 16, kRgbw));
}

static void scene_fog(void) { /* 2-cycle FOG_SHADE_A + G_MW_FOG, depth ramp left to right */
    Vtx1 *v = vbuf(4);
    frame_begin(FILL2(rgba5551(0, 0, 0, 1)));
    dp_cycle(CYC_2);
    dp_rendermode(RM_FOG_SHADE_A | RM_ZB_OPA2);
    dp_fogcolor(0xC0C0FFFFu);
    CC1(C_SHADE, A_ONE);
    sp_geom(G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH | G_FOG);
    sp_fog(0, 1000);
    vset(&v[0], -150, 100, 900, 0, 0, 0xFF4000FFu);
    vset(&v[1], 150, 100, -900, 0, 0, 0xFF4000FFu);
    vset(&v[2], -150, -100, 900, 0, 0, 0x4000FFFFu);
    vset(&v[3], 150, -100, -900, 0, 0, 0x4000FFFFu);
    draw_quad(v);
}

static void scene_texrect(void) { /* COPY-mode TEXRECT (sprites, gui) and a 1-cycle scaled one */
    void *t = tex_rgba16();
    tex_begin(FILL2(rgba5551(48, 0, 48, 1)));
    load_block(t, FMT_RGBA, SIZ_16, 16, 16, 0, 0, 0);
    dp_cycle(CYC_COPY);
    dp_rendermode(0);
    /* COPY: x1 inclusive, dsdx = 4.0 (4 texels per pixel in copy mode) */
    cmd(0xE4000000u | ((40u + 15u) << 2 << 12) | ((40u + 15u) << 2), (0u << 24) | (40u << 2 << 12) | (40u << 2));
    cmd(0xB4000000u, 0);                           /* s, t = 0 */
    cmd(0xB3000000u, (4u << 10 << 16) | (1u << 10)); /* dsdx 4.0, dtdy 1.0 (s5.10) */
    dp_pipesync();
    dp_cycle(CYC_1);
    dp_rendermode(RM_OPA);
    CC1(C_T0, A_ONE);
    /* 1-cycle, 4x magnified: 64x64 pixels, dsdx = dtdy = 0.25 */
    cmd(0xE4000000u | (200u << 2 << 12) | (164u << 2), (0u << 24) | (136u << 2 << 12) | (100u << 2));
    cmd(0xB4000000u, 0);
    cmd(0xB3000000u, (256u << 16) | 256u);
}

static void scene_xlu(void) { /* translucent surfaces in 1080's cycle-1-only render words */
    frame_begin(FILL2(rgba5551(0, 0, 0, 1)));
    sp_geom(G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH);
    dp_rendermode(RM_ZB_OPA);
    CC1(C_SHADE, A_ONE);
    draw_quad(quad(-150, -60, 30, 110, 0, 0, 0, kRgbw));
    dp_rendermode(RM_ZB_XLU);
    dp_prim(0xFFFFFF80u);
    CC1(C_SHADE, C_PRIM);                          /* alpha = PRIM_A = 0.5 */
    draw_quad(quad(-30, -110, 150, 60, 100, 0, 0, kWhite4));
}

static void scene_line3d(void) { /* G_LINE3D (patch 1): widths 0 (= 1) and 4 */
    Vtx1 *v = vbuf(4);
    frame_begin(FILL2(rgba5551(0, 0, 0, 1)));
    dp_rendermode(RM_OPA);
    sp_geom(G_SHADE | G_SHADING_SMOOTH);
    CC1(C_SHADE, A_ONE);
    vset(&v[0], -140, -100, 0, 0, 0, 0xFF0000FFu);
    vset(&v[1], 140, 100, 0, 0, 0, 0x00FF00FFu);
    vset(&v[2], -140, 100, 0, 0, 0, 0xFFFF00FFu);
    vset(&v[3], 140, -100, 0, 0, 0, 0x00FFFFFFu);
    sp_vtx(v, 0, 4);
    sp_line(0, 1, 0);
    sp_line(2, 3, 4);
}

/* ---------------- run 828: the 19.6 item 6 features, in 1080's own command forms ---------------- */

static void vsetn(Vtx1 *v, int x, int y, int z, int s, int t, int nx, int ny, int nz) { /* Vtx_tn: cn = s8 normal */
    vset(v, x, y, z, s, t, 0xFFu);
    v->cn[0] = (unsigned char)(signed char)nx;
    v->cn[1] = (unsigned char)(signed char)ny;
    v->cn[2] = (unsigned char)(signed char)nz;
}

/* a 5x5 vertex dome over [x0, x1] x [y0, y1]: flat in position, normals bulging like a
 * hemisphere (n = normalize(u, v, 1.2), u, v in [-1, 1]), 32 front-facing triangles */
static void draw_dome(int x0, int y0, int x1, int y1) {
    Vtx1 *v = vbuf(25);
    int i, j;
    for (j = 0; j < 5; j++) {
        for (i = 0; i < 5; i++) {
            float u = (float)(i - 2) / 2.0f, w = (float)(2 - j) / 2.0f, l;
            l = 1.0f / __builtin_sqrtf(u * u + w * w + 1.44f);
            vsetn(&v[j * 5 + i], x0 + (x1 - x0) * i / 4, y1 - (y1 - y0) * j / 4, 0, 0, 0, (int)(u * l * 127.0f),
                  (int)(w * l * 127.0f), (int)(1.2f * l * 127.0f));
        }
    }
    sp_vtx(v, 0, 25);
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            int a = j * 5 + i, b = a + 1, c = a + 5, d = c + 1; /* TL, TR, BL, BR */
            sp_tri(a, c, b);
            sp_tri(b, c, d);
        }
    }
}

/* the Lights record of gfx_emitLights_000454C4 (game_libs_ncc_454C4.cpp): Ambient (8 bytes) then Light[4]
 * (16 bytes each), dir bytes = dir * 120 */
static unsigned char *lights_rec(void) {
    unsigned char *r = alloc(0x48);
    memset(r, 0, 0x48);
    return r;
}
static void light_set(unsigned char *l, int r, int g, int b, int dx, int dy, int dz) {
    l[0] = l[4] = (unsigned char)r;
    l[1] = l[5] = (unsigned char)g;
    l[2] = l[6] = (unsigned char)b;
    l[8] = (unsigned char)(signed char)dx;
    l[9] = (unsigned char)(signed char)dy;
    l[10] = (unsigned char)(signed char)dz;
}
/* gfx_emitLights_000454C4, n lights lit: gSPLight(l[i], i + 1) each, then gSPLight(ambient, n + 1) and
 * gSPNumLights(n) (G_MOVEWORD NUMLIGHT, F3D form (n + 1) * 32 + 0x80000000) */
static void emit_lights(unsigned char *rec, int n) {
    int i;
    for (i = 0; i < n; i++) {
        cmd(0x03000010u | ((0x86u + (unsigned)i * 2u) << 16), ga(rec + 8 + 16 * i));
    }
    cmd(0x03000010u | ((0x86u + (unsigned)n * 2u) << 16), ga(rec));
    cmd(0xBC000002u, (unsigned)(n + 1) * 32u + 0x80000000u);
}
/* the n = 0 default path of gfx_emitLights_000454C4: l[0] = the ambient colour, gSPLight(l[0], 1),
 * gSPLight(ambient, 2), gSPNumLights(1) */
static void emit_lights_default(unsigned char *rec) {
    memcpy(rec + 8, rec, 3);
    memcpy(rec + 12, rec + 4, 3);
    cmd(0x03860010u, ga(rec + 8));
    cmd(0x03880010u, ga(rec));
    cmd(0xBC000002u, 0x80000040u);
}

static void scene_light(void) { /* G_LIGHTING: gfx_emitLights (2 lights + ambient) and gfx_emitOneLight */
    unsigned char *rec = lights_rec(), *one = lights_rec();
    frame_begin(FILL2(rgba5551(0, 0, 0, 1)));
    dp_rendermode(RM_ZB_OPA);
    CC1(C_SHADE, A_ONE);
    sp_geom(G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK | G_LIGHTING);
    light_set(rec, 24, 24, 40, 0, 0, 0);               /* ambient */
    light_set(rec + 8, 255, 64, 0, -85, 0, 85);        /* l[0]: orange from the left */
    light_set(rec + 24, 0, 96, 255, 0, 85, 85);        /* l[1]: blue from above */
    emit_lights(rec, 2);
    draw_dome(-150, -70, -10, 70);
    /* gfx_emitOneLight_00045CB0 as Mat_draw calls it (colour {1, 1, 0} * 255): one 0x48 record whose bytes
     * 0..2, 4..6, 8..10 and 12..14 all hold the colour; NUMLIGHT 1, then L1 (0x88) = record + 8 (the ambient)
     * and L0 (0x86) = the record (so light 0's direction bytes are the colour again) */
    one[0] = one[4] = one[8] = one[12] = 255;
    one[1] = one[5] = one[9] = one[13] = 255;
    one[2] = one[6] = one[10] = one[14] = 0;
    cmd(0xBC000002u, 0x80000040u);
    cmd(0x03880010u, ga(one + 8));
    cmd(0x03860010u, ga(one));
    draw_dome(10, -70, 150, 70);
    /* back to an unlit quad after lighting (the shade colours are used again) */
    sp_geom(G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK);
    draw_quad(quad(-150, -110, 150, -85, 0, 0, 0, kRgbw));
}

/* 32 x 32 (or 64 x 32) RGBA16 for texgen: red = s, green = t, blue grid lines every 8 texels */
static void *tex_st(int w) {
    unsigned char *t = alloc((size_t)w * 32 * 2);
    int x, y;
    for (y = 0; y < 32; y++) {
        for (x = 0; x < w; x++) {
            unsigned int c = rgba5551(x * 256 / w, y * 8, ((x & 7) == 0 || (y & 7) == 0) ? 255 : 0, 1);
            be16(t + (y * w + x) * 2, c);
        }
    }
    return t;
}

static void scene_texgen(void) { /* G_TEXTURE_GEN (+ LINEAR) with Mat_draw's LookAt and gfx_setupMatTextures */
    unsigned char *rec = lights_rec(), *la = alloc(32);
    void *t32 = tex_st(32), *t64 = tex_st(64);
    frame_begin(FILL2(rgba5551(0, 0, 0, 1)));
    dp_rendermode(RM_ZB_OPA);
    /* gfx_makeLookAt (guLookAtReflect layout): l[0] = the x axis (s), colour 0; l[1] = the y axis (t), colour
     * 0x80 green; dir * 120 */
    memset(la, 0, 32);
    la[8] = 120;
    la[16 + 1] = la[16 + 5] = 0x80;
    la[16 + 9] = 120;
    /* Mat_draw: LOOKATX = the LookAt, LOOKATY = the LookAt + 0x10 */
    cmd(0x03840010u, ga(la));
    cmd(0x03820010u, ga(la) + 0x10u);
    light_set(rec, 255, 255, 255, 0, 0, 0);
    emit_lights_default(rec);
    sp_geom(G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK | G_LIGHTING);
    /* gfx_setupMatTextures_00045E20: geomFlags bit 20 -> B7 TEXTURE_GEN (bit 24 adds _LINEAR), and
     * G_TEXTURE BB040801 with scale 1984 / 1984, or 3968 / 1984 for a 64-wide texture */
    cmd(0xB7000000u, 0x00040000u);
    cmd(0xBB040801u, (1984u << 16) | 1984u);
    CC1(C_T0, A_ONE);
    load_block(t32, FMT_RGBA, SIZ_16, 32, 32, 0, 0, 0);
    draw_dome(-150, -70, -10, 70);
    dp_pipesync();
    /* a new LookAt without a new NUMLIGHT (Mat_draw rebuilds only the texgen list): the x axis mirrored, so S
     * now falls from left to right */
    la = alloc(32);
    memset(la, 0, 32);
    la[8] = (unsigned char)(signed char)-120;
    la[16 + 1] = la[16 + 5] = 0x80;
    la[16 + 9] = 120;
    cmd(0x03840010u, ga(la));
    cmd(0x03820010u, ga(la) + 0x10u);
    cmd(0xB7000000u, 0x000C0000u);
    cmd(0xBB040801u, (3968u << 16) | 1984u);
    load_block(t64, FMT_RGBA, SIZ_16, 64, 32, 0, 0, 0);
    draw_dome(10, -70, 150, 70);
    sp_geom(G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK);
    cmd(0xBB000000u, 0xFFFFFFFFu);                     /* G_TEXTURE off */
    CC1(C_SHADE, A_ONE);
    draw_quad(quad(-150, -110, 150, -85, 0, 0, 0, kRgbw));
}

/* a 64 x 32 RGBA16 atlas (red = x, green = y, white 16 x 16 cell borders, a blue cell index dot) and
 * a 64 x 32 I4 atlas (intensity = x / 4) */
static void *atlas_rgba16(void) {
    unsigned char *t = alloc(64 * 32 * 2);
    int x, y;
    for (y = 0; y < 32; y++) {
        for (x = 0; x < 64; x++) {
            unsigned int c = rgba5551(x * 4, y * 8, 0, 1);
            if ((x & 15) == 0 || (y & 15) == 0) {
                c = rgba5551(255, 255, 255, 1);
            } else if ((x & 15) == 1 + (x >> 4) * 3 && (y & 15) == 4) {
                c = rgba5551(0, 0, 255, 1);
            }
            be16(t + (y * 64 + x) * 2, c);
        }
    }
    return t;
}
static void *atlas_i4(void) {
    unsigned char *t = alloc(64 * 32 / 2);
    int x, y;
    for (y = 0; y < 32; y++) {
        for (x = 0; x < 64; x += 2) {
            int a = ((x >> 2) ^ (y >> 3)) & 15, b = (((x + 1) >> 2) ^ (y >> 3)) & 15;
            t[(y * 64 + x) / 2] = (unsigned char)((a << 4) | b);
        }
    }
    return t;
}
/* 16 x 16 RGBA32: an RGB gradient with a dark diagonal */
static void *tex_rgba32(void) {
    unsigned char *t = alloc(16 * 16 * 4);
    int x, y;
    for (y = 0; y < 16; y++) {
        for (x = 0; x < 16; x++) {
            unsigned char *p = t + (y * 16 + x) * 4;
            p[0] = (unsigned char)(x * 16);
            p[1] = (unsigned char)(y * 16);
            p[2] = (unsigned char)(255 - x * 8);
            p[3] = 255;
            if (x == 15 - y) {
                p[0] = p[1] = p[2] = 0;
            }
        }
    }
    return t;
}
/* 24 x 8 RGBA16 (a width whose dxt is not exact: 6 words per row, dxt 342) */
static void *tex_24x8(void) {
    unsigned char *t = alloc(24 * 8 * 2);
    int x, y;
    for (y = 0; y < 8; y++) {
        for (x = 0; x < 24; x++) {
            be16(t + (y * 24 + x) * 2, (x / 3 + y) & 1 ? rgba5551(255, 255, 255, 1) : rgba5551(x * 10, 0, 128, 1));
        }
    }
    return t;
}

/* gfx_loadTexture_000578B4, texFlags 0x10 (texels -> a TexImage record): LoadTile of the (s, t) w x h
 * sub-rectangle of an image `width` texels wide. RGBA16 (switch 1, s1_110) */
static void loadtile_rgba16(const void *img, int width, int s, int t, int w, int h) {
    int line = ((w * 2) + 7) >> 3;
    cmd(0xFD100000u | (unsigned)((width - 1) & 0xFFF), ga(img));
    cmd(0xF5100000u | ((unsigned)line << 9), 0x07000000u);
    cmd(0xE6000000u, 0);
    cmd(0xF4000000u | ((unsigned)(s * 4 & 0xFFF) << 12) | (unsigned)(t * 4 & 0xFFF),
        0x07000000u | ((unsigned)(((s + w - 1) << 2) & 0xFFF) << 12) | (unsigned)(((t + h - 1) << 2) & 0xFFF));
    cmd(0xE7000000u, 0);
    cmd(0xF5100000u | ((unsigned)line << 9), 0);
    cmd(0xF2000000u, ((unsigned)((w - 1) << 2) << 12) | (unsigned)((h - 1) << 2));
}
/* the I4 arm (s1_304): the image is loaded as I8 at half width, the render tile is I4 */
static void loadtile_i4(const void *img, int width, int s, int t, int w, int h) {
    int line = ((w >> 1) + 7) >> 3;
    cmd(0xFD880000u | (unsigned)(((width >> 1) - 1) & 0xFFF), ga(img));
    cmd(0xF5880000u | ((unsigned)line << 9), 0x07000000u);
    cmd(0xE6000000u, 0);
    cmd(0xF4000000u | ((unsigned)(s * 2 & 0xFFF) << 12) | (unsigned)(t * 4 & 0xFFF),
        0x07000000u | ((unsigned)(((s + w - 1) << 1) & 0xFFF) << 12) | (unsigned)(((t + h - 1) << 2) & 0xFFF));
    cmd(0xE7000000u, 0);
    cmd(0xF5800000u | ((unsigned)line << 9), 0);
    cmd(0xF2000000u, ((unsigned)((w - 1) << 2) << 12) | (unsigned)((h - 1) << 2));
}
/* the plain-texels arm (switch 2): LoadBlock with dxt = (words + 0x7FF) / words, words = the 64-bit TMEM
 * words per row; RGBA32 (s2_120) loads as 32b, RGBA16 as 16b */
static void loadblock_1080(const void *tex, int siz, int w, int h) {
    int words = siz == SIZ_32 ? w * 4 / 8 : w * 2 / 8, lrs = w * h - 1, line;
    if (words <= 0) {
        words = 1;
    }
    if (lrs > 0x7FF) {
        lrs = 0x7FF;
    }
    line = siz == SIZ_32 ? (w * 2 + 7) >> 3 : (w * 2 + 7) >> 3;
    cmd(0xFD000000u | ((unsigned)siz << 19), ga(tex));
    cmd(0xF5000000u | ((unsigned)siz << 19), 0x07000000u);
    cmd(0xE6000000u, 0);
    cmd(0xF3000000u, ((unsigned)lrs << 12) | 0x07000000u | (unsigned)((words + 0x7FF) / words));
    cmd(0xE7000000u, 0);
    cmd(0xF5000000u | ((unsigned)siz << 19) | ((unsigned)line << 9), 0);
    cmd(0xF2000000u, ((unsigned)((w - 1) << 2) << 12) | (unsigned)((h - 1) << 2));
}

static void scene_loadtile(void) { /* LoadTile sub-rectangles (uls / ult != 0) and LoadBlock dxt */
    void *atlas = atlas_rgba16(), *ai4 = atlas_i4(), *t32 = tex_rgba32(), *t24 = tex_24x8();
    tex_begin(FILL2(rgba5551(0, 0, 64, 1)));
    loadtile_rgba16(atlas, 64, 24, 8, 16, 16);         /* straddles four atlas cells */
    textured_quad(-150, 10, -10, 110, 16, 16);
    dp_pipesync();
    loadtile_rgba16(atlas, 64, 48, 16, 16, 16);        /* the bottom-right cell */
    textured_quad(10, 10, 150, 110, 16, 16);
    dp_pipesync();
    loadtile_i4(ai4, 64, 32, 16, 16, 16);
    textured_quad(-150, -100, -60, -10, 16, 16);
    dp_pipesync();
    loadblock_1080(t32, SIZ_32, 16, 16);
    textured_quad(-50, -100, 40, -10, 16, 16);
    dp_pipesync();
    loadblock_1080(t24, SIZ_16, 24, 8);
    textured_quad(50, -100, 150, -10, 24, 8);
}

/* 16 x 16 RGBA16 sprite: a disc (alpha 1) with a ring of colours, transparent outside */
static void *tex_sprite(void) {
    unsigned char *t = alloc(16 * 16 * 2);
    int x, y;
    for (y = 0; y < 16; y++) {
        for (x = 0; x < 16; x++) {
            int dx = 2 * x - 15, dy = 2 * y - 15, r2 = dx * dx + dy * dy;
            unsigned int c = r2 > 225 ? rgba5551(0, 0, 0, 0) : r2 > 100 ? rgba5551(255, 255, 255, 1) : rgba5551(255, 128, 0, 1);
            be16(t + (y * 16 + x) * 2, c);
        }
    }
    return t;
}
/* Sprite_display: a 1-cycle TEXRECT at G_SETPRIMDEPTH, x, y, size in pixels, dsdx = 1024 / scale */
static void sprite_rect(int x, int y, int size, unsigned int z) {
    unsigned int d = (unsigned)(1024 * 16 / size);
    cmd(0xEE000000u, (z & 0x7FFFu) << 16);
    cmd(0xE4000000u | ((unsigned)((x + size) << 2) << 12) | (unsigned)((y + size - 1) << 2),
        ((unsigned)(x << 2) << 12) | (unsigned)(y << 2));
    cmd(0xB4000000u, 0);
    cmd(0xB3000000u, (d << 16) | d);
}

static void scene_sprite(void) { /* billboard sprites: TEXRECT + G_ZS_PRIM + G_SETPRIMDEPTH against a Z quad */
    void *t = tex_sprite();
    frame_begin(FILL2(rgba5551(0, 32, 64, 1)));
    sp_geom(G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH);
    dp_rendermode(RM_ZB_OPA);
    CC1(C_SHADE, A_ONE);
    draw_quad(quad(-60, -80, 60, 80, 0, 0, 0, kRgbw));  /* depth 0.5: prim depth ~0x3FFF */
    /* Sprite_ctor's "SS StartDL" (game_libs_ncc_4B620.cpp) */
    dp_pipesync();
    cmd(0xBA001402u, 0);                               /* 1-cycle */
    cmd(0xB9000201u, 4);                               /* Z source: G_ZS_PRIM */
    cmd(0xBA000C02u, 0x2000);                          /* bilerp */
    cmd(0xBA001301u, 0);                               /* perspective off */
    cmd(0xBB000001u, 0x80008000u);                     /* G_TEXTURE on */
    /* the sprite material: combiner T0 colour / T0 alpha, AA_ZB_TEX_EDGE (cycle 1 only) */
    CC1(C_T0, C_T0);
    dp_rendermode(0x443078u);
    load_block(t, FMT_RGBA, SIZ_16, 16, 16, 0, 0, 0);
    sprite_rect(70, 20, 64, 0x2000);                   /* near: over the quad's top-left corner */
    sprite_rect(130, 90, 64, 0x6000);                  /* far: hidden where the quad is */
    sprite_rect(210, 150, 48, 0x6000);                 /* far, off the quad: visible */
    sprite_rect(200, 30, 32, 0x2000);                  /* near, 2x magnified */
    /* "SS EndDL" */
    cmd(0xBA001301u, 0x80000u);
    cmd(0xB9000201u, 0);
    cmd(0xBB000000u, 0x80008000u);
}

static void scene_cull(void) { /* CULL_FRONT (the mirror pass) and G_CULLDL sub-lists (U64BGWall_ctor) */
    Vtx1 *v = vbuf(6), *off = vbuf(4), *on;
    G *a, *b;
    frame_begin(FILL2(rgba5551(0, 0, 0, 1)));
    dp_rendermode(RM_OPA);
    CC1(C_SHADE, A_ONE);
    vset(&v[0], -150, -40, 0, 0, 0, 0xFF0000FFu);       /* counter-clockwise on screen (front) */
    vset(&v[1], -10, -40, 0, 0, 0, 0xFF0000FFu);
    vset(&v[2], -80, 100, 0, 0, 0, 0xFF8000FFu);
    vset(&v[3], 10, -40, 0, 0, 0, 0x00FF00FFu);         /* clockwise on screen (back) */
    vset(&v[4], 80, 100, 0, 0, 0, 0x00FF80FFu);
    vset(&v[5], 150, -40, 0, 0, 0, 0x00FF00FFu);
    sp_geom(G_SHADE | G_SHADING_SMOOTH | G_CULL_FRONT); /* only the green (clockwise) one survives */
    sp_vtx(v, 0, 6);
    sp_tri(0, 1, 2);
    sp_tri(3, 4, 5);
    /* U64BGWall_ctor's per-tile form: G_VTX then G_CULLDL(0, 3) (F3DEX: w0 = vs * 2, w1 = ve * 2) in a list
     * called with G_DL; one tile on screen, one entirely off screen (the RSP ends that list) */
    sp_geom(G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK);
    on = quad(-150, -110, 150, -60, 0, 0, 0, kRgbw);
    vset(&off[0], 400, 300, 0, 0, 0, 0xFFFFFFFFu);
    vset(&off[1], 500, 300, 0, 0, 0, 0xFFFFFFFFu);
    vset(&off[2], 400, 200, 0, 0, 0, 0xFFFFFFFFu);
    vset(&off[3], 500, 200, 0, 0, 0, 0xFFFFFFFFu);
    sub_begin(16);
    sp_vtx(on, 0, 4);
    cmd(0xBE000000u, 6);
    sp_tri(0, 2, 1);
    sp_tri(1, 2, 3);
    a = sub_end();
    sub_begin(16);
    sp_vtx(off, 0, 4);
    cmd(0xBE000000u, 6);
    sp_tri(0, 2, 1);
    sp_tri(1, 2, 3);
    b = sub_end();
    cmd(0x06000000u, ga(a));
    cmd(0x06000000u, ga(b));
}

/* gfx_clearScreen per window: scissor, Z clear (CIMG = the Z buffer, 0xFFFC) and colour clear of the window's
 * rectangle only, then the window's viewport (ScreenWindow_display: vscale = (w * 2, h * 2, 0x1FF), vtrans =
 * (x * 4 + w * 2, y * 4 + h * 2, 0x1FF)) */
static void window_begin(int x, int y, int w, int h, unsigned int clear) {
    short *vp = alloc(16);
    vp[0] = (short)(w * 2); vp[1] = (short)(h * 2); vp[2] = 0x1FF; vp[3] = 0;
    vp[4] = (short)(x * 4 + w * 2); vp[5] = (short)(y * 4 + h * 2); vp[6] = 0x1FF; vp[7] = 0;
    dp_pipesync();
    dp_scissor(x, y, x + w, y + h);
    dp_cycle(CYC_FILL);
    dp_rendermode(0);
    cmd(0xFF100000u | 319, ga(sZimg));
    dp_fillcolor(0xFFFCFFFCu);
    dp_fillrect(x, y, x + w - 1, y + h - 1);
    dp_pipesync();
    cmd(0xFF100000u | 319, ga(sCimg));
    dp_fillcolor(clear);
    dp_fillrect(x, y, x + w - 1, y + h - 1);
    dp_pipesync();
    cmd(0x03800010u, ga(vp));
    dp_cycle(CYC_1);
    dp_rendermode(RM_ZB_OPA);
    CC1(C_SHADE, A_ONE);
    sp_geom(G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH);
}

static void scene_splitz(void) { /* split screen: per-window partial Z clears */
    static const unsigned int red[4] = { 0xFF0000FFu, 0xFF0000FFu, 0xC00000FFu, 0xC00000FFu };
    static const unsigned int yel[4] = { 0xFFFF00FFu, 0xFFFF00FFu, 0xC0C000FFu, 0xC0C000FFu };
    static const unsigned int blu[4] = { 0x4080FFFFu, 0x4080FFFFu, 0x4080FFFFu, 0x4080FFFFu };
    short *vpFull = alloc(16);
    frame_begin(FILL2(rgba5551(0, 0, 0, 1)));
    /* top window: a near red quad; bottom window: a near yellow quad. The proj maps the window to +-160 x +-120 */
    window_begin(0, 0, 320, 120, FILL2(rgba5551(40, 40, 40, 1)));
    draw_quad(quad(-100, -80, 0, 80, 500, 0, 0, red));
    window_begin(0, 120, 320, 120, FILL2(rgba5551(0, 40, 0, 1)));
    draw_quad(quad(0, -80, 100, 80, 500, 0, 0, yel));
    /* then the whole screen at a farther depth: hidden by both near quads only if the bottom window's Z clear left
     * the top window's Z alone */
    vpFull[0] = 640; vpFull[1] = 480; vpFull[2] = 511; vpFull[3] = 0;
    vpFull[4] = 640; vpFull[5] = 480; vpFull[6] = 511; vpFull[7] = 0;
    dp_pipesync();
    dp_scissor(0, 0, 320, 240);
    cmd(0x03800010u, ga(vpFull));
    draw_quad(quad(-120, -100, 120, 100, 0, 0, 0, blu));
}

/* Fader_display's list (bootup_uso_ncc_1304C.cpp), verbatim: 2-cycle, perspective off, XLU in both cycles,
 * T0 colour with T1 (I8 mask) alpha, bilerp, RGB dither off, TEXTLOD tile, a full-screen TEXRECT that magnifies
 * the 32 x 32 texture ~10x. Drawn over a shaded frame so the blend shows. */
static void *tex_fade0(void) { /* 32 x 32 RGBA16: soft stripes */
    unsigned char *t = alloc(32 * 32 * 2);
    int x, y;
    for (y = 0; y < 32; y++) {
        for (x = 0; x < 32; x++) {
            be16(t + (y * 32 + x) * 2, ((x + y) & 8) ? rgba5551(255, 255, 255, 1) : rgba5551(255, 40, 0, 1));
        }
    }
    return t;
}
static void *tex_fade1(void) { /* 32 x 32 I8 mask: a radial ramp, opaque at the rim */
    unsigned char *t = alloc(32 * 32);
    int x, y;
    for (y = 0; y < 32; y++) {
        for (x = 0; x < 32; x++) {
            int dx = 2 * x - 31, dy = 2 * y - 31, r2 = dx * dx + dy * dy;
            t[y * 32 + x] = (unsigned char)(r2 >= 961 ? 255 : r2 * 255 / 961);
        }
    }
    return t;
}
static void scene_fader(void) {
    void *t0 = tex_fade0(), *t1 = tex_fade1();
    frame_begin(FILL2(rgba5551(0, 0, 0, 1)));
    dp_rendermode(RM_OPA);
    sp_geom(G_SHADE | G_SHADING_SMOOTH);
    CC1(C_SHADE, A_ONE);
    draw_quad(quad(-160, -120, 160, 120, 0, 0, 0, kRgbw));
    cmd(0xE7000000u, 0);
    cmd(0xBA001402u, 0x100000u);
    cmd(0xBA001301u, 0);
    cmd(0xB900031Du, 0x504240u);
    cmd(0xFCFFFFFFu, 0xFFFCF438u);
    cmd(0xBA000C02u, 0x2000u);
    cmd(0xBA000602u, 0xC0u);
    cmd(0xF8000000u, 0xFFFFFFFFu);
    cmd(0xFD100000u, ga(t0));
    cmd(0xF5100000u, 0x07014050u);
    cmd(0xE6000000u, 0);
    cmd(0xF3000000u, 0x073FF100u);
    cmd(0xE7000000u, 0);
    cmd(0xF5101000u, 0x00014050u);
    cmd(0xF2000000u, 0x0007C07Cu);
    cmd(0xFD900000u, ga(t1));
    cmd(0xF5900100u, 0x07014050u);
    cmd(0xE6000000u, 0);
    cmd(0xF3000000u, 0x071FF200u);
    cmd(0xE7000000u, 0);
    cmd(0xF5880900u, 0x01014050u);
    cmd(0xF2000000u, 0x0107C07Cu);
    cmd(0xBA001001u, 0);
    cmd(0xE45003C0u, 0);
    cmd(0xB4000000u, 0);
    cmd(0xB3000000u, 0x00630084u);
    cmd(0xE7000000u, 0);
    cmd(0xBA001301u, 0x80000u);
}

typedef struct {
    const char *name;
    void (*build)(void);
} Scene;

static const Scene kScenes[] = {
    { "fill", scene_fill },         { "shade", scene_shade },     { "tex_rgba16", scene_tex_rgba16 },
    { "tex_ci", scene_tex_ci },     { "tex_ia_i", scene_tex_ia_i }, { "combine", scene_combine },
    { "multitex", scene_multitex }, { "fog", scene_fog },         { "texrect", scene_texrect },
    { "xlu", scene_xlu },           { "line3d", scene_line3d },
    /* run 828 */
    { "light", scene_light },       { "texgen", scene_texgen },   { "loadtile", scene_loadtile },
    { "sprite", scene_sprite },     { "cull", scene_cull },       { "splitz", scene_splitz },
    { "fader", scene_fader },
};

int main(int argc, char **argv) {
    size_t i;
    if (argc < 2) {
        fprintf(stderr, "usage: gold OUTDIR | --list\n");
        return 2;
    }
    for (i = 0; i < sizeof(kScenes) / sizeof(kScenes[0]); i++) {
        DlumpBuf buf;
        DlumpOptions opt;
        unsigned int *task, root, rootSize;
        char path[1024];
        FILE *f;
        int errs;
        if (strcmp(argv[1], "--list") == 0) {
            printf("%s\n", kScenes[i].name);
            continue;
        }
        memset(sArena, 0, sizeof(sArena));
        sTop = 0;
        sCimg = alloc(320 * 240 * 2);
        sZimg = alloc(320 * 240 * 2);
        kScenes[i].build();
        root = frame_end();
        rootSize = (unsigned int)(sN * 8);
        task = alloc(64);
        task[0] = 1;
        task[12] = root;
        task[13] = rootSize;
        sTaskAddr = ga(task);
        memset(&buf, 0, sizeof(buf));
        memset(&opt, 0, sizeof(opt));
        opt.arenaBase = (size_t)sArena - ARENA_ADDR;
        errs = dlump_task(&buf, &opt, (unsigned int)i + 1, sTaskAddr, 1, root, rootSize);
        if (errs != 0) {
            fprintf(stderr, "gold: %s: %d dump errors\n", kScenes[i].name, errs);
            return 1;
        }
        snprintf(path, sizeof(path), "%s/%s.dlmp", argv[1], kScenes[i].name);
        f = fopen(path, "wb");
        if (f == NULL || fwrite(buf.data, 1, buf.size, f) != buf.size) {
            fprintf(stderr, "gold: cannot write %s\n", path);
            return 1;
        }
        fclose(f);
        dlump_buf_free(&buf);
    }
    return 0;
}
