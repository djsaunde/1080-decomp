/* IDO 7.1 NCC (C++) EXACT donors for viewrec.uso (agent-a run 630), REPLACE_FUNC_BODY-spliced over
 * the empty placeholders in viewrec_uso.c on both build paths (Makefile VIEWREC_NCC_DONOR).
 * viewrec.uso is C++ (Process / Node subclasses with cfront vtables, vptr at +0x28); the functions
 * here are the ones whose virtual calls only edg's lowering reproduces (the object in v1, the vptr
 * in v0). */
#include "../../include/uso_call.h"

extern "C" int gl_func_00000000(...);

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
    /* 0x70 */ int pad70;
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

