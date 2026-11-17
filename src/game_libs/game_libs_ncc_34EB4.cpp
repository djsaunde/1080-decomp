#include "../../include/uso_call.h"  /* NCC donors build without -I include */
/* IDO 7.1 NCC (C++) donor for app_run (0x2B0 / 172 words): the frame
 * main loop. Pumps the per-object list, the app's virtual hooks (v3/v4/v13/v15
 * on a 40-byte base, so the vptr is at +0x28) and the one-shot callback at
 * +0x30, forever: `quit` is never set, so the epilogue is unreachable and the
 * outer loop restarts with the v13 hook. `for (quit = 0;;)` puts the `li s4,0`
 * after the hoisted string addresses (a plain `quit = 0;` before the loop
 * schedules it first). The flags word is a distinct zero alias so the test
 * does not share the s3 base. Spliced into game_libs_post0b.c.o via
 * REPLACE_FUNC_BODY (agent-c run 124). */
extern "C" {
void gl_func_00000000(...);
extern char D_00000000[];
extern int gHostio_34EB4_flags;
}
#define DB D_00000000
typedef int s32; typedef unsigned int u32; typedef short s16; typedef unsigned short u16; typedef unsigned char u8; typedef float f32; typedef unsigned long long u64;
#include "../../include/gdl/gfxlib.h" /* GfxLib.rcp */
#include "../../include/gdl/rcp.h" /* RcpMgr */
#define FW(p, o) (*(int *)((char *)(p) + (o)))
struct Pad40 { char pad[0x28]; };
#ifdef PORT
#include "port_cfront.h" /* cfront vptr at +0x28; v1..v15 are slots 0..14 (plan 17.8) */
struct AppB : Pad40 {
    char *vptr;
    CF_V0(void, v3, 2) CF_V0(void, v4, 3) CF_V0(void, v13, 12) CF_V0(void, v15, 14)
};
#else
struct AppB : Pad40 {
    virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4();
    virtual void v5(); virtual void v6(); virtual void v7(); virtual void v8();
    virtual void v9(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15();
};
#endif
struct App : AppB { char pad2[4]; void (*cb)(); };
struct Node { char pad[0x60]; Node *next; };
struct St { int flags; int pad4, pad8; Node *list; int f10; int count; };
extern "C" void app_run(St *a0, App *a1) {
    int quit;
    CALL(sys_imin)(DB + 0x1E47C);
    if (gHostio_34EB4_flags & 1) CALL(hostio_start)();
    for (quit = 0;;) {
        a0->flags &= ~1;
        a1->v4();
        CALL(snd_PlayPlayer0Seq0)();
        do {
            Node *n;
            for (n = a0->list; n != 0; n = n->next) CALL(pad_update)(n);
            if (GFXLIB(XDATA(DB, port_bootup_bss, 0x3C168))->rcp->preNmi != 0) a1->v15(); /* v15 is vtable slot 14, App_onPreNmi */
            while (a0->flags & 4) {
                a0->f10 = 0;
                CALL(gl_osYieldThread)();
            }
            a0->f10 = 0;
            CALL(sys_imin)(DB + 0x1E484);
            CALL(gfx_start)(XDATA(DB, port_bootup_bss, 0x3C168));
            CALL(prof_timerStart)(DB + 0x1E490);
            CALL(sys_imout)();
            CALL(sys_imin)(DB + 0x1E494);
            CALL(prof_timerStart)(DB + 0x1E4A0);
            a1->v4();
            CALL(prof_beginSection)(DB + 0x1E4AC);
            CALL(sys_imout)();
            CALL(sys_imin)(DB + 0x1E4B8);
            CALL(prof_timerStart)(DB + 0x1E4C4);
            a1->v3();
            CALL(prof_timerStop)(DB + 0x1E4D0);
            CALL(sys_imout)();
            CALL(sys_imin)(DB + 0x1E4DC);
            CALL(gfx_end)(XDATA(DB, port_bootup_bss, 0x3C168));
            CALL(sys_imout)();
            CALL(prof_timerStop)(DB + 0x1E4E4);
            CALL(prof_timerStop)(DB + 0x1E4F0);
            CALL(sys_imin)(DB + 0x1E4F4);
            CALL(prof_timerStart)(DB + 0x1E500);
            CALL(snd_FrameUpdate)();
            CALL(prof_timerStop)(DB + 0x1E508);
            CALL(sys_imout)();
            a0->count++;
            if (a1->cb) {
                a1->cb();
                a1->cb = 0;
            }
        } while (quit == 0);
        a1->v13();
    }
}

#ifdef PORT
/* PORT (plan 9.6.2): app_run split for the host's fixed-step loop. app_run itself stays as
 * above (its PORT copy is never called). App_main calls app_runInit instead of app_run and
 * returns; port/main.c then calls app_runOneFrame once per 30 Hz step. Differences from the
 * inner loop:
 *  - pad_portFrame() first: the controller read the pad thread did (requested by the RcpMgr
 *    thread every other retrace) completes here, before pad_update reads it;
 *  - RcpMgr_portDrain() after snd_FrameUpdate: the RcpMgr thread's work as a direct call
 *    (gfx_end's 'h' starts the frame's RCP tasks; the VI 'd' messages run the audio frames);
 *  - the trace hook port_trace_frame_end(), keyed by the frame's SysLib.frameCount (the value
 *    pad_update saw: the count before this frame's increment).
 * The outer loop's v13 hook is unreachable on the N64 (quit is never set) and is not called.
 * Globals are reached through the bootup.uso blobs (port_game.h): new functions have no ROM
 * relocations, so the D_00000000 placeholders would bind to nothing. */
#include "port_game.h"
#define PDB port_bootup_data

static St *sPortSys;
static App *sPortApp;

extern "C" void app_runInit(void *sys, void *app) {
    St *a0 = (St *)sys;
    App *a1 = (App *)app;

    sPortSys = a0;
    sPortApp = a1;
    sys_imin((int)(PDB + 0x1E47C));
    if (gHostio_34EB4_flags & 1) hostio_start();
    a0->flags &= ~1;
    a1->v4();
    snd_PlayPlayer0Seq0();
    RcpMgr_portDrain(GFXLIB(PORT_GFXLIB)->rcp); /* boot-time messages; run()'s entry state */
}

extern "C" void app_runOneFrame(void) {
    St *a0 = sPortSys;
    App *a1 = sPortApp;
    RcpMgr *rcp = GFXLIB(PORT_GFXLIB)->rcp;
    Node *n;

    if (a0 == 0) {
        return;
    }
    pad_portFrame();
    for (n = a0->list; n != 0; n = n->next) pad_update(n);
    if (rcp->preNmi != 0) a1->v15();
    while (a0->flags & 4) {
        a0->f10 = 0;
        gl_osYieldThread();
    }
    a0->f10 = 0;
    sys_imin((int)(PDB + 0x1E484));
    gfx_start(PORT_GFXLIB);
    prof_timerStart((int)(PDB + 0x1E490));
    sys_imout();
    sys_imin((int)(PDB + 0x1E494));
    prof_timerStart((int)(PDB + 0x1E4A0));
    a1->v4();
    prof_beginSection((int)(PDB + 0x1E4AC));
    sys_imout();
    sys_imin((int)(PDB + 0x1E4B8));
    prof_timerStart((int)(PDB + 0x1E4C4));
    a1->v3();
    prof_timerStop((int)(PDB + 0x1E4D0));
    sys_imout();
    sys_imin((int)(PDB + 0x1E4DC));
    gfx_end(PORT_GFXLIB);
    sys_imout();
    prof_timerStop((int)(PDB + 0x1E4E4));
    prof_timerStop((int)(PDB + 0x1E4F0));
    sys_imin((int)(PDB + 0x1E4F4));
    prof_timerStart((int)(PDB + 0x1E500));
    snd_FrameUpdate();
    prof_timerStop((int)(PDB + 0x1E508));
    sys_imout();
    RcpMgr_portDrain(rcp);
    a0->count++;
    if (a1->cb) {
        a1->cb();
        a1->cb = 0;
    }
    port_trace_frame_end((unsigned int)(a0->count - 1));
}

extern "C" unsigned int port_game_frame_count(void) {
    return sPortSys != 0 ? (unsigned int)sPortSys->count : 0;
}

extern "C" unsigned int port_game_retrace_count(void) {
    RcpMgr *rcp = GFXLIB(PORT_GFXLIB)->rcp;
    return rcp != 0 ? (unsigned int)rcp->retraceCount : 0;
}

/* The boot chain up to the frame loop. On the N64 the kernel loads bootup.uso and starts
 * boot_main_00066850 (VI / PI managers, SysApi hooks, SnowBoard_ctor -> the app's main ->
 * App_main -> app_run). On the port, port/uso "loads" bootup.uso (static initialisers) and
 * App_main ends in app_runInit, so this returns with the game ready for app_runOneFrame.
 * The kernel's part before boot_main (gSysApi = &gUsoCtx, boot_entry_00066C30) is
 * port_sysapi_install (port/sysapi.c).
 * The RcpMgr thread gets a port_ultra service step (RcpMgr_portStep), so a yield or blocking
 * wait anywhere drains its queue; the pad and fault threads stay parked. */
extern "C" void port_uso_init(void) __attribute__((weak));
extern "C" void port_sysapi_install(void) __attribute__((weak));
extern "C" void port_ultra_register_service(void (*entry)(void *), int (*step)(void *), const char *name);
extern "C" void boot_main_00066850(int);
extern "C" void RcpMgr_update(int);
extern "C" void port_ultra_log(const char *fmt, ...);

extern "C" void port_game_boot(void) {
    /* RcpMgr_ctor starts the thread at D_4D468_a2 = RcpMgr_update (port/gen/aliases.ld) */
    port_ultra_register_service((void (*)(void *))RcpMgr_update, RcpMgr_portStep, "RcpMgr");
    if (port_uso_init != 0) {
        port_uso_init();
    }
    /* gSysApi: the kernel's table (memory range, usoLoad / usoUnload, the ROM file services);
     * after port_uso_init, which clears bootup Bss where gSysApi lives (port/sysapi.c) */
    if (port_sysapi_install != 0) {
        port_sysapi_install();
    } else {
        port_ultra_log("port_game_boot: no port_sysapi_install; boot_main reads gSysApi = 0");
    }
    boot_main_00066850(0);
    if (sPortSys == 0) {
        port_ultra_log("port_game_boot: boot_main returned without reaching app_runInit");
    }
}
#endif
