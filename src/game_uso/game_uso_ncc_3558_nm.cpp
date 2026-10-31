/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for game_uso_func_00003558
 * (0x3F0 / 252 words). NM object only (build/non_matching); the ROM keeps
 * INCLUDE_ASM. History: agent-g run 160 C++ rewrite (Vec3 value class, BY-VALUE
 * operator= word copy into the param temp, operator- with a named return
 * r -> return temp -> binding temp -> e, M4::getPos, Ctx node accessor homed at
 * sp+252 and reloaded before both 072EE8 calls), agent-c run 201 (op= z, y, x),
 * agent-g runs 180-204 (decl order, named block pointers): 85.86.
 *
 * agent-g run 214 (structural pass, kit CCIX): 85.86 -> 88.56.
 * 1. Vec3 has NO constructors. Every object of a class with an inline
 *    `Vec3() {}` (and the Ctx(S *) ctor) costs edg a labelled basic block;
 *    uopt's callee-saved first-use cost grows with the BB count (0.25 x BBs
 *    above 16), and above ~19 BBs the target's ONE zero held in callee-saved
 *    $f20 (mtc1 once; side x/z, w.y store + reload, the three live else muls)
 *    is split and rematerialised per site instead. The old donor had 25 BBs,
 *    this one 15: the f20 web, the `sdc1 $f20` save and the w.y reload now
 *    come out as in the target.
 * 2. Frame homes fitted top-down to the target's map: c 252, va 240, vb 228,
 *    e 216, three function-scope scalars (k, zero, k2), stage 192, mag 188,
 *    w 176, t 172, tmp 140, r 104, bt 76, side 56. The matrix block is a
 *    named-pointer block with a `const Vec3 &v = stage` reference and named
 *    x / y / z (the target's 6 temp words between tmp and r, and the sp+40/44
 *    spill juggle with mixed b0+112 / b0+128 then `addiu v0,v0,112`
 *    addressing); op- computes into named a / b / c after r (the 4 words
 *    between r and bt).
 * 3. vb (and va) are uopt REGISTER candidates in the target: vb is reloaded
 *    into f14 / f16 / f18 at the top of the matrix block and each row is
 *    added as it completes. A member call on vb (`vb - va`, getPos(vb))
 *    takes its address through an edg this / reference temp and keeps it in
 *    memory; direct field stores plus a free by-value `mk(vb.x - va.x, ...)`
 *    (named return r -> return temp -> binding temp -> e, the same copy chain)
 *    make both vreg, frame 256 (88.56 -> 89.21 with one `if (1) {}` after the
 *    stage copy).
 * RESIDUAL: the target stores vb after its init and reloads it in the block
 * (split LR; ours keeps it in f14..f18 throughout), va.y is held in f20 until
 * the block (target reloads va from 240..248), tmp at 148 vs 140. */
