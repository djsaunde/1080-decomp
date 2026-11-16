#ifndef GDL_MATERIAL_H
#define GDL_MATERIAL_H

/* libgdl BaseMat (0x60) and Mat (0x90); game_libs post0b, docs/1080_GRAPHICS.md.
 *
 * BaseMat_init (0x517E4) resets every field below; Mat_fromFrameBuffer
 * (0x4F85C) builds a Mat whose texture is a ScreenWindow's colour buffer
 * (texels = colorBuf[0], width / height from the window, format 0x110);
 * gfx_setupMatTextures (0x45E20) and gfx_flushTexture (0x46C4C) read the
 * texture fields. The Cfront vtable pointer is at +0x5C (not at +0x28: BaseMat
 * is not a Node). BaseMat_hostioParms publishes "Texture", "minCut" (+0x50),
 * "maxCut" (+0x54).
 *
 * texFlags (+0x28): 0x10 = texels is an indirect image record {.., +0xC w,
 * +0x10 h}; setupMatTextures sets 0x1100. format (+0x24): 0x110 for a frame
 * buffer, 0x508 / 0x608 carry a 0x200-byte TLUT. geomFlags (+0x38): bit 20
 * enables texgen (G_TEXTURE_GEN), bit 24 adds G_TEXTURE_GEN_LINEAR. */

typedef struct BaseMat {
    /* 0x00 */ f32 colour[4];          /* 0 */
    /* 0x10 */ f32 unk10;              /* 1.0 */
    /* 0x14 */ s32 unk14;              /* Mat_print prints only when != 0 */
    /* 0x18 */ s32 id;                 /* -1; "MAT %d", "unknown mat(%d) size %d" */
    /* 0x1C */ void *texels;           /* GDL_MC 203 word 1 */
    /* 0x20 */ s16 width;              /* GDL_MC 203 word 2 */
    /* 0x22 */ s16 height;             /* GDL_MC 203 word 3 */
    /* 0x24 */ s32 format;             /* GDL_MC 203 word 0; MAT_FMT_* below */
    /* 0x28 */ s32 texFlags;           /* GDL_MC 203 word 4 (s16); MAT_TEX_* below */
    /* 0x2C */ s16 shiftS;             /* GDL_MC 203 word 5: G_TX shift S (SetTile) */
    /* 0x2E */ s16 shiftT;             /* GDL_MC 203 word 6: G_TX shift T */
    /* 0x30 */ s16 unk30;              /* GDL_MC 210: two hex digits of the code */
    /* 0x32 */ u8 unk32;               /* GDL_MC 210: first char of the code */
    /* 0x33 */ u8 pad33;
    /* 0x34 */ s32 unk34;              /* 8-byte pair with geomFlags (GDL_MC 204 / 207 / 208) */
    /* 0x38 */ s32 geomFlags;          /* GDL_MC 204 set, 205 |=, 206 &= ~ */
    /* 0x3C */ s32 tile;               /* render tile, gfx_setupMatTextures numbers the chain */
    /* 0x40 */ struct BaseMat *next;   /* next texture of a multi-texture material */
    /* 0x44 */ char *name;             /* "<no name>"; GDL_MC 209 */
    /* 0x48 */ s32 unk48;              /* GDL_MC 208: the looked-up shared record */
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 minCut;
    /* 0x54 */ s32 maxCut;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ char *vptr;             /* Cfront vtable pointer */
} BaseMat;

/* format (+0x24): (component kind << 8) | bits per texel; decoded from
 * gfx_loadTexture (0x578B4), gfx_texelBytes (0x52144), hostio_sendImage
 * (size = (fmt & 0xFF) * w * h / 8, + 0x200 for 0x508). docs/1080_GRAPHICS.md 6. */
#define MAT_FMT_RGBA32 0x120 /* G_IM_FMT_RGBA, G_IM_SIZ_32b */
#define MAT_FMT_RGBA16 0x110 /* G_IM_FMT_RGBA, G_IM_SIZ_16b (frame buffers too) */
#define MAT_FMT_IA8    0x408 /* G_IM_FMT_IA,   G_IM_SIZ_8b */
#define MAT_FMT_I8     0x308 /* G_IM_FMT_I,    G_IM_SIZ_8b */
#define MAT_FMT_I4     0x304 /* G_IM_FMT_I,    G_IM_SIZ_4b */
#define MAT_FMT_CI8    0x508 /* G_IM_FMT_CI,   G_IM_SIZ_8b, 256-entry RGBA16 TLUT */
#define MAT_FMT_CI8_B  0x608 /* 1 byte/texel + 0x200 TLUT, but gfx_loadTexture rejects it */

