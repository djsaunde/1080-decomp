#include "common.h"
#include "uso_call.h"
#include "gdl/node.h"
#include "game/input.h"
#include "game/guimenu.h"
#include "gdl/race.h"

extern int gl_func_00000000();
extern char D_00000000;

/* viewrec.uso ("GENNewRecords Process"): the records / options screens. */

/* the process and the six screen classes: include/viewrec.h */
#include "viewrec.h"

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
extern RaceSprite sVrLabels_318[];    /* 0x318 (sVrLabels[12..]): the 0x11B4 background tiles */
/* 0x540 (sVrLabels[35]), the left arrow: its own symbol lets 0x12D4 keep &sprite in a0 for the
 * call and the .image load (sVrLabels[35] folds the load into the reloc) */
extern RaceSprite sVrLabels_540;
/* bootup Bss+0x28C (the gui string-table / language base; trnproc's trn_gRowBase), 0-pinned */
extern s32 vr_gLanguage;
extern RaceSprite sVrLabels_3F0[8];   /* 0x3F0 (sVrLabels[21..28]): the options grid (0x1D64) */
/* the vptrs the ctors store: bootup's Node (Data+0xC338) / Process (Data+0xC3A0) vtables and this
 * module's screen vtables (Data+0x98 / +0x118); the offset is in the USO reloc, so each is its own
 * 0-pinned symbol */
extern char vr_vt_node, vr_vt_proc, vr_vt_screen54, vr_vt_screen60;
/* void-returning alias of the placeholder (0-pinned): no dead v0 def to colour around */
extern void gl_func_00000000_vrv();
/* bootup image header: width / height at +0x20 / +0x22 */
typedef struct VrImage {
    /* 0x00 */ char unk00[0x20];
    /* 0x20 */ s16 width;
    /* 0x22 */ s16 height;
} VrImage;
extern f32 sVrPrimColour[4];        /* 0x858 */
extern f32 sVrEnvColour[4];         /* 0x868 */
extern GuiMenu *sVrMenu;            /* 0x878: the menu that has the pad */
extern GENNewRecordsProcess *sVrProc;        /* 0x87C */
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

/* back to the main menu. EXACT via the NCC (C++) donor src/viewrec_uso/viewrec_uso_ncc.cpp (a
 * second code-free test of sVrScreen after the 0x1A08 call puts &sVrScreen in a1). */
void vr_backToMainMenu(void) {
}

void vr_showRecordOptions(void) {
    CALL(Process_stop)(sVrScreen);
    VR_SHOW_SCREEN(recordOptions);
    sVrProc->mode = 0x1000;
}

void vr_showSoundOptions(void) {
    CALL(Process_stop)(sVrScreen);
    VR_SHOW_SCREEN(soundOptions);
    sVrProc->mode = 0x40000;
}

void vr_showLanguageOptions(void) {
    CALL(Process_stop)(sVrScreen);
    VR_SHOW_SCREEN(languageOptions);
    sVrProc->mode = 0x20000;
}

void vr_showEraseOptions(void) {
    CALL(Process_stop)(sVrScreen);
    VR_SHOW_SCREEN(eraseOptions);
    sVrProc->mode = 0x80000;
}

void vr_showReplayOptions(void) {
    CALL(Process_stop)(sVrScreen);
    VR_SHOW_SCREEN(replayOptions);
    sVrProc->mode = 0x4000000;
}

void vr_emptyStub_00000278(void) {
}

void vr_emptyStub_00000280(void) {
}

void vr_emptyStub_00000288(void) {
}

/* EXACT via the NCC (C++) donor src/viewrec_uso/viewrec_uso_ncc.cpp (static colour objects). */
void vr_resetColours(void) {
}

/* EXACT via the NCC (C++) donor src/viewrec_uso/viewrec_uso_ncc_2d0.cpp: the records process ctor. */
void GENNewRecordsProcess_ctor(void) {
}

/* load the 21 sprites of sVrSprites (0x1F0000.., the riders 0x1D0000.., the boards 0x1E0000.., 0x250000) */
void vr_loadSprites(GENNewRecordsProcess *proc) {
    CALL(gui_spriteLoad)(&sVrSprites[0], 0x1F0000);
    CALL(gui_spriteLoad)(&sVrSprites[1], 0x1F0001);
    CALL(gui_spriteLoad)(&sVrSprites[2], 0x1F0002);
    CALL(gui_spriteLoad)(&sVrSprites[3], 0x1D0000);
    CALL(gui_spriteLoad)(&sVrSprites[4], 0x1D0001);
    CALL(gui_spriteLoad)(&sVrSprites[5], 0x1D0002);
    CALL(gui_spriteLoad)(&sVrSprites[6], 0x1D0003);
    CALL(gui_spriteLoad)(&sVrSprites[7], 0x1D0004);
    CALL(gui_spriteLoad)(&sVrSprites[8], 0x1D0005);
    CALL(gui_spriteLoad)(&sVrSprites[9], 0x1D0006);
    CALL(gui_spriteLoad)(&sVrSprites[10], 0x1D0007);
    CALL(gui_spriteLoad)(&sVrSprites[11], 0x1E0000);
    CALL(gui_spriteLoad)(&sVrSprites[12], 0x1E0001);
    CALL(gui_spriteLoad)(&sVrSprites[13], 0x1E0002);
    CALL(gui_spriteLoad)(&sVrSprites[14], 0x1E0003);
    CALL(gui_spriteLoad)(&sVrSprites[15], 0x1E0004);
    CALL(gui_spriteLoad)(&sVrSprites[16], 0x1E0005);
    CALL(gui_spriteLoad)(&sVrSprites[17], 0x1E0006);
    CALL(gui_spriteLoad)(&sVrSprites[18], 0x1E0007);
    CALL(gui_spriteLoad)(&sVrSprites[19], 0x1E0008);
    CALL(gui_spriteLoad)(&sVrSprites[20], 0x250000);
}

