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
/* 0x378 (sVrLabels[16..]): a separate static array in the original. 0x2F1C indexes it with a
 * variable and the target folds 0x378 into the reloc, which `&sVrLabels[i + 16]` cannot give. */
extern RaceSprite sVrLabels_378[52];
/* more indexed sprite runs (same tell: the variable index folds the start into the reloc) */
extern RaceSprite sVrSprites_048[8];  /* 0x048 (sVrSprites[3..]) */
extern RaceSprite sVrSprites_108[8];  /* 0x108 (sVrSprites[11..]) */
extern RaceSprite sVrLabels_7B0[7];   /* 0x7B0 (sVrLabels[61..]) */
extern f32 sVrPrimColour[4];        /* 0x858 */
extern f32 sVrEnvColour[4];         /* 0x868 */
extern GuiMenu *sVrMenu;            /* 0x878: the menu that has the pad */
extern ViewRecProc *sVrProc;        /* 0x87C */
extern Node *sVrScreen;             /* 0x880: the screen being shown */

#define gInputMgr_vr (*(InputMgr *)&D_00000000)
/* the same GameFlow import under its own symbol (pinned to 0): 0x4698 also passes the gui import
 * &D_00000000, and one shared symbol would let uopt CSE the two bases into one s-register */
extern InputMgr gInputMgr_vr4698;

/* Cfront virtual call through slot n of a Node's vtable */
#define VCALL(obj, n) ((void (*)(int))VT_SLOT((obj)->vptr, n).fn)(VT_SLOT((obj)->vptr, n).delta + (int)(obj))

/* 0x0 .. 0x214: switch the shown screen: Process_stop the old one, start the new one through
 * vtable slot 10, record the mode. The new screen is read through a local and the code-free
 * `if (sVrScreen) {}` tests around the start call give the target's copy of the screen into v1
 * with the vptr in v0 (`or v1,v0,zero; lw v0,40(v0)`) and the &sVrScreen base in a1. */
#define VR_SHOW_SCREEN(field)                                                                     \
    {                                                                                             \
        Node *s = sVrProc->field;                                                                 \
        sVrScreen = s;                                                                            \
        if (sVrScreen) {}                                                                         \
        VCALL(sVrScreen, 10);                                                                     \
        if (sVrScreen) {}                                                                         \
    }

/* NM: the show part matches; the tail differs. The target puts &sVrProc in v0 for the two loads
 * after the 0x1A08 call (`lui v0; addiu v0,v0,0x87C`) while keeping the load before the call as a
 * plain `lui t8; lw t8`; cfe uses a `lui` per load. */
#ifdef NON_MATCHING
void viewrec_uso_func_00000000(void) {
    gl_func_00000000(/* Process_stop */ sVrScreen);
    VR_SHOW_SCREEN(mainMenu);
    gl_func_00000000(/* viewrec_uso_func_00001A08 */ sVrProc->mainMenu);
    sVrProc->mode = 0x10000;
    sVrMenu = sVrProc->menu88;
}
#else
INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00000000);
#endif

void viewrec_uso_func_00000084(void) {
    CALL(Process_stop)(sVrScreen);
    VR_SHOW_SCREEN(screen60);
    sVrProc->mode = 0x1000;
}

void viewrec_uso_func_000000E8(void) {
    CALL(Process_stop)(sVrScreen);
    VR_SHOW_SCREEN(screen58);
    sVrProc->mode = 0x40000;
}

void viewrec_uso_func_0000014C(void) {
    CALL(Process_stop)(sVrScreen);
    VR_SHOW_SCREEN(screen5C);
    sVrProc->mode = 0x20000;
}

void viewrec_uso_func_000001B0(void) {
    CALL(Process_stop)(sVrScreen);
    VR_SHOW_SCREEN(screen68);
    sVrProc->mode = 0x80000;
}

void viewrec_uso_func_00000214(void) {
    CALL(Process_stop)(sVrScreen);
    VR_SHOW_SCREEN(screen64);
    sVrProc->mode = 0x4000000;
}

void viewrec_uso_func_00000278(void) {
}

void viewrec_uso_func_00000280(void) {
}

void viewrec_uso_func_00000288(void) {
}

