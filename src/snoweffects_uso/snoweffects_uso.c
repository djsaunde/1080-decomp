#include "common.h"
#include "uso_call.h"
#include "gdl/node.h"
#include "gdl/item.h"

extern int gl_func_00000000();
extern char D_00000000;

typedef struct { int a, b, c, d; } Quad4;
typedef struct { int a, b, c; } Tri3i;
typedef struct { float x, y, z; } Vec3;

/* the 0x40-byte course item the ItemType create methods allocate (0x7D8C / 0x91BC, the
 * edd_ItemType_create_000003BC twin): a ColourItem plus one word the create clears */
typedef struct SnowColourItem {
    /* 0x00 */ ColourItem base;
    /* 0x3C */ s32 unk3C;
} SnowColourItem;

/* one 0x58-byte snow particle record (SnowPool items) */
typedef struct SnowParticle {
    /* 0x00 */ char unk00[0x30];
    /* 0x30 */ f32 unk30;
    /* 0x34 */ char unk34[0x24];
} SnowParticle;

/* a Node-derived particle pool: items[max] from mem_new, count in use */
typedef struct SnowPool {
    /* 0x00 */ Node node;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ SnowParticle *items;
    /* 0x34 */ s32 unk34[2];
    /* 0x3C */ s32 count;
    /* 0x40 */ s32 max;
} SnowPool;

float snoweffects_uso_func_00000000(Vec3 *a, Vec3 *b);

/* the snow effect object whose 0x6144 / 0x6248 create the two child nodes at +0x98 / +0xA0 (only the
 * fields its default inits 0x6420 / 0x70C0 set) */
typedef struct SnowEffect {
    /* 0x000 */ char unk000[0x54];
    /* 0x054 */ s32 flags;
    /* 0x058 */ char unk058[0x174 - 0x58];
    /* 0x174 */ f32 unk174;
    /* 0x178 */ f32 unk178;
    /* 0x17C */ f32 unk17C;
    /* 0x180 */ f32 unk180;
    /* 0x184 */ f32 unk184;
    /* 0x188 */ f32 unk188;
    /* 0x18C */ f32 unk18C;
    /* 0x190 */ f32 unk190;
    /* 0x194 */ f32 unk194;
    /* 0x198 */ f32 unk198;
    /* 0x19C */ s32 unk19C;
    /* 0x1A0 */ s32 unk1A0;
    /* 0x1A4 */ s32 unk1A4;
    /* 0x1A8 */ s32 unk1A8;
    /* 0x1AC */ char unk1AC[0x1DC - 0x1AC];
    /* 0x1DC */ f32 unk1DC;
    /* 0x1E0 */ f32 unk1E0;
    /* 0x1E4 */ f32 unk1E4;
    /* 0x1E8 */ f32 unk1E8;
    /* 0x1EC */ f32 unk1EC;
    /* 0x1F0 */ f32 unk1F0;
    /* 0x1F4 */ f32 unk1F4;
    /* 0x1F8 */ f32 unk1F8;
    /* 0x1FC */ s32 unk1FC;
    /* 0x200 */ s32 unk200;
    /* 0x204 */ s32 unk204;
    /* 0x208 */ f32 unk208;
    /* 0x20C */ f32 unk20C;
    /* 0x210 */ f32 unk210;
    /* 0x214 */ f32 unk214;
    /* 0x218 */ f32 unk218;
    /* 0x21C */ f32 unk21C;
    /* 0x220 */ f32 unk220;
    /* 0x224 */ Vec3 scale;
    /* 0x230 */ f32 unk230;
} SnowEffect;

/* one 0x2C-byte emitter point (3EE4 allocates count of them at +0x90 and clears each with 0x3EB0) */
typedef struct SnowPoint {
    /* 0x00 */ Vec3 pos;
    /* 0x0C */ Vec3 vel;
    /* 0x18 */ f32 w[4];
    /* 0x28 */ s32 unk28;
} SnowPoint;

/* one 12-byte ring slot (state 2 = live, 0 = cleared; 0x4714) */
typedef struct SnowSlot {
    /* 0x00 */ s32 state;
    /* 0x04 */ s32 unk04[2];
} SnowSlot;

