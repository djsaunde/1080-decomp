/* IDO 7.1 NCC (C++) EXACT donors for viewrec.uso (agent-a run 630), REPLACE_FUNC_BODY-spliced over
 * the empty placeholders in viewrec_uso.c on both build paths (Makefile VIEWREC_NCC_DONOR).
 * viewrec.uso is C++ (Process / Node subclasses with cfront vtables, vptr at +0x28); the functions
 * here are the ones whose virtual calls only edg's lowering reproduces (the object in v1, the vptr
 * in v0). */
#include "../../include/uso_call.h"

extern "C" int gl_func_00000000(...);
/* void-returning alias of the placeholder (0-pinned): no dead v0 def to colour around */
extern "C" void gl_func_00000000_vrv(...);

/* gdl Node with its cfront vtable: the slots this module calls */
struct VrNode {
    char node[0x28];
    virtual void slot0(); virtual void slot1(); virtual void slot2();
    virtual void display();         /* slot 3 */
    virtual void slot4(); virtual void slot5(); virtual void slot6(); virtual void slot7();
    virtual void slot8(); virtual void slot9();
    virtual void start();           /* slot 10 */
    virtual void onFadedIn();       /* slot 11 */
};

/* the records process (ViewRecProc in viewrec_uso.c) */
struct VrProc : VrNode {
    /* 0x2C */ int started;
    /* 0x30 */ char pad30[0x50 - 0x30];
    /* 0x50 */ int *session;
    /* 0x54 */ char pad54[0x6C - 0x54];
    /* 0x6C */ int mode;
    /* 0x70 */ int nextMode;        /* the options mode the records menu picked (0x2000 / 0x4000) */
    /* 0x74 */ int flags;
};

/* a records / options screen (ViewRecScreen in viewrec_uso.c) */
struct VrScreen : VrNode {
    /* 0x2C */ char pad2C[0x50 - 0x2C];
    /* 0x50 */ VrProc *proc;
    /* 0x54 */ char pad54[0x98 - 0x54];
    /* 0x98 */ int cursor;
    /* 0x9C */ int minCursor;
};

/* GameFlow (include/game/input.h InputMgr): only the shared Fader */
struct VrGameFlow {
    char pad[0x40];
    int procState;              /* 0x40 */
    int nextProcState;          /* 0x44 */
    char pad48[0x190 - 0x48];
    void *fader;                /* 0x190 */
};
extern VrGameFlow gInputMgr_vrncc;      /* bootup Bss 0xF8 import */
extern VrNode *sVrScreen;               /* viewrec Bss 0x880 */
extern VrNode *sVrMenu;                 /* viewrec Bss 0x878 */
extern VrProc *sVrProc;                 /* viewrec Bss 0x87C */
struct VrSprite { char data[0x18]; };   /* RaceSprite (gdl/race.h) */
extern VrSprite sVrLabels[68];          /* viewrec Bss 0x1F8 */

/* the records menu screen (VrScreen60 in viewrec_uso.c, ctor 0x1B4C) */
struct VrScreen60 : VrNode {
    /* 0x2C */ char pad2C[0x50 - 0x2C];
    /* 0x50 */ VrProc *proc;
    /* 0x54 */ char pad54[0x6C - 0x54];
    /* 0x6C */ int frame;
    /* 0x70 */ char pad70[0x90 - 0x70];
    /* 0x90 */ int state;           /* 0: picking a tab, 1: in the options grid */
    /* 0x94 */ int tab;             /* 0..2 */
    /* 0x98 */ int cursor;          /* the grid cell (0..7, two per row) */
    /* 0x9C */ int minCursor;       /* the lowest selectable cell (2 on tab 0) */
};

/* the process update: until the fade-in has started, let the shown screen draw (slot 3); once it
 * has, wait for the shared Fader and then call onFadedIn (slot 11) */
