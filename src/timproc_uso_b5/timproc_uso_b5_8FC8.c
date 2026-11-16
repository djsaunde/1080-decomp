/* SelItemsMgr_ctor (0x1960 / 1624 words) -- the timing-screen master
 * constructor Screen::Screen(int *, int *), written as cfront-style C for IDO 7.1 cfe -O2
 * (agent-c runs 222-225).  EXACT donor: spliced over the unit's empty placeholder in both build
 * trees (Makefile TIMB5_8FC8_DONOR); the target's 5-entry tail switch is a cfe
 * jumptable (renamed SelItemsMgr_ctor_rodata by the splice, pinned to the
 * USO RoData +0x324 table), which the NCC donor (timproc_uso_b5_ncc_8FC8_nm.cpp, 93.39)
 * cannot produce.
 *
 * cfront lowering: `this` is the parameter itself (`if (arg0 || (arg0 = new(0x60)))`), the
 * inline members are expanded in place as statement macros (B::add, the S70 / Panel / W16C
 * ctors with the nested base-ctor allocation check, V3::seti, attachKids, the 0x80000 flag
 * restore) and the inline temporaries are a FEW function-scope pointers reused across all
 * expansions (w / _wp / n / t / _at): one variable per temporary site leaves uopt with too
 * many candidates (frame 0x280, everything spilled).  Which pointer each expansion reuses
 * decides the s0 / s1 colouring.  86.64 (m2c body) -> 94.36 standalone.
 * 94.36 -> 95.77: locals declared in the target's frame order (v2a..v3b at 0x154..0x130, the
 * row pointers 0x12C..0x120, items 0x100, e2 0xF0, the n spill home 0xE0, v1a / v1b, sb1 0xC0, the
 * site-1 loop counter home 0xBC, the two by-value V3 copies 0x94 / 0x84, sb2 0x80, sb3 0x68), one
 * V3 temporary per setPos site, and the kid ctor run of each widget written on ONE source line
 * (comma expression; the line layout moves the result stores behind the next call's arguments).
 * 96.01 -> 98.81 (agent-c run 223, length = target): no local may sit in t0 (uopt-coloured variables
 * leave ugen's temp ring one short -- the item loop holds its vtable in w and reads its float through
 * import_808000CC + 0x320, a named extern's address was hoisted into s0), per-expansion temp roles
 * (S70 outer t / inner w, rows bound through t, the +0x284 and 0x2B8 -> 0x29C copies through w, the
 * site-2/3 zero fills through u which site 3's +0x284 store reads again), one-line vtable / name store
 * pairs, the hide store through items[i], and the site-3 restore store through alias f8fc8_ee6c_r3.
 * 98.88 -> 99.03 (agent-c run 224): the kid-position zeros spelled 0.f / .0f / 0.00f (three uopt constants,
 * none coloured): sites 1/2 get the target's three mtc1 zero.  The body is now 4 words long; those are the
 * sites 2/3 loop-counter spill / reload pairs that the shared-zero f0 had been hiding.
 * 99.03 -> 99.96 (agent-c run 225, length = target): sites 2/3 counters i2 / i3 (function scope, declared
 * above sb3: sb3 home 0x68) in s0 -- nine empty blocks in the loop body drop their priority below t, and one
 * empty block in W16C drops _wp's below t so it still finds s0 already paid; the kid passed as the call
 * argument through _at (a1), the site-2/3 zero fills through _at (a1), site 3's +0x284 store through w.
 * 99.96 -> EXACT: no empty block inside site 3's +0x284 store, so the site-3 &t1 copy pointer's split
 * piece shares its block with the following FP(self, 0x58) temp (v0) and lands in a2 like sites 1/2. */
