#include "common.h"
#include "gdl/gl_data.h"
#include "gdl/gadget.h"
#include "gdl/race.h"
#include "game/input.h"
#include "gdl/item.h"
#include "gdl/pad.h"
#include "game/course.h"
#include "game/snowboarder.h"
#include "gdl/window.h"
#include "uso_call.h"

extern int D_00000000;
typedef struct { int a, b, c, d; } Quad4;

/* gl_func_00000000 (0x1CC) was the tail of PointsGadget_display_FFFFFFE0, whose 8-word
 * hoisted head sits before VRAM 0: it moved to game_libs_head.c (its own unit). */

/* TimeGadget_ctor: get-or-create constructor (0x13C object, one inlined
 * 0x10C base sub-object) + colour normalize (0x13C, 79 words). EXACT in-unit.
 * arg0 IS self (reassigned by the || alloc fallback): a separate self local
 * adds a named home (frame 0x40 instead of 0x38) and schedules `or s0,a0`
 * into the bnez delay slot. The sub pointer is declared BEFORE d255 so its
 * spill lands at +0x34 (d255 first -> +0x30). d255 keeps the four
 * divisions at run time. */
extern int gl_func_00000000();
char *TimeGadget_ctor(char *arg0, s32 arg1) {
    char *sub;
    f32 d255 = 255.0f;

    if ((arg0 != 0) || (arg0 = (char *)CALL(mem_new)(0x13C), (arg0 != 0))) {
        sub = arg0;
        if ((arg0 != 0) || (sub = (char *)CALL(mem_new)(0x10C), (sub != 0))) {
            CALL(Node_ctor)(sub, GL_DATA_STR(0xCAF4, "<Node>"));
            ((Node *)sub)->vptr = (char *)&TimeGadget_vtbl;
            CALL(TimeGadget_ctor_00013D40)(sub + 0x2C);
        }
        ((Gadget *)arg0)->node.name = (const char *)(sTimeGadgetName);
        CALL(gui_spriteInit)(((Gadget *)arg0)->sprite0, 0x50000);
        CALL(gui_spriteInit)(((Gadget *)arg0)->sprite1, 0x50007);
        CALL(TimeGadgetParms_set104_00014004)(arg0, ((Gadget *)arg0)->sprite1, ((Gadget *)arg0)->sprite0);
        ((Gadget *)arg0)->unkD4 = 0;
        ((Gadget *)arg0)->alpha = 0xFF;
        ((Gadget *)arg0)->labelAlpha = 0x82;
        ((Gadget *)arg0)->colour[0] = 250.0f / d255;
        ((Gadget *)arg0)->colour[1] = 235.0f / d255;
        ((Gadget *)arg0)->colour[2] = 100.0f / d255;
        ((Gadget *)arg0)->colour[3] = 0.0f / d255;
        ((Gadget *)arg0)->font = arg1;
        CALL(TimeGadgetParms_setLabelPos_00013FEC)(arg0, -78, -1, -51, 0);
    }
    return arg0;
}

extern int gl_func_00000000();
int TimeGadget_release(char *a0) {
    CALL(gui_spriteRelease)(((Gadget *)a0)->sprite0);
    return CALL(gui_spriteRelease)(((Gadget *)a0)->sprite1);
}

/* TimeGadget_setTime: MATCHED 2026-05-31. ms time formatter — SIGNED div (the
 * break-0x7/0x6 div-by-zero+INT_MIN-overflow traps confirm signed, not unsigned):
 * mins = a1/60000, rem = a1 - mins*60000, secs = rem/1000, ms = (rem - secs*1000)/10;
 * then sprintf(out+0xE4, &D+0xCB08, mins, secs, ms). The old stub had a1 unsigned +
 * wrong arithmetic (a1-mins, rem-secs*10). */
extern int gl_func_00000000();
void TimeGadget_setTime(char *a0, int a1) {
    int mins = a1 / 60000;
    int rem = a1 - mins * 60000;
    int secs = rem / 1000;
    int ms_part = (rem - secs * 1000) / 10;
    CALL(gl_sprintf)(((Gadget *)a0)->text, GL_DATA_STR(0xCB08, "%db%02da%02d"), mins, secs, ms_part);
}

/* BestTimeGadget_ctor: three-level get-or-create constructor (0x13C object ->
 * 0x10C sub -> 0x10C sub2) + colour normalize (0x174, 93 words). EXACT
 * in-unit. Same arg0-is-self || fallback as TimeGadget_ctor at the outer
 * level, but the target frame (0x38) has only four slots: the sub/sub2
 * spill homes and the two CSE'd obj+0x10C/obj+0x124 call-arg temps. A
 * named f32 d255 divisor costs a fifth (unused) home -> frame 0x40, so the
 * runtime div.s comes from store-forwarding instead: store the raw colour,
 * then `/= 255.0f` in place. cfe cannot fold a memory operand; uopt forwards
 * the 250/235/100/0 constants but never folds a float divide, and the first
 * stores die. */
extern int gl_func_00000000();
char *BestTimeGadget_ctor(char *arg0, s32 arg1) {
    char *sub;
    char *sub2;

    if ((arg0 != 0) || (arg0 = (char *)CALL(mem_new)(0x13C), (arg0 != 0))) {
        sub = arg0;
        if ((arg0 != 0) || (sub = (char *)CALL(mem_new)(0x10C), (sub != 0))) {
            sub2 = sub;
            if ((sub != 0) || (sub2 = (char *)CALL(mem_new)(0x10C), (sub2 != 0))) {
                CALL(Node_ctor)(sub2, GL_DATA_STR(0xCB18, "<Node>"));
                ((Node *)sub2)->vptr = (char *)&XDATA(TimeGadget_vtbl, port_bootup_data, 0xC77C);
                CALL(TimeGadget_ctor_00013D40)(sub2 + 0x2C);
            }
            ((Node *)sub)->vptr = (char *)&XDATA(BestGadget_vtbl, port_bootup_data, 0xC8B4);
        }
        ((Gadget *)arg0)->node.name = (const char *)(sBestTimeGadgetName);
        CALL(gui_spriteInit)(((Gadget *)arg0)->sprite0, 0x5001B);
        CALL(gui_spriteInit)(((Gadget *)arg0)->sprite1, 0x5001D);
        CALL(TimeGadgetParms_set104_00014004)(arg0, ((Gadget *)arg0)->sprite1, ((Gadget *)arg0)->sprite0);
        ((Gadget *)arg0)->unkD4 = 0;
        ((Gadget *)arg0)->alpha = 0xFF;
        ((Gadget *)arg0)->labelAlpha = 0x82;
        ((Gadget *)arg0)->colour[0] = 250.0f;
        ((Gadget *)arg0)->colour[1] = 235.0f;
        ((Gadget *)arg0)->colour[2] = 100.0f;
        ((Gadget *)arg0)->colour[3] = 0.0f;
        ((Gadget *)arg0)->colour[0] /= 255.0f;
        ((Gadget *)arg0)->colour[1] /= 255.0f;
        ((Gadget *)arg0)->colour[2] /= 255.0f;
        ((Gadget *)arg0)->colour[3] /= 255.0f;
        ((Gadget *)arg0)->font = arg1;
        CALL(TimeGadgetParms_setLabelPos_00013FEC)(arg0, -80, -1, -61, 0);
    }
    return arg0;
}

extern int gl_func_00000000();
int BestTimeGadget_release(char *a0) {
    CALL(gui_spriteRelease)(((Gadget *)a0)->sprite0);
    return CALL(gui_spriteRelease)(((Gadget *)a0)->sprite1);
}

extern int gl_func_00000000();
extern char sFmt_b_a_CB30;

void BestTimeGadget_setTime(char *a0, char a1, char a2, char a3) {
    CALL(gl_sprintf)(((Gadget *)a0)->text, &sFmt_b_a_CB30, a1, a2, a3);
}

/* BestScoreGadget_ctor: twin of BestTimeGadget_ctor (0x174, 93 words; vtable
 * 0xCB40/0xCB48, ids 0x5001E/0x5001D, tint -56/-1/-38). EXACT in-unit;
 * same store-then-/= colour normalize (no named divisor home). */
extern int gl_func_00000000();
char *BestScoreGadget_ctor(char *arg0, s32 arg1) {
    char *sub;
    char *sub2;

    if ((arg0 != 0) || (arg0 = (char *)CALL(mem_new)(0x13C), (arg0 != 0))) {
        sub = arg0;
        if ((arg0 != 0) || (sub = (char *)CALL(mem_new)(0x10C), (sub != 0))) {
            sub2 = sub;
            if ((sub != 0) || (sub2 = (char *)CALL(mem_new)(0x10C), (sub2 != 0))) {
                CALL(Node_ctor)(sub2, GL_DATA_STR(0xCB40, "<Node>"));
                ((Node *)sub2)->vptr = (char *)&XDATA(TimeGadget_vtbl, port_bootup_data, 0xC77C);
                CALL(TimeGadget_ctor_00013D40)(sub2 + 0x2C);
            }
            ((Node *)sub)->vptr = (char *)&XDATA(BestGadget_vtbl, port_bootup_data, 0xC8B4);
        }
        ((Gadget *)arg0)->node.name = (const char *)(sBestScoreGadgetName);
        CALL(gui_spriteInit)(((Gadget *)arg0)->sprite0, 0x5001E);
        CALL(gui_spriteInit)(((Gadget *)arg0)->sprite1, 0x5001D);
        CALL(TimeGadgetParms_set104_00014004)(arg0, ((Gadget *)arg0)->sprite1, ((Gadget *)arg0)->sprite0);
        ((Gadget *)arg0)->unkD4 = 0;
        ((Gadget *)arg0)->alpha = 0xFF;
        ((Gadget *)arg0)->labelAlpha = 0x82;
        ((Gadget *)arg0)->colour[0] = 250.0f;
        ((Gadget *)arg0)->colour[1] = 235.0f;
        ((Gadget *)arg0)->colour[2] = 100.0f;
        ((Gadget *)arg0)->colour[3] = 0.0f;
        ((Gadget *)arg0)->colour[0] /= 255.0f;
        ((Gadget *)arg0)->colour[1] /= 255.0f;
        ((Gadget *)arg0)->colour[2] /= 255.0f;
        ((Gadget *)arg0)->colour[3] /= 255.0f;
        ((Gadget *)arg0)->font = arg1;
        CALL(TimeGadgetParms_setLabelPos_00013FEC)(arg0, -56, -1, -38, 0);
    }
    return arg0;
}

extern int gl_func_00000000();
int BestScoreGadget_release(char *a0) {
    CALL(gui_spriteRelease)(((Gadget *)a0)->sprite0);
    return CALL(gui_spriteRelease)(((Gadget *)a0)->sprite1);
}

extern int gl_func_00000000();
extern char sFmt_num_CB58;
int BestScoreGadget_setScore(char *a0, int arg1) {
    return CALL(gl_sprintf)(((Gadget *)a0)->text, &sFmt_num_CB58, arg1);
}

/* TimeGadgetParms_ctor: MATCH 2026-07-15 (agent-h) — 5-member composite constructor,
 * full ||-alloc-fallback reconstruction (was 59.4% m2c lift). Levers, in order:
 * (1) head `if (self != 0 || (self = alloc(0x68)) != 0)` with alloc-failure
 *     falling to the SHARED epilogue; per-member `if ((m = self+OFF) != 0 ||
 *     (m = alloc(0x18)) != 0)` — IDO folds m!=0 into `bne s0,-OFF` with the
 *     addiu in the delay slot.
 * (2) the arg value is a 4-byte STRUCT passed by value: `u1.v = <sym>; t = u1;
 *     ctor(m, self, t, 1)` — struct-by-value gives BOTH the arg-slot store
 *     `sw a2,8(sp)` in the ctor jal delay AND the join-order `lw a2,home`
 *     before `move a1,s0`; the struct copy keeps the loaded value a RING temp
 *     feeding both member stores (every scalar spelling colored it $v0);
 *     u1..u4 = per-expansion dead-store homes 0x44..0x38, t = shared 0x24.
 * (3) gl_ref_0000CBxx_i SYMBOL loads (not literal derefs) for destructive
 *     base reuse `lui t7; lw t7,%lo(t7)`; 4 DISTINCT gl_ref_0000C764_N
 *     aliases bust the cross-expansion &sym CSE (a shared extern spilled the
 *     address across calls); addr-store FIRST + one-line join flips the
 *     addr/value ring creation order while keeping sw 0x10-before-0xC.
 * (4) volatile pads pad1/pad2 = the 2 phantom frame slots between p and m.
 * 102/120 .o words exact; all 18 remaining are %hi/%lo reloc fields resolved
 * by undefined_syms (ROM cmp = gate). */
#ifndef FW
#define FW(p, o) (*(int *)((char *)(p) + (o)))
#endif
extern char sStr_TimeGadget_Parms_CB60;
extern char gl_ref_0000C764_1;
extern char gl_ref_0000C764_2;
extern char gl_ref_0000C764_3;
extern char gl_ref_0000C764_4;
extern int sTag_xps_CB74;
extern int sTag_yps_CB78;
extern int sTag_shx_CB7C;
extern int sTag_shy_CB80;
struct SB7BC { int v; };
char *TimeGadgetParms_ctor(char *self) {
    struct SB7BC u1;
    struct SB7BC u2;
    struct SB7BC u3;
    struct SB7BC u4;
    char *p;
    volatile int pad1;
    volatile int pad2;
    char *m;
    struct SB7BC t;
    if (self != 0 || (self = (char *)CALL(mem_new)(0x68)) != 0) {
        if ((p = self) != 0 || (p = (char *)CALL(mem_new)(8)) != 0) {
            PARMS(p)->name = &sStr_TimeGadget_Parms_CB60;
            PARMS(p)->first = 0;
        }
        u1.v = sTag_xps_CB74;
        t = u1;
        if ((m = self + 8) != 0 || (m = (char *)CALL(mem_new)(0x18)) != 0) {
            CALL(Parms_addParm)(m, self, t, 1);
            PARM(m)->type = &gl_ref_0000C764_1; PARM(m)->value = 0x1E; PARM(m)->unk14 = 0;
        }
        u2.v = sTag_yps_CB78;
        t = u2;
        if ((m = self + 0x20) != 0 || (m = (char *)CALL(mem_new)(0x18)) != 0) {
            CALL(Parms_addParm)(m, self, t, 1);
            PARM(m)->type = &gl_ref_0000C764_2; PARM(m)->value = 0x1E; PARM(m)->unk14 = 0;
        }
        u3.v = sTag_shx_CB7C;
        t = u3;
        if ((m = self + 0x38) != 0 || (m = (char *)CALL(mem_new)(0x18)) != 0) {
            CALL(Parms_addParm)(m, self, t, 1);
            PARM(m)->type = &gl_ref_0000C764_3; PARM(m)->value = 4; PARM(m)->unk14 = 0;
        }
        u4.v = sTag_shy_CB80;
        t = u4;
        if ((m = self + 0x50) != 0 || (m = (char *)CALL(mem_new)(0x18)) != 0) {
            CALL(Parms_addParm)(m, self, t, 1);
            PARM(m)->type = &gl_ref_0000C764_4; PARM(m)->value = -2; PARM(m)->unk14 = 0;
        }
    }
    return self;
}


/* 3-insn 2-field setter: xps / yps. No frame, no jal. */
void CountDownGadget_setPos(int *a0, int a1, int a2) {
    COUNTDOWN(a0)->parms.xps.value = a1;
    COUNTDOWN(a0)->parms.yps.value = a2;
}

/* 3-insn 2-field setter: the shadow offset shx / shy. Sibling of 0x999C. */
void CountDownGadget_setPair_000009A8(int *a0, int a1, int a2) {
    COUNTDOWN(a0)->parms.shx.value = a1;
    COUNTDOWN(a0)->parms.shy.value = a2;
}

extern int gl_func_00000000();
void CountDownGadget_display(char *arg0) {
    char *a2;
    volatile int pad[7];  /* phantom-slot lever: the target's 0x20 unused stack (cf. 628EC) */

    CALL(gui_setupRenderMode_0000055C)(COUNTDOWN(arg0)->arg1);
    a2 = arg0 + 0x94;
    CALL(gui_setPrimColor_000006B8)(COUNTDOWN(arg0)->arg1,
                     (int)(*(float *)&D_00000000 * (float)*(int *)&COUNTDOWN(arg0)->alpha), /* int-load spelling: the plain member recolours the fp regs */
                     a2, COUNTDOWN(arg0)->unkA4);
    CALL(gui_drawString_00000D04)(COUNTDOWN(arg0)->arg1, COUNTDOWN(arg0)->parms.xps.value,
                     COUNTDOWN(arg0)->parms.yps.value, arg0 + 0xB4);
    CALL(gui_setupRenderMode_0000055C)(COUNTDOWN(arg0)->arg2);
    CALL(gui_setPrimColor_000006B8)(COUNTDOWN(arg0)->arg2,
                     (int)(*(float *)&D_00000000 * (float)*(int *)&COUNTDOWN(arg0)->alpha), /* int-load spelling: the plain member recolours the fp regs */
                     a2, COUNTDOWN(arg0)->unkA4);
    CALL(gui_drawString_00000D04)(COUNTDOWN(arg0)->arg2,
                     COUNTDOWN(arg0)->parms.xps.value + COUNTDOWN(arg0)->parms.shx.value,
                     COUNTDOWN(arg0)->parms.yps.value + COUNTDOWN(arg0)->parms.shy.value,
                     arg0 + 0xBC);
}

/* CountDownGadget_ctor: get-or-create ctor + colour normalize (0x100, 64 words).
 * EXACT in-unit with the TimeGadget_ctor / 0FD0 shape (arg0 is self via the
 * || fallback; sub declared before the named d255 whose unused home is the
 * otherwise-empty sp+0x20 slot, sub spills to sp+0x24). Was 95.9 NM with a
 * separate obj local + goto ladder (move s0 in the bnez delay, sub at 0x20). */
extern int gl_func_00000000();
char *CountDownGadget_ctor(char *arg0, int a1, int a2) {
    char *sub;
    f32 d255 = 255.0f;

    if ((arg0 != 0) || (arg0 = (char *)CALL(mem_new)(0xD4), (arg0 != 0))) {
        sub = arg0;
        if ((arg0 != 0) || (sub = (char *)CALL(mem_new)(0xC4), (sub != 0))) {
            CALL(Node_ctor)(sub, GL_DATA_STR(0xCB84, "<Node>"));
            ((Node *)sub)->vptr = (char *)&CountDownGadget_vtbl;
            CALL(TimeGadgetParms_ctor)(sub + 0x2C);
        }
        ((CountDownGadget *)arg0)->colour[0] = 235.0f / d255;
        ((CountDownGadget *)arg0)->colour[1] = 80.0f / d255;
        ((CountDownGadget *)arg0)->colour[2] = 80.0f / d255;
        ((CountDownGadget *)arg0)->colour[3] = 0.0f / d255;
        ((CountDownGadget *)arg0)->node.name = (const char *)(sCountDownGadgetName);
        ((CountDownGadget *)arg0)->unkA4 = 0;
        ((CountDownGadget *)arg0)->alpha = 0xFF;
        ((CountDownGadget *)arg0)->arg1 = a1;
        ((CountDownGadget *)arg0)->arg2 = a2;
        CALL(CountDownGadget_setPair_000009A8)(arg0, 0x59, 2);
        ((CountDownGadget *)arg0)->blink = 0;
        ((CountDownGadget *)arg0)->unkC8 = 1;
    }
    return arg0;
}

void CountDownGadget_release(int a0) {}

/* CountDownGadget_setTime: ms->time formatter (a1 / 60000 = minutes, etc.).
 * Merged 2026-05-23: splat mis-split this at 0xBAC into CountDownGadget_setTime
 * (head: div a1,60000; v0<<4; multu v0,60000 — no prologue/jr) + gl_func_00000BAC
 * (body: mflo of B94's multu, prologue, sprintf-style calls, epilogue). The
 * multu(B94)/mflo(BAC) LO-register dependency proves they are one function;
 * IDO scheduled the division setup before the addiu sp prologue. */
/* NM 94.84 (agent-g run 62; was 66.7). Re-decode: no 4th `rem` arg on the
 * sprintf-style calls (the target's a3 = rem at the first call is a leftover),
 * the 0xCB9C..0xCBB8 format strings are the baked `(char *)&D_00000000 + K`
 * carry form, a typed struct (B4/BC buffers, C4..D0 counters), and the
 * centiseconds test is `(D0 == 0 && (cs = X) == 0) || (cs = X, C8 == 0)`
 * -- that is what emits X twice (multu-by-1000 copy, then the shift-chain
 * copy on the fall-through) with one shared `C4 = 8` block. Residual:
 *  - uopt hoists the >=100 call's `addiu a0,s0,180` from the join block up
 *    into the entry block (target computes it at the join, before slti);
 *  - frame 0x48 vs 0x40 (cs home 0x24 is right here; dropping the
 *    `temp_a2` local and testing `unkCC != unkD0` gives frame 0x40 but cs at
 *    0x20, 94.82; a named `secs` local or a volatile pad adds 8);
 *  - `addu v1,t7,t0` vs target `addu v1,t0,t7` for qmin*60 + qsec (both
 *    source orders, and a two-step +=, emit t7,t0);
 *  - C8 store scheduled before the div-check instead of after the rem subu.
 * Passing rem to the first two calls puts it in s1 (88.2). */
#ifdef NON_MATCHING
#ifndef FW
#define FW(p, o) (*(int *)((char *)(p) + (o)))
#endif

void CountDownGadget_setTime(CountDownGadget *arg0, s32 arg1, s32 arg2) {
    s32 sp24;
    s32 qmin;
    s32 rem;
    s32 qsec;
    s32 temp_a2;
    s32 temp_v0;

    qmin = arg1 / 60000;
    rem = arg1 - (qmin * 60000);
    arg0->unkC8 = arg2;
    qsec = rem / 1000;
    arg0->secs = qmin * 60 + qsec;
    if ((arg0->secs == 0 && (sp24 = (rem - (qsec * 1000)) / 10) == 0) ||
        (sp24 = (rem - (qsec * 1000)) / 10, arg0->unkC8 == 0)) {
        arg0->blink = 8;
    }
    temp_a2 = arg0->secs;
    if (temp_a2 >= 100) {
        gl_func_00000000(arg0->secsText, (char *)&D_00000000 + 0xCB9C, temp_a2);
    } else if (temp_a2 >= 10) {
        gl_func_00000000(arg0->secsText, (char *)&D_00000000 + 0xCBA0, temp_a2);
    } else {
        if (temp_a2 != arg0->lastSecs) {
            gl_func_00000000(19, temp_a2);
        }
        temp_v0 = arg0->blink;
        arg0->blink = temp_v0 + 1;
        if (temp_v0 & 8) {
            gl_func_00000000(arg0->secsText, (char *)&D_00000000 + 0xCBA8, arg0->secs);
        } else {
            gl_func_00000000(arg0->secsText, (char *)&D_00000000 + 0xCBB0, arg0->secs);
        }
        arg0->lastSecs = arg0->secs;
    }
    gl_func_00000000(arg0->csText, (char *)&D_00000000 + 0xCBB8, sp24);
}
#else
INCLUDE_ASM("asm/nonmatchings/game_libs/game_libs", CountDownGadget_setTime);
#endif

/* PointsGadget_ctor - verified structural decode (0x118, 70 insns,
 * get-or-create constructor + child wiring).
 * Struct-typing reference: object = 316 (0x13C) bytes. s0->0xC (12)
 * descriptor/vtable ptr (&D); sub-object (separate alloc) ->0x28
 * (40) descriptor (&D); s0+0x10C (268) and s0+0x124 (292) = two
 * embedded child sub-objects init'd with packed ids 0x050001 /
 * 0x050007 then cross-wired; field block s0->0xC4/0xC8/0xCC
 * (196/200/204) f32 = a global default (loaded from &D + 0xC70),
 * s0->0xD0 (208) f32 = 0.0, s0->0xD4 (212)=0, s0->0xD8 (216)=255,
 * s0->0xDC (220)=130, s0->0xE0 (224)=a1. Final
 * gl_func_00000000(s0,-56,-1,-13) = a (mode,-1,-13) init.
 * Caps <80: get-or-create branch + ~8 gl_func_00000000 reloc calls +
 * &D descriptor relocs + FP const + packed-id args. INCLUDE_ASM
 * remains build path (no episode; tautology-trap rule). */
/* PointsGadget_ctor - get-or-create constructor + child wiring (0x118,
 * 70 insns). Two nested get-or-create branches: outer allocs the main
 * object s0 (316/0x13C bytes) when arg null; inner allocs a sub-object
 * `sub` (268/0x10C bytes) when null, then init-calls
 * gl_func_00000000(sub, &D+0xCBC0). sub->0x28 = &D descriptor, then
 * gl_func_00000000(sub+0x2C). s0->0xC = &D+0xCBC8 descriptor. Two
 * embedded children at s0+268 / s0+292 init'd with packed ids
 * 0x050001 / 0x050007 then cross-wired via
 * gl_func_00000000(s0, s0+292, s0+268). Field block 0xC4/0xC8/0xCC f32
 * = global default (&D+0xC70); 0xD0 f32 = 0.0; 0xD4=0; 0xD8=255;
 * 0xDC=130; 0xE0=a1. Final gl_func_00000000(s0,-56,-1,-13,0).
 * MATCHED 2026-07-11 (byte-exact): BASE-SYMBOLIZE (pointer-arith (char*)&D+off,
 * not (int)&D+off which folds to a raw `ori` since D is absolute-0) recovered
 * the lui/addiu descriptor relocs; outer alloc-fail routed to shared `done`
 * (returns s0=0) not a separate `return 0`; inner alloc-fail skips only the
 * sub-init block (goto skip_init) rather than the whole tail. */
void *PointsGadget_ctor(char *s0, int a1) {
    char *sub;
    float k;
    if (s0 == 0) {
        s0 = (char*)CALL(mem_new)(316);
        if (s0 == 0) goto done;
    }
    sub = s0;
    if (sub == 0) {
        sub = (char*)CALL(mem_new)(268);
        if (sub == 0) goto skip_init;
    }
    CALL(Node_ctor)(sub, GL_DATA_STR(0xCBC0, "<Node>"));
    ((Node *)sub)->vptr = (char *)&TimeGadget_vtbl;
    CALL(TimeGadget_ctor_00013D40)(sub + 0x2C);
skip_init:
    ((Gadget *)s0)->node.name = (const char *)(sPointsGadgetName);
    CALL(gui_spriteInit)(((Gadget *)s0)->sprite0, 0x050001);
    CALL(gui_spriteInit)(((Gadget *)s0)->sprite1, 0x050007);
    CALL(TimeGadgetParms_set104_00014004)(s0, ((Gadget *)s0)->sprite1, ((Gadget *)s0)->sprite0);
    k = *(float*)(GL_RODATA(0xC70));
    ((Gadget *)s0)->unkD4 = 0;
    ((Gadget *)s0)->alpha = 255;
    ((Gadget *)s0)->labelAlpha = 130;
    ((Gadget *)s0)->colour[0] = k;
    ((Gadget *)s0)->colour[1] = k;
    ((Gadget *)s0)->colour[2] = k;
    ((Gadget *)s0)->colour[3] = 0.0f;
    ((Gadget *)s0)->font = a1;
    CALL(TimeGadgetParms_setLabelPos_00013FEC)(s0, -56, -1, -13, 0);
done:
    return s0;
}

