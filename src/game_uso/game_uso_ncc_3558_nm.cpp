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
 * RESIDUAL: frame 264 vs 256 -- every home matches from the top but ugen
 * reserves two more spill words (40..55 vs 40..47); the target loads vb into
 * f14 / f16 / f18 at the top of the matrix block and adds each row as it
 * completes (ours loads vb after the stage stores). */
struct Vec3 {
    float x, y, z;
    void operator=(Vec3 v) { z = v.z; y = v.y; x = v.x; }
    Vec3 operator-(const Vec3 &v) const { Vec3 r; float a = x - v.x; float b = y - v.y; float c = z - v.z; r.x = a; r.y = b; r.z = c; return r; }
    void operator+=(const Vec3 &v) {  x += v.x;
 z += v.z;
 y += v.y;  }
    void operator*=(float s) { x = x * s; y = y * s; z = z * s; }
    float len2() const { float r = x * x + y * y + z * z; return r; }
};
struct M4 {
    float m[4][4];
    void getPos(Vec3 &v) const { v.x = m[3][0]; if (1) {} v.y = m[3][1]; v.z = m[3][2]; }
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
    { Node *o = c.getOut(); va.x = o->pos.x; va.y = o->pos.y; va.z = o->pos.z; }
    { Body *b = s0->b0; b->mtx.getPos(vb); }
    zero = 0.0f;
    stage = s0->v68;
    {
        M4 *m = &s0->b0->mtx;
        const Vec3 &v = stage;
        float x = m->m[0][0] * v.x + m->m[1][0] * v.y + m->m[2][0] * v.z;
        float y = m->m[0][1] * v.x + m->m[1][1] * v.y + m->m[2][1] * v.z;
        float z = m->m[0][2] * v.x + m->m[1][2] * v.y + m->m[2][2] * v.z;
        stage.x = x; stage.y = y; stage.z = z;
    }
    vb.x += stage.x; vb.z += stage.z; vb.y += stage.y;
    e = vb - va;
    s0->v3C = e;
    mag = game_uso_func_070238(e.len2());
    {
        Vec3 side;
        k = e.y * s0->f130;
        side.x = zero; side.y = k; side.z = zero;
        game_uso_func_072EE8(c.getOut()->f30, &side);
    }
    game_uso_func_071028(&e);
    s0->b1->mtx.getPos(w);
    w.x = w.x - va.x;
    w.z = w.z - va.z;
    w.y = zero;
    t = game_uso_func_070238(w.len2());
    game_uso_func_071028(&w);
    if (s0->f88 < t) {
        w *= t - s0->f88;
    } else if (t < s0->fA0) {
        w *= t - s0->fA0;
    } else {
        w *= 0.0f;
    }
    k2 = (mag - s0->fB8) * s0->f118;
    e *= k2;
    e += w;
    game_uso_func_072EE8(c.getOut()->f30, &e);
}
