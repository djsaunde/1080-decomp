/* MIPSpro 7.2.1 C++ (tools/ncc721.sh) NON_MATCHING donor for heli.uso 0xB08 (agent-g run 672), spliced
 * over the empty NON_MATCHING placeholder in heli_uso.c on the non_matching build only (the matching build
 * keeps INCLUDE_ASM). 0xB08 is the Heli per-frame update (decode: docs/1080_NEW_MODULES.md "heli.uso").
 * MIPSpro 7.2.1 is the right front end: the target's 8-way switch is an inline jump table (RoData+0x7C),
 * which IDO 7.1 NCC (EDG 2.19) never emits. Levers that took mp72 from 77.3 to 92.4:
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
 *  - run 682 also: the pitch clamp as an inline clampf() (88.2 -> 88.9).
 *  - run 689: the pitch dot product and the 0.2 smoothing as inline helpers (dot(), lerpTo() storing through a
 *    reference), so the smoothing, its store and the clamp compare share the fwdAx uopt block; 2.0f / 1.0f are
 *    then rematerialised per use and -pi/8 sits in f18 (88.9 -> 91.9). toTgt.y = 0 through setf() (not
 *    copy-propagated, so the zero is not coloured) with the length summed x, y, z as in the target (92.4).
 *  - run 695: clampf() with if / return arms and the yaw clamp through clampf() (each arm writes the result
 *    register directly, as the target's mov.s f2 / f12 arms); the pitch clamp stored through a reference
 *    (clampTo); tg for toTgt spelled as ps + 0x50 (= self + 0x12C), which keeps the toTgt loads in the
 *    target's order (92.4 -> 93.1).
 *  - run 706: `if (1) {}` before case 2's V3(0x12C) store: the target stores 300 / 304 / 308(s0) directly
 *    there; without the cut uopt colours self+0x12C (v0) for the three stores (93.1 -> 93.2).
 *  - run 710: operator* without a constructor temp: the products go through set3() (by-value float args, so
 *    the three mul.s are issued before the stores, as in the target) into a ctor-less V3b, which makes the
 *    temp a one-use LR in a t-reg (target `addiu t5,sp,136`); `if (1) {}` ahead of the mode test (93.21 -> 93.27).
 *  - run 714: the heading pointer h = self+0x138 as a named local for the normalise call, the three slerp
 *    stores and vel; it is spilled and reloaded per store as in the target (93.27 -> 93.25 objdiff, but 7 fewer
 *    sp+reg-masked diff rows: 93 -> 86).
 *  - run 719: the slerp is an inline function taking the heading pointer, and each component goes through an
 *    inline slc() whose first parameter is sin(t * ang). The argument is evaluated before the cos call, and as
 *    an inline parameter it is coloured (target reloads it into f16 per component), so ugen evaluates the
 *    old-heading term first as in the target. Frame 0x178 -> 0x170 (93.25 -> 94.44).
 *  - run 724: the frame laid out as the target's (EDG 2.29 allocates a scope's variables at its entry, in
 *    declaration order): one function-scope scalar (ps) before toTgt, vel direct-initialised between dist and
 *    side, mode in a block around the head + switch, key / p in the else-block, key2 / tg in the switch block,
 *    case 2's item / next / mpos / k in blocks, and yaw / hh / g / fwdAx / pf / axZ / axX / axY as if-block
 *    locals (g = roll, pf = pitch). The heading pointer is an expression again (the target spills it to a
 *    uopt temp, 48(sp), not to a home slot). sp-offset diff rows 279 -> 113, 94.44 -> 94.39.
 *  - run 730: EDG word placement for the slerp temps (pooled inline parameters take the oldest free word of their
 *    type; a word reused in a statement stays locked to its end; inline named locals are fresh): a code-free
 *    dum4A() in case 2 leaves two old pointer words, so the dir op-'s v / ctor this reuse them and its three
 *    float words start at -160 (the slerp's t); dumf(t) holds the third so the sin term takes a fresh word
 *    (-220); q0 / q1 are the two words between X and ang. Frame map = target's except the 48 / 56 uopt temps
 *    (94.39 -> 94.44).
 *  - run 730: operator*(const float &s) opens with `if (1) {}`: the loads of the heading come after a block
 *    cut from the `this` store, so they are not folded into 312(s0); they use the self+0x138 item, whose live range
 *    then reaches vel and overlaps the speed loads, so its uopt temp is 48 and t * ang's is 56 as in the target
 *    (spill slots 48 / 56 fixed; 94.44 -> 94.91).
 *  - run 735: toTgt's target point through the dead player pointer (assigned before vel, kept in v0) and a
 *    ctor-less sub() ending in `if (1) {}` (94.91 -> 95.39).
 *  - run 739: dir through the ctor-less sub() too, toTgt through subc() (the cut first) (95.39 -> 95.76).
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
static inline float clampf(float v, float lo, float hi) { if (v < lo) return lo; if (hi < v) return hi; return v; }
/* smoothing store through a reference: keeps the store, the pitch dot / lerp and the clamp compare in one uopt
 * block, so -pi/8 and the lerp value take f18 / f16 and 2.0f / 1.0f are rematerialised per use (run 689) */
