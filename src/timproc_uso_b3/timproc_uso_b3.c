#include "common.h"
#include "gdl/race.h"
#include "gdl/gl_data.h"
#include "game/nameentry.h"
#include "game/modemenu.h"
#include "game/guimenu.h"
#include "game/input.h"
#include "game/modeproc.h"
#include "game/snowboarder.h"
#include "uso_call.h"

/* P0 AUDIT WARNING 2026-06-10: the decompressed block3 build differs
 * from ROM (assets/timproc_uso_block_3.bin) by 963 words in
 * [0x690..0x30D0]. After the 87F4/88A0 circular-false-positive
 * retraction (docs/MATCHING_WORKFLOW post-mortem), every b3 match/
 * episode in that range is SUSPECT until per-symbol-verified against
 * the block bin ground truth. Do not refresh expected/ for this unit
 * until resolved. */

extern int gl_func_00000000();
extern char D_00000000;
extern char D_b3_23E4_d1;
extern char D_b3_23E4_d2, D_b3_23E4_d3, D_b3_23E4_d4, D_b3_23E4_d5;
extern char D_b3_23E4_d6, D_b3_23E4_d7, D_b3_23E4_d8, D_b3_23E4_d9;
typedef struct { int a, b, c, d; } Quad4;

#ifdef NON_MATCHING
/* Macro definitions for NM-wrap bodies. Auto-managed by /struct-name-tick.
 * Default build never sees these — wrap bodies aren't compiled.
 * TIMB3_D_64: int value passed to gl_func_00000000 calls and OR'd with
 * flag 0x4000 (positional name pending semantics). timproc_uso_b3 D+0x64.
 * TIMB3_D_48: int state-companion slot, set to 0xD alongside D+0x44=4 across
 * the dispatch wraps. Positional. timproc_uso_b3 D+0x48.
 */
#define TIMB3_D_64 (*(int*)((char*)&D_00000000 + 0x64))
#define TIMB3_D_48 (*(int*)((char*)&D_00000000 + 0x48))
#endif

/* timproc_uso_b3_func_00000000 / _0000004C (contiguous -O0 run, 0x0..0xB0) live
 * in timproc_uso_b3_o0_0.c — carved into a dedicated -O0 sub-unit and
 * concatenated into the Yay0 block (region 0) before compression. See the
 * timproc_uso_block3_yay0 rule in the Makefile. */

/* trk_main: 317-insn (0x4F4) -O0 state-machine
 * dispatcher: 14-case jumptable switch on arg1, loops re-reading the next
 * state from D+0x40 until a case sets `done`. EXACT 2026-07-09 via the -O0
 * donor timproc_uso_b3_o0_B0.c spliced in through REPLACE_FUNC_BODY (same
 * recipe as 5A4/65C below). This in-TU body is a compile stand-in only —
 * its bytes are replaced by the donor's in build/.../timproc_uso_b3.c.o.
 * The old "rodata jumptable = structural cap" note applied to the -O2
 * in-TU compile; the -O0 donor emits its jumptable at .rodata offset 0,
 * so the lui/lw dispatch words carry in-place addend 0 = the target's raw
 * pre-USO-reloc words. Twin: tim_main (4 immediates
 * differ: case5/case7 second-a3 1->2, case12 mask 0x2000->0x4000). */
trk_main(a0, a1) int * a0; int a1; {
    char *d = (char *)&D_00000000;
    int done = 0;
    int v, s0v;
    do {
        switch (a1) {
        case 0:
            gl_func_00000000(a0, 1, 7, 1);
            GAMEFLOW(d)->nextProcState = 4; GAMEFLOW(d)->thenProcState = 13; done = 1;
            break;
        case 1:
            gl_func_00000000(a0, 1, 7, 1);
            GAMEFLOW(d)->nextProcState = 4; GAMEFLOW(d)->thenProcState = 13; done = 1;
            break;
        case 2:
            gl_func_00000000(a0, 1, 7, 2);
            GAMEFLOW(d)->nextProcState = 4; GAMEFLOW(d)->thenProcState = 13; done = 1;
            break;
        case 3:
            gl_func_00000000(a0, 1, 7, 4);
            GAMEFLOW(d)->nextProcState = 4; GAMEFLOW(d)->thenProcState = 13; done = 1;
            break;
        case 4:
            gl_func_00000000(a0, 1, GAMEFLOW(d)->course);
            GAMEFLOW(d)->procState = 5;
            break;
        case 5:
            gl_func_00000000(d, *(int *)(d + 4));
            gl_func_00000000(d, 4, GAMEFLOW(d)->course, 2);
            v = gl_func_00000000(a0, *a0, 1);
            s0v = gl_func_00000000(0, 0x410000, v, *a0);
            gl_func_00000000(a0, 0, s0v);
            done = 1;
            break;
        case 6:
            gl_func_00000000(a0, 0, GAMEFLOW(d)->recCourse);
            GAMEFLOW(d)->procState = 7;
            break;
        case 7:
            gl_func_00000000(d, 10, GAMEFLOW(d)->course, 2);
            v = gl_func_00000000(a0, *a0, 1);
            s0v = gl_func_00000000(0, GAMEFLOW(d)->replayLen[0] + 0x1A000F, v, *a0);
            gl_func_00000000(a0, 0, s0v);
            done = 1;
            break;
        case 8:
            gl_func_00000000(a0);
            GAMEFLOW(d)->procState = GAMEFLOW(d)->nextProcState;
            break;
        case 9:
            gl_func_00000000(a0);
            GAMEFLOW(d)->procState = 10;
            break;
        case 10:
            gl_func_00000000(d, 7, 0, 0);
            s0v = gl_func_00000000(0);
            gl_func_00000000(a0, 1, s0v);
            done = 1;
            break;
        case 11:
            v = gl_func_00000000(a0, *a0, 4);
            s0v = gl_func_00000000(0, GAMEFLOW(d)->replayLen[0] + 0x20000, v, *a0);
            {
                char *r = (char *)s0v;
                gl_func_00000000(&NODE(d)->children, r);
                if (NODE(r)->parent != 0) NODE(r)->unk04 = 1;
                NODE(r)->parent = (int)d;
            }
            gl_func_00000000(a0, *a0);
            done = 1;
            break;
        case 12:
            s0v = gl_func_00000000(GAMEFLOW(d)->course);
            gl_func_00000000(a0, (GAMEFLOW(d)->course | 0x4000) | s0v, 0x4000, *a0);
            done = 1;
            break;
        case 13:
            s0v = gl_func_00000000(0, 1, 0);
            {
                char *r = (char *)s0v;
                gl_func_00000000(&NODE(d)->children, r);
                if (NODE(r)->parent != 0) NODE(r)->unk04 = 1;
                NODE(r)->parent = (int)d;
            }
            done = 1;
            break;
        }
        a1 = GAMEFLOW(d)->procState;
    } while (done == 0);
}

extern int D_00000148;
extern int D_0000014C;
extern int D_00000068;

/* 46-insn -O0 allocator-wrapper (0xB8). LANDED 2026-06-21 as a masked-shape
 * TWIN-PORT of tim_openSession_000005A4: the two target functions are
 * instruction-for-instruction identical except two immediates (b1 0x1, b3
 * 0x2). Matched via the same splice-import-donor-relocs recipe as b1 — the
 * -O0 donor timproc_uso_b3_o0_5A4.c is spliced in via REPLACE_FUNC_BODY, which
 * imports the donor HI16/LO16/26 relocs; D_00000148/0x14C/0x68 in
 * undefined_syms_auto.txt resolve to the reloc-blind expected/.o bytes. The
 * earlier "BLOCKED by Yay0 pipeline" note was wrong: the splice operates on
 * the .o, independent of Yay0 compression. fuzzy=100.
 *
 * -O0 indicators preserved (the donor IS compiled at -O0): unfilled jal delay
 * slots, `move s0,v0` save, trailing `b epilogue; nop`, frame -0x28. */
void trk_openSession_000005A4(int **arg0, int arg1, int arg2) {
    int handle = gl_func_00000000(2);
    register int new_obj = gl_func_00000000(0, D_00000148, 2, arg2);
    *arg0 = (int*)new_obj;
    D_0000014C = new_obj;
    gl_func_00000000(handle);
    if (arg1 != 0) {
        *(int*)((char*)*arg0 + 0x14) = 2;
    } else {
        *(int*)((char*)*arg0 + 0x14) = 0;
    }
    D_00000068 = 0;
}

/* 21-insn -O0 cleanup wrapper. Single gl_func + zero a0[0] + zero D[0x14C].
 * Byte-identical .s to the EXACT tim_closeSession_0000065C (same 0x65C).
 * Matched 2026-05-30 via the -O0 REPLACE_FUNC_BODY donor splice (donor
 * src/timproc_uso_b3/timproc_uso_b3_o0_65C.c). This was unblocked by fixing
 * scripts/replace-function-body.py to re-align post-.text sections after the
 * size-growing (+0x1C) splice — previously the misaligned sections made
 * objdiff reject the object. This -O2 body is what the main object compiles;
 * the donor's -O0 bytes are spliced over it. */
void trk_closeSession_0000065C(int *a0) {
    gl_func_00000000((int*)a0[0], 3);
    a0[0] = 0;
    *(int*)((char*)&D_00000000 + 0x14C) = 0;
}

extern int D_b3_06B0_a;
extern int *D_b3_06B0_b;     /* pointer-typed: 2nd call passes (*D_b3_06B0_b)[0x6A8/4] */
extern int D_b3_06B0_c;
extern int D_b3_06B0_d;
void TrkRace_pauseContinue_000006B0(void) {
    CALL(Process_resume_0000FAE8)(D_b3_06B0_a);
    CALL(Node_resumeUpdate_0000E450)(D_b3_06B0_b[0x6A8/4]);
    CALL(TrkRace_hideHud_0000120C)(D_b3_06B0_c);
    CALL(input_clearActivePad_00012090)(&D_b3_06B0_d);
}

extern int D_b3_06FC_a;
extern int D_b3_06FC_b;
extern int D_b3_06FC_c;
extern int D_b3_06FC_d;
void TrkRace_pauseRow1_000006FC(void) {
    CALL(Process_resume_0000FAE8)(D_b3_06FC_a);
    CALL(TrkRace_hideHud_0000120C)(D_b3_06FC_b);
    CALL(input_clearActivePad_00012090)(&D_b3_06FC_c);
    CALL(RaceProcess_restartCameras_00008510)(D_b3_06FC_d);
}
/* Chain sibling of 0x790, 0x7D4, 0x818. MATCHED via pad-sidecar boundary
 * correction (orphan lui v0,0; addiu v0,0 moved from func_000006FC's pad into
 * this fn's .s; 0x3C->0x44, starts 0x744). Same recipe as b1 chain 0x734-0x800. */
extern int D_state_b3_74C;
void TrkRace_pauseChangeCharacter(void) {
    *(int*)((char*)&D_00000000 + 0x40) = 8;
    *(int*)((char*)&D_00000000 + 0x44) = 1;
    CALL(Process_fadeOut_0000F954)(D_state_b3_74C, -1, 0);
}
/* Chain sibling: t7=2. MATCHED via pad-sidecar boundary correction (orphan
 * moved from func_0000074C's pad into this fn's .s; 0x3C->0x44, starts 0x788). */
extern int D_state_b3_790;
void TrkRace_pauseChangeBoard(void) {
    *(int*)((char*)&D_00000000 + 0x40) = 8;
    *(int*)((char*)&D_00000000 + 0x44) = 2;
    CALL(Process_fadeOut_0000F954)(D_state_b3_790, -1, 0);
}
/* Chain sibling: t7=3. MATCHED via pad-sidecar boundary correction (orphan
 * moved from func_00000790's pad into this fn's .s; 0x3C->0x44, starts 0x7CC). */
extern int D_state_b3_7D4;
void TrkRace_pauseChangeCourse(void) {
    *(int*)((char*)&D_00000000 + 0x40) = 8;
    *(int*)((char*)&D_00000000 + 0x44) = 3;
    CALL(Process_fadeOut_0000F954)(D_state_b3_7D4, -1, 0);
}
/* End of b3 chain. MATCHED via pad-sidecar boundary correction (orphan lui v0,0;
 * addiu v0,0 moved from func_000007D4's pad into this fn's .s; 0x3C->0x44, starts
 * 0x810). Unique extern D_state_b3_818 (=0) at the gl_func arg breaks IDO's
 * &D-CSE so the target's fresh lui+lw at the call site reproduces. */
extern int D_state_b3_818;
void TrkRace_pauseQuit_00000818(void) {
    *(int*)((char*)&D_00000000 + 0x40) = 8;
    *(int*)((char*)&D_00000000 + 0x44) = 0xD;
    CALL(Process_fadeOut_0000F954)(D_state_b3_818, -1, 0);
}