extern int gl_func_00000000();
int PointsGadget_release(char *a0) {
    CALL(gui_spriteRelease)(((Gadget *)a0)->sprite0);
    return CALL(gui_spriteRelease)(((Gadget *)a0)->sprite1);
}

extern int gl_func_00000000();
extern char sFmt_num_CBD8;
int PointsGadget_setPoints(char *a0, int arg1) {
    return CALL(gl_sprintf)(((Gadget *)a0)->text, &sFmt_num_CBD8, arg1);
}

/* PointsGadget_ctor_00000ED0: constructor. obj = a0 ?: alloc(212); a defensive second
 * obj-or-alloc (node); init(node, &D+0xCBE0); node->0x28 = &D; init(node+0x2C);
 * then field inits (vtable ptr &D+0xCBE8, three 0xC74-floats, flags). Returns
 * obj. Matched: param-direct (reuse a0 instead of separate obj local) puts obj
 * in $a2 / node in $a3; second-alloc failure falls through to field-init (not
 * ret); 0xA0/0xB0 store-order swap fixes the as1 scheduler tie. */
int PointsGadget_ctor_00000ED0(int a0, int a1) {
    int node;
    float v;
    if (a0 == 0) {
        a0 = CALL(mem_new)(212);
        if (a0 == 0) {
            goto ret;
        }
    }
    node = a0;
    if (node == 0) {
        node = CALL(mem_new)(212);
        if (node == 0) {
            goto initfields;
        }
    }
    CALL(Node_ctor)(node, GL_DATA_STR(0xCBE0, "<Node>"));
    ((Node *)node)->vptr = (char *)&PointsGadget_vtbl_C854;
    CALL(FloatPosParms_ctor)(node + 0x2C);
initfields:
    v = *(float*)(GL_RODATA(0xC74));
    POPUP(a0)->node.name = (const char *)(sPointsGadgetName_CBE8);
    POPUP(a0)->unk74 = 0;
    POPUP(a0)->alpha = 255;
    POPUP(a0)->colour[0] = v;
    POPUP(a0)->colour[1] = v;
    POPUP(a0)->colour[2] = v;
    POPUP(a0)->colour[3] = 0.0f;
    POPUP(a0)->arg = a1;
    POPUP(a0)->font = 0;
ret:
    return a0;
}

void PointsGadget_release_00000F9C(int a0) {
}

extern int gl_func_00000000();
extern char gl_ref_0000CBF8;
int PointsGadget_setValue(char *a0, int arg1) {
    return CALL(gl_sprintf)(a0 + 0xB4, &gl_ref_0000CBF8, arg1);
}

/* SpeedGadget_ctor: two-level get-or-create ctor + colour normalize (0x144,
 * 81 words). EXACT in-unit with the TimeGadget_ctor shape verbatim (arg0 is
 * self via the || fallback, sub declared before the named d255 whose unused
 * home completes the 0x38 frame); only the vtable/ids/0xD4 value differ. The
 * old wrap's separate self local + volatile pad scheduled `or s0,a0` into
 * the bnez delay slot. */
extern int gl_func_00000000();
char *SpeedGadget_ctor(char *arg0, s32 arg1) {
    char *sub;
    f32 d255 = 255.0f;

    if ((arg0 != 0) || (arg0 = (char *)CALL(mem_new)(0x13C), (arg0 != 0))) {
        sub = arg0;
        if ((arg0 != 0) || (sub = (char *)CALL(mem_new)(0x10C), (sub != 0))) {
            CALL(Node_ctor)(sub, GL_DATA_STR(0xCBFC, "<Node>"));
            ((Node *)sub)->vptr = (char *)&TimeGadget_vtbl;
            CALL(TimeGadget_ctor_00013D40)(sub + 0x2C);
        }
        ((Gadget *)arg0)->node.name = (const char *)(sSpeedGadgetName);
        CALL(gui_spriteInit)(((Gadget *)arg0)->sprite0, 0x50002);
        CALL(gui_spriteInit)(((Gadget *)arg0)->sprite1, 0x50008);
        CALL(TimeGadgetParms_set104_00014004)(arg0, ((Gadget *)arg0)->sprite1, ((Gadget *)arg0)->sprite0);
        ((Gadget *)arg0)->unkD4 = 0x80;
        ((Gadget *)arg0)->alpha = 0xFF;
        ((Gadget *)arg0)->labelAlpha = 0x82;
        ((Gadget *)arg0)->colour[0] = 250.0f / d255;
        ((Gadget *)arg0)->colour[1] = 235.0f / d255;
        ((Gadget *)arg0)->colour[2] = 100.0f / d255;
        ((Gadget *)arg0)->colour[3] = 0.0f / d255;
        ((Gadget *)arg0)->font = arg1;
        CALL(TimeGadgetParms_setLabelPos_00013FEC)(arg0, -3, -1, 0x25, 0);
    }
    return arg0;
}

extern int gl_func_00000000();
int SpeedGadget_release(char *a0) {
    return CALL(gui_spriteRelease)(((Gadget *)a0)->sprite0);
}

/* SpeedGadget_setSpeed: 3-way value-formatter dispatcher — picks one of
 * 3 format strings by a1's magnitude and calls the cross-USO
 * printf-like gl_func_00000000. Per-arm arg counts/buffers differ. */
extern int gl_data_00000000;
void SpeedGadget_setSpeed(char *a0, int a1) {
    if (a1 < 10) {
        CALL(gl_sprintf)(((Gadget *)a0)->text, (char*)&gl_data_00000000 + 0xCC10, a1);
    } else if (a1 < 100) {
        CALL(gl_sprintf)(((Gadget *)a0)->text, (char*)&gl_data_00000000 + 0xCC18);
    } else {
        CALL(gl_sprintf)(((Gadget *)a0)->text, (char*)&gl_data_00000000 + 0xCC1C);
    }
}

/* BoardsGadget_ctor: MATCH 2026-07-15 (agent-h) — ||-alloc-fallback rewrite of
 * the goto-form get-or-create body (89.7 -> 100). The documented residual
 * ("prologue scheduler tie + sub spill 0x24 vs 0x20 — not C-coaxable") was
 * the CONTROL-FLOW SPELLING: `if (self != 0 || (self = alloc(0x140)) != 0)`
 * param-reassign head gives move-s0-before-bnez with sw ra in the delay, and
 * the inner `if ((sub = self) != 0 || (sub = alloc(0x10C)) != 0)` recolors
 * sub to $a2 with home 0x24. Distinct abs-0 alias gl_data_00000000_11a4 for
 * the 0x54 load base busts the &gl_data CSE with the self->0x28 store
 * (target rematerializes; shared symbol spilled a candidate). 54/54 words. */
extern int gl_data_00000000;
extern int gl_data_00000000_11a4;
int* BoardsGadget_ctor(int *self) {
    int *sub;
    float f;
    if (self != 0 || (self = (int*)CALL(mem_new)(0x140)) != 0) {
        if ((sub = self) != 0 || (sub = (int*)CALL(mem_new)(0x10C)) != 0) {
            CALL(Node_ctor)(sub, (char*)&gl_data_00000000 + 0xCC20);
            ((Node *)sub)->vptr = (char *)&TimeGadget_vtbl;
            CALL(TimeGadget_ctor_00013D40)((char*)sub + 0x2C);
        }
        ((BoardsGadget *)self)->node.vptr = (char *)&gl_data_00000000;
        ((BoardsGadget *)self)->node.name = (const char *)((char*)&gl_data_00000000 + 0xCC28);
        CALL(gui_spriteInit)(((BoardsGadget *)self)->sprite,
            *(int*)((char*)&gl_data_00000000_11a4 + 0x54) | 0x001E0000);
        f = *(float*)(GL_RODATA(0xC78));
        ((BoardsGadget *)self)->unkD4 = 0;
        ((BoardsGadget *)self)->unkD8 = 0xFF;
        ((BoardsGadget *)self)->count = 0;
        ((BoardsGadget *)self)->colour[0] = f;
        ((BoardsGadget *)self)->colour[1] = f;
        ((BoardsGadget *)self)->colour[2] = f;
        ((BoardsGadget *)self)->colour[3] = 0.0f;
    }
    return self;
}

void BoardsGadget_release(char *a0) {
    CALL(gui_spriteRelease)(((BoardsGadget *)a0)->sprite);
}

/* BoardsGadget_display: Quad4(1.0f) colour + (int)(255 * alpha) setup, then a
 * per-entry loop over self->0x10C (0xB8, 46 words). EXACT in-unit. The
 * "caller-set $f0/$f4/$f6 float convention cap" was a splat boundary error:
 * the six words before the old 0x12B4 symbol (`lui/mtc1` 1.0f -> f0, 255.0f
 * -> f4, `lwc1 f6` of D_00C75C) are this function's own constant hoist that
 * IDO -O2 schedules ABOVE `addiu sp` (IDO_CODEGEN stolen-prologue FP-const
 * note). They were folded from gl_func_0000127C_pad.s into this .s (symbol
 * now at 0x129C; expected/ re-baselined, .text byte-identical). The float is
 * its own sym (TextReloc sym 858), so a REAL `float` extern
 * gl_data_00000000_12b4f: through `*(float *)&D_00000000` the load takes f4
 * and 255.0f f6 (pair swapped). Unreferenced `char pad[0x20]` after buf
 * gives the 0x68 frame (buf at sp+0x58). */
extern int gl_func_00000000();
extern float gl_data_00000000_12b4f;
void BoardsGadget_display(char *self) {
    float buf[4];
    char pad[0x20];
    char *s2;
    int i;
    buf[0] = 1.0f;
    buf[1] = 1.0f;
    buf[2] = 1.0f;
    buf[3] = 1.0f;
    CALL(gui_setColor_00001A5C)((char*)&D_00000000, (int)(255.0f * gl_data_00000000_12b4f), buf);
    s2 = ((BoardsGadget *)self)->sprite;
    CALL(gui_spriteRefresh)(s2);
    for (i = ((BoardsGadget *)self)->count - 1; i >= 0; i--) {
        CALL(gui_drawSprite)(s2, ((BoardsGadget *)self)->parms.xps.value + i * 4, ((BoardsGadget *)self)->parms.yps.value - i * 4, 3);
    }
}

void BoardsGadget_setCount(int *a0, int a1) {
    ((BoardsGadget *)a0)->count = a1;
}

#ifndef FW
#define FW(p, o) (*(int *)((char *)(p) + (o)))
#endif
/* CoolGadgetParms_ctor: MATCH 2026-07-15 (agent-h) — direct transfer of the
 * TimeGadgetParms_ctor struct-by-value ||-alloc-fallback kit (see 7BC comment /
 * docs/IDO_CODEGEN "struct-by-value arg carrier"): 6-member get-or-create
 * constructor, head alloc 0x98, header node CC38, member seeds CC40..CC54
 * as gl_ref symbol loads (destructive lui/lw base reuse), 4-byte struct t2
 * passed by value to the member ctor (sw a2,8(sp) delay store — the exact
 * residual the old cap note said "the C form can't force"), distinct
 * gl_ref_0000C764_135C_N aliases per expansion, volatile pads for the two
 * phantom slots. Retracts the "genuine cap / scheduler tie" verdict — the
 * prologue tie disappeared with this structure too. */
extern char sStr_Parms_CC38;
extern char gl_ref_0000C764_135C_1;
extern char gl_ref_0000C764_135C_2;
extern char gl_ref_0000C764_135C_3;
extern char gl_ref_0000C764_135C_4;
extern char gl_ref_0000C764_135C_5;
extern char gl_ref_0000C764_135C_6;
extern int sTag_xps_CC40;
extern int sTag_yps_CC44;
extern int sTag_lbx_CC48;
extern int sTag_lby_CC4C;
extern int sTag_l2x_CC50;
extern int sTag_l2y_CC54;
struct SB135C { int v; };
char *CoolGadgetParms_ctor(char *self) {
    struct SB135C u1;
    struct SB135C u2;
    struct SB135C u3;
    struct SB135C u4;
    struct SB135C u5;
    struct SB135C u6;
    char *p2;
    volatile int pad1;
    volatile int pad2;
    char *m2;
    struct SB135C t2;
    if (self != 0 || (self = (char *)CALL(mem_new)(0x98)) != 0) {
        if ((p2 = self) != 0 || (p2 = (char *)CALL(mem_new)(8)) != 0) {
            PARMS(p2)->name = &sStr_Parms_CC38;
            PARMS(p2)->first = 0;
        }
        u1.v = sTag_xps_CC40;
        t2 = u1;
        if ((m2 = self + 8) != 0 || (m2 = (char *)CALL(mem_new)(0x18)) != 0) {
            CALL(Parms_addParm)(m2, self, t2, 1);
            PARM(m2)->type = &gl_ref_0000C764_135C_1; PARM(m2)->value = 0x1E; PARM(m2)->unk14 = 0;
        }
        u2.v = sTag_yps_CC44;
        t2 = u2;
        if ((m2 = self + 0x20) != 0 || (m2 = (char *)CALL(mem_new)(0x18)) != 0) {
            CALL(Parms_addParm)(m2, self, t2, 1);
            PARM(m2)->type = &gl_ref_0000C764_135C_2; PARM(m2)->value = 0x1E; PARM(m2)->unk14 = 0;
        }
        u3.v = sTag_lbx_CC48;
        t2 = u3;
        if ((m2 = self + 0x38) != 0 || (m2 = (char *)CALL(mem_new)(0x18)) != 0) {
            CALL(Parms_addParm)(m2, self, t2, 1);
            PARM(m2)->type = &gl_ref_0000C764_135C_3; PARM(m2)->value = 2; PARM(m2)->unk14 = 0;
        }
        u4.v = sTag_lby_CC4C;
        t2 = u4;
        if ((m2 = self + 0x50) != 0 || (m2 = (char *)CALL(mem_new)(0x18)) != 0) {
            CALL(Parms_addParm)(m2, self, t2, 1);
            PARM(m2)->type = &gl_ref_0000C764_135C_4; PARM(m2)->value = -0xC; PARM(m2)->unk14 = 0;
        }
        u5.v = sTag_l2x_CC50;
        t2 = u5;
        if ((m2 = self + 0x68) != 0 || (m2 = (char *)CALL(mem_new)(0x18)) != 0) {
            CALL(Parms_addParm)(m2, self, t2, 1);
            PARM(m2)->type = &gl_ref_0000C764_135C_5; PARM(m2)->value = 2; PARM(m2)->unk14 = 0;
        }
        u6.v = sTag_l2y_CC54;
        t2 = u6;
        if ((m2 = self + 0x80) != 0 || (m2 = (char *)CALL(mem_new)(0x18)) != 0) {
            CALL(Parms_addParm)(m2, self, t2, 1);
            PARM(m2)->type = &gl_ref_0000C764_135C_6; PARM(m2)->value = -0xC; PARM(m2)->unk14 = 0;
        }
    }
    return self;
}


#ifdef NON_MATCHING
#ifndef FW
#define FW(p, o) (*(int *)((char *)(p) + (o)))
#endif
/* CoolGadget_ctor: EXACT via the NCC donor game_libs_ncc_15FC.cpp (a real
 * G::G(int) constructor, spliced into both build trees, agent-g run 163); the
 * cfe body below is the older comparison form (li 100 stays v0 under cfe).
 * History: NM wrap, 2026-07-15 agent-h wave re-probe: 82 -> ~90
 * (76/103 .o words, 10 of the 27 diffs are %hi/%lo reloc fields). Applied the
 * 7BC-kit: ||-param-reassign head fixed the prologue (move s0/sw ra/sw a1
 * delay ALL exact now — retracts "prologue placement cap"); FW(0x28) stores
 * &gl_data_00000000_15fc (abs-0 alias; was literal 0 = a DECODE bug, sw zero);
 * loaded-store-FIRST statement order per FP group (108=C80; 118=1.0f; then
 * the 0.0f fillers) lets as1 sink the loaded sw below the ready zero-stores —
 * groups B (114/110/10C/108/118) and C (124/120/11C) now byte-exact; INLINE
 * self+0x168 / self+0x180 call args (NOT named p1/p2) CSE across the calls
 * and spill at temp homes 0x2C/0x28 = target (named forms sit 4 higher and
 * +8 frame). RESIDUAL (~9 words): the D8..12C int/fp store stream — target
 * soft-pipelines every lwc1 latency with the NEXT const (li v1,100 / li
 * t9,130 / lui at) while keeping stores strictly source-order; ours hoists
 * the CC/C8 int stores above E4/E0 and sinks li t9. Coupled to the 100-temp
 * v0-vs-v1 tie (target v1, ours v0). 8 statement-order/valuation spellings
 * probed (D0 moves, joins, E4/E0 swaps, register f32, de-named f2 — the last
 * two REGRESS: de-named C7C double-deref does NOT CSE, self-stores may-alias
 * the extern pool). as1 stream-interleave class.
 * 2026-07-15 round 2 (agent-g): +4 ghost-word FIX — the CSE call-arg spill
 * homes sat at 0x2C/0x30 vs target 0x28/0x2C because temp_f2's dead decl
 * home ranked BELOW the compiler spill temps; `volatile int pad_15fc;`
 * declared AFTER temp_f2 claims that word (zero insns, frame unchanged
 * -0x38) and the spills drop to 0x28/0x2C exact (27->23 diffs; `register`
 * on temp_f2 is byte-inert). Remaining = the interleave hunk + li-100
 * v0/v1 tie only; re-probed D4/D0 source swap (t8/t9 role swap, no gain)
 * and D4/D0/12C same-line join (byte-inert). Cap stands.
 * 2026-10-23 agent-g run 141: the interleave hunk is GONE (fuzzy 90.06 ->
 * 99.22 standalone): E0 before E4, D4 = arg1 before the 128 zero, the two
 * 100 stores as ONE chained assignment (CC = C8 = 100, which also gives the
 * target's 204-then-200 store order) and 12C before D0 (found by a full
 * permutation sweep of the tail block under NCC; the same order gives the
 * same words under cfe). Only the li-100 v0-vs-v1 tie is left (3 words);
 * NCC + source literals, void / pointer / float callee returns, a named
 * constant local, struct-member spellings and empty tests are all inert on
 * it. */
extern f32 D_00000C7C, D_00000C80, D_00000C84, D_00000C88, D_00000C8C, D_00000C90, D_00000C94;
extern char D_0000CC58[], D_0000CC60[];
extern int gl_data_00000000_15fc;
char *CoolGadget_ctor(char *self, s32 arg1) {
    f32 temp_f2;
    volatile int pad_15fc;
    if (self != 0 || (self = (char *)func_00000000(0x198)) != 0) {
        func_00000000(self, D_0000CC58);
        COOLG(self)->node.vptr = (char *)&gl_data_00000000_15fc;
        func_00000000(&COOLG(self)->parms);
        temp_f2 = D_00000C7C;
        COOLG(self)->node.name = (const char *)D_0000CC60;
        COOLG(self)->lowColour.r = temp_f2;
        COOLG(self)->midColour.r = temp_f2;
        COOLG(self)->lowColour.g = 0.0f; COOLG(self)->lowColour.b = 0.0f; COOLG(self)->lowColour.a = 0.0f; COOLG(self)->midColour.g = 0.0f; COOLG(self)->midColour.b = 0.0f; COOLG(self)->midColour.a = 0.0f;
        COOLG(self)->pulseLo.r = D_00000C80;
        COOLG(self)->pulseHi.r = 1.0f;
        COOLG(self)->pulseLo.a = 0.0f;
        COOLG(self)->pulseLo.b = 0.0f;
        COOLG(self)->pulseLo.g = 0.0f;
        COOLG(self)->pulseHi.g = D_00000C84;
        COOLG(self)->pulseHi.a = 0.0f;
        COOLG(self)->pulseHi.b = 0.0f;
        COOLG(self)->colour[0] = D_00000C88;
        COOLG(self)->colour[1] = D_00000C8C;
        COOLG(self)->colour[2] = D_00000C90;
        COOLG(self)->colour[3] = 0.0f;
        COOLG(self)->total = arg1;
        COOLG(self)->phase = 0.0f;
        COOLG(self)->target = COOLG(self)->cur = 0x64;
        COOLG(self)->step = D_00000C94;
        COOLG(self)->labelAlpha = 0x82;
        func_00000000(COOLG(self)->barSprite, 0x5000A);
        func_00000000(COOLG(self)->capSprite, 0x50009);
        func_00000000(COOLG(self)->label2Sprite, 0x5000B);
        func_00000000(COOLG(self)->label1Sprite, 0x5001C);
        func_00000000(self, COOLG(self)->label1Sprite, COOLG(self)->label2Sprite);
        func_00000000(self, -0x49, -2, -0x32, -1);
    }
    return self;
}
#else
INCLUDE_ASM("asm/nonmatchings/game_libs/game_libs", CoolGadget_ctor);
#endif

void CoolGadget_release(char *a0) {
    CALL(gui_spriteRelease)(COOLG(a0)->barSprite);
    CALL(gui_spriteRelease)(COOLG(a0)->capSprite);
    CALL(gui_spriteRelease)(COOLG(a0)->label2Sprite);
    CALL(gui_spriteRelease)(COOLG(a0)->label1Sprite);
}

/* 3-insn 2-field setter: parms xps / yps. */
void CoolGadget_setPos(int *a0, int a1, int a2) {
    COOLG(a0)->parms.xps.value = a1;
    COOLG(a0)->parms.yps.value = a2;
}

/* 2-insn 1-field setter: target (in the jr-ra delay slot); display steps cur toward it. */
void CoolGadget_setValue(int *a0, int a1) {
    COOLG(a0)->target = a1;
}

/* 3-insn 2-field setter: label1 / label2 (the ctor passes &label1Sprite / &label2Sprite). */
void CoolGadget_setLabels(int *a0, int a1, int a2) {
    COOLG(a0)->label1 = (char *)a1;
    COOLG(a0)->label2 = (char *)a2;
}

/* 6-insn 4-arg setter: the label offsets lbx / lby / l2x / l2y (a4 from the caller-arg-save
 * sp+0x10); the ctor passes -73, -2, -50, -1. */
void CoolGadget_setLabelPos(int *a0, int a1, int a2, int a3, int a4) {
    COOLG(a0)->parms.lbx.value = a1;
    COOLG(a0)->parms.lby.value = a2;
    COOLG(a0)->parms.l2x.value = a3;
    COOLG(a0)->parms.l2y.value = a4;
}

/* CoolGadget_display: 16-segment meter/gauge tick (0x43C incl. its 2-word
 * hoisted head: lui/mtc1 f0 = 1.0f = the local float v[4] = {1,1,1,1} init
 * scheduled above addiu sp by 7.1 -O2). Orphan-sweep verdict 2026-09-05
 * (docs/MATCHING_WORKFLOW.md #uso-sym-export-oracle-orphan-sweep-arcproc-f48):
 * the old "gl_func_00001820" symbol was this function's body minus its head,
 * zero jal/reloc references to either word -> merged under the orphan's name,
 * 1820.s retired.
 *
 * Structure: cur (0xC8) steps toward tgt (0xCC) by 1; seg (0xC4) =
 * (int)(clamp(total-cur, 0, total) / total * 16); colour w = (seg >= 12)
 * random lerp between lo (0x108) / hi (0x118) driven by three phase-stepped
 * calls of a float import (sinf-like: (r+1)/2 in [0,1]), else the 0xF8 /
 * 0xE8 Vec4 constants; then the 0x130/0x134 sub-objects (if present) and the
 * embedded 0x138 / 0x150 sub-objects get (colour, position) calls with
 * m = seg*4+2 pixel offsets.
 *
 * NM 97.0% (263/271 words, agent-g 2026-09-05). Levers that got here (all in
 * docs/IDO_CODEGEN.md #gauge-tick-fp-kit-1818): float temps x/y/z for the
 * f16/f18 + z-home round trip; `(hi-lo)*(r+1)/2 + lo` (IDO flips the add
 * operands); `t = step + phase; phase = t; F(t)` for the add.s-f12/jal/swc1
 * delay shape; typed float extern for the 255.0f*scale sites (the cast form
 * `*(float*)&D` always lands the constant on the right); `int m[1]` as a
 * memory var (a scalar m is rematerialised from a homed n); scalar-home
 * frame tiling (cur/t/f1..f3/g1 pads).
 * RESIDUAL (8 words): (a) target emits `sw v0,0xC4(s0)` BEFORE the m home
 * store (`sw t0,0x7C(sp)` sits in the bnez delay) with the seg test still
 * forwarded from v0 -- every C order that puts the m store after the field
 * store kills the forwarding (array store = alias barrier), every scalar
 * spelling of m (plain / two-def / struct member / &m / register) is
 * rematerialised from n instead of homed; (b) the 0x148->+0x20 lh chain
 * colours pointer/value v0/v1 vs target v1/v0 (named q/h give v0/v1, inline
 * gives t4/v0), which renumbers the surrounding t4/t5/t6 ring.
 * EXACT 2026-10-20 (agent-g run 108): (b) an empty `if (1) {}` between the q
 * and h defs (BB boundary: q is born in the call's dead-v0 BB -> v1, h in the
 * next BB -> v0; docs #dead-v0-poison-is-per-bb-split-the-defs-683d4), then
 * the a1 sum spelled `m[0] + o->0x44` (m loads first -> t5); (a) the field
 * store FIRST, then the m home store, then test the LOCAL `n >= 12` (the
 * else-if keeps the field re-read). */
