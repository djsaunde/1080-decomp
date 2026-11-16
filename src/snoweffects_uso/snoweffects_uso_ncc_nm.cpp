/* IDO 7.1 NCC (C++) NON_MATCHING donors for snoweffects.uso (agent-h run 649), spliced over the empty
 * NON_MATCHING placeholders in snoweffects_uso.c on the non_matching build only (the matching build keeps
 * INCLUDE_ASM). The four texture-sprite inits 0x15D0 / 0x1A18 / 0x1E60 / 0x2310: set the object's name,
 * create + reset a 20-packet display-list context (+0x2C), then record PipeSync, the stock 2.0I
 * gDPLoadTextureTile of an 8-bit Data texture (w = h set just before), gSPTexture, cycle / persp / render
 * mode / combine / prim colour / depth source and EndDL. Each packet is `ctx->next()` with a named index and
 * result (`int n = dl->count++; Gfx *g = &dl->buf[n]; return g;`: packets 1-2 and the w / h constant then
 * colour like the target, 96 -> 66 diff lines on 0x15D0; run 660). gfx_resetDLContext must be a void-typed
 * alias, or its v0 result blocks v0 for the context temp. Left (all register colouring): from packet 3 on
 * the target's pointers skip a1 (a2 / a3 rotation, live-unit layout), and 0x1E60 leaves the jr ra delay
 * slot unfilled (301 vs 300 words). */
