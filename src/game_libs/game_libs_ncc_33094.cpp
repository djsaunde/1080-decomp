/* IDO 7.1 NCC (C++) donor for gl_func_00033094 (0x194 / 101 words): EXACT, REPLACE_FUNC_BODY-spliced over the
 * placeholder in game_libs_post.c on both build paths (agent-g run 289; was game_libs_ncc_33094_nm.cpp, NM 94.31).
 * Camera look-at update: eye = o->pos; target = the parent's pos, or o->pos + o->v104 * 10; both scaled by the
 * module vector at USO address 0x128 (D_33094_k), then emitted with the up vector o->v140; two optional calls keyed
 * by o->x130.
 * agent-c run 202: real C++ on the exact 432BC donor's V3 kit (was the run-134 C lift under NCC, 87.65): a = o->pos
 * and b = p->pos through set(f32, f32, f32) (edg evaluates the arguments right to left: the target's z, y, x loads
 * into registers before the stores); the else arm `b = o->v104; b *= 10; b += a;` with a BY-VALUE operator= (the
 * word copy into the sp+0x54 parameter temp, then set(const V3 &)) -- uopt forwards b.z and keeps the b.x / b.y
 * stores the target has; a and b then scaled by the k vector through mul(const V3 &). The unused volatiles sit in
 * a block at the end (frame 0x90).
 * agent-g run 289 (the last two residuals, the else arm's a.x reload and the 10.0f colour): the scale factor is
 * a named local `f32 ten = 10.0f;`. As a literal argument the constant took f14 and a.x stayed in f12. As a named
 * local its LR is coloured f12, so a.x is reloaded from its home (target `lwc1 $f16,120(sp)`, `mul.s ...,$f12`).
 * The operator+= order was then re-gridded (72: *= x y z, += x y z): 94.31 -> 100. */
typedef float f32;
struct V3 { f32 x, y, z;
    /* agent-g run 266: member-order grid over set / *= / += (216 orders); run 289: += re-gridded to x y z */
    void set(const V3 &v) { x = v.x; y = v.y; z = v.z; }
    void operator=(V3 v) { set(v); }
    void operator*=(f32 s) { x *= s; y *= s; z *= s; }
    void operator+=(const V3 &o) { x = x + o.x; y = y + o.y; z = z + o.z; }
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
        if (1) {}
    } else {
        b = o->v104;
        f32 ten = 10.0f; /* agent-g run 289: named scale factor (its LR takes f12; a.x then reloads from its home) */
        b *= ten;
        b += a;
        if (a.y != a.z) {} /* agent-g run 262 (kit CCXCVII priorities): code-free test + the if-arm if (1) colour a = target (x f12 / y f0 / z f2): 93.65 -> 94.20 */
    }
    V3 *k = (V3 *)D_33094_k;
    a.mul(*k);
    b.mul(*k);
    gl_33094_emit((char *)o + 0xB4, a.x, a.y, a.z, b.x, b.y, b.z, o->v140.x, o->v140.y, o->v140.z);
    if (o->x130 != 0) {
        gl_func_00000000((char *)o + 0xB4, 1, o->x138, 0);
        gl_func_00000000((char *)o + 0xB4, 0, o->x134, 0);
    }
    { volatile int q0, q1, q2; } /* agent-g run 256: a third volatile moves the o+0xB4 spill 60 -> 56 = target (frame 0x90 kept): 93.62 -> 93.65 */
    return 1;
}
