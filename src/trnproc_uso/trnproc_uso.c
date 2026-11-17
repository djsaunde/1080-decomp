#include "common.h"
#include "game/course.h"
#include "gdl/node.h"
#include "gdl/race.h"
#include "game/input.h"
#include "game/guimenu.h"
#include "game/trnproc.h"
#include "game/snowboarder.h"
#include "gdl/pad.h"
#include "uso_call.h"

extern int gl_func_00000000();
extern char D_00000000;

typedef struct { int a, b, c; } TrnTri3i;
typedef struct { int a, b, c, d; } TrnQuad4;

/* trnproc.uso's own Bss (relocations against the Bss section base, in-place addend = the
 * offset). trn_bss is pinned at 0 (undefined_syms_auto.txt): a symbol of its own, so uopt
 * does not share a lui with the bootup imports spelled off D_00000000. */
extern char trn_bss[];
/* Bss+0x1B0: the running TrnRace process (stored by onPauseSelect before the menu callback). */
#define gTrnRace (*(char **)(trn_bss + 0x1B0))
/* trnproc.uso's own Data section base (pinned at 0 like trn_bss). */
extern char trn_data[];
/* game.uso Bss+0x8: the TrickTable (TrickTable_getCount / getEntry / getInputMask). */
extern char *trn_gTrickTable;
/* gui.uso Bss+0x8: the gui colour state (gui_setColor_00001A5C's first argument). */
extern char trn_gGui;
/* trnproc Bss+0x0: the TrnRace HUD sprites (cards / buttons, the "+" at [15], the header at
 * +0x180 and the extra at +0x198 are spelled off trn_bss). */
extern RaceSprite trn_sprites[];
/* trnproc.uso RoData floats: the in-place addend is the RoData offset (0x20.., after the
 * procMain jumptable and its pad), one 0-pinned symbol per load so each keeps its own lui. */
#define TRN_RO(sym, off) (*(float *)((char *)&sym + (off)))
extern char trn_ro_20, trn_ro_24, trn_ro_28, trn_ro_2C;

/* trnproc_uso_func_00000000 / trn_nop_00000008: empty functions at Text+0 / +8 (unreferenced). */
/* UNNAMED: sentinel a *_00000000 name: rename-symbol.py refuses it (it doubles as the address-0 placeholder spelling) */
void trnproc_uso_func_00000000(void) {
}

void trn_nop_00000008(void) {
}

/* trn_main: trn_procMain, the mode's process loop (n64_procMain's template):
 * 0 launch the GenProc (next 1, then 4), 1 reset (trn_openSession, state 2), 2 sound + build the TrnRace
 * (TrnRace_ctor) and run it, 3 resume (state = next), 4 quit. The switch table is RoData+0 (the unit's
 * .rodata is pinned NOLOAD at VMA 0 in tenshoe.ld, the arcproc 240 recipe). */
void trn_openSession(int *out);
void trn_closeSession(int a0);
void trn_main(int arg0, int arg1) {
    int *cur;
    int flag = 0;
    int r;

    do {
        switch (arg1) {
        case 0:
            CALL(app_launchGenProc)(arg0, 1, 3, 1);
            flag = 1;
            INPUT_MGR(D_00000000).nextProcState = 1;
            INPUT_MGR(D_00000000).thenProcState = 4;
            break;
        case 1:
            CALL(trn_openSession)(arg0);
            INPUT_MGR(D_00000000).procState = 2;
            break;
        case 2:
            CALL(snd_playSeq_000120A8)(&D_00000000, 4, 8, 5);
            flag = 1;
            INPUT_MGR(D_00000000).nextFlowMode = 6;
            r = CALL(app_createGame_000092F4)(arg0, 1);
            INPUT_MGR(D_00000000).nextFlowMode = 7;
            r = CALL(TrnRace_ctor)(0, 0x4010000, r);
            CALL(app_runProcess)(arg0, 0, r);
            break;
        case 3:
            CALL(trn_closeSession)(arg0);
            INPUT_MGR(D_00000000).procState = INPUT_MGR(D_00000000).nextProcState;
            break;
        case 4:
            cur = (int *)CALL(QuitProcess_ctor)(0, 1, 0);
            flag = 1;
            CALL(list_append)(&NODE(&D_00000000)->children, cur);
            if ((int)NODE(cur)->parent != 0) {
                NODE(cur)->unk04 = 1;
            }
            NODE(cur)->parent = (Node *)&D_00000000;
            break;
        }
        arg1 = INPUT_MGR(D_00000000).procState;
    } while (flag == 0);
}

