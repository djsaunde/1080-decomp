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
    /* 0x5C */ s32 vptr;
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

typedef char BaseMat_size_check[sizeof(BaseMat) == 0x60 ? 1 : -1];
typedef char BaseMat_format_check[(int)&((BaseMat *)0)->format == 0x24 ? 1 : -1];
typedef char BaseMat_name_check[(int)&((BaseMat *)0)->name == 0x44 ? 1 : -1];
typedef char BaseMat_vptr_check[(int)&((BaseMat *)0)->vptr == 0x5C ? 1 : -1];

#endif /* GDL_MATERIAL_H */
