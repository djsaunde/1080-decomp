/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for timproc_uso_b5_func_00008FC8 (0x1960 / 1624 words):
 * the timing-screen master constructor Screen::Screen(int *, int *) (agent-g run 169, cfe C body 86.64).
 * Classes: B (out-of-line base ctor 04C678, vptr +0x28, inline add() = 07ACE0 + parent link), Sub30 / Sub2C,
 * the 0x2BC Panel over the 0x2B8 M2B8 base (members 2B74 / 32C8 out of line), the 0x168 WA / WB / WC widget
 * ctors over the out-of-line B2 base (0546DC) with an inline W16C child, Kid (05D0E0, cfront ctor(0, ...)),
 * the four 0x70 S70 rows with a virtual add() at vtable +0x58.  FP "D + off" loads are source literals
 * (expected RoData 0x23C..0x320: 1e7f, -1030f, +-1.5457f, ...).  The Sub2C and first Panel allocations are
 * written as explicit alloc / test / init sequences: edg pools every new-expression's temporary into one
 * function-scope variable whose long live range takes s0 / s1, the target (cfe on cfront output) colours
 * each site's temporary separately.  NOT matchable under NCC: the target's 5-entry tail switch is a
 * jumptable (cfe; NCC always lowers a switch to a compare chain).  Symbols renamed after compile (names
 * only): __ct__6ScreenFPiT1 -> timproc_uso_b5_func_00008FC8.  NM comparison object only.
 * 87.71 -> 88.08: the item loop stores items[i] (no pp walk), the row pointers / m38 copy declared at the top
 * (frame 368 -> 352 = target), direct `int *fp` for the kid flag RMW, third Panel explicit.
 * 88.03 -> 88.9: the WA flag RMW as one block-scope int x (and/and/and/ori/and on v0, as the target) and the three saved-bit locals volatile (the target spills them across the kid calls).
 * 88.9 -> 89.6: W-ctor field stores ordered f10C, f110, f114, f11C, i134 (store-order permutation sweep).
 * 89.6 -> 90.36: W16C stores f108/f10C/f110 before i120, S70 row stores f34 before f6C (permutation sweeps). */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef float f32; typedef double f64;
#define M2C_FIELD(ptr, type, off) (*(type)((char *)(ptr) + (off)))
extern "C" {
extern char import_800200CC, import_80020098;
char *timproc_uso_b5_func_00002B74(...);
char *timproc_uso_b5_func_000032C8(...);
char *timproc_uso_b5_func_00003890(...);
extern char timproc_uso_b5_D_807FDB64;
extern char import_8005EE6C;
extern char import_80807FB8;
extern char import_800201EC;
char *timproc_uso_b5_func_055750(...);
char *timproc_uso_b5_func_04C678(...);
char *timproc_uso_b5_func_07ACE0(...);
char *timproc_uso_b5_func_04DFFC(...);
char *timproc_uso_b5_func_05D0E0(...);
char *timproc_uso_b5_func_04C774(...);
char *timproc_uso_b5_func_048E7C(...);
char *timproc_uso_b5_func_0546DC(char *, char *, f32, f32, f32);
char *timproc_uso_b5_func_0000A95C(...);
void timproc_uso_b5_func_0000A928(...);
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
typedef struct { f32 f0; f32 f4; f32 f8; } VecH;

}
#define FW(p, o) (*(int *)((char *)(p) + (o)))
#define FF(p, o) (*(float *)((char *)(p) + (o)))
inline void *operator new(unsigned n) { return timproc_uso_b5_func_055750(n); }
struct V3 { f32 x, y, z; V3() {} V3(f32 a, f32 b, f32 c) { x = a; y = b; z = c; } void set(f32 a, f32 b, f32 c) { x = a; y = b; z = c; } void operator=(V3 v) { x = v.x; y = v.y; z = v.z; } };
struct Node { char p[0x10]; V3 pos; void setPos(V3 v) { pos = v; } };
struct Fl { int v; void clr(int m) { v &= ~m; } };
struct B { int f0; int flag; int f8; char *name; char list[4]; B *parent; Fl fl; char p1c[0x28 - 0x1C];
    B(char *); virtual void f1(); virtual void f2(); virtual void f3(); virtual void f4();
    void setup(int, int *);
    void add(B *n) { timproc_uso_b5_func_07ACE0(list, n); if (n->parent) n->flag = 1; n->parent = this; } };
