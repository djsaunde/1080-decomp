#include "common.h"
#include "uso_call.h"
#include "gdl/node.h"
#include "gdl/item.h"
#include "gdl/gfxlib.h"
#include "gdl/window.h"
#include "gdl/polyshape.h"

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

typedef struct SnowRGBA {
    u8 r, g, b, a;
} SnowRGBA;

/* one 0x58-byte snow particle record (SnowPool items) */
typedef struct SnowParticle {
    /* 0x00 */ Vec3 pos;
    /* 0x0C */ Vec3 vel;
    /* 0x18 */ Vec3 accel;           /* snowfall sway, added to vel (SnowFall_moveFlakes) */
    /* 0x24 */ f32 gravity;          /* added to vel.y per update */
    /* 0x28 */ f32 minVelY;          /* vel.y clamp (SnowFall_moveFlakes) */
    /* 0x2C */ f32 angle;            /* sys_randf() * 360 at snowfall spawn; no reader */
    /* 0x30 */ f32 life;             /* <= 0 kills (SnowPool_update); oldest = least life */
    /* 0x34 */ f32 decay;            /* life lost per update */
    /* 0x38 */ f32 size;             /* sprite width (SnowPool_display) */
    /* 0x3C */ f32 aspect;           /* sprite height / width (SnowPool_display) */
    /* 0x40 */ f32 sizeVel;          /* size change per update once life < fadeAt */
    /* 0x44 */ f32 fadeAt;
    /* 0x48 */ SnowRGBA col;
    /* 0x4C */ s32 unk4C;            /* only copied (SnowPool_remove) */
    /* 0x50 */ f32 alpha;            /* 0..255 primitive alpha; < 0 kills */
    /* 0x54 */ f32 alphaVel;         /* alpha lost per update */
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

/* the object a trail follows (SnowRing +0x2C, the donors' `cam`): only what the plain-C code reads */
typedef struct SnowOwner {
    /* 0x000 */ char unk000[0x970];
    /* 0x970 */ f32 lean;            /* signed sideways speed (BoardTrail_calcWidth) */
    /* 0x974 */ char unk974[0x9A2 - 0x974];
    /* 0x9A2 */ s16 state;           /* 97 / 98 select trail modes */
} SnowOwner;

/* the board trail (donor class SnowEffect, name "board trail"; a SnowRing subclass) whose
 * BoardTrail_createPowder / _createSpec make the two particle children at +0x98 / +0xA0 (only the
 * fields its plain-C methods touch) */
typedef struct SnowEffect {
    /* 0x000 */ char unk000[0x2C];
    /* 0x02C */ SnowOwner *cam;
    /* 0x030 */ char unk030[0x50 - 0x30];
    /* 0x050 */ f32 width;           /* end-point spread scale (virtual slot 13) */
    /* 0x054 */ s32 flags;           /* bit 0 / 1 / 2: spawn effects A / B / C */
    /* 0x058 */ char unk058[0x174 - 0x58];
    /* effect A (flag bit 0): powder puffs into the +0x98 PowderParticles pool (BoardTrail_spawnA; defaults
     * BoardTrail_enableA). Names from what spawnA does with each word. */
    /* 0x174 */ f32 powderGravity;   /* particle gravity (x 1.5 in a burst) */
    /* 0x178 */ f32 powderRiseY;     /* added to the spawn point's y */
    /* 0x17C */ f32 powderSizeVel;   /* particle sizeVel, times the |speed| term */
    /* 0x180 */ f32 powderDirScale;  /* trail-direction velocity, times speed */
    /* 0x184 */ f32 powderSpeedBias; /* (bias + |speed|) multiplies the owner-velocity term */
    /* 0x188 */ f32 powderAlphaVel;  /* particle alphaVel */
    /* 0x18C */ f32 powderDecay;     /* particle decay */
    /* 0x190 */ f32 powderMinSpeed;  /* no spawn below this |speed| */
    /* 0x194 */ f32 powderBurstVelScale; /* owner vel (+0x318) scale during a burst */
    /* 0x198 */ f32 powderVelScale;  /* owner vel (+0x318) scale otherwise */
    /* 0x19C */ s32 powderPeriod;    /* frames between spawns (timer reload) */
    /* 0x1A0 */ s32 powderTimer;
    /* 0x1A4 */ s32 unk1A4;          /* set to 5 on owner state 98; >= 5 re-arms the burst */
    /* 0x1A8 */ s32 powderBurst;     /* burst frames left (4 on landing, 2 re-armed) */
    /* 0x1AC */ s32 powderCount;     /* particles per spawn (spawnA sets 1) */
    /* effect B (flag bit 1): landing spray into the +0x9C NormalParticles pool (BoardTrail_spawnB); spawnB
     * rewrites +0x1B0..+0x1C4 every frame from one of two RoData sets picked by the game flags. */
    /* 0x1B0 */ f32 normSizeVel;     /* particle sizeVel */
    /* 0x1B4 */ f32 normGravity;     /* particle gravity */
    /* 0x1B8 */ f32 normUpScale;     /* owner up axis (+0x3B0) velocity scale (26 / 41) */
    /* 0x1BC */ f32 normDecay;       /* particle decay */
    /* 0x1C0 */ f32 normAlphaVel;    /* particle alphaVel */
    /* 0x1C4 */ f32 normVelScale;    /* owner vel (+0x318) scale */
    /* 0x1C8 */ s32 unk1C8;          /* frames with owner speed > 20, SnowRing +0x80 set, hits != 1; >= 7 at state 97 bursts */
    /* 0x1CC */ s32 normBurst;       /* burst frames left (3; spawns while 1..2) */
    /* 0x1D0 */ s32 normLastState;   /* owner state, copied while SnowRing +0x80 is clear */
    /* 0x1D4 */ char unk1D4[0x1D8 - 0x1D4];
    /* 0x1D8 */ f32 unk1D8;          /* owner +0xA20 last frame (0 after > 0 triggers a burst) */
    /* effect C (flag bit 2): side spray into the +0xA0 SpecParticles pool (BoardTrail_spawnC; defaults
     * BoardTrail_enableC). */
    /* 0x1DC */ f32 specSizeVel;     /* particle sizeVel */
    /* 0x1E0 */ f32 specGravity;     /* particle gravity */
    /* 0x1E4 */ f32 unk1E4;          /* enableC default; spawnC does not read it */
    /* 0x1E8 */ f32 specDecay;       /* particle decay */
    /* 0x1EC */ f32 specAlphaVel;    /* particle alphaVel */
    /* 0x1F0 */ f32 unk1F0;          /* enableC clears it; spawnC does not read it */
    /* 0x1F4 */ f32 unk1F4;
    /* 0x1F8 */ f32 specSize;        /* particle size */
    /* 0x1FC */ s32 specTimer;
    /* 0x200 */ s32 unk200;
    /* 0x204 */ s32 specPeriod;      /* frames between spawns (timer reload) */
    /* 0x208 */ f32 specVelScale;    /* owner vel (+0x318) scale */
    /* 0x20C */ f32 specUpScale;     /* owner up axis (+0x3B0) scale, times the side strength */
    /* 0x210 */ f32 specDirScale;    /* trail direction (+0x114) scale, times the side strength */
    /* 0x214 */ f32 specMinLean;     /* side strength 0 below this |lean| (+0x4C) */
    /* 0x218 */ f32 specMaxLean;     /* side strength 1 above it */
    /* 0x21C */ f32 specRampBase;    /* side strength at specMinLean (ramps to 1) */
    /* 0x220 */ f32 specTintMix;     /* lerp from the owner colour (+0xA30) to specTint */
    /* 0x224 */ Vec3 specTint;       /* r, g, b in 0..1 (enableC: white) */
    /* 0x230 */ f32 unk230;          /* enableC clears it; spawnC does not read it */
} SnowEffect;

/* the 0x50-byte link node (ctor 0x9E88, update 0x9F44): couples a source object to an SNode-subclass
 * target it creates (0x9328) */
typedef struct SnowLink {
    /* 0x00 */ Node node;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ void *src;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ Node *tgt;
    /* 0x3C */ char unk3C[0x50 - 0x3C];
} SnowLink;

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
    /* 0x2C */ struct SnowOwner *cam;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ char unk3C[8];
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 width;           /* end-point spread scale (virtual slot 13, SnowRing_calcWidth) */
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
    /* 0x84 */ s32 emitTimer;       /* frames to the next ring advance (SnowRing_emitTick) */
    /* 0x88 */ s32 emitPeriod;
    /* 0x8C */ void *vtx;           /* size Vtx (16-byte aligned) */
    /* 0x90 */ SnowPoint *points;   /* size points */
    /* 0x94 */ SnowSlot *slots;     /* count slots */
    /* 0x98 */ char unk98[0x18];
    /* 0xB0 */ void *dl;            /* gfx_newDLContext(0, .., 0x18, ..) */
    /* 0xB4 */ char unkB4[0x120 - 0xB4];
    /* 0x120 */ Vec3 unk120;
    /* 0x12C */ Vec3 unk12C;
    /* 0x138 */ char unk138[0x144 - 0x138];
    /* 0x144 */ s32 hits;           /* end points pushed back by the probe (placeEnds); >= 2 emits */
    /* 0x148 */ char unk148[0x154 - 0x148];
    /* 0x154 */ f32 unk154[8];
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
    /* 0x19C */ f32 unk19C;
    /* 0x1A0 */ f32 unk1A0;
    /* 0x1A4 */ f32 unk1A4;
    /* 0x1A8 */ f32 unk1A8;
    /* 0x1AC */ f32 unk1AC;
    /* 0x1B0 */ char unk1B0[0x1EC - 0x1B0];
    /* 0x1EC */ s32 unk1EC;
    /* 0x1F0 */ f32 unk1F0;
} SnowRing;

