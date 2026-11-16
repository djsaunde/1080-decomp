#include "common.h"
#include "uso_call.h"
#include "gdl/node.h"
#include "game/input.h"
#include "game/guimenu.h"
#include "gdl/race.h"

extern int gl_func_00000000();
extern char D_00000000;

/* viewrec.uso ("GENNewRecords Process"): the records / options screens. */

/* the records process (the ctor at 0x2D0 allocates 0xE0 bytes) */
typedef struct ViewRecProc {
    /* 0x00 */ Node node;
    /* 0x2C */ s32 started;          /* nonzero once the fade-in has run */
    /* 0x30 */ s32 unk30;
    /* 0x34 */ char pad34[0x48 - 0x34];
    /* 0x48 */ s32 unk48;
    /* 0x4C */ char pad4C[0x50 - 0x4C];
    /* 0x50 */ s32 *session;         /* ctor arg 4: Backup_sessionClearNew arg */
    /* 0x54 */ Node *mainMenu;       /* 0x1748 */
    /* 0x58 */ Node *screen58;       /* 0x4854 */
    /* 0x5C */ Node *screen5C;       /* 0x5200 */
    /* 0x60 */ Node *screen60;       /* 0x1B4C */
    /* 0x64 */ Node *screen64;       /* 0x4518 */
    /* 0x68 */ Node *screen68;       /* 0x36B4 */
    /* 0x6C */ s32 mode;             /* which screen is up: 0x1000 / 0x2000 / 0x4000 / 0x8000 / 0x10000 ... */
    /* 0x70 */ char pad70[0x74 - 0x70];
    /* 0x74 */ s32 flags;            /* ctor arg 1 */
    /* 0x78 */ s32 unk78;            /* (flags >> 4) & 0xFF */
    /* 0x7C */ s32 font;
    /* 0x80 */ char pad80[0x88 - 0x80];
    /* 0x88 */ GuiMenu *menu88;
    /* 0x8C */ char pad8C[0x9C - 0x8C];
    /* 0x9C */ GuiMenu *menuStack[16];
    /* 0xDC */ s32 menuDepth;
} ViewRecProc;

/* The module's Bss statics (relocated against viewrec Bss(base); each symbol is pinned at its
 * Bss offset in undefined_syms_auto.txt, so the linked words carry the offset as in the ROM). */
extern RaceSprite sVrSprites[21];   /* 0x000: gui_spriteLoad (0x718) */
extern RaceSprite sVrLabels[68];    /* 0x1F8: gui_spriteInit (0x8C8) */
extern f32 sVrPrimColour[4];        /* 0x858 */
extern f32 sVrEnvColour[4];         /* 0x868 */
extern GuiMenu *sVrMenu;            /* 0x878: the menu that has the pad */
extern ViewRecProc *sVrProc;        /* 0x87C */
extern Node *sVrScreen;             /* 0x880: the screen being shown */

#define gInputMgr_vr (*(InputMgr *)&D_00000000)

/* Cfront virtual call through slot n of a Node's vtable */
#define VCALL(obj, n) ((void (*)(int))VT_SLOT((obj)->vptr, n).fn)(VT_SLOT((obj)->vptr, n).delta + (int)(obj))

/* 0x0 .. 0x214: switch the shown screen (Process_stop the old one, start the new one through
 * vtable slot 10, record the mode). NM: the target copies the new screen into v1 and loads the
 * vptr into v0 (`or v1,v0,zero; lw v0,40(v0)`), one word longer; cfe and NCC (plain virtual
 * call, inline setter) both load the vptr into v1. */
#ifdef NON_MATCHING
void viewrec_uso_func_00000000(void) {
    gl_func_00000000(/* Process_stop */ sVrScreen);
    sVrScreen = sVrProc->mainMenu;
    VCALL(sVrScreen, 10);
    gl_func_00000000(/* viewrec_uso_func_00001A08 */ sVrProc->mainMenu);
    sVrProc->mode = 0x10000;
    sVrMenu = sVrProc->menu88;
}
#else
INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00000000);
#endif

#ifdef NON_MATCHING
void viewrec_uso_func_00000084(void) {
    gl_func_00000000(/* Process_stop */ sVrScreen);
    sVrScreen = sVrProc->screen60;
    VCALL(sVrScreen, 10);
    sVrProc->mode = 0x1000;
}
#else
INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00000084);
#endif

