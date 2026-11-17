#include "../../include/uso_call.h"  /* NCC donors build without -I include */
/* IDO 7.1 NCC (C++) EXACT donor for snd_TickChannel8Fade (0x104 / 65 words), agent-c run 177 / agent-g run 202.
 * Fade timer tick over the 8-word state block that the exact sibling 308AC (game_libs_ncc_308AC.cpp) reads as a
 * FILE-LOCAL static: as1 shares one `lui at` between two accesses only when both hit the same LOCAL symbol and
 * uopt has not CSE'd the address. So the two target pairs -- the zeroing stores `lui at; sw zero,12(at);
 * sw zero,8(at)` and the down-arm loads `lui at; lwc1 $f0,16(at); lwc1 $f2,20(at)` -- are two references to
 * fields of the static A308 in ONE block, and every other access is a per-site extern alias (D_307A8_x, the cfe
 * body's 349E0 alias-budget rule) so no address is CSE'd into v0. This removes the two extra `lui at` words the
 * cfe body could not lose (68 -> 65 words = target). The `while (0) { f = c + c; }` / `if (c) {}` keep-alives
 * are the cfe body's (load order of the up arm; c live into the else arm).
 * agent-g run 202: the last residual (hold count a uopt candidate in v1 where the target has the ugen ring t8 / t9)
 * goes with the decrement's re-read spelled `(&gSndCueQueues_307A8_i.fC)[0]`: uopt no longer CSEs it with the test's load
 * (different ucode shape, so no live range), while ugen still sees the same `lw 12(sym)` already in t8 from the
 * fall-through predecessor and reuses it -- one load, t8 / t9, word-exact (plain `gSndCueQueues_307A8_i.fC`, a volatile or
 * unsigned re-read, the static block and `((int *)&gSndCueQueues_307A8_i)[3]` all keep the candidate or reload).
 * Spliced into game_libs_post.c.o (build and non_matching); the .bss reloc to A308 is renamed
 * snd_TickChannel8Fade_bss by the splice (pinned to 0, the 308AC recipe). */
typedef float f32;
typedef struct { int mode; int f4; int f8; int fC; f32 f10; f32 f14; f32 f18; f32 f1C; } St308;
static St308 A308;
extern "C" St308 gSndCueQueues_307A8_a, gSndCueQueues_307A8_d, gSndCueQueues_307A8_e, gSndCueQueues_307A8_f, gSndCueQueues_307A8_g, gSndCueQueues_307A8_h, gSndCueQueues_307A8_i, gSndCueQueues_307A8_j,
    gSndCueQueues_307A8_m, gSndCueQueues_307A8_n, gSndCueQueues_307A8_o, gSndCueQueues_307A8_p;
extern "C" int D_00000000_1c0a8;
extern "C" void gl_func_00000000_f(int, f32);
#ifdef PORT /* one object for the bytes these tokens share, so gcc keeps the order (scripts/port-alias-audit.py, plan 18.7) */
extern "C" char port_bootup_bss[];
#define gSndCueQueues_307A8_d (*(__typeof__(gSndCueQueues_307A8_d) *)(port_bootup_bss + 0x338))
#define gSndCueQueues_307A8_e (*(__typeof__(gSndCueQueues_307A8_e) *)(port_bootup_bss + 0x338))
#define gSndCueQueues_307A8_g (*(__typeof__(gSndCueQueues_307A8_g) *)(port_bootup_bss + 0x338))
#define gSndCueQueues_307A8_h (*(__typeof__(gSndCueQueues_307A8_h) *)(port_bootup_bss + 0x338))
#define gSndCueQueues_307A8_i (*(__typeof__(gSndCueQueues_307A8_i) *)(port_bootup_bss + 0x338))
#define gSndCueQueues_307A8_j (*(__typeof__(gSndCueQueues_307A8_j) *)(port_bootup_bss + 0x338))
#define gSndCueQueues_307A8_n (*(__typeof__(gSndCueQueues_307A8_n) *)(port_bootup_bss + 0x338))
#define gSndCueQueues_307A8_o (*(__typeof__(gSndCueQueues_307A8_o) *)(port_bootup_bss + 0x338))
#define gSndCueQueues_307A8_p (*(__typeof__(gSndCueQueues_307A8_p) *)(port_bootup_bss + 0x338))
#endif
extern "C" void snd_TickChannel8Fade(void) {
    int c;
    f32 f;
    f32 fl;
    if (gSndCueQueues_307A8_a.mode == 0) {
        if (D_00000000_1c0a8 != 5) {
            return;
        }
        { int &r = A308.fC; int &q = A308.f8; r = 0; q = 0; }
    }
    c = gSndCueQueues_307A8_d.f8;
    if (c != 0) {
        f = gSndCueQueues_307A8_e.f10; while (0) { f = c + c; }
        f += gSndCueQueues_307A8_f.f18;
        c -= 1;
        gSndCueQueues_307A8_g.f8 = c;
        if (1.0f < f) {
            f = 1.0f;
        }
        gSndCueQueues_307A8_h.f10 = f;
    } else if (gSndCueQueues_307A8_i.fC != 0) {
        gSndCueQueues_307A8_j.fC = (&gSndCueQueues_307A8_i.fC)[0] - 1;
    } else {
        { f32 &r = A308.f10; f32 &q = A308.f14; f = r; fl = q; } if (c) {}
        f -= gSndCueQueues_307A8_m.f1C;
        gSndCueQueues_307A8_n.f10 = f;
        if (f < fl) {
            gSndCueQueues_307A8_o.f10 = fl;
        }
    }
    f = gSndCueQueues_307A8_p.f10;
    XCALL(gl_func_00000000_f, AudioThread_QueueCmdF32)(0x01000800, PORT_IBITS(f));
}
#ifdef PORT
#undef gSndCueQueues_307A8_d
#undef gSndCueQueues_307A8_e
#undef gSndCueQueues_307A8_g
#undef gSndCueQueues_307A8_h
#undef gSndCueQueues_307A8_i
#undef gSndCueQueues_307A8_j
#undef gSndCueQueues_307A8_n
#undef gSndCueQueues_307A8_o
#undef gSndCueQueues_307A8_p
#endif
