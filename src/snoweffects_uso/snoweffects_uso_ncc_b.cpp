#include "../../include/uso_call.h"  /* NCC donors build without -I include */
/* IDO 7.1 NCC (C++) donors for the snow effect's child-node creators (agent-b run 640), spliced into
 * snoweffects_uso.c.o via REPLACE_FUNC_BODY. All three EXACT:
 *   0x5FD4  SnowEffect::init (name Data+0x63C, both groups, SnowRing_init stride 4, 0x6B08, child0 +0x9C)
 *   0x6144  SnowEffect::createA (0x6420 defaults, childA +0x98, virtual slot 11 arg 8)
 *   0x6248  SnowEffect::createB (0x70C0 defaults, childB +0xA0, virtual slot 11 arg 16)
 * Each `new`s a 0x48-byte child: the inline SnowNode ctor (edg's this == 0 protocol, so a second
 * mem_new(0x48) fallback) calls the out-of-line Node ctor with the child's name (Data+0x648 / 0x650 /
 * 0x658), stores the SnowNode vptr (Data+0x60), then the child's own vptr (Data+0x1C8 / 0x158 / 0x238).
 * The child is attached to the groups at +0x238 then +0x234 (list_append on the group's child list,
 * flag = 1 if it already had a parent, parent = group), then gets virtual slot 11.
 * Load-bearing:
 *  - the `new` is an argument of the first attach (`lnk(childA = new SnowChildA, grpA)`): in one
 *    full-expression the inline ctor's `this` and the attach's q get separate edg pool words (0x24 /
 *    0x2C, a0 / a2). As two statements they share a word and the ctor's `this` is coloured a2.
 *  - list_append through a void-typed alias (gl_func_00000000_snowb_v): v0 is forbidden through a
 *    value-returning call's block, and the target keeps the group in v0 across the first attach.
 * Symbols are renamed after the compile (Makefile): the member functions to the C names, the Node
 * ctor and the four vtables to 0-pinned names in undefined_syms_auto.txt (blank lui/addiu in the ROM). */
extern "C" {
int gl_func_00000000(...);
void gl_func_00000000_snowb_v(...); /* list_append, typed void */
void gl_func_00000000_snow_3ee4(void *self, int a1, int a2, int a3, int a4, float f5, float f6);
extern char D_00000000[];
}
inline void *operator new(unsigned n) { return (void *)CALL(mem_new)(n); }

#ifdef PORT /* cfront layout (port_cfront.h, plan 17.8): explicit vptr at +0x28, the virtual slot 11 call
 * through the ROM vtable, the NCC vtable stores written out (the snowb_vtbl_* binds: snoweffects Data
 * 0x60 / 0x1C8 / 0x158 / 0x238); the Node ctor is the C Node_ctor (stores the Node vtable itself). */
#include "../../port/include/port_cfront.h"
extern "C" char snowb_vtbl_SnowNode_60[], snowb_vtbl_SnowChild0_1C8[], snowb_vtbl_SnowChildA_158[],
    snowb_vtbl_SnowChildB_238[];
