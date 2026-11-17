/* MIPSpro 7.2.1 C++ (o32 edgcpfe, EDG 2.29) EXACT donor for EraseOptions_drawGui (the erase screen
 * draw, 427 words), built by tools/ncc721.sh and spliced over the placeholder in viewrec_uso.c on both
 * build paths (agent-b run 675 body; agent-a run 743 made it exact). Its .rodata (four 0.1f literals at
 * +0x0..+0xC, the 5-entry switch table at +0x10) is viewrec RoData +0x0 (in-place %lo); the
 * splice-renamed reloc EraseOptions_drawGui_rodata is pinned to 0 in undefined_syms_auto.txt.
 * The last word: the target keeps a nop in the c.lt.s slot of the `selFade > 0` grid test, because as1
 * sees s5 / s6 / s7 (r and the constants 1 / 2) live at the switch's `jr`. as1 makes every text
 * address named by a data word before the function's `.end` a target of each jump-table `jr` in the
 * function, and this function's own address is one (viewrec Data+0x184, the erase screen's draw slot in
 * its vtable at Data+0x118). With that edge to the entry the callee-saved registers are live at the
 * jr, so as1 may no longer speculate the grid's `r = 0` / `li 2` / `li 1` above the bc1fl. The data word
 * below stands in for that vtable slot (the real table is in the viewrec data asset).
 * See docs/1080_NEW_MODULES.md (agent-b runs 662 / 670, agent-a runs 733 / 738 / 743). */
