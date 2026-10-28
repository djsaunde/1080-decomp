/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00037938 (0x154 / 85 words), agent-c run 162.
 * kit CXI's V3 family: a = s->mtx.pos through an inline gp(V3 *v, Node *n) with a KEPT v parameter (its pooled
 * temp burns the target's extra ugen ring register: t7 / t0 / t3 for the copy chain), b via M4::getPos0
 * (`lwc1 160(vN); addiu vN, vN, 112; 52 / 56(vN)`), arg = (b - a) * o->k through V3 operator- / operator*
 * returning V3(...) and a by-value operator=, `if (0) { gl(&s); }` / `if (0) { gl(&a, &b); }` for the homed
 * s / a / b reloads; frame 0xD8 and every named home = target. cfe body 93.73 -> 98.55. Residual: operator-'s ctor
 * / return temps 8 above the target's, operator*'s result 4 and the by-value parameter 8 below, the final s reload
 * (target v1 then addiu a0), and one trailing word (86 vs 85, as the cfe body). Still NON_MATCHING.
 * agent-c run 201 (sibling port): operator* as the exact 64388 donor's FREE `operator*(const V3 &, f32 s)` with a
 * named `f32 k = s`: operator*'s result slot moves to the target's sp+0x44 (98.49 -> 98.54 objdiff diff). */
typedef float f32;
struct V3 {
    f32 x, y, z;
    V3() {}
    V3(f32 a, f32 b, f32 c) { x = a; y = b; z = c; }
    V3 operator-(const V3 &o) const { return V3(x - o.x, y - o.y, z - o.z); }
    void operator=(V3 v) { x = v.x; y = v.y; z = v.z; }
};
inline V3 operator*(const V3 &a, f32 s) { f32 k = s; return V3(a.x * k, a.y * k, a.z * k); }
struct M4 { f32 m[12]; V3 pos; void getPos0(V3 *v) { v->x = pos.x; v->y = pos.y; v->z = pos.z; }
    void getPos(V3 *v) { if (0) { v = 0; } v->x = pos.x; v->y = pos.y; v->z = pos.z; } };
struct Sub { char pad[0x70]; M4 mtx; };
struct Node { char pad[0x30]; char m30[0x70 - 0x30]; M4 mtx; };
struct Obj { char pad[0x14]; Node *s; char pad18[0x2C - 0x18]; Sub *t; f32 k; };
inline void gp(V3 *v, Node *n) { if (0) { v = 0; } v->x = n->mtx.pos.x; v->y = n->mtx.pos.y; v->z = n->mtx.pos.z; }
extern "C" void gl_func_00000000(...);
extern "C" void gl_func_00037938(Obj *o) {
    Node *s = o->s;
    V3 a;
    V3 b;
    V3 arg;
    if (0) { gl_func_00000000(&s); }
    gp(&a, s);
    o->t->mtx.getPos0(&b);
    if (0) { gl_func_00000000(&a, &b); }
    arg = (b - a) * o->k;
    gl_func_00000000(s->m30, &arg);
}
