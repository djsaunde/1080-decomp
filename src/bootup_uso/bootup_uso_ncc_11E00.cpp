/* IDO 7.1 NCC (C++) -O0 donors for input_checkPressed_00011E00 / input_checkHeld_00011ED4 / input_checkReleased_00011FA8
 * (0xD4 / 53 words each): "first slot whose mask word (+0x18 / +0x10 / +0x1C)
 * has an a1 bit" (slot[] = a0 + 0xE0, a0->[0x128] == -1 scans all a0->[0x120]
 * slots). bootup_uso is C++: under cfe -O0 every fn carried an extra dead
 * `b epi; nop` pair before the epilogue (the closing-brace marker of the
 * return-value dead-double-b gap); edg's -O0 ucode has no such marker, so the
 * same body compiles to the shipped 53 words.
 * Spliced into bootup_uso_o0_11E00.c.o via REPLACE_FUNC_BODY.
 * agent-g run 470: field form over include/game/input.h (InputMgr / InputPad); same bytes. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef float f32; typedef double f64;
#include "../../include/game/input.h"
extern "C" int input_checkPressed_00011E00(register InputMgr *a0, register int a1) {
    int i;
    register InputPad *p;
    if (a0->locked == 0) {
        if (a0->activePad == -1) {
            i = 0;
            if (a0->numPads > 0) do {
                p = a0->pads[i];
                if (p->pressed & a1) return 1 << i;
            } while (++i < a0->numPads);
            goto ret0;
        }
        p = a0->pads[a0->activePad];
        if (p->pressed & a1) return 1 << a0->activePad;
    }
ret0:
    return 0;
}

extern "C" int input_checkHeld_00011ED4(register InputMgr *a0, register int a1) {
    int i;
    register InputPad *p;
    if (a0->locked == 0) {
        if (a0->activePad == -1) {
            i = 0;
            if (a0->numPads > 0) do {
                p = a0->pads[i];
                if (p->held & a1) return 1 << i;
            } while (++i < a0->numPads);
            goto ret0;
        }
        p = a0->pads[a0->activePad];
        if (p->held & a1) return 1 << a0->activePad;
    }
ret0:
    return 0;
}

extern "C" int input_checkReleased_00011FA8(register InputMgr *a0, register int a1) {
    int i;
    register InputPad *p;
    if (a0->locked == 0) {
        if (a0->activePad == -1) {
            i = 0;
            if (a0->numPads > 0) do {
                p = a0->pads[i];
                if (p->released & a1) return 1 << i;
            } while (++i < a0->numPads);
            goto ret0;
        }
        p = a0->pads[a0->activePad];
        if (p->released & a1) return 1 << a0->activePad;
    }
ret0:
    return 0;
}
