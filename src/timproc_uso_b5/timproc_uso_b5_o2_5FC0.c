/* timproc_uso_b5_func_00005FC0 (0x3D4 / 245 words) -- timing-screen per-frame
 * update: 0x3BC/0x3C8 record-slot select into import_000A7ECC, an 11-state
 * jumptable dispatch on scr->0x3CC (handlers 7078..85E0), the 0x4BC fade-in
 * ramp, the 0x3FC countdown, four vtable +0xBC/+0xB8 element updates, then
 * the state-5 / other FP placement (0000396C / 000038B0) and the 0x14C blend.
 *
 * IDO 7.1 -O2 donor (agent-g run 114), 245/245 words. Spliced into
 * timproc_uso_b5.c.o by REPLACE_FUNC_BODY only so the switch's donor-local
 * .rodata jumptable reloc is renamed timproc_uso_b5_func_00005FC0_rodata and
 * pinned to the table the USO ships at RoData +0x190 (the 29CCC recipe); an
 * in-unit compile would put a second table in the TU .rodata.
 *
 * What fixed the old 96.9 NM body: the float->float helper 07E7B0 takes a
 * PROTOTYPED float (the K&R `f32 fn()` declaration promoted every argument to
 * double: cvt.d.s before each call), `+=` for the 0x4BC ramp, the
 * 0x498 updates written base-first (`0x49C + fn(..) * 0x14C`), and the final
 * blend `0x4C0 + (0x498 - 0x4C0) * fn(..)` without the named f2 local (which
 * IDO spilled across the call: +8 frame). IDO 7.1 cfe emits the add/mul
 * operands in the reverse of the source order here. */
#include "common.h"

#define FW(p, o) (*(int *)((char *)(p) + (o)))

extern char D_00000000;
extern char import_80020098;
extern char import_800200CC;
extern void import_000A7ECC();
extern void timproc_uso_b5_func_00007078();
extern void timproc_uso_b5_func_000072D0();
extern void timproc_uso_b5_func_00007430();
extern void timproc_uso_b5_func_000077D8();
extern void timproc_uso_b5_func_000079A4();
extern void timproc_uso_b5_func_00007B2C();
extern void timproc_uso_b5_func_00007E34();
extern void timproc_uso_b5_func_000080F4();
extern void timproc_uso_b5_func_00008468();
extern f32 timproc_uso_b5_func_07E7B0(f32);
int timproc_uso_b5_func_00001CF0(int *a0, int a1);
int timproc_uso_b5_func_00008A38();
int timproc_uso_b5_func_00008AD4(int *a0);
void timproc_uso_b5_func_000073C0(char *scr);
void timproc_uso_b5_func_000085E0(int *a0);
void timproc_uso_b5_func_000038B0(char *arg0, s32 arg1, char *arg2, f32 arg3, f32 arg4);
void timproc_uso_b5_func_0000396C(char *arg0, s32 arg1, char *arg2, f32 arg3, f32 arg4);

typedef void (*FPv)();

