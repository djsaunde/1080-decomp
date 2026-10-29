/* timproc_uso_b5_func_00008FC8 (0x1960 / 1624 words) -- the timing-screen master
 * constructor Screen::Screen(int *, int *), written as cfront-style C for IDO 7.1 cfe -O2
 * (agent-c run 222).  NON_MATCHING comparison donor: spliced over the unit's C body in the
 * NM object only (Makefile TIMB5_8FC8_NM_DONOR); the target's 5-entry tail switch is a cfe
 * jumptable (renamed timproc_uso_b5_func_00008FC8_rodata by the splice, pinned to the
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
 * Open: sites 2/3 loop counter in s0 (ours: P in s0, counter spilled), site 2 P colour a1 (ours a2). */
#include "common.h"
struct DBlk_8D38 { char _pad[0x154]; unsigned char *ptr; };
extern char import_80020098; extern char import_800200CC;
extern char *timproc_uso_b5_func_00002B74(); extern char *timproc_uso_b5_func_000032C8(); extern char *timproc_uso_b5_func_00003890();
typedef struct { f32 f0; f32 f4; f32 f8; } VecH;
extern char timproc_uso_b5_D_807FDB64;
extern char import_8005EE6C;
extern char f8fc8_ee6c_r3; /* site-3 flag-restore store: separate symbol keeps the address un-CSE'd */
extern char import_80807FB8;
extern struct DBlk_8D38 import_800201EC;
extern int timproc_uso_b5_func_055750();
extern int timproc_uso_b5_func_04C678();
extern int timproc_uso_b5_func_07ACE0();
extern void timproc_uso_b5_func_04DFFC();
extern int timproc_uso_b5_func_05D0E0();
extern int timproc_uso_b5_func_04C774();
extern void timproc_uso_b5_func_048E7C();
extern int timproc_uso_b5_func_0546DC(char *, char *, f32, f32, f32);
extern int timproc_uso_b5_func_0000A95C();
void timproc_uso_b5_func_0000A928(int *a0);
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
#define ALLOC(n) ((char *)timproc_uso_b5_func_055750(n))
/* inline B::add(B *n): list insert + parent link */
#define ADDV(V, T, N) { V = (T); w = (N); timproc_uso_b5_func_07ACE0(V + 0x10, w); if (1) {} if (FP(w, 0x14)) FW(w, 4) = 1; FP(w, 0x14) = V; }
#define ADDW(V, W, T, N) { V = (T); W = (N); timproc_uso_b5_func_07ACE0(V + 0x10, W); if (1) {} if (FP(W, 0x14)) FW(W, 4) = 1; FP(W, 0x14) = V; }
#define ADD(T, N) { _at = (T); w = (N); timproc_uso_b5_func_07ACE0(_at + 0x10, w); if (1) {} if (FP(w, 0x14)) FW(w, 4) = 1; FP(w, 0x14) = _at; }
#define CFGS(S) FP(&S, 0x134)
/* S70 row: cfront ctor(this) with nested base-ctor allocation check */
#define NEWS70(P, D, VT, VT2, NM, S) { n = FP(self, 0x38); t = ALLOC(0x70); \
    if (t) { w = t; if (w || (w = ALLOC(0x70))) { \
        timproc_uso_b5_func_04C678(w, D); FP(w, 0x28) = (VT); } \
      FP(t, 0x28) = (VT2); FP(t, 0xC) = NM; FW(t, 0x2C) = FW(CFGS(S), 0x84); FW(t, 0x30) = FW(CFGS(S), 0x80); \
       FW(t, 0x34) = FW(CFGS(S), 0x8C); FP(t, 0x38) = n; FW(t, 0x6C) = 0;  } \
    P = t; }
#define NEWPANEL(P, D, NM, VT) { w = ALLOC(0x2BC); \
    if (w) { _wp = w; if (_wp || (_wp = ALLOC(0x2B8))) { \
        timproc_uso_b5_func_04C678(_wp, D); FP(_wp, 0x28) = &timproc_uso_b5_D_807FDB64; \
        timproc_uso_b5_func_00002B74(_wp + 0x2C); timproc_uso_b5_func_000032C8(_wp + 0x194); } \
      FP(w, 0x28) = (VT); FP(w, 0xC) = NM; timproc_uso_b5_func_00003890(w); } \
    P = w; }
