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

/* 0x2D0, the records process ctor (cfront chain 0xE0 / 0x50 / 0x2C, the six screens created and
 * attached, colours reset, the start screen by proc->mode, the shared Fader attached and started).
 * NEAR (4/274 words, kept out of the build; the matching build is INCLUDE_ASM in viewrec_uso.c): only
 * the loop counter's spill slot differs (sp+0x30, target sp+0x2C). It must be edg: the mode switch
 * keeps source case order (cfe sorts the cases). Levers found: screens attached through an inline
 * member addChild (cfe-style macros colour the child into s2 or spill the last one), the Fader
 * through a block-scope child, a separate 0-pinned GameFlow symbol for Fader_start, session read
 * through obj. Self-contained types; needs vr_vt_viewrec / vr_fontData / gInputMgr_vr2d0 pins. */
#ifdef VIEWREC_2D0_NEAR
struct VrNode;
struct VrNode {
    int unk00;
    int unk04;
    int unk08;
    const char *name;
    char pad10[0x14 - 0x10];
    VrNode *parent;
    char pad18[0x28 - 0x18];
    virtual void slot0(); virtual void slot1(); virtual void slot2();
    virtual void display();
    virtual void slot4(); virtual void slot5(); virtual void slot6(); virtual void slot7();
    virtual void slot8(); virtual void slot9();
    virtual void start();
    virtual void onFadedIn();
    void addChild(VrNode *c) { gl_func_00000000(/* list_append */ (char *)this + 16, c); if (c->parent != 0) c->unk04 = 1; c->parent = this; }
};
struct VrScreen : VrNode {
    char pad2C[0x58 - 0x2C];
    int sel;     /* 0x58 */
    int row;     /* 0x5C */
    char pad60[0x98 - 0x60];
    int flags98;
};
struct VrProc : VrNode {
    /* 0x2C */ int started;
    /* 0x30 */ int unk30;
    char pad34[0x48 - 0x34];
    /* 0x48 */ int unk48;
    char pad4C[0x50 - 0x4C];
    /* 0x50 */ int *session;
    /* 0x54 */ VrScreen *mainMenu;
    /* 0x58 */ VrScreen *screen58;
    /* 0x5C */ VrScreen *screen5C;
    /* 0x60 */ VrScreen *screen60;
    /* 0x64 */ VrScreen *screen64;
    /* 0x68 */ VrScreen *screen68;
    /* 0x6C */ int mode;
    /* 0x70 */ int pad70;
    /* 0x74 */ int flags;
    /* 0x78 */ int unk78;
    /* 0x7C */ int font;
    char pad80[0x88 - 0x80];
    /* 0x88 */ VrNode *menu88;
    char pad8C[0xDC - 0x8C];
    /* 0xDC */ int menuDepth;
};
struct VrGameFlow { char pad[0x190]; VrNode *fader; };
extern VrGameFlow gInputMgr_vrncc, gInputMgr_vr2d0;
extern VrNode *sVrScreen;
extern VrNode *sVrMenu;
extern char vr_vt_node, vr_vt_proc, vr_vt_viewrec, vr_fontData, D_00000000;
static float sPrimColour[4];
static float sEnvColour[4];
static inline void addChild(char *list, VrNode *self, VrNode *c) {
    gl_func_00000000(/* list_append */ list, c);
    if (c->parent != 0) c->unk04 = 1;
    c->parent = self;
}
#define ADDCHILD(o, x) (o)->addChild(x)