CF_EXT(void *, snowb_Node_ctor, Node_ctor, (void *, char *));
struct Node {
    char pad00[4];
    /* 0x04 */ int flag;
    char pad08[0xC - 8];
    /* 0x0C */ char *name;
    /* 0x10 */ int list;      /* child list */
    /* 0x14 */ Node *parent;
    char pad18[0x28 - 0x18];
    /* 0x28 */ char *vptr;
    CF_INL Node(char *name) { snowb_Node_ctor(this, name); }
    CF_V1(void, v11, 10, int)
};
inline void lnk(Node *q, Node *s) {
    XCALL(gl_func_00000000_snowb_v, list_append)(&s->list, q);
    if (q->parent) q->flag = 1;
    q->parent = s;
}
struct SnowNode : Node { char pad2c[0x48 - 0x2C]; CF_INL SnowNode(char *name) : Node(name) { vptr = snowb_vtbl_SnowNode_60; } };
struct SnowChild0 : SnowNode { CF_INL SnowChild0() : SnowNode(D_00000000 + 0x648) { vptr = snowb_vtbl_SnowChild0_1C8; } };
struct SnowChildA : SnowNode { CF_INL SnowChildA() : SnowNode(D_00000000 + 0x650) { vptr = snowb_vtbl_SnowChildA_158; } };
struct SnowChildB : SnowNode { CF_INL SnowChildB() : SnowNode(D_00000000 + 0x658) { vptr = snowb_vtbl_SnowChildB_238; } };
#else
/* bootup Node (0x28 data bytes, vptr at +0x28; virtual slot 11 takes an int) */
struct Node {
    char pad00[4];
    /* 0x04 */ int flag;
    char pad08[0xC - 8];
    /* 0x0C */ char *name;
    /* 0x10 */ int list;      /* child list */
    /* 0x14 */ Node *parent;
    char pad18[0x28 - 0x18];
    Node(char *name);
    virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6();
    virtual void v7(); virtual void v8(); virtual void v9(); virtual void v10(); virtual void v11(int);
};

/* attach q under group s */
inline void lnk(Node *q, Node *s) {
    XCALL(gl_func_00000000_snowb_v, list_append)(&s->list, q);
    if (q->parent) q->flag = 1;
    q->parent = s;
}

/* the 0x48-byte child nodes: a SnowNode base (vtable Data+0x60) and one class per child */
struct SnowNode : Node { char pad2c[0x48 - 0x2C]; SnowNode(char *name) : Node(name) {} virtual void v1(); };
struct SnowChild0 : SnowNode { SnowChild0() : SnowNode(D_00000000 + 0x648) {} virtual void v1(); };
struct SnowChildA : SnowNode { SnowChildA() : SnowNode(D_00000000 + 0x650) {} virtual void v1(); };
struct SnowChildB : SnowNode { SnowChildB() : SnowNode(D_00000000 + 0x658) {} virtual void v1(); };

#endif
struct SnowEffect : Node {
    char pad2c[0x98 - 0x2C];
    /* 0x098 */ SnowChildA *childA;
    /* 0x09C */ SnowChild0 *child0;
    /* 0x0A0 */ SnowChildB *childB;
    char padA4[0x234 - 0xA4];
    /* 0x234 */ Node *grpB;
    /* 0x238 */ Node *grpA;
    void init(Node *gB, Node *gA, int a3, int a4, int a5, float f6, float f7);
    void createA();
    void createB();
};

void SnowEffect::init(Node *gB, Node *gA, int a3, int a4, int a5, float f6, float f7) {
    name = D_00000000 + 0x63C;
    grpA = gA;
    grpB = gB;
    XCALL(gl_func_00000000_snow_3ee4, SnowRing_init_00003EE4)(this, a3, 4, a4, a5, f6, f7);
    CALL(BoardTrail_enableNormal_00006B08)(this);
    lnk(child0 = new SnowChild0, gA);
    lnk(child0, gB);
    child0->v11(8);
}

void SnowEffect::createA() {
    CALL(BoardTrail_enablePowder_00006420)(this);
    lnk(childA = new SnowChildA, grpA);
    lnk(childA, grpB);
    childA->v11(8);
}

void SnowEffect::createB() {
    CALL(BoardTrail_enableSpec_000070C0)(this);
    lnk(childB = new SnowChildB, grpA);
    lnk(childB, grpB);
    childB->v11(16);
}

/* gbi packets for 0x559C below (0x1E60 moved to agent-c's snoweffects_uso_ncc_nm.cpp, run 704). */
#include "../game_libs/ncc_drafts/gbi_subset.h"