/* the trail emitter (a Node subclass, set up by 0x3EE4): a ring of `count` slots of `stride`
 * points each, stepped by 0x46BC / 0x535C / 0x53E8 */
typedef struct SnowRing {
    /* 0x00 */ Node node;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ char unk3C[8];
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ char unk4C[8];
    /* 0x54 */ s32 unk54;
    /* 0x58 */ s32 readOfs;
    /* 0x5C */ s32 writeOfs;
    /* 0x60 */ s32 read;
    /* 0x64 */ s32 write;
    /* 0x68 */ s32 size;            /* stride * count points */
    /* 0x6C */ s32 stride;
    /* 0x70 */ s32 count;
    /* 0x74 */ s32 unk74;
    /* 0x78 */ s32 unk78;
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ s32 unk80;
    /* 0x84 */ s32 unk84;
    /* 0x88 */ s32 unk88;
    /* 0x8C */ void *vtx;           /* size Vtx (16-byte aligned) */
    /* 0x90 */ SnowPoint *points;   /* size points */
    /* 0x94 */ SnowSlot *slots;     /* count slots */
    /* 0x98 */ char unk98[0x18];
    /* 0xB0 */ void *dl;            /* gfx_newDLContext(0, .., 0x18, ..) */
    /* 0xB4 */ char unkB4[0x120 - 0xB4];
    /* 0x120 */ Vec3 unk120;
    /* 0x12C */ Vec3 unk12C;
    /* 0x138 */ char unk138[0x154 - 0x138];
    /* 0x154 */ f32 unk154[8];
} SnowRing;

/* typed blank (0) alias for the reloc'd call to the global 0x3EE4 (floats on the stack) */
extern void gl_func_00000000_snow_3ee4(void *self, int a1, int a2, int a3, int a4, float f5, float f6);

typedef struct SnowRGBA {
    u8 r, g, b, a;
} SnowRGBA;

/* CollShape_nearestHit's result record (only pos / dist are read here) */
typedef struct SnowHit {
    /* 0x00 */ Tri3i pos;
    /* 0x0C */ f32 dist;
    /* 0x10 */ s32 unk10[7];
} SnowHit;

/* typed blank (0) alias for the CollShape_nearestHit import: the float radius goes in a2 */
extern int gl_func_00000000_snow_hit(int shape, int ray, float radius, SnowHit *hit);

float snoweffects_uso_func_00000000(Vec3 *a, Vec3 *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

float snoweffects_uso_func_00000030(float *p, float *pl) {
    return -(p[1] * pl[1] + p[2] * pl[2] - pl[3]) / pl[0];
}

float snoweffects_uso_func_00000064(float *p, float *pl) {
    return -(p[0] * pl[0] + p[2] * pl[2] - pl[3]) / pl[1];
}

float snoweffects_uso_func_00000098(float *p, float *pl) {
    return -(p[0] * pl[0] + p[1] * pl[1] - pl[3]) / pl[2];
}

void snoweffects_uso_func_000000CC(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void snoweffects_uso_func_00000108(float *dst) {
    float buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void snoweffects_uso_func_00000144(Quad4 *dst) {
    Quad4 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 16);
    *dst = buf;
}

void snoweffects_uso_func_0000019C(Vec3 *dst) {
    int pad_top[1];
    Tri3i raw;
    int pad_mid[2];
    Tri3i tmp;
    int pad_bot[2];
    CALL(fs_gdlReadFile)(&D_00000000, &raw, 12);
    tmp = raw;
    dst->x = *(float*)&tmp.a;
    dst->y = *(float*)&tmp.b;
    dst->z = *(float*)&tmp.c;
}

void snoweffects_uso_func_0000020C(SnowPool *this, int n) {
    this->items = (SnowParticle *)CALL(mem_new)(n * sizeof(SnowParticle));
    this->count = 0;
    this->max = n;
    VT_SLOT(this->node.vptr, 11).fn((char *)this + VT_SLOT(this->node.vptr, 11).delta);
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00000270);

SnowParticle *snoweffects_uso_func_00000680(SnowPool *this) {
    float best = 65536.0f;
    SnowParticle *p = this->items;
    SnowParticle *found = 0;
    int i;
    for (i = 0; i < this->count; i++, p++) {
        if (p->unk30 < best) {
            best = p->unk30;
            found = p;
        }
    }
    return found;
}

SnowParticle *snoweffects_uso_func_000006D0(SnowPool *this) {
    if (this->count < this->max) {
        this->count++;
        return &this->items[this->count - 1];
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00000718);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000097C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00000A7C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00000B44);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00000C90);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000015D0);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00001A18);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00001E60);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00002310);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000027A8);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000028E0);