extern "C" void viewrec_uso_func_00001664(VrProc *self) {
    if (self->started == 0) {
        sVrScreen->display();
    }
    if (self->started != 0 && CALL(Fader_checkDone_00013924)(gInputMgr_vrncc.fader)) {
        self->onFadedIn();
    }
}

/* The prim / env colours (viewrec Bss 0x858 / 0x868; sVrPrimColour / sVrEnvColour in viewrec_uso.c).
 * They are file-local statics in the original: as1 shares one `lui at` across accesses only for the
 * same static symbol (IDO_CODEGEN kit LX), and each access through its own block-scope reference
 * keeps uopt from CSEing the base, which gives the target's one `lui at` per pair of stores. The
 * splice renames this donor's .bss reloc to viewrec_uso_func_00000290_bss (pinned to 0x858). */
static float sPrimColour[4];
static float sEnvColour[4];

/* 0x290: reset both colours to white (env green 0) */
extern "C" void viewrec_uso_func_00000290() {
    { float &r = sPrimColour[0]; r = 1.0f; }
    { float &r = sPrimColour[1]; r = 1.0f; }
    { float &r = sPrimColour[2]; r = 1.0f; }
    { float &r = sPrimColour[3]; r = 1.0f; }
    { float &r = sEnvColour[0]; r = 1.0f; }
    { float &r = sEnvColour[1]; r = 0; }
    { float &r = sEnvColour[2]; r = 1.0f; }
    { float &r = sEnvColour[3]; r = 1.0f; }
}

/* 0x22A8: the options screen input. The stick steers player 3's frequency scale; left / right move
 * the cursor to the previous / next enabled option (0x2088 / 0x2190); A leaves (fade out) unless the
 * screen was entered from the main menu, where A and B go back to it. Only edg's `sVrMenu->start()`
 * gives the menu / vptr registers of the final virtual call (cfe swaps them). */
extern "C" void viewrec_uso_func_000022A8(VrScreen *self) {
    char *pad;
    int back;
    pad = (char *)CALL(input_getPad_00011D40)(&gInputMgr_vrncc, 1);
    CALL(snd_setPlayer3FreqScaleFromStick)(*(signed char *)(pad + 8), *(signed char *)(pad + 9));
    if (CALL(input_checkPressed_00011E00)(&gInputMgr_vrncc, 0x10001)) {
        if (CALL(viewrec_uso_func_00002088)(self) != self->cursor) {
            CALL(snd_postEvent)(1);
            self->cursor = CALL(viewrec_uso_func_00002088)(self);
        }
    }
    if (CALL(input_checkPressed_00011E00)(&gInputMgr_vrncc, 0x4002)) {
        if (CALL(viewrec_uso_func_00002190)(self) != self->cursor) {
            CALL(snd_postEvent)(1);
            self->cursor = CALL(viewrec_uso_func_00002190)(self);
        }
    }
    back = 0;
    if (CALL(input_checkPressed_00011E00)(&gInputMgr_vrncc, 0x40100)) {
        if (!(self->proc->flags & 0x1000)) {
            gInputMgr_vrncc.procState = gInputMgr_vrncc.nextProcState;
            CALL(Process_fadeOut_0000F954)(self->proc, -1, 0);
            CALL(Backup_sessionClearNew)(self->proc->session);
        } else {
            back = 1;
        }
        CALL(snd_postEvent)(5);
    }
    if (back || CALL(input_checkPressed_00011E00)(&gInputMgr_vrncc, 0x200)) {
        CALL(snd_postEvent)(0x802);
        if (self->proc->flags & 0x1000) {
            if (self->proc->mode == 0x8000) {
                CALL(viewrec_uso_func_00001000)(self->proc);
            }
            self->proc->mode = 0x1000;
            sVrMenu->start();
        }
    }
}