#include "common.h"
#include "game/select.h"
#include "gdl/node.h"
#include "game/input.h"
#include "game/course.h" /* WorldRegistry (GameFlow.game) */
#include "gdl/light.h"   /* LightTarget (each Tesuri model's inner node) */
struct DBlk_8D38 { char _pad[0x154]; unsigned char *ptr; };
extern char import_80020098; extern char import_800200CC;
extern char *SelItemParms_ctor(); extern char *SelItemInfo_ctor(); extern char *SelItem_reset_00003890();
typedef struct { f32 f0; f32 f4; f32 f8; } VecH;
extern char timproc_uso_b5_D_807FDB64;
extern char gPolyGeomMode_3EEC8_8005EE6C;
extern char f8fc8_ee6c_r3; /* site-3 flag-restore store: separate symbol keeps the address un-CSE'd */
extern char import_80807FB8;
extern struct DBlk_8D38 import_800201EC;
extern int gen_imp_mem_new();
extern int gen_imp_Node_ctor();
extern int gen_imp_list_append();
extern void gen_imp_MNode_addChild();
extern int gen_imp_PolyShape_ctor();
extern int gen_imp_Node_Load();
extern void gen_imp_dbg_printf();
extern int gen_imp_QNode_ctor(char *, char *, f32, f32, f32);
extern int CommonInfo_add_0000A95C();
void CommonInfo_init_0000A928(int *a0);
extern char import_80063D04[];
extern char import_800664C4[];
extern char import_807664C4[];
extern char import_80768434[];
extern char import_8076A3A4[];
extern char import_8076C314[];
extern char import_8076E284[];
extern char import_807701F4[];
extern char import_80772164[];
extern char import_807740D4[];
extern char import_808000CC[];
extern char import_808001F4[];
extern char import_80802164[];
extern char import_80803D04[];
extern char import_808040D4[];
extern char import_808064C4[];
extern char import_80087FB8[];
extern char timproc_uso_b5_D_807E3D04[];
extern char timproc_uso_b5_D_807F8434[];
extern char timproc_uso_b5_D_807FA3A4[];
extern char timproc_uso_b5_D_807FC314[];
extern char timproc_uso_b5_D_807FE284[];
extern char timproc_uso_b5_D_807FE304[];
extern char timproc_uso_b5_D_807FE5F4[];
extern char timproc_uso_b5_D_807FEBB8[];
extern char timproc_uso_b5_D_807FEBCC[];
extern char timproc_uso_b5_D_807FEBD4[];
extern char timproc_uso_b5_D_807FEBE0[];
extern char timproc_uso_b5_D_807FEBE8[];
extern char timproc_uso_b5_D_807FEBF4[];
extern char timproc_uso_b5_D_807FEBFC[];
extern char timproc_uso_b5_D_807FEC08[];
extern char timproc_uso_b5_D_807FEC10[];
extern char timproc_uso_b5_D_807FEC18[];
extern char timproc_uso_b5_D_807FEC90[];
extern char timproc_uso_b5_D_807FEC98[];
extern char timproc_uso_b5_D_807FECA8[];
extern char timproc_uso_b5_D_807FECB0[];
extern char timproc_uso_b5_D_807FECBC[];
extern char timproc_uso_b5_D_807FECC4[];
extern char timproc_uso_b5_D_807FECD0[];
extern char timproc_uso_b5_D_807FECD8[];
extern char timproc_uso_b5_D_807FECE4[];
extern char timproc_uso_b5_D_807FECEC[];
extern char timproc_uso_b5_D_807FECF8[];
extern char timproc_uso_b5_D_807FED00[];
extern char timproc_uso_b5_D_807FED08[];
extern char timproc_uso_b5_D_807FED80[];
extern char timproc_uso_b5_D_807FED88[];
extern char timproc_uso_b5_D_807FED94[];
extern char timproc_uso_b5_D_807FED9C[];
extern char timproc_uso_b5_D_807FEDA4[];
extern char timproc_uso_b5_D_807FEE1C[];
extern char timproc_uso_b5_D_807FEE6C[];
extern f32 DF_244;
extern f32 DF_248;
extern f32 DF_24C;
extern f32 DF_250;
extern f32 DF_254;
extern f32 DF_258;
extern f32 DF_25C;
extern s32 DS_260;
extern f32 DF_264;
extern f32 DF_268;
extern f32 DF_26C;
extern f32 DF_278;
extern f32 DF_27C;
extern f32 DF_280;
extern f32 DF_284;
extern f32 DF_288;
extern f32 DF_28C;
extern f32 DF_290;
extern s32 DS_294;
extern s32 DS_298;
extern f32 DF_29C;
extern f32 DF_2A0;
extern f32 DF_2A4;
extern f32 DF_2A8;
extern f32 DF_2AC;
extern f32 DF_2B0;
extern f32 DF_2B4;
extern f32 DF_2BC;
extern f32 DF_2C0;
extern f32 DF_2C4;
extern f32 DF_2C8;
extern f32 DF_2CC;
extern f32 DF_2D0;
extern f32 DF_2D4;
extern s32 DS_2D8;
extern s32 DS_2DC;
extern f32 DF_2E0;
extern f32 DF_2E4;
extern f32 DF_2E8;
extern f32 DF_2EC;
extern f32 DF_2F0;
extern f32 DF_2F4;
extern f32 DF_2F8;
extern f32 DF_2FC;
extern f32 DF_300;
extern f32 DF_304;
extern f32 DF_308;
extern f32 DF_30C;
extern f32 DF_310;
extern f32 DF_314;
extern f32 DF_318;
extern f32 DF_320;
extern char f8fc8_e304_1[], f8fc8_e304_2[], f8fc8_e304_3[], f8fc8_e304_4[], f8fc8_e3d04_2[], f8fc8_ee6c_sb1[], f8fc8_ee6c_sb2[], f8fc8_ee6c_sb3[];
extern char import_80020098_a, import_80020098_b, import_80020098_c, import_80020098_d;
#define FW(p, o) (*(int *)((char *)(p) + (o)))
#define FF(p, o) (*(f32 *)((char *)(p) + (o)))
#define FP(p, o) (*(char **)((char *)(p) + (o)))
#define ALLOC(n) ((char *)gen_imp_mem_new(n))
/* inline B::add(B *n): list insert + parent link */
#define ADDV(V, T, N) { V = (T); w = (char *)(N); gen_imp_list_append(&NODE(V)->children, w); if (1) {} if (NODE(w)->parent) NODE(w)->unk04 = 1; NODE(w)->parent = (Node *)V; }
#define ADDW(V, W, T, N) { V = (T); W = (N); gen_imp_list_append(&NODE(V)->children, W); if (1) {} if (NODE(W)->parent) NODE(W)->unk04 = 1; NODE(W)->parent = (Node *)V; }
#define ADD(T, N) { _at = (T); w = (char *)(N); gen_imp_list_append(&NODE(_at)->children, w); if (1) {} if (NODE(w)->parent) NODE(w)->unk04 = 1; NODE(w)->parent = (Node *)_at; }
#define CFGS(S) FP(&S, 0x134)
/* The four stage lists (Character / Board / Course / LevelList, 0x70 = SelItemList):
 * cfront ctor(this) with the nested base-ctor allocation check. VT is the SelItemList
 * base vtable (Data 0xA94), VT2 the class vtable (0xB04 / 0xB74 / 0xBE4 / 0xC54), NM
 * the node name; the three CommonInfo words and dispItems are copied in, count = 0. */
