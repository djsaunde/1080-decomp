/* IDO 7.1 NCC (C++) NON_MATCHING donor for gl_func_00037540 (0x2FC / 191 words), agent-c run 200. NM object only
 * (build/non_matching); the ROM build keeps INCLUDE_ASM. Sibling of the exact 37348 donor: s = o->s homed (`if (0)
 * gl(&s)`), every s access through a kept Node * inline parameter, block-scoped ctor-built push vector, volatile flag
 * word through Inp::fl(). The saved v60 is read by ONE inline gr() that also clears x (one s load for all four
 * accesses, = 37348's gv): y / z come back through references (memory: the early stores to sp+172 / 176) and x as
 * the return value (a register, spilled to sp+168 only at the call). vB4 = set() then `*=` with compound `x *= k`
 * (target: x / y stored and re-read, z kept in f12); v9C built in place, then `+=`; v6C and v90 are copies through
 * a BY-VALUE operator= (the word copy into the sp+28 parameter temp, then z, y, x). Length = target (764).
 * cfe body 81.17 -> 93.58. LEFT: zero / a colouring (target zero f14, a f16; build swapped), t-register numbering. */
typedef float f32;
/* agent-g run 289: member orders re-gridded after the block-scoped v60 read below (ctor x, y, z; operator= y, x, z;
 * += z, x, y; 72 orders): 97.267 -> 97.634 */
struct V3 { f32 x, y, z; V3() {} V3(f32 a, f32 b, f32 c) { x = a; y = b; z = c; }
    void operator=(V3 v) { y = v.y; x = v.x; z = v.z; }
    void copy(V3 v) { x = v.x; y = v.y; z = v.z; } /* agent-c run 296: x, y, z member copy for v6C (per-site order grid) */
    void operator*=(f32 k) { x *= k; y *= k; z *= k; }
    void set(f32 a, f32 b, f32 c) { x = a; y = b; z = c; }
    void operator+=(const V3 &o) { z = z + o.z; x = x + o.x; y = y + o.y; } /* agent-c run 201 operator order grid: z, y, x (+ ctor z, y, x): 93.47 -> 95.61 standalone */
};
struct Node { char pad[0x30]; char m30[0x20]; V3 v50; char pad5C[4]; V3 v60; char pad6C[0xA0 - 0x6C]; V3 vA0; };
struct Inp { f32 x, y; int pad[2]; volatile int flags; int fl() { return flags; } };
struct Obj { char pad[0x14]; Node *s; char pad18[0x2C - 0x18]; Inp *in; V3 v30; f32 k3C; };
extern "C" void gl_func_00034458(...);
/* agent-g run 289 (decode fix, kit CCCLV literal audit): the three pool words are source LITERALS, not externs --
 * bootup.uso RoData (ROM 0xD9FE44 + off) 0x19F8 = 0x3D75C28F = 0.06f, 0x19FC = 0x3F266666 = 0.65f, 0x1A00 =
 * 0x3F666666 = 0.9f (37348's own 0.06f / 0.65f pair sits right before them at 0x19F0 / 0x19F4, in code order);
 * the former D_37540_19F8 / _19FC / _1A00 aliases are gone: 96.518 -> 97.565 standalone, same length. */
inline Node *id(Node *n) { if (0) { n = 0; } return n; }
inline void sx(Node *n, f32 v) { if (0) { n = 0; } n->v60.x = v; }
inline void sy(Node *n, f32 v) { if (0) { n = 0; } n->v60.y = v; }
inline void sz(Node *n, f32 v) { if (0) { n = 0; } n->v60.z = v; }
inline void call(Node *n, V3 *v, Obj *o) { if (0) { n = 0; } gl_func_00034458(n->m30, v, o); }
extern "C" void gl_func_00037540(Obj *o) {
    V3 vB4; /* agent-g run 206: declared first (lever-sweep hoist, report metric): 95.72 -> 96.32 */
    /* agent-g run 249 (kit CCLXXV scope rule, decl-order grid): hz / hy / p1 p2 / v90 / hx / v6C hoisted to
     * function-scope declarations right below vB4 (no code change; hz 176 / hy 172 like the target): 96.39 -> 96.42 */
    f32 hz;
    f32 hy;
    /* agent-g run 288 (kit CCCLII home order, -g3 name map vs the target's sp offsets): hx right below hy (M-24 = the
     * target's 168 spill word is its home), v9C / v90 at M-36 / M-48 (target 156 / 144), the unused p1 / p2 pads, v6C
     * M-84 (108), then b, a M-92 (100), s M-96 (96), ab; v9C declared here with a code-free `if (1) {}` left at its old
     * mid-block declaration point (kit CCCLIV): 96.429 -> 96.518, same length */
    f32 hx;
    V3 v9C;
    V3 v90;
    V3 p1, p2;
    V3 v6C;
    f32 b;
    f32 a;
    Node *s;
    f32 ab;
    s = o->s;
    if (0) { gl_func_00034458(&s); }
    V3 *pv = &v9C; /* agent-c run 293: v9C's += through a pointer (`*pv += v6C`): 97.634 -> 97.728, same length; the target still re-reads v9C after its stores (open) */
    a =o->in->y * o->k3C;
    if (a < 0.0f) ab = -a; else ab = a;
    if (ab < 2.0f) a = 0.0f;
    b = o->in->x * 0.06f;
    if (b < 0.0f) ab = -b; else ab = b;
    if (ab < 0.65f) b = 0.0f;
    /* agent-g run 289: the saved v60 read in a block through a kept-parameter id() (one s load, like the old gr()), x
     * assigned straight to hx: hx's register LR now spills to hx's own home sp+168 like the target (gr()'s local x
     * took a fresh sp+72 slot) and the push vector below moves to the target's sp+68: 97.565 -> 97.267 alone, 97.634
     * with the V3 member re-grid (a plain `Node *n = s` hoists the s load into the first compare block: 94.20) */
    { Node *n = id(s); hx = n->v60.x; hy = n->v60.y; hz = n->v60.z; n->v60.x = 0.0f; }
    sy(s, 0.0f); sz(s, 0.0f);
    { V3 v1(0.0f, -b, 0.0f); call(s, &v1, o); }
    vB4.set(s->v50.x, s->v50.y, s->v50.z);
    vB4 *= a;
    sx(s, hx); sy(s, hy); sz(s, hz);
    o->v30.y = 0.0f;
    if (o->in->fl() & 4) o->v30.y = 10.0f;
    if (o->in->fl() & 8) o->v30.y = -10.0f;
    /* agent-g run 206: unused p1, p2 (no code): frame 0xA8 -> 0xC0 = target (pad-position grid): 96.32 -> 96.39 */
    v6C.copy(vB4); /* agent-c run 296: by-value copy in x, y, z order (operator= stays y, x, z for v90): 97.749 -> 97.770 */
    if (1) {} /* v9C's old declaration point (agent-g run 288) */
    v9C.x = s->vA0.x; v9C.y = s->vA0.y; v9C.z = s->vA0.z;
    *pv += v6C;
    call(s, &o->v30, o);
    v90 = v9C;
    v90 += o->v30;
    v90 += v6C;
    o->v30.x = o->v30.x * 0.9f; /* agent-c run 293: the named `f32 L3 = 0.9f` dropped (stale after the pv change; remove-one sweep): 97.728 -> 97.749 */
    o->v30.z = o->v30.z * 0.9f;
    call(s, &vB4, o);
}
