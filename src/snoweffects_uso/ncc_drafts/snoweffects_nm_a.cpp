/* NCC (C++) near-miss DRAFTS for snoweffects.uso 0xA10C and 0x7FE0 -- NOT built, NOT matching
 * (agent-a run 661). Both functions stay INCLUDE_ASM in snoweffects_uso.c.
 *
 * Compile standalone with IDO 7.1 NCC from the project root:
 *   tools/ido-static-recomp/build/7.1/out/NCC -c -G 0 -non_shared -Wab,-r4300_mul -O2 -mips2 -32
 *       -o /tmp/x.o src/snoweffects_uso/ncc_drafts/snoweffects_nm_a.cpp
 * and diff each function against expected/src/snoweffects_uso/snoweffects_uso.c.o.
 * A matching donor would need typed 0-pinned aliases for the blank jal targets and a `_rodata` pin
 * (A10C: 1/127 at RoData+0xF4, donor .rodata+0).
 */
#include "../snoweffects_ncc.h"
/* the callees, by name (a matching donor would call them through typed 0-pinned aliases) */
extern "C" {
int ItemMgr_findType_0003EA98(...);
int PolyShape_getNormal8(...);
int PolyShape_getVertex(...);
int snoweffects_uso_func_00004374(...);
int snoweffects_uso_func_000046BC(...);
int snoweffects_uso_func_00009768(...);
int snoweffects_uso_func_00009884(...);
int snoweffects_uso_func_00009BE8(...);
int snoweffects_uso_func_00009C48(...);
int snoweffects_uso_func_00009FE8(...);
}

/* ---- 0xA10C snow light update: 401 / 405 instructions, 150 words off (frame and every instruction
 * shape match except: the ambient Parm pointer copy colours a0 -> v1 where the target has v1 -> s1;
 * the normal bytes sit at sp+120 (target sp+112: one more word in the loop-body block); the ItemType
 * walks lack the target's item copy (data in s1, item in s0); the second walk forwards the flags
 * value where the target reloads +0x234 after each store). Frame levers found in run 661 (read with
 * -O2 -g3 .mdebug): the second walk's ItemType at function scope (its by-value SKey temp is then the
 * first frame word, vfp-4 = sp+172), the first walk not in its own block (key temp vfp-28 = sp+148),
 * c / i at function scope, and the ambient Parm bound once as `const SParm<SVec> &a` before the
 * compare. SKey(int) ctor gives the `sw a1, K(sp)` arg-temp store. ---- */