#define SIL(p) ((SelItemList *)(p))
#define NEWS70(P, D, VT, VT2, NM, S) { n = MGR->dispItems; t = ALLOC(0x70); \
    if (t) { w = t; if (w || (w = ALLOC(0x70))) { \
        gen_imp_Node_ctor(w, D); SIL(w)->vptr = (VT); } \
      SIL(t)->vptr = (VT2); *(char **)&NODE(t)->name = NM; SIL(t)->bg = (char *)WORLDREG(CFGS(S))->bgRoot; SIL(t)->fg = (char *)WORLDREG(CFGS(S))->fgRoot; \
       SIL(t)->mainGame = (char *)WORLDREG(CFGS(S))->mainGame; SIL(t)->dispItems = n; SIL(t)->count = 0;  } \
    P = t; }
#define NEWPANEL(P, D, NM, VT) { w = ALLOC(0x2BC); \
    if (w) { _wp = w; if (_wp || (_wp = ALLOC(0x2B8))) { \
        gen_imp_Node_ctor(_wp, D); *(char **)&NODE(_wp)->vptr = &timproc_uso_b5_D_807FDB64; \
        SelItemParms_ctor(&((SelItem *)_wp)->parms); SelItemInfo_ctor(&((SelItem *)_wp)->info); } \
      *(char **)&NODE(w)->vptr = VT; *(char **)&NODE(w)->name = NM; SelItem_reset_00003890(w); } \
    P = (void *)w; }
#define LT(p) ((LightTarget *)(p))
#define TM(p) ((SelTesuriModel *)(p))
#define W16C(P, D, F0) { _wp = ALLOC(0x16C); \
    if (_wp) { gen_imp_QNode_ctor(_wp, D, 0.0f, 0.0f, 0.0f); fv = F0; *(char **)&NODE(_wp)->vptr = import_80087FB8; LT(_wp)->mask = 1; \
      LT(_wp)->unk108[0] = fv; LT(_wp)->unk108[1] = fv; LT(_wp)->unk108[2] = fv; LT(_wp)->scale = 1.0f; } \
    if (1) {} P = _wp; }
/* three DIFFERENT spellings of 0: cfe passes float literals to uopt as text, so each spelling is its own
 * constant candidate with one use per site (never coloured: three ugen mtc1 zero, as in the target) */
