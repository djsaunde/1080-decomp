/* SelectWin_update_00005FC0 (0x3D4 / 245 words) -- timing-screen per-frame
 * update: 0x3BC/0x3C8 record-slot select into import_000A7ECC, an 11-state
 * jumptable dispatch on scr->0x3CC (handlers 7078..85E0), the 0x4BC fade-in
 * ramp, the 0x3FC countdown, four vtable +0xBC/+0xB8 element updates, then
 * the state-5 / other FP placement (0000396C / 000038B0) and the 0x14C blend.
 *
 * IDO 7.1 -O2 donor (agent-g run 114), 245/245 words. Spliced into
 * timproc_uso_b5.c.o by REPLACE_FUNC_BODY only so the switch's donor-local
 * .rodata jumptable reloc is renamed SelectWin_update_00005FC0_rodata and
 * pinned to the table the USO ships at RoData +0x190 (the 29CCC recipe); an
 * in-unit compile would put a second table in the TU .rodata.
 *
 * What fixed the old 96.9 NM body: the float->float helper 07E7B0 takes a
 * PROTOTYPED float (the K&R `f32 fn()` declaration promoted every argument to
 * double: cvt.d.s before each call), `+=` for the 0x4BC ramp, the
 * 0x498 updates written base-first (`0x49C + fn(..) * 0x14C`), and the final
 * blend `0x4C0 + (0x498 - 0x4C0) * fn(..)` without the named f2 local (which
 * IDO spilled across the call: +8 frame). IDO 7.1 cfe emits the add/mul
 * operands in the reverse of the source order here.
 * Readability (agent-c run 490): the SelectWin scalars are SELWIN fields, except the
 * three `K * *(f32 *)(arg0 + 0x4BC)` (transition) operands of the 07E7B0 calls: as a
 * member read the lwc1 pair swaps f16/f18 and f6/f8. fade (0x480) is passed as its
 * int bits and the pointer fields stay FW reads. */
#include "common.h"
#include "game/select.h"

#define FW(p, o) (*(int *)((char *)(p) + (o)))

extern char D_00000000;
extern char import_80020098;
extern char import_800200CC;
extern void import_000A7ECC();
extern void SelectWin_state0_00007078();
extern void SelectWin_state1_000072D0();
extern void SelectWin_state3_00007430();
extern void SelectWin_state4_000077D8();
extern void SelectWin_state5_000079A4();
extern void SelectWin_state6_00007B2C();
extern void SelectWin_state7_00007E34();
extern void SelectWin_state8_000080F4();
extern void SelectWin_state9_00008468();
extern f32 timproc_uso_b5_func_07E7B0(f32);
int SelectionMgr_otherWin_00001CF0(int *a0, int a1);
int SelectWin_curItem_00008A38();
int SelectWin_prevFocusItem_00008AD4(int *a0);
void SelectWin_state2_000073C0(char *scr);
void SelectWin_state10_000085E0(int *a0);
void SelectWin_lerpCamToItem_000038B0(char *arg0, s32 arg1, char *arg2, f32 arg3, f32 arg4);
void SelectWin_lerpCamToItemZoom_0000396C(char *arg0, s32 arg1, char *arg2, f32 arg3, f32 arg4);

typedef void (*FPv)();