/* init the 68 label sprites of sVrLabels; the language-dependent ones take vr_gLanguage */
void vr_initLabels(GENNewRecordsProcess *proc) {
    CALL(gui_spriteInit)(&sVrLabels[0], 0x260000 | (vr_gLanguage * 8));
    CALL(gui_spriteInit)(&sVrLabels[1], 0x260000 | (vr_gLanguage * 8 + 1));
    CALL(gui_spriteInit)(&sVrLabels[2], 0x260000 | (vr_gLanguage * 8 + 2));
    CALL(gui_spriteInit)(&sVrLabels[3], 0x260000 | (vr_gLanguage * 8 + 3));
    CALL(gui_spriteInit)(&sVrLabels[4], 0x260000 | (vr_gLanguage * 8 + 4));
    CALL(gui_spriteInit)(&sVrLabels[5], 0x260000 | (vr_gLanguage * 8 + 5));
    CALL(gui_spriteInit)(&sVrLabels[6], 0x260000 | (vr_gLanguage * 8 + 6));
    CALL(gui_spriteInit)(&sVrLabels[7], 0x260000 | (vr_gLanguage * 8 + 7));
    CALL(gui_spriteInit)(&sVrLabels[8], ((vr_gLanguage + 47) << 16) | 10);
    CALL(gui_spriteInit)(&sVrLabels[9], ((vr_gLanguage + 47) << 16) | 15);
    CALL(gui_spriteInit)(&sVrLabels[10], ((vr_gLanguage + 47) << 16) | 17);
    CALL(gui_spriteInit)(&sVrLabels[16], 0x290002);
    CALL(gui_spriteInit)(&sVrLabels[17], 0x290000);
    CALL(gui_spriteInit)(&sVrLabels[18], 0x290003);
    CALL(gui_spriteInit)(&sVrLabels[19], 0x290001);
    CALL(gui_spriteInit)(&sVrLabels[20], 0x290004);
    CALL(gui_spriteInit)(&sVrLabels[21], 0x2C0000);
    CALL(gui_spriteInit)(&sVrLabels[22], 0x2C0001);
    CALL(gui_spriteInit)(&sVrLabels[23], 0x2C0002);
    CALL(gui_spriteInit)(&sVrLabels[24], 0x2C0003);
    CALL(gui_spriteInit)(&sVrLabels[25], 0x2C0004);
    CALL(gui_spriteInit)(&sVrLabels[26], 0x2C0005);
    CALL(gui_spriteInit)(&sVrLabels[27], 0x2C0006);
    CALL(gui_spriteInit)(&sVrLabels[28], 0x2C0007);
    CALL(gui_spriteInit)(&sVrLabels[29], (vr_gLanguage + 47) << 16);
    CALL(gui_spriteInit)(&sVrLabels[30], ((vr_gLanguage + 47) << 16) | 1);
    CALL(gui_spriteInit)(&sVrLabels[31], ((vr_gLanguage + 47) << 16) | 2);
    CALL(gui_spriteInit)(&sVrLabels[32], ((vr_gLanguage + 47) << 16) | 3);
    CALL(gui_spriteInit)(&sVrLabels[33], ((vr_gLanguage + 47) << 16) | 4);
    CALL(gui_spriteInit)(&sVrLabels[11], 0x270003);
    CALL(gui_spriteInit)(&sVrLabels[12], 0x280000);
    CALL(gui_spriteInit)(&sVrLabels[13], 0x280001);
    CALL(gui_spriteInit)(&sVrLabels[14], 0x280002);
    CALL(gui_spriteInit)(&sVrLabels[15], 0x280003);
    CALL(gui_spriteInit)(&sVrLabels[34], 0x70003);
    CALL(gui_spriteInit)(&sVrLabels[35], 0x70004);
    CALL(gui_spriteInit)(&sVrLabels[36], 0x7000B);
    CALL(gui_spriteInit)(&sVrLabels[37], 0x2F0012);
    CALL(gui_spriteInit)(&sVrLabels[38], (vr_gLanguage + 42) << 16);
    CALL(gui_spriteInit)(&sVrLabels[39], ((vr_gLanguage + 47) << 16) | 7);
    CALL(gui_spriteInit)(&sVrLabels[40], ((vr_gLanguage + 47) << 16) | 11);
    CALL(gui_spriteInit)(&sVrLabels[41], ((vr_gLanguage + 47) << 16) | 14);
    CALL(gui_spriteInit)(&sVrLabels[42], ((vr_gLanguage + 47) << 16) | 13);
    CALL(gui_spriteInit)(&sVrLabels[43], ((vr_gLanguage + 47) << 16) | 12);
    CALL(gui_spriteInit)(&sVrLabels[45], ((vr_gLanguage + 47) << 16) | 8);
    CALL(gui_spriteInit)(&sVrLabels[46], ((vr_gLanguage + 47) << 16) | 9);
    CALL(gui_spriteInit)(&sVrLabels[47], ((vr_gLanguage + 47) << 16) | 10);
    CALL(gui_spriteInit)(&sVrLabels[44], ((vr_gLanguage + 47) << 16) | 15);
    CALL(gui_spriteInit)(&sVrLabels[48], 0x310003);
    CALL(gui_spriteInit)(&sVrLabels[49], 0x310004);
    CALL(gui_spriteInit)(&sVrLabels[50], 0x310005);
    CALL(gui_spriteInit)(&sVrLabels[51], (vr_gLanguage + 49) << 16);
    CALL(gui_spriteInit)(&sVrLabels[52], ((vr_gLanguage + 49) << 16) | 2);
    CALL(gui_spriteInit)(&sVrLabels[53], ((vr_gLanguage + 49) << 16) | 1);
    CALL(gui_spriteInit)(&sVrLabels[54], 0x2D0000);
    CALL(gui_spriteInit)(&sVrLabels[55], 0x2D0002);
    CALL(gui_spriteInit)(&sVrLabels[56], 0x2D0001);
    CALL(gui_spriteInit)(&sVrLabels[57], ((vr_gLanguage + 45) << 16) | 3);
    CALL(gui_spriteInit)(&sVrLabels[58], ((vr_gLanguage + 45) << 16) | 4);
    CALL(gui_spriteInit)(&sVrLabels[59], ((vr_gLanguage + 45) << 16) | 5);
    CALL(gui_spriteInit)(&sVrLabels[60], ((vr_gLanguage + 45) << 16) | 6);
    CALL(gui_spriteInit)(&sVrLabels[61], ((vr_gLanguage + 52) << 16) | 1);
    CALL(gui_spriteInit)(&sVrLabels[62], ((vr_gLanguage + 52) << 16) | 2);
    CALL(gui_spriteInit)(&sVrLabels[63], ((vr_gLanguage + 49) << 16) | 6);
    CALL(gui_spriteInit)(&sVrLabels[64], ((vr_gLanguage + 49) << 16) | 7);
    CALL(gui_spriteInit)(&sVrLabels[65], ((vr_gLanguage + 49) << 16) | 8);
    CALL(gui_spriteInit)(&sVrLabels[66], ((vr_gLanguage + 49) << 16) | 9);
    CALL(gui_spriteInit)(&sVrLabels[67], ((vr_gLanguage + 49) << 16) | 10);
}

/* push the current menu on the process's menu stack and start `menu` */
void vr_pushMenu(GENNewRecordsProcess *proc, GuiMenu *menu) {
    CALL(Process_stop)(sVrMenu);
    proc->menuStack[proc->menuDepth] = sVrMenu;
    proc->menuDepth++;
    sVrMenu = menu;
    VCALL(&sVrMenu->node, 10);
}

/* pop the menu stack: stop the current menu and restart the previous one */
void vr_popMenu(GENNewRecordsProcess *proc) {
    CALL(Process_stop)(sVrMenu);
    proc->menuDepth--;
    sVrMenu = proc->menuStack[proc->menuDepth];
    VCALL(&sVrMenu->node, 10);
}

/* the menu input: A on an enabled row with a callback plays the select sound and calls the row's
 * callback (sVrProc set for it). snd_postEvent through the void alias: an int return leaves a dead
 * $v0 def that pushes the sVrMenu reload into $v1 (target $v0). */
void vr_menuSelectInput(GENNewRecordsProcess *proc) {
    if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x40100)) {
        GuiMenu *m = sVrMenu;
        if (m->rows[m->cursor].arg != 0 && m->rows[m->cursor].enabled != 0) {
            XCALL(gl_func_00000000_vrv, snd_postEvent)(5);
            sVrProc = proc;
            sVrMenu->rows[sVrMenu->cursor].arg();
        }
    }
}

void vr_menuBackInput(GENNewRecordsProcess *proc) {
    if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x200)) {
        CALL(snd_postEvent)(0x802);
        if (proc->menuDepth == 0) {
            if (proc->flags & 0x10000) {
                CALL(vr_backToMainMenu)();
            } else {
                gInputMgr_vr.procState = gInputMgr_vr.nextProcState;
                CALL(Backup_sessionClearNew)(proc->session);
                CALL(Process_fadeOut_0000F954)(proc, -1, 0);
            }
        } else {
            CALL(vr_popMenu)(proc);
        }
    }
}

/* the records background: a 5 x 7 grid of tiles (tile per column from the .data row at 0x3D4),
 * then label sprite 37 at (160, 37). The unused x / j give the 0x68 frame. */
typedef struct { int v[5]; } VrRow5;

void vr_drawBackground(void *arg0) {
    VrRow5 row;
    int x, y, i, j;
    RaceSprite *spr;

    row = *(VrRow5 *)((char *)&XDATA(D_00000000, port_viewrec_data, 0x0) + 0x3D4);
    CALL(gui_setCombine_00001794)(&XDATA(D_00000000, port_gui_bss, 0x8));
    for (y = 16; y != 240; y += 32) {
        for (i = 0; i < 5; i++) {
            spr = &sVrLabels_318[row.v[i]];
            CALL(gui_spriteRefresh)(spr);
            CALL(gui_drawSprite)(spr, i * 64, y, 0);
        }
    }
    CALL(gui_spriteRefresh)(&sVrLabels[37]);
    CALL(gui_drawSprite)(&sVrLabels[37], 160, 37, 3);
}

/* draw sprite spr centred at y, and every 8 frames the blinking right / left arrows beside it
 * (labels 34 / 35, tinted (233, 255, 0)); col2 is set and unused, as in the target. */

void vr_drawTitleArrows(GENNewRecordsProcess *self, RaceSprite *spr, s32 y, s32 left, s32 right) {
    f32 col2[4];
    f32 col[4];
    f32 d;
    RaceSpriteImage *img;
    s32 w;
    RaceSpriteImage *aimg;
    s32 aw;

    d = 255.0f;
    col2[0] = 200 / d;
    col2[1] = 168 / d;
    col2[2] = 255 / d;
    col2[3] = 255 / d;
    col[0] = 233 / d;
    col[1] = 255 / d;
    col[2] = 0 / d;
    col[3] = 255 / d;
    CALL(gui_setEnvPrimColor_00001EF4)(&D_00000000, 255, sVrPrimColour, sVrEnvColour);
    CALL(gui_spriteRefresh)(spr);
    CALL(gui_drawSprite)(spr, 160, y, 3);
    w = self->arrowBlink;
    self->arrowBlink = w + 1;
    if (w & 8) {
        if (right) {
            CALL(gui_spriteRefresh)(&sVrLabels[34]);
            CALL(gui_setColorEnvPrim_000017DC)(&sVrLabels[34], 255, col, 255);
            img = spr->image;
            w = img->width;
            CALL(gui_drawSprite)(&sVrLabels[34], w / 2 + 168, y, 2);
        }
        if (left) {
            CALL(gui_spriteRefresh)(&sVrLabels_540);
            CALL(gui_setColorEnvPrim_000017DC)(&sVrLabels_540, 255, col, 255);
            img = spr->image;
            aimg = sVrLabels_540.image;
            w = img->width;
            aw = aimg->width;
            CALL(gui_drawSprite)(&sVrLabels_540, -(w / 2) - aw + 152, y, 2);
        }
    }
}

