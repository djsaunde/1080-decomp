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
 * RESIDUAL: frame 264 vs 320 -- the target homes s240 / s224 and the three axis vectors at
 * the top of the arm (above m7C) with single-word gaps; declaring them there needs an
 * assignment form that still copies word-wise.
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
struct V3 { f32 x, y, z; V3() {} V3(f32 a, f32 b, f32 c) { x = a; y = b; z = c; } void operator=(V3 o) { x = o.x; y = o.y; z = o.z; }
    void operator+=(const V3 &v) { x += v.x; y += v.y; z += v.z; } };
inline V3 rel(const V3 &a, f32 *g) { if (0) { g[0] = a.x; } return V3(a.x - g[0], a.y - g[1], a.z - sZ); }
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
    V3 d = rel(o->pos, &sXY[0]);
    timproc_uso_b5_func_071028(&d);
    if (FW(o, 0x140) != 0) {
        o->q.x = 0.0f; o->q.y = 0.0f; o->q.z = 0.0f; o->q.w = 1.0f;
        { f32 ang = o->f110; V3 a(1.0f, 0.0f, 0.0f); o->q.rot(&a, ang); }
        { f32 ang = o->f10C; { V3 a(0.0f, 0.0f, 1.0f); o->q.rot(&a, ang); } ang = o->f114; { V3 a(0.0f, 1.0f, 0.0f); o->q.rot(&a, ang); } }
        if (FW(o, 0x13C) != 0) {
            s32 m7C[13];
            s32 m6C[4];
            timproc_uso_b5_func_0727A4(*(char **)((char *)(*(char **)&import_80087DA8) + 0x70) + 0xB4, m7C);
            import_0010806C(m7C, m6C);
            timproc_uso_b5_func_0714D4(&o->q, m6C, o->f128, 0);
        }
        o->sE8 = o->f118; o->sEC = o->f118; o->sF0 = o->f118;
        V3 s240 = pool(&sXY[0], timb5_B368_gscale);
        V3 s224 = s240 * o->f128;
        o->pos += s224;
        timproc_uso_b5_func_06970C(o->m108, o->i124);
        timproc_uso_b5_func_054CAC(o);
        o->i12C = 1;
    }
}
