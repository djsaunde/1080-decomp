/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for game_uso_func_00003558
 * (0x3F0 / 252 words). NM object only (build/non_matching); the ROM keeps
 * INCLUDE_ASM. agent-g run 160: the cfe NM body in game_uso.c emulated C++
 * with Tri3i word copies and pad arrays (72.33, 246 words); this is the same
 * routine written as C++: Vec3 value class with a BY-VALUE operator=
 * (`stage = s0->v68`, `s0->v3C = e`: word copy into the param temp at
 * sp+140, then float stores), an operator- with a named return (r at sp+104
 * -> return temp -> binding temp sp+76 -> e), M4::getPos (the target's
 * `lwc1 160(v0); addiu v0,v0,112; 52/56(v0)` base), and the rotated triple
 * held in the dead mag / t / k before the stage stores, which brings back the
 * sp+40/44 FP juggle of the matrix block (kit LXXXVI).
 * RESIDUAL: the target keeps one zero in $f20 (sdc1 save) for the side /
 * w.y / else-branch muls -- NCC const-propagates every spelling tried
 * (literal, double literal, named float) into per-site mtc1; vb is loaded
 * before the matrix block and held in f14/f16/f18 (explicit vb.x = vb.x + ..
 * statements lose the juggle, 58.7); frame 224 vs 256. getPos keeps only the
 * first if (1) {} (the second cost 0.16: 72.58 -> 72.74). The else factor is a
 * DOUBLE literal (`w *= 0.0;`): the target keeps the three mul.s by zero, a float
 * zero folds them into plain zero stores (72.74 -> 75.62; 1004 -> 1024 bytes, AWAY
 * from 1008 -- the muls are verified target words, the overshoot is elsewhere).
 * agent-g run 180: the node pointer goes through an inline Ctx accessor (a stack object holding
 * s0->node, re-read for each use) as in the 1DDC / 24BC / 28C0 siblings: the target homes it at
 * sp+252 and reloads it (`lw v1,252(sp)`) before both 072EE8 calls (75.62 -> 76.41). */
struct Vec3 {
    float x, y, z;
    Vec3() {}
    Vec3(float a, float b, float c) { x = a; y = b; z = c; }
    void operator=(Vec3 v) { x = v.x; y = v.y; z = v.z; }
    Vec3 operator-(const Vec3 &v) const { Vec3 r; r.x = x - v.x; r.y = y - v.y; r.z = z - v.z; return r; }
    void operator+=(const Vec3 &v) { x += v.x; y += v.y; z += v.z; }
    void operator*=(float s) { x = x * s; y = y * s; z = z * s; }
    float len2() const { float r = x * x + y * y + z * z; return r; } /* run 180: named result (76.41 -> 76.7) */
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
struct Ctx { Node *out; Ctx(S *s) { out = s->node; } Node *getOut() const { return out; } };
extern "C" void game_uso_func_00003558(S *s0) {
    Ctx c(s0);
    Vec3 va;
    { Node *o = c.getOut(); va.x = o->pos.x; va.y = o->pos.y; va.z = o->pos.z; }
    Vec3 vb;
    { Body *b = s0->b0; b->mtx.getPos(vb); } /* run 180: named block pointer (76.73 -> 77.2) */
    Vec3 e;
    Vec3 pad1;
    Vec3 stage;
    float mag;
    Vec3 w;
    float t;
    float k;
    float zero;
    zero = 0.0f;
    stage = s0->v68;
    {
        M4 *m = &s0->b0->mtx;
        mag = m->m[0][0] * stage.x + m->m[1][0] * stage.y + m->m[2][0] * stage.z;
        t = m->m[0][1] * stage.x + m->m[1][1] * stage.y + m->m[2][1] * stage.z;
        k = m->m[0][2] * stage.x + m->m[1][2] * stage.y + m->m[2][2] * stage.z;
        stage.y = t; stage.x = mag; stage.z = k;
    }
    vb += stage;
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
    w.y = zero;
    w.z = w.z - va.z;
    t = game_uso_func_070238(w.len2());
    game_uso_func_071028(&w);
    if (s0->f88 < t) {
        w *= t - s0->f88;
    } else if (t < s0->fA0) {
        w *= t - s0->fA0;
    } else {
        w *= 0.0;
    }
    k = (mag - s0->fB8) * s0->f118;
    e *= k;
    e += w;
    game_uso_func_072EE8(c.getOut()->f30, &e);
}
