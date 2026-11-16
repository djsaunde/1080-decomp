#include "common.h"
#include "uso_call.h"

extern int gl_func_00000000();
extern char D_00000000;

typedef struct { int a, b, c; } HeliTri3i;

/* heli_uso_func_00000000: read an int from the gdl file (cross-USO template, as checkpoint
 * checkpoint_uso_func_00000034 / game_uso stream_readS32_00000280). */
void heli_uso_func_00000000(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

/* heli_uso_func_0000003C: read a Vec3 from the gdl file (template, as balloon_uso_func_00000094 /
 * game_uso stream_readVec3_000001D4). */
void heli_uso_func_0000003C(float *dst) {
    int pad_top[1];
    HeliTri3i raw;
    int pad_mid[2];
    HeliTri3i tmp;
    int pad_bot[2];
    CALL(fs_gdlReadFile)(&D_00000000, &raw, 12);
    tmp = raw;
    dst[0] = *(float *)&tmp.a;
    dst[1] = *(float *)&tmp.b;
    dst[2] = *(float *)&tmp.c;
}

/* heli_uso_func_000000AC: read a float from the gdl file (template, as balloon_uso_func_00000104). */
void heli_uso_func_000000AC(float *dst) {
    float buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

INCLUDE_ASM("asm/nonmatchings/heli_uso/heli_uso", heli_uso_func_000000E8);

INCLUDE_ASM("asm/nonmatchings/heli_uso/heli_uso", heli_uso_func_00000140);

INCLUDE_ASM("asm/nonmatchings/heli_uso/heli_uso", heli_uso_func_000001D4);

INCLUDE_ASM("asm/nonmatchings/heli_uso/heli_uso", heli_uso_func_00000274);

INCLUDE_ASM("asm/nonmatchings/heli_uso/heli_uso", heli_uso_func_00000388);

INCLUDE_ASM("asm/nonmatchings/heli_uso/heli_uso", heli_uso_func_000003F4);

INCLUDE_ASM("asm/nonmatchings/heli_uso/heli_uso", heli_uso_func_00000458);

INCLUDE_ASM("asm/nonmatchings/heli_uso/heli_uso", heli_uso_func_00000460);

INCLUDE_ASM("asm/nonmatchings/heli_uso/heli_uso", heli_uso_func_000004C0);

INCLUDE_ASM("asm/nonmatchings/heli_uso/heli_uso", heli_uso_func_0000069C);

INCLUDE_ASM("asm/nonmatchings/heli_uso/heli_uso", heli_uso_func_000007B4);

INCLUDE_ASM("asm/nonmatchings/heli_uso/heli_uso", heli_uso_func_000008A8);

INCLUDE_ASM("asm/nonmatchings/heli_uso/heli_uso", heli_uso_func_00000980);

INCLUDE_ASM("asm/nonmatchings/heli_uso/heli_uso", heli_uso_func_00000A70);

INCLUDE_ASM("asm/nonmatchings/heli_uso/heli_uso", heli_uso_func_00000B08);

INCLUDE_ASM("asm/nonmatchings/heli_uso/heli_uso", heli_uso_func_000019C4);

INCLUDE_ASM("asm/nonmatchings/heli_uso/heli_uso", heli_uso_func_000019CC);

INCLUDE_ASM("asm/nonmatchings/heli_uso/heli_uso", heli_uso_func_000019E0);

INCLUDE_ASM("asm/nonmatchings/heli_uso/heli_uso", heli_uso_func_00001A0C);

INCLUDE_ASM("asm/nonmatchings/heli_uso/heli_uso", heli_uso_func_00001A3C);

INCLUDE_ASM("asm/nonmatchings/heli_uso/heli_uso", heli_uso_func_00001A6C);

INCLUDE_ASM("asm/nonmatchings/heli_uso/heli_uso", heli_uso_func_00001A9C);

INCLUDE_ASM("asm/nonmatchings/heli_uso/heli_uso", heli_uso_func_00001ACC);