/* draw label sprite i centred at y. EXACT via the NCC (C++) donor src/viewrec_uso/viewrec_uso_ncc.cpp. */
void vr_drawCenteredLabel(GENNewRecordsProcess *proc, int i, int y) {
}

/* with flag 0x10000 set, draw label sprites 57, 56, 55, 54 right to left from x 285 at y 220, each
 * spaced by its image width + 4 (+ 4 more after every odd one). The image and its width read into
 * locals give the target's v0 / v1. */
void vr_drawFooterLabels(GENNewRecordsProcess *proc) {
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
void vr_drawGhostRow(GENNewRecordsProcess *self, s32 y, s32 course, s32 rider, s32 board, s32 mode) {
    RaceSprite *spr;

    CALL(vr_drawCenteredLabel)(self, course, y);
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
void GENNewRecordsProcess_update(GENNewRecordsProcess *self) {
}

void GENNewRecordsProcess_handleMsg(GENNewRecordsProcess *self, int *msg) {
    int type = *msg;
    if (type == 21) {
        CALL(dbg_printf)((char *)&D_00000000 + 1000);
        if (gInputMgr_vr.unk88 == 0) {
            CALL(vr_initLabels)(self);
        }
    }
    CALL(Node_handleMsg)(self, msg);
}


/* its ctor: create the pause menu (proc->menu), fill its rows (0x1A08), centre it, start it and
 * attach it as a child; the screen starts stopped */

MainOptions *MainOptions_ctor(MainOptions *self, GENNewRecordsProcess *proc) {
    char *o1;
    char *o2;
    char *w;

    if (self != 0 || (self = (MainOptions *)CALL(mem_new)(0x58)) != 0) {
        o1 = (char *)self;
        if (o1 != 0 || (o1 = (char *)CALL(mem_new)(0x50)) != 0) {
            o2 = o1;
            if (o2 != 0 || (o2 = (char *)CALL(mem_new)(0x2C)) != 0) {
                CALL(Node_ctor)(o2, (char *)&D_00000000 + 1020); ((Node *)o2)->vptr = (void *)&vr_vt_node;
            }
            ((Node *)o1)->vptr = (void *)&vr_vt_proc;
        }
        self->node.vptr = (void *)&vr_vt_screen54;
        self->node.name = (char *)&D_00000000 + 1028;
        self->proc = proc;
        self->font = proc->font;
        self->proc->menu = (GuiMenu *)CALL(PauseMenu_create)(0);
        CALL(gui_initRows_000100F0)(self->proc->menu, self);
        CALL(MainOptions_buildMenu)(self);
        self->proc->menu->unk30 = self->font;
        CALL(gui_centerRows_0001016C)(self->proc->menu);
        XCALL(gl_func_00000000_vrv, gui_centerAt_000102A4)(self->proc->menu, 130);
        w = (char *)self->proc->menu;
        o1 = (char *)NODE(w)->vptr;
        ((void (*)(int))VT_SLOT(o1, 10).fn)(VT_SLOT(o1, 10).delta + (int)w);
        o1 = (char *)self->proc->menu;
        CALL(list_append)(&self->node.children, o1);
        if (NODE(o1)->parent != 0) {
            NODE(o1)->unk04 = 1;
        }
        NODE(o1)->parent = &self->node;
        CALL(Process_stop)(self);
        self->paused = 1;
        self->fadingOut = 0;
        self->pauseMenu = 0;
    }
    return self;
}


/* the records screen update: run the menu input (0x1068); B leaves (fade out, clear the session's new
 * marks). The void calls go through the void alias: an int-returning snd_postEvent leaves a dead $v0
 * def that pushes &gInputMgr into $v1 (target $v0). */
void MainOptions_update(MainOptions *self) {
    XCALL(gl_func_00000000_vrv, vr_menuSelectInput)(self->proc);
    if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x200)) {
        XCALL(gl_func_00000000_vrv, snd_postEvent)(0x802);
        gInputMgr_vr.procState = gInputMgr_vr.nextProcState;
        XCALL(gl_func_00000000_vrv, Backup_sessionClearNew)(self->proc->session);
        XCALL(gl_func_00000000_vrv, Process_fadeOut_0000F954)(self->proc, -1, 0);
    }
    XCALL(gl_func_00000000_vrv, Node_update)(self);
}

void MainOptions_drawGui(MainOptions *self) {
    CALL(vr_drawBackground)(self->proc);
    CALL(vr_drawFooterLabels)(self->proc);
    CALL(gui_setEnvPrimColor_00001EF4)(&D_00000000, 255, sVrPrimColour, sVrEnvColour);
    CALL(gui_spriteRefresh)(&sVrLabels[38]);
    CALL(gui_drawSprite)(&sVrLabels[38], 160, 37, 3);
}

void MainOptions_handleMsg(MainOptions *self, int *msg) {
    int type = *msg;
    if (type == 21) {
        CALL(MainOptions_buildMenu)(self);
    }
    CALL(Node_handleMsg)(self, msg);
}

/* The main menu's row callbacks (the screen switches): Text relocs whose in-place addend is 0,
 * so each address is a 0-pinned alias of the function (as trnproc's 3A8). */
extern void vr_showRecordOptions_ref(void), vr_showReplayOptions_ref(void),
    vr_showSoundOptions_ref(void), vr_showLanguageOptions_ref(void), vr_showEraseOptions_ref(void);

/* fill the main menu: rows 1..5 of string bank vr_gLanguage + 42 (row 2 only when the word at
 * 0xA0000284 is 0x240B17D7); row 1 is disabled without a saved ghost */
void MainOptions_buildMenu(MainOptions *self) {
    CALL(gui_clearRows_00010A9C)(self->proc->menu);
    CALL(gui_addRowSub_00010B6C)(self->proc->menu, ((vr_gLanguage + 42) << 16) | 1, -1, vr_showRecordOptions_ref);
    if (*(s32 *)0xA0000284 == 0x240B17D7) {
        CALL(gui_addRowSub_00010B6C)(self->proc->menu, ((vr_gLanguage + 42) << 16) | 2, -1, vr_showReplayOptions_ref);
    }
    CALL(gui_addRowSub_00010B6C)(self->proc->menu, ((vr_gLanguage + 42) << 16) | 3, -1, vr_showSoundOptions_ref);
    CALL(gui_addRowSub_00010B6C)(self->proc->menu, ((vr_gLanguage + 42) << 16) | 4, -1, vr_showLanguageOptions_ref);
    CALL(gui_addRowSub_00010B6C)(self->proc->menu, ((vr_gLanguage + 42) << 16) | 5, -1, vr_showEraseOptions_ref);
    if (CALL(Backup_ghostGetMode)((char *)gInputMgr_vr.backup + 4) == 0) {
        self->proc->menu->rows[1].enabled = 0;
    }
}


/* its ctor (0xA0 bytes, vtable Data+0x118): the grid / tab layout constants; starts stopped */

RecordOptions *RecordOptions_ctor(RecordOptions *self, GENNewRecordsProcess *proc) {
    char *o1;
    char *o2;
    char *o3;
    s32 i;

    if (self != 0 || (self = (RecordOptions *)CALL(mem_new)(0xA0)) != 0) {
        o1 = (char *)self;
        o2 = o1;
        if (o1 != 0 || (o2 = (char *)CALL(mem_new)(0x50)) != 0) {
            o3 = o2;
            if (o3 != 0 || (o3 = (char *)CALL(mem_new)(0x2C)) != 0) {
                CALL(Node_ctor)(o3, (char *)&D_00000000 + 1040); ((Node *)o3)->vptr = (void *)&vr_vt_node;
            }
            ((Node *)o2)->vptr = (void *)&vr_vt_proc;
        }
        self->node.vptr = (void *)&vr_vt_screen60;
        self->node.name = (char *)&D_00000000 + 1048;
        self->proc = proc;
        self->font = proc->font;
        self->arcadeSel = 0;
        self->arcadeRow = 0;
        i = 0;
        do {
            self->subRows[i] = 0;
            i++;
        } while (i < 3);
        self->frame = 0;
        self->gridX = 101;
        self->gridDx = 119;
        self->gridY = 114;
        self->gridDy = 26;
        self->tabX = 67;
        self->tabDx = 87;
        self->tabY = 81;
        self->unk84 = 15;
        self->state = 0;
        self->tab = 0;
        self->cursor = 0;
        self->minCursor = 0;
        CALL(Process_stop)(self);
        self->paused = 1;
        self->fadingOut = 0;
        self->pauseMenu = 0;
    }
    return self;
}

void RecordOptions_update(RecordOptions *self) {
    switch (self->proc->mode) {
        case 0x1000:
            CALL(RecordOptions_menuInput)(self);
            break;
        case 0x2000:
            CALL(RecordOptions_coursePageInput)(self);
            break;
        case 0x4000:
            CALL(RecordOptions_coursePageInput)(self);
            break;
        case 0x8000:
            CALL(RecordOptions_arcadePageInput)(self);
            break;
    }
    CALL(Node_update)(self);
}

