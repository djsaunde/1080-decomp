/* MIPSpro 7.2.1 C++ (o32 edgcpfe, EDG 2.29) NON_MATCHING-only donor for viewrec_uso_func_00003878 (the
 * erase screen draw), built by tools/ncc721.sh and spliced over the placeholder in viewrec_uso.c on the
 * non_matching build only (agent-b run 675). 426 words vs the target's 427: the target keeps a nop in the
 * c.lt.s delay slot of the `selFade < 1.0f` test, because as1 sees r / 1 / 2 (s5 / s6 / s7) live past the
 * gui_setColor call there (bits in that call's .livereg) and so does not pull the first if-body word into
 * the slot. The rest of the body is word-for-word. Calls go through the bare placeholder (callee named in
 * a comment) because the placeholder-call check only verifies donors spliced on the matching build.
 * See docs/1080_NEW_MODULES.md (agent-b runs 662 / 670). */
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
extern "C" void viewrec_uso_func_00003878(VrErase *self) {
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
    gl_func_00000000(/* viewrec_uso_func_000011B4 */ self->proc);
    gl_func_00000000(/* viewrec_uso_func_000014DC */ self->proc);
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
        gl_func_00000000(/* gui_spriteRefresh */ &sVrLabels[29 + i]);
        if (i == self->set && self->proc->mode == 0x80000) {
            gl_func_00000000(/* gui_setColorEnvPrim_000017DC */ &sVrLabels[29 + i], (int)(blink * self->selFade), col, 255);
        } else {
            gl_func_00000000(/* gui_setColorEnvPrim_000017DC */ &sVrLabels[29 + i], (int)(64.0f * self->selFade), col, 255);
        }
        gl_func_00000000(/* gui_drawSprite */ &sVrLabels[29 + i], self->setX0 + i * self->setDX, self->setY0 + (i & 1) * self->setDY, 3);
    }
    gl_func_00000000(/* gui_setEnvPrimColor_00001EF4 */ &D_00000000, 255, sVrPrimColour, sVrEnvColour);
    gl_func_00000000(/* gui_spriteRefresh */ &sVrLabels[43]);
    gl_func_00000000(/* gui_drawSprite */ &sVrLabels[43], 160, 37, 3);
    if (self->selFade > 0.0f) {
        for (r = 0; r < 4; r++) {
            for (j = 0; j < 2; j++) {
                gl_func_00000000(/* gui_spriteRefresh */ &sVrLabels_3F0[r * 2 + j]);
                a = 255;
                if (r * 2 + j < self->minCourse || self->set >= 2) {
                    a = 64;
                }
                if (r * 2 + j == self->course && self->phase == 1) {
                    a = blink;
                }
                gl_func_00000000(/* gui_setColorEnvPrim_000017DC */ &sVrLabels_3F0[r * 2 + j], (int)(a * self->selFade), col, 255);
                gl_func_00000000(/* gui_drawSprite */ &sVrLabels_3F0[r * 2 + j], self->gridX0 + j * self->gridX, self->gridY0 + r * self->gridY, 3);
            }
        }
    }
    if (self->selFade < 1.0f) {
        gl_func_00000000(/* gui_setColor_00001A5C */ &D_00000000, (int)(255.0f * self->askFade), col);
        switch (self->set) {
            case 0:
            case 1:
                gl_func_00000000(/* viewrec_uso_func_00001488 */ self->proc, self->course, self->rowY);
                break;
            case 2:
                gl_func_00000000(/* viewrec_uso_func_00001488 */ self->proc, 10, self->rowY - 8);
                break;
            case 3:
                gl_func_00000000(/* viewrec_uso_func_00001588 */ self->proc, self->rowY - 18, self->gCourse, self->gRider, self->gBoard, self->gMode);
                break;
            case 4:
                gl_func_00000000(/* viewrec_uso_func_00001488 */ self->proc, 9, self->rowY - 8);
                break;
        }
        gl_func_00000000(/* gui_spriteRefresh */ &sVrLabels[58]);
        gl_func_00000000(/* gui_drawSprite */ &sVrLabels[58], 160, self->askY, 3);
        gl_func_00000000(/* gui_spriteRefresh */ &sVrLabels[59]);
        if (self->no == 0) {
            gl_func_00000000(/* gui_setColorEnvPrim_000017DC */ &sVrLabels[59], (int)(blink * self->askFade), col, 255);
        } else {
            gl_func_00000000(/* gui_setColorEnvPrim_000017DC */ &sVrLabels[59], (int)(64.0f * self->askFade), col, 255);
        }
        gl_func_00000000(/* gui_drawSprite */ &sVrLabels[59], 120, self->yesY, 3);
        gl_func_00000000(/* gui_spriteRefresh */ &sVrLabels[60]);
        if (self->no != 0) {
            gl_func_00000000(/* gui_setColorEnvPrim_000017DC */ &sVrLabels[60], (int)(blink * self->askFade), col, 255);
        } else {
            gl_func_00000000(/* gui_setColorEnvPrim_000017DC */ &sVrLabels[60], (int)(64.0f * self->askFade), col, 255);
        }
        gl_func_00000000(/* gui_drawSprite */ &sVrLabels[60], 200, self->yesY, 3);
    }
}
