/* MIPSpro 7.2.1 (EDG 2.29, tools/ncc721.sh) donor for snoweffects.uso 0x64B8 (EXACT since agent-e run 744), 0x6B28
 * (EXACT since run 723) and 0x718C (EXACT since run 736), all spliced over their empty placeholders in both build
 * trees. Agent-e run 713; moved here from the IDO 7.1 NCC donor snoweffects_uso_ncc_nm_e.cpp (removed in run 744).
 * Compiler: 0x718C's chained operator* temporaries (v3B0 * f20C * |a| and v114 * .. * f210) take sp+320 / sp+308 in
 * the target; EDG 2.29 gives that order (first statement temporary = higher word), the 7.1 NCC donor the reverse.
 * 0x6B28 has no chained temporary (both front ends give it the same frame); it sits between 0x64B8 and 0x718C,
 * both 7.2.1 by the same test (run 707), so it goes with them.
 * Class shapes (run 707): SV / SVb are POD (any user-declared constructor changes EDG 2.29's struct-copy order);
 * ct3 / ct0 stand in for the constructors' `if (this != 0)` blocks. Callees go through typed blank (0-pinned)
 * aliases; RoData constants through 0-pinned snow_ro_* symbols. */
#include "snoweffects_ncc_e.h"
#include "../../include/uso_call.h"  /* NCC donors build without -I include */

struct M44 { float m[4][4]; };
extern "C" void *snow_nw(unsigned);
extern "C" float snow_ro_50, snow_ro_54;  /* RoData+0x50 / +0x54: the 0x64B8 burst speed scale / bias */
template<class T> inline T *ct0(T *p) { if (p == 0) p = (T *)snow_nw(sizeof(T)); return p; }
struct SV {
    float x, y, z;
    static void ct3(SV *p, float a, float b, float c) { if (p != 0) { p->x = a; p->y = b; p->z = c; } }
    SV &operator=(SV o) { x = o.x; y = o.y; z = o.z; return *this; }
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    SV operator*(float s) const { SV r; ct3(&r, x * s, y * s, z * s); return r; }
    SV &operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
    /* run 744: 0x64B8's `n += v318 * k * ..`. The plain `x = x + v.x` form has no compound-lvalue pointer word, one
     * edg word fewer below the op* temporaries. */
    SV &operator+=(const SV &v) { x = x + v.x; y = y + v.y; z = z + v.z; return *this; }
    /* run 744: 0x64B8's first product (v318 * k). A declaration plus a single return, so edg substitutes k for s
     * (no pooled float word): the three products take the three oldest pooled float words (target f0 / f2 / f12).
     * q is the named word above r (target sp+200), and the loads through it keep the dead `addiu v1,v1,792`. */
    void setIf(float a, float b, float c) { if (this != 0) { x = a; y = b; z = c; } }
    SV scaled(float s) const { const SV *q = this; SV r; return (r.setIf(q->x * s, q->y * s, q->z * s), r); }
};
/* SVb: the same Vec3 with operator* built through set() (a named r per expansion). Run 673: with it, 0x6B28 and the
 * 0x64B8 lerp materialise &vFC (addiu v0,a1,252) like the target, and 0x6B28's |speed| colours f2. */
struct SVb {
    float x, y, z;
    SVb &operator=(SVb o) { x = o.x; y = o.y; z = o.z; return *this; }
    /* run 736: 0x718C's w assignment. The code-free pointer parameters take edg pooled words: the two before o are
     * fresh, so o (the struct copy) lands two words lower (target sp+188); the two after it are fresh below the copy,
     * so a1's / a2's op* temporaries land three words lower (sp+168 / 152 / 128 / 116). */
    SVb &asg(void *a0, void *a1, SVb o, void *b0, void *b1) { a0 = a0; a1 = a1; b0 = b0; b1 = b1; x = o.x; y = o.y; z = o.z; return *this; }
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    SVb operator*(float s) const { SVb r; ct0(&r); r.set(x * s, y * s, z * s); return r; }
    /* run 727: operator* with a code-free block end after set(), so the next scalar load (0x718C's f210) starts a new
     * uopt block and stops interfering with this product's scalar */
    SVb mulS(float s) const { SVb r; ct0(&r); r.set(x * s, y * s, z * s); if (1) {} return r; }
    SVb &operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
    SVb &operator+=(const SVb &v) { x += v.x; y += v.y; z += v.z; return *this; }
    /* lerp toward a by r (0x64B8 / 0x6B28): the components pass as parameters, so they load up front into the
     * pool-word colours of the following set() (f2 / f12 / f14); lerpComp's body is the per-component formula. */
    void lerpTo(const SVb &a, float r) { lerpComp(r, x, y, z, a); }
    void lerpComp(float r, float px, float py, float pz, const SVb &a) { x = (a.x - px) * r + px; y = (a.y - py) * r + py; z = (a.z - pz) * r + pz; }
    /* per-component lerp (0x718C): one materialised parameter p per statement, so each component loads into the
     * same pool word (f2) right before its use. */
    static float lerpF(float p, float q, float r) { return (q - p) * r + p; }
    void lerpEach(const SVb &a, float r) { x = lerpF(x, a.x, r); y = lerpF(y, a.y, r); z = lerpF(z, a.z, r); }
};
/* run 723: the code-free `if (1) {}` ends uopt's block after set(), so the op+ return copy's &temp (0x6B28 sp+52)
 * no longer shares a block with the &vF0 / &vFC ranges and colours v0 at the copy, as in the target. */