void snoweffects_uso_func_00003544(char *this, float *out, Vec3 *v) {
    out[0] = snoweffects_uso_func_00000000(v, (Vec3 *)(this + 0x74));
    out[1] = snoweffects_uso_func_00000000(v, (Vec3 *)(this + 0x80));
    out[2] = snoweffects_uso_func_00000000(v, (Vec3 *)(this + 0x8C));
}

void snoweffects_uso_func_000035B0(char *this, float *n, float *p, float s) {
    Vec3 t;
    (void)&this;
    t.x = p[0] - n[0] * s;
    t.y = p[1] - n[1] * s;
    t.z = p[2] - n[2] * s;
    n[3] = snoweffects_uso_func_00000000(&t, (Vec3 *)n);
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00003620);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000374C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000039C4);

void snoweffects_uso_func_00003CA4(char *a0) {
    int tag;
    snoweffects_uso_func_000000CC(&tag);
    snoweffects_uso_func_00000108((float *)(a0 + 0x10));
}

void snoweffects_uso_func_00003CD4(char *a0) {
    int tag;
    snoweffects_uso_func_000000CC(&tag);
    snoweffects_uso_func_00000144((Quad4 *)(a0 + 0x10));
}

void snoweffects_uso_func_00003D04(char *a0) {
    int tag;
    snoweffects_uso_func_000000CC(&tag);
    snoweffects_uso_func_000000CC((int *)(a0 + 0x10));
}

void snoweffects_uso_func_00003D34(char *a0) {
    int tag;
    snoweffects_uso_func_000000CC(&tag);
    snoweffects_uso_func_0000019C((Vec3 *)(a0 + 0x10));
}

void snoweffects_uso_func_00003D70(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void snoweffects_uso_func_00003DAC(float *dst) {
    float buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void snoweffects_uso_func_00003DE8(Quad4 *dst) {
    Quad4 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 16);
    *dst = buf;
}

void snoweffects_uso_func_00003E40(Vec3 *dst) {
    int pad_top[1];
    Tri3i raw;
    int pad_mid[2];
    Tri3i tmp;
    int pad_bot[2];
    CALL(fs_gdlReadFile)(&D_00000000, &raw, 12);
    tmp = raw;
    dst->x = *(float*)&tmp.a;
    dst->y = *(float*)&tmp.b;
    dst->z = *(float*)&tmp.c;
}

/* clear a point: the second mtc1 zero of the target is the int 0 of a chained assignment
 * (a 0.0f chain shares the first register) */
void snoweffects_uso_func_00003EB0(SnowPoint *p) {
    p->pos.x = 0.0f;
    p->pos.y = 0.0f;
    p->pos.z = 0.0f;
    p->vel.x = 0.0f;
    p->vel.y = 0.0f;
    p->vel.z = 0.0f;
    p->w[0] = p->w[1] = p->w[2] = p->w[3] = 0;
}

/* two RoData floats of the emitter defaults (separate lui each: one pin per constant) */
extern f32 snow_ro_20;
extern f32 snow_ro_24;

/* emitter init: ring geometry, the Vtx / point / slot arrays, the display-list context, then the
 * virtual reset (slot 11) and the default colour / size parameters */
void snoweffects_uso_func_00003EE4(SnowRing *this, int a1, int stride, int count, int a4, float f5, float f6) {
    int i;
    int n;
    this->unk2C = a1;
    this->unk30 = a4;
    this->unk34 = f5;
    this->unk38 = f6;
    this->stride = stride;
    this->count = count;
    this->size = count * stride;
    this->unk54 = 0;
    this->unk78 = 0;
    this->unk80 = 0;
    this->unk7C = -1;
    this->read = 0;
    this->readOfs = 0;
    this->write = 0;
    this->writeOfs = 0;
    this->vtx = (void *)CALL(mem_newAligned)(this->size << 4, 16);
    n = this->size;
    this->points = (SnowPoint *)CALL(mem_new)(n * sizeof(SnowPoint));
    for (i = 0; i < this->size; i++) {
        CALL(snoweffects_uso_func_00003EB0)(&this->points[i]);
    }
    this->slots = (SnowSlot *)CALL(mem_new)(count * sizeof(SnowSlot));
    this->dl = (void *)CALL(gfx_newDLContext)(0, (char *)&D_00000000 + 0x634, 0x18, 0, 0, 0, 0);
    ((void (*)(void *))VT_SLOT(this->node.vptr, 11).fn)((char *)this + VT_SLOT(this->node.vptr, 11).delta);
    this->unk84 = this->unk88 = 4;
    this->unk48 = this->unk154[0] = 0.0f;
    this->unk44 = 1.0f;
    this->unk154[1] = 100.0f;
    this->unk154[2] = 184.0f;
    this->unk154[3] = 130.0f;
    this->unk154[5] = snow_ro_20;
    this->unk154[4] = 182.0f;
    this->unk154[6] = snow_ro_24;
    this->unk154[7] = 1.0f;
    this->unk120.z = this->unk120.y = this->unk120.x = 0;
    this->unk12C.z = this->unk12C.y = this->unk12C.x = 0;
}

/* EXACT via an IDO 7.1 NCC (C++) donor: src/snoweffects_uso/snoweffects_uso_ncc.cpp (agent-h run 628),
 * REPLACE_FUNC_BODY-spliced over this placeholder in both build trees. */
void snoweffects_uso_func_000040B4(void *self) {
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00004188);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00004374);

