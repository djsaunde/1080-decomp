/* timproc_uso_b5_func_00004118 (0x1AD8 / 1718 words) -- the timing-screen
 * constructor (cfront-lowered alloc-or-use cascades + an 8-way mode switch).
 * EXACT under IDO 7.1 cfe -O2 (agent-g run 161). Spliced over the unit's
 * INCLUDE_ASM by REPLACE_FUNC_BODY (Makefile TIMB5_4118_DONOR) so the
 * switch jumptable / float literal .rodata reloc can be renamed
 * timproc_uso_b5_func_00004118_rodata and pinned to the USO RoData at +0x14C
 * (0.45f, 0.036f x3, 0.1f x5, then the 8-entry table at +0x170). The body is
 * the unit's NM body verbatim; see the run-161 note there for the last
 * five source fixes. */
#include "common.h"
extern char *timproc_uso_b5_alias();

extern int gl_func_00000000();
extern char D_00000000;
extern char D_00000000_a;
typedef struct { int a, b, c, d; } Quad4;

#ifndef FW
#define FW(p, o) (*(int *)((char *)(p) + (o)))
#endif
/* per-site vtable-address aliases of D_00000000 for timproc_uso_b5_func_00004118 (defeat cross-record &D GCSE+spill) */
extern void timproc_uso_b5_alias_pf1(int, float);
extern void timproc_uso_b5_alias_pf2(int, float);
extern void timproc_uso_b5_alias_pf3(int, float);
extern char D_b5_4118_s01;
extern char D_b5_4118_s02;
extern char D_b5_4118_s03;
extern char D_b5_4118_s04;
extern char D_b5_4118_s05;
extern char D_b5_4118_s06;
extern char D_b5_4118_s07;
extern char D_b5_4118_s08;
extern char D_b5_4118_s09;
extern char D_b5_4118_s10;
extern char D_b5_4118_s11;
extern char D_b5_4118_s12;
extern char D_b5_4118_s13;
extern char D_b5_4118_s14;
extern char D_b5_4118_s15;
extern char D_b5_4118_s16;
extern char D_b5_4118_s17;
extern char D_b5_4118_s18;
extern char D_b5_4118_s19;
extern char D_b5_4118_s20;
extern char D_b5_4118_s21;
extern char D_b5_4118_s22;
extern char D_b5_4118_s23;
extern char D_b5_4118_s24;
extern char D_b5_4118_s25;
extern char D_b5_4118_s26;
extern char D_b5_4118_s27;
extern char D_b5_4118_s28;
extern char D_b5_4118_s29;
extern char D_b5_4118_s30;
extern char D_b5_4118_s31;
typedef struct { f32 unk0,unk4,unk8,unkC; } Q;
typedef char *(*FP4)();
typedef struct { char *p; } P1;
/* run 161 (agent-g, 99.14 -> 99.77, 2 words from exact): (1) the empty
 * `case 2/4/5/6: break;` arms go LAST in the switch: placed first they stop
 * uopt from giving arg0 a register piece inside case 0 (the target loads the
 * homed arg0 once into v0 there; ours reloaded it per access); (2) the case
 * 0/1 calls go to the real VOID callee 00008D90 (an int-returning alias keeps
 * the piece out of v0); (3) case 0's table index is read inline in the last
 * statement (a temp_t0 local is a uopt variable in v0/v1, target t2);
 * (4) `0x3C0 = arg5` before `0x414 = arg9` (as1 then issues the arg5 load and
 * the 0.5f lui like the target); (5) case 0's table store and its `break`
 * on ONE source line: as1 then schedules the a2 reload above the store and
 * the store fills the `b` delay slot (target). */
