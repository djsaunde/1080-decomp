/* Native original-compiler source port from src/game_libs/game_libs_post0b.c. Target bytes unchanged. */
#include "context_game_libs_post0b.h"
void gl_func_00036224(char *o, char *arg1) {
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
    char *src;
    f32 x, y, z;
    f32 z1;
    Vec3 rv;
    f32 out1[3];
    f32 out2[3];
    Vec3 * volatile pB;
    GlQuaternion *pA;
    void *ap = &pA;
    (void)ap;
    tmp.v = *(Vec3 *)(*(char **)(o + 0x1CC) + 0xF8);
    vecA.p[0] = tmp.v.x; vecA.p[1] = tmp.v.y; vecA.p[2] = tmp.v.z;
    vecA.v.x = vecA.p[0] - 160.0f;
    vecA.v.y = vecA.p[1] - 120.0f;
    tmp.v = *(Vec3 *)(*(char **)(o + 0x1CC) + 0xBC);
    vecB.p[0] = tmp.v.x; vecB.p[1] = tmp.v.y; vecB.p[2] = tmp.v.z;
    vecB.v.x = vecB.p[0] - 160.0f;
    vecB.v.y = vecB.p[1] - 120.0f;
    vecB.v.y = vecB.v.y * -1.0f;
    vecA.v.y = vecA.v.y * -1.0f;
    if (*(int *)(o + 0x1D8) != 0) {
        vecB.v.x *= -1.0f;
        vecB.v.y *= -1.0f;
        vecB.v.z *= -1.0f;
        vecA.v.x *= -1.0f;
        vecA.v.y *= -1.0f;
        vecA.v.z *= -1.0f;
    }
    {
        rv = *gl_36224_direction(&tmp.v, o, vecA.v, *(Vec3 *)(o + 0x2C), *(f32 *)(o + 0x44));
        *(f32 *)(o + 0xA0) = rv.x;
        *(f32 *)(o + 0xA4) = rv.y;
        *(f32 *)(o + 0xA8) = rv.z;
        rv = *gl_36224_direction(&tmp.v, o, vecB.v, *(Vec3 *)(o + 0x2C), *(f32 *)(o + 0x44));
        *(f32 *)(o + 0xAC) = rv.x;
        *(f32 *)(o + 0xB0) = rv.y;
        *(f32 *)(o + 0xB4) = rv.z;
    }
    gl_36224_matrix((f32 (*)[4])(*(char **)(o + 0x1D0) + 0xB4), mtx);
    x = *(f32 *)(o + 0xA0);
    y = *(f32 *)(o + 0xA4);
    z = *(f32 *)(o + 0xA8);
    out1[0] = mtx[2][0] * z + (mtx[1][0] * y + mtx[0][0] * x);
    out1[1] = mtx[2][1] * z + (mtx[1][1] * y + mtx[0][1] * x);
    out1[2] = mtx[2][2] * z + (mtx[1][2] * y + mtx[0][2] * x);
    *(f32 *)(o + 0xA0) = out1[0];
    *(f32 *)(o + 0xA4) = out1[1];
    *(f32 *)(o + 0xA8) = out1[2];
    x1 = *(f32 *)(o + 0xAC);
    y1 = *(f32 *)(o + 0xB0);
    z1 = *(f32 *)(o + 0xB4);
    out2[0] = mtx[2][0] * z1 + (mtx[1][0] * y1 + mtx[0][0] * x1);
    out2[1] = mtx[2][1] * z1 + (mtx[1][1] * y1 + mtx[0][1] * x1);
    out2[2] = mtx[2][2] * z1 + (mtx[1][2] * y1 + mtx[0][2] * x1);
    *(f32 *)(o + 0xAC) = out2[0];
    *(f32 *)(o + 0xB0) = out2[1];
    *(f32 *)(o + 0xB4) = out2[2];
    src = *(char **)(o + 0x1CC);
    ap = *(char **)(src + 0xB4);
    if ((*(int *)(o + 0x1D4) & *(int *)((char *)ap + 0x10)) && (*(int *)(src + 0x108) != 0)) {
        pA = (GlQuaternion *)(o + 0x68);
        gl_36224_between(pA, (Vec3 *)(o + 0xA0), pB = (Vec3 *)(o + 0xAC));
        gl_36224_product(pA, (GlQuaternion *)(o + 0x58), (GlQuaternion *)(o + 0x48));
        gl_36224_between((GlQuaternion *)(o + 0x78), (Vec3 *)(o + 0xD0), pB);
        tmp.v = *pB;
        *(f32 *)(o + 0xD0) = tmp.v.x;
        *(f32 *)(o + 0xD4) = tmp.v.y;
        *(f32 *)(o + 0xD8) = tmp.v.z;
    } else {
        gl_36224_product((GlQuaternion *)(o + 0x78), (GlQuaternion *)(o + 0x48), (GlQuaternion *)(o + 0x48));
        tmp.v = *(Vec3 *)(o + 0x48);
        *(f32 *)(o + 0x58) = tmp.v.x;
        *(f32 *)(o + 0x5C) = tmp.v.y;
        *(f32 *)(o + 0x60) = tmp.v.z;
        *(f32 *)(o + 0x64) = *(f32 *)(o + 0x54);
    }
    gl_36224_conjugate((GlQuaternion *)(o + 0x48), (GlQuaternion *)&vecC);
    gl_36224_rotation((f32 (*)[4])arg1, (GlQuaternion *)&vecC);
    gl_36224_translate((f32 (*)[4])arg1, (Vec3 *)(o + 0x38));
}
