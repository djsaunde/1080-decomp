/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for timproc_uso_b5_func_0000B368
 * (174 words, target 173). NM object only (build/non_matching); the ROM build keeps
 * INCLUDE_ASM. The cfe NM body lifted under NCC with the offset / scale vector read
 * from file-local statics (kit LX: x / y one object through references, z a second
 * one), as in the B624 donor: 175 -> 174 words, 87.47 -> 89.3 fuzzy (agent-g run 142).
 * RESIDUAL: head load order / shared-lui slot, FP colours in the rotate tail. */
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef double f64;
#define M2C_FIELD(ptr, type, off) (*(type)((char *)(ptr) + (off)))
typedef struct { f32 f0; f32 f4; f32 f8; } Vec154;
extern "C" int func_00000000(...);
extern "C" {
extern char D_00000000;
extern void timproc_uso_b5_func_00006E08(char *arg0, s32 arg1);
extern int timproc_uso_b5_func_0546DC(char *, char *, f32, f32, f32);
extern void *timproc_uso_b5_func_071028(...);
extern void timproc_uso_b5_func_071AE0(void *quat, void *axis, f32 angle);
extern void timproc_uso_b5_func_054CAC(void *obj);

extern f32 timb5_B368_gscale;
extern void timproc_uso_b5_func_06970C_r(...);
}
static f32 sXY[2];
static f32 sZ;
extern "C" void timproc_uso_b5_func_0000B368(char *arg0) {
    Vec154 c2_308;
    union { Vec154 v; s32 w[4]; } c1_292;
    volatile s32 padA[2];
    Vec154 d_272;
    volatile s32 padB[5];
    Vec154 s240;
    volatile s32 padC;
    Vec154 s224;
    Vec154 ax1;
    volatile s32 padD;
    Vec154 ax2;
    Vec154 ax3;
    volatile s32 padE[13];
    Vec154 s120;
    volatile s32 padF;
    Vec154 s104;
    Vec154 s92;
    union { Vec154 v; s32 w[4]; } c76;
    Vec154 s64;
    volatile s32 padLow[6];
    f32 ang;
    f32 *sp28;

    sp28 = (f32 *) (arg0 + 0xDC);
    do {
        { f32 &x = sXY[0]; f32 &y = sXY[1];
        d_272.f0 = sp28[0] - x;
        d_272.f4 = sp28[1] - y; }
        d_272.f8 = sp28[2] - sZ;
    } while (0);
    c1_292.v = d_272;
    c2_308 = c1_292.v;
    timproc_uso_b5_func_071028(&c2_308);
    if (M2C_FIELD(arg0, s32 *, 0x140) != 0) {
        ang = M2C_FIELD(arg0, f32 *, 0x110); if (arg0) {}
        M2C_FIELD(arg0, f32 *, 0xF4) = 0.0f;
        M2C_FIELD(arg0, f32 *, 0xF8) = 0.0f;
        M2C_FIELD(arg0, f32 *, 0xFC) = 0.0f;
        M2C_FIELD(arg0, f32 *, 0x100) = 1.0f;
        ax1.f8 = 0.0f;
        ax1.f4 = 0.0f;
        ax1.f0 = 1.0f;
        timproc_uso_b5_func_071AE0(arg0 + 0xF4, &ax1, ang);
        ang = M2C_FIELD(arg0, f32 *, 0x10C);
        ax2.f0 = 0.0f;
        ax2.f4 = 0.0f;
        ax2.f8 = 1.0f;
        timproc_uso_b5_func_071AE0(arg0 + 0xF4, &ax2, ang);
        ang = M2C_FIELD(arg0, f32 *, 0x114);
        ax3.f0 = 0.0f;
        ax3.f8 = 0.0f;
        ax3.f4 = 1.0f;
        timproc_uso_b5_func_071AE0(arg0 + 0xF4, &ax3, ang);
        if (M2C_FIELD(arg0, s32 *, 0x13C) != 0) {
            func_00000000(M2C_FIELD(*(char **) &D_00000000, char **, 0x70) + 0xB4, &s120);
            func_00000000(&s120, &s104);
            func_00000000(arg0 + 0xF4, &s104, M2C_FIELD(arg0, s32 *, 0x128), 0);
        }
        M2C_FIELD(arg0, f32 *, 0xE8) = M2C_FIELD(arg0, f32 *, 0x118);
        M2C_FIELD(arg0, f32 *, 0xEC) = M2C_FIELD(arg0, f32 *, 0x118);
        M2C_FIELD(arg0, f32 *, 0xF0) = M2C_FIELD(arg0, f32 *, 0x118); if (arg0) {}
        s92.f0 = sXY[0] * timb5_B368_gscale;
        s92.f4 = sXY[1] * timb5_B368_gscale;
        s92.f8 = sZ * timb5_B368_gscale;
        c1_292.v = s92;
        s240 = c1_292.v; if (arg0) {}
        ang = M2C_FIELD(arg0, f32 *, 0x128);
        s64.f0 = ang * s240.f0;
        s64.f4 = ang * s240.f4;
        s64.f8 = ang * s240.f8;
        c76.v = s64;
        s224 = c76.v;
        M2C_FIELD(arg0, f32 *, 0xDC) = M2C_FIELD(arg0, f32 *, 0xDC) + s224.f0;
        M2C_FIELD(arg0, f32 *, 0xE0) = M2C_FIELD(arg0, f32 *, 0xE0) + s224.f4;
        M2C_FIELD(arg0, f32 *, 0xE4) = M2C_FIELD(arg0, f32 *, 0xE4) + s224.f8;
        timproc_uso_b5_func_06970C_r(M2C_FIELD(arg0, void **, 0x108), M2C_FIELD(arg0, s32 *, 0x124));
        timproc_uso_b5_func_054CAC(arg0);
        M2C_FIELD(arg0, s32 *, 0x12C) = 1;
    }
}
