#ifndef GDL_TEXLOAD_H
#define GDL_TEXLOAD_H

/* libgdl texture loading (gfx_loadTexture, game_libs 0x578B4; agent-a run 520): the image
 * records a BaseMat (gdl/material.h) points at, and the Gfx word pair the hand-inlined
 * display-list appends write (gdl/dlctx.h). Uses s32 only. */

/* the image record BaseMat.texels points at when texFlags & MAT_TEX_RECORD (and for every
 * MAT_FMT_CI8 texture); gfx_loadTexture (0x578B4) loads the width x height sub-rectangle at
 * (s, t) of a `width`-texel-wide image (SetTextureImage gets width - 1, LoadTile starts at
 * (s, t)). Integer fields: the matched loader does all address math in int. */
typedef struct TexImage {
    /* 0x00 */ s32 pixels;             /* image address (TexCI * for MAT_FMT_CI8) */
    /* 0x04 */ s32 s;                  /* sub-rectangle origin (texels) */
    /* 0x08 */ s32 t;
    /* 0x0C */ s32 width;              /* image width = row stride (texels) */
    /* 0x10 */ s32 height;
} TexImage;

/* a colour-indexed image (MAT_FMT_CI8): TexImage.pixels -> this; the loader loads tlut
 * (256 RGBA16 entries) with gDPLoadTLUT_pal256 and then the 8-bit texels */
typedef struct TexCI {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 texels;             /* translated through the 0x34458 call */
    /* 0x0C */ s32 tlut;               /* stored as is */
} TexCI;

/* The two other texture paths that read these records (game_libs_post0b.c):
 *   gfx_setupMatTextures_00045E20(gfx, DLContext *dl, BaseMat *mat): emits the texture
 *     enable / G_TEXTURE packets for the BaseMat->next chain, numbering BaseMat.tile;
 *   gfx_flushTexture_00046C4C(gfx, BaseMat *mat): passes the texel block to the 0x578F0
 *     call with a byte count = TexImage width * height (BaseMat width * height without
 *     MAT_TEX_RECORD) times a float its first call returns (bytes per texel, presumably);
 *     for MAT_FMT_CI8 / _B it passes TexCI.texels and then TexCI.tlut with 0x200. */
#define TEXIMG(p) ((TexImage *)(p))
#define TEXCI(p) ((TexCI *)(p))

/* one Gfx command as the hand-inlined appends write it: two s32 words (the matched
 * emitters build the words in int, like DLBuf.base) */
typedef struct GfxWords {
    /* 0x0 */ s32 w0;
    /* 0x4 */ s32 w1;
} GfxWords;
#define GFXW(p) ((GfxWords *)(p))

typedef char TexImage_size_check[sizeof(TexImage) == 0x14 ? 1 : -1];
typedef char TexCI_size_check[sizeof(TexCI) == 0x10 ? 1 : -1];
typedef char GfxWords_size_check[sizeof(GfxWords) == 8 ? 1 : -1];

#endif /* GDL_TEXLOAD_H */
