#ifndef GDL_GFXLIB_H
#define GDL_GFXLIB_H

/* The libgdl GfxLib global (0x5D8 bytes, bootup.uso Bss+0x3C168, Sym 154; source spelling
 * `gGfxLib`, include/gdl/gl_data.h). docs/1080_GRAPHICS.md section 2 has the evidence table.
 * Built by GfxLib_init (game_libs 0x445AC), reset by GfxLib_reset (0x44918), published by
 * GfxLib_hostioParms (0x44144).
 *
 * Reach it through GFXLIB(&gGfxLib) or gGfx below. The per-site aliases (gGfxLib_42938_gb,
 * gGfxLib_41820_c, ...) are their own zero-valued symbols that keep the matched lui/addiu
 * pairs; type them with GFXLIB(&alias) the same way.
 *
 * Plain C types only (no s32 / f32): the NCC (edg C++) donors include this header too. */

struct ScreenWindow;
struct DLContext;
struct RcpMgr;

/* The GfxLib +0x22C morph list (GfxLib_init: `new L24`, capacity 10, a 40-byte item buffer). */
typedef struct GfxMorphList {
    /* 0x00 */ void **items;
    /* 0x04 */ int unk04;
    /* 0x08 */ int capacity;            /* 10 */
    /* 0x0C */ int count;
    /* 0x10 */ int unk10;
    /* 0x14 */ char *vptr;              /* slot 0 = the per-frame update (gfx_end) */
} GfxMorphList;

/* A DDM record (12 bytes; "DDM" = the per-frame render-state hooks, GfxLib.ddm[]). GfxLib holds a static
 * pool of 50 (ddmPool, count nDdms): gfx_addDdm (0x44CE8) takes the next one (sys_staticArrayOverflow at
 * 50), stores it in ddm[id], clears state.flags and calls gfx_initDdm, which records the static DL's Gfx
 * count in dlIndex and builds the render state {0, flags} in place. Its Cfront element ctor is
 * app_newListHead_000422AC (= El::El, passed to __vec_new(&ddmPool, 50, 12, ctor) by the static
 * initializer app_staticInit_000412E8: bootup.uso TextReloc HI16 / LO16 at Text 0x55ABC / 0x55AC0 ->
 * 0x422AC): it zeroes the state member, which has a ctor of its own (the `new(8)` fallback).
 * agent-b run 591. */
typedef struct GfxDdmState {         /* the GdlRenderState layout (dlctx.h); kept plain here for the NCC donors */
    /* 0x0 */ int unk0;
    /* 0x4 */ int flags;
} GfxDdmState;

typedef struct GfxDdm {
    /* 0x0 */ int dlIndex;              /* gfx_initDdm: gGfx->staticDL's Gfx count where this state's commands start */
    /* 0x4 */ GfxDdmState state;
} GfxDdm;

/* FogParms (0x18, parm group "FogParms"): FogParms_set / setDefault (white, 996..1000) / lerp /
 * emit (gfx_emitFog: G_SETFOGCOLOR + G_MW_FOG; the emit reads the low halves of min / max). */
typedef struct FogParms {
    /* 0x00 */ float colour[4];         /* RGBA 0..1 */
    /* 0x10 */ int min;                 /* hostio "Min" (500..1000) */
    /* 0x14 */ int max;                 /* hostio "Max" (500..1000) */
} FogParms;