/* 0x559C EXACT (agent-b run 679 decode, run 686 exact): the trail-ring draw. A Mat on the
 * stack (BaseMat ctor protocol: this == 0 test, vptr Data+0x20640 at +0x5C, the 8- / 4-byte members, the
 * out-of-line BaseMat_init_000517E4; Mat vptr Data+0x1FBEC) with colour 1,1,1,1 is handed to
 * gfx_emitLights_000454C4 (float k in a3, so a typed void alias) after a gSPDisplayList of the texture's
 * list (+0xB0 -> +0x60). Then for each live ring slot (SnowRing_countSlots_000053E8 >= 2) from the read
 * cursor: the stride points scaled by gGfxLib's world scale (+0x128) into s16 Vtx positions (+0.5f) with
 * alpha (+0x1C, clamped at 0, `dead` counts the clamped ones; slot state 2 if none ran or all dead), a
 * gSPVertex of the stride vertices at buffer index i0, the 32 x 32 RGBA32 texture load from the Data+0x588
 * table when the slot's texture changes (and is not 2), then from the third slot on a strip of
 * stride - 1 quads (two F3DEX gSP1Triangle, ring indices masked by 2 * stride - 1; texture 2 reloads one
 * table entry per quad). Ends with gSPTexture off and Node_display.
 * Load-bearing: the scale words through three 0-based aliases (one lui per load as in the target; a
 * shared base is hoisted, as agent-c's 0xC90), scaled() returning through the 3-float ctor (f0 / f2 / f12
 * products), next() as `&buf[count++]` (no pool words: frame 456 with the named-local form 616), the
 * texture test reading slot->tex directly (prevTex then lives in t5 like the target), and the 5-word dead
 * block before the slot loop (pool words: puts k / mask / v / the scaled() temps at the target's slots).
 * The texture-list address through an inline holder->phys(): its `this` takes the BaseMat ctor's pool word,
 * so both share one callee-saved register as in the target (93.0 -> 97.3 %).
 * Packets go through nx() -> n1() -> next() with `(this + 0)` hops (run 686): each hop takes an edg pool
 * word, so the packet count shares the member-ctor `this` word (s1) and the list pointer the BaseMat-ctor
 * word (s0), as in the target. The trailing `if (ctx) {}` drops ctx's priority below j's (ctx s4).
 * The quad indices are written `i0 + j` / `i1 + j` (no a / b locals): uopt makes them induction variables
 * whose i0 / i1 reloads sit after the loop guard, as in the target. The four slot-loop initialisers share
 * one source line (as1 tie-break: the i1 store schedules first). */
#define V2P(x) CALL(gl_osVirtualToPhysical)(x)
struct M8B { int a, b; M8B() { b = 0; a = 0; } };
struct M4B { int a; M4B() { a = 0; } };
#ifdef PORT /* cfront layout: the BaseMat vptr at +0x5C (bootup Data 0x20640), then the Mat one (0x1FBEC) */
extern "C" char snowb_vtbl_BaseMat_559C[], snowb_vtbl_Mat_559C[];
struct SnowBaseMatB {
    float c[4];
    char p10[0x34 - 0x10]; M8B m; char p3c[0x58 - 0x3C]; M4B n;
    char *vptr;
    CF_INL SnowBaseMatB() { vptr = snowb_vtbl_BaseMat_559C; CALL(BaseMat_init_000517E4)(this); }
};
struct SnowMatB : SnowBaseMatB { char p60[0x90 - 0x60]; CF_INL SnowMatB() { vptr = snowb_vtbl_Mat_559C; } };
#else
struct SnowBaseMatB {
    float c[4];
    char p10[0x34 - 0x10]; M8B m; char p3c[0x58 - 0x3C]; M4B n;
    SnowBaseMatB() { CALL(BaseMat_init_000517E4)(this); }
    virtual void v1();
};
struct SnowMatB : SnowBaseMatB { char p60[0x90 - 0x60]; SnowMatB() {} virtual void v1(); };
#endif
struct SnowDLB { Gfx *buf; int count; };
struct SnowCtxB { char pad0[0xC]; SnowDLB *dl; Gfx *next() { return &dl->buf[dl->count++]; } Gfx *n1() { return (this + 0)->next(); } Gfx *nx() { return (this + 0)->n1(); }
    void put(unsigned int w0, unsigned int w1) { Gfx *g = next(); g->words.w0 = w0; g->words.w1 = w1; } };