/* the options screen draw: the 4 x 2 option grid (alpha 64 below minCursor or on tab 2, the cursor
 * blinking when state == 1), the three tabs (the current one blinking), or per mode the sub-screens */
void RecordOptions_drawGui(RecordOptions *self) {
    f32 col[4];
    s32 pad[2];
    s32 blink;
    s32 pad2[3];
    s32 b1, b0;
    s32 a;
    s32 c;
    s32 i;
    s32 row;
    RaceSprite *spr;
    col[0] = 1.0f;
    col[1] = 1.0f;
    col[2] = 1.0f;
    col[3] = 1.0f;
    self->frame++;
    blink = (-self->frame << 4) & 0xFF;
    b1 = CALL(RecordOptions_prevCourse)(self) != self->cursor ? 1 : 0;
    b0 = CALL(RecordOptions_nextCourse)(self) != self->cursor ? 1 : 0;
    CALL(vr_drawBackground)(self->proc);
    XCALL(gl_func_00000000_vrv, vr_drawFooterLabels)(self->proc);
    switch (self->proc->mode) {
        case 0x1000:
            CALL(gui_setEnvPrimColor_00001EF4)(&D_00000000, 255, sVrPrimColour, sVrEnvColour);
            CALL(gui_spriteRefresh)(&sVrLabels[39]);
            XCALL(gl_func_00000000_vrv, gui_drawSprite)(&sVrLabels[39], 160, 37, 3);
            for (row = 0; row < 4; row++) {
                for (c = 0; c != 2; c++) {
                    spr = &sVrLabels_3F0[row * 2 + c];
                    CALL(gui_spriteRefresh)(spr);
                    a = 255;
                    if (row * 2 + c < self->minCursor || self->tab == 2) {
                        a = 64;
                    }
                    if (row * 2 + c == self->cursor && self->state == 1) {
                        a = blink;
                    }
                    CALL(gui_setColorEnvPrim_000017DC)(spr, a, col, 255);
                    CALL(gui_drawSprite)(spr, self->gridX + c * self->gridDx, self->gridY + row * self->gridDy, 3);
                }
            }
            spr = &sVrLabels[29];
            for (i = 0; i != 3; i++) {
                CALL(gui_spriteRefresh)(spr);
                if (i == self->tab) {
                    CALL(gui_setColorEnvPrim_000017DC)(spr, blink, col, 255);
                } else {
                    CALL(gui_setCombine_00001794)(spr);
                }
                CALL(gui_drawSprite)(spr, self->tabX + i * self->tabDx, self->tabY, 3);
                spr++;
            }
            break;
        case 0x2000:
            CALL(vr_drawTitleArrows)(self->proc, &sVrLabels[45], 37, b1, b0);
            CALL(RecordOptions_drawCourseTimes)(self);
            break;
        case 0x4000:
            CALL(vr_drawTitleArrows)(self->proc, &sVrLabels[46], 37, b1, b0);
            CALL(RecordOptions_drawCourseScores)(self);
            break;
        case 0x8000:
            CALL(RecordOptions_drawArcadeRecords)(self);
            break;
    }
}

/* options screen: the previous enabled option before the cursor (wrapping 0 -> 7), at least
 * minCursor; proc->enabledCourses holds the enabled bits. */
s32 RecordOptions_prevCourse(RecordOptions *self) {
    s32 c = self->cursor;
    s32 i;
    for (i = 0; i < 8; i++) {
        c--;
        if (c < 0) {
            c = 7;
        }
        if ((self->proc->enabledCourses & (1 << c)) && c >= self->minCursor) {
            return c;
        }
    }
    return c;
}

/* options screen: the next enabled option after the cursor (wrapping 7 -> 0) */
s32 RecordOptions_nextCourse(RecordOptions *self) {
    s32 c = self->cursor;
    s32 i;
    for (i = 0; i < 8; i++) {
        c++;
        if (c >= 8) {
            c = 0;
        }
        if ((self->proc->enabledCourses & (1 << c)) && c >= self->minCursor) {
            return c;
        }
    }
    return c;
}

/* EXACT via the NCC (C++) donor src/viewrec_uso/viewrec_uso_ncc.cpp (sVrMenu->start()). */
void RecordOptions_coursePageInput(RecordOptions *self) {
}

/* the arcade-records screen input: up / down move the highlighted row (0..2), A selects it,
 * B deselects or goes back, Start leaves (fade out, or back to the main menu when entered from it) */
