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
 *     after dir's construction and the normalise call, -> 90.602;
 *   - the flag-arm b->pos also a temporary inside dir's initialiser, with three unused Vec3 in the else
 *     arm: bp / last copy hop / dir at the target's 504 / 516 / 432, -> 90.621;
 *   - d2 a named XZ, the else-arm w->pos a temporary inside g's initialiser read through a const-ref view
 *     (with `XZ(s->w->pos)` written directly uopt moves s into s0 and the body loses 28 words), h a const
 *     reference and the return's w->pos through a view (random structural sampling + climb), -> 90.755;
 *   - d2 as the const-ref-bound difference of the same w->pos temporary form, lenf with dir first in both
 *     products, the return written directly (two-change neighbourhood sampling), -> 90.798;
 *   - the flag-arm w->pos temporary written directly (no view; in this basin s stays in a1), -> 91.182
 *     (flag-arm guard homes 504 / 360 / 480 / 340 vs the target's 504 / 356 / 480 / 332);
 *   - agent-h run 361: the target's double phase store (`sw t6` = phase + 1, then `sw t8` = & 3) as
 *     `s->phase++; switch (s->phase &= 3)`, a net gain in this basin, 91.182 -> 91.288 (2824 of 2836 B);
 *   - agent-h run 363: kept-identity K3 (`p = p`, kit CDXLVIII: an extra pool word without a block split) on
 *     dir's `a` operand and the return's `s` receiver (both needed), 91.288 -> 91.358, then on the else-arm
 *     dot's kDC receiver (`K3(s)->kDC`; a sweep over every s / a / b access), 91.358 -> 91.714; then K1
 *     (`if (0) { p = 0; }`: the dead block splits uopt blocks) on the receivers of the two flag-arm `+=`
 *     and the else-arm wpos view (greedy K1 / K3 climb), 91.714 -> 92.254;
 *   - agent-h run 368 (kit CDLIV): operator* returns through KR1 (a kept const-ref identity whose `if (0)` dead
 *     block sits INSIDE the chain temp's span, between the Vec3 temporary and the copy into the pooled return
 *     slot), so that chain temp's adjsave drops below the address constants it competed with, 92.254 -> 94.353;
 *     then the second flag-arm `+=` receiver without K1, -> 94.426.
 * LEFT: the XZ(const Vec3 &) ctor `this` is coloured a0 in the target (its fall-through `addiu a0,zero,12`
 * then cannot fill the guard's delay slot, so as1 emits `bnezl a0` + the b reload: +1 word at both
 * b->pos sites) and v1 here (a0 only at the d2 site, where &w->pos holds v1); pool-slot offsets. */
extern "C" void *game_uso_func_055750(unsigned);
template <class T> inline const T &KR1(const T &r) { const T *p = &r; if (0) { p = 0; } return *p; } /* kept const-ref identity with a dead block */
struct Vec3 {
    void *operator new(unsigned n) { return game_uso_func_055750(n); }
    float x, y, z;
    Vec3() {}
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    Vec3(float a, float b, float c) { set(a, b, c); }
    Vec3 &operator=(Vec3 v) { x = v.x; y = v.y; z = v.z; return *this; }
    Vec3 &operator+=(const Vec3 &v) { x += v.x; y += v.y; z += v.z; return *this; }
    Vec3 &operator*=(float s) { x = x * s; y = y * s; z = z * s; return *this; }
    Vec3 operator*(float s) const { return KR1(Vec3(x * s, y * s, z * s)); }
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
template <class T> inline T *K3(T *p) { p = p; return p; } /* kept identity: zero code */
template <class T> inline T *K1(T *p) { if (0) { p = 0; } return p; } /* kept identity with a dead block */
extern "C" XZ game_uso_func_00008CD8(Self *s, Node *a, Node *b, int flag) {
    if (flag != 0) {
        XZ dir(XZ(b->pos) - K3(a)->pos);
        if (1) {}
        game_uso_func_071028(&dir);
        if (1) {}
        const Vec3 &d2 = XZ(s->w->pos) - a->pos;
        float lenf = dir.x * d2.x + dir.z * d2.z;
        s->p90 = (const Vec3 &)XZ(a->pos);
        s->p84 = s->p90;
        float dot = lenf + (s->w->speed * s->kDC) / 2;
        K1(s)->p84 += dir * dot;
        dot = lenf - (s->w->speed * s->kDC) / 2;
        s->p90 += dir * dot;
        s->phase = 0;
    } else {
        Vec3 pad1, pad2, pad3; /* unused (no code) */
        XZ e(XZ(b->pos) - a->pos);
        const Vec3 &f = s->p84 - s->p90;
        const Vec3 &wpos = K1(s)->w->pos; /* zero-code view: keeps s out of s0 with the temporary below */
        XZ g = XZ(wpos) - s->p90;
        if ((e.x * f.x + e.z * f.z) * 0.5f < e.x * g.x + e.z * g.z) {
            s->p90 = s->p84;
            game_uso_func_071028(&e);
            XZ side = perp(e);
            s->p84 = (const Vec3 &)XZ(a->pos);
            const Vec3 &h = s->p90 - a->pos;
            float dot = h.x * e.x + e.z * h.z + s->w->speed * K3(s)->kDC;
            s->p84 += e * dot;
            side *= s->kE0 * (250.0f * b->width); /* the target muls 250 * width first, then kE0 */
            s->phase++; /* agent-h run 361: the target stores phase twice (phase + 1, then & 3), 91.182 -> 91.288 */
            switch (s->phase &= 3) {
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
    return K3(s)->p84 - s->w->pos;
}