void timproc_uso_b5_func_00008D90(int *a0, int a1);
char *timproc_uso_b5_func_00004118(char *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, char *arg6, char *arg7, s32 arg8, char *arg9, char *arg10, s32 arg11) {
    volatile int pad0;
    P1 sp3A0;
    P1 sp39C;
    P1 sp398;
    P1 sp394;
    P1 sp390;
    P1 sp38C;
    P1 sp388;
    P1 sp384;
    P1 sp380;
    P1 sp37C;
    Q sp36C;
    Q sp35C;
    Q sp34C;
    P1 sp348;
    Q sp338;
    Q sp328;
    Q sp318;
    P1 sp314;
    Q sp304;
    Q sp2F4;
    Q sp2E4;
    P1 sp2E0;
    Q sp2D0;
    Q sp2C0;
    Q sp2B0;
    P1 sp2AC;
    Q sp29C;
    Q sp28C;
    Q sp27C;
    P1 sp278;
    Q sp268;
    Q sp258;
    Q sp248;
    P1 sp244;
    Q sp234;
    Q sp224;
    Q sp214;
    P1 sp210;
    Q sp200;
    Q sp1F0;
    Q sp1E0;
    P1 sp1DC;
    Q sp1CC;
    Q sp1BC;
    Q sp1AC;
    P1 sp1A8;
    Q sp198;
    Q sp188;
    Q sp178;
    P1 sp174;
    Q sp164;
    Q sp154;
    Q sp144;
    P1 sp140;
    Q sp130;
    Q sp120;
    Q sp110;
    P1 sp10C;
    Q spFC;
    Q spEC;
    Q spDC;
    P1 spD8;
    P1 spD4;
    P1 spD0;
    P1 spCC;
    P1 spC8;
    P1 spC4;
    P1 spC0;
    P1 spBC;
    P1 spB8;
    char *temp_v0;
    char *temp_v0_2;
    char *temp_v0_3;
    char *spA8;
    P1 marsh;
    char *temp_v0_4;
    char *temp_v0_36;
    char *var_s0;
    char *var_v1;
    char *temp_t0;
    f32 v;
    f32 temp_f0;
    volatile int pad1[4];
    char *sp74;
    volatile int pad2;
    Q sp60;
    Q sp50;
    Q sp40;
    volatile int pad3[5];
    char *sp28;

    if ((arg0 != NULL) || (temp_v0 = timproc_uso_b5_alias((char *)0x4F0), arg0 = temp_v0, (temp_v0 != NULL))) {
        var_s0 = arg0;
        if ((var_s0 != NULL) || (temp_v0_2 = timproc_uso_b5_alias((char *)0x2C), var_s0 = temp_v0_2, (temp_v0_2 != NULL))) {
            timproc_uso_b5_alias(var_s0, ((char *)&D_00000000 + 0x1260));
            FW(var_s0, 0x28) = (s32)&D_00000000;
        }
        FW(arg0, 0x28) = (s32)&D_00000000;
        var_s0 = arg0 + 0x2C;
        if ((arg0 != (char *)-0x2C) || (temp_v0_3 = timproc_uso_b5_alias((char *)0x38C), var_s0 = temp_v0_3, (temp_v0_3 != NULL))) {
            var_v1 = var_s0;
            if ((var_s0 != NULL) || (temp_v0_4 = timproc_uso_b5_alias((char *)8), var_v1 = temp_v0_4, (temp_v0_4 != NULL))) {
                FW(var_v1, 0x0) = (s32)((char *)&D_00000000 + 0x1268);
                FW(var_v1, 0x4) = 0;
            }
            sp3A0.p = *(char **)((char *)&D_00000000 + 0x127C);
            spA8 = var_s0 + 8;
            marsh = sp3A0;
            if ((var_s0 != (char *)-8) || (temp_v0 = timproc_uso_b5_alias((char *)0x18), spA8 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(spA8, var_s0, marsh, 1);
                FW(spA8, 0xC) = (s32)((char *)&D_b5_4118_s01 + 0x158);
                FW(spA8, 0x14) = 0;
                *(f32 *)((char *)spA8 + 0x10) = 0.45f;
            }
            sp39C.p = *(char **)((char *)&D_00000000 + 0x1280);
            spA8 = var_s0 + 0x20;
            marsh = sp39C;
            if ((var_s0 != (char *)-0x20) || (temp_v0 = timproc_uso_b5_alias((char *)0x18), spA8 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(spA8, var_s0, marsh, 1);
                FW(spA8, 0xC) = (s32)((char *)&D_b5_4118_s02 + 0x158);
                FW(spA8, 0x14) = 0;
                *(f32 *)((char *)spA8 + 0x10) = 0.0f;
            }
            sp398.p = *(char **)((char *)&D_00000000 + 0x1284);
            spA8 = var_s0 + 0x38;
            marsh = sp398;
            if ((var_s0 != (char *)-0x38) || (temp_v0 = timproc_uso_b5_alias((char *)0x18), spA8 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(spA8, var_s0, marsh, 1);
                FW(spA8, 0xC) = (s32)((char *)&D_b5_4118_s03 + 0x1A0); FW(spA8, 0x10) = 0x18;
                FW(spA8, 0x14) = 0;
            }
            sp394.p = *(char **)((char *)&D_00000000 + 0x1288);
            spA8 = var_s0 + 0x50;
            marsh = sp394;
            if ((var_s0 != (char *)-0x50) || (temp_v0 = timproc_uso_b5_alias((char *)0x18), spA8 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(spA8, var_s0, marsh, 1);
                FW(spA8, 0xC) = (s32)((char *)&D_b5_4118_s04 + 0x1A0); FW(spA8, 0x10) = 0x78;
                FW(spA8, 0x14) = 0;
            }
            sp390.p = *(char **)((char *)&D_00000000 + 0x128C);
            spA8 = var_s0 + 0x68;
            marsh = sp390;
            if ((var_s0 != (char *)-0x68) || (temp_v0 = timproc_uso_b5_alias((char *)0x18), spA8 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(spA8, var_s0, marsh, 1);
                FW(spA8, 0xC) = (s32)((char *)&D_b5_4118_s05 + 0x1A0); FW(spA8, 0x10) = 0x78;
                FW(spA8, 0x14) = 0;
            }
            sp38C.p = *(char **)((char *)&D_00000000 + 0x1290);
            spA8 = var_s0 + 0x80;
            marsh = sp38C;
            if ((var_s0 != (char *)-0x80) || (temp_v0 = timproc_uso_b5_alias((char *)0x18), spA8 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(spA8, var_s0, marsh, 1);
                FW(spA8, 0xC) = (s32)((char *)&D_b5_4118_s06 + 0x158);
                FW(spA8, 0x14) = 0;
                *(f32 *)((char *)spA8 + 0x10) = 0.036f;
            }
            sp388.p = *(char **)((char *)&D_00000000 + 0x1294);
            spA8 = var_s0 + 0x98;
            marsh = sp388;
            if ((var_s0 != (char *)-0x98) || (temp_v0 = timproc_uso_b5_alias((char *)0x18), spA8 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(spA8, var_s0, marsh, 1);
                FW(spA8, 0xC) = (s32)((char *)&D_b5_4118_s07 + 0x158);
                FW(spA8, 0x14) = 0;
                *(f32 *)((char *)spA8 + 0x10) = 0.036f;
            }
            sp384.p = *(char **)((char *)&D_00000000 + 0x1298);
            spA8 = var_s0 + 0xB0;
            marsh = sp384;
            if ((var_s0 != (char *)-0xB0) || (temp_v0 = timproc_uso_b5_alias((char *)0x18), spA8 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(spA8, var_s0, marsh, 1);
                FW(spA8, 0xC) = (s32)((char *)&D_b5_4118_s08 + 0x158);
                FW(spA8, 0x14) = 0;
                *(f32 *)((char *)spA8 + 0x10) = 0.036f;
            }
            sp380.p = *(char **)((char *)&D_00000000 + 0x129C);
            spA8 = var_s0 + 0xC8;
            marsh = sp380;
            if ((var_s0 != (char *)-0xC8) || (temp_v0 = timproc_uso_b5_alias((char *)0x18), spA8 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(spA8, var_s0, marsh, 1);
                FW(spA8, 0xC) = (s32)((char *)&D_b5_4118_s09 + 0x1A0); FW(spA8, 0x10) = 0x99;
                FW(spA8, 0x14) = 0;
            }
            sp37C.p = *(char **)((char *)&D_00000000 + 0x12A0);
            spA8 = var_s0 + 0xE0;
            marsh = sp37C;
            if ((var_s0 != (char *)-0xE0) || (temp_v0 = timproc_uso_b5_alias((char *)0x18), spA8 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(spA8, var_s0, marsh, 1);
                FW(spA8, 0xC) = (s32)((char *)&D_b5_4118_s10 + 0x1A0); FW(spA8, 0x10) = 0x99;
                FW(spA8, 0x14) = 0;
            }
            sp36C.unkC = 0.0f;
            sp35C.unk0 = 0.0f;
            sp35C.unk4 = 0.0f;
            sp35C.unk8 = 0.0f;
            sp35C.unkC = 0.0f;
            sp36C.unk0 = 0.5f;
            sp36C.unk4 = 0.5f;
            sp36C.unk8 = 0.5f;
            if (1) {} /* BB split: 0.5f lui/mtc1 + stores before the 1.0f group (run 125, raw 86 -> 76) */
            sp34C.unk0 = 1.0f;
            sp34C.unk4 = 1.0f;
            sp34C.unk8 = 1.0f;
            sp34C.unkC = 1.0f;
            sp348.p = *(char **)((char *)&D_00000000 + 0x12A4);
            sp60 = sp36C;
            sp74 = var_s0 + 0xF8;
            sp50 = sp35C;
            sp40 = sp34C;
            marsh = sp348;
            if ((var_s0 != (char *)-0xF8) || (temp_v0 = timproc_uso_b5_alias((char *)0x24), sp74 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(sp74, var_s0, marsh, 1);
                FW(sp74, 0xC) = (s32)((char *)&D_b5_4118_s11 + 0x188);
                FW(sp74, 0x20) = 0;
                *(Q *)(sp74 + 0x10) = sp60;
            }
            sp338.unkC = 0.0f;
            sp328.unk0 = 0.0f;
            sp328.unk4 = 0.0f;
            sp328.unk8 = 0.0f;
            sp328.unkC = 0.0f;
            sp318.unk0 = 1.0f;
            sp318.unk4 = 1.0f;
            sp318.unk8 = 1.0f;
            sp318.unkC = 1.0f;
            sp338.unk0 = 0.5f;
            sp338.unk4 = 0.5f;
            sp338.unk8 = 0.5f;
            sp314.p = *(char **)((char *)&D_00000000 + 0x12A8);
            sp60 = sp338;
            sp28 = var_s0 + 0x11C;
            sp50 = sp328;
            sp40 = sp318;
            marsh = sp314;
            if ((var_s0 != (char *)-0x11C) || (temp_v0 = timproc_uso_b5_alias((char *)0x24), sp28 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(sp28, var_s0, marsh, 1);
                FW(sp28, 0xC) = (s32)((char *)&D_b5_4118_s12 + 0x188);
                FW(sp28, 0x20) = 0;
                *(Q *)(sp28 + 0x10) = sp60;
            }
            sp304.unkC = 0.0f;
            sp2F4.unk0 = 0.0f;
            sp2F4.unk4 = 0.0f;
            sp2F4.unk8 = 0.0f;
            sp2F4.unkC = 0.0f;
            sp2E4.unk0 = 1.0f;
            sp2E4.unk4 = 1.0f;
            sp2E4.unk8 = 1.0f;
            sp2E4.unkC = 1.0f;
            sp304.unk0 = 0.5f;
            sp304.unk4 = 0.5f;
            sp304.unk8 = 0.5f;
            sp2E0.p = *(char **)((char *)&D_00000000 + 0x12AC);
            sp60 = sp304;
            sp28 = var_s0 + 0x140;
            sp50 = sp2F4;
            sp40 = sp2E4;
            marsh = sp2E0;
            if ((var_s0 != (char *)-0x140) || (temp_v0 = timproc_uso_b5_alias((char *)0x24), sp28 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(sp28, var_s0, marsh, 1);
                FW(sp28, 0xC) = (s32)((char *)&D_b5_4118_s13 + 0x188);
                FW(sp28, 0x20) = 0;
                *(Q *)(sp28 + 0x10) = sp60;
            }
            sp2D0.unkC = 0.0f;
            sp2C0.unk0 = 0.0f;
            sp2C0.unk4 = 0.0f;
            sp2C0.unk8 = 0.0f;
            sp2C0.unkC = 0.0f;
            sp2B0.unk0 = 1.0f;
            sp2B0.unk4 = 1.0f;
            sp2B0.unk8 = 1.0f;
            sp2B0.unkC = 1.0f;
            sp2D0.unk0 = 0.5f;
            sp2D0.unk4 = 0.5f;
            sp2D0.unk8 = 0.5f;
            sp2AC.p = *(char **)((char *)&D_00000000 + 0x12B0);
            sp60 = sp2D0;
            sp28 = var_s0 + 0x164;
            sp50 = sp2C0;
            sp40 = sp2B0;
            marsh = sp2AC;
            if ((var_s0 != (char *)-0x164) || (temp_v0 = timproc_uso_b5_alias((char *)0x24), sp28 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(sp28, var_s0, marsh, 1);
                FW(sp28, 0xC) = (s32)((char *)&D_b5_4118_s14 + 0x188);
                FW(sp28, 0x20) = 0;
                *(Q *)(sp28 + 0x10) = sp60;
            }
            sp29C.unkC = 0.0f;
            sp28C.unk0 = 0.0f;
            sp28C.unk4 = 0.0f;
            sp28C.unk8 = 0.0f;
            sp28C.unkC = 0.0f;
            sp27C.unk0 = 1.0f;
            sp27C.unk4 = 1.0f;
            sp27C.unk8 = 1.0f;
            sp27C.unkC = 1.0f;
            sp29C.unk0 = 0.5f;
            sp29C.unk4 = 0.5f;
            sp29C.unk8 = 0.5f;
            sp278.p = *(char **)((char *)&D_00000000 + 0x12B4);
            sp60 = sp29C;
            sp28 = var_s0 + 0x188;
            sp50 = sp28C;
            sp40 = sp27C;
            marsh = sp278;
            if ((var_s0 != (char *)-0x188) || (temp_v0 = timproc_uso_b5_alias((char *)0x24), sp28 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(sp28, var_s0, marsh, 1);
                FW(sp28, 0xC) = (s32)((char *)&D_b5_4118_s15 + 0x188);
                FW(sp28, 0x20) = 0;
                *(Q *)(sp28 + 0x10) = sp60;
            }
            sp268.unkC = 0.0f;
            sp258.unk0 = 0.0f;
            sp258.unk4 = 0.0f;
            sp258.unk8 = 0.0f;
            sp258.unkC = 0.0f;
            sp248.unk0 = 1.0f;
            sp248.unk4 = 1.0f;
            sp248.unk8 = 1.0f;
            sp248.unkC = 1.0f;
            sp268.unk0 = 0.5f;
            sp268.unk4 = 0.5f;
            sp268.unk8 = 0.5f;
            sp244.p = *(char **)((char *)&D_00000000 + 0x12B8);
            sp60 = sp268;
            sp28 = var_s0 + 0x1AC;
            sp50 = sp258;
            sp40 = sp248;
            marsh = sp244;
            if ((var_s0 != (char *)-0x1AC) || (temp_v0 = timproc_uso_b5_alias((char *)0x24), sp28 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(sp28, var_s0, marsh, 1);
                FW(sp28, 0xC) = (s32)((char *)&D_b5_4118_s16 + 0x188);
                FW(sp28, 0x20) = 0;
                *(Q *)(sp28 + 0x10) = sp60;
            }
            sp234.unkC = 0.0f;
            sp224.unk0 = 0.0f;
            sp224.unk4 = 0.0f;
            sp224.unk8 = 0.0f;
            sp224.unkC = 0.0f;
            sp214.unk0 = 1.0f;
            sp214.unk4 = 1.0f;
            sp214.unk8 = 1.0f;
            sp214.unkC = 1.0f;
            sp234.unk0 = 0.5f;
            sp234.unk4 = 0.5f;
            sp234.unk8 = 0.5f;
            sp210.p = *(char **)((char *)&D_00000000 + 0x12BC);
            sp60 = sp234;
            sp28 = var_s0 + 0x1D0;
            sp50 = sp224;
            sp40 = sp214;
            marsh = sp210;
            if ((var_s0 != (char *)-0x1D0) || (temp_v0 = timproc_uso_b5_alias((char *)0x24), sp28 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(sp28, var_s0, marsh, 1);
                FW(sp28, 0xC) = (s32)((char *)&D_b5_4118_s17 + 0x188);
                FW(sp28, 0x20) = 0;
                *(Q *)(sp28 + 0x10) = sp60;
            }
            sp200.unkC = 0.0f;
            sp1F0.unk0 = 0.0f;
            sp1F0.unk4 = 0.0f;
            sp1F0.unk8 = 0.0f;
            sp1F0.unkC = 0.0f;
            sp1E0.unk0 = 1.0f;
            sp1E0.unk4 = 1.0f;
            sp1E0.unk8 = 1.0f;
            sp1E0.unkC = 1.0f;
            sp1DC.p = *(char **)((char *)&D_00000000 + 0x12C0);
            sp200.unk0 = 0.5f;
            sp200.unk4 = 0.5f;
            sp200.unk8 = 0.5f;
            sp60 = sp200;
            sp28 = var_s0 + 0x1F4;
            sp50 = sp1F0;
            sp40 = sp1E0;
            marsh = sp1DC;
            if ((var_s0 != (char *)-0x1F4) || (temp_v0 = timproc_uso_b5_alias((char *)0x24), sp28 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(sp28, var_s0, marsh, 1);
                FW(sp28, 0xC) = (s32)((char *)&D_b5_4118_s18 + 0x188);
                FW(sp28, 0x20) = 0;
                *(Q *)(sp28 + 0x10) = sp60;
            }
            sp1CC.unkC = 0.0f;
            sp1BC.unk0 = 0.0f;
            sp1BC.unk4 = 0.0f;
            sp1BC.unk8 = 0.0f;
            sp1BC.unkC = 0.0f;
            sp1AC.unk0 = 1.0f;
            sp1AC.unk4 = 1.0f;
            sp1AC.unk8 = 1.0f;
            sp1AC.unkC = 1.0f;
            sp1A8.p = *(char **)((char *)&D_00000000 + 0x12C4);
            sp1CC.unk0 = 0.5f;
            sp1CC.unk4 = 0.5f;
            sp1CC.unk8 = 0.5f;
            sp60 = sp1CC;
            sp28 = var_s0 + 0x218;
            sp50 = sp1BC;
            sp40 = sp1AC;
            marsh = sp1A8;
            if ((var_s0 != (char *)-0x218) || (temp_v0 = timproc_uso_b5_alias((char *)0x24), sp28 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(sp28, var_s0, marsh, 1);
                FW(sp28, 0xC) = (s32)((char *)&D_b5_4118_s19 + 0x188);
                FW(sp28, 0x20) = 0;
                *(Q *)(sp28 + 0x10) = sp60;
            }
            sp198.unkC = 0.0f;
            sp188.unk0 = 0.0f;
            sp188.unk4 = 0.0f;
            sp188.unk8 = 0.0f;
            sp188.unkC = 0.0f;
            sp178.unk0 = 1.0f;
            sp178.unk4 = 1.0f;
            sp178.unk8 = 1.0f;
            sp178.unkC = 1.0f;
            sp198.unk0 = 0.1f;
            sp198.unk4 = 0.1f;
            sp198.unk8 = 0.1f;
            sp174.p = *(char **)((char *)&D_00000000 + 0x12C8);
            sp60 = sp198;
            sp28 = var_s0 + 0x23C;
            sp50 = sp188;
            sp40 = sp178;
            marsh = sp174;
            if ((var_s0 != (char *)-0x23C) || (temp_v0 = timproc_uso_b5_alias((char *)0x24), sp28 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(sp28, var_s0, marsh, 1);
                FW(sp28, 0xC) = (s32)((char *)&D_b5_4118_s20 + 0x188);
                FW(sp28, 0x20) = 0;
                *(Q *)(sp28 + 0x10) = sp60;
            }
            sp164.unkC = 0.0f;
            sp154.unk0 = 0.0f;
            sp154.unk4 = 0.0f;
            sp154.unk8 = 0.0f;
            sp154.unkC = 0.0f;
            sp144.unk0 = 1.0f;
            sp144.unk4 = 1.0f;
            sp144.unk8 = 1.0f;
            sp144.unkC = 1.0f;
            sp140.p = *(char **)((char *)&D_00000000 + 0x12CC);
            sp164.unk0 = 0.5f;
            sp164.unk4 = 0.5f;
            sp164.unk8 = 0.5f;
            sp60 = sp164;
            sp28 = var_s0 + 0x260;
            sp50 = sp154;
            sp40 = sp144;
            marsh = sp140;
            if ((var_s0 != (char *)-0x260) || (temp_v0 = timproc_uso_b5_alias((char *)0x24), sp28 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(sp28, var_s0, marsh, 1);
                FW(sp28, 0xC) = (s32)((char *)&D_b5_4118_s21 + 0x188);
                FW(sp28, 0x20) = 0;
                *(Q *)(sp28 + 0x10) = sp60;
            }
            sp130.unk0 = 0.1f;
            sp130.unk4 = 0.1f;
            sp130.unk8 = 0.1f;
            sp130.unkC = 0.0f;
            sp120.unk0 = 0.0f;
            sp120.unk4 = 0.0f;
            sp120.unk8 = 0.0f;
            sp120.unkC = 0.0f;
            sp110.unk0 = 1.0f;
            sp110.unk4 = 1.0f;
            sp110.unk8 = 1.0f;
            sp110.unkC = 1.0f;
            sp10C.p = *(char **)((char *)&D_00000000 + 0x12D0);
            sp60 = sp130;
            sp28 = var_s0 + 0x284;
            sp50 = sp120;
            sp40 = sp110;
            marsh = sp10C;
            if ((var_s0 != (char *)-0x284) || (temp_v0 = timproc_uso_b5_alias((char *)0x24), sp28 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(sp28, var_s0, marsh, 1);
                FW(sp28, 0xC) = (s32)((char *)&D_b5_4118_s22 + 0x188);
                FW(sp28, 0x20) = 0;
                *(Q *)(sp28 + 0x10) = sp60;
            }
            spFC.unkC = 0.0f;
            spEC.unk0 = 0.0f;
            spEC.unk4 = 0.0f;
            spEC.unk8 = 0.0f;
            spEC.unkC = 0.0f;
            spDC.unk0 = 1.0f;
            spDC.unk4 = 1.0f;
            spDC.unk8 = 1.0f;
            spDC.unkC = 1.0f;
            spD8.p = *(char **)((char *)&D_00000000 + 0x12D4);
            spFC.unk0 = 0.5f;
            spFC.unk4 = 0.5f;
            spFC.unk8 = 0.5f;
            sp60 = spFC;
            sp28 = var_s0 + 0x2A8;
            sp50 = spEC;
            sp40 = spDC;
            marsh = spD8;
            if ((var_s0 != (char *)-0x2A8) || (temp_v0 = timproc_uso_b5_alias((char *)0x24), sp28 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(sp28, var_s0, marsh, 1);
                FW(sp28, 0xC) = (s32)((char *)&D_b5_4118_s23 + 0x188);
                FW(sp28, 0x20) = 0;
                *(Q *)(sp28 + 0x10) = sp60;
            }
            spD4.p = *(char **)((char *)&D_00000000 + 0x12D8);
            spA8 = var_s0 + 0x2CC;
            marsh = spD4;
            if ((var_s0 != (char *)-0x2CC) || (temp_v0 = timproc_uso_b5_alias((char *)0x18), spA8 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(spA8, var_s0, marsh, 1);
                FW(spA8, 0xC) = (s32)((char *)&D_b5_4118_s24 + 0x1A0); FW(spA8, 0x10) = -4;
                FW(spA8, 0x14) = 0;
            }
            spD0.p = *(char **)((char *)&D_00000000 + 0x12DC);
            spA8 = var_s0 + 0x2E4;
            marsh = spD0;
            if ((var_s0 != (char *)-0x2E4) || (temp_v0 = timproc_uso_b5_alias((char *)0x18), spA8 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(spA8, var_s0, marsh, 1);
                FW(spA8, 0xC) = (s32)((char *)&D_b5_4118_s25 + 0x1A0); FW(spA8, 0x10) = 0xD;
                FW(spA8, 0x14) = 0;
            }
            spCC.p = *(char **)((char *)&D_00000000 + 0x12E0);
            spA8 = var_s0 + 0x2FC;
            marsh = spCC;
            if ((var_s0 != (char *)-0x2FC) || (temp_v0 = timproc_uso_b5_alias((char *)0x18), spA8 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(spA8, var_s0, marsh, 1);
                FW(spA8, 0xC) = (s32)((char *)&D_b5_4118_s26 + 0x1A0); FW(spA8, 0x10) = -0x10;
                FW(spA8, 0x14) = 0;
            }
            spC8.p = *(char **)((char *)&D_00000000 + 0x12E4);
            spA8 = var_s0 + 0x314;
            marsh = spC8;
            if ((var_s0 != (char *)-0x314) || (temp_v0 = timproc_uso_b5_alias((char *)0x18), spA8 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(spA8, var_s0, marsh, 1);
                FW(spA8, 0xC) = (s32)((char *)&D_b5_4118_s27 + 0x1A0); FW(spA8, 0x10) = -0x15;
                FW(spA8, 0x14) = 0;
            }
            spC4.p = *(char **)((char *)&D_00000000 + 0x12E8);
            spA8 = var_s0 + 0x32C;
            marsh = spC4;
            if ((var_s0 != (char *)-0x32C) || (temp_v0 = timproc_uso_b5_alias((char *)0x18), spA8 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(spA8, var_s0, marsh, 1);
                FW(spA8, 0xC) = (s32)((char *)&D_b5_4118_s28 + 0x1A0); FW(spA8, 0x10) = 0x73;
                FW(spA8, 0x14) = 0;
            }
            spC0.p = *(char **)((char *)&D_00000000 + 0x12EC);
            spA8 = var_s0 + 0x344;
            marsh = spC0;
            if ((var_s0 != (char *)-0x344) || (temp_v0 = timproc_uso_b5_alias((char *)0x18), spA8 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(spA8, var_s0, marsh, 1);
                FW(spA8, 0xC) = (s32)((char *)&D_b5_4118_s29 + 0x1A0); FW(spA8, 0x10) = 0x62;
                FW(spA8, 0x14) = 0;
            }
            spBC.p = *(char **)((char *)&D_00000000 + 0x12F0);
            spA8 = var_s0 + 0x35C;
            marsh = spBC;
            if ((var_s0 != (char *)-0x35C) || (temp_v0 = timproc_uso_b5_alias((char *)0x18), spA8 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(spA8, var_s0, marsh, 1);
                FW(spA8, 0xC) = (s32)((char *)&D_b5_4118_s30 + 0x1A0); FW(spA8, 0x10) = 0x62;
                FW(spA8, 0x14) = 0;
            }
            spB8.p = *(char **)((char *)&D_00000000 + 0x12F4);
            spA8 = var_s0 + 0x374;
            marsh = spB8;
            if ((var_s0 != (char *)-0x374) || (temp_v0 = timproc_uso_b5_alias((char *)0x18), spA8 = temp_v0, (temp_v0 != NULL))) {
                timproc_uso_b5_alias(spA8, var_s0, marsh, 1);
                FW(spA8, 0xC) = (s32)((char *)&D_b5_4118_s31 + 0x1A0); FW(spA8, 0x10) = 0xD;
                FW(spA8, 0x14) = 0;
            }
        }
        FW(arg0, 0xC) = (s32)((char *)&D_00000000 + 0x12F8);
        FW(arg0, 0x4D4) = arg11;
        FW(arg0, 0x418) = arg2;
        FW(arg0, 0x40C) = arg7;
        FW(arg0, 0x410) = arg8;
        FW(arg0, 0x3BC) = arg3;
        FW(arg0, 0x41C) = arg1;
        FW(arg0, 0x3B8) = arg4;
        FW(arg0, 0x3C8) = arg6;
        temp_v0_36 = timproc_uso_b5_alias(arg0, arg6);
        FW(arg0, 0x3C4) = temp_v0_36;
        FW(arg0, 0x3F0) = temp_v0_36;
        FW(arg0, 0x3F4) = 0;
        if (FW(arg0, 0x3BC) == 2) {
            FW(arg0, 0x4D8) = 0x78;
        } else {
            FW(arg0, 0x4D8) = 0xF0;
        }
        FW(arg0, 0x48C) = 0;
        FW(arg0, 0x490) = 0;
        switch (FW(arg0, 0x3C8) - 1) {
        case 0:
            timproc_uso_b5_func_00008D90((int *)arg0, 0);
            FW(arg0, 0x3CC) = 0;
            *(f32 *)((char *)arg0 + 0x480) = 0.0f;
            *(f32 *)((char *)arg0 + 0x484) = 0.0f;
            ((int *)((char *)&D_00000000 + 0xB8))[FW(arg0, 0x3B8)] = 1; break; /* one line: as1 hoists the a2 reload above the store */
        case 1:
            timproc_uso_b5_func_00008D90((int *)arg0, 1);
            FW(arg0, 0x3CC) = 6;
            *(f32 *)((char *)arg0 + 0x480) = 1.0f;
            *(f32 *)((char *)arg0 + 0x484) = 1.0f;
            break;
        case 3:
            FW(arg0, 0x3CC) = 8;
            *(f32 *)((char *)arg0 + 0x480) = 1.0f;
            *(f32 *)((char *)arg0 + 0x484) = 1.0f;
            break;
        case 7:
            FW(arg0, 0x3CC) = 9;
            *(f32 *)((char *)arg0 + 0x480) = 1.0f;
            *(f32 *)((char *)arg0 + 0x484) = 1.0f;
            break;
        case 2:
        case 4:
        case 5:
        case 6:
            break;
        }
        FW(arg0, 0x3C0) = arg5;
        FW(arg0, 0x414) = arg9;
        temp_f0 = *(f32 *)((char *)FW(FW(arg9, 0xC), 0x70) + 0x14C);
        FW(arg0, 0x4B8) = 0;
        FW(arg0, 0x4B4) = 0;
        *(f32 *)((char *)arg0 + 0x494) = temp_f0;
        *(f32 *)((char *)arg0 + 0x498) = temp_f0;
        *(f32 *)((char *)arg0 + 0x49C) = temp_f0;
        timproc_uso_b5_alias_pf3(FW(FW(FW(FW(arg0, 0x40C), 0x54), 0x2B8), 0x108), 0.5f);
        timproc_uso_b5_alias_pf1(FW(FW(FW(FW(arg0, 0x40C), 0x58), 0x2B8), 0x108), 0.5f);
        if (*(s32 *)((char *)&D_00000000 + 0x34) != 2) {
            timproc_uso_b5_alias_pf2(FW(FW(FW(FW(arg0, 0x40C), 0x5C), 0x2B8), 0x108), 0.5f);
        }
        timproc_uso_b5_alias(arg0, (char *)1, arg10);
    }
    return arg0;
}