void RecordOptions_arcadePageInput(RecordOptions *self) {
    s32 back = 0;
    if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x8008)) {
        if (self->arcadeRow < 2) {
            CALL(snd_postEvent)(1);
            self->arcadeRow++;
        }
        if (self->arcadeSel) {
            self->arcadeSel = self->arcadeRow + 1;
        }
    }
    if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x2004)) {
        if (self->arcadeRow > 0) {
            CALL(snd_postEvent)(1);
            self->arcadeRow--;
        }
        if (self->arcadeSel) {
            self->arcadeSel = self->arcadeRow + 1;
        }
    }
    if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x100)) {
        if (self->arcadeSel == 0) {
            CALL(snd_postEvent)(5);
            self->arcadeSel = self->arcadeRow + 1;
        } else if (!(self->proc->flags & 0x1000)) {
            back = 1;
        }
        CALL(snd_postEvent)(5);
    }
    if ((self->proc->flags & 0x1000) && CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x200)) {
        if (self->arcadeSel) {
            CALL(snd_postEvent)(0x802);
            self->arcadeSel = 0;
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

/* EXACT via the NCC (C++) donor src/viewrec_uso/viewrec_uso_ncc.cpp: the records menu input. */
void RecordOptions_menuInput(RecordOptions *self) {
}

/* a record row's sprites: the rank number i (tinted, skipped when i < 0), rider and board at x + 26 / x + 60 */
void vr_drawRankRider(RecordOptions *self, s32 i, s32 x, s32 y, s32 rider, s32 board) {
    RaceSprite *spr = &sVrSprites[i];
    f32 col[4];
    f32 d;

    d = 255.0f;
    col[0] = 20 / d;
    col[1] = 100 / d;
    col[2] = 255 / d;
    col[3] = 255 / d;
    if (i >= 0) {
        CALL(gui_spriteRefresh)(spr);
        CALL(gui_setColorEnvPrim_000017DC)(spr, 255, col, 255);
        CALL(gui_drawSprite)(spr, x, y, 2);
    }
    spr = &sVrSprites_048[rider];
    CALL(gui_spriteRefresh)(spr);
    CALL(gui_setCombine_00001794)(spr);
    CALL(gui_drawSprite)(spr, x + 26, y, 2);
    spr = &sVrSprites_108[board];
    CALL(gui_spriteRefresh)(spr);
    CALL(gui_setCombine_00001794)(spr);
    CALL(gui_drawSprite)(spr, x + 60, y, 2);
}

/* the time digits: three sprite-20 glyphs (digits[i] * 22 + 2) 21 px apart, centred on y. The dead
 * `h = 0` initialiser creates h's pseudo first, which gives it $v0 ahead of the image pointer ($v1),
 * as in the target (permuter-found; early-pseudo lever). */
void vr_drawTimeDigits(RecordOptions *self, s32 x, s32 y, u8 *digits) {
    RaceSprite *spr = &sVrSprites[20];
    f32 col[4];
    f32 d;
    s32 i;
    VrImage *img;
    s32 h = 0;

    d = 255.0f;
    col[0] = 40 / d;
    col[1] = 138 / d;
    col[2] = 255 / d;
    col[3] = 255 / d;
    CALL(gui_spriteRefresh)(spr);
    CALL(gui_setColorEnvPrim_000017DC)(spr, 255, col, 255);
    for (i = 0; i < 3; i++) {
        img = (VrImage *)spr->image;
        h = img->height;
        CALL(gui_drawSpriteIA8Clip_00002BB0)(spr, x + i * 21, y - h / 2, digits[i] * 22 + 2, 20);
    }
}

/* the time string: t[0..2] sprintf'd with the Data+0x428 format, drawn as label-sprite-11 glyphs 12 px
 * apart ('[' -> 10, ']' -> 11). As 0x2AE4: the dead `h = 0` initialiser gives h $v0; the image
 * pointer is a named local declared first (its home sits above buf[12], as 0x2D9C's buf). */
void vr_drawTime(RecordOptions *self, s32 x, s32 y, u8 *t) {
    VrImage *img;
    RaceSprite *spr = &sVrLabels[11];
    char buf[12];
    f32 col[4];
    f32 d;
    u32 i;
    u8 c;
    s32 h = 0;

    d = 255.0f;
    col[0] = 40 / d;
    col[1] = 100 / d;
    col[2] = 255 / d;
    col[3] = 255 / d;
    CALL(gui_spriteRefresh)(spr);
    CALL(gui_setColorEnvPrim_000017DC)(spr, 255, col, 255);
    CALL(gl_sprintf)(buf, (char *)&D_00000000 + 1064, t[0], t[1], t[2]);
    for (i = 0; i < CALL(str_len)(buf); i++) {
        if (buf[i] == '[') {
            c = 10;
        } else if (buf[i] == ']') {
            c = 11;
        } else {
            c = buf[i] - '0';
        }
        img = (VrImage *)spr->image;
        h = img->height;
        CALL(gui_drawSpriteIA8Clip_00002BB0)(spr, x + i * 12, y - h / 2, c * 16, 16);
    }
}

/* draw `val` (sprintf'd with the Data+0x438 format) as label-sprite-11 digits from x, 12 px apart,
 * skipping leading zeros ('[' -> glyph 10, ']' -> glyph 11; the last character is always drawn) */
void vr_drawNumber(RecordOptions *self, s32 x, s32 y, s32 val, f32 *col) {
    RaceSprite *spr = &sVrLabels[11];
    s32 started = 0;
    char buf[12];
    u32 i;
    s32 d;

    CALL(gui_spriteRefresh)(spr);
    CALL(gui_setColorEnvPrim_000017DC)(spr, 255, col, 255);
    CALL(gl_sprintf)(buf, (char *)&D_00000000 + 1080, val);
    for (i = 0; i < CALL(str_len)(buf); i++) {
        u8 c;
        if (buf[i] == '[') {
            c = 10;
        } else if (buf[i] == ']') {
            c = 11;
        } else {
            c = buf[i] - '0';
        }
        d = c;
        if (d) {
            started = 1;
        }
        if (started || i == CALL(str_len)(buf) - 1) {
            VrImage *img = (VrImage *)spr->image;
            s32 h = img->height;
            CALL(gui_drawSpriteIA8Clip_00002BB0)(spr, x + i * 12, y - h / 2, d * 16, 16);
        }
    }
}

/* draw label sprite i (from 0x378) at (x, y) tinted (255, 175, 44): the colour is r / d with the
 * divisor in a local (fire ctor lever) and int numerators, which keeps the 255.0f dividend apart from
 * the divisor; the colour is declared first for its sp+0x28 slot. */
void vr_drawTrickLabel(RecordOptions *self, s32 i, s32 x, s32 y) {
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

/* the course records: three rows through 0x29B0 / 0x2AE4 / 0x2C08, a new record blinking, ties drawn
 * without a rank. The three row draws are void calls (void alias): an int-returning call leaves the
 * dead $v0 def that pushes the rec[5] CSE into $v1. */
typedef struct VrTime { u8 b[4]; } VrTime;
typedef struct VrCourse { char e[6][8]; } VrCourse;
void RecordOptions_drawCourseTimes(RecordOptions *self) {
    s32 pad1;
    f32 col[4];
    s32 pad2[3];
    u8 prev[4];
    s32 pad3[2];
    s32 course;
    s32 y;
    s32 i;
    s32 show;
    s32 rank;
    u8 *rec;

    col[0] = 1.0f;
    col[1] = 1.0f;
    col[2] = 1.0f;
    col[3] = 1.0f;
    course = self->cursor;
    y = 123;
    CALL(gui_setCombine_00001794)(&XDATA(D_00000000, port_gui_bss, 0x8));
    CALL(vr_drawCenteredLabel)(self->proc, self->cursor, 83);
    *(VrTime *)prev = *(VrTime *)((char *)&XDATA(D_00000000, port_viewrec_data, 0x0) + 1088);
    for (i = 0; i < 3; y += 34, i++) {
        rec = (u8 *)CALL(Backup_courseRecordGetTime)(*self->proc->session + course * 48, i);
        show = 1;
        if (CALL(Backup_timeEntryIsNew)(((VrCourse *)*self->proc->session)[course].e[i])) {
            if (self->frame & 8) {
                show = 0;
            }
        }
        rank = i;
        if (prev[0] == rec[5] && prev[1] == rec[6] && prev[2] == rec[7]) {
            rank = -1;
        }
        if (show) {
            XCALL(gl_func_00000000_vrv, vr_drawRankRider)(self, rank, 38, y, rec[0] & 7, (rec[0] & 0x78) >> 3);
            XCALL(gl_func_00000000_vrv, vr_drawTimeDigits)(self, 132, y, rec + 2);
            XCALL(gl_func_00000000_vrv, vr_drawTime)(self, 196, y, rec + 5);
        }
        prev[0] = rec[5];
        prev[1] = rec[6];
        prev[2] = rec[7];
    }
}

/* 0x319C: the course scores page (three score rows of the selected course). Levers (agent-b run 662,
 * read in the -Wo,-zdbug:5 colouring trace): uopt colours the saved registers in priority order
 * (refs / live units), lowest free s-register first. `y = y * 1`, `self = self * 1` (in the loop) and
 * `course = course * 1` are emission-free extra refs that put self > y > i > show and course >
 * course*48 (the target's s1 / s2 / s3 / s4 and s7 / s8). The void alias on the 0x2D9C call drops its
 * dead $v0 def so e->score takes v0. `show = 1` after the GetScore call and `i = 0; do {` on one line
 * are as1 order levers (as1 schedules within the source-line groups). `if (1) {}` stops uopt hoisting
 * &col into s8. */
typedef struct { u8 b0; char name[3]; s32 score; } VrScoreEnt;
typedef struct { char pad[24]; VrScoreEnt scores[3]; } VrCourseRec;
void RecordOptions_drawCourseScores(RecordOptions *self) {
    f32 d;
    f32 col[4];
    s32 course;
    s32 i;
    s32 y;
    s32 prev;
    s32 show;
    s32 rank;
    VrScoreEnt *e;

    d = 255.0f;
    col[0] = 40 / d;
    col[1] = 100 / d;
    col[2] = 255 / d;
    col[3] = 255 / d;
    y = 123;
    course = self->cursor;
    CALL(gui_setCombine_00001794)(&D_00000000);
    CALL(vr_drawCenteredLabel)(self->proc, course, 83);
    course = course * 1;
    prev = -1;
    i = 0; do {
        e = (VrScoreEnt *)CALL(Backup_courseRecordGetScore)(&((VrCourseRec *)*self->proc->session)[course], i);
        show = 1;
        if (CALL(Backup_scoreEntryIsNew)(&((VrCourseRec *)*self->proc->session)[course].scores[i]) &&
            (self->frame & 8)) {
            show = 0;
        }
        rank = i;
        if (prev == e->score) {
            rank = -1;
        }
        if (show) {
            if (1) {}
            CALL(vr_drawRankRider)(self, rank, 38, y, e->b0 & 7, (e->b0 & 0x78) >> 3);
            CALL(vr_drawTimeDigits)(self, 132, y, e->name);
            XCALL(gl_func_00000000_vrv, vr_drawNumber)(self, 196, y, e->score, col);
        }
        prev = e->score;
        y = y * 1;
        self = (RecordOptions *)((s32)self * 1);
        y += 34;
        i++;
    } while (i < 3);
}

typedef struct VrArcadeEnt {
    /* 0x00 */ s32 b0;
    /* 0x04 */ u8 count;
    /* 0x05 */ char name[3];
    /* 0x08 */ s32 sub[5];
    /* 0x1C */ s32 score;
} VrArcadeEnt;
/* the arcade records screen's draw: each entry's sub-score row count (+0x60[i]) grows toward the
 * entry's count while it is the opened one (arcadeSel is 1-based) and shrinks otherwise; the block is
 * centred on y = 150; then per entry: the cursor sprite (label 36) on the selected row, rank / icons,
 * the sub scores (label j, 0x2D9C) and the name / total score. Levers: `d` declared first (its home
 * above the three colours puts them at sp+0x74 / 0x84 / 0x94), int numerators over d, do-while loops
 * (the outer one orders the loop-end increments; the inner guarded do-while, too), `show = 1` after
 * the entry read, the counter updates spelled on the array element (no named temp: one home less). */
void RecordOptions_drawArcadeRecords(RecordOptions *self) {
    f32 d;
    f32 col3[4];
    f32 col1[4];
    f32 col2[4];
    s32 i;
    s32 j;
    s32 total;
    s32 y;
    s32 prev;
    s32 show;
    s32 rank;
    VrArcadeEnt *e;

    d = 255.0f;
    col1[0] = 40 / d;
    col1[1] = 100 / d;
    col1[2] = 255 / d;
    col1[3] = 255 / d;
    col2[0] = 255 / d;
    col2[1] = 255 / d;
    col2[2] = 160 / d;
    col2[3] = 255 / d;
    col3[0] = 233 / d;
    col3[1] = 255 / d;
    col3[2] = 0 / d;
    col3[3] = 255 / d;
    total = 0;
    for (i = 0; i < 3; i++) {
        e = (VrArcadeEnt *)CALL(Backup_arcadeGetEntry)(((VrArcadeEnt **)self->proc->session)[1], i);
        /* `*(s32 *)&` keeps the target's registers here (the plain member read reorders the i + 1) */
        if (i + 1 == *(s32 *)&self->arcadeSel) {
            if (self->subRows[i] < e->count) {
                self->subRows[i]++;
            }
        } else if (self->subRows[i] > 0) {
            self->subRows[i]--;
        }
        if (self->subRows[i] != 0) {
            total += self->subRows[i] * 15;
        }
        total += 30;
    }
    y = 150 - total / 2;
    CALL(gui_setEnvPrimColor_00001EF4)(&D_00000000, 255, sVrPrimColour, sVrEnvColour);
    CALL(vr_drawCenteredLabel)(self->proc, 8, 37);
    prev = -1;
    i = 0;
    do {
        e = (VrArcadeEnt *)CALL(Backup_arcadeGetEntry)(((VrArcadeEnt **)self->proc->session)[1], i);
        show = 1;
        if (CALL(Backup_arcadeEntryIsNew)(&((VrArcadeEnt **)self->proc->session)[1][i]) &&
            (self->frame & 8)) {
            show = 0;
        }
        if (i == self->arcadeRow) {
            CALL(gui_spriteRefresh)(&sVrLabels[36]);
            CALL(gui_setColorEnvPrim_000017DC)(&sVrLabels[36], 255, col3, 255);
            CALL(gui_drawSprite)(&sVrLabels[36], 30, y, 3);
        }
        rank = i;
        if (prev == e->score) {
            rank = -1;
        }
        if (show) {
            CALL(vr_drawRankRider)(self, rank, 38, y, e->b0 & 7, (e->b0 & 0x78) >> 3);
        }
        j = 0;
        if (self->subRows[i] > 0) {
            do {
                CALL(vr_drawTrickLabel)(self, j, 132, y);
                CALL(vr_drawNumber)(self, 196, y, e->sub[j], col2);
                y += 15;
                j++;
            } while (j < self->subRows[i]);
        }
        if (show) {
            CALL(vr_drawTimeDigits)(self, 132, y, e->name);
            CALL(vr_drawNumber)(self, 196, y, e->score, col1);
        }
        prev = e->score;
        y += 30;
        i++;
    } while (i < 3);
}

/* the erase screen's ctor (200 bytes): the cfront chain Node (0x2C) -> Process (0x50) -> screen, each
 * level allocating when called with 0; the Node ctor call and its vptr store on one line (as in the
 * trnproc / h2h race ctors) give the target's schedule of the vptr address ahead of the o1 reload.
 * The three vtables are 0-pinned per-site symbols: bootup Data+0xC338 / +0xC3A0 imports and the
 * module's own Data+0x198. */
extern char vr_vtNode_36B4[], vr_vtProc_36B4[], vr_vtErase_36B4[];
EraseOptions *EraseOptions_ctor(EraseOptions *self, GENNewRecordsProcess *proc) {
    char *o1;
    char *o2;

    if (self != 0 || (self = (EraseOptions *)CALL(mem_new)(200)) != 0) {
        o1 = (char *)self;
        if (o1 != 0 || (o1 = (char *)CALL(mem_new)(0x50)) != 0) {
            o2 = o1;
            if (o2 != 0 || (o2 = (char *)CALL(mem_new)(0x2C)) != 0) { CALL(Node_ctor)(o2, &D_00000000 + 0x444); NODE(o2)->vptr = vr_vtNode_36B4; }
            NODE(o1)->vptr = vr_vtProc_36B4;
        }
        self->node.vptr = vr_vtErase_36B4;
        self->node.name = &D_00000000 + 0x44C;
        self->proc = proc;
        self->font = proc->font;
        self->gridX = 101;
        self->frame = 0;
        self->gridDx = 119;
        self->gridY = 115;
        self->gridDy = 26;
        self->setX = 66;
        self->setDx = 44;
        self->setY = 72;
        self->setDy = 17;
        self->rowY = 120;
        self->askY = 161;
        self->yesY = 181;
        self->phase = 0;
        self->set = 0;
        self->course = 0;
        self->minCourse = 0;
        self->selFade = 0.0f;
        self->askFade = 0.0f;
        CALL(Process_stop)(self);
        self->paused = 1;
        self->fadingOut = 0;
        self->pauseMenu = 0;
    }
    return self;
}

/* the erase screen's update: dispatch on the process mode */
void EraseOptions_update(EraseOptions *self) {
    switch (self->proc->mode) {
        case 0x80000:
            CALL(EraseOptions_selectInput)(self);
            break;
        case 0x2000000:
            CALL(EraseOptions_confirmInput)(self);
            break;
    }
    CALL(Node_update)(self);
}

/* the erase screen's draw: the five set labels, the course grid (selFade > 0) and the yes / no prompt
 * (selFade < 1). EXACT MIPSpro 7.2.1 donor src/viewrec_uso/viewrec_uso_mp72_3878.cpp (REPLACE_FUNC_BODY),
 * spliced over this placeholder on both build paths. */
void EraseOptions_drawGui(void *self) {
}

/* the erase screen's selection pad handling (proc mode 0x80000). Phase +0xA0 = 0: up / down move
 * the cursor +0xA4 over the five record sets (wrapping); A picks one: course times / scores go to
 * phase 1 (course picker), arcade / ghost / everything ask for confirmation (proc mode 0x2000000,
 * +0x70 = 1; the ghost only when one is saved). Phase 1: pick the course +0xA8 on the 2 x 4 grid
 * from +0xAC (2 for course times); A asks for confirmation, B goes back. */
void EraseOptions_selectInput(EraseOptions *self) {
    s32 old;

    switch (self->phase) {
        case 0:
            if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x10001)) {
                CALL(snd_postEvent)(1);
                self->set -= 1;
                if (self->set < 0) {
                    self->set = 4;
                }
            }
            if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x4002)) {
                CALL(snd_postEvent)(1);
                self->set += 1;
                if (self->set >= 5) {
                    self->set = 0;
                }
            }
            if (self->set == 0) {
                self->minCourse = 2;
            } else {
                self->minCourse = 0;
            }
            if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x100)) {
                switch (self->set) {
                    case 0:
                        XCALL(gl_func_00000000_vrv, snd_postEvent)(5);
                        self->proc->nextMode = 0x100000;
                        if (self->course < self->minCourse) {
                            self->course = self->minCourse;
                        }
                        self->phase += 1;
                        break;
                    case 1:
                        CALL(snd_postEvent)(5);
                        self->proc->nextMode = 0x200000;
                        self->phase += 1;
                        break;
                    case 2:
                        CALL(snd_postEvent)(0x195);
                        self->proc->nextMode = 0x400000;
                        self->proc->mode = 0x2000000;
                        self->no = 1;
                        break;
                    case 3:
                        if (CALL(Backup_ghostGetMode)((char *)gInputMgr_vr.backup + 4)) {
                            CALL(snd_postEvent)(0x195);
                            self->proc->nextMode = 0x800000;
                            self->proc->mode = 0x2000000;
                            self->ghostCourse = CALL(Backup_ghostGetCourse)((char *)gInputMgr_vr.backup + 4);
                            self->ghostRider = CALL(Backup_ghostGetRider)((char *)gInputMgr_vr.backup + 4);
                            self->ghostBoard = CALL(Backup_ghostGetBoard)((char *)gInputMgr_vr.backup + 4);
                            self->ghostMode = CALL(Backup_ghostGetMode)((char *)gInputMgr_vr.backup + 4) - 1;
                            self->no = 1;
                        }
                        break;
                    case 4:
                        CALL(snd_postEvent)(0x195);
                        self->proc->nextMode = 0x1000000;
                        self->proc->mode = 0x2000000;
                        self->no = 1;
                        break;
                }
            }
            if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x200)) {
                CALL(snd_postEvent)(0x802);
                if (self->proc->flags & 0x10000) {
                    CALL(vr_backToMainMenu)();
                }
            }
            break;
        case 1:
            old = self->course;
            if (self->course > self->minCourse && CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x10001) &&
                (self->course & 1)) {
                CALL(snd_postEvent)(1);
                self->course -= 1;
            }
            if (self->course < 8 && CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x4002) &&
                !(self->course & 1)) {
                CALL(snd_postEvent)(1);
                self->course += 1;
            }
            if (self->course - 2 >= self->minCourse &&
                CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x2004)) {
                CALL(snd_postEvent)(1);
                self->course -= 2;
            }
            if (self->course + 2 < 8 && CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x8008)) {
                CALL(snd_postEvent)(1);
                self->course += 2;
            }
            if (old != self->course) {
                self->frame = 0;
            }
            if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x100)) {
                CALL(snd_postEvent)(0x195);
                self->proc->mode = 0x2000000;
                self->no = 1;
            }
            if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x200)) {
                CALL(snd_postEvent)(0x802);
                self->phase -= 1;
            }
            break;
    }
}