#define W16C(P, D, F0) { _wp = ALLOC(0x16C); \
    if (_wp) { timproc_uso_b5_func_0546DC(_wp, D, 0.0f, 0.0f, 0.0f); fv = F0; FP(_wp, 0x28) = import_80087FB8; FW(_wp, 0x120) = 1; \
      FF(_wp, 0x108) = fv; FF(_wp, 0x10C) = fv; FF(_wp, 0x110) = fv; FF(_wp, 0x124) = 1.0f; } \
    P = _wp; }
/* three DIFFERENT spellings of 0: cfe passes float literals to uopt as text, so each spelling is its own
 * constant candidate with one use per site (never coloured: three ugen mtc1 zero, as in the target) */
#define SETI(P, V) { P = (V) + 0xB4; if (1) {} FF(P, 0) = 0.f; if (1) {} FF(P, 4) = .0f; if (1) {} FF(P, 8) = 0.00f; }
#define SETE(P, V) { P = (V); if (1) {} FF(P, 0xB4) = 0.0; if (1) {} FF(P, 0xB8) = 0.0; if (1) {} FF(P, 0xBC) = 0.0; }
#define ATTACH(P, W) {  int _i;  for (_i = 0; _i < FW(W, 0x13C); _i++) { timproc_uso_b5_func_04DFFC(W, FP(W, 0x148 + _i * 4));  { P = FP(W, 0x148 + _i * 4) + 0x18; *(int *)P &= ~4; }  } }
#define ATTACH1(P, W) { for (i1 = 0; i1 < FW(W, 0x13C); i1++) { timproc_uso_b5_func_04DFFC(W, FP(W, 0x148 + i1 * 4)); { P = FP(W, 0x148 + i1 * 4) + 0x18; *(int *)P &= ~4; } } }
#define RESTORE(SB) if (SB) { do {   _x = FW(&import_8005EE6C, 4); _x |= 0x80000; FW(&import_8005EE6C, 4) = _x; } while (0); } else { do {   _x = FW(&import_8005EE6C, 4); _x &= 0xFFF7FFFF; FW(&import_8005EE6C, 4) = _x; } while (0); }
#define WTAIL(W, F0, F1, F2, F3, F4, F5, F6) FF(W, 0xDC) = F0; FF(W, 0xE0) = 720.0f; FF(W, 0xE4) = -220.0f; \
    FF(W, 0x10C) = F1; FF(W, 0x110) = F2; FF(W, 0x114) = F3; FF(W, 0x11C) = 0.0f; FW(W, 0x134) = 0; FF(W, 0x120) = F4; FF(W, 0x124) = 1.0f; FF(W, 0x12C) = F5; FF(W, 0x128) = F6;
