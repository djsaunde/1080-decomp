/* IDO 7.1 NCC (C++) donor for gl_func_00041A9C (0x2A4 / 169 words): debug-menu
 * registration -- sixteen calls, two `new` objects (D41/E41 over the B41 base whose
 * ctor stores the 0x1F4B0 vptr), then a {cur, nxt} list walk calling every child.
 * next() is written as an if/return (kit XIX #2): with the ?: form the first next()
 * result shared the D41 new-expression's edg temp and coloured v1 (target v0).
 * The blank USO refs are distinct symbols pinned at 0 (one shared symbol is CSE'd
 * into s0). Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
extern "C" void gl_func_00000000(...);
extern "C" void *gl_alloc_2D130(...);
extern "C" char D_00000000[];
extern "C" char D_41A9C_s1[], D_41A9C_s2[], D_41A9C_s3[], D_41A9C_s4[], D_41A9C_s5[];
extern "C" char D_41A9C_s6[], D_41A9C_s7[], D_41A9C_s8[], D_41A9C_s9[], D_41A9C_s10[];
extern "C" char D_41A9C_s12[], D_41A9C_s14[], D_41A9C_s15[], D_41A9C_s16[];
extern "C" char D_41A9C_s17[], D_41A9C_s18[];
extern "C" int D_41A9C_v1[], D_41A9C_v2[], D_41A9C_v3[], D_41A9C_v4[];
extern "C" int D_41A9C_t1[], D_41A9C_t2[];
struct Link41A { void *obj; Link41A *next; };
struct Iter41A {
    Link41A *cur, *nxt;
    Iter41A(Link41A *h) { nxt = h; }
    void *next() { if ((cur = nxt) != 0) { nxt = nxt->next; return cur->obj; } return 0; }
    void *get() { return cur->obj; }
};
inline void *operator new(unsigned n) { return gl_alloc_2D130(n); }
struct B41 {
    int vt;
    B41() { vt = (int)(D_00000000 + 0x1F4B0); }
};
struct D41 : B41 { int x; D41() { vt = (int)D_41A9C_v1; x = (int)D_41A9C_v2; } };
struct E41 : B41 { int x; E41() { vt = (int)D_41A9C_v3; x = (int)D_41A9C_v4; } };
struct Self41A { char pad[0x2C]; Link41A *head; char pad30[0xC]; char name[4]; };
extern "C" void gl_func_00041A9C(Self41A *self) {
    char buf[64];
    gl_func_00000000(D_41A9C_s1, D_00000000 + 0x1F658, 0);
    gl_func_00000000(D_41A9C_s2, D_00000000 + 0x1F65C);
    gl_func_00000000(self->name, buf);
    gl_func_00000000(D_41A9C_s3, buf);
    gl_func_00000000(self->name, buf);
    gl_func_00000000(D_41A9C_s4, buf);
    gl_func_00000000(D_41A9C_s5);
    gl_func_00000000(D_41A9C_s6, D_00000000 + 0x1F668, new D41, 0);
    gl_func_00000000(D_41A9C_s7, D_00000000 + 0x1F670, D_41A9C_s8, 1);
    gl_func_00000000(D_41A9C_s9, D_00000000 + 0x1F678, D_41A9C_s10, 4);
    gl_func_00000000(buf, D_00000000 + 0x1F680, D_41A9C_t1[2], D_41A9C_t1[0]);
    gl_func_00000000(D_41A9C_s12, buf);
    gl_func_00000000(buf, D_00000000 + 0x1F690, D_41A9C_t2[2], D_41A9C_t2[0]);
    gl_func_00000000(D_41A9C_s14, buf);
    gl_func_00000000(D_41A9C_s15, D_00000000 + 0x1F6A0, new E41, 1);
    gl_func_00000000(D_41A9C_s16, D_00000000 + 0x1F6AC);
    for (Iter41A it(self->head); it.next(); )
        gl_func_00000000(it.get());
    gl_func_00000000(D_41A9C_s17);
    gl_func_00000000(D_41A9C_s18);
    gl_func_00000000(self);
}
