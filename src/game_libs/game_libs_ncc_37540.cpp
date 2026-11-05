/* IDO 7.1 NCC (C++) EXACT donor for gl_func_00037540 (0x2FC / 191 words), REPLACE_FUNC_BODY-spliced over the
 * placeholder in game_libs_post0b.c on both build paths (agent-c run 200 lift, agent-c run 298 exact).
 * Sibling of the exact 37348 donor: s = o->s homed (`if (0) gl(&s)`), every s access through a kept Node * inline
 * parameter, block-scoped ctor-built push vector, volatile flag word through Inp::fl(). The saved v60 is read in a
 * block through a kept-parameter id() (one s load); vB4 = set() then `*=`; v6C is a by-value copy of vB4 (the word
 * copy into the sp+28 parameter temp, then x, y, z), v90 a by-value copy of v9C.
 * agent-c run 298 (kit CCCLXX): v9C is a union of a plain P3 and a VOLATILE view VW (layout-compatible, so reading
 * the volatile view inspects the stored P3's common initial sequence). The `+=` reads each component through the
 * volatile view and writes the plain one: the target re-reads all three v9C words after their stores (never
 * forwarded) while keeping v6C / the copy temp in f6 / f8 / f10, and the plain stores stay freely scheduled
 * (`lwc1 f6,160(sp)` above `swc1 f4,156(sp)`). With it the run-288 `if (1) {}` and the run-293 `*pv` route are
 * gone; operator= / `+=` re-gridded to x, y, z; the frame word pv held is kept by an unused `pad9C` home.
 * 97.770 -> 100. The three pool words are source literals (bootup.uso RoData 0x19F8 / 0x19FC / 0x1A00 = 0.06f,
 * 0.65f, 0.9f; agent-g run 289 literal audit): the donor's .rodata reloc is renamed by the splice to
 * gl_func_00037540_rodata and pinned to 0x19F8 in undefined_syms_auto.txt (the USO already ships the words).
 * The three calls are blank USO jal words: gl_func_00000000. */
typedef float f32;
struct P3 { f32 x, y, z; };
struct V3 { f32 x, y, z; V3() {} V3(f32 a, f32 b, f32 c) { x = a; y = b; z = c; }
    void operator=(P3 v) { x = v.x; y = v.y; z = v.z; }
    void copy(V3 v) { x = v.x; y = v.y; z = v.z; }
    void operator*=(f32 k) { x *= k; y *= k; z *= k; }
    void set(f32 a, f32 b, f32 c) { x = a; y = b; z = c; }
    void operator+=(const V3 &o) { x = x + o.x; y = y + o.y; z = z + o.z; }
};
struct VW { volatile f32 x, y, z; };
union VU { P3 v; VW w; };
struct Node { char pad[0x30]; char m30[0x20]; V3 v50; char pad5C[4]; V3 v60; char pad6C[0xA0 - 0x6C]; V3 vA0; };
struct Inp { f32 x, y; int pad[2]; volatile int flags; int fl() { return flags; } };
struct Obj { char pad[0x14]; Node *s; char pad18[0x2C - 0x18]; Inp *in; V3 v30; f32 k3C; };
extern "C" void gl_func_00000000(...);
inline Node *id(Node *n) { if (0) { n = 0; } return n; }
inline void sx(Node *n, f32 v) { if (0) { n = 0; } n->v60.x = v; }
inline void sy(Node *n, f32 v) { if (0) { n = 0; } n->v60.y = v; }
inline void sz(Node *n, f32 v) { if (0) { n = 0; } n->v60.z = v; }
inline void call(Node *n, V3 *v, Obj *o) { if (0) { n = 0; } gl_func_00000000(n->m30, v, o); }
extern "C" void gl_func_00037540(Obj *o) {
    /* declaration order = the target's homes (agent-g runs 206 / 249 / 288): vB4 first, hz 176 / hy 172, hx at the
     * 168 spill word, v9C / v90 at 156 / 144, the unused p1 / p2 frame pads, v6C 108, then b, a, s 96, ab */
    V3 vB4;
    f32 hz;
    f32 hy;
    f32 hx;
    VU u9C;
    V3 v90;
    V3 p1, p2;
    V3 v6C;
    f32 b;
    f32 a;
    Node *s;
    f32 ab;
    s = o->s;
    if (0) { gl_func_00000000(&s); }
    int pad9C = 0;
    a = o->in->y * o->k3C;
    if (a < 0.0f) ab = -a; else ab = a;
    if (ab < 2.0f) a = 0.0f;
    b = o->in->x * 0.06f;
    if (b < 0.0f) ab = -b; else ab = b;
    if (ab < 0.65f) b = 0.0f;
    { Node *n = id(s); hx = n->v60.x; hy = n->v60.y; hz = n->v60.z; n->v60.x = 0.0f; }
    sy(s, 0.0f); sz(s, 0.0f);
    { V3 v1(0.0f, -b, 0.0f); call(s, &v1, o); }
    vB4.set(s->v50.x, s->v50.y, s->v50.z);
    vB4 *= a;
    sx(s, hx); sy(s, hy); sz(s, hz);
    o->v30.y = 0.0f;
    if (o->in->fl() & 4) o->v30.y = 10.0f;
    if (o->in->fl() & 8) o->v30.y = -10.0f;
    v6C.copy(vB4);
    u9C.v.x = s->vA0.x; u9C.v.y = s->vA0.y; u9C.v.z = s->vA0.z;
    u9C.v.x = u9C.w.x + v6C.x; u9C.v.y = u9C.w.y + v6C.y; u9C.v.z = u9C.w.z + v6C.z;
    call(s, &o->v30, o);
    v90 = u9C.v;
    v90 += o->v30;
    v90 += v6C;
    o->v30.x = o->v30.x * 0.9f;
    o->v30.z = o->v30.z * 0.9f;
    call(s, &vB4, o);
}