extern int gl_func_00000000();
extern float gl_ffunc_00001818(float);
extern float gl_ref_00001818_scale_f;
void CoolGadget_display(char *o) {
    GadgetRGBA w;
    float v[4];
    int m[1];
    int n, rem, tot, cur;
    float a;
    float t;
    float b;
    float c, x, y, f1, f2, f3;
    float z;
    float g1;
    char *p, *q;
    short h;

    /* Field-form notes (agent-h run 509): with members IDO loads a commutative pair in the
     * opposite order from the raw form, so the sums below are written `phase + step`,
     * `pulseLo + ...`, `xps + lbx`; the two `m[0] + xps` sums stay raw (`*(int *)(o + 0x44)`
     * = parms.xps.value): every member spelling swaps their registers. */
    v[0] = 1.0f; v[1] = 1.0f; v[2] = 1.0f; v[3] = 1.0f;
    if (COOLG(o)->target < COOLG(o)->cur) {
        COOLG(o)->cur -= 1;
    } else if (COOLG(o)->cur < COOLG(o)->target) {
        COOLG(o)->cur += 1;
    }
    tot = COOLG(o)->total;
    rem = tot - COOLG(o)->cur;
    if (rem < 0) rem = 0;
    if (tot < rem) rem = tot;
    n = (int)((float)rem / (float)tot * 16.0f);
    COOLG(o)->seg = n;
    m[0] = n * 4 + 2;
    if (n >= 12) {
        t = COOLG(o)->phase + COOLG(o)->step; COOLG(o)->phase = t; a = XCALL(gl_ffunc_00001818, fsin)(t);
        t = COOLG(o)->phase + COOLG(o)->step; COOLG(o)->phase = t; b = XCALL(gl_ffunc_00001818, fsin)(t);
        t = COOLG(o)->phase + COOLG(o)->step; COOLG(o)->phase = t; c = XCALL(gl_ffunc_00001818, fsin)(t);
        x = COOLG(o)->pulseLo.r + (COOLG(o)->pulseHi.r - COOLG(o)->pulseLo.r) * (a + 1) / 2;
        y = COOLG(o)->pulseLo.g + (COOLG(o)->pulseHi.g - COOLG(o)->pulseLo.g) * (b + 1) / 2;
        z = COOLG(o)->pulseLo.b + (COOLG(o)->pulseHi.b - COOLG(o)->pulseLo.b) * (c + 1) / 2;
        w.r = x; w.g = y; w.b = z; w.a = 1.0f;
    } else if (COOLG(o)->seg >= 4) {
        w = COOLG(o)->midColour;
    } else {
        w = COOLG(o)->lowColour;
    }
    if (COOLG(o)->label1 != 0) {
        CALL(gui_spriteRefresh)(COOLG(o)->label1);
        CALL(gui_setColorEnvPrim_000017DC)(COOLG(o)->label1, (int)(gl_ref_00001818_scale_f * (float)COOLG(o)->labelAlpha), v, 0);
        CALL(gui_drawSprite)(COOLG(o)->label1, COOLG(o)->parms.xps.value + COOLG(o)->parms.lbx.value, COOLG(o)->parms.yps.value + COOLG(o)->parms.lby.value, 0);
    }
    if (COOLG(o)->label2 != 0) {
        CALL(gui_spriteRefresh)(COOLG(o)->label2);
        CALL(gui_setColorEnvPrim_000017DC)(COOLG(o)->label2, (int)(255.0f * gl_ref_00001818_scale_f), v, 0);
        CALL(gui_drawSprite)(COOLG(o)->label2, COOLG(o)->parms.xps.value + COOLG(o)->parms.l2x.value, COOLG(o)->parms.yps.value + COOLG(o)->parms.l2y.value, 0);
    }
    if (COOLG(o)->seg > 0) {
        p = COOLG(o)->barSprite;
        CALL(gui_spriteRefresh)(p);
        CALL(gui_setColorEnvPrim_000017DC)(p, (int)(255.0f * gl_ref_00001818_scale_f), &w, 0xFF);
        CALL(gui_drawSpriteIA8Clip_00002BB0)(p, COOLG(o)->parms.xps.value + 4, COOLG(o)->parms.yps.value, 3, m[0] - 4);
    }
    p = COOLG(o)->barSprite;
    CALL(gui_spriteRefresh)(p);
    CALL(gui_setColorEnvPrim_000017DC)(p, (int)(255.0f * gl_ref_00001818_scale_f), COOLG(o)->colour, 0xFF);
    q = (char *)((RaceSprite *)COOLG(o)->barSprite)->image; /* the bar image */
    if (1) {}
    h = ((RaceSpriteImage *)q)->width;
    CALL(gui_drawSpriteIA8Clip_00002BB0)(p, m[0] + *(int *)&COOLG(o)->parms.xps.value, COOLG(o)->parms.yps.value, m[0] - 1, h - m[0] - 1);
    if (COOLG(o)->seg > 0) {
        p = COOLG(o)->capSprite;
        CALL(gui_spriteRefresh)(p);
        CALL(gui_setColorEnvPrim_000017DC)(p, (int)(255.0f * gl_ref_00001818_scale_f), &w, 0xFF);
        CALL(gui_drawSprite)(p, m[0] + *(int *)&COOLG(o)->parms.xps.value - 3, COOLG(o)->parms.yps.value, 0);
    }
}

/* CourseGadgetParms_ctor: EXACT 2026-10-16 (agent-g). The "sibling-scope slot-sharing
 * cap" below was the 4244 lever: ONE function-scope `char *m; struct SB1C54 t;`
 * declared LAST (homes 0x88/0x84) instead of per-block pairs, with an empty
 * `if (1) {}` after each of the 16 ctor calls so neither m nor &t is colored
 * (no s-reg, no &t CSE); plus the unit-15 `u15.v = ...; srcC0[2] = ...;` pair on
 * ONE line for the lui at / lui t3 order. See docs/IDO_CODEGEN.md
 * #if1-bbs-uncolor-shared-name-4244. */
/* CourseGadgetParms_ctor: PASS-5 2026-07-15 (agent-h) 77.1 -> ~85: 7BC/135C
 * struct-by-value ||-alloc-fallback kit, BLOCK-SCOPED {m,t} per expansion.
 * ALL 458/458 instructions now have exact mnemonic+register shape (m in $a0
 * with call-crossing spill, id value a ring temp feeding sw-rot + sw-t-home
 * with the t store in the bne delay, lw a2 join, sw a2,8(sp) struct-arg
 * slot store in the ctor jal delay). RESIDUAL = pure frame-slot placement:
 * target shares ONE m/t home pair (0x88/0x84) across all 16 expansions
 * (frame 0x108); IDO 7.1 cfe gives every sibling-block {m,t} pair its own
 * 8 bytes (frame 0x180) and NEVER overlays sibling scopes. Probed at n=16
 * standalone: function-scope shared t flips the struct copy to memcpy-form
 * (&t CSE'd into $s2) at >=8 copies (scalar t.v= spelling DCEs the u-store
 * and colors $v0; volatile/chain/array/prototyped-direct-pass/comma/ternary
 * spellings all fail one leg). Slot-sharing cap, not a register cap. */
extern int gl_func_00000000();
extern char sStr_Parms_CC6C;
extern int sTag_xps_CC74;
extern int sTag_yps_CC78;
extern int sTag_mx0_CC7C;
extern int sTag_my0_CC80;
extern int sTag_sx0_CC84;
extern int sTag_sy0_CC88;
extern int sTag_mx1_CC8C;
extern int sTag_my1_CC90;
extern int sTag_gx0_CC94;
extern int sTag_gy0_CC98;
extern int sTag_pxp_CC9C;
extern int sTag_pyp_CCA0;
extern int sTag_bxp_CCA4;
extern int sTag_byp_CCA8;
extern int sTag_col_CCAC;
extern int sTag_adz_CCB0;
extern char gl_ref_0000C764_1C54_1;
extern char gl_ref_0000C764_1C54_2;
extern char gl_ref_0000C764_1C54_3;
extern char gl_ref_0000C764_1C54_4;
extern char gl_ref_0000C764_1C54_5;
extern char gl_ref_0000C764_1C54_6;
extern char gl_ref_0000C764_1C54_7;
extern char gl_ref_0000C764_1C54_8;
extern char gl_ref_0000C764_1C54_9;
extern char gl_ref_0000C764_1C54_10;
extern char gl_ref_0000C764_1C54_11;
extern char gl_ref_0000C764_1C54_12;
extern char gl_ref_0000C764_1C54_13;
extern char gl_ref_0000C764_1C54_14;
extern char gl_ref_0000C764_1C54_15;
extern char gl_ref_0000CA34_1C54;
extern float gl_ref_00000C98_f;
struct SB1C54 { int v; };
char *CourseGadgetParms_ctor(char *self) {
    struct SB1C54 u1;
    struct SB1C54 u2;
    struct SB1C54 u3;
    struct SB1C54 u4;
    struct SB1C54 u5;
    struct SB1C54 u6;
    struct SB1C54 u7;
    struct SB1C54 u8;
    struct SB1C54 u9;
    struct SB1C54 u10;
    struct SB1C54 u11;
    struct SB1C54 u12;
    struct SB1C54 u13;
    struct SB1C54 u14;
    f32 srcC0[4];
    f32 srcB0[4];
    f32 srcA0[4];
    struct SB1C54 u15;
    struct SB1C54 u16;
    char *p;
    volatile int pad1;
    volatile int pad2;
    char *m;
    struct SB1C54 t;
    if (self != 0 || (self = (char *)CALL(mem_new)(0x194)) != 0) {
        if ((p = self) != 0 || (p = (char *)CALL(mem_new)(8)) != 0) {
            PARMS(p)->name = &sStr_Parms_CC6C;
            PARMS(p)->first = 0;
        }
        u1.v = sTag_xps_CC74;
        {
            t = u1;
            if ((m = self + 0x8) != 0 || (m = (char *)CALL(mem_new)(0x18)) != 0) {
                CALL(Parms_addParm)(m, self, t, 1); if (1) {}
                PARM(m)->type = &gl_ref_0000C764_1C54_1; PARM(m)->value = 0x1E; PARM(m)->unk14 = 0;
            }
        }
        u2.v = sTag_yps_CC78;
        {
            t = u2;
            if ((m = self + 0x20) != 0 || (m = (char *)CALL(mem_new)(0x18)) != 0) {
                CALL(Parms_addParm)(m, self, t, 1); if (1) {}
                PARM(m)->type = &gl_ref_0000C764_1C54_2; PARM(m)->value = 0x1E; PARM(m)->unk14 = 0;
            }
        }
        u3.v = sTag_mx0_CC7C;
        {
            t = u3;
            if ((m = self + 0x38) != 0 || (m = (char *)CALL(mem_new)(0x18)) != 0) {
                CALL(Parms_addParm)(m, self, t, 1); if (1) {}
                PARM(m)->type = &gl_ref_0000C764_1C54_3; PARM(m)->value = 0xA; PARM(m)->unk14 = 0;
            }
        }
        u4.v = sTag_my0_CC80;
        {
            t = u4;
            if ((m = self + 0x50) != 0 || (m = (char *)CALL(mem_new)(0x18)) != 0) {
                CALL(Parms_addParm)(m, self, t, 1); if (1) {}
                PARM(m)->type = &gl_ref_0000C764_1C54_4; PARM(m)->value = 0x73; PARM(m)->unk14 = 0;
            }
        }
        u5.v = sTag_sx0_CC84;
        {
            t = u5;
            if ((m = self + 0x68) != 0 || (m = (char *)CALL(mem_new)(0x18)) != 0) {
                CALL(Parms_addParm)(m, self, t, 1); if (1) {}
                PARM(m)->type = &gl_ref_0000C764_1C54_5; PARM(m)->unk14 = 0; PARM(m)->value = 0;
            }
        }
        u6.v = sTag_sy0_CC88;
        {
            t = u6;
            if ((m = self + 0x80) != 0 || (m = (char *)CALL(mem_new)(0x18)) != 0) {
                CALL(Parms_addParm)(m, self, t, 1); if (1) {}
                PARM(m)->type = &gl_ref_0000C764_1C54_6; PARM(m)->unk14 = 0; PARM(m)->value = 0;
            }
        }
        u7.v = sTag_mx1_CC8C;
        {
            t = u7;
            if ((m = self + 0x98) != 0 || (m = (char *)CALL(mem_new)(0x18)) != 0) {
                CALL(Parms_addParm)(m, self, t, 1); if (1) {}
                PARM(m)->type = &gl_ref_0000C764_1C54_7; PARM(m)->value = 0xB; PARM(m)->unk14 = 0;
            }
        }
        u8.v = sTag_my1_CC90;
        {
            t = u8;
            if ((m = self + 0xB0) != 0 || (m = (char *)CALL(mem_new)(0x18)) != 0) {
                CALL(Parms_addParm)(m, self, t, 1); if (1) {}
                PARM(m)->type = &gl_ref_0000C764_1C54_8; PARM(m)->value = 0xD; PARM(m)->unk14 = 0;
            }
        }
        u9.v = sTag_gx0_CC94;
        {
            t = u9;
            if ((m = self + 0xC8) != 0 || (m = (char *)CALL(mem_new)(0x18)) != 0) {
                CALL(Parms_addParm)(m, self, t, 1); if (1) {}
                PARM(m)->type = &gl_ref_0000C764_1C54_9; PARM(m)->unk14 = 0; PARM(m)->value = 0;
            }
        }
        u10.v = sTag_gy0_CC98;
        {
            t = u10;
            if ((m = self + 0xE0) != 0 || (m = (char *)CALL(mem_new)(0x18)) != 0) {
                CALL(Parms_addParm)(m, self, t, 1); if (1) {}
                PARM(m)->type = &gl_ref_0000C764_1C54_10; PARM(m)->unk14 = 0; PARM(m)->value = 0;
            }
        }
        u11.v = sTag_pxp_CC9C;
        {
            t = u11;
            if ((m = self + 0xF8) != 0 || (m = (char *)CALL(mem_new)(0x18)) != 0) {
                CALL(Parms_addParm)(m, self, t, 1); if (1) {}
                PARM(m)->type = &gl_ref_0000C764_1C54_11; PARM(m)->value = 0x10C; PARM(m)->unk14 = 0;
            }
        }
        u12.v = sTag_pyp_CCA0;
        {
            t = u12;
            if ((m = self + 0x110) != 0 || (m = (char *)CALL(mem_new)(0x18)) != 0) {
                CALL(Parms_addParm)(m, self, t, 1); if (1) {}
                PARM(m)->type = &gl_ref_0000C764_1C54_12; PARM(m)->value = 0x50; PARM(m)->unk14 = 0;
            }
        }
        u13.v = sTag_bxp_CCA4;
        {
            t = u13;
            if ((m = self + 0x128) != 0 || (m = (char *)CALL(mem_new)(0x18)) != 0) {
                CALL(Parms_addParm)(m, self, t, 1); if (1) {}
                PARM(m)->type = &gl_ref_0000C764_1C54_13; PARM(m)->value = 0xA0; PARM(m)->unk14 = 0;
            }
        }
        u14.v = sTag_byp_CCA8;
        {
            t = u14;
            if ((m = self + 0x140) != 0 || (m = (char *)CALL(mem_new)(0x18)) != 0) {
                CALL(Parms_addParm)(m, self, t, 1); if (1) {}
                PARM(m)->type = &gl_ref_0000C764_1C54_14; PARM(m)->value = 0xDC; PARM(m)->unk14 = 0;
            }
        }
        {
            volatile int padA[11];
            char *mf;
            volatile int pad3;
            Quad4 dst40;
            Quad4 dst30;
            Quad4 dst20;
            srcC0[1] = 1.0f;
            srcC0[3] = 1.0f;
            srcA0[0] = 1.0f; srcA0[1] = 1.0f; srcA0[2] = 1.0f; srcA0[3] = 1.0f;
            srcC0[0] = 0.0f;
            srcB0[0] = 0.0f; srcB0[1] = 0.0f; srcB0[2] = 0.0f; srcB0[3] = 0.0f;
            u15.v = sTag_col_CCAC; srcC0[2] = gl_ref_00000C98_f;
            dst40 = *(Quad4 *)srcC0;
            dst30 = *(Quad4 *)srcB0;
            dst20 = *(Quad4 *)srcA0;
            t = u15;
            if ((mf = self + 0x158) != 0 || (mf = (char *)CALL(mem_new)(0x24)) != 0) {
                CALL(Parms_addParm)(mf, self, t, 1);
                PARMRGBA(mf)->type = &gl_ref_0000CA34_1C54;
                PARMRGBA(mf)->unk20 = 0;
                *(Quad4 *)PARMRGBA(mf)->value = dst40;
            }
        }
        u16.v = sTag_adz_CCB0;
        {
            t = u16;
            if ((m = self + 0x17C) != 0 || (m = (char *)CALL(mem_new)(0x18)) != 0) {
                CALL(Parms_addParm)(m, self, t, 1); if (1) {}
                PARM(m)->type = &gl_ref_0000C764_1C54_15; PARM(m)->unk14 = 0; PARM(m)->value = 0;
            }
        }
    }
    return self;
}

/* CourseGadget_ctor: get-or-create constructor (0x2FC object) + eleven
 * sub-object inits at +0x1F4..+0x2E4 (0x148, 82 words). EXACT in-unit.
 * Levers: arg0 IS self (reassigned by the || alloc fallback; a separate
 * var_s0 local moves `or s0,a0` into the bnez delay slot), and the +0x1C0
 * arg1 store comes BEFORE the four zero-float stores (after them the
 * arg1 reload / lui a1 schedule below the swc1 run). */
char *CourseGadget_ctor(char *arg0, s32 arg1) {
    if ((arg0 != 0) || (arg0 = (char *)CALL(mem_new)(0x2FC), (arg0 != 0))) {
        CALL(Node_ctor)(arg0, GL_DATA_STR(0xCCB4, "<Node>"));
        COURSEG(arg0)->node.vptr = (char *)&CourseGadget_vtbl;
        CALL(CourseGadgetParms_ctor)(arg0 + 0x2C);
        COURSEG(arg0)->node.name = (const char *)(sCourseGadgetName);
        COURSEG(arg0)->arg1 = arg1;
        COURSEG(arg0)->marker[1][0] = 0.0f;
        COURSEG(arg0)->marker[0][0] = 0.0f;
        COURSEG(arg0)->marker[1][1] = 0.0f;
        COURSEG(arg0)->marker[0][1] = 0.0f;
        CALL(CourseGadget_setCourse)(arg0, GAMEFLOW(&gInputMgr)->course);
        CALL(gui_spriteInit)(COURSEG(arg0)->sprites[0], 0x50019);
        CALL(gui_spriteInit)(COURSEG(arg0)->sprites[1], 0x5001A);
        CALL(gui_spriteInit)(COURSEG(arg0)->sprites[2], 0x50010);
        CALL(gui_spriteInit)(COURSEG(arg0)->sprites[3], 0x50011);
        CALL(gui_spriteInit)(COURSEG(arg0)->sprites[4], 0x50012);
        CALL(gui_spriteInit)(COURSEG(arg0)->sprites[5], 0x50013);
        CALL(gui_spriteInit)(COURSEG(arg0)->sprites[6], 0x50014);
        CALL(gui_spriteInit)(COURSEG(arg0)->sprites[7], 0x50015);
        CALL(gui_spriteInit)(COURSEG(arg0)->sprites[8], 0x50016);
        CALL(gui_spriteInit)(COURSEG(arg0)->sprites[9], 0x50017);
        CALL(gui_spriteInit)(COURSEG(arg0)->sprites[10], 0x50018);
    }
    return arg0;
}

extern int gl_func_00000000();
void CourseGadget_release(char *a0) {
    CALL(gui_spriteRelease)(COURSEG(a0)->sprites[0]);
    CALL(gui_spriteRelease)(COURSEG(a0)->sprites[1]);
    CALL(gui_spriteRelease)(COURSEG(a0)->sprites[2]);
    CALL(gui_spriteRelease)(COURSEG(a0)->sprites[3]);
    CALL(gui_spriteRelease)(COURSEG(a0)->sprites[4]);
    CALL(gui_spriteRelease)(COURSEG(a0)->sprites[5]);
    CALL(gui_spriteRelease)(COURSEG(a0)->sprites[6]);
    CALL(gui_spriteRelease)(COURSEG(a0)->sprites[7]);
    CALL(gui_spriteRelease)(COURSEG(a0)->sprites[8]);
    CALL(gui_spriteRelease)(COURSEG(a0)->sprites[9]);
    CALL(gui_spriteRelease)(COURSEG(a0)->sprites[10]);
}

/* 3-insn 2-int setter: the screen position. */
void CourseGadget_setPos(int *a0, int a1, int a2) {
    COURSEG(a0)->x = a1;
    COURSEG(a0)->y = a2;
}

/* 5-insn 2-float setter (mtc1 in body — caller passes int in $a1/$a2). */
void CourseGadget_setFloat2_0000254C(int *a0, float a1, float a2) {
    COURSEG(a0)->pct[0] = a1;
    COURSEG(a0)->pct[1] = a2;
}

/* 8-insn 3-float setter (3rd float via stack round-trip per K&R ABI). */
void CourseGadget_setVec3_00002560(int *a0, float a1, float a2, float a3) {
    COURSEG(a0)->originX = a1;
    COURSEG(a0)->originZ = a2;
    COURSEG(a0)->extent = a3;
}

/* 10-insn 4-float setter (3rd+4th via stack round-trip; 4th from caller sp+0x10). */
void CourseGadget_setVec4_00002580(int *a0, float a1, float a2, float a3, float a4) {
    COURSEG(a0)->marker[0][0] = a1;
    COURSEG(a0)->marker[0][1] = a2;
    COURSEG(a0)->marker[1][0] = a3;
    COURSEG(a0)->marker[1][1] = a4;
}

/* CourseGadget_setCourse: mode -> slot-value setter (0x298, 166 words) -- EXACT
 * via the IDO 7.1 -O2 donor src/game_libs/game_libs_o2_25A8.c
 * (REPLACE_FUNC_BODY splice). switch(arg1) cases 2..7 (table read from
 * bootup.uso RoData +0xC9C: arms in order 2,3,4,5,6,7) each write eight
 * 0x18-stride slot values at +0x74 and store an alloc(24) result at +0x1F0;
 * default writes four values and clears +0x1F0. The jumptable's donor-local
 * .rodata reloc is renamed CourseGadget_setCourse_rodata and pinned to +0xC9C (an
 * in-unit compile cannot bake the %lo). Body below is a placeholder for the
 * splice. */
void CourseGadget_setCourse(char *arg0, s32 arg1) {
    COURSEG(arg0)->course = arg1;
}

/* CourseGadget_drawMarker: EXACT via the IDO 7.1 NCC donor src/game_libs/game_libs_ncc_2840.cpp
 * (agent-g run 277; REPLACE_FUNC_BODY splice over the placeholder below, both build
 * trees). The donor's p is address-taken by a code-free `if (&p == 0) {}`, so it is
 * stored to its own home sp+0x28 like the target instead of a uopt spill temp (the
 * cfe decode that preceded the NCC donor is in git history). */
void CourseGadget_drawMarker(char *arg0, s32 arg1, f32 arg2, f32 arg3, s32 arg4) {
}

/* hud_findMarkers_000029F8: list walk -> per-item marker draw (0x19C, 103 words).
 * scale = parms my1 - my0 (CourseGadget, gdl/gadget.h); a by-value 4-byte key
 * read from D+0xCCCC ("mrk") selects the marker item list; each item's (+0x30,
 * +0x38) world x / z is mapped through originX / originZ and extent into the
 * map box at (xps + mx0, yps + my0) and drawn as a 0xF801F801 box. The two
 * member sums are written `xps + mx0` / `yps + my0`: the other order swaps
 * the two loads.
 * Match levers: the iterator is a two-pointer ARRAY {cur, next} (a struct
 * or pointer-walked struct is promoted to registers; the array stays in the
 * frame at +0xAC/+0xB0), and its advance tests the NEXT slot after the copy
 * (`(cur = next, next != 0) ? (next = next->next, cur->data) : 0`) -- the
 * cur-test spelling picks v1 instead of t3 for the loop-bottom node; the
 * key is a struct whose member is assigned (struct-copy spelling computes
 * the address and copies through it); the per-axis scale goes as
 * `q * (t = scale)` (docs/IDO_CODEGEN.md
 * #feedback-ido-fp-commutative-operand-order-assignment-lever); the x/y draw
 * args are spelled out twice so the CSE'd sum lands in a1 and is copied to
 * a3 (a variable lands in a3 and is copied to a1). padA/padB/padC are
 * unidentified frame locals that place key at +0xD4, it at +0xAC, vec at
 * +0x98 in the 0xD8 frame. */
typedef struct GlNode29F8 {
    char *data;
    struct GlNode29F8 *next;
} GlNode29F8;
typedef struct GlKey29F8 {
    s32 v;
} GlKey29F8;
#define GL29F8_ITER_NEXT(it) \
    (((it)[0] = (it)[1], (it)[1] != 0) ? ((it)[1] = (it)[1]->next, (it)[0]->data) : 0)
void hud_findMarkers_000029F8(char *arg0) {
    GlKey29F8 key;
    char padA[0x20];
    GlNode29F8 *it[2];
    char padB[4];
    f32 vec[4];
    char padC[0x34];
    char *list;
    char *item;
    f32 scale;
    f32 t;
    s32 x;
    s32 y;

    scale = (f32)COURSEG(arg0)->parms.my1.value - (f32)COURSEG(arg0)->parms.my0.value;
    key.v = *(s32 *)(GL_DATA_STR(0xCCCC, "mrk"));
    list = (char *)CALL(ItemMgr_findType_0003EA98)(gl_data_00000000, key);
    if (list != 0) {
        it[1] = (GlNode29F8 *)ITEMTYPE(list)->node.children; /* the type's items */
        while (GL29F8_ITER_NEXT(it) != 0) {
            item = it[0]->data;
            /* the "mrk" type's items are CourseMarkers (game/course.h, CourseMarker_registerTemplates) */
            x = (s32)(((((CourseMarker *)item)->pos[0] - COURSEG(arg0)->originX) / COURSEG(arg0)->extent) * (t = scale));
            y = (s32)(((((CourseMarker *)item)->pos[2] - COURSEG(arg0)->originZ) / COURSEG(arg0)->extent) * (t = scale));
            vec[3] = 0.0f;
            vec[2] = 0.0f;
            vec[1] = 0.0f;
            vec[0] = 0.0f;
            CALL(gui_setFillColor_00001CA8)(&D_00000000, 255, vec);
            CALL(gui_fillRect)(&D_00000000,
                             COURSEG(arg0)->parms.xps.value + COURSEG(arg0)->parms.mx0.value + x,
                             COURSEG(arg0)->parms.yps.value + COURSEG(arg0)->parms.my0.value + y,
                             COURSEG(arg0)->parms.xps.value + COURSEG(arg0)->parms.mx0.value + x,
                             COURSEG(arg0)->parms.yps.value + COURSEG(arg0)->parms.my0.value + y,
                             0xF801F801);
        }
    }
}

/* CourseGadget_display: HUD gauge draw (0x5A4, 361 words) -- EXACT via the IDO
 * 7.1 -O2 donor src/game_libs/game_libs_o2_2B94.c (REPLACE_FUNC_BODY splice,
 * 6F684 / 68990 recipe). The donor's four .rodata colour literals are
 * renamed CourseGadget_display_rodata and pinned to bootup.uso RoData +0xCB8
 * (an in-unit compile cannot bake those %lo words). Decode history and the
 * load-bearing shapes are in the donor header. Body below is a placeholder
 * for the splice. */
void CourseGadget_display(char *self) {
    volatile int ret = 0;
    if (self[0x1F0] != 0) {
        ret = 1;
    }
    (void)ret;
}