extern "C" int snowDirtyFlag_A30;
extern char *snow_itemMgr;
struct SVec { float x, y, z; SVec &operator+=(const SVec &o) { x += o.x; y += o.y; z += o.z; return *this; } void set(float a, float b, float c) { x = a; y = b; z = c; } };
template <class T> struct SParm { char hdr[0x10]; T v; const T &get() const { return v; } };
struct SCol { float r, g, b; int a; };
struct SMtx { float m[4][4]; };
struct SSrc { char pad[0x70]; SMtx mtx; char padB0[0x114 - 0xB0]; SParm<SCol> colour; };
struct SAmb { char pad[0x114]; SParm<SVec> v; };
struct STgt { char pad[0x2C]; SMtx mtx; char pad6C[0x278 - 0x6C]; SParm<SCol> colourParm; };
struct SShape { char pad[0x40]; unsigned int nVtx; char pad44[0x50 - 0x44]; unsigned char *cols; char pad54[0xC4 - 0x54]; unsigned char *drawCols; };
struct SKey { int v; SKey(int a) { v = a; } };
struct SIt { char *data; SIt *next; };
struct SType { char pad[0x10]; SIt *items; };
struct SItem { char pad[0x230]; int vtxCount; int flags; int f238; int f23C; };
struct SLights {
    char pad[0x2C];
    SShape *shape;   /* 0x2C */
    SAmb *ambient;   /* 0x30 */
    SSrc *src;       /* 0x34 */
    STgt *tgt;       /* 0x38 */
    char pad3C[0x40 - 0x3C];
    SVec amb;        /* 0x40 */
};
#define ITER_NEXT(it) (((it)[0] = (it)[1], (it)[1] != 0) ? ((it)[1] = (it)[1]->next, (it)[0]->data) : 0)
struct SIter {
    SIt *cur, *nxt;
    SIter(SIt *l) { nxt = l; }
    char *next() { cur = nxt; if (nxt != 0) { nxt = nxt->next; return cur->data; } return 0; }
};
struct SIterB {
    SIt *it[2];
};
extern "C" void snoweffects_uso_func_0000A10C(SLights *t) {
    SType *type;
    unsigned char *c;
    unsigned int i;
    t->tgt->mtx = t->src->mtx;
    t->tgt->colourParm.v = t->src->colour.get();
    snoweffects_uso_func_00009768(t->tgt);
    const SParm<SVec> &a = t->ambient->v;
    if (t->amb.x != a.get().x || t->amb.y != a.get().y || t->amb.z != a.get().z) {
        t->amb.x = t->ambient->v.get().x;
        t->amb.y = t->ambient->v.get().y;
        t->amb.z = t->ambient->v.get().z;
        snowDirtyFlag_A30 = 1;
    }
    if (snowDirtyFlag_A30) {
        c = t->shape->cols;
        for (i = 0; i < t->shape->nVtx; i++) {
            SVec col;
            short *v = (short *)PolyShape_getVertex(t->shape, i);
            SVec *p = (SVec *)(D_00000000 + 0x40);
            p->x = v[0];
            p->y = v[1];
            p->z = v[2];
            {
                signed char n8[4];
                PolyShape_getNormal8(t->shape, i, n8);
                SVec *n = (SVec *)(D_00000000 + 0x50);
                n->x = n8[0] * (1.0f / 127.0f);
                n->y = n8[1] * (1.0f / 127.0f);
                n->z = n8[2] * (1.0f / 127.0f);
            }
            col.set(0.0f, 0.0f, 0.0f);
            col.x += t->ambient->v.get().x;
            col.y += t->ambient->v.get().y;
            col.z += t->ambient->v.get().z;
            snoweffects_uso_func_00009FE8(t, t->tgt, &col);
            int j;
            for (j = 0; j < 3; j++) {
                float *f = &col.x + j;
                *f *= 255.0f;
                if (*f < 0.0f) *f = 0.0f;
                if (255.0f < *f) *f = 255.0f;
            }
            c[0] = col.x;
            c[1] = col.y;
            c[2] = col.z;
            c[3] = 255;
            c += 4;
        }
        type = (SType *)ItemMgr_findType_0003EA98(snow_itemMgr, SKey(*(int *)(D_00000000 + 0xA78)));
        if (type != 0) {
            SIter it(type->items);
            while (it.next() != 0) {
                SItem *item = (SItem *)it.cur->data;
                if (item->vtxCount == 0) {
                    snoweffects_uso_func_00009884(item, t->shape);
                }
                snoweffects_uso_func_00009BE8(item, t->shape);
                item->flags = 4;
            }
        }
        t->shape->drawCols = t->shape->cols;
        snowDirtyFlag_A30 = 0;
    }
    type = (SType *)ItemMgr_findType_0003EA98(snow_itemMgr, SKey(*(int *)(D_00000000 + 0xA7C)));
    {
        if (type != 0) {
            SIter it(type->items);
            while (it.next() != 0) {
                SItem *item = (SItem *)it.cur->data;
                if (item->flags & 2) {
                    snoweffects_uso_func_00009884(item, t->shape);
                    item->flags &= ~2;
                }
                if (item->flags & 1) {
                    snoweffects_uso_func_00009BE8(item, t->shape);
                    item->f238 = 0;
                    item->f23C = 0;
                    item->flags &= ~1;
                }
                if (item->flags & 4) {
                    snoweffects_uso_func_00009C48(item, t->shape);
                    item->flags &= ~4;
                }
            }
        }
    }
}

/* ---- 0x7FE0 trail emitter update (decoded whole, first NCC draft: 789 / 800 instructions,
 * frame 320 vs 384, far from the 80 % NM threshold). Structure: speed (+0x4C) = +-cam lean (+0x970,
 * sign from cam flags +0xA58 & 0x80), clear +0x120..+0x134, copy +0x34/+0x38 -> +0x3C/+0x40,
 * 0x4374(this); s = 0 below cam speed 60 (+0x348) or |speed| < lo (+0x1A4), +-1 above hi (+0x1A8),
 * else +-((1 - base) * t + base); sign flips clear +0x144; s -> +0x1A0; state (+0x9A2) != 97 or slow
 * or s == 0 clears +0x144; +0x74 = 0x46BC(this), slots[write].unk04[0] = mode (+0x1EC);
 * 0x4714(this, on, prevOn, w = +0x164); s *= 0.25 when the write slot is live; then for each of the
 * stride (+0x6C) points of the write ring row: pos = +0x138 (mode 2) or lerp(+0x120, +0x12C, i/(n-1)),
 * vel = cam +0x318 * +0x188 plus side / lift terms (cam +0x3B0 * +0x180 * |s|, +0x114 * (i or n-1-i)/n
 * * +0x184 * |s|, negated when s > 0), w[1..3], vtx s/t (texStep +0xA8, *3 unless mode 2, * 64) and
 * the cam colour bytes (+0xA30..; points 1 and 2 faded towards 255 by +0x178, 0 in state 98).
 * Edg temp chains: op* result -> return temp (sp+244) -> by-value operator= param (sp+188). Next:
 * the frame (target named Vec3 locals at 336 / 324 / 312 / 300 / 284 / 272 / 100) and in-loop
 * 64.0f. ---- */