/* store through a reference: uopt does not copy-propagate it, so the zero has one use and is not coloured;
 * ugen then reuses the stored register for the toTgt.y square, as in the target (run 689) */
static inline void setf(float &d, float v) { d = v; }
/* ctor-less result struct for operator* (run 710) */
struct V3b { float x, y, z; };
static inline void set3q(V3b &r, float a, float b, float c) { r.x = a; r.y = b; r.z = c; if (0 * (unsigned)b) r.y = b; }
static inline void set3(V3b &r, float a, float b, float c) { r.x = a; r.y = b; r.z = c; }
struct Vec3 {
    float x, y, z;
    Vec3() {}
    Vec3(float a, float b, float c) { x = a; y = b; z = c; }
    Vec3 operator-(const Vec3 &v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    /* toTgt's difference (run 735): ctor-less like operator*, so the result temp's address is not a
     * this-or-new join value (no coloured register; ugen recomputes sp+124 per use as in the target); the
     * `if (1) {}` after the stores ends the uopt block, so the 2.0f / HF(0xF8) live ranges of the side
     * vector do not cover the subtraction and take f2 / f14 as in the target */
    Vec3 sub(const Vec3 &v) const { V3b r; set3(r, x - v.x, y - v.y, z - v.z); if (1) {} return *(Vec3 *)&r; }
    /* the same with the cut first (run 739): dir goes through sub(), toTgt through subc(); the end cut stays on
     * dir's, the start cut on toTgt's (the 2.0f / 1.0f side-vector constants then load after the subtraction) */
    /* trailing cut (agent-e run 771): M-168 leaves the side-vector block, so HF(0xF8) takes f14 */
    Vec3 subc(const Vec3 &v) const { if (1) {} V3b r; set3(r, x - v.x, y - v.y, z - v.z); if (1) {} return *(Vec3 *)&r; }
    Vec3 mq(const float &s) const { if (1) {} V3b r; set3q(r, x * s, y * s, z * s); return *(Vec3 *)&r; }
    Vec3 operator*(const float &s) const { if (1) {} V3b r; set3(r, x * s, y * s, z * s); return *(Vec3 *)&r; }
    Vec3 &operator=(Vec3 v) { x = v.x; y = v.y; z = v.z; return *this; }
    void operator+=(const Vec3 &v) { x += v.x; y += v.y; z += v.z; }
};
static inline float dot(const Vec3 &a, const Vec3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static inline void fwdOf(Vec3 &r, const float *q) {
    r.x = q[0] * q[2] * 2.0f + q[3] * q[1] * 2.0f;
    r.y = q[1] * q[2] * 2.0f - q[3] * q[0] * 2.0f;
    r.z = 1.0f - q[0] * q[0] * 2.0f - q[1] * q[1] * 2.0f;
}
/* one slerp component: cos-weighted old heading plus sin-weighted target direction, both over sin(ang) */
static inline float slc(float sa, float t, float ang, float o, float d, float s) { return gl_func_00000000_heli_ff((1.0f - t) * ang) * o / s + d * sa / s; }
/* slerp the heading h toward dir by t (run 719) */
static inline void dum4A(void *a0, void *a1, void *a2, void *a3) { a0 = a0; a1 = a1; a2 = a2; a3 = a3; }
static inline void dumf(float a0) { a0 = a0; }
static inline int zf1(float lo, int z) { return z * (int)(lo); }
static inline int zf4(float lo, int z) { return z * (int)(lo + lo + lo + lo); }
static inline int zf6(float lo, int z) { return z * (int)(lo + lo + lo + lo + lo + lo); }
static inline float idm(float x) { x = x; return x; }
static inline float clampP(float v, float lo, float hi, int z) { if ((v < lo)) return lo; if (zf1(v, z)) v = lo; if ((hi < v) + zf4(v, z)) return hi; return v; }
static inline void slerp(Vec3 *h, const Vec3 &dir, float t) {
    float q0 = 0; float q1 = 0;
    float ang = gl_func_00000000_heli_ff(h->x * dir.x + h->y * dir.y + h->z * dir.z);
    float s = gl_func_00000000_heli_ff(ang);
    Vec3 old = *h;
    dumf(t);
    if ((s < 0.0f ? -s : s) > (0.001)) {
        h->x = slc(gl_func_00000000_heli_ff(t * ang), t, ang, old.x, dir.x, s);
        h->y = slc(gl_func_00000000_heli_ff(t * ang), t, ang, old.y, dir.y, s);
        h->z = slc(gl_func_00000000_heli_ff(t * ang), t, ang, old.z, dir.z, s);
    }
}
#define V3(off) (*(Vec3 *)(self + (off)))
#define FWD ((Vec3 *)(self + 0x138))
#define COURSE (*(char **)(&heli_flow_b08 + 0x138))
extern "C" void heli_uso_func_00000B08(char *self);
/* the Heli vtable's update slot, emitted before the body (run 742, agent-a's viewrec 3878 lever): as1 treats every
 * text address named by a data word before the function's .end as a target of each jump-table jr, so the case-6
 * default is no longer an exclusive block and xbb fills the bound check's delay slot with the dispatch sll as in
 * the target (95.76 -> 96.39) */
extern "C" void *heli_updateSlot_b08 = (void *)heli_uso_func_00000B08;
extern "C" void heli_uso_func_00000B08(char *self) {
    struct { float x, y, z; } ppr;
    char *player;
    Vec3 *ps;
    Vec3 toTgt;
    Vec3 dir;
    float dist;
    if (COURSE == 0 && *(int *)(COURSE + 0x48) == 0) {
        return;
    }
    player = **(char ***)(COURSE + 0x44);
    ppr.x = *(float *)(player + 0xA0);
    ppr.y = *(float *)(player + 0xA4);
    ppr.z = *(float *)(player + 0xA8);
    if (1) {}
    {
    int mode;
    if (HI(0x170) != 0) {
        mode = HI(0x170);
    } else {
        HeliTypeTag key;
        char *p;
        if (*(int *)(player + 0xA54) == 6) {
            gl_func_00000000(self);
            HI(0x170) = 8;
        }
        if (*(char **)(self + 0x168) == 0) {
            key.v = *(int *)(&heli_data_00000000 + 0xCBAC);
            p = (char *)gl_func_00000000(heli_itemMgr_b08, key);
            if (p != 0) {
#define VT (((Node *)(p))->vptr)
                *(char **)(self + 0x168) = (char *)((int (*)(...))(((GdlVtEnt *)((char *)(VT) + 8))[12]).fn)((((GdlVtEnt *)((char *)(VT) + 8))[12]).delta + p, &(*(Vec3 *)&ppr), 0);
            }
        }
        p = *(char **)(self + 0x168);
        if (p != 0) {
            if ((*(char **)(p + 0x2C)) != 0) { p = (*(char **)(p + 0x2C)); if (*(float *)(p + 0x38) < (*(Vec3 *)&ppr).z) {
                *(char **)(self + 0x168) = p;
            } }
            HI(0x16C) = *(int *)(*(char **)(self + 0x168) + 0x3C);
        }
        mode = HI(0x16C);
    }
    {
    HeliTypeTag key2;
    Vec3 *tg;
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
        if (1) {} /* keeps L4 a block of its own after the if's join (which takes case 3's tg def) */
    L4:
        break;
    case 4:
        V3(0x114) = (*(Vec3 *)&ppr);
        HF(0x118) += (5000.0f);
        HF(0x11C) += (5000.0f);
        HF(0x15C) = 1000.0f;
        HF(0x158) = 1.0f;
        HF(0x150) = 800.0f;
        V3(0x12C) = *tg;
        /* = break. L4 (end of case 3) is a code-free block with two predecessors, so uopt's split of the sp+252
         * lda live range sees case 4's piece with two entry units and two uses (num 0): case 4 is not coloured v1
         * and rematerializes t8 / t4 as in the target. L4 takes the ps (a2) PRE def; ending in `b join`, as1 copies
         * it into case 4, and case 3 falls into it with a2 in the delay slot, as in the target (run 747) */
        goto L4;
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
    case 2: {
        char *p;
        key2.v = *(int *)(&heli_data_00000000 + 0xCBB0);
        p = (char *)gl_func_00000000(heli_itemMgr2_b08, key2, ps, 0);
        if (p != 0 && *(int *)(p + 0x2C) != 0) {
            char *q;
            Vec3 mpos;
            q = *(char **)(p + 0x2C);
            dum4A(q, q, q, q);
            mpos = *(Vec3 *)(q + 0x30);
            mpos.y += (3000.0f);
            HF(0x114) = HF(0x114) + (mpos.x - HF(0x114)) * (0.2);
            HF(0x118) = HF(0x118) + (mpos.y - HF(0x118)) * (0.2);
            HF(0x11C) = HF(0x11C) + (mpos.z - HF(0x11C)) * (0.2);
            if (1) {} /* block cut: self+0x12C is not a coloured LR here, the stores fold to 300(s0) (run 706) */
            V3(0x12C) = V3(0x114);
            HF(0x15C) = 0.0f;
            HF(0x158) = 2.0f;
            HF(0x150) = *(float *)(player + 0x348) + 5.0f;
            if (HF(0x150) < 100.0f) {
                HF(0x150) = 100.0f;
            }
        }
        break;
    }
    case 6:
        V3(0x114) = (*(Vec3 *)&ppr);
        break;
    default:
        break;
    }
    /* ctor-less like toTgt's (run 739): the op- result temp (sp+212) is no longer a coloured this-or-new join
     * value; ugen recomputes it in t3 and the op= temp (sp+252) takes v0 as in the target */
    dir = tg->sub(*ps);
    dist = gl_func_00000000_heli_ff(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z) * *(float *)(&heli_gGfxLib_d + 0x128);
    gl_func_00000000(&dir);
    gl_func_00000000(FWD);
    slerp(FWD, dir, HF(0x164));
    if (dist < HF(0x15C)) {
        HF(0x154) = HF(0x160) * (dist / HF(0x15C));
    } else {
        HF(0x154) += HF(0x158);
        HF(0x160) = HF(0x154);
    }
    if (HF(0x150) < HF(0x154)) {
        HF(0x154) = HF(0x150);
    }
    /* player is dead here: reused for the target point self+0x12C, assigned in this earlier block so
     * uopt keeps it in a register (target `addiu v0,s0,300`, loads 0 / 4 / 8(v0)) instead of folding it
     * into 80(ps) (run 735) */
    player = self + 0x12C;
    }
    }
    /* mq() / set3q(): the code-free `if (0 * (unsigned)b)` adds uses to set3q's middle word (M-168 3.33, above
     * M-220), and the dumf() pad shifts the pooled words (agent-e run 771) */
    Vec3 vel = (dumf(HF(0x164)), FWD->mq(HF(0x154)));
    Vec3 side;
    float f;
    V3(0xDC) += vel;
    toTgt = (*(Vec3 *)player).subc(*ps);
    side.x = 1.0f - HF(0xF8) * HF(0xF8) * 2.0f - HF(0xFC) * HF(0xFC) * 2.0f;
    side.y = HF(0xF4) * HF(0xF8) * 2.0f + HF(0x100) * HF(0xFC) * 2.0f;
    side.z = HF(0xF4) * HF(0xFC) * 2.0f - HF(0x100) * HF(0xF8) * 2.0f;
    setf(toTgt.y, 0.0f);
    f = gl_func_00000000_heli_ff(toTgt.x * toTgt.x + toTgt.y * toTgt.y + toTgt.z * toTgt.z);
    if (1.0f < f && f < (100000.0f)) {
        float yaw;
        int hh;
        float g;
        Vec3 fwdAx;
        float pf;
        Vec3 axZ;
        Vec3 axX;
        Vec3 axY;
        yaw = -(gl_func_00000000_heli_fff(toTgt.x, toTgt.z) - (3.1415927f)) - HF(0x14C);
        if ((3.1415927f) < yaw) {
            yaw -= (6.2831855f);
        } else if (yaw < (-3.1415927f)) {
            yaw = (6.2831855f) + yaw; /* target add.s keeps the constant first */
        }
        g = -(side.x * dir.x + side.y * dir.y + side.z * dir.z) * 1.5;
        if (HF(0x154) < 100.0f) {
            hh = 100;
            g *= HF(0x154) / hh;
        }
        if (1) {}
        *(float *)(self + 0x144 + zf6(-0.3f, 0) + 0 * (unsigned)(yaw + yaw)) = (g - HF(0x144)) * (0.07f) + HF(0x144);
        yaw = clampf(yaw, -0.3f, 0.3f);
        *(float *)(self + 0x14C + zf4(-0.3926991f, 0)) = HF(0x14C) + yaw * (0.06);
        if (1) {}
        fwdOf(fwdAx, &HF(0xF4));
        pf = dot(fwdAx, vel) / 200.0f;
        pf = pf + fwdAx.y * (0.1);
        /* three code-free dumf() calls hold three pooled edg float words, so idm()'s parameter (the lerp value) takes
         * the slerp's sin-term word M-220 (target lwc1 f16,148(sp)); clampP's copy / return take M-160 / M-164
         * (agent-g run 770 m3c0, agent-e run 771) */
        f = (dumf(pf), dumf(pf), dumf(pf), idm((pf - HF(0x148)) * 0.2f + HF(0x148)));
        /* the (unsigned) fold adds an old-h use like zf1(), but zf1()'s (int) conversion costs one hidden ugen t-ring
         * temp in the store address (run 770) */
        *(float *)(self + 0x148 + 0 * (unsigned)(HF(0x148))) = f;
        if (1) {}
        *(float *)(self + 0x148) = clampP(f, -0.3926991f, 0.3926991f, 0);
        HF(0xF4) = 0.0f;
        HF(0xF8) = 0.0f;
        HF(0xFC) = 0.0f;
        HF(0x100) = 1.0f;
        /* code-free use of g: its live range reaches the zero stores' block start, so the zero piece takes
         * f0 and the angle f2, as in the target (run 755) */
        dumf(g);
        g = HF(0x144); axZ.y = 0.0f; axZ.x = 0.0f; axZ.z = 1.0f;
        gl_func_00000000_heli_ppf(self + 0xF4, &axZ, g);
        g = HF(0x148); axX.x = 1.0f; axX.y = 0.0f; axX.z = 0.0f;
        gl_func_00000000_heli_ppf(self + 0xF4, &axX, g);
        g = -HF(0x14C); axY.y = 1.0f; axY.x = 0.0f; axY.z = 0.0f;
        gl_func_00000000_heli_ppf(self + 0xF4, &axY, g);
    }
    gl_func_00000000(self);
}