/* PointsGadget_ctor_00003138: get-or-create ctor (0x168 object) + 8-slot fill + colour
 * normalize (0x160, 88 words). EXACT in-unit: arg0 IS self (reassigned by the
 * || alloc fallback, no separate self local) -- the old var_s3 = arg0 copy
 * scheduled `or s3,a0` into the bnez delay slot instead of before `sw ra`
 * (96.9 NM). Same lever as TimeGadget_ctor / 0FD0 / 0A8C. */
#ifndef FW
#define FW(p, o) (*(int *)((char *)(p) + (o)))
#endif
extern int gl_func_00000000();
char *PointsGadget_ctor_00003138(char *arg0, s32 arg1, s32 arg2, s32 arg3) {
    f32 d255 = 255.0f;
    f32 temp_f0;
    f32 temp_f12;
    f32 temp_f2;
    s32 var_s0;
    char *var_s1;

    if ((arg0 != 0) || (arg0 = (char *)CALL(mem_new)(0x168), (arg0 != 0))) {
        CALL(Node_ctor)(arg0, GL_DATA_STR(0xCCD0, "<Node>"));
        TPOINTS(arg0)->node.vptr = (char *)&PointsGadget_vtbl_CA4C;
        TPOINTS(arg0)->node.name = (const char *)(sPointsGadgetName_CCD8);
        TPOINTS(arg0)->font = arg3;
        TPOINTS(arg0)->popupArg = arg1;
        TPOINTS(arg0)->popupArg2 = arg2;
        CALL(gui_spriteInit)(TPOINTS(arg0)->sprite0, 0x50003);
        CALL(gui_spriteInit)(TPOINTS(arg0)->sprite1, 0x50004);
        CALL(gui_spriteInit)(TPOINTS(arg0)->sprite2, 0x50006);
        CALL(gui_spriteInit)(TPOINTS(arg0)->sprite4, 0x50005);
        CALL(gui_spriteInit)(TPOINTS(arg0)->sprite3, 0x50021);
        var_s0 = 0;
        var_s1 = arg0;
        do {
            TPOINTS(var_s1)->slots[0] = (PointsPopup *)CALL(PointsGadget_ctor_00000ED0)(0, arg1);
            var_s1 += 4;
            var_s0 += 1;
        } while (var_s0 != 8);
        CALL(TrickPointsGadget_reset_00003298)(arg0);
        temp_f0 = (f32)255 / d255;
        temp_f2 = 102.0f / d255;
        TPOINTS(arg0)->colour.r = temp_f0;
        TPOINTS(arg0)->colour.a = temp_f0;
        TPOINTS(arg0)->pointsColour.g = temp_f0;
        TPOINTS(arg0)->pointsColour.a = temp_f0;
        temp_f12 = 80.0f / d255;
        TPOINTS(arg0)->colour.g = temp_f2;
        TPOINTS(arg0)->pointsColour.r = temp_f2;
        TPOINTS(arg0)->colour.b = temp_f12;
        TPOINTS(arg0)->pointsColour.b = temp_f12;
    }
    return arg0;
}

/* USO Sym[943] exports Text+0x17904 (synthetic 0x3298); 0x32B0 is
 * an internal prologue split, not a separate function. Shared float divisor
 * preserves the four runtime divisions. Both flag stores address the same
 * child: the asm walker advances before its final lw 0x28(s0).
 * NM 99.69879%: 83 instructions; final 0xFF uses v0 instead of target v1
 * (five raw-word differences). Typed float call avoids K&R promotion;
 * Color3298 preserves the +0x64 base. Pointer scopes/register hints, int/void
 * callee, signed/unsigned or != loop, tail indexing/chained stores, if(1)
 * blocks and a 2,971-iteration permuter pass did not close those five words.
 * EXACT 2026-10-23 (agent-g run 134): the 0xFF constant took v0 because the
 * walker's color pointer (v0 + 0x64 after the last child) was dead after the
 * loop. Declaring color at function scope and ending with an empty
 * `if (color) {}` (PR #10's trailing empty conditional on a live pointer)
 * keeps v0 busy, so 0xFF goes to v1 as in the target. */
extern void gl_reset_3298(void *, int, int, float, float);
typedef struct { float r, g, b, a; } Color3298;
void TrickPointsGadget_reset_00003298(int *a0) {
    Color3298 *color;
    float div255 = 255.0f;
    float r = 100.0f / div255;
    float g = 235.0f / div255;
    float b = 250.0f / div255;
    float al = 0.0f / div255;
    int i;
    for (i = 0; i < 8; i++) {
        TPOINTS(a0)->slots[i]->value = 0;
        gl_reset_3298(TPOINTS(a0)->slots[i], 0xA0, 0x5A, 1.0f, 1.0f);
        color = (Color3298*)TPOINTS(a0)->slots[i]->colour;
        color->a = al;
        color->b = b;
        color->g = g;
        color->r = r;
        TPOINTS(a0)->slots[i]->state = 0;
    }
    TPOINTS(a0)->text[0] = 0;
    TPOINTS(a0)->alpha[0] = 0;
    TPOINTS(a0)->age[0] = 0xFF;
    TPOINTS(a0)->age[1] = 0xFF;
    TPOINTS(a0)->age[2] = 0xFF;
    TPOINTS(a0)->age[3] = 0xFF;
    TPOINTS(a0)->text[1] = 0;
    TPOINTS(a0)->alpha[1] = 0;
    TPOINTS(a0)->text[2] = 0;
    TPOINTS(a0)->alpha[2] = 0;
    TPOINTS(a0)->text[3] = 0;
    TPOINTS(a0)->alpha[3] = 0;
    TPOINTS(a0)->count = 0;
    TPOINTS(a0)->head = 0;
    TPOINTS(a0)->tail = 0;
    TPOINTS(a0)->scroll = 0;
    if (color) {}
}

extern int gl_func_00000000();

void PointsGadget_release_000033E4(char *a0) {
    CALL(gui_spriteRelease)(a0 + 0xF0);
    CALL(gui_spriteRelease)(a0 + 0x108);
    CALL(gui_spriteRelease)(a0 + 0x120);
    CALL(gui_spriteRelease)(a0 + 0x150);
    CALL(gui_spriteRelease)(a0 + 0x138);
}

/* PointsGadget_update_00003430: 8-slot state-machine updater (0x4C4, 305 words) -- EXACT
 * via the IDO 7.1 -O2 donor src/game_libs/game_libs_o2_3430.c
 * (REPLACE_FUNC_BODY splice, agent-c run 218). The "step" is the literal
 * 0.1f, not a variable: uopt keeps it in caller-saved f2 and rematerialises
 * it after the calls of cases 6/7/8 (one fresh .rodata literal per load =
 * the four 0.1f words at 0xCC8/0xCEC/0xCF0/0xCF4 around the jumptable at
 * 0xCCC); three trailing `if (1) {}` make a callee-saved f24 too expensive.
 * The donor's .rodata reloc is renamed PointsGadget_update_00003430_rodata and pinned
 * to bootup.uso RoData +0xCC8. Body below is a placeholder for the splice.
 * Decode history (field spellings carried into the donor):
 * PASS-2 2026-06-22 (big-swing): full float-typed reconstruction of the 8-way
 * state-machine jumptable; FF=f32 fields, FW=int fields per mnemonics.
 * Distinct externs per pool/global ref defeat the &D_00000000 base-pointer
 * CSE hoist (target reloads `lui at` each use).
 * PASS-3 2026-10-15 (agent-g): re-decode 77.93 -> 95.79, same size (305).
 *  - jumptable read from the ROM (RoData +0xCCC): case 1 -> 0x204,
 *    2 -> 0x2B0, 3 -> 0x80, 4 -> 0x190, 5..8 -> 0x2F8/0x318/0x33C/0x3A4.
 *    The old wrap had arms 1<->3 and 2<->4 swapped; source order is
 *    3, 4, 1, 2, 5, 6, 7, 8.
 *  - case 5 stores the decremented 0x9C unconditionally (it sits in the
 *    bgez delay slot), the old body stored it only inside the if.
 *  - hand-written `i += 4; v0 += 4` cursors -> plain indexed loops
 *    (`for (i = 0; i != 8; i++)` gives the target's li 32 / bne; loop 2
 *    uses its own index so it stays in a0/v0 instead of s-regs).
 *  - case 7's counter has its own variable: sharing `t` with the
 *    post-call `0x74 + 16` puts the whole web in s0 (live in a call block).
 *  - 0xCCE8 is `(char *)&D_00000000 + 0xCCE8` (addend baked in lui 1 /
 *    addiu -0x3318), not a separate symbol.
 *  - the step words at .rodata 0xCC8/0xCEC/0xCF0/0xCF4 are all 0.1f
 *    literals, but four identical `step = 0.1f` stores const-propagate into
 *    one hoisted f24; separate externs keep the target's f2 reloads.
 *  - float `x = x + y` written `y + x` and the 0x44 update before the 0x5C
 *    one in case 3 match IDO's operand and store order.
 * PASS-4 2026-10-15 (agent-g run 60): 95.90 -> see report. Every
 *  load/adjust/store counter (0x78 ramps, 0x9C/0x98/0x74 decrements, the
 *  0x8C/0x9C/0xBC table loop, 0x88) is written ON THE FIELD with no local
 *  (`if (FW(s0,0x78) < 0xFF) { FW(s0,0x78) += 0x20; if (FW(s0,0x78) >=
 *  0x100) ... }`): uopt forwards the stored values, so the loaded value
 *  keeps v0/v1 and the adjusted one is a ugen t-temp, exactly the target's
 *  `lw v0; addiu t1,v0,32; sw t1` (the `t`/`n` locals coloured them and
 *  shifted every later t-reg). Case 7 passes FW(s0,0x98) itself as the
 *  call arg (= the target's a2 forwarding); `*p = FW(s0,0x98) + *p` gives
 *  the target load order for the 0x138 -> 0x44 -> 0x18 counter.
 * Residual (14 words + 5 reloc words): prologue order only - the target
 * materializes the hoisted 1.0f/10.625f constants inside the save sequence
 * and loads step LAST (after the EC store); every placement of `step =`
 * / the EC store / named constant locals keeps the constants right after
 * the step load. Plus the jumptable %lo (step loads 0xCC8/0xCEC/0xCF0/0xCF4,
 * all 0.1f, are base-0 externs + the offset -- literals would merge; run 184
 * decode-scan: the bare externs read address 0, `--raw --pins` 15 -> 12). */
void PointsGadget_update_00003430(char *arg0) {
    TPOINTS(arg0)->allIdle = 1;
}

/* PointsGadget_display_000038F4 — EXACT MATCH 138/138 words (2026-07-03, agent-e w5).
 * Was 118/138. Fix stack, in order:
 * 1. RELOC EXTERNS: the two format strings are baked USO data addrs. Target
 *    words lui a1,0x1 / addiu a1,CCEC compose to 0x0000CCEC (NOT 0x1CCEC —
 *    prior extern names D_0001CCEC/F8 mis-derived the value by ignoring lo16
 *    sign). extern char sFmt_pts_CCEC / sFmt_pts_CCF8 + undefined_syms_auto.txt
 *    entries `sFmt_pts_CCEC = 0x0000CCEC; sFmt_pts_CCF8 = 0x0000CCF8;` make ld bake
 *    the exact 4 words (verified by link simulation of HI16/LO16).
 * 2. NAMED idx LOCAL: `idx = (FW(arg0,0x80)+var_s4)&3; s0v = &((int*)arg0)[idx]`
 *    instead of one nested expression — named int colors to v0 and reuses it
 *    (addu v0/andi v0), snapping the ENTIRE downstream t-reg cascade
 *    (13 words: t4/t6/t9 -> t2/t4/t6, t0 -> t9, slt operand).
 * 3. NAMED-SLOT BUDGET (frame): every named scalar reserves a decl-order home
 *    even when colored (dead space). Target = 5 dead words ABOVE spE4 +
 *    9-word temp area BELOW sp64; we had 11 named scalars below. Fixed by
 *    (a) moving 5 scalars (temp_s2,temp_s3,temp_s7,var_s4,var_s2) ABOVE the
 *    buffer decls, (b) merging the two same-register disjoint pairs into one
 *    name each: idx+temp_v0 -> tv0 (both v0), var_s0+temp_s0 -> s0v (both s0)
 *    — one name = ONE named slot; disjoint ranges keep their per-range color.
 *    Frame 0x170->0x168, buffers 0x74/0xF4 -> 0x64/0xE4, all byte-exact.
 * 4. ARRAY-IXA for the final addu operand order: `(char*)&((int*)arg0)[tv0]`
 *    emits addu s0,s1,t1 (base-first); `arg0 + tv0*4` emits addu s0,t1,s1
 *    and IDO canonicalizes the source-level operand swap back.
 * REQUIRES (report to lander): undefined_syms_auto.txt additions
 *   sFmt_pts_CCEC = 0x0000CCEC;
 *   sFmt_pts_CCF8 = 0x0000CCF8;
 */
#ifndef FW
#define FW(p, o) (*(int *)((char *)(p) + (o)))
#endif
typedef int (*GP_000038F4)();
#ifndef FF
#define FF(p, o) (*(f32 *)((char *)(p) + (o)))
#endif
extern char sFmt_pts_CCEC;  /* baked USO data addr 0xCCEC (lui 1/addiu CCEC); defined in undefined_syms_auto.txt */
extern char sFmt_pts_CCF8;  /* baked USO data addr 0xCCF8 */
void PointsGadget_display_000038F4(char *arg0) {
    int temp_s2;
    int temp_s3;
    char *temp_s7;
    s32 var_s4;
    char *var_s2;
    char spE4[112];
    char sp64[128];
    int *temp_a3;
    char *temp_v1;
    int tv0;
    char *s0v;

    s0v = 0;
    var_s2 = arg0;
    do {
        temp_v1 = (char *)TPOINTS(var_s2)->slots[0];
        if (POPUP(temp_v1)->state != 0) {
            tv0 = (int)NODE(temp_v1)->vptr;
            ((GP_000038F4)VT_SLOT(tv0, 2).fn)(VT_SLOT(tv0, 2).delta + temp_v1);
        }
        s0v += 4;
        var_s2 += 4;
    } while (s0v != (char *)0x20);
    FCALL(gui_setupRenderMode_0000055C)(TPOINTS(arg0)->font);
    var_s4 = 0;
    temp_s7 = (char *)&TPOINTS(arg0)->colour;
    if (TPOINTS(arg0)->count > 0) {
        do {
            tv0 = (TPOINTS(arg0)->tail + var_s4) & 3;
            s0v = (char *)&((int *)arg0)[tv0]; /* the list shifted by 4 * tv0: TPOINTS(s0v)->text[0] is text[tv0] (the named idx base is a lever) */
            temp_a3 = (int *)TPOINTS(s0v)->points[0];
            if (temp_a3 != (int *)-1) {
                FCALL(gl_sprintf)(spE4, &sFmt_pts_CCEC, TPOINTS(s0v)->text[0], temp_a3);
                FCALL(gl_sprintf)(sp64, &sFmt_pts_CCF8, TPOINTS(s0v)->points[0]);
                temp_s2 = FCALL(gui_centerStringX)(TPOINTS(arg0)->font, TPOINTS(s0v)->x[0], spE4);
                temp_s3 = FCALL(gui_stringWidth)(TPOINTS(arg0)->font, TPOINTS(s0v)->text[0]) + temp_s2;
                FCALL(gui_setPrimColor_000006B8)(TPOINTS(arg0)->font, (s32) (*(f32 *)&D_00000000 * (f32) TPOINTS(s0v)->alpha[0]), temp_s7, 0xFF);
                FCALL(gui_drawString)(TPOINTS(arg0)->font, temp_s2, TPOINTS(s0v)->y[0], TPOINTS(s0v)->text[0]);
                FCALL(gui_setPrimColor_000006B8)(TPOINTS(arg0)->font, (s32) (*(f32 *)&D_00000000 * (f32) TPOINTS(s0v)->alpha[0]), &TPOINTS(arg0)->pointsColour, 0xFF);
                FCALL(gui_drawString)(TPOINTS(arg0)->font, temp_s3, TPOINTS(s0v)->y[0], sp64);
            } else {
                FCALL(gui_setPrimColor_000006B8)(TPOINTS(arg0)->font, (s32) (*(f32 *)&D_00000000 * (f32) TPOINTS(s0v)->alpha[0]), temp_s7, 0xFF);
                FCALL(gui_drawString)(TPOINTS(arg0)->font, FCALL(gui_centerStringX)(TPOINTS(arg0)->font, TPOINTS(s0v)->x[0], TPOINTS(s0v)->text[0]), TPOINTS(s0v)->y[0], TPOINTS(s0v)->text[0]);
            }
            var_s4 += 1;
        } while (var_s4 < TPOINTS(arg0)->count);
    }
}

/* PointsGadget_allocSlot_00003B1C - free-slot allocator + initializer (0xF0, 60 insns),
 * EXACT 60/60 (agent-g run 61, re-decoded after the 3C0C donor match).
 * a0 = owner with eight slot pointers at +0x2C; claims the first slot whose
 * 0x94 state is 0 (state 5, 0x9C = 32 or 0 by a3), runs the (int,int,int,
 * float,float) primary init, fills 78/A0/98/B0/80/84/A8 and runs the second
 * init with the baked absolute 0xCD00 (`(char *)&D_00000000 + 0xCD00` = the
 * `lui 1; addiu -13056` carry form, not an int constant). Load-bearing:
 *  - `for (i = 0; i != 8; i++)` indexing a typed slots[8] array: the 32
 *    loop bound is then a compiler IV constant (a1) and does NOT CSE with
 *    the stored `0x9C = 32` (a do/while on a byte offset shares one web);
 *  - test `arg0->slots[i]->unk94` inline and assign `slot = arg0->slots[i]`
 *    INSIDE the if: the scan load stays in a0 and the found slot is the
 *    `or s0,a0` copy (a slot local assigned before the test colours s0 for
 *    the whole loop);
 *  - store order 78, A0, 98, B0, 80, 84, A8 (the only order of the seven
 *    that schedules like the target). */
/* The owner is the TrickPointsGadget (gdl/gadget.h, agent-h run 519; was a private GlOwn3B1C view), the
 * slots its PointsPopup objects (agent-h run 501). */

extern void *gl_func_00000000_i3ff(int, int, int, float, float);
PointsPopup *PointsGadget_allocSlot_00003B1C(TrickPointsGadget *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 i;
    PointsPopup *slot;

    for (i = 0; i != 8; i++) {
        if (arg0->slots[i]->state == 0) {
            slot = arg0->slots[i];
            if (arg3 != 0) {
                slot->state = 5;
                slot->timer = 32;
            } else {
                slot->state = 5;
                slot->timer = 0;
            }
            XCALL(gl_func_00000000_i3ff, PointsPopup_setPosScale)((int)slot, 161, arg2 + 90, 1.0f, 1.0f);
            slot->alpha = 255;
            slot->font = 0;
            slot->value = arg1;
            slot->arg = arg0->popupArg;
            slot->unk80 = 0;
            slot->unk84 = -18;
            slot->velY = 0.0f;
            CALL(gl_sprintf)(slot->text, GL_DATA(0xCD00), arg1);
            return slot;
        }
    }
    return 0;
}

/* PointsGadget_allocSlot_00003C0C: free-slot allocator (0x1AC, 107 words; 3B1C/3DB8
 * family) -- EXACT via the IDO 7.1 -O2 donor src/game_libs/game_libs_o2_3C0C.c
 * (REPLACE_FUNC_BODY splice). Its 0.1f literal's donor-local .rodata reloc is
 * renamed PointsGadget_allocSlot_00003C0C_rodata and pinned to bootup.uso RoData +0xCF8 (an
 * in-unit compile cannot bake the %lo). Body below is a placeholder for the
 * splice. */
char *PointsGadget_allocSlot_00003C0C(char *arg0) {
    volatile int ret = 0;
    if (arg0[0x94] != 0) {
        ret = 1;
    }
    (void)ret;
    return 0;
}

/* PointsGadget_allocSlot_00003DB8 - free-slot allocator variant (0x150, 84 insns; 3B1C/3C0C
 * family), EXACT in-unit (agent-g run 62). a0 = owner with eight slot
 * pointers at +0x2C; claims the first slot whose 0x94 state is 0 (state 1;
 * 9C/78 = 16/0 or 0/255 by a3), runs the (int,int,int,float,float) primary
 * init, clears 98/A4/A8/AC/80 (80 -= 6 when arg1 >= 10), sets A0 = owner+0xF0,
 * 84 = 8, B0 = owner+0x58 and runs the second init with the baked absolute
 * 0xCD08 (`(char *)&D_00000000 + 0xCD08` carry form). Load-bearing:
 *  - `for (i = 0; i != 8; i++)` over a typed slots[8] member: the 32 bound is
 *    an IV constant materialized first (li a1,32 before or v0,zero);
 *  - NO slot local -- every field write goes through arg0->slots[i] so IDO
 *    reloads lw x,0x2C(v1) per store (the target's reload-per-access shape);
 *  - the decrement through `s32 *p = &...->unk80; *p = *p - 6;` gives the
 *    target's addiu v0,v0,0x80 / sw 0(v0) base form. */
/* Owner = TrickPointsGadget, slots = PointsPopup (gdl/gadget.h; were private GlOwn3DB8 / GlSlot3DB8
 * views, agent-h run 519). */

PointsPopup *PointsGadget_allocSlot_00003DB8(TrickPointsGadget *arg0, s32 arg1, int arg2, s32 arg3) {
    s32 i;
    s32 *p;

    for (i = 0; i != 8; i++) {
        if (arg0->slots[i]->state == 0) {
            if (arg3 != 0) {
                arg0->slots[i]->state = 1;
                arg0->slots[i]->timer = 16;
                arg0->slots[i]->alpha = 0;
            } else {
                arg0->slots[i]->state = 1;
                arg0->slots[i]->timer = 0;
                arg0->slots[i]->alpha = 255;
            }
            XCALL(gl_func_00000000_i3ff, PointsPopup_setPosScale)((int)arg0->slots[i], 220, 120, 1.0f, 1.0f);
            arg0->slots[i]->value = 0;
            arg0->slots[i]->font = arg0->sprite0;
            arg0->slots[i]->velX = 0.0f;
            arg0->slots[i]->velY = 0.0f;
            arg0->slots[i]->accelY = 0.0f;
            arg0->slots[i]->unk80 = 0;
            if (arg1 >= 10) {
                p = &arg0->slots[i]->unk80;
                *p = *p - 6;
            }
            arg0->slots[i]->unk84 = 8;
            arg0->slots[i]->arg = arg0->popupArg2;
            CALL(gl_sprintf)(arg0->slots[i]->text, GL_DATA(0xCD08), arg1);
            return arg0->slots[i];
        }
    }
    return 0;
}

void TrickPointsGadget_push(int *a0, int a1, int a2) {
    int diff;
    TPOINTS(a0)->text[TPOINTS(a0)->head] = a1;
    TPOINTS(a0)->points[TPOINTS(a0)->head] = a2;
    TPOINTS(a0)->alpha[TPOINTS(a0)->head] = 0;
    TPOINTS(a0)->age[TPOINTS(a0)->head] = 0;
    TPOINTS(a0)->y[TPOINTS(a0)->head] = 200;
    diff = TPOINTS(a0)->y[TPOINTS(a0)->head] - TPOINTS(a0)->y[(TPOINTS(a0)->head - 1) & 3];
    if (diff < 15) {
        TPOINTS(a0)->y[TPOINTS(a0)->head] = TPOINTS(a0)->y[(TPOINTS(a0)->head - 1) & 3] + 16;
    }
    TPOINTS(a0)->x[TPOINTS(a0)->head] = 160;
    TPOINTS(a0)->scroll += 16;
    TPOINTS(a0)->head = (TPOINTS(a0)->head + 1) & 3;
    if (TPOINTS(a0)->head == TPOINTS(a0)->tail) { /* written head first: tail == head swaps the bne operands */
        TPOINTS(a0)->tail = (TPOINTS(a0)->tail + 1) & 3;
    }
    if (TPOINTS(a0)->count < 4) {
        TPOINTS(a0)->count = TPOINTS(a0)->count + 1;
    }
}

/* TrickPointsGadget_clampRows: clamp four ring-slot fields (+0x8C..+0x98) to a floor of 112.
 * VOID return is load-bearing: as int*-returning, the return-value LR claims $v0 (3-LR
 * coloring p=v1/R=v0 + trailing move v0,v1 — unflippable, every barrier/decl lever floored);
 * as void, the copy candidate p itself colors $v0 first-appearance and the jr delay is a nop.
 * The a0=0 kill after block 1 keeps p as a real second web (block 1 via a0, blocks 2-4 via p). */
void TrickPointsGadget_clampRows(int *a0) {
    int *p;
    p = a0;
    if (a0[0x8C / 4] < 112) a0[0x8C / 4] = 112;
    a0 = 0;
    if (p[0x90 / 4] < 112) p[0x90 / 4] = 112;
    if (p[0x94 / 4] < 112) p[0x94 / 4] = 112;
    if (p[0x98 / 4] < 112) p[0x98 / 4] = 112;
}

extern int gl_ref_00013C70();
int Parm_read_0000405C(char *a0) {
    int scratch;
    gl_ref_00013C70(&scratch);
    return gl_ref_00013C70(a0 + 0x10);
}

extern int gl_ref_00013C70();
extern int gl_ref_00013CAC();
int Parm_read_0000408C(char *a0) {
    int scratch;
    gl_ref_00013C70(&scratch);
    return gl_ref_00013CAC(a0 + 0x10);
}

extern int gl_ref_00013C70();
extern int gl_ref_00013CE8();
void Parm_read_000040BC(char *a0) {
    int scratch;
    gl_ref_00013C70(&scratch);
    gl_ref_00013CE8(a0 + 0x10);
}

/* Body is 4 insns (addiu t6, a0, 4; li t7, 1; jr ra; sllv v0, t7, t6),
 * but symbol coverage includes 2 leading nops at 0x40EC/0x40F0 (artifact
 * of split-fragments.py including inter-function alignment pad in the
 * split-off symbol). Previously matched via PREFIX_BYTES (8-byte leading-
 * nop injection). PREFIX_BYTES REMOVED 2026-05-27 (was missed by
 * 2026-05-23 sweep) per feedback_no_instruction_forcing_matches_policy
 * — leading-nop injection on a non-USO-header function is instruction-
 * appending match-faking. Function stays NM-wrapped (INCLUDE_ASM build
 * path covers the 2 leading nops + body bytes); honest unmatched. */
/* game_libs_func_000040EC BOUNDARY-CORRECTED 2026-06-10 (the 3ECDC
 * class): the 0x18 symbol was TWO misattributed inter-fn pad nops + a
 * true 4-insn fn. Split into the _pad sidecar and
 * app_flowModeBit = `return 1 << (a0 + 4);` -- BYTE-EXACT
 * standalone (4/4, 0 diffs) but mid-file land-blocked (catch-all unit);
 * relayout queue (10 ready bodies). */
