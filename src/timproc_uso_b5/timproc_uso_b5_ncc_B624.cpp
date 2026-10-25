/* IDO 7.1 NCC (C++) donor for timproc_uso_b5_func_0000B624 (0x22C / 139 words): AC20 / B154 /
 * B368's sibling (reference-point offset normalised, facing test against D_807FFBB0, quaternion
 * rebuilt from three axis-angle rotations, 06970C, the 0x144 countdown / 077C44 tail).
 * Spliced into timproc_uso_b5.c.o (both trees) via REPLACE_FUNC_BODY; its .bss relocs are renamed
 * timproc_uso_b5_func_0000B624_bss by the splice and pinned to 0 (the USO bakes 0 / 4 / 8).
 * agent-g run 167 (docs/IDO_CODEGEN.md kit CIX):
 * - the first rotation through an inline `one(Quat &, f32 xa)` whose float parameter is kept by
 *   `if (0) { xa = 0.0f; }`: the parameter temp is a pooled float temp that reuses the head's
 *   x-difference temp (the V3 ctor's first parameter), the merged LR outranks y, and the head takes
 *   the target's colours (x f2, y f0) with no extra home (run 166 needed a named while (0) anchor
 *   costing two homes);
 * - the third angle as a named `ang3` declared at the if level (homed above the first axis vector)
 *   and the second as a block-scoped `ang` (the one home between the first and second vectors).
 * History: runs 142-166 (cfe lift 88.57 -> 96.10, NM donor 99.92). */
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
struct V3 { f32 x, y, z; V3() {} V3(f32 a, f32 b, f32 c) { x = a; y = b; z = c; } };
inline f32 dot(const V3 &a, const V3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline V3 rel(const V3 &a, f32 *g) { if (0) { g[1] = a.y; } return V3(a.x - g[0], a.y - g[1], a.z - sZ); }
struct Quat {
    f32 x, y, z, w;
    void rot(V3 *axis, f32 ang) { timproc_uso_b5_func_071AE0(this, axis, ang); }
};
inline void one(Quat &q, f32 xa) { if (0) { xa = 0.0f; } V3 a(1.0f, 0.0f, 0.0f); q.rot(&a, xa); }
struct Obj {
    char pad0[0xDC]; V3 pos; f32 sE8, sEC, sF0; Quat q; char pad104[0x108 - 0x104]; s32 i108;
    char *p10C; f32 f110, f114, f118;
};
extern "C" void timproc_uso_b5_func_0000B624(Obj *o) {
    V3 d = rel(o->pos, &sXY[0]);
    timproc_uso_b5_func_071028(&d);
    if ((f64)dot(*(V3 *)&timproc_uso_b5_D_807FFBB0, d) < *(f64 *)((char *)&timproc_uso_b5_D_807FF370 + 0x350)) {
        o->q.x = 0.0f; o->q.y = 0.0f; o->q.z = 0.0f; o->q.w = 1.0f;
        f32 ang3;
        one(o->q, o->f114);
        { f32 ang = o->f110; V3 a(0.0f, 0.0f, 1.0f); o->q.rot(&a, ang); }
        ang3 = o->f118;
        { V3 a(0.0f, 1.0f, 0.0f); o->q.rot(&a, ang3); }
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
