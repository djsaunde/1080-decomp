/* IDO 7.1 NCC (C++) donor for SelCharacter_display_0000B624 (0x22C / 139 words): AC20 / B154 /
 * B368's sibling (reference-point offset normalised, facing test against D_807FFBB0, quaternion
 * rebuilt from three axis-angle rotations, 06970C, the 0x144 countdown / 077C44 tail).
 * Spliced into timproc_uso_b5.c.o (both trees) via REPLACE_FUNC_BODY; its .bss relocs are renamed
 * SelCharacter_display_0000B624_bss by the splice and pinned to 0 (the USO bakes 0 / 4 / 8).
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
/* named genproc RoData offset: the token is relocated against the RoData base (same name style
 * as the kRo*_gen_ block in game/select.h, which this donor does not include) */
#define kRoD_gen_m0p64_350 0x350 /* RoData+0x350: f64 -0.64 (the facing-cone cosine, as SelBoard_display's 0x338) */
extern char timproc_uso_b5_D_807FFBB0;
extern int timproc_uso_b5_D_807FFBBC;
void *gen_imp_math_vec3Normalize(...);
void gen_imp_math_quatRotateAxisAngle(void *quat, void *axis, f32 angle);
void gen_imp_QNode_display(void *obj);
int gen_imp_QNode_update(...);
int gen_imp_AnimPlayer_start(...);
void timproc_uso_b5_func_06970C_i(...);
#ifdef PORT64
/* x86-64 (plan 21.9): light_setF124 with its float parameter (abi-scan --x64 X64KR) */
void port64_light_setF124(void *obj, f32 val) __asm__("light_setF124_000550A0");
#endif
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
struct V3 { f32 x, y, z; V3() {} V3(f32 a, f32 b, f32 c) { x = a; y = b; z = c; } };
inline f32 dot(const V3 &a, const V3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline V3 rel(const V3 &a, f32 *g) { if (0) { g[1] = a.y; } return V3(a.x - g[0], a.y - g[1], a.z - sZ); }
struct Quat {
    f32 x, y, z, w;
    void rot(V3 *axis, f32 ang) { gen_imp_math_quatRotateAxisAngle(this, axis, ang); }
};
inline void one(Quat &q, f32 xa) { if (0) { xa = 0.0f; } V3 a(1.0f, 0.0f, 0.0f); q.rot(&a, xa); }
/* select.h SelCharAnim: the AnimPlayer (0xB0) plus keyHit, set by its onKey. */
struct SelCharAnim { char player[0xB0]; s32 keyHit; };
/* select.h SelCharModel, with the QNode head's pos / rotation spelled out (edg sees no gdl headers). */
struct Obj {
    char pad0[0xDC]; V3 pos; f32 sE8, sEC, sF0; Quat q; char pad104[0x108 - 0x104];
    s32 inner;              /* 0x108: SelModel.inner */
    SelCharAnim *anim;      /* 0x10C */
    f32 view[4];            /* 0x110: item rot z / x / y, scale */
    f32 alpha, unk124, unk128, unk12C;
    f32 highlight;          /* 0x130 */
    s32 visible;            /* 0x134: 1 when drawn this frame (facing the camera) */
    s32 focused;            /* 0x138 */
    s32 chosen;             /* 0x13C */
    s32 animSet;            /* 0x140 */
    s32 animTimer;          /* 0x144 */
};
extern "C" void SelCharacter_display_0000B624(Obj *o) {
    V3 d = rel(o->pos, &sXY[0]);
    gen_imp_math_vec3Normalize(&d);
    if ((f64)dot(*(V3 *)&timproc_uso_b5_D_807FFBB0, d) < *(f64 *)((char *)&timproc_uso_b5_D_807FF370 + kRoD_gen_m0p64_350)) {
        o->q.x = 0.0f; o->q.y = 0.0f; o->q.z = 0.0f; o->q.w = 1.0f;
        f32 ang3;
        one(o->q, o->view[1]);
        { f32 ang = o->view[0]; V3 a(0.0f, 0.0f, 1.0f); o->q.rot(&a, ang); }
        ang3 = o->view[2];
        { V3 a(0.0f, 1.0f, 0.0f); o->q.rot(&a, ang3); }
#ifdef PORT64
        /* x86-64 (plan 21.9): light_setF124 takes the float in xmm0; a variadic call passes the bits in rsi */
        port64_light_setF124(o->inner, o->highlight);
#else
        timproc_uso_b5_func_06970C_i(o->inner, *(s32 *)&o->highlight);
#endif
        if (o->animTimer != 0 || o->focused != 0) {
            gen_imp_QNode_update(o);
            timproc_uso_b5_D_807FFBBC += 1;
            if (o->animTimer != 0) o->animTimer -= 1;
            if (o->animTimer == 0) {
                if (o->anim->keyHit != 0) {
                    gen_imp_AnimPlayer_start(o->anim, o->animSet, 0, 2, 2, 1);
                    o->anim->keyHit = 0;
                }
            }
        }
        gen_imp_QNode_display(o);
        o->visible = 1;
        return;
    }
    o->visible = 0;
}
