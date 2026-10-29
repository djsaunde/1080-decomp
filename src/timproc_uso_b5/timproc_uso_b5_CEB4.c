/* timproc_uso_b5_func_0000CEB4 (0x1AC / 107 words, real entry 0xCEAC) --
 * three HUD-emit calls (func_00004068) whose RGBA colour is white when
 * a0->0x2AC selects that slot and a0->0x2A0 > 0.5, else grey 0.3.
 *
 * EXACT under IDO 7.1 cfe -O2 (agent-c run 218). Spliced over the unit's
 * empty placeholder by REPLACE_FUNC_BODY (Makefile TIMB5_CEB4_DONOR) so the
 * donor-local .rodata reloc of the three 0.3f literals (+0 / +4 / +8) is
 * renamed timproc_uso_b5_func_0000CEB4_rodata and pinned to RoData +0x394
 * (the USO ships 0x3E99999A at 0x394 / 0x398 / 0x39C: one literal per l.s).
 *
 * Load-bearing:
 *  - the grey is the LITERAL 0.3f, not a RoData extern: a memory load stays
 *    in program order after the 0x2AC load, the literal is a constant uopt
 *    hoists with 1.0f into the test block (target: 0.3 -> f0, 1.0 -> f2).
 *  - `if (1) {}` closing each white arm: the extra block puts the 0.3f
 *    constant ahead of 1.0f (colour and emission order), and in block 1 it
 *    keeps the target's `bc1f else; b join` shape with the join's `lw a1`
 *    in the delay slot (the white stores are redundant with the init and
 *    vanish).
 *  - typing func_00004068's colour param as `f32 *` (agent-g run 106) and
 *    `pad[6]` after v (0x50 frame, v at 0x40). */
#include "common.h"

extern char timproc_uso_b5_D_807FFB08;
extern char timproc_uso_b5_D_807FFB20;
extern char timproc_uso_b5_D_807FFB38;
void timproc_uso_b5_func_00004068(int a0, int a1, int a2, f32 *a3, int arg5, int arg6);

void timproc_uso_b5_func_0000CEB4(char *a0, int a1) {
    f32 v[4];
    s32 pad[6];
    f32 f;
    v[0] = 1.0f; v[1] = 1.0f; v[2] = 1.0f; v[3] = 1.0f;
    if (*(s32 *)(a0 + 0x2AC) == 0 && 0.5f < *(f32 *)(a0 + 0x2A0)) {
        v[0] = 1.0f; v[1] = 1.0f; v[2] = 1.0f; v[3] = 1.0f;
        if (1) {}
    } else {
        f = 0.3f;
        v[0] = f; v[1] = f; v[2] = f; v[3] = f;
    }
    timproc_uso_b5_func_00004068((int)a0, *(s32 *)(a0 + 0x44), *(s32 *)(a0 + 0x5C), v, (int)(&timproc_uso_b5_D_807FFB08 + 0x688), 0xFF);
    f = 0.3f;
    if (*(s32 *)(a0 + 0x2AC) == 1 && 0.5f < *(f32 *)(a0 + 0x2A0)) {
        v[0] = 1.0f; v[1] = 1.0f; v[2] = 1.0f; v[3] = 1.0f;
        if (1) {}
    } else {
        v[0] = f; v[1] = f; v[2] = f; v[3] = f;
    }
    timproc_uso_b5_func_00004068((int)a0, *(s32 *)(a0 + 0xD4), *(s32 *)(a0 + 0xEC), v, (int)(&timproc_uso_b5_D_807FFB20 + 0x6A0), 0xFF);
    f = 0.3f;
    if (*(s32 *)(a0 + 0x2AC) == 2 && 0.5f < *(f32 *)(a0 + 0x2A0)) {
        v[0] = 1.0f; v[1] = 1.0f; v[2] = 1.0f; v[3] = 1.0f;
        if (1) {}
    } else {
        v[0] = f; v[1] = f; v[2] = f; v[3] = f;
    }
    timproc_uso_b5_func_00004068((int)a0, *(s32 *)(a0 + 0x104), *(s32 *)(a0 + 0x11C), v, (int)(&timproc_uso_b5_D_807FFB38 + 0x6B8), 0xFF);
}
