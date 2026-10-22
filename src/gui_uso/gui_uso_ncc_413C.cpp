/* IDO 7.1 NCC (C++) donor for gui_uso_func_0000413C.
 *
 * Texture-tile loader for the gfx context c: SETTIMG, load SETTILE (tile 7),
 * LOADSYNC, LOADTILE, PIPESYNC, render SETTILE (tile `tile`, tmem tile << 8)
 * and SETTILESIZE (origin 0), G_IM_FMT_RGBA / G_IM_SIZ_16b, maskt 8 / masks 8. Written
 * with the real gbi.h macro bodies (src/gui_uso/gui_ncc_gbi.h) and the
 * context's inline next() append; the lower-right corners are macro text
 * (uls + w - 1), so IDO's reassociation gives the target's (uls + w) - uls
 * and a3 - 1 forms. The cfe NM body (an open-coded reconstruction) capped
 * at ~86%; this is word-exact. The fourth parameter is unused (edg still
 * stores it to its home). docs/IDO_CODEGEN.md kit XXV. Siblings: 413C
 * (RGBA16), 4354 (IA8), 3F18 (RGBA32).
 * Spliced via REPLACE_FUNC_BODY into both gui_uso trees. */
#include "gui_ncc_gbi.h"
struct DL { Gfx *buf; int n; };
struct Ctx {
    char pad[0xC];
    DL *dl;
    Gfx *next() { int n = dl->n++; return &dl->buf[n]; }
};
#define FMT G_IM_FMT_RGBA
#define SIZ G_IM_SIZ_16b
#define LRS (uls + w - 1)
#define LRT (ult + h - 1)
extern "C" void gui_uso_func_0000413C(Ctx *c, void *timg, int width, int unused, int uls, int ult, int w, int h,
                                       int tile) {
    gDPSetTextureImage(c->next(), FMT, SIZ, width, timg);
    gDPSetTile(c->next(), FMT, SIZ, ((((LRS) - (uls) + 1) * G_IM_SIZ_16b_TILE_BYTES) + 7) >> 3, tile << 8, G_TX_LOADTILE,
               0, 0, 8, 0, 0, 8, 0);
    gDPLoadSync(c->next());
    gDPLoadTile(c->next(), G_TX_LOADTILE, (uls) << G_TEXTURE_IMAGE_FRAC, (ult) << G_TEXTURE_IMAGE_FRAC,
                (LRS) << G_TEXTURE_IMAGE_FRAC, (LRT) << G_TEXTURE_IMAGE_FRAC);
    gDPPipeSync(c->next());
    gDPSetTile(c->next(), FMT, SIZ, ((((LRS) - (uls) + 1) * G_IM_SIZ_16b_LINE_BYTES) + 7) >> 3, tile << 8, tile,
               0, 0, 8, 0, 0, 8, 0);
    gDPSetTileSize(c->next(), tile, 0, 0, ((LRS) - (uls)) << G_TEXTURE_IMAGE_FRAC,
                   ((LRT) - (ult)) << G_TEXTURE_IMAGE_FRAC);
}
