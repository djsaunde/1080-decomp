/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_0004D468 (0x1F0 / 124
 * words): module init -- allocate the 0x50 owner if none, clear it, register 16
 * new 0x78-byte nodes, allocate the 0x19000-byte heap with a 0x12345678 guard
 * word, start the 0x3C950 task, set the 0x3CB00 guard pair, three bind calls.
 * agent-c run 137 (cfe NM 78.6 -> NCC 124/124 words, 9 raw left). Load-bearing:
 * each node is `p = alloc(0x78, 16u); reg(p ? p : 0, s)` (the edg new-expression
 * null test; 16u keeps the loop bound 16 out of the argument); the heap guard is
 * an UNSIGNED store (an int store CSEs 0x12345678 into s2 across the calls); the
 * 0x3CB00 guard pair is ONE static `long long = 0x1234567812345678` (one ugen
 * temp for both words, one shared `lui at`: file-local object, kit LX); each
 * bind call reads its D+0x1C argument into a block-scope int first (target loads
 * a1 before a0). Residual: the node pointer's merge register (a2 here, target a3,
 * both sites); an exact donor would also pin the renamed .bss reloc to 0x3CB00.
 * Spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
extern "C" {
extern int gl_func_00000000(...);
extern char D_00000000[];
extern int D_4D468_x, D_4D468_y, D_4D468_z;
extern char D_4D468_w[];
extern char D_4D468_a2[];
}
struct E { E() {} char pad[0x78]; };
void *operator new(unsigned int n, int al);
static long long sCanL;
struct S4D { int w0, w4, w8, wC; int w10; int w14; int w18; int w1C, w20, w24, w28, w2C; char pad30[0x14]; int w44; };
extern "C" S4D *gl_func_0004D468(S4D *s) {
    if (s != 0 || (s = (S4D *)gl_func_00000000(0x50)) != 0) {
    gl_func_00000000(D_00000000 + 0x201F4, 0);
    gl_func_00000000(D_00000000 + 0x3EB00, 0xB0000B70, 0x100);
    s->w44 = 0;
    s->w0 = 0;
    s->w4 = 0;
    s->w8 = 0;
    s->wC = 0;
    for (int i = 0; i < 16; i++) {
        int p = gl_func_00000000(0x78, 16u); gl_func_00000000(p ? p : 0, s);
    }
    { int p = gl_func_00000000(0x78, 16); s->w14 = p ? p : 0; }
    D_4D468_x = gl_func_00000000(0xC00, 16);
    D_4D468_y = 0x19000;
    D_4D468_z = gl_func_00000000(D_4D468_y + 4, 64);
    ((unsigned *)D_4D468_z)[(unsigned)D_4D468_y / 4] = 0x12345678u;
    s->w2C = gl_func_00000000(0x400, 16);
    gl_func_00000000(D_00000000 + 0x3C950, 4, D_4D468_a2, s, D_00000000 + 0x3EB00, 11);
    sCanL = 0x1234567812345678LL;
    { int h = *(int *)(D_4D468_w + 0x1C); gl_func_00000000(9, h, 102); }
    { int h = *(int *)(D_4D468_w + 0x1C); gl_func_00000000(4, h, 101); }
    { int h = *(int *)(D_4D468_w + 0x1C); gl_func_00000000(14, h, 108); }
    gl_func_00000000(D_00000000 + 0x3C950);
    s->w20 = 0;
    s->w1C = 0;
    s->w24 = 0;
    s->w28 = 0;
    gl_func_00000000();
    s->w20 = 0;
    s->w1C = 0;
    }
    return s;
}