template <class T> inline T *me(T *p) { return p; } /* agent-c run 253: zero-code accessor (kit CCLXXXIV) */
struct Vec3 {
    float x, y, z;
    void operator=(Vec3 v) { x = v.x; y = v.y; z = v.z; } /* agent-g run 245: x, y, z source order (uopt emits the stores z, y, x like the target now): 91.56 -> 91.58 */
    Vec3 operator-(const Vec3 &v) const { Vec3 r; float a = x - v.x; float b = y - v.y; float c = z - v.z; r.x = a; r.y = b; r.z = c; return r; }
    void operator+=(const Vec3 &v) {  x += v.x;
 z += v.z;
 y += v.y;  }
    void operator*=(float s) { x = x * s; y = y * s; z = z * s; }
    float len2x() const { float r = x * x + y * y + z * z; return r; } /* agent-c run 248: e's length in x, y, z order (w keeps len2): 93.484 -> 93.492 */
    float len2() const { return y * y + x * x + z * z; } /* agent-g run 259: the named result moved out to the caller's function-scope l (see below); agent-g run 231 (operand-commute sweep, 3 rounds): y first, z row m22 term second, k2 = f118 * (..): 89.98 -> 91.56 */
};
inline Vec3 mk(float a, float b, float c) { Vec3 r; float p = a; float q = b; float u = c; r.x = p; r.z = u; r.y = q; return r; } /* agent-g run 245: x, z, y store order (91.58 -> 91.62) */
struct M4 {
    float m[4][4];
    void getPos(Vec3 &v) const { v.x = m[3][0]; v.y = m[3][1]; v.z = m[3][2]; } /* agent-c run 248: the BB split moved out to before the vb getPos call */
};
struct Node { char pad[0x30]; char f30[0x70]; Vec3 pos; };
struct Body { char pad[0x70]; M4 mtx; };
struct S {
    char pad0[0x14]; Node *node;
    char pad18[0x3C - 0x18]; Vec3 v3C;
    Body *b0; Body *b1;
    char pad50[0x68 - 0x50]; Vec3 v68;
    char pad74[0x88 - 0x74]; float f88;
    char pad8C[0xA0 - 0x8C]; float fA0;
    char padA4[0xB8 - 0xA4]; float fB8;
    char padBC[0x118 - 0xBC]; float f118;
    char pad11C[0x130 - 0x11C]; float f130;
};
extern "C" {
float game_uso_func_070238(float);
void game_uso_func_071028(Vec3 *);
void game_uso_func_072EE8(void *, Vec3 *);
}
struct Ctx { Node *out; Node *getOut() const { return out; } };
extern "C" void game_uso_func_00003558(S *s0) {
    Ctx c = { s0->node };
    Vec3 va;
    Vec3 vb;
    Vec3 e;
    float k;
    float zero;
    float k2;
    Vec3 stage;
    float mag;
    Vec3 w;
    float t;
    float l;
    { Node *o = c.getOut(); va.x = o->pos.x; va.y = o->pos.y; va.z = o->pos.z; }
    if (1) {} s0->b0->mtx.getPos(vb); /* agent-c run 248: `if (1) {}` before this call instead of inside getPos (between v.x and v.y, both getPos uses): 92.60 -> 93.47 standalone. agent-c run 247: vb through getPos (address taken = memory-resident like the target's stores after init), no `if (1) {}` after the stage copy, x / y / z adds and `e = vb - va`: 91.62 at 956 B -> 92.60 at 1008 B = target length */
    zero = 0.0f;
    stage = s0->v68;
    {
        M4 *m = &s0->b0->mtx;
        const Vec3 &v = stage;
        float x = m->m[0][0] * v.x + m->m[1][0] * v.y + m->m[2][0] * v.z;
        float y = m->m[0][1] * v.x + m->m[1][1] * v.y + m->m[2][1] * v.z;
        float z = m->m[0][2] * v.x + m->m[2][2] * v.z + m->m[1][2] * v.y;
        stage.x = x; vb.x += x; stage.y = y; vb.y += y; stage.z = z; vb.z += z; /* agent-c run 248: adds interleaved in the block (93.47 -> 93.48) */
    }
    e = vb - va;
    s0->v3C = e;
    mag = game_uso_func_070238(e.len2x());
    {
        Vec3 side;
        k = e.y * s0->f130;
        side.x = zero; side.y = k; side.z = zero; game_uso_func_072EE8(c.getOut()->f30, &side); /* agent-g run 228: one line with the call (kit CCXXXIV): 89.21 -> 89.98 */
    }
    game_uso_func_071028(&e);
    me(s0)->b1->mtx.getPos(w); /* agent-c run 253: accessor sweep (kit CCLXXXIV): 93.49 -> 93.61 */
    w.x = w.x - va.x;
    w.z = w.z - va.z;
    w.y = zero;
    l = w.len2(); /* agent-g run 259 (kit CCXCIII): len2's named r was an inline-expansion home at the frame bottom (every low object 8 above the target); as a function-scope l declared after t its home sits in the decl region instead -- the sp-offset set now equals the target's (93.61 -> 93.68 standalone) */
    t = game_uso_func_070238(l);
    game_uso_func_071028(&w);
    if (s0->f88 < t) {
        w *= t - s0->f88;
    } else if (t < s0->fA0) {
        w *= t - s0->fA0;
    } else {
        w *= 0.0f;
    }
    k2 = s0->f118 * (mag - s0->fB8);
    e *= k2;
    e += w;
    game_uso_func_072EE8(c.getOut()->f30, &e);
}
