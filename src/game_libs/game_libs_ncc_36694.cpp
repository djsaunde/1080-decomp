/* IDO 7.1 NCC (C++) donor for gl_func_00036694 (0x2C8 / 178 words), EXACT (agent-g run 401, kit CDLXXXIV).
 * `V3 Obj::perp(V3 p, V3 n)` (renamed to gl_func_00036694 with objcopy; a0 = hidden return, a1 = unused this):
 * r = p - n * dot(n, p) through by-value V3 operators; if |r|^2 > 0, r flipped to z >= 0 and divided by sqrt(|r|^2),
 * else the fallback axis (1, 0, 0) or the normalised (-n.y, n.x, 0).
 * History (agent-c runs 134-312, agent-g run 313): the 64388 V3 kit (P3 base, free operators on const V3 &),
 * the target's frame order (kit CCCVI), operand / store-order grids: 84.03 -> 96.08.
 * agent-g run 398 (kit CDLXXXIII), 96.08 -> 99.92: constructor-free V3 (dot product in the entry block, parameter
 * words uncoloured), zero-code `id(p);` (frame slots), six 0.0f uses in the end block (K0 takes f0),
 * `(r * 1.0f) / sqrt((const f32 &)l)` through operator/, whole-function operand-order climb.
 * agent-g run 401 (kit CDLXXXIV), 99.92 -> 100: the residual was r.z in f12 (target f2). zdbug:6 showed the
 * |r|^2 sum as its own expression LR (pri 3.0) taking f2 ahead of r.z. `m = lsq2(r)` through an inline
 * returning f32 makes the compare read m itself (no sum LR); then counted zero-code uses order the colouring
 * r.x / r.y (28 / 29) -> r.z whole (split, 4-7 piece f2) -> v.z (assign's word, f12) -> m (f14): three in the
 * m test (r.x / r.y / r.z vs 0.0f), two in the r.z test (v.z via copy propagation), one 1.0f use in the n.z test. */
typedef float f32;
extern "C" {
extern f32 gl_sqrtf_36694(f32);
}
struct P3 { f32 x, y, z; };
struct V3 : P3 {
    void set(f32 a, f32 b, f32 c) { x = a; y = b; z = c; }
    void set(const P3 &v) { x = v.x; y = v.y; z = v.z; }
    void assign(V3 v) { set(v); }
};
struct V3c : V3 { V3c(f32 a, f32 b, f32 c) { this->set(a, b, c); } };
inline const V3 &id(const V3 &v) { return v; }
inline f32 lsq2(const V3 &r) { return (r.x * r.x + r.y * r.y) + r.z * r.z; }
inline V3 operator-(const V3 &a, const V3 &b) { return V3c(a.x - b.x, a.y - b.y, a.z - b.z); }
inline V3 operator*(const V3 &a, f32 s) { V3c t(a.x * s, a.y * s, a.z * s); return t; }
extern "C" { extern void gl_normalize_36694(V3 *); }
inline V3 operator/(const V3 &a, f32 s) { return V3c(a.x / s, a.y / s, a.z / s); }
struct Obj { V3 perp(V3 p, V3 n); };
V3 Obj::perp(V3 p, V3 n) {
    V3 a;
    V3 r;
    f32 l;
    id(p);
    r.assign(p - (a = id(n) * ((n.x * p.x + n.y * p.y) + n.z * p.z)));
    f32 m = lsq2(r);
    if ((m > 0.0f) | (0 * ((r.y != 0.0f) | (r.x == 0.0f) | (r.z > 0.0f)))) {
        if ((r.z < 0.0f) | (0 * ((r.z != r.x) | (r.z > 0.0f)))) {
            r.x = r.x * -1.0f;
            r.y = r.y * -1.0f;
            r.z = r.z * -1.0f;
        }
        l = m;
        return (r * 1.0f) / gl_sqrtf_36694((const f32 &)l);
    }
    if ((n.z == 1.0f) | (0 * ((p.x != 1.0f)))) {
        r.z = 0.0f;
        r.x = 1.0f; if (1) {}
        r.y = 0.0f;
    } else {
        r.z = 0.0f;
        r.x = -n.y;
        r.y = n.x;
        gl_normalize_36694(&r);
    }
    if ((n.x != 0.0f) | (n.y != 0.0f) | (n.z != 0.0f) | (p.x != 0.0f) | (p.y != 0.0f) | (p.z != 0.0f)) {}
    return r;
}
