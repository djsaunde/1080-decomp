#ifndef GDL_WINDOW_H
#define GDL_WINDOW_H

#ifdef __cplusplus /* the NCC (edg) donors build without -I include and include this header by relative path */
#include "node.h"
#else
#include "gdl/node.h"
#endif

/* libgdl Window / ScreenWindow (game_libs post0b; see docs/1080_GRAPHICS.md).
 *
 * Window (0x144, Window_ctor 0x4CCB4, name "win", parm group "Window") is the
 * camera-to-screen view: a projection matrix at +0x30 built by
 * Window_setupView (perspective from the camera's fov when projType == 1,
 * otherwise an ortho box of width x height), the frame-buffer pointers that
 * ScreenWindow_allocBuffers fills, and the viewport ScreenWindow_display
 * turns into a Vp (vscale = vpW * 2, vtrans = vpX * 4 + vpW * 2, both in the
 * RSP's 1/4-pixel units).
 *
 * ScreenWindow (0x1CC, ScreenWindow_ctor 0x4C190, built by GfxLib_init as
 * "ScreenWindow") adds the per-frame display lists: two "startdraw" lists,
 * two "bg" / "bgDyn" lists and two "setupDyn" lists (one per frame buffer,
 * indexed by the GfxLib frame index at gfx +0x204), the current list at
 * +0x158, and two DLMark spans that the frame submit uses to cut the RSP
 * task out of the list.
 *
 * Window.flags (+0x78) bits seen in the bodies:
 *   0x2    allow the Z-buffer clear (with clearFlags bit 2)
 *   0x4    clear the colour buffer to clearColour (gfx_clearScreen)
 *   0x20   wrap Display() in two extra debug calls
 *   0x200  own Z buffer (else share the main ScreenWindow's)
 *   0x400  double-buffered colour (two buffers at +0xF4 / +0xF8)
 *   0x800  own colour buffer(s)
 *   0x1000 dynamic bg / setup display lists ("bgDyn" / "setupDyn") */

typedef struct DLMark {
    /* 0x0 */ struct DLContext *ctx; /* gfx_dlMarkBegin */
    /* 0x4 */ s32 start;             /* ctx->gfx.cur->count at begin */
    /* 0x8 */ s32 end;               /* ctx->gfx.cur->count at gfx_dlMarkEnd */
} DLMark;

typedef struct Window {
    /* 0x000 */ Node node;
    /* 0x02C */ s32 unk2C;
    /* 0x030 */ f32 proj[4][4];      /* projection, Window_setupView */
    /* 0x070 */ char *camera;        /* MCamera: fov word at +0x14C, view matrix at +0xB4 */
    /* 0x074 */ s32 unk74;
    /* 0x078 */ s32 flags;           /* see above */
    /* 0x07C */ s32 projType;        /* 1 = perspective */
    /* 0x080 */ f32 coneCos;         /* cam_isSphereInView: a sphere whose direction from the camera
                                   * has dot(dir, view forward) below this is outside the view cone */
    /* 0x084 */ f32 drawDist[5];     /* per-class draw distance at the reference 85-degree fov
                                   * (Window_setDefaults: 0, 2200, 3000, 19584, ...) */
    /* 0x098 */ f32 drawDistFov[5];  /* drawDist * 85 / camera fov (Window_display); cam_isSphereInView
                                   * culls a sphere of class i when drawDistFov[i] <= dist - radius */
    /* 0x0AC */ f32 near;
    /* 0x0B0 */ f32 far;
    /* 0x0B4 */ f32 aspect;          /* width / height, Window_updateAspect */
    /* 0x0B8 */ s32 width;           /* frame-buffer size in pixels */
    /* 0x0BC */ s32 height;
    /* 0x0C0 */ s32 originX;         /* window origin on the frame buffer: set with vpX / vpY by
                                   * Window_doCommand 110; the clear rectangle's corner */
    /* 0x0C4 */ s32 originY;
    /* 0x0C8 */ s32 vpW;             /* viewport size and origin, pixels */
    /* 0x0CC */ s32 vpH;
    /* 0x0D0 */ s32 vpX;
    /* 0x0D4 */ s32 vpY;
    /* 0x0D8 */ s32 unkD8;
    /* 0x0DC */ f32 clearColour[4];  /* RGBA 0..1, gfx_clearScreen's fill colour (flag 0x4) */
    /* 0x0EC */ s32 unkEC;
    /* 0x0F0 */ u16 *zBuffer;        /* own (flag 0x200) or the main ScreenWindow's */
    /* 0x0F4 */ u16 *colorBuf[2];    /* RGBA5551; [1] == [0] unless flag 0x400 */
    /* 0x0FC */ u16 *bufAlloc;       /* one allocation holding all of the above */
    /* 0x100 */ s32 unk100;          /* Window_doCommand 109 zeroes it */
    /* 0x104 */ s32 clearFlags;      /* bit 2: clear the Z buffer (with flag 0x2); Window_doCommand 109 */
    /* 0x108 */ s32 displayMask;     /* Window_display: Node_setDisplayMask(displayMask); doCommand 117; 1 by default */
    /* 0x10C */ s32 clipRowLo;       /* row range [lo, hi] of row-structured shapes to draw: */
    /* 0x110 */ s32 clipRowHi;       /* PolyShape_clipRows / clipToWindow; MapProp_display sets it (0..127) */
    /* 0x114 */ s32 unk114;
    /* 0x118 */ s32 unk118;
    /* 0x11C */ s32 nTaskHooks;      /* zeroed at the start of Window_display */
    /* 0x120 */ s32 nHooks;          /* zeroed at the start of Window_display */
    /* 0x124 */ void *taskHooks[4];  /* Window_addTaskHook; Window_runTaskHooks (RcpMgr_startTask) calls vtable +0xC of each */
    /* 0x134 */ void *hooks[4];      /* Window_addHook_0004EDEC / Window_runHooks_0004ED0C */
} Window;