typedef struct GfxLib {
    /* 0x000 */ int every4thFrame;      /* (frame counter & 3) == 0; gfx_setEvery4thFrameFlag */
    /* 0x004 */ int unk004[23];
    /* 0x060 */ float proj[4][4];       /* projection matrix: gfx_matrixModeProjection */
    /* 0x0A0 */ float modelView[4][4];  /* gfx_matrixModeModelView / gfx_resetModelView */
    /* 0x0E0 */ float (*curMtx)[4];     /* proj or modelView: gfx_loadMatrix / multMatrix */
    /* 0x0E4 */ float (*extraMtx)[4];   /* extra model matrix (shadow / mirror):
                                         * light_setShadowMatrix / light_setMirrorMatrix */
    /* 0x0E8 */ float worldMtx[4][4]; /* diag(worldScale): game_initDdm */
    /* 0x128 */ float worldScale[3];    /* CollShape_query divides the probe point by it;
                                         * [2] scales camera +0xA8 distances (MapProp / Fog / Snower) */
    /* 0x134 */ int unk134;
    /* 0x138 */ int unk138;             /* zeroed by GfxLib_reset */
    /* 0x13C */ int renderFlags;        /* GfxLib_reset: &= ~0x10000 / ~2 / ~0x1000, |= 1, 0x4000,
                                         * 0x8000, 0x20000, 0x80 */
    /* 0x140 */ int unk140[2];          /* 2-word copy source of Shadow_ctor_0004B0A8 */
    /* 0x148 */ void *frameStartCbs[5];  /* BaseMats (bootup ScreenThumbMat): gfx_start calls
                                         * slot 0 of each one's vtable (BaseMat.vptr, +0x5C), arg 0 */
    /* 0x15C */ int nFrameStartCbs;
    /* 0x160 */ int unk160[10];         /* a table SnowBoarder_ctor fills (agent-h) */
    /* 0x188 */ int viewStage;          /* 0 projection, 1 modelview (gfx_setViewStage_00047F68) */
    /* 0x18C */ void *lights;           /* light list head (Link {obj, next}): gfx_updateLights */
    /* 0x190 */ void *shadowLight;      /* MNode (position +0xA0): gfx_setShadowLight */
    /* 0x194 */ int unk194;
    /* 0x198 */ void *drawUpdates[10];  /* gfx_queuePush_0004E1BC (sys_staticArrayOverflow at 10);
                                         * gfx_end calls vtable (+0) slot 0 of each ("ADrawUpd") */
    /* 0x1C0 */ int nDrawUpdates;       /* zeroed by gfx_start */
    /* 0x1C4 */ int flags;              /* bit 1 cleared by gfx_beginView; bit 2 = clear with a
                                         * zero quad; 0x8 = GFX_FLAG_MAPPARMS */
    /* 0x1C8 */ void *unk1C8;
    /* 0x1CC */ int unk1CC[5];
    /* 0x1E0 */ int stageIsProj;        /* viewStage == 0 */
    /* 0x1E4 */ int unk1E4[4];
    /* 0x1F4 */ int mtxMode;            /* 1 projection, 0 modelview (gfx_matrixMode*) */
    /* 0x1F8 */ signed char dir[4];     /* s8 unit vector (float * 127) + pad */
    /* 0x1FC */ unsigned short colour[3]; /* RGB 0..255 from floats: gfx_colorToRGB16_0004771C */
    /* 0x202 */ unsigned short pad202;
    /* 0x204 */ int fbIndex;            /* frame-buffer index (0 / 1) */
    /* 0x208 */ unsigned short perspNorm; /* gfx_calcPerspNorm -> gfx_emitPerspNorm */
    /* 0x20A */ unsigned short pad20A;
    /* 0x20C */ unsigned int frameTime; /* prof_timerRead("OneFrame") at init */
    /* 0x210 */ int unk210;             /* zeroed by GfxLib_reset */
    /* 0x214 */ struct DLContext *staticDL; /* "MainStaticDL": gfx_callStaticDL, MorphPoly_init */
    /* 0x218 */ struct RcpMgr *rcp;     /* GfxLib_hostioParms, GfxLib_reset busy-wait */
    /* 0x21C */ int textX;              /* gfx_drawText cursor and character cell (pixels):  */
    /* 0x220 */ int textY;              /* dbg_screenOverlay sets 0x32,0x50 with 8x8 cells,  */
    /* 0x224 */ int charW;              /* then 0x78,0x78 with 16x16 cells; sys_sprintf     */
    /* 0x228 */ int charH;              /* (0x61B30) sets them for its CPU-drawn text        */
    /* 0x22C */ struct GfxMorphList *morphs; /* MorphPoly_display pushes itself; gfx_end runs its
                                         * update ("MorphUpd"); gfx_start empties it */
    /* 0x230 */ int unk230;             /* zeroed by GfxLib_reset */
    /* 0x234 */ int unk234[3];
    /* 0x240 */ struct ScreenWindow *mainWindow; /* GfxLib_init; gfx_drawFbChar / gfx_clearFbRect */
    /* 0x244 */ int unk244;             /* zeroed by gfx_start */
    /* 0x248 */ int unk248;             /* zeroed by gfx_start */
    /* 0x24C */ float unk24C;           /* GfxLib_hostioParms float parm; reset from a constant */
    /* 0x250 */ int unk250;             /* GfxLib_hostioParms parm; zeroed by GfxLib_reset */
    /* 0x254 */ struct ScreenWindow *curWindow; /* emitters append to curWindow->dl */
    /* 0x258 */ int nWindows;           /* gfx_addWindow ("too many windows" at 8); zeroed by gfx_start */
    /* 0x25C */ struct ScreenWindow *windows[8]; /* this frame's windows: gfx_end submits each */
    /* 0x27C */ int unk27C;
    /* 0x280 */ struct DLContext *oneFrameDL[2]; /* GfxLib_init; per frame buffer (fbIndex),
                                         * reset by gfx_end */
    /* 0x288 */ int unk288;
    /* 0x28C */ int cpuDraw;            /* gfx_drawText / gfx_fillRect draw into the frame buffer */
    /* 0x290 */ int unk290;
    /* 0x294 */ FogParms fog;           /* the gGfxLib_*_src aliases; FogParms_emit / lerp, GfxLib_reset /
                                         * hostioParms pass its address */
    /* 0x2AC */ int ddm[51];            /* per-frame DDM render-state hooks (gfx_getDdm); zeroed by reset */
    /* 0x378 */ GfxDdm ddmPool[50];    /* the DDM records ddm[] points at (gfx_addDdm; GfxDdm above) */
    /* 0x5D0 */ int nDdms;              /* ddmPool records in use; zeroed by GfxLib_reset */
    /* 0x5D4 */ int unk5D4;
} GfxLib;


