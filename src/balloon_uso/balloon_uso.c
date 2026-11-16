#include "common.h"
#include "uso_call.h"

extern int gl_func_00000000();
extern char D_00000000;
extern char gl_data_00000000;

typedef struct { int a, b, c; } BalloonTri3i;

/* balloon.uso: the balloon course object (an SNode subclass, 0x9C bytes) and
 * its PolyShape-based template. IDO 7.1 cfe -O2, like game_uso.
 * Relocs (scripts/uso-imports.py sites balloon): bootup Bss+0x3C740 = the
 * gdl file system object (fs_gdlReadFile / fs_gdlWriteFile), Bss+0x3C768 =
 * the hostio object, Bss+0x3C168 = GfxLib. Object fields: 0x88 Vec3 bob
 * amplitude (hostio "x/y/z" +-500), 0x94 float bob speed, 0x98 float phase. */

/* balloon_uso_func_00000000: write a Vec3 to the gdl file (the cross-USO
 * template, same body as game_libs fs_gdlWriteVec3_00037D48). */
void balloon_uso_func_00000000(int *a0) {
    volatile int padA[1];
    float fbuf[3];
    volatile int padB[2];
    float tmp[3];
    volatile int padC[3];
    *(BalloonTri3i *)tmp = *(BalloonTri3i *)a0;
    fbuf[0] = tmp[0]; fbuf[1] = tmp[1]; fbuf[2] = tmp[2];
    CALL(fs_gdlWriteFile)(&D_00000000, fbuf, 12);
    (void)padA;
    (void)padB;
    (void)padC;
}

/* balloon_uso_func_00000060: write a float to the gdl file. */
void balloon_uso_func_00000060(float *a0) {
    float pad, scratch;
    scratch = *a0;
    CALL(fs_gdlWriteFile)(&D_00000000, &scratch, 4);
}

/* balloon_uso_func_00000094: read a Vec3 from the gdl file (game_uso
 * stream_readVec3_000001D4 template). */
void balloon_uso_func_00000094(float *dst) {
    int pad_top[1];
    BalloonTri3i raw;
    int pad_mid[2];
    BalloonTri3i tmp;
    int pad_bot[2];
    CALL(fs_gdlReadFile)(&D_00000000, &raw, 12);
    tmp = raw;
    dst[0] = *(float *)&tmp.a;
    dst[1] = *(float *)&tmp.b;
    dst[2] = *(float *)&tmp.c;
}

/* balloon_uso_func_00000104: read a float from the gdl file. */
void balloon_uso_func_00000104(float *dst) {
    float buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

/* balloon_uso_func_00000140: Balloon ctor (0x9C bytes; cfront this==0 alloc). */
int *balloon_uso_func_00000140(char *self) {
    if (self != 0 || (self = (char *)CALL(mem_new)(0x9C)) != 0) {
        CALL(SNode_ctor)(self);
        *(char **)(self + 0x28) = &D_00000000; /* Data+0x4128: Balloon vtable */
        *(float *)(self + 0x88) = 0.0f;
        *(float *)(self + 0x8C) = 0.0f;
        *(float *)(self + 0x90) = 0.0f;
        *(float *)(self + 0x94) = *(float *)(&gl_data_00000000 + 0x1C); /* RoData+0x1C (a separate zero symbol: RoData base) */
        *(float *)(self + 0x98) = 0.0f;
    }
    return (int *)self;
}

INCLUDE_ASM("asm/nonmatchings/balloon_uso/balloon_uso", balloon_uso_func_000001A8);

/* balloon_uso_func_000002BC: saveData: bob amplitude, bob speed, then the SNode part. */
void balloon_uso_func_000002BC(char *self) {
    balloon_uso_func_00000000((int *)(self + 0x88));
    balloon_uso_func_00000060((float *)(self + 0x94));
    CALL(SNode_saveData)(self);
}

/* balloon_uso_func_000002F4: loadData (mirror of saveData). */
void balloon_uso_func_000002F4(char *self) {
    balloon_uso_func_00000094((float *)(self + 0x88));
    balloon_uso_func_00000104((float *)(self + 0x94));
    CALL(SNode_loadData)(self);
}

/* balloon_uso_func_0000032C: hostioParms (group named by the node's name). */
extern void gl_func_00000000_ff(void *, char *, void *, float, float, int);
void balloon_uso_func_0000032C(char *self) {
    char *name = *(char **)(self + 0xC); /* Node name */
#define h (&D_00000000)
    CALL(hostio_beginGroup)(h, name, 0);
    XCALL(gl_func_00000000_ff, hostio_addFloat)(h, &D_00000000 + 0x4188, self + 0x94, 0.0f, 1.0f, 1);
    XCALL(gl_func_00000000_ff, hostio_addFloat)(h, &D_00000000 + 0x4190, self + 0x88, -500.0f, 500.0f, 1);
    XCALL(gl_func_00000000_ff, hostio_addFloat)(h, &D_00000000 + 0x4198, self + 0x8C, -500.0f, 500.0f, 1);
    XCALL(gl_func_00000000_ff, hostio_addFloat)(h, &D_00000000 + 0x41A0, self + 0x90, -500.0f, 500.0f, 1);
    CALL(hostio_endGroup)(h);
    CALL(SNode_hostioParms)(self);
}
#undef h

/* balloon_uso_func_0000042C: handleMsg: messages 7 and 19 reset the bob phase. */
void balloon_uso_func_0000042C(char *self, int *msg) {
    CALL(SNode_handleMsg)(self, msg);
    if (*msg == 7 || *msg == 19) {
        *(float *)(self + 0x98) = 0.0f;
    }
}

INCLUDE_ASM("asm/nonmatchings/balloon_uso/balloon_uso", balloon_uso_func_0000047C);