struct SnowV3B { float x, y, z; SnowV3B() {} SnowV3B(float a, float b, float c) { x = a; y = b; z = c; } };
inline SnowV3B scaled(const SnowV3B &a, float sx, float sy, float sz) { return SnowV3B(a.x * sx, a.y * sy, a.z * sz); }
struct SnowPtB { SnowV3B pos; char p0c[0x1C - 0xC]; float alpha; char p20[0x2C - 0x20]; };
struct SnowSlotB { int state; int tex; int pad; };
struct SnowVtxB { short ob[3]; short flag; short tc[2]; unsigned char cn[4]; };
struct SnowDLHolderB { char pad[0x60]; int dl; int phys() { return V2P(dl); } };
struct SnowRingB {
    char pad00[0x58];
    int readOfs;     /* 0x58 */
    int writeOfs;    /* 0x5C */
    int read;        /* 0x60 */
    int write;       /* 0x64 */
    int size;        /* 0x68 */
    int stride;      /* 0x6C */
    int count;       /* 0x70 */
    char pad74[0x8C - 0x74];
    SnowVtxB *vtx;   /* 0x8C */
    SnowPtB *points; /* 0x90 */
    SnowSlotB *slots; /* 0x94 */
    char pad98[0xA8 - 0x98];
    int w, h;        /* 0xA8 */
    SnowDLHolderB *dlh; /* 0xB0 */
};
struct SnowGfxB { char pad[0x128]; SnowV3B scale; char pad134[0x254 - 0x134]; struct { char pad[0x158]; SnowCtxB *ctx; } *win; };
extern "C" SnowGfxB snowb_gfx_559C;
extern "C" int snowb_texTbl_588[];  /* Data+0x588: the ring texture table */
#define gfxB snowb_gfx_559C
extern "C" void gl_func_00000000_snowb_lights(void *, SnowCtxB *, void *, float);
#define PKTB(pkt, a, b) { Gfx *_g = (pkt); _g->words.w0 = (a); _g->words.w1 = (unsigned int)(b); }
#define TRIB(pkt, a, b, c) PKTB(pkt, 0xBF000000, _SHIFTL((a) * 2, 16, 8) | _SHIFTL((b) * 2, 8, 8) | _SHIFTL((c) * 2, 0, 8))
#define LOADTEXB(pkt, timg, width, height) \
{ gDPSetTextureImage(pkt, G_IM_FMT_RGBA, G_IM_SIZ_32b, width, timg); \
  gDPSetTile(pkt, G_IM_FMT_RGBA, G_IM_SIZ_32b, (((((width) - 1) + 1) * G_IM_SIZ_32b_TILE_BYTES) + 7) >> 3, 0, G_TX_LOADTILE, 0, 0, 5, 0, 0, 5, 0); \
  gDPLoadSync(pkt); \
  gDPLoadTile(pkt, G_TX_LOADTILE, 0, 0, ((width) - 1) << G_TEXTURE_IMAGE_FRAC, ((height) - 1) << G_TEXTURE_IMAGE_FRAC); \
  gDPPipeSync(pkt); \
  gDPSetTile(pkt, G_IM_FMT_RGBA, G_IM_SIZ_32b, (((((width) - 1) + 1) * G_IM_SIZ_32b_LINE_BYTES) + 7) >> 3, 0, G_TX_RENDERTILE, 0, 0, 5, 0, 0, 5, 0); \
  gDPSetTileSize(pkt, G_TX_RENDERTILE, 0, 0, ((width) - 1) << G_TEXTURE_IMAGE_FRAC, ((height) - 1) << G_TEXTURE_IMAGE_FRAC) }

