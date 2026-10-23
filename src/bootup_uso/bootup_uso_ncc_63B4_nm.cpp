/* IDO 7.1 NCC (C++) NM-only comparison donor for func_000063B4: the NM C body lifted under NCC + one parameter empty test from a fz sweep (agent-c run 134, cfe 76.09).
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
#undef NULL
#define NULL 0
typedef struct { float x, y, z; } Vec3;
typedef struct { float q[4]; float pad[3]; Vec3 B; Vec3 A; } Bu63b4F;
#define BU63B4_MC(c) ((float (*)[4])(*(char **)((c) + 0x70) + 0xB4))
#define BU63B4_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define BU63B4_MAX(a, b) (((b) < (a)) ? (a) : (b))
#define BU63B4_XFORM(m, v)                                                   \
    {                                                                          \
        x_ = (v).x; y_ = (v).y; z_ = (v).z;                                    \
        rx_ = (m)[0][0] * x_ + (m)[1][0] * y_ + (m)[2][0] * z_;               \
        ry_ = (m)[0][1] * x_ + (m)[1][1] * y_ + (m)[2][1] * z_;               \
        rz_ = (m)[0][2] * x_ + (m)[1][2] * y_ + (m)[2][2] * z_;               \
        (v).x = rx_;                                                           \
        (v).y = ry_;                                                           \
        (v).z = rz_;                                                           \
    }
extern "C" {
extern char D_00000000;
extern void func_00000000_63b4_fpp(float, float *, float *);
extern void func_477c_reg(void *, void *, void *, float, float, int);
extern f32 func_00000000_43d4_f(void);
extern u8 bu_63b4_c57c[];
extern float func_00000000_63b4_ff(float);
extern void func_00000000_63b4_fpp(float, float *, float *);
extern void func_00000000_63b4_pq(void *, Bu63b4F *);
extern void func_00000000_63b4_p(void *);
}
extern "C" void func_000063B4(char *obj) {
    Bu63b4F fr;
    float inv;
    float fn, t;
    int ix, iy;
    float (*m)[4];
    char *ctx;
    char *v1;
    float x_, y_, z_, rx_, ry_, rz_;
    char *base;

    inv = func_00000000_63b4_ff((float)*(int *)(obj + 0xBC) / *(float *)(bu_63b4_c57c + 0x34));
    fr.A.y = 0.0f;
    fr.A.x = 0.0f;
    fr.A.z = (float)(-*(int *)(obj + 0xB8)) / inv;
    func_00000000_63b4_fpp((float)*(int *)(obj + 0xBC), &fr.A.x, &fr.A.z);
    base = (char *)&D_00000000;
    ctx = *(char **)(base + 0x254);
    fr.B.y = 0.0f;
    fr.B.x = -fr.A.x;
    fr.B.z = fr.A.z;
    m = BU63B4_MC(ctx); if (obj) {}
    BU63B4_XFORM(m, fr.A);
    m = BU63B4_MC(ctx);
    ctx = *(char **)(base + 0x254);
    BU63B4_XFORM(m, fr.B);
    v1 = *(char **)(*(char **)(obj + 0xB4) + 0xA0);
    fn = (float)(int)((*(float *)(*(char **)(ctx + 0x70) + 0xA8) * *(float *)(base + 0x130) -
                       (float)*(int *)(v1 + 0xC)) / *(float *)(v1 + 0x14));
    fr.A.z += fn;
    fr.B.z += fn;
    t = BU63B4_MIN(fr.A.z, fn);
    ix = (int)t;
    t = BU63B4_MIN((float)ix, fr.B.z);
    ix = (int)t;
    t = BU63B4_MAX(fr.A.z, fn);
    iy = (int)t;
    t = BU63B4_MAX((float)iy, fr.B.z);
    iy = (int)t;
    ix -= *(int *)(obj + 0xC0);
    iy += *(int *)(obj + 0xC0);
    if (ix < 0) ix = 0;
    if (ix >= 0x80) ix = 0x7F;
    if (iy < 0) iy = 0;
    if (iy >= 0x80) iy = 0x7F;
    *(int *)(ctx + 0x10C) = ix;
    *(int *)(*(char **)(base + 0x254) + 0x110) = iy;
    fr.q[2] = 0.0f;
    fr.q[1] = 0.0f;
    fr.q[0] = 0.0f;
    fr.q[3] = 1.0f;
    func_00000000_63b4_pq(base, &fr);
    func_00000000_63b4_p(obj);
}
