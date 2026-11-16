#include "common.h"
#include "uso_call.h"

extern int gl_func_00000000();
extern char D_00000000;

typedef struct { int a, b, c, d; } Quad4;
typedef struct { int a, b, c; } Tri3i;
typedef struct { float x, y, z; } Vec3;

float snoweffects_uso_func_00000000(Vec3 *a, Vec3 *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

float snoweffects_uso_func_00000030(float *p, float *pl) {
    return -(p[1] * pl[1] + p[2] * pl[2] - pl[3]) / pl[0];
}

float snoweffects_uso_func_00000064(float *p, float *pl) {
    return -(p[0] * pl[0] + p[2] * pl[2] - pl[3]) / pl[1];
}

float snoweffects_uso_func_00000098(float *p, float *pl) {
    return -(p[0] * pl[0] + p[1] * pl[1] - pl[3]) / pl[2];
}

void snoweffects_uso_func_000000CC(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void snoweffects_uso_func_00000108(float *dst) {
    float buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void snoweffects_uso_func_00000144(Quad4 *dst) {
    Quad4 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 16);
    *dst = buf;
}

void snoweffects_uso_func_0000019C(Vec3 *dst) {
    int pad_top[1];
    Tri3i raw;
    int pad_mid[2];
    Tri3i tmp;
    int pad_bot[2];
    CALL(fs_gdlReadFile)(&D_00000000, &raw, 12);
    tmp = raw;
    dst->x = *(float*)&tmp.a;
    dst->y = *(float*)&tmp.b;
    dst->z = *(float*)&tmp.c;
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000020C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00000270);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00000680);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000006D0);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00000718);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000097C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00000A7C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00000B44);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00000C90);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000015D0);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00001A18);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00001E60);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00002310);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000027A8);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000028E0);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00003544);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000035B0);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00003620);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000374C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000039C4);

void snoweffects_uso_func_00003CA4(char *a0) {
    int tag;
    snoweffects_uso_func_000000CC(&tag);
    snoweffects_uso_func_00000108((float *)(a0 + 0x10));
}

void snoweffects_uso_func_00003CD4(char *a0) {
    int tag;
    snoweffects_uso_func_000000CC(&tag);
    snoweffects_uso_func_00000144((Quad4 *)(a0 + 0x10));
}

void snoweffects_uso_func_00003D04(char *a0) {
    int tag;
    snoweffects_uso_func_000000CC(&tag);
    snoweffects_uso_func_000000CC((int *)(a0 + 0x10));
}

void snoweffects_uso_func_00003D34(char *a0) {
    int tag;
    snoweffects_uso_func_000000CC(&tag);
    snoweffects_uso_func_0000019C((Vec3 *)(a0 + 0x10));
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00003D70);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00003DAC);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00003DE8);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00003E40);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00003EB0);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00003EE4);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000040B4);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00004188);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00004374);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000046BC);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00004714);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000047D4);

void snoweffects_uso_func_00004E4C(char *a0) {
    *(float *)(a0 + 0x50) = 1.0f;
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00004E5C);

void snoweffects_uso_func_000052CC(int a0) {
    (void)&a0;
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000052D4);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000535C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000053E8);

void snoweffects_uso_func_00005440(int a0, int a1) {
    (void)&a0;
    (void)&a1;
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000544C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000559C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00005FD4);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00006144);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00006248);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000634C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000063B4);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00006420);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000064B8);

void snoweffects_uso_func_00006B08(char *a0) {
    *(int *)(a0 + 0x1C8) = 0;
    *(int *)(a0 + 0x54) |= 2;
    *(int *)(a0 + 0x1CC) = 0;
    *(float *)(a0 + 0x1D8) = 0.0f;
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00006B28);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000070C0);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000718C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000078E8);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00007960);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00007CCC);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00007CFC);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00007D2C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00007D5C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00007D8C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00007E20);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00007E5C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00007E98);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00007EF0);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00007F60);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00007FE0);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00008C60);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00008D70);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009010);

void snoweffects_uso_func_000090F0(int a0, int a1) {
    (void)&a0;
    (void)&a1;
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000090FC);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000912C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000915C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000918C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000091BC);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009250);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000928C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000092C8);

void snoweffects_uso_func_00009320(void) {
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009328);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009704);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000973C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009768);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009884);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009BE8);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009C48);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009E10);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009E88);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009F44);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009FE8);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000A10C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000A760);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000A870);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000A8A0);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000A8D0);