/* trn_openSession: clear *out and GameFlow +0x14C. */
void trn_openSession(int *out) {
    *out = 0;
    INPUT_MGR(D_00000000).session = 0;
}

/* trn_closeSession: empty one-argument function (the word after it is .text pad). */
void trn_closeSession(int a0) {
}

/* TrnRace_pauseContinue: pause menu "continue" (as TimRace_pauseContinue_000006B0, without
 * the input_clearActivePad). */
void TrnRace_pauseContinue(void) {
    CALL(Process_resume_0000FAE8)(gTrnRace);
    CALL(Node_resumeUpdate_0000E450)(TRNRACE(gTrnRace)->dataManager);
}

/* TrnRace_pauseRestart: pause menu row 1 (restart): resume, then the restart helper TrnRace_forceEnd. */
void TrnRace_pauseRestart(void) {
    CALL(Process_resume_0000FAE8)(gTrnRace);
    CALL(TrnRace_forceEnd)(gTrnRace);
}

/* TrnRace_pauseQuit: pause menu "quit": next flow state 3 / 4, save the setting at
 * +0x6B0, fade the process out (as TrkRace_pauseQuit_00000818). */
void TrnRace_pauseQuit(void) {
    INPUT_MGR(D_00000000).procState = 3;
    INPUT_MGR(D_00000000).nextProcState = 4;
    CALL(settings_set8_00012818)(&D_00000000, TRNRACE(gTrnRace)->trickDone);
    CALL(Process_fadeOut_0000F954)(gTrnRace, -1, 0);
}

/* trn_streamReadS32: read an int from the gdl file (cross-USO template, as checkpoint
 * checkpoint_gdlReadInt_00000034 / game_uso stream_readS32_00000280). */