int snoweffects_uso_func_000046BC(char *this) {
    if (*(int *)(this + 0x144) >= 2) {
        if (--*(int *)(this + 0x84) < 0) {
            *(int *)(this + 0x84) = *(int *)(this + 0x88);
            CALL(snoweffects_uso_func_0000535C)(this);
        }
        return 1;
    }
    *(int *)(this + 0x84) = 1;
    return 0;
}

/* on a level change, mark the write slot 2; a change to 0 also advances the ring (0x535C) and
 * marks the next slot 2. No change while set: clear the write slot. */
void snoweffects_uso_func_00004714(SnowRing *r, int on, int prev, int a3) {
    (void)&a3;
    if (on != prev) {
        if (on != 0) {
            r->slots[r->write].state = 2;
        } else {
            r->slots[r->write].state = 2;
            CALL(snoweffects_uso_func_0000535C)(r);
            r->slots[r->write].state = 2;
        }
    } else if (on) {
        r->slots[r->write].state = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000047D4);

void snoweffects_uso_func_00004E4C(char *a0) {
    *(float *)(a0 + 0x50) = 1.0f;
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00004E5C);

void snoweffects_uso_func_000052CC(int a0) {
    (void)&a0;
}

float snoweffects_uso_func_000052D4(char *this, int a1, Vec3 *out, int a3) {
    SnowHit hit;
    int pad[3];
    Tri3i tmp;
    int pad2[2];
    (void)&this;
    (void)&a3;
    if (XCALL(gl_func_00000000_snow_hit, CollShape_nearestHit)(*(int *)&D_00000000, a1, 2000.0f, &hit)) {
        tmp = hit.pos;
        out->x = *(float *)&tmp.a;
        out->y = *(float *)&tmp.b;
        out->z = *(float *)&tmp.c;
        return hit.dist;
    }
    return 0.0f;
}

void snoweffects_uso_func_0000535C(SnowRing *r) {
    if (++r->write == r->count) {
        r->write = 0;
    }
    if (r->write == r->read) {
        if (++r->read == r->count) {
            r->read = 0;
        }
    }
    if ((r->writeOfs += r->stride) == r->size) {
        r->writeOfs = 0;
    }
    if (r->writeOfs == r->readOfs) {
        if ((r->readOfs += r->stride) == r->size) {
            r->readOfs = 0;
        }
    }
}

int snoweffects_uso_func_000053E8(char *this) {
    int d = *(int *)(this + 0x5C) - *(int *)(this + 0x58);
    if (d < 0) {
        d += *(int *)(this + 0x68);
    }
    d = d / *(int *)(this + 0x6C) + 1;
    return d;
}

void snoweffects_uso_func_00005440(int a0, int a1) {
    (void)&a0;
    (void)&a1;
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000544C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000559C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00005FD4);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00006144);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00006248);