struct Sub30 : B { void *owner; Sub30(void *o) : B(timproc_uso_b5_D_807FEBCC + 0x135C) { name = timproc_uso_b5_D_807FEBD4 + 0x1364; owner = o; } virtual void v(); };
struct Sub2C : B { Sub2C() : B(timproc_uso_b5_D_807FEBE0 + 0x1370) { name = timproc_uso_b5_D_807FEBE8 + 0x1378; } void init() { timproc_uso_b5_func_04C678(this, timproc_uso_b5_D_807FEBE0 + 0x1370); FW(this, 0x28) = (int)(timproc_uso_b5_D_807FEBE8 + 0xCC4); name = timproc_uso_b5_D_807FEBE8 + 0x1378; } virtual void v(); };
struct S70 : B { int f2C, f30, f34; B *f38; char p3c[0x6C - 0x3C]; int f6C;
    S70(char *d, B *o) : B(d) { f38 = o; }
    virtual void f5(); virtual void f6(); virtual void f7(); virtual void f8(); virtual void f9(); virtual void f10();
    virtual B *add(int, int, f32, f32, f32, f32, f32, f32); };
#define CFG (*(char **)((char *)&import_80020098 + 0x134))
#define NEWS70(P, D, VT, NM) { B *o = m38; S70 *np = (S70 *)timproc_uso_b5_func_055750(0x70); \
    if (np) { S70 *bp = np; if (bp || (bp = (S70 *)timproc_uso_b5_func_055750(0x70))) { \
        timproc_uso_b5_func_04C678(bp, D); FW(bp, 0x28) = (int)(f8fc8_e304_1 + 0xA94); } \
      FW(np, 0x28) = (int)(VT); np->name = NM; np->f2C = FW(CFG, 0x84); np->f30 = FW(CFG, 0x80); np->f6C = 0; np->f38 = o; np->f34 = FW(CFG, 0x8C); } \
    P = np; }
#define S70CLASS(N, D, NM) struct N : S70 { N(B *o) : S70(D, o) { name = NM; f2C = FW(CFG, 0x84); f30 = FW(CFG, 0x80); f34 = FW(CFG, 0x8C); f6C = 0; } \
    virtual void f5(); virtual void f6(); virtual void f7(); virtual void f8(); virtual void f9(); virtual void f10(); virtual B *add(int, int, f32, f32, f32, f32, f32, f32); };
S70CLASS(S70a, timproc_uso_b5_D_807FEC90 + 0x1420, timproc_uso_b5_D_807FEC98 + 0x1428)
S70CLASS(S70b, timproc_uso_b5_D_807FECA8 + 0x1438, timproc_uso_b5_D_807FECB0 + 0x1440)
S70CLASS(S70c, timproc_uso_b5_D_807FECBC + 0x144C, timproc_uso_b5_D_807FECC4 + 0x1454)
S70CLASS(S70d, timproc_uso_b5_D_807FECD0 + 0x1460, timproc_uso_b5_D_807FECD8 + 0x1468)
struct Kid : B { Kid(char *, char *, int); virtual void v(); };
struct B2 : B { char p2c[0xB4 - 0x2C]; V3 pos; char pc0[0xDC - 0xC0]; f32 fDC, fE0, fE4; char pe8[0x108 - 0xE8];
    B2(char *, f32, f32, f32); void addChild(B *); virtual void v(); };
struct W16C : B2 { f32 f108, f10C, f110; char p114[0x120 - 0x114]; int i120; f32 f124; char p128[0x16C - 0x128];
    W16C(char *d) : B2(d, 0.0f, 0.0f, 0.0f) { f108 = 10000000.0f; f10C = 10000000.0f; f110 = 10000000.0f; i120 = 1; f124 = 1.0f; } virtual void v(); };
#define WFIELDS W16C *w; f32 f10C, f110, f114; int p118; f32 f11C, f120, f124, f128, f12C; int p130; int i134; int p138; int n; int i140; int i144; Kid *kid[8]; \
    void attachKids() { for (int i = 0; i < n; i++) { addChild(kid[i]); int *fp = &kid[i]->fl.v; *fp &= ~4; } }