void trn_streamReadS32(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

/* trn_streamReadF32: read a float from the gdl file (template, as balloon_gdlReadFloat_00000104). */
void trn_streamReadF32(float *dst) {
    float buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

/* trn_streamReadQuad: read a 16-byte quad from the gdl file (template, as
 * tim_streamReadQuad_000008B4). */
void trn_streamReadQuad(TrnQuad4 *dst) {
    TrnQuad4 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 16);
    *dst = buf;
}

/* trn_streamReadVec3: read a Vec3 from the gdl file (template, as balloon_gdlReadVec3_00000094 /
 * game_uso stream_readVec3_000001D4). */
void trn_streamReadVec3(float *dst) {
    int pad_top[1];
    TrnTri3i raw;
    int pad_mid[2];
    TrnTri3i tmp;
    int pad_bot[2];
    CALL(fs_gdlReadFile)(&D_00000000, &raw, 12);
    tmp = raw;
    dst[0] = *(float *)&tmp.a;
    dst[1] = *(float *)&tmp.b;
    dst[2] = *(float *)&tmp.c;
}

/* TrnRace_ctor: TrnRace ctor (TimRace_ctor's skeleton: inlined Node / Process /
 * RaceProcess base ctors, then the TrnDataManager (0x19C RaceTracker subclass, lesson count +0xB4 = 17),
 * a three-row pause menu and the saved trick bits read into +0x6B0). The vtable stores carry their
 * Data offset in the USO reloc, so each vptr is its own 0-pinned symbol. */
extern char trn_vt_node, trn_vt_proc, trn_vt_raceproc, trn_vt_trnrace, trn_vt_dm;
/* bootup Bss+0x28C: the gui string-table base of the pause-menu rows. */
extern int trn_gRowBase;
/* The pause rows' callbacks (1B0 / 1E4 / 214): Text relocs whose in-place addend is 0, so
 * each address is a 0-pinned alias of the function. */
extern void TrnRace_pauseContinue_ref(void), TrnRace_pauseRestart_ref(void), TrnRace_pauseQuit_ref(void);
/* D_00000000 (bootup Bss+0xF8, the InputMgr) under a second 0-pinned name: the dataMgr store
 * must not share the earlier &D_00000000 lui (as TimRace's D_b1_097C_g). */
extern char trn_gInputMgr;
/* void-typed alias of the global TrnDataManager_init call (its dead int return would hold v0; the vtable base
 * that follows colours v0, as TrkRace_ctor's gl_func_00000000_994v). */
extern void trn_func_00000A2C_v();
char *TrnRace_ctor(char *obj, int a1, int a2) {
    char *o1;
    char *o2;
    char *o3;
    char *w;

    if (obj != 0 || (obj = (char *)CALL(mem_new)(0x6BC)) != 0) {
    o1 = obj;
    if (o1 != 0 || (o1 = (char *)CALL(mem_new)(0x6A8)) != 0) {
        o2 = o1;
        if (o1 != 0 || (o2 = (char *)CALL(mem_new)(0x50)) != 0) {
            o3 = o2;
            if (o3 != 0 || (o3 = (char *)CALL(mem_new)(0x2C)) != 0) { CALL(Node_ctor)(o3, trn_data + 0x2F8); NODE(o3)->vptr = (char *)&trn_vt_node; }
            NODE(o2)->vptr = (char *)&trn_vt_proc;
        }
        NODE(o1)->vptr = (char *)&trn_vt_raceproc;
        CALL(ColourParms_ctor)(&RACEPROC(o1)->colours);
    }
    NODE(obj)->vptr = (char *)&trn_vt_trnrace;
    RACEPROC(obj)->spriteSet = 0;
    CALL(RaceProcess_init)(obj, a1, trn_data + 0x300, a2);
    CALL(TrnRace_initLabels)(obj);
    TRNRACE(obj)->unk6AC = 0.0f;
    CALL(memtrack_begin)(trn_data + 0x314, 0);
    CALL(input_setActivePad_0001207C)(&D_00000000, 0);

    {
        o2 = (char *)CALL(mem_new)(0x19C);
        if (o2 != 0) {
            CALL(RaceTracker_ctor)(o2);
            NODE(o2)->vptr = (char *)&trn_vt_dm;
        }
        TRNRACE(obj)->dataManager = (TrnDataManager *)o2;
        INPUT_MGR(trn_gInputMgr).dataMgr = (struct RaceTracker *)o2;
        o1 = (char *)TRNRACE(obj)->dataManager;
        TRNDM(o1)->base.hudFlags = 17; /* RTF_FLUSH | RTF_LOOKUP */
        XCALL(trn_func_00000A2C_v, TrnDataManager_init)((char *)TRNRACE(obj)->dataManager, obj, RACEPROC(obj)->spriteSet);

        w = (char *)TRNRACE(obj)->dataManager;
        o1 = (char *)NODE(w)->vptr;
        ((void (*)(int))VT_SLOT(o1, 10).fn)(VT_SLOT(o1, 10).delta + (int)w);

        o1 = (char *)TRNRACE(obj)->dataManager;
        CALL(list_append)(&NODE(obj)->children, o1);
        if (NODE(o1)->parent != 0) {
            NODE(o1)->unk04 = 1;
        }
        NODE(o1)->parent = (Node *)obj;

        CALL(RaceProcess_restart)(obj, a1);

        if ((RACEPROC(obj)->flags << 15) < 0) {
            RACEPROC(obj)->menu = (GuiMenu *)CALL(PauseMenu_create)(0);
            CALL(gui_initRows_000100F0)(RACEPROC(obj)->menu, obj);
            RACEPROC(obj)->menu->unk30 = RACEPROC(obj)->spriteSet;
            CALL(gui_addRowSub_00010B6C)(RACEPROC(obj)->menu, (*(int *)&trn_gRowBase + 3) << 16, -1, TrnRace_pauseContinue_ref);
            CALL(gui_addRowSub_00010B6C)(RACEPROC(obj)->menu, ((*(int *)&trn_gRowBase + 3) << 16) | 1, -1, TrnRace_pauseRestart_ref);
            CALL(gui_addRowSub_00010B6C)(RACEPROC(obj)->menu, ((*(int *)&trn_gRowBase + 3) << 16) | 5, -1, TrnRace_pauseQuit_ref);
            CALL(gui_centerRows_0001016C)(RACEPROC(obj)->menu);
            o1 = (char *)RACEPROC(obj)->menu;
            CALL(list_append)(&NODE(obj)->children, o1);
            if (NODE(o1)->parent != 0) {
                NODE(o1)->unk04 = 1;
            }
            NODE(o1)->parent = (Node *)obj;
        }

        CALL(settings_get8_000127CC)(&D_00000000, TRNRACE(obj)->trickDone);
        o1 = (char *)INPUT_MGR(D_00000000).fader;
        CALL(list_append)(&NODE(obj)->children, o1);
        if (NODE(o1)->parent != 0) {
            NODE(o1)->unk04 = 1;
        }
        NODE(o1)->parent = (Node *)obj;
        CALL(Fader_start)((int *)INPUT_MGR(D_00000000).fader, 1, 0);
        CALL(memtrack_end)();
    }
    }
    return obj;
}

/* TrnRace_initLabels: TrnRace initLabels: the HUD sprites in trnproc Bss (the lesson
 * header at +0x180, the 16 trick-lesson cards 0x330001..0x330010 at +0x0 step 0x18, and
 * 0x7000C at +0x198). self is unused (cfe still homes it). */
void TrnRace_initLabels(char *self) {
    CALL(gui_spriteInit)(trn_bss + 0x180, (trn_gRowBase + 0x36) << 16);
    CALL(gui_spriteInit)(trn_bss + 0x0, 0x00330001);
    CALL(gui_spriteInit)(trn_bss + 0x18, 0x00330002);
    CALL(gui_spriteInit)(trn_bss + 0x30, 0x00330003);
    CALL(gui_spriteInit)(trn_bss + 0x48, 0x00330004);
    CALL(gui_spriteInit)(trn_bss + 0x60, 0x00330005);
    CALL(gui_spriteInit)(trn_bss + 0x78, 0x00330006);
    CALL(gui_spriteInit)(trn_bss + 0x90, 0x00330007);
    CALL(gui_spriteInit)(trn_bss + 0xA8, 0x00330008);
    CALL(gui_spriteInit)(trn_bss + 0xC0, 0x00330009);
    CALL(gui_spriteInit)(trn_bss + 0xD8, 0x0033000A);
    CALL(gui_spriteInit)(trn_bss + 0xF0, 0x0033000B);
    CALL(gui_spriteInit)(trn_bss + 0x108, 0x0033000C);
    CALL(gui_spriteInit)(trn_bss + 0x120, 0x0033000D);
    CALL(gui_spriteInit)(trn_bss + 0x138, 0x0033000E);
    CALL(gui_spriteInit)(trn_bss + 0x150, 0x0033000F);
    CALL(gui_spriteInit)(trn_bss + 0x168, 0x00330010);
    CALL(gui_spriteInit)(trn_bss + 0x198, 0x0007000C);
}

/* TrnRace_onPauseSelect: TrnRace onPauseSelect (as TimRace_onPauseSelect, plus the
 * dataManager +0xFC guard). */
void TrnRace_onPauseSelect(int *self) {
    int *v0;
    int new_var;
    int v1;
    int stride;
    if (TRNRACE(self)->dataManager->results != 0) {
        return;
    }
    if (CALL(Fader_isDone_00013980)(INPUT_MGR(D_00000000).fader, self) == 0) {
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
        gTrnRace = (char *)self;
        if (1) {
        }
        v0 = (int *) self[(0x48 / 4) ^ 0];
        (*((void (**)(void)) ((((char *) v0) + (v0[0x7C / 4] * stride)) + (new_var = 0x90))))();
    }
}

/* TrnRace_onEndTimerDone: clear the exit step (+0x504). */
void TrnRace_onEndTimerDone(char *self) {
    RACEPROC(self)->unk504 = 0;
}

/* TrnRace_onRaceOver: TrnRace onTimeout: step 0 fades out (Fader_start(3, 1, self)),
 * step 1 waits for the fader, restarts (TrnRace_restart) and fades back in. */
void TrnRace_onRaceOver(char *self) {
    switch (RACEPROC(self)->unk504) {
    case 0:
        CALL(Fader_start)(INPUT_MGR(D_00000000).fader, 3, 1, self);
        RACEPROC(self)->unk504 = 1;
        break;
    case 1:
        if (CALL(Fader_checkDone_00013924)(INPUT_MGR(D_00000000).fader) != 0) {
            CALL(TrnRace_restart)(self);
            CALL(Fader_start)(INPUT_MGR(D_00000000).fader, 1, 1);
        }
        break;
    }
}

/* TrnRace_forceEnd: pause the race: race pads off, DataManager update paused,
 * sub-state 3, state flag 1, counter 0. */
void TrnRace_forceEnd(char *self) {
    CALL(input_setRacePadsActive)(&D_00000000, 0);
    CALL(Node_pauseUpdate_0000E464)(TRNRACE(self)->dataManager);
    RACEPROC(self)->phase = RACE_PHASE_END;
    RACEPROC(self)->raceState = RACE_STATE_RACING;
    RACEPROC(self)->timer = 0;
}

/* TrnRace_restart: restart: reset the pad of the GameFlow's current player
 * (+0x134 -> +0xC4 -> +0x800), then the RaceProcess global slot-11 call. */
void TrnRace_restart(char *self) {
    char *p = WORLDREG(INPUT_MGR(D_00000000).game)->players[0].rider;
    char *pad = (char *)SNOWB(p)->input;
    CALL(pad_reset_000673FC)(pad, PADCTL(pad)->port);
    CALL(RaceProcess_callGlobalSlot11_00008674)(self);
}

/* TrnDataManager_init: TrnDataManager init (TimDataManager_init's shape): name, owner race
 * (+0xB8), HUD layout words, lesson count (+0x108) from the TrickTable, first lesson (TrnDataManager_nextLesson from
 * -1) shown through TrnDataManager_buildLessonInputs; with a pause menu, the points-gadget list and white HUD colour; then
 * virtual slot 10. */
void TrnDataManager_init(char *self, char *race, int spriteSet) {
    int cur;
    NODE(self)->name = (const char *)(trn_data + 0x31C);
    TRNDM(self)->race = (TrnRace *)race;
    TRNDM(self)->base.blinkTimer = 0;
    TRNDM(self)->hudX = 24;
    TRNDM(self)->hudY = 8;
    TRNDM(self)->hudDX = -1;
    TRNDM(self)->hudDY = 40;
    TRNDM(self)->panelX = 270;
    TRNDM(self)->panelY = 215;
    TRNDM(self)->frame = 0;
    TRNDM(self)->count = CALL(TrickTable_getCount_0000C3E8)(trn_gTrickTable);
    TRNDM(self)->cursor = 0;
    cur = CALL(TrnDataManager_nextLesson)(self, -1);
    TRNDM(self)->lesson = cur;
    TRNDM(self)->armed = 0;
    TRNDM(self)->results = 0;
    TRNDM(self)->repeatTimer = 0;
    TRNDM(self)->firstPress = 1;
    TRNDM(self)->button = 0;
    CALL(TrnDataManager_buildLessonInputs)(self, cur);
    if ((TRNDM(self)->race->race.flags << 15) < 0) {
        TRNDM(self)->base.pointsListArg = spriteSet;
        TRNDM(self)->alpha = 0xFF;
        TRNDM(self)->pulse = 0;
        TRNDM(self)->colour[3] = 1.0f;
        TRNDM(self)->colour[2] = 1.0f;
        TRNDM(self)->colour[1] = 1.0f;
        TRNDM(self)->colour[0] = 1.0f;
        CALL(hud_addPointsGadgetList)(self);
    }
    {
        char *vt = (char *)NODE(self)->vptr;
        ((void (*)(int))VT_SLOT(vt, 10).fn)(VT_SLOT(vt, 10).delta + (int)self);
    }
}

/* TrnDataManager_reset: TrnDataManager reset: clear the trick state, then RaceTracker_reset. */
void TrnDataManager_reset(char *self) {
    TRNDM(self)->trickDone = 0;
    TRNDM(self)->course = -1;
    TRNDM(self)->results = 0;
    TRNDM(self)->armed = 0;
    TRNDM(self)->button = 0;
    CALL(RaceTracker_reset)(self);
}

/* TrnDataManager_drawLesson: TrnDataManager HUD draw: the current lesson's title ("%d: %s"
 * with the TrickTable entry name; red once the lesson's trick is done), then its input list
 * (+0x118, -1 terminated): 255 starts a new line, any other entry is a button sprite (0..31 in
 * the Bss table at +0x0, 32.. in the second table at +0xF0), with the "+" sprite between two
 * buttons on one line. */
/* EXACT via the NCC (C++) donor src/trnproc_uso/trnproc_uso_ncc.cpp (REPLACE_FUNC_BODY). */
void TrnDataManager_drawLesson(char *self) {
}

/* TrnDataManager_drawResults: TrnDataManager results screen (12 lesson rows, done ones red, the
 * cursor sprite). EXACT via the NCC (C++) donor src/trnproc_uso/trnproc_uso_ncc.cpp
 * (REPLACE_FUNC_BODY). */
void TrnDataManager_drawResults(char *self) {
}

/* TrnDataManager_scrollResults: results-list scrolling with key repeat: up (0x8008) / down
 * (0x2004) move the cursor (+0x100) through the lesson count (+0x108), scrolling the
 * 12-row window top (+0x104); a move plays sound 1; the repeat timer (+0x10C) restarts at 8
 * on the first press (+0x110) and 2 while held. */
void TrnDataManager_scrollResults(char *self) {
    int held;
    int old;

    old = TRNDM(self)->cursor;
    held = 0;
    if (CALL(input_checkHeld_00011ED4)(&D_00000000, 0x8008) != 0) {
        held = 1;
        if (TRNDM(self)->repeatTimer == 0 && TRNDM(self)->cursor < TRNDM(self)->count - 1) {
            (TRNDM(self)->cursor)++;
            if (TRNDM(self)->cursor - TRNDM(self)->top >= 12
                && TRNDM(self)->top < TRNDM(self)->count - 12) {
                (TRNDM(self)->top)++;
            }
        }
    }
    if (CALL(input_checkHeld_00011ED4)(&D_00000000, 0x2004) != 0) {
        held = 1;
        if (TRNDM(self)->repeatTimer == 0 && TRNDM(self)->cursor > 0) {
            (TRNDM(self)->cursor)--;
            if (TRNDM(self)->cursor - TRNDM(self)->top < 0 && TRNDM(self)->top > 0) {
                (TRNDM(self)->top)--;
            }
        }
    }
    if (old != TRNDM(self)->cursor) {
        CALL(snd_postEvent)(1);
    }
    if (TRNDM(self)->repeatTimer == 0 && held != 0) {
        if (TRNDM(self)->firstPress != 0) {
            TRNDM(self)->repeatTimer = 8;
        } else {
            TRNDM(self)->repeatTimer = 2;
        }
        TRNDM(self)->firstPress = 0;
    }
    if (held == 0) {
        TRNDM(self)->repeatTimer = 0;
        TRNDM(self)->firstPress = 1;
    }
    if (TRNDM(self)->repeatTimer != 0) {
        (TRNDM(self)->repeatTimer)--;
    }
}

/* TrnDataManager_onCheckpoint: TrnDataManager onCheckpoint: +0xF8 = (gate +0x88 < 21), arm the
 * trick lesson (+0xF4), show lesson +0xF0 (TrnDataManager_buildLessonInputs), then RaceTracker_onCheckpoint. */
void TrnDataManager_onCheckpoint(char *self, char *gate) {
    /* RAW: unknown-object gate is RaceTracker_onCheckpoint's checkpoint argument; its +0x88 word (>= 21 = an unrestricted course section) has no named field yet */
    if (*(int *)(gate + 0x88) >= 21) {
        TRNDM(self)->course = 0;
    } else {
        TRNDM(self)->course = 1;
    }
    TRNDM(self)->armed = 1;
    CALL(TrnDataManager_buildLessonInputs)(self, TRNDM(self)->lesson);
    CALL(RaceTracker_onCheckpoint)(self, gate);
}

/* TrnDataManager_onTrickStart: mark the current lesson's trick done (+0x114) when it is index a1. */
void TrnDataManager_onTrickStart(char *self, int idx) {
    if (TRNDM(self)->armed != 0 && idx == TRNDM(self)->lesson) {
        TRNDM(self)->trickDone = 1;
    }
}

/* TrnDataManager_onTrickEnd: lesson passed: sound 39, set the trick's done bit (TrnDataManager_setTrickDone), advance
 * to the next lesson (TrnDataManager_nextLesson) and show it (TrnDataManager_buildLessonInputs). */
void TrnDataManager_onTrickEnd(char *self, int ok) {
    int next;
    if (TRNDM(self)->armed != 0 && TRNDM(self)->trickDone != 0 && ok != 0) {
        CALL(snd_sendEvent)(39, 0);
        CALL(TrnDataManager_setTrickDone)(self, TRNDM(self)->lesson);
        next = CALL(TrnDataManager_nextLesson)(self, TRNDM(self)->lesson);
        TRNDM(self)->lesson = next;
        CALL(TrnDataManager_buildLessonInputs)(self, next);
    }
    TRNDM(self)->trickDone = 0;
}

/* TrnDataManager_setTrickDone: set trick idx's done bit in the race's bitset (+0xB8 -> +0x6B0);
 * fatal on an index outside the TrickTable. */
void TrnDataManager_setTrickDone(char *self, int idx) {
    if (idx < 0 || (unsigned)idx >= (unsigned)CALL(TrickTable_getCount_0000C3E8)(trn_gTrickTable)) {
        CALL(sys_fatal)(trn_data + 0x338);
    }
    {
        char *bits = (char *)&TRNDM(self)->race->trickDone[idx >> 3];
        *bits = *bits | (1 << (idx & 7));
    }
}

/* TrnDataManager_isTrickDone: trick idx's done bit. */
int TrnDataManager_isTrickDone(char *self, int idx) {
    return TRNDM(self)->race->trickDone[idx >> 3] & (1 << (idx & 7));
}

/* TrnDataManager_buildLessonInputs: build lesson idx's input list at +0x118. EXACT via the NCC (C++)
 * donor src/trnproc_uso/trnproc_uso_ncc.cpp (REPLACE_FUNC_BODY). */
void TrnDataManager_buildLessonInputs(char *self, int idx) {
}

/* TrnDataManager_stickDirSprite: stick direction bits (input & 0xF0) -> direction sprite 1..8
 * (0 = none). self is unused. */
int TrnDataManager_stickDirSprite(char *self, int input) {
    int d = input & 0xF0;
    if (d == 0x40) {
        return 1;
    }
    if (d == 0x60) {
        return 2;
    }
    if (d == 0x20) {
        return 3;
    }
    if (d == 0xA0) {
        return 4;
    }
    if (d == 0x80) {
        return 5;
    }
    if (d == 0x90) {
        return 6;
    }
    if (d == 0x10) {
        return 7;
    }
    if (d == 0x50) {
        return 8;
    }
    return 0;
}

/* TrnDataManager_isLessonAvailable: is lesson idx available (TrickTable unlock mask against the
 * player's bits; flag 4 excluded on the restricted course). EXACT via the NCC (C++) donor
 * src/trnproc_uso/trnproc_uso_ncc.cpp (REPLACE_FUNC_BODY). */
int TrnDataManager_isLessonAvailable(char *self, int idx) {
}

/* TrnDataManager_nextLesson: next lesson after cur (wrapping at the count +0x108) that TrnDataManager_isLessonAvailable
 * accepts; cur itself if none. */
int TrnDataManager_nextLesson(char *self, int cur) {
    int i;
    for (i = 0; i < TRNDM(self)->count; i++) {
        cur++;
        if (cur == TRNDM(self)->count) {
            cur = 0;
        }
        if (CALL(TrnDataManager_isLessonAvailable)(self, cur) != 0) {
            return cur;
        }
    }
    return cur;
}

/* TrnDataManager_update: TrnDataManager update (Start disarms the lesson or opens the results
 * screen; there a picked available lesson becomes current). EXACT via the NCC (C++) donor
 * src/trnproc_uso/trnproc_uso_ncc.cpp (REPLACE_FUNC_BODY). */
void TrnDataManager_update(char *self) {
}

/* TrnDataManager_display: TrnDataManager display: the results screen (TrnDataManager_drawResults) once the course
 * is over (+0xFC); else while the race is not paused (+0x30) and a lesson is set (+0xF8 >= 0),
 * the lesson panel (+0x150) and its header (+0x180) at (+0xE8, +0xEC), and the lesson HUD (TrnDataManager_drawLesson)
 * while armed; then RaceTracker_display. */
void TrnDataManager_display(char *self) {
    if (TRNDM(self)->results != 0) {
        CALL(TrnDataManager_drawResults)(self);
    } else if (TRNDM(self)->race->race.unk030 == 0 && TRNDM(self)->course >= 0) {
        CALL(gui_spriteRefresh)(trn_bss + 0x150);
        CALL(gui_setCombine_00001794)(trn_bss + 0x150);
        CALL(gui_drawSprite)(trn_bss + 0x150, TRNDM(self)->panelX, TRNDM(self)->panelY, 2);
        CALL(gui_spriteRefresh)(trn_bss + 0x180);
        CALL(gui_setCombine_00001794)(trn_bss + 0x180);
        CALL(gui_drawSprite)(trn_bss + 0x180, TRNDM(self)->panelX - 4, TRNDM(self)->panelY, 6);
        if (TRNDM(self)->race->race.unk030 == 0 && TRNDM(self)->armed != 0) {
            CALL(TrnDataManager_drawLesson)(self);
        }
    }
    CALL(RaceTracker_display)(self);
}

/* ParmInt_read_00001B84: Parm<Int>::read: skip the tag int, read the value at +0x10
 * (template, as tit_readIntParm). */
void ParmInt_read_00001B84(char *dst) {
    int tmp;
    trn_streamReadS32(&tmp);
    trn_streamReadS32((int *)(dst + 0x10));
}

/* ParmFloat_read_00001BB4: Parm<Float>::read: skip the tag int, read the value at +0x10
 * (template, as tit_readFloatParm). */
void ParmFloat_read_00001BB4(char *dst) {
    int tmp;
    trn_streamReadS32(&tmp);
    trn_streamReadF32((float *)(dst + 0x10));
}

/* ParmQuad_read_00001BE4: Parm<Quad>::read: skip the tag int, read the value at +0x10
 * (template, as tit_readQuadParm). */
void ParmQuad_read_00001BE4(char *dst) {
    int tmp;
    trn_streamReadS32(&tmp);
    trn_streamReadQuad((TrnQuad4 *)(dst + 0x10));
}

/* TrnItemType_create: ItemType create: new ColourItem subclass (0x40 bytes, vtable
 * bootup Data+0x75A0), linked under the template's list head at +0x40 (template, as
 * h2h_ItemType_create_00001A6C). */
void *TrnItemType_create(int *arg0) {
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

/* ParmVec3_read_00001CA4: Parm<Vec3>::read: skip the tag int, read the value at +0x10
 * (template, as tit_readVec3Parm). */
void ParmVec3_read_00001CA4(char *dst) {
    int tmp;
    trn_streamReadS32(&tmp);
    trn_streamReadVec3((float *)(dst + 0x10));
}

