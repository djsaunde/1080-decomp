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
};

/* GameFlow (include/game/input.h InputMgr): only the shared Fader */
struct VrGameFlow { char pad[0x190]; void *fader; };
extern VrGameFlow gInputMgr_vrncc;      /* bootup Bss 0xF8 import */
extern VrNode *sVrScreen;               /* viewrec Bss 0x880 */

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