void snoweffects_uso_func_0000634C(char *this) {
    if (*(int *)(this + 0x54) & 1) {
        CALL(snoweffects_uso_func_000064B8)(this);
    }
    if (*(int *)(this + 0x54) & 2) {
        CALL(snoweffects_uso_func_00006B28)(this);
    }
    if (*(int *)(this + 0x54) & 4) {
        CALL(snoweffects_uso_func_0000718C)(this);
    }
}

void snoweffects_uso_func_000063B4(char *this) {
    char *v = *(char **)(this + 0x2C);
    float x, a;
    if (*(s16 *)(v + 0x9A2) != 98) {
        x = *(float *)(v + 0x970);
        a = (x < 0.0f) ? -x : x;
        /* RoData+0x28: the module's own .rodata, spelled off the zero placeholder */
        *(float *)(this + 0x50) = 1.0f - *(float *)((char *)&D_00000000 + 0x28) * a;
    } else {
        *(float *)(this + 0x50) = 1.0f;
    }
}

/* the RoData floats of the 0x6420 defaults (own lui each) */
extern f32 snow_ro_2C, snow_ro_30, snow_ro_34, snow_ro_38, snow_ro_3C, snow_ro_40;
extern f32 snow_ro_44, snow_ro_48, snow_ro_4C;

/* default parameters of the 0x6144 child (flag bit 0 at +0x54), the 0x70C0 sibling */
void snoweffects_uso_func_00006420(SnowEffect *this) {
    this->flags |= 1;
    this->unk174 = snow_ro_2C;
    this->unk178 = snow_ro_30;
    this->unk180 = snow_ro_34;
    this->unk184 = 0.0f;
    this->unk198 = snow_ro_38;
    this->unk194 = snow_ro_3C;
    this->unk190 = snow_ro_40;
    this->unk17C = snow_ro_44;
    this->unk188 = snow_ro_48;
    this->unk18C = snow_ro_4C;
    this->unk19C = 1;
    this->unk1A0 = 0;
    this->unk1A4 = 0;
    this->unk1A8 = 0;
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000064B8);

void snoweffects_uso_func_00006B08(char *a0) {
    *(int *)(a0 + 0x1C8) = 0;
    *(int *)(a0 + 0x54) |= 2;
    *(int *)(a0 + 0x1CC) = 0;
    *(float *)(a0 + 0x1D8) = 0.0f;
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00006B28);

/* the RoData floats of the 0x70C0 defaults (each load takes its own lui: one pin per constant) */
extern f32 snow_ro_80, snow_ro_84, snow_ro_88, snow_ro_8C, snow_ro_90, snow_ro_94;
extern f32 snow_ro_98, snow_ro_9C, snow_ro_A0, snow_ro_A4, snow_ro_A8, snow_ro_AC;

/* default parameters of a new snow effect (called from its create, 0x6248) */
void snoweffects_uso_func_000070C0(SnowEffect *this) {
    this->flags |= 4;
    this->unk1E0 = snow_ro_80;
    this->unk1F0 = 0.0f;
    this->unk1E4 = snow_ro_84;
    this->unk1F8 = snow_ro_88;
    this->unk1DC = snow_ro_8C;
    this->unk1EC = 0.0f;
    this->unk1E8 = snow_ro_90;
    this->unk1FC = 0;
    this->unk204 = 0;
    this->unk20C = snow_ro_94;
    this->unk210 = snow_ro_98;
    this->unk208 = snow_ro_9C;
    this->unk214 = snow_ro_A0;
    this->unk218 = snow_ro_A4;
    this->unk21C = snow_ro_A8;
    this->scale.z = this->scale.y = this->scale.x = 1.0f;
    this->unk230 = 0.0f;
    this->unk220 = snow_ro_AC;
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000718C);

