/* NCC (C++) near-miss DRAFTS for snoweffects.uso functions at 0x6000+ -- NOT built, NOT matching
 * (agent-e run 650; 0x9884 went EXACT in run 653, snoweffects_uso_ncc_e.cpp). The functions stay INCLUDE_ASM in snoweffects_uso.c (see the notes there).
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
void *mem_new(unsigned int);
}
/* the libgdl Vec3 as the 0x7960 draft uses it (value temporaries, inline helpers) */
struct SnowV {
    float x, y, z;
    SnowV() {}
    SnowV(float a, float b, float c) { x = a; y = b; z = c; }
    SnowV &operator*=(const SnowV &o) { x *= o.x; y *= o.y; z *= o.z; return *this; }
    SnowV operator*(float s) const { return SnowV(x * s, y * s, z * s); }
    SnowV &operator+=(const SnowV &v) { x += v.x; y += v.y; z += v.z; return *this; }
    float length() const { return sqrtf(x * x + y * y + z * z); }
};
inline SnowV operator-(const SnowV &a, const SnowV &b) { return SnowV(a.x - b.x, a.y - b.y, a.z - b.z); }
inline float dotv(const SnowV &a, const SnowV &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
/* 0x7960 (NM draft; run 653: 193 / 219 words, every instruction except the frame offsets and one FP
 * colour): trail head update. +0x148 = -dot(+0x114, +0x108) and its acosf gives the trail angle (negated
 * when the camera's +0x318 / +0x3C8 vectors point apart); when |speed| (+0x4C) is at least RoData+0xB4,
 * probe 0x52D4 at +0x138 and push the point back by -d * normal when 0 < d < +0x15C, then set the two
 * side points +0x120 / +0x12C from cos / sin of the angle times +0x3C / +0x40 and transform them by the
 * matrix at +0x30.
 * Run 653 layout levers (read from -O2 -g3 .mdebug): `ang` declared before `n` (n at vfp-16), no named
 * |speed| local (inline ?: expression), d in the outer if-block, a NAMED `push = n * -d` in the inner
 * block and `operator+=(const SnowV &)`: operator* result vfp-72, copy -60, push -44 = the target's.
 * Left: (1) the xform x/y/z homes sit 8 bytes too high (z spills -104 / -116, target -112 / -124) and
 * the frame is 160 vs 176. Two extra pool words fix the homes: a 5-argument SnowV ctor called as
 * SnowV(x * s, y * s, z * s, s, s) from operator* gives -112 / -124 with identical code (frame 168),
 * but that is a dummy-parameter lever, and 8 bytes below the second xform block are still missing.
 * (2) the |speed| value colours f12 and the zero f2; the target has them swapped. Pins a donor needs:
 * typed float-return 0 aliases for 0x52D4 and acosf / fcos / fsin, and RoData+0xB4. */
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
    float ang;
    SnowV n;
    ang = math_acosf(t->dist = -dotv(t->p, t->n));
    if (dotv(t->cam->fwd, t->cam->dir) < 0.0f) {
        ang = -ang;
    }
    t->hits = 0;
    if (!((t->speed < 0.0f ? -t->speed : t->speed) < 0.1f)) {
        float d = snoweffects_uso_func_000052D4(t, &t->pos, &n, t->qD4);
        if (d != 0.0f && d < t->maxDist) {
            SnowV push = n * -d;
            t->pos += push;
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

/* 0x9328 (NM draft, run 653, 78 / 247 words): the SnowTarget Cfront constructor, written as real C++
 * (edg's `this == 0 -> operator new` protocol comes out of the inline ctors by itself). SNode base
 * (out of line, then vptr +0x28 = Data+0), a Parms member at +0x240 (0x8C bytes, an 8-byte list head
 * {Data+0xA34, 0} with its own new(8) fallback), then five inline Parm ctors over GdlParmBase
 * (= Parms_addParm(this, owner, name by value, flag), vptr at +0xC): float 40 (name Data+0xA44, vtbl
 * Data+0x838), float 10 (0xA48), Colour value / min / max (1,1,1,1) / (0,0,0,0) / (1,1,1,1) passed by
 * value (0xA4C, vtbl 0x850; only the value is stored), int 0 flag 0 (0xA50, vtbl 0x868), int 3 (0xA54).
 * Body: name (+0xC) = Data+0xA58, +0x22C..+0x23C = 0, 0, 1, 3, 0, colour +0x88 = 1 x4, +0x98..+0xA8 = 0.
 * Left: the target keeps the Parms `this` in a1 (spilled to sp+0xDC around every call, only s0 saved)
 * where this keeps it in s1, and every Parm's name copy has its own frame word (sp+0xD8 / 0xD4 / 0xA0 /
 * 0x9C / 0x98, outgoing copy 0x84); frame 224 vs 168. */
struct PName { char *s; };
struct Colour { float r, g, b, a; Colour() {} Colour(float r_, float g_, float b_, float a_) { r = r_; g = g_; b = b_; a = a_; } };
struct Parms9;
struct GdlParmBase9 {
    char hdr[0xC];
    GdlParmBase9(Parms9 *o, PName n, int fl);   /* Parms_addParm */
    virtual void v1();
    void *operator new(unsigned n) { return mem_new(n); }
};
struct ParmF9 : GdlParmBase9 {
    float v; int f14;
    ParmF9(Parms9 *o, PName n, float val, int fl) : GdlParmBase9(o, n, fl) { v = val; f14 = 0; }
    virtual void v1();
};
struct ParmI9 : GdlParmBase9 {
    int v; int f14;
    ParmI9(Parms9 *o, PName n, int val, int fl) : GdlParmBase9(o, n, fl) { v = val; f14 = 0; }
    virtual void v1();
};
struct ParmC9 : GdlParmBase9 {
    Colour v; int f20;
    ParmC9(Parms9 *o, PName n, Colour val, Colour lo, Colour hi, int fl) : GdlParmBase9(o, n, fl) { v = val; f20 = 0; }
    virtual void v1();
};
struct ParmsHead9 {
    char *vt;
    int count;
    ParmsHead9() { vt = D_00000000 + 0xA34; count = 0; }
    void *operator new(unsigned n) { return mem_new(n); }
};
#define NM9(off) (*(PName *)(D_00000000 + (off)))
struct Parms9 : ParmsHead9 {
    ParmF9 p08;
    ParmF9 p20;
    ParmC9 p38;
    ParmI9 p5C;
    ParmI9 p74;
    Parms9() : p08(this, NM9(0xA44), 40.0f, 1), p20(this, NM9(0xA48), 10.0f, 1),
               p38(this, NM9(0xA4C), Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 0.0f),
                   Colour(1.0f, 1.0f, 1.0f, 1.0f), 1),
               p5C(this, NM9(0xA50), 0, 0), p74(this, NM9(0xA54), 3, 1) {}
    void *operator new(unsigned n) { return mem_new(n); }
};
struct SNode9 {
    char pad00[0xC];
    char *name;
    char pad10[0x28 - 0x10];
    SNode9();                                     /* SNode_ctor */
    virtual void v1();
    void *operator new(unsigned n) { return mem_new(n); }
};
struct SnowTarget9 : SNode9 {
    char pad2c[0x88 - 0x2C];
    /* 0x88 */ float col[4];
    /* 0x98 */ float f98, f9C, fA0, fA4, fA8;
    char padAC[0x22C - 0xAC];
    int f22C, f230, f234, f238, f23C;
    /* 0x240 */ Parms9 parms;
    SnowTarget9();
    virtual void v1();
};
SnowTarget9::SnowTarget9() {
    name = D_00000000 + 0xA58;
    f22C = 0; f230 = 0; f234 = 1; f238 = 3; f23C = 0;
    col[0] = 1.0f; col[1] = 1.0f; col[2] = 1.0f; col[3] = 1.0f;
    f98 = 0.0f; f9C = 0.0f; fA0 = 0.0f; fA4 = 0.0f; fA8 = 0.0f;
}
