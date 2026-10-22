/* IDO 7.1 NCC (C++) NON_MATCHING donor for game_uso_func_000024BC (0x258 / 150 words).
 *
 * Camera-homing update, sibling of 28C0/1DDC (same V3 class kit): key 3
 * snaps out->goal and out->pos to the source body's matrix translation;
 * otherwise q = obj->pos + 023D4(this) (by-value return), n = normalize(XZ
 * of the input vector) * spd, q -= n, q.y += height (key 2 selects 0xF4,
 * else 0xDC), q -= in / (sx, sy, sx), and goal = pos = q.
 *
 * agent-g run 119 rewrite of the 72% cfe body. RESIDUAL: the key-3 arm's
 * pointer reload cadence, two words of length, frame 8 bytes short.
 * Spliced into the NON_MATCHING object only; the ROM keeps INCLUDE_ASM.
 * docs/IDO_CODEGEN.md#kit-xxi-agent-g-run-119. */
struct V3 {
    float x, y, z;
    V3() {}
    V3(float a, float b, float c) { set(a, b, c); }
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    void set(const V3 &v) { x = v.x; y = v.y; z = v.z; }
    V3 &operator+=(const V3 &v) { x += v.x; y += v.y; z += v.z; return *this; }
    V3 &operator-=(const V3 &v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
    void operator*=(float s) { x *= s; y *= s; z *= s; }
};
struct M4 {
    float m[4][4];
    void getPos(V3 &v) const { v.x = m[3][0]; v.y = m[3][1]; v.z = m[3][2]; }
};
struct Out { char pad[0x60]; V3 goal; char pad6c[0x34]; V3 pos; };
struct Body { char pad[0x70]; M4 mtx; };
struct Self { char pad[0x14]; Out *out; char pad18[0x38 - 0x18]; Body *obj; Body *src; int key;
              char pad44[0x94 - 0x44]; float spd; char pad98[0xAC - 0x98]; float sx;
              char padb0[0xDC - 0xB0]; float hA; char pade0[0xF4 - 0xE0]; float hB;
              char padf8[0x10C - 0xF8]; float sy; };
extern "C" {
V3 game_uso_func_000023D4(Self *a1);
void game_uso_func_071028(V3 *);
}
struct Ctx { Out *out; Ctx(Self *s) { out = s->out; } };
extern "C" void game_uso_func_000024BC(Self *a0, V3 *in) {
    Ctx c(a0);
    int key = a0->key;
    if (key == 0) return;
    if (key == 3) {
        V3 m;
        a0->src->mtx.getPos(m);
        Out *o = c.out; o->goal.x = m.x; o->goal.y = m.y; o->goal.z = m.z;
        o = c.out; o->pos.x = m.x; o->pos.y = m.y; o->pos.z = m.z;
    } else {
        V3 r;
        V3 q;
        a0->obj->mtx.getPos(q);
        r = game_uso_func_000023D4(a0);
        q += r;
        V3 n(in->x, 0.0f, in->z);
        game_uso_func_071028(&n);
        n *= a0->spd;
        q -= n;
        q.y += a0->key == 2 ? a0->hB : a0->hA;
        V3 s(in->x / a0->sx, in->y / a0->sy, in->z / a0->sx);
        q -= s;
        c.out->goal.set(q);
        c.out->pos.set(q);
    }
}