void snoweffects_uso_func_000078E8(char *this, int a1, int a2, int a3, int a4, int a5, float f6, float f7) {
    (void)&a1;
    (void)&a2;
    (void)&a3;
    *(char **)(this + 0xC) = (char *)&D_00000000 + 0x660;
    XCALL(gl_func_00000000_snow_3ee4, snoweffects_uso_func_00003EE4)(this, a3, 2, a4, a5, f6, f7);
    *(float *)(this + 0x15C) = 24.0f;
    *(float *)(this + 0x164) = 140.0f;
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00007960);

void snoweffects_uso_func_00007CCC(char *a0) {
    int tag;
    snoweffects_uso_func_00003D70(&tag);
    snoweffects_uso_func_00003DAC((float *)(a0 + 0x10));
}

void snoweffects_uso_func_00007CFC(char *a0) {
    int tag;
    snoweffects_uso_func_00003D70(&tag);
    snoweffects_uso_func_00003DE8((Quad4 *)(a0 + 0x10));
}

void snoweffects_uso_func_00007D2C(char *a0) {
    int tag;
    snoweffects_uso_func_00003D70(&tag);
    snoweffects_uso_func_00003D70((int *)(a0 + 0x10));
}

void snoweffects_uso_func_00007D5C(char *a0) {
    int tag;
    snoweffects_uso_func_00003D70(&tag);
    snoweffects_uso_func_00003E40((Vec3 *)(a0 + 0x10));
}

void *snoweffects_uso_func_00007D8C(int *arg0) {
    int *p2;
    int *head;
    int *p1;
    p1 = (int*)CALL(mem_new)(0x40);
    if (p1 != 0) {
        CALL(ColourItem_ctor)(p1);
        NODE(p1)->vptr = (char *)&D_00000000;
        ((SnowColourItem *)p1)->unk3C = 0;
    }
    p2 = p1;
    p1 = arg0;
    head = (int*)p1[0x40 / 4];
    if ((int*)p1[0x40 / 4] != 0) {
        CALL(list_append)((char*)p2 + 0x10, head);
        if ((int)NODE(head)->parent != 0) {
            NODE(head)->unk04 = 1;
        }
        NODE(head)->parent = (Node *)p2;
    }
    return p2;
}

void snoweffects_uso_func_00007E20(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void snoweffects_uso_func_00007E5C(float *dst) {
    float buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void snoweffects_uso_func_00007E98(Quad4 *dst) {
    Quad4 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 16);
    *dst = buf;
}

void snoweffects_uso_func_00007EF0(Vec3 *dst) {
    int pad_top[1];
    Tri3i raw;
    int pad_mid[2];
    Tri3i tmp;
    int pad_bot[2];
    CALL(fs_gdlReadFile)(&D_00000000, &raw, 12);
    tmp = raw;
    dst->x = *(float*)&tmp.a;
    dst->y = *(float*)&tmp.b;
    dst->z = *(float*)&tmp.c;
}

/* emitter ctor tail, the 0x78E8 twin: name = Data+0x810, 0x3EE4 init with stride 4, then 0x9010(this, 97) */
void snoweffects_uso_func_00007F60(char *this, int a1, int a2, int a3, int a4, int a5, float f6, float f7) {
    (void)&a1;
    (void)&a2;
    (void)&a3;
    *(char **)(this + 0xC) = (char *)&D_00000000 + 0x810;
    XCALL(gl_func_00000000_snow_3ee4, snoweffects_uso_func_00003EE4)(this, a3, 4, a4, a5, f6, f7);
    CALL(snoweffects_uso_func_00009010)(this, 97);
    *(float *)(this + 0x194) = 0.0f;
    *(float *)(this + 0x198) = 100.0f;
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00007FE0);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00008C60);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00008D70);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009010);

void snoweffects_uso_func_000090F0(int a0, int a1) {
    (void)&a0;
    (void)&a1;
}

void snoweffects_uso_func_000090FC(char *a0) {
    int tag;
    snoweffects_uso_func_00007E20(&tag);
    snoweffects_uso_func_00007E5C((float *)(a0 + 0x10));
}

void snoweffects_uso_func_0000912C(char *a0) {
    int tag;
    snoweffects_uso_func_00007E20(&tag);
    snoweffects_uso_func_00007E98((Quad4 *)(a0 + 0x10));
}

void snoweffects_uso_func_0000915C(char *a0) {
    int tag;
    snoweffects_uso_func_00007E20(&tag);
    snoweffects_uso_func_00007E20((int *)(a0 + 0x10));
}