/* typed blank (0) alias for the reloc'd call to the global 0x3EE4 (floats on the stack) */
extern void gl_func_00000000_snow_3ee4(void *self, int a1, int a2, int a3, int a4, float f5, float f6);


/* CollShape_nearestHit's result record (only pos / dist are read here) */
typedef struct SnowHit {
    /* 0x00 */ Tri3i pos;
    /* 0x0C */ f32 dist;
    /* 0x10 */ s32 unk10[7];
} SnowHit;

/* the snowfall effect (0x27A8 / 0x3620 / 0x374C): particles at +0x30 (count +0xB8), the camera's
 * view rotation copied in both orientations, and six view-frustum planes built from it */
typedef struct SnowFall {
    /* 0x000 */ Node node;
    /* 0x02C */ s32 unk2C;
    /* 0x030 */ SnowParticle *items;
    /* 0x034 */ char unk34[0x50 - 0x34];
    /* 0x050 */ f32 camRotT[3][3];  /* camera +0xB4 matrix, transposed */
    /* 0x074 */ f32 camRot[3][3];   /* camera +0xB4 matrix rows (0x3544 rotates by these) */
    /* 0x098 */ char unk98[0xAC - 0x98];
    /* 0x0AC */ f32 nearDist;
    /* 0x0B0 */ f32 farDist;
    /* 0x0B4 */ s32 unkB4;
    /* 0x0B8 */ s32 count;
    /* 0x0BC */ char unkBC[0xD0 - 0xBC];
    /* 0x0D0 */ f32 planes[6][4];    /* normal + distance (0x3544 / 0x35B0) */
    /* 0x130 */ s32 nPlanes;
} SnowFall;

/* typed blank (0) alias for the sys_randf import (float return) */
extern float gl_func_00000000_snow_randf(void);

/* typed blank (0) alias for the CollShape_nearestHit import: the float radius goes in a2 */
extern int gl_func_00000000_snow_hit(int shape, int ray, float radius, SnowHit *hit);

