/* NCC (C++) DRAFT for gl_func_000578B4 -- NOT built, NOT matching (agent-g run 113).
 *
 * Compile standalone with IDO 7.1 NCC:
 *   tools/ido-static-recomp/build/7.1/out/NCC -c -G 0 -non_shared -Wab,-r4300_mul
 *       -O2 -mips2 -32 -I src/game_libs/ncc_drafts -o /tmp/x.o <this file>
 * Score vs expected/: 2418/2419 insns, objdiff 95.0% (the cfe NM wrap in
 * game_libs_post0b.c scores 96.3 but is a 12-pass m2c transcription).
 *
 * What this establishes (docs/IDO_CODEGEN.md#ncc-game-libs-vec3-62a2c,
 * #ncc-578b4-texture-loader):
 * - The function is the game's texture loader written with the libultra
 *   2.0I gbi macros: gDPSetTextureLUT, then for tex->flags & 0x10 a
 *   gDPLoadTextureTile variant per format (SetTileSize at 0,0) and
 *   otherwise gDPLoadMultiBlock / _4b with tmem and render tile tex->0x3C;
 *   CI8 formats load a 256-entry TLUT first. Every gbi command's block
 *   local `_g` is one frame slot (7 per arm = the 0x1C arm stride).
 * - The display-list pointer is a C++ inline member,
 *   `Gfx *next() { return &dl->buf[dl->count++]; }` (cfe needs a two-variable
 *   comma macro to get the count store before the buf reload).
 * - Under NCC the four wrap/mask locals stay memory-resident like the
 *   target (cfe colours one of them into $ra).
 * - The real dispatch is a C++ `switch (tex->fmt)`: edg lowers it to a
 *   SOURCE-ORDER compare chain ending `beq ci8; lui a0,2; b default; nop`
 *   exactly as in the target (cfe sorts case values). With the switch, the
 *   switch-2 temp lands at sp+0x34 like the target, but cmt gets coloured
 *   $ra again (89.4%); this draft keeps the goto ladder (95.0%).
 * Remaining: switch form + cmt colouring, switch-1 temp $a1 vs $t1, the F518
 * constant hoist in arms 1-2, CI8 arm $t1/$ra swap, switch-2 register
 * pressure (target spills the LoadBlock _g at slot 4 of each arm).
 */
typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32; typedef signed char s8; typedef short s16; typedef int s32; typedef float f32;
#include "gbi_subset.h"
extern "C" { int game_libs_func_00062F08(int); int gl_func_00034458(...); }
extern "C" char gl_d_578b4_s1[];
extern "C" char gl_d_578b4_s2a[];

