/* IDO 7.1 NCC (C++) EXACT donor for gl_func_0003B01C (0x17C / 95 words), agent-c run 162.
 * Contact / push-out test: guarded by o->flags bits 1|2; sets bit 2, n = other->pos - o->pos through
 * V3 operator- (ctor temp -> return slot -> by-value operator= parameter -> float copies, the target's
 * three word/float copy hops); r = probe(o); if r < 0: n = o->nrm, normalise(&n), n *= -r, set bit 1.
 * Load-bearing: both position pointers are kept named pointers (`if (0) { p = 0; }`: no offset folding,
 * the target's addiu v1 / v0 bases); a dead `while (0) { b = &other->pos; }` at the top puts the
 * other+0x54 expression first in uopt's itab so the equal-adjsave tie colours it v0 (kit XCIX); twelve
 * top-level volatile pads + `volatile int q` before r + a block-scoped volatile pair at the end give the
 * target's 0xC0 frame and every home (kit CXII: unused volatiles = frame homes, zero code). The cfe body
 * reached 98.63 (FP load / ring order). Spliced over an empty placeholder in both build trees. */
typedef float f32;
struct V3 {
    f32 x, y, z;
    V3() {}
    V3(f32 a, f32 b, f32 c) { x = a; y = b; z = c; }
    V3 operator-(const V3 &o) const { return V3(x - o.x, y - o.y, z - o.z); }
    void operator=(V3 v) { x = v.x; y = v.y; z = v.z; }
    void operator*=(f32 s) { x *= s; y *= s; z *= s; }
};
struct Obj { char pad[0x30]; int flags; char pad34[0x54 - 0x34]; V3 pos; char pad60[0x6C - 0x60]; V3 nrm; };
extern "C" {
void gl_func_00000000(...);
f32 gl_3B01C_f(...);
}
extern "C" int gl_func_0003B01C(Obj *o, Obj *other) {
    volatile int p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11;
    V3 n;
    V3 *b;
    while (0) { b = &other->pos; }
    V3 *a = &o->pos;
    if (0) { a = 0; }
    if (o->flags & 6) {
        return 0;
    }
    o->flags |= 2;
    b = &other->pos;
    if (0) { b = 0; }
    n = *b - *a;
    volatile int q;
    f32 r = gl_3B01C_f(o);
    if (0.0f <= r) {
        return 0;
    }
    n = o->nrm;
    gl_func_00000000(&n);
    n *= -r;
    o->flags |= 1;
    { volatile int z1, z2; }
    return 1;
}
