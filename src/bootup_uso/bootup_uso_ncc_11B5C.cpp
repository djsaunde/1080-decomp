/* IDO 7.1 NCC (C++) -O0 donor for input_pauseRacePads: the NM C body of
 * bootup_uso_tail3a_mid.c compiled as C++ (bootup_uso is C++). cfe -O0 left a dead
 * closing-brace branch the shipped code lacks; edg's -O0 ucode has none.
 * Spliced into bootup_uso_tail3a_mid.c.o via REPLACE_FUNC_BODY. */
typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;
typedef signed char s8; typedef signed short s16; typedef signed int s32; typedef float f32; typedef double f64;
#include "../../include/game/input.h"  /* InputMgr (GameFlow) */
#include "../../include/uso_call.h"  /* NCC donors build without -I include */
extern "C" {
extern int func_00000000(...);

/* Mid-tail assembly gap after GameFlow_addPads_00011AB4 was split to an -O0 file.
 * Holds 0x11B5C..0x11C70 so the following o0_11C70 split keeps its offset. */
/* input_pauseRacePads: 37-insn -O0 flag-set + per-element-1-call loop, returns the
 * old _188 flag (saves it, sets _188=1, calls helper(arg,1) per item).
 * CAP: -O0 return-value DEAD-double-b toolchain gap. Identical structure to
 * the LANDED void sibling input_unpauseRacePads below (if-guard + do-while) — the ONLY
 * difference is this one returns a value, so our cc emits `lw v0; b epi; nop;
 * b epi; nop` (second b dead/unreachable) where the target has ONE b -> built
 * .o is +8 bytes, never matches. Proven a toolchain-binary codegen gap (trivial
 * `int f(int*a){int x=a[0];g();return x;}` reproduces it; no -g/-O level or IDO
 * 5.3-vs-7.1 flips it; void -O0 fns are unaffected, hence BF0 lands). NOT
 * C-fixable, NOT permuter-fixable; stripping the dead b = match-faking (banned).
 * See docs/IDO_CODEGEN.md#feedback-ido-o0-return-value-dead-double-b. */
s32 input_pauseRacePads(s32 *a0) {
    s32 saved = GAMEFLOW(a0)->racePadsPaused;
    s32 i;
    GAMEFLOW(a0)->racePadsPaused = 1;
    i = 0;
    if (GAMEFLOW(a0)->numRacePads > 0) {
        do {
            FCALL(pad_setPause_00067470)(GAMEFLOW(a0)->racePads[i], 1);
        } while (++i < GAMEFLOW(a0)->numRacePads);
    }
    return saved;
}

void input_unpauseRacePads(char *a0) {
    s32 i;

    i = 0;
    if (GAMEFLOW(a0)->numRacePads > 0) {
        do {
            FCALL(pad_setPause_00067470)(GAMEFLOW(a0)->racePads[i], 0);
        } while (++i < GAMEFLOW(a0)->numRacePads);
    }
    GAMEFLOW(a0)->racePadsPaused = 0;
}
}