#include "../game_libs/ncc_drafts/gbi_subset.h"
extern "C" int gl_func_00000000(...);
extern "C" char D_00000000[];
/* the textures (Data+0x6FD8 / 0x6BD0 / 0x5AC0 / 0x73E0; USO reloc carries the offset, lo16 0) */
extern "C" char snow_tex_6FD8[], snow_tex_6BD0[], snow_tex_5AC0[], snow_tex_73E0[];
extern "C" float snow_ro_8;  /* RoData+0x8 */
struct GCtx;
extern "C" void gl_func_00000000_snow_reset(GCtx *);  /* gfx_resetDLContext, void-typed blank jal */
#define gDPLoadTextureTile(pkt, timg, fmt, siz, width, height, uls, ult, lrs, lrt, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ gDPSetTextureImage(pkt, fmt, siz, width, timg); \
  gDPSetTile(pkt, fmt, siz, (((((lrs)-(uls)+1) * siz##_TILE_BYTES)+7)>>3), 0, G_TX_LOADTILE, 0, cmt, maskt, shiftt, cms, masks, shifts); \
  gDPLoadSync(pkt); \
  gDPLoadTile(pkt, G_TX_LOADTILE, (uls)<<G_TEXTURE_IMAGE_FRAC, (ult)<<G_TEXTURE_IMAGE_FRAC, (lrs)<<G_TEXTURE_IMAGE_FRAC, (lrt)<<G_TEXTURE_IMAGE_FRAC); \
  gDPPipeSync(pkt); \
  gDPSetTile(pkt, fmt, siz, (((((lrs)-(uls)+1) * siz##_LINE_BYTES)+7)>>3), 0, G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks, shifts); \
  gDPSetTileSize(pkt, G_TX_RENDERTILE, (uls)<<G_TEXTURE_IMAGE_FRAC, (ult)<<G_TEXTURE_IMAGE_FRAC, (lrs)<<G_TEXTURE_IMAGE_FRAC, (lrt)<<G_TEXTURE_IMAGE_FRAC) }
#define gDPWord(pkt, a, b) { Gfx *_g = (Gfx *)(pkt); _g->words.w0 = (a); _g->words.w1 = (unsigned int)(b); }
typedef struct { Gfx *buf; int count; } DList;
struct GCtx { char pad0[0xC]; DList *dl; Gfx *next() { int n = dl->count++; Gfx *g = &dl->buf[n]; return g; } };
struct SnowTex {
    char pad00[0xC];
    char *name;
    char pad10[0x2C - 0x10];
    GCtx *dl;
    int pad30;
    int w;
    int h;
    char pad3C[0x98 - 0x3C];
    float f98, f9C, fA0, fA4, fA8;
    int pAC, pB0;
    int iB4, iB8, iBC;
    float fC0, fC4, fC8, fCC;
};


extern "C" void snoweffects_uso_func_000015D0(SnowTex *t) {
    t->name = D_00000000 + 0x2C8;
    t->dl = (GCtx *)gl_func_00000000(0, D_00000000 + 0x2E0, 20, 0, 0, 0, 0);
    gl_func_00000000_snow_reset(t->dl);
    gDPPipeSync(t->dl->next());
    t->w = 32;
    t->h = 32;
    gDPLoadTextureTile(t->dl->next(), snow_tex_6FD8, G_IM_FMT_I, G_IM_SIZ_8b, t->w, t->h, 0, 0, t->w - 1, t->h - 1, 0, 0, 0, 5, 5, 0, 0);
    gDPWord(t->dl->next(), 0xBB000001, 0x80008000);
    gDPWord(t->dl->next(), 0xBA001402, 0);
    gDPWord(t->dl->next(), 0xBA001301, 0);
    gDPWord(t->dl->next(), 0xB900031D, 0x00404A50);
    gDPWord(t->dl->next(), 0xFCFF9BFF, 0xFF36FF7F);
    gDPWord(t->dl->next(), 0xFA000000, 0);
    gDPWord(t->dl->next(), 0xB9000201, 4);
    gDPWord(t->dl->next(), 0xB8000000, 0);
}

extern "C" void snoweffects_uso_func_00001A18(SnowTex *t) {
    t->name = D_00000000 + 0x2EC;
    t->dl = (GCtx *)gl_func_00000000(0, D_00000000 + 0x304, 20, 0, 0, 0, 0);
    gl_func_00000000_snow_reset(t->dl);
    gDPPipeSync(t->dl->next());
    t->w = 32;
    t->h = 32;
    gDPLoadTextureTile(t->dl->next(), snow_tex_6BD0, G_IM_FMT_I, G_IM_SIZ_8b, t->w, t->h, 0, 0, t->w - 1, t->h - 1, 0, 0, 0, 5, 5, 0, 0);
    gDPWord(t->dl->next(), 0xBB000001, 0x80008000);
    gDPWord(t->dl->next(), 0xBA001402, 0);
    gDPWord(t->dl->next(), 0xBA001301, 0);
    gDPWord(t->dl->next(), 0xB900031D, 0x00404A50);
    gDPWord(t->dl->next(), 0xFCFF9BFF, 0xFF36FF7F);
    gDPWord(t->dl->next(), 0xFA000000, 0);
    gDPWord(t->dl->next(), 0xB9000201, 4);
    gDPWord(t->dl->next(), 0xB8000000, 0);
}

/* DRAFT, not spliced: 301 words vs 300 (the target fills the jr ra delay slot with the sp restore). */
extern "C" void snoweffects_uso_func_00001E60(SnowTex *t) {
    t->name = D_00000000 + 0x310;
    t->dl = (GCtx *)gl_func_00000000(0, D_00000000 + 0x328, 20, 0, 0, 0, 0);
    gl_func_00000000_snow_reset(t->dl);
    gDPPipeSync(t->dl->next());
    t->w = 64;
    t->h = 64;
    gDPLoadTextureTile(t->dl->next(), snow_tex_5AC0, G_IM_FMT_IA, G_IM_SIZ_8b, t->w, t->h, 0, 0, t->w - 1, t->h - 1, 0, 0, 0, 6, 6, 0, 0);
    gDPWord(t->dl->next(), 0xBB000001, 0x80008000);
    gDPWord(t->dl->next(), 0xBA001402, 0);
    gDPWord(t->dl->next(), 0xBA001301, 0);
    gDPWord(t->dl->next(), 0xBA000C02, 0);
    gDPWord(t->dl->next(), 0xBA000602, 0xC0);
    gDPWord(t->dl->next(), 0xB900031D, 0x00404A50);
    gDPWord(t->dl->next(), 0xFCFF9BFF, 0xFF377FBF);
    gDPWord(t->dl->next(), 0xFA000000, 0);
    gDPWord(t->dl->next(), 0xB9000201, 4);
    gDPWord(t->dl->next(), 0xB8000000, 0);
}

extern "C" void snoweffects_uso_func_00002310(SnowTex *t) {
    t->name = D_00000000 + 0x334;
    t->iB4 = 0; t->iB8 = 0; t->iBC = 0;
    t->fA8 = 1.0f;
    t->fC0 = 1.0f; t->fC4 = 1.0f; t->fC8 = 1.0f;
    t->fCC = 0.5f;
    t->fA4 = snow_ro_8;
    t->f98 = 0.0f; t->f9C = 0.0f; t->fA0 = 0.0f;
    t->dl = (GCtx *)gl_func_00000000(0, D_00000000 + 0x340, 20, 0, 0, 0, 0);
    gl_func_00000000_snow_reset(t->dl);
    gDPPipeSync(t->dl->next());
    t->w = 8;
    t->h = 8;
    gDPLoadTextureTile(t->dl->next(), snow_tex_73E0, G_IM_FMT_I, G_IM_SIZ_8b, t->w, t->h, 0, 0, t->w - 1, t->h - 1, 0, 0, 0, 5, 5, 0, 0);
    gDPWord(t->dl->next(), 0xBB000001, 0x80008000);
    gDPWord(t->dl->next(), 0xBA001402, 0);
    gDPWord(t->dl->next(), 0xBA001301, 0);
    gDPWord(t->dl->next(), 0xB900031D, 0x00404A50);
    gDPWord(t->dl->next(), 0xFCFFB3FF, 0xFF65FEFF);
    gDPWord(t->dl->next(), 0xFA000000, 0x80);
    gDPWord(t->dl->next(), 0xB9000201, 4);
    gDPWord(t->dl->next(), 0xB8000000, 0);
}

/* 0x4374 (0xD2 words, NM ~90 % lines): rebuild the emitter's basis from its world matrix (+0x30): the two
 * end points (+-len, 0, 0) and the origin go through the matrix (inline point transform); dir = a - b
 * (operator- through set(), by-value operator=), normalized, kept as fwd (+0xFC), then crossed with the
 * camera's +0x3B0 vector in place; +0x108 = camera +0x318, normalized; then virtual slots 14 and 15.
 * Frame 160 as the target (run 666): no named end-point pointers, the transform's results declared z, y, x,
 * and one dead scratch word in operator- (agent-e run 659 lever). Left: the operator- result address
 * (sp+0x74) is scheduled up into a1 at entry; the target forms it in v0 right before the copy. */
struct SnowBV3 {
    float x, y, z;
    SnowBV3() {}
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    SnowBV3 &operator=(SnowBV3 o) { x = o.x; y = o.y; z = o.z; return *this; }
    SnowBV3 &operator+=(SnowBV3 o) { x += o.x; y += o.y; z += o.z; return *this; }
};
inline SnowBV3 operator-(const SnowBV3 &a, const SnowBV3 &b) {
    SnowBV3 r;
    r.set(a.x - b.x, a.y - b.y, a.z - b.z);
    { int sc[1]; sc[0] = 0; }  /* dead scratch word: puts the operator= param at the target's sp+60 */
    return r;
}
inline SnowBV3 operator*(const SnowBV3 &a, float s) { SnowBV3 r; r.set(a.x * s, a.y * s, a.z * s); return r; }
inline float dot(const SnowBV3 &a, const SnowBV3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline void xf(SnowBV3 *v, float (*m)[4]) {
    float nz, ny, nx;
    nx = m[0][0] * v->x + m[1][0] * v->y + m[2][0] * v->z + m[3][0];
    ny = m[0][1] * v->x + m[1][1] * v->y + m[2][1] * v->z + m[3][1];
    nz = m[0][2] * v->x + m[1][2] * v->y + m[2][2] * v->z + m[3][2];
    v->x = nx; v->y = ny; v->z = nz;
}
struct SnowVEnt { short d; short pad; void (*fn)(void *); };
struct SnowBCam {
    char pad[0x318];
    SnowBV3 v318;
    char pad2[0x3B0 - 0x324];
    SnowBV3 up;      /* 3B0 */
    char pad3BC[0x3C8 - 0x3BC];
    SnowBV3 v3C8;
    char pad3D4[0xA58 - 0x3D4];
    int flags;       /* A58 (bit 0x80 mirrors the 0x4E5C angle) */
};
struct SnowQuad { int a, b, c, d; };
struct SnowBasis {
    char pad00[0x28];
    SnowVEnt *vptr;
    SnowBCam *cam;
    float (*m)[4];
    char pad34[0x3C - 0x34];
    float len;
    float u40;
    char pad44[0x50 - 0x44];
    float u50;
    char pad54[0xB4 - 0x54];
    SnowQuad uB4;    /* B4: 0x52D4 out-param of the p probe */
    SnowQuad uC4;    /* C4: 0x52D4 out-param of the q probe */
    SnowQuad uD4;    /* D4: copy of uC4 */
    SnowBV3 a;       /* E4 */
    SnowBV3 b;       /* F0 */
    SnowBV3 fwd;     /* FC */
    SnowBV3 v108;    /* 108 */
    SnowBV3 dir;     /* 114 */
    SnowBV3 p;       /* 120 */
    SnowBV3 q;       /* 12C */
    SnowBV3 c;       /* 138 */
    int hits;        /* 144 */
    float cosA;      /* 148 */
    float d1;        /* 14C */
    float d2;        /* 150 */
    char pad154[0x15C - 0x154];
    float maxDist;   /* 15C */
};
extern "C" void snoweffects_uso_func_00004374(SnowBasis *t) {
    t->a.x = t->len; t->a.y = 0.0f; t->a.z = 0.0f;
    xf(&t->a, t->m);
    t->b.x = -t->len; t->b.z = 0.0f; t->b.y = 0.0f;
    xf(&t->b, t->m);
    t->dir = t->a - t->b;
    gl_func_00000000(&t->dir);  /* math_vec3Normalize */
    t->fwd = t->dir;
    gl_func_00000000(&t->dir, &t->cam->up, &t->dir);  /* math_vec3Cross */
    t->v108 = t->cam->v318;
    gl_func_00000000(&t->v108);  /* math_vec3Normalize */
    t->c.x = 0.0f; t->c.y = 0.0f; t->c.z = 0.0f;
    xf(&t->c, t->m);
    t->vptr[14].fn((void *)((int)t->vptr[14].d + (int)t));
    t->vptr[15].fn((void *)((int)t->vptr[15].d + (int)t));
}