#define SETI(P, V) { P = (char *)&((QNode *)(V))->vqs; if (1) {} ((AnimPose *)P)->trans[0] = 0.f; if (1) {} ((AnimPose *)P)->trans[1] = .0f; if (1) {} ((AnimPose *)P)->trans[2] = 0.00f; }
/* sites 2/3: the kid is read into P and passed as the call argument (an LR live into a call as its
 * argument is forbidden v0/v1: P lands in a1 like the target), the counter is its own function-scope
 * variable, and nine empty blocks in the loop body lower the counter's priority below the first s0 user
 * so it is coloured with an already-paid s0 instead of v1 + spill/reload around the call */
#define ATTACH(P, W, I) { for (I = 0; I < TM(W)->numShapes; I++) { P = TM(W)->shapes[I]; gen_imp_MNode_addChild(W, P); { P = (char *)&NODE(TM(W)->shapes[I])->doFlags; *(int *)P &= ~4; } if (1) {} if (1) {} if (1) {} if (1) {} if (1) {} if (1) {} if (1) {} if (1) {} if (1) {}  } }
#define ATTACH1(P, W) { for (i1 = 0; i1 < TM(W)->numShapes; i1++) { gen_imp_MNode_addChild(W, TM(W)->shapes[i1]); { P = (char *)&NODE(TM(W)->shapes[i1])->doFlags; *(int *)P &= ~4; } } }
#define RESTORE(SB) if (SB) { do {   _x = FW(&gPolyGeomMode_3EEC8_8005EE6C, 4); _x |= 0x80000; FW(&gPolyGeomMode_3EEC8_8005EE6C, 4) = _x; } while (0); } else { do {   _x = FW(&gPolyGeomMode_3EEC8_8005EE6C, 4); _x &= 0xFFF7FFFF; FW(&gPolyGeomMode_3EEC8_8005EE6C, 4) = _x; } while (0); }
#define WTAIL(W, F0, F1, F2, F3, F4, F5, F6) TM(W)->head.qnode.cvqs.trans[0] = F0; TM(W)->head.qnode.cvqs.trans[1] = 720.0f; TM(W)->head.qnode.cvqs.trans[2] = -220.0f; \
    TM(W)->view[0] = F1; TM(W)->view[1] = F2; TM(W)->view[2] = F3; TM(W)->alpha = 0.0f; TM(W)->focused = 0; TM(W)->unk120 = F4; TM(W)->highlight = 1.0f; TM(W)->unk12C = F5; TM(W)->unk128 = F6;
typedef void *(*AddFn)(void *, s32, s32, f32, f32, f32, f32, f32, f32);
/* virtual call through a held vtable pointer V (the target keeps it in s1 / a register local) */
#define VSEL(V, R) V = SIL(R)->vptr;
/* slot 10 = createItem(list, 1, index, x, y, z, rx, ry, rz): the signature of the
 * CharacterList / BoardList / CourseList / LevelList _createItem overrides */