#pragma GLOBAL_ASM("asm/nonmatchings/game_libs/game_libs/game_libs_func_000040EC_pad.s")
int app_flowModeBit(int a0) {
    return 1 << (a0 + 4);
}

extern int gl_func_00000000();
void fs_gdlReadInt_00004104(int *dst) {
    int pad, scratch;
    CALL(fs_gdlReadFile)(&gSysLib, &scratch, 4);
#ifdef PORT
    PORT_BE_SWAP32S(&scratch, 4);
#endif
    *dst = scratch;
}

extern int gl_func_00000000();
void fs_gdlReadQuad_00004140(Quad4 *dst) {
    Quad4 scratch;
    CALL(fs_gdlReadFile)(&gSysLib, &scratch, 16);
#ifdef PORT
    PORT_BE_SWAP32S(&scratch, 16);
#endif
    *dst = scratch;
}

extern int gl_func_00000000();
void fs_gdlReadFloat_00004198(float *dst) {
    int pad;
    float scratch;
    CALL(fs_gdlReadFile)(&gSysLib, &scratch, 4);
#ifdef PORT
    PORT_BE_SWAP32S(&scratch, 4);
#endif
    *dst = scratch;
}

typedef struct { int a, b, c; } Tri3i;
typedef struct { float x, y, z; } Vec3;
void fs_gdlReadVec3_000041D4(Vec3 *dst) {
    int pad_top[1];
    Tri3i raw;
    int pad_mid[2];
    Tri3i tmp;
    int pad_bot[2];
    CALL(fs_gdlReadFile)(&gSysLib, &raw, 12);
#ifdef PORT
    PORT_BE_SWAP32S(&raw, 12);
#endif
    tmp = raw;
    dst->x = *(float*)&tmp.a;
    dst->y = *(float*)&tmp.b;
    dst->z = *(float*)&tmp.c;
}

// ColourParms_ctor -- EXACT 2026-10-16 (agent-g); see the note above the function. Historical notes follow.
/* ColourParms_ctor — BEST VERIFIED STATE 2363/2479 words (95.3% positional,
 * byte-level vs own .s, 2026-07-03 agent-e w5). NOT exact; deliverable is this
 * best state + the corrected residual map below. Source below is byte-identical
 * to the in-tree NM wrap (no change improved it this session).
 *
 * RESIDUAL MAP — CORRECTION TO THE DOCUMENTED 66-WORD COUNT: the true
 * positional word residual vs the fn's own .s is 116, not 66. agent-y's
 * "reloc-filtered word LCS" EXCLUDED 50 real mismatch words because they carry
 * HI16 relocs — but they are ORDER swaps of adjacent lui pairs, which mismatch
 * positionally in the linked ROM. Full 116 = 4 classes:
 *
 * 1. [58w, 29 sw/lw pairs] CLASS C spill-home sharing: target spills each
 *    unit's alloc-ptr to ONE gettemp slot 0x38(sp) (+unit1 at its named 0x8C);
 *    any C spelling gives N names -> N slots / 1 name -> s3 promotion /
 *    volatile-or-addressed -> extra loads-stores. RE-VERIFIED this session:
 *    (a) per-unit BLOCK-SCOPED `char *va` (NEW axis, not in agent-y's sweep):
 *        NO overlay — each scoped candidate got its own gettemp slot
 *        0x3C..0xB8 ascending, frame grew 0x748->0x7C8. Rule 1 holds for
 *        scoped names too.
 *    (b) single fn-scope name: reconfirmed s3 promotion (2449 insns, -30).
 *    (c) `if(0){func(&va);}` address-take: shares ONE slot AND it is exactly
 *        0x38 with va declared last (slot placement IS reachable) but
 *        addressed-residency adds ~96 insns (2575) — dead stores at the
 *        pre-if def + post-call reloads. CONFIRMED CAP.
 * 2. [50w, 25 lui pairs] LUI-ORDER — RESOLVED 2026-07-15 agent-h via the
 *    SAME-LINE JOIN lever (as1 debug-line tie-break, the 3D68C crack):
 *    joining the unit's `uN_f.v = *(s32*)cfxx;` and `uN_a.z = *(f32*)0dxx;`
 *    statements onto ONE source line lets as1 cross-schedule the two lui's
 *    (line-number tie no longer forces statement order). Join ORDER is
 *    shape-dependent: units whose f.v line is PRECEDED by another float
 *    D-load (u5,7,9,11,13,15,16,17,20,21,22,23,25,26,27,28,29,31: type A)
 *    join in original order `f.v; a.z`; units without one (u6,8,10,12,14,
 *    18,19: type B) join REVERSED `a.z; f.v`. All 25 pairs now byte-exact.
 *    Word residual 116 -> 66; fn now AT its documented ceiling 2413/2479
 *    (97.3%) + the 58w class-C + 8w no-handle below.
 * 3. [6w] prologue as1 schedule (move s0,a0 hoisted above saves, sdc1 $f20
 *    in bnez delay) — no-handle (documented).
 * 4. [2w] li at,-8 vs addiu s2,sp,84 hoist order — no-handle (documented).
 *
 * The A-win lever of this session (decl-order named-home moves, cracked
 * 7FF4/38F4/85B0) does NOT apply here: 4244's 58w class is gettemp-region
 * sharing across 29 distinct webs, not a named-home position issue. */
/* 2026-10-16 agent-g: EXACT (was 2413/2479 words, "capped").
 *  - prologue: param reassign (arg0 is the live object, no var_s0 copy;
 *    var_s0 kept as an unused decl so the frame map holds) puts
 *    move s0,a0 right after sw s0 (docs/IDO_CODEGEN.md
 *    #prologue-move-param-vs-copy-62368).
 *  - class C (58w): units 2..32 share ONE name (var_a0_2, declared LAST ->
 *    home 0x38). One name alone colors to s3 once it covers >= ~26 units;
 *    an empty if (1) {} after the ctor call in 18..30 of the units adds BBs
 *    so the LR stays uncolored (home 0x38 + in-BB a0 cache = the target's
 *    sw/lw a0,0x38 pairs). 31 if(1)s or any in unit 1 flips s1/s2. The
 *    unused var_a0_4..30 decls must stay (dropping them moves the frame).
 *  - class B (2w, li at,-8 vs addiu s2,sp,84): the hoisted &sp78 / &sp54
 *    address materializations carry the .loc of the last statement before
 *    unit 1 (the var_v1 store); as1 puts li at,-8 first only when that
 *    statement and unit 1's null test share ONE source line, hence the long
 *    joined line below (same-line join lever, as for the 25 lui pairs).
 *  The older notes below describe the pre-crack state.
 *  See docs/IDO_CODEGEN.md #if1-bbs-uncolor-shared-name-4244. */
/* PASS-O 2026-06-11 (578B4-ladder from 53.60 -> 99.76, insns 2479 exact,
 * 96.5% words byte-exact): 32 light-record units rewritten as per-unit
 * V4/F1 structs w/ aggregate copies (beats const-forwarding/DSE);
 * &D_00000000 depooled into per-offset aliases (kills HI16-pool s-reg);
 * per-unit cd58 aliases break the 31-use constant LR (s-reg cascade
 * snaps: arg0->s0, &sp78->s1@120, &sp54->s2@84); ptr-name pairing
 * (14 names) + decl layout + 2 pads = frame 1864 exact.
 * RESIDUE CHARACTERIZED (agent-y 2026-06-15, reloc-filtered word LCS
 * 2417/2479, positional 2413; the "~87" was a fuzzy artifact, true 66
 * non-reloc words). ZERO logic/structural errors; all 66 = 3 caps:
 *   C (58w, 29 sw/lw pairs): per-unit var_a0 spill home — target shares
 *     ONE temploc 0x38(sp) (anonymous spilltemp, rule-6 region sharing);
 *     we get distinct named-M-class homes 0xC0..0x90 (rule-1, no repack).
 *     NO-CLEAN-HANDLE: distinct names=distinct slots; single name=COLORS
 *     to a new s-reg (spill vanishes, -30 insns); volatile=shares slot
 *     but reloads every use (+insns). All verified in-tree + 2-unit mini
 *     + -zdbug:6. See docs/IDO_CODEGEN.md "4244 ladder levers".
 *   A (6w): prologue as1 schedule — target hoists move s0,a0 above saves,
 *     fills bnez delay with sdc1 $f20. No-handle.
 *   B (2w): li at,-8 (sentinel) vs addiu s2,sp,84 (&sp54 hoist) order.
 *     No-handle (sp54 is loop-invariant; source position doesn't move it).
 * At byte-exact ceiling under no-instruction-forcing policy; leave NM. */
extern int D_00000000_cd58_u1;
extern int D_00000000_cd58_u2;
extern int D_00000000_cd58_u3;
extern int D_00000000_cd58_u4;
extern int D_00000000_cd58_u5;
extern int D_00000000_cd58_u6;
extern int D_00000000_cd58_u7;
extern int D_00000000_cd58_u8;
extern int D_00000000_cd58_u9;
extern int D_00000000_cd58_u10;
extern int D_00000000_cd58_u11;
extern int D_00000000_cd58_u12;
extern int D_00000000_cd58_u13;
extern int D_00000000_cd58_u14;
extern int D_00000000_cd58_u15;
extern int D_00000000_cd58_u16;
extern int D_00000000_cd58_u17;
extern int D_00000000_cd58_u18;
extern int D_00000000_cd58_u19;
extern int D_00000000_cd58_u20;
extern int D_00000000_cd58_u21;
extern int D_00000000_cd58_u22;
extern int D_00000000_cd58_u23;
extern int D_00000000_cd58_u24;
extern int D_00000000_cd58_u25;
extern int D_00000000_cd58_u26;
extern int D_00000000_cd58_u27;
extern int D_00000000_cd58_u28;
extern int D_00000000_cd58_u29;
extern int D_00000000_cd58_u30;
extern int D_00000000_cd58_u31;
extern int D_00000000_cd58_u32;
extern f32 kRo_ColourParms_ctor_D0C;
extern f32 kRo_ColourParms_ctor_D10;
extern f32 kRo_ColourParms_ctor_D14;
extern f32 kRo_ColourParms_ctor_D18;
extern f32 kRo_ColourParms_ctor_D1C;
extern f32 kRo_ColourParms_ctor_D20;
extern f32 kRo_ColourParms_ctor_D24;
extern f32 kRo_ColourParms_ctor_D28;
extern f32 kRo_ColourParms_ctor_D2C;
extern f32 kRo_ColourParms_ctor_D30;
extern f32 kRo_ColourParms_ctor_D34;
extern f32 kRo_ColourParms_ctor_D38;
extern f32 kRo_ColourParms_ctor_D3C;
extern f32 kRo_ColourParms_ctor_D40;
extern f32 kRo_ColourParms_ctor_D44;
extern f32 kRo_ColourParms_ctor_D48;
extern f32 kRo_ColourParms_ctor_D4C;
extern f32 kRo_ColourParms_ctor_D50;
extern f32 kRo_ColourParms_ctor_D54;
extern f32 kRo_ColourParms_ctor_D58;
extern f32 kRo_ColourParms_ctor_D5C;
extern f32 kRo_ColourParms_ctor_D60;
extern f32 kRo_ColourParms_ctor_D64;
extern f32 kRo_ColourParms_ctor_D68;
extern f32 kRo_ColourParms_ctor_D6C;
extern f32 kRo_ColourParms_ctor_D70;
extern f32 kRo_ColourParms_ctor_D74;
extern f32 kRo_ColourParms_ctor_D78;
extern f32 kRo_ColourParms_ctor_D7C;
extern f32 kRo_ColourParms_ctor_D80;
extern f32 kRo_ColourParms_ctor_D84;
extern f32 kRo_ColourParms_ctor_D88;
extern f32 kRo_ColourParms_ctor_D8C;
extern f32 kRo_ColourParms_ctor_D90;
extern f32 kRo_ColourParms_ctor_D94;
extern f32 kRo_ColourParms_ctor_D9C;
extern f32 kRo_ColourParms_ctor_DA0;
extern f32 kRo_ColourParms_ctor_DA4;
extern f32 kRo_ColourParms_ctor_DA8;
extern f32 kRo_ColourParms_ctor_DAC;
extern f32 kRo_ColourParms_ctor_DB0;
extern f32 kRo_ColourParms_ctor_DB4;
extern f32 kRo_ColourParms_ctor_DB8;
extern f32 kRo_ColourParms_ctor_DBC;
extern f32 kRo_ColourParms_ctor_DC0;
extern f32 kRo_ColourParms_ctor_DC4;
extern f32 kRo_ColourParms_ctor_DC8;
extern f32 kRo_ColourParms_ctor_DCC;
extern f32 kRo_ColourParms_ctor_DD0;
extern f32 kRo_ColourParms_ctor_DD4;
extern f32 kRo_ColourParms_ctor_DDC;
extern f32 kRo_ColourParms_ctor_DE0;
extern f32 kRo_ColourParms_ctor_DE4;
extern char sStr_Colour_Parms_CF80; /* "Colour::Parms" */
extern int sTag_rdP_CF90;
extern int sTag_rdE_CF94;
extern int sTag_32P_CF98;
extern int sTag_32E_CF9C;
extern int sTag_goP_CFA0;
extern int sTag_goE_CFA4;
extern int sTag_glP_CFA8;
extern int sTag_glE_CFAC;
extern int sTag_lsP_CFB0;
extern int sTag_lsE_CFB4;
extern int sTag_rtP_CFB8;
extern int sTag_rtE_CFBC;
extern int sTag_toP_CFC0;
extern int sTag_toE_CFC4;
extern int sTag_rnP_CFC8;
extern int sTag_rnE_CFCC;
extern int sTag_rsP_CFD0;
extern int sTag_rsE_CFD4;
extern int sTag_nrP_CFD8;
extern int sTag_nrE_CFDC;
extern int sTag_adP_CFE0;
extern int sTag_adE_CFE4;
extern int sTag_csP_CFE8;
extern int sTag_csE_CFEC;
extern int sTag_grP_CFF0;
extern int sTag_grE_CFF4;
extern int sTag_dwP_CFF8;
extern int sTag_dwE_CFFC;
extern int sTag_ctt_D000;
extern int sTag_cnm_D004;
extern int sTag_ccl_D008;
extern int sTag_clg_D00C;
typedef struct { f32 x, y, z, w; } GlV4_4244;
typedef struct { s32 v; } GlF1_4244;
/* 9916-byte fully-unrolled initializer (32 units, zero backward branches).
 * NM 99.762%, residual = CONFIRMED CAP, no logic/decode error remains.
 * DO NOT re-grind names/decl-order/pads — see docs/IDO_CODEGEN.md
 * "4244 ladder levers" (agent-y 2026-06-15, full characterization).
 * Residual re-verified independently 2026-06-17 (116 insn diffs, identical
 * picture): 58 = CLASS C spill-home sharing (target reuses ONE temploc
 * sw/lw a0,0x38(sp) for every unit's alloc-ptr spill; any C spelling makes
 * it a NAMED M-class home — uopt never repacks named homes, rule 1; the
 * only C forms that share a slot either drop the spill entirely or force
 * volatile reloads); 50 = lui-order schedule swap; 6 = prologue or-first.
 * Canonical "spilltemps region-sharing unreproduced" cap. */
