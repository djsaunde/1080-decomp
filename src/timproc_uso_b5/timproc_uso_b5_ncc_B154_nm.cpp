/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for timproc_uso_b5_func_0000B154
 * (133 words = target). NM object only (build/non_matching); the ROM build keeps INCLUDE_ASM.
 * agent-g run 166 (kit CVII): the AC20 / B624 / B368 family recipe (file-local reference point
 * through rel()'s pointer parameter, x / y differences named with a while (0) anchor on dx,
 * o->q.rot() via the pooled this temporary, nested angle blocks) plus: the 05B750 vector as a
 * V4 declared FIRST (frame top) and filled by an inline set(); (f128 - f12C) * f11C operand
 * order; the 0x140 counter incremented as a plain store (a ++ result took a uopt colour):
 * cfe 92.29 -> 98.87 standalone.
 * RESIDUAL: the head temp one word high / the axis temps one word low; the V4 w value is in
 * f12 (target f0: set()'s float parameters share edg's pooled float temps with V3's ctor). */
typedef float f32; typedef int s32; typedef double f64;
extern "C" {
extern char import_8005C108;
void *timproc_uso_b5_func_071028(...);
void timproc_uso_b5_func_071AE0(void *quat, void *axis, f32 angle);
void timproc_uso_b5_func_05B750(void *arg, void *vec4);
void timproc_uso_b5_func_06970C(void *obj, f32 val);
void timproc_uso_b5_func_054CAC(void *obj);
}
static float sXY[2]; static float sZ;
struct V3 { f32 x, y, z; V3() {} V3(f32 a, f32 b, f32 c) { x = a; y = b; z = c; } void operator=(V3 o) { x = o.x; y = o.y; z = o.z; } };
struct V4 { f32 x, y, z, w; V4() {} void set(f32 a, f32 b, f32 c, f32 d) { x = a; y = b; z = c; w = d; } };
inline V3 rel(const V3 &a, f32 *g) { f32 dx; f32 dy; while (0) { dx = 0.0f; } dx = a.x - g[0]; dy = a.y - g[1]; return V3(dx, dy, a.z - sZ); }
struct Quat {
    f32 x, y, z, w;
    void rot(V3 *axis, f32 ang) { timproc_uso_b5_func_071AE0(this, axis, ang); }
};
struct Elem { char p[0x18]; s32 flags; };
struct Obj {
    char pad0[0xDC]; V3 pos; f32 sE8, sEC, sF0; Quat q; char pad104[0x108 - 0x104]; void *m108;
    f32 f10C, f110, f114, f118, f11C, f120, f124, f128, f12C; char p130[0x13C - 0x130]; s32 i13C, i140, i144; Elem *list[1];
};
extern "C" void timproc_uso_b5_func_0000B154(Obj *o) {
    V4 r;
    V3 d = rel(o->pos, &sXY[0]);
    timproc_uso_b5_func_071028(&d);
    o->q.x = 0.0f; o->q.y = 0.0f; o->q.z = 0.0f; o->q.w = 1.0f;
    { f32 ang = o->f110; V3 a(1.0f, 0.0f, 0.0f); o->q.rot(&a, ang); }
    { f32 ang = o->f10C; { V3 a(0.0f, 0.0f, 1.0f); o->q.rot(&a, ang); } ang = o->f114; { V3 a(0.0f, 1.0f, 0.0f); o->q.rot(&a, ang); } }
    o->sE8 = o->f118; o->sEC = o->f118; o->sF0 = o->f118;
    r.set(0.0f, 0.0f, 0.0f, o->f120);
    timproc_uso_b5_func_05B750(&import_8005C108, &r);
    timproc_uso_b5_func_06970C(o->m108, 0.5f);
    timproc_uso_b5_func_06970C(o->m108, o->f128 - (o->f128 - o->f12C) * o->f11C);
    if (!(o->i144++ & 1)) {
        o->i140 = o->i140 + 1;
        if (o->i140 == o->i13C) o->i140 = 0;
    }
    o->list[o->i140]->flags |= 4;
    timproc_uso_b5_func_054CAC(o);
    o->list[o->i140]->flags &= ~4;
}
