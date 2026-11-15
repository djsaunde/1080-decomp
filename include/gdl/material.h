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
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;              /* -1 */
    /* 0x1C */ void *texels;
    /* 0x20 */ s16 width;
    /* 0x22 */ s16 height;
    /* 0x24 */ s32 format;
    /* 0x28 */ s32 texFlags;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ u8 unk32;
    /* 0x33 */ u8 pad33;
    /* 0x34 */ s32 unk34;              /* 8-byte member (0x34-0x3C) in the ctor */
    /* 0x38 */ s32 geomFlags;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ char *name;             /* "<no name>" */
    /* 0x48 */ s32 unk48;
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 minCut;
    /* 0x54 */ s32 maxCut;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s32 vptr;
} BaseMat;

typedef char BaseMat_size_check[sizeof(BaseMat) == 0x60 ? 1 : -1];
typedef char BaseMat_format_check[(int)&((BaseMat *)0)->format == 0x24 ? 1 : -1];
typedef char BaseMat_name_check[(int)&((BaseMat *)0)->name == 0x44 ? 1 : -1];
typedef char BaseMat_vptr_check[(int)&((BaseMat *)0)->vptr == 0x5C ? 1 : -1];

#endif /* GDL_MATERIAL_H */
