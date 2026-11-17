/* IDO 7.1 NCC (C++) NON_MATCHING donors for snoweffects.uso functions at 0x6000+ (agent-e run 664), spliced
 * over the empty NON_MATCHING placeholders in snoweffects_uso.c on the non_matching build only (the matching
 * build keeps INCLUDE_ASM). Callees go through typed blank (0-pinned) aliases; RoData constants through
 * 0-pinned snow_ro_* symbols. 0x7960 moved to the matching donor snoweffects_uso_ncc_e.cpp (run 673).
 * Spliced (run 673): 0x64B8 (97.0 %, 404 words), 0x6B28 (93.8 %, 358 words) and 0x718C (96.9 %, 471 words), all
 * the target's length. Run 680: 0x64B8 97.7 % and 0x6B28 95.1 %, both at the target frame (344 / 232): the lerp x, y, z
 * live in the loop body / p block above the spawn temps, the randf result reuses a dead float (k / s) instead of a new
 * named r, and life / decay and alpha / alphaVel are stored through inline two-parameter setters (their uopt-coloured
 * parameters give the target's early f0 loads). Run 673 levers: the +0x1A0 / +0x1FC timers as `t->timer--; if (t->timer < 0)` (the
 * pre-decrement form colours the counter v0 / v1, the target uses t-registers), and the set()-built operator*
 * of SVb (below). Run 685: 0x64B8 99.8 %, 0x6B28 98.4 %, 0x718C 96.9 % (objdiff). The spawn lerps are SVb members
 * (lerpTo / lerpEach): reading the components as inline parameters through `this` keeps uopt from copy-propagating
 * them, so they load up front into pool-word colours as in the target; the lerped point is then copied into the
 * x / y / z locals for pos.set(). */
#include "snoweffects_ncc_e.h"
extern "C" float snow_ro_50, snow_ro_54;  /* RoData+0x50 / +0x54: the 0x64B8 burst speed scale / bias */

struct M44 { float m[4][4]; };
struct SV {
    float x, y, z;
    SV() {}
    SV(float a, float b, float c) { x = a; y = b; z = c; }
    SV &operator=(SV o) { x = o.x; y = o.y; z = o.z; return *this; }
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    SV operator*(float s) const { return SV(x * s, y * s, z * s); }
    /* run 698: the named f gives 0x64B8's `n *= ..` a float word above the later operator* temporaries, so the
     * v318 * k result lands at the target's sp+200 */
    SV &operator*=(float s) { float f = s; x *= f; y *= f; z *= f; return *this; }
    SV &operator+=(const SV &v) { x += v.x; y += v.y; z += v.z; return *this; }
};
/* SVb: the same Vec3 with operator* built through set() (a named r per expansion). Run 673: with it, 0x6B28 and the
 * 0x64B8 lerp materialise &vFC (addiu v0,a1,252) like the target, and 0x6B28's |speed| colours f2. */
