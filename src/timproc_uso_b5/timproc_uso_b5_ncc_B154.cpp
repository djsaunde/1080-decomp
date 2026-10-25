/* IDO 7.1 NCC (C++) donor for timproc_uso_b5_func_0000B154 (0x214 / 133 words): AC20 / B368's
 * sibling (reference-point offset, quaternion rebuilt from three axis-angle rotations, sE8..sF0 =
 * f118, 05B750 with {0, 0, 0, f120}, two 06970C calls, the circular 0x140 index and the element
 * flag toggle around 054CAC). Spliced into timproc_uso_b5.c.o (both trees) via REPLACE_FUNC_BODY;
 * its .bss relocs are renamed timproc_uso_b5_func_0000B154_bss by the splice and pinned to 0 (the
 * USO bakes 0 / 4 / 8). agent-g run 167 (docs/IDO_CODEGEN.md kit CIX), on top of run 166's NM donor:
 * - the sE8..sF0 triple is a V3 member set by an inline `all(f32 v)` whose body copies v into a
 *   named local (`f32 s = v;`): the named local is the home the target has between the third axis
 *   vector and the pooled `this` temp, and without the run-166 `while (0)` anchor in rel() the head
 *   differences take the target's f0 / f2 / f12 (the anchor's dx home was the one misplaced word);
 * - V4::set stores w first (`w = d; x = a; ...`): the f120 load follows the sE8 stores and lands
 *   in f0 like the target;
 * - rel() keeps its pointer parameters through `if (0) { g[0] = a.x; }` (v0 = &o->pos, z through a
 *   third pointer parameter).
 * History: runs 142-166 (cfe lift 92.29, NM donor 98.90). */
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
struct V3 { f32 x, y, z; V3() {} void all(f32 v) { f32 s = v; x = s; y = s; z = s; } V3(f32 a, f32 b, f32 c) { x = a; y = b; z = c; } };
struct V4 { f32 x, y, z, w; V4() {} void set(f32 a, f32 b, f32 c, f32 d) { w = d; x = a; y = b; z = c; } };
inline V3 rel(const V3 &a, f32 *g, f32 *z) { if (0) { g[0] = a.x; } return V3(a.x - g[0], a.y - g[1], a.z - *z); }
struct Quat {
    f32 x, y, z, w;
    void rot(V3 *axis, f32 ang) { timproc_uso_b5_func_071AE0(this, axis, ang); }
};
struct Elem { char p[0x18]; s32 flags; };
struct Obj {
    char pad0[0xDC]; V3 pos; V3 sc; Quat q; char pad104[0x108 - 0x104]; void *m108;
    f32 f10C, f110, f114, f118, f11C, f120, f124, f128, f12C; char p130[0x13C - 0x130]; s32 i13C, i140, i144; Elem *list[1];
};
extern "C" void timproc_uso_b5_func_0000B154(Obj *o) {
    V4 r;
    V3 d = rel(o->pos, &sXY[0], &sZ);
    timproc_uso_b5_func_071028(&d);
    o->q.x = 0.0f; o->q.y = 0.0f; o->q.z = 0.0f; o->q.w = 1.0f;
    { f32 ang = o->f110; V3 a(1.0f, 0.0f, 0.0f); o->q.rot(&a, ang); }
    { f32 ang = o->f10C; { V3 a(0.0f, 0.0f, 1.0f); o->q.rot(&a, ang); } ang = o->f114; { V3 a(0.0f, 1.0f, 0.0f); o->q.rot(&a, ang); } }
    o->sc.all(o->f118);
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
