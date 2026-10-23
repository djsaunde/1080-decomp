/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for gui_func_00003714 (256 words = target). NM object
 * only; the ROM build keeps INCLUDE_ASM. Run 138: the old register-mapped hand decode under NCC + four
 * parameter empty tests, 88.72 (250 of 256 words). Run 139: rewritten as its siblings 2DE0 / 329C -- the
 * stock gbi.h gSPTexture + texture-load call + gSPScisTextureRectangle (MAX / MIN (s16) ternaries,
 * dsdx / dtdy re-evaluated per use) with x = arg1 + (-(w / 2)) * arg5, w = arg4 -- under NCC: 256 words,
 * 93.45; `s32 pad[5]; pad[0] = 0;` (a dead store, no code) gives frame 96 = target: 93.51.
 * (The same body under cfe is 260 words / 81.3.) RESIDUAL: the ring context colours v0 (target a1, after
 * spilling arg1 first), h spills at 52 (target 48), and the register rotation that follows. */
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
extern "C" void gui_func_00003714(char *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5, f32 arg6) {
    s32 pad[5];
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
    h = *(s16 *)(spr + 0x22);
    gui_func_00000000((*(char **)&D_00000000), *(s32 *)(spr + 8), *(s16 *)(spr + 0x20), h, arg3, 0, arg4, h, 0);
    w = arg4;
    x = (f32)arg1 + (-((f32)w / 2) * arg5);
    fw = w * arg5;
    y = (f32)arg2 + (-((f32)h / 2) * arg6);
    fh = h * arg6;
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
