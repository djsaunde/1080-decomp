/* IDO 7.1 NCC (C++) donor for SelCourseMap_display_0000B368 (0x2B4 / 173 words): AC20's sibling
 * (reference-point offset normalised, quaternion rebuilt from three axis-angle rotations, optional
 * 0727A4 / 0714D4 orientation, sE8..sF0 = f118, then pos += (pool * gscale) * f128).
 * Spliced into timproc_uso_b5.c.o (both trees) via REPLACE_FUNC_BODY; its .bss relocs are renamed
 * SelCourseMap_display_0000B368_bss by the splice and pinned to 0 (the USO bakes 0 / 4 / 8), the
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
#include "../../include/game/camera.h" /* CamWindow / MCamera (import_80087DA8 is the CamWindow *); offset asserts are C-only */
#define FW(p, o) (*(s32 *)((char *)(p) + (o)))
extern "C" {
extern char import_80087DA8;
void *gen_imp_math_vec3Normalize(...);
void gen_imp_math_quatRotateAxisAngle(void *quat, void *axis, f32 angle);
void gen_imp_math_mtx4Transpose(void *src, void *dst);
void import_0010806C(void *src, void *dst);
void gen_imp_math_quatSlerp(void *quat, void *vec, f32 scale, s32 flag);
void gen_imp_light_setF124_000550A0(void *obj, s32 val);
#ifdef PORT64
/* x86-64 (plan 21.9): light_setF124 with its float parameter (abi-scan --x64 X64P1) */
void port64_light_setF124(void *obj, f32 val) __asm__("light_setF124_000550A0");
#endif
void gen_imp_QNode_display(void *obj);
extern f32 timb5_B368_gscale;
}
#ifdef PORT
/* plan 22.6: the ROM's x / y / z here are relocated to genproc Bss+0x730 (uso-data-sites: sym41, the
 * camera look axis DispItems_display_0000E5D8 stores every frame), which the port names
 * timproc_uso_b5_D_807FFBB0. File statics read 0: Select Course's photo never zoomed toward the camera
 * (B368's pos += look * gscale * f128), and the board / rider / tesuri facing used pos, not pos - look. */
extern "C" char timproc_uso_b5_D_807FFBB0;
#define sXY ((float *)&timproc_uso_b5_D_807FFBB0)
#define sZ (((float *)&timproc_uso_b5_D_807FFBB0)[2])
#else
static float sXY[2]; static float sZ;
#endif
struct V3 { f32 x, y, z; V3() {} V3(f32 a, f32 b, f32 c) { x = a; y = b; z = c; }
    V3 &operator+=(const V3 &v) { x += v.x; y += v.y; z += v.z; return *this; } };
inline V3 rel(const V3 &a, f32 *g) { f32 &x = g[0]; if (0) { x = a.x; } return V3(a.x - x, a.y - g[1], a.z - sZ); }
inline V3 pool(f32 *g, const f32 &s) { if (0) { g[0] = s; } return V3(g[0] * s, g[1] * s, sZ * s); }
inline V3 operator*(const V3 &a, const f32 &s) { f32 k = s; return V3(a.x * k, a.y * k, a.z * k); }
struct Quat {
    f32 x, y, z, w;
    void rot(V3 *axis, f32 ang) { gen_imp_math_quatRotateAxisAngle(this, axis, ang); }
};
struct Obj {
    char pad0[0xDC]; V3 pos; f32 sE8, sEC, sF0; Quat q; char pad104[0x108 - 0x104]; void *m108;
    f32 f10C, f110, f114, f118; char p11C[0x124 - 0x11C]; s32 i124; f32 f128; s32 i12C;
    char p130[0x13C - 0x130]; s32 unk13C; s32 unk140; /* SelCourseModel (game/select.h) */
};
extern "C" void SelCourseMap_display_0000B368(Obj *o) {
    V3 d = rel(o->pos, &sXY[0]);
    gen_imp_math_vec3Normalize(&d);
    if (o->unk140 != 0) {
        V3 s240;
        f32 ang;
        V3 s224;
        o->q.x = 0.0f; o->q.y = 0.0f; o->q.z = 0.0f; o->q.w = 1.0f;
        { ang = o->f110; V3 a(1.0f, 0.0f, 0.0f); o->q.rot(&a, ang); }
        { f32 ang = o->f10C; { V3 a(0.0f, 0.0f, 1.0f); o->q.rot(&a, ang); } ang = o->f114; { V3 a(0.0f, 1.0f, 0.0f); o->q.rot(&a, ang); } }
        if (o->unk13C != 0) {

            s32 m7C[16];
            s32 m6C[4];
            gen_imp_math_mtx4Transpose((*(CamWindow **)&import_80087DA8)->cam->view, m7C); /* the active view camera */
            import_0010806C(m7C, m6C);
            gen_imp_math_quatSlerp(&o->q, m6C, o->f128, 0);
        }
        o->sE8 = o->f118; o->sEC = o->f118; o->sF0 = o->f118;
        s224 = (s240 = pool(&sXY[0], timb5_B368_gscale)) * o->f128;
        o->pos += s224;
#ifdef PORT64
        /* x86-64 (plan 21.9): the word at 0x124 is the float's bits; light_setF124 takes the float (xmm0) */
        port64_light_setF124(o->m108, *(f32 *)&o->i124);
#else
        gen_imp_light_setF124_000550A0(o->m108, o->i124);
#endif
        gen_imp_QNode_display(o);
        o->i12C = 1;
    }
}
