/* IDO 7.1 NCC (C++) donor for func_00000CA0 (0xF4 / 61 words): the
 * constructor D::D(int, int, int) of a 0x40-byte class with a base B whose
 * ctor is out of line (B(&D_000066A8), import) and a vptr at +0x28 (B
 * declares the virtual). edg's ctor protocol supplies the get-or-create head
 * (`if (this == 0) this = new(0x40); if (this) { ... } return this;`) with the
 * alloc forwarded through an inline operator new. Body: scale (1,1,1), 0.0
 * at +0x38, a1 at +0xC, a2 at +0x3C, a3 to two imported words, then a3 / 2
 * written as the four corners of a 16-byte vertex table.
 *
 * The last corner is written as three statements on three LINES: as1 only
 * reorders inside one source-line (.loc) group, and with set(h, nh, h) on a
 * single line it issued the nh store (+0x32) before the h store (+0x30).
 * Symbols renamed after compile (names only): __ct__1DFiN21 -> func_00000CA0,
 * __ct__1BFPc -> func_00000000_ca0_base, __vtbl__1D -> bu_ca0_desc.
 * Spliced into bootup_uso.c.o via REPLACE_FUNC_BODY (agent-g run 150). */
extern "C" {
void *func_00000000(...);
extern char D_000066A8;
extern int bu_ca0_g[], bu_ca0_g_b[];
}
inline void *operator new(unsigned n) { return func_00000000(n); }
struct V { short x, y, z, w; int p, q; void set(int a, int b, int c) { x = a; y = b; z = c; } };
extern "C" V bu_ca0_box[];
struct B { int p0[3]; int a1; int p1[6]; B(char *); virtual void g(); };
struct D : B { float sx, sy, sz, r; int a2; D(int a1, int a2, int a3); virtual void g(); };
D::D(int a1_, int a2_, int a3) : B(&D_000066A8) {
    int h, nh;
    a1 = a1_;
    sz = 1.0f;
    sy = 1.0f;
    sx = 1.0f;
    r = 0.0f;
    a2 = a2_;
    bu_ca0_g[0] = a3;
    bu_ca0_g_b[1] = a3;
    h = a3 / 2;
    nh = -h;
    bu_ca0_box[0].set(h, h, h);
    bu_ca0_box[1].set(nh, h, h);
    bu_ca0_box[2].set(nh, nh, h);
    bu_ca0_box[3].x = h;
    bu_ca0_box[3].y = nh;
    bu_ca0_box[3].z = h;
}