static inline char *nodeCtor(char *o3) {
    if (o3 != 0 || (o3 = (char *)gl_func_00000000(/* mem_new */ 0x2C)) != 0) {
        gl_func_00000000(/* Node_ctor */ o3, &D_00000000 + 0x3B4);
        *(char **)(o3 + 0x28) = &vr_vt_node;
    }
    return o3;
}
static inline char *procCtor(char *o2) {
    if (o2 != 0 || (o2 = (char *)gl_func_00000000(/* mem_new */ 0x50)) != 0) {
        nodeCtor(o2);
        *(char **)(o2 + 0x28) = &vr_vt_proc;
    }
    return o2;
}
extern "C" VrProc *viewrec_uso_func_000002D0(VrProc *obj, int flags, int mode, int *session) {

    if (obj != 0 || (obj = (VrProc *)gl_func_00000000(/* mem_new */ 0xE0)) != 0) {
        procCtor((char *)obj);
        *(char **)((char *)obj + 0x28) = &vr_vt_viewrec;
        obj->name = &D_00000000 + 0x3BC;
        obj->session = session;
        obj->flags = flags;
        obj->mode = mode;
        obj->unk78 = (flags >> 4) & 0xFF;
        gl_func_00000000(/* viewrec_uso_func_00000718 */ obj);
        gl_func_00000000(/* viewrec_uso_func_000008C8 */ obj);
        obj->font = gl_func_00000000(/* gui_fontCreate */ 0, &vr_fontData, 72, 221, 3, 13);
        gl_func_00000000(/* gui_fontAdjustAdvance */ obj->font, -3);
        obj->menuDepth = 0;
        ADDCHILD(obj, obj->mainMenu = (VrScreen *)gl_func_00000000(/* viewrec_uso_func_00001748 */ 0, obj));
        ADDCHILD(obj, obj->screen60 = (VrScreen *)gl_func_00000000(/* viewrec_uso_func_00001B4C */ 0, obj));
        ADDCHILD(obj, obj->screen68 = (VrScreen *)gl_func_00000000(/* viewrec_uso_func_000036B4 */ 0, obj));
        ADDCHILD(obj, obj->screen64 = (VrScreen *)gl_func_00000000(/* viewrec_uso_func_00004518 */ 0, obj));
        ADDCHILD(obj, obj->screen58 = (VrScreen *)gl_func_00000000(/* viewrec_uso_func_00004854 */ 0, obj));
        ADDCHILD(obj, obj->screen5C = (VrScreen *)gl_func_00000000(/* viewrec_uso_func_00005200 */ 0, obj));

        sPrimColour[3] = 1.0f;
        sPrimColour[2] = 1.0f;
        sPrimColour[1] = 1.0f;
        sPrimColour[0] = 1.0f;
        sEnvColour[0] = 1.0f;
        sEnvColour[2] = 1.0f;
        sEnvColour[3] = 1.0f;
        sEnvColour[1] = 0.0f;

        switch (obj->mode) {
            case 0x10000:
                obj->mainMenu->start();
                sVrScreen = obj->mainMenu;
                sVrMenu = obj->menu88;
                break;
            case 0x20000:
                sVrScreen = obj->screen5C;
                sVrScreen->start();
                obj->mode = 0x20000;
                break;
            case 0x2000:
            case 0x4000:
                obj->screen60->start();
                sVrScreen = obj->screen60;
                break;
            case 0x8000: {
                obj->screen60->start();
                int i;
                for (i = 0; i < 3; i++) {
                    if (gl_func_00000000(/* Backup_arcadeEntryIsNew */ obj->session[1] + i * 32)) {
                        obj->screen60->sel = i + 1;
                        obj->screen60->row = i;
                    }
                }
                sVrScreen = obj->screen60;
                break;
            }
        }
        obj->screen60->flags98 = obj->flags & 0xF;
        { VrNode *c = gInputMgr_vrncc.fader; gl_func_00000000(/* list_append */ (char *)obj + 16, c); if (c->parent != 0) c->unk04 = 1; c->parent = obj; }
        gl_func_00000000(/* Fader_start */ gInputMgr_vr2d0.fader, 1, 0);
        obj->unk30 = 1;
        obj->started = 0;
        obj->unk48 = 0;
    }
    return obj;
}
#endif /* VIEWREC_2D0_NEAR */
