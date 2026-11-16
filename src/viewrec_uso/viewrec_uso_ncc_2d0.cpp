/* IDO 7.1 NCC (C++) EXACT donor for viewrec.uso 0x2D0, the records process ctor (agent-a run 648),
 * REPLACE_FUNC_BODY-spliced over the INCLUDE_ASM in viewrec_uso.c (Makefile VIEWREC_NCC_DONOR_2D0).
 * Its own file because its self-contained Node / Process / Screen types differ from the ones in
 * viewrec_uso_ncc.cpp. cfront alloc chain 0xE0 / 0x50 / 0x2C, the six screens created and attached,
 * colours reset, the start screen by proc->mode, the shared Fader attached and started.
 * It must be edg: the mode switch keeps the source case order (cfe sorts the cases). Levers:
 * screens attached through an inline MEMBER addChild (a free inline or cfe-style macro colours the
 * child into s2 or spills the last one), the Fader through a child declared in its own block around
 * the switch (frame homes, see below), a separate 0-pinned GameFlow symbol for Fader_start, session
 * read through obj. The .bss reloc of the static colours is renamed viewrec_uso_func_000002D0_bss by
 * the splice (pinned to 0x858, sVrPrimColour). */
#include "../../include/uso_call.h"

extern "C" int gl_func_00000000(...);
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
    void addChild(VrNode *c) { CALL(list_append)((char *)this + 16, c); if (c->parent != 0) c->unk04 = 1; c->parent = this; }
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
#define ADDCHILD(o, x) (o)->addChild(x)

static inline char *nodeCtor(char *o3) {
    if (o3 != 0 || (o3 = (char *)CALL(mem_new)(0x2C)) != 0) {
        CALL(Node_ctor)(o3, &D_00000000 + 0x3B4);
        *(char **)(o3 + 0x28) = &vr_vt_node;
    }
    return o3;
}
static inline char *procCtor(char *o2) {
    if (o2 != 0 || (o2 = (char *)CALL(mem_new)(0x50)) != 0) {
        nodeCtor(o2);
        *(char **)(o2 + 0x28) = &vr_vt_proc;
    }
    return o2;
}
extern "C" VrProc *viewrec_uso_func_000002D0(VrProc *obj, int flags, int mode, int *session) {
    if (obj != 0 || (obj = (VrProc *)CALL(mem_new)(0xE0)) != 0) {
        procCtor((char *)obj);
        *(char **)((char *)obj + 0x28) = &vr_vt_viewrec;
        obj->name = &D_00000000 + 0x3BC;
        obj->session = session;
        obj->flags = flags;
        obj->mode = mode;
        obj->unk78 = (flags >> 4) & 0xFF;
        CALL(viewrec_uso_func_00000718)(obj);
        CALL(viewrec_uso_func_000008C8)(obj);
        obj->font = CALL(gui_fontCreate)(0, &vr_fontData, 72, 221, 3, 13);
        CALL(gui_fontAdjustAdvance)(obj->font, -3);
        obj->menuDepth = 0;
        ADDCHILD(obj, obj->mainMenu = (VrScreen *)CALL(viewrec_uso_func_00001748)(0, obj));
        ADDCHILD(obj, obj->screen60 = (VrScreen *)CALL(viewrec_uso_func_00001B4C)(0, obj));
        ADDCHILD(obj, obj->screen68 = (VrScreen *)CALL(viewrec_uso_func_000036B4)(0, obj));
        ADDCHILD(obj, obj->screen64 = (VrScreen *)CALL(viewrec_uso_func_00004518)(0, obj));
        ADDCHILD(obj, obj->screen58 = (VrScreen *)CALL(viewrec_uso_func_00004854)(0, obj));
        ADDCHILD(obj, obj->screen5C = (VrScreen *)CALL(viewrec_uso_func_00005200)(0, obj));

        sPrimColour[3] = 1.0f;
        sPrimColour[2] = 1.0f;
        sPrimColour[1] = 1.0f;
        sPrimColour[0] = 1.0f;
        sEnvColour[0] = 1.0f;
        sEnvColour[2] = 1.0f;
        sEnvColour[3] = 1.0f;
        sEnvColour[1] = 0.0f;

        {
        /* the Fader child is declared in a block of its own around the mode switch: edg numbers a
         * block's named locals, then its inline temps, then the nested blocks, so this puts it
         * between the ctor-chain temps and the loop counter (counter home sp+0x2C, as the target) */
        VrNode *c;
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
                    if (CALL(Backup_arcadeEntryIsNew)(obj->session[1] + i * 32)) {
                        obj->screen60->sel = i + 1;
                        obj->screen60->row = i;
                    }
                }
                sVrScreen = obj->screen60;
                break;
            }
        }
        obj->screen60->flags98 = obj->flags & 0xF;
        { c = gInputMgr_vrncc.fader; CALL(list_append)((char *)obj + 16, c); if (c->parent != 0) c->unk04 = 1; c->parent = obj; }
        }
        CALL(Fader_start)(gInputMgr_vr2d0.fader, 1, 0);
        obj->unk30 = 1;
        obj->started = 0;
        obj->unk48 = 0;
    }
    return obj;
}
