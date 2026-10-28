/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00054E78 (0x228 / 138 words), agent-c run 175.
 * Real C++ `void Obj::reflect(const Plane &p)` (renamed with objcopy; ident() renamed to the gl_func_00034458
 * placeholder): mtx (at this+300) = identity call, then the plane-reflection matrix I - 2 n n^T written row by row
 * through the reference, the translation row from `a.set(p.n * p.d)` (the target's three back-to-back products
 * stored to the sp+80 return temporary, lw/sw copies into sp+100 / the sp+44 by-value set() parameter, member
 * stores to a at sp+136) and k = 2 a.n, then the current-matrix global (&D_00000000 + 0xE4) = &mtx.
 * The diagonal ones are an int inline `i1()` (each use folds to its own 1.0f after candidate selection: length
 * = target). cfe body 86.62 -> 91.07, length = target. [run 203: frame and every home now = target (cp(), u, t);
 * only the FP colouring / shared 1.0f below is left.] Old residual: frame 128 vs 152 (the return / parameter temporaries sit
 * 8-24 bytes low) and the 1.0f constants still share one FP register.
 * Still NON_MATCHING: build/non_matching/ only. */
typedef float f32;
struct V3 { f32 x, y, z;
    void cp(const V3 &p) { x = p.x; y = p.y; z = p.z; }
    void set(V3 p) { cp(p); } /* agent-g run 203 (kit CLXXIII): by-value set() forwarding to cp(const V3 &) homes
                                 the parameter / return / copy temps on the target's sp+44 / 80 / 100 */
    V3() {}
    V3(f32 a, f32 b, f32 c) { x = a; y = b; z = c; }
    V3 operator*(f32 s) const { return V3(x * s, y * s, z * s); }
    f32 dot(const V3 &o) const { return y * o.y + x * o.x + z * o.z; } /* run 200: y term first (operand-order grid) */
};
inline int i1() { return 1; }
struct Plane { V3 n; f32 d; };
struct M4 { f32 m[4][4]; void ident(); };
extern "C" char D_00000000[];
struct Obj { char pad[300]; M4 mtx; void reflect(const Plane &p); };
void Obj::reflect(const Plane &p) {
    volatile int u; /* agent-g run 203: homed above a (the target's 4 bytes at sp+148; a's home is always frame - 12 without it) */
    V3 a; /* agent-g run 195: declared at the top (NCC homes it first, the frame-top aggregate) */
    { V3 t; } /* run 203: 12 unreferenced bytes below a (63964 rule): frame 152, a at sp+136 = target */
    mtx.ident();
    mtx.m[0][0] = i1() - 2.0f * (p.n.x * p.n.x);
    mtx.m[0][1] = 0.0f - 2.0f * (p.n.y * p.n.x);
    mtx.m[0][2] = 0.0f - 2.0f * (p.n.x * p.n.z);
    mtx.m[0][3] = 0.0f;
    mtx.m[1][0] = 0.0f - 2.0f * (p.n.y * p.n.x);
    mtx.m[1][1] = i1() - 2.0f * (p.n.y * p.n.y);
    mtx.m[1][2] = 0.0f - 2.0f * (p.n.y * p.n.z);
    mtx.m[1][3] = 0.0f;
    mtx.m[2][0] = 0.0f - 2.0f * (p.n.x * p.n.z);
    mtx.m[2][1] = 0.0f - 2.0f * (p.n.y * p.n.z); if (1) {} /* agent-g run 201: block-split position grid (the run-190 / run-195 splits after m[0][0] / m[0][2] moved here) */
    mtx.m[2][2] = i1() - 2.0f * (p.n.z * p.n.z); if (1) {}
    mtx.m[2][3] = 0.0f;
    a.set(p.n * p.d);
    f32 k = a.dot(p.n) * 2.0f;
    mtx.m[3][0] = k * p.n.x; /* run 200: back to x-first with the y-first dot (target stores 348 first) */
    mtx.m[3][1] = k * p.n.y;
    mtx.m[3][2] = k * p.n.z;
    mtx.m[3][3] = i1(); /* run 203: i1() like the diagonals -- a literal 1.0f here was CSE'd with them into one long-lived f16 */
    *(M4 **)(D_00000000 + 0xE4) = &mtx;
}