struct WBase : B2 { WFIELDS WBase(); virtual void v(); };
extern "C" char import_80087FB8[];
struct WA : B2 { WFIELDS
    WA() : B2(timproc_uso_b5_D_807FEC08 + 0x1398, 0.0f, 0.0f, 0.0f) {
        name = timproc_uso_b5_D_807FEC10 + 0x13A0;
        w = new W16C(timproc_uso_b5_D_807FEC18 + 0x13A8);
        addChild(w);
        w->pos.set(0.0, 0.0, 0.0);
        f124 = 1.0f;
        volatile int sb = FW(f8fc8_ee6c_sb1, 4) & 0x80000;
        { int x = FW(&import_8005EE6C, 4); x &= 0xFFF7FFFF; x &= ~1; x &= 0xFFFDFFFF; x |= 0x2000; x &= ~2; FW(import_800664C4, 4) = x; }
        kid[0] = new Kid(import_808064C4 + 0x13C0, import_807664C4, 0);
        kid[1] = new Kid(timproc_uso_b5_D_807F8434 + 0x13CC, import_80768434, 0);
        kid[2] = new Kid(timproc_uso_b5_D_807FA3A4 + 0x13D8, import_8076A3A4, 0);
        kid[3] = new Kid(timproc_uso_b5_D_807FC314 + 0x13E4, import_8076C314, 0);
        kid[4] = new Kid(timproc_uso_b5_D_807FE284 + 0x13F0, import_8076E284, 0);
        kid[5] = new Kid(import_808001F4 + 0x13FC, import_807701F4, 0);
        kid[6] = new Kid(import_80802164 + 0x1408, import_80772164, 0);
        kid[7] = new Kid(import_808040D4 + 0x1414, import_807740D4, 0);
        n = 8;
        i140 = 0;
        attachKids();
        i144 = 0;
        if (sb) FW(&import_8005EE6C, 4) |= 0x80000; else FW(&import_8005EE6C, 4) &= 0xFFF7FFFF;
        fDC = -1030.0f; fE0 = 720.0f; fE4 = -220.0f;
        f10C = -1.5457f; f110 = 1.5457f; f114 = -0.0251f; f11C = 0.0f; i134 = 0; f120 = 0.28f; f124 = 1.0f; f12C = 0.764f; f128 = 0.972f;
    } virtual void v(); };
struct WB : B2 { WFIELDS
    WB() : B2(timproc_uso_b5_D_807FECF8 + 0x1488, 0.0f, 0.0f, 0.0f) {
        name = timproc_uso_b5_D_807FED00 + 0x1490;
        w = new W16C(timproc_uso_b5_D_807FED08 + 0x1498);
        addChild(w);
        w->pos.set(0.0, 0.0, 0.0);
        f124 = 1.0f;
        volatile int sb = FW(f8fc8_ee6c_sb2, 4) & 0x80000;
        { int fl = (FW(&import_8005EE6C, 4) & 0xFFF7FFFF & ~1 & 0xFFFDFFFF) | 0x2000; if (1) { fl |= 2; } FW(import_80063D04, 4) = fl; }
        kid[0] = new Kid(import_80803D04 + 0x14A4, timproc_uso_b5_D_807E3D04, 0);
        n = 1;
        i140 = 0;
        attachKids();
        i144 = 0;
        if (sb) FW(&import_8005EE6C, 4) |= 0x80000; else FW(&import_8005EE6C, 4) &= 0xFFF7FFFF;
        fDC = -1030.0f; fE0 = 720.0f; fE4 = -220.0f;
        f10C = -1.5457f; f110 = 1.5457f; f114 = -0.0251f; f11C = 0.0f; i134 = 0; f120 = 0.28f; f124 = 1.0f; f12C = 0.764f; f128 = 0.972f;
    } virtual void v(); };
