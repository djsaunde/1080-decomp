/* IDO 7.1 NCC (C++) donor for game_uso_func_00002CC8 (0x300 / 192 words).
 *
 * Camera position snap (sibling of 1DDC / 24BC / 28C0 / BB8C): key 0 does
 * nothing; key 1 puts the output position at the target body's matrix
 * translation + the rotated local offset from 023D4 (returned BY VALUE as a
 * POD P3) minus vel / self->k; keys 2/3 put it at obj->pos + body-rotated
 * (0, 0, -/+1000). Both arms write the result to out+0x60 and out+0xA0.
 *
 * agent-g run 129, written from scratch (the cfe NM body was 190 raw words):
 *  - out / tgt live in a Ctx aggregate (stored at entry, sp+212 / 208) and
 *    are read through inline accessors getOut() / getTgt(): the accessor's
 *    result is an edg temporary, which uopt colours v0 at each use (the
 *    target's `lw v0,212(sp)` reloads); reading c.out / c.tgt directly gives
 *    ugen t-register loads and shifts the whole t-ring by one.
 *  - 023D4 returns a POD P3, so `r = f()` is a plain word copy (ret temp
 *    sp+140 -> r sp+192), while V3 has a BY-VALUE void operator=, so
 *    `d = *vel / k` goes ret temp -> param temp (sp+84) -> float stores to d
 *    (sp+168), the target's interleaved copy chain.
 *  - `V3 v = V3(0, 0, +-1000)` (copy-init from a ctor temp, word copy
 *    sp+64 -> sp+40); `V3 v(...)` builds in place and drops the copy.
 *  - Frame 216: both arms open with a dead `int` and a nested block (as in
 *    28C0: edg places a block's temporaries before its named locals).
 * Spliced into game_uso.c.o via REPLACE_FUNC_BODY
 * (docs/MATCHING_WORKFLOW.md#game-uso-ncc-cplusplus-donor-recipe). */
struct P3 { float x, y, z; };
struct V3 {
    float x, y, z;
    V3() {}
    V3(float a, float b, float c) { set(a, b, c); }
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    V3 &operator+=(const V3 &v) { x += v.x; y += v.y; z += v.z; return *this; }
    V3 &operator+=(const P3 &v) { x += v.x; y += v.y; z += v.z; return *this; }
    V3 &operator-=(const V3 &v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
    void operator=(V3 v) { x = v.x; y = v.y; z = v.z; }
    V3 operator/(float s) const { return V3(x / s, y / s, z / s); }
};
struct M4 {
    float m[4][4];
    void rotate(V3 &v) const {
        float x = m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z;
        float y = m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z;
        float z = m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z;
        v.x = x; v.y = y; v.z = z;
    }
    void getPos(V3 &v) const { v.x = m[3][0]; v.y = m[3][1]; v.z = m[3][2]; }
};
struct Out {
    char pad[0x60]; float gx, gy, gz; char pad6c[0x34]; float px, py, pz;
    void setG(const V3 &v) { gx = v.x; gy = v.y; gz = v.z; }
    void setP(const V3 &v) { px = v.x; py = v.y; pz = v.z; }
};
struct Body { char pad[0x70]; M4 mtx; };
struct Src;
struct Self { char pad[0x14]; Out *out; char pad18[0x38 - 0x18]; Body *obj; Src *src; int key; char pad44[0x5C - 0x44]; float k; };
struct Src { char pad[0x38]; Body *body; };
extern "C" {
P3 game_uso_func_000023D4(Src *a1);
}
struct Ctx {
    Body *tgt; Out *out;
    Ctx(Self *s) { out = s->out; tgt = s->src->body; }
    Out *getOut() const { return out; }
    Body *getTgt() const { return tgt; }
};
extern "C" void game_uso_func_00002CC8(Self *a0, V3 *vel) {
    Ctx c(a0);
    int key;
    P3 r;
    V3 q;
    V3 d;
    key = a0->key;
    if (key == 0) return;
    if (key == 1) {
        int yy; yy = 0;
        {
        q.x = c.getTgt()->mtx.m[3][0]; q.y = c.getTgt()->mtx.m[3][1]; q.z = c.getTgt()->mtx.m[3][2];
        r = game_uso_func_000023D4(a0->src);
        q += r;
        d = *vel / a0->k;
        q -= d;
        c.getOut()->setG(q);
        c.getOut()->setP(q);
        }
    } else {
        int zz; zz = 0;
        {
        V3 p;
        a0->obj->mtx.getPos(p);
        V3 v = V3(0.0f, 0.0f, a0->key == 3 ? -1000.0f : 1000.0f);
        c.getTgt()->mtx.rotate(v);
        p += v;
        c.getOut()->setG(p);
        c.getOut()->setP(p);
        }
    }
}