void *ColourParms_ctor(char *arg0) {
    GlV4_4244 u1_a;
    GlV4_4244 u1_b;
    GlV4_4244 u1_c;
    GlF1_4244 u1_f;
    GlV4_4244 u2_a;
    GlV4_4244 u2_b;
    GlV4_4244 u2_c;
    GlF1_4244 u2_f;
    GlV4_4244 u3_a;
    GlV4_4244 u3_b;
    GlV4_4244 u3_c;
    GlF1_4244 u3_f;
    GlV4_4244 u4_a;
    GlV4_4244 u4_b;
    GlV4_4244 u4_c;
    GlF1_4244 u4_f;
    GlV4_4244 u5_a;
    GlV4_4244 u5_b;
    GlV4_4244 u5_c;
    GlF1_4244 u5_f;
    GlV4_4244 u6_a;
    GlV4_4244 u6_b;
    GlV4_4244 u6_c;
    GlF1_4244 u6_f;
    GlV4_4244 u7_a;
    GlV4_4244 u7_b;
    GlV4_4244 u7_c;
    GlF1_4244 u7_f;
    GlV4_4244 u8_a;
    GlV4_4244 u8_b;
    GlV4_4244 u8_c;
    GlF1_4244 u8_f;
    GlV4_4244 u9_a;
    GlV4_4244 u9_b;
    GlV4_4244 u9_c;
    GlF1_4244 u9_f;
    GlV4_4244 u10_a;
    GlV4_4244 u10_b;
    GlV4_4244 u10_c;
    GlF1_4244 u10_f;
    GlV4_4244 u11_a;
    GlV4_4244 u11_b;
    GlV4_4244 u11_c;
    GlF1_4244 u11_f;
    GlV4_4244 u12_a;
    GlV4_4244 u12_b;
    GlV4_4244 u12_c;
    GlF1_4244 u12_f;
    GlV4_4244 u13_a;
    GlV4_4244 u13_b;
    GlV4_4244 u13_c;
    GlF1_4244 u13_f;
    GlV4_4244 u14_a;
    GlV4_4244 u14_b;
    GlV4_4244 u14_c;
    GlF1_4244 u14_f;
    GlV4_4244 u15_a;
    GlV4_4244 u15_b;
    GlV4_4244 u15_c;
    GlF1_4244 u15_f;
    GlV4_4244 u16_a;
    GlV4_4244 u16_b;
    GlV4_4244 u16_c;
    GlF1_4244 u16_f;
    GlV4_4244 u17_a;
    GlV4_4244 u17_b;
    GlV4_4244 u17_c;
    GlF1_4244 u17_f;
    GlV4_4244 u18_a;
    GlV4_4244 u18_b;
    GlV4_4244 u18_c;
    GlF1_4244 u18_f;
    GlV4_4244 u19_a;
    GlV4_4244 u19_b;
    GlV4_4244 u19_c;
    GlF1_4244 u19_f;
    GlV4_4244 u20_a;
    GlV4_4244 u20_b;
    GlV4_4244 u20_c;
    GlF1_4244 u20_f;
    GlV4_4244 u21_a;
    GlV4_4244 u21_b;
    GlV4_4244 u21_c;
    GlF1_4244 u21_f;
    GlV4_4244 u22_a;
    GlV4_4244 u22_b;
    GlV4_4244 u22_c;
    GlF1_4244 u22_f;
    GlV4_4244 u23_a;
    GlV4_4244 u23_b;
    GlV4_4244 u23_c;
    GlF1_4244 u23_f;
    GlV4_4244 u24_a;
    GlV4_4244 u24_b;
    GlV4_4244 u24_c;
    GlF1_4244 u24_f;
    GlV4_4244 u25_a;
    GlV4_4244 u25_b;
    GlV4_4244 u25_c;
    GlF1_4244 u25_f;
    GlV4_4244 u26_a;
    GlV4_4244 u26_b;
    GlV4_4244 u26_c;
    GlF1_4244 u26_f;
    GlV4_4244 u27_a;
    GlV4_4244 u27_b;
    GlV4_4244 u27_c;
    GlF1_4244 u27_f;
    GlV4_4244 u28_a;
    GlV4_4244 u28_b;
    GlV4_4244 u28_c;
    GlF1_4244 u28_f;
    GlV4_4244 u29_a;
    GlV4_4244 u29_b;
    GlV4_4244 u29_c;
    GlF1_4244 u29_f;
    GlV4_4244 u30_a;
    GlV4_4244 u30_b;
    GlV4_4244 u30_c;
    GlF1_4244 u30_f;
    GlV4_4244 u31_a;
    GlV4_4244 u31_b;
    GlV4_4244 u31_c;
    GlF1_4244 u31_f;
    GlV4_4244 u32_a;
    GlV4_4244 u32_b;
    GlV4_4244 u32_c;
    GlF1_4244 u32_f;
    volatile s32 pad_4244_a;
    char *var_a0_4;
    char *var_a0_6;
    char *var_a0_8;
    char *var_a0_10;
    char *var_a0_12;
    char *var_a0_14;
    char *var_a0_16;
    char *var_a0_18;
    char *var_a0_20;
    char *var_a0_22;
    char *var_a0_24;
    char *var_a0_26;
    char *var_a0_30;
    char *var_a0;
    volatile s32 pad_4244_b;
    GlV4_4244 sp78;
    GlV4_4244 sp68;
    GlV4_4244 sp58;
    GlF1_4244 sp54;
    f32 temp_f0;
    f32 temp_f0_2;
    char *temp_v0;
    char *temp_v0_2;
    char *var_s0;
    char *var_v1;
    char *var_a0_2;

    if ((arg0 != 0) || (temp_v0 = FCALL(mem_new)(0x488), arg0 = temp_v0, (temp_v0 != 0))) {
        var_v1 = arg0;
        if ((arg0 != 0) || (temp_v0_2 = FCALL(mem_new)(8), var_v1 = temp_v0_2, (temp_v0_2 != 0))) {
            PARMS(var_v1)->name = &sStr_Colour_Parms_CF80;
        PARMS(var_v1)->first = 0; } u1_a.x = 1.0f; u1_a.y = 1.0f; u1_a.z = 1.0f; u1_c.x = 1.0f; u1_c.y = 1.0f; u1_c.z = 1.0f; u1_c.w = 1.0f; u1_a.w = 0.0f; u1_b.x = 0.0f; u1_b.y = 0.0f; u1_b.z = 0.0f; u1_b.w = 0.0f; u1_f.v = sTag_rdP_CF90; sp78 = u1_a; var_a0 = (char *)&((RaceColourParms *)arg0)->pair[RACECOL_RD][0]; sp68 = u1_b; sp58 = u1_c; sp54 = u1_f; if ((arg0 != (void *)-8) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0, arg0, sp54, 1);
            PARMRGBA(var_a0)->type = (char *)&D_00000000_cd58_u1 + 0xCD58;
            PARMRGBA(var_a0)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0)->value = sp78;
        }
        u2_a.x = 1.0f;
        u2_a.y = 1.0f;
        u2_a.z = 1.0f;
        u2_a.w = 0.0f;
        u2_b.x = 0.0f;
        u2_b.y = 0.0f;
        u2_b.z = 0.0f;
        u2_b.w = 0.0f;
        u2_c.x = 1.0f;
        u2_c.y = 1.0f;
        u2_c.z = 1.0f;
        u2_c.w = 1.0f;
        u2_f.v = sTag_rdE_CF94;
        sp78 = u2_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->pair[RACECOL_RD][1];
        sp68 = u2_b;
        sp58 = u2_c;
        sp54 = u2_f;
        if ((arg0 != (void *)-0x2C) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1); if (1) {}
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u2 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        u3_a.x = 1.0f;
        u3_a.y = 1.0f;
        u3_a.z = 1.0f;
        u3_a.w = 0.0f;
        u3_b.x = 0.0f;
        u3_b.y = 0.0f;
        u3_b.z = 0.0f;
        u3_b.w = 0.0f;
        u3_c.x = 1.0f;
        u3_c.y = 1.0f;
        u3_c.z = 1.0f;
        u3_c.w = 1.0f;
        u3_f.v = sTag_32P_CF98;
        sp78 = u3_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->pair[RACECOL_32][0];
        sp68 = u3_b;
        sp58 = u3_c;
        sp54 = u3_f;
        if ((arg0 != (void *)-0x50) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1); if (1) {}
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u3 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        u4_a.x = 1.0f;
        u4_a.y = 1.0f;
        u4_a.z = 1.0f;
        u4_a.w = 0.0f;
        u4_b.x = 0.0f;
        u4_b.y = 0.0f;
        u4_b.z = 0.0f;
        u4_b.w = 0.0f;
        u4_c.x = 1.0f;
        u4_c.y = 1.0f;
        u4_c.z = 1.0f;
        u4_c.w = 1.0f;
        u4_f.v = sTag_32E_CF9C;
        sp78 = u4_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->pair[RACECOL_32][1];
        sp68 = u4_b;
        sp58 = u4_c;
        sp54 = u4_f;
        if ((arg0 != (void *)-0x74) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1); if (1) {}
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u4 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        u5_a.x = 1.0f;
        u5_a.w = 0.0f;
        u5_b.x = 0.0f;
        u5_b.y = 0.0f;
        u5_b.z = 0.0f;
        u5_b.w = 0.0f;
        u5_c.x = 1.0f;
        u5_c.y = 1.0f;
        u5_c.z = 1.0f;
        u5_c.w = 1.0f;
        u5_a.y = kRo_ColourParms_ctor_D0C;
        u5_f.v = sTag_goP_CFA0; u5_a.z = kRo_ColourParms_ctor_D10;
        sp78 = u5_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->pair[RACECOL_GO][0];
        sp68 = u5_b;
        sp58 = u5_c;
        sp54 = u5_f;
        if ((arg0 != (void *)-0x98) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1); if (1) {}
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u5 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        u6_a.x = 1.0f;
        u6_a.y = 1.0f;
        u6_a.w = 0.0f;
        u6_b.x = 0.0f;
        u6_b.y = 0.0f;
        u6_b.z = 0.0f;
        u6_b.w = 0.0f;
        u6_c.x = 1.0f;
        u6_c.y = 1.0f;
        u6_c.z = 1.0f;
        u6_c.w = 1.0f;
        u6_a.z = kRo_ColourParms_ctor_D14; u6_f.v = sTag_goE_CFA4;
        sp78 = u6_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->pair[RACECOL_GO][1];
        sp68 = u6_b;
        sp58 = u6_c;
        sp54 = u6_f;
        if ((arg0 != (void *)-0xBC) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1); if (1) {}
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u6 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        u7_a.x = 1.0f;
        u7_a.w = 0.0f;
        u7_b.x = 0.0f;
        u7_b.y = 0.0f;
        u7_b.z = 0.0f;
        u7_b.w = 0.0f;
        u7_c.x = 1.0f;
        u7_c.y = 1.0f;
        u7_c.z = 1.0f;
        u7_c.w = 1.0f;
        u7_a.y = kRo_ColourParms_ctor_D18;
        u7_f.v = sTag_glP_CFA8; u7_a.z = kRo_ColourParms_ctor_D1C;
        sp78 = u7_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->pair[RACECOL_GL][0];
        sp68 = u7_b;
        sp58 = u7_c;
        sp54 = u7_f;
        if ((arg0 != (void *)-0xE0) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1); if (1) {}
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u7 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        u8_a.x = 1.0f;
        u8_a.y = 1.0f;
        u8_a.w = 0.0f;
        u8_b.x = 0.0f;
        u8_b.y = 0.0f;
        u8_b.z = 0.0f;
        u8_b.w = 0.0f;
        u8_c.x = 1.0f;
        u8_c.y = 1.0f;
        u8_c.z = 1.0f;
        u8_c.w = 1.0f;
        u8_a.z = kRo_ColourParms_ctor_D20; u8_f.v = sTag_glE_CFAC;
        sp78 = u8_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->pair[RACECOL_GL][1];
        sp68 = u8_b;
        sp58 = u8_c;
        sp54 = u8_f;
        if ((arg0 != (void *)-0x104) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1); if (1) {}
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u8 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        u9_a.x = 1.0f;
        u9_a.w = 0.0f;
        u9_b.x = 0.0f;
        u9_b.y = 0.0f;
        u9_b.z = 0.0f;
        u9_b.w = 0.0f;
        u9_c.x = 1.0f;
        u9_c.y = 1.0f;
        u9_c.z = 1.0f;
        u9_c.w = 1.0f;
        u9_a.y = kRo_ColourParms_ctor_D24;
        u9_f.v = sTag_lsP_CFB0; u9_a.z = kRo_ColourParms_ctor_D28;
        sp78 = u9_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->pair[RACECOL_LS][0];
        sp68 = u9_b;
        sp58 = u9_c;
        sp54 = u9_f;
        if ((arg0 != (void *)-0x128) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1); if (1) {}
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u9 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        u10_a.x = 1.0f;
        u10_a.y = 1.0f;
        u10_a.w = 0.0f;
        u10_b.x = 0.0f;
        u10_b.y = 0.0f;
        u10_b.z = 0.0f;
        u10_b.w = 0.0f;
        u10_c.x = 1.0f;
        u10_c.y = 1.0f;
        u10_c.z = 1.0f;
        u10_c.w = 1.0f;
        u10_a.z = kRo_ColourParms_ctor_D2C; u10_f.v = sTag_lsE_CFB4;
        sp78 = u10_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->pair[RACECOL_LS][1];
        sp68 = u10_b;
        sp58 = u10_c;
        sp54 = u10_f;
        if ((arg0 != (void *)-0x14C) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1); if (1) {}
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u10 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        u11_a.x = 1.0f;
        u11_a.w = 0.0f;
        u11_b.x = 0.0f;
        u11_b.y = 0.0f;
        u11_b.z = 0.0f;
        u11_b.w = 0.0f;
        u11_c.x = 1.0f;
        u11_c.y = 1.0f;
        u11_c.z = 1.0f;
        u11_c.w = 1.0f;
        u11_a.y = kRo_ColourParms_ctor_D30;
        u11_f.v = sTag_rtP_CFB8; u11_a.z = kRo_ColourParms_ctor_D34;
        sp78 = u11_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->pair[RACECOL_RT][0];
        sp68 = u11_b;
        sp58 = u11_c;
        sp54 = u11_f;
        if ((arg0 != (void *)-0x170) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1); if (1) {}
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u11 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        u12_a.x = 1.0f;
        u12_a.y = 1.0f;
        u12_a.w = 0.0f;
        u12_b.x = 0.0f;
        u12_b.y = 0.0f;
        u12_b.z = 0.0f;
        u12_b.w = 0.0f;
        u12_c.x = 1.0f;
        u12_c.y = 1.0f;
        u12_c.z = 1.0f;
        u12_c.w = 1.0f;
        u12_a.z = kRo_ColourParms_ctor_D38; u12_f.v = sTag_rtE_CFBC;
        sp78 = u12_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->pair[RACECOL_RT][1];
        sp68 = u12_b;
        sp58 = u12_c;
        sp54 = u12_f;
        if ((arg0 != (void *)-0x194) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1); if (1) {}
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u12 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        u13_a.x = 1.0f;
        u13_a.w = 0.0f;
        u13_b.x = 0.0f;
        u13_b.y = 0.0f;
        u13_b.z = 0.0f;
        u13_b.w = 0.0f;
        u13_c.x = 1.0f;
        u13_c.y = 1.0f;
        u13_c.z = 1.0f;
        u13_c.w = 1.0f;
        u13_a.y = kRo_ColourParms_ctor_D3C;
        u13_f.v = sTag_toP_CFC0; u13_a.z = kRo_ColourParms_ctor_D40;
        sp78 = u13_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->pair[RACECOL_TO][0];
        sp68 = u13_b;
        sp58 = u13_c;
        sp54 = u13_f;
        if ((arg0 != (void *)-0x1B8) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1); if (1) {}
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u13 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        u14_a.x = 1.0f;
        u14_a.y = 1.0f;
        u14_a.w = 0.0f;
        u14_b.x = 0.0f;
        u14_b.y = 0.0f;
        u14_b.z = 0.0f;
        u14_b.w = 0.0f;
        u14_c.x = 1.0f;
        u14_c.y = 1.0f;
        u14_c.z = 1.0f;
        u14_c.w = 1.0f;
        u14_a.z = kRo_ColourParms_ctor_D44; u14_f.v = sTag_toE_CFC4;
        sp78 = u14_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->pair[RACECOL_TO][1];
        sp68 = u14_b;
        sp58 = u14_c;
        sp54 = u14_f;
        if ((arg0 != (void *)-0x1DC) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1); if (1) {}
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u14 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        u15_a.x = 1.0f;
        u15_a.w = 0.0f;
        u15_b.x = 0.0f;
        u15_b.y = 0.0f;
        u15_b.z = 0.0f;
        u15_b.w = 0.0f;
        u15_c.x = 1.0f;
        u15_c.y = 1.0f;
        u15_c.z = 1.0f;
        u15_c.w = 1.0f;
        u15_a.y = kRo_ColourParms_ctor_D48;
        u15_f.v = sTag_rnP_CFC8; u15_a.z = kRo_ColourParms_ctor_D4C;
        sp78 = u15_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->pair[RACECOL_RN][0];
        sp68 = u15_b;
        sp58 = u15_c;
        sp54 = u15_f;
        if ((arg0 != (void *)-0x200) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1); if (1) {}
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u15 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        u16_a.z = 0.0f;
        u16_a.w = 0.0f;
        u16_b.x = 0.0f;
        u16_b.y = 0.0f;
        u16_b.z = 0.0f;
        u16_b.w = 0.0f;
        u16_c.x = 1.0f;
        u16_c.y = 1.0f;
        u16_c.z = 1.0f;
        u16_c.w = 1.0f;
        u16_a.x = kRo_ColourParms_ctor_D50;
        u16_f.v = sTag_rnE_CFCC; u16_a.y = kRo_ColourParms_ctor_D54;
        sp78 = u16_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->pair[RACECOL_RN][1];
        sp68 = u16_b;
        sp58 = u16_c;
        sp54 = u16_f;
        if ((arg0 != (void *)-0x224) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1); if (1) {}
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u16 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        u17_a.w = 0.0f;
        u17_b.x = 0.0f;
        u17_b.y = 0.0f;
        u17_b.z = 0.0f;
        u17_b.w = 0.0f;
        u17_c.x = 1.0f;
        u17_c.y = 1.0f;
        u17_c.z = 1.0f;
        u17_c.w = 1.0f;
        u17_a.x = kRo_ColourParms_ctor_D58;
        u17_a.y = kRo_ColourParms_ctor_D5C;
        u17_f.v = sTag_rsP_CFD0; u17_a.z = kRo_ColourParms_ctor_D60;
        sp78 = u17_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->pair[RACECOL_RS][0];
        sp68 = u17_b;
        sp58 = u17_c;
        sp54 = u17_f;
        if ((arg0 != (void *)-0x248) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1); if (1) {}
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u17 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        u18_a.x = 0.0f;
        u18_a.y = 0.0f;
        u18_a.w = 0.0f;
        u18_b.x = 0.0f;
        u18_b.y = 0.0f;
        u18_b.z = 0.0f;
        u18_b.w = 0.0f;
        u18_c.x = 1.0f;
        u18_c.y = 1.0f;
        u18_c.z = 1.0f;
        u18_c.w = 1.0f;
        u18_a.z = kRo_ColourParms_ctor_D64; u18_f.v = sTag_rsE_CFD4;
        sp78 = u18_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->pair[RACECOL_RS][1];
        sp68 = u18_b;
        sp58 = u18_c;
        sp54 = u18_f;
        if ((arg0 != (void *)-0x26C) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1); if (1) {}
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u18 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        u19_a.y = 1.0f;
        u19_a.z = 0.0f;
        u19_a.w = 0.0f;
        u19_b.x = 0.0f;
        u19_b.y = 0.0f;
        u19_b.z = 0.0f;
        u19_b.w = 0.0f;
        u19_c.x = 1.0f;
        u19_c.y = 1.0f;
        u19_c.z = 1.0f;
        u19_c.w = 1.0f;
        u19_a.x = kRo_ColourParms_ctor_D68; u19_f.v = sTag_nrP_CFD8;
        sp78 = u19_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->pair[RACECOL_NR][0];
        sp68 = u19_b;
        sp58 = u19_c;
        sp54 = u19_f;
        if ((arg0 != (void *)-0x290) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1); if (1) {}
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u19 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        u20_a.z = 0.0f;
        u20_a.w = 0.0f;
        u20_b.x = 0.0f;
        u20_b.y = 0.0f;
        u20_b.z = 0.0f;
        u20_b.w = 0.0f;
        u20_c.x = 1.0f;
        u20_c.y = 1.0f;
        u20_c.z = 1.0f;
        u20_c.w = 1.0f;
        u20_a.x = kRo_ColourParms_ctor_D6C;
        u20_f.v = sTag_nrE_CFDC; u20_a.y = kRo_ColourParms_ctor_D70;
        sp78 = u20_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->pair[RACECOL_NR][1];
        sp68 = u20_b;
        sp58 = u20_c;
        sp54 = u20_f;
        if ((arg0 != (void *)-0x2B4) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1); if (1) {}
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u20 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        u21_a.w = 0.0f;
        u21_b.x = 0.0f;
        u21_b.y = 0.0f;
        u21_b.z = 0.0f;
        u21_b.w = 0.0f;
        u21_c.x = 1.0f;
        u21_c.y = 1.0f;
        u21_c.z = 1.0f;
        u21_c.w = 1.0f;
        u21_a.x = kRo_ColourParms_ctor_D74;
        u21_a.y = kRo_ColourParms_ctor_D78;
        u21_f.v = sTag_adP_CFE0; u21_a.z = kRo_ColourParms_ctor_D7C;
        sp78 = u21_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->pair[RACECOL_AD][0];
        sp68 = u21_b;
        sp58 = u21_c;
        sp54 = u21_f;
        if ((arg0 != (void *)-0x2D8) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1); if (1) {}
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u21 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        u22_a.w = 0.0f;
        u22_b.x = 0.0f;
        u22_b.y = 0.0f;
        u22_b.z = 0.0f;
        u22_b.w = 0.0f;
        u22_c.x = 1.0f;
        u22_c.y = 1.0f;
        u22_c.z = 1.0f;
        u22_c.w = 1.0f;
        u22_a.x = kRo_ColourParms_ctor_D80;
        u22_a.y = kRo_ColourParms_ctor_D84;
        u22_f.v = sTag_adE_CFE4; u22_a.z = kRo_ColourParms_ctor_D88;
        sp78 = u22_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->pair[RACECOL_AD][1];
        sp68 = u22_b;
        sp58 = u22_c;
        sp54 = u22_f;
        if ((arg0 != (void *)-0x2FC) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1);
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u22 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        u23_a.x = 1.0f;
        u23_a.w = 0.0f;
        u23_b.x = 0.0f;
        u23_b.y = 0.0f;
        u23_b.z = 0.0f;
        u23_b.w = 0.0f;
        u23_c.x = 1.0f;
        u23_c.y = 1.0f;
        u23_c.z = 1.0f;
        u23_c.w = 1.0f;
        u23_a.y = kRo_ColourParms_ctor_D8C;
        u23_f.v = sTag_csP_CFE8; u23_a.z = kRo_ColourParms_ctor_D90;
        sp78 = u23_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->pair[RACECOL_CS][0];
        sp68 = u23_b;
        sp58 = u23_c;
        sp54 = u23_f;
        if ((arg0 != (void *)-0x320) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1);
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u23 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        temp_f0 = kRo_ColourParms_ctor_D94;
        u24_a.w = 0.0f;
        u24_b.x = 0.0f;
        u24_b.y = 0.0f;
        u24_b.z = 0.0f;
        u24_b.w = 0.0f;
        u24_c.x = 1.0f;
        u24_c.y = 1.0f;
        u24_c.z = 1.0f;
        u24_c.w = 1.0f;
        u24_a.x = temp_f0;
        u24_a.y = temp_f0;
        u24_a.z = temp_f0;
        u24_f.v = sTag_csE_CFEC;
        sp78 = u24_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->pair[RACECOL_CS][1];
        sp68 = u24_b;
        sp58 = u24_c;
        sp54 = u24_f;
        if ((arg0 != (void *)-0x344) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1);
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u24 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        u25_a.w = 0.0f;
        u25_b.x = 0.0f;
        u25_b.y = 0.0f;
        u25_b.z = 0.0f;
        u25_b.w = 0.0f;
        u25_c.x = 1.0f;
        u25_c.y = 1.0f;
        u25_c.z = 1.0f;
        u25_c.w = 1.0f;
        u25_a.x = kRo_ColourParms_ctor_D9C;
        u25_a.y = kRo_ColourParms_ctor_DA0;
        u25_f.v = sTag_grP_CFF0; u25_a.z = kRo_ColourParms_ctor_DA4;
        sp78 = u25_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->pair[RACECOL_GR][0];
        sp68 = u25_b;
        sp58 = u25_c;
        sp54 = u25_f;
        if ((arg0 != (void *)-0x368) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1);
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u25 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        u26_a.w = 0.0f;
        u26_b.x = 0.0f;
        u26_b.y = 0.0f;
        u26_b.z = 0.0f;
        u26_b.w = 0.0f;
        u26_c.x = 1.0f;
        u26_c.y = 1.0f;
        u26_c.z = 1.0f;
        u26_c.w = 1.0f;
        u26_a.x = kRo_ColourParms_ctor_DA8;
        u26_a.y = kRo_ColourParms_ctor_DAC;
        u26_f.v = sTag_grE_CFF4; u26_a.z = kRo_ColourParms_ctor_DB0;
        sp78 = u26_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->pair[RACECOL_GR][1];
        sp68 = u26_b;
        sp58 = u26_c;
        sp54 = u26_f;
        if ((arg0 != (void *)-0x38C) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1);
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u26 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        u27_a.w = 0.0f;
        u27_b.x = 0.0f;
        u27_b.y = 0.0f;
        u27_b.z = 0.0f;
        u27_b.w = 0.0f;
        u27_c.x = 1.0f;
        u27_c.y = 1.0f;
        u27_c.z = 1.0f;
        u27_c.w = 1.0f;
        u27_a.x = kRo_ColourParms_ctor_DB4;
        u27_a.y = kRo_ColourParms_ctor_DB8;
        u27_f.v = sTag_dwP_CFF8; u27_a.z = kRo_ColourParms_ctor_DBC;
        sp78 = u27_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->pair[RACECOL_DW][0];
        sp68 = u27_b;
        sp58 = u27_c;
        sp54 = u27_f;
        if ((arg0 != (void *)-0x3B0) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1);
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u27 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        u28_a.w = 0.0f;
        u28_b.x = 0.0f;
        u28_b.y = 0.0f;
        u28_b.z = 0.0f;
        u28_b.w = 0.0f;
        u28_c.x = 1.0f;
        u28_c.y = 1.0f;
        u28_c.z = 1.0f;
        u28_c.w = 1.0f;
        u28_a.x = kRo_ColourParms_ctor_DC0;
        u28_a.y = kRo_ColourParms_ctor_DC4;
        u28_f.v = sTag_dwE_CFFC; u28_a.z = kRo_ColourParms_ctor_DC8;
        sp78 = u28_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->pair[RACECOL_DW][1];
        sp68 = u28_b;
        sp58 = u28_c;
        sp54 = u28_f;
        if ((arg0 != (void *)-0x3D4) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1);
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u28 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        u29_a.x = 1.0f;
        u29_a.w = 0.0f;
        u29_b.x = 0.0f;
        u29_b.y = 0.0f;
        u29_b.z = 0.0f;
        u29_b.w = 0.0f;
        u29_c.x = 1.0f;
        u29_c.y = 1.0f;
        u29_c.z = 1.0f;
        u29_c.w = 1.0f;
        u29_a.y = kRo_ColourParms_ctor_DCC;
        u29_f.v = sTag_ctt_D000; u29_a.z = kRo_ColourParms_ctor_DD0;
        sp78 = u29_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->ctt;
        sp68 = u29_b;
        sp58 = u29_c;
        sp54 = u29_f;
        if ((arg0 != (void *)-0x3F8) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1);
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u29 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        temp_f0_2 = kRo_ColourParms_ctor_DD4;
        u30_a.w = 0.0f;
        u30_b.x = 0.0f;
        u30_b.y = 0.0f;
        u30_b.z = 0.0f;
        u30_b.w = 0.0f;
        u30_c.x = 1.0f;
        u30_c.y = 1.0f;
        u30_c.z = 1.0f;
        u30_c.w = 1.0f;
        u30_a.x = temp_f0_2;
        u30_a.y = temp_f0_2;
        u30_a.z = temp_f0_2;
        u30_f.v = sTag_cnm_D004;
        sp78 = u30_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->cnm;
        sp68 = u30_b;
        sp58 = u30_c;
        sp54 = u30_f;
        if ((arg0 != (void *)-0x41C) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1);
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u30 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        u31_a.w = 0.0f;
        u31_b.x = 0.0f;
        u31_b.y = 0.0f;
        u31_b.z = 0.0f;
        u31_b.w = 0.0f;
        u31_c.x = 1.0f;
        u31_c.y = 1.0f;
        u31_c.z = 1.0f;
        u31_c.w = 1.0f;
        u31_a.x = kRo_ColourParms_ctor_DDC;
        u31_a.y = kRo_ColourParms_ctor_DE0;
        u31_f.v = sTag_ccl_D008; u31_a.z = kRo_ColourParms_ctor_DE4;
        sp78 = u31_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->ccl;
        sp68 = u31_b;
        sp58 = u31_c;
        sp54 = u31_f;
        if ((arg0 != (void *)-0x440) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1);
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u31 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
        u32_a.x = 1.0f;
        u32_a.y = 1.0f;
        u32_a.z = 1.0f;
        u32_a.w = 0.0f;
        u32_b.x = 0.0f;
        u32_b.y = 0.0f;
        u32_b.z = 0.0f;
        u32_b.w = 0.0f;
        u32_c.x = 1.0f;
        u32_c.y = 1.0f;
        u32_c.z = 1.0f;
        u32_c.w = 1.0f;
        u32_f.v = sTag_clg_D00C;
        sp78 = u32_a;
        var_a0_2 = (char *)&((RaceColourParms *)arg0)->clg;
        sp68 = u32_b;
        sp58 = u32_c;
        sp54 = u32_f;
        if ((arg0 != (void *)-0x464) || (temp_v0_2 = FCALL(mem_new)(0x24), var_a0_2 = temp_v0_2, (temp_v0_2 != 0))) {
            FCALL(Parms_addParm)(var_a0_2, arg0, sp54, 1);
            PARMRGBA(var_a0_2)->type = (char *)&D_00000000_cd58_u32 + 0xCD58;
            PARMRGBA(var_a0_2)->unk20 = 0;
            *(GlV4_4244 *)PARMRGBA(var_a0_2)->value = sp78;
        }
    }
    return arg0;
}

#ifndef FW
#define FW(p, o) (*(int *)((char *)(p) + (o)))
#endif
void RaceProcess_init(RaceProcess *arg0, s32 arg1, const char *arg2, s32 arg3) {
    volatile int pad;
    char *sp28;
    char *temp_v0;
    char *temp_v0_2;

    arg0->name = arg2;
    arg0->unk04C = arg3;
    arg0->flags = arg1;
    if (arg1 & RACEF_SPRITES) {
        if (arg0->spriteSet == 0) {
            temp_v0 = (char*)FCALL(gui_fontCreate)(0, &D_00000000, 0x48, 0xEE, 3, 0xE);
            arg0->spriteSet = temp_v0;
            (char*)FCALL(gui_fontAdjustAdvance)(temp_v0, (char *)-8);
        }
        (char*)FCALL(gui_spriteLoad)(&arg0->sprites[0], 0x60000);
        (char*)FCALL(gui_spriteLoad)(&arg0->sprites[1], 0x60003);
        (char*)FCALL(gui_spriteLoad)(&arg0->sprites[2], 0x60004);
        (char*)FCALL(gui_spriteLoad)(&arg0->sprites[3], 0x60005);
        (char*)FCALL(gui_spriteLoad)(&arg0->sprites[4], 0x60001);
        (char*)FCALL(gui_spriteInit)(&arg0->sprites[5], 0x60002);
        (char*)FCALL(gui_spriteInit)(&arg0->sprites[6], 0x60006);
        (char*)FCALL(gui_spriteInit)(&arg0->sprites[7], 0x60007);
        (char*)FCALL(gui_spriteInit)(&arg0->sprites[9], 0x60008);
        (char*)FCALL(gui_spriteInit)(&arg0->sprites[10], 0x60009);
        (char*)FCALL(gui_spriteInit)(&arg0->sprites[11], 0x6000A);
        (char*)FCALL(gui_spriteInit)(&arg0->sprites[8], 0x6000B);
    }
    arg0->subState = 0;
    (char*)FCALL(Node_Load)(arg0, (char *)1, GL_DATA_STR(0xD010, "rom/racecol.bin"));
    temp_v0_2 = (char*)FCALL(Node_ctor)(0, GL_DATA_STR(0xD020, "Extensions"));
    arg0->handler = temp_v0_2;
    sp28 = temp_v0_2;
    (char*)FCALL(list_append)((int)arg0->children, temp_v0_2);
    if (NODE(sp28)->parent != 0) {
        NODE(sp28)->unk04 = 1;
    }
    NODE(sp28)->parent = (Node *)arg0;
}

extern int gl_func_00000000();
int RaceProcess_restart(RaceProcess *a0, int a1) {
    a0->flags = a1;
    return CALL(RaceProcess_reset)(a0);
}

/* RaceProcess_reset - verified structural decode (0xD4, 53 insns,
 * object reset/init). SIBLING of RaceProcess_endRace (same large object
 * + same global G = *(&D)); this is the clear/reset entry.
 * Struct-typing reference (extends the RaceProcess_endRace object map):
 * 0x2C (44) / 0x30 (48) s32 = 0; 0x4DC (1244) / 0x4E0 (1248) s32 = 0
 * (the "2/3" pair RaceProcess_endRace sets - here zeroed); 0x4E4 (1252)
 * s32 timer = 120 or 0 by bit 18 of 0x4F0; 0x4E8 (1256) s32 = 20;
 * 0x4F0 (1264) s32 mode/flags (bit 18 gate, also copied to G->0x8C);
 * 0x4F4 (1268) s32 = 0x1FFFF mask; 0x4FC (1276) / 0x500 (1280) /
 * 0x508 (1288) / 0x510 (1296) s32 = 0; 0x534 (1332) / 0x538 (1336)
 * f32 = 1.0; 0x540 (1344) s32 = 255. Global G = *(&D): G->0x78 (120)
 * = 1, G->0x7C (124) = 0, G->0x8C (140) = a0->0x4F0; plus 3 standalone
 * global f32 zeroed.
 * EXACT MATCH 2026-07-11 (agent-f, mis-diagnosed-cap crossing). The "caps <80"
 * were global-CSE + store-order, not FP/reloc: target holds the &D base in ONE
 * register across the two post-call stores (G->0x8C, G->0x7C); build re-derived
 * it via lui $at per site. Fix: a FRESH post-call base pointer H=(int*)&D reused
 * for both stores (persistent reg) cascades to fix the FP-const load order +
 * jr-delay fill; swap the 0x510/0x508 zero-stores; gl_d_x/y/z = 3 distinct
 * base-0 aliases reproduce target's 3 separate lui $at,0 (no CSE). cmp clean. */
extern float gl_d_x, gl_d_y, gl_d_z;  /* 3 global floats zeroed by this fn */
void RaceProcess_reset(RaceProcess *a0) {
    int *G = (int*)&gInputMgr;
    int *H;
    a0->unk030 = 0;
    a0->unk02C = 0;
    a0->phase = 0;
    a0->raceState = 0;
    a0->unk4FC = 0;
    a0->timeout = 0x1FFFF;
    G[0x78 / 4] = 1;
    CALL(input_setRacePadsActive)(&gInputMgr, 0);
    a0->countStep = 20;
    a0->timer = (a0->flags & RACEF_INTRO_DELAY) ? 120 : 0;
    H = (int*)&gInputMgr;
    H[0x8C / 4] = a0->flags;
    a0->countAlpha = 255;
    a0->scaleX = 1.0f;
    a0->scaleY = 1.0f;
    gl_d_x = 0.0f;
    gl_d_y = 0.0f;
    gl_d_z = 0.0f;
    a0->leave = 0;
    a0->leftOnce = 0;
    a0->unk508 = 0;
    H[0x7C / 4] = 0;
}

/* RaceProcess_endRace - object initializer (0xB8, 46 insns). MATCHED 2026-06-20.
 * Two landing levers: (1) assign a0->0x4E4 = a1 BEFORE the two zero-stores so
 * IDO's as1 scheduler hoists the a1 reload + the &D mode-load lui to cover
 * load latency (matches target schedule exactly); (2) call the vtable fn
 * THROUGH the dereferenced pointer (no intermediate `fn` local) so IDO uses
 * $t9 for the jalr target instead of $v1. a0 = large object: 0x4DC=2, 0x4E0=3,
 * 0x4E4=a1, 0x4EC=0, 0x4F8=a2, 0x518=0, 0x544=255, 0x54C f32 duration (120,
 * or 60 when global mode *(&D)->0x34==2), 0x550=0.0, 0x554=150.0; a0->0x28
 * vtable ptr, fn @0x7C, s16 base-adjust @0x78 (obj-0x28 dispatch idiom). */
void RaceProcess_endRace(RaceProcess *a0, int a1, int a2) {
    RaceProcessVt *vt;
    a0->result = a2;
    a0->phase = RACE_PHASE_END;
    a0->raceState = RACE_STATE_ENDED;
    CALL(input_setRacePadsActive)(&gInputMgr, 0);
    a0->timer = a1;
    a0->unk4EC = 0;
    a0->endCounter = 0;
    a0->msgY = 120.0f;
    if (GAMEFLOW(&gInputMgr)->nextFlowMode == 2) {
        a0->msgY = 60.0f;
    }
    a0->msgVel = 0.0f;
    a0->msgAlpha = 255;
    a0->msgFloor = 150.0f;
    vt = a0->vt;
    vt->slot[RACEVT_ON_END_RACE].fn((char *)a0 + vt->slot[RACEVT_ON_END_RACE].delta);
}

