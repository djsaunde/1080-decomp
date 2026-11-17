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
 * slot unfilled (301 vs 300 words).
 * Run 671: the X* gbi macros take a per-packet hook; N1 (`_g = (Gfx *)((int)_g * 1);` on the packet pointer
 * inside the macro) at packet sites 3, 9, 11, 12 (PipeSync = 0, tile sub-packets 1-7, then the words) moves
 * the packet-pointer colouring closer: 15D0 66 -> 42, 1A18 70 -> 46, 2310 72 -> 48 diff lines.
 * Run 676: three zero-code counted loads (`(*(int *)((char *)t + K) & 0)`, distinct K) in the prim colour (FA)
 * packet move the later live-unit boundaries: 42 -> 36, 46 -> 40, 48 -> 42.
 * Run 691 (agent-c): 0x15D0 / 0x1A18 / 0x2310 EXACT, spliced in both build trees. Every packet goes through
 * two `(this + 0)` hops (hop / hopI), one `if (1) {}` cut after the SetTImg packet and ten SNOW_PH zero-code
 * counted loads; together they put uopt's 20-load unit cuts inside the hop chains of packets 4 / 6 / 8 / 10,
 * so the hop temp holds a1 from packet 2 to packet 12 (the target's a2 / a3 rotation), and the last cut after
 * the dl use of the EndDL packet (v1). The N1 hooks are no longer used. 0x1A18's texture is IA8.
 * Run 704 (agent-c): 0x1E60 EXACT the same way (cut sites of its own, see the function). */
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
struct GCtx {
    char pad0[0xC];
    DList *dl;
    Gfx *next() { int n = dl->count++; Gfx *g = &dl->buf[n]; return g; }
    Gfx *nextI() { int n = dl->count++; return &dl->buf[n]; }
    /* two (this + 0) hops (agent-b's 0x559C lever): each hop's `this` is a pooled edg temp. Inside a live
     * unit the copy is forwarded away, but where a uopt unit cut falls inside the hop chain the temp lives
     * across the cut, so one pool word is live from P2 to P12 (refs only at the cuts) and takes a1 ahead
     * of the packets: the target's a2 / a3 rotation. */
    Gfx *hop1() { return (this + 0)->next(); }
    Gfx *hop() { return (this + 0)->hop1(); }
    Gfx *hop1I() { return (this + 0)->nextI(); }
    Gfx *hopI() { return (this + 0)->hop1I(); }
};
/* a zero-code counted load (uopt's 20-load live-unit counter counts it; masked away at emission) */
#define SNOW_PH(p, off) (*(int *)((char *)(p) + (off)) & 0)
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


#define N0(g)
#define N1(g) g = (Gfx *)((int)g * 1);
#define XgSetImage(m, pkt, cmd, fmt, siz, width, i) \
{ Gfx *_g = (Gfx *)(pkt); m(_g) \
  _g->words.w0 = _SHIFTL(cmd, 24, 8) | _SHIFTL(fmt, 21, 3) | _SHIFTL(siz, 19, 2) | _SHIFTL((width)-1, 0, 12); \
  _g->words.w1 = (unsigned int)(i); }
#define XgDPNoParam(m, pkt, cmd) \
{ Gfx *_g = (Gfx *)(pkt); m(_g) _g->words.w0 = _SHIFTL(cmd, 24, 8); _g->words.w1 = 0; }
#define XgDPLoadTileGeneric(m, pkt, c, tile, uls, ult, lrs, lrt) \
{ Gfx *_g = (Gfx *)(pkt); m(_g) \
  _g->words.w0 = _SHIFTL(c, 24, 8) | _SHIFTL(uls, 12, 12) | _SHIFTL(ult, 0, 12); \
  _g->words.w1 = _SHIFTL(tile, 24, 3) | _SHIFTL(lrs, 12, 12) | _SHIFTL(lrt, 0, 12); }
#define XgDPSetTile(m, pkt, fmt, siz, line, tmem, tile, palette, cmt, maskt, shiftt, cms, masks, shifts) \
{ Gfx *_g = (Gfx *)(pkt); m(_g) \
  _g->words.w0 = _SHIFTL(G_SETTILE, 24, 8) | _SHIFTL(fmt, 21, 3) | _SHIFTL(siz, 19, 2) | _SHIFTL(line, 9, 9) | _SHIFTL(tmem, 0, 9); \
  _g->words.w1 = _SHIFTL(tile, 24, 3) | _SHIFTL(palette, 20, 4) | _SHIFTL(cmt, 18, 2) | _SHIFTL(maskt, 14, 4) | \
                 _SHIFTL(shiftt, 10, 4) | _SHIFTL(cms, 8, 2) | _SHIFTL(masks, 4, 4) | _SHIFTL(shifts, 0, 4); }
#define XgDPWord(m, pkt, a, b) { Gfx *_g = (Gfx *)(pkt); m(_g) _g->words.w0 = (a); _g->words.w1 = (unsigned int)(b); }
#define XgDPLoadTextureTile7(m1, m2, m3, m4, m5, m6, m7, pkt, timg, fmt, siz, width, height, uls, ult, lrs, lrt, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ XgSetImage(m1, pkt, G_SETTIMG, fmt, siz, width, timg); \
  XgDPSetTile(m2, pkt, fmt, siz, (((((lrs)-(uls)+1) * siz##_TILE_BYTES)+7)>>3), 0, G_TX_LOADTILE, 0, cmt, maskt, shiftt, cms, masks, shifts); \
  XgDPNoParam(m3, pkt, G_RDPLOADSYNC); \
  XgDPLoadTileGeneric(m4, pkt, G_LOADTILE, G_TX_LOADTILE, (uls)<<G_TEXTURE_IMAGE_FRAC, (ult)<<G_TEXTURE_IMAGE_FRAC, (lrs)<<G_TEXTURE_IMAGE_FRAC, (lrt)<<G_TEXTURE_IMAGE_FRAC); \
  XgDPNoParam(m5, pkt, G_RDPPIPESYNC); \
  XgDPSetTile(m6, pkt, fmt, siz, (((((lrs)-(uls)+1) * siz##_LINE_BYTES)+7)>>3), 0, G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks, shifts); \
  XgDPLoadTileGeneric(m7, pkt, G_SETTILESIZE, G_TX_RENDERTILE, (uls)<<G_TEXTURE_IMAGE_FRAC, (ult)<<G_TEXTURE_IMAGE_FRAC, (lrs)<<G_TEXTURE_IMAGE_FRAC, (lrt)<<G_TEXTURE_IMAGE_FRAC) }
extern "C" void PowderParticles_buildTexDL_000015D0(SnowTex *t) {
    t->name = D_00000000 + 0x2C8;
    t->dl = (GCtx *)gl_func_00000000(0, D_00000000 + 0x2E0, 20, 0, 0, 0, 0);
    gl_func_00000000_snow_reset(t->dl);
    XgDPNoParam(N0, t->dl->hop(), G_RDPPIPESYNC);
    t->w = 32;
    t->h = 32;
    XgSetImage(N0, t->dl->hop(), G_SETTIMG, G_IM_FMT_I, G_IM_SIZ_8b, t->w, snow_tex_6FD8);
    if (1) {}
    XgDPSetTile(N0, t->dl->hop(), G_IM_FMT_I, G_IM_SIZ_8b, ((((t->w - 1 - 0 + 1) * G_IM_SIZ_8b_TILE_BYTES) + 7) >> 3), 0, G_TX_LOADTILE, 0, 0, 5, 0, 0, 5, 0);
    XgDPNoParam(N0, t->dl->hop(), G_RDPLOADSYNC);
    XgDPLoadTileGeneric(N0, t->dl->hopI(), G_LOADTILE + SNOW_PH(t, 0x1A0), G_TX_LOADTILE, 0, 0, (t->w - 1) << G_TEXTURE_IMAGE_FRAC, (t->h - 1) << G_TEXTURE_IMAGE_FRAC);
    XgDPNoParam(N0, t->dl->hop(), G_RDPPIPESYNC + SNOW_PH(t, 0x1C8));
    XgDPSetTile(N0, t->dl->hop(), G_IM_FMT_I, G_IM_SIZ_8b, ((((t->w - 1 - 0 + 1) * G_IM_SIZ_8b_LINE_BYTES) + 7) >> 3), 0, G_TX_RENDERTILE, 0, 0, 5, 0, 0, 5, 0);
    XgDPLoadTileGeneric(N0, t->dl->hop(), G_SETTILESIZE + SNOW_PH(t, 0x218), G_TX_RENDERTILE, 0, 0, (t->w - 1) << G_TEXTURE_IMAGE_FRAC, (t->h - 1) << G_TEXTURE_IMAGE_FRAC);
    XgDPWord(N0, t->dl->hop(), 0xBB000001 + SNOW_PH(t, 0x240), 0x80008000);
    XgDPWord(N0, t->dl->hop(), 0xBA001402 + SNOW_PH(t, 0x268) + SNOW_PH(t, 0x26C) + SNOW_PH(t, 0x270), 0);
    XgDPWord(N0, t->dl->hop(), 0xBA001301, 0);
    XgDPWord(N0, t->dl->hop(), 0xB900031D, 0x00404A50);
    XgDPWord(N0, t->dl->hop(), 0xFCFF9BFF + SNOW_PH(t, 0x2E0) + SNOW_PH(t, 0x2E4) + SNOW_PH(t, 0x2E8), 0xFF36FF7F);
    XgDPWord(N0, t->dl->hop(), 0xFA000000, 0);
    XgDPWord(N0, t->dl->hop(), 0xB9000201, 4);
    XgDPWord(N0, t->dl->hop(), 0xB8000000, 0);
}

extern "C" void NormalParticles_buildTexDL_00001A18(SnowTex *t) {
    t->name = D_00000000 + 0x2EC;
    t->dl = (GCtx *)gl_func_00000000(0, D_00000000 + 0x304, 20, 0, 0, 0, 0);
    gl_func_00000000_snow_reset(t->dl);
    XgDPNoParam(N0, t->dl->hop(), G_RDPPIPESYNC);
    t->w = 32;
    t->h = 32;
    XgSetImage(N0, t->dl->hop(), G_SETTIMG, G_IM_FMT_IA, G_IM_SIZ_8b, t->w, snow_tex_6BD0);
    if (1) {}
    XgDPSetTile(N0, t->dl->hop(), G_IM_FMT_IA, G_IM_SIZ_8b, ((((t->w - 1 - 0 + 1) * G_IM_SIZ_8b_TILE_BYTES) + 7) >> 3), 0, G_TX_LOADTILE, 0, 0, 5, 0, 0, 5, 0);
    XgDPNoParam(N0, t->dl->hop(), G_RDPLOADSYNC);
    XgDPLoadTileGeneric(N0, t->dl->hopI(), G_LOADTILE + SNOW_PH(t, 0x1A0), G_TX_LOADTILE, 0, 0, (t->w - 1) << G_TEXTURE_IMAGE_FRAC, (t->h - 1) << G_TEXTURE_IMAGE_FRAC);
    XgDPNoParam(N0, t->dl->hop(), G_RDPPIPESYNC + SNOW_PH(t, 0x1C8));
    XgDPSetTile(N0, t->dl->hop(), G_IM_FMT_IA, G_IM_SIZ_8b, ((((t->w - 1 - 0 + 1) * G_IM_SIZ_8b_LINE_BYTES) + 7) >> 3), 0, G_TX_RENDERTILE, 0, 0, 5, 0, 0, 5, 0);
    XgDPLoadTileGeneric(N0, t->dl->hop(), G_SETTILESIZE + SNOW_PH(t, 0x218), G_TX_RENDERTILE, 0, 0, (t->w - 1) << G_TEXTURE_IMAGE_FRAC, (t->h - 1) << G_TEXTURE_IMAGE_FRAC);
    XgDPWord(N0, t->dl->hop(), 0xBB000001 + SNOW_PH(t, 0x240), 0x80008000);
    XgDPWord(N0, t->dl->hop(), 0xBA001402 + SNOW_PH(t, 0x268) + SNOW_PH(t, 0x26C) + SNOW_PH(t, 0x270), 0);
    XgDPWord(N0, t->dl->hop(), 0xBA001301, 0);
    XgDPWord(N0, t->dl->hop(), 0xB900031D, 0x00404A50);
    XgDPWord(N0, t->dl->hop(), 0xFCFF9BFF + SNOW_PH(t, 0x2E0) + SNOW_PH(t, 0x2E4) + SNOW_PH(t, 0x2E8), 0xFF36FF7F);
    XgDPWord(N0, t->dl->hop(), 0xFA000000, 0);
    XgDPWord(N0, t->dl->hop(), 0xB9000201, 4);
    XgDPWord(N0, t->dl->hop(), 0xB8000000, 0);
}

/* 0x1E60 (64 x 64 IA8, Data+0x5AC0; agent-c run 704): the same hop-everywhere form. Its two extra packets
 * (TextFilt / RGBDither) move uopt's 20-load unit cuts, so the cut sites differ from 0x15D0: `if (1) {}` after
 * the SetTImg and after the second PipeSync, hopI on the render SetTile, and 6 SNOW_PH loads (SetTileSize,
 * the 0xBA001402 word, the combine word). The hop temps that straddle a cut colour the target's hidden t0, so
 * E700 takes t1 and F568 a3; the 0xBA000602 / EndDL packets straddle a cut (a3). No dead switch is needed
 * for the jr ra delay slot in this form. */
extern "C" void SpecParticles_buildTexDL_00001E60(SnowTex *t) {
    t->name = D_00000000 + 0x310;
    t->dl = (GCtx *)gl_func_00000000(0, D_00000000 + 0x328, 20, 0, 0, 0, 0);
    gl_func_00000000_snow_reset(t->dl);
    XgDPNoParam(N0, t->dl->hop(), G_RDPPIPESYNC);
    t->w = 64;
    t->h = 64;
    XgSetImage(N0, t->dl->hop(), G_SETTIMG, G_IM_FMT_IA, G_IM_SIZ_8b, t->w, snow_tex_5AC0);
    if (1) {}
    XgDPSetTile(N0, t->dl->hop(), G_IM_FMT_IA, G_IM_SIZ_8b, ((((t->w - 1 - 0 + 1) * G_IM_SIZ_8b_TILE_BYTES) + 7) >> 3), 0, G_TX_LOADTILE, 0, 0, 6, 0, 0, 6, 0);
    XgDPNoParam(N0, t->dl->hop(), G_RDPLOADSYNC);
    XgDPLoadTileGeneric(N0, t->dl->hop(), G_LOADTILE, G_TX_LOADTILE, 0, 0, (t->w - 1) << G_TEXTURE_IMAGE_FRAC, (t->h - 1) << G_TEXTURE_IMAGE_FRAC);
    XgDPNoParam(N0, t->dl->hop(), G_RDPPIPESYNC);
    if (1) {}
    XgDPSetTile(N0, t->dl->hopI(), G_IM_FMT_IA, G_IM_SIZ_8b, ((((t->w - 1 - 0 + 1) * G_IM_SIZ_8b_LINE_BYTES) + 7) >> 3), 0, G_TX_RENDERTILE, 0, 0, 6, 0, 0, 6, 0);
    XgDPLoadTileGeneric(N0, t->dl->hop(), G_SETTILESIZE + SNOW_PH(t, 0x250), G_TX_RENDERTILE, 0, 0, (t->w - 1) << G_TEXTURE_IMAGE_FRAC, (t->h - 1) << G_TEXTURE_IMAGE_FRAC);
    XgDPWord(N0, t->dl->hop(), 0xBB000001, 0x80008000);
    XgDPWord(N0, t->dl->hop(), 0xBA001402 + SNOW_PH(t, 0x2B0) + SNOW_PH(t, 0x2B4), 0);
    XgDPWord(N0, t->dl->hop(), 0xBA001301, 0);
    XgDPWord(N0, t->dl->hop(), 0xBA000C02, 0);
    XgDPWord(N0, t->dl->hop(), 0xBA000602, 0xC0);
    XgDPWord(N0, t->dl->hop(), 0xB900031D, 0x00404A50);
    XgDPWord(N0, t->dl->hop(), 0xFCFF9BFF + SNOW_PH(t, 0x3A0) + SNOW_PH(t, 0x3A4) + SNOW_PH(t, 0x3A8), 0xFF377FBF);
    XgDPWord(N0, t->dl->hop(), 0xFA000000, 0);
    XgDPWord(N0, t->dl->hop(), 0xB9000201, 4);
    XgDPWord(N0, t->dl->hop(), 0xB8000000, 0);
}

extern "C" void SnowFall_buildTexDL_00002310(SnowTex *t) {
    t->name = D_00000000 + 0x334;
    t->iB4 = 0;
    t->iB8 = 0;
    t->iBC = 0;
    t->fA8 = 1.0f;
    t->fC0 = 1.0f; t->fC4 = 1.0f; t->fC8 = 1.0f;
    t->fCC = 0.5f;
    t->fA4 = snow_ro_8;
    t->f98 = 0.0f; t->f9C = 0.0f; t->fA0 = 0.0f;
    t->dl = (GCtx *)gl_func_00000000(0, D_00000000 + 0x340, 20, 0, 0, 0, 0);
    gl_func_00000000_snow_reset(t->dl);
    XgDPNoParam(N0, t->dl->hop(), G_RDPPIPESYNC);
    t->w = 8;
    t->h = 8;
    XgSetImage(N0, t->dl->hop(), G_SETTIMG, G_IM_FMT_I, G_IM_SIZ_8b, t->w, snow_tex_73E0);
    if (1) {}
    XgDPSetTile(N0, t->dl->hop(), G_IM_FMT_I, G_IM_SIZ_8b, ((((t->w - 1 - 0 + 1) * G_IM_SIZ_8b_TILE_BYTES) + 7) >> 3), 0, G_TX_LOADTILE, 0, 0, 5, 0, 0, 5, 0);
    XgDPNoParam(N0, t->dl->hop(), G_RDPLOADSYNC);
    XgDPLoadTileGeneric(N0, t->dl->hopI(), G_LOADTILE + SNOW_PH(t, 0x1A0), G_TX_LOADTILE, 0, 0, (t->w - 1) << G_TEXTURE_IMAGE_FRAC, (t->h - 1) << G_TEXTURE_IMAGE_FRAC);
    XgDPNoParam(N0, t->dl->hop(), G_RDPPIPESYNC + SNOW_PH(t, 0x1C8));
    XgDPSetTile(N0, t->dl->hop(), G_IM_FMT_I, G_IM_SIZ_8b, ((((t->w - 1 - 0 + 1) * G_IM_SIZ_8b_LINE_BYTES) + 7) >> 3), 0, G_TX_RENDERTILE, 0, 0, 5, 0, 0, 5, 0);
    XgDPLoadTileGeneric(N0, t->dl->hop(), G_SETTILESIZE + SNOW_PH(t, 0x218), G_TX_RENDERTILE, 0, 0, (t->w - 1) << G_TEXTURE_IMAGE_FRAC, (t->h - 1) << G_TEXTURE_IMAGE_FRAC);
    XgDPWord(N0, t->dl->hop(), 0xBB000001 + SNOW_PH(t, 0x240), 0x80008000);
    XgDPWord(N0, t->dl->hop(), 0xBA001402 + SNOW_PH(t, 0x268) + SNOW_PH(t, 0x26C) + SNOW_PH(t, 0x270), 0);
    XgDPWord(N0, t->dl->hop(), 0xBA001301, 0);
    XgDPWord(N0, t->dl->hop(), 0xB900031D, 0x00404A50);
    XgDPWord(N0, t->dl->hop(), 0xFCFFB3FF + SNOW_PH(t, 0x2E0) + SNOW_PH(t, 0x2E4) + SNOW_PH(t, 0x2E8), 0xFF65FEFF);
    XgDPWord(N0, t->dl->hop(), 0xFA000000, 0x80);
    XgDPWord(N0, t->dl->hop(), 0xB9000201, 4);
    XgDPWord(N0, t->dl->hop(), 0xB8000000, 0);
}