#define gSPVertexB(pkt, v, n, v0) { Gfx *_g = (Gfx *)(pkt); _g->words.w0 = _SHIFTL(4, 24, 8) | _SHIFTL((v0) * 2, 16, 8) | _SHIFTL(((n) << 10) | (sizeof(SnowVtxB) * (n) - 1), 0, 16); _g->words.w1 = (unsigned int)(v); }
extern "C" SnowGfxB snowb_gfxX_559C, snowb_gfxY_559C, snowb_gfxZ_559C;

extern "C" void SnowRing_display_0000559C(SnowRingB *t) {
    SnowMatB m;
    m.c[0] = m.c[1] = m.c[2] = m.c[3] = 1.0f;
    SnowCtxB *ctx = gfxB.win->ctx;
    ctx->put(0x06000000, t->dlh->phys());
    XCALL(gl_func_00000000_snowb_lights, gfx_emitLights_000454C4)(&gfxB, ctx, &m, 1.0f);
    int vi = t->readOfs;
    int si = t->read;
    int n = CALL(SnowRing_countSlots_000053E8)(t);
    int prevTex = -1; int prevState = 2; int i0 = 0; int i1 = 1; /* one line: as1 tie-break order */
    { int sc0[4]; sc0[0] = 0; } /* dead: four edg pool words (frame lever) */
    if (n >= 2) {
        for (int k = 0; k < n; k++) {
            SnowPtB *p = &t->points[vi];
            int any = 0, dead = 0;
            int j;
            for (j = 0; j < t->stride; j++) {
                any = 1;
                SnowV3B v = scaled(p->pos, snowb_gfxX_559C.scale.x, snowb_gfxY_559C.scale.y, snowb_gfxZ_559C.scale.z);
                t->vtx[vi + j].ob[0] = (int)(v.x + 0.5f);
                t->vtx[vi + j].ob[1] = (int)(v.y + 0.5f);
                t->vtx[vi + j].ob[2] = (int)(v.z + 0.5f);
                if (p->alpha < 0.0f) {
                    p->alpha = 0.0f;
                    dead++;
                }
                t->vtx[vi + j].cn[3] = (int)p->alpha;
                p++;
            }
            if (!any || dead == t->stride) t->slots[si].state = 2;
            gSPVertexB(ctx->nx(),V2P(&t->vtx[vi]), t->stride, i0);
            if (t->slots[si].tex != prevTex && t->slots[si].tex != 2) {
                t->w = 32;
                t->h = 32;
                LOADTEXB(ctx->nx(), V2P(snowb_texTbl_588[t->slots[si].tex]),t->w, t->h);
            }
            prevTex = t->slots[si].tex;
            int mask = t->stride * 2 - 1;
            if (prevState != 2 || t->slots[si].state != 2) {
                if (k >= 2) {
                    for (j = 0; j < t->stride - 1; j++) {
                        if (prevTex == 2) {
                            t->w = 32;
                            t->h = 32;
                            LOADTEXB(ctx->nx(), V2P(snowb_texTbl_588[t->slots[si].tex + j]), t->w, t->h);
                        }
                        TRIB(ctx->nx(), (i0 + j - t->stride) & mask, (i1 + j - t->stride) & mask, i1 + j);
                        TRIB(ctx->nx(), i1 + j, i0 + j, (i0 + j - t->stride) & mask);
                    }
                }
            }
            prevState = t->slots[si].state;
            vi += t->stride;
            i0 = (i0 + t->stride) & mask;
            i1 = (i1 + t->stride) & mask;
            if (vi == t->size) vi = 0;
            if (++si == t->count) si = 0;
        }
    }
    PKTB(ctx->nx(), 0xBB000000, 0);
    CALL(Node_display)(t);
    if (ctx) {} /* code-free use: ctx priority 151.2 -> 140.5 (s2 -> s4) */
}
