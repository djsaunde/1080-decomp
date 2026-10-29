/* IDO 7.1 NCC (C++) NON_MATCHING donor for game_uso_func_00008CD8
 * (709 words). NM object only (build/non_matching); the ROM build keeps
 * INCLUDE_ASM.
 *
 * agent-c run 199: rewritten from the cfe emulation (game_uso.c NM body,
 * 86.05) into the 7C1C value-class kit: every `bnez &stack; jal 055750(12)`
 * guard is an edg XZ ctor, every word-copy chain through sp+372 is a
 * by-value return through edg's pooled return slot, and the 0x84 / 0x90
 * stores are by-value operator= calls. Length = target (709 words) on the
 * first build. Spellings from a spelling-grid pass: the normalised
 * directions (dir, e) and f / side / h are XZ copies, d2 / g bind the
 * difference to a const reference (two copies instead of three), and
 * `p = (const Vec3 &)XZ(v)` binds operator= to the base.
 * LEFT: the phase switch, register colouring of the guard pointers. */
extern "C" void *game_uso_func_055750(unsigned);
struct Vec3 {
    void *operator new(unsigned n) { return game_uso_func_055750(n); }
    float x, y, z;
    Vec3() {}
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    Vec3(float a, float b, float c) { set(a, b, c); }
    Vec3 &operator=(Vec3 v) { x = v.x; y = v.y; z = v.z; return *this; }
    Vec3 &operator+=(const Vec3 &v) { x = x + v.x; y = y + v.y; z = z + v.z; return *this; }
    Vec3 &operator*=(float s) { x = x * s; y = y * s; z = z * s; return *this; }
    Vec3 operator*(float s) const { return Vec3(x * s, y * s, z * s); }
};
struct XZ : Vec3 {
    XZ(const Vec3 &v) : Vec3(v.x, 0.0f, v.z) {}
    XZ(float a, float c) : Vec3(a, 0.0f, c) {}
};
inline XZ operator-(const Vec3 &a, const Vec3 &b) { return XZ(a.x - b.x, a.z - b.z); }
inline XZ perp(const Vec3 &v) { return XZ(v.z, -v.x); }
struct Node { char pad0[0x30]; Vec3 pos; char pad3C[0x54 - 0x3C]; float width; };
struct W { char pad0[0xB4]; Vec3 pos; char padC0[0x348 - 0xC0]; float speed; };
struct Self {
    char pad0[0x30]; W *w; char pad34[0x84 - 0x34];
    Vec3 p84; Vec3 p90; int phase; char padA0[0xDC - 0xA0]; float kDC; float kE0;
};
extern "C" {
void game_uso_func_071028(Vec3 *v);
}
/* agent-g run 205: the function returns XZ (the return temp goes straight from
 * operator-'s slot to the caller's buffer: 36 -> 372 -> *ret, no Vec3 slicing
 * copies), dir is a Vec3 initialised from the XZ result and e is direct-
 * initialised: the target's three-copy chains (356 -> 372 -> 516 -> 432) at both
 * difference sites now line up. 87.19 -> 88.68 standalone, length = target. */
extern "C" XZ game_uso_func_00008CD8(Self *s, Node *a, Node *b, int flag) {
    float dot; /* agent-g run 206: the else-arm dot declared at function scope (lever-sweep hoist): 89.475 -> 89.499 */
    if (flag != 0) {
        XZ bp(b->pos);
        Vec3 dir = bp - a->pos;
        game_uso_func_071028(&dir);
        XZ wp(s->w->pos);
        const Vec3 &d2 = wp - a->pos;
        float lenf = dir.x * d2.x + d2.z * dir.z;
        s->p90 = (const Vec3 &)XZ(a->pos);
        s->p84 = s->p90;
        float dot = lenf + (s->w->speed * s->kDC) / 2; /* agent-g run 231 (operand-commute sweep): speed first, 89.51 -> 90.41 */
        s->p84 += dir * dot;
        dot = lenf - (s->w->speed * s->kDC) / 2;
        s->p90 += dir * dot;
        s->phase = 0;
    } else {
        Vec3 pb1, pb2, pb3, pb4, pb5, pb6; /* unused: frame 456 -> 528 = target (no code) */
        XZ bp(b->pos);
        XZ e(bp - a->pos);
        XZ f = s->p84 - s->p90; if (1) {} /* agent-g run 224 (bb sweep): 89.499 -> 89.506 */
        XZ wp(s->w->pos);
        const Vec3 &g = wp - s->p90;
        if ((e.x * f.x + e.z * f.z) * 0.5f < e.z * g.z + g.x * e.x) {
            s->p90 = s->p84;
            game_uso_func_071028(&e);
            XZ side = perp(e);
            s->p84 = (const Vec3 &)XZ(a->pos);
            XZ h = s->p90 - a->pos;
            dot = h.x * e.x + e.z * h.z + s->w->speed * s->kDC;
            s->p84 += e * dot;
            side *= s->kE0 * (250.0f * b->width); /* run 205: target muls 250 * width first, then kE0 (88.68 -> 89.48) */
            switch (s->phase = (s->phase + 1) & 3) {
            case 0:
                side *= 0.0f;
                break;
            case 1:
                side *= -1.0f;
                break;
            case 2:
            case 3:
                break;
            }
            s->p84 += side;
        }
    }
    return s->p84 - s->w->pos;
}
