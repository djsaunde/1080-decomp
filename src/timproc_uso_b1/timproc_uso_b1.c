#include "common.h"
#include "gdl/backup.h"
#include "game/course.h"
#include "gdl/race.h"
#include "gdl/gl_data.h"
#include "game/nameentry.h"
#include "game/modemenu.h"
#include "game/guimenu.h"
#include "game/input.h"
#include "game/snowboarder.h"
#include "gdl/gadget.h"
#include "game/mode_uso_data.h"
#include "uso_call.h"

extern int gl_func_00000000();
extern char D_00000000;
typedef struct { int a, b, c, d; } Quad4;

/* timproc_uso_b1_func_00000000 / _0000004C (contiguous -O0 run, 0x0..0xB0) live
 * in timproc_uso_b1_o0_0.c — carved into a dedicated -O0 sub-unit and
 * concatenated into the Yay0 block (region 0) before compression. See the
 * timproc_uso_block1_yay0 rule in the Makefile. */

/* tim_main: 317-insn (0x4F4) -O0 state-machine
 * dispatcher: 14-case jumptable switch on arg1, loops re-reading the next
 * state from D+0x40 until a case sets `done`. EXACT 2026-07-09 via the -O0
 * donor timproc_uso_b1_o0_B0.c spliced in through REPLACE_FUNC_BODY (same
 * recipe as 5A4/65C below). This in-TU body is a compile stand-in only —
 * its bytes are replaced by the donor's in build/.../timproc_uso_b1.c.o.
 * The old "rodata jumptable = structural cap" note applied to the -O2
 * in-TU compile; the -O0 donor emits its jumptable at .rodata offset 0,
 * so the lui/lw dispatch words carry in-place addend 0 = the target's raw
 * pre-USO-reloc words. Twin: trk_main (4 immediates
 * differ: case5/case7 second-a3 1->2, case12 mask 0x2000->0x4000). */
extern char D_00000000_raw[]; /* bootup gProcUsoPaths (Data+0xC510 import; the B0 donor's name) */
tim_main(a0, a1) int * a0; int a1; {
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
            gl_func_00000000(d, ((char **)D_00000000_raw)[1]); /* stand-in; the donor's game_loadUso(&GameFlow, gProcUsoPaths[1] = "rom/nameent.uso"): D_00000000_raw is the bootup Data+0xC510 import */
            gl_func_00000000(d, 4, GAMEFLOW(d)->course, 1);
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
            gl_func_00000000(d, 10, GAMEFLOW(d)->course, 1);
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
                NODE(r)->parent = (Node *)d;
            }
            gl_func_00000000(a0, *a0);
            done = 1;
            break;
        case 12:
            s0v = gl_func_00000000(GAMEFLOW(d)->course);
            gl_func_00000000(a0, (GAMEFLOW(d)->course | 0x2000) | s0v, 0x2000, *a0);
            done = 1;
            break;
        case 13:
            s0v = gl_func_00000000(0, 1, 0);
            {
                char *r = (char *)s0v;
                gl_func_00000000(&NODE(d)->children, r);
                if (NODE(r)->parent != 0) NODE(r)->unk04 = 1;
                NODE(r)->parent = (Node *)d;
            }
            done = 1;
            break;
        }
        a1 = GAMEFLOW(d)->procState;
    } while (done == 0);
}

extern int gInputMgr_backup_148;
extern int gInputMgr_session_14C;
/* UNNAMED: data-base D_00000068 = 0x68 is RoData +0x68 in bootup snd_soundObjSetPan_00000B2C but InputMgr +0x68 (import mod0) in the arcproc / timproc b1 / b3 openSession helpers; a pure rename would merge them, it needs a per-site split. */
extern int D_00000068;

/* 46-insn -O0 allocator-wrapper (0xB8). LANDED 2026-05-25 via splice-import-
 * donor-relocs recipe — compiled through donor file timproc_uso_b1_o0_5A4.c,
 * spliced in via REPLACE_FUNC_BODY; the updated splice (commit 8d1b9c93a in
 * scripts/replace-function-body.py) now imports the donor's HI16/LO16/26
 * relocs into the destination .o instead of dropping them. Combined with
 * gInputMgr_backup_148/0x14C/0x68 defined in undefined_syms_auto.txt = their offsets,
 * objdiff resolves the relocs to the same bytes expected/.o has post-link.
 * fuzzy=100. See feedback_splice_import_donor_relocs_recipe + the
 * project_1080_reloc_blind_caps_are_unlockable parent breakthrough.
 *
 * -O0 indicators preserved (the donor IS compiled at -O0):
 *   1. Unfilled jal delay slots, 2. `move s0, v0` save, 3. trailing `b
 *   epilogue; nop` before the epilogue, 4. Frame -0x28 with s0 save. */
void tim_openSession_000005A4(int **arg0, int arg1, int arg2) {
    int handle = CALL(app_setHeapMode)(2);
    register int new_obj = CALL(Backup_openSession)(0, gInputMgr_backup_148, 1, arg2);
    *arg0 = (int*)new_obj;
    gInputMgr_session_14C = new_obj;
    CALL(app_setHeapMode)(handle);
    if (arg1 != 0) {
        ((BackupSession *)*arg0)->modes = 1;
    } else {
        ((BackupSession *)*arg0)->modes = 0;
    }
    D_00000068 = 0;
}

/* 21-insn -O0 helper. Sibling of 0x5A4. RELOC-BLIND CAP (residual 1 insn,
 * NOT byte-exact — corrected 2026-05-31). The donor (timproc_uso_b1_o0_65C.c,
 * spliced via REPLACE_FUNC_BODY) drops relocs to mirror the reloc-blind
 * expected/.o, but the final store `sw s0,0x14C(at)` needs 0x14C baked into the
 * %lo field — and no IDO-emittable C produces a 2-insn scalar store with the
 * offset literal in .text (the offset lives in the reloc-filled %lo; `gInputMgr_session_14C`
 * gives field=0, `&D+0x14C` gives 3 insns at -O0). Real fix = spimdisasm USO-reloc
 * migration (expected/ becomes reloc-aware), per the donor comment + the
 * project's USO-reloc-migration TODO. Distinct from RiderAnim_checkReverseBrake_0000D7F4, which
 * landed only because its 0xE90 baked into an ADDIU (struct-by-value base-
 * materialization), not a load/store %lo. Default build = donor splice. */
void tim_closeSession_0000065C(int *a0) {
    CALL(Backup_closeSession)((int*)a0[0], 3);
    a0[0] = 0;
    *(int*)((char*)&D_00000000 + 0x14C) = 0;
}

extern int D_b1_06B0_a;
extern char *D_b1_06B0_b;
extern int D_b1_06B0_c;
void TimRace_pauseContinue_000006B0(void) {
    CALL(Process_resume_0000FAE8)(D_b1_06B0_a);
    CALL(Node_resumeUpdate_0000E450)(TIMRACE(D_b1_06B0_b)->dataManager);
    CALL(input_clearActivePad_00012090)(&D_b1_06B0_c);
}

extern int D_b1_06F0_a;
extern int D_b1_06F0_b;
extern int D_b1_06F0_c;
void TimRace_pauseRow1_000006F0(void) {
    CALL(Process_resume_0000FAE8)(D_b1_06F0_a);
    CALL(input_clearActivePad_00012090)(&D_b1_06F0_b);
    CALL(RaceProcess_restartCameras_00008510)(D_b1_06F0_c);
}
/* Chain sibling of 0x778, 0x7BC, 0x800. MATCHED via boundary correction
 * (replacing the banned PROLOGUE_STEALS): the leading `lui v0,0; addiu v0,0`
 * (&D_00000000 base for the +0x40/+0x44 stores) was emitted as the predecessor
 * func_000006F0's pad sidecar (stolen prologue); moved it into this fn's .s
 * (now 17 insns / 0x44, starts 0x72C) and removed that pad pragma. ROM bytes
 * preserved. (This fn's OWN pad sidecar below still holds 0x778's orphan.) */
extern int D_state_b1_734;
void TimRace_pauseChangeCharacter(void) {
    *(int*)((char*)&D_00000000 + 0x40) = 8;
    *(int*)((char*)&D_00000000 + 0x44) = 1;
    CALL(Process_fadeOut_0000F954)(D_state_b1_734, -1, 0);
}
/* Chain sibling: t7=2. MATCHED via pad-sidecar boundary correction (orphan
 * lui v0,0; addiu v0,0 moved from func_00000734's pad into this fn's .s;
 * 0x3C->0x44, starts 0x770; pad pragma+.s removed). */
extern int D_state_b1_778;
void TimRace_pauseChangeBoard(void) {
    *(int*)((char*)&D_00000000 + 0x40) = 8;
    *(int*)((char*)&D_00000000 + 0x44) = 2;
    CALL(Process_fadeOut_0000F954)(D_state_b1_778, -1, 0);
}
/* Chain sibling: t7=3. MATCHED via pad-sidecar boundary correction (orphan
 * moved from func_00000778's pad into this fn's .s; 0x3C->0x44, starts 0x7B4). */
extern int D_state_b1_7BC;
void TimRace_pauseChangeCourse(void) {
    *(int*)((char*)&D_00000000 + 0x40) = 8;
    *(int*)((char*)&D_00000000 + 0x44) = 3;
    CALL(Process_fadeOut_0000F954)(D_state_b1_7BC, -1, 0);
}
/* Byte-identical sibling of TrkRace_pauseQuit_00000818. MATCHED via
 * pad-sidecar boundary correction (orphan lui v0,0; addiu v0,0 moved from
 * func_000007BC's pad into this fn's .s; 0x3C->0x44, starts 0x7F8). End of the
 * timproc_uso_b1 chain (no own pad). */
extern int D_state_b1_800;
void TimRace_pauseQuit_00000800(void) {
    *(int*)((char*)&D_00000000 + 0x40) = 8;
    *(int*)((char*)&D_00000000 + 0x44) = 0xD;
    CALL(Process_fadeOut_0000F954)(D_state_b1_800, -1, 0);
}