inline SVb operator+(const SVb &a, const SVb &b) { SVb r; ct0(&r); r.set(a.x + b.x, a.y + b.y, a.z + b.z); if (1) {} return r; }
inline SV operator-(const SV &a, const SV &b) { SV r; ct0(&r); r.set(a.x - b.x, a.y - b.y, a.z - b.z); return r; }
inline SV operator+(const SV &a, const SV &b) { SV r; r.set(a.x + b.x, a.y + b.y, a.z + b.z); return r; }
inline void xformB(SV &v, const M44 &m) { float x = m.m[0][0] * v.x + m.m[1][0] * v.y + m.m[2][0] * v.z + m.m[3][0]; float y = m.m[0][1] * v.x + m.m[1][1] * v.y + m.m[2][1] * v.z + m.m[3][1]; float z = m.m[0][2] * v.x + m.m[1][2] * v.y + m.m[2][2] * v.z + m.m[3][2]; v.x = x; v.y = y; v.z = z; }
struct C4 { unsigned char c[4]; };
struct SnowCamE {
    char pad000[0x318];
    /* 0x318 */ SV v318;
    char pad324[0x348 - 0x324];
    /* 0x348 */ float f348;
    char pad34C[0x9A2 - 0x34C];
    /* 0x9A2 */ short state;
    char pad9A4[0xA30 - 0x9A4];
    /* 0xA30 */ C4 col;
};
struct SPart {
    SV pos; SV vel; SV acc;
    /* 0x24 */ float gravity;
    float minVelY, angle;
    /* 0x30 */ float life;
    /* 0x34 */ float decay;
    /* 0x38 */ float size;
    /* 0x3C */ float aspect;
    /* 0x40 */ float sizeVel;
    /* 0x44 */ float fadeAt;
    /* 0x48 */ C4 col;
    int unk4C;
    /* 0x50 */ float alpha;
    /* 0x54 */ float alphaVel;
    void setLife(float l, float d) { life = l; decay = d; }
    void setAlpha(float a, float v) { alpha = a; alphaVel = v; }
    /* 0x718C (run 732): size / fadeAt / aspect, life / decay and alpha / alphaVel each through an inline, so the
     * loaded fields go through edg's pooled float words and take the target's uopt colours (f16 / f16 / f12). The
     * unused middle parameter of setAlpha3 shifts alphaVel to the third pooled word (f12, as in the target). */
    void setSz3(float s, float u, float f) { size = s; fadeAt = f; aspect = u; }
    void setAlpha3(float a, float unused, float v) { alpha = a; alphaVel = v; }
};
extern "C" int gl_func_00000000_snow_norm(SV *);  /* math_vec3Normalize (int return): blank jal */
extern "C" SPart *gl_func_00000000_snow_pool(void *);  /* SnowPool_alloc_000006D0 / SnowPool_oldest_00000680: pointer-return blank jal */
struct SnowEm {
    char pad00[0x2C];
    /* 0x2C */ SnowCamE *cam;
    /* 0x30 */ M44 *mtx;
    char pad34[0x3C - 0x34];
    /* 0x3C */ float f3C;
    /* 0x40 */ float f40;
    char pad44[0x7C - 0x44];
    /* 0x7C */ int state7C;
    char pad80[0x98 - 0x80];
    /* 0x98 */ void *pool;
    char pad9C[0xF0 - 0x9C];
    /* 0xF0 */ SVb vF0;
    /* 0xFC */ SVb vFC;
    char pad108[0x138 - 0x108];
    /* 0x138 */ SV pos;
    /* 0x144 */ int on;
    /* 0x148 */ float speed;
    char pad14C[0x174 - 0x14C];
    /* 0x174 */ float powderGravity;
    /* 0x178 */ float powderRiseY;
    /* 0x17C */ float powderSizeVel;
    /* 0x180 */ float powderDirScale;
    /* 0x184 */ float powderSpeedBias;
    /* 0x188 */ float powderAlphaVel;
    /* 0x18C */ float powderDecay;
    /* 0x190 */ float powderMinSpeed;
    /* 0x194 */ float powderBurstVelScale;
    /* 0x198 */ float powderVelScale;
    /* 0x19C */ int period;
    /* 0x1A0 */ int timer;
    /* 0x1A4 */ int s1A4;
    /* 0x1A8 */ int burst;
    /* 0x1AC */ int count;
};
/* 0x64B8: spawn update of effect A (flag bit 0). EXACT (agent-e run 744). Levers: run 707's POD class set; the
 * scaled() / setIf() first product and the plain op+= (above); math_vec3Normalize returns int, and the call's
 * v0 result makes uopt keep the v318 pointer word out of v0 (target v1). */