struct WC : B2 { WFIELDS
    WC() : B2(timproc_uso_b5_D_807FED94 + 0x1524, 0.0f, 0.0f, 0.0f) {
        name = timproc_uso_b5_D_807FED9C + 0x152C;
        w = new W16C(timproc_uso_b5_D_807FEDA4 + 0x1534);
        addChild(w);
        w->pos.set(0.0, 0.0, 0.0);
        f124 = 1.0f;
        volatile int sb = FW(f8fc8_ee6c_sb3, 4) & 0x80000;
        { int fl = (FW(&import_8005EE6C, 4) & 0xFFF7FFFF & ~1 & 0xFFFDFFFF) | 0x2000; if (1) { fl |= 2; } FW(import_80063D04, 4) = fl; }
        kid[0] = new Kid(import_80803D04 + 0x1540, f8fc8_e3d04_2, 0);
        n = 1;
        i140 = 0;
        attachKids();
        i144 = 0;
        if (sb) FW(&import_8005EE6C, 4) |= 0x80000; else FW(&import_8005EE6C, 4) &= 0xFFF7FFFF;
        fDC = -1030.0f; fE0 = 720.0f; fE4 = -220.0f;
        f10C = -1.5457f; f110 = 1.5457f; f114 = -0.0251f; f11C = 0.0f; i134 = 0; f120 = 0.28f; f124 = 1.0f; f12C = 0.764f; f128 = 0.972f;
    } virtual void v(); };
struct C2B74 { char p[0x194 - 0x2C]; C2B74(); };
struct NodeF { char p[0x10]; f32 f; };
struct C32C8 { char p[0x22C - 0x194]; Node n22C; char p248[0x24C - 0x248]; Node n24C; char p268[0x284 - 0x268]; NodeF n284; char p298[0x29C - 0x298]; B2 *m29C; char p2a0[0x2B8 - 0x2A0]; C32C8(); };
struct M2B8 : B { C2B74 a; C32C8 b; M2B8(char *d) : B(d) {} virtual void v(); };
struct Panel : M2B8 { WBase *w; Panel(char *d, char *n) : M2B8(d) { name = n; init(); } void init(); virtual void v(); };
extern "C" char *timproc_uso_b5_func_00002B74(...);
#define NEWPANEL(P, D, NM) { Panel *np = (Panel *)timproc_uso_b5_func_055750(0x2BC); \
    if (np) { M2B8 *bp = np; if (bp || (bp = (M2B8 *)timproc_uso_b5_func_055750(0x2B8))) { \
        timproc_uso_b5_func_04C678(bp, D); FW(bp, 0x28) = (int)&timproc_uso_b5_D_807FDB64; \
        timproc_uso_b5_func_00002B74((char *)bp + 0x2C); timproc_uso_b5_func_000032C8((char *)bp + 0x194); } \
      FW(np, 0x28) = (int)(NM - 0x138C + 0x8B4); np->name = NM; np->init(); } \
    P = np; }
struct Screen : B { int f2c; B *m30; int f34; B *m38; char p3c[0x50 - 0x3C]; B *m50; Panel *m54; Panel *m58; Panel *m5C;
    Screen(int *arg1, int *arg2); void reg(S70 *); virtual void f1(); };
