/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00054E78 (0x228 / 138 words), agent-c run 175.
 * Real C++ `void Obj::reflect(const Plane &p)` (renamed with objcopy; ident() renamed to the gl_func_00034458
 * placeholder): mtx (at this+300) = identity call, then the plane-reflection matrix I - 2 n n^T written row by row
 * through the reference, the translation row from `a.set(p.n * p.d)` (the target's three back-to-back products
 * stored to the sp+80 return temporary, lw/sw copies into sp+100 / the sp+44 by-value set() parameter, member
 * stores to a at sp+136) and k = 2 a.n, then the current-matrix global (&D_00000000 + 0xE4) = &mtx.
 * The diagonal ones are an int inline `i1()` (each use folds to its own 1.0f after candidate selection: length
 * = target). cfe body 86.62 -> 91.07, length = target. Residual: frame 128 vs 152 (the return / parameter temporaries sit
 * 8-24 bytes low) and the 1.0f constants still share one FP register.
 * Still NON_MATCHING: build/non_matching/ only. */
typedef float f32;
struct V3 { f32 x, y, z;
    void set(V3 p) { x = p.x; y = p.y; z = p.z; }
    V3() {}
    V3(f32 a, f32 b, f32 c) { x = a; y = b; z = c; }
    V3 operator*(f32 s) const { return V3(x * s, y * s, z * s); }
    f32 dot(const V3 &o) const { return x * o.x + y * o.y + z * o.z; }
};
inline int i1() { return 1; }
struct Plane { V3 n; f32 d; };
struct M4 { f32 m[4][4]; void ident(); };
extern "C" char D_00000000[];
struct Obj { char pad[300]; M4 mtx; void reflect(const Plane &p); };
void Obj::reflect(const Plane &p) {
    V3 a; /* agent-g run 195: declared at the top (NCC homes it first, the frame-top aggregate) */
    mtx.ident();
    mtx.m[0][0] = i1() - 2.0f * (p.n.x * p.n.x); if (1) {} /* agent-g run 195: lever-sweep bbsplit after the a-top move */
    mtx.m[0][1] = 0.0f - 2.0f * (p.n.x * p.n.y);
    mtx.m[0][2] = 0.0f - 2.0f * (p.n.x * p.n.z); if (1) {} /* agent-g run 190: lever-sweep bbsplit (91.01 -> 91.23) */
    mtx.m[0][3] = 0.0f;
    mtx.m[1][0] = 0.0f - 2.0f * (p.n.y * p.n.x);
    mtx.m[1][1] = i1() - 2.0f * (p.n.y * p.n.y);
    mtx.m[1][2] = 0.0f - 2.0f * (p.n.y * p.n.z);
    mtx.m[1][3] = 0.0f;
    mtx.m[2][0] = 0.0f - 2.0f * (p.n.z * p.n.x);
    mtx.m[2][1] = 0.0f - 2.0f * (p.n.z * p.n.y);
    mtx.m[2][2] = i1() - 2.0f * (p.n.z * p.n.z);
    mtx.m[2][3] = 0.0f;
    a.set(p.n * p.d);
    f32 k = a.dot(p.n) * 2.0f;
    mtx.m[3][1] = k * p.n.y; /* run 194 (lever-sweep order) */
    mtx.m[3][0] = k * p.n.x;
    mtx.m[3][2] = k * p.n.z;
    mtx.m[3][3] = 1.0f;
    *(M4 **)(D_00000000 + 0xE4) = &mtx;
}
