/* IDO 7.1 NCC (C++) NON_MATCHING donor for gl_func_00037540 (0x2FC / 191 words), agent-c run 200. NM object only
 * (build/non_matching); the ROM build keeps INCLUDE_ASM. Sibling of the exact 37348 donor: s = o->s homed (`if (0)
 * gl(&s)`), every s access through a kept Node * inline parameter, block-scoped ctor-built push vector, volatile flag
 * word through Inp::fl(). vB4 / v9C are declared where they are built (ctor straight into the frame slot); v6C and
 * v90 are copies through a BY-VALUE operator= (the target's word copy into the sp+28 parameter temp, then z, y, x).
 * cfe body 81.17 -> this. LEFT: save.x held in a register until the call (target), vB4 / v9C store-reload pairs. */
typedef float f32;
struct V3 { f32 x, y, z; V3() {} V3(f32 a, f32 b, f32 c) { x = a; y = b; z = c; }
    void operator=(V3 v) { z = v.z; y = v.y; x = v.x; }
    void operator+=(const V3 &o) { x = x + o.x; y = y + o.y; z = z + o.z; }
};
struct Node { char pad[0x30]; char m30[0x20]; V3 v50; char pad5C[4]; V3 v60; char pad6C[0xA0 - 0x6C]; V3 vA0; };
struct Inp { f32 x, y; int pad[2]; volatile int flags; int fl() { return flags; } };
struct Obj { char pad[0x14]; Node *s; char pad18[0x2C - 0x18]; Inp *in; V3 v30; f32 k3C; };
extern "C" void gl_func_00034458(...);
extern "C" f32 D_37540_19F8[], D_37540_19FC[], D_37540_1A00[];
inline void gv(Node *n, V3 &o) { if (0) { n = 0; } o.x = n->v60.x; o.y = n->v60.y; o.z = n->v60.z; }
inline void sx(Node *n, f32 v) { if (0) { n = 0; } n->v60.x = v; }
inline void sy(Node *n, f32 v) { if (0) { n = 0; } n->v60.y = v; }
inline void sz(Node *n, f32 v) { if (0) { n = 0; } n->v60.z = v; }
inline void call(Node *n, V3 *v, Obj *o) { if (0) { n = 0; } gl_func_00034458(n->m30, v, o); }
extern "C" void gl_func_00037540(Obj *o) {
    V3 save;
    Node *s = o->s;
    f32 a, b, ab;
    if (0) { gl_func_00034458(&s); }
    a = o->in->y * o->k3C;
    if (a < 0.0f) ab = -a; else ab = a;
    if (ab < 2.0f) a = 0.0f;
    b = o->in->x * D_37540_19F8[0x19F8 / 4];
    if (b < 0.0f) ab = -b; else ab = b;
    if (ab < D_37540_19FC[0x19FC / 4]) b = 0.0f;
    gv(s, save);
    sx(s, 0.0f); sy(s, 0.0f); sz(s, 0.0f);
    { V3 v1(0.0f, -b, 0.0f); call(s, &v1, o); }
    V3 vB4(s->v50.x * a, s->v50.y * a, s->v50.z * a);
    sx(s, save.x); sy(s, save.y); sz(s, save.z);
    o->v30.y = 0.0f;
    if (o->in->fl() & 4) o->v30.y = 10.0f;
    if (o->in->fl() & 8) o->v30.y = -10.0f;
    V3 v6C;
    v6C = vB4;
    V3 v9C;
    v9C.x = s->vA0.x; v9C.y = s->vA0.y; v9C.z = s->vA0.z;
    v9C += v6C;
    call(s, &o->v30, o);
    f32 L3 = D_37540_1A00[0x1A00 / 4];
    V3 v90;
    v90 = v9C;
    v90 += o->v30;
    v90 += v6C;
    o->v30.x = o->v30.x * L3;
    o->v30.z = o->v30.z * L3;
    call(s, &vB4, o);
}
