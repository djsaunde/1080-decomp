/* IDO 7.1 NCC (C++) NM-only comparison donor for game_libs_func_0004FD00 (0x3EC / 251 words), agent-c run 175.
 * Real C++ `int Col::side(int f1, int f2)` (renamed with objcopy): the collision record of the 3B6A0 donor (faces at
 * +0x68, s16 vertex triples at +0x60, face normals at +0x54). Sums each face's three vertices in a short vector
 * (an inline `Col::sum(V3 &, int)` with its own local SV -- the target's double `move` on the forwarded third add is
 * that inline local), converts to float through `V3::set(const SV &)`, then mid = a; mid += b; mid /= 2 and
 * d = mid - a / mid - b through a by-value `V3::set(V3)` (the target's lw/sw copy into the sp+20 temp followed by
 * member stores), both dots named before the test, `return 1` / `return 0`.
 * cfe body 80.29 -> 95.51, length 972 -> 1004 = target.
 * Residual: the two inline SV locals get two homes (target shares one at sp+68), the &temp / &mid / 12 constants
 * are not hoisted to the entry, and `li v0,0` is scheduled late.
 * Still NON_MATCHING: build/non_matching/ only. */
typedef float f32; typedef short s16; typedef unsigned short u16;
template <class T> inline T *me(T *p) { return p; } /* zero-code accessor (kit CCLXXXIV) */
struct SV { s16 x, y, z;
    void set(const SV &o) { x = o.x; y = o.y; z = o.z; }
    SV &operator+=(const SV &o) { x = x + o.x; y = y + o.y; z = z + o.z; return *this; } };
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
    void sum(V3 &o, int f, SV &s) { s.set(verts[me(this)->faces[f].vi[0]]); s += verts[faces[f].vi[1]]; s += verts[faces[f].vi[2]]; o.set(s); }
    int side(int f1, int f2);
};
int Col::side(int f1, int f2) {
    V3 a, b, mid, d1, d2;
    SV s;
    sum(a, f1, s);
    sum(b, f2, s);
    mid.set(a);
    mid += b;
    mid /= 2.0f;
    d1.set(mid);
    d1 -= a;
    d2.set(mid);
    d2 -= b;
    f32 p1 = d1 * me(me(this))->gn()[f1];
    f32 p2 = d2 * me(me(this))->nrm[f2];
    if (p1 >= 0.0f && p2 >= 0.0f) {
        return 1;
    }
    return 0;
}