void tim_streamReadS32_0000083C(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void tim_streamReadF32_00000878(float *dst) {
    float buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void tim_streamReadQuad_000008B4(Quad4 *dst) {
    Quad4 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 16);
    *dst = buf;
}

typedef struct { int a, b, c; } Tri3i;
typedef struct { float x, y, z; } Vec3;
void tim_streamReadVec3_0000090C(Vec3 *dst) {
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

/* TimRace_ctor: 232-insn constructor, near-twin of
 * TrkRace_ctor.
 *
 * 2026-08-22 RETRACTION + redecode: the previous "225/230 (97.8%)" claim
 * was a measurement artifact (target-vs-target compare via the INCLUDE_ASM
 * build .o; see MATCHING_WORKFLOW stale-NM-.o trap). True baseline was
 * 67.2%. Real decode errors fixed this pass (67.2 -> 94.96): return a0 via
 * shared epilogue (not return 0); missing gl(sub) init call in the 0xE0
 * block; fictional sub[0xB4] bgez block removed; missing 2-arg gl(self,a1)
 * call after registration 1; tail registration call is 3-arg (no 4th);
 * single spilled n web (home 52) + volatile sub (home 48, triple-reload
 * signature) + m reload temps; D_b1_097C_g alias kills the &D lui CSE
 * (s1 elimination); D_b1_097C_r read-alias splits the six-call block's
 * *(int*)&D read from the &D 4th arg (defeats base CSE).
 * RESIDUAL (~59 words, mostly renames): as1 t-ring off-by-one from the
 * after_n3 block onward (build starts t6, target t7 — one phantom ugen
 * temp unaccounted); or-copy placement in the 0xE0 guard (build hoists
 * or a0,v0 pre-branch, target sinks it into the jal delay); registration-1
 * def colors a3+copy vs direct a1; vt CSE temp v1-vs-v0 + commutative addu
 * operand order. 
 * EXACT 2026-10-20 (agent-c run 112): rewritten on the b3 0994 / mgrproc
 * E04 skeleton (nested `oN = oN-1; if (oN-1 || (oN = alloc(n)))` inlined
 * base ctors, void registration alias gl_func_00000000_994v, no 0xB4
 * block, alloc 0xE0). The memory-resident o2 (home 48) is NOT volatile:
 * it is a PLAIN local whose web is spilled around the calls (home 48 is
 * its spill slot). Within a block uopt still uses the just-stored value
 * (the 0xE0 guard tests v0 and passes v0 in the jal delay), and dropping
 * volatile also removes its extra temp (the t-ring phantom) and the two
 * extra frame words. (A `(void)&o2;` statement is a no-op in IDO cfe --
 * the run-112 commit message calling it "address-taken" was wrong.) */
extern int gl_func_00000000();
extern char D_b1_097C_v0;
extern char D_b1_097C_v1;
extern char D_b1_097C_v2;
extern char D_b1_097C_v3;
extern char D_b1_097C_v4;
extern char D_b1_097C_g;
extern char D_b1_097C_r;
extern void gl_func_00000000_994v();
int TimRace_ctor(char *obj, int a1, int a2, int a3) {
    char *o1;
    char *o2;
    char *o3;
    char *w;

    if (obj != 0 || (obj = (char *)CALL(mem_new)(0x730)) != 0) {
    o1 = obj;
    if (o1 != 0 || (o1 = (char *)CALL(mem_new)(0x6A8)) != 0) {
        o2 = o1;
        if (o1 != 0 || (o2 = (char *)CALL(mem_new)(0x50)) != 0) {
            o3 = o2;
            if (o3 != 0 || (o3 = (char *)CALL(mem_new)(0x2C)) != 0) { CALL(Node_ctor)(o3, (char *)&D_00000000 + 0x3B8); NODE(o3)->vptr = (char *)&D_b1_097C_v0; }
            NODE(o2)->vptr = (char *)&D_b1_097C_v1;
        }
        NODE(o1)->vptr = (char *)&D_b1_097C_v2;
        CALL(ColourParms_ctor)(&RACEPROC(o1)->colours);
    }
    NODE(obj)->vptr = (char *)&D_b1_097C_v3;
    RACEPROC(obj)->spriteSet = 0;
    CALL(RaceProcess_init)(obj, a1, (char *)&D_00000000 + 0x3C0, a2);
    RACEPROC(obj)->session = (BackupSession *)a3;
    CALL(TimRace_initLabels_00000D1C)(obj);
    TIMRACE(obj)->recordFade = 0.0f;
    CALL(memtrack_begin)((char *)&D_00000000 + 0x3D0, 0);
    CALL(input_setActivePad_0001207C)(&D_00000000, 0);

    {
        o2 = (char *)CALL(mem_new)(0xE0);
        if (o2 != 0) {
            CALL(RaceTracker_ctor)(o2);
            NODE(o2)->vptr = (char *)&D_b1_097C_v4;
        }
        TIMRACE(obj)->dataManager = (RaceTracker *)o2;
        INPUT_MGR(D_b1_097C_g).dataMgr = (struct RaceTracker *)o2;

        XCALL(gl_func_00000000_994v, TimDataManager_init_00001A64)(TIMRACE(obj)->dataManager, obj, RACEPROC(obj)->spriteSet, (int)RACEPROC(obj)->session);

        w = (char *)TIMRACE(obj)->dataManager;
        o1 = (char *)NODE(w)->vptr;
        ((void (*)(int))VT_SLOT(o1, 10).fn)(VT_SLOT(o1, 10).delta + (int)w);

        o1 = (char *)TIMRACE(obj)->dataManager;
        CALL(list_append)(obj + 16, o1);
        if (NODE(o1)->parent != 0) {
            NODE(o1)->unk04 = 1;
        }
        NODE(o1)->parent = (Node *)obj;

        CALL(RaceProcess_restart)(obj, a1);

        if ((RACEPROC(obj)->flags << 15) < 0) {
            RACEPROC(obj)->menu = (GuiMenu *)CALL(PauseMenu_create)(0);
            CALL(gui_initRows_000100F0)(RACEPROC(obj)->menu, obj);
            RACEPROC(obj)->menu->unk30 = RACEPROC(obj)->spriteSet;
            CALL(gui_addRowSub_00010B6C)(RACEPROC(obj)->menu, (*(int *)&D_b1_097C_r + 3) << 16, -1, &D_00000000);
            CALL(gui_addRowSub_00010B6C)(RACEPROC(obj)->menu, ((*(int *)&D_b1_097C_r + 3) << 16) | 1, -1, &D_00000000);
            CALL(gui_addRowSub_00010B6C)(RACEPROC(obj)->menu, ((*(int *)&D_b1_097C_r + 3) << 16) | 4, -1, &D_00000000);
            CALL(gui_addRowSub_00010B6C)(RACEPROC(obj)->menu, ((*(int *)&D_b1_097C_r + 3) << 16) | 3, -1, &D_00000000);
            CALL(gui_addRowSub_00010B6C)(RACEPROC(obj)->menu, ((*(int *)&D_b1_097C_r + 3) << 16) | 2, -1, &D_00000000);
            CALL(gui_addRowSub_00010B6C)(RACEPROC(obj)->menu, ((*(int *)&D_b1_097C_r + 3) << 16) | 5, -1, &D_00000000);
            CALL(gui_centerRows_0001016C)(RACEPROC(obj)->menu);
            o1 = (char *)RACEPROC(obj)->menu;
            CALL(list_append)(obj + 16, o1);
            if (NODE(o1)->parent != 0) {
                NODE(o1)->unk04 = 1;
            }
            NODE(o1)->parent = (Node *)obj;
        } else {
            RACEPROC(obj)->timeout = a1 & 0xFFFF;
            RACEPROC(obj)->menu = 0;
        }

        o1 = *(char **)((char *)&D_00000000 + 0x190);
        CALL(list_append)(obj + 16, o1);
        if (NODE(o1)->parent != 0) {
            NODE(o1)->unk04 = 1;
        }
        NODE(o1)->parent = (Node *)obj;
        CALL(Fader_start)(*(int **)((char *)&D_00000000 + 0x190), 1, 0);
        CALL(memtrack_end)();
    }
    }
    return (int)obj;
}

/* 33-insn 5-call dispatcher: each call passes a different a0+offset and
 * a packed (high16|low16) constant. First two calls OR in the high half
 * with values loaded from D[0x4C]/D[0x54]; last three use pure constants
 * (0x0021000A / 0x0021000D / 0x00210009). */
extern int gl_func_00000000();
void TimRace_initLabels_00000D1C(TimRace *a0) {
    CALL(gui_spriteInit)((char *)&a0->labels[0], *(int*)((char*)&D_00000000 + 0x4C) | 0x001D0000);
    CALL(gui_spriteInit)((char *)&a0->labels[1], *(int*)((char*)&D_00000000 + 0x54) | 0x001E0000);
    CALL(gui_spriteInit)((char *)&a0->labels[3], 0x0021000A);
    CALL(gui_spriteInit)((char *)&a0->labels[4], 0x0021000D);
    CALL(gui_spriteInit)((char *)&a0->labels[2], 0x00210009);
}

void TimRace_freeLabels_00000DA0(TimRace *a0) {
    CALL(gui_spriteRelease)((char *)&a0->labels[0]);
    CALL(gui_spriteRelease)((char *)&a0->labels[1]);
    CALL(gui_spriteRelease)((char *)&a0->labels[3]);
    CALL(gui_spriteRelease)((char *)&a0->labels[4]);
    CALL(gui_spriteRelease)((char *)&a0->labels[2]);
}

/* TWIN-LEVER 2026-06-21: cracked via b3_func_00001184 twin shape. The old
 * "regalloc-renumber cap, a3-reuse vs t7/t9/t8 fresh" note was WRONG: the
 * lever was `int *a0` array-index form (a0[off/4]) instead of `char *a0`
 * pointer arith — array indexing keeps the post-call self web memory-
 * resident, forcing fresh reload temps that match the target. Byte-exact. */
int TimRace_endTimerEnabled(TimRace *a0) {
    if (a0->race.unk4FC == 0) {
        CALL(hud_loadRecords_0000CE38)((int *)a0->dataManager, 0, 1);
        CALL(RaceTracker_flagReplayEnd_0000D090)((int *)a0->dataManager);
        a0->race.unk4FC = 1;
    }
    return 1;
}

/* 32-insn dispatcher: lookup-call gates state set on D[0x40]/D[0x44]
 * then ALWAYS runs the t9 check (a0->[0x4F8] -> store-or-call).
 * v0!=0: D[0x40]=12, D[0x44]=9; v0==0: D[0x40]=9 (no D[0x44] write). */
void TimRace_onEndTimerDone(TimRace *a0) {
    int v0 = CALL(Backup_sessionHasNew)(a0->race.session);
    if (v0 != 0) {
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

void TimRace_onAltEndTimerDone(int a0) {
    CALL(sys_fatal)(&D_00000000 + 0x3D8);
    (void)a0;
}

/* TimRace_onRaceOver: 118-insn state-machine dispatcher, twin of
 * TrkRace_onRaceOver (same skeleton; b1 state-0 adds the 3-byte
 * [5]/[6]/[7] compare ladder and reads work->[0x30] not [0x44]->[0x14]).
 * Old "2-FUNCTION BUNDLE" note obsolete — splat boundary now splits 10C0.
 *
 * 2026-07-23: 78.8 -> 99.92 via the b3 0E60 structural kit:
 *  - goto dispatch (state==0 -> state0; state!=1 -> return; -> state1)
 *    = beql/beq/b entry shape with lw v0,0x6A8 in the likely delay;
 *  - volatile int *base = &D: s4 base held + reloaded per use (d[0x64]
 *    per loop iter, d[0x190] per call site);
 *  - `state` REUSED as the loop counter -> colors s0 (fresh local = v0);
 *  - const-1 s3 web (entry compare + 0x4D8/0x504 stores + created[1]=1,
 *    li s3,1 remats in the bltzl/bnel likely delays);
 *  - call-arg spelled mult-first (base[0x19]*0x30 + *(int*)*(0x528 ptr))
 *    -> target ugen temp numbering (table-deref t0 after base t1).
 * 2026-07-23 final 2 words (lw/sw a1-vs-t8) closed, BYTE-EXACT: the
 * [0x44]->[0x38] value is ALSO the first call's SECOND ARG, spelled as a
 * store-forward read-back `*(int*)(s2+0x6B0)` — forwarding coalesces the
 * lw temp into a1 while its ring slot (t8) stays allocated (t9 for 0x528
 * ptr preserved). CRITICAL: the volatile base[0x64/4] read in arg1 is a
 * store-forward BARRIER (read-back reloads = extra insn); cast it away at
 * this one site (`*(int*)((int)base + 0x64)`) — the loop site keeps the
 * volatile spelling. Named-n-as-arg2 also colors a1 but FREES the ring
 * slot (whole downstream ring shifts -1 = worse).
 * Unwrapped EXACT 2026-10-16 (run 96): the block is cut from the unlinked
 * .o, so the placeholder callees/data stay 0 like the ROM. */
void TimRace_onRaceOver(char *a0arg) {
    volatile int *base;
    TimRace *s2 = (TimRace *)a0arg;
    int state = RACEPROC(a0arg)->unk504;
    char *work;
    char *s1;
    char *r;
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
    work = (char *)s2->dataManager;
    base = (volatile int *)&D_00000000;
    s2->finalTime = DATAMGR(work)->elapsed;
    s2->recordRank = DATAMGR(work)->entries->rank[0];
    s1 = (char *)CALL(Backup_courseRecordGetTime)(((InputMgr *)base)->course * 0x30 + *(int *)s2->race.session, s2->recordRank); /* base = the GameFlow import (bootup Bss+0xF8); a plain (non-volatile) read, as the target */
    for (state = s2->recordRank - 1; state >= 0; state--) {
        r = (char *)CALL(Backup_courseRecordGetTime)(base[0x64 / 4] * 0x30 + *(int *)s2->race.session, state);
        if (((BackupTimeEntry *)s1)->time[0] != ((BackupTimeEntry *)r)->time[0]) break;
        if (((BackupTimeEntry *)s1)->time[1] != ((BackupTimeEntry *)r)->time[1]) break;
        if (((BackupTimeEntry *)s1)->time[2] != ((BackupTimeEntry *)r)->time[2]) break;
        s2->recordRank = s2->recordRank - 1;
    }
    s2->race.subState = 1;
    CALL(Fader_start)(base[0x190 / 4], 3, 1);
    s2->race.unk504 = 1;
    return;

state1:
    base = (volatile int *)&D_00000000;
    if (CALL(Fader_checkDone_00013924)(base[0x190 / 4]) == 0) {
        return;
    }
    CALL(TimRace_freeLabels_00000DA0)(s2);
    CALL(GameFlow_releaseFiles_00012110)(base);
    CALL(RaceProcess_createReplayCam)(s2);
    CALL(RaceProcess_restartWithTimeout)(s2, base[0x170 / 4] + 0x00220000);
    s2->race.nameEntry = CALL(TimNameEntry_ctor_000016F8)(0, s2, 0);
    CALL(NameEntry_init)(s2->race.nameEntry, s2->race.session); /* UNNAMED: unknown-role jal -> nameent.uso Text+0x18 (TextReloc import); nameent.uso is not split, so the callee has no symbol: called on the new mode NameEntry with (nameEntry, session) right after its ctor */
    created = (int *)s2->race.nameEntry;
    link = (char *)s2->race.handler;
    CALL(list_append)(link + 0x10, created);
    if (created[0x14 / 4] != 0) {
        created[1] = 1;
    }
    created[0x14 / 4] = (int)link;
    CALL(Fader_start)(base[0x190 / 4], 1, 1);
}

void TimRace_onTimeoutExit(int a0) {
    *(int *)((char *)&D_00000000 + 0x40) = 9;
}

/* TimRace_onTimeout (0x2C, 11 insns): hoisted-head merge of 10D4 +
 * 10DC (2026-09-05 agent-c, TWENTY-FIFTH mis-split class, see
 * docs/MATCHING_WORKFLOW.md #hoisted-head-119c-11a4-mgrproc). The 8-byte
 * "pre-load fragment" `lui a1; lw a1,0x170(a1)` is this function's own first
 * statement scheduled above `addiu sp` by IDO 7.1 -O2; 10DC was never an entry.
 * Oracle = the timproc.uso Kyoto tables (inner header ROM 0x5AF114): Sym[89]
 * EXPORTS text offset 0x10D4 and DataReloc @0x33C stores its address in a
 * function-pointer table; 0x10DC has no export and no R_MIPS_26. The old 10DC
 * "exact" existed only via a fake `char *a1` parameter (episode retired).
 * Twin: TrkRace_onTimeout (trkproc.uso). */
void timproc_uso_b1_func_00000000();

void TimRace_onTimeout(int a0) {
    XCALL(timproc_uso_b1_func_00000000, RaceProcess_restartWithTimeout)(a0, *(char **)((char *)&D_00000000 + 0x170) + 0x00220000);
}

extern int gInputMgr_procState_40;
extern int sTimPauseMenu_208;
extern int sTimPauseProc_20C;

void TimRace_onQuitButton(int a0) {
    *(int *)((char *)&D_00000000 + 0x40) = 9;
    CALL(Process_fadeOut_0000F954)(a0, -1, 0);
}

/* Vtable-entry dispatcher (twin of TrkRace_onPauseSelect): gate on
 * gl_func(D[0x190]), reload the sub-object/count after the refresh call, then
 * indirect-call the fn-ptr at entry+0x90. */
extern int D_arg_b1_1130[];
extern int D_cur_b1_1130;
/* EXACT (2026-06-21, decomp-permuter). The single residual addu operand-order
 * diff (target `addu rd,base,prod` vs the inline-web's `addu rd,prod,base`) is
 * resolved by two scheduler/coloring nudges that the permuter discovered:
 *   - `self[(0x48 / 4) ^ 0]` (identity XOR) re-orders the index web so the
 *     final addr addu emits base-rs while preserving the $t9 coalesce, and
 *   - `(new_var = 0x90)` parks the +0x90 offset in a throwaway local, which
 *     fixes the temp-numbering of the call register.
 * Both are semantic no-ops; verified in-tree (0 non-reloc word diffs, 40/40
 * words, reloc records at the 6 target offsets). */
void TimRace_onPauseSelect(int *self) {
    int *v0;
    int new_var;
    int v1;
    int stride;
    if (CALL(Fader_isDone_00013980)(D_arg_b1_1130[0x190 / 4]) == 0) {
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
        D_cur_b1_1130 = (int) self;
        if (1) {
        }
        v0 = (int *) self[(0x48 / 4) ^ 0];
        (*((void (**)(void)) ((((char *) v0) + (v0[0x7C / 4] * stride)) + (new_var = 0x90))))();
    }
}

/* TimRace_drawResultExtra (0x170, 92 insns): hoisted-head merge of 11D0 +
 * 11D8 (2026-09-05 agent-c, TWENTY-FIFTH mis-split class, see
 * docs/MATCHING_WORKFLOW.md #hoisted-head-119c-11a4-mgrproc). The 8-byte
 * "float-constant preload" `lui at,0x3F80; mtc1 at,$f0` is this function's own
 * `buf[i] = 1.0f` materialization scheduled above `addiu sp` by IDO 7.1 -O2
 * (the build's own first word was already above the prologue). Oracle = the
 * timproc.uso Kyoto tables (inner header ROM 0x5AF114): Sym[96] EXPORTS 0x11D0
 * and DataReloc @0x374 stores its address in a function-pointer table; 0x11D8
 * has no export and no R_MIPS_26. The old 11D8 "HARD CAP" (extra
 * `lwc1 $f0,40(sp)` home load, frame 0x68 vs 0x60) was an artifact of faking
 * the hoisted constant with an uninitialized `register float`.
 * MERGED STATE: 89/92 words (objdiff fuzzy 99.75). Frame 0x60 exact: buf at
 * the top (0x50-0x5C), 7 volatile pad words, call-arg spill home 0x24, dead
 * word 0x20. The old `char *s0 = a0` two-web local is GONE: IDO does the
 * a0/s0 split itself (clamp on a0, rest on s0, same words) and the extra
 * pointer local was what displaced the spill home to 0x20 (7 pads + s0 ->
 * saved 0x20; 8 pads + s0 -> saved 0x24 but frame 0x68).
 * RESIDUAL (3 words, both documented version-independent caps, see the
 * 1C74 wrap + docs/IDO_CODEGEN FP-const-hoist / 2E3C entries):
 *   +0x078 add.s f16 operand order (target field+const, build const+field);
 *   +0x0E8/+0x0EC the 255.0f mtc1 / field lwc1 register pair-swap (target
 *   allocates the constant first: mtc1 at,$f18; lwc1 $f4 -- build gives the
 *   loaded value the first ring reg in every probed spelling). Honest NM. */
/* Decode history (as timproc_uso_b1_func_000011D8, 82.34 -> 98.61, 2026-07-15 agent-g): the
 * three "deferred decode corrections" applied together, plus THREE MORE
 * decode errors found in the bytes:
 *  (1) D<=0 gate is the body-WRAP form (bc1fl skips body when D>0), not an
 *      early return;
 *  (2) &D+0x138 chain is p1=*(D+0x138); p2=*(p1+0x44); v=*(p2+0x38);
 *  (3) dead 4-float buffer at sp+0x50 storing garbage $f0 (see cap below);
 *  (4) `saved` = s0+0x6E4 (the CALL ARG, homed at sp+0x24) — NOT the first
 *      call's result; all three guarded calls pass the same pointer;
 *  (5) the guard-false call is gl(s0, 140, obj->0x30) — a1=140 SAME as the
 *      counter&8 call (the old `0` was invented; target hoists its li a1,0x8C
 *      to the clamp region since no call clobbers it on that path);
 *  (6) tail structure: guard-false -> one call; guard-true & !(0x508&8) ->
 *      NO further call (straight to epilogue).
 * Levers: per-site alias externs D_b1_11d8_a/_b/_c/_d KEEPING the +0/0x40/
 * 0x44/0x138 offsets in the C (target imms are baked; a bare distinct-float
 * extern gives imm 0 = wrong bytes); s0=a0 assigned AFTER the clamp so the
 * a0/s0 webs don't coalesce (clamp reads/writes a0, rest s0); explicit
 * flag=0/if(&&&&)flag=1 guard (matches or-v0-zero + li-v0-1, no sltu);
 * buf-first + volatile pad layout pins buf to sp+0x50 and saved-home to 0x24.
 * RESIDUAL (documented HARD CAP, docs/IDO_CODEGEN "quad-store family" /
 * timproc 19C0 2026-06-10): the target stores garbage $f0 4x with NO home
 * load; IDO 7.1 always homes an uninitialized float (extra lwc1 $f0,40(sp)
 * = 91 vs 90 insns, frame -0x68 vs -0x60). Plus one canonical commutative
 * word: add.s emits fresh-load-first (f10,f0) vs target (f0,f10), both
 * spellings tested. Floor 98.61, honest NM. */
extern char D_b1_11d8_a, D_b1_11d8_b, D_b1_11d8_c, D_b1_11d8_d;
/* EXACT 2026-10-16 (agent-c): both "version-independent" FP caps closed by
 * the assignment-expression operand lever (docs/IDO_CODEGEN.md
 * #feedback-ido-fp-commutative-operand-order-assignment-lever):
 * `K + (ft = fv)` puts fv as add.s fs, `field * (fv = 255.0f)` gives the
 * constant the later ring reg ($f18, field $f4). The new float local ft costs
 * one stack word, so ONE volatile pad is dropped (7 -> 6) to keep the call-arg
 * spill home at sp+0x24. */
void TimRace_drawResultExtra(TimRace *a0) {
    float buf[4];            /* dead color buffer, sp+0x50..0x5C (frame top) */
    volatile int pad0, pad1, pad2, pad3, pad4, pad5;
    char *saved;             /* call-arg spill home sp+0x24 */
    int flag;
    float fv;
    float ft;

    buf[0] = 1.0f;
    buf[1] = 1.0f;
    buf[2] = 1.0f;
    buf[3] = 1.0f;
    if (a0->race.leave == 0) return;
    if (*(float *)&D_b1_11d8_a <= 0.0f) {
        fv = a0->recordFade;
        if (fv < *(float *)((char *)&D_b1_11d8_b + kRoF_tim_recordFadeMax_40)) {
            a0->recordFade = *(float *)((char *)&D_b1_11d8_c + kRoF_tim_recordFadeIn_44) + (ft = fv);
        }
        a0->race.unk508 = a0->race.unk508 + 1;
        flag = 0;
        if ((a0->race.session->modes & BACKUP_MODE_TIME) &&
            (a0->race.unk4FC != 0) &&
            (INPUT_MGR(D_b1_11d8_d).dataMgr->entries->rank[0] < 3)) {
            flag = 1;
        }
        if (flag != 0) {
            saved = (char *)&a0->labels[2];
            CALL(gui_spriteRefresh)(saved);
            CALL(gui_setEnvPrimColor_00001EF4)(saved, (int)(a0->recordFade * (fv = 255.0f)),
                             (char *)a0 + 0x2F0, (char *)a0 + 0x314);
            if (a0->race.unk508 & 8) {
                CALL(gui_drawSprite)(saved, 160, 160, 3);
                CALL(tim_drawTime_00001340)(a0, 140, DATAMGR(a0->dataManager)->elapsed);
            }
        } else {
            CALL(tim_drawTime_00001340)(a0, 140, DATAMGR(a0->dataManager)->elapsed);
        }
    }
}

/* EXACT via an IDO 7.1 NCC (C++) donor: src/timproc_uso_b1/timproc_uso_b1_ncc_1340.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder (agent-g run 198, kit CLXI / CLXIII). The history
 * below describes the earlier cfe decode (removed). */
void tim_drawTime_00001340(char *arg0, s32 arg1, s32 arg2) {
    volatile s32 r = arg1;
}
/* tim_drawTime_00001340: 238-insn timer-HUD draw (min:sec.centis).
 * 2026-07-23 redecode 44.96 -> ~70 (frame 336 EXACT, prologue 18/18, div
 * chain + call skeleton aligned). Decode errors fixed vs old m2c body:
 *  - first call a0 = &D_00000000 (baked USO reloc lui/addiu), was literal 0;
 *  - format arg = (char*)&D_00000000 + 0x3F4 (reloc), was literal 0x3F4;
 *  - time split is an ARG-MUTATION chain (min=arg2/60000; arg2-=min*60000;
 *    sec=arg2/1000; arg2-=sec*1000; arg2/=10;) — this pins the divisions
 *    BEFORE call 1 (move s0,a2 at entry; a named-temp chain gets
 *    forward-substituted/sunk to the sprintf call; if(1){} does NOT stop
 *    the sink);
 *  - loop glyph load is lbu (u8), was *(int*);
 *  - saved64/saved60 widget-ptr copies at the strlen call (the 100/96
 *    stores) reloaded by the tail draws — restores frame to 336.
 * 2026-07-30 76.5 -> 84.44: dead kills `w6CC = 0; w6B4 = 0;` right after
 * the saved64/saved60 copies UN-COALESCE the widget-ptr webs from the
 * frame-homed saved copies (baseline coalesced both into one memory web
 * at 92/88 -> per-use lw $4 reloads).  Cascade recovered: 13 no longer
 * registerized (multu 5 -> 3; both count*13 strength-reduce
 * sll/subu/sll/addu like target), buf -> s8, count demoted off s6,
 * size 223 -> 229 (target 238).  Kill placement is load-bearing:
 * before-strlen == after-strlen (84.44), at tail = DCE'd early ->
 * coalescing returns (76.3).
 * Remaining (~35 words): s-reg ROTATION — ours s1..s8 =
 * w6CC,w6B4,glyph,w6FC,arg0,min,sec,buf; target =
 * w6FC,min,glyph,sec,arg0,w6CC,w6B4,buf (uoptlist: candidates
 * 125/128 (w6CC/w6B4) color 4th/5th, need 7th/8th; min/sec are
 * late-created webs 179/180 that must outrank glyph/arg0).  Probed
 * and FAILED: while(0) ref-boost of w6FC/arg2/sec (re-registerizes 13,
 * 78.9); dead if(ptr){} EMITS beq for ptr-arith vars; decl reorder,
 * dead pre-defs at block entry = no-ops.  FP temp ring f4/f6 (+f16/f18,
 * f8/f10) const-vs-load numbering: cast-deref rank beats textual order;
 * one-level typed-member spelling and (f32)255 cast-literal both no-op
 * (two-level chain unavailable — direct member).  Frame 328 vs 336
 * (one[] sits 312-324 vs target 320-332; the 8B slot pair returns if
 * the rotation is fixed?).
 * 2026-10-24 (agent-g run 160) 85.58 -> 85.69, 229 -> 233 of 238 words: the
 * glyph loop is an INDEX loop `for (i = 0; i < count; i++)` over buf[i] with
 * x = pos6 + i * 13 (uopt strength-reduces both and LFTRs the test to
 * i*13 != count*13, but keeps count live in a caller-saved register spilled
 * around every call -- the target's t1 / sp+152 juggle); padq dropped (frame
 * stays 336); sum order count*13 + pt4 + pt2 + pt3 (target addu order). */

/* TimNameEntry_ctor_000016F8 — EXACT (90/90 words incl. relocs, 2026-07-09).
 * Twin of ArcNameEntry_ctor_0000199C (same 4-level alloc-cascade ctor
 * template; deltas: init-call base +0x404, a1 float @+0x72c,
 * packed-id ptr field @+0x6b0). Crack lever: the packed-id call's id
 * chain must be ucode-created before the ptr chain — spell the ptr chain
 * as a two-level typed member chain ((TimRace *)MODENAMEENT(arg0)->race)->recordRank (cfe then
 * follows textual order). Siblings: arcproc 199C, timproc_b3 1660, mgrproc 2940. */
extern int gl_func_00000000();
extern char D_tim16F8_v[];   /* init-call a1 base (&sym+0x404) */
extern char D_tim16F8_w[];   /* a0->0x28 (level-4) */
extern char D_tim16F8_x[];   /* v1->0x28 (level-3) */
extern char D_tim16F8_y[];   /* a2->0x28 (level-2) */
extern char D_tim16F8_z[];   /* s0->0x28 (level-1/main) */
extern int D_tim16F8_id;     /* packed-id value read */


char *TimNameEntry_ctor_000016F8(char *arg0, char *arg1, int arg2) {
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
                    CALL(Node_ctor)(a0, D_tim16F8_v + 0x404); NODE(a0)->vptr = (char *)D_tim16F8_w;
                }
                NODE(v1)->vptr = (char *)D_tim16F8_x;
            }
            NODE(a2)->vptr = (char *)D_tim16F8_y;
        }
        NODE(arg0)->vptr = (char *)D_tim16F8_z;
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
            ((D_tim16F8_id + 0x23) << 16) | (((TimRace *)MODENAMEENT(arg0)->race)->recordRank + 7));
    }
    return arg0;
}

