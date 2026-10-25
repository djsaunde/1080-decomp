/* IDO 7.1 NCC (C++) donor for timproc_uso_b5_func_0000B368 (0x2B4 / 173 words): AC20's sibling
 * (reference-point offset normalised, quaternion rebuilt from three axis-angle rotations, optional
 * 0727A4 / 0714D4 orientation, sE8..sF0 = f118, then pos += (pool * gscale) * f128).
 * Spliced into timproc_uso_b5.c.o (both trees) via REPLACE_FUNC_BODY; its .bss relocs are renamed
 * timproc_uso_b5_func_0000B368_bss by the splice and pinned to 0 (the USO bakes 0 / 4 / 8), the
 * gscale base timb5_B368_gscale pinned to 0. agent-g run 166 (docs/IDO_CODEGEN.md kit CVII):
 * - the AC20 recipe: file-local x / y pair + z read through rel()'s pointer parameter with an
 *   `if (0)` keeper, o->q.rot() via edg's pooled `this` temp, nested angle blocks;
 * - V3 with NO user operator= and both scaled vectors in ONE expression
 *   `s224 = (s240 = pool(...)) * o->f128`: implicit memberwise copies = the target's word copies,
 *   and the scale gets its own second return temp (sp+76, `addiu v0,sp,76`);
 * - frame homes: an arm-level `f32 ang` between s240 and s224, rel()'s x through a named
 *   reference (inline-body local, homed below rel's return temp), the 0727A4 buffer as 16 words,
 *   operator+= returning V3 &, and operator*'s factor copied into a named local `k` (homed below
 *   the scale's return temp; the reference parameter's own temp sat above it).
 * History: runs 142-157 cfe lift / relPos helpers (87.47 -> 95.71). */
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
    V3 &operator+=(const V3 &v) { x += v.x; y += v.y; z += v.z; return *this; } };
inline V3 rel(const V3 &a, f32 *g) { f32 &x = g[0]; if (0) { x = a.x; } return V3(a.x - x, a.y - g[1], a.z - sZ); }
inline V3 pool(f32 *g, const f32 &s) { if (0) { g[0] = s; } return V3(g[0] * s, g[1] * s, sZ * s); }
inline V3 operator*(const V3 &a, const f32 &s) { f32 k = s; return V3(a.x * k, a.y * k, a.z * k); }
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
        V3 s240;
        f32 ang;
        V3 s224;
        o->q.x = 0.0f; o->q.y = 0.0f; o->q.z = 0.0f; o->q.w = 1.0f;
        { ang = o->f110; V3 a(1.0f, 0.0f, 0.0f); o->q.rot(&a, ang); }
        { f32 ang = o->f10C; { V3 a(0.0f, 0.0f, 1.0f); o->q.rot(&a, ang); } ang = o->f114; { V3 a(0.0f, 1.0f, 0.0f); o->q.rot(&a, ang); } }
        if (FW(o, 0x13C) != 0) {
            s32 m7C[16];
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