#ifdef NON_MATCHING
void viewrec_uso_func_000000E8(void) {
    gl_func_00000000(/* Process_stop */ sVrScreen);
    sVrScreen = sVrProc->screen58;
    VCALL(sVrScreen, 10);
    sVrProc->mode = 0x40000;
}
#else
INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_000000E8);
#endif

#ifdef NON_MATCHING
void viewrec_uso_func_0000014C(void) {
    gl_func_00000000(/* Process_stop */ sVrScreen);
    sVrScreen = sVrProc->screen5C;
    VCALL(sVrScreen, 10);
    sVrProc->mode = 0x20000;
}
#else
INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_0000014C);
#endif

#ifdef NON_MATCHING
void viewrec_uso_func_000001B0(void) {
    gl_func_00000000(/* Process_stop */ sVrScreen);
    sVrScreen = sVrProc->screen68;
    VCALL(sVrScreen, 10);
    sVrProc->mode = 0x80000;
}
#else
INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_000001B0);
#endif

#ifdef NON_MATCHING
void viewrec_uso_func_00000214(void) {
    gl_func_00000000(/* Process_stop */ sVrScreen);
    sVrScreen = sVrProc->screen64;
    VCALL(sVrScreen, 10);
    sVrProc->mode = 0x4000000;
}
#else
INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00000214);
#endif

void viewrec_uso_func_00000278(void) {
}

void viewrec_uso_func_00000280(void) {
}

void viewrec_uso_func_00000288(void) {
}

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00000290);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_000002D0);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00000718);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_000008C8);

/* push the current menu on the process's menu stack and start `menu` */
void viewrec_uso_func_00000F84(ViewRecProc *proc, GuiMenu *menu) {
    CALL(Process_stop)(sVrMenu);
    proc->menuStack[proc->menuDepth] = sVrMenu;
    proc->menuDepth++;
    sVrMenu = menu;
    VCALL(&sVrMenu->node, 10);
}

/* pop the menu stack: stop the current menu and restart the previous one */
void viewrec_uso_func_00001000(ViewRecProc *proc) {
    CALL(Process_stop)(sVrMenu);
    proc->menuDepth--;
    sVrMenu = proc->menuStack[proc->menuDepth];
    VCALL(&sVrMenu->node, 10);
}

/* A / Start on the current menu row: run the row callback. NM: the target reloads sVrMenu into
 * v0 for the call (here v1). */
#ifdef NON_MATCHING
void viewrec_uso_func_00001068(ViewRecProc *proc) {
    if (gl_func_00000000(/* input_checkPressed_00011E00 */ &gInputMgr_vr, 0x40100)) {
        GuiMenu *m = sVrMenu;
        if (m->rows[m->cursor].arg != 0 && m->rows[m->cursor].enabled != 0) {
            gl_func_00000000(/* snd_postEvent */ 5);
            sVrProc = proc;
            sVrMenu->rows[sVrMenu->cursor].arg();
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00001068);
#endif

void viewrec_uso_func_0000110C(ViewRecProc *proc) {
    if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x200)) {
        CALL(snd_postEvent)(0x802);
        if (proc->menuDepth == 0) {
            if (proc->flags & 0x10000) {
                CALL(viewrec_uso_func_00000000)();
            } else {
                gInputMgr_vr.procState = gInputMgr_vr.nextProcState;
                CALL(Backup_sessionClearNew)(proc->session);
                CALL(Process_fadeOut_0000F954)(proc, -1, 0);
            }
        } else {
            CALL(viewrec_uso_func_00001000)(proc);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_000011B4);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_000012D4);

/* draw label sprite i centred at y. NM: the target frame is 0x28 (sprite pointer at sp+0x1C). */
#ifdef NON_MATCHING
void viewrec_uso_func_00001488(ViewRecProc *proc, int i, int y) {
    RaceSprite *spr = &sVrLabels[i];
    gl_func_00000000(/* gui_spriteRefresh */ spr);
    gl_func_00000000(/* gui_drawSprite */ spr, 160, y, 3);
}
#else
INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00001488);
#endif

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_000014DC);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00001588);

/* EXACT via the NCC (C++) donor src/viewrec_uso/viewrec_uso_ncc.cpp (REPLACE_FUNC_BODY). */
void viewrec_uso_func_00001664(ViewRecProc *self) {
}

