/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for func_000031C0 (0x328 / 202 words).
 * agent-g run 164 (docs/IDO_CODEGEN.md kit CIV): the cfe body was C++ decoded as C.
 * `obj = new Obj2` (global inline operator new -> edg keeps the new-temp T and copies it
 * to obj at the join; Obj2 adds 4 bytes to the 0xB4 Obj, so the inlined Obj ctor's
 * this==0 fallback allocates 0xB4 while the new-expression allocates 0xB8), the Obj ctor
 * = out-of-line base B(&D_7460), vptr +0x28, member M at +0x2C with nested N (two
 * protocol tests), fin(this), pos.set(V3(0, 500, 0)); the 0-first-argument calls are
 * out-of-line ctors called by `new` (cfront protocol: ctor(0, args)); case 1 is
 * `new KB(obj, a3v)` (inline ctor over the out-of-line base B2, vptr = D_7120); the tail
 * is an inline P::add(n) twice plus the vec copies, in its own block (puts the
 * V3(0, 0, -1000) temp at the bottom like the target). The `if (1) {}` splits the
 * a3v live unit so obj takes v0 / a3v v1 as in the target.
 * 202/202 words; LEFT: edg frame layout (frame 128 vs 144). Run 165 (kit CV): V3's ctor
 * forwarding to an inline set() plus the Obj ctor's V3 temp moved into an inline pos() put
 * every home from the V3(0, 500, 0) temp down at the target offsets; still missing: a fresh
 * KB new-temp right below Obj-this (ours reuses T's slot) and 3 more words above the V3 temp.
 * -1500.0f = bootup RoData +0x530/+0x534.
 * Symbols renamed after compile (names only, see Makefile). */
#define FW(p, o) (*(int *)((char *)(p) + (o)))
#define FF(p, o) (*(float *)((char *)(p) + (o)))
struct V3 { float x, y, z; void set(float a, float b, float c) { x = a; y = b; z = c; } V3(float a, float b, float c) { set(a, b, c); } };
extern "C" {
void *fn_31c0_alloc(int);
void  fn_31c0_fin(void *);
void  fn_31c0_vec(void *, const V3 &);
void  fn_31c0_link(void *, void *);
void  fn_31c0_tail(void *, void *);
extern char D_00007460[], D_0000746C[], D_00007480[], D_000074A4[], D_000074B0[], D_000074C4[], D_00007490[];
}
inline void *operator new(unsigned n) { return fn_31c0_alloc(n); }
struct N { int v; N() { v = 0; } };
struct M { N n; M() {} };
struct B { char p[0x28]; B(void *); virtual void g(); };
struct Obj : B { M m; char rest[0xB4 - 0x30]; Obj() : B(D_00007460) { fn_31c0_fin(this); pos(); } void pos() { fn_31c0_vec((char *)this + 0x30, V3(0.0f, 500.0f, 0.0f)); } virtual void g(); };
struct Obj2 : Obj { int x; Obj2() {} };
struct B2 { char p[0x28]; B2(Obj *, float, float, float); virtual void g(); };
struct KB : B2 { char q[0x14C - 0x2C]; float f14c; float f150; char *f154; int f158;
    KB(Obj *o, char *a) : B2(o, 0.0f, 500.0f, -1500.0f) { f158 = 0; f154 = a; f150 = f14c; } virtual void g(); };
struct WA { WA(char *, Obj *, float, float, float, int, char *); };
struct WR { WR(char *, int, char *); };
struct W4 { W4(char *, char *, char *); };
struct WV { WV(char *, char *, char *); };
struct P { char p[0x10]; char list[4];
    void add(char *n) { fn_31c0_tail(list, n); if (FW(n, 0x14) != 0) FW(n, 0x4) = 1; FW(n, 0x14) = (int)this; } };
extern "C" void *func_000031C0(int a0, int mode, P *parent, char *a3v, int p4) {
    char *w;
    char *h;
    char *v;
    char *r2;
    char *r3;
    Obj *obj;
    obj = new Obj2;
    switch (mode) {
    case 0:
    case 2:
        w = (char *)new WA(a3v, obj, 0.0f, 500.0f, -1500.0f, p4, D_0000746C);
        h = w;
        r2 = (char *)new WR(a3v, FW(a3v, 0x874), D_00007480);
        fn_31c0_link(w, r2);
        FW(w, 0x154) = (int)r2;
        r3 = (char *)new W4(w, r2, D_00007490);
        fn_31c0_link(obj, r3);
        FW(obj, 0xB4) = (int)r3;
        FW(r3, 0xC) = (int)D_000074A4;
        break;
    case 1:
        h = (char *)new KB(obj, a3v);
        v = (char *)new WV(a3v, a3v, D_000074B0);
        fn_31c0_link(obj, v);
        FW(v, 0xC) = (int)D_000074C4;
        break;
    }
    {
    parent->add(h);
    parent->add((char *)obj);
    FF(obj, 0x60) = FF(a3v, 0x3A4);
    FF(obj, 0x64) = FF(a3v, 0x3A8);
    FF(obj, 0x68) = FF(a3v, 0x3AC);
    FF(h, 0x60) = FF(a3v, 0x3A4);
    if (1) {}
    FF(h, 0x64) = FF(a3v, 0x3A8);
    FF(h, 0x68) = FF(a3v, 0x3AC);
    fn_31c0_vec(h + 0x30, V3(0.0f, 0.0f, -1000.0f));
    }
    return h;
}