struct SVb {
    float x, y, z;
    SVb() {}
    SVb(float a, float b, float c) { x = a; y = b; z = c; }
    SVb &operator=(SVb o) { x = o.x; y = o.y; z = o.z; return *this; }
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    SVb operator*(float s) const { SVb r; r.set(x * s, y * s, z * s); return r; }
    SVb &operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
    SVb &operator+=(const SVb &v) { x += v.x; y += v.y; z += v.z; return *this; }
    /* lerp toward a by r (0x64B8 / 0x6B28): the components pass as parameters, so they load up front into the
     * pool-word colours of the following set() (f2 / f12 / f14); lerpComp's body is the per-component formula. */
    void lerpTo(const SVb &a, float r) { lerpComp(r, x, y, z, a); }
    void lerpComp(float r, float px, float py, float pz, const SVb &a) { x = (a.x - px) * r + px; y = (a.y - py) * r + py; z = (a.z - pz) * r + pz; }
    /* per-component lerp (0x718C): one materialised parameter p per statement, so each component loads into the
     * same pool word (f2) right before its use. */
    static float lerpF(float p, float q, float r) { ; return (q - p) * r + p; }
    void lerpEach(const SVb &a, float r) { x = lerpF(x, a.x, r); y = lerpF(y, a.y, r); z = lerpF(z, a.z, r); }
};
inline SVb operator+(const SVb &a, const SVb &b) { SVb r; r.set(a.x + b.x, a.y + b.y, a.z + b.z); return r; }
inline SV operator-(const SV &a, const SV &b) { SV r; r.set(a.x - b.x, a.y - b.y, a.z - b.z); return r; }
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
    float minVelY, unk2C;
    /* 0x30 */ float life;
    /* 0x34 */ float decay;
    /* 0x38 */ float size;
    /* 0x3C */ float unk3C;
    /* 0x40 */ float sizeVel;
    /* 0x44 */ float fadeAt;
    /* 0x48 */ C4 col;
    int unk4C;
    /* 0x50 */ float alpha;
    /* 0x54 */ float alphaVel;
    void setLife(float l, float d) { life = l; decay = d; }
    void setAlpha(float a, float v) { alpha = a; alphaVel = v; }
};
extern "C" void gl_func_00000000_snow_norm(SV *);  /* math_vec3Normalize: blank jal */
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
    /* 0x174 */ float f174;
    /* 0x178 */ float f178;
    /* 0x17C */ float f17C;
    /* 0x180 */ float f180;
    /* 0x184 */ float f184;
    /* 0x188 */ float f188;
    /* 0x18C */ float f18C;
    /* 0x190 */ float f190;
    /* 0x194 */ float f194;
    /* 0x198 */ float f198;
    /* 0x19C */ int period;
    /* 0x1A0 */ int timer;
    /* 0x1A4 */ int s1A4;
    /* 0x1A8 */ int burst;
    /* 0x1AC */ int count;
};
extern "C" void snoweffects_uso_func_000064B8(SnowEm *t) {
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
        k = t->f198;
        if (t->burst != 0) {
            t->burst--;
            spd = gl_func_00000000_snow_randf() * snow_ro_50 + snow_ro_54;
            k = t->f194;
        }
        if (t->cam->state == 98 && (t->burst != 0 || 20.0f < t->cam->f348) && i && t->f190 < spd) {
            SV v;
            v.x = 0.0f;
            v.y = 0.0f;
            v.z = t->f40;
            xformB(v, *t->mtx);
            SV n = v - t->pos;
            gl_func_00000000_snow_norm(&n);
            n *= t->f180 * t->speed;
            n += t->cam->v318 * k * (t->f184 + spd);
            if (n.y > 0.0f) {
                n.y = 0.0f;
            }
            if (gl_func_00000000_snow_randf() < spd) {
                for (i = 0; i < t->count; i++) {
                    float x, y, z;
                    SPart *p = gl_func_00000000_snow_pool(t->pool);
                    if (p == 0) {
                        p = gl_func_00000000_snow_pool(t->pool);
                    }
                    if (p != 0) {
                        SVb u = t->vF0 + t->vFC * (t->f3C * 2.0f);
                        k = gl_func_00000000_snow_randf();
                        u.lerpTo(t->vF0, k);
                        x = u.x; y = u.y; z = u.z;
                        p->pos.set(x, y + t->f178, z);
                        p->vel.set(n.x, n.y, n.z);
                        p->size = 1.0f;
                        p->unk3C = 0.5f;
                        p->fadeAt = 256.0f;
                        p->col = t->cam->col;
                        if (t->burst != 0) {
                            p->sizeVel = t->f17C * spd;
                            p->gravity = t->f174 * 1.5f;
                        } else {
                            p->sizeVel = t->f17C * (0.5f + spd / 2.0f);
                            p->gravity = t->f174;
                        }
                        p->setLife(256.0f, t->f18C);
                        p->setAlpha(256.0f, t->f188);
                    }
                }
            }
        }
    }
}

