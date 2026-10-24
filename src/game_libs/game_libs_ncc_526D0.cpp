/* IDO 7.1 NCC (C++) donor for gl_func_000526D0 (0x248 / 146 words) -- EXACT.
 *
 * A C++ constructor Obj::Obj(void *, void *, Obj *src) with cfront's
 * `this == 0 -> new(0xB0)` protocol, the external base ctor, the vptr store
 * at 0x28, the inline Sub() member ctor (its own alloc-fallback for 0x14) and
 * an optional implicit memberwise copy from src. The donor's symbols are
 * renamed in the Makefile (names only, no bytes change): the ctor to
 * gl_func_000526D0, and the base ctor / vtable / callees to zero-valued
 * aliases (the ROM's calls are blank USO imports, jal 0 / lui-addiu 0).
 * What pins it (docs/IDO_CODEGEN.md, agent-g run 147):
 * - the 0x18 member is a one-word STRUCT (Flags): a 1-word struct copy takes
 *   a temp pair and stores the second, which is the target's one-register
 *   advance of the t-ring after the Blk6 block copy.
 * - Base derives from the POD Blk6 (+8 frame: the derived-to-base conversion
 *   in the implicit operator=) and the copy sits in an inline member that
 *   does its own `if (o)` test (+8 frame: frame 72); with the test outside
 *   the inline, &m30 stops being CSEd into v0.
 */
extern "C" {
void *gl_func_00000000_526d0_alloc(unsigned);
void gl_func_00000000_526d0_reg(void *, void *);
void gl_func_00000000_526d0_fin(void *, void *);
}
struct Blk6 { int w[6]; };
struct Blk2 { int w[2]; };
struct Flags { int v; };
struct Sub {
    void *operator new(unsigned n) { return gl_func_00000000_526d0_alloc(n); }
    int a, b, c, d, e;
    Sub() { c = 0; d = 0; }
};
struct Base : Blk6 {
    Flags m18;
    int m1C, m20, m24;
    Base(void *a1);
    virtual void f();
};
struct Obj : Base {
    void *operator new(unsigned n) { return gl_func_00000000_526d0_alloc(n); }
    int m2C;
    Flags m30;
    Blk2 m34;
    int m3C, m40, m44, m48, m4C, m50, m54, m58, m5C, m60, m64, m68, m6C, m70, m74, m78, m7C, m80;
    Sub m84;
    int m98;
    float m9C;
    int mA0, mA4, mA8, mAC;
    Obj(void *a1, void *a2, Obj *src);
    void copy(const Obj *o) { if (o) { int save = m1C; *this = *o; m30.v |= 0x10; m1C = save; } }
    virtual void f();
};
Obj::Obj(void *a1, void *a2, Obj *src) : Base(a1) {
    gl_func_00000000_526d0_reg(this, &m84);
    copy(src);
    if (a2 != 0) {
        gl_func_00000000_526d0_fin(this, a2);
    }
}
