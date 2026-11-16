#include "common.h"
#include "gdl/node.h"
#include "game/input.h"
#include "uso_call.h"

extern int gl_func_00000000();
extern char D_00000000;
/* nameent.uso's own Data section base (Data-base relocs, in-place addend = offset; pinned at
 * 0 in undefined_syms_auto.txt). */
extern char ne_data[];
/* void-typed alias of the placeholder (pinned 0): a call whose dead int return would hold v0. */
extern void ne_func_00000000_v();
/* gui.uso Bss (the gui draw state: gui_setColor / gui_setFillColor / gui_fillRect first argument). */
extern char ne_gGui;

/* nameent_uso_func_00000000: empty function. */
void nameent_uso_func_00000000(void) {
}

/* nameent_uso_func_00000008: empty function. */
void nameent_uso_func_00000008(void) {
}

/* nameent_uso_func_00000010: empty function. */
void nameent_uso_func_00000010(void) {
}

/* nameent_uso_func_00000018: NameEnter process init: name (Data+0x140), mode a1 at +0x50,
 * cursor 160 / 85, the entry object (760) at +0x54, pad 0 active, three gui sprites
 * 0x280000..2. */
void nameent_uso_func_00000018(char *self, int mode) {
    *(char **)(self + 0xC) = ne_data + 0x140;
    *(int *)(self + 0x50) = mode;
    *(int *)(self + 0x6C) = 0;
    *(int *)(self + 0x58) = 160;
    *(int *)(self + 0x5C) = 85;
    *(int *)(self + 0x70) = 1;
    *(int *)(self + 0x54) = CALL(nameent_uso_func_00000760)(0, self, mode);
    CALL(input_setActivePad_0001207C)(&D_00000000, 0);
    CALL(gui_spriteInit)(self + 0x8C, 0x280000);
    CALL(gui_spriteInit)(self + 0xA4, 0x280001);
    CALL(gui_spriteInit)(self + 0xBC, 0x280002);
    *(int *)(self + 0x48) = 0;
    *(int *)(self + 0x30) = 1;
    *(int *)(self + 0x2C) = 0;
}

/* nameent_uso_func_000000C8: start: phase 1 (+0x60), bump GameFlow procState, link this under
 * the shared Fader and fade in. */
void nameent_uso_func_000000C8(char *self) {
    char *head;
    *(int *)(self + 0x70) = 0;
    *(int *)(self + 0x60) = 1;
    INPUT_MGR(D_00000000).procState++;
    head = INPUT_MGR(D_00000000).fader;
    CALL(list_append)(self + 0x10, head);
    if (NODE(head)->parent != 0) {
        NODE(head)->unk04 = 1;
    }
    NODE(head)->parent = (Node *)self;
    CALL(Fader_start)(INPUT_MGR(D_00000000).fader, 1, 0);
}

/* nameent_uso_func_00000150: update: stick sound from pad 1, then the phase's virtual
 * (slots 14 / 15 / 16 by +0x60). */
/* The stick-sound call goes through a void alias: its dead int return would hold v0, and the
 * target keeps the phase (then the vtable) in v0. */