/* Window +0x100 / +0x104 are one 8-byte member object: Window_ctor builds it as a unit (the
 * new(8) fallback of an inline member ctor, both words zeroed) and Window_setDefaults copies it
 * from gWindowDefaults +0x0 as a pair. WINCLEAR(p) views a pointer to it. */
typedef struct WinClear {
    /* 0x0 */ s32 unk0;              /* Window +0x100 */
    /* 0x4 */ s32 clearFlags;        /* Window +0x104 */
} WinClear;
#define WINCLEAR(p) ((WinClear *)(p))

typedef struct ScreenWindow {
    /* 0x000 */ Window win;
    /* 0x144 */ s32 fbIndex;          /* copy of the GfxLib frame index (gfx +0x204) */
    /* 0x148 */ struct ScreenWindow *fbOwner; /* self with flag 0x800, else the main ScreenWindow */
    /* 0x14C */ struct DLContext *startDL[3]; /* "startdraw" ([0], [1] used); [fbIndex]->frameStart is called with G_DL */
    /* 0x158 */ struct DLContext *dl;  /* current list: the gfx +0x254 window's ->dl is where emitters write */
    /* 0x15C */ DLMark mark15C;
    /* 0x168 */ DLMark mark168;       /* closed by ScreenWindow_vfn12 */
    /* 0x174 */ s32 unk174[3];
    /* 0x180 */ struct DLContext *bgDL[2];    /* "bg" / "bgDyn" */
    /* 0x188 */ s32 unk188[4];
    /* 0x198 */ struct DLContext *setupDL[2]; /* "setupDyn" (or the main window's) */
    /* 0x1A0 */ s32 unk1A0;
    /* 0x1A4 */ s32 unk1A4;
    /* 0x1A8 */ s32 unk1A8;           /* -1 after allocBuffers */
    /* 0x1AC */ s32 unk1AC;           /* -1 after allocBuffers */
    /* 0x1B0 */ s32 clearAlpha;       /* 1 after allocBuffers: the RGBA5551 alpha bit of the clear fill */
    /* 0x1B4 */ s32 unk1B4;           /* zeroed by ScreenWindow_display */
    /* 0x1B8 */ s32 unk1B8;
    /* 0x1BC */ s32 unk1BC;
    /* 0x1C0 */ struct DLContext *curBg; /* bgDL[fbIndex] for this frame */
    /* 0x1C4 */ s32 unk1C4[2];
} ScreenWindow;

#ifndef __cplusplus /* edg (NCC donors): &((T *)0)->f is not an integral constant expression */
typedef char DLMark_size_check[sizeof(DLMark) == 0xC ? 1 : -1];
typedef char Window_size_check[sizeof(Window) == 0x144 ? 1 : -1];
#ifndef __cplusplus /* edg (NCC donors): &((T *)0)->f is not an integral constant expression */
typedef char Window_proj_check[(int)&((Window *)0)->proj == 0x30 ? 1 : -1];
typedef char Window_flags_check[(int)&((Window *)0)->flags == 0x78 ? 1 : -1];
typedef char Window_aspect_check[(int)&((Window *)0)->aspect == 0xB4 ? 1 : -1];
typedef char Window_vpW_check[(int)&((Window *)0)->vpW == 0xC8 ? 1 : -1];
typedef char Window_zBuffer_check[(int)&((Window *)0)->zBuffer == 0xF0 ? 1 : -1];
typedef char Window_nTaskHooks_check[(int)&((Window *)0)->nTaskHooks == 0x11C ? 1 : -1];
typedef char Window_clearColour_check[(int)&((Window *)0)->clearColour == 0xDC ? 1 : -1];
typedef char Window_displayMask_check[(int)&((Window *)0)->displayMask == 0x108 ? 1 : -1];
typedef char Window_hooks_check[(int)&((Window *)0)->hooks == 0x134 ? 1 : -1];
typedef char ScreenWindow_size_check[sizeof(ScreenWindow) == 0x1CC ? 1 : -1];
typedef char ScreenWindow_dl_check[(int)&((ScreenWindow *)0)->dl == 0x158 ? 1 : -1];
typedef char ScreenWindow_bgDL_check[(int)&((ScreenWindow *)0)->bgDL == 0x180 ? 1 : -1];
typedef char ScreenWindow_setupDL_check[(int)&((ScreenWindow *)0)->setupDL == 0x198 ? 1 : -1];
typedef char ScreenWindow_curBg_check[(int)&((ScreenWindow *)0)->curBg == 0x1C0 ? 1 : -1];
#endif

#endif /* GDL_WINDOW_H */