#include "../../include/uso_call.h"  /* donors build without -I include */
extern "C" int gl_func_00000000(...);
extern "C" char D_00000000;
struct VrSpr { char b[24]; };
extern "C" VrSpr sVrLabels[68];
extern "C" float sVrPrimColour[4];
extern "C" float sVrEnvColour[4];
struct VrProcE { char pad[0x50]; int *session; char pad54[0x6C - 0x54]; int mode; int set; int flags; };
struct VrErase {
    char node[0x50];
    VrProcE *proc;     /* 0x50 */
    char pad54[0x6C - 0x54];
    int frame;         /* 0x6C */
    int no;            /* 0x70 */
    int gridX;         /* 0x74 */
    int gridY;         /* 0x78 */
    int gridX0;        /* 0x7C */
    int gridY0;        /* 0x80 */
    int setDX;         /* 0x84 */
    int setDY;         /* 0x88 */
    int setX0;         /* 0x8C */
    int setY0;         /* 0x90 */
    int rowY;          /* 0x94 */
    int askY;          /* 0x98 */
    int yesY;          /* 0x9C */
    int phase;         /* 0xA0 */
    int set;           /* 0xA4 */
    int course;        /* 0xA8 */
    int minCourse;     /* 0xAC */
    float selFade;     /* 0xB0 */
    float askFade;     /* 0xB4 */
    int gCourse, gRider, gBoard, gMode; /* 0xB8 */
};
extern "C" VrSpr sVrLabels_3F0[8];
extern "C" void EraseOptions_drawGui(VrErase *self);
/* the erase screen vtable's draw slot (viewrec Data+0x184): emitted before this body, see above */
extern "C" void *vr_eraseDrawSlot_3878 = (void *)EraseOptions_drawGui;
extern "C" void EraseOptions_drawGui(VrErase *self) {
    float col[4];
    int blink;
    volatile int unused[6];
    int i;
    int j;
    int r;
    int idx;
    int rowIdx;
    int a;
    VrSpr *spr;
    VrSpr *row;
    VrSpr *g;

    col[0] = 1.0f;
    col[1] = 1.0f;
    col[2] = 1.0f;
    col[3] = 1.0f;
    self->frame++;
    blink = (-self->frame << 4) & 0xFF;
    CALL(vr_drawBackground)(self->proc);
    CALL(vr_drawFooterLabels)(self->proc);
    if (self->proc->mode == 0x80000) {
        if (self->askFade > 0.0f) {
            self->askFade -= 0.1f;
            if (self->askFade < 0.0f) {
                self->askFade = 0.0f;
            }
        }
        if (self->askFade < 0.25f && self->selFade < 1.0f) {
            self->selFade += 0.1f;
            if (self->selFade > 1.0f) {
                self->selFade = 1.0f;
            }
        }
    } else {
        if (self->selFade > 0.0f) {
            self->selFade -= 0.1f;
            if (self->selFade < 0.0f) {
                self->selFade = 0.0f;
            }
        }
        if (self->selFade < 0.25f && self->askFade < 1.0f) {
            self->askFade += 0.1f;
            if (self->askFade > 1.0f) {
                self->askFade = 1.0f;
            }
        }
    }
    for (i = 0; i < 5; i++) {
        CALL(gui_spriteRefresh)(&sVrLabels[29 + i]);
        if (i == self->set && self->proc->mode == 0x80000) {
            CALL(gui_setColorEnvPrim_000017DC)(&sVrLabels[29 + i], (int)(blink * self->selFade), col, 255);
        } else {
            CALL(gui_setColorEnvPrim_000017DC)(&sVrLabels[29 + i], (int)(64.0f * self->selFade), col, 255);
        }
        CALL(gui_drawSprite)(&sVrLabels[29 + i], self->setX0 + i * self->setDX, self->setY0 + (i & 1) * self->setDY, 3);
    }
    CALL(gui_setEnvPrimColor_00001EF4)(&D_00000000, 255, sVrPrimColour, sVrEnvColour);
    CALL(gui_spriteRefresh)(&sVrLabels[43]);
    CALL(gui_drawSprite)(&sVrLabels[43], 160, 37, 3);
    if (self->selFade > 0.0f) {
        for (r = 0; r < 4; r++) {
            for (j = 0; j < 2; j++) {
                CALL(gui_spriteRefresh)(&sVrLabels_3F0[r * 2 + j]);
                a = 255;
                if (r * 2 + j < self->minCourse || self->set >= 2) {
                    a = 64;
                }
                if (r * 2 + j == self->course && self->phase == 1) {
                    a = blink;
                }
                CALL(gui_setColorEnvPrim_000017DC)(&sVrLabels_3F0[r * 2 + j], (int)(a * self->selFade), col, 255);
                CALL(gui_drawSprite)(&sVrLabels_3F0[r * 2 + j], self->gridX0 + j * self->gridX, self->gridY0 + r * self->gridY, 3);
            }
        }
    }
    if (self->selFade < 1.0f) {
        CALL(gui_setColor_00001A5C)(&D_00000000, (int)(255.0f * self->askFade), col);
        switch (self->set) {
            case 0:
            case 1:
                CALL(vr_drawCenteredLabel)(self->proc, self->course, self->rowY);
                break;
            case 2:
                CALL(vr_drawCenteredLabel)(self->proc, 10, self->rowY - 8);
                break;
            case 3:
                CALL(vr_drawGhostRow)(self->proc, self->rowY - 18, self->gCourse, self->gRider, self->gBoard, self->gMode);
                break;
            case 4:
                CALL(vr_drawCenteredLabel)(self->proc, 9, self->rowY - 8);
                break;
        }
        CALL(gui_spriteRefresh)(&sVrLabels[58]);
        CALL(gui_drawSprite)(&sVrLabels[58], 160, self->askY, 3);
        CALL(gui_spriteRefresh)(&sVrLabels[59]);
        if (self->no == 0) {
            CALL(gui_setColorEnvPrim_000017DC)(&sVrLabels[59], (int)(blink * self->askFade), col, 255);
        } else {
            CALL(gui_setColorEnvPrim_000017DC)(&sVrLabels[59], (int)(64.0f * self->askFade), col, 255);
        }
        CALL(gui_drawSprite)(&sVrLabels[59], 120, self->yesY, 3);
        CALL(gui_spriteRefresh)(&sVrLabels[60]);
        if (self->no != 0) {
            CALL(gui_setColorEnvPrim_000017DC)(&sVrLabels[60], (int)(blink * self->askFade), col, 255);
        } else {
            CALL(gui_setColorEnvPrim_000017DC)(&sVrLabels[60], (int)(64.0f * self->askFade), col, 255);
        }
        CALL(gui_drawSprite)(&sVrLabels[60], 200, self->yesY, 3);
    }
}
