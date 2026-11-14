/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00036694 (0x2C8 / 178 words).
 * `V3 Obj::perp(V3 p, V3 n)` (renamed to gl_func_00036694 with objcopy; a0 = hidden return, a1 = unused this):
 * r = p - n * dot(n, p) through by-value V3 operators; if |r|^2 > 0, r flipped to z >= 0 and divided by sqrt(|r|^2),
 * else the fallback axis (1, 0, 0) or the normalised (-n.y, n.x, 0).
 * History (agent-c runs 134-312, agent-g run 313): the 64388 V3 kit (P3 base, free operators on const V3 &),
 * the target's frame order (kit CCCVI), operand / store-order grids: 84.03 -> 96.08.
 * agent-g run 398 (kit CDLXXXIII), 96.08 -> 98.79 -> 99.92, all 13 frame slots and the length = target:
 *  - V3 has NO user constructors (the 3-float constructor lives on a derived V3c whose results slice back to V3):
 *    `V3 a; V3 r;` then emit no inline constructor call, so the dot product is in the entry block and uopt leaves
 *    the six parameter-word live ranges uncoloured (the target RELOADS n.x / n.y / n.z for the products);
 *  - a zero-code `id(p);` statement and `id(n)` in the product: the pointer home they take is allocated first,
 *    which puts the product temp / copy / sub chain on the target's sp+172 / 188 / 120 / 140 / 92;
 *  - one OR'd code-free test of six parameter words against 0.0f at the end block: six counted uses of the 0.0f
 *    constant in ONE existing block raise its priority 1.33 -> 3.33, above the |r|^2 sum (3.0), so 0.0f takes f0;
 *  - the sqrt branch as `(r * 1.0f) / sqrt((const f32 &)l)` through operator/(const V3 &, f32) (the old
 *    divsq4 shared the product's z parameter home, so its div.s took f14 / f2 / f12; the const-ref view keeps
 *    l in memory for the `lwc1 f12,212(sp)` reload), 98.79 -> 98.96;
 *  - a whole-function operand-order climb (kit CDXXXII: the ugen FP temp FIFO is phased by LATER code):
 *    dot (n.x * p.x + n.y * p.y) + n.z * p.z, |r|^2 (x + y) + z, `m > 0.0f`: 98.96 -> 99.92.
 * Residual: r.z is f12 (target f2) in three instructions. r.z's LR (pri 1.5) is coloured after the |r|^2 sum
 * expression LR (pri 3.0) and the assign parameter word v.z (pri 5.0), which take f2 without using it.
 * Still NON_MATCHING: build/non_matching/ only. */
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
    f32 m = (r.x * r.x + r.y * r.y) + r.z * r.z;
    if (m > 0.0f) {
        if (r.z < 0.0f) {
            r.x = r.x * -1.0f;
            r.y = r.y * -1.0f;
            r.z = r.z * -1.0f;
        }
        l = m;
        return (r * 1.0f) / gl_sqrtf_36694((const f32 &)l);
    }
    if (n.z == 1.0f) {
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
