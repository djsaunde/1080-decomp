/* IDO 7.1 NCC (C++) NON_MATCHING donor for game_uso_func_000028C0 (0x3E8 / 250 words).
 *
 * Camera-homing update (sibling of 1DDC / 24BC / 2CC8): key 1 homes the
 * output position onto the target body's matrix translation plus the
 * rotated local offset from 023D4 (returned BY VALUE, copied into r);
 * keys 2/3 rotate a (0, 0, -/+1000) probe by the body matrix and home onto
 * obj->pos + probe. Both arms: d = goal - pos, len = sqrtf(|d|^2),
 * normalize(d), d *= len * k, accumulate into out+0x30, pos = out+0x60.
 *
 * agent-g run 119 rewrite (cfe NM body was 69%): same instruction count and
 * every NCC temp/home in place (frame 232, the out/tgt homes come from the
 * Ctx aggregate whose inline ctor takes its address; the frame-top named
 * homes r/p/q/d/len, the operator- ctor temp at 120 and return slot at 140
 * all match). RESIDUAL: integer reloads of out/tgt colour t-regs/a0 where the
 * target uses v1 (uopt live-range split), the operator- result colours
 * rotate f12/f0/f2 vs the target's f0/f2/f12, and the else-arm probe temp
 * sits 4 bytes high. Spliced into the NON_MATCHING object only; the ROM
 * keeps INCLUDE_ASM. docs/IDO_CODEGEN.md#kit-xxi-agent-g-run-119. */
struct V3 {
    float x, y, z;
    V3() {}
    V3(float a, float b, float c) { set(a, b, c); }
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    V3 &operator+=(const V3 &v) { x += v.x; y += v.y; z += v.z; return *this; }
    V3 &operator-=(const V3 &v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
    void operator*=(float s) { x *= s; y *= s; z *= s; }
    V3 operator-(const V3 &v) const { return V3(x - v.x, y - v.y, z - v.z); }
    float lenSq() const { return x * x + y * y + z * z; }
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
struct Out { char pad[0x30]; char acc[0x30]; float gx, gy, gz; char pad6c[0x34]; float px, py, pz; };
struct Body { char pad[0x70]; M4 mtx; };
struct Src;
struct Self { char pad[0x14]; Out *out; char pad18[0x38 - 0x18]; Body *obj; Src *src; int key; char pad44[0x5C - 0x44]; float k; };
struct Src { char pad[0x38]; Body *body; };
extern "C" {
V3 game_uso_func_000023D4(Src *a1);
float game_uso_func_070238(float);
void game_uso_func_071028(V3 *);
void game_uso_func_072EE8(void *, V3 *);
}

struct Ctx { Body *tgt; Out *out; Ctx(Self *s) { out = s->out; tgt = s->src->body; } };
extern "C" void game_uso_func_000028C0(Self *a0) {
    Ctx c(a0);
    int key;
    V3 r;
    V3 p;
    V3 q;
    V3 d;
    float len;
    key = a0->key;
    if (key == 0) return;
    if (key == 1) {
        p.x = c.out->px; p.y = c.out->py; p.z = c.out->pz;
        q.x = c.tgt->mtx.m[3][0]; q.y = c.tgt->mtx.m[3][1]; q.z = c.tgt->mtx.m[3][2];
        r = game_uso_func_000023D4(a0->src);
        q += r;
        d = q - p;
        len = game_uso_func_070238(d.lenSq());
        game_uso_func_071028(&d);
        float f = len * a0->k; d.x *= f; d.y *= f; d.z *= f;
        game_uso_func_072EE8(c.out->acc, &d);
        c.out->px = c.out->gx;
        c.out->py = c.out->gy;
        c.out->pz = c.out->gz;
    } else {
        int zz; zz = 0; /* dead block-scope int: one pool slot (else homes) */
        V3 p;
        V3 q;
        V3 v;
        float len;
        p.x = c.out->px; p.y = c.out->py; p.z = c.out->pz;
        a0->obj->mtx.getPos(q);
        v = V3(0.0f, 0.0f, a0->key == 3 ? -1000.0f : 1000.0f);
        c.tgt->mtx.rotate(v);
        q += v;
        q -= p;
        len = game_uso_func_070238(q.lenSq());
        game_uso_func_071028(&q);
        float f2 = len * a0->k; q.x *= f2; q.y *= f2; q.z *= f2;
        game_uso_func_072EE8(c.out->acc, &q);
        c.out->px = c.out->gx;
        c.out->py = c.out->gy;
        c.out->pz = c.out->gz;
    }
}