void nameent_uso_func_00000150(char *self) {
    char *pad = (char *)CALL(input_getPad_00011D40)(&D_00000000, 1);
    XCALL(ne_func_00000000_v, snd_setPlayer3FreqScaleFromStick)(*(signed char *)(pad + 8), *(signed char *)(pad + 9));
    switch (*(int *)(self + 0x60)) {
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

/* nameent_uso_func_00000204: display by phase (+0x60): phases 0 / 1 draw the sprites (5B4) and
 * the board (2F4) or a cleared screen (458), then the phase's virtual (slots 17 / 18 / 19). */
void nameent_uso_func_00000204(char *self) {
    switch (*(int *)(self + 0x60)) {
    case 0:
        CALL(nameent_uso_func_000005B4)(self);
        if (*(int *)(self + 0x70) != 0) {
            CALL(nameent_uso_func_000002F4)(self);
        } else {
            CALL(nameent_uso_func_00000458)(self);
        }
        VT_SLOT(NODE(self)->vptr, 17).fn(VT_SLOT(NODE(self)->vptr, 17).delta + self);
        break;
    case 1:
        CALL(nameent_uso_func_000005B4)(self);
        if (*(int *)(self + 0x70) != 0) {
            CALL(nameent_uso_func_000002F4)(self);
        } else {
            CALL(nameent_uso_func_00000458)(self);
        }
        VT_SLOT(NODE(self)->vptr, 18).fn(VT_SLOT(NODE(self)->vptr, 18).delta + self);
        break;
    case 2:
        VT_SLOT(NODE(self)->vptr, 19).fn(VT_SLOT(NODE(self)->vptr, 19).delta + self);
        break;
    }
}

/* nameent_uso_func_000002F4: board draw: the name-board image (a copy of the Data+0x158 image
 * header with the current rider's texture from virtual slot 11) centred on the cursor x (+0x58)
 * at y 40 in white, then the two black letterbox bars. */
/* EXACT via the NCC (C++) donor src/nameent_uso/nameent_uso_ncc.cpp (REPLACE_FUNC_BODY). */
void nameent_uso_func_000002F4(char *self) {
}

/* nameent_uso_func_00000458: clear the screen: fill colour {0, 0, 0, 0} at alpha 255, then a
 * full-screen 320x240 fillRect (as tit_WhiteFader_display). */
void nameent_uso_func_00000458(char *self) {
    float colour[4];
    int pad[6]; /* frame 0x48: 24 bytes below the colour, as the target */
    colour[0] = 0.0f;
    colour[1] = 0.0f;
    colour[2] = 0.0f;
    colour[3] = 0.0f;
    CALL(gui_setFillColor_00001CA8)(&D_00000000, 255, colour);
    CALL(gui_fillRect)(&D_00000000, 0, 0, 0x13F, 0xEF, 0x10001);
}

/* nameent_uso_func_000004C8: empty one-argument function (a no-op virtual). */
void nameent_uso_func_000004C8(int a0) {
}

/* nameent_uso_func_000004D0: finish: with a name entered (+0x6C) and a record session (+0x50),
 * copy the 3-char name + the +0xB0 word into GameFlow +0x158 / +0x15C, write it to the session
 * (with GameFlow +0x4F / +0x57) and set the new names; fade the process out. Without a name,
 * forward to the entry object's virtual slot 3. */
void nameent_uso_func_000004D0(char *self) {
    char name[4];
    char *dst;
    int i;
    char *obj;
    if (*(int *)(self + 0x6C) != 0) {
        if (*(int *)(self + 0x50) != 0) {
            i = 0;
            *(int *)((char *)&D_00000000 + 0x15C) = *(int *)(*(char **)(self + 0x54) + 0xB0);
            dst = (char *)&D_00000000 + 0x158;
            do {
                *dst++ = (*(char **)(self + 0x54))[0xB8 + i];
                name[i] = (*(char **)(self + 0x54))[0xB8 + i];
                i++;
            } while (i != 3);
            CALL(Backup_writeCourseArcadeOptions_0000B3DC)(*(int *)(self + 0x50), name,
                *((unsigned char *)&D_00000000 + 0x4F), *((unsigned char *)&D_00000000 + 0x57));
            CALL(Backup_sessionSetNewNames)(*(int *)(self + 0x50));
        }
        CALL(Process_fadeOut_0000F954)(self, -1, 0);
    } else {
        obj = *(char **)(self + 0x54);
        VT_SLOT(NODE(obj)->vptr, 3).fn(VT_SLOT(NODE(obj)->vptr, 3).delta + obj);
    }
}

/* nameent_uso_func_000005AC: empty one-argument function (a no-op virtual). */
void nameent_uso_func_000005AC(int a0) {
}

/* nameent_uso_func_000005B4: board frame sprites: the left (+0xA4) and right (+0xBC) edge
 * columns every 32 px, and the top piece (+0x8C) along rows y 8 / 136 / 168 / 200 every 64 px.
 * The right column's `!=` test (the target's held-232 bnel) also leaves self homed in memory
 * (each sprite address is a fresh `lw; addiu`). */
void nameent_uso_func_000005B4(int *self) {
    char *spr;
    int i;

    CALL(gui_setCombine_00001794)(&ne_gGui);
    spr = (char *)&self[0xA4 / 4];
    CALL(gui_spriteRefresh)(spr);
    for (i = 8; i < 232; i += 32) {
        CALL(gui_drawSprite)(spr, 0, i, 0);
    }
    spr = (char *)&self[0xBC / 4];
    CALL(gui_spriteRefresh)(spr);
    for (i = 8; i != 232; i += 32) {
        CALL(gui_drawSprite)(spr, 256, i, 0);
    }
    spr = (char *)&self[0x8C / 4];
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

/* nameent_uso_func_00000710: empty one-argument function (a no-op virtual). */
void nameent_uso_func_00000710(int a0) {
}

/* nameent_uso_func_00000718: forward to the +0x54 child's virtual slot 2 (as
 * CommonInfo_loadData_00008F98). */
void nameent_uso_func_00000718(char *self) {
    int *p2 = (int *)NODE(*(char **)(self + 0x54))->vptr;
    char *p1 = *(char **)(self + 0x54);
    VT_SLOT(p2, 2).fn(VT_SLOT(p2, 2).delta + p1);
}

/* nameent_uso_func_00000748: empty one-argument function (a no-op virtual). */
void nameent_uso_func_00000748(int a0) {
}

/* nameent_uso_func_00000750: empty one-argument function (a no-op virtual). */
void nameent_uso_func_00000750(int a0) {
}

/* nameent_uso_func_00000758: empty one-argument function (a no-op virtual). */
void nameent_uso_func_00000758(int a0) {
}

/* nameent_uso_func_00000760: the name-entry object's ctor. EXACT via the NCC (C++) donor
 * src/nameent_uso/nameent_uso_ncc.cpp (REPLACE_FUNC_BODY). */
int nameent_uso_func_00000760(char *self, char *owner, int mode) {
}

/* nameent_uso_func_000009BC: the name-entry display (board sprites, letters, the letter wheel).
 * NM 286/290: the NCC (C++) donor src/nameent_uso/nameent_uso_ncc.cpp is spliced into the
 * NON_MATCHING object only; the residual is the two scale spill slots (4 bytes off). */
#ifdef NON_MATCHING
void nameent_uso_func_000009BC(char *self) {
}
#else
INCLUDE_ASM("asm/nonmatchings/nameent_uso/nameent_uso", nameent_uso_func_000009BC);
#endif

/* nameent_uso_func_00000E44: the name-entry input (the letter wheel). EXACT via the NCC (C++)
 * donor src/nameent_uso/nameent_uso_ncc.cpp (REPLACE_FUNC_BODY). */
void nameent_uso_func_00000E44(char *self) {
}