typedef void *(*AddFn)(void *, s32, s32, f32, f32, f32, f32, f32, f32);
#define VF(R) (*(AddFn *)(FP(R, 0x28) + 0x5C))
#define VT(R) (*(s16 *)(FP(R, 0x28) + 0x58) + (R))
/* virtual call through a held vtable pointer V (the target keeps it in s1 / a register local) */
#define VSEL(V, R) V = FP(R, 0x28);
#define VCALL(V, R) (*(AddFn *)(V + 0x5C))(*(s16 *)(V + 0x58) + (R)
#define self ((char *)arg0)
void **timproc_uso_b5_func_00008FC8(void **arg0, int *arg1, int *arg2) {
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
    volatile int sb3;
    if (arg0 || (arg0 = (void **)ALLOC(0x60))) {
        timproc_uso_b5_func_04C678(self, timproc_uso_b5_D_807FEBB8 + 0x1348);
        FP(self, 0x28) = timproc_uso_b5_D_807FE5F4;
        FP(self, 0xC) = timproc_uso_b5_D_807FE5F4 + 0x1350;
        { char *p = ALLOC(0x30); w = p; if (w) { timproc_uso_b5_func_04C678(w, timproc_uso_b5_D_807FEBCC + 0x135C); FP(w, 0x28) = timproc_uso_b5_D_807FEBD4; FP(w, 0xC) = timproc_uso_b5_D_807FEBD4 + 0x1364; FP(w, 0x2C) = self; } FP(self, 0x50) = w; }
        ADD(self, FP(self, 0x50));
        timproc_uso_b5_func_0000A928(self);
        { char *p = ALLOC(0x2C); char *q; if (p) { timproc_uso_b5_func_04C678(p, timproc_uso_b5_D_807FEBE0 + 0x1370);
            FP(p, 0x28) = timproc_uso_b5_D_807FEBE8 + 0xCC4; FP(p, 0xC) = timproc_uso_b5_D_807FEBE8 + 0x1378; }
          q = FP(self, 0x30); FP(self, 0x38) = p;  ADD(q, p); }
        if (FW(&import_800200CC, 0x34) != 2) {
            NEWPANEL(FP(self, 0x5C), timproc_uso_b5_D_807FEBF4 + 0x1384, timproc_uso_b5_D_807FEBFC + 0x138C, timproc_uso_b5_D_807FEBFC + 0x8B4)
            ADD(self, FP(self, 0x5C));
            w = ALLOC(0x168);
            if (w) {
                timproc_uso_b5_func_0546DC(w, timproc_uso_b5_D_807FEC08 + 0x1398, 0.0f, 0.0f, 0.0f);
                FP(w, 0x28) = timproc_uso_b5_D_807FEC10 + 0x4AC; FP(w, 0xC) = timproc_uso_b5_D_807FEC10 + 0x13A0;
                W16C(FP(w, 0x108), timproc_uso_b5_D_807FEC18 + 0x13A8, FF(&import_80807FB8, 0x23C))
                timproc_uso_b5_func_04DFFC(w, FP(w, 0x108));
                SETI(t, FP(w, 0x108))
                FF(w, 0x124) = 1.0f;
                sb1 = FW(f8fc8_ee6c_sb1, 4) & 0x80000; { _x = FW(&import_8005EE6C, 4); _x &= 0xFFF7FFFF; _x &= ~1; _x &= 0xFFFDFFFF; _x |= 0x2000; _x &= ~2; FW(import_800664C4, 4) = _x; } FP(w, 0x148) = timproc_uso_b5_func_05D0E0(0, import_808064C4 + 0x13C0, import_807664C4, 0), FP(w, 0x14C) = timproc_uso_b5_func_05D0E0(0, timproc_uso_b5_D_807F8434 + 0x13CC, import_80768434, 0), FP(w, 0x150) = timproc_uso_b5_func_05D0E0(0, timproc_uso_b5_D_807FA3A4 + 0x13D8, import_8076A3A4, 0), FP(w, 0x154) = timproc_uso_b5_func_05D0E0(0, timproc_uso_b5_D_807FC314 + 0x13E4, import_8076C314, 0), FP(w, 0x158) = timproc_uso_b5_func_05D0E0(0, timproc_uso_b5_D_807FE284 + 0x13F0, import_8076E284, 0), FP(w, 0x15C) = timproc_uso_b5_func_05D0E0(0, import_808001F4 + 0x13FC, import_807701F4, 0), FP(w, 0x160) = timproc_uso_b5_func_05D0E0(0, import_80802164 + 0x1408, import_80772164, 0), FP(w, 0x164) = timproc_uso_b5_func_05D0E0(0, import_808040D4 + 0x1414, import_807740D4, 0); FW(w, 0x13C) = 8; if (1) {}
                FW(w, 0x140) = 0;
                ATTACH1(t, w)
                FW(w, 0x144) = 0;
                RESTORE(sb1)
                WTAIL(w, DF_244, DF_248, DF_24C, DF_250, DF_254, DF_258, DF_25C)
            }
            FP(FP(self, 0x5C), 0x2B8) = w;
            v1a.f0 = 110.0f; v1a.f4 = -96.0f; v1a.f8 = -510.0f;
            { n = FP(self, 0x5C) + 0x22C; *(VecH *)&t1 = v1a; *(VecH *)&t2 = *(VecH *)&t1; FF(n, 0x10) = ((VecH *)&t2)->f0; FF(n, 0x14) = ((VecH *)&t2)->f4; FF(n, 0x18) = ((VecH *)&t2)->f8; }
            v1b.f0 = *(f32 *)&DS_260; v1b.f4 = 0.0f; v1b.f8 = 0.0f;
            { n = FP(self, 0x5C) + 0x24C; *(VecH *)&t1 = v1b; *(VecH *)&t2 = *(VecH *)&t1; FF(n, 0x10) = ((VecH *)&t2)->f0; FF(n, 0x14) = ((VecH *)&t2)->f4; FF(n, 0x18) = ((VecH *)&t2)->f8; }
            { w = FP(self, 0x5C) + 0x284; if (1) {} FF(w, 0x10) = DF_264; }
            { w = FP(FP(self, 0x5C), 0x2B8); FP(FP(self, 0x5C), 0x29C) = w; }
            FF(FP(FP(self, 0x5C), 0x2B8), 0x12C) = DF_268;
            FF(FP(FP(self, 0x5C), 0x2B8), 0x128) = DF_26C;
            ADDV(t, FP(self, 0x38), FP(FP(self, 0x5C), 0x2B8));
        }
        NEWS70(a, timproc_uso_b5_D_807FEC90 + 0x1420, f8fc8_e304_1 + 0xA94, timproc_uso_b5_D_807FEC98 + 0xB04, timproc_uso_b5_D_807FEC98 + 0x1428, import_80020098_a)
        timproc_uso_b5_func_0000A95C(self, a);
        ADDW(_at, t, self, a);
        NEWS70(b, timproc_uso_b5_D_807FECA8 + 0x1438, f8fc8_e304_2 + 0xA94, timproc_uso_b5_D_807FECB0 + 0xB74, timproc_uso_b5_D_807FECB0 + 0x1440, import_80020098_b)
        timproc_uso_b5_func_0000A95C(self, b);
        ADDW(_at, t, self, b);
        NEWS70(c, timproc_uso_b5_D_807FECBC + 0x144C, f8fc8_e304_3 + 0xA94, timproc_uso_b5_D_807FECC4 + 0xBE4, timproc_uso_b5_D_807FECC4 + 0x1454, import_80020098_c)
        timproc_uso_b5_func_0000A95C(self, c);
        ADDW(_at, t, self, c);
        NEWS70(d, timproc_uso_b5_D_807FECD0 + 0x1460, f8fc8_e304_4 + 0xA94, timproc_uso_b5_D_807FECD8 + 0xC54, timproc_uso_b5_D_807FECD8 + 0x1468, import_80020098_d)
        timproc_uso_b5_func_0000A95C(self, d);
        ADDW(_at, t, self, d);
        {
            NEWPANEL(FP(self, 0x54), timproc_uso_b5_D_807FECE4 + 0x1474, timproc_uso_b5_D_807FECEC + 0x147C, timproc_uso_b5_D_807FECEC + 0x8B4)
            ADD(self, FP(self, 0x54));
            w = ALLOC(0x168);
            if (w) {
                timproc_uso_b5_func_0546DC(w, timproc_uso_b5_D_807FECF8 + 0x1488, 0.0f, 0.0f, 0.0f);
                FP(w, 0x28) = timproc_uso_b5_D_807FED00 + 0x4AC; FP(w, 0xC) = timproc_uso_b5_D_807FED00 + 0x1490;
                W16C(FP(w, 0x108), timproc_uso_b5_D_807FED08 + 0x1498, FF(&import_80807FB8, 0x270))
                timproc_uso_b5_func_04DFFC(w, FP(w, 0x108));
                SETI(u, FP(w, 0x108))
                FF(w, 0x124) = 1.0f;
                sb2 = FW(f8fc8_ee6c_sb2, 4) & 0x80000; { _x = (FW(&import_8005EE6C, 4) & 0xFFF7FFFF & ~1 & 0xFFFDFFFF) | 0x2000; if (1) { _x |= 2; } FW(import_80063D04, 4) = _x; } FP(w, 0x148) = timproc_uso_b5_func_05D0E0(0, import_80803D04 + 0x14A4, timproc_uso_b5_D_807E3D04, 0); FW(w, 0x13C) = 1; if (1) {}
                FW(w, 0x140) = 0;
                ATTACH(t, w)
                FW(w, 0x144) = 0;
                RESTORE(sb2)
                WTAIL(w, DF_278, DF_27C, DF_280, DF_284, DF_288, DF_28C, DF_290)
            }
            FP(FP(self, 0x54), 0x2B8) = w;
            v2a.f0 = *(f32 *)&DS_294; v2a.f4 = 528.0f; v2a.f8 = 180.0f;
            { n = FP(self, 0x54) + 0x22C; *(VecH *)&t1 = v2a; *(VecH *)&t2 = *(VecH *)&t1; FF(n, 0x10) = ((VecH *)&t2)->f0; FF(n, 0x14) = ((VecH *)&t2)->f4; FF(n, 0x18) = ((VecH *)&t2)->f8; }
            v2b.f0 = *(f32 *)&DS_298; v2b.f4 = DF_29C; v2b.f8 = DF_2A0;
            { n = FP(self, 0x54) + 0x24C; *(VecH *)&t1 = v2b; *(VecH *)&t2 = *(VecH *)&t1; FF(n, 0x10) = ((VecH *)&t2)->f0; FF(n, 0x14) = ((VecH *)&t2)->f4; FF(n, 0x18) = ((VecH *)&t2)->f8; }
            { w = FP(self, 0x54) + 0x284; if (1) {} FF(w, 0x10) = DF_2A4; }
            { w = FP(FP(self, 0x54), 0x2B8); FP(FP(self, 0x54), 0x29C) = w; }
            FF(FP(FP(self, 0x54), 0x2B8), 0x12C) = DF_2A8;
            FF(FP(FP(self, 0x54), 0x2B8), 0x128) = DF_2AC;
            ADDV(t, FP(self, 0x38), FP(FP(self, 0x54), 0x2B8));
        }
        {
            NEWPANEL(FP(self, 0x58), timproc_uso_b5_D_807FED80 + 0x1510, timproc_uso_b5_D_807FED88 + 0x1518, timproc_uso_b5_D_807FED88 + 0x8B4)
            ADD(self, FP(self, 0x58));
            w = ALLOC(0x168);
            if (w) {
                timproc_uso_b5_func_0546DC(w, timproc_uso_b5_D_807FED94 + 0x1524, 0.0f, 0.0f, 0.0f);
                FP(w, 0x28) = timproc_uso_b5_D_807FED9C + 0x4AC; FP(w, 0xC) = timproc_uso_b5_D_807FED9C + 0x152C;
                { _wp = ALLOC(0x16C);
                  if (_wp) { timproc_uso_b5_func_0546DC(_wp, timproc_uso_b5_D_807FEDA4 + 0x1534, 0.0f, 0.0f, 0.0f); FP(_wp, 0x28) = import_80087FB8;
                    FF(_wp, 0x108) = DF_2B0; FF(_wp, 0x10C) = DF_2B4; FF(_wp, 0x110) = FF(timproc_uso_b5_D_807FEE6C, 0x2B8); FW(_wp, 0x120) = 1; FF(_wp, 0x124) = 1.0f; }
                  FP(w, 0x108) = _wp; }
                timproc_uso_b5_func_04DFFC(w, FP(w, 0x108));
                SETI(u, FP(w, 0x108))
                FF(w, 0x124) = 1.0f;
                sb3 = FW(f8fc8_ee6c_sb3, 4) & 0x80000; { _x = (FW(&import_8005EE6C, 4) & 0xFFF7FFFF & ~1 & 0xFFFDFFFF) | 0x2000; if (1) { _x |= 2; } FW(import_80063D04, 4) = _x; } FP(w, 0x148) = timproc_uso_b5_func_05D0E0(0, import_80803D04 + 0x1540, f8fc8_e3d04_2, 0); FW(w, 0x13C) = 1; if (1) {}
                FW(w, 0x140) = 0;
                ATTACH(t, w)
                FW(w, 0x144) = 0;
                if (sb3) { do { _x = FW(&import_8005EE6C, 4); _x |= 0x80000; FW(&f8fc8_ee6c_r3, 4) = _x; } while (0); } else { do { _x = FW(&import_8005EE6C, 4); _x &= 0xFFF7FFFF; FW(&import_8005EE6C, 4) = _x; } while (0); }
                WTAIL(w, DF_2BC, DF_2C0, DF_2C4, DF_2C8, DF_2CC, DF_2D0, DF_2D4)
            }
            FP(FP(self, 0x58), 0x2B8) = w;
            v3a.f0 = *(f32 *)&DS_2D8; v3a.f4 = 528.0f; v3a.f8 = -480.0f;
            { n = FP(self, 0x58) + 0x22C; *(VecH *)&t1 = v3a; *(VecH *)&t2 = *(VecH *)&t1; FF(n, 0x10) = ((VecH *)&t2)->f0; FF(n, 0x14) = ((VecH *)&t2)->f4; FF(n, 0x18) = ((VecH *)&t2)->f8; }
            v3b.f0 = *(f32 *)&DS_2DC; v3b.f4 = DF_2E0; v3b.f8 = DF_2E4;
            { n = FP(self, 0x58) + 0x24C; *(VecH *)&t1 = v3b; *(VecH *)&t2 = *(VecH *)&t1; FF(n, 0x10) = ((VecH *)&t2)->f0; FF(n, 0x14) = ((VecH *)&t2)->f4; FF(n, 0x18) = ((VecH *)&t2)->f8; }
            { u = FP(self, 0x58) + 0x284; if (1) {} FF(u, 0x10) = DF_2E8; }
            { w = FP(FP(self, 0x58), 0x2B8); FP(FP(self, 0x58), 0x29C) = w; }
            FF(FP(FP(self, 0x58), 0x2B8), 0x12C) = DF_2EC;
            FF(FP(FP(self, 0x58), 0x2B8), 0x128) = 1.0f;
            ADDV(t, FP(self, 0x38), FP(FP(self, 0x58), 0x2B8));
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
            tu = **(u16 **)((char *)&import_800201EC + 0x154);
            if (FW(&import_800200CC, 0x34) != 6) tu &= ~3;
            for (i = 0; i != 8; i++) {
                char *r;
                VSEL(w, c) r = VCALL(w, c), 1, i, 400.0f, 210.0f, -280.0f, 0.0f, 0.0f, FF(import_808000CC, 0x320));
                items[i] = r;
                if (!(tu & (1U << i))) FF(items[i], 0x2A4) = 0.0f;
            }
            VSEL(w, d) VCALL(w, d), 1, 1, -220.0f, 200.0f, -400.0f, 0.0f, 0.0f, 0.0f);
            VSEL(w, d) e2 = VCALL(w, d), 1, 2, -220.0f, 200.0f, -400.0f, 0.0f, 0.0f, 0.0f);
            VSEL(w, d) e3 = VCALL(w, d), 1, 3, -220.0f, 200.0f, -400.0f, 0.0f, 0.0f, 0.0f);
            timproc_uso_b5_func_048E7C(timproc_uso_b5_D_807FEE1C + 0x15AC, (*(u16 **)((char *)&import_800201EC + 0x154))[1]);
            switch ((*(u16 **)((char *)&import_800201EC + 0x154))[1]) {
            case 1:
                 FF(e2, 0x2A4) = 0.0f; FF(e3, 0x2A4) = 0.0f; break;
            case 2:
                FF(e3, 0x2A4) = 0.0f;
                break;
            case 3: case 4: case 5:
                break;
            }
        }
        timproc_uso_b5_func_04C774(self, 1, arg1);
        timproc_uso_b5_func_04C774(FP(self, 0x50), 1, arg2);
        w = FP(self, 0x28); (*(void (**)(void *))(w + 0x24))(*(s16 *)(w + 0x20) + self);
        { char *m = FP(self, 0x38); w = FP(m, 0x28); (*(void (**)(void *))(w + 0x24))(*(s16 *)(w + 0x20) + m); }
    }
    return arg0;
}
