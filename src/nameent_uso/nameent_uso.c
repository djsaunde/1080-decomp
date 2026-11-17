#include "common.h"
#include "gdl/node.h"
#include "game/input.h"
#include "uso_call.h"
#include "game/nameent.h"

extern int gl_func_00000000();
extern char D_00000000;
/* nameent.uso's own Data section base (Data-base relocs, in-place addend = offset; pinned at
 * 0 in undefined_syms_auto.txt). */
extern char ne_data[];
/* void-typed alias of the placeholder (pinned 0): a call whose dead int return would hold v0. */
extern void ne_func_00000000_v();
/* gui.uso Bss (the gui draw state: gui_setColor / gui_setFillColor / gui_fillRect first argument). */
extern char ne_gGui;

/* nameent_uso_func_00000000: empty function at Text+0 (unreferenced). */
/* UNNAMED: sentinel a *_00000000 name: rename-symbol.py refuses it (it doubles as the address-0 placeholder spelling) */
void nameent_uso_func_00000000(void) {
}

/* ne_nop_00000008: empty function. */
void ne_nop_00000008(void) {
}

/* ne_nop_00000010: empty function. */
void ne_nop_00000010(void) {
}

/* NameEntry_init: NameEnter process init: name (Data+0x140), mode a1 at +0x50,
 * cursor 160 / 85, the entry object (760) at +0x54, pad 0 active, three gui sprites
 * 0x280000..2. */
void NameEntry_init(char *self, int mode) {
    NODE(self)->name = ne_data + 0x140; /* "NameEnter Process" */
    NEPROC(self)->session = (struct BackupSession *)mode;
    NEPROC(self)->entered = 0;
    NEPROC(self)->cursorX = 160;
    NEPROC(self)->cursorY = 85;
    NEPROC(self)->showBoard = 1;
    NEPROC(self)->message = (NameEnterMessage *)CALL(NameEnterMessage_ctor)(0, self, mode);
    CALL(input_setActivePad_0001207C)(&D_00000000, 0);
    CALL(gui_spriteInit)(NEPROC(self)->frameTop, 0x280000);
    CALL(gui_spriteInit)(NEPROC(self)->frameLeft, 0x280001);
    CALL(gui_spriteInit)(NEPROC(self)->frameRight, 0x280002);
    NEPROC(self)->pauseMenu = 0;
    NEPROC(self)->paused = 1;
    NEPROC(self)->fadingOut = 0;
}

/* NameEntry_startEntry: start: phase 1 (+0x60), bump GameFlow procState, link this under
 * the shared Fader and fade in. */
void NameEntry_startEntry(char *self) {
    char *head;
    NEPROC(self)->showBoard = 0;
    NEPROC(self)->phase = 1;
    INPUT_MGR(D_00000000).procState++;
    head = INPUT_MGR(D_00000000).fader;
    CALL(list_append)(&NODE(self)->children, head);
    if (NODE(head)->parent != 0) {
        NODE(head)->unk04 = 1;
    }
    NODE(head)->parent = (Node *)self;
    CALL(Fader_start)(INPUT_MGR(D_00000000).fader, 1, 0);
}

/* NameEntry_handleInput: update: stick sound from pad 1, then the phase's virtual
 * (slots 14 / 15 / 16 by +0x60). */
/* The stick-sound call goes through a void alias: its dead int return would hold v0, and the
 * target keeps the phase (then the vtable) in v0. */
void NameEntry_handleInput(char *self) {
    char *pad = (char *)CALL(input_getPad_00011D40)(&D_00000000, 1);
    XCALL(ne_func_00000000_v, snd_setPlayer3FreqScaleFromStick)(((InputPad *)pad)->rawStickX, ((InputPad *)pad)->rawStickY);
    switch (NEPROC(self)->phase) {
    case 0:
        VT_SLOT(NODE(self)->vptr, 14).fn(VT_SLOT(NODE(self)->vptr, 14).delta + self);
        return;
    case 1:
        VT_SLOT(NODE(self)->vptr, 15).fn(VT_SLOT(NODE(self)->vptr, 15).delta + self);
        return;
    case 2:
        VT_SLOT(NODE(self)->vptr, 16).fn(VT_SLOT(NODE(self)->vptr, 16).delta + self);
        return;
    }
}