/* The erase screen's pad handling. Left / right pick yes / no (+0x70), A erases the record set named
 * by proc+0x70 (0x100000 course times, 0x200000 course scores, 0x400000 arcade, 0x800000 ghost,
 * 0x1000000 everything, which restarts the game flow). The snd_postEvent(5) before the switch goes
 * through the void alias: with no dead int $v0 def at the call, the switch value takes v0 and the
 * reloaded proc v1. */
void EraseOptions_confirmInput(EraseOptions *self) {
    if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x10001) && self->no != 0) {
        CALL(snd_postEvent)(1);
        self->no = 0;
    }
    if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x4002) && self->no == 0) {
        CALL(snd_postEvent)(1);
        self->no = 1;
    }
    if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x40100)) {
        if (self->no == 0) {
            XCALL(gl_func_00000000_vrv, snd_postEvent)(5);
            switch (self->proc->nextMode) {
                case 0x100000:
                    CALL(Backup_sessionResetCourseTimes)(self->proc->session, self->course);
                    break;
                case 0x200000:
                    CALL(Backup_sessionResetCourseScores)(self->proc->session, self->course);
                    break;
                case 0x400000:
                    CALL(Backup_sessionResetArcade)(self->proc->session);
                    break;
                case 0x800000:
                    CALL(Backup_sessionResetGhost)(self->proc->session);
                    break;
                case 0x1000000:
                    gInputMgr_vr.procState = gInputMgr_vr.nextProcState;
                    gInputMgr_vr.unk84 = 0;
                    gInputMgr_vr.unk88 = 1;
                    CALL(Backup_resetAll_0000B710)(self->proc->session);
                    CALL(Process_fadeOut_0000F954)(self->proc, -1, 0);
                    return;
            }
            self->proc->mode = 0x80000;
        } else {
            CALL(snd_postEvent)(0x802);
            self->proc->mode = 0x80000;
        }
    }
    if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x200)) {
        CALL(snd_postEvent)(0x802);
        self->proc->mode = 0x80000;
    }
}

