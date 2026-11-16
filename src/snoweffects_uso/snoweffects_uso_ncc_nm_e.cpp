/* IDO 7.1 NCC (C++) NON_MATCHING donors for snoweffects.uso functions at 0x6000+ (agent-e run 664), spliced
 * over the empty NON_MATCHING placeholders in snoweffects_uso.c on the non_matching build only (the matching
 * build keeps INCLUDE_ASM). Callees go through typed blank (0-pinned) aliases; RoData constants through
 * 0-pinned snow_ro_* symbols. Spliced: 0x7960 (99.7 %) and 0x64B8 (94.2 %, run 668: the
 * lerp reads u into locals and the pos / vel stores go through set(), which forwards the lerped point and
 * brings the body to the target's 404 words), 0x6B28 (90.7 %, run 668). */
#include "snoweffects_ncc_e.h"
extern "C" float gl_func_00000000_snow_52d4(void *self, void *pos, void *out, void *a3);  /* 0x52D4 probe */
extern "C" float gl_func_00000000_snow_ff(float);  /* math_acosf / fcos / fsin: float(float) blank jal */
extern "C" float snow_ro_B4;  /* RoData+0xB4: the 0x7960 minimum speed */
extern "C" float snow_ro_50, snow_ro_54;  /* RoData+0x50 / +0x54: the 0x64B8 burst speed scale / bias */

/* the libgdl Vec3 as the 0x7960 body uses it (value temporaries, inline helpers) */
struct SnowV {
    float x, y, z;
    SnowV() {}
    SnowV(float a, float b, float c) { x = a; y = b; z = c; }
    SnowV operator*(float s) const { return SnowV(x * s, y * s, z * s); }
    SnowV &operator+=(const SnowV &v) { x += v.x; y += v.y; z += v.z; return *this; }
};
inline float dotv(const SnowV &a, const SnowV &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
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
    ang = gl_func_00000000_snow_ff(t->dist = -dotv(t->p, t->n));
    if (dotv(t->cam->fwd, t->cam->dir) < 0.0f) {
        ang = -ang;
    }
    t->hits = 0;
    if (!((t->speed < 0.0f ? -t->speed : t->speed) < snow_ro_B4)) {
        float d = gl_func_00000000_snow_52d4(t, &t->pos, &n, t->qD4);
        if (d != 0.0f && d < t->maxDist) {
            SnowV push = n * -d;
            t->pos += push;
            t->hits = 2;
            { int sc[2]; sc[0] = 0; }
            t->a.x = gl_func_00000000_snow_ff(ang) * t->w;
            t->a.y = 0.0f;
            t->a.z = gl_func_00000000_snow_ff(ang) * t->h;
            xform(t->a, *t->mtx);
            t->b.x = gl_func_00000000_snow_ff(ang) * -t->w;
            t->b.y = 0.0f;
            t->b.z = gl_func_00000000_snow_ff(ang) * -t->h;
            xform(t->b, *t->mtx);
            { int sc2[2]; sc2[0] = 0; }
        }
    }
}

struct SV {
    float x, y, z;
    SV() {}
    SV(float a, float b, float c) { x = a; y = b; z = c; }
    SV &operator=(SV o) { x = o.x; y = o.y; z = o.z; return *this; }
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    SV operator*(float s) const { return SV(x * s, y * s, z * s); }
    SV &operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
    SV &operator+=(const SV &v) { x += v.x; y += v.y; z += v.z; return *this; }
};
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
    /* 0xF0 */ SV vF0;
    /* 0xFC */ SV vFC;
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
        if (--t->timer < 0) {
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
                    SPart *p = gl_func_00000000_snow_pool(t->pool);
                    if (p == 0) {
                        p = gl_func_00000000_snow_pool(t->pool);
                    }
                    if (p != 0) {
                        SV u = t->vF0 + t->vFC * (t->f3C * 2.0f);
                        float r = gl_func_00000000_snow_randf();
                        { float x = u.x, y = u.y, z = u.z;
                        u.x = (t->vF0.x - x) * r + x;
                        u.y = (t->vF0.y - y) * r + y;
                        u.z = (t->vF0.z - z) * r + z; }
                        p->pos.set(u.x, u.y + t->f178, u.z);
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
                        p->life = 256.0f;
                        p->decay = t->f18C;
                        p->alpha = 256.0f;
                        p->alphaVel = t->f188;
                    }
                }
            }
        }
    }
}

/* 0x6B28: spawn update of effect B (flag bit 1), run 668: two RoData default sets picked by the game
 * flags, the course-state counters +0x1C8 / +0x1CC, then a camera-relative burst spawn (90.7 %, 357 words
 * vs 358: the target materialises &vFC for the scale). */
extern "C" float snow_ro_58, snow_ro_5C, snow_ro_60, snow_ro_64, snow_ro_68;
extern "C" float snow_ro_6C, snow_ro_70, snow_ro_74, snow_ro_78, snow_ro_7C;
struct SnowGame { char pad[0x8C]; /* 0x8C */ int flags; };
extern "C" SnowGame snow_game_6B28;  /* a game-state global (+0x8C flags, bits 0x820000) */
struct SnowCamB {
    char pad000[0x318];
    /* 0x318 */ SV v318;
    char pad324[0x348 - 0x324];
    /* 0x348 */ float f348;
    char pad34C[0x3B0 - 0x34C];
    /* 0x3B0 */ SV v3B0;
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
    /* 0xF0 */ SV vF0;
    /* 0xFC */ SV vFC;
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
        if (0.0f < t->f1D8 && 0.0f == t->cam->fA20) {
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
            SV n = t->cam->v318 * t->f1C4;
            n += t->cam->v3B0 * t->f1B8;
            SPart *p = gl_func_00000000_snow_pool(t->pool);
            if (p == 0) {
                p = gl_func_00000000_snow_pool(t->pool);
            }
            if (p != 0) {
                float x, y, z;
                s = t->speed < 0.0f ? -t->speed : t->speed;
                if (s < 0.5) {
                    s = 0.5f;
                }
                SV u = t->vF0 + t->vFC * (t->f3C * 2.0f * s);
                float r = gl_func_00000000_snow_randf();
                { x = u.x; y = u.y; z = u.z;
                u.x = (t->vF0.x - x) * r + x;
                u.y = (t->vF0.y - y) * r + y;
                u.z = (t->vF0.z - z) * r + z; }
                p->pos.set(u.x, u.y, u.z);
                p->vel.set(n.x, n.y, n.z);
                p->fadeAt = 256.0f;
                p->size = 1.0f;
                p->unk3C = 1.0f;
                p->col.c[0] = col.c[0];
                p->col.c[1] = col.c[1];
                p->col.c[2] = col.c[2];
                p->sizeVel = t->f1B0;
                p->gravity = t->f1B4;
                p->life = 256.0f;
                p->decay = t->f1BC;
                p->alpha = 256.0f;
                p->alphaVel = t->f1C0;
            }
        }
    }
    if (t->i80 == 0) {
        t->i1D0 = t->cam->state;
    }
}
