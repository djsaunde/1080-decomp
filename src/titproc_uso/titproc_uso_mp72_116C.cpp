#include "../../include/uso_call.h"  /* NCC donors build without -I include */
/* MIPSpro 7.2.1 C++ (o32 edgcpfe, EDG 2.29) EXACT donor for TitleDemoProcess_handleInput (290 words), built by
 * tools/ncc721.sh and spliced over a placeholder in both trees (agent-h runs 355 / 364). Its 4-label
 * `switch (s0->0x40)` is a native 4-entry table only under EDG 2.29 (the cfe body needs a phantom case 4).
 * lay_5DC only puts the table at .rodata+0x30 (5DC's 12-word table is RoData +0x0); it is not spliced.
 * Run 364 (kit CDLI): the case-3 pointer is ONE variable `v1` for the then-arm, the else-arm's &D, the fade
 * and the 0x40100 record; with `c = v1 + 0x3C` from it, uopt sinks c's addu into the then-arm, so the then
 * block starts by READING v1 and as1 xbb can no longer hoist the else-arm's `la v1` into the if-head (it
 * pulls the addu back instead: the target's `addiu v0,v1,60; slti; beqz; nop`). The `if (1) {}` splits in the fade / record
 * spread v1's live range over more uopt blocks: its priority drops below `c`'s, so `c` and the constant 40
 * take v0 and the pointer v1 as in the target. The record's code-free `if ((char *)TITLEDEMO(s0)->menu) {}`
 * tests are gone (they made a second record-pointer LR). Unused d3 kept from the earlier body.
 */
/* Field views (agent-h run 529). edg rejects the gdl headers' &((T *)0)->f asserts, so this donor
 * gets layout mirrors of the include/ types it touches (keep them in step): TitleDemoProcess
 * (game/modeproc.h), WhiteFader (modeproc.h), GuiMenu rows / cursor (game/guimenu.h), the Node vptr
 * and Cfront vtable slots (gdl/node.h VT_SLOT) and the DataManager entries[0].finished
 * (gdl/race.h RaceTracker / RaceEntry). */
typedef struct Node { char pad00[0x28]; int vptr; } Node;
typedef struct GdlVtEnt { short delta; short pad; int (*fn)(...); } GdlVtEnt;
#define VT_SLOT(vt, n) (((GdlVtEnt *)((char *)(vt) + 8))[n])
#define NODE(p) ((Node *)(p))
typedef struct TitleDemoProcess {
    /* 0x00 */ char process[0x3C];
    /* 0x3C */ int timer;
    /* 0x40 */ int state;
    /* 0x44 */ char unk44[0x50 - 0x44];
    /* 0x50 */ struct DataManager *dataMgr;
    /* 0x54 */ struct KeyMessage *keyMessage;
    /* 0x58 */ struct MainLogo *mainLogo;
    /* 0x5C */ struct GuiMenu *menu;
    /* 0x60 */ Node *langMenu;
    /* 0x64 */ struct WhiteFader *whiteFader;
    /* 0x68 */ int keyDelay;
    /* 0x6C */ int unk6C;
    /* 0x70 */ int unk70;
    /* 0x74 */ int unk74;
} TitleDemoProcess;
#define TITLEDEMO(p) ((TitleDemoProcess *)(p))
typedef struct WhiteFader { char node[0x2C]; float colour[4]; int alpha; } WhiteFader;
#define WFADER(p) ((WhiteFader *)(p))
typedef void (*GuiRowFn)(void); /* as game/guimenu.h */
typedef struct GuiRow { int label; int enabled; int id; GuiRowFn arg; char sub[0x18]; } GuiRow;
typedef struct GuiMenu { char head[0x7C]; int cursor; int locked; GuiRow rows[16]; } GuiMenu;
#define GUIMENU(p) ((GuiMenu *)(p))
typedef struct RaceEntry { char pad00[0x34]; int finished; } RaceEntry;
typedef struct DataManager { char pad00[0x44]; RaceEntry *entries; } DataManager;
/* game/input.h InputMgr (GameFlow, bootup Bss 0xF8) up to unk84 (the picked title-menu row) */
typedef struct InputMgr { char pad00[0x84]; int unk84; } InputMgr;
#define INPUT_MGR(sym) (*(InputMgr *)&(sym))
#define DATAMGR(p) ((DataManager *)(p))
/* import_00263D30 (game_libs 0x80263D30): the shared 0x18-byte gui sprite table the title overlays draw from
 * (KeyMessage_display +0x0 / +0x18, MainLogo_display +0x60 / +0x78 / +0x90), then a word at +0xA8 that this
 * function sets to the TitleDemoProcess just before it runs the picked row's callback (the menu callbacks'
 * owner process). */
typedef struct GuiSpriteTable { char sprites[7][0x18]; char *menuOwner; } GuiSpriteTable;