void TimNameEntry_update_00001860(int *a0) {
    if (CALL(Fader_isDone_00013980)(*(int*)(&D_00000000 + 0x190)) == 0) return;
    if (CALL(input_checkPressed_00011E00)(&D_00000000, 0x40100) == 0) return;
    if (CALL(Backup_sessionHasNew)(MODENAMEENT(a0)->base.entry) != 0) {
        MODENAMEENT(a0)->base.phase = 1;
    } else {
        CALL(Process_fadeOut_0000F954)(a0, -1, 0);
    }
}

void TimNameEntry_drawEntryOverlay(char *a0) {
    char *p;
    CALL(TimNameEntry_drawTitle_000019C0)(a0);
    p = (char *)MODENAMEENT(a0)->race;
    CALL(tim_drawTime_00001340)(p, 0x8C, TIMRACE(p)->finalTime);
}

/* Decay-tick handler: decrement sub->f_72C by the global rate (D+0x48),
 * clamp at 0, fire the 140-event mid-call, and on reaching <= 0 fire
 * the threshold call. MATCHED 2026-06-10 (44/44): the old wrap had a
 * DECODE ERROR (the mid-call's arg0 is SUB, not self) and a misplaced
 * sub-reload (only the clamp-taken path reloads); the final read goes
 * through the inlined full chain (fresh temp t6) rather than the sub
 * variable (whose web would color a0). bc1fl delay structure falls out
 * naturally from the val-carried if/if shape. */
