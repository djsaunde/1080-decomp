/* MIPSpro 7.2.1 C++ (tools/ncc721.sh) NON_MATCHING donor for heli.uso 0xB08 (agent-g run 672), spliced
 * over the empty NON_MATCHING placeholder in heli_uso.c on the non_matching build only (the matching build
 * keeps INCLUDE_ASM). 0xB08 is the Heli per-frame update (decode: docs/1080_NEW_MODULES.md "heli.uso").
 * MIPSpro 7.2.1 is the right front end: the target's 8-way switch is an inline jump table (RoData+0x7C),
 * which IDO 7.1 NCC (EDG 2.19) never emits. Levers that took mp72 from 77.3 to 88.7:
 *  - an explicit empty `default: break;` after case 6. The default edge then gets its own block, so uopt
 *    computes tg / ps (self+0x114 / self+0xDC) at the end of every case, as the target does; without it the
 *    default goes straight to the join and both are computed once there.
 *  - `mode = mode * 1;` before the switch (code-free; mode takes v0 as in the target);
 *  - the marker field is read back through self (`*(self + 0x168)`) after the advance;
 *  - `Vec3 old = *FWD;` copy-initialised (a word copy, no operator= by-value temp);
 *  - `f *= HF(0x154) / h` with `int h = 100` (run 678): with two 100.0f literals uopt keeps one constant LR
 *    (f14) for the compare and the divide; the int is converted late, so each site rematerialises
 *    `lui at,0x42c8` as the target does (84.2 -> 85.7).
 *  - the three axis-angle angles go through the float local f, assigned before each axis (run 682): the target
 *    loads each angle with lwc1 + mfc1 a2 ahead of the axis stores (a direct HF() argument gives lw a2;
 *    a block-scope float costs frame slots) (85.7 -> 88.2).
 *  - run 682 also: the pitch clamp as an inline clampf() on f re-read from +0x148, and the toTgt length summed
 *    y, x, z (the y-first order only rotates the FP temp ring closer to the target's; the target multiplies
 *    x first) (88.2 -> 88.7).
 * Left: register numbering, the slerp's heading pointer (target keeps self+0x138 in a spilled register), the
 * frame (temps sit between the named locals in the target). RoData floats are literals. */
