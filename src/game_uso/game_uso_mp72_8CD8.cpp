/* MIPSpro 7.2.1 C++ (o32 edgcpfe, EDG 2.29) NON_MATCHING donor for game_uso_func_00008CD8 (709 words),
 * compiled by tools/ncc721.sh and spliced into the non_matching object only. The ROM build keeps INCLUDE_ASM.
 *
 * The target lowers the 4-label `switch (s->phase)` as a native 4-entry jump table (sltiu 4), which only
 * EDG 2.29's switch rule produces. History: agent-c run 199 rewrote the cfe NM body into this value-class
 * kit under IDO 7.1 NCC (edg 2.19) and later runs tuned it to 90.43 there; those levers do not carry over
 * (88.41 under 7.2.1). agent-h run 357 rebuilt the spellings on 7.2.1 from the plain body (88.30):
 *   - dir / e are direct-initialised XZ (`XZ dir(bp - a->pos)`): the last hop of the copy chain is a word
 *     copy as in the target (a Vec3 copy-init from the XZ result copies memberwise as floats), 88.30 -> 90.19;
 *   - f is a const reference to the difference, 90.19 -> 90.28;
 *   - the else-arm b->pos is a temporary inside e's initialiser (`XZ e(XZ(b->pos) - a->pos)`): edg 2.29
 *     gives expression temporaries their own pool words above the named locals, like the target's
 *     else-arm layout (bp 228 > wp 216 > e 204 > f 192 > g 180), 90.30 -> 90.51;
 *   - `+=` written with compound members, 90.51 -> 90.55; the else-arm second dot as e.x * g.x + e.z * g.z
 *     (the target multiplies e first in both products), 90.55 -> 90.567;
 *   - four unused Vec3 in the flag arm (frame 528 = target, no code) and two `if (1) {}` block splits
 *     after dir's construction and the normalise call, -> 90.602.
 * LEFT: the XZ(const Vec3 &) ctor `this` is coloured a0 in the target (its fall-through `addiu a0,zero,12`
 * then cannot fill the guard's delay slot, so as1 emits `bnezl a0` + the b reload: +1 word at both
 * b->pos sites) and v1 here; the target's double phase store (`sw t6` = phase + 1, then `sw t8` = & 3:
 * `s->phase++; switch (s->phase &= 3)` reproduces it, 90.40 in this basin); pool-slot offsets. */
extern "C" void *game_uso_func_055750(unsigned);
struct Vec3 {
    void *operator new(unsigned n) { return game_uso_func_055750(n); }
    float x, y, z;
    Vec3() {}
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    Vec3(float a, float b, float c) { set(a, b, c); }
    Vec3 &operator=(Vec3 v) { x = v.x; y = v.y; z = v.z; return *this; }
    Vec3 &operator+=(const Vec3 &v) { x += v.x; y += v.y; z += v.z; return *this; }
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
extern "C" XZ game_uso_func_00008CD8(Self *s, Node *a, Node *b, int flag) {
    if (flag != 0) {
        Vec3 pb0, pb1, pb2, pb3; /* unused: frame 528 = target (no code) */
        XZ bp(b->pos);
        XZ dir(bp - a->pos);
        if (1) {}
        game_uso_func_071028(&dir);
        if (1) {}
        XZ wp(s->w->pos);
        const Vec3 &d2 = wp - a->pos;
        float lenf = dir.x * d2.x + d2.z * dir.z;
        s->p90 = (const Vec3 &)XZ(a->pos);
        s->p84 = s->p90;
        float dot = lenf + (s->w->speed * s->kDC) / 2;
        s->p84 += dir * dot;
        dot = lenf - (s->w->speed * s->kDC) / 2;
        s->p90 += dir * dot;
        s->phase = 0;
    } else {
        XZ e(XZ(b->pos) - a->pos);
        const Vec3 &f = s->p84 - s->p90;
        XZ wp(s->w->pos);
        const Vec3 &g = wp - s->p90;
        if ((e.x * f.x + e.z * f.z) * 0.5f < e.x * g.x + e.z * g.z) {
            s->p90 = s->p84;
            game_uso_func_071028(&e);
            XZ side = perp(e);
            s->p84 = (const Vec3 &)XZ(a->pos);
            XZ h = s->p90 - a->pos;
            float dot = h.x * e.x + e.z * h.z + s->w->speed * s->kDC;
            s->p84 += e * dot;
            side *= s->kE0 * (250.0f * b->width); /* the target muls 250 * width first, then kE0 */
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