void TimNameEntry_drawExitOverlay(int *self) {
    char *base = &D_00000000;
    int *sub;
    float *fp;
    float val;

    CALL(TimNameEntry_drawTitle_000019C0)(self);
    sub = (int *)MODENAMEENT(self)->race;
    fp = (float *)((char *)sub + 0x72C);
    val = *fp;
    if (val > 0.0f) {
        *fp -= *(float *)(base + kRoF_tim_recordFadeOut_48); /* timproc RoData section base + named offset */
        sub = (int *)MODENAMEENT(self)->race;
        if (TIMRACE(sub)->recordFade < 0.0f) {
            TIMRACE(sub)->recordFade = 0.0f;
            sub = (int *)MODENAMEENT(self)->race;
        }
        CALL(tim_drawTime_00001340)(sub, 140, TIMRACE(sub)->finalTime);
        val = TIMRACE(MODENAMEENT(self)->race)->recordFade;
    }
    if (val <= 0.0f) {
        CALL(NameEntry_drawExitOverlay)(self); /* UNNAMED: unknown-role jal -> nameent.uso Text+0x718 (TextReloc import); nameent.uso is not split, so the callee has no symbol: called with the NameEntry once its exit-overlay fade reaches 0 */
    }
}
/* TimNameEntry_drawTitle_000019C0: historical name, true entry 0x19B8.
 * EXACT 2026-09-09: the ROM's timproc.uso Sym163 exports 0x19B8 and
 * TextReloc sites 0x18DC/0x1910 call it; 0x19C0 is not an entry. Fold
 * the two-word orphan into this function's assembly boundary. IDO -O2
 * hoists buf[]'s 1.0f materialization above the prologue itself.
 * The complete 43-word function is identical to b3_func_00001920.
 * field * (scale = 255.0f) puts the constant in f4 before the field
 * load into f6; scale's home + pad[0x1C] preserve the 0x58 frame,
 * buf at sp+0x48 and tgt spill at sp+0x20. Separate assignment or
 * reversing these multiply operands loses the exact FP setup order.
 * Verified against original ROM Text, not the stored-asset ROM link. */
