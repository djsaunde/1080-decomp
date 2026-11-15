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
#define FW(p, o) (*(int *)((char *)(p) + (o)))
struct Pad40 { char pad[0x28]; };
struct AppB : Pad40 {
    virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4();
    virtual void v5(); virtual void v6(); virtual void v7(); virtual void v8();
    virtual void v9(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15();
};
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
            if (FW(FW(DB, 0x218), 0x48) != 0) a1->v15();
            while (a0->flags & 4) {
                a0->f10 = 0;
                CALL(gl_osYieldThread)();
            }
            a0->f10 = 0;
            CALL(sys_imin)(DB + 0x1E484);
            CALL(gfx_start)(DB);
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
            CALL(gfx_end)(DB);
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