Screen::Screen(int *arg1, int *arg2) : B(timproc_uso_b5_D_807FEBB8 + 0x1348) {
    S70 *a, *b, *c, *d; B *o;
    volatile int pad[4];
    name = timproc_uso_b5_D_807FE5F4 + 0x1350;
    m50 = new Sub30(this);
    add(m50);
    timproc_uso_b5_func_0000A928(this);
    { Sub2C *p = (Sub2C *)timproc_uso_b5_func_055750(0x2C); if (p) { p->init(); } B *q = m30; m38 = p; q->add(p); }

    if (FW(&import_800200CC, 0x34) != 2) {
        NEWPANEL(m5C, timproc_uso_b5_D_807FEBF4 + 0x1384, timproc_uso_b5_D_807FEBFC + 0x138C)
        add(m5C);
        m5C->w = (WBase *)new WA;
        m5C->b.n22C.setPos(V3(110.0f, -96.0f, -510.0f));
        m5C->b.n24C.setPos(V3(0.0377f, 0.0f, 0.0f));
        m5C->b.n284.f = 1.936f;
        m5C->b.m29C = m5C->w;
        m5C->w->f12C = 0.296f;
        m5C->w->f128 = 0.496f;
        m38->add(m5C->w);
    }

    o = m38;
    a = new S70a(o);
    reg(a);
    add(a);
    o = m38;
    b = new S70b(o);
    reg(b);
    add(b);
    o = m38;
    c = new S70c(o);
    reg(c);
    add(c);
    o = m38;
    d = new S70d(o);
    reg(d);
    add(d);
    m54 = new Panel(timproc_uso_b5_D_807FECE4 + 0x1474, timproc_uso_b5_D_807FECEC + 0x147C);
    add(m54);
    m54->w = (WBase *)new WB;
    m54->b.n22C.setPos(V3(-750.0f, 528.0f, 180.0f));
    m54->b.n24C.setPos(V3(-1.5582f, 1.5457f, -0.0251f));
    m54->b.n284.f = 0.728f;
    m54->b.m29C = m54->w;
    m54->w->f12C = 0.288f;
    m54->w->f128 = 0.972f;
    m38->add(m54->w);
    m58 = new Panel(timproc_uso_b5_D_807FED80 + 0x1510, timproc_uso_b5_D_807FED88 + 0x1518);
    add(m58);
    m58->w = (WBase *)new WC;
    m58->b.n22C.setPos(V3(-750.0f, 528.0f, -480.0f));
    m58->b.n24C.setPos(V3(-1.5582f, 1.5457f, -0.0251f));
    m58->b.n284.f = 0.728f;
    m58->b.m29C = m58->w;
    m58->w->f12C = 0.34f;
    m58->w->f128 = 1.0f;
    m38->add(m58->w);
    a->add(1, 5, 0.0f, -150.0f, -1000.0f, 0.0f, 0.0f, 0.0f);
    a->add(1, 3, 750.0f, -150.0f, -375.0f, 0.0f, 0.0f, 0.0f);
    a->add(1, 4, 750.0f, -150.0f, 375.0f, 0.0f, 0.0f, 0.0f);
    a->add(1, 2, 750.0f, -150.0f, 375.0f, 0.0f, 0.0f, 0.0f);
    a->add(1, 1, 750.0f, -150.0f, 375.0f, 0.0f, 0.0f, 0.0f);
    b->add(1, 0, 400.0f, -150.0f, -1000.0f, 0.0f, 2.89f, 0.0f);
    b->add(1, 1, 750.0f, 200.0f, -375.0f, 0.0f, 0.0f, 0.0f);
    b->add(1, 2, 750.0f, 200.0f, 375.0f, 0.0f, 0.0f, 0.0f);
    b->add(1, 3, 400.0f, -150.0f, -1000.0f, 0.0f, 2.89f, 0.0f);
    b->add(1, 4, 400.0f, -150.0f, -1000.0f, 0.0f, 2.89f, 0.0f);
    b->add(1, 5, 400.0f, -150.0f, -1000.0f, 0.0f, 2.89f, 0.0f);
    b->add(1, 6, 400.0f, -150.0f, -1000.0f, 0.0f, 2.89f, 0.0f);
    b->add(1, 7, 400.0f, -150.0f, -1000.0f, 0.0f, 2.89f, 0.0f);
    int tu = **(u16 **)((char *)&import_800201EC + 0x154);
    if (FW(&import_800200CC, 0x34) != 6) tu &= ~3;
    B *items[8];
    for (int i = 0; i != 8; i++) {
        B *r = c->add(1, i, 400.0f, 210.0f, -280.0f, 0.0f, 0.0f, 4.712389f);
        items[i] = r;
        if (!(tu & (1 << i))) FF(r, 0x2A4) = 0.0f;
    }
    d->add(1, 1, -220.0f, 200.0f, -400.0f, 0.0f, 0.0f, 0.0f);
    B *e2 = d->add(1, 2, -220.0f, 200.0f, -400.0f, 0.0f, 0.0f, 0.0f);
    B *e3 = d->add(1, 3, -220.0f, 200.0f, -400.0f, 0.0, 0.0f, 0.0f);
    timproc_uso_b5_func_048E7C(timproc_uso_b5_D_807FEE1C + 0x15AC, (*(u16 **)((char *)&import_800201EC + 0x154))[1]);
    switch ((*(u16 **)((char *)&import_800201EC + 0x154))[1]) {
    case 1:
        FF(e2, 0x2A4) = 0.0;
    case 2:
        FF(e3, 0x2A4) = 0.0f;
        break;
    case 3: case 4: case 5:
        break;
    }
    setup(1, arg1);
    m50->setup(1, arg2);
    f4();
    m38->f4();
}
