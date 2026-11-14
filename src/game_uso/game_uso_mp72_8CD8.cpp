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
 *     then the second flag-arm `+=` receiver without K1, -> 94.426; KR1's pool words grew the frame to 544,
 *     so one else-arm pad Vec3 goes (frame 528 = target again), -> 94.509; d2 and f bound through KR1 too
 *     (their chain temps lowered the same way) with one pad Vec3 left for the frame, -> 94.561; dir's initialiser
 *     and the first `+=` operand through KR1 as well, no pad Vec3 left (frame 528), -> 94.564.
 *   - agent-c run 409 (kit CDXCII): `operator-` returns through KR1 (every per-site chain temp drops to 0.667),
 *     then edg pool words steered with leading kept dummy int parameters (`z = z;`, kit CDXC): XZ(const Vec3 &)
 *     overloads with 1-3 dummies at five conversion sites and sub1 / sub2 (KR1 return) / subp1 (plain return)
 *     at dir, d2, f, h and the return. The sp+468 / sp+132 chain temps now take the target's v1 (their ctor
 *     `this` a0), 94.564 -> 96.275, 2828 B (target 2836), frame 544 (target 528). The dummy KR1 words cost the
 *     frame; every spelling that puts the sp+372 pool in a2 also puts s in s0 (2712 B).
 *   - agent-c run 411 (kit CDXCVI): h as plain subp2 (frame 536), dir as subp1 with a 2-dummy XZ(b->pos) (dir's
 *     conversion `this` leaves the d2-conversion-`v` word and takes a0 as in the target), g as subp1, K1 moved from
 *     the flag-arm `+=` to the else-arm one, K3 on the p90 store and the h-dot speed read: frame 528 = target,
 *     pool at the target's sp+372 / M-156, 96.275 -> 96.683, 2832 B (target 2836).
 *   - agent-h run 414 (kit CDXCVII, instrumented-uopt colouring trace): the pool / return slot (K-164, 24 / 26 =
 *     0.923) was coloured after seven 1.0 LRs that took a2 on the register tie (the wpos view word and the `this`
 *     address constants of the six plain-return subp1 / subp2 / perp chain temps). Four code-free `if (1) {}` in a
 *     separate XZ(float, float, int) ctor used only by those three, plus four after the wpos binding, take them to
 *     10-11 blocks (3 / 4 = 0.75) while the slot only falls to 24 / 31 = 0.774, still below s (1.39): pool a2,
 *     every chain temp a3, s a1, 96.683 -> 97.289 (2832 B).
 *   - agent-c run 415 (kit CDXCIX): the flag-arm dot as one named `float dot = lenf;` read by both `+=`
 *     operands (`dot + half` / `dot - half`) instead of a reassigned local: 97.289 -> 97.458, frame 528; then
 *     an XZ member copy of operator* (used by the else-arm `e * dot`), -> 97.472 (2832 B).
 * LEFT: the else-arm XZ(wpos) `this` shares its word with perp's `this` (v1; target a0 / v1); d2's conversion `v`
 * home sp+400 vs the target's sp+416 (in the target d2's and g's `v` are one word: a direct XZ(K1(s)->w->pos) at g
 * does join them, but the conversion-`this` word then outranks e1189 and s falls to s0, 2712 B); f0 / f12 swaps at
 * the sub / operator* sites; `s->p84 = s->p90` in the target copies through its own by-value operator= temp
 * (sp+284), which a chained `s->p84 = K3(s)->p90 = (const Vec3 &)XZ(...)` reproduces (2836 B = target; with the
 * flag-arm KR1 dropped, e as subp1 over the 3-dummy XZ(b->pos): frame 528, pool a2 sp+372, s a1, but 97.223 on
 * the remaining slot order, kit CDXCIX). */
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
    /* leading kept dummy parameters: each takes the next edg pool word (kit CDXC) */
    XZ(int z0, const Vec3 &v) : Vec3(v.x, 0.0f, v.z) { z0 = z0; }
    XZ(int z0, int z1, const Vec3 &v) : Vec3(v.x, 0.0f, v.z) { z0 = z0; z1 = z1; }
    XZ(int z0, int z1, int z2, const Vec3 &v) : Vec3(v.x, 0.0f, v.z) { z0 = z0; z1 = z1; z2 = z2; }
    XZ(float a, float c) : Vec3(a, 0.0f, c) {}
    /* run 415: XZ's own copy of Vec3::operator* (only `e * dot` uses it; the dir products are byte-identical
     * either way): the else-arm product's float temps leave the Vec3 operator's words, 97.458 -> 97.472 */
    Vec3 operator*(float s) const { return KR1(Vec3(x * s, y * s, z * s)); }
    /* agent-h run 414: code-free block splits inside the plain-return (subp1 / subp2 / perp) chain temps' ctor:
     * their `this` address LRs grow from 7 to 11 blocks, uopt priority 3/3 = 1.0 -> 3/4 = 0.75 */
    XZ(float a, float c, int) : Vec3(a, 0.0f, c) { if (1) {} if (1) {} if (1) {} if (1) {} }
};
inline XZ operator-(const Vec3 &a, const Vec3 &b) { return KR1(XZ(a.x - b.x, a.z - b.z)); }
inline XZ sub1(int z0, const Vec3 &a, const Vec3 &b) { z0 = z0; return KR1(XZ(a.x - b.x, a.z - b.z)); }
inline XZ sub2(int z0, int z1, const Vec3 &a, const Vec3 &b) { z0 = z0; z1 = z1; return KR1(XZ(a.x - b.x, a.z - b.z)); }
inline XZ subp1(int z0, const Vec3 &a, const Vec3 &b) { z0 = z0; return XZ(a.x - b.x, a.z - b.z, 0); }
inline XZ subp2(int z0, int z1, const Vec3 &a, const Vec3 &b) { z0 = z0; z1 = z1; return XZ(a.x - b.x, a.z - b.z, 0); }
inline XZ perp(const Vec3 &v) { return XZ(v.z, -v.x, 0); }
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
        XZ dir(subp1((int)a, XZ((int)a, (int)a, b->pos), K3(a)->pos));
        game_uso_func_071028(&dir);
        if (1) {}
        const Vec3 &d2 = subp1((int)a, XZ((int)a, s->w->pos), a->pos);
        float lenf = dir.x * d2.x + dir.z * d2.z;
        float dot = lenf; /* run 415: one named dot read by both operands, 97.289 -> 97.458, frame 528 */
        K3(s)->p90 = (const Vec3 &)XZ((int)a, (int)a, a->pos);
        s->p84 = s->p90;
        s->p84 += KR1(dir * (dot + (s->w->speed * s->kDC) / 2));
        s->p90 += dir * (dot - (s->w->speed * s->kDC) / 2);
        s->phase = 0;
    } else {
        XZ e(XZ((int)a, (int)a, (int)a, b->pos) - a->pos);
        const Vec3 &f = subp1((int)a, s->p84, s->p90);
        const Vec3 &wpos = K1(s)->w->pos; /* zero-code view: keeps s out of s0 with the temporary below */
        if (1) {} if (1) {} if (1) {} if (1) {} /* run 414: wpos LR 6 -> 10 blocks, priority 1.0 -> 0.75 */
        XZ g = subp1((int)a, XZ(wpos), s->p90);
        if ((e.x * f.x + e.z * f.z) * 0.5f < e.x * g.x + e.z * g.z) {
            s->p90 = s->p84;
            game_uso_func_071028(&e);
            XZ side = perp(e);
            s->p84 = (const Vec3 &)XZ((int)a, (int)a, (int)a, a->pos);
            const Vec3 &h = subp2((int)a, (int)a, s->p90, a->pos);
            float dot = h.x * e.x + e.z * h.z + K3(s)->w->speed * K3(s)->kDC;
            K1(s)->p84 += e * dot;
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
    return sub1((int)a, K3(s)->p84, s->w->pos);
}