/* NameEntry_drawGui: display by phase (+0x60): phases 0 / 1 draw the sprites (5B4) and
 * the board (2F4) or a cleared screen (458), then the phase's virtual (slots 17 / 18 / 19). */
void NameEntry_drawGui(char *self) {
    switch (NEPROC(self)->phase) {
    case 0:
        CALL(NameEntry_drawFrame)(self);
        if (NEPROC(self)->showBoard != 0) {
            CALL(NameEntry_drawBoard)(self);
        } else {
            CALL(NameEntry_clearScreen)(self);
        }
        VT_SLOT(NODE(self)->vptr, 17).fn(VT_SLOT(NODE(self)->vptr, 17).delta + self);
        break;
    case 1:
        CALL(NameEntry_drawFrame)(self);
        if (NEPROC(self)->showBoard != 0) {
            CALL(NameEntry_drawBoard)(self);
        } else {
            CALL(NameEntry_clearScreen)(self);
        }
        VT_SLOT(NODE(self)->vptr, 18).fn(VT_SLOT(NODE(self)->vptr, 18).delta + self);
        break;
    case 2:
        VT_SLOT(NODE(self)->vptr, 19).fn(VT_SLOT(NODE(self)->vptr, 19).delta + self);
        break;
    }
}

/* NameEntry_drawBoard: board draw: the name-board image (a copy of the Data+0x158 image
 * header with the current rider's texture from virtual slot 11) centred on the cursor x (+0x58)
 * at y 40 in white, then the two black letterbox bars. */
/* EXACT via the NCC (C++) donor src/nameent_uso/nameent_uso_ncc.cpp (REPLACE_FUNC_BODY). */
void NameEntry_drawBoard(char *self) {
}

/* NameEntry_clearScreen: clear the screen: fill colour {0, 0, 0, 0} at alpha 255, then a
 * full-screen 320x240 fillRect (as tit_WhiteFader_display). */
void NameEntry_clearScreen(char *self) {
    float colour[4];
    int pad[6]; /* frame 0x48: 24 bytes below the colour, as the target */
    colour[0] = 0.0f;
    colour[1] = 0.0f;
    colour[2] = 0.0f;
    colour[3] = 0.0f;
    CALL(gui_setFillColor_00001CA8)(&D_00000000, 255, colour);
    CALL(gui_fillRect)(&D_00000000, 0, 0, 0x13F, 0xEF, 0x10001);
}

/* NameEntry_updateResult: empty one-argument function (a no-op virtual). */
void NameEntry_updateResult(int a0) {
}

/* NameEntry_updateEntry: finish: with a name entered (+0x6C) and a record session (+0x50),
 * copy the 3-char name + the +0xB0 word into GameFlow +0x158 / +0x15C, write it to the session
 * (with GameFlow +0x4F / +0x57) and set the new names; fade the process out. Without a name,
 * forward to the entry object's virtual slot 3. */
void NameEntry_updateEntry(char *self) {
    char name[4];
    char *dst;
    int i;
    char *obj;
    if (NEPROC(self)->entered != 0) {
        if (NEPROC(self)->session != 0) {
            i = 0;
            INPUT_MGR(D_00000000).unk15C = NEPROC(self)->message->len; /* GameFlow name length */
            dst = INPUT_MGR(D_00000000).initials;
            do {
                *dst++ = NEPROC(self)->message->letters[i];
                name[i] = NEPROC(self)->message->letters[i];
                i++;
            } while (i != 3);
            CALL(Backup_writeCourseArcadeOptions_0000B3DC)(NEPROC(self)->session, name,
                ((u8 *)&INPUT_MGR(D_00000000).playerRider)[3], ((u8 *)&INPUT_MGR(D_00000000).playerBoard)[3]); /* the s32s' low bytes */
            CALL(Backup_sessionSetNewNames)(NEPROC(self)->session);
        }
        CALL(Process_fadeOut_0000F954)(self, -1, 0);
    } else {
        obj = (char *)NEPROC(self)->message;
        VT_SLOT(NODE(obj)->vptr, 3).fn(VT_SLOT(NODE(obj)->vptr, 3).delta + obj);
    }
}