void SelectWin_update_00005FC0(char *arg0) {
    char *sp2C;
    char *v0;
    char *v1;

    if (SELWIN(arg0)->numPlayers == 2) {
        if ((SELWIN(arg0)->stageBit == 4) &&
            (FW(SelectionMgr_otherWin_00001CF0((int *)FW(arg0, 0x41C), SELWIN(arg0)->player), 0x3C8) == 4)) {
            import_000A7ECC(&import_80020098, -1);
        } else {
            import_000A7ECC(&import_80020098, SELWIN(arg0)->player);
        }
    } else {
        import_000A7ECC(&import_80020098, SELWIN(arg0)->player);
    }
    switch ((u32)SELWIN(arg0)->state) {
    case 0:
        SelectWin_state0_00007078(arg0);
        break;
    case 1:
        SelectWin_state1_000072D0(arg0);
        break;
    case 2:
        SelectWin_state2_000073C0(arg0);
        break;
    case 3:
        SelectWin_state3_00007430(arg0);
        break;
    case 4:
        SelectWin_state4_000077D8(arg0);
        break;
    case 5:
        SelectWin_state5_000079A4(arg0);
        break;
    case 6:
        SelectWin_state6_00007B2C(arg0);
        break;
    case 7:
        SelectWin_state7_00007E34(arg0);
        break;
    case 8:
        SelectWin_state8_000080F4(arg0);
        break;
    case 9:
        SelectWin_state9_00008468(arg0);
        break;
    case 10:
        SelectWin_state10_000085E0(arg0);
        break;
    }
    if (SELWIN(arg0)->busy != 0) {
        SELWIN(arg0)->transition += SELWIN(arg0)->parms.msp.value;
        if (SELWIN(arg0)->transition >= 1.0f) {
            SELWIN(arg0)->transition = 1.0f;
            SELWIN(arg0)->busy = 0;
            SELWIN(arg0)->stageLock = 0;
        }
    }
    if (SELWIN(arg0)->transCountdown != 0) {
        SELWIN(arg0)->transCountdown = SELWIN(arg0)->transCountdown - 4;
        if (SELWIN(arg0)->transCountdown <= 0) {
            SELWIN(arg0)->transCountdown = 0;
        }
    }
    if (SELWIN(arg0)->prevItemActive != 0) {
        v0 = (char *)SelectWin_prevFocusItem_00008AD4(arg0);
        v1 = (char *)FW(v0, 0x28);
        ((FPv)FW(v1, 0xBC))(*(s16 *)(v1 + 0xB8) + v0, FW(arg0, 0x480));
    }
    v0 = (char *)SelectWin_curItem_00008A38(arg0);
    v1 = (char *)FW(v0, 0x28);
    ((FPv)FW(v1, 0xBC))(*(s16 *)(v1 + 0xB8) + v0, FW(arg0, 0x480));
    v1 = (char *)FW(FW(arg0, 0x40C), 0x54);
    v0 = (char *)FW(v1, 0x28);
    ((FPv)FW(v0, 0xBC))(*(s16 *)(v0 + 0xB8) + v1, FW(arg0, 0x480));
    v0 = (char *)FW(FW(arg0, 0x40C), 0x58);
    v1 = (char *)FW(v0, 0x28);
    ((FPv)FW(v1, 0xBC))(*(s16 *)(v1 + 0xB8) + v0, FW(arg0, 0x480));
    if (*(s32 *)(&import_800200CC + 0x34) != 2) {
        v0 = (char *)FW(FW(arg0, 0x40C), 0x5C);
        v1 = (char *)FW(v0, 0x28);
        ((FPv)FW(v1, 0xBC))(*(s16 *)(v1 + 0xB8) + v0, FW(arg0, 0x480));
    }
    if (SELWIN(arg0)->state == 5) {
        sp2C = (char *)SelectWin_curItem_00008A38(arg0);
        SelectWin_lerpCamToItemZoom_0000396C(sp2C, FW(FW(arg0, 0x414), 0x10), arg0 + 0x4C4,
            timproc_uso_b5_func_07E7B0(*(f32 *)(&D_00000000 + 0x1BC) * *(f32 *)(arg0 + 0x4BC)),
            SELWIN(arg0)->parms.voy.value);
        sp2C = (char *)SelectWin_curItem_00008A38(arg0);
        SELWIN(arg0)->fovTarget =
            SELWIN(arg0)->fovBase +
            timproc_uso_b5_func_07E7B0(*(f32 *)(&D_00000000 + 0x1C0) * *(f32 *)(arg0 + 0x480)) *
            *(f32 *)(sp2C + 0x14C);
    } else {
        sp2C = (char *)SelectWin_curItem_00008A38(arg0);
        SelectWin_lerpCamToItem_000038B0(sp2C, FW(FW(arg0, 0x414), 0x10), arg0 + 0x4C4,
            timproc_uso_b5_func_07E7B0(*(f32 *)(&D_00000000 + 0x1C4) * *(f32 *)(arg0 + 0x4BC)),
            SELWIN(arg0)->parms.voy.value);
        sp2C = (char *)SelectWin_curItem_00008A38(arg0);
        SELWIN(arg0)->fovTarget =
            SELWIN(arg0)->fovBase +
            timproc_uso_b5_func_07E7B0(*(f32 *)(&D_00000000 + 0x1C8) * *(f32 *)(arg0 + 0x480)) *
            *(f32 *)(sp2C + 0x134);
    }
    *(f32 *)(FW(FW(FW(arg0, 0x414), 0xC), 0x70) + 0x14C) =
        SELWIN(arg0)->fovFrom +
        (SELWIN(arg0)->fovTarget - SELWIN(arg0)->fovFrom) *
        timproc_uso_b5_func_07E7B0(*(f32 *)(&D_00000000 + 0x1CC) * *(f32 *)(arg0 + 0x4BC));
}
