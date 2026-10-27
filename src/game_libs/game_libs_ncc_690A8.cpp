/* IDO 7.1 NCC (C++) donor for gl_func_000690A8 (0x2FC / 191 words): vertex transform. EXACT; spliced
 * into BOTH build trees via REPLACE_FUNC_BODY over the INCLUDE_ASM body (Makefile GAMELIBS_690A8_DONOR).
 * agent-g run 182 (docs/IDO_CODEGEN.md kit CXXXI): the cfe body (86.90) needed scrambled operand orders
 * to approximate the transform; it is an inline M4 member applied to a stack V3 by reference: the
 * target's `lwc1 112(v0); lwc1 128(v0); addiu v0,v0,112` is the matrix `this` temp materialised after its
 * first two folded loads (kit CXXIX this-adjust shape), and v is ONE memory object updated in place by
 * both transforms; natural row-vector sums give every FP instruction.
 * Frame: v is declared at function scope, BEFORE loop 1. uopt only reserves frame bytes down to the
 * lowest memory-resident variable; register-coloured edg temporaries (the `-=` lvalue address of loop 1,
 * the xf `this`) cost nothing when they are allocated BELOW v, but the loop-1 temp sits above a v declared
 * inside loop 2 and pushes v (and the frame) down by 8 (99.91). The call targets are blank imports
 * (`jal 0`): gl_func_00000000 is pinned to 0. */
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
    V3 v;
    for (i = 0; i < o->count; i++) {
        t->s[o->pairs[i].a] -= o->pairs[i].b;
    }
    o->out = (V3s *)gl_func_00000000(o->count * sizeof(V3s));
    o->m68 = o->src->mtx;
    gl_func_00000000(&o->src->mtx, &o->m28);
    for (i = 0; i < o->count; i++) {
        V3s *s = &arg2[o->pairs[i].a];
        v.x = s->x; v.y = s->y; v.z = s->z;
        t->body->mtx.xf(v);
        o->m28.xf(v);
        o->out[i].x = v.x;
        o->out[i].y = v.y;
        o->out[i].z = v.z;
    }
}
