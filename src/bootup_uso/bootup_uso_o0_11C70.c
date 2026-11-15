#include "common.h"
#include "game/input.h"

extern int func_00000000();

/* -O0 sub-file split out of bootup_uso_tail3a.c on 2026-05-07. Holds the
 * 0x11C70..0x11D40 -O0 cluster: 11C70/11CA4 (append-to-array helpers,
 * register-typed-local recipe) + 11CD8 (if-else flag-setter, full
 * arg-spill).
 *
 * Pattern: minimal -O0 frame, register-typed locals for leaf, full
 * arg-spill+reload for non-leaf, b .+1 dead-branch BB markers. */

/* input_addPad_00011C70: append-to-array helper. a0->0x120 is the count, the array
 * is at a0->0xE0 (4-byte entries). Decl order matters — `idx` declared
 * BEFORE `p` so that the local pointer copy lands in $a3 (higher arg slot)
 * via IDO -O0's first-allocated-reg-local pick. Body byte-exact at -O0
 * (verified 2026-05-02 and re-verified 2026-05-07). Field form over game/input.h: agent-g run 470. */
void input_addPad_00011C70(InputMgr *a0, InputPad *a1) {
    register s32 idx;
    register InputMgr *p = a0;
    idx = p->numPads;
    p->numPads = idx + 1;
    a0->pads[idx] = a1;
}

/* Sibling of input_addPad_00011C70: same append-to-array shape with offsets 0x124
 * (count) + 0x100 (4-byte entries). Verified byte-exact at -O0 with the
 * same `register s32 idx; register s32 *p = a0;` decl-order recipe
 * (2026-05-07). */
void input_addRacePad(InputMgr *a0, InputPad *a1) {
    register s32 idx;
    register InputMgr *p = a0;
    idx = p->numRacePads;
    p->numRacePads = idx + 1;
    a0->racePads[idx] = a1;
}

/* 26-insn -O0 if-else flag-setter. Sets a0->0x188 to 0 if a1 != 0 else 1.
 * Both branches call func_00000000(a0) (cross-USO placeholder). Verified
 * byte-exact at -O0 standalone (2026-05-07). */
void input_setRacePadsActive(int *a0, int a1) {
    if (a1 != 0) {
        func_00000000(a0);
        *(int*)((char*)a0 + 0x188) = 0;
    } else {
        func_00000000(a0);
        *(int*)((char*)a0 + 0x188) = 1;
    }
}
