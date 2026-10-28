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
 * Still NON_MATCHING: build/non_matching/ only. */
typedef float f32;
extern "C" {
extern f32 gl_sqrtf_36694(f32);
}
struct V3 { f32 x, y, z;
    V3() {}
    V3(f32 a, f32 b, f32 c) { x = a; y = b; z = c; }
    V3 operator*(f32 s) const { return V3(x * s, y * s, z * s); }
    V3 operator/(f32 s) const { return V3(x / s, y / s, z / s); }
    V3 operator-(V3 o) const { return V3(x - o.x, y - o.y, z - o.z); }
    f32 dot(const V3 &o) const { return x * o.x + y * o.y + z * o.z; }
};
extern "C" { extern void gl_normalize_36694(V3 *); }
struct Obj { V3 perp(V3 p, V3 n); };
V3 Obj::perp(V3 p, V3 n) {
    f32 px, py, pz;
    { V3 t = p - n * (n.x * p.x + n.y * p.y + n.z * p.z); px = t.x; py = t.y; pz = t.z; }
    f32 l;
    V3 r;
    l = px * px + py * py + pz * pz;
    if (0.0f < l) {
        if (pz < 0.0f) {
            py = py * -1.0f;
            pz = pz * -1.0f;
            px = px * -1.0f;
        }
         r.x = px; r.y = py; r.z = pz;
         return (r * 1.0f) / gl_sqrtf_36694(l);
    }
    if (n.z == 1.0f) {
        r.x = 1.0f;
        r.y = 0.0f;
        r.z = 0.0f;
    } else {
        r.z = 0.0f;
        r.x = -n.y;
        r.y = n.x;
        gl_normalize_36694(&r);
    }
    return r;
}
