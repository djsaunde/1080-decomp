/* timproc_uso_b5_func_0000DF14 (0x698 / 422 words) -- D884's sibling panel
 * constructor (the 0x2AC slot index, a 14-case switch on arg2 + 10 wiring the
 * 05D0E0 children, the saved 0x80000 bit restore, the arg3..arg8 placement).
 *
 * EXACT under IDO 7.1 cfe -O2 (agent-c run 217). Spliced over the unit's empty
 * placeholder by REPLACE_FUNC_BODY (Makefile TIMB5_DF14_DONOR) so the switch's
 * donor-local .rodata jumptable reloc is renamed
 * timproc_uso_b5_func_0000DF14_rodata and pinned to the table the USO ships at
 * RoData +0x400 (the target reads it through D_807FF420 + 0x400), right after
 * D884's table at +0x3C0.
 *
 * Run 217 fixes over the NM body: (1) the D884 flag-chain recipe (per-pair EE6C
 * aliases: each store and the reload after it share an alias, each alias is used
 * again after the switch calls so uopt leaves its address uncoloured; u32 stores,
 * s32 loads); (2) the 0x140 / 0x13C stores on one line; (3) `if (1) {}` right
 * after the head block's `rr = r` (the r reload is issued before the arg2 reload,
 * target order); (4) the tail block writes through gsub instead of a block-local
 * rr: the r reload then colours v1 and FW(arg0, 0x38) v0, as the target. */
#include "common.h"

#ifndef FW
#define FW(p, o) (*(int *)((char *)(p) + (o)))
#endif
extern int timproc_uso_b5_func_055750();
extern int timproc_uso_b5_func_04C678();
extern int timproc_uso_b5_func_07ACE0();
extern char D_00000000;
char *timproc_uso_b5_func_00002B74(char *arg0);
char *timproc_uso_b5_func_000032C8(char *arg0);
extern int timproc_uso_b5_func_00003890();
/* Prototyped 5-arg decl: real signature passes f32 singles (mfc1 a2/a3 +
 * swc1 16(sp) 5th arg in the jal delay). */
extern int timproc_uso_b5_func_0546DC(char *, char *, f32, f32, f32);
extern int timproc_uso_b5_func_05D0E0();
extern int timproc_uso_b5_func_04DFFC();
extern char timproc_uso_b5_D_807FDB64;
extern char timproc_uso_b5_D_807FEBB4;
extern char timproc_uso_b5_D_807FA914;
extern char import_800200CC;
extern char import_80807FB8;
extern char import_8077EBB4;
extern char import_8077A914;
extern char import_80806674;
extern char import_80776674;
/* Data symbols subscripted as byte arrays so &SYM[N] forms HI16/LO16 against the named global. */
extern char timproc_uso_b5_D_807FEF48[];
extern char timproc_uso_b5_D_807FEF50[];
extern char timproc_uso_b5_D_807FEF5C[];
extern char timproc_uso_b5_D_807FEF64[];
extern char timproc_uso_b5_D_807FEF70[];
extern char timproc_uso_b5_D_807FEFA0[];
extern char timproc_uso_b5_D_807FEFAC[];
extern char timproc_uso_b5_D_807FEFB8[];
extern char timproc_uso_b5_D_807FEFC4[];
extern char timproc_uso_b5_D_807FEFD0[];
extern char timproc_uso_b5_D_807FEFDC[];
extern char timproc_uso_b5_D_807FEFE8[];
extern char timproc_uso_b5_D_807FEFF4[];
extern char import_80087FB8[];
/* import_8005EE6C aliases (all 0): l1/l2 single reads, s1/s2/s4 store+reload pairs, s7 else-arm store */
extern char df14_ee6c_l1, df14_ee6c_l2;
extern char df14_ee6c_s1, df14_ee6c_s2, df14_ee6c_s4, df14_ee6c_s7;
#define EE6C_L(s) (*(s32 *)((char *)&s + 4))
#define EE6C_S(s) (*(u32 *)((char *)&s + 4))

