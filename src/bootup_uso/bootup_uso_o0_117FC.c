#include "common.h"
#include "gdl/gl_data.h"
#include "game/input.h"

/* -O0 sub-file covering bootup_uso offset 0x117FC..0x118E3 (single function,
 * 58 insns, 0xE8 bytes), split from the tail of bootup_uso_tail3a.c
 * (-O2 -g3) on 2026-07-10. The function is an -O0 island member (8 unfilled
 * jal delays, memory-homed args, dead `b .+1` closing-brace branch); the old
 * tail3a note "no standard opt level reproduces it" was based on a
 * standalone-cc probe of a WRONG C body (early-return locals form). */

extern int func_00000000();
extern char D_00000000;

/* GameFlow_loadProcUso_000117FC: handle replace-with-validation + re-init (structural
 * decode retained in bootup_uso_tail3a.c history).
 * MATCHED 2026-07-10 (w50 island sweep, 58/58, zero diffs pre-link).
 * Key -O0 shapes: (1) the validate guard is a FUSED-|| wrapper around the
 * whole body: `if (chain == 0 || validate(a1, chain) != 0) { ... }` — the
 * short-circuit branches emit DIRECT (beqz t7 -> body, beqz v0 -> end, no
 * materialized `b`; the &&-goto spelling adds bnez+b = +2 words), and cfe
 * DAG-shares the 0x2C load between the null test and the call arg
 * (`or a1,t7` — same DAG-share lever as F6C4/FEA0);
 * (2) the free block re-tests the slot as a SEPARATE statement (fresh t0
 * reload, no share); (3) h is the only named local (slot 0x1C). */
void GameFlow_loadProcUso_000117FC(char *a0, void *a1) {
    int h;

    if (GAMEFLOW(a0)->procUsoPath == 0 ||
        func_00000000(a1, GAMEFLOW(a0)->procUsoPath) != 0) {
        if (GAMEFLOW(a0)->procUsoPath != 0) {
            func_00000000(&gSysLib, GAMEFLOW(a0)->procUsoPath);
            func_00000000(3);
        }
        h = func_00000000(3);
        func_00000000(&gSnowBoard, 1);
        func_00000000(&gSysLib, a1);
        func_00000000(&gSnowBoard, 0);
        func_00000000(h);
        GAMEFLOW(a0)->procUsoPath = a1;
    }
}
