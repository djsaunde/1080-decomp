/* IDO 7.1 NCC (C++) NM-only comparison donor for game_libs_func_0004FD00 (0x3EC / 251 words), agent-c run 175.
 * Real C++ `int Col::side(int f1, int f2)` (renamed with objcopy): the collision record of the 3B6A0 donor (faces at
 * +0x68, s16 vertex triples at +0x60, face normals at +0x54). Sums each face's three vertices in a short vector
 * (an inline `Col::sum(V3 &, int)` with its own local SV -- the target's double `move` on the forwarded third add is
 * that inline local), converts to float through `V3::set(const SV &)`, then mid = a; mid += b; mid /= 2 and
 * d = mid - a / mid - b through a by-value `V3::set(V3)` (the target's lw/sw copy into the sp+20 temp followed by
 * member stores), both dots named before the test, `return 1` / `return 0`.
 * cfe body 80.29 -> 95.51, length 972 -> 1004 = target.
 * Residual (agent-c run 348): the two normal pointers (v1-base a3 / t1, target t0-base v1 / v1: ours computes the
 * second pointer before the first dot); every other instruction and sp offset = target.
 * Still NON_MATCHING: build/non_matching/ only. */
typedef float f32; typedef short s16; typedef unsigned short u16;
template <class T> inline T *me(T *p) { return p; } /* zero-code accessor (kit CCLXXXIV) */
struct SV { s16 x, y, z;
    void set(const SV &o) { x = o.x; y = o.y; z = o.z; }
    SV &operator+=(const SV &o) { x = x + o.x; y = y + o.y; z = z + o.z; return *this; }
};
struct V3 { f32 x, y, z;
    void set(V3 p) { x = p.x; y = p.y; z = p.z; }
    void set(const SV &s) { x = s.x; y = s.y; z = s.z; }
    V3 &operator+=(const V3 &o) { x += o.x; y += o.y; z += o.z; return *this; }
    V3 &operator-=(const V3 &o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
    V3 &operator/=(f32 s) { x /= s; y /= s; z /= s; return *this; }
    f32 operator*(const V3 &o) const { return x * o.x + y * o.y + z * o.z; }
};
struct Face { u16 mat; u16 vi[3]; };
struct Col {
    char pad[0x54]; V3 *nrm; char pad58[8]; SV *verts; char pad64[4]; Face *faces;
    /* agent-g run 293: a zero-code nrm getter plus me() accessors (kit CCCLXIV getter / accessor grid, then a local
     * search from a +4 B lead back to the target length): sum()'s first face read through me(this), the two normal
     * reads through me(me(this)) (the first via gn()): 95.506 -> 97.992, same length */
    V3 *gn() { return nrm; }
    /* agent-g run 294: the short accumulator is ONE caller local passed by reference (the target homes both sums' SV
     * at the same sp+68; per-expansion inline locals got two homes): 97.992 -> 98.088, same length */
    /* agent-c run 347: BOTH face sums in ONE inline expansion (separate statements inside one wrapper). A second
     * wrapper expansion binds its parameters through pool temps and keeps a reused reference word held for the whole
     * expansion (run 346: the second sum's adds on a fresh word, t0); inside a single expansion every reference --
     * all six vertex pointers -- shares one pool word, one LR in v0 like the target, and `move v0,zero` lands after
     * the sums (98.685 -> 99.004 with the second sum's first read through me(this) and normal-read depths 7 / 4). */
    void sums(V3 &a, int f1, V3 &b, int f2, SV &s) {
        s.set(verts[me(this)->faces[f1].vi[0]]); s += verts[faces[f1].vi[1]]; s += verts[faces[f1].vi[2]]; a.set(s);
        s.set(verts[me(this)->faces[f2].vi[0]]); s += verts[faces[f2].vi[1]]; s += verts[faces[f2].vi[2]]; b.set(s);
    }
    int side(int f1, int f2);
};
int Col::side(int f1, int f2) {
    V3 a, b, mid, d1, d2;
    SV s;
    sums(a, f1, b, f2, s);
    mid.set(a);
    mid += b;
    mid /= 2.0f;
    d1.set(mid);
    d1 -= a;
    d2.set(mid);
    d2 -= b;
    /* agent-g run 296: both normal reads through four nested zero-code me() accessors (depth grid 0-6 x 0-6, 49):
     * the extra `or v0,zero,zero` ours emitted inside the second face sum is gone (98.088 -> 98.586, same length);
     * then the second read also through gn() at depth 5 (random joint search + depth x getter grid, 169): -> 98.645;
     * agent-c run 347: depths 7 / 4 with the single sums() expansion (random climb) */
    /* agent-c run 348: BOTH normal references bound before either dot (depths 7 / 8, pad dropped): the second
     * pointer's binding moves uopt's 20-load block boundary in front of p1, so the 2.0f divisor takes $f0 like the
     * target (99.004 -> 99.024, same length, frame 136) */
    /* agent-h run 378: both dots inline (p1 depth 7, p2 depth 5) after two folded `f1 = f1 + 0 * (int)&d1;` kills
     * (kit CDLXIV): p1's depth-7 chain spills gn()'s return to the edg pool word -100 and reads it in bb 1, so the
     * sums' dead -100 store survives in bb 0 (t0) and K6 takes t1 like the target; the &d1 folds are counted
     * zero-code refs that move the bb 1 / bb 2 boundary in front of p1 (2.0f in $f0) without extending &s into
     * bb 1 (99.024 -> 99.084, same length); then p2 also at depth 7 behind `f2 = f2 + 0 * (int)nrm; f2 = f2;`:
     * the bb-2 fold keeps p2's operator* `o` store (pool word -128) from being forwarded, so n1 and n2 are ONE
     * variable LR like the target's shared v1 (99.084 -> 99.781) */
    {
    f1 = f1 + 0 * (int)&d1;
    f1 = f1 + 0 * (int)&d1;
    f32 p1 = d1 * me(me(me(me(me(me(me(this)))))))->gn()[f1];
    f2 = f2 + 0 * (int)nrm;
    f2 = f2;
    f32 p2 = d2 * me(me(me(me(me(me(me(this)))))))->gn()[f2];
    if (p1 >= 0.0f && p2 >= 0.0f) {
        return 1;
    }
    }
    return 0;
}