void TimNameEntry_drawTitle_000019C0(int *a0) {
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
/* TimDataManager_init_00001A64 — verified structural decode (89-insn
 * init+configure constructor; 8 gl_ calls + branch-likely list-link tail
 * = documented constructor sub-80 ceiling → INCLUDE_ASM build path;
 * struct-typing reference).
 * void f(int *a0, int a1, int a2, int a3){
 *   s0 = a0;
 *   a0->0xC  = &D_00000000 + 0x40C;
 *   a0->0xB8 = a1;  a0->0x50 = a1;  a0->0x4C = a3;
 *   a0->0xAC = 0;   a0->0xD8 = 0;   a0->0x54 = a2;
 *   a0->0xD0 = 255; a0->0xD4 = 0;   a0->0xDC = 1;
 *   *(float*)(a0+0xCC)=*(float*)(a0+0xC8)=*(float*)(a0+0xC4)=
 *     *(float*)(a0+0xC0)=1.0f;
 *   if (((int*)a1)->0x4F0 << 15 >= 0) {        // bgezl on (a1->0x4F0)<<15
 *     gl_func_00000000(s0, 232, 19, s0->0x44 + 16);     // s0->0x44 = a3 home
 *     gl_func_00000000(s0, 291, 225, 1);
 *     gl_func_00000000(s0, 71, 19, s0+0x30);
 *     gl_func_00000000(s0, 68, 34, s0->0x44 + 40);
 *     // table lookup: t3=*(int*)(&D+0x64); base=*(int*)s0->0x4C;
 *     gl_func_00000000(base + (t3*0x3C)*16 ...);          // (t3*4-t3)<<4 = t3*0x30
 *     v0 = gl_func_00000000(s0->0x60); s0->0xBC = v0;
 *     gl_func_00000000(v0, v1[5], v1[6], v1[7]);          // v1 = prev ret
 *     gl_func_00000000(s0->0xBC, 75, 214);
 *     gl_func_00000000(s0+0x10, s0->0xBC);
 *     a1x = ...; if (a1x->0x14 == 0) a1x->0x4 = 1;
 *     a1x->0x14 = s0;                                     // list link
 *   }
 * }
 * Struct-typing: a0 widget/element — ptr @0xC (&D+0x40C), @0xB8/@0x50
 * both = a1 (parent?), @0x4C = a3, @0x54 = a2, flags @0xAC/@0xD8/@0xD4
 * zeroed, @0xD0 = 255 (alpha), @0xDC = 1, float quad @0xC0..@0xCC = 1.0f
 * (scale/color), @0x44 sub-region base, @0x60 handle, @0xBC built child,
 * @0x30 inline sub-struct, @0x10 another. a1->0x4F0 bit-31-after-<<15
 * gates the whole configure block. Linked into a1x->0x14 list (sets
 * ->0x4=1 on first).
 *
 * RETRACTED 2026-06-22 (agent-e): the earlier "byte-exact" land was a FALSE
 * MATCH. The build .text is NOT byte-identical: decompressing the BUILT
 * timproc_uso_block1 and diffing against assets/timproc_uso_block_1.bin shows
 * 4 real spill-slot word diffs in this function:
 *     +0xf0 sw v0,0x20(sp)  vs build 0x30(sp)
 *     +0xfc lw v1,0x20(sp)  vs build 0x30(sp)
 *    +0x134 sw a1,0x34(sp)  vs build 0x24(sp)
 *    +0x138 lw a1,0x34(sp)  vs build 0x24(sp)
 * The "verify-blocks other=0" gate was FOOLED: spill-slot offset diffs live in
 * the lo16 field (sw/lw immediate), so verify-yay0-blocks.classify() buckets
 * them as reloc-class "lo16" instead of "other". And `make verify` ROM-OK is
 * irrelevant for Yay0 code blocks — the ROM link uses the build/assets/%.bin.o:
 * assets/%.bin rule (stored ground-truth asset), NOT the rebuilt block, so the
 * ROM never contains this rebuilt code. objdiff report = 99.956 (the 4 spill
 * diffs; the gl_func/&D placeholder reloc form is already credited at 100).
 * This is the documented "2 swapped spill slots" genuine coloring tie — no
 * symbol-form change can move a stack offset. Honest NON_MATCHING. */
/* TimDataManager_init_00001A64 — EXACT 90/90 vs own .s (2026-07-03, agent-e).
 * The old "genuine coloring tie / 2 swapped spill slots" cap FELL to a 3-lever combo:
 *  (1) REMOVE the `int *s0 = a0;` alias (use a0 directly) — under 6-local pressure the
 *      alias itself was homing at sp+0x20 and shifting every slot (docs F284 extension);
 *      IDO still emits `move s0,a0` for the cross-call param.
 *  (2) decls `int *a1x; volatile int p1..p4; int r5; int r6;` — the 4 pads home at
 *      0x24-0x30 so r5's home = 0x20 (v0 spill) and a1x tops out at 0x34.
 *  (3) comma-assign the list-head INSIDE the call arg:
 *      gl(((int)a0)+0x10, (int)(a1x = (int*)a0[0xBC/4])) — puts the a1 save in the
 *      jal delay slot at 0x34 (plain `new_var = ...;` form homes it low at 0x24).
 * Frame 0x38, all four spill words exact. All jals are the USO gl_func_00000000
 * placeholder (zero-target) — no baked-jal issue in this fn. */
void TimDataManager_init_00001A64(int *a0, int a1, int a2, int a3) {
  int *a1x;
  volatile int p1;
  volatile int p2;
  volatile int p3;
  volatile int p4;
  int r5;
  int r6;
  NODE(a0)->name = (const char *)(((char *) (&D_00000000)) + 0x40C);
  TIMDATAMGR(a0)->race = a1;
  DATAMGR(a0)->owner = (RaceProcess *)a1;
  DATAMGR(a0)->session = (BackupSession *)a3;
  DATAMGR(a0)->blinkTimer = 0;
  TIMDATAMGR(a0)->beepTimer = 0;
  DATAMGR(a0)->pointsListArg = a2;
  TIMDATAMGR(a0)->alpha = 0xFF;
  TIMDATAMGR(a0)->unkD4 = 0;
  TIMDATAMGR(a0)->ghostReplayMode = 1;
  TIMDATAMGR(a0)->colour[3] = 1.0f;
  TIMDATAMGR(a0)->colour[2] = 1.0f;
  TIMDATAMGR(a0)->colour[1] = 1.0f;
  TIMDATAMGR(a0)->colour[0] = 1.0f;
  if ((RACEPROC(a1)->flags << 15) >= 0)
  {
    return;
  }
  CALL(hud_addSpeedGadget)(a0, 0xE8, 0x13, (int)DATAMGR(a0)->entries + 0x10);
  CALL(hud_addCourseGadget)(a0, 0x123, 0xE1, 1);
  CALL(hud_addTimeGadget)(a0, 0x47, 0x13, ((int) a0) + 0x30);
  CALL(hud_addCoolGadget)(a0, 0x44, 0x22, (int)DATAMGR(a0)->entries + 0x28);
  r5 = CALL(Backup_courseRecordGetTime)(((*((int *) (((char *) (&D_00000000)) + 0x64))) * 0x30) + (*((int *) DATAMGR(a0)->session)), 0);
  r6 = CALL(BestTimeGadget_ctor)(0, DATAMGR(a0)->sprites60);
  TIMDATAMGR(a0)->rankLabel = r6;
  CALL(BestTimeGadget_setTime)(r6, *((unsigned char *) (r5 + 5)), *((unsigned char *) (r5 + 6)), *((unsigned char *) (r5 + 7)));
  CALL(TimeGadgetParms_setPos_00013FE0)(TIMDATAMGR(a0)->rankLabel, 0x4B, 0xD6);
  CALL(list_append)(((int) a0) + 0x10, (int) (a1x = (int *) TIMDATAMGR(a0)->rankLabel));
  if (NODE(a1x)->parent != 0)
  {
    NODE(a1x)->unk04 = 1;
  }
  NODE(a1x)->parent = (Node *) a0;
}

/* TimDataManager_update_00001BCC - per-frame update state machine, EXACT 158/158
 * (2026-07-07). Levers: per-region pointer locals (pa..pf) so no single web
 * spans a $v0-def (each colors $v0); if(1) BB-barriers on defs that follow a
 * call in the same BB; counters as direct memory compound += (no user local ->
 * t-temps, CSE folds the reload); tail 0x78 ptr advanced +0x18 then *p RMW
 * (addiu materialized, sw 0(p)); third arg of the 0x80-dispatch call is
 * FLOAT 0.0f to prototyped (int,int,float) gl_proto_1c94 -> li a2,0 (O32
 * float-after-2-ints passes bits in $a2; int-0/K&R forms all emit move). */
extern int gl_proto_1c68(void *, int, int, float, float);
extern int gl_proto_1c94(int, int, float);
void TimDataManager_update_00001BCC(char *arg0) {
    int v1;
    char *pa;
    char *pb;
    char *pc;
    char *pd;
    char *pe;
    char *pf;

    CALL(RaceTracker_toggleGhost)();
    if (1) {
        pa = (char *)TIMDATAMGR(arg0)->race;
        if ((RACEPROC(pa)->flags & 0x10000) && (RACEPROC(pa)->raceState == 1)) {
            DATAMGR(arg0)->elapsed = DATAMGR(arg0)->elapsed + 0x21;
        }
    }
    CALL(RaceTracker_update)(arg0);
    if (1) {
        pb = (char *)TIMDATAMGR(arg0)->race;
        v1 = RACEPROC(pb)->flags & 0x10000;
    }
    if ((v1 != 0) && (RACEPROC(pb)->raceState == 1)) {
        if (DATAMGR(arg0)->numEntries == 2) {
            if ((TIMDATAMGR(arg0)->ghostReplayMode == 1) &&
                (((PadCtl *)SNOWB(DATAMGR(arg0)->entries[1].rider)->input)->replayMode == 0)) {
                CALL(RaceTracker_sendEntryLoseResult)(arg0, 1);
            }
            TIMDATAMGR(arg0)->ghostReplayMode = ((PadCtl *)SNOWB(DATAMGR(arg0)->entries[1].rider)->input)->replayMode;
        }
        if (DATAMGR(arg0)->entries->finished != 0) {
            CALL(RaceProcess_finishCourse)(TIMDATAMGR(arg0)->race);
        }
        if (((CoolGadget *)DATAMGR(arg0)->cool)->cur <= 0) {
            CALL(RaceProcess_endResult4_00006EE0)(TIMDATAMGR(arg0)->race);
            RACEPROC((char *)TIMDATAMGR(arg0)->race)->msgFloor = 192.0f;
            RACEPROC((char *)TIMDATAMGR(arg0)->race)->msgAlpha = 0xFF;
        }
        TIMDATAMGR(arg0)->beepTimer += 1;
        if (TIMDATAMGR(arg0)->beepTimer >= 0x33) {
            TIMDATAMGR(arg0)->beepTimer = 0;
        }
        pc = (char *)DATAMGR(arg0)->cool;
        if ((pc != 0) && (((CoolGadget *)pc)->seg >= 0xC) && (TIMDATAMGR(arg0)->beepTimer == 0)) {
            CALL(snd_sendEvent)(0x135, 0);
        }
        pb = (char *)TIMDATAMGR(arg0)->race;
        v1 = RACEPROC(pb)->flags & 0x10000;
    }
    if (v1 != 0) {
        if ((RACEPROC(pb)->raceState == 1) && (CALL(RaceTracker_checkTimeLimit)(arg0) != 0)) {
            CALL(RaceProcess_timeUp)(TIMDATAMGR(arg0)->race);
            RACEPROC((char *)TIMDATAMGR(arg0)->race)->msgFloor = 192.0f;
            RACEPROC((char *)TIMDATAMGR(arg0)->race)->msgAlpha = 0xFF;
            DATAMGR(arg0)->active = 0;
        }
        gl_proto_1c94(DATAMGR(arg0)->course, *(int *)&DATAMGR(arg0)->entries->pct, 0.0f);
        if (1) {
            pd = (char *)DATAMGR(arg0)->entries;
            gl_proto_1c68(DATAMGR(arg0)->course, *(int *)&((RaceEntry *)pd)->posX, *(int *)&((RaceEntry *)pd)->progress, 0.0f, 0.0f);
        }
        if (1) {
            pe = (char *)TIMDATAMGR(arg0)->race;
        }
        if ((RACEPROC(pe)->raceState == 2) && (RACEPROC(pe)->result == 0)) {
            DATAMGR(arg0)->blinkTimer += 1;
            if (DATAMGR(arg0)->blinkTimer & 8) {
                pf = (char *)DATAMGR(arg0)->time + 0x18;
                *(int *)pf = *(int *)pf & ~4;
            } else {
                pf = (char *)DATAMGR(arg0)->time + 0x18;
                *(int *)pf = *(int *)pf | 4;
            }
        }
    }
    CALL(Node_update)(arg0);
}

/* In-unit jal aliases (agent-c 2026-10-16): the ROM ships these four callers'
 * jal fields pre-filled with the module-relative targets 0x83C/0x878/0x8B4/
 * 0x90C. Each alias is pinned to its target in undefined_syms_auto.txt and the
 * b1 block rule bakes it (bake-data-relocs.py --bake-jal-pinned), as the
 * linker does for the same convention in linked units. A `static` callee
 * does not work here: the TU starts at block 0xB0 (region-0 -O0 .o first). */
extern void timproc_uso_b1_func_0000083C_jal(int *dst);
extern void timproc_uso_b1_func_00000878_jal(float *dst);
extern void timproc_uso_b1_func_000008B4_jal(Quad4 *dst);
extern void timproc_uso_b1_func_0000090C_jal(Vec3 *dst);
void ParmInt_read_00001E44(char *dst) {
    int tmp;
    timproc_uso_b1_func_0000083C_jal(&tmp);
    timproc_uso_b1_func_0000083C_jal((int*)(dst + 0x10));
}

void ParmFloat_read_00001E74(char *dst) {
    int tmp;
    timproc_uso_b1_func_0000083C_jal(&tmp);
    timproc_uso_b1_func_00000878_jal((float*)(dst + 0x10));
}

void ParmQuad_read_00001EA4(char *dst) {
    int tmp;
    timproc_uso_b1_func_0000083C_jal(&tmp);
    timproc_uso_b1_func_000008B4_jal((Quad4*)(dst + 0x10));
}

/* TimItemType_create_00001ED4: 36-insn (0x90) constructor — BYTE-IDENTICAL
 * mirror of edd_ItemType_create_000003BC (sig=f167638a8a, 4th clone of this
 * family). Same alloc + init + list-add structure with beql speculative
 * double-store. ~60% NM cap inherited. Multi-tick decomp.
 *
 * Find via: scripts/find-byte-identical-clones.py */
/* 2026-05-31: BYTE-IDENTICAL to the 100% edd_ItemType_create_000003BC; applied its body
 * (the old structure was wrong — nested the head-insert inside the alloc-success and
 * scrambled the slot/flag order; correct is the two-phase var-reuse below). */
void *TimItemType_create_00001ED4(int *arg0) {
    int *p2;
    int *head;
    int *p1;
    p1 = (int*)CALL(mem_new)(0x40);
    if (p1 != 0) {
        CALL(ColourItem_ctor)(p1);
        NODE(p1)->vptr = (char *)&D_00000000;
        ((ColourSubItem *)p1)->unk3C = 0;
    }
    p2 = p1;
    p1 = arg0;
    head = (int*)p1[0x40 / 4];
    if ((int*)p1[0x40 / 4] != 0) {
        CALL(list_append)((char*)p2 + 0x10, head);
        if (NODE(head)->parent != 0) {
            NODE(head)->unk04 = 1;
        }
        NODE(head)->parent = (Node *)p2;
    }
    return p2;
}

void ParmVec3_read_00001F64(char *dst) {
    int tmp;
    timproc_uso_b1_func_0000083C_jal(&tmp);
    timproc_uso_b1_func_0000090C_jal((Vec3*)(dst + 0x10));
}
/* 3-word trailing pad sidecar (emits exactly; >=2-word blocks are safe).
 * The old P0 "LEN -0xC" triage blamed this region incorrectly: the real
 * causes were the two dropped stolen-prologue orphans at 0x19B8/0x2028
 * (restored 2026-06-10 as standalone 2-word GLOBAL_ASM blocks) plus a
 * stale YAY0_TEXT_SIZE (0x2ED4 -> 0x2EE0). Block byte-verified vs
 * assets/timproc_uso_block_1.bin. */
#pragma GLOBAL_ASM("asm/nonmatchings/timproc_uso_b1/timproc_uso_b1/timproc_uso_b1_func_00001F64_pad.s")

void TimMenu_rowRetry_00001FA0(void) {
    CALL(Process_stop)(sTimPauseMenu_208);
    gInputMgr_procState_40 = 4;
    CALL(Process_fadeOut_0000F954)(sTimPauseProc_20C, -1, 0);
}

/* EXACT 2026-05-03. Mirror of TrkMenu_rowGhost_000021F4. Same recipe:
 * 3 unique externs (D_b1_1FE4_a/b/c) all mapped to 0x0 + offset cast in C.
 * (The 2-word "orphan" after it at 0x2028 is the next function's own
 * hoisted first load; it is now part of TimMenu_rowToggle_00002028.)
 * Per feedback_unique_extern_with_offset_cast_breaks_cse.md. */
/* timproc's own Bss +0x208 / +0x20C (sTimPauseMenu_* / sTimPauseProc_*; trkproc has the same pair,
 * arcproc at +0x70 / +0x74): the pause GuiMenu and the TimMenu process itself. TimMenu_update_00002740
 * stores MENUPROC(a0)->menu and a0 there before calling the row; the rows stop the GuiMenu and fade
 * the menu process out. The aliases here are 0, so the offsets stay in the C. */
typedef struct TimPauseBss {
    /* 0x0 */ int menu;             /* GuiMenu (Process_stop) */
    /* 0x4 */ int proc;             /* the TimMenu process (Process_fadeOut) */
} TimPauseBss;
#define TIM_PAUSE(sym) ((TimPauseBss *)((char *)&(sym) + 0x208))
extern char D_b1_1FE4_a;
extern char D_b1_1FE4_b;
extern char D_b1_1FE4_c;
void TimMenu_rowGhost_00001FE4(void) {
    CALL(Process_stop)(TIM_PAUSE(D_b1_1FE4_a)->menu);
    INPUT_MGR(D_b1_1FE4_b).procState = 6;
    CALL(Process_fadeOut_0000F954)(TIM_PAUSE(D_b1_1FE4_c)->proc, -1, 0);
}

/* Dual-branch state setter (twin of TrkMenu_rowToggle_00002238).
 * Starts at 0x2028 (was split as a 2-word orphan + func_00002030 taking
 * a0 as a parameter): the lui/lw of D+0x148 is the first call's argument,
 * and IDO hoists that load above the addiu sp prologue. Passing the load
 * straight into the call (no named local) keeps it in $a0.
 * MATCHED 2026-06-10 (31/31) as 2030: the old "register-allocator cap" had
 * two decode errors -- the first call's arg is the pointer + 4, and both arms call through the SAME +0x208 target. Load-
 * bearing shapes: volatile pointer-fetch global for the per-store
 * reload of cur (+0x20C), and the if(1){} BB-split inside each arm
 * AFTER the jal (the documented v0/v1 lever, 4-for-4) which flips the
 * address temp into $v0 / the li-1 into $v1. */
extern int sTimPauseMenu_2030;
extern int * volatile sTimPauseProc_2030;
void TimMenu_rowToggle_00002028(void) {
    if (CALL(Backup_ghostGetMode)(*(char**)((char*)&D_00000000 + 0x148) + 4) != 0) {
        CALL(Process_stop)(sTimPauseMenu_2030);
        if (1) {}
        sTimPauseProc_2030[0x14] = 2;
        sTimPauseProc_2030[0x16] = 1;
    } else {
        CALL(Process_stop)(sTimPauseMenu_2030);
        if (1) {}
        sTimPauseProc_2030[0x14] = 1;
        sTimPauseProc_2030[0x16] = 1;
    }
}

void TimMenu_rowChangeCharacter(void) {
    CALL(Process_stop)(sTimPauseMenu_208);
    gInputMgr_procState_40 = 1;
    CALL(Process_fadeOut_0000F954)(sTimPauseProc_20C, -1, 0);
}

void TimMenu_rowChangeBoard(void) {
    CALL(Process_stop)(sTimPauseMenu_208);
    gInputMgr_procState_40 = 2;
    CALL(Process_fadeOut_0000F954)(sTimPauseProc_20C, -1, 0);
}

extern int sTimPauseMenu_208;
extern int sTimPauseProc_20C;

void TimMenu_rowChangeCourse(void) {
    CALL(Process_stop)(sTimPauseMenu_208);
    gInputMgr_procState_40 = 3;
    CALL(Process_fadeOut_0000F954)(sTimPauseProc_20C, -1, 0);
}

void TimMenu_rowQuit_00002178(void) {
    CALL(Process_stop)(sTimPauseMenu_208);
    gInputMgr_procState_40 = 0xD;
    CALL(Process_fadeOut_0000F954)(sTimPauseProc_20C, -1, 0);
}

/* 000021BC/21C4/21CC: 3 empty stubs RECOVERED 2026-05-28 from the Yay0 gap
 * (no .s, missing from build; bytes from block_1). void f(void){} -> jr ra;nop. */
void tim_nop_000021BC(void) {}
void tim_nop_000021C4(void) {}
void tim_nop_000021CC(void) {}

/* TimMenu_ctor - EXACT (run 113 agent-c): twin port of the
 * exact TrkMenu_ctor (agent-g 2026-10-16). Same 3-stage
 * short-circuit alloc cascade with one a3 web, unused volatile pad, `if (1) {}`
 * before the dispatch base, the first registration on ONE source line, and
 * distinct per-site symbols (D_b1_21D4_*, all 0 in this raw-.word USO) so IDO
 * re-materializes the base with a fresh lui per call instead of CSEing &D into
 * an s-reg. b1 deltas vs b3: descriptor offsets 0x4B8/0x4C0, the region test
 * reads the absolute word 0xA0000200 == 0xAC290000, and the final call takes
 * 3 args. The earlier hand decode (153/200, goto cascade + `char * volatile`
 * arg-fold, frame 0x50 vs 0x40) is superseded. */
extern char D_b1_21D4_d1;
extern char D_b1_21D4_d2, D_b1_21D4_d3, D_b1_21D4_d4, D_b1_21D4_d5;
extern char D_b1_21D4_d6, D_b1_21D4_d7, D_b1_21D4_d8;
extern char D_b1_21D4_base;
char *TimMenu_ctor(char *arg0) {
    char *a3;
    char *a0;
    char *v0;
    volatile int pad;

    if ((arg0 != 0) || (arg0 = (char *)CALL(mem_new)(108), (arg0 != 0))) {
        a3 = arg0;
        if ((arg0 != 0) || (a3 = (char *)CALL(mem_new)(80), (a3 != 0))) {
            a0 = a3;
            if ((a3 != 0) || (a0 = (char *)CALL(mem_new)(44), (a0 != 0))) {
                CALL(Node_ctor)(a0, (char *)&D_00000000 + 0x4B8); *(char **)&NODE(a0)->vptr = &D_00000000;
            }
            *(char **)&NODE(a3)->vptr = &D_00000000;
        }
        TIMMENU(arg0)->base.node.vptr = (char *)(&D_00000000);
        TIMMENU(arg0)->base.node.name = (const char *)((char *)&D_00000000 + 0x4C0);
        CALL(TimMenu_initLabels_000024F4)(arg0);
        TIMMENU(arg0)->unk60 = 120;
        TIMMENU(arg0)->unk64 = 161;
        TIMMENU(arg0)->promptY = 191;
        TIMMENU(arg0)->base.menu = CALL(PauseMenu_create)(0);
        CALL(gui_initRows_000100F0)(TIMMENU(arg0)->base.menu, arg0);
        CALL(gui_addRowSub_00010B6C)(TIMMENU(arg0)->base.menu, ((*(int *)&D_b1_21D4_d2 + 3) << 16) | 1, -1, &D_b1_21D4_d1);
        CALL(gui_addRowSub_00010B6C)(TIMMENU(arg0)->base.menu, ((*(int *)&D_b1_21D4_d3 + 3) << 16) | 6, -1, &D_b1_21D4_d1);
        CALL(gui_addRowSub_00010B6C)(TIMMENU(arg0)->base.menu, ((*(int *)&D_b1_21D4_d4 + 3) << 16) | 7, -1, &D_b1_21D4_d1);
        CALL(gui_addRowSub_00010B6C)(TIMMENU(arg0)->base.menu, ((*(int *)&D_b1_21D4_d5 + 3) << 16) | 4, -1, &D_b1_21D4_d1);
        CALL(gui_addRowSub_00010B6C)(TIMMENU(arg0)->base.menu, ((*(int *)&D_b1_21D4_d6 + 3) << 16) | 3, -1, &D_b1_21D4_d1);
        CALL(gui_addRowSub_00010B6C)(TIMMENU(arg0)->base.menu, ((*(int *)&D_b1_21D4_d7 + 3) << 16) | 2, -1, &D_b1_21D4_d1);
        CALL(gui_addRowSub_00010B6C)(TIMMENU(arg0)->base.menu, ((*(int *)&D_b1_21D4_d8 + 3) << 16) | 5, -1, &D_b1_21D4_d1);
        GUIMENU(TIMMENU(arg0)->base.menu)->unk30 = CALL(gui_fontCreate)(0, &D_00000000, 72, 221, 3, 13);
        CALL(gui_centerRows_0001016C)(TIMMENU(arg0)->base.menu);
        CALL(gui_centerAt_000102A4)(TIMMENU(arg0)->base.menu, 140);
        if (1) {}
        v0 = (char *)TIMMENU(arg0)->base.menu;
        a3 = (char *)NODE(v0)->vptr;
        ((void (*)(int))VT_SLOT(a3, 10).fn)(VT_SLOT(a3, 10).delta + (int)v0);
        a3 = (char *)TIMMENU(arg0)->base.menu;
        CALL(list_append)(arg0 + 16, a3); if (NODE(a3)->parent != 0) { NODE(a3)->unk04 = 1; } NODE(a3)->parent = (Node *)arg0;
        if (INPUT_MGR(D_b1_21D4_base).unk184 == 0 ||
            (*(int *)0xA0000200 == 0xAC290000) == 0) {
            GUIMENU(TIMMENU(arg0)->base.menu)->rows[2].enabled = 0;
        }
        a3 = (char *)INPUT_MGR(D_b1_21D4_base).fader;
        CALL(list_append)(arg0 + 16, a3);
        if (NODE(a3)->parent != 0) {
            NODE(a3)->unk04 = 1;
        }
        NODE(a3)->parent = (Node *)arg0;
        CALL(Fader_start)(INPUT_MGR(D_b1_21D4_base).fader, 1, 0);
        TIMMENU(arg0)->unk54 = 0;
        TIMMENU(arg0)->base.state = 0;
        TIMMENU(arg0)->base.unk30 = 1;
        TIMMENU(arg0)->base.unk2C = 0;
    }
    return arg0;
}

/* TimMenu_initLabels_000024F4: GBI command emitter (8x 0x260000|(D<<3)+N etc).
 * The 0x260000-series `or a1, s1, tN` wants the hoisted constant (s1) as rs;
 * IDO defaults to value-as-rs. The old "not C-reachable" claim was WRONG —
 * forced via the assignment-expression operand-order lever `value | (t = const)`
 * (pins the const as rs at the op; CSE keeps the single lui). Byte-identical
 * sibling of TrkMenu_initLabels_00002700. See
 * docs/IDO_CODEGEN.md#feedback-ido-commutative-op-operand-order. */
void TimMenu_initLabels_000024F4(int a0) {
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

/* TimMenu_update_00002740 — 95.69% → 99.35% (2026-05-18) via distinct
 * externs sTimPauseMenu_2740/g20C for the &D+0x208/+0x20C absolute stores so each
 * sw gets its own R_MIPS_HI16/LO16 reloc (per
 * docs/IDO_CODEGEN.md#feedback-ido-cse-bust-via-distinct-externs).
 * switch on a0->0x50: case 0 → gl_func(&D,0x40100) gate, then walk
 *   slot = base + base->0x7C * 0x28; if slot->0x90 != 0 && slot->0x88 != 0
 *   { gl(5); D[0x208]=a0->0x48; D[0x20C]=a0; recompute slot2; (*slot2->0x90)(); }
 *   else gl(165). case 1/2 → gl_func(a0).
 * Was promoted 2026-05-21 with INSN_PATCH for fixed register/operand choice
 * caps plus raw-word relocation cleanup at 0x90/0x98/0x9C/0xA0. INSN_PATCH
 * REMOVED 2026-05-23 as match-faking per
 * feedback_no_instruction_forcing_matches_policy — fn rolled back to NM-wrap;
 * NATURAL CEILING (fixed-register diffs stay NM). docs/POST_CC_RECIPES.md
 * is DEPRECATED.
 *
 * 2026-05-31 MIGRATION-VALUE NOTE: this fn is the canonical MIXED-cap case —
 * 7 real diffs = 2 reloc-blind (`sw $8,0x208($1)`/`sw $6,0x20C($1)`: the
 * sTimPauseMenu_2740/g20C scalar %lo stores expected bakes but C-emit relocs,
 * NOT C-bakeable per MATCHING_WORKFLOW#addend-only-folds-in-addiu) PLUS 5
 * register-renumber/operand-order ($2/$3, addu $2+$15 vs $15+$2). So the
 * USO-reloc migration would only DROP the 2 reloc diffs (98.9%->~99.x), NOT
 * land it — the register caps remain. 2026-06-10: the if(1){} BB lever
 * after gl(5) cuts 5 register diffs to 3; the remaining 3 are the addu
 * OPERAND-ORDER canonical (target addu v1,v0,t7 base-first; every C
 * form tested — ptr+int, int+ptr, char* arith, &arr[idx], struct-idx,
 * 0xA*idx — emits scaled-first) + its dependent lw. Same cosmetic class
 * as the documented beq operand order. True ceiling = 2 reloc-blind +
 * 3 operand-order. General finding: near-100 USO fns are
 * typically mixed reloc-blind + regalloc, so the migration helps only fns
 * whose SOLE residual is reloc-blind; it does not credit the mixed near-100
 * band. C here is already CORRECT (right symbols); nothing to fix in C. */
extern int sTimPauseMenu_2740;
extern int sTimPauseProc_2740;
/* 2026-06-11: scoped permuter run IN FLIGHT (base 80, log
 * /tmp/perm2740.log) -- CONCLUDED 2026-06-11 ~02:50: both twins
 * terminally plateau at permuter-score 60 (= one diff, objdiff 99.84;
 * b3 at 573k iters, b1 at 289k). The class's LAST addu order resists
 * randomized search symmetrically -- consistent with a genuinely
 * allocator-internal choice. 99.84 is the twins' C+permuter ceiling;
 * uopt-internals research is the only remaining instrument. */
void TimMenu_update_00002740(int *a0)
{
  int new_var;
  switch (MENUPROC(a0)->state)
  {
    case 0:
      if (CALL(input_checkPressed_00011E00)(&D_00000000, 0x40100) != 0)
    {
      /* base[i] = the menu shifted by i rows (Row2740 = one GuiRow), so slot's rows[0] is
       * rows[cursor]; the 2D-array (IXA) index is the addu-order lever (&menu->rows[cursor]
       * costs an extra addiu) */
      typedef int Row2740[10];
      Row2740 *base = (Row2740 *) MENUPROC(a0)->menu;
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
          sTimPauseMenu_2740 = MENUPROC(slot)->menu;
          sTimPauseProc_2740 = (int) a0;
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
      CALL(TimMenu_handleInput_00002A8C)(a0);
      break;

  }

}


/* TimMenu_draw_00002838 - HUD draw dispatcher (switch arg0->0x50,
 * cases 0/1/2). Struct-typing: s0->0x50 state selector, s0->0x5C frame
 * counter (-=16), s0->0x68 position (+10/-8 by global flag *(int*)&D),
 * &D+0x178 u8 digit; panel rows registered against &D+0x1F0/0x1C0/0x160/
 * 0x148/0x178 (&D-relative ADDRESSES = lui+addiu, not literals).
 * 2026-07-17 agent-g: 149/149 words EXACT (was 86.64). Levers that landed:
 * shared obj local colors the recurring s0 web (44EDC recipe, jal-delay
 * move s0,a0); case-1 first arg is &D+0 not literal 0; single-def
 * x = v0-0x27 AFTER `if (f) { if(1){ v0 += 0xA; } }` -> IDO tail-dups the
 * x def into the beql likely-delay (rematerialize-in-delay) AND demotes
 * x to last-colored web (v=v0/f=v1/x=a0; two-def if/else spelling rotates
 * the coloring); separate case-2 local w splits the spill homes
 * (v0->0x2C, a2->0x28, w->0x24; decl order f,v0,a2,w,x with 5 decls puts
 * f at unused 0x30 and v0 at 0x2C); a2 = x; if (f) a2 = x - 0xE (source
 * x, not a2 -= 0xE). Unwrapped EXACT 2026-10-16 (run 96): glv_2838 is a
 * per-site blank callee alias pinned to 0 in undefined_syms_auto.txt. */
extern void glv_2838();
void TimMenu_draw_00002838(char *arg0) {
    char *obj = arg0; /* shared obj local colors the recurring s0 web (44EDC recipe) */
    int f;
    int v0;
    int a2;
    int w;
    int x;

    glv_2838(obj);
    TIMMENU(obj)->blink = TIMMENU(obj)->blink - 0x10;
    switch (TIMMENU(obj)->base.state) {
    case 1:
        glv_2838((char *)&D_00000000);
        glv_2838((char *)&D_00000000 + 0x1F0);
        glv_2838((char *)&D_00000000 + 0x1F0, 0xA0, 0x25, 3);
        glv_2838(obj, 0x25);
        glv_2838(obj, *(unsigned char *)((char *)&D_00000000 + 0x178), 0x50);
        glv_2838((char *)&D_00000000 + 0x1C0);
        glv_2838((char *)&D_00000000 + 0x1C0);
        glv_2838((char *)&D_00000000 + 0x1C0, 0xA0, 0x69, 3);
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
        glv_2838((char *)&D_00000000 + 0x148);
        glv_2838((char *)&D_00000000 + 0x148);
        glv_2838((char *)&D_00000000 + 0x148, 0xA0, a2, 3);
        glv_2838(obj, v0 - 0x14);
        return;
    case 2:
        glv_2838((char *)&D_00000000);
        glv_2838((char *)&D_00000000 + 0x1F0);
        glv_2838((char *)&D_00000000 + 0x1F0, 0xA0, 0x25, 3);
        glv_2838(obj, 0x25);
        glv_2838(obj, *(unsigned char *)((char *)&D_00000000 + 0x178), 0x50);
        glv_2838((char *)&D_00000000 + 0x1C0);
        glv_2838((char *)&D_00000000 + 0x1C0);
        glv_2838((char *)&D_00000000 + 0x1C0, 0xA0, 0x69, 3);
        glv_2838((char *)&D_00000000 + 0x160);
        glv_2838((char *)&D_00000000 + 0x160);
        glv_2838((char *)&D_00000000 + 0x160, 0xA0, 0x8E, 3);
        w = TIMMENU(obj)->promptY;
        if (*(int *)&D_00000000 != 0) {
            w -= 8;
        }
        glv_2838((char *)&D_00000000 + 0x178);
        glv_2838((char *)&D_00000000 + 0x178);
        glv_2838((char *)&D_00000000 + 0x178, 0xA0, w - 0x13, 3);
        glv_2838(obj, w);
        break;
    case 0:
        break;
    }
}

/* bootup imports of the TimMenu_handleInput_00002A8C below, read from the module's Sym table
 * (kind-1 import entries: exporter module 0 = bootup, word3 top half = bootup Sym index;
 * agent-c run 495). They stay jal 0 / lui 0 placeholders in this unlinked Yay0 block.
 * This is the post-race save-ghost prompt: the stick (0x10001 / 0x4002) moves the cursor
 * obj+0x58 between 0 (save) and 1 (skip); confirm (0x40100) on row 0 saves the recorded run
 * as the SRAM ghost (replay_saveGhost, BACKUP_MODE_TIME); cancel (0x200) skips. */
extern int tim_imp_input_checkPressed();
extern int tim_imp_snd_postEvent();
extern int tim_imp_replay_saveGhost();

/* TimMenu_handleInput_00002A8C — sequence of gl_func_00000000(&D, mask)
 * gates toggling s0->0x58, then a recurring vtable-dispatch tail:
 *   obj=(int*)s0->0x48; s0->0x50=0; vt=(int*)obj->0x28;
 *   (*(fn)vt->0x5C)( (short)vt->0x58 + (int)obj );
 * Masks: 0x10001 (clear 0x58), 0x4002 (set 0x58=1), 0x40100 (branch:
 * s0->0x58==0 -> gl(5)+gl(&D,1)+obj->0xD8=0+vtail, else gl(2050)+vtail),
 * 0x200 (gl(2050)+vtail). IDO's allocator emits the vtail through v1/a1;
 * the target uses v0/v1 (obj reuses $v0 freed by the preceding void
 * call). INSN_PATCH removed 2026-05-23. 2026-06-10: the E04 web-order
 * inversion (inline the full vt chain, let CSE merge) is NEUTRAL here
 * -- 15 diffs either way; the lever flips v0/v1 pairs in single-block
 * chains (E04) but not in this multi-arm shape where each arm's vtail
 * re-derives the web after different call sequences. v0-reuse-after-
 * void-call coloring; uoptlist class. */
/* LANDED via twin-port from TrkMenu_handleInput_00002C98 (byte-identical
 * except the 0x40100-arm mask literal is 1 instead of 2). Same `if (1) {}`
 * basic-block-boundary lever that colors o48 into the dead $v0. */
void TimMenu_handleInput_00002A8C(char *obj) {
    char *o48;
    char *vt;

    if (tim_imp_input_checkPressed(&gInputMgr, 0x10001) != 0 && TIMMENU(obj)->choice != 0) {
        tim_imp_snd_postEvent(1);
        TIMMENU(obj)->choice = 0;
    }
    if (tim_imp_input_checkPressed(&gInputMgr, 0x4002) != 0 && TIMMENU(obj)->choice == 0) {
        tim_imp_snd_postEvent(1);
        TIMMENU(obj)->choice = 1;
    }
    if (tim_imp_input_checkPressed(&gInputMgr, 0x40100) != 0) {
        if (TIMMENU(obj)->choice == 0) {
            tim_imp_snd_postEvent(5);
            tim_imp_replay_saveGhost(&gInputMgr, BACKUP_MODE_TIME);
            GUIMENU(TIMMENU(obj)->base.menu)->rows[2].enabled = 0;
            if (1) {}
            o48 = (char *)TIMMENU(obj)->base.menu;
            TIMMENU(obj)->base.state = 0;
            vt = (char *)NODE(o48)->vptr;
            ((void (*)(int))VT_SLOT(vt, 10).fn)(VT_SLOT(vt, 10).delta + (int)o48);
        } else {
            tim_imp_snd_postEvent(0x802);
            if (1) {}
            o48 = (char *)TIMMENU(obj)->base.menu;
            TIMMENU(obj)->base.state = 0;
            vt = (char *)NODE(o48)->vptr;
            ((void (*)(int))VT_SLOT(vt, 10).fn)(VT_SLOT(vt, 10).delta + (int)o48);
        }
    }
    if (tim_imp_input_checkPressed(&gInputMgr, 0x200) != 0) {
        tim_imp_snd_postEvent(0x802);
        if (1) {}
        o48 = (char *)TIMMENU(obj)->base.menu;
        TIMMENU(obj)->base.state = 0;
        vt = (char *)NODE(o48)->vptr;
        ((void (*)(int))VT_SLOT(vt, 10).fn)(VT_SLOT(vt, 10).delta + (int)o48);
    }
}

/* TimMenu_drawHeader_00002BE4: 0xFC (63 insns) BYTE-EXACT (2026-07-23).
 * 7x5 grid dispatch: copy 5 tile indices from D+0x4D0, then for each row
 * (s3=0x10..0xD0 step 0x20) x col (i=0..4) call helper(&D+0x10+vec4[i]*0x18)
 * and helper(ptr, i*0x40, s3, 0).
 * Levers that cracked the s8-&vec4-cache cap (was 10 saved regs, frame 0x78):
 *  - struct-assign copy (B1Vec5) = target's t9/t8 ping-pong interleave;
 *  - indexed vec4[i] + derived i*0x40 arg (NO named walking ptr): uopt
 *    strength-reduction re-creates s1 with per-outer-iter addiu s1,sp,84
 *    remat (named ptr = s8 cache disease);
 *  - pointer-MUTATION base (s4=&D; if(1){s4+=0x10;}) keeps s4 a variable
 *    web -> colors s4 (copy-propped const base colored s5 / swapped with
 *    the 24-const web) and bakes +0x10 in the addiu;
 *  - copy source as *(B1Vec5*)((char*)&D+0x4D0) folds 1232 into %lo.
 * Final word (addu s2,s4,t1 base-first) closed by COMPOUND-ASSIGN split:
 * `s2 = (char*)s4; s2 += vec4[i]*0x18;` — the += tree puts the dest/base
 * first in the commutative addu (single-expr spellings all emit mult-first;
 * naming the mult temp `int off` flips order but costs +8 frame and colors
 * mflo v0). Unwrapped EXACT 2026-10-16 (run 96): placeholder callees stay
 * jal 0 because the Yay0 block is cut from the unlinked .o. */
typedef struct { int v[5]; } B1Vec5_2BE4;
typedef struct { char pad[0x18]; } B1S18_2BE4;
extern int gl_func_00000000();
void TimMenu_drawHeader_00002BE4(int *a0) {
    int vec4[5];
    B1S18_2BE4 *s4;
    char *s2;
    int s3, i;
    (void)a0;

    *(B1Vec5_2BE4 *)vec4 = *(B1Vec5_2BE4 *)((char *)&D_00000000 + 0x4D0);
    CALL(gui_setCombine_00001794)(&D_00000000);

    s4 = (B1S18_2BE4 *)&D_00000000;
    if (1) { s4 = (B1S18_2BE4 *)((char *)s4 + 0x10); }
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

/* TimMenu_drawLabel_00002CE0: 26-insn (0x68) 3-call wrapper indexed by a1.
 * Promoted 95.12% → 100% via two levers:
 *   1. Unique extern at table offset (D_b1_2CE0_table = 0x70) bakes the
 *      0x70 into the addiu reloc, eliminating the redundant addiu.
 *   2. Second pointer local (`spillee = entry`) shifts IDO's spill slot
 *      from sp+0x18 to sp+0x1C to match target's frame layout. */
extern int gl_func_00000000();
extern char D_b1_2CE0_table;
void TimMenu_drawLabel_00002CE0(int a0, int a1, int a2) {
    char *entry, *spillee;
    (void)a0;
    (void)spillee;
    CALL(gui_setCombine_00001794)(&D_00000000);
    entry = (char*)&D_b1_2CE0_table + a1 * 24;
    spillee = entry;
    CALL(gui_spriteRefresh)(entry);
    CALL(gui_drawSprite)(entry, 0xA0, a2, 3);
}

/* TimMenu_drawPair_00002D48: 0x108 (66 insns). Two-batch UI/render
 * dispatcher: each batch sets a Quad4-of-1.0 (only the first batch
 * stores it), calls a base init, then a mirror-swapped conditional
 * call selected by a0->0x58, then a secondary call. Frame 0x48; quad4
 * lands at sp+0x38 because it's declared FIRST (highest sp slot).
 * Adding `char pad[32]` AFTER quad4 holds the frame at -0x48. */
void TimMenu_drawPair_00002D48(int *a0, int a1) {
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

/* 34-insn 3-jal UI/render dispatcher, sibling of 00002D48. Two Quad4 args:
 * quad_a = {1,1,1,1} (sp+0x48), quad_b = {1,0,1,1} (sp+0x38). The 0.0 at
 * quad_b[1] lands in the first-jal delay slot. char pad[32] grows the
 * frame from 0x38 to 0x58 to match target's stack layout. */
void TimMenu_drawLabel130_00002E50(int *a0, int a1) {
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
#pragma GLOBAL_ASM("asm/nonmatchings/timproc_uso_b1/timproc_uso_b1/timproc_uso_b1_func_00002E50_pad.s")

/* SUFFIX_BYTES-absorbed orphans (per
 * docs/MATCHING_WORKFLOW.md#feedback-splat-orphan-duplicate-symbol-pruning):
 * tim_nop_000021BC + 000021C4 (concatenated 6 words) ←
 * TimMenu_rowQuit_00002178 SUFFIX_BYTES
 *   (0x03E00008,0x00000000,0x03E00008,0x00000000,0x03E00008,0x00000000).
 * Pre-prune symbols were at .o offsets 0x2EE0/0x2EE8 (tail, past natural
 * function layout end at 0x21D4); no-truncate variant of the orphan-prune. */

/* Historical orphan status (2026-06-10): _00002028 and _000019B8 were standalone
 * 2-word GLOBAL_ASM blocks at their vram positions (the old SUFFIX_BYTES /
 * C-emit absorptions silently vanished with the 2026-05-23 purge, leaving
 * the block -16). _000011D0 ← _00001130 (decl 0xA0, .o 0xA8) still
 * C-emit-absorbed and verified present. Update 2026-09-09: _000019B8
 * is now part of the corrected 43-word 19C0 C/ASM function boundary. */