/* 0x6B28: spawn update of effect B (flag bit 1), run 668: two RoData default sets picked by the game
 * flags, the course-state counters +0x1C8 / +0x1CC, then a camera-relative burst spawn (93.8 %, 358 words;
 * run 673's SVb gives the &vFC materialisation and |speed| in f2). Left: frame 240 vs 232 and the ugen FP
 * ring in the lerp. Run 692: `fA20 == 0.0f` operand order and the nested spawn block (op+ temporary below x / y / z).
 * Left: one inline word between the two n products (target r2 at vfp-100, ours -96) and the u copy address
 * (target addiu v0 at the copy, ours hoisted into a0). */
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
    /* 0x1B0 */ float f1B0;
    /* 0x1B4 */ float f1B4;
    /* 0x1B8 */ float f1B8;
    /* 0x1BC */ float f1BC;
    /* 0x1C0 */ float f1C0;
    /* 0x1C4 */ float f1C4;
    /* 0x1C8 */ int c1C8;
    /* 0x1CC */ int c1CC;
    /* 0x1D0 */ int i1D0;
    char pad1D4[0x1D8 - 0x1D4];
    /* 0x1D8 */ float f1D8;
};
extern "C" void snoweffects_uso_func_00006B28(SnowEmB *t) {
    float s;
    C4 col;
    if (snow_game_6B28.flags & 0x820000) {
        t->f1B4 = snow_ro_58;
        t->f1C4 = snow_ro_5C;
        t->f1B8 = 26.0f;
        t->f1B0 = snow_ro_60;
        t->f1C0 = snow_ro_64;
        t->f1BC = snow_ro_68;
    } else {
        t->f1B4 = snow_ro_6C;
        t->f1C4 = snow_ro_70;
        t->f1B8 = 41.0f;
        t->f1B0 = snow_ro_74;
        t->f1C0 = snow_ro_78;
        t->f1BC = snow_ro_7C;
    }
    if (20.0f < t->cam->f348 && t->i80 != 0 && t->on != 1) {
        t->c1C8++;
    }
    if ((snow_game_6B28.flags & 0x820000) && t->cam->s9A0 == 97) {
        if (0.0f < t->f1D8 && t->cam->fA20 == 0.0f) { /* run 692: fA20 on the left gives the target's mtc1 zero (f16) before the fA20 load (f18) */
            t->c1CC = 3;
        }
        t->f1D8 = t->cam->fA20;
    }
    if (t->c1CC != 0 || (t->on >= 2 && t->cam->state == 97)) {
        if (t->c1C8 >= 7 && t->cam->state == 97) {
            t->c1CC = 3;
        }
        if (t->cam->state == 97) {
            t->c1C8 = 0;
        }
        if (t->c1CC != 0) {
            t->c1CC--;
        }
        col = t->cam->col;
        gl_func_00000000_snow_randf();
        if (t->c1CC != 0 && t->c1CC < 3) {
            SVb n = t->cam->v318 * t->f1C4;
            if (0) n *= t->f1B8; /* run 698: an edg float temporary between the two products (r2 at the target's sp+132) */
            n += t->cam->v3B0 * t->f1B8;
            SPart *p = gl_func_00000000_snow_pool(t->pool);
            if (p == 0) {
                p = gl_func_00000000_snow_pool(t->pool);
            }
            if (p != 0) {
                float x, y, z;
                { /* run 692: the spawn statements in a nested block put the op+ temporary below x / y / z, as in the target */
                    s = t->speed < 0.0f ? -t->speed : t->speed;
                    if (s < 0.5) {
                        s = 0.5f;
                    }
                    SVb u = t->vF0 + t->vFC * (t->f3C * 2.0f * s);
                    s = gl_func_00000000_snow_randf();
                    u.lerpTo(t->vF0, s);
                    x = u.x; y = u.y; z = u.z;
                    p->pos.set(x, y, z);
                    p->vel.set(n.x, n.y, n.z);
                    p->fadeAt = 256.0f;
                    p->size = 1.0f;
                    p->unk3C = 1.0f;
                    p->col.c[0] = col.c[0];
                    p->col.c[1] = col.c[1];
                    p->col.c[2] = col.c[2];
                    p->sizeVel = t->f1B0;
                    p->gravity = t->f1B4;
                    p->setLife(256.0f, t->f1BC);
                    p->setAlpha(256.0f, t->f1C0);
                }
            }
        }
    }
    if (t->i80 == 0) {
        t->i1D0 = t->cam->state;
    }
}