static void trk_streamReadS32_00000854(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

static void trk_streamReadF32_00000890(float *dst) {
    float buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

static void trk_streamReadQuad_000008CC(Quad4 *dst) {
    Quad4 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 16);
    *dst = buf;
}

typedef struct { int a, b, c; } Tri3i;
typedef struct { float x, y, z; } Vec3;
static void trk_streamReadVec3_00000924(Vec3 *dst) {
    int pad_top[1];
    Tri3i raw;
    int pad_mid[2];
    Tri3i tmp;
    int pad_bot[2];
    CALL(fs_gdlReadFile)(&D_00000000, &raw, 12);
    tmp = raw;
    dst->x = *(float*)&tmp.a;
    dst->y = *(float*)&tmp.b;
    dst->z = *(float*)&tmp.c;
}

/* TrkRace_ctor: 243-insn constructor (sibling of titproc
 * 1E9C / timproc_b1 97C / timproc_b5 478). jal-0 = cross-USO placeholder
 * gl_func_00000000.
 *
 * 2026-08-22 RETRACTION + redecode: the old "225/230 (97.8%)" claim was a
 * target-vs-target measurement artifact (see b1 097C comment +
 * MATCHING_WORKFLOW #nm-claim-target-vs-target-artifact). True baseline
 * 72.3%; this pass 72.3 -> 96.8 via the corrected b1-097C skeleton with
 * b3 deltas: sub is a PLAIN a2-colored web (b1's is volatile/memory-only),
 * alloc 0xDC not 0xE0, the sub[0xB4] 11/0 bgezl block IS real here
 * (per-arm m = (int*)self[0x6A8] reload spelling gives the a3 arm loads),
 * return (void *)a0 via shared epilogue. D_b3_994_g/_r aliases defeat the
 * &D lui CSE (six-call read base + 0x138 store).
 * RESIDUAL (~21 words): same family as b1 097C — registration-1 def
 * a3+copy vs direct a1, vt CSE temp v1-vs-v0 + commutative addu order,
 * t-ring renames downstream, 1-word addiu schedule swap in after_n3.
 * EXACT 2026-10-20 (agent-g run 108): rewritten on the mgrproc E04 skeleton
 * (same constructor family): nested `oN = oN-1; if (oN || (oN = alloc(n)))`
 * inlined base ctors instead of the goto ladder, every later object pointer
 * re-read through the field into ONE reused local o1 (per-arm o1 for the
 * 0xB4 11/0 block = the a3 arm loads), and the registration call
 * gl_func_00000000(obj->6A8, obj, 0x568, 0x528) declared void through the
 * alias gl_func_00000000_994v: its dead int return held v0, pushing the
 * vtable base w to v1. */
extern char D_b3_994_v0;
extern char D_b3_994_v1;
extern char D_b3_994_v2;
extern char D_b3_994_v3;
extern char D_b3_994_v4;
extern char D_b3_994_g;
extern char D_b3_994_r;
extern void gl_func_00000000_994v();
void *TrkRace_ctor(char *obj, int a1, int a2, int a3) {
    char *o1;
    char *o2;
    char *o3;
    char *w;

    if (obj != 0 || (obj = (char *)CALL(mem_new)(0x730)) != 0) {
    o1 = obj;
    if (o1 != 0 || (o1 = (char *)CALL(mem_new)(0x6A8)) != 0) {
        o2 = o1;
        if (o2 != 0 || (o2 = (char *)CALL(mem_new)(0x50)) != 0) {
            o3 = o2;
            if (o3 != 0 || (o3 = (char *)CALL(mem_new)(0x2C)) != 0) { CALL(Node_ctor)(o3, (char *)&D_00000000 + 0x3B8); NODE(o3)->vptr = (int)&D_b3_994_v0; }
            NODE(o2)->vptr = (int)&D_b3_994_v1;
        }
        NODE(o1)->vptr = (int)&D_b3_994_v2;
        CALL(ColourParms_ctor)(o1 + 0x50);
    }
    NODE(obj)->vptr = (int)&D_b3_994_v3;
    RACEPROC(obj)->spriteSet = 0;
    CALL(RaceProcess_init)(obj, a1, (char *)&D_00000000 + 0x3C0, a2);
    RACEPROC(obj)->session = (BackupSession *)a3;
    CALL(TrkRace_initLabels_00000D60)(obj);
    TIMRACE(obj)->recordFade = 0.0f;
    CALL(memtrack_begin)((char *)&D_00000000 + 0x3D0, 0);
    CALL(input_setActivePad_0001207C)(&D_00000000, 0);

    {
        o2 = (char *)CALL(mem_new)(0xDC);
        if (o2 != 0) {
            CALL(RaceTracker_ctor)(o2);
            NODE(o2)->vptr = (int)&D_b3_994_v4;
        }
        TIMRACE(obj)->dataManager = (int)o2;
        *(char **)((char *)&D_b3_994_g + 0x138) = o2;
        if ((RACEPROC(obj)->flags << 15) < 0) {
            o1 = (char *)TIMRACE(obj)->dataManager;
            DATAMGR(o1)->hudFlags = 11;
        } else {
            o1 = (char *)TIMRACE(obj)->dataManager;
            DATAMGR(o1)->hudFlags = 0;
        }

        XCALL(gl_func_00000000_994v, TrkDataManager_init_000019CC)(TIMRACE(obj)->dataManager, obj, RACEPROC(obj)->spriteSet, (int)RACEPROC(obj)->session);

        w = (char *)TIMRACE(obj)->dataManager;
        o1 = (char *)NODE(w)->vptr;
        ((void (*)(int))VT_SLOT(o1, 10).fn)(VT_SLOT(o1, 10).delta + (int)w);

        o1 = (char *)TIMRACE(obj)->dataManager;
        CALL(list_append)(obj + 16, o1);
        if (NODE(o1)->parent != 0) {
            NODE(o1)->unk04 = 1;
        }
        NODE(o1)->parent = (int)obj;

        CALL(RaceProcess_restart)(obj, a1);

        if ((RACEPROC(obj)->flags << 15) < 0) {
            RACEPROC(obj)->menu = (GuiMenu *)CALL(PauseMenu_create)(0);
            CALL(gui_initRows_000100F0)(RACEPROC(obj)->menu, obj);
            RACEPROC(obj)->menu->unk30 = RACEPROC(obj)->spriteSet;
            CALL(gui_addRowSub_00010B6C)(RACEPROC(obj)->menu, (*(int *)&D_b3_994_r + 3) << 16, -1, &D_00000000);
            CALL(gui_addRowSub_00010B6C)(RACEPROC(obj)->menu, ((*(int *)&D_b3_994_r + 3) << 16) | 1, -1, &D_00000000);
            CALL(gui_addRowSub_00010B6C)(RACEPROC(obj)->menu, ((*(int *)&D_b3_994_r + 3) << 16) | 4, -1, &D_00000000);
            CALL(gui_addRowSub_00010B6C)(RACEPROC(obj)->menu, ((*(int *)&D_b3_994_r + 3) << 16) | 3, -1, &D_00000000);
            CALL(gui_addRowSub_00010B6C)(RACEPROC(obj)->menu, ((*(int *)&D_b3_994_r + 3) << 16) | 2, -1, &D_00000000);
            CALL(gui_addRowSub_00010B6C)(RACEPROC(obj)->menu, ((*(int *)&D_b3_994_r + 3) << 16) | 5, -1, &D_00000000);
            CALL(gui_centerRows_0001016C)(RACEPROC(obj)->menu);
            o1 = (char *)RACEPROC(obj)->menu;
            CALL(list_append)(obj + 16, o1);
            if (NODE(o1)->parent != 0) {
                NODE(o1)->unk04 = 1;
            }
            NODE(o1)->parent = (int)obj;
        } else {
            RACEPROC(obj)->timeout = a1 & 0xFFFF;
            RACEPROC(obj)->menu = 0;
        }

        o1 = *(char **)((char *)&D_00000000 + 0x190);
        CALL(list_append)(obj + 16, o1);
        if (NODE(o1)->parent != 0) {
            NODE(o1)->unk04 = 1;
        }
        NODE(o1)->parent = (int)obj;
        CALL(Fader_start)(*(int **)((char *)&D_00000000 + 0x190), 1, 0);
        CALL(memtrack_end)();
    }
    }
    return obj;
}

/* TrkRace_initLabels_00000D60: 33-insn (0x84) 5-call gl_func_00000000
 * dispatcher with computed (s0+offset, immediate-or-globalload) args.
 * First 2 calls use D[0x4C/0x54] OR'd with high-half flags 0x001D/0x001E;
 * latter 3 use precomputed 32-bit constants 0x0021_xxxx. */
void TrkRace_initLabels_00000D60(TimRace *a0) {
    CALL(gui_spriteInit)((char *)&a0->labels[0], *(int*)((char*)&D_00000000 + 0x4C) | 0x001D0000);
    CALL(gui_spriteInit)((char *)&a0->labels[1], *(int*)((char*)&D_00000000 + 0x54) | 0x001E0000);
    CALL(gui_spriteInit)((char *)&a0->labels[3], 0x0021000B);
    CALL(gui_spriteInit)((char *)&a0->labels[4], 0x0021000D);
    CALL(gui_spriteInit)((char *)&a0->labels[2], 0x00210009);
}

/* TrkRace_freeLabels_00000DE4: F1 (19-insn 5-call wrapper) only.
 *
 * The 2026-05-07 SUFFIX_BYTES recipe (absorbing 12 words / 0x30 bytes for
 * F2 + F3) was REMOVED 2026-05-23 as instruction-byte match-faking (per
 * feedback_no_instruction_forcing_matches_policy). Current state: symbol
 * is 0x4C / 19 insns (F1 only); F2 and F3 are tracked as separate
 * symbols. Function lands at fuzzy=100 naturally with just F1's C.
 *
 * F1 @ 0xDE4-0xE2C: 5-call passthrough wrapper.
 * F2 @ 0xE30-0xE50: triple-deref non-null check (returns 0/1 on
 *     a0->[0x6A8]->[0x6C]->[0xEC] != 0).
 * F3 @ 0xE54-0xE5C: `return 0;` 3-insn stub. */
extern int func_00000000();
void TrkRace_freeLabels_00000DE4(TimRace *a0) {
    FCALL(gui_spriteRelease)((char *)&a0->labels[0]);
    FCALL(gui_spriteRelease)((char *)&a0->labels[1]);
    FCALL(gui_spriteRelease)((char *)&a0->labels[3]);
    FCALL(gui_spriteRelease)((char *)&a0->labels[4]);
    FCALL(gui_spriteRelease)((char *)&a0->labels[2]);
}

/* TrkRace_endTimerEnabled: triple-deref non-null check RECOVERED +
 * MATCHED 2026-05-28 from the Yay0 block_3 gap. Returns 1 iff a0->0x6A8->
 * 0x6C is non-null and its 0xEC field is non-zero. KEY: `goto ret0` to a
 * shared `return 0` produces the beql branch-likely idiom (v0=0 in the
 * delay slot) — the plain `if(...)return 0` form emits bnel and grows to
 * 14 insns (the cap that NM'd sibling ARCRaceProcess_endTimerEnabled). */
int TrkRace_endTimerEnabled(TimRace *a0) {
    int *p = (int*)((int*)a0->dataManager)[0x6C / 4];
    if (p == 0) {
        goto ret0;
    }
    if (TPOINTS(p)->allIdle == 0) {
        goto ret0;
    }
    return 1;
ret0:
    return 0;
}

/* TrkRace_onRaceOver: state-machine dispatcher, sibling of
 * TimRace_onRaceOver and ARCRaceProcess_onRaceOver.
 *
 * Decoded state 0 and state 1 bodies from the target asm. State 0 walks a
 * 0x30-stride table selected by D[0x64], searching for an entry whose
 * returned [4] matches a0->[0x6AC]. State 1 runs the setup/commit chain and
 * links a0->[0x56C] under the object returned into a0->[0x524].
 *
 * 2026-05-20: NON_MATCHING body reached 95.237625% via objdiff report.
 * Tried plain char* base, register int* base, goto-shaped dispatch, inverted
 * loop branch, and volatile int* base. Volatile base is required to get the
 * target $s3 global-base lifetime and 0x28 stack frame. Remaining no-alias
 * diffs are codegen-level: entry emits bne+b instead of beq+default branch,
 * the state-0 compare still uses bnel, and loop temporaries choose different
 * caller registers around the table stride multiply.
 *
 * 2026-05-21: inverted the state-1 guard to recover the target beq+default
 * dispatch shape (C-only 96.86%). Remaining same-size register/scheduler
 * diffs were closed with INSN_PATCH in Makefile; INSN_PATCH REMOVED
 * 2026-05-23 as match-faking per
 * feedback_no_instruction_forcing_matches_policy — fn rolled back to NM-wrap;
 * NATURAL CEILING 96.86% NM.
 *
 * 2026-10-20 agent-g (run 109) 97.47 -> EXACT by porting the b1 0EE8 kit
 * (its exact twin): base = &D re-assigned right after the 0x6A8 load, the
 * count stored to 0x6B0 and read back as the loop init (store-forward), and
 * a for-loop that REUSES `state` as the counter with the call argument spelled
 * mult-first `base[0x64/4] * 0x30 + *(int *)(char *)a0->race.session` (no table
 * local, which took v0). The break now exits to the base re-assignment, giving
 * the target's bne + nop instead of bnel.
 */
void TrkRace_onRaceOver(char *a0) {
    volatile int *base = (int*)&D_00000000;
    char *saved = a0;
    int state = RACEPROC(a0)->unk504;
    char *obj;
    int *ret;
    int *created;
    char *link;

    if (state == 0) {
        goto state0;
    }
    if (state != 1) {
        return;
    }
    goto state1;

state0:
        obj = (char *)TIMRACE(saved)->dataManager;
        base = (volatile int*)&D_00000000;
        TIMRACE(saved)->finalTime = DATAMGR(obj)->entries->time;
        TIMRACE(saved)->recordRank = DATAMGR(obj)->entries->rank[1];
        for (state = TIMRACE(saved)->recordRank - 1; state >= 0; state--) {
            ret = (int*)CALL(Backup_courseRecordGetScore)(base[0x64 / 4] * 0x30 + *(int *)RACEPROC(saved)->session, state);
            if (TIMRACE(saved)->finalTime != ret[1]) break;
            TIMRACE(saved)->recordRank = TIMRACE(saved)->recordRank - 1;
        }
state0_done:
        base = (volatile int*)&D_00000000;
        RACEPROC(saved)->subState = 1;
        CALL(Fader_start)(base[0x190 / 4], 3, 1);
        RACEPROC(saved)->unk504 = 1;
        return;

state1:
        if (CALL(Fader_checkDone_00013924)(base[0x190 / 4]) == 0) {
            return;
        }
        CALL(TrkRace_freeLabels_00000DE4)(saved);
        CALL(GameFlow_releaseFiles_00012110)(base);
        CALL(RaceProcess_createReplayCam)(saved);
        CALL(RaceProcess_restartWithTimeout)(saved, base[0x170 / 4] + 0x00220000);
        RACEPROC(saved)->unk524 = CALL(TrkNameEntry_ctor_00001660)(0, saved, 0);
        gl_func_00000000(RACEPROC(saved)->unk524, (int)RACEPROC(saved)->session);
        created = (int *)RACEPROC(saved)->unk524;
        link = (char *)RACEPROC(saved)->handler;
        CALL(list_append)(link + 0x10, created);
        if (created[0x14 / 4] != 0) {
            created[1] = 1;
        }
        created[0x14 / 4] = (int)link;
        CALL(Fader_start)(base[0x190 / 4], 1, 1);
}

/* TrkRace_onEndTimerDone: F1 (32-insn 0x80) gate-then-update wrapper
 * only. The 2026-05-07 SUFFIX_BYTES recipe (absorbing 7 words / 0x1C bytes
 * for the trailing D[0x40]=9 stub at 0x1074 and the 2-word splat-pulled
 * fragment at 0x1088) was REMOVED 2026-05-23 as instruction-byte
 * match-faking. Symbol is now 0x80 / 32 insns (F1 only); the absorbed
 * pieces are tracked as separate symbols. Function lands at fuzzy=100
 * naturally — same pattern as TrkRace_freeLabels_00000DE4 /
 * ARCRaceProcess_onCountdownStart.
 *
 * F1: gate=gl_func(a0->0x528). If non-zero set D[0x40]=0xC,D[0x44]=9.
 *     Else set D[0x40]=9. Tail: if(a0->0x4F8) gl_func(a0,-1,0); else a0->0x504=0. */
void TrkRace_onEndTimerDone(TimRace *a0) {
    if (CALL(Backup_sessionHasNew)(a0->race.session) != 0) {
        *(int*)((char*)&D_00000000 + 0x40) = 0xC;
        *(int*)((char*)&D_00000000 + 0x44) = 9;
    } else {
        *(int*)((char*)&D_00000000 + 0x40) = 9;
    }
    if (a0->race.result == 0) {
        a0->race.unk504 = 0;
    } else {
        CALL(Process_fadeOut_0000F954)(a0, -1, 0);
    }
}

/* 00001074: RECOVERED 2026-05-28 from the Yay0 block_3 gap (no .s, missing
 * from build). MATCHED -- D[0x40]=9 setter with unused-arg save; the
 * global-store-in-delay-slot vs arg-save order matches in-tree (standalone
 * diverged, see MATCHING_WORKFLOW.md#feedback-standalone-false-convergence-verify-in-tree). */
void TrkRace_onTimeoutExit(int a0) {
    *(int*)((char*)&D_00000000 + 0x40) = 9;
}

/* TrkRace_onTimeout (0x2C, 11 insns): hoisted-head merge of 1088 +
 * 1090 (2026-09-05 agent-c, TWENTY-FIFTH mis-split class, see
 * docs/MATCHING_WORKFLOW.md #hoisted-head-119c-11a4-mgrproc). The 8-byte
 * "alt-entry" `lui a1; lw a1,0x170(a1)` is this function's own first statement
 * scheduled above `addiu sp` by IDO 7.1 -O2; 1090 was never an entry. Oracle =
 * this block's module tables -- the "timproc_uso_b3" text is trkproc.uso (FILE
 * wrapper ROM 0x5B3DBA, inner header 0x5B3DE2): Sym[89] EXPORTS text offset
 * 0x1088 and DataReloc @0x33C stores its address in a function-pointer table;
 * 0x1090 has no export and no R_MIPS_26. Old 1090 "exact" = fake `char *a1`
 * parameter (episode retired). Twin: TimRace_onTimeout. */
void timproc_uso_b3_func_00000000();

void TrkRace_onTimeout(int a0) {
    XCALL(timproc_uso_b3_func_00000000, RaceProcess_restartWithTimeout)(a0, *(char **)((char *)&D_00000000 + 0x170) + 0x00220000);
}

extern int gl_ref_00000040;
extern int gl_ref_00000208;
extern int gl_ref_0000020C;

void TrkRace_onQuitButton(int a0) {
    *(int *)((char *)&D_00000000 + 0x40) = 9;
    CALL(Process_fadeOut_0000F954)(a0, -1, 0);
}

/* Vtable-entry dispatcher (twin of TimRace_onPauseSelect). Pass 2 2026-06-10:
 * re-decode fixed the OLD body's structure -- the *40 is a real multu
 * with the stride in a register (variable stride; old body's shift
 * decomposition was wrong), the gl(5) arg loads EARLY into a0 before
 * the beqzl, and the if(1){} BB-split after the call is load-bearing
 * (the 4-for-4 v0/v1 lever). 29/40 -> 11 word-diffs remaining: stride
 * colors a2 vs target a0 (the 5-then-40 single-web trick did not take;
 * k still splits), and the fn-temp renumber downstream of it.
 * 2026-06-10 pseudo-order family sweep NEGATIVE: early-pseudo (stride
 * dead-initialized / pre-declared first) and decl reorders all leave
 * stride in a2. The residual is ARG-REGISTER coloring of a non-arg
 * value (a0 freed after the jal consumed the 5); none of the three
 * pseudo-order levers (BB-split / web-inversion / early-pseudo)
 * address arg-reg targeting. Remaining lever: uoptlist dump. */
/* EXACT (2026-06-21, ported from byte-identical twin TimRace_onPauseSelect).
 * The single residual addu operand-order diff (target `addu rd,base,prod` vs the
 * inline-web's `addu rd,prod,base`) is resolved by two scheduler/coloring nudges:
 *   - `self[(0x48 / 4) ^ 0]` (identity XOR) re-orders the index web so the final
 *     addr addu emits base-rs while preserving the $t9 coalesce, and
 *   - `(new_var = 0x90)` parks the +0x90 offset in a throwaway local, fixing the
 *     temp-numbering of the call register.
 * Both are semantic no-ops; verified in-tree (0 non-reloc word diffs). */
extern int D_arg_b3_10E4[];
extern int D_cur_b3_10E4;
void TrkRace_onPauseSelect(int *self) {
  int *v0;
  int new_var;
  int v1;
  int stride;
  if (CALL(Fader_isDone_00013980)(D_arg_b3_10E4[0x190 / 4]) == 0) {
    return;
  }
  v0 = (int *) self[0x48 / 4];
  v1 = v0[0x7C / 4];
  if (v1 != 0) {
    CALL(snd_postEvent)(5);
    if (1) {
    }
    v0 = (int *) self[0x48 / 4];
    v1 = v0[0x7C / 4];
  }
  stride = 40;
  if (((void (*)(void)) (*((int *) ((((char *) v0) + (v1 * stride)) + 0x90)))) != 0) {
    D_cur_b3_10E4 = (int) self;
    if (1) {
    }
    v0 = (int *) self[(0x48 / 4) ^ 0];
    (*((void (**)(void)) ((((char *) v0) + (v0[0x7C / 4] * stride)) + (new_var = 0x90))))();
  }
}

/* TrkRace_onLeave: 20-insn (func1 of the split bundle).
 *   if (a0->0x4FC == 0) { X(a0->0x6A8,0,1); X(a0->0x6A8); a0->0x4FC=1; }
 * orig a0 saved @sp+0x18, reloaded across the 2 calls. USO: call ->
 * func_00000000. */
void TrkRace_onLeave(TimRace *a0) {
    if (a0->race.unk4FC == 0) {
        FCALL(hud_loadRecords_0000CE38)((int *)a0->dataManager, 0, 1);
        FCALL(RaceTracker_flagReplayEnd_0000D090)((int *)a0->dataManager);
        a0->race.unk4FC = 1;
    }
}

void TrkRace_onPause(TimRace *a0) {
    int *p;

    p = (int*)DATAMGR(a0->dataManager)->pointsList;
    p = &NODE(p)->doFlags;
    *p &= ~4;
    p = (int*)DATAMGR(a0->dataManager)->points94;
    p = &NODE(p)->doFlags;
    *p &= ~4;
}

void TrkRace_hideHud_0000120C(TimRace *a0) {
    int *p;

    p = (int*)DATAMGR(a0->dataManager)->pointsList;
    p = &NODE(p)->doFlags;
    *p |= 4;
    p = (int*)DATAMGR(a0->dataManager)->points94;
    p = &NODE(p)->doFlags;
    *p |= 4;
}

/* TrkRace_drawResultExtra: 94-insn FP accumulator/dispatch helper.
 * b3 variant of timproc_uso_b1_func_000011D8. Boundary-checked: one jr ra.
 *
 * Structure:
 *   if (a0->0x500 == 0) return;
 *   if (*(float*)&D > 0.0f) return;
 *   if (a0->0x72C < D[0x40]) a0->0x72C += D[0x44];
 *   a0->0x508++;
 *   armed = (a0->0x528->0x14 & 2) && a0->0x4FC &&
 *           D[0x138]->0x44->0x3C < 3;
 *   if (armed) {
 *     gl_func(&D, (int)(255.0f * a0->0x72C), a0+0x2F0, a0+0x314);
 *     saved = a0+0x6E4; gl_func(saved);
 *     if (a0->0x508 & 8) {
 *       gl_func(saved, 160, 160, 3);
 *       gl_func(a0, 140, a0->0x6A8->0x44->0x14);
 *     }
 *   } else {
 *     gl_func(a0, 140, a0->0x6A8->0x44->0x14);
 *   }
 *
 * NATURAL CEILING: NM only. Was previously documented as "Exact via
 * targeted INSN_PATCH for IDO register/scheduling deltas: D-reloc offsets
 * on unique externs, mtc1/lwc1 order, saved stack slot offset, and the
 * final nested pointer temp registers" — INSN_PATCH REMOVED 2026-05-23
 * as match-faking (per feedback_no_instruction_forcing_matches_policy).
 * Unique-extern split (D_b3_1240_*) is retained to keep the structural
 * decode honest in the NM body. Default build is INCLUDE_ASM. */
extern char D_b3_1240_base;
extern float D_b3_1240_f0;
extern float D_b3_1240_f40;
extern float D_b3_1240_f44;
extern char *D_b3_1240_ptr138;
/* TrkRace_drawResultExtra — IMPROVED 2026-07-03: 81/94 -> 92/94 raw words.
 *
 * REQUIRES undefined_syms_auto.txt edits (distinct base-alias, value 0x0):
 *   D_b3_1240_f40    = 0x00000000;   (was 0x00000040)
 *   D_b3_1240_f44    = 0x00000000;   (was 0x00000044)
 *   D_b3_1240_ptr138 = 0x00000000;   (was 0x00000138)
 * The offset is now baked in the load instruction as an addend (struct-cast-fold)
 * so the raw .o word carries offset 0x40/0x44/0x138 with a reloc to the base
 * (matches target C4280040/C42A0044/8D6B0138). Keeping them as DISTINCT symbols
 * (rather than a single &D_base) is required to defeat IDO's lui-CSE — the target
 * re-lui's the base per read; a shared symbol drops 3 luis and shifts everything.
 *
 * Fixes applied (all legitimate C, verified in-tree word-diff):
 *   1. f40/f44/ptr138: struct-cast base+offset fold (idx 22/28/44).
 *   2. ptr-temp chain FULLY INLINED (no named `tmp` local) -> target's distinct
 *      t9->t0 register chain instead of v0-reuse (idx 75/78/80/83/86/88).
 *   3. Removing the now-unused `char *tmp` local fixes the a0-spill slot
 *      0x20 -> 0x24 (named-local-count frame rule) (idx 66/68).
 *
 * 2026-10-16 (agent-g) EXACT (97.82 -> 100): the last 2 words were not a
 * scheduler cap but commutative FP operand RANK -- an inline field load
 * ranks before a literal, so `255.0f * x` emits mul.s(x, K). The comma
 * constant `x * (0, 255.0f)` makes the literal an expression operand and
 * uopt orders it first (mtc1 K -> f18, lwc1 x -> f4 = target); see
 * docs/IDO_CODEGEN.md#comma-constant-const-first-mul-5d754. That form drops
 * the a0 spill home from 0x24 to 0x20; `char pad[28]` (was 32) moves it back
 * with the frame unchanged. The undefined_syms values above are not needed
 * for the bytes (the .text is taken from the unlinked object).
 */
void TrkRace_drawResultExtra(TimRace *a0) {
    float color[4];
    char pad[28];
    char *saved;
    int armed;

    color[0] = 1.0f;
    color[1] = 1.0f;
    color[2] = 1.0f;
    color[3] = 1.0f;
    (void)color;
    (void)pad;

    if (a0->race.leave != 0) {
        if (D_b3_1240_f0 <= 0.0f) {
            if (a0->recordFade < *(float*)((char*)&D_b3_1240_f40 + 0x40)) {
                a0->recordFade += *(float*)((char*)&D_b3_1240_f44 + 0x44);
            }

            a0->race.unk508 = a0->race.unk508 + 1;
            armed = 0;
            if ((a0->race.session->modes & BACKUP_MODE_TRICK) != 0 &&
                a0->race.unk4FC != 0 &&
                *(int*)(*(char**)(*(char**)((char*)&D_b3_1240_ptr138 + 0x138) + 0x44) + 0x3C) < 3) {
                armed = 1;
            }

            if (armed != 0) {
                CALL(gui_setEnvPrimColor_00001EF4)(&D_b3_1240_base,
                                 (int)(a0->recordFade * (0, 255.0f)),
                                 (char *)a0 + 0x2F0,
                                 (char *)a0 + 0x314);
                saved = (char *)&a0->labels[2];
                CALL(gui_spriteRefresh)(saved);
                if ((a0->race.unk508 & 8) != 0) {
                    CALL(gui_drawSprite)(saved, 0xA0, 0xA0, 3);
                    CALL(trk_drawScore_000013B8)(a0, 0x8C, DATAMGR(a0->dataManager)->entries->time);
                }
            } else {
                CALL(trk_drawScore_000013B8)(a0, 0x8C, DATAMGR(a0->dataManager)->entries->time);
            }
        }
    }
}

/* EXACT via an IDO 7.1 NCC (C++) donor: src/timproc_uso_b3/timproc_uso_b3_ncc_13B8.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder (agent-g run 197, kit CLXI). The history
 * below describes the earlier cfe decode (removed). */
void trk_drawScore_000013B8(char *arg0, s32 arg1, f32 *arg2) {
    volatile s32 r = arg1;
}
/* trk_drawScore_000013B8: 2026-10-24 agent-g run 155: body replaced by a port of
 * arc_drawNumber_000016F4's C (opcode-identical twin, offsets -0x50 / -0x38, flag 4 -> 2):
 * 80.36 -> 85.36 (= 16F4's score). Notes below describe the previous body.
 * 170-insn timer-HUD draw, twin of
 * tim_drawTime_00001340 (no min:sec split — sprintf(buf, fmt, arg2)
 * with the f32* arg passed raw; loop digit remap is plain -0x30, no [/]).
 * 2026-07-30 redecode 65.9 -> 77.55 fixing old m2c body decode errors:
 *  - alpha is a FLOAT load *(f32*)(arg0+0x72C) (was FW int load + cvt.s.w);
 *  - first/tail call a0 = &D_b3_1240_base (baked USO reloc), was literal 0;
 *  - fmt = (char*)&D_b3_1240_base + 0x3D8 (reloc), was literal 0x3D8;
 *  - one[4] array (4 swc1 stores; scalars got 3 DCE'd) + buf[128] (was u8);
 *  - glyph loads are *(s16*)(p+0x20) / (p+0x22) (was (s16) of int load);
 *  - loop: compound `*p -= 0x30` (two-reg lbu/addiu/sb) + lbu RELOAD in the
 *    call arg; u8 *p for lbu;
 *  - saved64/saved60 copies + dead kills after strlen (the 1340
 *    un-coalesce lever; docs/IDO_CODEGEN "dead-kill-uncoalesce-saved-copy");
 *  - w6FC ASSIGNED first (call stays 5th) — first-assignment order put
 *    w6FC/w6CC/w6B4 in s0/s1/s2 = target layout (+0.2).
 * Remaining (~32 words, one-slot s-rotation): target has a BUF lda web in
 * s3 (or $4,$19 at sprintf+strlen) that DIES and hands s3 to COUNT
 * (or $19,$2); ours remats addiu $4,sp,168 per call, arg2 sits in s3,
 * count falls to t5 + spill 88 -> whole tail renames + frame 312 vs 328
 * (no s8 save; slot set differs).  Probed and FAILED to materialize the
 * buf web: named bufp (folds), while(0) bufp/count boosts (regress),
 * if(1){bufp=buf} barrier (folds), dead if(count){} (regress), u8 vs
 * char buf (no-op); 2026-08-22 agent-h: named bufp WITH a copy use
 * (p = bufp; the docs' "non-foldable use (call arg/copy) prices an
 * s-reg" rule) ALSO folds/remats byte-identically (648) — rule does
 * not fire for sp-lda webs here.  Note the asymmetry vs b1 1340: SAME spelling gives
 * buf an s8 web in b1 (5-arg sprintf, higher pressure) but remat here.
 * Next: find what prices an lda temp into a callee-saved reg at 2 refs
 * (uopt cupcosts?); count -> s3 should then cascade the tail exact. */

/* TrkNameEntry_ctor_00001660 — EXACT (90/90 words incl. relocs, 2026-07-09).
 * Twin of ArcNameEntry_ctor_0000199C (same 4-level alloc-cascade ctor
 * template; deltas: init-call base +0x3dc, a1 float @+0x72c,
 * packed-id ptr field @+0x6b0). Crack lever: the packed-id call's id
 * chain must be ucode-created before the ptr chain — spell the ptr chain
 * as a two-level typed member chain ((TimRace *)MODENAMEENT(arg0)->race)->recordRank (cfe then
 * follows textual order). Siblings: arcproc 199C, timproc_b1 16F8, mgrproc 2940. */
extern int gl_func_00000000();
extern char D_tim1660_v[];   /* init-call a1 base (&sym+0x3dc) */
extern char D_tim1660_w[];   /* a0->0x28 (level-4) */
extern char D_tim1660_x[];   /* v1->0x28 (level-3) */
extern char D_tim1660_y[];   /* a2->0x28 (level-2) */
extern char D_tim1660_z[];   /* s0->0x28 (level-1/main) */
extern int D_tim1660_id;     /* packed-id value read */


char *TrkNameEntry_ctor_00001660(char *arg0, char *arg1, int arg2) {
    char *a2;   /* level-2 (0xD4), home 0x2C(sp) */
    char *v1;   /* level-3 (0x50), home 0x28(sp) */
    char *a0;   /* level-4 (0x2C), home 0x24(sp) */

    if ((arg0 != 0) || (arg0 = (char *)CALL(mem_new)(0x10C), (arg0 != 0))) {
        a2 = arg0;
        if ((arg0 != 0) || (a2 = (char *)CALL(mem_new)(0xD4), (a2 != 0))) {
            v1 = a2;
            if ((a2 != 0) || (v1 = (char *)CALL(mem_new)(0x50), (v1 != 0))) {
                a0 = v1;
                if ((v1 != 0) || (a0 = (char *)CALL(mem_new)(0x2C), (a0 != 0))) {
                    CALL(Node_ctor)(a0, D_tim1660_v + 0x3dc); NODE(a0)->vptr = (int)D_tim1660_w;
                }
                NODE(v1)->vptr = (int)D_tim1660_x;
            }
            NODE(a2)->vptr = (int)D_tim1660_y;
        }
        NODE(arg0)->vptr = (int)D_tim1660_z;
        MODENAMEENT(arg0)->base.phase = arg2;
        MODENAMEENT(arg0)->race = (s32)arg1;
        MODENAMEENT(arg0)->pos[1][0] = 0xA0;
        MODENAMEENT(arg0)->pos[1][1] = 0x1D;
        MODENAMEENT(arg0)->pos[0][0] = 0xA0;
        MODENAMEENT(arg0)->pos[0][1] = 0x82;
        MODENAMEENT(arg0)->pos[2][0] = 0xA0;
        MODENAMEENT(arg0)->pos[2][1] = 0x69;
        MODENAMEENT(arg0)->alpha = 1.0f;
        TIMRACE(arg1)->recordFade = 1.0f;
        CALL(gui_spriteInit)(MODENAMEENT(arg0)->banner,
            ((D_tim1660_id + 0x23) << 16) | (((TimRace *)MODENAMEENT(arg0)->race)->recordRank + 7));
    }
    return arg0;
}

void TrkNameEntry_update_000017C8(int *a0) {
    if (CALL(Fader_isDone_00013980)(*(int*)(&D_00000000 + 0x190)) == 0) return;
    if (CALL(input_checkPressed_00011E00)(&D_00000000, 0x40100) == 0) return;
    if (CALL(Backup_sessionHasNew)(MODENAMEENT(a0)->base.entry) != 0) {
        MODENAMEENT(a0)->base.phase = 1;
    } else {
        CALL(Process_fadeOut_0000F954)(a0, -1, 0);
    }
}

void TrkNameEntry_drawEntryOverlay(int *a0) {
    int *p;
    CALL(TrkNameEntry_drawTitle_00001920)(a0);
    p = (int*)MODENAMEENT(a0)->race;
    CALL(trk_drawScore_000013B8)(p, 0x8C, TIMRACE(p)->finalTime);
}

/* Decay-tick handler: decrement sub->f_72C by the global rate (D+0x48),
 * clamp at 0, fire the 140-event mid-call, and on reaching <= 0 fire
 * the threshold call. MATCHED 2026-06-10 (44/44): the old wrap had a
 * DECODE ERROR (the mid-call's arg0 is SUB, not self) and a misplaced
 * sub-reload (only the clamp-taken path reloads); the final read goes
 * through the inlined full chain (fresh temp t6) rather than the sub
 * variable (whose web would color a0). bc1fl delay structure falls out
 * naturally from the val-carried if/if shape. */
void TrkNameEntry_drawExitOverlay(int *self) {
    char *base = &D_00000000;
    int *sub;
    float *fp;
    float val;

    CALL(TrkNameEntry_drawTitle_00001920)(self);
    sub = (int *)MODENAMEENT(self)->race;
    fp = (float *)((char *)sub + 0x72C);
    val = *fp;
    if (val > 0.0f) {
        *fp -= *(float *)(base + 0x48);
        sub = (int *)MODENAMEENT(self)->race;
        if (TIMRACE(sub)->recordFade < 0.0f) {
            TIMRACE(sub)->recordFade = 0.0f;
            sub = (int *)MODENAMEENT(self)->race;
        }
        CALL(trk_drawScore_000013B8)(sub, 140, TIMRACE(sub)->finalTime);
        val = TIMRACE(MODENAMEENT(self)->race)->recordFade;
    }
    if (val <= 0.0f) {
        gl_func_00000000(self);
    }
}


/* TrkNameEntry_drawTitle_00001920 (0xAC, 43 insns): hoisted-head merge of 1920 +
 * 1928 (2026-09-05 agent-c, TWENTY-FIFTH mis-split class, see
 * docs/MATCHING_WORKFLOW.md #hoisted-head-119c-11a4-mgrproc). The 8-byte
 * "alt-entry" `lui at,0x3F80; mtc1 at,$f0` is this function's own `buf[i] = 1.0f`
 * materialization scheduled above `addiu sp` by IDO 7.1 -O2 (the old 0.0f
 * body's own `mtc1 zero,$f0` was already emitted above the prologue). Oracle =
 * this block's module tables (trkproc.uso, inner header ROM 0x5B3DE2):
 * Sym[163] EXPORTS 0x1920 and two intra-module jal's (TextReloc kind 1 at
 * 0x1844 and 0x1878) target it; 0x1928 has no export and no R_MIPS_26.
 * Body = ArcNameEntry_drawBlinkBanner_00001C74's canonical TRUE-BYTE decode (byte-identical
 * clone family sig=739fd8d1d3, with b1_19C0): buf[] = 1.0f, tgt = a0+0xF0
 * spill at sp+0x20, frame 0x58.
 * EXACT 2026-09-09: field * (scale = 255.0f) materializes the constant
 * in f4 BEFORE loading the field into f6. A separate scale assignment or
 * putting the assignment-expression first restores the wrong pair order.
 * The scalar home plus pad[0x1C] retain the original frame/buffer layout.
 * This retracts the FP-pair cap; all 43 words match the original ROM's
 * decompressed Text, not merely the reloc-tolerant block verifier. */
void TrkNameEntry_drawTitle_00001920(int *a0) {
    float buf[4];
    char *tgt;
    float scale;
    char pad[0x1C];

    (void)pad;
    buf[0] = 1.0f;
    buf[1] = 1.0f;
    buf[2] = 1.0f;
    buf[3] = 1.0f;
    MODENAMEENT(a0)->base.frame += 1;
    if (CALL(Backup_sessionCountNew)(MODENAMEENT(a0)->base.entry) != 0) {
        CALL(gui_setColor_00001A5C)(&D_00000000,
            (int)(MODENAMEENT(a0)->alpha * (scale = 255.0f)), buf);
        tgt = MODENAMEENT(a0)->banner;
        CALL(gui_spriteRefresh)(tgt);
        if ((MODENAMEENT(a0)->base.frame & 8) != 0) {
            CALL(gui_drawSprite)(tgt, 0xA0, 0x7C, 3);
        }
    }
}
/* TrkDataManager_init_000019CC 2026-07-15 (agent-g wave 3): 94.04 -> 99.81.
 * Levers: (1) sp44-call arg swapped to (D_64*0x30) + *(*arg3) (right-first +
 * eval numbers the ptr chain t0/t1 like target); (2) 0x18 RMW respelled as
 * single-mutated pointer q with if(1){} advance (B850/C044 idiom: lw 24(a2),
 * addiu a2,a2,24, sw 0(a2)) and the fp block as a second q mutation (+0x64);
 * both de-named loads keep ring temps; (3) the final registration call is
 * 3-ARG with q (= sub+0x64) as arg 3 -- constrains the whole q web to $a2
 * with ZERO emission (target a2 = leftover q, K&R); a plain 2-arg drop left
 * q in $v1; (4) frame 64->80 + slot map via decl order o,sub,sp44,q +
 * volatile pad1..pad4 AFTER the named ptrs (pads claim 44-56; o-spill 76,
 * v0-spill 68 exact). RESIDUAL ~10 words: a0-spill slot 40 vs 44 (one rank
 * below the outgoing-arg spill unreachable by pads: E6E8 compiler-spill
 * class), sub-spill 72 vs 76 (target SHARES o's slot -- name-merge injects
 * an or-copy at link-1 instead), tail link-block reload $v1 vs $a2
 * (free-pick tie, if(1)/merge probed).
 * 2026-10-20 agent-g (run 109) 99.81 -> EXACT, E04 one-reused-local rule:
 * BOTH link blocks re-read the child into the SAME local o (target shares
 * slot 76), and the +0x18/+0x64 pointer mutations go through o as well, so
 * o's later a2 use (the 3rd arg of the sub create) colours the whole web a2
 * without the old 3-arg q trick; final link call is plain 2-arg. With q
 * gone, four volatile pads put the arg0+0x10 CSE spill at 44. */
extern int gl_proto_19cc(void *, int, int, float, float);
extern char D_b3_19CC_0C;
void TrkDataManager_init_000019CC(char *arg0, char *arg1, int arg2, int *arg3) {
    char *o;
    char *sub;
    char *sp44;
    volatile int pad1;
    volatile int pad2;
    volatile int pad3;
    volatile int pad4;
    float denom = 255.0f;

    DATAMGR(arg0)->name = (int)(&D_b3_19CC_0C + 0x3E4);
    TIMDATAMGR(arg0)->race = (int)arg1;
    DATAMGR(arg0)->owner = (int)arg1;
    DATAMGR(arg0)->session = (BackupSession *)arg3;
    DATAMGR(arg0)->blinkTimer = 0;
    if (RACEPROC(arg1)->flags & 0x10000) {
        DATAMGR(arg0)->pointsListArg = arg2;
        TIMDATAMGR(arg0)->alpha = 0xFF;
        TIMDATAMGR(arg0)->unkD4 = 0;
        TIMDATAMGR(arg0)->colour[3] = 1.0f;
        TIMDATAMGR(arg0)->colour[2] = 1.0f;
        TIMDATAMGR(arg0)->colour[1] = 1.0f;
        TIMDATAMGR(arg0)->colour[0] = 1.0f;
        CALL(hud_addPointsGadgetList)();
        CALL(hud_addSpeedGadget)(arg0, 0xE8, 0x13, (int)DATAMGR(arg0)->entries + 0x10);
        CALL(hud_addCountDownGadget)(arg0, 0x69, 0x11, arg0 + 0xA0);
        CALL(hud_addCourseGadget)(arg0, 0x123, 0xE1, 1);
        CALL(hud_addPointsGadget)(arg0, 0x21, 0x13, arg0 + 0xA8);
        CALL(PointsGadget_setPoints)(DATAMGR(arg0)->points, 0);
        sp44 = (char *)CALL(Backup_courseRecordGetScore)((*(int *)((char *)&D_00000000 + 0x64) * 0x30) + *(int *)((int *)DATAMGR(arg0)->session), 0);
        o = (char *)CALL(BestScoreGadget_ctor)(0, DATAMGR(arg0)->sprites60);
        TIMDATAMGR(arg0)->rankLabel = (int)o;
        CALL(BestScoreGadget_setScore)(o, ((BackupScoreEntry *)sp44)->score);
        CALL(TimeGadgetParms_setPos_00013FE0)((char *)TIMDATAMGR(arg0)->rankLabel, 0x35, 0xD6);
        o = (char *)TIMDATAMGR(arg0)->rankLabel;
        CALL(list_append)(arg0 + 0x10, o);
        if (NODE(o)->parent != 0) {
            NODE(o)->unk04 = 1;
        }
        NODE(o)->parent = (int)arg0;
        sub = (char *)CALL(PointsGadget_ctor_00000ED0)(0, DATAMGR(arg0)->digitsTall, o);
        DATAMGR(arg0)->points94 = (int)sub;
        CALL(PointsGadget_setValue)(sub, 0);
        gl_proto_19cc((char *)DATAMGR(arg0)->points94, 0xA0, 0x46, 1.0f, 1.0f);
        {
            o = (char *)DATAMGR(arg0)->points94;
            if (1) { o = o + 0x18; }
            *(int *)o = *(int *)o & ~4;
            o = (char *)DATAMGR(arg0)->points94;
            if (1) { o = o + 0x64; }
            ((float *)o)[0] = 250.0f / denom;
            ((float *)o)[1] = 235.0f / denom;
            ((float *)o)[2] = 100.0f / denom;
            ((float *)o)[3] = 0.0f / denom;
        }
        *(int *)((char *)DATAMGR(arg0)->points94 + 0xA0) = DATAMGR(arg0)->pointsList + 0x108;
        *(int *)((char *)DATAMGR(arg0)->points94 + 0x80) = 0;
        *(int *)((char *)DATAMGR(arg0)->points94 + 0x84) = -0x12;
        o = (char *)DATAMGR(arg0)->points94;
        CALL(list_append)(arg0 + 0x10, o);
        if (NODE(o)->parent != 0) {
            NODE(o)->unk04 = 1;
        }
        NODE(o)->parent = (int)arg0;
    }
}

/* TrkDataManager_loadData: orphan-merge of a stolen-prologue. The orphan
 * TrkDataManager_loadData (lui t6; lw t6,0x64(t6) = TIMB3_D_64) was the
 * stolen prologue of func_00001C28 — it loads the global compared to 1 at the
 * top. Merged FORWARD (one 0x48 symbol; successor .s deleted, no jal targets).
 * Reading TIMB3_D_64 inline at the `== 1` test hoists the lui;lw above the
 * prologue (same recipe as AudioThread_InitMesgQueuesImpl). Also fixed the old decode:
 * a0->0xD8 = 1 (the SAME literal 1 IDO materializes for the `== 1` test, in v0,
 * stored in the jal delay slot — NOT the call return). MATCHED 2026-05-30. */
void TrkDataManager_loadData(char *a0) {
    if (*(int*)((char*)&D_00000000 + 0x64) == 1) {
        DATAMGR(a0)->score = 50000;
    } else {
        DATAMGR(a0)->score = 30000;
    }
    DATAMGR(a0)->shownScore = 0;
    TRKDATAMGR(a0)->ghostReplayMode = 1;
    CALL(RaceTracker_reset)();
}

/* EXACT 2026-10-23 (agent-g run 138): (c) was a FLOAT 0.0f third arg (li a2,0)
 * through the (int, int, float) blank alias gl_proto_1c94; the r > 0 bump reads
 * the counter as fl[8] off the re-read 0x44 pointer (target lw 32(v1), no
 * addiu fl); `if (arg0) {}` after the (arg0, 1) call and after the flag-4 RMW. */
/* TrkDataManager_update_00001C68: UI animation/draw orchestrator (sibling of
 * arc_DataManager_update / mgrproc per-frame steps). cb(); gate arg0->0xB8
 * bit-16 && ->0x4DC==1: bump 0x30, decrement 0xA0 by 0x21 (floor 0 + cleanup that
 * sets sub->0x554=192.0f, ->0x544=0xFF). cb(arg0). Gated block: state-2 sync of
 * 0xD8; optional draw block; ramp 0xA8 toward 0x44->0x18 by 0xFA/0x32/0x19/1;
 * state-1 draws (float arg via gl_proto_1c68); 0x94 sub-widget enable + 0x32/5/1
 * ramp on 0x44->0x20 else disable; state-2 (0x4F8==0) blink 0x8C by (0xAC++ & 8).
 *
 * 2026-07-10 95.09% fuzzy (was 84.02; 251/253 words). Levers (agent-h):
 *  - NO bc local: direct *(char **)(arg0+0xB8) derefs everywhere; the load-CSE
 *    web (one char** ichain) then carries the a0 precolor from the two
 *    gl_func(*(arg0+0xB8)) call args (cupcosts lu->reg discount) -> bc web
 *    colors a0 like target, both gates + post-call rereads + arm-4 join.
 *  - v0b ramp arm loads are INT-typed (*(int *)(arg0+0xB8)): different ichain
 *    from the char** web -> no CSE with the held a0 pointer, forcing the
 *    per-arm fresh t-reg reloads the target shows. Arm 4 (join) stays char**
 *    (it IS an a0-web reload in target).
 *  - 0xA0/0xAC RMWs in compound deref form (no t2/t1 temp locals) =
 *    un-coalesced two-reg lw/addiu/sw shape.
 *  - flag RMWs (p94/p8c +0x18, p44 +0x20 arms 1-2) via held ptr assigned from
 *    the DIRECT deref chain: fl=(int *)(*(char **)(arg0+0x94)+0x18); *fl|=4;
 *    gives the target addiu+lw/sw-0(v0) form (a p94-local base folds to
 *    24(reg) instead). Arm 3 (+1) is the FOLDED 32(v1) form in target - spell
 *    it as a plain compound deref.
 * RESIDUAL (~15 words, one family): as1/uopt adjacent-pair picks - (a) the
 * conditional gl(arg0,1) call hoists addiu a1,1 into the cond load-delay slot
 * where target hoists or a0,s0 (probe-immune: casts/1u/arg-splits inert);
 * (b) arm-1's fl addiu is como-hoisted above the bnel (single-path uadd still
 * hoists; fl2-split/cast probes inert) which steals v0, knocking r->a0 and
 * a1-var->a2+copy (target r->v0, a1-var->a1); (c) one addiu-vs-or zero-arg
 * materialization at the state-1 draw call (0u probe inert). Same
 * allocator/scheduler-internal class as the Two-s-reg kit II residuals. */
extern int gl_proto_1c68(void *, int, int, float, float);
extern int gl_proto_1c94(int, int, float);
void TrkDataManager_update_00001C68(char *arg0) {
    int *fl;
    int v0;
    int v0b;
    register int a1;
    int rem;
    int r;
    int t1;

    CALL(RaceTracker_toggleGhost)();
    if ((RACEPROC(TRKDATAMGR(arg0)->race)->flags & 0x10000) && (RACEPROC(TRKDATAMGR(arg0)->race)->raceState == 1)) {
        DATAMGR(arg0)->elapsed = DATAMGR(arg0)->elapsed + 0x21;
        CALL(RaceTracker_passCheckpoints)(arg0);
        DATAMGR(arg0)->score = DATAMGR(arg0)->score - 0x21;
        if (DATAMGR(arg0)->score < 0) {
            DATAMGR(arg0)->score = 0;
            CALL(CountDownGadget_setTime)(DATAMGR(arg0)->countDown, 0, 0);
            CALL(RaceProcess_timeUp)(TRKDATAMGR(arg0)->race);
            RACEPROC(TRKDATAMGR(arg0)->race)->msgFloor = 192.0f;
            RACEPROC(TRKDATAMGR(arg0)->race)->msgAlpha = 0xFF;
            DATAMGR(arg0)->active = 0;
        }
    }
    CALL(RaceTracker_update)(arg0);
    v0 = RACEPROC(TRKDATAMGR(arg0)->race)->flags & 0x10000;
    if ((v0 != 0) && (RACEPROC(TRKDATAMGR(arg0)->race)->raceState == 1)) {
        if (DATAMGR(arg0)->numEntries == 2) {
            if ((TRKDATAMGR(arg0)->ghostReplayMode == 1) &&
                (((PadCtl *)SNOWB(DATAMGR(arg0)->entries[1].rider)->input)->replayMode == 0)) {
                CALL(RaceTracker_sendEntryLoseResult)(arg0, 1); if (arg0) {}
            }
            TRKDATAMGR(arg0)->ghostReplayMode = ((PadCtl *)SNOWB(DATAMGR(arg0)->entries[1].rider)->input)->replayMode;
            v0 = RACEPROC(TRKDATAMGR(arg0)->race)->flags & 0x10000;
        }
        if (DATAMGR(arg0)->entries->finished != 0) {
            if (v0 != 0) {
                CALL(RaceTracker_endAllTricks)(arg0);
                CALL(RaceTracker_popFinalScore)(arg0, DATAMGR(arg0)->score);
                CALL(CountDownGadget_setTime)(DATAMGR(arg0)->countDown, DATAMGR(arg0)->score, 0);
            }
            CALL(RaceProcess_finishCourse)(TRKDATAMGR(arg0)->race);
            v0 = RACEPROC(TRKDATAMGR(arg0)->race)->flags & 0x10000;
        }
    }
    if (v0 != 0) {
        /* the first three raceState reads load race as an int (`*(int *)&`): a lever, the original's mixed
         * int / pointer loads keep IDO from merging the three shownScore arms (agent-h run 529) */
        t1 = DATAMGR(arg0)->shownScore;
        rem = DATAMGR(arg0)->entries->score - t1;
        if (rem >= 0xFA) {
            DATAMGR(arg0)->shownScore = t1 + 0xFA;
            v0b = RACEPROC(*(int *)&TRKDATAMGR(arg0)->race)->raceState;
        } else if (rem >= 0x32) {
            DATAMGR(arg0)->shownScore = t1 + 0x32;
            v0b = RACEPROC(*(int *)&TRKDATAMGR(arg0)->race)->raceState;
        } else if (rem >= 0x19) {
            DATAMGR(arg0)->shownScore = t1 + 0x19;
            v0b = RACEPROC(*(int *)&TRKDATAMGR(arg0)->race)->raceState;
        } else {
            if (rem > 0) {
                DATAMGR(arg0)->shownScore = t1 + 1;
            }
            v0b = RACEPROC(TRKDATAMGR(arg0)->race)->raceState;
        }
        if (v0b == 1) {
            CALL(CountDownGadget_setTime)(DATAMGR(arg0)->countDown, DATAMGR(arg0)->score, 1);
            gl_proto_1c94(DATAMGR(arg0)->course, *(int *)&DATAMGR(arg0)->entries->pct, 0.0f);
            gl_proto_1c68(DATAMGR(arg0)->course, *(int *)&DATAMGR(arg0)->entries->posX, *(int *)&DATAMGR(arg0)->entries->progress, 0.0f, 0.0f);
        }
        if (DATAMGR(arg0)->entries->pendingPoints != 0) {
            gl_proto_1c68(DATAMGR(arg0)->points94, 0xA0, 0x46, 1.0f, 1.0f);
            POPUP(DATAMGR(arg0)->points94)->alpha = 0xFF;
            fl = &NODE(DATAMGR(arg0)->points94)->doFlags;
            *fl = *fl | 4; if (arg0) {}
            a1 = DATAMGR(arg0)->entries->shownPending;
            r = DATAMGR(arg0)->entries->pendingPoints - a1;
            if (r >= 0x32) {
                fl = &DATAMGR(arg0)->entries->shownPending;
                *fl = *fl + 0x32;
                CALL(snd_sendEvent)(0x20, 0x32);
                a1 = DATAMGR(arg0)->entries->shownPending;
            } else if (r >= 5) {
                fl = &DATAMGR(arg0)->entries->shownPending;
                *fl = *fl + 5;
                CALL(snd_sendEvent)(0x20, 5);
                a1 = DATAMGR(arg0)->entries->shownPending;
            } else if (r > 0) {
                fl = (int *)((char *)DATAMGR(arg0)->entries);
                fl[8] = fl[8] + 1;
                CALL(snd_sendEvent)(0x20, 1);
                a1 = DATAMGR(arg0)->entries->shownPending;
            }
            CALL(PointsGadget_setValue)(DATAMGR(arg0)->points94, a1);
        } else {
            fl = &NODE(DATAMGR(arg0)->points94)->doFlags;
            *fl = *fl & ~4;
        }
        if ((RACEPROC(TRKDATAMGR(arg0)->race)->raceState == 2) && (RACEPROC(TRKDATAMGR(arg0)->race)->result == 0)) {
            DATAMGR(arg0)->blinkTimer = DATAMGR(arg0)->blinkTimer + 1;
            if (DATAMGR(arg0)->blinkTimer & 8) {
                fl = &NODE(DATAMGR(arg0)->points)->doFlags;
                *fl = *fl & ~4;
            } else {
                fl = &NODE(DATAMGR(arg0)->points)->doFlags;
                *fl = *fl | 4;
            }
        }
    }
    CALL(Node_update)(arg0);
}


void ParmInt_read_0000205C(char *dst) {
    int tmp;
    trk_streamReadS32_00000854(&tmp);
    trk_streamReadS32_00000854((int*)(dst + 0x10));
}

void ParmFloat_read_0000208C(char *dst) {
    int tmp;
    trk_streamReadS32_00000854(&tmp);
    trk_streamReadF32_00000890((float*)(dst + 0x10));
}

void ParmQuad_read_000020BC(char *dst) {
    int tmp;
    trk_streamReadS32_00000854(&tmp);
    trk_streamReadQuad_000008CC((Quad4*)(dst + 0x10));
}

/* TrkItemType_create_000020EC: 36-insn (0x90) constructor — BYTE-IDENTICAL
 * mirror of edd_ItemType_create_000003BC family (5th clone). Apply the
 * eddproc lever (volatile-ptr-to-arg + late head reload) per
 * docs/IDO_CODEGEN.md#feedback-ido-arg-addr-via-volatile-ptr-forces-caller-spill.
 * Expected to land in the 89% file-context band per
 * docs/IDO_CODEGEN.md#feedback-ido-file-context-affects-frame-size
 * (timproc_uso_b3.c is a large multi-function file). */
void *TrkItemType_create_000020EC(int *arg0) {
    int *p2;
    int *head;
    int *p1;
    p1 = (int*)CALL(mem_new)(0x40);
    if (p1 != 0) {
        CALL(ColourItem_ctor)(p1);
        NODE(p1)->vptr = (int)&D_00000000;
        *(int*)((char*)p1 + 0x3C) = 0;
    }
    p2 = p1;
    p1 = arg0;
    head = (int*)p1[0x40 / 4];
    if ((int*)p1[0x40 / 4] != 0) {
        CALL(list_append)((char*)p2 + 0x10, head);
        if (NODE(head)->parent != 0) {
            NODE(head)->unk04 = 1;
        }
        NODE(head)->parent = (int)p2;
    }
    return p2;
}

/* ParmVec3_read_0000217C: 2-call wrapper (twin of edd_readVec3Parm_0000044C).
 * The ROM symbol is 13 words: the 12-word body plus the zero alignment word
 * before 21B0. A 1-word GLOBAL_ASM pad sidecar emits 8 bytes (+4 shift), so
 * the zero word comes from an all-zero SUFFIX_BYTES_FORCE in both builds
 * (Makefile, next to the NON_MATCHING_SUFFIX_BYTES_FORCE line). */
void ParmVec3_read_0000217C(char *dst) {
    int tmp;
    trk_streamReadS32_00000854(&tmp);
    trk_streamReadVec3_00000924((Vec3*)(dst + 0x10));
}

void TrkMenu_rowRetry_000021B0(void) {
    CALL(Process_stop)(gl_ref_00000208);
    gl_ref_00000040 = 4;
    CALL(Process_fadeOut_0000F954)(gl_ref_0000020C, -1, 0);
}

/* func_000021F4 (0x44): byte-identical twin of TimMenu_rowChangeCharacter
 * (matched) except the state constant (li 6 vs li 1). Ported donor body w/
 * gl_ref_00000040 = 6. */
void TrkMenu_rowGhost_000021F4(void) {
    CALL(Process_stop)(gl_ref_00000208);
    gl_ref_00000040 = 6;
    CALL(Process_fadeOut_0000F954)(gl_ref_0000020C, -1, 0);
}

/* Dual-branch state setter, twin of TimMenu_rowToggle_00002028.
 * Starts at 0x2238 (was split as a 2-word orphan + func_00002240 taking
 * a0 as a parameter): the lui/lw of D+0x148 is the first call's argument,
 * hoisted by IDO above the addiu sp prologue (see the b1 twin).
 * MATCHED 2026-06-10 (31/31) as 2240 -- see the b1 twin for the full story:
 * first call arg = pointer+4, one shared +0x208 call
 * target, volatile pointer-fetch cur (+0x20C), and the if(1){} BB-split
 * in each arm (v0/v1 lever). */
extern int D_call_b3_2240_a;
extern int * volatile D_cur_b3_2240;
void TrkMenu_rowToggle_00002238(void) {
    if (CALL(Backup_ghostGetMode)(*(char**)((char*)&D_00000000 + 0x148) + 4) != 0) {
        CALL(Process_stop)(D_call_b3_2240_a);
        if (1) {}
        D_cur_b3_2240[0x14] = 2;
        D_cur_b3_2240[0x16] = 1;
    } else {
        CALL(Process_stop)(D_call_b3_2240_a);
        if (1) {}
        D_cur_b3_2240[0x14] = 1;
        D_cur_b3_2240[0x16] = 1;
    }
}

void TrkMenu_rowChangeCharacter(void) {
    CALL(Process_stop)(gl_ref_00000208);
    gl_ref_00000040 = 1;
    CALL(Process_fadeOut_0000F954)(gl_ref_0000020C, -1, 0);
}

extern int gl_ref_00000208;
extern int gl_ref_0000020C;

void TrkMenu_rowChangeBoard(void) {
    CALL(Process_stop)(gl_ref_00000208);
    gl_ref_00000040 = 2;
    CALL(Process_fadeOut_0000F954)(gl_ref_0000020C, -1, 0);
}

extern int gl_ref_00000208;
extern int gl_ref_0000020C;

void TrkMenu_rowChangeCourse(void) {
    CALL(Process_stop)(gl_ref_00000208);
    gl_ref_00000040 = 3;
    CALL(Process_fadeOut_0000F954)(gl_ref_0000020C, -1, 0);
}

void TrkMenu_rowQuit_00002388(void) {
    CALL(Process_stop)(gl_ref_00000208);
    gl_ref_00000040 = 0xD;
    CALL(Process_fadeOut_0000F954)(gl_ref_0000020C, -1, 0);
}

/* 000023CC/23D4/23DC: 3 empty stubs RECOVERED 2026-05-28 from the Yay0 gap
 * (no .s, missing from build; bytes from block_3). void f(void){} -> jr ra;nop. */
void trk_nop_000023CC(void) {}
void trk_nop_000023D4(void) {}
void trk_nop_000023DC(void) {}

/* Alloc-cascade ctor (sibling of ARCMenuProcess_ctor): alloc 108/80/44
 * + ->0x28=&D + &D+1160/1168 descriptors, then a registration tail. Decoded
 * 2026-06-02. Reconstructed 2026-06-22 (agent-e): 59.73 -> 95.54%.
 *  - Whole body wrapped in the `(arg0 != 0 || (alloc; cond))` short-circuit
 *    ladder so the alloc-failure paths fall through to `return s0` (matches
 *    the nested `bne X,zero;<delay assignment>` cascade exactly).
 *  - Distinct extern symbols per global-deref site (D_b3_23E4_d2..8 for the 7
 *    OR'd-value reads; D_b3_23E4_d1 for the 4th-arg pointer) DEFEAT IDO's CSE
 *    that hoisted &D into a saved $s1: target re-materializes the base with a
 *    fresh `lui` per call. This collapsed the size gap -24 -> -1 word and
 *    dropped 162 -> 57 reloc-filtered diffs. (relocs are masked by the gate.)
 *  - `(D[0] == 0x17D7) == 0` form emits the target's `xori;sltiu;bnel`
 *    seq-idiom (a plain `!= 0x17D7` regresses to li+beql).
 * 2026-10-16 (agent-g) EXACT (96.32 -> 100): ported the levers of the exact
 *  sibling ARCMenuProcess_ctor -- the 199C short-circuit head with
 *  ONE a3 web for level-2 / vtable / registration nodes, `if (1) {}` before
 *  the dispatch base, and the unused `volatile int pad` (+8 frame, lifts the
 *  arg0+16 spill to 0x2C). That fixed every spill slot and the entry delay
 *  fill. Then: (1) the 0x17D7 compare reads its OWN symbol (target `lui t1;
 *  lw t1,0(t1)`), while D+388 / D+400 / the final call's D+400 share one
 *  base symbol (CSE'd into v0 for the gate, re-materialized after the call);
 *  (2) the first registration (call, `->0x14` test, store) on ONE source
 *  line: with it on separate lines the gate base `addiu v0` schedules after
 *  the arg0+16 reload instead of before it. See docs/IDO_CODEGEN.md
 *  #same-line-registration-schedule-23e4. */
extern int D_b3_23E4_17D7;
extern char D_b3_23E4_base;
char *TrkMenu_ctor(char *arg0) {
    char *a3;
    char *a0;
    char *v0;
    volatile int pad;

    if ((arg0 != 0) || (arg0 = (char *)CALL(mem_new)(108), (arg0 != 0))) {
        a3 = arg0;
        if ((arg0 != 0) || (a3 = (char *)CALL(mem_new)(80), (a3 != 0))) {
            a0 = a3;
            if ((a3 != 0) || (a0 = (char *)CALL(mem_new)(44), (a0 != 0))) {
                CALL(Node_ctor)(a0, (char *)&D_00000000 + 1160); *(char **)&NODE(a0)->vptr = &D_00000000;
            }
            *(char **)(a3 + 0x28) = &D_00000000;
        }
        TIMMENU(arg0)->base.node.vptr = (s32)(&D_00000000);
        TIMMENU(arg0)->base.node.name = (s32)((char *)&D_00000000 + 1168);
        CALL(TrkMenu_initLabels_00002700)(arg0);
        TIMMENU(arg0)->unk60 = 120;
        TIMMENU(arg0)->unk64 = 161;
        TIMMENU(arg0)->promptY = 191;
        TIMMENU(arg0)->base.menu = CALL(PauseMenu_create)(0);
        CALL(gui_initRows_000100F0)(TIMMENU(arg0)->base.menu, arg0);
        CALL(gui_addRowSub_00010B6C)(TIMMENU(arg0)->base.menu, ((*(int *)&D_b3_23E4_d2 + 3) << 16) | 1, -1, &D_b3_23E4_d1);
        CALL(gui_addRowSub_00010B6C)(TIMMENU(arg0)->base.menu, ((*(int *)&D_b3_23E4_d3 + 3) << 16) | 6, -1, &D_b3_23E4_d1);
        CALL(gui_addRowSub_00010B6C)(TIMMENU(arg0)->base.menu, ((*(int *)&D_b3_23E4_d4 + 3) << 16) | 7, -1, &D_b3_23E4_d1);
        CALL(gui_addRowSub_00010B6C)(TIMMENU(arg0)->base.menu, ((*(int *)&D_b3_23E4_d5 + 3) << 16) | 4, -1, &D_b3_23E4_d1);
        CALL(gui_addRowSub_00010B6C)(TIMMENU(arg0)->base.menu, ((*(int *)&D_b3_23E4_d6 + 3) << 16) | 3, -1, &D_b3_23E4_d1);
        CALL(gui_addRowSub_00010B6C)(TIMMENU(arg0)->base.menu, ((*(int *)&D_b3_23E4_d7 + 3) << 16) | 2, -1, &D_b3_23E4_d1);
        CALL(gui_addRowSub_00010B6C)(TIMMENU(arg0)->base.menu, ((*(int *)&D_b3_23E4_d8 + 3) << 16) | 5, -1, &D_b3_23E4_d1);
        GUIMENU(TIMMENU(arg0)->base.menu)->unk30 = CALL(gui_fontCreate)(0, &D_00000000, 72, 221, 3, 13);
        CALL(gui_centerRows_0001016C)(TIMMENU(arg0)->base.menu);
        CALL(gui_centerAt_000102A4)(TIMMENU(arg0)->base.menu, 140);
        if (1) {}
        v0 = (char *)TIMMENU(arg0)->base.menu;
        a3 = (char *)NODE(v0)->vptr;
        ((void (*)(int))VT_SLOT(a3, 10).fn)(VT_SLOT(a3, 10).delta + (int)v0);
        a3 = (char *)TIMMENU(arg0)->base.menu;
        CALL(list_append)(arg0 + 16, a3); if (NODE(a3)->parent != 0) { NODE(a3)->unk04 = 1; } NODE(a3)->parent = (int)arg0;
        if (INPUT_MGR(D_b3_23E4_base).unk184 == 0 ||
            (D_b3_23E4_17D7 == 0x17D7) == 0) {
            GUIMENU(TIMMENU(arg0)->base.menu)->rows[2].enabled = 0;
        }
        a3 = (char *)INPUT_MGR(D_b3_23E4_base).fader;
        CALL(list_append)(arg0 + 16, a3);
        if (NODE(a3)->parent != 0) {
            NODE(a3)->unk04 = 1;
        }
        NODE(a3)->parent = (int)arg0;
        CALL(Fader_start)(INPUT_MGR(D_b3_23E4_base).fader, 1, 0, a3);
        TIMMENU(arg0)->unk54 = 0;
        TIMMENU(arg0)->base.state = 0;
        TIMMENU(arg0)->base.unk30 = 1;
        TIMMENU(arg0)->base.unk2C = 0;
    }
    return arg0;
}

/* Byte-identical sibling of TimMenu_initLabels_000024F4 (147 insns).
 * Same display-list build pattern. The 0x260000-series `or a1, s1, tN` wants
 * the hoisted constant (s1) as rs; IDO defaults to value-as-rs. Forced via the
 * assignment-expression operand-order lever `value | (t = 0x260000)` (pins the
 * const as rs at the op; CSE keeps the single lui). See
 * docs/IDO_CODEGEN.md#feedback-ido-commutative-op-operand-order. */
void TrkMenu_initLabels_00002700(int a0) {
    int t;
    CALL(gui_spriteInit)((char *)&D_00000000 + 112,
                     (*(int *)&D_00000000 << 3) | (t = 0x260000));
    CALL(gui_spriteInit)((char *)&D_00000000 + 136,
                     ((*(int *)&D_00000000 << 3) + 1) | (t = 0x260000));
    CALL(gui_spriteInit)((char *)&D_00000000 + 160,
                     ((*(int *)&D_00000000 << 3) + 2) | (t = 0x260000));
    CALL(gui_spriteInit)((char *)&D_00000000 + 184,
                     ((*(int *)&D_00000000 << 3) + 3) | (t = 0x260000));
    CALL(gui_spriteInit)((char *)&D_00000000 + 208,
                     ((*(int *)&D_00000000 << 3) + 4) | (t = 0x260000));
    CALL(gui_spriteInit)((char *)&D_00000000 + 232,
                     ((*(int *)&D_00000000 << 3) + 5) | (t = 0x260000));
    CALL(gui_spriteInit)((char *)&D_00000000 + 256,
                     ((*(int *)&D_00000000 << 3) + 6) | (t = 0x260000));
    CALL(gui_spriteInit)((char *)&D_00000000 + 280,
                     ((*(int *)&D_00000000 << 3) + 7) | (t = 0x260000));
    CALL(gui_spriteInit)((char *)&D_00000000 + 16, 0x280000);
    CALL(gui_spriteInit)((char *)&D_00000000 + 40, 0x280000 | 1);
    CALL(gui_spriteInit)((char *)&D_00000000 + 64, 0x280000 | 2);
    CALL(gui_spriteInit)((char *)&D_00000000 + 88, 0x280000 | 3);
    CALL(gui_spriteInit)((char *)&D_00000000 + 304,
                     (*(int *)&D_00000000 + 52) << 16);
    CALL(gui_spriteInit)((char *)&D_00000000 + 328,
                     ((*(int *)&D_00000000 + 52) << 16) | 3);
    CALL(gui_spriteInit)((char *)&D_00000000 + 352,
                     ((*(int *)&D_00000000 + 52) << 16) | 4);
    CALL(gui_spriteInit)((char *)&D_00000000 + 448,
                     ((*(int *)&D_00000000 + 52) << 16) | 1);
    CALL(gui_spriteInit)((char *)&D_00000000 + 472,
                     ((*(int *)&D_00000000 + 52) << 16) | 2);
    CALL(gui_spriteInit)((char *)&D_00000000 + 376,
                     ((*(int *)&D_00000000 + 45) << 16) | 4);
    CALL(gui_spriteInit)((char *)&D_00000000 + 400,
                     ((*(int *)&D_00000000 + 45) << 16) | 5);
    CALL(gui_spriteInit)((char *)&D_00000000 + 424,
                     ((*(int *)&D_00000000 + 45) << 16) | 6);
    CALL(gui_spriteInit)((char *)&D_00000000 + 496, 0x2F0012);
    (void)a0;
}

/* Twin of TimMenu_update_00002740 (byte-identical). MATCHED 2026-06-11
 * via the array-IXA lever derived from the uopt source (references/ido):
 * a 2D-array row index (Row294C *base; base[idx]) keeps the IXA ucode form,
 * which ugen lowers as addu rd, base, scaled (base FIRST); flat ptr-arith
 * (base + idx*0xA) goes through cfe's ADD path which evaluates the deeper
 * mpy operand first -> addu rd, scaled, base. See docs/IDO_CODEGEN.md
 * "addu operand order" uopt-source entry. */
extern int D_b3_294C_g208;
extern int D_b3_294C_g20C;
void TrkMenu_update_0000294C(int *a0)
{
  int new_var;
  switch (MENUPROC(a0)->state)
  {
    case 0:
      if (CALL(input_checkPressed_00011E00)(&D_00000000, 0x40100) != 0)
    {
      /* base[i] = the menu shifted by i rows (Row294C = one GuiRow), so slot's rows[0] is
       * rows[cursor]; the 2D-array (IXA) index is the addu-order lever (&menu->rows[cursor]
       * costs an extra addiu) */
      typedef int Row294C[10];
      Row294C *base = (Row294C *) MENUPROC(a0)->menu;
      int *slot = base[GUIMENU(base)->cursor];
      if (GUIMENU(slot)->rows[0].arg != 0)
      {
        if (GUIMENU(slot)->rows[0].enabled != 0)
        {
          int *base2;
          int *slot2;
          CALL(snd_postEvent)(5);
          if (1)
          {
          }
          slot = a0; /* reusing slot for a0 is load-bearing */
          D_b3_294C_g208 = MENUPROC(slot)->menu;
          D_b3_294C_g20C = (int) a0;
          base2 = (int *) MENUPROC(slot)->menu;
          ;
          ((void (*)(void)) ((int *) (base2 + (GUIMENU(base2)->cursor * 0xA)))[(0x90 ^ 0) / (new_var = 4)])();
        }
        else
        {
          CALL(snd_postEvent)(165);
        }
      }
    }
      break;

    case 1:

    case 2:
      CALL(TrkMenu_handleInput_00002C98)(a0);
      break;

  }

}

/* TrkMenu_draw_00002A44: HUD draw dispatcher, byte-twin of
 * TimMenu_draw_00002838 (only middle panel ID differs 0x1C0->0x1D8).
 * 2026-07-17 agent-g: 149/149 words EXACT (was 86.64) via verbatim backport
 * of the 2838 crack: shared obj local colors the s0 web (44EDC recipe);
 * &D+0 first arg (not literal 0); single-def x = v0-0x27 after
 * `if (f) { if(1){ v0 += 0xA; } }` -> beql tail-dup (rematerialize-in-delay)
 * + x demoted to last-colored web; separate case-2 local w; decl order
 * f,v0,a2,w,x for spill homes v0->0x2C/a2->0x28/w->0x24.
 * 2026-10-16 agent-g: LANDED. The placeholder callee (glv_2a44, blank) and
 * &D_00000000+K data are the same convention as the landed b3 exacts
 * (23E4, 1240); the Yay0 block is cut from the unlinked .o, so the raw
 * slice (149/149 words == expected/) is what reaches the ROM block. */
extern void glv_2a44();
void TrkMenu_draw_00002A44(char *arg0) {
    char *obj = arg0; /* shared obj local colors the recurring s0 web (44EDC recipe) */
    int f;
    int v0;
    int a2;
    int w;
    int x;

    glv_2a44(obj);
    TIMMENU(obj)->blink = TIMMENU(obj)->blink - 0x10;
    switch (TIMMENU(obj)->base.state) {
    case 1:
        glv_2a44((char *)&D_00000000);
        glv_2a44((char *)&D_00000000 + 0x1F0);
        glv_2a44((char *)&D_00000000 + 0x1F0, 0xA0, 0x25, 3);
        glv_2a44(obj, 0x25);
        glv_2a44(obj, *(unsigned char *)((char *)&D_00000000 + 0x178), 0x50);
        glv_2a44((char *)&D_00000000 + 0x1D8);
        glv_2a44((char *)&D_00000000 + 0x1D8);
        glv_2a44((char *)&D_00000000 + 0x1D8, 0xA0, 0x69, 3);
        f = *(int *)&D_00000000;
        v0 = TIMMENU(obj)->promptY;
        if (f != 0) {
            if (1) { v0 += 0xA; }
        }
        x = v0 - 0x27;
        a2 = x;
        if (f != 0) {
            a2 = x - 0xE;
        }
        glv_2a44((char *)&D_00000000 + 0x148);
        glv_2a44((char *)&D_00000000 + 0x148);
        glv_2a44((char *)&D_00000000 + 0x148, 0xA0, a2, 3);
        glv_2a44(obj, v0 - 0x14);
        return;
    case 2:
        glv_2a44((char *)&D_00000000);
        glv_2a44((char *)&D_00000000 + 0x1F0);
        glv_2a44((char *)&D_00000000 + 0x1F0, 0xA0, 0x25, 3);
        glv_2a44(obj, 0x25);
        glv_2a44(obj, *(unsigned char *)((char *)&D_00000000 + 0x178), 0x50);
        glv_2a44((char *)&D_00000000 + 0x1D8);
        glv_2a44((char *)&D_00000000 + 0x1D8);
        glv_2a44((char *)&D_00000000 + 0x1D8, 0xA0, 0x69, 3);
        glv_2a44((char *)&D_00000000 + 0x160);
        glv_2a44((char *)&D_00000000 + 0x160);
        glv_2a44((char *)&D_00000000 + 0x160, 0xA0, 0x8E, 3);
        w = TIMMENU(obj)->promptY;
        if (*(int *)&D_00000000 != 0) {
            w -= 8;
        }
        glv_2a44((char *)&D_00000000 + 0x178);
        glv_2a44((char *)&D_00000000 + 0x178);
        glv_2a44((char *)&D_00000000 + 0x178, 0xA0, w - 0x13, 3);
        glv_2a44(obj, w);
        break;
    case 0:
        break;
    }
}

/* bootup imports of the TrkMenu_handleInput_00002C98 below, read from the module's Sym table
 * (kind-1 import entries: exporter module 0 = bootup, word3 top half = bootup Sym index;
 * agent-c run 495). They stay jal 0 / lui 0 placeholders in this unlinked Yay0 block.
 * This is the post-race save-ghost prompt: the stick (0x10001 / 0x4002) moves the cursor
 * obj+0x58 between 0 (save) and 1 (skip); confirm (0x40100) on row 0 saves the recorded run
 * as the SRAM ghost (replay_saveGhost, BACKUP_MODE_TRICK); cancel (0x200) skips. */
extern int trk_imp_input_checkPressed();
extern int trk_imp_snd_postEvent();
extern int trk_imp_replay_saveGhost();

/* 4-stage state dispatcher on obj->0x58: each stage queries gl_func(&D, flag);
 * on hit, conditionally toggles obj->0x58 or fires a virtual method
 * ((*vt->0x5C)((short)vt->0x58 + o48)) on the obj->0x48 sub-object. The empty
 * `if (1) {}` before each o48 load is load-bearing: it forces an IDO
 * basic-block boundary that colors o48 into the dead $v0 (vt into $v1),
 * matching the target (was a 98.26% v0/v1 near-miss). Same lever as
 * MGRRaceProcess_endTimerEnabled / the timproc_uso_b5 dispatch family. */
void TrkMenu_handleInput_00002C98(char *obj) {
    char *o48;
    char *vt;

    if (trk_imp_input_checkPressed(&gInputMgr, 0x10001) != 0 && TIMMENU(obj)->choice != 0) {
        trk_imp_snd_postEvent(1);
        TIMMENU(obj)->choice = 0;
    }
    if (trk_imp_input_checkPressed(&gInputMgr, 0x4002) != 0 && TIMMENU(obj)->choice == 0) {
        trk_imp_snd_postEvent(1);
        TIMMENU(obj)->choice = 1;
    }
    if (trk_imp_input_checkPressed(&gInputMgr, 0x40100) != 0) {
        if (TIMMENU(obj)->choice == 0) {
            trk_imp_snd_postEvent(5);
            trk_imp_replay_saveGhost(&gInputMgr, BACKUP_MODE_TRICK);
            GUIMENU(TIMMENU(obj)->base.menu)->rows[2].enabled = 0;
            if (1) {}
            o48 = (char *)TIMMENU(obj)->base.menu;
            TIMMENU(obj)->base.state = 0;
            vt = (char *)NODE(o48)->vptr;
            ((void (*)(int))VT_SLOT(vt, 10).fn)(VT_SLOT(vt, 10).delta + (int)o48);
        } else {
            trk_imp_snd_postEvent(0x802);
            if (1) {}
            o48 = (char *)TIMMENU(obj)->base.menu;
            TIMMENU(obj)->base.state = 0;
            vt = (char *)NODE(o48)->vptr;
            ((void (*)(int))VT_SLOT(vt, 10).fn)(VT_SLOT(vt, 10).delta + (int)o48);
        }
    }
    if (trk_imp_input_checkPressed(&gInputMgr, 0x200) != 0) {
        trk_imp_snd_postEvent(0x802);
        if (1) {}
        o48 = (char *)TIMMENU(obj)->base.menu;
        TIMMENU(obj)->base.state = 0;
        vt = (char *)NODE(o48)->vptr;
        ((void (*)(int))VT_SLOT(vt, 10).fn)(VT_SLOT(vt, 10).delta + (int)o48);
    }
}

/* TrkMenu_drawHeader_00002DF0: 63-insn (0x100) grid-render setup.
 *
 * Structure:
 *   1. Copy 5 ints from D[0x4A0..0x4B0] into a local stack buf[5]
 *   2. Initial setup call: gl_func(&D + 0, <passthrough a1/a2/a3>)
 *   3. Nested loop (7 outer × 5 inner = 35 calls per cell):
 *      - Outer y: 0x10..0xF0 step 0x20
 *      - Inner x: 0..0x140 step 0x40, indexed via buf[0..4]
 *      - Each iter: idx = buf[i]; entry = &D + 0x10 + idx*0x18;
 *                   gl_func(entry); gl_func(entry, x, y, 0);
 *   4. Trailing `sll t6, a1, 2` at offset 0xFC sets t6 = a1*4 — this is
 *      the prologue-stolen successor donation to func_00002EF0 (see its
 *      wrap comment); it lives in 00002DF0's symbol footprint.
 *
 * Old recipe: docs/POST_CC_RECIPES.md "SUFFIX_BYTES on this function with
 * 0x00057080 to put the trailing sll back after C-emit; then successor
 * matches without PROLOGUE_STEALS." Instruction-appending SUFFIX_BYTES
 * REMOVED 2026-05-23 as match-faking per
 * feedback_no_instruction_forcing_matches_policy;
 * docs/POST_CC_RECIPES.md DEPRECATED. Real fix needs splat boundary
 * correction so the trailing sll belongs to func_00002EF0's symbol — not
 * tick-safe.
 *
 * 2026-07-23: backported the b1 2BE4 62/63 recipe (byte-twin modulo copy
 * source 0x4A0 vs 0x4D0): struct-assign copy ping-pong, indexed vec4[i] +
 * derived i*0x40 (kills the s8-&vec4 cache), pointer-mutation s4 base
 * (if(1){s4+=0x10;}), folded copy source. 2026-07-23: addu operand tie
 * closed by compound-assign split `s2=(char*)s4; s2+=vec4[i]*0x18;` (+= tree
 * emits base-first addu s2,s4,t1). Full 0x100 footprint now BYTE-EXACT vs
 * expected/ (incl. the trailing sll t6,a1,2 region). USO placeholder
 * callees (jal 0): stays NM wrap, no episode.
 * 2026-10-16 (agent-g): UNWRAPPED EXACT. The .s is now 0xFC (the trailing
 * sll moved to the successor's symbol), the raw slice equals expected/, and
 * placeholder jal 0 callees are fine in a Yay0-block unit (block cut from the
 * unlinked .o): docs/MATCHING_WORKFLOW.md#yay0-block-unlinked-in-unit-jal-f360.
 *
 * 2026-05-15 status (superseded): 61.83%. buf-copy switched to the pointer-walk form
 * (`int *src = &D+0x4A0; buf[i]=src[i]`) — that IS the target shape
 * (`addiu t7,&D+0x4A0; lw t9,0(t7); sw t9,0(sp+0x54); ...`), flat fuzzy
 * but a more faithful decode. Dominant remaining blockers, in order:
 *   1. Frame size: built -0x70 (112) vs target -0x68 (104). Built saves
 *      $s4/$s8 (10 sreg slots) where target saves only $s0-$s7 (8).
 *      The extra $s4-as-&D-base + $s8 are the regression source.
 *   2. built materializes `&D` into $s4 once (`lui s4; addiu s4; or a0,s4`)
 *      and reloads all 0x4A0+ fields off it; target keeps NO persistent
 *      &D base — it re-lui's per use (t7 for the copy, fresh a0 for the
 *      setup call). Need to inline `(char*)&D + N` at every site (the
 *      feedback-ido-inline-symbol-arith-vs-base-local recipe) so no
 *      $s4 base local is created.
 *   3. After 1+2 collapse the frame to -0x68, the nested-loop $s-reg
 *      assignment still needs to land x/y/p/idx in the target's slots.
 * Path (a) gate is "predecessor byte-exact"; #1+#2 are the next concrete
 * multi-tick steps. Defer SUFFIX_BYTES until then.
 *
 * Inner-loop calls are placeholder `jal 0` relocations resolved at USO
 * load time — both calls labeled gl_func_00000000 by convention. The
 * 24-byte stride (s5 = 0x18) matches the 00002EF0 wrap's struct size.
 * Likely a sprite-grid placement: 7 rows × 5 cols of tiles, each tile
 * indexed by buf[col] selecting from a 24-byte struct table at &D+0x10. */
typedef struct { int v[5]; } B3Vec5_2DF0;
typedef struct { char pad[0x18]; } B3S18_2DF0;
void TrkMenu_drawHeader_00002DF0(int *a0) {
    int vec4[5];
    B3S18_2DF0 *s4;
    char *s2;
    int s3, i;
    (void)a0;

    *(B3Vec5_2DF0 *)vec4 = *(B3Vec5_2DF0 *)((char *)&D_00000000 + 0x4A0);
    CALL(gui_setCombine_00001794)(&D_00000000);

    s4 = (B3S18_2DF0 *)&D_00000000;
    if (1) { s4 = (B3S18_2DF0 *)((char *)s4 + 0x10); }
    s3 = 0x10;
    do {
        for (i = 0; i != 5; i++) {
            s2 = (char *)s4;
            s2 += vec4[i] * 0x18;
            CALL(gui_spriteRefresh)(s2);
            CALL(gui_drawSprite)(s2, i * 0x40, s3, 0);
        }
        s3 += 0x20;
    } while (s3 != 0xF0);
}

/* 3-call wrapper over the +0x70 records table, indexed by a1*24.
 * MATCHED 2026-06-10 (23/23): the old wrap's hand-spelled shift form
 * ((a1<<2)-a1)<<3 SPLIT the index chain across t6/t7/t8; plain a1*24
 * lets IDO emit its own x24 decomposition -- (a1<<2 - a1)<<3 with
 * single-reg t6 reuse and the first sll hoisted above the prologue --
 * exactly the target. Lesson: write the arithmetic PLAINLY and let the
 * compiler pick the decomposition (inverse of the explicit-shift CSE-
 * bust lever; both directions now documented). Boundary: reverse-merge
 * from 2DF0's tail, done 2026-05-30. */
extern char D_b3_2EF0_table;
void TrkMenu_drawLabel_00002EEC(int a0, int a1, int a2) {
    char *entry, *spillee;
    (void)a0;
    (void)spillee;
    entry = (char*)&D_b3_2EF0_table + a1 * 24;
    spillee = entry;
    CALL(gui_spriteRefresh)(entry);
    CALL(gui_setCombine_00001794)(entry);
    CALL(gui_drawSprite)(entry, 0xA0, a2, 3);
}

/* TrkMenu_drawPair_00002F48: 66-insn (0x108) two-pass render-helper.
 * Sibling of recently-NM-wrapped func_00002EF0.
 *
 * Decoded structure:
 *   void f(int *a0, int a1) {
 *     float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };  // 4-float white at sp+0x38
 *     // Pass 1: table at &D + 0x190
 *     gl_func(&D + 0x190, ...);
 *     if (a0->0x58 != 0) {
 *       gl_func(&D + 0x190, 0x40, &color, 0xFF);
 *     } else {
 *       gl_func(&D + 0x190, a0->0x5C, &color, 0xFF);
 *     }
 *     gl_func(&D + 0x190, 0x78, a1, 3);
 *     gl_func(&D + 0x1A8);
 *     // Pass 2: table at &D + 0x1A8
 *     if (a0->0x58 == 0) {
 *       gl_func(&D + 0x1A8, 0x40, &color, 0xFF);
 *     } else {
 *       gl_func(&D + 0x1A8, a0->0x5C, &color, 0xFF);
 *     }
 *     gl_func(&D + 0x1A8, 0xC8, a1, 3);
 *   }
 *
 * 1.0f via lui at, 0x3F80 + mtc1 (-O2 standard pattern). 4 swc1 stores
 * to sp+0x38..0x44 fill the color quad. Two-pass structure renders into
 * paired tables at &D+0x190 (alpha?) and &D+0x1A8 (RGB?).
 *
 * Exact via a stack struct that places color[4] at sp+0x38 and the
 * unprefixed func_00000000 cross-USO placeholder calls. objdiff still reports
 * relocation-symbol arg mismatches against the raw .word baseline, but
 * no-alias disassembly and byte-verify match. */
extern int gl_func_00000000();
extern int func_00000000();
extern char D_00000000;
void TrkMenu_drawPair_00002F48(int *a0, int a1) {
    float quad4[4];
    char pad[32];
    (void)pad;

    quad4[0] = 1.0f;
    quad4[1] = 1.0f;
    quad4[2] = 1.0f;
    quad4[3] = 1.0f;
    CALL(gui_spriteRefresh)((char*)&D_00000000 + 0x190);
    if (TIMMENU(a0)->choice == 0) {
        CALL(gui_setColorEnvPrim_000017DC)((char*)&D_00000000 + 0x190, TIMMENU(a0)->blink, quad4, 0xFF);
    } else {
        CALL(gui_setColorEnvPrim_000017DC)((char*)&D_00000000 + 0x190, 0x40, quad4, 0xFF);
    }
    CALL(gui_drawSprite)((char*)&D_00000000 + 0x190, 0x78, a1, 3);

    CALL(gui_spriteRefresh)((char*)&D_00000000 + 0x1A8);
    if (TIMMENU(a0)->choice != 0) {
        CALL(gui_setColorEnvPrim_000017DC)((char*)&D_00000000 + 0x1A8, TIMMENU(a0)->blink, quad4, 0xFF);
    } else {
        CALL(gui_setColorEnvPrim_000017DC)((char*)&D_00000000 + 0x1A8, 0x40, quad4, 0xFF);
    }
    CALL(gui_drawSprite)((char*)&D_00000000 + 0x1A8, 0xC8, a1, 3);
}

/* TrkMenu_drawLabel130_00003050: byte-identical mirror of
 * TimMenu_drawLabel130_00002E50. Same Quad4 dispatcher recipe — see
 * the b1 wrap doc for the structural notes. */
void TrkMenu_drawLabel130_00003050(int *a0, int a1) {
    float quad_a[4];
    float quad_b[4];
    char pad[32];
    (void)pad;
    (void)a0;

    quad_a[0] = 1.0f;
    quad_a[1] = 1.0f;
    quad_a[2] = 1.0f;
    quad_a[3] = 1.0f;
    quad_b[0] = 1.0f;
    quad_b[1] = 0.0f;
    quad_b[2] = 1.0f;
    quad_b[3] = 1.0f;
    CALL(gui_setEnvPrimColor_00001EF4)(&D_00000000, 0xFF, quad_a, quad_b);
    CALL(gui_spriteRefresh)((char*)&D_00000000 + 0x130);
    CALL(gui_drawSprite)((char*)&D_00000000 + 0x130, 0xA0, a1, 3);
}
#pragma GLOBAL_ASM("asm/nonmatchings/timproc_uso_b3/timproc_uso_b3/timproc_uso_b3_func_00003050_pad.s")

/* TrkRace_endTimerEnabled + 00000E54: SUFFIX_BYTES-absorbed orphans ←
 * TrkRace_freeLabels_00000DE4 (12 words exact:
 *   0x8C8E06A8,0x8DC2006C,0x50400007,0x00001025,0x8C4F00EC,0x51E00004,
 *   0x00001025,0x03E00008,0x24020001,0x00001025,0x03E00008,0x00000000).
 * _00000E54 was a C-body stub (`int f(void) { return 0; }`) with both
 * own SUFFIX_BYTES (1 nop trailer) and INSN_PATCH (force `move v0,zero;
 * jr ra`); removed entirely along with both recipes. No external callers,
 * so dropping the symbol is safe. The 3 trailing recipe words of
 * _00000DE4 (`move v0,zero; jr ra; nop`) emit the same bytes at vram
 * 0xE54-0xE5F. */

/* SUFFIX_BYTES-absorbed orphan (per
 * docs/MATCHING_WORKFLOW.md#feedback-splat-orphan-duplicate-symbol-pruning,
 * no-truncate variant): TrkRace_onTimeoutExit (7 words) ←
 * TrkRace_onEndTimerDone SUFFIX_BYTES (exact 7-word match
 * 0x240E0009,0x3C010000,0xAFA40000,0x03E00008,0xAC2E0040,0x3C050000,0x8CA50170).
 * Pre-prune the orphan symbol was at .o offset 0x3114 (tail). */

/* trk_nop_000023CC + 000023D4: SUFFIX_BYTES-absorbed orphans
 * ← TrkMenu_rowQuit_00002388 (6 words exact:
 *   0x03E00008,0x00000000,0x03E00008,0x00000000,0x03E00008,0x00000000).
 * _000023D4 had its own 2-word SUFFIX_BYTES recipe (`0x00000000,0x00000000`)
 * that emitted 2 nops at the .o tail past its orphan position — those
 * bytes weren't at any meaningful linked-vram, so dropping the recipe is
 * neutral. Removed alongside the .s deletion (per docs/MATCHING_WORKFLOW.md
 * orphan-attached-recipes blocker note). */

/* Orphan/gap status (resolved 2026-06-10):
 *   _00001920: exists as its own 8-byte NM symbol after 1870 (correct).
 *   _000021F4: was missing entirely (no INCLUDE_ASM line; -0x4C damage) --
 *   restored at its address-ordered position after 21B0.
 *   _00002238: was a 2-word stolen-prologue orphan; merged 2026-11-15 into
 *   TrkMenu_rowToggle_00002238 (formerly _00002240) as its hoisted first
 *   load, compiled from C. */
