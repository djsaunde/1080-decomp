/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for timproc_uso_b5_func_0000396C
 * (47 words; this body 46, the same length as the cfe NM body). NM object only
 * (build/non_matching); the ROM build keeps INCLUDE_ASM. The cfe body recompiled
 * unchanged under NCC: 41.8 -> 60.2 fuzzy.
 * RESIDUAL: the target loads all three arg2 components before the first store
 * to o (one extra word), and colours tt f18 / x f16. */
typedef int s32;
typedef float f32;
#define FW(p, o) (*(int *)((char *)(p) + (o)))
extern "C" {
}
extern "C" void timproc_uso_b5_func_0000396C(char *arg0, s32 arg1, char *arg2, f32 arg3, f32 arg4) {
    volatile f32 pad0;
    volatile f32 y;
    volatile f32 pad1;
    volatile f32 pad2;
    volatile f32 pad3;
    volatile f32 pad4;
    volatile f32 pad5;
    volatile f32 t;
    f32 x;
    f32 yv;
    f32 z;
    f32 tv;
    f32 tt;
    f32 h;
    char *B;
    char *o;
    f32 cz;

    h = arg3;
    tt = *(f32 *)((char *)arg0 + 0x2A0);
    B = (char *) FW(arg0, 0x29C);
    x = (*(f32 *)((char *)arg0 + 0x184) * tt) + *(f32 *)((char *)B + 0xDC);
    y = yv = ((*(f32 *)((char *)arg0 + 0x188) * tt) + *(f32 *)((char *)B + 0xE0)) + (arg4 * (1.0f - tt));
    z = (*(f32 *)((char *)arg0 + 0x18C) * tt) + *(f32 *)((char *)B + 0xE4);
    o = (char *) arg1 + 0x30;
    if (1) {
        t = *(f32 *)((char *)arg2 + 0x0);
        tv = t;
        *(f32 *)((char *)o + 0x30) = tv + ((x - tv) * h);
        t = *(f32 *)((char *)arg2 + 0x4);
        tv = t;
        *(f32 *)((char *)o + 0x34) = tv + ((yv - tv) * h);
        cz = *(f32 *)((char *)arg2 + 0x8);
        *(f32 *)((char *)o + 0x38) = cz + ((z - cz) * h);
    }
}
