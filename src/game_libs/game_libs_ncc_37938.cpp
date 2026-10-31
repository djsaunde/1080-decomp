/* IDO 7.1 NCC (C++) EXACT donor for gl_func_00037938 (0x158 / 86 words incl. the jr-ra delay nop), agent-c run 162,
 * EXACT agent-c run 246: REPLACE_FUNC_BODY-spliced over the empty placeholder in game_libs_post0b.c (both trees).
 * kit CXI's V3 family: a = s->mtx.pos through an inline gp(V3 *v, Node *n) with a KEPT v parameter (its pooled
 * temp burns the target's extra ugen ring register: t7 / t0 / t3 for the copy chain), b via M4::getPos0
 * (`lwc1 160(vN); addiu vN, vN, 112; 52 / 56(vN)`), arg = (b - a) * o->k through V3 operator- / operator*
 * returning V3(...) and a by-value operator=, `if (0) { gl(&s); }` / `if (0) { gl(&a, &b); }` for the homed
 * s / a / b reloads; frame 0xD8 and every named home = target. cfe body 93.73 -> 98.55. Residual: operator-'s ctor
 * / return temps 8 above the target's, operator*'s result 4 and the by-value parameter 8 below, the final s reload
 * (target v1 then addiu a0), and one trailing word (86 vs 85, as the cfe body). Still NON_MATCHING.
 * agent-c run 201 (sibling port): operator* as the exact 64388 donor's FREE `operator*(const V3 &, f32 s)` with a
 * named `f32 k = s`: operator*'s result slot moves to the target's sp+0x44 (98.49 -> 98.54 objdiff diff).
 * agent-g run 211 (frame-slot layout): s through an inline Ctx accessor (the 3558 kit: `lw vN, 200(sp);
 * addiu a0, vN, 48` reload form) with `if (1) {}` after the arg statement; the scalar `o->k` named in the
 * expression (`(kk = o->k)`: its 8-byte slot puts operator-'s ctor / return temps at the target's sp+0x78 /
 * 0x84); operator= through `set(const V3 &)` (the reference temp below the by-value parameter puts it at the
 * target's sp+0x24); operator* without the named k (frame back to 0xD8). Every frame slot = target:
 * 98.6 -> 98.71. Residual: the final reload lands in v0 (target v1) and the trailing word.
 * agent-c run 246 (kit CCLXX): EXACT. The final c.out reload was the getOut() result temp, alone in its block and
 * coloured v0 first. A named `Node *n = o->s;` (declared mid-body, after getPos0: no home shift) with a
 * liveness-extending empty test `if (n) {}` after the arg statement makes n's LR (one LR over both webs) overlap
 * the block's v0 owners, so the reload `n = c.getOut()` colours v1 = target (n is defined before every read). */
typedef float f32;
struct V3 {
    f32 x, y, z;
    V3() {}
    V3(f32 a, f32 b, f32 c) { x = a; y = b; z = c; }
    V3 operator-(const V3 &o) const { return V3(x - o.x, y - o.y, z - o.z); }
    void operator=(V3 v) { set(v); }
    void set(const V3 &v) { x = v.x; y = v.y; z = v.z; }
};
inline V3 operator*(const V3 &a, f32 s) { return V3(a.x * s, a.y * s, a.z * s); }
struct M4 { f32 m[12]; V3 pos; void getPos0(V3 *v) { v->x = pos.x; v->y = pos.y; v->z = pos.z; }
    void getPos(V3 *v) { if (0) { v = 0; } v->x = pos.x; v->y = pos.y; v->z = pos.z; } };
struct Sub { char pad[0x70]; M4 mtx; };
struct Node { char pad[0x30]; char m30[0x70 - 0x30]; M4 mtx; };
struct Obj { char pad[0x14]; Node *s; char pad18[0x2C - 0x18]; Sub *t; f32 k; };
inline void gp(V3 *v, Node *n) { if (0) { v = 0; } v->x = n->mtx.pos.x; v->y = n->mtx.pos.y; v->z = n->mtx.pos.z; }
extern "C" void gl_func_00000000(...);
struct Ctx { Node *out; Ctx(Obj *o) { out = o->s; } Node *getOut() const { return out; } };
extern "C" void gl_func_00037938(Obj *o) {
    Ctx c(o);
    V3 a;
    V3 b;
    V3 arg;
    f32 kk;
    gp(&a, c.getOut());
    o->t->mtx.getPos0(&b);
    Node *n = o->s;
    if (0) { gl_func_00000000(&a, &b); }
    arg = (b - a) * (kk = o->k); if (1) {}
    if (n) {}
    n = c.getOut(); gl_func_00000000(n->m30, &arg);
}