extern "C" void SnowRing_setSlotState_00004714(void *, int, int, float);

struct TV {
    float x, y, z;
    TV() {}
    TV(float a, float b, float c) { x = a; y = b; z = c; }
    TV operator*(float s) const { TV r(x * s, y * s, z * s); return r; }
    TV &operator=(const TV &o) { x = o.x; y = o.y; z = o.z; return *this; }
    TV &operator+=(const TV &o) { x += o.x; y += o.y; z += o.z; return *this; }
    TV &operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
};
struct TCam {
    char pad0[0x318];
    /* 0x318 */ TV fwd;
    char pad324[0x348 - 0x324];
    /* 0x348 */ float speed;
    char pad34C[0x3B0 - 0x34C];
    /* 0x3B0 */ TV up;
    char pad3BC[0x970 - 0x3BC];
    /* 0x970 */ float lean;
    char pad974[0x9A2 - 0x974];
    /* 0x9A2 */ short state;
    char pad9A4[0xA30 - 0x9A4];
    /* 0xA30 */ unsigned char rgb[3];
    char padA33[0xA58 - 0xA33];
    /* 0xA58 */ int flags;
};
struct TPoint { TV pos; TV vel; float w[4]; int f28; };
struct TSlot { int state; int unk04[2]; };
struct TVtx { short ob[3]; short flag; short s, t; signed char rgba[4]; };
struct TTrail {
    char pad00[0x2C];
    /* 0x2C */ TCam *cam;
    char pad30[0x34 - 0x30];
    /* 0x34 */ float f34;
    /* 0x38 */ float f38;
    /* 0x3C */ float f3C;
    /* 0x40 */ float f40;
    char pad44[0x4C - 0x44];
    /* 0x4C */ float speed;
    char pad50[0x5C - 0x50];
    /* 0x5C */ int writeOfs;
    /* 0x60 */ int read;
    /* 0x64 */ int write;
    /* 0x68 */ int size;
    /* 0x6C */ int stride;
    /* 0x70 */ int count;
    /* 0x74 */ int on;
    /* 0x78 */ int prevOn;
    /* 0x7C */ int prevState;
    char pad80[0x8C - 0x80];
    /* 0x8C */ TVtx *vtx;
    /* 0x90 */ TPoint *points;
    /* 0x94 */ TSlot *slots;
    char pad98[0xA8 - 0x98];
    /* 0xA8 */ int texStep;
    char padAC[0xFC - 0xAC];
    /* 0xFC */ TV vFC;
    char pad108[0x114 - 0x108];
    /* 0x114 */ TV v114;
    /* 0x120 */ TV a;
    /* 0x12C */ TV b;
    /* 0x138 */ TV mid;
    /* 0x144 */ int hits;
    char pad148[0x164 - 0x148];
    /* 0x164 */ float f164;
    char pad168[0x16C - 0x168];
    /* 0x16C */ float f16C;
    char pad170[0x178 - 0x170];
    /* 0x178 */ float fade;
    /* 0x17C */ float f17C;
    /* 0x180 */ float f180;
    /* 0x184 */ float f184;
    /* 0x188 */ float f188;
    char pad18C[0x19C - 0x18C];
    /* 0x19C */ float f19C;
    /* 0x1A0 */ float s;
    /* 0x1A4 */ float lo;
    /* 0x1A8 */ float hi;
    /* 0x1AC */ float base;
    char pad1B0[0x1EC - 0x1B0];
    /* 0x1EC */ int mode;
};

