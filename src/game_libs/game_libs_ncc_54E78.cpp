/* IDO 7.1 NCC (C++) EXACT donor for gl_func_00054E78 (0x228 / 138 words), agent-c run 175 lift, agent-g run 304 exact.
 * Real C++ `void Obj::reflect(const Plane &p)` (renamed with objcopy; ident() renamed to the gl_func_00034458
 * placeholder): mtx (at this+300) = identity call, then the plane-reflection matrix I - 2 n n^T written row by row
 * through the reference, the translation row from `a.set(p.n * p.d)` (the target's three back-to-back products
 * stored to the sp+80 return temporary, lw/sw copies into sp+100 / the sp+44 by-value set() parameter, member
 * stores to a at sp+136) and k = 2 a.n, then the current-matrix global (&D_00000000 + 0xE4) = &mtx.
 * The diagonal ones are an int inline `i1()` (each use folds to its own 1.0f after candidate selection).
 * agent-g run 304 (docs/IDO_CODEGEN.md kit CCCLXXXIX): the zero-code identity idv() on the RIGHT n.x of the m[0][0]
 * square (and on the left n.y / n.z of the m[1][1] / m[2][2] squares) is one shared edg pool variable whose uopt
 * live range now starts in the m[0][0] block: it colours f12 before n.y / n.z, which take the target's f14 / f16
 * (the run-303 code-free n.z test after m[0][0] is dropped). operator* keeps a 4-byte pool temporary inside the
 * translation full-expression (the folded `- 0 * idv(0)`: the result / copy temporaries stay at the target's
 * sp+80 / +100) and routes the y product through idv() with the x product named first: x*d / y*d / z*d take
 * f0 / f12 / f14 in the target's order. Byte-exact (552 bytes). */
typedef float f32;
struct V3 { f32 x, y, z;
    void cp(const V3 &p) { x = p.x; y = p.y; z = p.z; }
    void set(V3 p) { cp(p); } /* agent-g run 203 (kit CLXXIII): by-value set() forwarding to cp(const V3 &) homes
                                 the parameter / return / copy temps on the target's sp+44 / 80 / 100 */
    V3() {}
    V3(f32 a, f32 b, f32 c) { x = a; y = b; z = c; }
    V3 operator*(const f32 &s) const; /* defined below idv() */
    f32 dot(const V3 &o) const { return x * o.x + y * o.y + z * o.z; } /* run 200: y term first (operand-order grid); agent-g run 231: back to x first (stale after later levers, 98.22 -> 98.66) */
};
inline int i1() { return 1; }
template <class T> inline T idv(T v) { return v; } /* agent-g run 301: zero-code by-value identity inline (kit CCCLXXX) */
inline V3 V3::operator*(const f32 &s) const { f32 a = x * s - 0 * idv(0); return V3(a, idv(y * s), z * s); } /* agent-g run 304:
    `- 0 * idv(0)` folds to nothing (x * s - 0.0f is the identity for every float, -0 included) but its int pool temporary
    is created inside the translation full-expression like run 303's idv(s) (frame 152, temps at the target's sp+80 /
    +100) without a float pool read in the product block; idv(y * s) with the x product named first gives the
    target's product registers and issue order */
struct Plane { V3 n; f32 d; };
struct M4 { f32 m[4][4]; void ident(); };
extern "C" char D_00000000[];
struct Obj { char pad[300]; M4 mtx; void reflect(const Plane &p); };
void Obj::reflect(const Plane &p) {
    volatile int u; /* agent-g run 203: homed above a (the target's 4 bytes at sp+148; a's home is always frame - 12 without it) */
    V3 a; /* agent-g run 195: declared at the top (NCC homes it first, the frame-top aggregate) */
    { V3 t; } /* run 203: 12 unreferenced bytes below a (63964 rule): frame 152, a at sp+136 = target */
    /* agent-g run 260 (kit CCXCV code-free constant lever): two empty tests add uses to the 0.0f / 2.0f LRs (no code): 2.0f -> f2, n.x -> f12 = target (98.66 -> 99.06).
     * agent-c run 273: the two tests moved to right after the m[2][3] = 0.0f store (and the two `if (1) {}` splits dropped):
     * the test block ends the 0.0f LR before the product block, so 0.0f takes f0 and n.y f14 like the target (99.06 -> 99.48) */
    mtx.ident();
    mtx.m[0][0] = i1() - 2.0f * (p.n.x * idv(p.n.x)); /* agent-g run 304: idv() on the right n.x (the pool variable's first
        register-worthy block), the run-303 n.z test dropped */
    mtx.m[0][1] = 0.0f - 2.0f * (p.n.x * p.n.y); /* run 203: operand-order grid (m[0][1], m[2][0], m[2][1]) */
    mtx.m[0][2] = 0.0f - 2.0f * (p.n.x * p.n.z);
    mtx.m[0][3] = 0.0f;
    mtx.m[1][0] = 0.0f - 2.0f * (p.n.y * p.n.x);
    if (1) {} /* agent-g run 301: BB split before m[1][1] (found as a position grid of the `{ V3 t; }` block, whose empty
               * constructor call splits the block the same way): p.d / z * d take the target's f18 / f14 (99.638 -> 99.746) */
    mtx.m[1][1] = i1() - 2.0f * (idv(p.n.y) * p.n.y);
    mtx.m[1][2] = 0.0f - 2.0f * (p.n.y * p.n.z);
    mtx.m[1][3] = 0.0f;
    mtx.m[2][0] = 0.0f - 2.0f * (p.n.z * p.n.x);
    mtx.m[2][1] = 0.0f - 2.0f * (p.n.z * p.n.y); /* agent-g run 201: block-split position grid (the run-190 / run-195 splits after m[0][0] / m[0][2] moved here; agent-c run 273: dropped) */
    mtx.m[2][2] = i1() - 2.0f * (idv(p.n.z) * p.n.z);
    mtx.m[2][3] = 0.0f; if (p.d != 2.0f) {} /* agent-c run 287: the `p.d != 0.0f` test dropped (stale: removal sweep, instruction stream identical at 99.48) */ a.set(p.n * p.d); /* agent-g run 298 (kit CCCLXXIV source-line scheduling,
        line-join sweep): a.set() on the test's line -- as1 now issues the p.d load after the p.n loads like the target (99.478 -> 99.529) */
    f32 k = a.dot(p.n) * 2.0f;
    mtx.m[3][0] = k * p.n.x; /* run 200: back to x-first with the y-first dot (target stores 348 first) */
    mtx.m[3][1] = k * p.n.y;
    mtx.m[3][2] = k * p.n.z;
    mtx.m[3][3] = i1(); /* run 203: i1() like the diagonals -- a literal 1.0f here was CSE'd with them into one long-lived f16 */
    *(M4 **)(D_00000000 + 0xE4) = &mtx;
}