/* the replay screen's ctor (0x9C bytes): Node -> Process -> replay screen, as the erase screen's 0x36B4 */
extern char vr_vtNode_4518[], vr_vtProc_4518[], vr_vtReplay_4518[];
ReplayOptions *ReplayOptions_ctor(ReplayOptions *self, GENNewRecordsProcess *proc) {
    char *o1;
    char *o2;

    if (self != 0 || (self = (ReplayOptions *)CALL(mem_new)(156)) != 0) {
        o1 = (char *)self;
        if (o1 != 0 || (o1 = (char *)CALL(mem_new)(0x50)) != 0) {
            o2 = o1;
            if (o2 != 0 || (o2 = (char *)CALL(mem_new)(0x2C)) != 0) { CALL(Node_ctor)(o2, &D_00000000 + 0x45C); NODE(o2)->vptr = vr_vtNode_4518; }
            NODE(o1)->vptr = vr_vtProc_4518;
        }
        self->node.vptr = vr_vtReplay_4518;
        self->node.name = &D_00000000 + 0x464;
        self->proc = proc;
        self->font = proc->font;
        self->unk68 = 98;
        self->frame = 0;
        self->unk60 = 122;
        self->unk6C = 101;
        self->unk64 = 30;
        self->unk78 = 46;
        self->unk70 = 53;
        self->unk7C = 59;
        self->unk74 = 15;
        self->rowY = 96;
        self->unk84 = 165;
        self->unk88 = 181;
        self->unk8C[0] = 0;
        self->unk8C[1] = 0;
        self->unk8C[2] = 0;
        self->unk8C[3] = 0;
        CALL(Process_stop)(self);
        self->paused = 1;
        self->fadingOut = 0;
        self->pauseMenu = 0;
    }
    return self;
}

void ReplayOptions_update(ReplayOptions *self) {
    CALL(ReplayOptions_input)(self);
    CALL(Node_update)(self);
}

/* the replay screen's draw: frame count, the records draws, the colours, label sprite 40 at
 * (160, 37), then the saved ghost's row (course / rider / board / mode - 1 via 0x1588).
 * The white `col` is set and never read, as in the target; the unused locals keep the frame
 * (0x58) and the homes of course / rider (sp+0x40 / 0x3C); the last two reads stay call
 * arguments (one spilled to the temp area at sp+0x28). */
void ReplayOptions_drawGui(ReplayOptions *self) {
    f32 col[4];
    s32 unused;
    s32 course, rider;
    s32 unused2[3];

    col[0] = 1.0f;
    col[1] = 1.0f;
    col[2] = 1.0f;
    col[3] = 1.0f;
    self->frame++;
    CALL(vr_drawBackground)(self->proc);
    CALL(vr_drawFooterLabels)(self->proc);
    CALL(gui_setEnvPrimColor_00001EF4)(&D_00000000, 255, sVrPrimColour, sVrEnvColour);
    CALL(gui_spriteRefresh)(&sVrLabels[40]);
    CALL(gui_drawSprite)(&sVrLabels[40], 160, 37, 3);
    CALL(gui_setCombine_00001794)(&D_00000000);
    course = CALL(Backup_ghostGetCourse)((char *)gInputMgr_vr4698.backup + 4);
    rider = CALL(Backup_ghostGetRider)((char *)gInputMgr_vr4698.backup + 4);
    CALL(vr_drawGhostRow)(self->proc, self->rowY, course, rider,
                                     CALL(Backup_ghostGetBoard)((char *)gInputMgr_vr4698.backup + 4),
                                     CALL(Backup_ghostGetMode)((char *)gInputMgr_vr4698.backup + 4) - 1);
}

/* the replay screen's pad handling: A / Start fades out to the saved ghost's mode
 * (procState = Backup_ghostGetMode + 6, then state 1); B goes back to the main menu */
void ReplayOptions_input(ReplayOptions *self) {
    if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x40100)) {
        CALL(snd_postEvent)(5);
        gInputMgr_vr.procState = CALL(Backup_ghostGetMode)((char *)gInputMgr_vr.backup + 4) + 6;
        gInputMgr_vr.nextProcState = 1;
        CALL(Process_fadeOut_0000F954)(self->proc, -1, 0);
    }
    if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x200)) {
        CALL(snd_postEvent)(0x802);
        CALL(vr_backToMainMenu)();
    }
}

/* the 0x98-byte screen's ctor: Node -> Process -> screen, as the erase screen's 0x36B4; here every
 * constant field store follows the Process_stop call, made through the void alias so that no v0 def
 * sits at the call and the shared constant 76 colours v0 (lever from agent-g) */
extern char vr_vtNode_4854[], vr_vtProc_4854[], vr_vtScreen_4854[];
SoundOptions *SoundOptions_ctor(SoundOptions *self, GENNewRecordsProcess *proc) {
    char *o1;
    char *o2;

    if (self != 0 || (self = (SoundOptions *)CALL(mem_new)(0x98)) != 0) {
        o1 = (char *)self;
        if (o1 != 0 || (o1 = (char *)CALL(mem_new)(0x50)) != 0) {
            o2 = o1;
            if (o2 != 0 || (o2 = (char *)CALL(mem_new)(0x2C)) != 0) { CALL(Node_ctor)(o2, &D_00000000 + 0x474); NODE(o2)->vptr = vr_vtNode_4854; }
            NODE(o1)->vptr = vr_vtProc_4854;
        }
        self->node.vptr = vr_vtScreen_4854;
        self->node.name = &D_00000000 + 0x47C;
        self->proc = proc;
        self->font = proc->font;
        XCALL(gl_func_00000000_vrv, Process_stop)(self);
        self->modeX = 76;
        self->modeStep = 83;
        self->modeY = 173;
        self->unk5C = 32;
        self->labelX = 38;
        self->unk70 = 57;
        self->rowStep = 24;
        self->barX = 111;
        self->barStep = 15;
        self->rowY = 76;
        self->cursor = 0;
        self->paused = 1;
        self->fadingOut = 0;
        self->pauseMenu = 0;
    }
    return self;
}

/* 0x4990: the sound options screen's pad handling (up / down move the row, left / right step the BGM /
 * SE / voice level or the sound mode, B / Start save and go back). EXACT via the MIPSpro 7.2.1 donor
 * src/viewrec_uso/viewrec_uso_mp72_4990.cpp (REPLACE_FUNC_BODY): its 4-label switches are 4-entry jump
 * tables, which only EDG 2.29 emits. This placeholder's text is dropped by the splice. */
void SoundOptions_update(void *self) {
}


/* 0x7E0 (sVrLabels[63..65]): the three level-row labels. Indexed by the row, so a separate static
 * (the target folds 0x7E0 into the reloc). */
extern RaceSprite sVrLabels_7E0[3];

/* 0x4DEC: the sound options draw: the records draws, the colours, title label 41 at (160, 37); per
 * level row its label (sVrLabels_7E0[i]), the bar ends 48 / 49 and ten bar cells (50), the cell at the
 * current level in onBar's red; the highlighted row (cursor) blinks; then the three sound-mode labels
 * 51..53, the current one white (blinking when the cursor is on row 3), the others grey. `unused` /
 * `unused2` keep the 0x100 frame and the homes (white at sp+0xEC); `&sVrLabels_7E0[i]` with no pointer
 * local is a strength-reduced uopt temp spilled to sp+0x64, as in the target; `if (1) {}` lets d take
 * white's f0. */
