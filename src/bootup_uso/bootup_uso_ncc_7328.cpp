/* IDO 7.1 NCC (C++) donor for EnvSettings_load (0x1C0 / 112 words): drain the
 * message queue -- for each message look the id up in the handler list (stack
 * {cur, nxt} iterator, the same class as game_libs 38964 / 3D48C) and fire
 * the handler's virtual slot 8, else report + reply; then the flag tail.
 *
 * Load-bearing (cfe NM was 66.6):
 * - the list iterator's next() advances through its own nxt member,
 *   `(cur = nxt) ? (nxt = nxt->next, cur->data) : 0`;
 * - Entry::matches(Msg) takes the message BY VALUE (the per-iteration copy
 *   through the s1 = sp+52 pointer);
 * - frame 88: msg/out/found at function scope (found above the iterator),
 *   the handler call without a named local, `mode` in its own block (a
 *   function-scope `mode` costs a slot above the iterator);
 * - tail: the |= 2 before the 800 store (ring t5/t3/t4).
 * Spliced into bootup_uso.c.o via REPLACE_FUNC_BODY. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef float f32; typedef double f64;
#include "../../include/game/course.h"
#include "../../include/game/envobj.h" /* EnvSettings (handlers / clipNear / clipFar / flags) */
#include "../../include/game/input.h"  /* GameFlow (InputMgr) */
#include "../../include/gdl/gfxlib.h"  /* GfxLib.fog */
#include "../../include/uso_call.h"  /* NCC donors build without -I include */
extern "C" {
void func_00000000(...);
void stream_readS32_00005EF8(void *);
void stream_readS32_0000502C(void *);
void stream_readS32_00004FF0(void *);
extern char D_00000000, sFmt_Env_Load_7F90, sFmt_Env_Skip_7FA0;
extern char func_0000027C[], func_00000008[], func_0000029C[];
}
/* The relocs (uso-data-sites.py sites EnvSettings_load): func_00000008 + 0x34 = GameFlow (bootup Bss
 * 0xF8, sym272) +0x3C, func_0000029C + 4 = GfxLib (sym154) +0x2A0 = fog.colour[3]; the symbols and
 * addends stay the target's (agent-g run 580). */
#define GAMEFLOW_7328 GAMEFLOW(func_00000008 - 8)
#define GFX_7328 GFXLIB(func_0000029C - 0x29C)
struct Msg73 { int id; };
struct Handler73 { char pad[0x28]; virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7(); virtual void handle(); };
struct Entry73 { Handler73 *h; int id; int matches(Msg73 m) { return m.id == id; } };
struct Link73 { Entry73 *data; Link73 *next; };
struct Iter73 {
    Link73 *cur; Link73 *nxt;
    void start(Link73 *h) { nxt = h; }
    Entry73 *next() { return (cur = nxt) ? (nxt = nxt->next, cur->data) : 0; }
    Entry73 *get() { return cur->data; }
};
extern "C" void EnvSettings_load(char *self) {
    Msg73 msg;
    int out;
    Entry73 *found;
    for (;;) {
        msg.id = 0;
        stream_readS32_00005EF8(&msg);
        if (msg.id == 0) break;
        Iter73 it;
        stream_readS32_0000502C(&out);
        found = 0;
        it.start(*(Link73 **)&ENVSET(self)->handlers);
        while (it.next() != 0) {
            if (it.get()->matches(msg)) {
                found = it.get();
                break;
            }
        }
        if (found != 0) {
            FCALL(dbg_printf)(&sFmt_Env_Load_7F90, &found->id);
            Handler73 *h = found->h;
            h->handle();
        } else {
            FCALL(dbg_printf)(&sFmt_Env_Skip_7FA0, &msg);
            FCALL(fs_gdlSeekFile)(&D_00000000, 1, out);
        }
    }
    stream_readS32_0000502C(&ENVSET(self)->flags);
    if (ENVSET(self)->flags & 1) {
        FCALL(gfx_loadQuadIntInt_00044034)(func_0000027C + 0x18);
    }
    if (ENVSET(self)->flags & 2) {
        stream_readS32_00004FF0(&ENVSET(self)->clipNear);
        stream_readS32_00004FF0(&ENVSET(self)->clipFar);
    }
    { int mode = GAMEFLOW_7328->gameMode;
    if (mode == 6 || mode == 11) {
        GFX_7328->fog.colour[3] = 1.0f; /* fog alpha */
        ENVSET(self)->flags |= 2;
        ENVSET(self)->clipFar = 800;
    } }
}