/* NameEntry_updatePhase2: empty one-argument function (a no-op virtual). */
void NameEntry_updatePhase2(int a0) {
}

/* NameEntry_drawFrame: board frame sprites: the left (+0xA4) and right (+0xBC) edge
 * columns every 32 px, and the top piece (+0x8C) along rows y 8 / 136 / 168 / 200 every 64 px.
 * The right column's `!=` test (the target's held-232 bnel) also leaves self homed in memory
 * (each sprite address is a fresh `lw; addiu`). */
void NameEntry_drawFrame(int *self) {
    char *spr;
    int i;

    CALL(gui_setCombine_00001794)(&ne_gGui);
    spr = NEPROC(self)->frameLeft;
    CALL(gui_spriteRefresh)(spr);
    for (i = 8; i < 232; i += 32) {
        CALL(gui_drawSprite)(spr, 0, i, 0);
    }
    spr = NEPROC(self)->frameRight;
    CALL(gui_spriteRefresh)(spr);
    for (i = 8; i != 232; i += 32) {
        CALL(gui_drawSprite)(spr, 256, i, 0);
    }
    spr = NEPROC(self)->frameTop;
    CALL(gui_spriteRefresh)(spr);
    for (i = 64; i < 256; i += 64) {
        CALL(gui_drawSprite)(spr, i, 8, 0);
    }
    for (i = 64; i < 256; i += 64) {
        CALL(gui_drawSprite)(spr, i, 136, 0);
    }
    for (i = 64; i < 256; i += 64) {
        CALL(gui_drawSprite)(spr, i, 168, 0);
    }
    for (i = 64; i < 256; i += 64) {
        CALL(gui_drawSprite)(spr, i, 200, 0);
    }
}

/* NameEntry_drawEntryOverlay: empty one-argument function (a no-op virtual). */
void NameEntry_drawEntryOverlay(int a0) {
}

/* NameEntry_drawExitOverlay: forward to the +0x54 child's virtual slot 2 (as
 * CommonInfo_loadData_00008F98). */
void NameEntry_drawExitOverlay(char *self) {
    int *p2 = (int *)NODE(NEPROC(self)->message)->vptr;
    char *p1 = (char *)NEPROC(self)->message;
    VT_SLOT(p2, 2).fn(VT_SLOT(p2, 2).delta + p1);
}

/* NameEntry_drawPhase2: empty one-argument function (a no-op virtual). */
void NameEntry_drawPhase2(int a0) {
}

/* NameEntry_nopSlot20: empty one-argument function (a no-op virtual). */
void NameEntry_nopSlot20(int a0) {
}

/* NameEntry_nopSlot21: empty one-argument function (a no-op virtual). */
void NameEntry_nopSlot21(int a0) {
}

/* NameEnterMessage_ctor: the name-entry object's ctor. EXACT via the NCC (C++) donor
 * src/nameent_uso/nameent_uso_ncc.cpp (REPLACE_FUNC_BODY). */
int NameEnterMessage_ctor(char *self, char *owner, int mode) {
}

/* NameEnterMessage_display: the name-entry display (board sprites, letters, the letter wheel).
 * EXACT via the NCC (C++) donor src/nameent_uso/nameent_uso_ncc.cpp (REPLACE_FUNC_BODY). */
void NameEnterMessage_display(char *self) {
}

/* NameEnterMessage_update: the name-entry input (the letter wheel). EXACT via the NCC (C++)
 * donor src/nameent_uso/nameent_uso_ncc.cpp (REPLACE_FUNC_BODY). */
void NameEnterMessage_update(char *self) {
}