extern "C" void snoweffects_uso_func_00007FE0(TTrail *t) {
    float s;
    float w;
    int i;
    if (t->cam->flags & 0x80) {
        t->speed = -t->cam->lean;
    } else {
        t->speed = t->cam->lean;
    }
    t->a.x = 0.0f;
    t->a.y = 0.0f;
    t->a.z = 0.0f;
    t->b.x = 0.0f;
    t->b.y = 0.0f;
    t->b.z = 0.0f;
    t->f3C = t->f34;
    t->f40 = t->f38;
    snoweffects_uso_func_00004374(t);
    if (t->cam->speed < 60.0f) {
        s = 0.0f;
    } else if ((t->speed < 0.0f ? -t->speed : t->speed) < t->lo) {
        s = 0.0f;
    } else if (t->hi < (t->speed < 0.0f ? -t->speed : t->speed)) {
        s = t->speed < 0.0f ? -1.0f : 1.0f;
    } else {
        float f = ((t->speed < 0.0f ? -t->speed : t->speed) - t->lo) / (t->hi - t->lo);
        if (t->speed < 0.0f) {
            s = -((1.0f - t->base) * f + t->base);
        } else {
            s = (1.0f - t->base) * f + t->base;
        }
    }
    if (s < 0.0f && 0.0f < t->s) {
        t->hits = 0;
    }
    if (0.0f < s && t->s < 0.0f) {
        t->hits = 0;
    }
    t->s = s;
    if (t->cam->state != 97 || t->cam->speed < 60.0f || (s < 0.0f ? -s : s) == 0.0f) {
        t->hits = 0;
    }
    t->on = snoweffects_uso_func_000046BC(t);
    t->slots[t->write].unk04[0] = t->mode;
    w = t->f164;
    SnowRing_setSlotState_00004714(t, t->on, t->prevOn, w);
    if (t->slots[t->write].state == 2) {
        s *= 0.25f;
    }
    if (t->on != 0) {
        float fade = t->fade;
        if (t->cam->state == 98) {
            fade = 0.0f;
        }
        for (i = 0; i < t->stride; i++) {
            TV p;
            if (t->mode == 2) {
                p = t->mid;
            } else {
                p = t->a;
                float f = (float)i / (float)(t->stride - 1);
                p.x = (t->b.x - p.x) * f + p.x;
                p.y = (t->b.y - p.y) * f + p.y;
                p.z = (t->b.z - p.z) * f + p.z;
            }
            t->points[t->writeOfs + i].pos = p;
            t->points[t->writeOfs + i].vel = t->cam->fwd * t->f188;
            t->points[t->writeOfs + i].f28 = 0;
            t->points[t->writeOfs + i].w[2] = w;
            t->points[t->writeOfs + i].w[3] = t->f16C;
            TV up = t->cam->up * t->f180;
            TV a = up * (s < 0.0f ? -s : s);
            TV b = t->v114 * ((float)(s < 0.0f ? t->stride - i - 1 : i) / (float)t->stride);
            TV c = b * t->f184;
            TV d = c * (s < 0.0f ? -s : s);
            TV e = t->vFC * -1.0f;
            if (0.0f < s) {
                d *= -1.0f;
            }
            t->points[t->writeOfs + i].vel += d;
            if (i == 3) {
                t->points[t->writeOfs + i].w[1] = t->f17C * t->f19C;
            }
            if (i == 2) {
                t->points[t->writeOfs + i].vel += a;
                t->points[t->writeOfs + i].w[1] = t->f17C;
            }
            if (i == 1) {
                t->points[t->writeOfs + i].vel += a;
                t->points[t->writeOfs + i].w[1] = t->f17C;
            }
            if (i == 0) {
                t->points[t->writeOfs + i].w[1] = t->f17C * t->f19C;
            }
            float u = i * t->texStep - 0.5;
            if (t->mode != 2) {
                u *= 3.0f;
            }
            t->vtx[t->writeOfs + i].s = u * 64.0f;
            t->vtx[t->writeOfs + i].t = (float)(((t->writeOfs & t->stride) >> 2) * 31) * 64.0f;
            if (i == 1 || i == 2) {
                t->vtx[t->writeOfs + i].rgba[0] = t->cam->rgb[0] + (255.0f - t->cam->rgb[0]) * fade;
                t->vtx[t->writeOfs + i].rgba[1] = t->cam->rgb[1] + (255.0f - t->cam->rgb[1]) * fade;
                t->vtx[t->writeOfs + i].rgba[2] = t->cam->rgb[2] + (255.0f - t->cam->rgb[2]) * fade;
            } else {
                t->vtx[t->writeOfs + i].rgba[0] = t->cam->rgb[0];
                t->vtx[t->writeOfs + i].rgba[1] = t->cam->rgb[1];
                t->vtx[t->writeOfs + i].rgba[2] = t->cam->rgb[2];
            }
        }
    }
    t->prevOn = t->on;
    t->prevState = t->cam->state;
}