/* 0x266C: the records menu input (proc mode 0x1000). State 0: left / right pick one of the three
 * tabs, A enters the options grid for tabs 0 / 1 (the options mode they lead to is kept in
 * proc->nextMode) or opens the arcade records (tab 2), B returns to the main menu (0x0) when the
 * process was entered from it. State 1: the d-pad moves the cursor around the 2-wide grid, A shows
 * the picked options screen, B goes back to the tabs. edg: `old` declared at function scope (a
 * block-scope one takes a frame home 8 bytes higher), the state read through a local (v0, not
 * v1), the void alias on the snd_postEvent(5) before each proc / tab read. */
extern "C" void viewrec_uso_func_0000266C(VrScreen60 *self) {
    int old;
    int st = self->state;
    switch (st) {
    case 0:
        if (CALL(input_checkPressed_00011E00)(&gInputMgr_vrncc, 0x10001)) {
            CALL(snd_postEvent)(1);
            self->tab--;
            if (self->tab < 0) self->tab = 2;
        }
        if (CALL(input_checkPressed_00011E00)(&gInputMgr_vrncc, 0x4002)) {
            CALL(snd_postEvent)(1);
            self->tab++;
            if (self->tab >= 3) self->tab = 0;
        }
        if (self->tab == 0) self->minCursor = 2; else self->minCursor = 0;
        if (CALL(input_checkPressed_00011E00)(&gInputMgr_vrncc, 0x100)) {
            XCALL(gl_func_00000000_vrv, snd_postEvent)(5);
            switch (self->tab) {
            case 0:
                self->proc->nextMode = 0x2000;
                if (self->cursor < self->minCursor) self->cursor = self->minCursor;
                self->state++;
                break;
            case 1:
                self->proc->nextMode = 0x4000;
                self->state++;
                break;
            case 2:
                sVrProc->mode = 0x8000;
                break;
            }
        }
        if (CALL(input_checkPressed_00011E00)(&gInputMgr_vrncc, 0x200)) {
            CALL(snd_postEvent)(0x802);
            if (self->proc->flags & 0x10000) {
                CALL(viewrec_uso_func_00000000)();
            }
        }
        break;
    case 1:
        old = self->cursor;
        if (self->cursor > self->minCursor && CALL(input_checkPressed_00011E00)(&gInputMgr_vrncc, 0x10001) &&
            (self->cursor & 1)) {
            CALL(snd_postEvent)(1);
            self->cursor--;
        }
        if (self->cursor < 8 && CALL(input_checkPressed_00011E00)(&gInputMgr_vrncc, 0x4002) && !(self->cursor & 1)) {
            CALL(snd_postEvent)(1);
            self->cursor++;
        }
        if (self->cursor - 2 >= self->minCursor && CALL(input_checkPressed_00011E00)(&gInputMgr_vrncc, 0x2004)) {
            CALL(snd_postEvent)(1);
            self->cursor -= 2;
        }
        if (self->cursor + 2 < 8 && CALL(input_checkPressed_00011E00)(&gInputMgr_vrncc, 0x8008)) {
            CALL(snd_postEvent)(1);
            self->cursor += 2;
        }
        if (old != self->cursor) self->frame = 0;
        if (CALL(input_checkPressed_00011E00)(&gInputMgr_vrncc, 0x100)) {
            XCALL(gl_func_00000000_vrv, snd_postEvent)(5);
            self->proc->mode = self->proc->nextMode;
        }
        if (CALL(input_checkPressed_00011E00)(&gInputMgr_vrncc, 0x200)) {
            CALL(snd_postEvent)(0x802);
            self->state--;
        }
        break;
    }
}

/* 0x1488: draw label sprite i centred at y. edg: the sprite bound through a reference and then
 * addressed gives the target's frame (0x28, the pointer spilled at sp+0x1C across the first call);
 * a plain pointer local is 7 words off. */
extern "C" void viewrec_uso_func_00001488(VrProc *proc, int i, int y) {
    VrSprite &label = sVrLabels[i];
    VrSprite *spr = &label;
    CALL(gui_spriteRefresh)(spr);
    CALL(gui_drawSprite)(spr, 160, y, 3);
}
