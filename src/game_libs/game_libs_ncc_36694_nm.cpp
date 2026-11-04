/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00036694 (0x2C8 / 178 words).
 * agent-c run 182: rewritten as real C++ (was the run-134 C lift under NCC + one parameter empty test, 84.03):
 * `V3 Obj::perp(V3 p, V3 n)` (renamed to gl_func_00036694 with objcopy; a0 = hidden return, a1 = unused this):
 * the in-plane component t = p - n * n.dot(p) through by-value V3 operators (the target's A -> B -> C copy chains
 * are operator*'s V3 result / return temporary / operator-'s by-value parameter), t block-scoped (its frame slot
 * sits low like the target's sp+92), flipped to z >= 0 and normalised, else the fallback axis. 84.03 -> 84.80
 * standalone, length 704 -> 716 (target 712: distance 8 -> 4). The dot product written out on the members (n * (n.x * p.x
 * + ...)) instead of n.dot(p): -> 85.80 standalone, length unchanged.
 * agent-c run 190 (lever-sweep + order grid): the flip's negations in y, z, x order and the fallback axis's
 * zero stores y before z: 85.80 -> 85.95 standalone, length unchanged.
 * agent-c run 201 (sibling port): the V3 kit of the exact 64388 donor (P3 base, ctor through set(), FREE
 * operators on const V3 & parameters, a named `f32 k = s` in operator/ only): 85.95 -> 87.90 standalone,
 * length 716 -> 712 = target.
 * agent-c run 245: 96-variant operand / term order grid on the dot product (commutations only): the z term first
 * and the x product written p.x * n.x: 90.68 -> 90.96, length unchanged.
 * Still NON_MATCHING: build/non_matching/ only. */
typedef float f32;
extern "C" {
extern f32 gl_sqrtf_36694(f32);
}
struct P3 { f32 x, y, z; };
struct V3 : P3 {
    V3() {}
    V3(f32 a, f32 b, f32 c) { set(a, b, c); }
    void set(f32 a, f32 b, f32 c) { x = a; y = b; z = c; }
};
/* run 201 grid: operator- through a named, set()-filled result and operator* with the named k (frame 0xE8 -> 0xF0 = target): 89.21 -> 90.61 standalone */
/* agent-c run 265 (kit CCCV): operator- / operator* plain V3(...) returns (no named r / k): the product's s parameter is
 * the only float pool entry, so the result / inner temps sit at M-52 / M-68 like the target */
inline V3 operator-(const V3 &a, const V3 &b) { return V3(a.x - b.x, a.y - b.y, a.z - b.z); }
inline V3 operator*(const V3 &a, f32 s) { return V3(a.x * s, a.y * s, a.z * s); }
inline V3 operator/(const V3 &a, f32 s) { f32 k = s; return V3(a.x / k, a.y / k, a.z / k); }
extern "C" { extern void gl_normalize_36694(V3 *); }
struct Obj { V3 perp(V3 p, V3 n); };
V3 Obj::perp(V3 p, V3 n) {
    /* agent-c run 265 (kit CCCV, edg frame order = declaration order at block entry): the target's first frame word M-12
     * is a V3 local holding n * dot (operator-'s subtrahend at sp+228), then r (M-24), l (M-28), px / py / pz; the
     * difference bound to a const reference in a block; flip in x, y, z order. 90.96 -> 94.00, length 712 = target.
     * Residual: the target's sub copy goes through a FRESH entry M-100 (shared with the first return's value temp) and
     * the bound difference sits at M-148; ours reuses the product's M-52 return temp and binds at M-136. */
    V3 a = n * (n.z * p.z + (p.x * n.x + n.y * p.y)); /* run 265: 96-variant dot order grid on the new layout, 94.00 -> 94.05 */
    V3 r;
    f32 l;
    f32 px, py, pz;
    { const V3 &t = p - a; px = t.x; py = t.y; pz = t.z; }
    l = px * px + py * py + pz * pz;
    if (0.0f < l) {
        if (pz < 0.0f) {
            px = px * -1.0f;
            py = py * -1.0f;
            pz = pz * -1.0f;
        }
         r.x = px; r.y = py; r.z = pz;
         return (r * 1.0f) / gl_sqrtf_36694(l);
    }
    if (n.z == 1.0f) {
        r.x = 1.0f;
        r.y = 0.0f; if (1) {} /* agent-c run 201 lever-sweep bbsplit on the sibling-kit body: 87.90 -> 89.21 standalone */
        r.z = 0.0f;
    } else {
        r.z = 0.0f;
        r.x = -n.y;
        r.y = n.x;
        gl_normalize_36694(&r);
    }
    return r;
}