extern "C" void BoardTrail_spawnA_000064B8(SnowEm *t) {
    int i;
    float spd;
    float k;
    t->count = 1;
    if (t->cam->state == 98) {
        t->s1A4 = 5;
        if (t->state7C != 98) {
            t->burst = 4;
        }
    }
    if (t->on > 0) {
        i = 0;
        t->timer--;
        if (t->timer < 0) {
            t->timer = t->period;
            i = 1;
        }
        spd = t->speed < 0.0f ? -t->speed : t->speed;
        if (t->s1A4 >= 5 && t->cam->state == 98) {
            t->burst = 2;
        }
        k = t->powderVelScale;
        if (t->burst != 0) {
            t->burst--;
            spd = XCALL(gl_func_00000000_snow_randf, sys_randf)() * snow_ro_50 + snow_ro_54;
            k = t->powderBurstVelScale;
        }
        if (t->cam->state == 98 && (t->burst != 0 || 20.0f < t->cam->f348) && i && t->powderMinSpeed < spd) {
            SV v;
            v.x = 0.0f;
            v.y = 0.0f;
            v.z = t->f40;
            xformB(v, *t->mtx);
            SV n = v - t->pos;
            XCALL(gl_func_00000000_snow_norm, math_vec3Normalize)(&n);
            n *= t->powderDirScale * t->speed;
            n += t->cam->v318.scaled(k) * (t->powderSpeedBias + spd);
            if (n.y > 0.0f) {
                n.y = 0.0f;
            }
            if (XCALL(gl_func_00000000_snow_randf, sys_randf)() < spd) {
                for (i = 0; i < t->count; i++) {
                    float x, y, z, r;
                    SPart *p = XCALL(gl_func_00000000_snow_pool, SnowPool_alloc_000006D0)(t->pool);
                    if (p == 0) {
                        p = XCALL(gl_func_00000000_snow_pool, SnowPool_oldest_00000680)(t->pool);
                    }
                    if (p != 0) {
                        SVb u = t->vF0 + t->vFC * (t->f3C * 2.0f);
                        r = XCALL(gl_func_00000000_snow_randf, sys_randf)();
                        u.lerpTo(t->vF0, r);
                        x = u.x; y = u.y; z = u.z;
                        p->pos.set(x, y + t->powderRiseY, z);
                        p->vel.set(n.x, n.y, n.z);
                        p->size = 1.0f;
                        p->aspect = 0.5f;
                        p->fadeAt = 256.0f;
                        p->col = t->cam->col;
                        if (t->burst != 0) {
                            p->sizeVel = t->powderSizeVel * spd;
                            p->gravity = t->powderGravity * 1.5f;
                        } else {
                            p->sizeVel = t->powderSizeVel * (0.5f + spd / 2.0f);
                            p->gravity = t->powderGravity;
                        }
                        p->setLife(256.0f, t->powderDecay);
                        p->setAlpha(256.0f, t->powderAlphaVel);
                    }
                }
            }
        }
    }
}