extern "C" {
extern int gl_func_00000000(...);
extern char D_00000000;
extern char import_00020098;
extern char import_00263D30;
extern void gl_func_00000000_116c_v(...);

void titproc_uso_func_000005DC_lay(int x, int *o) {
    switch (x) {
    case 0: o[0] = 10; break; case 1: o[1] = 11; break; case 2: o[2] = 12; break; case 3: o[3] = 13; break;
    case 4: o[4] = 14; break; case 5: o[5] = 15; break; case 6: o[6] = 16; break; case 7: o[7] = 17; break;
    case 8: o[8] = 18; break; case 9: o[9] = 19; break; case 10: o[10] = 20; break; case 11: o[11] = 21; break;
    }
}

void TitleDemoProcess_handleInput(char *s0) {
    char *v1, *vt;
    char *d3;
    char *vt2;
    char *vt3;
    char *vt4;
    char *vt5;
    char *vt6;
    char *vt7;
    char *vt8;
    void (*fn)(int);
    int v;

    switch (TITLEDEMO(s0)->state) {
    case 0: {
        TITLEDEMO(s0)->timer = TITLEDEMO(s0)->timer - 1;
        if (TITLEDEMO(s0)->timer >= 0) {
            if (CALL(input_checkPressed_00011E00)((char *)&D_00000000, 0x40300) == 0) {
                return;
            }
        }
        vt = (char *)NODE(TITLEDEMO(s0)->mainLogo)->vptr;
        v1 = (char *)TITLEDEMO(s0)->mainLogo;
        ((void (*)(int))VT_SLOT(vt, 10).fn)((VT_SLOT(vt, 10).delta + (int)v1));
        XCALL(gl_func_00000000_116c_v, MainLogo_reset)(TITLEDEMO(s0)->mainLogo);
        TITLEDEMO(s0)->state = TITLEDEMO(s0)->state + 1;
        TITLEDEMO(s0)->timer = TITLEDEMO(s0)->unk70 * 30;
        TITLEDEMO(s0)->keyDelay = 40;
        XCALL(gl_func_00000000_116c_v, snd_postEvent)(13);
        break;
    }
    case 1: {
        v = TITLEDEMO(s0)->keyDelay;
        if (v != 0) {
            TITLEDEMO(s0)->keyDelay = v - 1; if (s0) {}
            v = TITLEDEMO(s0)->keyDelay;
        }
        if (v == 0) {
            vt2 = (char *)NODE(TITLEDEMO(s0)->keyMessage)->vptr;
            v1 = (char *)TITLEDEMO(s0)->keyMessage;
            ((void (*)(int))VT_SLOT(vt2, 10).fn)((VT_SLOT(vt2, 10).delta + (int)v1));
        }
        TITLEDEMO(s0)->timer = TITLEDEMO(s0)->timer - 1;
        if (TITLEDEMO(s0)->timer == 0) goto fire;
        if (DATAMGR(TITLEDEMO(s0)->dataMgr)->entries->finished != 0) {
fire:
            *(int *)((char *)&D_00000000 + 0x34) = 1;
            v = CALL(tit_nextDemo_000000C0)();
            XCALL(gl_func_00000000_116c_v, Process_fadeOut_0000F954)(s0, v, 0);
            return;
        }
        if (TITLEDEMO(s0)->keyDelay != 0) {
            return;
        }
        if (CALL(input_checkPressed_00011E00)((char *)&D_00000000, 0x40300) == 0) {
            return;
        }
        XCALL(gl_func_00000000_116c_v, Process_stop)(TITLEDEMO(s0)->mainLogo);
        XCALL(gl_func_00000000_116c_v, Process_stop)(TITLEDEMO(s0)->keyMessage);
        if (*(unsigned short *)(*(char **)((char *)&D_00000000 + 0x154) + 4) & 8) {
            vt3 = (char *)NODE(TITLEDEMO(s0)->langMenu)->vptr;
            v1 = (char *)TITLEDEMO(s0)->langMenu;
            ((void (*)(int))VT_SLOT(vt3, 10).fn)((VT_SLOT(vt3, 10).delta + (int)v1));
            TITLEDEMO(s0)->state = 2;
            XCALL(gl_func_00000000_116c_v, snd_postEvent)(144);
        } else {
            vt4 = (char *)NODE(TITLEDEMO(s0)->menu)->vptr;
            v1 = (char *)TITLEDEMO(s0)->menu;
            ((void (*)(int))VT_SLOT(vt4, 10).fn)((VT_SLOT(vt4, 10).delta + (int)v1));
            TITLEDEMO(s0)->state = 3;
            TITLEDEMO(s0)->timer = TITLEDEMO(s0)->unk74 * 30;
            XCALL(gl_func_00000000_116c_v, snd_postEvent)(144);
        }
        break;
    }
    case 2: {
        XCALL(gl_func_00000000_116c_v, tit_langMenuInput_00001BB8)(TITLEDEMO(s0)->langMenu);
        if (CALL(input_checkPressed_00011E00)((char *)&D_00000000, 0x40100) != 0) {
            XCALL(gl_func_00000000_116c_v, snd_postEvent)(5);
            if (1) {
                char *z = *(char **)((char *)&D_00000000 + 0x154) + 4;
                *(short *)z = (short)(*(unsigned short *)z & 0xFFF7);
            }
            XCALL(gl_func_00000000_116c_v, settings_save_00012150)((char *)&D_00000000);
            XCALL(gl_func_00000000_116c_v, Process_stop)(TITLEDEMO(s0)->langMenu);
            vt5 = (char *)NODE(TITLEDEMO(s0)->menu)->vptr;
            v1 = (char *)TITLEDEMO(s0)->menu;
            ((void (*)(int))VT_SLOT(vt5, 10).fn)((VT_SLOT(vt5, 10).delta + (int)v1));
            TITLEDEMO(s0)->state = 3;
            TITLEDEMO(s0)->timer = TITLEDEMO(s0)->unk74 * 30;
        }
        break;
    }
    case 3: {
        int *c;
        if (TITLEDEMO(s0)->timer == 0) {
            *(int *)((char *)&D_00000000 + 0x34) = 1;
            v = CALL(tit_nextDemo_000000C0)();
            XCALL(gl_func_00000000_116c_v, Process_fadeOut_0000F954)(s0, v, 0);
        }
        if (DATAMGR(TITLEDEMO(s0)->dataMgr)->entries->finished != 0) {
            v1 = (char *)TITLEDEMO(s0)->whiteFader;
            c = &WFADER(v1)->alpha;
            if (WFADER(v1)->alpha < 255) {
                *c = *c + 16;
                if (1) {}
                v1 = (char *)TITLEDEMO(s0)->whiteFader;
                if (WFADER(v1)->alpha >= 256) {
                    WFADER(v1)->alpha = 720;
                }
            } else {
                v1 = (char *)&D_00000000;
                vt6 = (char *)NODE(v1)->vptr;
                ((void (*)(int))VT_SLOT(vt6, 11).fn)((VT_SLOT(vt6, 11).delta + (int)v1));
            }
        } else {
            int av;
            v1 = (char *)TITLEDEMO(s0)->whiteFader;
            if (1) {}
            if (1) {}
            av = WFADER(v1)->alpha;
            if (1) {}
            if (1) {}
            if (1) {}
            c = &WFADER(v1)->alpha;
            if (av > 0) {
                *c = *c - 16;
                v1 = (char *)TITLEDEMO(s0)->whiteFader;
                if (1) {}
                if (1) {}
                av = WFADER(v1)->alpha;
                if (1) {}
                if (1) {}
                if (av < 0) {
                    if (1) {}
                    WFADER(v1)->alpha = 0;
                    av = WFADER(TITLEDEMO(s0)->whiteFader)->alpha;
                }
            }
            if (av == 0) {
                if (CALL(input_checkPressed_00011E00)((char *)&D_00000000, 512) != 0) {
                    vt7 = (char *)NODE(TITLEDEMO(s0)->mainLogo)->vptr;
                    v1 = (char *)TITLEDEMO(s0)->mainLogo;
                    ((void (*)(int))VT_SLOT(vt7, 10).fn)((VT_SLOT(vt7, 10).delta + (int)v1));
                    XCALL(gl_func_00000000_116c_v, MainLogo_reset)(TITLEDEMO(s0)->mainLogo);
                    vt8 = (char *)NODE(TITLEDEMO(s0)->keyMessage)->vptr;
                    v1 = (char *)TITLEDEMO(s0)->keyMessage;
                    ((void (*)(int))VT_SLOT(vt8, 10).fn)((VT_SLOT(vt8, 10).delta + (int)v1));
                    XCALL(gl_func_00000000_116c_v, Process_stop)(TITLEDEMO(s0)->menu);
                    TITLEDEMO(s0)->state = 1;
                    TITLEDEMO(s0)->timer = TITLEDEMO(s0)->unk70 * 30;
                    XCALL(gl_func_00000000_116c_v, snd_postEvent)(2050);
                }
            }
            if (CALL(input_checkPressed_00011E00)((char *)&D_00000000, 0x40100) != 0) {
                XCALL(gl_func_00000000_116c_v, Node_clearFlag8_0000F564)(TITLEDEMO(s0)->menu);
                XCALL(gl_func_00000000_116c_v, snd_postEvent)(5);
                v1 = (char *)TITLEDEMO(s0)->menu;
                if (1) {}
                if (1) {}
                if (GUIMENU(v1)->rows[GUIMENU(v1)->cursor].arg != 0) {
                    ((GuiSpriteTable *)&import_00263D30)->menuOwner = s0;
                    INPUT_MGR(import_00020098).unk84 = GUIMENU(TITLEDEMO(s0)->menu)->cursor;
                    v1 = (char *)TITLEDEMO(s0)->menu;
                    if (1) {}
                    GUIMENU(v1)->rows[GUIMENU(v1)->cursor].arg();
                }
            }
        }
        break;
    }
    }
}

}
