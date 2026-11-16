#include "common.h"
#include "game/course.h"
#include "gdl/node.h"
#include "gdl/race.h"
#include "game/input.h"
#include "game/guimenu.h"
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

/* trnproc_uso_func_00000000 / 00000008: empty functions. */
void trnproc_uso_func_00000000(void) {
}

void trnproc_uso_func_00000008(void) {
}

/* trnproc_uso_func_00000010: trn_procMain, the mode's process loop (n64_procMain's template):
 * 0 launch the GenProc (next 1, then 4), 1 reset (194, state 2), 2 sound + build the TrnRace
 * (3A8) and run it, 3 resume (state = next), 4 quit. The switch table is RoData+0 (the unit's
 * .rodata is pinned NOLOAD at VMA 0 in tenshoe.ld, the arcproc 240 recipe). */
void trnproc_uso_func_00000194(int *out);
void trnproc_uso_func_000001A4(int a0);
void trnproc_uso_func_00000010(int arg0, int arg1) {
    int *cur;
    int flag = 0;
    int r;

    do {
        switch (arg1) {
        case 0:
            CALL(app_launchGenProc)(arg0, 1, 3, 1);
            flag = 1;
            *(int *)((char *)&D_00000000 + 0x44) = 1;
            *(int *)((char *)&D_00000000 + 0x48) = 4;
            break;
        case 1:
            CALL(trnproc_uso_func_00000194)(arg0);
            *(int *)((char *)&D_00000000 + 0x40) = 2;
            break;
        case 2:
            CALL(snd_playSeq_000120A8)(&D_00000000, 4, 8, 5);
            flag = 1;
            *(int *)((char *)&D_00000000 + 0x34) = 6;
            r = CALL(app_createGame_000092F4)(arg0, 1);
            *(int *)((char *)&D_00000000 + 0x34) = 7;
            r = CALL(trnproc_uso_func_000003A8)(0, 0x4010000, r);
            CALL(app_runProcess)(arg0, 0, r);
            break;
        case 3:
            CALL(trnproc_uso_func_000001A4)(arg0);
            *(int *)((char *)&D_00000000 + 0x40) = *(int *)((char *)&D_00000000 + 0x44);
            break;
        case 4:
            cur = (int *)CALL(QuitProcess_ctor)(0, 1, 0);
            flag = 1;
            CALL(list_append)((char *)&D_00000000 + 0x10, cur);
            if ((int)NODE(cur)->parent != 0) {
                NODE(cur)->unk04 = 1;
            }
            NODE(cur)->parent = (Node *)&D_00000000;
            break;
        }
        arg1 = *(int *)((char *)&D_00000000 + 0x40);
    } while (flag == 0);
}

/* trnproc_uso_func_00000194: clear *out and GameFlow +0x14C. */
void trnproc_uso_func_00000194(int *out) {
    *out = 0;
    *(int *)((char *)&D_00000000 + 0x14C) = 0;
}

/* trnproc_uso_func_000001A4: empty one-argument function (the word after it is .text pad). */
void trnproc_uso_func_000001A4(int a0) {
}

/* trnproc_uso_func_000001B0: pause menu "continue" (as TimRace_pauseContinue_000006B0, without
 * the input_clearActivePad). */
void trnproc_uso_func_000001B0(void) {
    CALL(Process_resume_0000FAE8)(gTrnRace);
    CALL(Node_resumeUpdate_0000E450)(TIMRACE(gTrnRace)->dataManager);
}

/* trnproc_uso_func_000001E4: pause menu row 1 (restart): resume, then the restart helper 9A0. */
void trnproc_uso_func_000001E4(void) {
    CALL(Process_resume_0000FAE8)(gTrnRace);
    CALL(trnproc_uso_func_000009A0)(gTrnRace);
}

/* trnproc_uso_func_00000214: pause menu "quit": next flow state 3 / 4, save the setting at
 * +0x6B0, fade the process out (as TrkRace_pauseQuit_00000818). */
void trnproc_uso_func_00000214(void) {
    INPUT_MGR(D_00000000).procState = 3;
    *(int *)((char *)&D_00000000 + 0x44) = 4;
    CALL(settings_set8_00012818)(&D_00000000, gTrnRace + 0x6B0);
    CALL(Process_fadeOut_0000F954)(gTrnRace, -1, 0);
}