/* RaceProcess_endRaceQuiet: 41-insn struct-init. Sets a0->{0x4F8,0x4E0,0x4DC} from
 * args; calls gl_func_0(&D, 0, a2, orig_a0) (4 args, a3 = saved orig_a0);
 * post-call: writes a0->{0x4E4=a1, 0x4EC=0, 0x518=0, 0x54C=120.0f, 0x550=0.0f,
 * 0x544=0xFF, 0x554=150.0f}; if (D[0x34]==2) overwrite 0x54C with 60.0f.
 * Match key: the 0x4E4=a1 store is the FIRST post-call statement so IDO hoists
 * the a1 reload ahead of the FP setup and fills the load-delay slots with the
 * zero stores. (Prior "INSN_PATCH scheduling cap" diagnosis was wrong.) */
void RaceProcess_endRaceQuiet(RaceProcess *a0, int a1, int a2) {
    a0->result = a2;
    a0->phase = RACE_PHASE_END;
    a0->raceState = RACE_STATE_ENDED;
    CALL(input_setRacePadsActive)(&gInputMgr, 0, a2, a0);
    a0->timer = a1;
    a0->unk4EC = 0;
    a0->endCounter = 0;
    a0->msgY = 120.0f;
    if (GAMEFLOW(&gInputMgr)->nextFlowMode == 2) {
        a0->msgY = 60.0f;
    }
    a0->msgVel = 0.0f;
    a0->msgAlpha = 0xFF;
    a0->msgFloor = 150.0f;
}

/* RaceProcess_endRaceAlt - large-object initializer, near-sibling of the matched
 * RaceProcess_endRace. Variant differences from 6B80: 0x518 (1304) s32 = 60
 * (0 in 6B80); extra global-mode==3 branch calls gl_func_00000000(48); vtable
 * dispatch via obj->0x28 -> {fn@0x84, s16@0x80}. Struct map: 0x4DC=2, 0x4E0=3,
 * 0x4E4=a1, 0x4EC=0, 0x4F8=a2, 0x544=255, 0x54C f32=120/60 by mode, 0x550=0.0,
 * 0x554=150.0. */
void RaceProcess_endRaceAlt(RaceProcess *a0, int a1, int a2) {
  char *v;
  v = (char *)&a0->raceState;
  a0->result = a2;
  a0->phase = RACE_PHASE_END;
  *((int *) v) = RACE_STATE_ENDED;
  CALL(input_setRacePadsActive)(&gInputMgr, 0);
  a0->timer = a1;
  a0->unk4EC = 0;
  a0->endCounter = 60;
 if (1) { }
  a1++;
  a1--;
  a0->msgY = 120.0f;
  if ((*((int *) (((char *) (&gInputMgr)) + 0x34))) == 2)
  {
    a0->msgY = 60.0f;
  }
  if ((*((int *) (((char *) (&gInputMgr)) + 0x34))) == 3)
  {
    CALL(snd_postEvent)(48);
  }
  a0->msgAlpha = 255;
  a0->msgVel = 0.0f;
  a0->msgFloor = 150.0f;
  v = (char *)a0->vt;
  ((RaceProcessVt *)v)->slot[RACEVT_ON_END_RACE_ALT].fn((char *)a0 + ((RaceProcessVt *)v)->slot[RACEVT_ON_END_RACE_ALT].delta);
}

void RaceProcess_onEndRace(int a0) {}

void RaceProcess_onEndRaceAlt(int a0) {}

extern int gl_func_00000000();
extern int *gl_ref_00000138;

void RaceProcess_finishCourse(int a0) {
    RaceTracker *p;
    CALL(RaceProcess_endRace)(a0, 0x1E0, 0);
    p = GAMEFLOW(&gInputMgr)->dataMgr;
    p->hudFlags = 0;
}

/* Sibling of RaceProcess_endResult4_00006EE0 (matched 2026-05-08): identical 32-insn
 * template, only 4 constants differ (0xF0/4/6/6 → 0x168/3/5/5). */
void RaceProcess_endResult3_00006DF8(RaceProcess *a0) {
    RaceTracker *p;
    CALL(RaceProcess_endRaceAlt)(a0, 0x168, 3);
    a0->msgAlpha = 0;
    a0->msgFloor = 120.0f;
    p = GAMEFLOW(&gInputMgr)->dataMgr;
    p->hudFlags = 0;
    CALL(RaceTracker_sendLoseResult)(GAMEFLOW(&gInputMgr)->dataMgr, 0);
    CALL(snd_seqEndDbg_00000188)(5, 0, 0);
    CALL(snd_playSeq_000120A8)(&gInputMgr, 5, 0, 0);
}

void RaceProcess_endResult2_00006E78(int a0_passthrough) {
    RaceTracker *p;
    CALL(RaceProcess_endRaceQuiet)(a0_passthrough, 0x140, 2);
    p = GAMEFLOW(&gInputMgr)->dataMgr;
    p->hudFlags = 0;
    CALL(RaceTracker_sendDrawResult)(GAMEFLOW(&gInputMgr)->dataMgr, 0);
    CALL(snd_seqEndDbg_00000188)(5, 0, 0);
    CALL(snd_playSeq_000120A8)(&gInputMgr, 5, 3, 0);
}

/* Init function: zero-initialize a0 fields, store 120.0f at +0x554, clear
 * a child obj field, then 3 calls. Composite of int-reader-shape prologue
 * + custom init logic. */
void RaceProcess_endResult4_00006EE0(RaceProcess *a0) {
    RaceTracker *p;
    CALL(RaceProcess_endRaceAlt)(a0, 0xF0, 4);
    a0->msgAlpha = 0;
    a0->msgFloor = 120.0f;
    p = GAMEFLOW(&gInputMgr)->dataMgr;
    p->hudFlags = 0;
    CALL(RaceTracker_sendResult5_0000DE80)(GAMEFLOW(&gInputMgr)->dataMgr, 0);
    CALL(snd_seqEndDbg_00000188)(6, 0, 0);
    CALL(snd_playSeq_000120A8)(&gInputMgr, 6, 0, 0);
}

void RaceProcess_endResult5_00006F60(int a0) {
    RaceTracker *p;
    CALL(RaceProcess_endRace)(a0, 0x1E0, 5);
    p = GAMEFLOW(&gInputMgr)->dataMgr;
    p->hudFlags = 0;
}

/* Sibling of RaceProcess_endResult4_00006EE0 (just-landed): same 32-insn 4-call init
 * shape but with different field offsets and 1-arg jal at end. */
void RaceProcess_endResult4_00006F90(RaceProcess *a0) {
    RaceTracker *p;
    CALL(RaceProcess_endRaceAlt)(a0, 0xF0, 4);
    a0->msgAlpha = 0;
    a0->msgFloor = 120.0f;
    p = GAMEFLOW(&gInputMgr)->dataMgr;
    p->hudFlags = 0;
    CALL(RaceTracker_sendRetireResult)(GAMEFLOW(&gInputMgr)->dataMgr, 0);
    CALL(snd_seqEndDbg_00000188)(6, 0, 0);
    CALL(snd_playSeq_000120A8)(&gInputMgr, 6, 0, 0);
}

/* Sibling of RaceProcess_endResult2_00006E78 (matched 2026-05-08): same 26-insn 4-call
 * dispatch shape with different constants (0xF0/6/6/1 vs 0x140/2/5/3).
 * Same named-local+inline-second-deref recipe from
 * docs/IDO_CODEGEN.md#feedback-ido-v1-base-materialization-via-second-inline-deref. */
void RaceProcess_timeUp(int a0_passthrough) {
    RaceTracker *p;
    CALL(RaceProcess_endRaceAlt)(a0_passthrough, 0xF0, 6);
    p = GAMEFLOW(&gInputMgr)->dataMgr;
    p->hudFlags = 0;
    CALL(RaceTracker_sendTimeUpResult)(GAMEFLOW(&gInputMgr)->dataMgr, 0);
    CALL(snd_seqEndDbg_00000188)(6, 0, 0);
    CALL(snd_playSeq_000120A8)(&gInputMgr, 6, 1, 0);
}

/* 4-function bundle 0x7078..0x709F split via split-fragments.py 2026-05-08:
 * 2x save-arg sentinels + 2x return-1 stubs. */
void RaceProcess_onPause(int a0) {}
void RaceProcess_onCountdownStart(int a0) {}
int  RaceProcess_endTimerEnabled(int a0) { return 1; }
int  RaceProcess_altEndTimerEnabled(int a0) { return 1; }

/* RaceProcess_onRaceOver: 9-insn 1-call wrapper.
 *
 * F1 @ 0x70A0-0x70C0: gl_func_00000000(a0, -1, 0). The following
 * 0x70C4..0x70F8 empty stubs are standalone split symbols. */
extern int gl_func_00000000();
void RaceProcess_onRaceOver(int a0) {
    CALL(Process_fadeOut_0000F954)(a0, -1, 0);
}

void RaceProcess_onEndTimerDone(int a0) {}

void RaceProcess_onTimeoutExit(int a0) {}

void RaceProcess_onTimeout(int a0) {}

void RaceProcess_onAltEndTimerDone(int a0) {}

void RaceProcess_onPauseSelect(int a0) {}

/* Empty K&R one-arg function: IDO keeps the unused a0 save in the
 * jr-ra delay slot (`sw a0, 0(sp)`). */
void RaceProcess_onQuitButton(a0) int a0; {}

void RaceProcess_onLeave(int a0) {}

/* RaceProcess_leave: one-shot mode-transition dispatcher (138 insns + 4
 * trailing alt-entry stub symbols). EXACT in-unit. Old "hard cap" note was
 * wrong: blank-jal/abs-0 relocs bake the same words. Levers:
 *  - every lui/addiu &G (bootup import sym 272) is re-materialized per block;
 *    one symbol CSEs into s0 across calls, so the 0x138 read on the success
 *    path (_70fc_1), the success/third-dispatch &G (_70fc) and the second
 *    dispatch (D_00000000) are distinct abs-0 aliases (G1 and G3 share an
 *    exclusive-path hoist if they are the same symbol).
 *  - `if (a0->0x510 == 0) { ... a0->0x510 = 1; }` (not an early return):
 *    the early-return form allocates one extra t-reg (t-ring shifted by one).
 *  - func_000188 (5/6,0,0) is void (gl_func_00000000_70fcv): an int-returning
 *    blank keeps v0 busy so the following G->0x34 compare value took v1.
 *  - G->0x34 is re-read inline in each || test (no `m` local); the redundant
 *    `== 5` in the third dispatch is in the source. */
extern void gl_func_00000000_70fcv();
extern int gl_data_00000000_70fc;
extern int gl_data_00000000_70fc_1;
#define G1 ((char*)&gl_data_00000000_70fc_1)
#define G2 ((char*)&gl_data_00000000_70fc)
#define G3 ((char*)&D_00000000)
#define G4 ((char*)&gl_data_00000000_70fc)
#define G5 ((char*)&gl_data_00000000_70fc)
void RaceProcess_leave(RaceProcess *a0) {
    int *next;
    RaceTracker *obj;
    if (a0->leftOnce == 0) {
        next = (int*)a0->session;
        if (next != 0 && CALL(Backup_sessionHasNew)(next) != 0) {
            CALL(RaceTracker_sendWinResult)(GAMEFLOW(G1)->dataMgr, 0);
            XCALL(gl_func_00000000_70fcv, snd_seqEndDbg_00000188)(5, 0, 0);
            if (GAMEFLOW(G2)->nextFlowMode == 5 || GAMEFLOW(G2)->nextFlowMode == 4 || GAMEFLOW(G2)->nextFlowMode == 6) {
                if (GAMEFLOW(G2)->winVariant == 1) {
                    CALL(snd_playSeq_000120A8)(G2, 5, 2, 0);
                } else {
                    CALL(snd_playSeq_000120A8)(G2, 5, 1, 0);
                }
            } else {
                CALL(snd_playSeq_000120A8)(G2, 5, 1, 0);
            }
        } else {
            if (GAMEFLOW(G3)->nextFlowMode == 6 || GAMEFLOW(G3)->nextFlowMode == 5) {
                obj = GAMEFLOW(G3)->dataMgr;
                if (obj->entries[0].time == 0) {
                    CALL(RaceTracker_sendLoseResult)(obj, 0);
                } else {
                    CALL(RaceTracker_sendWinResult)(obj, 0);
                }
            } else {
                CALL(RaceTracker_sendWinResult)(GAMEFLOW(G3)->dataMgr, 0);
            }
            if (a0->result == 5) {
                XCALL(gl_func_00000000_70fcv, snd_seqEndDbg_00000188)(6, 0, 0);
                CALL(snd_playSeq_000120A8)(G4, 6, 0, 0);
            } else {
                XCALL(gl_func_00000000_70fcv, snd_seqEndDbg_00000188)(5, 0, 0);
                if (GAMEFLOW(G5)->nextFlowMode == 5) {
                    if (GAMEFLOW(G5)->course == 4) {
                        CALL(snd_playSeq_000120A8)(G5, 5, 0, 0);
                    } else {
                        CALL(snd_playSeq_000120A8)(G5, 5, 1, 0);
                    }
                } else if (GAMEFLOW(G5)->nextFlowMode == 4 || GAMEFLOW(G5)->nextFlowMode == 6 || GAMEFLOW(G5)->nextFlowMode == 5) {
                    CALL(snd_playSeq_000120A8)(G5, 5, 0, 0);
                } else {
                    CALL(snd_playSeq_000120A8)(G5, 5, 1, 0);
                }
            }
        }
        a0->leftOnce = 1;
    }
#undef G1
#undef G2
#undef G3
#undef G4
#undef G5
}

void RaceProcess_drawHud(int a0) {}

void RaceProcess_drawIntro(int a0) {}

void RaceProcess_drawResultExtra(int a0) {
}

void RaceProcess_drawTimeUpExtra(int a0) {}

/* RaceProcess_drawResult: per-panel HUD-element updater (0x384, 225 words) --
 * EXACT via the IDO 7.1 -O2 donor src/game_libs/game_libs_o2_7344.c
 * (REPLACE_FUNC_BODY splice, 2E290 / 68990 recipe). The switch's donor-local
 * .rodata jumptable reloc is renamed RaceProcess_drawResult_rodata and pinned to
 * bootup.uso RoData +0xDE8 (an in-unit compile cannot bake the %lo). The
 * ROM case->arm map and the load-bearing shapes are in the donor header.
 * Body below is a placeholder for the splice. */
void RaceProcess_drawResult(char *arg0) {
    volatile int ret = 0;
    if (arg0[0x544] != 0) {
        ret = 1;
    }
    (void)ret;
}

/* Empty one-arg stub; IDO keeps the caller-slot spill in the return delay. */
void RaceProcess_updateSub0(int a0) {
}

void RaceProcess_updateSub1(int a0) {}

void RaceProcess_updateSub2(int a0) {}

/* Empty one-arg stub; IDO keeps the caller-slot spill in the return delay. */
void RaceProcess_updateSub3(int a0) {
}

/* Empty one-arg stub; sibling of RaceProcess_updateSub3. */
void RaceProcess_onMessageBounce(int a0) {
}

/* RaceProcess_handleInput: per-frame state machine (577 words). EXACT 2026-11-13 (agent-h run 368, kit CDLIV)
 * through the MIPSpro 7.2.1 C++ front end donor src/game_libs/game_libs_mp72_76F0.cpp (tools/ncc721.sh),
 * REPLACE_FUNC_BODY-spliced in both trees: its 4-label switches are native 4-entry jump tables only under
 * EDG 2.29. The donor .rodata reloc is renamed RaceProcess_handleInput_rodata by the splice and pinned to the
 * game_libs RoData at +0xE04 (tables + float literals in the target's order). This placeholder's text and
 * relocs are dropped by the splice (the INCLUDE_ASM .s carried no RoData). The cfe body stays under #if 0. */
void RaceProcess_handleInput(char *self) {
    *(volatile int *)self = 0;
}


// RaceProcess_drawGui — FULL DECODE, 99.98% (220/222 words byte-exact).
// Logic verified exact: new_var-CSE for case-3 (&D base shared by the 0x34 flag
// + ((int*)&D)[idx] array load), s16 vtable offsets (0xE8/0x108/0xF0), float-trunc
// call args ((s32)*(f32*)…0x548/0x54C), frame -0x90 via the volatile pad layout
// (pu[9] upper dead-local gap → sp8C@0x8C; pl0 → sp58@0x58). Sole residual: the
// temp_lo spill lands at sp+0x48 vs target sp+0x3C — an IDO stack-spill-allocator
// slot diff (4 lw/sw words). Resists pad relayout (pads move spill+arg-spills
// together) AND the permuter (floored at score 130 — regalloc-renumber class).
/* RaceProcess_drawGui — EXACT MATCH 222/222 words (2026-07-03, agent-e w5).
 * Was 218/222: temp_lo jal-delay spill homed at sp+0x48 vs target sp+0x3C.
 * LEVER (new, cracked the documented "regalloc-renumber" cap): NAMED-SCALAR
 * DECL-ORDER SLOT MAP — non-addressed named scalars still reserve decl-order
 * homes (framesize-4, -8, ... after the addressed/volatile locals), and a
 * named var's caller-save spill goes to ITS OWN named home. Moving `temp_lo`
 * 3 decl positions later (after temp_a2/temp_a3/new_var) moved its home
 * 0x48->0x3C. Verified word-exact vs own .s on clean rebuild. */

#ifndef FW
#define FW(p, o) (*(int *)((char *)(p) + (o)))
#endif
typedef int (*GP_00007FF4)();
extern int gl_func_00000000();
void RaceProcess_drawGui(RaceProcess *arg0) {
    int *sp8C;
    volatile int pu0, pu1, pu2, pu3, pu4, pu5, pu6, pu7, pu8;
    f32 sp58[4];
    volatile int pl0;
    s32 sp3C;
    RaceProcessVt *vt;
    int *temp_a2;
    int *temp_a3;
    int *new_var;
    int temp_lo;
    f32 temp_f0;

    if (arg0->flags & RACEF_CLEAR_SCREEN) {
        sp58[0] = 0.0f;
        sp58[1] = 0.0f;
        sp58[2] = 0.0f;
        sp58[3] = 0.0f;
        CALL(gui_setFillColor_00001CA8)(&XDATA(D_00000000, port_gui_bss, 0x8), 0xFF, sp58);
        CALL(gui_fillRect)(&XDATA(D_00000000, port_gui_bss, 0x8), 0, 0x77, 0x13F, 0x78, 0x10001);
    }
    if (arg0->unk030 == 0) {
        if (arg0->flags & RACEF_SPRITES) {
            CALL(gui_setupRenderMode_0000055C)(arg0->spriteSet);
            CALL(gui_setPrimColorClamped_000008C0)(arg0->spriteSet);
        }
        switch (arg0->phase) {                   /* irregular */
        case 0:
            vt = arg0->vt;
            vt->slot[RACEVT_DRAW_INTRO].fn((char *)arg0 + vt->slot[RACEVT_DRAW_INTRO].delta);
            break;
        case 1:
            if ((arg0->flags & RACEF_SPRITES) && (arg0->flags & RACEF_COUNTDOWN_FADE)) {
                temp_lo = (s32)arg0->timer / (s32)arg0->countStep;
                if (temp_lo < 4) {
                    temp_f0 = *(f32 *)(GL_RODATA(0xE50));
                    arg0->countAlpha = arg0->countAlpha - 4;
                    arg0->scaleX = arg0->scaleX + temp_f0;
                    arg0->scaleY = arg0->scaleY + temp_f0;
                    if (temp_lo != -1) {
                        sp8C = (int *)((int)arg0 + temp_lo * (int)sizeof(RaceSprite) + 0x588);
                    } else {
                        sp8C = (int *)&arg0->sprites[0];
                    }
                    sp3C = temp_lo;
                    CALL(gui_spriteRefresh)(sp8C);
                    if ((temp_lo == 0) || (temp_lo == -1)) {
                        temp_a2 = (int *)arg0->colours.pair[RACECOL_GO][0].value;
                        temp_a3 = (int *)arg0->colours.pair[RACECOL_GO][1].value;
                        CALL(gui_setEnvPrimColor_00001EF4)(sp8C, arg0->countAlpha, temp_a2, temp_a3);
                        CALL(gui_drawSprite)(sp8C, (s32)arg0->msgX, (s32)arg0->msgY, 3);
                        if (GAMEFLOW(&XDATA(gInputMgr, port_bootup_bss, 0xF8))->nextFlowMode == 2) {
                            CALL(gui_setEnvPrimColor_00001EF4)(sp8C, arg0->countAlpha, temp_a2, temp_a3);
                            CALL(gui_drawSprite)(sp8C, (s32)arg0->msgX, (s32)(arg0->msgY + 120.0f), 3);
                        }
                    } else {
                        CALL(gui_setEnvPrimColor_00001EF4)(sp8C, arg0->countAlpha, (int)arg0->colours.pair[RACECOL_32][0].value, (int)arg0->colours.pair[RACECOL_32][1].value);
                        CALL(gui_drawSprite)(sp8C, 0xA0, 0xA0, 3);
                    }
                    if (sp3C != 0) {
                        CALL(gui_spriteRefresh)(&arg0->sprites[4]);
                        CALL(gui_setEnvPrimColor_00001EF4)(&arg0->sprites[4], 0xFF, (int)arg0->colours.pair[RACECOL_RD][0].value, (int)arg0->colours.pair[RACECOL_RD][1].value);
                        CALL(gui_drawSprite)(&arg0->sprites[4], 0xA8, 0x50, 3);
                    }
                }
            }
            break;
        case 3:
#ifndef PORT
            new_var = &D_00000000;
#else /* PC port (plan 18.8): one source site, two ROM objects: the nextFlowMode read is the GameFlow
       * (HI16 / LO16 +0x300 / +0x304 = Bss+0xF8, field 0x34 in the lw), the indexed word the result
       * table (+0x330 / +0x340 = Data+0xCF64); gcc keeps one address, so name each object */
            new_var = &XDATA(D_00000000, port_bootup_bss, 0xF8);
#endif
            if (!(arg0->flags & RACEF_NO_RESULT_MSG)) {
                vt = arg0->vt;
                vt->slot[RACEVT_DRAW_RESULT].fn((char *)arg0 + vt->slot[RACEVT_DRAW_RESULT].delta, arg0->result, (s32)arg0->msgY);
                if (GAMEFLOW(new_var)->nextFlowMode == 2) { /* new_var = the GameFlow (Bss+0xF8 Sym 272) */
                    vt = arg0->vt;
#ifndef PORT
                    vt->slot[RACEVT_DRAW_RESULT].fn((char *)arg0 + vt->slot[RACEVT_DRAW_RESULT].delta, ((int *)new_var)[arg0->result], (s32)(arg0->msgY + 120.0f));
#else
                    vt->slot[RACEVT_DRAW_RESULT].fn((char *)arg0 + vt->slot[RACEVT_DRAW_RESULT].delta, ((int *)&XDATA(D_00000000, port_bootup_data, 0xCF64))[arg0->result], (s32)(arg0->msgY + 120.0f));
#endif
                }
            }
            break;
        }
    }
    vt = arg0->vt;
    vt->slot[RACEVT_DRAW_HUD].fn((char *)arg0 + vt->slot[RACEVT_DRAW_HUD].delta);
}

extern int gl_func_00000000();
void RaceProcess_handleMsg(int a0, int *a1) {
    int v = *a1;
    if (v == 9) {
        CALL(RaceProcess_reset)(a0, a1);
    }
    CALL(Node_handleMsg)(a0, a1);
}

extern int gl_func_00000000();
int RaceProcess_loadData(RaceProcess *a0) {
    return CALL(Parms_load_00035C6C)(a0->colours.head);
}

/* RaceProcess_createReplayCam: 79-insn 5-call state swap (0x13C). EXACT in-unit.
 * ROM text relocs (bootup.uso TextReloc, shim +0x1466C): the 0x134 state
 * holder is import sym 272 at every site, but call2's a1 = `lui/addiu +0x70`
 * is a DIFFERENT symbol (sym 256 = D_007294) -- distinct abs-0 alias
 * gl_data_00000000_83cc; the old decode had it as a literal 0x70. And
 * state->0x108 takes call2's result (stored in call3's jal delay slot), not
 * call3's. call4's 3rd arg is the cached c4 (sp+0x4C), not a reload.
 * Frame 0x50: decl order = home order (c4 0x4C, x800 0x48, v70 0x44, r1 0x40,
 * obj 0x3C, r2 0x38, r3 0x34, q 0x30). ONE reused q for both flag clears
 * (v1 for both). The trailing emission-free `if (a0) {}` keeps a0 live to the
 * end so s0 is not recoloured to `side` (target spills side to sp+0x2C) --
 * the dead-if lever, IDO_CODEGEN #ce38-dead-if-lifts-constant-lr-over-passthrough-params. */
extern int gl_func_00000000();
extern int gl_data_00000000_83cc;
void RaceProcess_createReplayCam(RaceProcess *a0) {
#define STATE ((WorldRegistry *)GAMEFLOW(&gInputMgr)->game)
    int *c4;
    int x800;
    int v70;
    int r1;
    int *obj;
    int r2;
    int r3;
    int *q;
    int *side;
    c4 = (int *)STATE->players[0].rider;
    x800 = (int)SNOWB(c4)->input;
    v70 = (int)((Window *)STATE->windows[0])->camera;
    r1 = CALL(cam_create)(STATE, 1, STATE->fgRoot, c4, x800);
    a0->savedCam = STATE->windows[0];
    q = &NODE(a0->savedCam)->doFlags;
    *q &= ~8;   /* DoUpdate */
    q = &NODE(a0->savedCam)->doFlags;
    *q &= ~4;   /* DoDisplay */
    r2 = CALL(game_create3DWindow)(STATE, (char*)&gl_data_00000000_83cc + 0x70, v70);
    a0->replayCam = r2;
    STATE->windows[0] = r2;
    r3 = CALL(mem_new)(0x80);
    if (r3 != 0) {
        CALL(pad_open)(r3, 1);
    }
    obj = (int*)CALL(CamDirector_ctor)(0, r3, c4, a0->replayCam, v70, r1);
    side = (int *)STATE->bgRoot;
    CALL(list_append)(&NODE(side)->children, obj);
    if (NODE(obj)->parent != 0) NODE(obj)->unk04 = 1;
    NODE(obj)->parent = (Node *)side;
    if (a0) {}
#undef STATE
}


