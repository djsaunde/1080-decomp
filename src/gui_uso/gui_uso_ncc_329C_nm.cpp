/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for gui_func_0000329C (286 words; this body 284, the
 * same length as the cfe NM body). NM object only; the ROM build keeps INCLUDE_ASM. Run 139: the cfe gbi.h
 * macro body (gSPTexture + 3-way texture load + gSPScisTextureRectangle) recompiled unchanged under NCC:
 * 86.6 -> 88.50 standalone; `s32 pad[2]; pad[0] = 0;` (a dead store, no code) gives frame 88 = target. As for
 * 3714 (same macro, 93.5 under NCC). RESIDUAL: the target keeps the ring context / switch value in $s0. */
typedef float f32; typedef double f64; typedef int s32; typedef unsigned int u32; typedef short s16; typedef unsigned short u16; typedef signed char s8; typedef unsigned char u8;
extern "C" {
extern char D_00000000;
extern int gui_func_00000000(...);
}
#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((1 << (w)) - 1)) << (s)))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define PKT_BEGIN { s32 _i; u32 *_g; t = (s32)(*(char **)&D_00000000); _i = (*(s32 **)((char *)t + 0xC))[1]; \
    (*(s32 **)((char *)t + 0xC))[1] = _i + 1; _g = (u32 *)(**(s32 **)((char *)t + 0xC) + _i * 8);
#define PKT_END }

extern "C" void gui_func_0000329C(char *arg0, s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 pad[2];
    char *spr;
    f32 fw;
    f32 fh;
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    s32 t;

    pad[0] = 0;
    PKT_BEGIN _g[0] = 0xBB000001; _g[1] = 0x80008000; PKT_END
    spr = *(char **)(arg0 + 0x10);
    w = *(s16 *)(spr + 0x20);
    h = *(s16 *)(spr + 0x22);
    t = *(s32 *)(spr + 0x24);
    if (t != 0x408) {
        if (t != 0x120) {
            if (t != 0x110) {
            } else goto c110;
        } else goto c120;
    } else goto c408;
    goto done;
c408:
    gui_func_00000000((*(char **)&D_00000000), *(s32 *)(spr + 8), w, h, 0, 0, w, h, 0);
    goto done;
c120:
    gui_func_00000000((*(char **)&D_00000000), *(s32 *)(spr + 8), w, h, 0, 0, w, h, 0);
    goto done;
c110:
    gui_func_00000000((*(char **)&D_00000000), *(s32 *)(spr + 8), w, h, 0, 0, w, h, 0);
done:
    x = (f32)x0;
    fw = x1 - x0;
    y = (f32)y0;
    fh = y1 - y0;
#define XL (x << 2)
#define YL (y << 2)
#define XH ((x + (s32)fw) << 2)
#define YH ((y + (s32)fh) << 2)
#define DSDX ((s32)(((f32)(w - 1) * 1024.0f) / fw))
#define DTDY ((s32)(((f32)(h - 1) * 1024.0f) / fh))
    PKT_BEGIN
    _g[0] = (_SHIFTL(0xE4, 24, 8) | _SHIFTL(MAX((s16)(XH), 0), 12, 12) | _SHIFTL(MAX((s16)(YH), 0), 0, 12));
    _g[1] = (_SHIFTL(0, 24, 3) | _SHIFTL(MAX((s16)(XL), 0), 12, 12) | _SHIFTL(MAX((s16)(YL), 0), 0, 12));
    PKT_END
    PKT_BEGIN
    _g[0] = _SHIFTL(0xB4, 24, 8);
    _g[1] = (_SHIFTL((0 - (((s16)(XL) < 0) ? (((s16)(DSDX) < 0) ? (MAX((((s16)(XL) * (s16)(DSDX)) >> 7), 0)) : (MIN((((s16)(XL) * (s16)(DSDX)) >> 7), 0))) : 0)), 16, 16) |
             _SHIFTL((0 - (((YL) < 0) ? (((s16)(DTDY) < 0) ? (MAX((((s16)(YL) * (s16)(DTDY)) >> 7), 0)) : (MIN((((s16)(YL) * (s16)(DTDY)) >> 7), 0))) : 0)), 0, 16));
    PKT_END
    PKT_BEGIN
    _g[1] = (_SHIFTL(DSDX, 16, 16) | _SHIFTL(DTDY, 0, 16));
    _g[0] = _SHIFTL(0xB3, 24, 8);
    PKT_END
}
#undef XL
#undef YL
#undef XH
#undef YH
#undef DSDX
#undef DTDY
#undef PKT_BEGIN
#undef PKT_END
#undef MAX
#undef MIN
