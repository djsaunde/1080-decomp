/* IDO 7.1 NCC (C++) -O0 donors for mgr_initMatchRace_00000700 (0x2A8 / 170
 * words) and mgr_prepareReplay_00000A14 (0xC0 / 48 words): the cfe NM bodies
 * unchanged (700's K&R definition spelled as a prototype). mgrproc is C++;
 * edg -O0 emits no closing-brace b-epilogue, which was the whole cfe residual
 * (+2 words each). Spliced into mgrproc_uso_o0_19C.c.o via REPLACE_FUNC_BODY. */
extern "C" {
typedef int s32;
#define FW(p, o) (*(int *)((char *)(p) + (o)))
struct IC { int pad[0x13]; int v; };
extern struct IC import_800200E4;
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

int mgr_initMatchRace_00000700(char *arg0, int arg1) {
    register char *base; /* s0; case-1 dead address of import_8005C6E0 */
    *(int*)(FW(arg0, 0x8)) = 3;
    FW(FW(arg0, 0x8), 0x4) = 0;
    switch (arg1) { /* irregular */
    case 1:
        base = &import_8005C6E0;
        if (*(s32 *)0xA0000284 == 0x240B17D7) {
            FW(FW(arg0, 0x8), 0x8) = 4;
        } else {
            FW(FW(arg0, 0x8), 0x8) = 3;
        }
        FW(FW(arg0, 0x8), 0xC) = 2;
        FW(FW(arg0, 0x8), 0x10) = 3;
        FW(FW(arg0, 0x8), 0x14) = 4;
        FW(FW(arg0, 0x8), 0x18) = 5;
        mgr_pickRivals_000005D0(arg0);
        break;
    case 2:
        FW(FW(arg0, 0x8), 0x8) = 5;
        FW(FW(arg0, 0x8), 0xC) = 2;
        FW(FW(arg0, 0x8), 0x10) = 3;
        FW(FW(arg0, 0x8), 0x14) = 4;
        FW(FW(arg0, 0x8), 0x18) = 5;
        FW(FW(arg0, 0x8), 0x1C) = 6;
        mgr_pickRivals_000005D0(arg0);
        FW(FW(arg0, 0x8), 0x34) = import_800200E4.v;
        break;
    case 3:
        FW(FW(arg0, 0x8), 0x8) = 6;
        FW(FW(arg0, 0x8), 0xC) = 2;
        FW(FW(arg0, 0x8), 0x10) = 3;
        FW(FW(arg0, 0x8), 0x14) = 4;
        FW(FW(arg0, 0x8), 0x18) = 5;
        FW(FW(arg0, 0x8), 0x1C) = 6;
        FW(FW(arg0, 0x8), 0x20) = 7;
        mgr_pickRivals_000005D0(arg0);
        FW(FW(arg0, 0x8), 0x34) = import_800200E4.v;
        if (import_800201EC.options->seqStep == 3) {
            FW(FW(arg0, 0x8), 0x38) = 5;
        } else if (import_800201EC.options->seqStep == 4) {
            FW(FW(arg0, 0x8), 0x38) = 6;
        } else {
            FW(FW(arg0, 0x8), 0x38) = 7;
        }
        break;
    }
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