/* 0x6B28: spawn update of effect B (flag bit 1), run 668: two RoData default sets picked by the game
 * flags, the course-state counters +0x1C8 / +0x1CC, then a camera-relative burst spawn. EXACT (run 723).
 * Levers: run 673's SVb (the &vFC materialisation, |speed| in f2); run 692's `fA20 == 0.0f` operand order and
 * nested spawn block; run 698's dead `if (0) n *= ..` word; run 723's block-scoped z and the `if (1) {}` in op+. */
extern "C" float snow_ro_58, snow_ro_5C, snow_ro_60, snow_ro_64, snow_ro_68;
extern "C" float snow_ro_6C, snow_ro_70, snow_ro_74, snow_ro_78, snow_ro_7C;
struct SnowGame { char pad[0x8C]; /* 0x8C */ int flags; };
extern "C" SnowGame snow_game_6B28;  /* a game-state global (+0x8C flags, bits 0x820000) */
struct SnowCamB {
    char pad000[0x318];
    /* 0x318 */ SVb v318;
    char pad324[0x348 - 0x324];
    /* 0x348 */ float f348;
    char pad34C[0x3B0 - 0x34C];
    /* 0x3B0 */ SVb v3B0;
    char pad3BC[0x9A0 - 0x3BC];
    /* 0x9A0 */ short s9A0;
    /* 0x9A2 */ short state;
    char pad9A4[0xA20 - 0x9A4];
    /* 0xA20 */ float fA20;
    char padA24[0xA30 - 0xA24];
    /* 0xA30 */ C4 col;
};
struct SnowEmB {
    char pad00[0x2C];
    /* 0x2C */ SnowCamB *cam;
    char pad30[0x3C - 0x30];
    /* 0x3C */ float f3C;
    char pad40[0x80 - 0x40];
    /* 0x80 */ int i80;
    char pad84[0x9C - 0x84];
    /* 0x9C */ void *pool;
    char padA0[0xF0 - 0xA0];
    /* 0xF0 */ SVb vF0;
    /* 0xFC */ SVb vFC;
    char pad108[0x144 - 0x108];
    /* 0x144 */ int on;
    /* 0x148 */ float speed;
    char pad14C[0x1B0 - 0x14C];
    /* 0x1B0 */ float normSizeVel;
    /* 0x1B4 */ float normGravity;
    /* 0x1B8 */ float normUpScale;
    /* 0x1BC */ float normDecay;
    /* 0x1C0 */ float normAlphaVel;
    /* 0x1C4 */ float normVelScale;
    /* 0x1C8 */ int c1C8;
    /* 0x1CC */ int normBurst;
    /* 0x1D0 */ int normLastState;
    char pad1D4[0x1D8 - 0x1D4];
    /* 0x1D8 */ float f1D8;
};
extern "C" void BoardTrail_spawnB_00006B28(SnowEmB *t) {
    float s;
    C4 col;
    if (snow_game_6B28.flags & 0x820000) {
        t->normGravity = snow_ro_58;
        t->normVelScale = snow_ro_5C;
        t->normUpScale = 26.0f;
        t->normSizeVel = snow_ro_60;
        t->normAlphaVel = snow_ro_64;
        t->normDecay = snow_ro_68;
    } else {
        t->normGravity = snow_ro_6C;
        t->normVelScale = snow_ro_70;
        t->normUpScale = 41.0f;
        t->normSizeVel = snow_ro_74;
        t->normAlphaVel = snow_ro_78;
        t->normDecay = snow_ro_7C;
    }
    if (20.0f < t->cam->f348 && t->i80 != 0 && t->on != 1) {
        t->c1C8++;
    }
    if ((snow_game_6B28.flags & 0x820000) && t->cam->s9A0 == 97) {
        if (0.0f < t->f1D8 && t->cam->fA20 == 0.0f) { /* run 692: fA20 on the left gives the target's mtc1 zero (f16) before the fA20 load (f18) */
            t->normBurst = 3;
        }
        t->f1D8 = t->cam->fA20;
    }
    if (t->normBurst != 0 || (t->on >= 2 && t->cam->state == 97)) {
        if (t->c1C8 >= 7 && t->cam->state == 97) {
            t->normBurst = 3;
        }
        if (t->cam->state == 97) {
            t->c1C8 = 0;
        }
        if (t->normBurst != 0) {
            t->normBurst--;
        }
        col = t->cam->col;
        XCALL(gl_func_00000000_snow_randf, sys_randf)();
        if (t->normBurst != 0 && t->normBurst < 3) {
            SVb n = t->cam->v318 * t->normVelScale;
            if (0) n *= t->normUpScale; /* run 698: an edg float temporary between the two products (r2 at the target's sp+132) */
            n += t->cam->v3B0 * t->normUpScale;
            SPart *p = XCALL(gl_func_00000000_snow_pool, SnowPool_alloc_000006D0)(t->pool);
            if (p == 0) {
                p = XCALL(gl_func_00000000_snow_pool, SnowPool_oldest_00000680)(t->pool);
            }
            if (p != 0) {
                float x, y;
                { /* run 692: the spawn statements in a nested block put the op+ temporary below x / y / z, as in the target */
                    float z; /* run 723: edg allocates a block's statement temporaries before its named locals, so z
                              * declared here sits between the op+ temporary (sp+96) and u (sp+80), as in the target */
                    s = t->speed < 0.0f ? -t->speed : t->speed;
                    if (s < 0.5) {
                        s = 0.5f;
                    }
                    SVb u = t->vF0 + t->vFC * (t->f3C * 2.0f * s);
                    s = XCALL(gl_func_00000000_snow_randf, sys_randf)();
                    u.lerpTo(t->vF0, s);
                    x = u.x; y = u.y; z = u.z;
                    p->pos.set(x, y, z);
                    p->vel.set(n.x, n.y, n.z);
                    p->fadeAt = 256.0f;
                    p->size = 1.0f;
                    p->aspect = 1.0f;
                    p->col.c[0] = col.c[0];
                    p->col.c[1] = col.c[1];
                    p->col.c[2] = col.c[2];
                    p->sizeVel = t->normSizeVel;
                    p->gravity = t->normGravity;
                    p->setLife(256.0f, t->normDecay);
                    p->setAlpha(256.0f, t->normAlphaVel);
                }
            }
        }
    }
    if (t->i80 == 0) {
        t->normLastState = t->cam->state;
    }
}