char *timproc_uso_b5_func_0000DF14(char *arg0, s32 arg1, s32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8) {
    char *volatile r;
    volatile int pad0;
    char *sub;
    char *child;
    volatile int pad[7];
    s32 flag;
    f32 *p;
    char *gsub;
    s32 mode;
    f32 t;

    child = (char *) timproc_uso_b5_func_055750((char *)0x2BC);
    if (child != 0) {
        sub = child;
        if ((sub != 0) || ((sub = (char *) timproc_uso_b5_func_055750((char *)0x2B8)) != 0)) {
            timproc_uso_b5_func_04C678(sub, &timproc_uso_b5_D_807FEF48[5848]);
            FW(sub, 0x28) = (s32) (&timproc_uso_b5_D_807FDB64 + 0);
            timproc_uso_b5_func_00002B74(sub + 0x2C);
            timproc_uso_b5_func_000032C8(sub + 0x194);
        }
        FW(child, 0x28) = (s32) &timproc_uso_b5_D_807FEF50[2468]; FW(child, 0xC) = (s32) &timproc_uso_b5_D_807FEF50[5856];
        timproc_uso_b5_func_00003890(child);
    }
    r = child;
    FW(child, 0x2AC) = FW(arg0, 0x6C);
    timproc_uso_b5_func_07ACE0(arg0 + 0x10, child);
    if (FW(child, 0x14) != 0) {
        FW(child, 0x4) = 1;
    }
    {
        char *rr = r;
        if (1) {} /* run 217: the r reload issues before the arg2 reload (target) */
        FW(child, 0x14) = (s32) arg0;
        *(f32 *)(rr + 0x2A4) = (f32) arg1; /* run 216: cvt store before the 0x2B0 store */
        FW(rr, 0x2B0) = arg2;
        sub = (char *)(arg2 + 10);
    }
    child = (char *) timproc_uso_b5_func_055750((char *)0x144);
    if (child != 0) {
        timproc_uso_b5_func_0546DC(child, &timproc_uso_b5_D_807FEF5C[5868], 0.0f, 0.0f, 0.0f);
        FW(child, 0x28) = (s32) &timproc_uso_b5_D_807FEF64[1300]; FW(child, 0xC) = (s32) &timproc_uso_b5_D_807FEF64[5876];
        gsub = (char *) timproc_uso_b5_func_055750((char *)0x16C);
        if (gsub != 0) {
            timproc_uso_b5_func_0546DC(gsub, &timproc_uso_b5_D_807FEF70[5888], 0.0f, 0.0f, 0.0f);
            t = *(f32 *)(&import_80807FB8 + 1020);
            FW(gsub, 0x28) = (s32) &import_80087FB8[0]; *(s32 *)(gsub + 0x120) = 1; /* run 216: one line = D884's store order (0x120 first) */
            *(f32 *)(gsub + 0x108) = t;
            *(f32 *)(gsub + 0x10C) = t;
            *(f32 *)(gsub + 0x110) = t;
            *(f32 *)(gsub + 0x124) = 1.0f;
        }
        FW(child, 0x108) = (s32) gsub;
        timproc_uso_b5_func_04DFFC(child, gsub);
        if (1) {}
        p = (f32 *)((char *) FW(child, 0x108) + 0xB4);
        p[2] = 0; /* int 0: one mtc1 zero for the three stores (0.0f gives three) */
        p[1] = 0;
        p[0] = 0;
        *(f32 *)(child + 0x124) = 1.0f;
        /* flag chain on one line (run 216); per-pair aliases (run 217, see top) */
        flag = EE6C_L(df14_ee6c_l1) & 0x80000; EE6C_S(df14_ee6c_s1) = EE6C_L(df14_ee6c_l2) & 0xFFF7FFFF; EE6C_S(df14_ee6c_s2) = EE6C_L(df14_ee6c_s1) & ~1; EE6C_S(df14_ee6c_s2) = EE6C_L(df14_ee6c_s2) | 2; EE6C_S(df14_ee6c_s4) = EE6C_L(df14_ee6c_s2) & 0xFFFDFFFF; EE6C_S(df14_ee6c_s4) = EE6C_L(df14_ee6c_s4) | 0x2000;
        FW(child, 0x140) = 1; FW(child, 0x13C) = 1;
        switch ((s32) sub) {
        case 0:
            if (*(s32 *)(&import_800200CC + 52) == 3) {
                timproc_uso_b5_func_04DFFC(child, timproc_uso_b5_func_05D0E0(0, (&timproc_uso_b5_D_807FEBB4 + 5900), (&import_8077EBB4 + 0), 0));
            }
            FW(child, 0x13C) = 0;
            break;
        case 1:
            if (*(s32 *)(&import_800200CC + 52) == 3) {
                timproc_uso_b5_func_04DFFC(child, timproc_uso_b5_func_05D0E0(0, (&timproc_uso_b5_D_807FA914 + 5912), (&import_8077A914 + 0), 0));
            }
            FW(child, 0x13C) = 0;
            break;
        case 2:
            if (*(s32 *)(&import_800200CC + 52) == 3) {
                timproc_uso_b5_func_04DFFC(child, timproc_uso_b5_func_05D0E0(0, (&import_80806674 + 5924), (&import_80776674 + 0), 0));
            }
            FW(child, 0x13C) = 0;
            break;
        case 3:
            if (*(s32 *)(&import_800200CC + 52) != 2) {
                timproc_uso_b5_func_04DFFC(child, timproc_uso_b5_func_05D0E0(0, &timproc_uso_b5_D_807FEFA0[5936], (&D_00000000 + 0), 0));
            }
            break;
        case 4:
            if (*(s32 *)(&import_800200CC + 52) != 2) {
                timproc_uso_b5_func_04DFFC(child, timproc_uso_b5_func_05D0E0(0, &timproc_uso_b5_D_807FEFAC[5948], (&D_00000000 + 0), 0));
            }
            break;
        case 5:
            mode = *(s32 *)(&import_800200CC + 52);
            if ((mode != 3) && (mode != 5)) {
                timproc_uso_b5_func_04DFFC(child, timproc_uso_b5_func_05D0E0(0, &timproc_uso_b5_D_807FEFB8[5960], (&D_00000000 + 0), 0));
            }
            break;
        case 6:
            mode = *(s32 *)(&import_800200CC + 52);
            if ((mode != 3) && (mode != 5)) {
                timproc_uso_b5_func_04DFFC(child, timproc_uso_b5_func_05D0E0(0, &timproc_uso_b5_D_807FEFC4[5972], (&D_00000000 + 0), 0));
            }
            break;
        case 7:
            mode = *(s32 *)(&import_800200CC + 52);
            if ((mode != 3) && (mode != 5)) {
                timproc_uso_b5_func_04DFFC(child, timproc_uso_b5_func_05D0E0(0, &timproc_uso_b5_D_807FEFD0[5984], (&D_00000000 + 0), 0));
            }
            break;
        case 8:
            mode = *(s32 *)(&import_800200CC + 52);
            if ((mode != 3) && (mode != 5)) {
                timproc_uso_b5_func_04DFFC(child, timproc_uso_b5_func_05D0E0(0, &timproc_uso_b5_D_807FEFDC[5996], (&D_00000000 + 0), 0));
            }
            break;
        case 9:
            mode = *(s32 *)(&import_800200CC + 52);
            if ((mode != 3) && (mode != 5)) {
                timproc_uso_b5_func_04DFFC(child, timproc_uso_b5_func_05D0E0(0, &timproc_uso_b5_D_807FEFE8[6008], (&D_00000000 + 0), 0));
            }
            break;
        case 10:
            mode = *(s32 *)(&import_800200CC + 52);
            if ((mode != 3) && (mode != 5)) {
                timproc_uso_b5_func_04DFFC(child, timproc_uso_b5_func_05D0E0(0, &timproc_uso_b5_D_807FEFF4[6020], (&D_00000000 + 0), 0));
            }
            break;
        case 11:
        case 12:
        case 13:
            break;
        }
        /* one line (D884's lui order fix, run 216) */
        if (flag) EE6C_S(df14_ee6c_s2) = EE6C_L(df14_ee6c_s1) | 0x80000; else EE6C_S(df14_ee6c_s7) = EE6C_L(df14_ee6c_s4) & 0xFFF7FFFF;
        *(f32 *)(child + 0xDC) = arg3;
        *(f32 *)(child + 0xE0) = arg4;
        *(f32 *)(child + 0xE4) = arg5;
        *(f32 *)(child + 0x10C) = arg6;
        *(f32 *)(child + 0x110) = arg7;
        *(f32 *)(child + 0x114) = arg8; /* arg8 right after arg7, 0x120 before 0x124: as1 then issues the stack-arg loads like the target (run 161) */
        FW(child, 0x134) = 0;
        FW(child, 0x130) = 0;
        *(f32 *)(child + 0x128) = 0.0f;
        *(f32 *)(child + 0x11C) = 0.0f;
        *(f32 *)(child + 0x120) = *(f32 *)&import_80087FB8[1080];
        *(f32 *)(child + 0x124) = 0.0f;
    }
    {
        gsub = r; /* run 217: through gsub (a block-local rr took v0 from FW(arg0, 0x38)) */
        FW(gsub, 0x2B8) = (s32) child;
        FW(gsub, 0x29C) = (s32) child;
        *(f32 *)(gsub + 0x134) = -32.0f; if (1) {}
    }
    sub = (char *) FW(arg0, 0x38);
    timproc_uso_b5_func_07ACE0(sub + 0x10, child);
    if (FW(child, 0x14) != 0) {
        FW(child, 0x4) = 1;
    }
    FW(child, 0x14) = (s32) sub;
    child = (char *)FW(arg0, 0x6C);
    FW(arg0, 0x6C) = (s32) child + 1;
    FW(arg0 + (s32) child * 4, 0x3C) = (s32) r;
    return r;
}
