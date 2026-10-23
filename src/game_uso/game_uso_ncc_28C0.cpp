/* IDO 7.1 NCC (C++) NON_MATCHING donor for game_uso_func_000028C0 (0x3E8 / 250 words).
 *
 * Camera-homing update (sibling of 1DDC / 24BC / 2CC8): key 1 homes the
 * output node onto the target node's matrix translation plus the rotated
 * local offset from 023D4; keys 2/3 rotate a (0, 0, -/+1000) probe by the
 * target matrix and home onto obj translation + probe. Both arms: d = goal -
 * pos, len = sqrtf(|d|^2), normalize(d), d *= len * k, accumulate into
 * node+0x30, node translation = node+0x60.
 *
 * agent-g run 131 (72 -> 4 words): out, tgt and obj are ONE node type (the
 * output "px/py/pz" at 0xA0 is the translation row of the M4 at 0x70), so
 * the else arm's dead int slot is really a named Node pointer reused for the
 * head load AND the rotate base; that one uopt candidate takes v1 like the
 * target. Accessor results go through edg's pooled temporary (v0).
 * RESIDUAL (4 words): both 072EE8 arguments (lw v1 / addiu a0,v1,48) come
 * out as the pooled temp in v0; routing them through the named pointer makes
 * it outrank the pooled temp and flips every v0/v1 (29-31 words).
 * Spliced into the NON_MATCHING object only; the ROM keeps INCLUDE_ASM.
 * docs/IDO_CODEGEN.md kit XLII (agent-g run 131). */
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
struct Node { char pad[0x30]; char acc[0x30]; float gx, gy, gz; char pad6c[4]; M4 mtx; };
struct Src;
struct Self { char pad[0x14]; Node *out; char pad18[0x38 - 0x18]; Node *obj; Src *src; int key; char pad44[0x5C - 0x44]; float k; };
struct Src { char pad[0x38]; Node *body; };
extern "C" {
V3 game_uso_func_000023D4(Src *a1);
float game_uso_func_070238(float);
void game_uso_func_071028(V3 *);
void game_uso_func_072EE8(void *, V3 *);
}
struct Ctx { Node *tgt; Node *out; Ctx(Self *s) { out = s->out; tgt = s->src->body; }
    Node *getOut() const { return out; } Node *getTgt() const { return tgt; } };
extern "C" void game_uso_func_000028C0(Self *a0) {
    Ctx c(a0);
    int key;
    V3 r; V3 p; V3 q; V3 d;
    float len;
    key = a0->key;
    if (key == 0) return;
    if (key == 1) {
        p.x = c.getOut()->mtx.m[3][0]; p.y = c.getOut()->mtx.m[3][1]; p.z = c.getOut()->mtx.m[3][2];
        q.x = c.getTgt()->mtx.m[3][0]; q.y = c.getTgt()->mtx.m[3][1]; q.z = c.getTgt()->mtx.m[3][2];
        r = game_uso_func_000023D4(a0->src);
        q += r;
        d = q - p;
        len = game_uso_func_070238(d.x * d.x + d.y * d.y + d.z * d.z);
        game_uso_func_071028(&d);
        float f = len * a0->k; d.x *= f; d.y *= f; d.z *= f;
        game_uso_func_072EE8(c.getOut()->acc, &d);
        c.out->mtx.m[3][0] = c.out->gx; c.out->mtx.m[3][1] = c.out->gy; c.out->mtx.m[3][2] = c.out->gz;
    } else {
        Node *o;
        {
        V3 p; V3 q; V3 v;
        float len;
        o = c.getOut(); p.x = o->mtx.m[3][0]; p.y = o->mtx.m[3][1]; p.z = o->mtx.m[3][2];
        a0->obj->mtx.getPos(q);
        v = V3(0.0f, 0.0f, a0->key == 3 ? -1000.0f : 1000.0f);
        o = c.getTgt(); o->mtx.rotate(v);
        q += v;
        q -= p;
        len = game_uso_func_070238(q.x * q.x + q.y * q.y + q.z * q.z);
        game_uso_func_071028(&q);
        float f2 = len * a0->k; q.x *= f2; q.y *= f2; q.z *= f2;
        game_uso_func_072EE8(c.getOut()->acc, &q);
        c.out->mtx.m[3][0] = c.out->gx; c.out->mtx.m[3][1] = c.out->gy; c.out->mtx.m[3][2] = c.out->gz;
        }
    }
}