/* 0x718C: spawn update of effect C (flag bit 2), run 673. On course state 99 with +0x80 clear: a = 0 when
 * cam f348 < 60 or |speed| < +0x214, +-1 above +0x218, else the +0x21C-based ramp, signed by speed; the
 * +0x1FC / +0x204 timer, then (a != 0) one pool +0xA0 spawn: pos = lerp(+0x138, +0xF0, randf), vel =
 * v318 * f208 + v114 * (1 + randf * RoData+0xB0) * f210 * |a| * (a > 0 ? -1 : 1) + v3B0 * f20C * |a|, fields
 * from +0x1DC..+0x1F8, alpha 128 + randf * 127, colour lerped from the camera colour to +0x224 by +0x220.
 * EXACT (run 736). Run 727: the |speed| / op* scalar LR is f16, |a| f0 and the op* products f2 / f12 / f14 as in
 * the target (mulS). Run 732: every register matches (named ramp d, setSz3 / setLife / setAlpha3). Run 736: the frame
 * words (go reused for n, the asg pooled words, colB's pooled f / c0). */
extern "C" float snow_ro_B0;  /* RoData+0xB0: the 0x718C spin scale */
struct SnowCamC {
    char pad000[0x318];
    /* 0x318 */ SVb v318;
    char pad324[0x348 - 0x324];
    /* 0x348 */ float f348;
    char pad34C[0x3B0 - 0x34C];
    /* 0x3B0 */ SVb v3B0;
    char pad3BC[0x9A2 - 0x3BC];
    /* 0x9A2 */ short state;
    char pad9A4[0xA30 - 0x9A4];
    /* 0xA30 */ C4 col;
};
struct SnowEmC {
    char pad00[0x2C];
    /* 0x2C */ SnowCamC *cam;
    char pad30[0x4C - 0x30];
    /* 0x4C */ float speed;
    char pad50[0x80 - 0x50];
    /* 0x80 */ int i80;
    char pad84[0xA0 - 0x84];
    /* 0xA0 */ void *pool;
    char padA4[0xF0 - 0xA4];
    /* 0xF0 */ SVb vF0;
    char padFC[0x114 - 0xFC];
    /* 0x114 */ SVb v114;
    char pad120[0x138 - 0x120];
    /* 0x138 */ SVb pos;
    char pad144[0x1DC - 0x144];
    /* 0x1DC */ float specSizeVel;
    /* 0x1E0 */ float specGravity;
    /* 0x1E4 */ float f1E4;
    /* 0x1E8 */ float specDecay;
    /* 0x1EC */ float specAlphaVel;
    /* 0x1F0 */ float f1F0;
    /* 0x1F4 */ float f1F4;
    /* 0x1F8 */ float specSize;
    /* 0x1FC */ int timer;
    /* 0x200 */ int i200;
    /* 0x204 */ int period;
    /* 0x208 */ float specVelScale;
    /* 0x20C */ float specUpScale;
    /* 0x210 */ float specDirScale;
    /* 0x214 */ float specMinLean;
    /* 0x218 */ float specMaxLean;
    /* 0x21C */ float specRampBase;
    /* 0x220 */ float specTintMix;
    /* 0x224 */ float specTint[3];
};
/* run 736: the colour lerp. f and c0 are pooled parameter words (f reuses the word that colours f2, c0 one that
 * colours f14), so only c1 / c2 take fresh frame words; with three named c's the frame was 376. q0 / m0 pick the
 * pooled words. */
