/* IDO 7.1 NCC (C++) donor for gl_func_000015FC (0x19C / 103 words), EXACT.
 * The function is a cfront-style constructor G::G(int) of a 0x198-byte class:
 * edg's ctor protocol gives the get-or-create head (`if (this == 0) this =
 * new(0x198)`, operator new forwarded inline to the alloc), the out-of-line
 * base ctor B(&D[0xCC58]) runs first, then the vptr at +0x28 (B declares the
 * virtual after 0x28 bytes of data), then the member ctor at +0x2C, then the
 * body. agent-g run 163 (docs/IDO_CODEGEN.md kit CII): as cfe C the li 100
 * kept v0 (target v1) under every spelling; as a real ctor with a named
 * `char *self = (char *)this` the return-value LR is live through the body
 * block and v0 is forbidden for the constant. The 100 stores are two
 * statements (a chained assignment costs an edg temp home, frame 64 vs 56),
 * and the last three init calls take ((char *)this) instead of self, which
 * orders the two call-crossing address spills like the target (0x180 at
 * sp+40, 0x168 at sp+44).
 * Symbols renamed after compile (names only): __ct__1GFi -> gl_func_000015FC,
 * __ct__1BFPc -> gl_func_00000000_15fc_base, __ct__1MFv ->
 * gl_func_00000000_15fc_m, __vtbl__1G -> gl_data_00000000_15fc. The data
 * words are pinned at their USO offsets (D_15FC_* in undefined_syms_auto.txt). */
#define FW(p, o) (*(int *)((char *)(p) + (o)))
#define FF(p, o) (*(float *)((char *)(p) + (o)))
extern "C" {
void *gl_func_00000000(...);
extern char D_15FC_cc58[], D_15FC_cc60[];
extern float D_15FC_c7c, D_15FC_c80, D_15FC_c84, D_15FC_c88, D_15FC_c8c, D_15FC_c90, D_15FC_c94;
}
inline void *operator new(unsigned n) { return gl_func_00000000(n); }
struct B { char p[0x28]; B(char *); virtual void g(); };
struct M { char p[0xA8]; M(); };
struct G : B { M m; char rest[0x198 - 0xD4]; G(int arg1); virtual void g(); };
G::G(int arg1) : B(D_15FC_cc58) {
    char *self = (char *)this;
    float temp_f2 = D_15FC_c7c;
    FW(self, 0xC) = (int)D_15FC_cc60;
    FF(self, 0xE8) = temp_f2;
    FF(self, 0xF8) = temp_f2;
    FF(self, 0xEC) = 0.0f; FF(self, 0xF0) = 0.0f; FF(self, 0xF4) = 0.0f; FF(self, 0xFC) = 0.0f; FF(self, 0x100) = 0.0f; FF(self, 0x104) = 0.0f;
    FF(self, 0x108) = D_15FC_c80;
    FF(self, 0x118) = 1.0f;
    FF(self, 0x114) = 0.0f;
    FF(self, 0x110) = 0.0f;
    FF(self, 0x10C) = 0.0f;
    FF(self, 0x11C) = D_15FC_c84;
    FF(self, 0x124) = 0.0f;
    FF(self, 0x120) = 0.0f;
    FF(self, 0xD8) = D_15FC_c88;
    FF(self, 0xDC) = D_15FC_c8c;
    FF(self, 0xE0) = D_15FC_c90;
    FF(self, 0xE4) = 0.0f;
    FW(self, 0xD4) = arg1;
    FF(self, 0x128) = 0.0f;
    FW(self, 0xCC) = 0x64;
    FW(self, 0xC8) = 0x64;
    FF(self, 0x12C) = D_15FC_c94;
    FW(self, 0xD0) = 0x82;
    gl_func_00000000(self + 0x138, 0x5000A);
    gl_func_00000000(self + 0x150, 0x50009);
    gl_func_00000000(((char *)this) + 0x180, 0x5000B);
    gl_func_00000000(((char *)this) + 0x168, 0x5001C);
    gl_func_00000000(((char *)this), ((char *)this) + 0x168, ((char *)this) + 0x180);
    gl_func_00000000(self, -0x49, -2, -0x32, -1);
}