void snoweffects_uso_func_0000918C(char *a0) {
    int tag;
    snoweffects_uso_func_00007E20(&tag);
    snoweffects_uso_func_00007EF0((Vec3 *)(a0 + 0x10));
}

void *snoweffects_uso_func_000091BC(int *arg0) {
    int *p2;
    int *head;
    int *p1;
    p1 = (int*)CALL(mem_new)(0x40);
    if (p1 != 0) {
        CALL(ColourItem_ctor)(p1);
        NODE(p1)->vptr = (char *)&D_00000000;
        ((SnowColourItem *)p1)->unk3C = 0;
    }
    p2 = p1;
    p1 = arg0;
    head = (int*)p1[0x40 / 4];
    if ((int*)p1[0x40 / 4] != 0) {
        CALL(list_append)((char*)p2 + 0x10, head);
        if ((int)NODE(head)->parent != 0) {
            NODE(head)->unk04 = 1;
        }
        NODE(head)->parent = (Node *)p2;
    }
    return p2;
}

void snoweffects_uso_func_00009250(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void snoweffects_uso_func_0000928C(float *dst) {
    float buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void snoweffects_uso_func_000092C8(Quad4 *dst) {
    Quad4 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 16);
    *dst = buf;
}

void snoweffects_uso_func_00009320(void) {
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009328);

void snoweffects_uso_func_00009704(char *a0) {
    CALL(gfx_setBlendColour)(&D_00000000, a0 + 0x288);
    CALL(SNode_display)(a0);
}

void snoweffects_uso_func_0000973C(char *a0) {
    CALL(Parms_load_00035C6C)(a0 + 0x240);
    CALL(SNode_loadData)(a0);
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009768);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009884);

void snoweffects_uso_func_00009BE8(char *this, char *src) {
    SnowRGBA *cols = *(SnowRGBA **)(src + 0x50);
    int i;
    for (i = 0; i < *(int *)(this + 0x22C); i++) {
        int idx = ((s16 *)(this + 0xAC))[i];
        ((SnowRGBA *)(this + 0x12C))[i].r = cols[idx].r;
        ((SnowRGBA *)(this + 0x12C))[i].g = cols[idx].g;
        ((SnowRGBA *)(this + 0x12C))[i].b = cols[idx].b;
    }
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009C48);

#ifdef NON_MATCHING
/* item-type create: 0x9328 ctor(0), +0x70 = 1.0f, attach the type's +0x40 node.
 * Logic exact; residual = item / head colouring swap (target item v1, head a2). */
void *snoweffects_uso_func_00009E10(char *type) {
    int *item;
    int *head;
    item = (int *)gl_func_00000000(0) /* 0x9328 ctor */;
    *(float *)((char *)item + 0x70) = 1.0f;
    head = *(int **)(type + 0x40);
    if (*(int **)(type + 0x40) != 0) {
        gl_func_00000000( /* list_append */ (char *)item + 0x10, head);
        if ((int)NODE(head)->parent != 0) {
            NODE(head)->unk04 = 1;
        }
        NODE(head)->parent = (Node *)item;
    }
    return item;
}
#else
INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009E10);
#endif

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009E88);

/* EXACT via an IDO 7.1 NCC (C++) donor: src/snoweffects_uso/snoweffects_uso_ncc_9F44.cpp
 * (agent-h run 621), REPLACE_FUNC_BODY-spliced over this placeholder in both build trees. */
void snoweffects_uso_func_00009F44(void *self) {
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009FE8);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000A10C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000A760);

void snoweffects_uso_func_0000A870(char *a0) {
    int tag;
    snoweffects_uso_func_00009250(&tag);
    snoweffects_uso_func_0000928C((float *)(a0 + 0x10));
}

void snoweffects_uso_func_0000A8A0(char *a0) {
    int tag;
    snoweffects_uso_func_00009250(&tag);
    snoweffects_uso_func_000092C8((Quad4 *)(a0 + 0x10));
}

void snoweffects_uso_func_0000A8D0(char *a0) {
    int tag;
    snoweffects_uso_func_00009250(&tag);
    snoweffects_uso_func_00009250((int *)(a0 + 0x10));
}

