#include "common.h"
#include "uso_call.h"

extern int gl_func_00000000();
extern char D_00000000;

typedef struct { int a, b, c; } TrnTri3i;

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00000000);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00000008);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00000010);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00000194);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_000001A4);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_000001B0);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_000001E4);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00000214);

/* trnproc_uso_func_00000268: read an int from the gdl file (cross-USO template, as checkpoint
 * checkpoint_uso_func_00000034 / game_uso stream_readS32_00000280). */
void trnproc_uso_func_00000268(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

/* trnproc_uso_func_000002A4: read a float from the gdl file (template, as balloon_uso_func_00000104). */
void trnproc_uso_func_000002A4(float *dst) {
    float buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_000002E0);

/* trnproc_uso_func_00000338: read a Vec3 from the gdl file (template, as balloon_uso_func_00000094 /
 * game_uso stream_readVec3_000001D4). */
void trnproc_uso_func_00000338(float *dst) {
    int pad_top[1];
    TrnTri3i raw;
    int pad_mid[2];
    TrnTri3i tmp;
    int pad_bot[2];
    CALL(fs_gdlReadFile)(&D_00000000, &raw, 12);
    tmp = raw;
    dst[0] = *(float *)&tmp.a;
    dst[1] = *(float *)&tmp.b;
    dst[2] = *(float *)&tmp.c;
}

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_000003A8);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_000006CC);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00000858);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00000908);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00000910);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_000009A0);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_000009F0);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00000A2C);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00000B44);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00000B78);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00000DD4);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00001098);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00001228);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00001284);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_000012AC);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00001324);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_000013A0);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_000013C4);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_000016C8);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00001778);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_0000180C);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_0000188C);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00001A98);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00001B84);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00001BB4);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00001BE4);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00001C14);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00001CA4);