#define VCALL(V, R) ((AddFn)VT_SLOT(V, 10).fn)(VT_SLOT(V, 10).delta + (R)
#define self ((char *)arg0)
#define MGR ((SelItemsMgr *)arg0)
#define TESURI(k) (MGR->tesuri[k])
void **SelItemsMgr_ctor(void **arg0, int *arg1, int *arg2) {
    VecH v2a, v2b, v3a, v3b;
    char *a, *b, *c, *d;
    char *items[8];
    int tu;
    int i;
    char *e3;
    char *volatile e2;
    char *_at, *w, *_wp;
    char *n;
    char *t;
    VecH v1a, v1b;
    volatile int sb1;
    int i1;
    s32 t1[10];
    s32 t2[4];
    volatile int sb2;
    char *u;
    int _x;
    f32 fv;
    int i2;
    int i3;
    volatile int sb3;
    if (arg0 || (arg0 = (void **)ALLOC(0x60))) {
        gen_imp_Node_ctor(self, timproc_uso_b5_D_807FEBB8 + 0x1348);
        MGR->vptr = timproc_uso_b5_D_807FE5F4;
        *(char **)&NODE(self)->name = timproc_uso_b5_D_807FE5F4 + 0x1350;
        { char *p = ALLOC(0x30); w = p; if (w) { gen_imp_Node_ctor(w, timproc_uso_b5_D_807FEBCC + 0x135C); *(char **)&NODE(w)->vptr = timproc_uso_b5_D_807FEBD4; *(char **)&NODE(w)->name = timproc_uso_b5_D_807FEBD4 + 0x1364; FP(w, 0x2C) = self; } MGR->commonInfo = w; }
        ADD(self, MGR->commonInfo);
        CommonInfo_init_0000A928(self);
        { char *p = ALLOC(0x2C); char *q; if (p) { gen_imp_Node_ctor(p, timproc_uso_b5_D_807FEBE0 + 0x1370);
            *(char **)&NODE(p)->vptr = timproc_uso_b5_D_807FEBE8 + 0xCC4; *(char **)&NODE(p)->name = timproc_uso_b5_D_807FEBE8 + 0x1378; }
          q = MGR->fg; MGR->dispItems = p;  ADD(q, p); }
        if (INPUT_MGR(import_800200CC).nextFlowMode != 2) {
            NEWPANEL(TESURI(2), timproc_uso_b5_D_807FEBF4 + 0x1384, timproc_uso_b5_D_807FEBFC + 0x138C, timproc_uso_b5_D_807FEBFC + 0x8B4)
            ADD(self, TESURI(2));
            w = ALLOC(0x168);
            if (w) {
                gen_imp_QNode_ctor(w, timproc_uso_b5_D_807FEC08 + 0x1398, 0.0f, 0.0f, 0.0f);
                *(char **)&NODE(w)->vptr = timproc_uso_b5_D_807FEC10 + 0x4AC; *(char **)&NODE(w)->name = timproc_uso_b5_D_807FEC10 + 0x13A0;
                W16C(TM(w)->head.inner, timproc_uso_b5_D_807FEC18 + 0x13A8, FF(&import_80807FB8, 0x23C))
                gen_imp_MNode_addChild(w, TM(w)->head.inner);
                SETI(t, TM(w)->head.inner)
                TM(w)->highlight = 1.0f;
                sb1 = FW(f8fc8_ee6c_sb1, 4) & 0x80000; { _x = FW(&gPolyGeomMode_3EEC8_8005EE6C, 4); _x &= 0xFFF7FFFF; _x &= ~1; _x &= 0xFFFDFFFF; _x |= 0x2000; _x &= ~2; FW(import_800664C4, 4) = _x; } TM(w)->shapes[0] = (char *)gen_imp_PolyShape_ctor(0, import_808064C4 + 0x13C0, import_807664C4, 0), TM(w)->shapes[1] = (char *)gen_imp_PolyShape_ctor(0, timproc_uso_b5_D_807F8434 + 0x13CC, import_80768434, 0), TM(w)->shapes[2] = (char *)gen_imp_PolyShape_ctor(0, timproc_uso_b5_D_807FA3A4 + 0x13D8, import_8076A3A4, 0), TM(w)->shapes[3] = (char *)gen_imp_PolyShape_ctor(0, timproc_uso_b5_D_807FC314 + 0x13E4, import_8076C314, 0), TM(w)->shapes[4] = (char *)gen_imp_PolyShape_ctor(0, timproc_uso_b5_D_807FE284 + 0x13F0, import_8076E284, 0), TM(w)->shapes[5] = (char *)gen_imp_PolyShape_ctor(0, import_808001F4 + 0x13FC, import_807701F4, 0), TM(w)->shapes[6] = (char *)gen_imp_PolyShape_ctor(0, import_80802164 + 0x1408, import_80772164, 0), TM(w)->shapes[7] = (char *)gen_imp_PolyShape_ctor(0, import_808040D4 + 0x1414, import_807740D4, 0); TM(w)->numShapes = 8; if (1) {}
                TM(w)->unk140 = 0;
                ATTACH1(t, w)
                TM(w)->unk144 = 0;
                RESTORE(sb1)
                WTAIL(w, DF_244, DF_248, DF_24C, DF_250, DF_254, DF_258, DF_25C)
            }
            TESURI(2)->model = (void *)w;
            v1a.f0 = 110.0f; v1a.f4 = -96.0f; v1a.f8 = -510.0f;
            { n = (char *)&TESURI(2)->base.info.pos; *(VecH *)&t1 = v1a; *(VecH *)&t2 = *(VecH *)&t1; ((SelParmV3 *)n)->v[0] = ((VecH *)&t2)->f0; ((SelParmV3 *)n)->v[1] = ((VecH *)&t2)->f4; ((SelParmV3 *)n)->v[2] = ((VecH *)&t2)->f8; }
            v1b.f0 = *(f32 *)&DS_260; v1b.f4 = 0.0f; v1b.f8 = 0.0f;
            { n = (char *)&TESURI(2)->base.info.rot; *(VecH *)&t1 = v1b; *(VecH *)&t2 = *(VecH *)&t1; ((SelParmV3 *)n)->v[0] = ((VecH *)&t2)->f0; ((SelParmV3 *)n)->v[1] = ((VecH *)&t2)->f4; ((SelParmV3 *)n)->v[2] = ((VecH *)&t2)->f8; }
            { w = (char *)&TESURI(2)->base.info.scl; if (1) {} ((SelParmF *)w)->value = DF_264; }
            { w = (char *)TESURI(2)->model; TESURI(2)->base.node = w; }
            TESURI(2)->model->unk12C = DF_268;
            TESURI(2)->model->unk128 = DF_26C;
            ADDV(t, MGR->dispItems, TESURI(2)->model);
        }
        NEWS70(a, timproc_uso_b5_D_807FEC90 + 0x1420, f8fc8_e304_1 + 0xA94, timproc_uso_b5_D_807FEC98 + 0xB04, timproc_uso_b5_D_807FEC98 + 0x1428, import_80020098_a)
        CommonInfo_add_0000A95C(self, a);
        ADDW(_at, t, self, a);
        NEWS70(b, timproc_uso_b5_D_807FECA8 + 0x1438, f8fc8_e304_2 + 0xA94, timproc_uso_b5_D_807FECB0 + 0xB74, timproc_uso_b5_D_807FECB0 + 0x1440, import_80020098_b)
        CommonInfo_add_0000A95C(self, b);
        ADDW(_at, t, self, b);
        NEWS70(c, timproc_uso_b5_D_807FECBC + 0x144C, f8fc8_e304_3 + 0xA94, timproc_uso_b5_D_807FECC4 + 0xBE4, timproc_uso_b5_D_807FECC4 + 0x1454, import_80020098_c)
        CommonInfo_add_0000A95C(self, c);
        ADDW(_at, t, self, c);
        NEWS70(d, timproc_uso_b5_D_807FECD0 + 0x1460, f8fc8_e304_4 + 0xA94, timproc_uso_b5_D_807FECD8 + 0xC54, timproc_uso_b5_D_807FECD8 + 0x1468, import_80020098_d)
        CommonInfo_add_0000A95C(self, d);
        ADDW(_at, t, self, d);
        {
            NEWPANEL(TESURI(0), timproc_uso_b5_D_807FECE4 + 0x1474, timproc_uso_b5_D_807FECEC + 0x147C, timproc_uso_b5_D_807FECEC + 0x8B4)
            ADD(self, TESURI(0));
            w = ALLOC(0x168);
            if (w) {
                gen_imp_QNode_ctor(w, timproc_uso_b5_D_807FECF8 + 0x1488, 0.0f, 0.0f, 0.0f);
                *(char **)&NODE(w)->vptr = timproc_uso_b5_D_807FED00 + 0x4AC; *(char **)&NODE(w)->name = timproc_uso_b5_D_807FED00 + 0x1490;
                W16C(TM(w)->head.inner, timproc_uso_b5_D_807FED08 + 0x1498, FF(&import_80807FB8, 0x270))
                gen_imp_MNode_addChild(w, TM(w)->head.inner);
                SETI(_at, TM(w)->head.inner)
                TM(w)->highlight = 1.0f;
                sb2 = FW(f8fc8_ee6c_sb2, 4) & 0x80000; { _x = (FW(&gPolyGeomMode_3EEC8_8005EE6C, 4) & 0xFFF7FFFF & ~1 & 0xFFFDFFFF) | 0x2000; if (1) { _x |= 2; } FW(import_80063D04, 4) = _x; } TM(w)->shapes[0] = (char *)gen_imp_PolyShape_ctor(0, import_80803D04 + 0x14A4, timproc_uso_b5_D_807E3D04, 0); TM(w)->numShapes = 1; if (1) {}
                TM(w)->unk140 = 0;
                ATTACH(_at, w, i2)
                TM(w)->unk144 = 0;
                RESTORE(sb2)
                WTAIL(w, DF_278, DF_27C, DF_280, DF_284, DF_288, DF_28C, DF_290)
            }
            TESURI(0)->model = (void *)w;
            v2a.f0 = *(f32 *)&DS_294; v2a.f4 = 528.0f; v2a.f8 = 180.0f;
            { n = (char *)&TESURI(0)->base.info.pos; *(VecH *)&t1 = v2a; *(VecH *)&t2 = *(VecH *)&t1; ((SelParmV3 *)n)->v[0] = ((VecH *)&t2)->f0; ((SelParmV3 *)n)->v[1] = ((VecH *)&t2)->f4; ((SelParmV3 *)n)->v[2] = ((VecH *)&t2)->f8; }
            v2b.f0 = *(f32 *)&DS_298; v2b.f4 = DF_29C; v2b.f8 = DF_2A0;
            { n = (char *)&TESURI(0)->base.info.rot; *(VecH *)&t1 = v2b; *(VecH *)&t2 = *(VecH *)&t1; ((SelParmV3 *)n)->v[0] = ((VecH *)&t2)->f0; ((SelParmV3 *)n)->v[1] = ((VecH *)&t2)->f4; ((SelParmV3 *)n)->v[2] = ((VecH *)&t2)->f8; }
            { w = (char *)&TESURI(0)->base.info.scl; if (1) {} ((SelParmF *)w)->value = DF_2A4; }
            { w = (char *)TESURI(0)->model; TESURI(0)->base.node = w; }
            TESURI(0)->model->unk12C = DF_2A8;
            TESURI(0)->model->unk128 = DF_2AC;
            ADDV(t, MGR->dispItems, TESURI(0)->model);
        }
        {
            NEWPANEL(TESURI(1), timproc_uso_b5_D_807FED80 + 0x1510, timproc_uso_b5_D_807FED88 + 0x1518, timproc_uso_b5_D_807FED88 + 0x8B4)
            ADD(self, TESURI(1));
            w = ALLOC(0x168);
            if (w) {
                gen_imp_QNode_ctor(w, timproc_uso_b5_D_807FED94 + 0x1524, 0.0f, 0.0f, 0.0f);
                *(char **)&NODE(w)->vptr = timproc_uso_b5_D_807FED9C + 0x4AC; *(char **)&NODE(w)->name = timproc_uso_b5_D_807FED9C + 0x152C;
                { _wp = ALLOC(0x16C);
                  if (_wp) { gen_imp_QNode_ctor(_wp, timproc_uso_b5_D_807FEDA4 + 0x1534, 0.0f, 0.0f, 0.0f); *(char **)&NODE(_wp)->vptr = import_80087FB8; /* vptr: bootup LightTarget_vtbl (gl_data.h) */
                    LT(_wp)->unk108[0] = DF_2B0; LT(_wp)->unk108[1] = DF_2B4; LT(_wp)->unk108[2] = FF(timproc_uso_b5_D_807FEE6C, 0x2B8); LT(_wp)->mask = 1; LT(_wp)->scale = 1.0f; }
                  TM(w)->head.inner = _wp; }
                gen_imp_MNode_addChild(w, TM(w)->head.inner);
                SETI(_at, TM(w)->head.inner)
                TM(w)->highlight = 1.0f;
                sb3 = FW(f8fc8_ee6c_sb3, 4) & 0x80000; { _x = (FW(&gPolyGeomMode_3EEC8_8005EE6C, 4) & 0xFFF7FFFF & ~1 & 0xFFFDFFFF) | 0x2000; if (1) { _x |= 2; } FW(import_80063D04, 4) = _x; } TM(w)->shapes[0] = (char *)gen_imp_PolyShape_ctor(0, import_80803D04 + 0x1540, f8fc8_e3d04_2, 0); TM(w)->numShapes = 1; if (1) {}
                TM(w)->unk140 = 0;
                ATTACH(_at, w, i3)
                TM(w)->unk144 = 0;
                if (sb3) { do { _x = FW(&gPolyGeomMode_3EEC8_8005EE6C, 4); _x |= 0x80000; FW(&f8fc8_ee6c_r3, 4) = _x; } while (0); } else { do { _x = FW(&gPolyGeomMode_3EEC8_8005EE6C, 4); _x &= 0xFFF7FFFF; FW(&gPolyGeomMode_3EEC8_8005EE6C, 4) = _x; } while (0); }
                WTAIL(w, DF_2BC, DF_2C0, DF_2C4, DF_2C8, DF_2CC, DF_2D0, DF_2D4)
            }
            TESURI(1)->model = (void *)w;
            v3a.f0 = *(f32 *)&DS_2D8; v3a.f4 = 528.0f; v3a.f8 = -480.0f;
            { n = (char *)&TESURI(1)->base.info.pos; *(VecH *)&t1 = v3a; *(VecH *)&t2 = *(VecH *)&t1; ((SelParmV3 *)n)->v[0] = ((VecH *)&t2)->f0; ((SelParmV3 *)n)->v[1] = ((VecH *)&t2)->f4; ((SelParmV3 *)n)->v[2] = ((VecH *)&t2)->f8; }
            v3b.f0 = *(f32 *)&DS_2DC; v3b.f4 = DF_2E0; v3b.f8 = DF_2E4;
            { n = (char *)&TESURI(1)->base.info.rot; *(VecH *)&t1 = v3b; *(VecH *)&t2 = *(VecH *)&t1; ((SelParmV3 *)n)->v[0] = ((VecH *)&t2)->f0; ((SelParmV3 *)n)->v[1] = ((VecH *)&t2)->f4; ((SelParmV3 *)n)->v[2] = ((VecH *)&t2)->f8; }
            { w = (char *)&TESURI(1)->base.info.scl; ((SelParmF *)w)->value = DF_2E8; }
            { w = (char *)TESURI(1)->model; TESURI(1)->base.node = w; }
            TESURI(1)->model->unk12C = DF_2EC;
            TESURI(1)->model->unk128 = 1.0f;
            ADDV(t, MGR->dispItems, TESURI(1)->model);
        }
        VSEL(w, a) VCALL(w, a), 1, 5, 0.0f, -150.0f, -1000.0f, 0.0f, 0.0f, 0.0f);
        VSEL(w, a) VCALL(w, a), 1, 3, 750.0f, -150.0f, DF_2F0, 0.0f, 0.0f, 0.0f);
        VSEL(w, a) VCALL(w, a), 1, 4, 750.0f, -150.0f, DF_2F4, 0.0f, 0.0f, 0.0f);
        VSEL(w, a) VCALL(w, a), 1, 2, 750.0f, -150.0f, DF_2F8, 0.0f, 0.0f, 0.0f);
        VSEL(w, a) VCALL(w, a), 1, 1, 750.0f, -150.0f, DF_2FC, 0.0f, 0.0f, 0.0f);
        VSEL(w, b) VCALL(w, b), 1, 0, 400.0f, -150.0f, -1000.0f, 0.0f, DF_300, 0.0f);
        VSEL(w, b) VCALL(w, b), 1, 1, 750.0f, 200.0f, DF_304, 0.0f, 0.0f, 0.0f);
        VSEL(w, b) VCALL(w, b), 1, 2, 750.0f, 200.0f, DF_308, 0.0f, 0.0f, 0.0f);
        VSEL(w, b) VCALL(w, b), 1, 3, 400.0f, -150.0f, -1000.0f, 0.0f, DF_30C, 0.0f);
        VSEL(w, b) VCALL(w, b), 1, 4, 400.0f, -150.0f, -1000.0f, 0.0f, DF_310, 0.0f);
        VSEL(w, b) VCALL(w, b), 1, 5, 400.0f, -150.0f, -1000.0f, 0.0f, DF_314, 0.0f);
        VSEL(w, b) VCALL(w, b), 1, 6, 400.0f, -150.0f, -1000.0f, 0.0f, DF_318, 0.0f);
        VSEL(w, b) VCALL(w, b), 1, 7, 400.0f, -150.0f, -1000.0f, 0.0f, FF(import_808000CC, 0x31C), 0.0f);
        {
            tu = **(u16 **)&INPUT_MGR(import_800201EC).options;
            if (INPUT_MGR(import_800200CC).nextFlowMode != 6) tu &= ~3;
            for (i = 0; i != 8; i++) {
                char *r;
                VSEL(w, c) r = VCALL(w, c), 1, i, 400.0f, 210.0f, -280.0f, 0.0f, 0.0f, FF(import_808000CC, 0x320));
                items[i] = r;
                if (!(tu & (1U << i))) ((SelItem *)items[i])->enabled = 0.0f; /* locked: bit i of the BackupOptions unlocks word */
            }
            VSEL(w, d) VCALL(w, d), 1, 1, -220.0f, 200.0f, -400.0f, 0.0f, 0.0f, 0.0f);
            VSEL(w, d) e2 = VCALL(w, d), 1, 2, -220.0f, 200.0f, -400.0f, 0.0f, 0.0f, 0.0f);
            VSEL(w, d) e3 = VCALL(w, d), 1, 3, -220.0f, 200.0f, -400.0f, 0.0f, 0.0f, 0.0f);
            gen_imp_dbg_printf(timproc_uso_b5_D_807FEE1C + 0x15AC, (*(u16 **)&INPUT_MGR(import_800201EC).options)[1]);
            switch ((*(u16 **)&INPUT_MGR(import_800201EC).options)[1]) {
            case 1:
                 ((SelItem *)e2)->enabled = 0.0f; ((SelItem *)e3)->enabled = 0.0f; break;
            case 2:
                ((SelItem *)e3)->enabled = 0.0f;
                break;
            case 3: case 4: case 5:
                break;
            }
        }
        gen_imp_Node_Load(self, 1, arg1);
        gen_imp_Node_Load(MGR->commonInfo, 1, arg2);
        w = MGR->vptr; ((void (*)(void *))VT_SLOT(w, 3).fn)(VT_SLOT(w, 3).delta + self);
        { char *m = MGR->dispItems; w = *(char **)&NODE(m)->vptr; ((void (*)(void *))VT_SLOT(w, 3).fn)(VT_SLOT(w, 3).delta + m); }
    }
    return arg0;
}
