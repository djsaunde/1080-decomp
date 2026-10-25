/* IDO 7.1 NCC (C++) EXACT donor for gl_func_00037348 (0x1F8 / 126 words), agent-c run 164 / 173, spliced over an
 * empty placeholder in game_libs_post0b.c in both build trees.
 * Input deadzone + two pushes through the blank callee (jal 0 = gl_func_00000000): s = o->s homed (`if (0) gl(&s)`),
 * every s access through a KEPT Node * inline parameter (`if (0) { n = 0; }`: one pooled pointer temp = the target's
 * v0 reload after each store through s); gv() copies the saved vector AND clears x in one inline; restore via
 * per-component rx/ry/rz(Node *, V3 &); the flag word is a volatile member read through Inp::fl(). The first push
 * vector is a BLOCK-scoped `V3 v1(y, x, 0.0f)` built by a ctor right at the call. p0/p1 = the two words under s.
 * Run 173: the second vector's zeros through `z3(V3 &, int)` called with 0 (agent-g kit CXIX: the int -> float fold
 * happens after candidate selection, so these stores get their own local zero in f0 instead of joining the head's
 * 0.0 candidate, whose LR forbids f0); the FP pool words read through the per-site aliases D_37348_19F0 / _19F4
 * (pinned 0: the target's baked `lui at, 0; lwc1 0x19F0(at)`), which costs the end-of-body volatile. */
typedef float f32;
struct V3 { f32 x, y, z; V3() {} V3(f32 a, f32 b, f32 c) { x = a; y = b; z = c; } };
struct Node { char pad[0x30]; char m30[0x30]; V3 v60; };
struct Inp { f32 x, y; int pad[2]; volatile int flags; int fl() { return flags; } };
struct Obj { char pad[0x14]; Node *s; char pad18[0x2C - 0x18]; Inp *in; };
extern "C" void gl_func_00000000(...);
extern "C" f32 D_37348_19F0[], D_37348_19F4[];
inline void gv(Node *n, V3 &o) { if (0) { n = 0; } o.x = n->v60.x; o.y = n->v60.y; o.z = n->v60.z; n->v60.x = 0.0f; }
inline void zy(Node *n) { if (0) { n = 0; } n->v60.y = 0.0f; }
inline void zz(Node *n) { if (0) { n = 0; } n->v60.z = 0.0f; }
inline void rx(Node *n, V3 &o) { if (0) { n = 0; } n->v60.x = o.x; }
inline void ry(Node *n, V3 &o) { if (0) { n = 0; } n->v60.y = o.y; }
inline void rz(Node *n, V3 &o) { if (0) { n = 0; } n->v60.z = o.z; }
inline void sx(Node *n, f32 v) { if (0) { n = 0; } n->v60.x = v; }
inline void sy(Node *n, f32 v) { if (0) { n = 0; } n->v60.y = v; }
inline void sz(Node *n, f32 v) { if (0) { n = 0; } n->v60.z = v; }
inline void call(Node *n, V3 *v, Obj *o) { if (0) { n = 0; } gl_func_00000000(n->m30, v, o); }
inline void callr(Node *n, const V3 &v, Obj *o) { if (0) { n = 0; } gl_func_00000000(n->m30, &v, o); }
inline void z3(V3 &v, int z) { v.x = z; v.y = z; v.z = z; }
extern "C" void gl_func_00037348(Obj *o) {
    Node *s = o->s;
    volatile int p0, p1;
    V3 save;
    V3 v2;
    f32 x, y;
    f32 ab;
    if (0) { gl_func_00000000(&s); }
    x = o->in->x * D_37348_19F0[0x19F0 / 4];
    y = o->in->y * D_37348_19F0[0x19F0 / 4];
    if (x < 0.0f) ab = -x; else ab = x;
    if (ab < D_37348_19F4[0x19F4 / 4]) x = 0.0f;
    if (y < 0.0f) ab = -y; else ab = y;
    if (ab < D_37348_19F4[0x19F4 / 4]) y = 0.0f;
    gv(s, save);
        zy(s);
    zz(s);
    { V3 v1(y, x, 0.0f); call(s, &v1, o); }
    rx(s, save); ry(s, save); rz(s, save);
    z3(v2, 0);
    if (o->in->fl() & 4) v2.y = 10.0f;
    if (o->in->fl() & 8) v2.y = -10.0f;
    if (o->in->fl() & 1) v2.x = -10.0f;
    if (o->in->fl() & 2) v2.x = 10.0f;
    if (o->in->fl() & 0x40) v2.z = 10.0f;
    if (o->in->fl() & 0x80) v2.z = -10.0f;
    call(s, &v2, o);
}
