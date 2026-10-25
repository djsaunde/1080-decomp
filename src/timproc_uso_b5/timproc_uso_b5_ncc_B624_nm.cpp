/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for timproc_uso_b5_func_0000B624
 * (139 words = target). NM object only (build/non_matching); the ROM build keeps
 * INCLUDE_ASM. Run 166 (kit CVII): rewritten on the exact AC20 donor's recipe (its sibling):
 * the reference point as a file-local x / y pair + z read through rel()'s pointer parameter
 * (the `if (0)` keeps the a / g parameter temporaries: v0 = &o->pos and the frame homes),
 * every quaternion rotation as o->q.rot() (edg's pooled `this` temporary at sp+40), the angle
 * blocks nested as in AC20: 96.10 -> 99.86 standalone, frame 152 = target. Then the x / y
 * differences named with a dead `while (0) { dy = 0.0f; }` anchor ahead of them (uopt colours
 * unconstrained LRs in first-appearance order: dy first takes f0, dx f2 as in the target):
 * 99.92. RESIDUAL: the two named differences add two frame homes (frame 160 vs 152); every
 * home-free spelling of the anchor (expression anchors, register, block-scoped) keeps the
 * x-first colours. An exact landing would also pin the
 * timproc_uso_b5_func_0000B624_bss relocs to 0. History: runs 142-163 (cfe-lift NCC body,
 * relPos helper, 88.57 -> 96.10). */
typedef float f32; typedef int s32; typedef double f64;
#define FW(p, o) (*(s32 *)((char *)(p) + (o)))
#define FP(p, o) (*(char **)((char *)(p) + (o)))
extern "C" {
extern char timproc_uso_b5_D_807FF370;
extern char timproc_uso_b5_D_807FFBB0;
extern int timproc_uso_b5_D_807FFBBC;
void *timproc_uso_b5_func_071028(...);
void timproc_uso_b5_func_071AE0(void *quat, void *axis, f32 angle);
void timproc_uso_b5_func_054CAC(void *obj);
int timproc_uso_b5_func_054FE0(...);
int timproc_uso_b5_func_077C44(...);
void timproc_uso_b5_func_06970C_i(...);
}
static float sXY[2]; static float sZ;
struct V3 { f32 x, y, z; V3() {} V3(f32 a, f32 b, f32 c) { x = a; y = b; z = c; } void operator=(V3 o) { x = o.x; y = o.y; z = o.z; } };
inline f32 dot(const V3 &a, const V3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline V3 rel(const V3 &a, f32 *g) { f32 dy; f32 dx; while (0) { dy = 0.0f; } dx = a.x - g[0]; dy = a.y - g[1]; return V3(dx, dy, a.z - sZ); }
struct Quat {
    f32 x, y, z, w;
    void rot(V3 *axis, f32 ang) { timproc_uso_b5_func_071AE0(this, axis, ang); }
};
struct Obj {
    char pad0[0xDC]; V3 pos; f32 sE8, sEC, sF0; Quat q; char pad104[0x108 - 0x104]; s32 i108;
    char *p10C; f32 f110, f114, f118;
};
extern "C" void timproc_uso_b5_func_0000B624(Obj *o) {
    V3 d = rel(o->pos, &sXY[0]);
    timproc_uso_b5_func_071028(&d);
    if ((f64)dot(*(V3 *)&timproc_uso_b5_D_807FFBB0, d) < *(f64 *)((char *)&timproc_uso_b5_D_807FF370 + 0x350)) {
        o->q.x = 0.0f; o->q.y = 0.0f; o->q.z = 0.0f; o->q.w = 1.0f;
        { f32 ang = o->f114; V3 a(1.0f, 0.0f, 0.0f); o->q.rot(&a, ang); }
        { f32 ang = o->f110; { V3 a(0.0f, 0.0f, 1.0f); o->q.rot(&a, ang); } ang = o->f118; { V3 a(0.0f, 1.0f, 0.0f); o->q.rot(&a, ang); } }
        timproc_uso_b5_func_06970C_i(o->i108, FW(o, 0x130));
        if (FW(o, 0x144) != 0 || FW(o, 0x138) != 0) {
            timproc_uso_b5_func_054FE0(o);
            timproc_uso_b5_D_807FFBBC += 1;
            if (FW(o, 0x144) != 0) FW(o, 0x144) -= 1;
            if (FW(o, 0x144) == 0) {
                if (FW(o->p10C, 0xB0) != 0) {
                    timproc_uso_b5_func_077C44(o->p10C, FW(o, 0x140), 0, 2, 2, 1);
                    FW(o->p10C, 0xB0) = 0;
                }
            }
        }
        timproc_uso_b5_func_054CAC(o);
        FW(o, 0x134) = 1;
        return;
    }
    FW(o, 0x134) = 0;
}
