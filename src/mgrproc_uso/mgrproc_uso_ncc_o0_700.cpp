/* IDO 7.1 NCC (C++) -O0 donors for mgr_initMatchRace_00000700 (0x2A8 / 170
 * words) and mgr_prepareReplay_00000A14 (0xC0 / 48 words): the cfe NM bodies
 * unchanged (700's K&R definition spelled as a prototype). mgrproc is C++;
 * edg -O0 emits no closing-brace b-epilogue, which was the whole cfe residual
 * (+2 words each). Spliced into mgrproc_uso_o0_19C.c.o via REPLACE_FUNC_BODY. */
typedef signed short s16; typedef signed int s32; typedef unsigned char u8; typedef unsigned short u16;
typedef float f32;
#include "../../include/game/modeproc.h" /* ModeFlowProc / MgrCourseList: arg0 is the mgr flow object */
extern "C" {
/* import_800200E4 = GameFlow +0x4C: playerRider (game/input.h). */
extern struct { char pad[0x4C]; int playerRider; } import_800200E4;
/* import_800201EC = GameFlow (bootup Bss 0xF8, game/input.h) +0x154: `options`, the
 * BackupOptions settings record (gdl/backup.h). Local view of its head: this .cpp builds
 * without common.h (agent-h run 509). */
struct GameFlowOptionsHead { unsigned short unlocks; unsigned short seqStep; };
extern struct { char pad[0x154]; struct GameFlowOptionsHead *options; } import_800201EC;
extern char import_8005C6E0;
void mgr_pickRivals_000005D0(char *a0);
int import_000B574C(...);
int mgrproc_uso_D_01F97C(...);
int import_000AE5B0(...);

int mgr_initMatchRace_00000700(ModeFlowProc *flow, int level) {
    register char *base; /* s0; case-1 dead address of import_8005C6E0 */
    flow->courseList->retries = 3;
    flow->courseList->idx = 0;
    switch (level) { /* irregular */
    case 1:
        base = &import_8005C6E0;
        if (*(s32 *)0xA0000284 == 0x240B17D7) {
            flow->courseList->count = 4;
        } else {
            flow->courseList->count = 3;
        }
        flow->courseList->courses[0] = 2;
        flow->courseList->courses[1] = 3;
        flow->courseList->courses[2] = 4;
        flow->courseList->courses[3] = 5;
        mgr_pickRivals_000005D0((char *)flow);
        break;
    case 2:
        flow->courseList->count = 5;
        flow->courseList->courses[0] = 2;
        flow->courseList->courses[1] = 3;
        flow->courseList->courses[2] = 4;
        flow->courseList->courses[3] = 5;
        flow->courseList->courses[4] = 6;
        mgr_pickRivals_000005D0((char *)flow);
        flow->courseList->riders[4] = import_800200E4.playerRider;
        break;
    case 3:
        flow->courseList->count = 6;
        flow->courseList->courses[0] = 2;
        flow->courseList->courses[1] = 3;
        flow->courseList->courses[2] = 4;
        flow->courseList->courses[3] = 5;
        flow->courseList->courses[4] = 6;
        flow->courseList->courses[5] = 7;
        mgr_pickRivals_000005D0((char *)flow);
        flow->courseList->riders[4] = import_800200E4.playerRider;
        if (import_800201EC.options->seqStep == 3) {
            flow->courseList->riders[5] = 5;
        } else if (import_800201EC.options->seqStep == 4) {
            flow->courseList->riders[5] = 6;
        } else {
            flow->courseList->riders[5] = 7;
        }
        break;
    }
#ifdef PORT /* g++ -O2 treats flowing off a non-void function as unreachable: no `ret` was emitted (run 839) */
    return 0;
#endif
}

s32 mgr_prepareReplay_00000A14(s32 *arg0, s32 *arg1) {
    union { s32 arr[8]; s32 head; } buf;
    s32 count;
    s32 acc;
    s32 i;
    register s32 r;

    acc = 0;
    count = import_000B574C(*arg0);
    mgrproc_uso_D_01F97C(*arg0, &buf);
    i = 0;
    if (count > 0)
        do {
            r = import_000AE5B0(buf.arr[i]);
            acc |= r;
        } while (++i < count);
    *arg1 = buf.head;
    return acc;
}
}
