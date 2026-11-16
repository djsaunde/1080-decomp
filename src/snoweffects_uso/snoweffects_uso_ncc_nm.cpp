/* IDO 7.1 NCC (C++) NON_MATCHING donors for snoweffects.uso (agent-h run 649), spliced over the empty
 * NON_MATCHING placeholders in snoweffects_uso.c on the non_matching build only (the matching build keeps
 * INCLUDE_ASM). The four texture-sprite inits 0x15D0 / 0x1A18 / 0x1E60 / 0x2310: set the object's name,
 * create + reset a 20-packet display-list context (+0x2C), then record PipeSync, the stock 2.0I
 * gDPLoadTextureTile of an 8-bit Data texture (w = h set just before), gSPTexture, cycle / persp / render
 * mode / combine / prim colour / depth source and EndDL. Each packet is `ctx->next()`, the unnamed-index
 * append (`&dl->buf[dl->count++]`). gfx_resetDLContext must be a void-typed alias, or its v0 result
 * blocks v0 for the context temp. Left (all register colouring, 80-92 % lines): the per-packet pointers
 * rotate a1 / a2 where the target rotates a2 / a3 (live-unit layout), and 0x1E60 leaves the jr ra delay
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
struct GCtx { char pad0[0xC]; DList *dl; Gfx *next() { return &dl->buf[dl->count++]; } };
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