static inline void colB(float q0, float f, float m0, float c0, SPart *p, SnowEmC *t) {
    float c1, c2;
    p->col.c[0] = (int)(c0 + (t->specTint[0] * 255.0f - c0) * f);
    c1 = t->cam->col.c[1];
    p->col.c[1] = (int)(c1 + (t->specTint[1] * 255.0f - c1) * f);
    c2 = t->cam->col.c[2];
    p->col.c[2] = (int)(c2 + (t->specTint[2] * 255.0f - c2) * f);
}
extern "C" void BoardTrail_spawnC_0000718C(SnowEmC *t) {
    float a, sp, r;
    int i, go;
    if (t->cam->state != 99 || t->i80 != 0) {
        return;
    }
    sp = t->speed;
    if (t->cam->f348 < 60.0f || (sp < 0.0f ? -sp : sp) < t->specMinLean) {
        a = 0.0f;
    } else if (t->specMaxLean < (sp < 0.0f ? -sp : sp)) {
        if (sp < 0.0f) {
            a = -1.0f;
        } else {
            a = 1.0f;
        }
    } else {
        float d = t->specMaxLean - t->specMinLean; /* run 732: named d gives the target's lo f12 / d f14 / r f2 */
        r = ((sp < 0.0f ? -sp : sp) - t->specMinLean) / d;
        if (sp < 0.0f) {
            a = -((1.0f - t->specRampBase) * r + t->specRampBase);
        } else {
            a = (1.0f - t->specRampBase) * r + t->specRampBase;
        }
    }
    go = 0;
    t->timer--;
    if (t->timer < 0) {
        t->timer = t->period;
        go = 1;
    }
    if (!go || !(60.0f < t->cam->f348)) {
        return;
    }
    go = a != 0.0f; /* run 736: go reused (a separate n cost one frame word) */
    if ((go < 0 ? -go : go) == 0) return;
    for (i = 0; i < 1; i++) {
        SPart *p = XCALL(gl_func_00000000_snow_pool, SnowPool_alloc_000006D0)(t->pool);
        if (p == 0) {
            p = XCALL(gl_func_00000000_snow_pool, SnowPool_oldest_00000680)(t->pool);
        }
        if (p != 0) {
            SVb w;
            SVb u = t->pos;
            r = XCALL(gl_func_00000000_snow_randf, sys_randf)();
            u.lerpEach(t->vF0, r);
            w.asg(t, t, t->cam->v318 * t->specVelScale, t, t);
            if (0) w *= t->specUpScale; /* run 698: one more edg float temporary (objdiff 96.99 -> 97.07) */
            SVb a1 = t->cam->v3B0 * t->specUpScale * (a < 0.0f ? -a : a);
            SVb a2 = t->v114.mulS(1.0f + XCALL(gl_func_00000000_snow_randf, sys_randf)() * snow_ro_B0) * t->specDirScale;
            a2 *= (a < 0.0f ? -a : a);
            if (0.0f < a) {
                a2 *= -1.0f;
            }
            w += a2;
            w += a1;
            p->pos.set(u.x, u.y, u.z);
            p->vel.set(w.x, w.y, w.z);
            p->setSz3(t->specSize, 1.0f, 256.0f);
            p->sizeVel = t->specSizeVel;
            p->gravity = t->specGravity;
            p->setLife(256.0f, t->specDecay);
            r = XCALL(gl_func_00000000_snow_randf, sys_randf)();
            p->setAlpha3(128.0f + r * 127.0f, t->specAlphaVel, t->specAlphaVel);
            colB(r, t->specTintMix, r, t->cam->col.c[0], p, t);
        }
    }
}
