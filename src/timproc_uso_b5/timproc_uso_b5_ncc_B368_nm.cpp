/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for timproc_uso_b5_func_0000B368
 * (173 words = target). NM object only (build/non_matching); the ROM build keeps INCLUDE_ASM.
 * Run 166 (kit CVII): rewritten on the exact AC20 donor's recipe (its sibling): rel() /
 * pool() read the file-local x / y pair through a pointer parameter (the `if (0)` keeps
 * the parameter temporaries, so the pair loads go through `lui at` like the target, not a
 * CSE'd v0 base), every rotation as o->q.rot() (pooled `this` temporary at sp+36), the
 * angle blocks nested as in AC20, and the two scaled vectors COPY-INITIALISED at their use
 * (`V3 s240 = pool(...)`: result -> shared temp -> s240 as word copies, no float element
 * tail), the 0727A4 / 0806C buffers scoped to the 0x13C block: 95.71 -> 99.16 standalone,
 * every instruction but the sp offsets.
 * Then: V3 WITHOUT a user operator= and the two scaled vectors in ONE expression
 * `s224 = (s240 = pool(...)) * o->f128` (implicit memberwise copies are the target's word
 * copies; the pool result's return temp is still in use, so the scale gets the target's own
 * second return temp at sp+76 and its `addiu v0,sp,76`): 692 bytes = target, 99.73; an
 * arm-level `f32 ang` between s240 and s224, rel()'s z through a third pointer parameter and
 * the 0727A4 buffer as 15 words: 99.76, every instruction equal, frame 304 vs 320.
 * RESIDUAL: homes only -- the rel() head temp one word low, one more word between a3 and m7C,
 * one between the pool temp and the second return temp, two more at the bottom.
 * History: runs 142-157 (cfe lift, relPos helpers, 87.47 -> 95.71). */
typedef float f32; typedef int s32; typedef double f64;
#define FW(p, o) (*(s32 *)((char *)(p) + (o)))
extern "C" {
extern char import_80087DA8;
void *timproc_uso_b5_func_071028(...);
void timproc_uso_b5_func_071AE0(void *quat, void *axis, f32 angle);
void timproc_uso_b5_func_0727A4(void *src, void *dst);
void import_0010806C(void *src, void *dst);
void timproc_uso_b5_func_0714D4(void *quat, void *vec, f32 scale, s32 flag);
void timproc_uso_b5_func_06970C(void *obj, s32 val);
void timproc_uso_b5_func_054CAC(void *obj);
extern f32 timb5_B368_gscale;
}
static float sXY[2]; static float sZ;
struct V3 { f32 x, y, z; V3() {} V3(f32 a, f32 b, f32 c) { x = a; y = b; z = c; }
    void operator+=(const V3 &v) { x += v.x; y += v.y; z += v.z; } };
inline V3 rel(const V3 &a, f32 *g, f32 *z) { if (0) { g[0] = a.x; } return V3(a.x - g[0], a.y - g[1], a.z - *z); }
inline V3 pool(f32 *g, const f32 &s) { if (0) { g[0] = s; } return V3(g[0] * s, g[1] * s, sZ * s); }
inline V3 operator*(const V3 &a, const f32 &s) { return V3(a.x * s, a.y * s, a.z * s); }
struct Quat {
    f32 x, y, z, w;
    void rot(V3 *axis, f32 ang) { timproc_uso_b5_func_071AE0(this, axis, ang); }
};
struct Obj {
    char pad0[0xDC]; V3 pos; f32 sE8, sEC, sF0; Quat q; char pad104[0x108 - 0x104]; void *m108;
    f32 f10C, f110, f114, f118; char p11C[0x124 - 0x11C]; s32 i124; f32 f128; s32 i12C;
};
extern "C" void timproc_uso_b5_func_0000B368(Obj *o) {
    V3 d = rel(o->pos, &sXY[0], &sZ);
    timproc_uso_b5_func_071028(&d);
    if (FW(o, 0x140) != 0) {
        V3 s240;
        f32 ang;
        V3 s224;
        o->q.x = 0.0f; o->q.y = 0.0f; o->q.z = 0.0f; o->q.w = 1.0f;
        { ang = o->f110; V3 a(1.0f, 0.0f, 0.0f); o->q.rot(&a, ang); }
        { f32 ang = o->f10C; { V3 a(0.0f, 0.0f, 1.0f); o->q.rot(&a, ang); } ang = o->f114; { V3 a(0.0f, 1.0f, 0.0f); o->q.rot(&a, ang); } }
        if (FW(o, 0x13C) != 0) {
            s32 m7C[15];
            s32 m6C[4];
            timproc_uso_b5_func_0727A4(*(char **)((char *)(*(char **)&import_80087DA8) + 0x70) + 0xB4, m7C);
            import_0010806C(m7C, m6C);
            timproc_uso_b5_func_0714D4(&o->q, m6C, o->f128, 0);
        }
        o->sE8 = o->f118; o->sEC = o->f118; o->sF0 = o->f118;
        s224 = (s240 = pool(&sXY[0], timb5_B368_gscale)) * o->f128;
        o->pos += s224;
        timproc_uso_b5_func_06970C(o->m108, o->i124);
        timproc_uso_b5_func_054CAC(o);
        o->i12C = 1;
    }
}
