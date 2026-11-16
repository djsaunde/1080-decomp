#include "common.h"
#include "uso_call.h"

extern int gl_func_00000000();
extern char D_00000000;

/* checkpoint.uso: checkpoint course objects (two SNode subclasses, a CollObj
 * cylinder gate at 0xE0 and a box gate at 0x58C, plus their templates).
 * IDO 7.1 cfe -O2, like game_uso. Relocs (scripts/uso-imports.py sites
 * checkpoint): bootup Bss+0x3C740 = the gdl file system object, Bss+0x3C768 =
 * the hostio object. */

/* checkpoint_uso_func_00000000: write an int to the gdl file (cross-USO
 * template, as game_libs fs_gdlWriteInt_0003D000). */
void checkpoint_uso_func_00000000(int *a0) {
    int pad, scratch;
    scratch = *a0;
    CALL(fs_gdlWriteFile)(&D_00000000, &scratch, 4);
}

/* checkpoint_uso_func_00000034: read an int from the gdl file (template, as
 * game_uso stream_readS32_00000280). */
void checkpoint_uso_func_00000034(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

/* checkpoint_uso_func_00000070: second copy of the int writer (one per class TU). */
void checkpoint_uso_func_00000070(int *a0) {
    int pad, scratch;
    scratch = *a0;
    CALL(fs_gdlWriteFile)(&D_00000000, &scratch, 4);
}

/* checkpoint_uso_func_000000A4: second copy of the int reader. */
void checkpoint_uso_func_000000A4(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

INCLUDE_ASM("asm/nonmatchings/checkpoint_uso/checkpoint_uso", checkpoint_uso_func_000000E0);

/* checkpoint_uso_func_000001E4: cylinder checkpoint loadData (int at 0x88). */
void checkpoint_uso_func_000001E4(char *self) {
    checkpoint_uso_func_00000034((int *)(self + 0x88));
    CALL(SNode_loadData)(self);
}

/* checkpoint_uso_func_00000210: cylinder checkpoint saveData. */
void checkpoint_uso_func_00000210(char *self) {
    checkpoint_uso_func_00000000((int *)(self + 0x88));
    CALL(SNode_saveData)(self);
}

/* checkpoint_uso_func_0000023C: cylinder checkpoint hostioParms (one int, 0..100). */
void checkpoint_uso_func_0000023C(char *self) {
    char *name = *(char **)(self + 0xC); /* Node name */
    CALL(hostio_beginGroup)(&D_00000000, name, 0);
    CALL(hostio_addInt)(&D_00000000, &D_00000000 + 0x3EE0, self + 0x88, 0, 100, 0);
    CALL(hostio_endGroup)(&D_00000000);
    CALL(SNode_hostioParms)(self);
}

INCLUDE_ASM("asm/nonmatchings/checkpoint_uso/checkpoint_uso", checkpoint_uso_func_000002AC);

INCLUDE_ASM("asm/nonmatchings/checkpoint_uso/checkpoint_uso", checkpoint_uso_func_0000045C);

INCLUDE_ASM("asm/nonmatchings/checkpoint_uso/checkpoint_uso", checkpoint_uso_func_0000058C);

/* checkpoint_uso_func_0000063C: box checkpoint loadData (int at 0xC8). */
void checkpoint_uso_func_0000063C(char *self) {
    checkpoint_uso_func_000000A4((int *)(self + 0xC8));
    CALL(SNode_loadData)(self);
}

/* checkpoint_uso_func_00000668: box checkpoint saveData. */
void checkpoint_uso_func_00000668(char *self) {
    checkpoint_uso_func_00000070((int *)(self + 0xC8));
    CALL(SNode_saveData)(self);
}

/* checkpoint_uso_func_00000694: box checkpoint handleMsg: messages 7 and 19
 * copy the Vec3 at 0x5C into 0xA0. */
void checkpoint_uso_func_00000694(char *self, int *msg) {
    CALL(SNode_handleMsg)(self, msg);
    if (*msg == 7 || *msg == 19) {
        *(float *)(self + 0xA0) = *(float *)(self + 0x5C);
        *(float *)(self + 0xA4) = *(float *)(self + 0x60);
        *(float *)(self + 0xA8) = *(float *)(self + 0x64);
    }
}

INCLUDE_ASM("asm/nonmatchings/checkpoint_uso/checkpoint_uso", checkpoint_uso_func_000006F0);

INCLUDE_ASM("asm/nonmatchings/checkpoint_uso/checkpoint_uso", checkpoint_uso_func_000007F4);

INCLUDE_ASM("asm/nonmatchings/checkpoint_uso/checkpoint_uso", checkpoint_uso_func_000008E8);

