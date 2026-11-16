/* IDO 7.1 NCC (C++) donors for snoweffects.uso below 0x6000 (agent-c run 655): 0x270 / 0xC90 / 0x28E0.
 * Kept apart from agent-h's snoweffects_uso_ncc*.cpp and agent-e's _e donor (own Makefile block at the
 * end of the Makefile). NCC donors build without -I include. */
#include "../game_libs/ncc_drafts/gbi_subset.h"
extern "C" int gl_func_00000000(...);
extern "C" char D_00000000[];
struct GCtx;
/* gfx_resetDLContext, void-typed blank jal (frees v0 for the context temp; agent-h run 649) */
extern "C" void gl_func_00000000_snow_reset(GCtx *);

#define gDPWord(pkt, a, b) { Gfx *_g = (Gfx *)(pkt); _g->words.w0 = (a); _g->words.w1 = (unsigned int)(b); }
typedef struct { Gfx *buf; int count; } DList;
/* libgdl display-list context: the Gfx head at +0xC. next() appends one packet; nextN() is the same
 * append through named locals (same code, a different uopt live-unit layout). */
struct GCtx {
    char pad0[0xC];
    DList *dl;
    Gfx *next() { return &dl->buf[dl->count++]; }
    Gfx *nextN() { int n = dl->count++; Gfx *g = &dl->buf[n]; return g; }
};
/* a texture sprite (name +0xC, 20-packet DL context +0x2C, texture size +0x34 / +0x38) */
struct SnowTex {
    char pad00[0xC];
    char *name;
    char pad10[0x2C - 0x10];
    GCtx *dl;
    int pad30;
    int w;
    int h;
};

/* snoweffects_uso_func_00000270 (NON_MATCHING, 245 / 260 words): a texture-sprite init (the 0x15D0
 * family with the render state first): name Data+0x2A8, a 20-packet DL context named Data+0x2BC, then
 * PipeSync, cycle 1, persp off, render mode 0x00404240, combine, prim colour 0, w = h = 32, the stock
 * 2.0I gDPLoadTextureTile of the 32x32 IA8 texture at Data+0 (mask 5 / 5), gSPTexture and EndDL.
 * The `if (1) {}` before the prim colour packet and `h = w` keep the 32 out of a hoisted constant
 * register (the target loads it inside that packet; `w = 32; h = 32` hoists it to the entry), which
 * matches the whole first half (the two size stores need separate lines, or they come out h, w).
 * Left: five packet pointers of the texture load take a3 / t0 / a1 where the target has a2 / a3
 * (live-unit layout, as agent-h's 0x15D0 family). */
extern "C" void snoweffects_uso_func_00000270(SnowTex *t) {
    t->name = D_00000000 + 0x2A8;
    t->dl = (GCtx *)gl_func_00000000(0, D_00000000 + 0x2BC, 20, 0, 0, 0, 0);
    gl_func_00000000_snow_reset(t->dl);
    gDPPipeSync(t->dl->nextN());
    gDPWord(t->dl->nextN(), 0xBA001402, 0);
    gDPWord(t->dl->nextN(), 0xBA001301, 0);
    gDPWord(t->dl->nextN(), 0xB900031D, 0x00404240);
    gDPWord(t->dl->nextN(), 0xFCFFB3FF, 0xFF64FE7F);
    if (1) {}
    gDPWord(t->dl->next(), 0xFA000000, 0);
    t->w = 32;
    t->h = t->w;
    gDPSetTextureImage(t->dl->nextN(), G_IM_FMT_IA, G_IM_SIZ_8b, t->w, D_00000000);
    gDPSetTile(t->dl->next(), G_IM_FMT_IA, G_IM_SIZ_8b, (((t->w - 1 + 1) * G_IM_SIZ_8b_TILE_BYTES) + 7) >> 3, 0, G_TX_LOADTILE, 0, 0, 5, 0, 0, 5, 0);
    gDPLoadSync(t->dl->next());
    gDPLoadTile(t->dl->nextN(), G_TX_LOADTILE, 0, 0, (t->w - 1) << G_TEXTURE_IMAGE_FRAC, (t->h - 1) << G_TEXTURE_IMAGE_FRAC);
    gDPPipeSync(t->dl->next());
    gDPSetTile(t->dl->next(), G_IM_FMT_IA, G_IM_SIZ_8b, (((t->w - 1 + 1) * G_IM_SIZ_8b_LINE_BYTES) + 7) >> 3, 0, G_TX_RENDERTILE, 0, 0, 5, 0, 0, 5, 0);
    gDPSetTileSize(t->dl->next(), G_TX_RENDERTILE, 0, 0, (t->w - 1) << G_TEXTURE_IMAGE_FRAC, (t->h - 1) << G_TEXTURE_IMAGE_FRAC);
    gDPWord(t->dl->next(), 0xBB000001, 0x80008000);
    if (1) {}
    gDPWord(t->dl->nextN(), 0xB8000000, 0);
}
