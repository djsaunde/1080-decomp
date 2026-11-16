/* Native original-compiler source port from src/game_libs/game_libs_post0b.c. Target bytes unchanged. */
#include "context_game_libs_post0b.h"
#include "gdl/ballrot.h"
#include "gdl/pad.h"
void BallRotator_applyArcball(char *o, char *arg1) {
    extern Vec3 *gl_36224_direction(Vec3 *, char *, Vec3, Vec3, f32);
    extern void gl_36224_matrix(f32 (*)[4], f32 (*)[4]);
    extern void gl_36224_between(GlQuaternion *, Vec3 *, Vec3 *);
    extern void gl_36224_product(GlQuaternion *, GlQuaternion *, GlQuaternion *);
    extern void gl_36224_conjugate(GlQuaternion *, GlQuaternion *);
    extern void gl_36224_rotation(f32 (*)[4], GlQuaternion *);
    extern void gl_36224_translate(f32 (*)[4], Vec3 *);
    union { Vec3 v; f32 p[3]; } vecA;
    union { Vec3 v; f32 p[3]; } vecB;
    f32 mtx[4][4];
    f32 vecC[4];
    f32 x1, y1;
    union { Vec3 v; f32 pad[3]; } tmp;
    GdlInput *src;
    f32 x, y, z;
    f32 z1;
    Vec3 rv;
    f32 out1[3];
    f32 out2[3];
    Vec3 * volatile pB;
    GlQuaternion *pA;
    void *ap = &pA;
    (void)ap;
    tmp.v = *(Vec3 *)BALLROT(o)->input->curPoint;
    vecA.p[0] = tmp.v.x; vecA.p[1] = tmp.v.y; vecA.p[2] = tmp.v.z;
    vecA.v.x = vecA.p[0] - 160.0f;
    vecA.v.y = vecA.p[1] - 120.0f;
    tmp.v = *(Vec3 *)BALLROT(o)->input->prevPoint;
    vecB.p[0] = tmp.v.x; vecB.p[1] = tmp.v.y; vecB.p[2] = tmp.v.z;
    vecB.v.x = vecB.p[0] - 160.0f;
    vecB.v.y = vecB.p[1] - 120.0f;
    vecB.v.y = vecB.v.y * -1.0f;
    vecA.v.y = vecA.v.y * -1.0f;
    if (BALLROT(o)->invert != 0) {
        vecB.v.x *= -1.0f;
        vecB.v.y *= -1.0f;
        vecB.v.z *= -1.0f;
        vecA.v.x *= -1.0f;
        vecA.v.y *= -1.0f;
        vecA.v.z *= -1.0f;
    }
    {
        rv = *gl_36224_direction(&tmp.v, o, vecA.v, *(Vec3 *)BALLROT(o)->center, BALLROT(o)->radius);
        BALLROT(o)->hitCur[0] = rv.x;
        BALLROT(o)->hitCur[1] = rv.y;
        BALLROT(o)->hitCur[2] = rv.z;
        rv = *gl_36224_direction(&tmp.v, o, vecB.v, *(Vec3 *)BALLROT(o)->center, BALLROT(o)->radius);
        BALLROT(o)->hitPrev[0] = rv.x;
        BALLROT(o)->hitPrev[1] = rv.y;
        BALLROT(o)->hitPrev[2] = rv.z;
    }
    gl_36224_matrix((f32 (*)[4])(BALLROT(o)->camera + 0xB4), mtx);
    x = BALLROT(o)->hitCur[0];
    y = BALLROT(o)->hitCur[1];
    z = BALLROT(o)->hitCur[2];
    out1[0] = mtx[2][0] * z + (mtx[1][0] * y + mtx[0][0] * x);
    out1[1] = mtx[2][1] * z + (mtx[1][1] * y + mtx[0][1] * x);
    out1[2] = mtx[2][2] * z + (mtx[1][2] * y + mtx[0][2] * x);
    BALLROT(o)->hitCur[0] = out1[0];
    BALLROT(o)->hitCur[1] = out1[1];
    BALLROT(o)->hitCur[2] = out1[2];
    x1 = BALLROT(o)->hitPrev[0];
    y1 = BALLROT(o)->hitPrev[1];
    z1 = BALLROT(o)->hitPrev[2];
    out2[0] = mtx[2][0] * z1 + (mtx[1][0] * y1 + mtx[0][0] * x1);
    out2[1] = mtx[2][1] * z1 + (mtx[1][1] * y1 + mtx[0][1] * x1);
    out2[2] = mtx[2][2] * z1 + (mtx[1][2] * y1 + mtx[0][2] * x1);
    BALLROT(o)->hitPrev[0] = out2[0];
    BALLROT(o)->hitPrev[1] = out2[1];
    BALLROT(o)->hitPrev[2] = out2[2];
    src = BALLROT(o)->input;
    ap = src->pad;
    /* *(int *)&...->held: the plain member read changes the bytes (byte lever) */
    if ((BALLROT(o)->buttonMask & *(int *)&((PadCtl *)ap)->held) && (src->enable != 0)) {
        pA = (GlQuaternion *)BALLROT(o)->dragRot;
        gl_36224_between(pA, (Vec3 *)BALLROT(o)->hitCur, pB = (Vec3 *)BALLROT(o)->hitPrev);
        gl_36224_product(pA, (GlQuaternion *)BALLROT(o)->prevRot, (GlQuaternion *)BALLROT(o)->rot);
        gl_36224_between((GlQuaternion *)BALLROT(o)->baseRot, (Vec3 *)BALLROT(o)->lastHit, pB);
        tmp.v = *pB;
        BALLROT(o)->lastHit[0] = tmp.v.x;
        BALLROT(o)->lastHit[1] = tmp.v.y;
        BALLROT(o)->lastHit[2] = tmp.v.z;
    } else {
        gl_36224_product((GlQuaternion *)BALLROT(o)->baseRot, (GlQuaternion *)BALLROT(o)->rot, (GlQuaternion *)BALLROT(o)->rot);
        tmp.v = *(Vec3 *)BALLROT(o)->rot;
        BALLROT(o)->prevRot[0] = tmp.v.x;
        BALLROT(o)->prevRot[1] = tmp.v.y;
        BALLROT(o)->prevRot[2] = tmp.v.z;
        BALLROT(o)->prevRot[3] = BALLROT(o)->rot[3];
    }
    gl_36224_conjugate((GlQuaternion *)BALLROT(o)->rot, (GlQuaternion *)&vecC);
    gl_36224_rotation((f32 (*)[4])arg1, (GlQuaternion *)&vecC);
    gl_36224_translate((f32 (*)[4])arg1, (Vec3 *)BALLROT(o)->offset);
}
