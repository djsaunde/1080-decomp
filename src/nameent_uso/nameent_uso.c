#include "common.h"
#include "gdl/node.h"
#include "uso_call.h"

extern int gl_func_00000000();
extern char D_00000000;

/* nameent_uso_func_00000000: empty function. */
void nameent_uso_func_00000000(void) {
}

/* nameent_uso_func_00000008: empty function. */
void nameent_uso_func_00000008(void) {
}

/* nameent_uso_func_00000010: empty function. */
void nameent_uso_func_00000010(void) {
}

INCLUDE_ASM("asm/nonmatchings/nameent_uso/nameent_uso", nameent_uso_func_00000018);

INCLUDE_ASM("asm/nonmatchings/nameent_uso/nameent_uso", nameent_uso_func_000000C8);

INCLUDE_ASM("asm/nonmatchings/nameent_uso/nameent_uso", nameent_uso_func_00000150);

INCLUDE_ASM("asm/nonmatchings/nameent_uso/nameent_uso", nameent_uso_func_00000204);

INCLUDE_ASM("asm/nonmatchings/nameent_uso/nameent_uso", nameent_uso_func_000002F4);

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

INCLUDE_ASM("asm/nonmatchings/nameent_uso/nameent_uso", nameent_uso_func_000004D0);

/* nameent_uso_func_000005AC: empty one-argument function (a no-op virtual). */
void nameent_uso_func_000005AC(int a0) {
}

INCLUDE_ASM("asm/nonmatchings/nameent_uso/nameent_uso", nameent_uso_func_000005B4);

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

INCLUDE_ASM("asm/nonmatchings/nameent_uso/nameent_uso", nameent_uso_func_00000760);

INCLUDE_ASM("asm/nonmatchings/nameent_uso/nameent_uso", nameent_uso_func_000009BC);

INCLUDE_ASM("asm/nonmatchings/nameent_uso/nameent_uso", nameent_uso_func_00000E44);

