#include "../../include/uso_call.h"  /* NCC donors build without -I include */
/* IDO 7.1 NCC (C++) donor for SkinAnim_bind (0x2FC / 191 words): vertex transform. EXACT; spliced
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
struct Pr { s16 vert, weight; };
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
/* Local views of include/gdl/skin.h with its field names (agent-b run 489): Pr = SkinWeight,
 * Bone = the MNode (world matrix at +0x70), Mgr = SkinMgr, Obj = SkinAnim. */
struct Bone { char pad[0x70]; M4 mtx; };
struct Mgr { char pad[0x30]; s16 *weights; char pad34[4]; Bone *bone; };   /* SkinMgr: main-bone weights, main bone */
struct Obj { char pad[0x18]; Bone *bone; Pr *weights; int count; V3s *bindVerts; M4 invBone; M4 boneAtBind; };
/* Take this SkinAnim's vertices away from the main bone (weights -= its share) and store them in
 * this bone's space: bindVerts[i] = invBone * mainBone * mgrBindVerts[vert] */
extern "C" void SkinAnim_bind(Obj *o, Mgr *t, V3s *mgrBindVerts) {
    int i;
    V3 v;
    for (i = 0; i < o->count; i++) {
        t->weights[o->weights[i].vert] -= o->weights[i].weight;
    }
    o->bindVerts = (V3s *)CALL(mem_new)(o->count * sizeof(V3s));
    o->boneAtBind = o->bone->mtx;
    CALL(math_mtx4Inverse)(&o->bone->mtx, &o->invBone);           /* invert */
    for (i = 0; i < o->count; i++) {
        V3s *s = &mgrBindVerts[o->weights[i].vert];
        v.x = s->x; v.y = s->y; v.z = s->z;
        t->bone->mtx.xf(v);
        o->invBone.xf(v);
        o->bindVerts[i].x = v.x;
        o->bindVerts[i].y = v.y;
        o->bindVerts[i].z = v.z;
    }
}