void SoundOptions_drawGui(SoundOptions *self) {
    s32 unused;
    f32 white[4];
    f32 hiBar[4];
    f32 onBar[4];
    f32 hiLabel[4];
    f32 grey[4];
    f32 on[4];
    f32 d;
    f32 w, g, r;
    s32 i, j;
    s32 a;
    s32 unused2[4];

    white[0] = 1.0f;
    white[1] = 1.0f;
    white[2] = 1.0f;
    white[3] = 1.0f;
    if (1) {}
    d = 255.0f;
    w = 255 / d;
    g = 100 / d;
    on[0] = w;
    on[1] = w;
    on[2] = w;
    on[3] = w;
    grey[3] = w;
    grey[0] = g;
    grey[1] = g;
    grey[2] = g;
    CALL(vr_drawBackground)(self->proc);
    CALL(vr_drawFooterLabels)(self->proc);
    CALL(gui_setEnvPrimColor_00001EF4)(&D_00000000, 255, sVrPrimColour, sVrEnvColour);
    CALL(gui_spriteRefresh)(&sVrLabels[41]);
    CALL(gui_drawSprite)(&sVrLabels[41], 160, 37, 3);
    r = 50 / d;
    self->blink -= 16;
    for (i = 0; i < 3; i++) {
        a = 255;
        if (i == self->cursor) {
            a = self->blink;
            hiBar[0] = w; hiBar[1] = w; hiBar[2] = w; hiBar[3] = w;
            onBar[0] = r; onBar[1] = r; onBar[2] = r; onBar[3] = w;
            hiLabel[0] = w; hiLabel[1] = w; hiLabel[2] = w; hiLabel[3] = w;
        } else {
            hiBar[0] = g; hiBar[1] = g; hiBar[2] = g; hiBar[3] = w;
            onBar[0] = r; onBar[1] = r; onBar[2] = r; onBar[3] = w;
            hiLabel[0] = g; hiLabel[1] = g; hiLabel[2] = g; hiLabel[3] = w;
        }
        CALL(gui_spriteRefresh)(&sVrLabels_7E0[i]);
        CALL(gui_setColorEnvPrim_000017DC)(&sVrLabels_7E0[i], a, on, 255);
        CALL(gui_drawSprite)(&sVrLabels_7E0[i], self->labelX, self->rowY + self->rowStep * i, 2);
        CALL(gui_spriteRefresh)(&sVrLabels[48]);
        CALL(gui_setColorEnvPrim_000017DC)(&sVrLabels[48], a, hiLabel, 255);
        CALL(gui_drawSprite)(&sVrLabels[48], self->barX, self->rowY + self->rowStep * i, 3);
        CALL(gui_spriteRefresh)(&sVrLabels[49]);
        CALL(gui_setColorEnvPrim_000017DC)(&sVrLabels[49], a, hiLabel, 255);
        CALL(gui_drawSprite)(&sVrLabels[49], self->barX + self->barStep * 11, self->rowY + self->rowStep * i, 3);
        CALL(gui_spriteRefresh)(&sVrLabels[50]);
        CALL(gui_setColorEnvPrim_000017DC)(&sVrLabels[50], 255, hiBar, 255);
        for (j = 0; j != 10; j++) {
            if (j == self->levels[i]) {
                CALL(gui_setColorEnvPrim_000017DC)(&sVrLabels[50], 255, onBar, 255);
            }
            CALL(gui_drawSprite)(&sVrLabels[50], self->barX + self->barStep * (j + 1), self->rowY + self->rowStep * i, 3);
        }
    }
    a = (self->cursor == 3) ? self->blink : 255;
    for (j = 0; j < 3; j++) {
        CALL(gui_spriteRefresh)(&sVrLabels[51 + j]);
        if (j != self->mode) {
            CALL(gui_setColorEnvPrim_000017DC)(&sVrLabels[51 + j], 255, grey, 255);
        } else {
            CALL(gui_setColorEnvPrim_000017DC)(&sVrLabels[51 + j], a, on, 255);
        }
        CALL(gui_drawSprite)(&sVrLabels[51 + j], self->modeX + j * self->modeStep, self->modeY, 3);
    }
}

/* the 0x70-byte screen's ctor: Node -> Process -> screen, as 0x4854; +0x68 = the language word,
 * white colour at +0x58 spelled 255 / d with the divisor in a local and an int numerator (the 0x2F1C
 * lever), which keeps the 255.0f dividend apart from the divisor (lever from agent-g) */
extern char vr_vtNode_5200[], vr_vtProc_5200[], vr_vtScreen_5200[];
LanguageOptions *LanguageOptions_ctor(LanguageOptions *self, GENNewRecordsProcess *proc) {
    char *o1;
    char *o2;
    f32 c;
    f32 d;

    if (self != 0 || (self = (LanguageOptions *)CALL(mem_new)(0x70)) != 0) {
        o1 = (char *)self;
        if (o1 != 0 || (o1 = (char *)CALL(mem_new)(0x50)) != 0) {
            o2 = o1;
            if (o2 != 0 || (o2 = (char *)CALL(mem_new)(0x2C)) != 0) { CALL(Node_ctor)(o2, &D_00000000 + 0x48C); NODE(o2)->vptr = vr_vtNode_5200; }
            NODE(o1)->vptr = vr_vtProc_5200;
        }
        self->node.vptr = vr_vtScreen_5200;
        self->node.name = &D_00000000 + 0x494;
        self->proc = proc;
        self->font = proc->font;
        CALL(Process_stop)(self);
        self->language = vr_gLanguage;
        d = 255.0f; c = 255 / d;
        self->paused = 1;
        self->fadingOut = 0;
        self->pauseMenu = 0;
        self->colour[0] = c;
        self->colour[1] = c;
        self->colour[2] = c;
        self->colour[3] = c;
    }
    return self;
}


/* 0x5318: LanguageOptions pad handling: left / right set the flag (0 / 1, settings_setFlag4), A / B /
 * Start clear options bit 3 and save the settings, then return to the caller screen (proc flags
 * bit 16) or fade out to the next process state. */
void LanguageOptions_update(LanguageOptions *self) {
    if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x10001) && self->language != 0) {
        CALL(snd_postEvent)(1);
        self->language = 0;
        CALL(settings_setFlag4_000126EC)(&gInputMgr_vr, 0);
    }
    if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x4002) && self->language == 0) {
        CALL(snd_postEvent)(1);
        self->language = 1;
        CALL(settings_setFlag4_000126EC)(&gInputMgr_vr, 1);
    }
    if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x40300)) {
        CALL(snd_postEvent)(5);
        {
            u16 *p;
            if (1) {}
            p = (u16 *)(gInputMgr_vr.options + 4);
            *p &= ~8;
        }
        CALL(settings_save_00012150)(&gInputMgr_vr);
        if (self->proc->flags & 0x10000) {
            CALL(vr_backToMainMenu)();
        } else {
            gInputMgr_vr.procState = gInputMgr_vr.nextProcState;
            CALL(Process_fadeOut_0000F954)(self->proc, -1, 0);
        }
    }
    CALL(Node_update)(self);
}

/* 0x5440: LanguageOptions draw: the records draws, the colours, label sprite 42 at (160, 37), then
 * the two choice labels 66 / 67 at (90, 126) / (220, 126). The chosen one (flag 0 -> 66,
 * flag 1 -> 67) blinks through the blink fade counter (-16 per frame, the alpha wraps through
 * the low byte); the other is drawn at 128. `unused` keeps the 0x48 frame (col at sp+0x38). */
void LanguageOptions_drawGui(LanguageOptions *self) {
    f32 col[4];
    s32 unused[5];

    col[0] = 1.0f;
    col[1] = 1.0f;
    col[2] = 1.0f;
    col[3] = 1.0f;
    CALL(vr_drawBackground)(self->proc);
    CALL(vr_drawFooterLabels)(self->proc);
    self->blink -= 16;
    CALL(gui_setEnvPrimColor_00001EF4)(&D_00000000, 255, sVrPrimColour, sVrEnvColour);
    CALL(gui_spriteRefresh)(&sVrLabels[42]);
    CALL(gui_drawSprite)(&sVrLabels[42], 160, 37, 3);
    CALL(gui_spriteRefresh)(&sVrLabels[66]);
    if (self->language == 0) {
        CALL(gui_setColorEnvPrim_000017DC)(&sVrLabels[66], self->blink, col, 255);
    } else {
        CALL(gui_setColorEnvPrim_000017DC)(&sVrLabels[66], 128, col, 255);
    }
    CALL(gui_drawSprite)(&sVrLabels[66], 90, 126, 3);
    CALL(gui_spriteRefresh)(&sVrLabels[67]);
    if (self->language != 0) {
        CALL(gui_setColorEnvPrim_000017DC)(&sVrLabels[67], self->blink, col, 255);
    } else {
        CALL(gui_setColorEnvPrim_000017DC)(&sVrLabels[67], 128, col, 255);
    }
    CALL(gui_drawSprite)(&sVrLabels[67], 220, 126, 3);
}

void GENNewRecordsProcess_drawGui(int a0) {
}

