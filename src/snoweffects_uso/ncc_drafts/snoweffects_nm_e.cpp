/* NCC (C++) near-miss DRAFTS for snoweffects.uso functions at 0x6000+ -- NOT built, NOT matching
 * (agent-e run 650). The functions stay INCLUDE_ASM in snoweffects_uso.c (see the notes there).
 *
 * Compile standalone with IDO 7.1 NCC from the project root:
 *   tools/ido-static-recomp/build/7.1/out/NCC -c -G 0 -non_shared -Wab,-r4300_mul -O2 -mips2 -32
 *       -I src/snoweffects_uso -o /tmp/x.o <this file>
 * and diff each function against expected/src/snoweffects_uso/snoweffects_uso.c.o.
 */
#include "snoweffects_ncc_e.h"
extern "C" float sqrtf(float);
/* the callees, by name (a matching donor would call them through typed 0-pinned aliases) */
extern "C" {
float snoweffects_uso_func_000052D4(void *self, void *pos, void *out, void *a3);
float math_acosf(float);
float fcos(float);
float fsin(float);
void MorphPoly_applyUpdateList_0004A9F0(void *, void *, void *);
void mem_delete(void *);
void *PolyShape_getVertex(void *, unsigned int);
void *MorphPoly_genUpdateList(void *, int, short *);
}
/* the libgdl Vec3 as the 0x7960 / 0x9884 drafts use it (value temporaries, inline helpers) */
struct SnowV {
    float x, y, z;
    SnowV() {}
    SnowV(float a, float b, float c) { x = a; y = b; z = c; }
    SnowV &operator*=(const SnowV &o) { x *= o.x; y *= o.y; z *= o.z; return *this; }
    SnowV operator*(float s) const { return SnowV(x * s, y * s, z * s); }
    SnowV &operator+=(SnowV v) { x += v.x; y += v.y; z += v.z; return *this; }
    float length() const { return sqrtf(x * x + y * y + z * z); }
};
inline SnowV operator-(const SnowV &a, const SnowV &b) { return SnowV(a.x - b.x, a.y - b.y, a.z - b.z); }
inline float dotv(const SnowV &a, const SnowV &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
/* 0x9884 (NM draft, 203 / 217 words): rebuild a snow light's vertex list. Restore the saved vertex
 * colours and free the old MorphPoly update list, then scale the light position by gGfxLib's scale
 * (bootup Bss+0x3C168 + 0x128), lift it by +0x270 and collect up to 64 mesh vertices with
 * 0.1 < |pos - v| < radius (+0x258): index into +0xAC, falloff (1 - d / radius) * 255 into the alpha
 * of +0x12C, then MorphPoly_genUpdateList. Left: the target's light position local sits at sp+0xB8
 * under the vertex local (sp+0xC4), with 16 unreferenced bytes above it; here it takes the top slot
 * (sp+0xD0). The 0.1f literal is RoData+0xF0 (needs a `snoweffects_uso_func_00009884_rodata` pin). */
struct SnowMesh {
    char pad00[0x40];
    /* 0x40 */ unsigned int nverts;
    char pad44[0x50 - 0x44];
    /* 0x50 */ unsigned char (*cols)[4];
};
struct SnowGlowE {
    char pad00[0x5C];
    /* 0x5C */ SnowV pos;
    char pad68[0xAC - 0x68];
    /* 0x0AC */ short vtx[0x40];
    /* 0x12C */ unsigned char col[0x40][4];
    /* 0x22C */ int n;
    /* 0x230 */ void *updateList;
    char pad234[0x258 - 0x234];
    /* 0x258 */ float radius;
    char pad25C[0x270 - 0x25C];
    /* 0x270 */ float yofs;
};
extern "C" void snoweffects_uso_func_00009884(SnowGlowE *t, SnowMesh *m) {
    unsigned char (*cols)[4] = m->cols;
    if (t->updateList != 0) {
        for (int i = 0; i < t->n; i++) {
            int idx = t->vtx[i];
            cols[idx][0] = t->col[i][0];
            cols[idx][1] = t->col[i][1];
            cols[idx][2] = t->col[i][2];
        }
        MorphPoly_applyUpdateList_0004A9F0(m, m->cols, t->updateList);
        mem_delete(t->updateList);
    }
    t->updateList = 0;
    t->n = 0;
    SnowV p;
    SnowV vf;
    p.x = t->pos.x;
    p.y = t->pos.y;
    p.z = t->pos.z;
    SnowV *gp = (SnowV *)(D_00000000 + 0x128);
    const SnowV &g = *gp;
    p *= g;
    p.y += t->yofs;
    for (unsigned int i = 0; i < m->nverts; i++) {
        short *v = (short *)PolyShape_getVertex(m, i);
        vf.x = v[0];
        vf.y = v[1];
        vf.z = v[2];
        SnowV dd = p - vf;
        float len = dd.length();
        if (0.1f < len && len < t->radius) {
            float f = 1.0f - len / t->radius;
            t->vtx[t->n] = i;
            t->col[t->n][3] = (unsigned int)(f * 255.0f);
            t->n++;
            if (t->n == 64) {
                i = m->nverts;
            }
        }
    }
    if (t->n > 0) {
        t->updateList = MorphPoly_genUpdateList(m, t->n, t->vtx);
    }
}

/* 0x7960 (NM draft, 194 / 219 words): trail head update. +0x148 = -dot(+0x114, +0x108) and its acosf
 * gives the trail angle (negated when the camera's +0x318 / +0x3C8 vectors point apart); when the speed
 * (+0x4C) is at least RoData+0xB4, probe 0x52D4 at +0x138 and push the point back by -d * normal when
 * 0 < d < +0x15C, then set the two side points +0x120 / +0x12C from cos / sin of the angle times
 * +0x3C / +0x40 and transform them by the matrix at +0x30. Left: the frame (target 176, here 168;
 * the normal at sp+0xA0 and the push temporaries at 0x68 / 0x74 / 0x84) and the zero / fabs
 * registers (f12 / f2 swapped). Pins a donor needs: typed float-return 0 aliases for 0x52D4 and
 * acosf / fcos / fsin, and RoData+0xB4. */
struct M44 { float m[4][4]; };
inline void xform(SnowV &v, const M44 &m) { float x = m.m[0][0] * v.x + m.m[1][0] * v.y + m.m[2][0] * v.z + m.m[3][0]; float y = m.m[0][1] * v.x + m.m[1][1] * v.y + m.m[2][1] * v.z + m.m[3][1]; float z = m.m[0][2] * v.x + m.m[1][2] * v.y + m.m[2][2] * v.z + m.m[3][2]; v.x = x; v.y = y; v.z = z; }
struct SnowCam { char pad[0x318]; SnowV fwd; char pad324[0x3C8 - 0x324]; SnowV dir; };
struct SnowBody7 {
    char pad00[0x2C];
    /* 0x2C */ SnowCam *cam;
    /* 0x30 */ M44 *mtx;
    char pad34[0x3C - 0x34];
    /* 0x3C */ float w;
    /* 0x40 */ float h;
    char pad44[0x4C - 0x44];
    /* 0x4C */ float speed;
    char pad50[0xD4 - 0x50];
    /* 0xD4 */ int qD4[4];
    char padE4[0x108 - 0xE4];
    /* 0x108 */ SnowV n;
    /* 0x114 */ SnowV p;
    /* 0x120 */ SnowV a;
    /* 0x12C */ SnowV b;
    /* 0x138 */ SnowV pos;
    /* 0x144 */ int hits;
    /* 0x148 */ float dist;
    char pad14C[0x15C - 0x14C];
    /* 0x15C */ float maxDist;
};
extern "C" void snoweffects_uso_func_00007960(SnowBody7 *t) {
    SnowV n;
    float ang = math_acosf(t->dist = -dotv(t->p, t->n));
    if (dotv(t->cam->fwd, t->cam->dir) < 0.0f) {
        ang = -ang;
    }
    t->hits = 0;
    float s = t->speed < 0.0f ? -t->speed : t->speed;
    if (!(s < 0.1f)) {
        float d = snoweffects_uso_func_000052D4(t, &t->pos, &n, t->qD4);
        if (d != 0.0f && d < t->maxDist) {
            t->pos += n * -d;
            t->hits = 2;
            t->a.x = fcos(ang) * t->w;
            t->a.y = 0.0f;
            t->a.z = fsin(ang) * t->h;
            xform(t->a, *t->mtx);
            t->b.x = fcos(ang) * -t->w;
            t->b.y = 0.0f;
            t->b.z = fsin(ang) * -t->h;
            xform(t->b, *t->mtx);
        }
    }
}
