/* IDO 7.1 NCC (C++) EXACT donor for gl_func_000445AC (0x36C / 219 words), agent-c run 162.
 * One-shot UI / object bootstrap: registration calls, a new-expression of A40 (inline ctor: a 32-byte
 * buffer + init call) stored into the global holder and passed on, a new-expression of L24 : L20 (the
 * list-header ctor chain with edg's this == 0 fallback) and the float-returning count read.
 * The cfe body (99.82, 6 words) evaluated the last vararg into a coloured variable (v1, target t7) and
 * folded the holder store into a t-register base; as C++ the varargs load in source order (t5 / t6 / t7)
 * and `gl(D_445AC_b.x1C = new A40, 0x64, 1)` -- the store as the call's first argument, no named
 * pointer -- makes the holder base a v0 variable with the target's lui / addiu / sw 28(v0) and frees the
 * home that a named `p` cost (frame 0x60 -> 0x58 = target). Spliced over an empty placeholder in both trees. */
typedef unsigned int u32;
extern "C" {
int gl_func_00000000(...);
float gl_func_00000000_445f(...);
extern char D_00000000[];
extern int D_445AC_ga, D_445AC_gb, D_445AC_gc, D_445AC_gd, D_445AC_ge;
}
#define S(o) (D_00000000 + (o))
inline void *operator new(unsigned n) { return (void *)gl_func_00000000(n); }
struct A40 {
    char pad[0x18];
    char *x18;
    char pad1C[0x28 - 0x1C];
    A40() { x18 = (char *)gl_func_00000000(32); gl_func_00000000(this, x18, 8); }
};
struct Holder { char pad[0x1C]; A40 *x1C; };
extern "C" Holder D_445AC_b;
struct L20 {
    int x0; int x4; int x8; int xC; int x10;
    L20() { x8 = 10; xC = 0; x0 = gl_func_00000000(40); }
};
struct L24 : L20 {
    char *x14;
    L24() { x14 = D_00000000; }
};
struct Obj { char pad[0x20C]; int x20C; int pad210; int x214; int x218; char pad21C[0x22C - 0x21C]; L24 *x22C; char pad230[0x240 - 0x230]; int x240; char pad244[0x280 - 0x244]; int x280; int x284; };
extern "C" void gl_func_000445AC(Obj *a0) {
    gl_func_00000000(0x140, 0xF0);
    gl_func_00000000(0x140, 0xF0);
    gl_func_00000000(S(0x1FDE4), 0);
    a0->x240 = gl_func_00000000(0, 0x64, S(0x1FDEC), 0x6D, 2, 0x6E, 0, 0, 0x6F, D_445AC_ga, D_445AC_gb, 0x70, 0x1E00, 0x71, 2, 0);
    gl_func_00000000();
    a0->x280 = gl_func_00000000(0, S(0x1FDFC), 40, 0, 0, 0, 0);
    a0->x284 = gl_func_00000000(0, S(0x1FE04), 40, 0, 0, 0, 0);
    *(int *)D_00000000 = gl_func_00000000(0, S(0x1FE0C), 20, 0, 0, 0, 0);
    gl_func_00000000(a0);
    gl_func_00000000(S(0x1FE14), 0);
    a0->x214 = gl_func_00000000(0, S(0x1FE24), D_445AC_ga, D_445AC_gb, D_445AC_gc, D_445AC_gd, D_445AC_ge);
    gl_func_00000000();
    gl_func_00000000(D_445AC_b.x1C = new A40, 0x64, 1);
    a0->x22C = new L24;
    a0->x218 = gl_func_00000000(0);
    gl_func_00000000(a0);
    gl_func_00000000(a0);
    a0->x20C = 1;
    gl_func_00000000(S(0x1FE30));
    gl_func_00000000(a0);
    gl_func_00000000(S(0x1FE3C));
    a0->x20C = (u32)gl_func_00000000_445f(S(0x1FE48));
    if ((u32)a0->x20C < 10) {
        gl_func_00000000(S(0x1FE54));
    }
    gl_func_00000000(a0);
    gl_func_00000000(0x202);
    gl_func_00000000(a0, 1);
    gl_func_00000000(a0);
}