/* UNNAMED: sentinel the Vec3 dot product at Text+0: a *_00000000 name is never renamed (rename-symbol.py refuses it) */
float snoweffects_uso_func_00000000(Vec3 *a, Vec3 *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

float SnowPlane_solveX_00000030(float *p, float *pl) {
    return -(p[1] * pl[1] + p[2] * pl[2] - pl[3]) / pl[0];
}

float SnowPlane_solveY_00000064(float *p, float *pl) {
    return -(p[0] * pl[0] + p[2] * pl[2] - pl[3]) / pl[1];
}

float SnowPlane_solveZ_00000098(float *p, float *pl) {
    return -(p[0] * pl[0] + p[1] * pl[1] - pl[3]) / pl[2];
}

void fs_gdlReadInt_snow_000000CC(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void fs_gdlReadFloat_snow_00000108(float *dst) {
    float buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void fs_gdlReadQuad_snow_00000144(Quad4 *dst) {
    Quad4 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 16);
    *dst = buf;
}

void fs_gdlReadVec3_snow_0000019C(Vec3 *dst) {
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

void SnowPool_init_0000020C(SnowPool *this, int n) {
    this->items = (SnowParticle *)CALL(mem_new)(n * sizeof(SnowParticle));
    this->count = 0;
    this->max = n;
    VT_SLOT(this->node.vptr, 11).fn((char *)this + VT_SLOT(this->node.vptr, 11).delta);
}

/* texture-sprite init (the 0x15D0 family, render state first): EXACT via the IDO 7.1 NCC donor
 * src/snoweffects_uso/snoweffects_uso_ncc_c.cpp (agent-c runs 655 / 683), REPLACE_FUNC_BODY-spliced over this
 * placeholder in both build trees. */
void SnowPool_buildTexDL_00000270(void) {
}

SnowParticle *SnowPool_oldest_00000680(SnowPool *this) {
    float best = 65536.0f;
    SnowParticle *p = this->items;
    SnowParticle *found = 0;
    int i;
    for (i = 0; i < this->count; i++, p++) {
        if (p->life < best) {
            best = p->life;
            found = p;
        }
    }
    return found;
}

SnowParticle *SnowPool_alloc_000006D0(SnowPool *this) {
    if (this->count < this->max) {
        this->count++;
        return &this->items[this->count - 1];
    }
    return 0;
}

/* SnowPool remove: move the last live particle into p's slot (field by field, the Vec3s through one temp) */
void SnowPool_remove_00000718(SnowPool *this, SnowParticle *p) {
    int pad[2];
    Vec3 t;
    int pad2[2];
    SnowParticle *last;
    this->count--;
    last = &this->items[this->count];
    if (p != last) {
        t = last->pos;
        p->pos.x = t.x;
        p->pos.y = t.y;
        p->pos.z = t.z;
        t = this->items[this->count].vel;
        p->vel.x = t.x;
        p->vel.y = t.y;
        p->vel.z = t.z;
        t = this->items[this->count].accel;
        p->accel.x = t.x;
        p->accel.y = t.y;
        p->accel.z = t.z;
        p->gravity = this->items[this->count].gravity;
        p->minVelY = this->items[this->count].minVelY;
        p->angle = this->items[this->count].angle;
        p->life = this->items[this->count].life;
        p->decay = this->items[this->count].decay;
        p->size = this->items[this->count].size;
        p->aspect = this->items[this->count].aspect;
        p->sizeVel = this->items[this->count].sizeVel;
        p->fadeAt = this->items[this->count].fadeAt;
        p->col = this->items[this->count].col;
        p->unk4C = this->items[this->count].unk4C;
        p->alpha = this->items[this->count].alpha;
        p->alphaVel = this->items[this->count].alphaVel;
    }
}

/* SnowPool spawn with a random size / fade (sys_randf). The first sys_randf, the size store and the
 * aspect store share one source line: as1 schedules by source line, and only with these three on one line
 * does the p reload land after the r * 2 mul.s, as in the target. */
SnowParticle *SnowPool_spawnRandom_0000097C(SnowPool *this, Vec3 *pos, Vec3 *vel) {
    SnowParticle *p = (SnowParticle *)CALL(SnowPool_alloc_000006D0)(this);
    if (p != 0) {
        float x, y, z, r;
        x = pos->x; y = pos->y; z = pos->z;
        p->pos.x = x;
        p->pos.y = y;
        p->pos.z = z;
        x = vel->x; y = vel->y; z = vel->z;
        p->vel.z = z;
        p->vel.y = y;
        p->vel.x = x;
        p->gravity = *(f32 *)&D_00000000;
        r = XCALL(gl_func_00000000_snow_randf, sys_randf)() /* sys_randf */; p->size = (0, 10.0f) + r * 2; p->aspect = 1.0f;
        p->fadeAt = 0.0f;
        p->life = 256.0f;
        p->decay = 16.0f;
        r = XCALL(gl_func_00000000_snow_randf, sys_randf)() /* sys_randf */;
        p->col.r = 251;
        p->col.g = 251;
        p->col.b = 251;
        p->col.a = 0;
        p->alpha = (0, 64.0f) + r * 64.0f;
        p->alphaVel = 0.0f;
    }
    return p;
}

SnowParticle *SnowPool_spawn_00000A7C(SnowPool *this, Vec3 *pos) {
    SnowParticle *p = (SnowParticle *)CALL(SnowPool_alloc_000006D0)(this);
    if (p == 0) {
        p = (SnowParticle *)CALL(SnowPool_oldest_00000680)(this);
    }
    if (p != 0) {
        float x = pos->x, y = pos->y, z = pos->z;
        p->col.a = 0;
        p->col.b = 0xFB;
        p->col.g = 0xFB;
        p->col.r = 0xFB;
        p->pos.x = x;
        p->pos.y = y;
        p->pos.z = z;
        p->decay = 255.0f;
        p->alpha = 255.0f;
        p->vel.x = 0.0f;
        p->vel.y = 0.0f;
        p->vel.z = 0.0f;
        p->gravity = 0.0f;
        p->sizeVel = 0.0f;
        p->fadeAt = 0.0f;
        p->alphaVel = 0.0f;
        p->size = 2.0f;
        p->aspect = 1.0f;
        p->life = 256.0f;
    }
    return p;
}

/* SnowPool update: EXACT via the IDO 7.1 NCC donor src/snoweffects_uso/snoweffects_uso_ncc.cpp (user
 * Vec3 operator+= schedule), REPLACE_FUNC_BODY-spliced over this placeholder. */
void SnowPool_update_00000B44(SnowPool *this) {
}

/* particle sprite draw (projected texture rectangles): EXACT via the IDO 7.1 NCC donor
 * src/snoweffects_uso/snoweffects_uso_ncc_nm_h.cpp (agent-c run 655, agent-h runs 687-783, agent-g run 785),
 * REPLACE_FUNC_BODY-spliced over this placeholder in both build trees. */
void SnowPool_display_00000C90(void) {
}

/* texture-sprite init: EXACT via the IDO 7.1 NCC donor src/snoweffects_uso/snoweffects_uso_ncc_nm.cpp
 * (agent-h run 649, agent-c run 691), REPLACE_FUNC_BODY-spliced over this placeholder in both build trees. */
void PowderParticles_buildTexDL_000015D0(void) {
}

/* texture-sprite init: EXACT via the IDO 7.1 NCC donor src/snoweffects_uso/snoweffects_uso_ncc_nm.cpp
 * (agent-h run 649, agent-c run 691), REPLACE_FUNC_BODY-spliced over this placeholder in both build trees. */
void NormalParticles_buildTexDL_00001A18(void) {
}

/* texture-sprite init (64 x 64 IA8, Data+0x5AC0): EXACT via the IDO 7.1 NCC donor
 * src/snoweffects_uso/snoweffects_uso_ncc_nm.cpp (agent-c run 704), REPLACE_FUNC_BODY-spliced over this placeholder
 * in both build trees. */
void SpecParticles_buildTexDL_00001E60(void) {
}

/* texture-sprite init: EXACT via the IDO 7.1 NCC donor src/snoweffects_uso/snoweffects_uso_ncc_nm.cpp
 * (agent-h run 649, agent-c run 691), REPLACE_FUNC_BODY-spliced over this placeholder in both build trees. */
void SnowFall_buildTexDL_00002310(void) {
}

/* SnowFall flake motion: EXACT via the IDO 7.1 NCC donor src/snoweffects_uso/snoweffects_uso_ncc.cpp
 * (user Vec3 operator+=, RoData+0xC literal pin), REPLACE_FUNC_BODY-spliced over this placeholder. */
void SnowFall_moveFlakes_000027A8(SnowFall *this) {
}

/* snowfall draw (camera-motion integration, camera-space box wrap, projected texture rectangles): EXACT via
 * the IDO 7.1 NCC donor src/snoweffects_uso/snoweffects_uso_ncc_c.cpp (agent-c runs 667-794, agent-g run 793),
 * REPLACE_FUNC_BODY-spliced over this placeholder in both build trees. */
void SnowFall_display_000028E0(void) {
}

/* out = camRot * v (one dot per camera axis row) */
void SnowFall_rotateNormal_00003544(SnowFall *this, float *out, Vec3 *v) {
    out[0] = snoweffects_uso_func_00000000(v, (Vec3 *)this->camRot[0]); /* UNNAMED: sentinel Text+0 Vec3 dot */
    out[1] = snoweffects_uso_func_00000000(v, (Vec3 *)this->camRot[1]); /* UNNAMED: sentinel Text+0 Vec3 dot */
    out[2] = snoweffects_uso_func_00000000(v, (Vec3 *)this->camRot[2]); /* UNNAMED: sentinel Text+0 Vec3 dot */
}

void SnowFall_setPlaneDist_000035B0(char *this, float *n, float *p, float s) {
    Vec3 t;
    (void)&this;
    t.x = p[0] - n[0] * s;
    t.y = p[1] - n[1] * s;
    t.z = p[2] - n[2] * s;
    n[3] = snoweffects_uso_func_00000000(&t, (Vec3 *)n); /* UNNAMED: sentinel Text+0 Vec3 dot */
}

/* the current window's camera view matrix (camera +0xB4, rows of four) */
#define SNOW_CAM_ROT(r, c) \
    (((f32 (*)[4])(((GfxLib *)&D_00000000)->curWindow->win.camera + 0xB4))[r][c])

/* typed blank (0) alias for the call to the global 0x35B0 (float in a3) */
extern void gl_func_00000000_snow_35b0(void *self, f32 (*pl)[4], Vec3 *p, float s);

/* the current window's camera position (camera +0xA0) */
#define SNOW_CAM_POS(i) (((f32 *)(((GfxLib *)&D_00000000)->curWindow->win.camera + 0xA0))[i])

/* SnowFall: copy the camera rotation, transposed into camRotT and as-is into camRot */
void SnowFall_copyCamRot_00003620(SnowFall *this) {
    this->camRotT[0][0] = SNOW_CAM_ROT(0, 0);
    this->camRotT[0][1] = SNOW_CAM_ROT(1, 0);
    this->camRotT[0][2] = SNOW_CAM_ROT(2, 0);
    this->camRotT[1][0] = SNOW_CAM_ROT(0, 1);
    this->camRotT[1][1] = SNOW_CAM_ROT(1, 1);
    this->camRotT[1][2] = SNOW_CAM_ROT(2, 1);
    this->camRotT[2][0] = SNOW_CAM_ROT(0, 2);
    this->camRotT[2][1] = SNOW_CAM_ROT(1, 2);
    this->camRotT[2][2] = SNOW_CAM_ROT(2, 2);
    this->camRot[0][0] = SNOW_CAM_ROT(0, 0);
    this->camRot[0][1] = SNOW_CAM_ROT(0, 1);
    this->camRot[0][2] = SNOW_CAM_ROT(0, 2);
    this->camRot[1][0] = SNOW_CAM_ROT(1, 0);
    this->camRot[1][1] = SNOW_CAM_ROT(1, 1);
    this->camRot[1][2] = SNOW_CAM_ROT(1, 2);
    this->camRot[2][0] = SNOW_CAM_ROT(2, 0);
    this->camRot[2][1] = SNOW_CAM_ROT(2, 1);
    this->camRot[2][2] = SNOW_CAM_ROT(2, 2);
}

/* SnowFall: build the six view-frustum planes (near / far / left / right / bottom / top as camera-space
 * normals rotated by 0x3544, placed through the camera position by 0x35B0) */
void SnowFall_buildFrustum_0000374C(SnowFall *this) {
    Vec3 eye;
    Vec3 n;
    int pad; /* frame slot: the planes spill lands at sp+0x34 */
    f32 (*pl)[4];
    this->nearDist = 10.0f;
    this->farDist = 100.0f;
    CALL(SnowFall_copyCamRot_00003620)(this);
    eye.x = ((GfxLib *)&D_00000000)->worldScale[0] * SNOW_CAM_POS(0);
    eye.y = ((GfxLib *)&D_00000000)->worldScale[1] * SNOW_CAM_POS(1);
    eye.z = ((GfxLib *)&D_00000000)->worldScale[2] * SNOW_CAM_POS(2);
    n.x = 0.0f;
    n.y = 0.0f;
    n.z = 1.0f;
    pl = this->planes;
    CALL(SnowFall_rotateNormal_00003544)(this, pl, &n);
    XCALL(gl_func_00000000_snow_35b0, SnowFall_setPlaneDist_000035B0)(this, pl++, &eye, this->farDist);
    n.z = -1.0f;
    CALL(SnowFall_rotateNormal_00003544)(this, pl, &n);
    XCALL(gl_func_00000000_snow_35b0, SnowFall_setPlaneDist_000035B0)(this, pl++, &eye, -this->nearDist);
    n.y = 0.0f;
    n.z = 0.0f;
    n.x = 1.0f;
    CALL(SnowFall_rotateNormal_00003544)(this, pl, &n);
    XCALL(gl_func_00000000_snow_35b0, SnowFall_setPlaneDist_000035B0)(this, pl++, &eye, this->farDist / 2);
    n.x = -1.0f;
    CALL(SnowFall_rotateNormal_00003544)(this, pl, &n);
    XCALL(gl_func_00000000_snow_35b0, SnowFall_setPlaneDist_000035B0)(this, pl++, &eye, this->farDist / 2);
    n.x = 0.0f;
    n.z = 0.0f;
    n.y = 1.0f;
    CALL(SnowFall_rotateNormal_00003544)(this, pl, &n);
    XCALL(gl_func_00000000_snow_35b0, SnowFall_setPlaneDist_000035B0)(this, pl++, &eye, this->farDist / 2);
    n.y = -1.0f;
    CALL(SnowFall_rotateNormal_00003544)(this, pl, &n);
    XCALL(gl_func_00000000_snow_35b0, SnowFall_setPlaneDist_000035B0)(this, pl++, &eye, this->farDist / 2);
    this->nPlanes = ((char *)pl - (char *)this->planes) / 16;
}

/* SnowFall fill: EXACT via the IDO 7.1 NCC donor src/snoweffects_uso/snoweffects_uso_ncc.cpp
 * (RoData+0x14..+0x1C literal pin), REPLACE_FUNC_BODY-spliced over this placeholder. */
void SnowFall_fill_000039C4(SnowFall *this) {
}

void ParmFloat_read_snow_00003CA4(char *a0) {
    int tag;
    fs_gdlReadInt_snow_000000CC(&tag);
    fs_gdlReadFloat_snow_00000108((float *)(a0 + 0x10));
}

void ParmQuad_read_snow_00003CD4(char *a0) {
    int tag;
    fs_gdlReadInt_snow_000000CC(&tag);
    fs_gdlReadQuad_snow_00000144((Quad4 *)(a0 + 0x10));
}

void ParmInt_read_snow_00003D04(char *a0) {
    int tag;
    fs_gdlReadInt_snow_000000CC(&tag);
    fs_gdlReadInt_snow_000000CC((int *)(a0 + 0x10));
}

void ParmVec3_read_snow_00003D34(char *a0) {
    int tag;
    fs_gdlReadInt_snow_000000CC(&tag);
    fs_gdlReadVec3_snow_0000019C((Vec3 *)(a0 + 0x10));
}

void fs_gdlReadInt_snow_00003D70(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void fs_gdlReadFloat_snow_00003DAC(float *dst) {
    float buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void fs_gdlReadQuad_snow_00003DE8(Quad4 *dst) {
    Quad4 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 16);
    *dst = buf;
}

void fs_gdlReadVec3_snow_00003E40(Vec3 *dst) {
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
void SnowPoint_clear_00003EB0(SnowPoint *p) {
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
void SnowRing_init_00003EE4(SnowRing *this, int a1, int stride, int count, int a4, float f5, float f6) {
    int i;
    int n;
    this->cam = (SnowOwner *)a1;
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
        CALL(SnowPoint_clear_00003EB0)(&this->points[i]);
    }
    this->slots = (SnowSlot *)CALL(mem_new)(count * sizeof(SnowSlot));
    this->dl = (void *)CALL(gfx_newDLContext)(0, (char *)&D_00000000 + 0x634, 0x18, 0, 0, 0, 0);
    ((void (*)(void *))VT_SLOT(this->node.vptr, 11).fn)((char *)this + VT_SLOT(this->node.vptr, 11).delta);
    this->emitTimer = this->emitPeriod = 4;
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
void SnowRing_draw_000040B4(void *self) {
}

/* display-list setup: EXACT via the IDO 7.1 NCC donor src/snoweffects_uso/snoweffects_uso_ncc.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder. */
void SnowRing_setupDL_00004188(void) {
}

/* emitter basis rebuild: EXACT via the IDO 7.1 NCC donor src/snoweffects_uso/snoweffects_uso_ncc.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder. */
void SnowRing_updateBasis_00004374(void) {
}

/* both end points landed (hits >= 2): count the emit timer down, advancing the ring when it runs out */
int SnowRing_emitTick_000046BC(SnowRing *this) {
    if (this->hits >= 2) {
        if (--this->emitTimer < 0) {
            this->emitTimer = this->emitPeriod;
            CALL(SnowRing_advance_0000535C)(this);
        }
        return 1;
    }
    this->emitTimer = 1;
    return 0;
}

/* on a level change, mark the write slot 2; a change to 0 also advances the ring (0x535C) and
 * marks the next slot 2. No change while set: clear the write slot. */
void SnowRing_setSlotState_00004714(SnowRing *r, int on, int prev, int a3) {
    (void)&a3;
    if (on != prev) {
        if (on != 0) {
            r->slots[r->write].state = 2;
        } else {
            r->slots[r->write].state = 2;
            CALL(SnowRing_advance_0000535C)(r);
            r->slots[r->write].state = 2;
        }
    } else if (on) {
        r->slots[r->write].state = 0;
    }
}

/* trail emitter update: EXACT via the IDO 7.1 NCC donor src/snoweffects_uso/snoweffects_uso_ncc_47d4.cpp
 * (agent-h run 709), REPLACE_FUNC_BODY-spliced over this placeholder in both build trees. */
void SnowRing_emit_000047D4(void) {
}

void SnowRing_calcWidth_00004E4C(SnowRing *this) {
    this->width = 1.0f;
}

/* basis end points + collision push-back: EXACT via the IDO 7.1 NCC donor
 * src/snoweffects_uso/snoweffects_uso_ncc.cpp, REPLACE_FUNC_BODY-spliced over this placeholder. */
void SnowRing_placeEnds_00004E5C(void) {
}

void SnowRing_spawn_000052CC(int a0) {
    (void)&a0;
}

float SnowRing_probe_000052D4(char *this, int a1, Vec3 *out, int a3) {
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

void SnowRing_advance_0000535C(SnowRing *r) {
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

int SnowRing_countSlots_000053E8(SnowRing *this) {
    int d = this->writeOfs - this->readOfs;
    if (d < 0) {
        d += this->size;
    }
    d = d / this->stride + 1;
    return d;
}

/* UNNAMED: unknown-role empty two-argument virtual (trail vtables slot 16); no caller in the module */
void SnowRing_vslot16_00005440(int a0, int a1) {
    (void)&a0;
    (void)&a1;
}

/* SnowRing update: virtual slot 12, then step the live slots' points from the read cursor (drag by
 * +0x44 on the velocity, w[0] accumulates +0x48 and feeds vel.y, w[1] decays by w[2]), then Node_update */
void SnowRing_update_0000544C(SnowRing *this) {
    int idx, n, k, j;
    SnowPoint *pt;
    VT_SLOT(this->node.vptr, 12).fn((char *)this + VT_SLOT(this->node.vptr, 12).delta);
    idx = this->readOfs;
    n = CALL(SnowRing_countSlots_000053E8)(this);
    if (n >= 2) {
        for (k = 0; k < n;) {
            pt = &this->points[idx];
            j = 0;
            k++; /* counted here: the addiu sits between the j = 0 and the index mflo */
            for (; j < this->stride; j++, pt++) {
                f32 drag = this->unk44;
                pt->vel.x *= drag;
                pt->vel.y *= drag;
                pt->vel.z *= drag;
                pt->w[0] += this->unk48;
                pt->vel.y += pt->w[0];
                pt->pos.x += pt->vel.x;
                pt->pos.y += pt->vel.y;
                pt->pos.z += pt->vel.z;
                pt->w[1] -= pt->w[2];
            }
            idx += this->stride;
            if (idx == this->size) {
                idx = 0;
            }
        }
    }
    CALL(Node_update)(this);
}

/* 0x559C: the trail-ring draw (Mat lights, scaled Vtx ring, per-slot texture loads, gSP1Triangle strips).
 * EXACT via the IDO 7.1 NCC donor in snoweffects_uso_ncc_b.cpp (agent-b runs 679 / 686), REPLACE_FUNC_BODY-
 * spliced over this placeholder. */
void SnowRing_display_0000559C(void) {
}

/* BoardTrail_init_00005FD4: SnowEffect::init (name, groups, SnowRing_init, child0 at +0x9C).
 * EXACT via an IDO 7.1 NCC (C++) donor, src/snoweffects_uso/snoweffects_uso_ncc_b.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder. */
void BoardTrail_init_00005FD4(char *this, int a1, int a2, int a3, int a4, int a5, float f6, float f7) {
}

/* BoardTrail_createPowder_00006144: SnowEffect::createA (childA at +0x98, attached to both groups).
 * EXACT via the NCC donor src/snoweffects_uso/snoweffects_uso_ncc_b.cpp (placeholder). */
void BoardTrail_createPowder_00006144(char *this) {
}

/* BoardTrail_createSpec_00006248: SnowEffect::createB (childB at +0xA0, attached to both groups).
 * EXACT via the NCC donor src/snoweffects_uso/snoweffects_uso_ncc_b.cpp (placeholder). */
void BoardTrail_createSpec_00006248(char *this) {
}

void BoardTrail_spawn_0000634C(SnowEffect *this) {
    if (this->flags & 1) {
        CALL(BoardTrail_spawnA_000064B8)(this);
    }
    if (this->flags & 2) {
        CALL(BoardTrail_spawnB_00006B28)(this);
    }
    if (this->flags & 4) {
        CALL(BoardTrail_spawnC_0000718C)(this);
    }
}

/* the trail narrows with the owner's sideways lean (state 98: full width) */
void BoardTrail_calcWidth_000063B4(SnowEffect *this) {
    SnowOwner *v = this->cam;
    float x, a;
    if (v->state != 98) {
        x = v->lean;
        a = (x < 0.0f) ? -x : x;
        /* RAW: data-table RoData+0x28 (the lean factor): the module's own .rodata, spelled off the zero placeholder */
        this->width = 1.0f - *(float *)((char *)&D_00000000 + 0x28) * a;
    } else {
        this->width = 1.0f;
    }
}

/* the RoData floats of the 0x6420 defaults (own lui each) */
extern f32 snow_ro_2C, snow_ro_30, snow_ro_34, snow_ro_38, snow_ro_3C, snow_ro_40;
extern f32 snow_ro_44, snow_ro_48, snow_ro_4C;

/* default parameters of the 0x6144 child (flag bit 0 at +0x54), the 0x70C0 sibling */
void BoardTrail_enableA_00006420(SnowEffect *this) {
    this->flags |= 1;
    this->powderGravity = snow_ro_2C;
    this->powderRiseY = snow_ro_30;
    this->powderDirScale = snow_ro_34;
    this->powderSpeedBias = 0.0f;
    this->powderVelScale = snow_ro_38;
    this->powderBurstVelScale = snow_ro_3C;
    this->powderMinSpeed = snow_ro_40;
    this->powderSizeVel = snow_ro_44;
    this->powderAlphaVel = snow_ro_48;
    this->powderDecay = snow_ro_4C;
    this->powderPeriod = 1;
    this->powderTimer = 0;
    this->unk1A4 = 0;
    this->powderBurst = 0;
}

/* 0x64B8: spawn update of effect A (flag bit 0): burst / timer gating, a direction from the +0x30-matrix
 * point minus +0x138 (normalized, scaled, plus the camera +0x318 lean) and per-particle pool spawns.
 * EXACT via a MIPSpro 7.2.1 (C++) donor: src/snoweffects_uso/snoweffects_uso_mp72_nm_e.cpp (agent-e run 744),
 * REPLACE_FUNC_BODY-spliced over this placeholder in both build trees. */
void BoardTrail_spawnA_000064B8(void) {
}

void BoardTrail_enableB_00006B08(SnowEffect *this) {
    this->unk1C8 = 0;
    this->flags |= 2;
    this->normBurst = 0;
    this->unk1D8 = 0.0f;
}

/* 0x6B28: spawn update of effect B (flag bit 1): game-flag RoData defaults (+0x1B0..+0x1C4), course-state
 * counters (+0x1C8 / +0x1CC / +0x1D8 from camera +0x9A0 / +0x9A2 / +0xA20), then a burst spawn along the
 * camera +0x318 / +0x3B0 axes. EXACT via a MIPSpro 7.2.1 (C++) donor: src/snoweffects_uso/snoweffects_uso_mp72_nm_e.cpp
 * (agent-e run 723), REPLACE_FUNC_BODY-spliced over this placeholder in both build trees. */
void BoardTrail_spawnB_00006B28(void *self) {
}

/* the RoData floats of the 0x70C0 defaults (each load takes its own lui: one pin per constant) */
extern f32 snow_ro_80, snow_ro_84, snow_ro_88, snow_ro_8C, snow_ro_90, snow_ro_94;
extern f32 snow_ro_98, snow_ro_9C, snow_ro_A0, snow_ro_A4, snow_ro_A8, snow_ro_AC;

/* default parameters of a new snow effect (called from its create, 0x6248) */
void BoardTrail_enableC_000070C0(SnowEffect *this) {
    this->flags |= 4;
    this->specGravity = snow_ro_80;
    this->unk1F0 = 0.0f;
    this->unk1E4 = snow_ro_84;
    this->specSize = snow_ro_88;
    this->specSizeVel = snow_ro_8C;
    this->specAlphaVel = 0.0f;
    this->specDecay = snow_ro_90;
    this->specTimer = 0;
    this->specPeriod = 0;
    this->specUpScale = snow_ro_94;
    this->specDirScale = snow_ro_98;
    this->specVelScale = snow_ro_9C;
    this->specMinLean = snow_ro_A0;
    this->specMaxLean = snow_ro_A4;
    this->specRampBase = snow_ro_A8;
    this->specTint.z = this->specTint.y = this->specTint.x = 1.0f;
    this->unk230 = 0.0f;
    this->specTintMix = snow_ro_AC;
}

/* 0x718C: spawn update of effect C (flag bit 2): speed-ramped side push, the +0x1FC timer, one pool spawn
 * with a lerped position, a velocity from the camera +0x318 / +0x3B0 axes and +0x114, and a lerped colour.
 * EXACT via a MIPSpro 7.2.1 (C++) donor: src/snoweffects_uso/snoweffects_uso_mp72_nm_e.cpp (agent-e run 736),
 * REPLACE_FUNC_BODY-spliced over this placeholder in both build trees. */
void BoardTrail_spawnC_0000718C(void) {
}

void HandTrail_init_000078E8(SnowRing *this, int a1, int a2, int a3, int a4, int a5, float f6, float f7) {
    (void)&a1;
    (void)&a2;
    (void)&a3;
    this->node.name = (char *)&D_00000000 + 0x660; /* "hand trail" */
    XCALL(gl_func_00000000_snow_3ee4, SnowRing_init_00003EE4)(this, a3, 2, a4, a5, f6, f7);
    this->unk154[2] = 24.0f;
    this->unk154[4] = 140.0f;
}

/* EXACT via an IDO 7.1 NCC (C++) donor: src/snoweffects_uso/snoweffects_uso_ncc_e.cpp (agent-e run 673),
 * REPLACE_FUNC_BODY-spliced over this placeholder in both build trees. */
void HandTrail_placeEnds_00007960(void *self) {
}

void ParmFloat_read_snow_00007CCC(char *a0) {
    int tag;
    fs_gdlReadInt_snow_00003D70(&tag);
    fs_gdlReadFloat_snow_00003DAC((float *)(a0 + 0x10));
}

void ParmQuad_read_snow_00007CFC(char *a0) {
    int tag;
    fs_gdlReadInt_snow_00003D70(&tag);
    fs_gdlReadQuad_snow_00003DE8((Quad4 *)(a0 + 0x10));
}

void ParmInt_read_snow_00007D2C(char *a0) {
    int tag;
    fs_gdlReadInt_snow_00003D70(&tag);
    fs_gdlReadInt_snow_00003D70((int *)(a0 + 0x10));
}

void ParmVec3_read_snow_00007D5C(char *a0) {
    int tag;
    fs_gdlReadInt_snow_00003D70(&tag);
    fs_gdlReadVec3_snow_00003E40((Vec3 *)(a0 + 0x10));
}

void *SnowColourItemType_create_00007D8C(int *arg0) {
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

void fs_gdlReadInt_snow_00007E20(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void fs_gdlReadFloat_snow_00007E5C(float *dst) {
    float buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void fs_gdlReadQuad_snow_00007E98(Quad4 *dst) {
    Quad4 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 16);
    *dst = buf;
}

void fs_gdlReadVec3_snow_00007EF0(Vec3 *dst) {
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
void BoardSpray_init_00007F60(SnowRing *this, int a1, int a2, int a3, int a4, int a5, float f6, float f7) {
    (void)&a1;
    (void)&a2;
    (void)&a3;
    this->node.name = (char *)&D_00000000 + 0x810; /* "board spray" */
    XCALL(gl_func_00000000_snow_3ee4, SnowRing_init_00003EE4)(this, a3, 4, a4, a5, f6, f7);
    CALL(BoardSpray_setDefaults_00009010)(this, 97);
    this->unk194 = 0.0f;
    this->unk198 = 100.0f;
}

/* EXACT via an IDO 7.1 NCC (C++) donor: src/snoweffects_uso/snoweffects_uso_ncc_7fe0.cpp (trail emitter update,
 * agent-a runs 661-748), REPLACE_FUNC_BODY-spliced over this placeholder in both build trees. */
void BoardSpray_emit_00007FE0(void) {
}

/* EXACT via an IDO 7.1 NCC (C++) donor: src/snoweffects_uso/snoweffects_uso_ncc_e.cpp (agent-e run 639),
 * REPLACE_FUNC_BODY-spliced over this placeholder in both build trees. */
void BoardSpray_draw_00008C60(void *self) {
}

/* EXACT via an IDO 7.1 NCC (C++) donor: src/snoweffects_uso/snoweffects_uso_ncc_e.cpp (agent-e run 659),
 * REPLACE_FUNC_BODY-spliced over this placeholder in both build trees. */
void BoardSpray_placeEnds_00008D70(void *self) {
}

/* the RoData floats of the 0x9010 defaults (own lui each) */
extern f32 snow_ro_C0, snow_ro_C4, snow_ro_C8, snow_ro_CC, snow_ro_D0, snow_ro_D4, snow_ro_D8, snow_ro_DC, snow_ro_E0, snow_ro_E4;

/* emitter defaults of the 0x7F60 emitter (a1 unused): parameter floats, counts 3 / 2 */
void BoardSpray_setDefaults_00009010(SnowRing *this, int a1) {
    (void)&a1;
    this->unk1F0 = 0.0f;
    this->unk178 = snow_ro_C0;
    this->unk17C = snow_ro_C4;
    this->unk48 = 0.0f;
    this->unk19C = 0.0f;
    this->unk180 = snow_ro_C8;
    this->unk184 = snow_ro_CC;
    this->unk44 = snow_ro_D0;
    this->unk188 = 1.0f;
    this->unk1A4 = snow_ro_D4;
    this->unk1A8 = snow_ro_D8;
    this->unk1AC = snow_ro_DC;
    this->unk154[1] = 0.0f;
    this->emitPeriod = 3;
    this->unk154[2] = 184.0f;
    this->unk154[3] = 130.0f;
    this->unk154[5] = snow_ro_E0;
    this->unk154[4] = 255.0f;
    this->unk154[6] = 8.5f;
    this->unk154[7] = snow_ro_E4;
    this->unk1EC = 2;
}

/* UNNAMED: unknown-role empty two-argument virtual (board spray slot 16, SnowRing_vslot16 override) */
void BoardSpray_vslot16_000090F0(int a0, int a1) {
    (void)&a0;
    (void)&a1;
}

void ParmFloat_read_snow_000090FC(char *a0) {
    int tag;
    fs_gdlReadInt_snow_00007E20(&tag);
    fs_gdlReadFloat_snow_00007E5C((float *)(a0 + 0x10));
}

void ParmQuad_read_snow_0000912C(char *a0) {
    int tag;
    fs_gdlReadInt_snow_00007E20(&tag);
    fs_gdlReadQuad_snow_00007E98((Quad4 *)(a0 + 0x10));
}

void ParmInt_read_snow_0000915C(char *a0) {
    int tag;
    fs_gdlReadInt_snow_00007E20(&tag);
    fs_gdlReadInt_snow_00007E20((int *)(a0 + 0x10));
}

void ParmVec3_read_snow_0000918C(char *a0) {
    int tag;
    fs_gdlReadInt_snow_00007E20(&tag);
    fs_gdlReadVec3_snow_00007EF0((Vec3 *)(a0 + 0x10));
}

void *SnowColourItemType_create_000091BC(int *arg0) {
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

void fs_gdlReadInt_snow_00009250(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void fs_gdlReadFloat_snow_0000928C(float *dst) {
    float buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void fs_gdlReadQuad_snow_000092C8(Quad4 *dst) {
    Quad4 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 16);
    *dst = buf;
}

/* UNNAMED: unknown-role empty virtual (MapLighter vtable Data+0x9C4 slot 11) */
void MapLighter_vslot11_00009320(void) {
}

/* 0x9328: the SnowTarget Cfront ctor (SNode base, Parms at +0x240 with five inline Parm ctors).
 * EXACT via the MIPSpro 7.2.1 (EDG 2.29) donor src/snoweffects_uso/snoweffects_uso_mp72_9328.cpp (agent-b run
 * 711; agent-e run 692 / agent-b runs 697 / 705 NCC drafts before it), REPLACE_FUNC_BODY-spliced over this
 * placeholder in both build trees. */
void LocalLight_ctor_00009328(void) {
}

void LocalLight_display_00009704(char *a0) {
    CALL(gfx_setBlendColour)(&D_00000000, a0 + 0x288);
    CALL(SNode_display)(a0);
}

void LocalLight_loadData_0000973C(char *a0) {
    CALL(Parms_load_00035C6C)(a0 + 0x240);
    CALL(SNode_loadData)(a0);
}

/* EXACT via an IDO 7.1 NCC (C++) donor: src/snoweffects_uso/snoweffects_uso_ncc_e.cpp (agent-e run 645),
 * REPLACE_FUNC_BODY-spliced over this placeholder in both build trees. */
void LocalLight_checkDirty_00009768(void *self) {
}

/* EXACT via an IDO 7.1 NCC (C++) donor: src/snoweffects_uso/snoweffects_uso_ncc_e.cpp (agent-e run 653),
 * REPLACE_FUNC_BODY-spliced over this placeholder in both build trees. */
void LocalLight_buildLitVerts_00009884(void *self) {
}

/* a "Local Light" (LocalLight_ctor_00009328, an SNode with the DynLight::Parms at +0x240): the
 * mesh vertices it lights and their flicker state (only what the plain-C methods touch) */
typedef struct LocalLight {
    /* 0x000 */ char unk000[0x5C];
    /* 0x05C */ Vec3 pos;
    /* 0x068 */ char unk068[0x70 - 0x68];
    /* 0x070 */ f32 unk70;            /* 1.0f from LocalLightType_create */
    /* 0x074 */ char unk074[0xAC - 0x74];
    /* 0x0AC */ s16 vtx[0x40];        /* lit mesh vertex indices */
    /* 0x12C */ SnowRGBA col[0x40];   /* their saved colours; .a = falloff */
    /* 0x22C */ s32 n;
    /* 0x230 */ s32 updateList;
    /* 0x234 */ s32 flags;            /* bit 2: in view (MapLighter_cullLights) */
    /* 0x238 */ s32 timer;
    /* 0x23C */ s32 step;
    /* 0x240 */ char unk240[0x288 - 0x240];
    /* 0x288 */ Vec3 colour;
    /* 0x294 */ char unk294[0x2AC - 0x294];
    /* 0x2AC */ s32 seq;              /* flicker sequence index (Data+0x958) */
    /* 0x2B0 */ char unk2B0[0x2C4 - 0x2B0];
    /* 0x2C4 */ s32 period;
} LocalLight;

/* save the mesh colours of the lit vertices (the base the flicker adds to) */
void LocalLight_saveColours_00009BE8(LocalLight *this, MorphPoly *src) {
    SnowRGBA *cols = (SnowRGBA *)src->poly.base.vtxColours;
    int i;
    for (i = 0; i < this->n; i++) {
        int idx = this->vtx[i];
        this->col[i].r = cols[idx].r;
        this->col[i].g = cols[idx].g;
        this->col[i].b = cols[idx].b;
    }
}

/* the flicker sequences named by Data+0x958[this+0x2AC]: count, then count brightness bytes */
typedef struct SnowFlicker {
    s32 count;
    u8 *vals;
} SnowFlicker;
extern SnowFlicker *snow_flickerTbl_958[];


void LocalLight_flicker_00009C48(LocalLight *this, MorphPoly *src) {
    SnowRGBA *cols = (SnowRGBA *)src->poly.base.vtxColours;
    SnowFlicker *fl = snow_flickerTbl_958[this->seq];
    float f;
    int i;
    if (--this->timer < 0) {
        this->timer = this->period;
        if (fl->count == ++this->step) {
            this->step = 0;
        }
    }
    f = (unsigned int)fl->vals[this->step] / 255.0f;
    for (i = 0; i < this->n; i++) {
        int idx = this->vtx[i];
        int r = this->col[i].r;
        int g = this->col[i].g;
        int b = this->col[i].b;
        int k = (unsigned int)this->col[i].a * f;
        r += this->colour.x * k;
        g += this->colour.y * k;
        b += this->colour.z * k;
        if (r > 255) r = 255;
        if (g > 255) g = 255;
        if (b > 255) b = 255;
        cols[idx].r = r;
        cols[idx].g = g;
        cols[idx].b = b;
    }
    if (this->updateList != 0) {
        CALL(MorphPoly_applyUpdateList_0004A9F0)(src, (void *)src->poly.base.vtxColours, this->updateList);
    }
}

/* item-type create: 0x9328 ctor(0), +0x70 = 1.0f, attach the type's +0x40 node.
 * The test re-reads type->head instead of the head local (the target's beqz + delay-slot copy), and the
 * re-read value goes to list_append as an extra third argument: it is what keeps it in a2, with the head
 * copy in a1 (the "invisible extra arg" lever, IDO_CODEGEN kit II; agent-b run 697). */
void *LocalLightType_create_00009E10(char *type) {
    int *item;
    int *head;
    item = (int *)CALL(LocalLight_ctor_00009328)(0) /* 0x9328 ctor */;
    ((LocalLight *)item)->unk70 = 1.0f;
    head = (int *)((ItemType *)type)->unk40;
    if ((int *)((ItemType *)type)->unk40 != 0) {
        CALL(list_append)((char *)item + 0x10, head, (int *)((ItemType *)type)->unk40);
        if ((int)NODE(head)->parent != 0) {
            NODE(head)->unk04 = 1;
        }
        NODE(head)->parent = (Node *)item;
    }
    return item;
}

/* SnowLink ctor (the Cfront this == 0 protocol as `this != 0 || (this = new) != 0`): Node_ctor(Data+0xA64), vtable Data+0x9C4, name Data+0xA6C,
 * then a new 0x9328 target attached as its child */
/* the SnowLink vtable (Data+0x9C4; its USO reloc names the object, so the lo16 is 0: a 0 pin) */
extern char snowLink_vtbl_9C4;

SnowLink *MapLighter_ctor_00009E88(SnowLink *this, int a1, void *src, int a3) {
    Node *c;
    if (this != 0 || (this = (SnowLink *)CALL(mem_new)(0x50)) != 0) {
        CALL(Node_ctor)(this, (char *)&D_00000000 + 0xA64);
        this->node.vptr = (char *)&snowLink_vtbl_9C4;
        this->node.name = (char *)&D_00000000 + 0xA6C;
        this->unk2C = a1;
        this->src = src;
        this->unk34 = a3;
        this->tgt = c = (Node *)CALL(LocalLight_ctor_00009328)(0);
        CALL(list_append)(&this->node.children, c);
        if (c->parent != 0) {
            c->unk04 = 1;
        }
        c->parent = (Node *)this;
    }
    return this;
}

/* EXACT via an IDO 7.1 NCC (C++) donor: src/snoweffects_uso/snoweffects_uso_ncc_9F44.cpp
 * (agent-h run 621), REPLACE_FUNC_BODY-spliced over this placeholder in both build trees. */
void MapLighter_syncLight_00009F44(void *self) {
}

/* EXACT via an IDO 7.1 NCC (C++) donor: src/snoweffects_uso/snoweffects_uso_ncc_e.cpp (agent-e run 639),
 * REPLACE_FUNC_BODY-spliced over this placeholder in both build trees. */
void MapLighter_addDiffuse_00009FE8(void *self) {
}

/* EXACT via an IDO 7.1 NCC (C++) donor: src/snoweffects_uso/snoweffects_uso_ncc_a.cpp (snow light update,
 * agent-a runs 661 / 665 / 669 / 677), REPLACE_FUNC_BODY-spliced over this placeholder in both build trees. */
void MapLighter_update_0000A10C(void) {
}

/* the ItemType item list walked by 0xA760 (a cursor / next iterator, as game_libs
 * hud_findMarkers_000029F8) */
typedef struct SnowIterNode {
    char *data;
    struct SnowIterNode *next;
} SnowIterNode;
typedef struct SnowTypeKey {
    s32 v;
} SnowTypeKey;
#define SNOW_ITER_NEXT(it) \
    (((it)[0] = (it)[1], (it)[1] != 0) ? ((it)[1] = (it)[1]->next, (it)[0]->data) : 0)
extern char *snow_itemMgr;  /* bootup Data+0x776C (0 pin) */
extern char snow_gGfxLib;   /* bootup Bss+0x3C168 (0 pin) */
extern int gl_func_00000000_snow_cull(void *, int, float, float, float, int, float); /* cam_isSphereInView */

/* 0xA760: cull pass over the snow lights. Look the ItemType tagged by Data+0xA80 up in the ItemMgr
 * and walk its items; an item with +0x22C and +0x2AC set whose position (+0x5C) is in view
 * (cam_isSphereInView on the GfxLib camera root, radius 0) gets bit 2 of +0x234. Frame levers: the
 * iterator is block-scoped in the if (so it sits below the call's key copy) and padA is an
 * unidentified 8-byte local; item is assigned before the tests that read it[0]->data again (the
 * target's v0 -> s0 copy). */
void MapLighter_cullLights_0000A760(char *self) {
    SnowTypeKey key;
    char padA[0x8];
    char *type;
    char *item;
    key.v = *(int *)(&D_00000000 + 0xA80); /* RAW: data-table Data+0xA80: the "lig" ItemType tag word */
    type = (char *)CALL(ItemMgr_findType_0003EA98)(snow_itemMgr, key);
    if (type != 0) {
        SnowIterNode *it[2];
        it[1] = (SnowIterNode *)((ItemType *)type)->node.children; /* the type's items */
        while (SNOW_ITER_NEXT(it) != 0) {
            item = it[0]->data;
            if (((LocalLight *)it[0]->data)->n != 0 && ((LocalLight *)it[0]->data)->seq != 0) {
                if (XCALL(gl_func_00000000_snow_cull, cam_isSphereInView)(((GfxLib *)&snow_gGfxLib)->curWindow, 3,
                        ((LocalLight *)item)->pos.x, ((LocalLight *)item)->pos.y, ((LocalLight *)item)->pos.z, 0, 0.0f)) {
                    ((LocalLight *)item)->flags |= 4;
                }
            }
        }
    }
}

void ParmFloat_read_snow_0000A870(char *a0) {
    int tag;
    fs_gdlReadInt_snow_00009250(&tag);
    fs_gdlReadFloat_snow_0000928C((float *)(a0 + 0x10));
}

void ParmQuad_read_snow_0000A8A0(char *a0) {
    int tag;
    fs_gdlReadInt_snow_00009250(&tag);
    fs_gdlReadQuad_snow_000092C8((Quad4 *)(a0 + 0x10));
}

void ParmInt_read_snow_0000A8D0(char *a0) {
    int tag;
    fs_gdlReadInt_snow_00009250(&tag);
    fs_gdlReadInt_snow_00009250((int *)(a0 + 0x10));
}