#define gDPLoadTextureTileO(pkt, timg, fmt, siz, width, height, uls, ult, lrs, lrt, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ gDPSetTextureImage(pkt, fmt, siz, width, timg); \
  gDPSetTile(pkt, fmt, siz, (((((lrs)-(uls)+1) * siz##_TILE_BYTES)+7)>>3), 0, G_TX_LOADTILE, 0, cmt, maskt, shiftt, cms, masks, shifts); \
  gDPLoadSync(pkt); \
  gDPLoadTile(pkt, G_TX_LOADTILE, (uls)<<G_TEXTURE_IMAGE_FRAC, (ult)<<G_TEXTURE_IMAGE_FRAC, (lrs)<<G_TEXTURE_IMAGE_FRAC, (lrt)<<G_TEXTURE_IMAGE_FRAC); \
  gDPPipeSync(pkt); \
  gDPSetTile(pkt, fmt, siz, (((((lrs)-(uls)+1) * siz##_LINE_BYTES)+7)>>3), 0, G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks, shifts); \
  gDPSetTileSize(pkt, G_TX_RENDERTILE, 0, 0, ((lrs)-(uls))<<G_TEXTURE_IMAGE_FRAC, ((lrt)-(ult))<<G_TEXTURE_IMAGE_FRAC) }
#define gDPLoadTextureTile_4bO(pkt, timg, fmt, width, height, uls, ult, lrs, lrt, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ gDPSetTextureImage(pkt, fmt, G_IM_SIZ_8b, ((width)>>1), timg); \
  gDPSetTile(pkt, fmt, G_IM_SIZ_8b, (((((lrs)-(uls)+1)>>1)+7)>>3), 0, G_TX_LOADTILE, 0, cmt, maskt, shiftt, cms, masks, shifts); \
  gDPLoadSync(pkt); \
  gDPLoadTile(pkt, G_TX_LOADTILE, (uls)<<(G_TEXTURE_IMAGE_FRAC-1), (ult)<<(G_TEXTURE_IMAGE_FRAC), (lrs)<<(G_TEXTURE_IMAGE_FRAC-1), (lrt)<<(G_TEXTURE_IMAGE_FRAC)); \
  gDPPipeSync(pkt); \
  gDPSetTile(pkt, fmt, G_IM_SIZ_4b, (((((lrs)-(uls)+1)>>1)+7)>>3), 0, G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks, shifts); \
  gDPSetTileSize(pkt, G_TX_RENDERTILE, 0, 0, ((lrs)-(uls))<<G_TEXTURE_IMAGE_FRAC, ((lrt)-(ult))<<G_TEXTURE_IMAGE_FRAC) }

typedef struct { Gfx *buf; s32 count; } DList;
struct GCtx { char pad0[0xC]; DList *dl; Gfx *next() { return &dl->buf[dl->count++]; } };
typedef struct { char pad0[8]; void *pixels; void *tlut; } CITex;
typedef struct { void *img; s32 x; s32 y; s32 width; s32 height; } SubImg;
typedef struct {
    char pad0[0x1C];
    void *img;      /* 0x1C */
    s16 w;          /* 0x20 */
    s16 h;          /* 0x22 */
    s32 fmt;        /* 0x24 */
    s32 flags;      /* 0x28 */
    s16 shifts;     /* 0x2C */
    s16 shiftt;     /* 0x2E */
    char pad30[0xC];
    s32 rtile;      /* 0x3C */
} TexDesc;

#define GDL(c) ((c)->next())
#define TEX_RGBA32 0x120
#define TEX_RGBA16 0x110
#define TEX_IA8 0x408
#define TEX_I8 0x308
#define TEX_I4 0x304
#define TEX_CI8 0x508

extern "C" void gl_func_000578B4(GCtx *ctx, TexDesc *tex, s32 tmem) {
    s32 masks;
    s32 maskt;
    s32 cms;
    s32 cmt;
    SubImg *sub;


    s32 flags;
    s32 fmt;

    masks = game_libs_func_00062F08(tex->w);
    maskt = game_libs_func_00062F08(tex->h);
    cmt = 0;
    cms = 0;
    gDPSetTextureLUT(GDL(ctx), G_TT_NONE);

    flags = tex->flags;
    if (!(flags & 0x100)) {
        if (flags & 0x200) {
            cms = 2;
        } else if (flags & 0x400) {
            cms = 1;
        }
    }
    if (!(flags & 0x1000)) {
        if (flags & 0x2000) {
            cmt = 2;
        } else if (flags & 0x4000) {
            cmt = 1;
        }
    }
    if (flags & 0x10) {

        sub = (SubImg *)tex->img;
        fmt = tex->fmt;
        if (fmt == TEX_RGBA32) goto s1_rgba32;
        if (fmt == TEX_RGBA16) goto s1_rgba16;
        if (fmt == TEX_IA8) goto s1_ia8;
        if (fmt == TEX_I8) goto s1_i8;
        if (fmt == TEX_I4) goto s1_i4;
        if (fmt == TEX_CI8) goto s1_ci8;
        goto s1_def;
        {
        s1_rgba32:
            gDPLoadTextureTileO(GDL(ctx), gl_func_00034458(sub->img), G_IM_FMT_RGBA, G_IM_SIZ_32b, sub->width, sub->height,
                sub->x, sub->y, sub->x + tex->w - 1, sub->y + tex->h - 1, 0, cms, cmt, masks, maskt, 0, 0);
            return;
        s1_rgba16:
            gDPLoadTextureTileO(GDL(ctx), gl_func_00034458(sub->img), G_IM_FMT_RGBA, G_IM_SIZ_16b, sub->width, sub->height,
                sub->x, sub->y, sub->x + tex->w - 1, sub->y + tex->h - 1, 0, cms, cmt, masks, maskt, 0, 0);
            return;
        s1_ia8:
            gDPLoadTextureTileO(GDL(ctx), gl_func_00034458(sub->img), G_IM_FMT_IA, G_IM_SIZ_8b, sub->width, sub->height,
                sub->x, sub->y, sub->x + tex->w - 1, sub->y + tex->h - 1, 0, cms, cmt, masks, maskt, 0, 0);
            return;
        s1_i8:
            gDPLoadTextureTileO(GDL(ctx), gl_func_00034458(sub->img), G_IM_FMT_I, G_IM_SIZ_8b, sub->width, sub->height,
                sub->x, sub->y, sub->x + tex->w - 1, sub->y + tex->h - 1, 0, cms, cmt, masks, maskt, 0, 0);
            return;
        s1_i4:
            gDPLoadTextureTile_4bO(GDL(ctx), gl_func_00034458(sub->img), G_IM_FMT_I, sub->width, sub->height,
                sub->x, sub->y, sub->x + tex->w - 1, sub->y + tex->h - 1, 0, cms, cmt, masks, maskt, 0, 0);
            return;
        s1_ci8: {
            CITex *ci = (CITex *)sub->img;
            gDPLoadTLUT_pal256(GDL(ctx), ci->tlut);
            gDPSetTextureLUT(GDL(ctx), G_TT_RGBA16);
            gDPLoadTextureTileO(GDL(ctx), gl_func_00034458(ci->pixels), G_IM_FMT_CI, G_IM_SIZ_8b, sub->width, sub->height,
                sub->x, sub->y, sub->x + tex->w - 1, sub->y + tex->h - 1, 0, cms, cmt, masks, maskt, 0, 0);
            return;
        }
        s1_def:
            gl_func_00034458(gl_d_578b4_s1 + 0x219B8, tex->fmt);
            return;
        }
    } else {

        fmt = tex->fmt;
        if (fmt == TEX_RGBA32) goto s2_rgba32;
        if (fmt == TEX_RGBA16) goto s2_rgba16;
        if (fmt == TEX_IA8) goto s2_ia8;
        if (fmt == TEX_I8) goto s2_i8;
        if (fmt == TEX_I4) goto s2_i4;
        if (fmt == TEX_CI8) goto s2_ci8;
        goto s2_def;
        {
        s2_rgba32:
            gDPLoadMultiBlock(GDL(ctx), gl_func_00034458(tex->img), tmem, tex->rtile, G_IM_FMT_RGBA, G_IM_SIZ_32b, tex->w, tex->h,
                0, cms, cmt, masks, maskt, tex->shifts, tex->shiftt);
            return;
        s2_rgba16:
            gDPLoadMultiBlock(GDL(ctx), gl_func_00034458(tex->img), tmem, tex->rtile, G_IM_FMT_RGBA, G_IM_SIZ_16b, tex->w, tex->h,
                0, cms, cmt, masks, maskt, tex->shifts, tex->shiftt);
            return;
        s2_ia8:
            gDPLoadMultiBlock(GDL(ctx), gl_func_00034458(tex->img), tmem, tex->rtile, G_IM_FMT_IA, G_IM_SIZ_8b, tex->w, tex->h,
                0, cms, cmt, masks, maskt, tex->shifts, tex->shiftt);
            return;
        s2_i8:
            gDPLoadMultiBlock(GDL(ctx), gl_func_00034458(tex->img), tmem, tex->rtile, G_IM_FMT_I, G_IM_SIZ_8b, tex->w, tex->h,
                0, cms, cmt, masks, maskt, tex->shifts, tex->shiftt);
            return;
        s2_i4:
            gDPLoadMultiBlock_4b(GDL(ctx), gl_func_00034458(tex->img), tmem, tex->rtile, G_IM_FMT_I, tex->w, tex->h,
                0, cms, cmt, masks, maskt, tex->shifts, tex->shiftt);
            return;
        s2_ci8: {
            CITex *ci = (CITex *)sub->img;
            gDPLoadTLUT_pal256(GDL(ctx), ci->tlut);
            gDPSetTextureLUT(GDL(ctx), G_TT_RGBA16);
            gDPLoadMultiBlock(GDL(ctx), gl_func_00034458(ci->pixels), tmem, tex->rtile, G_IM_FMT_CI, G_IM_SIZ_8b, sub->width, sub->height,
                0, cms, cmt, masks, maskt, tex->shifts, tex->shiftt);
        }
        s2_def:
            gl_func_00034458(gl_d_578b4_s2a + 0x219C4, tex->fmt);
            return;
        }
    }
}