/* GfxLib.flags 0x8: set by game_createMap (bootup) when the course has a COURSE_OP_MAP record (the
 * map angle / end / add parms); Mat_reset_00043584 then gives each shape a 0x100-byte buffer
 * at +0x7C. */
#define GFX_FLAG_MAPPARMS 0x8

#define GFXLIB(p) ((GfxLib *)(p))
/* the global itself (gGfxLib = D_00000000 + K sites, gl_data.h) */
#define gGfx GFXLIB(&gGfxLib)

typedef char FogParms_size_check[sizeof(FogParms) == 0x18 ? 1 : -1];
typedef char GfxDdm_size_check[sizeof(GfxDdm) == 0xC ? 1 : -1];
typedef char GfxLib_size_check[sizeof(GfxLib) == 0x5D8 ? 1 : -1];
#ifndef __cplusplus /* edg (NCC donors): &((T *)0)->f is not an integral constant expression */
typedef char GfxLib_extraMtx_check[(int)&((GfxLib *)0)->extraMtx == 0xE4 ? 1 : -1];
typedef char GfxLib_worldScale_check[(int)&((GfxLib *)0)->worldScale == 0x128 ? 1 : -1];
typedef char GfxLib_flags_check[(int)&((GfxLib *)0)->flags == 0x1C4 ? 1 : -1];
typedef char GfxLib_fbIndex_check[(int)&((GfxLib *)0)->fbIndex == 0x204 ? 1 : -1];
typedef char GfxLib_staticDL_check[(int)&((GfxLib *)0)->staticDL == 0x214 ? 1 : -1];
typedef char GfxLib_textX_check[(int)&((GfxLib *)0)->textX == 0x21C ? 1 : -1];
typedef char GfxLib_mainWindow_check[(int)&((GfxLib *)0)->mainWindow == 0x240 ? 1 : -1];
typedef char GfxLib_curWindow_check[(int)&((GfxLib *)0)->curWindow == 0x254 ? 1 : -1];
typedef char GfxLib_cpuDraw_check[(int)&((GfxLib *)0)->cpuDraw == 0x28C ? 1 : -1];
typedef char GfxLib_nFrameStartCbs_check[(int)&((GfxLib *)0)->nFrameStartCbs == 0x15C ? 1 : -1];
typedef char GfxLib_drawUpdates_check[(int)&((GfxLib *)0)->drawUpdates == 0x198 ? 1 : -1];
typedef char GfxLib_nDrawUpdates_check[(int)&((GfxLib *)0)->nDrawUpdates == 0x1C0 ? 1 : -1];
typedef char GfxLib_morphs_check[(int)&((GfxLib *)0)->morphs == 0x22C ? 1 : -1];
typedef char GfxLib_windows_check[(int)&((GfxLib *)0)->windows == 0x25C ? 1 : -1];
typedef char GfxLib_oneFrameDL_check[(int)&((GfxLib *)0)->oneFrameDL == 0x280 ? 1 : -1];
typedef char GfxLib_ddm_check[(int)&((GfxLib *)0)->ddm == 0x2AC ? 1 : -1];
typedef char GfxLib_ddmPool_check[(int)&((GfxLib *)0)->ddmPool == 0x378 ? 1 : -1];
typedef char GfxLib_nDdms_check[(int)&((GfxLib *)0)->nDdms == 0x5D0 ? 1 : -1];
#endif

#endif /* GDL_GFXLIB_H */