void viewrec_uso_func_000016E8(ViewRecProc *self, int *msg) {
    int type = *msg;
    if (type == 21) {
        CALL(dbg_printf)((char *)&D_00000000 + 1000);
        if (gInputMgr_vr.unk88 == 0) {
            CALL(viewrec_uso_func_000008C8)(self);
        }
    }
    CALL(Node_handleMsg)(self, msg);
}

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00001748);

typedef struct ViewRecScreen {
    /* 0x00 */ Node node;
    /* 0x2C */ char pad2C[0x50 - 0x2C];
    /* 0x50 */ ViewRecProc *proc;
} ViewRecScreen;

/* NM: the target holds &gInputMgr in v0 for the procState copy (here v1). */
#ifdef NON_MATCHING
void viewrec_uso_func_000018D4(ViewRecScreen *self) {
    gl_func_00000000(/* viewrec_uso_func_00001068 */ self->proc);
    if (gl_func_00000000(/* input_checkPressed_00011E00 */ &gInputMgr_vr, 0x200)) {
        gl_func_00000000(/* snd_postEvent */ 0x802);
        gInputMgr_vr.procState = gInputMgr_vr.nextProcState;
        gl_func_00000000(/* Backup_sessionClearNew */ self->proc->session);
        gl_func_00000000(/* Process_fadeOut_0000F954 */ self->proc, -1, 0);
    }
    gl_func_00000000(/* Node_update */ self);
}
#else
INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_000018D4);
#endif

void viewrec_uso_func_00001954(ViewRecScreen *self) {
    CALL(viewrec_uso_func_000011B4)(self->proc);
    CALL(viewrec_uso_func_000014DC)(self->proc);
    CALL(gui_setEnvPrimColor_00001EF4)(&D_00000000, 255, sVrPrimColour, sVrEnvColour);
    CALL(gui_spriteRefresh)(&sVrLabels[38]);
    CALL(gui_drawSprite)(&sVrLabels[38], 160, 37, 3);
}

void viewrec_uso_func_000019C8(ViewRecScreen *self, int *msg) {
    int type = *msg;
    if (type == 21) {
        CALL(viewrec_uso_func_00001A08)(self);
    }
    CALL(Node_handleMsg)(self, msg);
}

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00001A08);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00001B4C);

void viewrec_uso_func_00001CCC(ViewRecScreen *self) {
    switch (self->proc->mode) {
        case 0x1000:
            CALL(viewrec_uso_func_0000266C)(self);
            break;
        case 0x2000:
            CALL(viewrec_uso_func_000022A8)(self);
            break;
        case 0x4000:
            CALL(viewrec_uso_func_000022A8)(self);
            break;
        case 0x8000:
            CALL(viewrec_uso_func_00002468)(self);
            break;
    }
    CALL(Node_update)(self);
}

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00001D64);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00002088);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00002190);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_000022A8);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00002468);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_0000266C);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_000029B0);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00002AE4);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00002C08);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00002D9C);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00002F1C);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00002FC0);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_0000319C);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00003364);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_000036B4);

/* the erase screen's update: dispatch on the process mode */
void viewrec_uso_func_00003818(ViewRecScreen *self) {
    switch (self->proc->mode) {
        case 0x80000:
            CALL(viewrec_uso_func_00003F24)(self);
            break;
        case 0x2000000:
            CALL(viewrec_uso_func_00004354)(self);
            break;
    }
    CALL(Node_update)(self);
}

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00003878);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00003F24);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00004354);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00004518);

void viewrec_uso_func_00004670(ViewRecScreen *self) {
    CALL(viewrec_uso_func_000047B0)(self);
    CALL(Node_update)(self);
}

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00004698);

/* the replay screen's pad handling: A / Start fades out to the saved ghost's mode
 * (procState = Backup_ghostGetMode + 6, then state 1); B goes back to the main menu */
void viewrec_uso_func_000047B0(ViewRecScreen *self) {
    if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x40100)) {
        CALL(snd_postEvent)(5);
        gInputMgr_vr.procState = CALL(Backup_ghostGetMode)((char *)gInputMgr_vr.backup + 4) + 6;
        gInputMgr_vr.nextProcState = 1;
        CALL(Process_fadeOut_0000F954)(self->proc, -1, 0);
    }
    if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x200)) {
        CALL(snd_postEvent)(0x802);
        CALL(viewrec_uso_func_00000000)();
    }
}

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00004854);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00004990);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00004DEC);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00005200);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00005318);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00005440);

void viewrec_uso_func_000055A4(int a0) {
}