/* EXACT via the NCC (C++) donor src/viewrec_uso/viewrec_uso_ncc.cpp (static colour objects). */
void viewrec_uso_func_00000290(void) {
}

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

/* with flag 0x10000 set, draw label sprites 57, 56, 55, 54 right to left from x 285 at y 220, each
 * spaced by its image width + 4 (+ 4 more after every odd one). The image and its width read into
 * locals give the target's v0 / v1. */
void viewrec_uso_func_000014DC(ViewRecProc *proc) {
    s32 i;
    s32 x;
    RaceSprite *spr;

    if (proc->flags & 0x10000) {
        x = 285;
        for (i = 0; i < 4; i++) {
            spr = &sVrLabels[57 - i];
            CALL(gui_spriteRefresh)(spr);
            CALL(gui_setCombine_00001794)(spr);
            CALL(gui_drawSprite)(spr, x, 220, 6);
            {
                RaceSpriteImage *img = spr->image;
                s32 w = img->width;
                x -= w + (i & 1) * 4 + 4;
            }
        }
    }
}

/* draw one record's row at y (0x4698 passes a saved ghost's course / rider / board / mode - 1):
 * the course title (label sprite via 0x1488), then the rider and board sprites at x 46 / 240 and
 * the mode label at 160, y + 30 */
void viewrec_uso_func_00001588(ViewRecProc *self, s32 y, s32 course, s32 rider, s32 board, s32 mode) {
    RaceSprite *spr;

    CALL(viewrec_uso_func_00001488)(self, course, y);
    spr = &sVrSprites_048[rider];
    CALL(gui_spriteRefresh)(spr);
    CALL(gui_drawSprite)(spr, 46, y, 2);
    spr = &sVrSprites_108[board];
    CALL(gui_spriteRefresh)(spr);
    CALL(gui_drawSprite)(spr, 240, y, 2);
    spr = &sVrLabels_7B0[mode];
    CALL(gui_spriteRefresh)(spr);
    CALL(gui_drawSprite)(spr, 160, y + 30, 3);
}

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
    /* 0x54 */ char pad54[0x58 - 0x54];
    /* 0x58 */ s32 frame;            /* replay screen: counted up per draw (0x4698); 0x2468: the
                                      * selected row + 1 (0 = none) */
    /* 0x5C */ s32 unk5C;            /* 0x2468: the highlighted row (0..2) */
    /* 0x60 */ char pad60[0x80 - 0x60];
    /* 0x80 */ s32 rowY;             /* replay screen: y of the ghost's row (0x1588) */
    /* 0x84 */ char pad84[0x98 - 0x84];
    /* 0x98 */ s32 cursor;           /* options screen: the selected option (0..7) */
    /* 0x9C */ s32 minCursor;        /* the lowest selectable option */
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

/* options screen: the previous enabled option before the cursor (wrapping 0 -> 7), at least
 * minCursor; proc->unk78 holds the enabled bits. */
s32 viewrec_uso_func_00002088(ViewRecScreen *self) {
    s32 c = self->cursor;
    s32 i;
    for (i = 0; i < 8; i++) {
        c--;
        if (c < 0) {
            c = 7;
        }
        if ((self->proc->unk78 & (1 << c)) && c >= self->minCursor) {
            return c;
        }
    }
    return c;
}

/* options screen: the next enabled option after the cursor (wrapping 7 -> 0) */
s32 viewrec_uso_func_00002190(ViewRecScreen *self) {
    s32 c = self->cursor;
    s32 i;
    for (i = 0; i < 8; i++) {
        c++;
        if (c >= 8) {
            c = 0;
        }
        if ((self->proc->unk78 & (1 << c)) && c >= self->minCursor) {
            return c;
        }
    }
    return c;
}

/* EXACT via the NCC (C++) donor src/viewrec_uso/viewrec_uso_ncc.cpp (sVrMenu->start()). */
void viewrec_uso_func_000022A8(ViewRecScreen *self) {
}

/* the arcade-records screen input: up / down move the highlighted row (0..2), A selects it,
 * B deselects or goes back, Start leaves (fade out, or back to the main menu when entered from it) */
