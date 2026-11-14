/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00036694 (0x2C8 / 178 words).
 * `V3 Obj::perp(V3 p, V3 n)` (renamed to gl_func_00036694 with objcopy; a0 = hidden return, a1 = unused this):
 * r = p - n * dot(n, p) through by-value V3 operators; if |r|^2 > 0, r flipped to z >= 0 and divided by sqrt(|r|^2),
 * else the fallback axis (1, 0, 0) or the normalised (-n.y, n.x, 0).
 * History (agent-c runs 134-312, agent-g run 313): the 64388 V3 kit (P3 base, free operators on const V3 &),
 * the target's frame order (kit CCCVI), operand / store-order grids: 84.03 -> 96.08.
 * agent-g run 398 (kit CDLXXXIII), 96.08 -> 98.79, all 13 frame slots and the length = target:
 *  - V3 has NO user constructors (the 3-float constructor lives on a derived V3c whose results slice back to V3):
 *    `V3 a; V3 r;` then emit no inline constructor call, so the dot product is in the entry block and uopt leaves
 *    the six parameter-word live ranges uncoloured (the target RELOADS n.x / n.y / n.z for the products);
 *  - a zero-code `id(p);` statement and `id(n)` in the product: the pointer home they take is allocated first,
 *    which puts the product temp / copy / sub chain on the target's sp+172 / 188 / 120 / 140 / 92;
 *  - one OR'd code-free test of six parameter words against 0.0f at the end block: six counted uses of the 0.0f
 *    constant in ONE existing block raise its priority 1.33 -> 3.33, above the |r|^2 sum (3.0), so 0.0f takes f0;
 *  - |r|^2 written z term first (same add order in the output).
 * Residual: the dot product's FP temp FIFO order (ugen), r.z f12 (target f2: the |r|^2 sum LR, pri 3.0, holds a
 * phantom f2), and the three div.s colours of the sqrt branch.
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
inline V3 divsq4(const V3 &a, const f32 &l) { f32 k = gl_sqrtf_36694(l); return V3c(a.x / k, a.y / k, a.z / k); }
struct Obj { V3 perp(V3 p, V3 n); };
V3 Obj::perp(V3 p, V3 n) {
    V3 a;
    V3 r;
    f32 l;
    id(p);
    r.assign(p - (a = id(n) * (n.z * p.z + (p.x * n.x + n.y * p.y))));
    f32 m = r.z * r.z + (r.x * r.x + r.y * r.y);
    if (0.0f < m) {
        if (r.z < 0.0f) {
            r.x = r.x * -1.0f;
            r.y = r.y * -1.0f;
            r.z = r.z * -1.0f;
        }
        l = m;
        return divsq4(r * 1.0f, l);
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
