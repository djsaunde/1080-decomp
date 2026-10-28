/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00033094 (0x194 / 101 words).
 * agent-c run 202: rewritten as real C++ on the exact 432BC donor's V3 kit (was the run-134 C lift under NCC,
 * 87.65): a = o->pos and b = p->pos through set(f32, f32, f32) (edg evaluates the arguments right to left:
 * the target's z, y, x loads into registers before the stores); the else arm `b = o->v104; b *= 10; b += a;`
 * with a BY-VALUE operator= (the word copy into the sp+0x54 parameter temp, then set(const V3 &)) -- uopt
 * forwards b.z and keeps the b.x / b.y stores the target has; a and b then scaled by the k vector through
 * mul(const V3 &). Operator-body order grid (kit CLXXX): set y, x, z; *= x, z, y; += y, z, x. The two unused
 * volatiles sit in a block at the end (frame 0x90). 87.65 -> 93.46 standalone, length 404 = target.
 * Left: the FP colours of a (target x f12 / y f0 / z f2, ours f0 / f2 / f12) and the else-arm's a.x reload,
 * the parameter temp 8 below the target's sp+0x54. Still NON_MATCHING: build/non_matching/ only. */
typedef float f32;
struct V3 { f32 x, y, z;
    void set(const V3 &v) { y = v.y; x = v.x; z = v.z; }
    void operator=(V3 v) { set(v); }
    void operator*=(f32 s) { x *= s; z *= s; y *= s; }
    void operator+=(const V3 &o) { y = y + o.y; z = z + o.z; x = x + o.x; }
    void set(f32 a, f32 b, f32 c) { x = a; y = b; z = c; }
    void mul(const V3 &k) { x = x * k.x; y = y * k.y; z = z * k.z; }
};
struct Obj { char pad[0xA0]; V3 pos; char padAC[0xF4 - 0xAC]; Obj *p; char padF8[0x104 - 0xF8]; V3 v104;
    char pad110[0x130 - 0x110]; int x130, x134, x138; char pad13C[4]; V3 v140; };
extern "C" {
extern int gl_func_00000000(...);
extern void gl_33094_emit(void *, float, float, float, float, float, float, float, float, float);
extern char D_33094_k[];
}
extern "C" int gl_func_00033094(Obj *o) {
    V3 b;
    V3 a;
    a.set(o->pos.x, o->pos.y, o->pos.z);
    if (o->p != 0) {
        b.set(o->p->pos.x, o->p->pos.y, o->p->pos.z);
    } else {
        b = o->v104;
        b *= 10.0f;
        b += a;
    }
    V3 *k = (V3 *)D_33094_k;
    a.mul(*k);
    b.mul(*k);
    gl_33094_emit((char *)o + 0xB4, a.x, a.y, a.z, b.x, b.y, b.z, o->v140.x, o->v140.y, o->v140.z);
    if (o->x130 != 0) {
        gl_func_00000000((char *)o + 0xB4, 1, o->x138, 0);
        gl_func_00000000((char *)o + 0xB4, 0, o->x134, 0);
    }
    { volatile int q0, q1; }
    return 1;
}