void timproc_uso_b5_func_00005FC0(char *arg0) {
    char *sp2C;
    char *v0;
    char *v1;

    if (FW(arg0, 0x3BC) == 2) {
        if ((FW(arg0, 0x3C8) == 4) &&
            (FW(timproc_uso_b5_func_00001CF0((int *)FW(arg0, 0x41C), FW(arg0, 0x3B8)), 0x3C8) == 4)) {
            import_000A7ECC(&import_80020098, -1);
        } else {
            import_000A7ECC(&import_80020098, FW(arg0, 0x3B8));
        }
    } else {
        import_000A7ECC(&import_80020098, FW(arg0, 0x3B8));
    }
    switch ((u32)FW(arg0, 0x3CC)) {
    case 0:
        timproc_uso_b5_func_00007078(arg0);
        break;
    case 1:
        timproc_uso_b5_func_000072D0(arg0);
        break;
    case 2:
        timproc_uso_b5_func_000073C0(arg0);
        break;
    case 3:
        timproc_uso_b5_func_00007430(arg0);
        break;
    case 4:
        timproc_uso_b5_func_000077D8(arg0);
        break;
    case 5:
        timproc_uso_b5_func_000079A4(arg0);
        break;
    case 6:
        timproc_uso_b5_func_00007B2C(arg0);
        break;
    case 7:
        timproc_uso_b5_func_00007E34(arg0);
        break;
    case 8:
        timproc_uso_b5_func_000080F4(arg0);
        break;
    case 9:
        timproc_uso_b5_func_00008468(arg0);
        break;
    case 10:
        timproc_uso_b5_func_000085E0(arg0);
        break;
    }
    if (FW(arg0, 0x4B4) != 0) {
        *(f32 *)(arg0 + 0x4BC) += *(f32 *)(arg0 + 0xBC);
        if (*(f32 *)(arg0 + 0x4BC) >= 1.0f) {
            *(f32 *)(arg0 + 0x4BC) = 1.0f;
            FW(arg0, 0x4B4) = 0;
            FW(arg0, 0x4B8) = 0;
        }
    }
    if (FW(arg0, 0x3FC) != 0) {
        FW(arg0, 0x3FC) = FW(arg0, 0x3FC) - 4;
        if (FW(arg0, 0x3FC) <= 0) {
            FW(arg0, 0x3FC) = 0;
        }
    }
    if (FW(arg0, 0x3F8) != 0) {
        v0 = (char *)timproc_uso_b5_func_00008AD4(arg0);
        v1 = (char *)FW(v0, 0x28);
        ((FPv)FW(v1, 0xBC))(*(s16 *)(v1 + 0xB8) + v0, FW(arg0, 0x480));
    }
    v0 = (char *)timproc_uso_b5_func_00008A38(arg0);
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
    if (FW(arg0, 0x3CC) == 5) {
        sp2C = (char *)timproc_uso_b5_func_00008A38(arg0);
        timproc_uso_b5_func_0000396C(sp2C, FW(FW(arg0, 0x414), 0x10), arg0 + 0x4C4,
            timproc_uso_b5_func_07E7B0(*(f32 *)(&D_00000000 + 0x1BC) * *(f32 *)(arg0 + 0x4BC)),
            *(f32 *)(arg0 + 0x5C));
        sp2C = (char *)timproc_uso_b5_func_00008A38(arg0);
        *(f32 *)(arg0 + 0x498) =
            *(f32 *)(arg0 + 0x49C) +
            timproc_uso_b5_func_07E7B0(*(f32 *)(&D_00000000 + 0x1C0) * *(f32 *)(arg0 + 0x480)) *
            *(f32 *)(sp2C + 0x14C);
    } else {
        sp2C = (char *)timproc_uso_b5_func_00008A38(arg0);
        timproc_uso_b5_func_000038B0(sp2C, FW(FW(arg0, 0x414), 0x10), arg0 + 0x4C4,
            timproc_uso_b5_func_07E7B0(*(f32 *)(&D_00000000 + 0x1C4) * *(f32 *)(arg0 + 0x4BC)),
            *(f32 *)(arg0 + 0x5C));
        sp2C = (char *)timproc_uso_b5_func_00008A38(arg0);
        *(f32 *)(arg0 + 0x498) =
            *(f32 *)(arg0 + 0x49C) +
            timproc_uso_b5_func_07E7B0(*(f32 *)(&D_00000000 + 0x1C8) * *(f32 *)(arg0 + 0x480)) *
            *(f32 *)(sp2C + 0x134);
    }
    *(f32 *)(FW(FW(FW(arg0, 0x414), 0xC), 0x70) + 0x14C) =
        *(f32 *)(arg0 + 0x4C0) +
        (*(f32 *)(arg0 + 0x498) - *(f32 *)(arg0 + 0x4C0)) *
        timproc_uso_b5_func_07E7B0(*(f32 *)(&D_00000000 + 0x1CC) * *(f32 *)(arg0 + 0x4BC));
}