/* 0x718C: spawn update of effect C (flag bit 2), run 673. On course state 99 with +0x80 clear: a = 0 when
 * cam f348 < 60 or |speed| < +0x214, +-1 above +0x218, else the +0x21C-based ramp, signed by speed; the
 * +0x1FC / +0x204 timer, then (a != 0) one pool +0xA0 spawn: pos = lerp(+0x138, +0xF0, randf), vel =
 * v318 * f208 + v114 * (1 + randf * RoData+0xB0) * f210 * |a| * (a > 0 ? -1 : 1) + v3B0 * f20C * |a|, fields
 * from +0x1DC..+0x1F8, alpha 128 + randf * 127, colour lerped from the camera colour to +0x224 by +0x220.
 * NM (96.9 %, 471 words = target, frame 368 = target): the operator* scalars colour f12 where the target has
 * f16 (the |speed| temp too), the u copy materialises &t->pos, and the inner edg temps sit 4..8 bytes off. */
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
    /* 0x1DC */ float f1DC;
    /* 0x1E0 */ float f1E0;
    /* 0x1E4 */ float f1E4;
    /* 0x1E8 */ float f1E8;
    /* 0x1EC */ float f1EC;
    /* 0x1F0 */ float f1F0;
    /* 0x1F4 */ float f1F4;
    /* 0x1F8 */ float f1F8;
    /* 0x1FC */ int timer;
    /* 0x200 */ int i200;
    /* 0x204 */ int period;
    /* 0x208 */ float f208;
    /* 0x20C */ float f20C;
    /* 0x210 */ float f210;
    /* 0x214 */ float lo;
    /* 0x218 */ float hi;
    /* 0x21C */ float f21C;
    /* 0x220 */ float f220;
    /* 0x224 */ float col[3];
};
extern "C" void snoweffects_uso_func_0000718C(SnowEmC *t) {
    float a, sp, r;
    int i, n, go;
    if (t->cam->state != 99 || t->i80 != 0) {
        return;
    }
    sp = t->speed;
    if (t->cam->f348 < 60.0f || (sp < 0.0f ? -sp : sp) < t->lo) {
        a = 0.0f;
    } else if (t->hi < (sp < 0.0f ? -sp : sp)) {
        if (sp < 0.0f) {
            a = -1.0f;
        } else {
            a = 1.0f;
        }
    } else {
        r = ((sp < 0.0f ? -sp : sp) - t->lo) / (t->hi - t->lo);
        if (sp < 0.0f) {
            a = -((1.0f - t->f21C) * r + t->f21C);
        } else {
            a = (1.0f - t->f21C) * r + t->f21C;
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
    n = a != 0.0f;
    if ((n < 0 ? -n : n) == 0) return;
    for (i = 0; i < 1; i++) {
        SPart *p = gl_func_00000000_snow_pool(t->pool);
        if (p == 0) {
            p = gl_func_00000000_snow_pool(t->pool);
        }
        if (p != 0) {
            SVb w;
            SVb u = t->pos;
            r = gl_func_00000000_snow_randf();
            u.lerpEach(t->vF0, r);
            { int sc[2]; sc[0] = 0; }
            w = t->cam->v318 * t->f208;
            if (0) w *= t->f20C; /* run 698: one more edg float temporary (objdiff 96.99 -> 97.07) */
            SVb a1 = t->cam->v3B0 * t->f20C * (a < 0.0f ? -a : a);
            SVb a2 = t->v114 * (1.0f + gl_func_00000000_snow_randf() * snow_ro_B0) * t->f210;
            a2 *= (a < 0.0f ? -a : a);
            if (0.0f < a) {
                a2 *= -1.0f;
            }
            w += a2;
            w += a1;
            p->pos.set(u.x, u.y, u.z);
            p->vel.set(w.x, w.y, w.z);
            p->size = t->f1F8;
            p->unk3C = 1.0f;
            p->fadeAt = 256.0f;
            p->sizeVel = t->f1DC;
            p->gravity = t->f1E0;
            p->decay = t->f1E8;
            p->life = 256.0f;
            r = gl_func_00000000_snow_randf();
            p->alphaVel = t->f1EC;
            p->alpha = 128.0f + r * 127.0f;
            {
                float c, f = t->f220;
                c = t->cam->col.c[0];
                p->col.c[0] = (int)(c + (t->col[0] * 255.0f - c) * f);
                c = t->cam->col.c[1];
                p->col.c[1] = (int)(c + (t->col[1] * 255.0f - c) * f);
                c = t->cam->col.c[2];
                p->col.c[2] = (int)(c + (t->col[2] * 255.0f - c) * f);
            }
        }
    }
}