extern "C" {
#include "../../include/common.h"  /* NCC donors build without -I include */
#include "../../include/uso_call.h"
#include "../../include/gdl/node.h"
#include "../../include/gdl/mnode.h"
#include "../../include/gdl/item.h"

extern int gl_func_00000000(...);
extern char D_00000000;

extern char heli_data_00000000;
extern void gl_func_00000000_heli_ppf(void *, void *, float);         /* math_quatRotateAxisAngle */
/* the 4-byte ItemType name tag, passed by value (lands in a1 and the a1 home slot) */
typedef struct HeliTypeTag { int v; } HeliTypeTag;
extern char heli_flow_b08;      /* bootup Bss+0xF8 (GameFlow); +0x138 the course state */
extern char *heli_itemMgr_b08;  /* bootup Data+0x776C */
extern char *heli_itemMgr2_b08; /* bootup Data+0x7768 */
extern char heli_gGfxLib_d;
extern float gl_func_00000000_heli_ff(float);
extern float gl_func_00000000_heli_fff(float, float);
#define HF(off) (*(float *)(self + (off)))
#define HI(off) (*(int *)(self + (off)))
#define HV(off) (*(Vec3 *)(self + (off)))

}
static inline float clampf(float v, float lo, float hi) { return (v < lo) ? lo : ((hi < v) ? hi : v); }
struct Vec3 {
    float x, y, z;
    Vec3() {}
    Vec3(float a, float b, float c) { x = a; y = b; z = c; }
    Vec3 operator-(const Vec3 &v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    Vec3 operator*(float s) const { return Vec3(x * s, y * s, z * s); }
    Vec3 &operator=(Vec3 v) { x = v.x; y = v.y; z = v.z; return *this; }
    void operator+=(const Vec3 &v) { x += v.x; y += v.y; z += v.z; }
};
#define V3(off) (*(Vec3 *)(self + (off)))
#define FWD ((Vec3 *)(self + 0x138))
extern "C" void heli_uso_func_00000B08(char *self) {
    struct { float x, y, z; } ppr;
    char *player;
    char *q;
    Vec3 toTgt;
    Vec3 dir;
    float dist;
    Vec3 side;
    HeliTypeTag key;
    HeliTypeTag key2;
    Vec3 tmp;
    Vec3 mpos;
    Vec3 *tg;
    Vec3 *ps;
    float t;
    float f;
    float yaw;
    float ang;
    float s;
    float sa;
    char *p;
    int mode;
    Vec3 fwdAx;
    Vec3 axZ;
    Vec3 axX;
    Vec3 axY;
    p = *(char **)(&heli_flow_b08 + 0x138);
    if (p == 0 && *(int *)(p + 0x48) == 0) {
        return;
    }
    player = **(char ***)(p + 0x44);
    ppr.x = *(float *)(player + 0xA0);
    ppr.y = *(float *)(player + 0xA4);
    ppr.z = *(float *)(player + 0xA8);
    if (HI(0x170) != 0) {
        mode = HI(0x170);
    } else {
        if (*(int *)(player + 0xA54) == 6) {
            gl_func_00000000(self);
            HI(0x170) = 8;
        }
        if (*(char **)(self + 0x168) == 0) {
            key.v = *(int *)(&heli_data_00000000 + 0xCBAC);
            p = (char *)gl_func_00000000(heli_itemMgr_b08, key);
            if (p != 0) {
                char *vt = ((Node *)(p))->vptr;
                *(char **)(self + 0x168) = (char *)((int (*)(...))(((GdlVtEnt *)((char *)(vt) + 8))[12]).fn)((((GdlVtEnt *)((char *)(vt) + 8))[12]).delta + p, &(*(Vec3 *)&ppr), 0);
            }
        }
        p = *(char **)(self + 0x168);
        if (p != 0) {
            q = *(char **)(p + 0x2C);
            if (q != 0 && *(float *)(q + 0x38) < (*(Vec3 *)&ppr).z) {
                *(char **)(self + 0x168) = q;
            }
            HI(0x16C) = *(int *)(*(char **)(self + 0x168) + 0x3C);
        }
        mode = HI(0x16C);
    }
    tg = (Vec3 *)(self + 0x114);
    ps = (Vec3 *)(self + 0xDC);
    mode = mode * 1;
    switch (mode) {
    case 8:
        HF(0x150) = 1.0f;
        break;
    case 3:
        if (HF(0xE4) < (*(Vec3 *)&ppr).z + 4000.0f) {
            HI(0x16C) = 1;
        }
        break;
    case 4:
        f = (5000.0f);
        V3(0x114) = (*(Vec3 *)&ppr);
        HF(0x118) += f;
        HF(0x11C) += f;
        HF(0x15C) = 1000.0f;
        HF(0x158) = 1.0f;
        HF(0x150) = 800.0f;
        V3(0x12C) = *tg;
        break;
    case 7:
        V3(0x114) = V3(0x108);
        HF(0x15C) = 250.0f;
        HF(0x158) = 1.0f;
        HF(0x150) = 200.0f;
        V3(0x12C) = (*(Vec3 *)&ppr);
        break;
    case 1:
        V3(0x114) = (*(Vec3 *)&ppr);
        V3(0x12C) = *tg;
        HF(0x114) = HF(0x114) + 0.0f;
        HF(0x118) = HF(0x118) + 600.0f;
        HF(0x11C) = HF(0x11C) + (3000.0f);
        HF(0x15C) = 250.0f;
        HF(0x158) = 2.0f;
        HF(0x150) = 1000.0f;
        break;
    case 5:
        V3(0x114) = (*(Vec3 *)&ppr);
        V3(0x12C) = *tg;
        HF(0x114) = HF(0x114) + 0.0f;
        HF(0x118) = HF(0x118) + -368.0f;
        HF(0x11C) = HF(0x11C) + (3000.0f);
        HF(0x15C) = 180.0f;
        HF(0x158) = 2.0f;
        HF(0x150) = 500.0f;
        HF(0x160) = 400.0f;
        break;
    case 2:
        key2.v = *(int *)(&heli_data_00000000 + 0xCBB0);
        p = (char *)gl_func_00000000(heli_itemMgr2_b08, key2, ps, 0);
        if (p != 0 && *(int *)(p + 0x2C) != 0) {
            p = *(char **)(p + 0x2C);
            mpos = *(Vec3 *)(p + 0x30);
            mpos.y += (3000.0f);
            HF(0x114) = HF(0x114) + (mpos.x - HF(0x114)) * (0.2);
            HF(0x118) = HF(0x118) + (mpos.y - HF(0x118)) * (0.2);
            HF(0x11C) = HF(0x11C) + (mpos.z - HF(0x11C)) * (0.2);
            V3(0x12C) = V3(0x114);
            HF(0x15C) = 0.0f;
            HF(0x158) = 2.0f;
            HF(0x150) = *(float *)(player + 0x348) + 5.0f;
            if (HF(0x150) < 100.0f) {
                HF(0x150) = 100.0f;
            }
        }
        break;
    case 6:
        V3(0x114) = (*(Vec3 *)&ppr);
        break;
    default:
        break;
    }
    dir = *tg - *ps;
    dist = gl_func_00000000_heli_ff(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z) * *(float *)(&heli_gGfxLib_d + 0x128);
    gl_func_00000000(&dir);
    gl_func_00000000(FWD);
    t = HF(0x164);
    ang = gl_func_00000000_heli_ff(FWD->x * dir.x + FWD->y * dir.y + FWD->z * dir.z);
    s = gl_func_00000000_heli_ff(ang);
    Vec3 old = *FWD;
    if ((s < 0.0f ? -s : s) > (0.001)) {
        sa = gl_func_00000000_heli_ff(t * ang);
        FWD->x = gl_func_00000000_heli_ff((1.0f - t) * ang) * old.x / s + dir.x * sa / s;
        sa = gl_func_00000000_heli_ff(t * ang);
        FWD->y = gl_func_00000000_heli_ff((1.0f - t) * ang) * old.y / s + dir.y * sa / s;
        sa = gl_func_00000000_heli_ff(t * ang);
        FWD->z = gl_func_00000000_heli_ff((1.0f - t) * ang) * old.z / s + dir.z * sa / s;
    }
    if (dist < HF(0x15C)) {
        HF(0x154) = HF(0x160) * (dist / HF(0x15C));
    } else {
        HF(0x154) += HF(0x158);
        HF(0x160) = HF(0x154);
    }
    if (HF(0x150) < HF(0x154)) {
        HF(0x154) = HF(0x150);
    }
    Vec3 vel = *FWD * HF(0x154);
    V3(0xDC) += vel;
    tg = (Vec3 *)(self + 0x12C);
    toTgt = *tg - *ps;
    side.x = 1.0f - HF(0xF8) * HF(0xF8) * 2.0f - HF(0xFC) * HF(0xFC) * 2.0f;
    side.y = HF(0xF4) * HF(0xF8) * 2.0f + HF(0x100) * HF(0xFC) * 2.0f;
    side.z = HF(0xF4) * HF(0xFC) * 2.0f - HF(0x100) * HF(0xF8) * 2.0f;
    toTgt.y = 0.0f;
    f = gl_func_00000000_heli_ff(toTgt.y * toTgt.y + toTgt.x * toTgt.x + toTgt.z * toTgt.z);
    if (1.0f < f && f < (100000.0f)) {
        yaw = -(gl_func_00000000_heli_fff(toTgt.x, toTgt.z) - (3.1415927f)) - HF(0x14C);
        if ((3.1415927f) < yaw) {
            yaw -= (6.2831855f);
        } else if (yaw < (-3.1415927f)) {
            yaw += (6.2831855f);
        }
        f = -(side.x * dir.x + side.y * dir.y + side.z * dir.z) * 1.5;
        if (HF(0x154) < 100.0f) {
            int h = 100; /* int: uopt folds the conversion late, so 100.0f is materialised twice as in the target */
            f *= HF(0x154) / h;
        }
        HF(0x144) = (f - HF(0x144)) * (0.07f) + HF(0x144);
        yaw = (yaw < (-0.3f)) ? (-0.3f) : (((0.3f) < yaw) ? (0.3f) : yaw);
        HF(0x14C) = HF(0x14C) + yaw * (0.06);
        fwdAx.x = HF(0xF4) * HF(0xFC) * 2.0f + HF(0x100) * HF(0xF8) * 2.0f;
        fwdAx.y = HF(0xF8) * HF(0xFC) * 2.0f - HF(0x100) * HF(0xF4) * 2.0f;
        fwdAx.z = 1.0f - HF(0xF4) * HF(0xF4) * 2.0f - HF(0xF8) * HF(0xF8) * 2.0f;
        f = (fwdAx.x * vel.x + fwdAx.y * vel.y + fwdAx.z * vel.z) / 200.0f + fwdAx.y * (0.1);
        HF(0x148) = (f - HF(0x148)) * (0.2f) + HF(0x148);
        f = HF(0x148);
        HF(0x148) = clampf(f, -0.3926991f, 0.3926991f);
        HF(0xF4) = 0.0f;
        HF(0xF8) = 0.0f;
        HF(0xFC) = 0.0f;
        HF(0x100) = 1.0f;
        f = HF(0x144); axZ.y = 0.0f; axZ.x = 0.0f; axZ.z = 1.0f;
        gl_func_00000000_heli_ppf(self + 0xF4, &axZ, f);
        f = HF(0x148); axX.x = 1.0f; axX.y = 0.0f; axX.z = 0.0f;
        gl_func_00000000_heli_ppf(self + 0xF4, &axX, f);
        f = -HF(0x14C); axY.y = 1.0f; axY.x = 0.0f; axY.z = 0.0f;
        gl_func_00000000_heli_ppf(self + 0xF4, &axY, f);
    }
    gl_func_00000000(self);
}