void viewrec_uso_func_00002468(ViewRecScreen *self) {
    s32 back = 0;
    if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x8008)) {
        if (self->unk5C < 2) {
            CALL(snd_postEvent)(1);
            self->unk5C++;
        }
        if (self->frame) {
            self->frame = self->unk5C + 1;
        }
    }
    if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x2004)) {
        if (self->unk5C > 0) {
            CALL(snd_postEvent)(1);
            self->unk5C--;
        }
        if (self->frame) {
            self->frame = self->unk5C + 1;
        }
    }
    if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x100)) {
        if (self->frame == 0) {
            CALL(snd_postEvent)(5);
            self->frame = self->unk5C + 1;
        } else if (!(self->proc->flags & 0x1000)) {
            back = 1;
        }
        CALL(snd_postEvent)(5);
    }
    if ((self->proc->flags & 0x1000) && CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x200)) {
        if (self->frame) {
            CALL(snd_postEvent)(0x802);
            self->frame = 0;
        } else {
            back = 1;
        }
    }
    if (back || CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x40000)) {
        if (!(self->proc->flags & 0x1000)) {
            CALL(snd_postEvent)(5);
            gInputMgr_vr.procState = gInputMgr_vr.nextProcState;
            CALL(Process_fadeOut_0000F954)(self->proc, -1, 0);
            CALL(Backup_sessionClearNew)(self->proc->session);
        } else {
            CALL(snd_postEvent)(0x802);
            self->proc->mode = 0x1000;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_0000266C);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_000029B0);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00002AE4);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00002C08);

INCLUDE_ASM("asm/nonmatchings/viewrec_uso/viewrec_uso", viewrec_uso_func_00002D9C);

/* draw label sprite i (from 0x378) at (x, y) tinted (255, 175, 44): the colour is r / d with the
 * divisor in a local (fire ctor lever) and int numerators, which keeps the 255.0f dividend apart from
 * the divisor; the colour is declared first for its sp+0x28 slot. */
void viewrec_uso_func_00002F1C(ViewRecScreen *self, s32 i, s32 x, s32 y) {
    f32 col[4];
    RaceSprite *spr = &sVrLabels_378[i];
    f32 d;

    d = 255.0f;
    col[0] = 255 / d;
    col[1] = 175 / d;
    col[2] = 44 / d;
    col[3] = 255 / d;
    CALL(gui_spriteRefresh)(spr);
    CALL(gui_setColorEnvPrim_000017DC)(spr, 255, col, 255);
    CALL(gui_drawSprite)(spr, x, y, 2);
}

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

/* the replay screen's draw: frame count, the records draws, the colours, label sprite 40 at
 * (160, 37), then the saved ghost's row (course / rider / board / mode - 1 via 0x1588).
 * The white `col` is set and never read, as in the target; the unused locals keep the frame
 * (0x58) and the homes of course / rider (sp+0x40 / 0x3C); the last two reads stay call
 * arguments (one spilled to the temp area at sp+0x28). */
void viewrec_uso_func_00004698(ViewRecScreen *self) {
    f32 col[4];
    s32 unused;
    s32 course, rider;
    s32 unused2[3];

    col[0] = 1.0f;
    col[1] = 1.0f;
    col[2] = 1.0f;
    col[3] = 1.0f;
    self->frame++;
    CALL(viewrec_uso_func_000011B4)(self->proc);
    CALL(viewrec_uso_func_000014DC)(self->proc);
    CALL(gui_setEnvPrimColor_00001EF4)(&D_00000000, 255, sVrPrimColour, sVrEnvColour);
    CALL(gui_spriteRefresh)(&sVrLabels[40]);
    CALL(gui_drawSprite)(&sVrLabels[40], 160, 37, 3);
    CALL(gui_setCombine_00001794)(&D_00000000);
    course = CALL(Backup_ghostGetCourse)((char *)gInputMgr_vr4698.backup + 4);
    rider = CALL(Backup_ghostGetRider)((char *)gInputMgr_vr4698.backup + 4);
    CALL(viewrec_uso_func_00001588)(self->proc, self->rowY, course, rider,
                                     CALL(Backup_ghostGetBoard)((char *)gInputMgr_vr4698.backup + 4),
                                     CALL(Backup_ghostGetMode)((char *)gInputMgr_vr4698.backup + 4) - 1);
}

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