/* trnproc_uso_func_00000268: read an int from the gdl file (cross-USO template, as checkpoint
 * checkpoint_gdlReadInt_00000034 / game_uso stream_readS32_00000280). */
void trnproc_uso_func_00000268(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

/* trnproc_uso_func_000002A4: read a float from the gdl file (template, as balloon_gdlReadFloat_00000104). */
void trnproc_uso_func_000002A4(float *dst) {
    float buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

/* trnproc_uso_func_000002E0: read a 16-byte quad from the gdl file (template, as
 * tim_streamReadQuad_000008B4). */
void trnproc_uso_func_000002E0(TrnQuad4 *dst) {
    TrnQuad4 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 16);
    *dst = buf;
}

/* trnproc_uso_func_00000338: read a Vec3 from the gdl file (template, as balloon_gdlReadVec3_00000094 /
 * game_uso stream_readVec3_000001D4). */
void trnproc_uso_func_00000338(float *dst) {
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

/* trnproc_uso_func_000003A8: TrnRace ctor (TimRace_ctor's skeleton: inlined Node / Process /
 * RaceProcess base ctors, then the TrnDataManager (0x19C RaceTracker subclass, lesson count +0xB4 = 17),
 * a three-row pause menu and the saved trick bits read into +0x6B0). The vtable stores carry their
 * Data offset in the USO reloc, so each vptr is its own 0-pinned symbol. */
extern char trn_vt_node, trn_vt_proc, trn_vt_raceproc, trn_vt_trnrace, trn_vt_dm;
/* bootup Bss+0x28C: the gui string-table base of the pause-menu rows. */
extern int trn_gRowBase;
/* The pause rows' callbacks (1B0 / 1E4 / 214): Text relocs whose in-place addend is 0, so
 * each address is a 0-pinned alias of the function. */
extern void trnproc_uso_func_000001B0_ref(void), trnproc_uso_func_000001E4_ref(void), trnproc_uso_func_00000214_ref(void);
/* D_00000000 (bootup Bss+0xF8, the InputMgr) under a second 0-pinned name: the dataMgr store
 * must not share the earlier &D_00000000 lui (as TimRace's D_b1_097C_g). */
extern char trn_gInputMgr;
/* void-typed alias of the global A2C call (its dead int return would hold v0; the vtable base
 * that follows colours v0, as TrkRace_ctor's gl_func_00000000_994v). */
extern void trn_func_00000A2C_v();
char *trnproc_uso_func_000003A8(char *obj, int a1, int a2) {
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
    CALL(trnproc_uso_func_000006CC)(obj);
    *(float *)(obj + 0x6AC) = 0.0f;
    CALL(memtrack_begin)(trn_data + 0x314, 0);
    CALL(input_setActivePad_0001207C)(&D_00000000, 0);

    {
        o2 = (char *)CALL(mem_new)(0x19C);
        if (o2 != 0) {
            CALL(RaceTracker_ctor)(o2);
            NODE(o2)->vptr = (char *)&trn_vt_dm;
        }
        *(char **)(obj + 0x6A8) = o2;
        INPUT_MGR(trn_gInputMgr).dataMgr = (struct RaceTracker *)o2;
        o1 = *(char **)(obj + 0x6A8);
        *(int *)(o1 + 0xB4) = 17;
        XCALL(trn_func_00000A2C_v, trnproc_uso_func_00000A2C)(*(char **)(obj + 0x6A8), obj, RACEPROC(obj)->spriteSet);

        w = *(char **)(obj + 0x6A8);
        o1 = (char *)NODE(w)->vptr;
        ((void (*)(int))VT_SLOT(o1, 10).fn)(VT_SLOT(o1, 10).delta + (int)w);

        o1 = *(char **)(obj + 0x6A8);
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
            CALL(gui_addRowSub_00010B6C)(RACEPROC(obj)->menu, (*(int *)&trn_gRowBase + 3) << 16, -1, trnproc_uso_func_000001B0_ref);
            CALL(gui_addRowSub_00010B6C)(RACEPROC(obj)->menu, ((*(int *)&trn_gRowBase + 3) << 16) | 1, -1, trnproc_uso_func_000001E4_ref);
            CALL(gui_addRowSub_00010B6C)(RACEPROC(obj)->menu, ((*(int *)&trn_gRowBase + 3) << 16) | 5, -1, trnproc_uso_func_00000214_ref);
            CALL(gui_centerRows_0001016C)(RACEPROC(obj)->menu);
            o1 = (char *)RACEPROC(obj)->menu;
            CALL(list_append)(obj + 16, o1);
            if (NODE(o1)->parent != 0) {
                NODE(o1)->unk04 = 1;
            }
            NODE(o1)->parent = (Node *)obj;
        }

        CALL(settings_get8_000127CC)(&D_00000000, obj + 0x6B0);
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
    return obj;
}

/* trnproc_uso_func_000006CC: TrnRace initLabels: the HUD sprites in trnproc Bss (the lesson
 * header at +0x180, the 16 trick-lesson cards 0x330001..0x330010 at +0x0 step 0x18, and
 * 0x7000C at +0x198). self is unused (cfe still homes it). */
void trnproc_uso_func_000006CC(char *self) {
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

/* trnproc_uso_func_00000858: TrnRace onPauseSelect (as TimRace_onPauseSelect, plus the
 * dataManager +0xFC guard). */
void trnproc_uso_func_00000858(int *self) {
    int *v0;
    int new_var;
    int v1;
    int stride;
    if (*(int *)((char *)TIMRACE(self)->dataManager + 0xFC) != 0) {
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

/* trnproc_uso_func_00000908: clear the exit step (+0x504). */
void trnproc_uso_func_00000908(char *self) {
    *(int *)(self + 0x504) = 0;
}

/* trnproc_uso_func_00000910: TrnRace onTimeout: step 0 fades out (Fader_start(3, 1, self)),
 * step 1 waits for the fader, restarts (9F0) and fades back in. */
void trnproc_uso_func_00000910(char *self) {
    switch (*(int *)(self + 0x504)) {
    case 0:
        CALL(Fader_start)(INPUT_MGR(D_00000000).fader, 3, 1, self);
        *(int *)(self + 0x504) = 1;
        break;
    case 1:
        if (CALL(Fader_checkDone_00013924)(INPUT_MGR(D_00000000).fader) != 0) {
            CALL(trnproc_uso_func_000009F0)(self);
            CALL(Fader_start)(INPUT_MGR(D_00000000).fader, 1, 1);
        }
        break;
    }
}

/* trnproc_uso_func_000009A0: pause the race: race pads off, DataManager update paused,
 * sub-state 3, state flag 1, counter 0. */
void trnproc_uso_func_000009A0(char *self) {
    CALL(input_setRacePadsActive)(&D_00000000, 0);
    CALL(Node_pauseUpdate_0000E464)(TIMRACE(self)->dataManager);
    *(int *)(self + 0x4E0) = 3;
    *(int *)(self + 0x4DC) = 1;
    *(int *)(self + 0x4E4) = 0;
}

/* trnproc_uso_func_000009F0: restart: reset the pad of the GameFlow's current player
 * (+0x134 -> +0xC4 -> +0x800), then the RaceProcess global slot-11 call. */
void trnproc_uso_func_000009F0(char *self) {
    char *p = *(char **)(*(char **)((char *)&D_00000000 + 0x134) + 0xC4);
    char *pad = *(char **)(p + 0x800);
    CALL(pad_reset_000673FC)(pad, *(int *)(pad + 0x34));
    CALL(RaceProcess_callGlobalSlot11_00008674)(self);
}

/* trnproc_uso_func_00000A2C: TrnDataManager init (TimDataManager_init's shape): name, owner race
 * (+0xB8), HUD layout words, lesson count (+0x108) from the TrickTable, first lesson (180C from
 * -1) shown through 13C4; with a pause menu, the points-gadget list and white HUD colour; then
 * virtual slot 10. */
void trnproc_uso_func_00000A2C(char *self, char *race, int spriteSet) {
    int cur;
    NODE(self)->name = (const char *)(trn_data + 0x31C);
    *(char **)(self + 0xB8) = race;
    *(int *)(self + 0xAC) = 0;
    *(int *)(self + 0xD8) = 24;
    *(int *)(self + 0xDC) = 8;
    *(int *)(self + 0xE0) = -1;
    *(int *)(self + 0xE4) = 40;
    *(int *)(self + 0xE8) = 270;
    *(int *)(self + 0xEC) = 215;
    *(int *)(self + 0xD4) = 0;
    *(int *)(self + 0x108) = CALL(TrickTable_getCount_0000C3E8)(trn_gTrickTable);
    *(int *)(self + 0x100) = 0;
    cur = CALL(trnproc_uso_func_0000180C)(self, -1);
    *(int *)(self + 0xF0) = cur;
    *(int *)(self + 0xF4) = 0;
    *(int *)(self + 0xFC) = 0;
    *(int *)(self + 0x10C) = 0;
    *(int *)(self + 0x110) = 1;
    *(int *)(self + 0x198) = 0;
    CALL(trnproc_uso_func_000013C4)(self, cur);
    if ((RACEPROC(*(char **)(self + 0xB8))->flags << 15) < 0) {
        *(int *)(self + 0x54) = spriteSet;
        *(int *)(self + 0xCC) = 0xFF;
        *(int *)(self + 0xD0) = 0;
        *(float *)(self + 0xC8) = 1.0f;
        *(float *)(self + 0xC4) = 1.0f;
        *(float *)(self + 0xC0) = 1.0f;
        *(float *)(self + 0xBC) = 1.0f;
        CALL(hud_addPointsGadgetList)(self);
    }
    {
        char *vt = (char *)NODE(self)->vptr;
        ((void (*)(int))VT_SLOT(vt, 10).fn)(VT_SLOT(vt, 10).delta + (int)self);
    }
}

/* trnproc_uso_func_00000B44: TrnDataManager reset: clear the trick state, then RaceTracker_reset. */
void trnproc_uso_func_00000B44(char *self) {
    *(int *)(self + 0x114) = 0;
    *(int *)(self + 0xF8) = -1;
    *(int *)(self + 0xFC) = 0;
    *(int *)(self + 0xF4) = 0;
    *(int *)(self + 0x198) = 0;
    CALL(RaceTracker_reset)(self);
}

/* trnproc_uso_func_00000B78: TrnDataManager HUD draw: the current lesson's title ("%d: %s"
 * with the TrickTable entry name; red once the lesson's trick is done), then its input list
 * (+0x118, -1 terminated): 255 starts a new line, any other entry is a button sprite (0..31 in
 * the Bss table at +0x0, 32.. in the second table at +0xF0), with the "+" sprite between two
 * buttons on one line. */
/* EXACT via the NCC (C++) donor src/trnproc_uso/trnproc_uso_ncc.cpp (REPLACE_FUNC_BODY). */
void trnproc_uso_func_00000B78(char *self) {
}

/* trnproc_uso_func_00000DD4: TrnDataManager results screen (12 lesson rows, done ones red, the
 * cursor sprite). EXACT via the NCC (C++) donor src/trnproc_uso/trnproc_uso_ncc.cpp
 * (REPLACE_FUNC_BODY). */
void trnproc_uso_func_00000DD4(char *self) {
}

/* trnproc_uso_func_00001098: results-list scrolling with key repeat: up (0x8008) / down
 * (0x2004) move the cursor (+0x100) through the lesson count (+0x108), scrolling the
 * 12-row window top (+0x104); a move plays sound 1; the repeat timer (+0x10C) restarts at 8
 * on the first press (+0x110) and 2 while held. */
void trnproc_uso_func_00001098(char *self) {
    int held;
    int old;

    old = *(int *)(self + 0x100);
    held = 0;
    if (CALL(input_checkHeld_00011ED4)(&D_00000000, 0x8008) != 0) {
        held = 1;
        if (*(int *)(self + 0x10C) == 0 && *(int *)(self + 0x100) < *(int *)(self + 0x108) - 1) {
            (*(int *)(self + 0x100))++;
            if (*(int *)(self + 0x100) - *(int *)(self + 0x104) >= 12
                && *(int *)(self + 0x104) < *(int *)(self + 0x108) - 12) {
                (*(int *)(self + 0x104))++;
            }
        }
    }
    if (CALL(input_checkHeld_00011ED4)(&D_00000000, 0x2004) != 0) {
        held = 1;
        if (*(int *)(self + 0x10C) == 0 && *(int *)(self + 0x100) > 0) {
            (*(int *)(self + 0x100))--;
            if (*(int *)(self + 0x100) - *(int *)(self + 0x104) < 0 && *(int *)(self + 0x104) > 0) {
                (*(int *)(self + 0x104))--;
            }
        }
    }
    if (old != *(int *)(self + 0x100)) {
        CALL(snd_postEvent)(1);
    }
    if (*(int *)(self + 0x10C) == 0 && held != 0) {
        if (*(int *)(self + 0x110) != 0) {
            *(int *)(self + 0x10C) = 8;
        } else {
            *(int *)(self + 0x10C) = 2;
        }
        *(int *)(self + 0x110) = 0;
    }
    if (held == 0) {
        *(int *)(self + 0x10C) = 0;
        *(int *)(self + 0x110) = 1;
    }
    if (*(int *)(self + 0x10C) != 0) {
        (*(int *)(self + 0x10C))--;
    }
}

/* trnproc_uso_func_00001228: TrnDataManager onCheckpoint: +0xF8 = (gate +0x88 < 21), arm the
 * trick lesson (+0xF4), show lesson +0xF0 (13C4), then RaceTracker_onCheckpoint. */
void trnproc_uso_func_00001228(char *self, char *gate) {
    if (*(int *)(gate + 0x88) >= 21) {
        *(int *)(self + 0xF8) = 0;
    } else {
        *(int *)(self + 0xF8) = 1;
    }
    *(int *)(self + 0xF4) = 1;
    CALL(trnproc_uso_func_000013C4)(self, *(int *)(self + 0xF0));
    CALL(RaceTracker_onCheckpoint)(self, gate);
}

/* trnproc_uso_func_00001284: mark the current lesson's trick done (+0x114) when it is index a1. */
void trnproc_uso_func_00001284(char *self, int idx) {
    if (*(int *)(self + 0xF4) != 0 && idx == *(int *)(self + 0xF0)) {
        *(int *)(self + 0x114) = 1;
    }
}

/* trnproc_uso_func_000012AC: lesson passed: sound 39, set the trick's done bit (1324), advance
 * to the next lesson (180C) and show it (13C4). */
void trnproc_uso_func_000012AC(char *self, int ok) {
    int next;
    if (*(int *)(self + 0xF4) != 0 && *(int *)(self + 0x114) != 0 && ok != 0) {
        CALL(snd_sendEvent)(39, 0);
        CALL(trnproc_uso_func_00001324)(self, *(int *)(self + 0xF0));
        next = CALL(trnproc_uso_func_0000180C)(self, *(int *)(self + 0xF0));
        *(int *)(self + 0xF0) = next;
        CALL(trnproc_uso_func_000013C4)(self, next);
    }
    *(int *)(self + 0x114) = 0;
}

/* trnproc_uso_func_00001324: set trick idx's done bit in the race's bitset (+0xB8 -> +0x6B0);
 * fatal on an index outside the TrickTable. */
void trnproc_uso_func_00001324(char *self, int idx) {
    if (idx < 0 || (unsigned)idx >= (unsigned)CALL(TrickTable_getCount_0000C3E8)(trn_gTrickTable)) {
        CALL(sys_fatal)(trn_data + 0x338);
    }
    {
        char *bits = *(char **)(self + 0xB8) + (idx >> 3) + 0x6B0;
        *bits = *bits | (1 << (idx & 7));
    }
}

/* trnproc_uso_func_000013A0: trick idx's done bit. */
int trnproc_uso_func_000013A0(char *self, int idx) {
    return (*(unsigned char **)(self + 0xB8) + 0x6B0)[idx >> 3] & (1 << (idx & 7));
}

/* trnproc_uso_func_000013C4: build lesson idx's input list at +0x118. EXACT via the NCC (C++)
 * donor src/trnproc_uso/trnproc_uso_ncc.cpp (REPLACE_FUNC_BODY). */
void trnproc_uso_func_000013C4(char *self, int idx) {
}

/* trnproc_uso_func_000016C8: stick direction bits (input & 0xF0) -> direction sprite 1..8
 * (0 = none). self is unused. */
int trnproc_uso_func_000016C8(char *self, int input) {
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

/* trnproc_uso_func_00001778: is lesson idx available (TrickTable unlock mask against the
 * player's bits; flag 4 excluded on the restricted course). EXACT via the NCC (C++) donor
 * src/trnproc_uso/trnproc_uso_ncc.cpp (REPLACE_FUNC_BODY). */
int trnproc_uso_func_00001778(char *self, int idx) {
}

/* trnproc_uso_func_0000180C: next lesson after cur (wrapping at the count +0x108) that 1778
 * accepts; cur itself if none. */
int trnproc_uso_func_0000180C(char *self, int cur) {
    int i;
    for (i = 0; i < *(int *)(self + 0x108); i++) {
        cur++;
        if (cur == *(int *)(self + 0x108)) {
            cur = 0;
        }
        if (CALL(trnproc_uso_func_00001778)(self, cur) != 0) {
            return cur;
        }
    }
    return cur;
}

/* trnproc_uso_func_0000188C: TrnDataManager update (Start disarms the lesson or opens the results
 * screen; there a picked available lesson becomes current). EXACT via the NCC (C++) donor
 * src/trnproc_uso/trnproc_uso_ncc.cpp (REPLACE_FUNC_BODY). */
void trnproc_uso_func_0000188C(char *self) {
}

/* trnproc_uso_func_00001A98: TrnDataManager display: the results screen (DD4) once the course
 * is over (+0xFC); else while the race is not paused (+0x30) and a lesson is set (+0xF8 >= 0),
 * the lesson panel (+0x150) and its header (+0x180) at (+0xE8, +0xEC), and the lesson HUD (B78)
 * while armed; then RaceTracker_display. */
void trnproc_uso_func_00001A98(char *self) {
    if (*(int *)(self + 0xFC) != 0) {
        CALL(trnproc_uso_func_00000DD4)(self);
    } else if (*(int *)(*(char **)(self + 0xB8) + 0x30) == 0 && *(int *)(self + 0xF8) >= 0) {
        CALL(gui_spriteRefresh)(trn_bss + 0x150);
        CALL(gui_setCombine_00001794)(trn_bss + 0x150);
        CALL(gui_drawSprite)(trn_bss + 0x150, *(int *)(self + 0xE8), *(int *)(self + 0xEC), 2);
        CALL(gui_spriteRefresh)(trn_bss + 0x180);
        CALL(gui_setCombine_00001794)(trn_bss + 0x180);
        CALL(gui_drawSprite)(trn_bss + 0x180, *(int *)(self + 0xE8) - 4, *(int *)(self + 0xEC), 6);
        if (*(int *)(*(char **)(self + 0xB8) + 0x30) == 0 && *(int *)(self + 0xF4) != 0) {
            CALL(trnproc_uso_func_00000B78)(self);
        }
    }
    CALL(RaceTracker_display)(self);
}

/* trnproc_uso_func_00001B84: Parm<Int>::read: skip the tag int, read the value at +0x10
 * (template, as tit_readIntParm). */
void trnproc_uso_func_00001B84(char *dst) {
    int tmp;
    trnproc_uso_func_00000268(&tmp);
    trnproc_uso_func_00000268((int *)(dst + 0x10));
}

/* trnproc_uso_func_00001BB4: Parm<Float>::read: skip the tag int, read the value at +0x10
 * (template, as tit_readFloatParm). */
void trnproc_uso_func_00001BB4(char *dst) {
    int tmp;
    trnproc_uso_func_00000268(&tmp);
    trnproc_uso_func_000002A4((float *)(dst + 0x10));
}

/* trnproc_uso_func_00001BE4: Parm<Quad>::read: skip the tag int, read the value at +0x10
 * (template, as tit_readQuadParm). */
void trnproc_uso_func_00001BE4(char *dst) {
    int tmp;
    trnproc_uso_func_00000268(&tmp);
    trnproc_uso_func_000002E0((TrnQuad4 *)(dst + 0x10));
}

/* trnproc_uso_func_00001C14: ItemType create: new ColourItem subclass (0x40 bytes, vtable
 * bootup Data+0x75A0), linked under the template's list head at +0x40 (template, as
 * h2h_ItemType_create_00001A6C). */
void *trnproc_uso_func_00001C14(int *arg0) {
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

/* trnproc_uso_func_00001CA4: Parm<Vec3>::read: skip the tag int, read the value at +0x10
 * (template, as tit_readVec3Parm). */
void trnproc_uso_func_00001CA4(char *dst) {
    int tmp;
    trnproc_uso_func_00000268(&tmp);
    trnproc_uso_func_00000338((float *)(dst + 0x10));
}