/* texFlags (+0x28) */
#define MAT_TEX_RECORD   0x0010 /* texels -> TexImage record (sub-rectangle / CI) */
#define MAT_TEX_WRAP_S   0x0100 /* overrides the two below */
#define MAT_TEX_CLAMP_S  0x0200 /* G_TX_CLAMP */
#define MAT_TEX_MIRROR_S 0x0400 /* G_TX_MIRROR */
#define MAT_TEX_WRAP_T   0x1000
#define MAT_TEX_CLAMP_T  0x2000
#define MAT_TEX_MIRROR_T 0x4000

/* Mat (0x90, Mat_fromFrameBuffer_0004F85C: mem_new(0x90), BaseMat base, Mat vtable at +0x5C).
 * PolyShape_buildStateDL_00043654 compiles the material's render state into the GfxLib static
 * display list: stateDL = gGfx->staticDL, stateStart = its Gfx count at the start (the span
 * gfx_callStaticDL replays); with geomFlags bit 22 (textured) the texture setup goes in, and with
 * bit 20 (texgen) as well a 5-Gfx list of its own (texgenDL) is built and called with G_DL;
 * untextured materials append G_TEXTURE off (0xBB040000, -1). */
typedef struct Mat {
    /* 0x00 */ BaseMat base;
    /* 0x60 */ struct DLContext *lightDL; /* PolyShape_allocLightDL_000435FC ("light"), geomFlags 0x2000 */
    /* 0x64 */ struct DLContext *stateDL;  /* gGfx->staticDL ("MainStaticDL") */
    /* 0x68 */ struct DLContext *texgenDL; /* geomFlags bit 20: gfx_newDLContext(.., 0xA, 0, 5, ..) */
    /* 0x6C */ struct DLContext *triDL;    /* PolyShape_init (ncc_493AC.cpp Mat::triDL): gGfx->staticDL,
                                            * 0 when no triangle uses this material */
    /* 0x70 */ s32 unk70;
    /* 0x74 */ s32 unk74;
    /* 0x78 */ s32 unk78;
    /* 0x7C */ s16 *rowGfx;                /* GFX_FLAG_MAPPARMS only (0x100 bytes, Mat_reset_00043584):
                                            * triDL Gfx count at the end of each clip row (grid row),
                                            * the spans PolyShape_clipRows draws for rows minCut..maxCut */
    /* 0x80 */ s16 rowStart;               /* triDL Gfx count where the rows begin */
    /* 0x82 */ s16 pad82;
    /* 0x84 */ s32 unk84;                  /* 1 after Mat_reset_00043584 */
    /* 0x88 */ s16 stateStart;             /* stateDL->gfx.cur->count before the state commands */
    /* 0x8A */ s16 triStart;               /* triDL Gfx count before this material's triangles */
    /* 0x8C */ s32 unk8C;
} Mat;

#define MAT_GEOM_TEXGEN   0x00100000 /* geomFlags bit 20: G_TEXTURE_GEN */
#define MAT_GEOM_TEXTURED 0x00400000 /* geomFlags bit 22: Mat_print "Tex" (else "RGB") */

typedef char BaseMat_size_check[sizeof(BaseMat) == 0x60 ? 1 : -1];
typedef char Mat_size_check[sizeof(Mat) == 0x90 ? 1 : -1];
#ifndef __cplusplus /* edg (NCC donors): &((T *)0)->f is not an integral constant expression */
typedef char BaseMat_format_check[(int)&((BaseMat *)0)->format == 0x24 ? 1 : -1];
typedef char BaseMat_name_check[(int)&((BaseMat *)0)->name == 0x44 ? 1 : -1];
typedef char BaseMat_vptr_check[(int)&((BaseMat *)0)->vptr == 0x5C ? 1 : -1];
typedef char Mat_stateStart_check[(int)&((Mat *)0)->stateStart == 0x88 ? 1 : -1];
typedef char Mat_rowGfx_check[(int)&((Mat *)0)->rowGfx == 0x7C ? 1 : -1];
typedef char Mat_unk84_check[(int)&((Mat *)0)->unk84 == 0x84 ? 1 : -1];
#endif

#endif /* GDL_MATERIAL_H */
