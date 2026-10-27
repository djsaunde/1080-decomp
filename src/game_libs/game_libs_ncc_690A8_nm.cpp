/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for gl_func_000690A8 (0x2FC / 191 words).
 * NM object only (build/non_matching); the ROM keeps INCLUDE_ASM.
 * agent-g run 182 (docs/IDO_CODEGEN.md kit CXXXI): the cfe body (86.90) needed scrambled operand orders
 * to approximate the transform; it is an inline M4 member applied to a stack V3 by reference: the
 * target's `lwc1 112(v0); lwc1 128(v0); addiu v0,v0,112` is the matrix `this` materialised after its first
 * two folded loads (kit CXXIX this-adjust shape), and v[] is ONE memory object updated in place by both
 * transforms. Natural row-vector sums (m[0][0]*x + m[1][0]*y + m[2][0]*z + m[3][0]) give every FP
 * instruction. 191 words = target.
 * LEFT: frame 56 vs 48 -- the target homes only i and v; the compound `-=` in loop 1 makes edg home its
 * lvalue address (4 bytes -> +8 frame), while the plain `x = x - b` spelling keeps the frame but swaps
 * v0 / v1 between the pair pointer and the destination address. */
typedef float f32;
typedef short s16;
extern "C" void *gl_func_00000000(...);
struct V3s { s16 x, y, z; };
struct Pr { s16 a, b; };
struct V3 { f32 x, y, z; };
struct M4 {
    f32 m[4][4];
    void xf(V3 &v) const {
        f32 x = m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0];
        f32 y = m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1];
        f32 z = m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2];
        v.x = x; v.y = y; v.z = z;
    }
};
struct Body { char pad[0x70]; M4 mtx; };
struct Tab { char pad[0x30]; s16 *s; char pad34[4]; Body *body; };
struct Obj { char pad[0x18]; Body *src; Pr *pairs; int count; V3s *out; M4 m28; M4 m68; };
extern "C" void gl_func_000690A8(Obj *o, Tab *t, V3s *arg2) {
    int i;
    for (i = 0; i < o->count; i++) {
        t->s[o->pairs[i].a] -= o->pairs[i].b;
    }
    o->out = (V3s *)gl_func_00000000(o->count * sizeof(V3s));
    o->m68 = o->src->mtx;
    gl_func_00000000(&o->src->mtx, &o->m28);
    for (i = 0; i < o->count; i++) {
        V3s *s = &arg2[o->pairs[i].a];
        V3 v;
        v.x = s->x; v.y = s->y; v.z = s->z;
        t->body->mtx.xf(v);
        o->m28.xf(v);
        o->out[i].x = v.x;
        o->out[i].y = v.y;
        o->out[i].z = v.z;
    }
}
