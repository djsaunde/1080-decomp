/* agent-e's additions to the snoweffects.uso C++ model (agent-e run 639), for the NCC donors in
 * snoweffects_uso_ncc_e.cpp (functions at 0x6000 and above). Builds on agent-h's snoweffects_ncc.h. */
#ifndef SNOWEFFECTS_NCC_E_H
#define SNOWEFFECTS_NCC_E_H

#include "snoweffects_ncc.h"

/* zero-code inline accessor: routes a pointer through one more edg inline level, which moves the
 * pool colouring of the temps that read it (the gfx_drawText kit, game_libs_ncc_46050.cpp) */
template <class T> inline T *me(T *p) { return p; }

/* the libgdl display-list context seen from the third TU (0x7E20..0x924C): same layout as DLCtx,
 * with the unnamed-index append and an inline one-packet emitter */
struct SnowDL {
    char pad[0xC];
    DLHead *dl;
    Gfx *next() { int n = dl->count++; Gfx *g = &dl->buf[n]; return g; }
    Gfx *nextr() { return &dl->buf[dl->count++]; }
    void put(unsigned int w0, unsigned int w1) { Gfx *g = next(); g->w0 = w0; g->w1 = w1; }
};

/* the third TU's trail emitter (display list at +0xB0, as SnowRing) */
struct SnowTrail {
    char pad00[0xB0];
    /* 0xB0 */ SnowDL *dl;
};

/* the libgdl Vec3 as these donors use it: field-wise copies, component-wise scale, inline normalize */
struct SnowVec3 {
    float x, y, z;
    SnowVec3 &operator*=(const SnowVec3 &o) { x *= o.x; y *= o.y; z *= o.z; return *this; }
    void normalize() { CALL(math_vec3Normalize)(this); }
};
inline float dot(const SnowVec3 &a, const SnowVec3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

/* a snow light / target object: position at +0x5C, an RGB colour parm value at +0x288
 * (Parm<Colour> at +0x278, value at +0x10; SnowTarget in snoweffects_uso_ncc_9F44.cpp) */
struct SnowLightObj {
    char pad00[0x5C];
    /* 0x5C */ SnowVec3 pos;
    char pad68[0x288 - 0x68];
    /* 0x288 */ SnowVec3 colour;
};

/* a libgdl HostIO parm (GdlParmBase header, value from +0x10), read through an inline get() */
template <class T> struct SnowParm { char hdr[0x10]; T v; const T &get() const { return v; } };

/* RGB colour as the target's floats plus a fourth word */
struct SnowColour { float r, g, b; int a; };

/* the SnowTarget node (snoweffects_uso_ncc_9F44.cpp): live parm values at +0x248 / +0x260 / +0x278 and
 * the copies the draw uses (+0x88 colour, +0x98 / +0x9C, +0xA0 position) */
struct SnowTargetE {
    char pad00[0x5C];
    /* 0x5C */ SnowVec3 pos;
    char pad68[0x88 - 0x68];
    /* 0x88 */ SnowColour colour;
    /* 0x98 */ float f98;
    /* 0x9C */ float f9C;
    /* 0xA0 */ SnowVec3 drawPos;
    char padAC[0x248 - 0xAC];
    /* 0x248 */ SnowParm<float> parm248;
    char pad25C[0x260 - 0x25C];
    /* 0x260 */ SnowParm<float> parm260;
    char pad274[0x278 - 0x274];
    /* 0x278 */ SnowParm<SnowColour> colourParm;
};

/* Data+0xA30: the module's "targets changed" flag (0x9768 sets 2, SnowLink_update 1; 0 pin, the USO
 * reloc carries the offset) */
extern "C" int snowDirtyFlag_A30;

/* typed blank jal for the sqrtf import (float in $f12, float out $f0) */
extern "C" float gl_func_00000000_snow_sqrtf(float);

/* the libgdl Vec3 as 0x9884 uses it: value temporaries (by-value operator-), in-place scale, length */
struct SnowLV {
    float x, y, z;
    SnowLV() {}
    SnowLV(float a, float b, float c) { x = a; y = b; z = c; }
    SnowLV &operator*=(const SnowLV &o) { x *= o.x; y *= o.y; z *= o.z; return *this; }
    float length() const { return XCALL(gl_func_00000000_snow_sqrtf, sqrtf)(x * x + y * y + z * z); }
};
inline SnowLV operator-(const SnowLV &a, const SnowLV &b) { return SnowLV(a.x - b.x, a.y - b.y, a.z - b.z); }

/* a MorphPoly mesh as 0x9884 reads it: vertex count +0x40, RGBA colour array +0x50 */
struct SnowMesh {
    char pad00[0x40];
    /* 0x40 */ unsigned int nverts;
    char pad44[0x50 - 0x44];
    /* 0x50 */ unsigned char (*cols)[4];
};

/* a snow light (0x9884): position +0x5C, up to 64 lit vertex indices (+0xAC) with their saved colours
 * (+0x12C, alpha = falloff), count +0x22C, MorphPoly update list +0x230, radius +0x258, height +0x270 */
struct SnowGlowE {
    char pad00[0x5C];
    /* 0x5C */ SnowLV pos;
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

#endif
