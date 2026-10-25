/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00037348 (0x1F8 / 126 words), agent-c run 164.
 * Input deadzone + two pushes through 34458: s = o->s homed (`if (0) gl(&s)`), every s access through a KEPT
 * Node * inline parameter (`if (0) { n = 0; }`: one pooled pointer temp = the target's v0 reload after each store
 * through s); gv() copies the saved vector AND clears x in one inline (the target's fourth access reuses the
 * first s load); restore via per-component rx/ry/rz(Node *, V3 &) (ugen ft0..ft2 copies); the flag word is a
 * volatile member read through Inp::fl() (o->in CSE'd in v0 across the beqzl path, flags re-read every test).
 * The first push vector is a BLOCK-scoped `V3 v1(y, x, 0.0f)` built by a ctor right at the call: the stores
 * then follow the s load and the o home store as in the target (named function-scope v1: the o spill was
 * scheduled after the stores), and the block local is homed at the bottom (sp+0x2C with one end-of-body
 * block volatile). p0/p1 = the two words under s. cfe body 93.13 -> 99.73 (local objdiff), frame 0x70.
 * Residual (4 words): the post-call FP zero for v2 in f2 (target f0). zdbug:5: ONE 0.0 constant LR covers the
 * head and the post-call BBs and is forbidden f0 by k / ab at the head. Still NON_MATCHING. */
typedef float f32;
struct V3 { f32 x, y, z; V3() {} V3(f32 a, f32 b, f32 c) { x = a; y = b; z = c; } };
struct Node { char pad[0x30]; char m30[0x30]; V3 v60; };
struct Inp { f32 x, y; int pad[2]; volatile int flags; int fl() { return flags; } };
struct Obj { char pad[0x14]; Node *s; char pad18[0x2C - 0x18]; Inp *in; };
extern "C" void gl_func_00034458(...);
inline void gv(Node *n, V3 &o) { if (0) { n = 0; } o.x = n->v60.x; o.y = n->v60.y; o.z = n->v60.z; n->v60.x = 0.0f; }
inline void zy(Node *n) { if (0) { n = 0; } n->v60.y = 0.0f; }
inline void zz(Node *n) { if (0) { n = 0; } n->v60.z = 0.0f; }
inline void rx(Node *n, V3 &o) { if (0) { n = 0; } n->v60.x = o.x; }
inline void ry(Node *n, V3 &o) { if (0) { n = 0; } n->v60.y = o.y; }
inline void rz(Node *n, V3 &o) { if (0) { n = 0; } n->v60.z = o.z; }
inline void sx(Node *n, f32 v) { if (0) { n = 0; } n->v60.x = v; }
inline void sy(Node *n, f32 v) { if (0) { n = 0; } n->v60.y = v; }
inline void sz(Node *n, f32 v) { if (0) { n = 0; } n->v60.z = v; }
inline void call(Node *n, V3 *v, Obj *o) { if (0) { n = 0; } gl_func_00034458(n->m30, v, o); }
inline void callr(Node *n, const V3 &v, Obj *o) { if (0) { n = 0; } gl_func_00034458(n->m30, &v, o); }
extern "C" void gl_func_00037348(Obj *o) {
    Node *s = o->s;
    volatile int p0, p1;
    V3 save;
    V3 v2;
    f32 x, y;
    f32 ab;
    if (0) { gl_func_00034458(&s); }
    x = o->in->x * 0.0123f;
    y = o->in->y * 0.0123f;
    if (x < 0.0f) ab = -x; else ab = x;
    if (ab < 0.15f) x = 0.0f;
    if (y < 0.0f) ab = -y; else ab = y;
    if (ab < 0.15f) y = 0.0f;
    gv(s, save);
        zy(s);
    zz(s);
    { V3 v1(y, x, 0.0f); call(s, &v1, o); }
    rx(s, save); ry(s, save); rz(s, save);
    v2.z = 0.0f;
    v2.y = 0.0f;
    v2.x = 0.0f;
    if (o->in->fl() & 4) v2.y = 10.0f;
    if (o->in->fl() & 8) v2.y = -10.0f;
    if (o->in->fl() & 1) v2.x = -10.0f;
    if (o->in->fl() & 2) v2.x = 10.0f;
    if (o->in->fl() & 0x40) v2.z = 10.0f;
    if (o->in->fl() & 0x80) v2.z = -10.0f;
    call(s, &v2, o);
    { volatile int e0; }
}