/* RaceProcess_restartCameras_00008510: 42-insn dispatcher. ORPHAN-MERGED 2026-05-30: the 8-byte
 * stolen prologue (`lui v1,0; lw v1,0x134(v1)` = load `state = D[0x134]`),
 * formerly the separate orphan symbol game_libs_func_00008508 at 0x8508, is now
 * the first 2 words of this function's .s (entry moved 0x8510 -> 0x8508, size
 * 0xA0 -> 0xA8). This is the LEGITIMATE form of the previously-banned
 * PROLOGUE_STEALS=8 (removed 2026-05-23): the bytes belong to this function, so
 * the boundary is corrected rather than the match faked. The C below already
 * emits the state load.
 *
 * RESIDUAL CAP (blocks byte-exact): the merged target keeps `state` in $v1 (the
 * donated register) and reuses $v0 as scratch for the state->0xC4/0xCC reloads;
 * IDO -O2 puts our named `state` local in $v0 (highest-priority pseudo -> lowest
 * reg) and the reloads in $t-regs, giving a $v0/$v1 + cascade register-renumber
 * delta (~14/42 insns). Same class as the documented permuter register-rename
 * caps; not C-reachable without the regalloc dump. Stays NM until then.
 *
 * Structure:
 *   state = D[0x134]                    (predecessor-loaded into v1)
 *   p     = state->0xC4->0x800          (saved at sp+0x20)
 *   if (arg0->0x528->0x8->0x8 != 0) {
 *       p2 = state->0xCC->0x800
 *       gl_func(p2, p2->0x34)           (call 1)
 *       t = arg0->0x528->0x8            (refetched)
 *       gl_func(p2, t->0x8, t->0x4)     (call 2)
 *   }
 *   gl_func(p, p->0x34)                 (call 3)
 *   gl_func(p, D[0x168], D[0x164], &D + 0x170)  (call 4)
 *   gl_func(arg0)                       (call 5)
 *
 * History: once promoted via PROLOGUE_STEALS=8 (banned 2026-05-23 as match-
 * faking), rolled back to NM, now orphan-MERGED (the correct boundary fix).
 * 2026-10-20 agent-g: EXACT via twin port (twin-finder.py) of the exact
 * sibling RaceProcess_restartWithTimeout: ONE function-scope `int *q` reused for the three
 * reloaded pointers (state->0xC4, state->0xCC, arg0->0x528->8) outranks
 * `state` (q = v0, state = v1) and its slot moves p2's home to 0x18. The
 * "regalloc-dump-only" residual note above is historical. */
void RaceProcess_restartCameras_00008510(RaceProcess *arg0) {
    WorldRegistry *state;
    PadCtl *p;
    int *q;

    state = (WorldRegistry *)GAMEFLOW(&gInputMgr)->game;
    q = (int *)state->players[0].rider;
    p = (PadCtl *)SNOWB(q)->input;
    if (arg0->session->ghost->data != 0) {
        PadCtl *p2;
        q = (int *)state->players[1].rider;
        p2 = (PadCtl *)SNOWB(q)->input;
        CALL(pad_reset_000673FC)(p2, p2->port);
        q = (int *)arg0->session->ghost; /* the shared q temp is load-bearing */
        CALL(pad_startPlay_00067A10)(p2, ((BackupGhost *)q)->data, ((BackupGhost *)q)->count);
    }
    CALL(pad_reset_000673FC)(p, p->port);
    CALL(pad_startRecord_000679BC)(p,
        GAMEFLOW(&gInputMgr)->replayData[0],
        GAMEFLOW(&gInputMgr)->recordCap,
        &GAMEFLOW(&gInputMgr)->replayLen[0]);
    CALL(RaceProcess_callGlobalSlot11_00008674)(arg0);
}

/* RaceProcess_restartWithTimeout — EXACT MATCH 46/46 words (2026-07-03, agent-e w5).
 * Was 28/46: whole-body temp-reg cascade (+1/+2) plus p2 spill 0x24 vs 0x20.
 * ROOT: target reuses ONE named temp (v0) for the three reloaded pointers
 * (*(state+0xC4), *(state+0xCC), *(*(arg0+0x528)+8)) and homes `state` in v1.
 * FIX: single function-scope `int *q` assigned at all 3 sites (replacing two
 * anonymous temps + the block-scoped `t`). q (6 refs) outranks state at
 * coloring -> q=v0, state=v1, t6.. cascade snaps. q's decl between `p` and
 * the block-scoped `p2` also shifts p2's named home 0x24->0x20 (q's dead
 * colored slot absorbs 0x24), fixing the spill slot. First-variant match. */
void RaceProcess_restartWithTimeout(RaceProcess *arg0, int arg1) {
    WorldRegistry *state;
    PadCtl *p;
    int *q;

    CALL(RaceProcess_restart)(arg0 KR_ARG(arg1));
    state = (WorldRegistry *)GAMEFLOW(&gInputMgr)->game;
    q = (int *)state->players[0].rider;
    p = (PadCtl *)SNOWB(q)->input;
    if (arg0->session->ghost->data != 0) {
        PadCtl *p2;
        q = (int *)state->players[1].rider;
        p2 = (PadCtl *)SNOWB(q)->input;
        CALL(pad_reset_000673FC)(p2, p2->port);
        q = (int *)arg0->session->ghost; /* the shared q temp is load-bearing */
        CALL(pad_startPlay_00067A10)(p2, ((BackupGhost *)q)->data, ((BackupGhost *)q)->count);
    }
    CALL(pad_reset_000673FC)(p, 0);
    CALL(pad_startPlay_00067A10)(p,
        GAMEFLOW(&gInputMgr)->replayData[0],
        GAMEFLOW(&gInputMgr)->replayLen[0]);
    CALL(RaceProcess_callGlobalSlot11_00008674)(arg0);
    arg0->timeout = arg1 & 0xFFFF;
}

/* RaceProcess_callGlobalSlot11_00008674: RECONSTRUCTED 2026-07-11. NOT caller-set — the prior
 * comment mis-read the prologue. The dispatcher pointer comes from a GLOBAL:
 *   v1 = &D_00000000 (absolute base, links at 0)
 *   v0 = *(int**)(v1 + 0x28)          (the object)
 * then calls obj->fn64 with arg = &D_00000000 + (s16)obj->off60 (addu reuses
 * the live base register v1). The incoming $a0 is homed to 0x18(sp) via
 * &unused. This is a global-table indirect dispatch, coaxable in C. */
int RaceProcess_callGlobalSlot11_00008674(int unused) {
    int *obj = (int *)NODE(&gSnowBoard)->vptr;
    volatile int *spill = &unused;
    (void)spill;
    return ((int (*)(int))VT_SLOT(obj, 11).fn)((int)((char*)&gSnowBoard + VT_SLOT(obj, 11).delta));
}

/* RaceProcess_raiseMessage: 31-insn FPU-only updater on two adjacent floats
 * at a0+0x550 (f550) and a0+0x54C (f54C).
 *
 * Pseudocode:
 *   f550 = *(float*)(a0 + 0x550);
 *   if (f550 < 4.0f) {
 *     f550 = (float)((double)f550 + *(double*)(&D + 0xE58));  // double step
 *     *(float*)(a0 + 0x550) = f550;
 *     f550 = *(float*)(a0 + 0x550);  // reload (IDO emit quirk)
 *   }
 *   f54C = *(float*)(a0 + 0x54C);
 *   *(float*)(a0 + 0x54C) = f54C - f550;
 *   f54C = *(float*)(a0 + 0x54C);  // reload
 *   if (f54C < 58.0f) {
 *     *(float*)(a0 + 0x54C) = f54C + f550;
 *     *(float*)(a0 + 0x550) = -(f550 / 4.0f);
 *   }
 *
 * NON_MATCHING (2 diffs): FP-temp-register swap in the double add — built does
 * ldc1->f4 (Dconst) / cvt.d.s->f6 (d550); target wants ldc1->f6 / cvt.d.s->f4.
 * IDO schedules the memory load (ldc1) first regardless of C operand order
 * (verified: operand swap is no-op, explicit-var ordering cascades 26 diffs).
 * Which f-reg holds load vs convert is IDO's FP allocator choice, not
 * C-reachable. Was INSN_PATCH'd (2 words, removed 2026-05-23). */
void RaceProcess_raiseMessage(RaceProcess *a0) {
    float four = 4.0f;
    float f550 = a0->msgVel;

    if (f550 < four) {
        double t;
        a0->msgVel = (float)(*(double*)(GL_RODATA(0xE58)) + (t = (double)f550));
        f550 = a0->msgVel;
    }

    a0->msgY = a0->msgY - f550;
    if (a0->msgY < 58.0f) {
        a0->msgY = a0->msgY + f550;
        a0->msgVel = -(f550 / four);
    }
}

/* RaceProcess_dropMessage: 54-insn (0xD8) FPU updater + conditional indirect-call.
 *   if (a0->f550 < 4.0f) a0->f550 = (float)((double)a0->f550 + D[0xE60]);
 *   a0->f54C += a0->f550;
 *   if (a0->f554 < a0->f54C) {
 *     if (2.0f < a0->f550) {
 *       v = a0->[0x28]; short off = (s16)v->[0x130];
 *       ((fn*)v->[0x134])(off + (int)a0);
 *     }
 *     a0->f54C -= a0->f550;
 *     a0->f550 = -(a0->f550 / 4.0f);
 *   }
 * MATCHED 2026-06-21: as1 FP-setup scheduler tie (lwc1 f550 vs mtc1 four
 * order swap) flipped by inlining the f550 load into the comparison
 * condition `(f550 = *...) < four`, forcing `four` materialization first.
 * Byte-exact; only reloc residue is benign &D_00000000+0xE60. */
void RaceProcess_dropMessage(RaceProcess *a0) {
    float four = 4.0f;
    float f550;
    if ((f550 = a0->msgVel) < four) {
        double t;
        a0->msgVel =
            (float)(*(double*)(GL_RODATA(0xE60)) + (t = (double)f550));
        f550 = a0->msgVel;
    }
    a0->msgY = a0->msgY + f550;
    if (a0->msgFloor < a0->msgY) {
        if (2.0f < f550) {
            RaceProcessVt *vt = a0->vt;
            vt->slot[RACEVT_ON_MESSAGE_BOUNCE].fn((char *)a0 + vt->slot[RACEVT_ON_MESSAGE_BOUNCE].delta);
            f550 = a0->msgVel;
        }
        a0->msgY =
            a0->msgY - f550;
        a0->msgVel = -(f550 / four);
    }
}

extern int gl_ref_00018770();
extern int gl_ref_000187AC();
int Parm_read_000087F4(char *a0) {
    int scratch;
    gl_ref_00018770(&scratch);
    return gl_ref_000187AC(a0 + 0x10);
}

extern int gl_ref_00018770();
int Parm_read_00008824(char *a0) {
    int scratch;
    gl_ref_00018770(&scratch);
    return gl_ref_00018770(a0 + 0x10);
}

extern int gl_ref_00018770();
extern int gl_ref_00018804();
int Parm_read_00008854(char *a0) {
    int scratch;
    gl_ref_00018770(&scratch);
    return gl_ref_00018804(a0 + 0x10);
}

extern int gl_ref_00018770();
extern int gl_ref_00018840();
int Parm_read_00008884(char *a0) {
    int scratch;
    gl_ref_00018770(&scratch);
    return gl_ref_00018840(a0 + 0x10);
}

/* Constructor pattern: alloc 64 bytes, init two fields, link into orig->field_40
 * list. 36 insns / 0x90 at -O2; 3 cross-USO jal calls, branch-likely on field
 * presence test.
 *
 * Flow (decoded from asm 2026-05-07, sibling of 0x8944/0x949C cluster):
 *   ptr = alloc(0x40);            // jal#1 with li a0, 0x40
 *   if (ptr) {
 *       init(ptr);                // jal#2 with a0=ptr
 *       ptr->field_28 = &D_00000000;
 *       ptr->field_3C = 0;
 *   }
 *   // following block runs unconditionally (alloc-fail path corrupts pointers
 *   // but original code does this — typical 90s "alloc never fails" pattern):
 *   existing = orig->field_40;
 *   if (existing) {
 *       link(ptr + 0x10, existing);   // jal#3 with a0=ptr+0x10, a1=existing
 *       if (existing->field_14 != 0) {
 *           existing->field_4 = 1;     // beql delay-likely emit pattern
 *       }
 *       existing->field_14 = ptr;
 *   }
 *   return ptr;
 *
 * (Old recipe: "INSN_PATCH for the 3 jal targets (all encoded as 0x0C000000
 * placeholders, runtime-resolved by USO loader); recipe at
 * docs/POST_CC_RECIPES.md". docs/POST_CC_RECIPES.md is DEPRECATED and
 * INSN_PATCH REMOVED 2026-05-23 as match-faking per
 * feedback_no_instruction_forcing_matches_policy.) Beql for
 * `if (existing->field_14 != 0)` followed by `existing->field_14 = ptr`
 * relies on the duplicate-store IDO emit pattern; the C body above triggers
 * it (write same value in both branches → IDO speculates via delay-likely).
 *
 * 2026-05-08 grind: tried to break the cap via frame-size lever.
 *   Built emits sp -= 0x20 (32 bytes), target sp -= 0x28 (40 bytes).
 *   v1) `char frame_pad[8]` — IDO optimized away, frame stayed 0x20.
 *   v2) `volatile char frame_pad[8]` (no access) — also elided.
 *   v3) `char frame_pad[16]` — also elided.
 *   v4) `volatile int frame_pad; frame_pad = 0;` — frame grew to 0x28
 *       BUT the added `sw zero, 28(sp)` insn doesn't match target → no
 *       net fuzzy% gain.
 * Confirms: bare-array frame-pad trick (works for 1DDC's 184-byte gap)
 * does NOT work here for the 8-byte gap. Possibly because: (a) IDO's
 * spill allocator already has 32 bytes of "slack" with the existing 4
 * spill slots, so an 8-16 byte unused array fits without growing; OR
 * (b) IDO's frame-rounding heuristic only kicks in for larger arrays.
 * 2026-05-08 follow-up: fixed GlConstructed padding so field_28/field_3C
 * compile at the target offsets, and split `ret` from `ptr` to grow the
 * C-body frame to 0x28 without an extra store. Remaining cap is the
 * ptr-in-a2 vs target ptr-in-v1 allocation plus a beqzl-vs-beqz layout in the
 * existing-link branch. Objdiff C-body score: 88.83%.
 *
 * 2026-05-08 (later): tried the sibling-port test with the matched
 * tit_ItemType_create_00002980 / mgr_ItemType_create_00003358 / arcproc_uso
 * _func_00002334 verbatim C body (`if (p == 0) return 0; init;`).
 * Result: fuzzy DROPPED 89.31% → 77.83%. Same C body that lands
 * byte-exact in titproc/mgrproc/arcproc fails here because the file-
 * context (game_libs.c is large, ~1500 functions) perturbs IDO's
 * register allocator differently. Both the if-branch direction (target
 * uses beq early-exit, mine emits bne+goto-init) and frame size
 * (target -0x28 vs mine -0x20) diverge under game_libs.c's allocator
 * state, despite identical C source. The sibling-port test recipe
 * (docs/MATCHING_WORKFLOW.md#feedback-port-matched-sibling-c-before-
 * trusting-frame-regalloc-cap-claim) has a SCALE LIMIT — it only works
 * across files of similar size+complexity. Reverted.
 *
 * 2026-05-08 Codex pass: tried `register struct GlConstructed *ret asm("$3")`
 * to force the desired v1 return carrier. IDO ignored the hard-register hint
 * for this local: build/non_matching emitted byte-identical code to the
 * baseline (ptr still lives in a2, same beqzl early-exit). Reverted.
 *
 * 2026-05-11 Codex pass: was promoted with default-build INSN_PATCH.
 * INSN_PATCH REMOVED 2026-05-23 as match-faking per
 * feedback_no_instruction_forcing_matches_policy — fn rolled back to NM-wrap.
 * C-only emit caps at the a2/v1 allocator split + one-block schedule shift
 * + 3 jal-target relocs; NATURAL CEILING, stays NM. */
struct GlConstructed {
    char pad[0x10];          /* embedded array passed to link() */
    char pad2[0x18];
    int *field_28;           /* set to &D_00000000 */
    char pad3[0x10];
    int field_3C;            /* set to 0 */
};
struct GlOrig {
    char pad[0x40];
    struct GlExisting *field_40;
};
struct GlExisting {
    int field_0;
    int field_4;
    char pad8[0x14 - 8];
    void *field_14;
};
extern int gl_func_00000000();
/* ColourItem_create_000088B4: 36-insn constructor + list insert. MATCHED 2026-06-21 via
 * masked-twin port from game_ItemType_create_00003A28 (byte-exact donor; same
 * edd_ItemType_create_000003BC family). The prior duplicate struct-typed body sat
 * in a dead #else/#ifdef branch (never compiled) while the active body produced
 * frame 0x30; the donor's flat int* form (decl-order p2,head,p1; p1-reuse-for-arg0;
 * condition re-load) gives the tight two-web spill (frame 0x28). jal targets are
 * USO placeholders (jal 0 in both build and expected) -> reloc-blind exact. */
extern int gl_func_00000000();
int *ColourItem_create_000088B4(int *arg0) {
    int *p2;
    int *head;
    int *p1;
    p1 = (int*)CALL(mem_new)(0x40);
    if (p1 != 0) {
        CALL(ColourItem_ctor)(p1);
        NODE(p1)->vptr = (char *)&ColourItem_vtbl_75A0;
        ((ColourSubItem *)p1)->unk3C = 0;
    }
    p2 = p1;
    p1 = arg0;
    head = (int*)p1[0x40 / 4];
    if ((int*)p1[0x40 / 4] != 0) {
        CALL(list_append)(&NODE(p2)->children, head);
        if (NODE(head)->parent != 0) {
            NODE(head)->unk04 = 1;
        }
        NODE(head)->parent = (Node *)p2;
    }
    return p2;
}

/* Cluster 0x8944..0x8A40 (-O0 reader templates + sandwich INCLUDE_ASM
 * for 0x8990) split out to game_libs_o0_8944.c on 2026-05-07.
 * Mid 0x8A40..0x949C split out to game_libs_mid.c. Cluster 0x949C..
 * 0x959C (-O0) in game_libs_o0_949C.c. Tail (0x959C+) in
 * game_libs_tail.c. */


/* Fragments 000097B4..00009D24 moved to game_libs_tail.c on
 * 2026-05-21; game_libs.c is truncated at 0x8944. */

/* Fragments 00037E98, 00037F10, 00037F40, 0003AA40, 0003AA5C, 00061F70
 * moved to game_libs_post.c (correct file for VRAM past 0x8944 cap of
 * game_libs.c). */

/* AnimGroup_init split off from AnimData_waitFrames bundle on
 * 2026-05-08; decompiled to C body in game_libs_post.c. */

/* AudioLoad_SetDmaHandler + AudioLoad_SetUnusedHandler split off from
 * AudioLoad_ProcessLoads bundle on 2026-05-08; decompiled to C bodies in
 * game_libs_post.c.
 *
 * AudioLoad_InitSlowLoads split off from AudioLoad_Init bundle on
 * 2026-05-08; decompiled to C body in game_libs_post.c (correct file
 * for VRAM > 0x8944 cap).
 *
 * AudioLoad_Unused2 split off from AudioLoad_GetFontSample bundle on
 * 2026-05-08; 2-insn `jr ra; nop` alt-entry stub. Stays INCLUDE_ASM in
 * game_libs_post.c (no C-decompilable form per fall-through-prologue-stub
 * doc — IDO empty void emit doesn't reproduce explicit nop in delay). */

/* pad_setSlotValue_000671D4 split off from pad_sync_0006719C bundle on 2026-05-08; decompiled in game_libs_post.c. */

/* MemTrackerMgr_clearList split off from MemTracker_walkAll bundle on 2026-05-08; decompiled in game_libs_post.c. */

/* game_libs_func_0006833{8,40,48} split off from BinFile_init_000682F8 bundle on 2026-05-08; INCLUDE_ASMs in game_libs_post.c. */

/* BinFile_set_000683C4 split off from BinFile_load_00068350 bundle on 2026-05-08; INCLUDE_ASM in game_libs_post.c. */

/* ScreenWindow_getCurBuffer_0004D014 split off from PolyShape_copyMatDL bundle on 2026-05-08; NM-wrapped in game_libs_post.c. */

/* BasePolyShape_triCentroid split off from BasePolyShape_genNormalsFast bundle on 2026-05-08; INCLUDE_ASM in game_libs_post.c. */

/* snd_InitCueQueues split off from snd_PreNMI bundle on 2026-05-08; NM-wrapped in game_libs_post.c. */

/* Window_updateAspect + Window_stub_0004EB4C split off from Window_doCommand bundle on 2026-05-08; decompiled in game_libs_post.c. */

/* ItemType_buildItems, _0003EB98 split off from ItemMgr_firstItemOfType bundle
 * 2026-05-08; decompiled in game_libs_post.c. */

/* ItemType_setNext split off from ItemMgr_registerTemplate bundle 2026-05-08;
 * decompiled in game_libs_post.c. */

/* gfx_emptyStub_00046048 split off from gfx_waitRetrace bundle 2026-05-08;
 * decompiled in game_libs_post.c. */

/* RcpTask_ctor bundle split 2026-05-08; children INCLUDE_ASM live in
 * game_libs_post.c next to the parent RcpTask_ctor. */

/* RcpTask_noUcode_fastQ bundle split 2026-05-08; children INCLUDE_ASM live in
 * game_libs_post.c next to the parent RcpTask_noUcode_fastQ. */

/* RcpTask_noUcode_NoC bundle split 2026-05-08; children INCLUDE_ASM live in
 * game_libs_post.c next to the parent RcpTask_noUcode_NoC. */

/* RcpMgr_loadTask_0004D354 bundle split 2026-05-08; children INCLUDE_ASM live in
 * game_libs_post.c next to the parent RcpMgr_loadTask_0004D354. */

/* sys_timeInFrames_00033444 split 2026-05-15; INCLUDE_ASM lives in
 * game_libs_post.c next to parent heap_tempFreeDefault_0003341C. */

/* MNode_getCMtx split 2026-05-15; INCLUDE_ASM/body lives in
 * game_libs_post.c next to parent MNode_setAnimPose. */

/* sys_getResetType split 2026-05-15; INCLUDE_ASM lives in
 * game_libs_post.c next to parent heap_tempFree. */

/* boot_stub_00066C58 / _00066C60 split 2026-05-15 from boot_entry_00066C30;
 * bodies live in game_libs_post.c next to parent. */

/* game_libs_func_0004D3D0: orphan absorbed by the C-emit of
 * RcpMgr_insertTask (doubly-linked-list insert-after-head;
 * 18/18 exact at .o size 0x48 in game_libs_post.c.o, covers vram
 * 0x4D39C-0x4D3E3 inclusive of the orphan's 5-insn range). The
 * INCLUDE_ASM here was past TRUNCATE_TEXT=0x8944 anyway (dead).
 * Same pattern as the SUFFIX_BYTES-absorbed orphan-prune variants
 * documented in MATCHING_WORKFLOW.md, except absorbed by a C body
 * instead of a Makefile recipe. */

/* game_libs_func_00066200 + game_libs_func_00066208 + game_libs_func_0002D7C0:
 * SUFFIX_BYTES-absorbed orphans whose bytes are emitted by the post-cc
 * recipes on adjacent gl_func_* entries in game_libs_post.c.o (see Makefile:
 * fs_fatal_000661D8 SUFFIX_BYTES covers 0x66200/0x66208; snd_SetPlayer2VolumeLevel
 * INSN_PATCH+SUFFIX_BYTES + snd_ResetVolumeLevels PROLOGUE_STEALS cover 0x2D7C0).
 * The .s files existed as splat-extracted symbol entries but were never used
 * by the build (game_libs.c.o gets truncated to 0x8944, dropping any
 * INCLUDE_ASM bytes placed past that offset). Removed to stop the discover
 * tool from re-surfacing them as candidates. See
 * docs/MATCHING_WORKFLOW.md#feedback-splat-orphan-duplicate-symbol-pruning
 * SUFFIX_BYTES-absorbed orphan variant. */

/* Additional SUFFIX_BYTES-absorbed orphans (same pattern as the cluster
 * above): game_libs_func_0003058C → snd_SetPlayer0Channel8IO1_30564 SUFFIX_BYTES (3 words
 * 0x3C020000,0x24420000,0x8C4E0008); game_libs_func_00030610 →
 * snd_SetPlayer0Channel8Pan_305CC SUFFIX_BYTES (3 words 0x3C020000,0x24420000,0x8C4E0000);
 * game_libs_func_0005FDC0 → math_printBounds SUFFIX_BYTES (3 words
 * 0x00000000,0x3C060000,0x8CC60000); snd_PlayPlayer2Seq →
 * snd_PlayPlayer0Seq0 SUFFIX_BYTES (3 words 0x000470C0,0x01C52021,0x2484001A).
 * All four predecessor recipes live on game_libs_post.c.o. */

/* game_libs_func_00008668: SUFFIX_BYTES-absorbed orphan ← RaceProcess_restartWithTimeout
 * (3 words 0x3C030000,0x24630000,0x8C620028 exact match). RaceProcess_restartWithTimeout
 * is in this same file with size 0xC4 covering the orphan's vram. The
 * orphan symbol pre-prune sat at .o offset 0x8944 (= TRUNCATE_TEXT cap)
 * with size 0 — its bytes never reached the link. */

/* game_libs_func_00070A0C RE-HOMED 2026-06-10: it was guMtxL2F's
 * stolen prologue (lui/mtc1 FP-constant setup) emitting at the wrong
 * position (build 0x8938); the words are now prepended to 70A14's .s
 * in post1c where ROM has them. (Relayout ledger event.) */

/* game_libs_func_0003FB64 / _0004E57C / _00051654 / _0006BA74: dead 2-word
 * orphan INCLUDE_ASMs, REMOVED agent-g run 438 with their .s files. Each one
 * emitted at game_libs.c.o offset 0x8778 (= TRUNCATE_TEXT) as a size-0 symbol,
 * so none of its bytes reached the link. The ROM words are the hoisted first
 * insns of the next function, which already emits them from source:
 *   0x3FB64 andi/multu       -> hostio_sendImage (C body in post0b.c)
 *   0x4E57C lui/addiu a1     -> Window_staticInit (game_libs_ncc_4E584.cpp donor
 *                               spliced into post0b.c.o)
 *   0x51654 lui/addiu v1     -> CMod_staticInit (game_libs_ncc_5165C.cpp donor
 *                               spliced into post0b.c.o)
 *   0x6BA74 lui 0xA460/lw    -> gl_osPiRawReadIo (game_libs_ido53_6BA7C.c donor
 *                               spliced into post1b.c.o)
 * Each of those symbols starts at the orphan's address in the linked ROM. */

/* game_libs_func_0002DF64 (ONE word, mtc1 a1,$f12 -- the stolen leading
 * insn of snd_SetPlayer0ChannelFreqScale): re-homed during the relayout session as the
 * prepended first word of snd_SetPlayer0ChannelFreqScale.s in post.c (74EFC-style
 * leading-insn boundary correction). The orphan INCLUDE that used to sit
 * here emitted at game_libs.c.o offset 0x8944 (= TRUNCATE_TEXT cap) and
 * never reached the link; removed with the re-home. */
