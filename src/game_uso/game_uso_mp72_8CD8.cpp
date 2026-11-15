#include "../../include/uso_call.h"  /* NCC donors build without -I include */
/* MIPSpro 7.2.1 C++ (o32 edgcpfe, EDG 2.29) EXACT donor for SnowAI_aimWeave (709 words; agent-h run 428),
 * compiled by tools/ncc721.sh and spliced over a placeholder in both build trees (the B3C recipe).
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
 *   - agent-c run 418 (kit DII, edg frame-word model): d2 is a temporary inside lenf's initialiser through an
 *     inline dot2 (no named d2 word: lenf at the target's M-100), 97.472 -> 97.528; dir's `a` without K3 and
 *     subp2 (the flag-arm `if (1) {}` no longer needed), -> 97.533 (2832 B, pool a2 at sp+372); f as a named
 *     XZ value (`XZ f = subp2(...)`, the target's M-336 between e and g) with g bound as `const XZ &`, -> 97.537.
 *   - agent-h run 426 (kit DX): the pool slot (21 / 30) lost a2 to the plain-return chain temps (3 / 4); block
 *     counts are ratio-locked (every chain-temp block is also a slot block). A KR5 return (`return r;` through a
 *     const-ref parameter) reads the copy source through the ctor's `this` result, so the chain temp's num drops
 *     3 -> 2 (2 / 3 = 0.667) with no code change, provided the site's leading dummy count keeps the `this` result
 *     out of the shared pooled VREGs: f and h subr1 (1 dummy), g subr (0). perp (no neutral KR form) and dir
 *     (its chain temp precedes the pool's first use) keep the plain return with 8 ctor blocks (3 / 5), and the
 *     wpos view takes 8 blocks (3 / 5): pool a2 at 21 / 30 = 0.70 ahead of all of them, every chain temp a3 as in
 *     the target, 98.966 -> 99.564 (dir as KR5) -> 99.567 (dir plain).
 *   - agent-c run 426 (kit DXI, frame words by the kit DII model; agent-c 19e708dc8): every edg word from M-96 to
 *     M-492 at the target's offset (no named dot / wpos, g a value, kept-parameter pool growth in lenf / the chain /
 *     the inner +=), then the colours back with code-free changes (a block split in Vec3(float x3), K1 on the first
 *     +=, sub2 return), 98.966 -> 99.113 (2836 B, frame 528), with the pool a3 / chain-temp a2 swap still in it.
 *   - agent-h run 426: kit DX's lever on that basin (f and h through subr1 / KR5, the dir / perp ctor at 8 blocks;
 *     g goes through operator-'s KR2 already, and there is no wpos view to steer): pool a2, 99.567 -> 99.997. Left:
 *     d2's conversion `v` spill word M-120 (sp+408) where the target has M-112 (sp+416), and the jump-table addend.
 *   - agent-h run 428 (kit DXIII), on agent-c's run 426 scratch body (lenf a member call on the d2 temporary, the d2 / g
 *     conversions with two trailing kept pointers: every target spill word, 99.116): f's KR5 chain temp K-372 needs
 *     its `this`-result word (M-116) to be a live LR in g's statement, so g goes back to the plain conversion (its
 *     base-ctor `this` is word 4 again) through a 5-block Vec3(float, int, float) base ctor (M-116 7 / 3 -> 7 / 4, under
 *     the d2 / g pointer's 13 / 7: pointer v1, M-116 a0, s a1); K3 for lenf's K1; the other conversions through a
 *     block-free Vec3(float, short, float), and 7 blocks in XZ(float, float, int), so the pool slot keeps n = 121
 *     (21 / 31) above every chain temp (2 / 3, 3 / 5): EXACT (2836 B, frame 528). The table sits at donor .rodata
 *     +0x100 through game_uso_func_00008CD8_lay.
 * EARLIER LEFT (before run 426): the else-arm XZ(wpos) `this` shares its word with perp's `this` (v1; target a0 / v1); d2's conversion `v`
 * home sp+400 vs the target's sp+416 (in the target d2's and g's `v` are one word: a direct XZ(K1(s)->w->pos) at g
 * does join them, but the conversion-`this` word then outranks e1189 and s falls to s0, 2712 B); f0 / f12 swaps at
 * the sub / operator* sites; `s->p84 = s->p90` in the target copies through its own by-value operator= temp
 * (sp+284), which a chained `s->p84 = K3(s)->p90 = (const Vec3 &)XZ(...)` reproduces (2836 B = target; with the
 * flag-arm KR1 dropped, e as subp1 over the 3-dummy XZ(b->pos): frame 528, pool a2 sp+372, s a1, but 97.223 on
 * the remaining slot order, kit CDXCIX). */
extern "C" void *game_uso_func_055750(unsigned);
template <class T> inline const T &KR1(const T &r) { const T *p = &r; if (0) { p = 0; } return *p; } /* kept const-ref identity with a dead block */
/* run 425: the same dead-block identity written with parameters only (kit DII): its pointer is a pooled word,
 * not a named one; used at the sites below that the target frame has no named word for */
template <class T> inline const T *KP(const T *p) { if (0) { p = 0; } return p; }
template <class T> inline const T &KR2(const T &r) { return *KP(&r); }
template <class T> inline const T &KR5(const T &r) { return r; } /* run 426: plain const-ref identity (chain temp num 3 -> 2) */
struct Vec3 {
    void *operator new(unsigned n) { return game_uso_func_055750(n); }
    float x, y, z;
    Vec3() {}
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    /* agent-c run 426: a code-free block split here takes the ctor-`this` word M-128 from 28 / 6 to 28 / 7, under
     * e1189 (4.5): e1189 takes v1 again and the ctor-`this` sites a0, as in the target */
    Vec3(float a, float b, float c) { if (1) {} set(a, b, c); }
    /* agent-h run 428 (kit DXIII): base ctors for the XZ(const Vec3 &) conversions, chosen by the y argument's type
     * (0 folds to 0.0f, no code). (short): the dummy-pointer / int-dummy conversions without the code-free block,
     * which takes the pool slot K-156 back to n = 121 (den 31). (int): g's plain conversion, five blocks: its
     * base-ctor `this` (word 4, M-116) goes 7 / 3 -> 7 / 4 under the shared d2 / g pointer (13 / 7), which then
     * takes v1 and M-116 a0, with s in a1 */
    Vec3(float a, short b, float c) { set(a, b, c); }
    Vec3(float a, int b, float c) { if (1) {} if (1) {} if (1) {} if (1) {} if (1) {} set(a, b, c); }
    /* agent-h run 425: mvf's product ctor, member stores (no set() call, so set's float parameters take no pool
     * words); the kept int splits float slot 3 in the flag arm (kit DVI section 6); the three kept dead floats
     * grow the float pool by one word in that statement (the target's flag-arm pool word), which puts e .. the
     * += objects at the target's offsets */
    Vec3(float a, float b, float c, float d0, float d1, float d2, int d) { x = a; y = b; z = c; d0 = d0; d1 = d1; d2 = d2; d = d; }
    Vec3 &operator=(Vec3 v) { x = v.x; y = v.y; z = v.z; return *this; }
    /* agent-c run 426 (kit DII model): operator= with five kept float parameters. The statement's float demand grows
     * the pool by two words AFTER the by-value parameter temp (the target's O-244 VV), so mvf's float word is already
     * there and the dir products land at the target's M-264 / M-276 */
    Vec3 &asg5(Vec3 v, float f0, float f1, float f2, float f3, float f4) { f0 = f0; f1 = f1; f2 = f2; f3 = f3; f4 = f4; x = v.x; y = v.y; z = v.z; return *this; }
    Vec3 &operator+=(const Vec3 &v) { x += v.x; y += v.y; z += v.z; return *this; }
    Vec3 &operator*=(float s) { x = x * s; y = y * s; z = z * s; return *this; }
    Vec3 operator*(float s) const { return KR1(Vec3(x * s, y * s, z * s)); }
    /* agent-c run 426: lenf's dot as a member called on the d2 temporary. The receiver is evaluated before the
     * member's parameters are reserved, so only operator-'s a precedes the conversion (its pointer = word 3, M-112);
     * the kept pointer keeps 9 pool words after d2's object */
    float dotp(const Vec3 &v, const Vec3 *p0) const { p0 = p0; return v.x * x + v.z * z; }
};
struct XZ : Vec3 {
    XZ(const Vec3 &v) : Vec3(v.x, 0, v.z) {} /* run 428: g only, the 5-block base ctor */
    /* agent-c run 426: two kept pointers AFTER v take words 4 / 5, so the Vec3 base ctor's `this` (which the d2 /
     * g pointer word overlaps) moves off word 4 (lenf only since run 428) */
    XZ(const Vec3 &v, const Vec3 *p0, const Vec3 *p1) : Vec3(v.x, (short)0, v.z) { p0 = p0; p1 = p1; }
    /* leading kept dummy parameters: each takes the next edg pool word (kit CDXC) */
    XZ(int z0, const Vec3 &v) : Vec3(v.x, 0.0f, v.z) { z0 = z0; }
    XZ(int z0, int z1, const Vec3 &v) : Vec3(v.x, (short)0, v.z) { z0 = z0; z1 = z1; }
    XZ(int z0, int z1, int z2, const Vec3 &v) : Vec3(v.x, (short)0, v.z) { z0 = z0; z1 = z1; z2 = z2; }
    XZ(float a, float c) : Vec3(a, 0.0f, c) {}
    /* run 415: XZ's own copy of Vec3::operator* (only `e * dot` uses it; the dir products are byte-identical
     * either way): the else-arm product's float temps leave the Vec3 operator's words, 97.458 -> 97.472 */
    Vec3 operator*(float s) const { return KR2(Vec3(x * s, y * s, z * s)); }
    Vec3 mvf(float s) const { return KR2(Vec3(x * s, y * s, z * s, x, x, x, (int)this)); } /* run 425: first flag-arm dir * (...) */
    /* agent-h run 414: code-free block splits inside the plain-return chain temp's ctor (run 426: dir and perp only, 8 blocks:
     * each `this` address LR grows from 7 to 15 blocks, uopt priority 3/3 = 1.0 -> 3/5 = 0.6, under the pool's 0.70) */
    XZ(float a, float c, int) : Vec3(a, 0.0f, c) { if (1) {} if (1) {} if (1) {} if (1) {} if (1) {} if (1) {} if (1) {} } /* run 428: 7 (n = 14, den 5) */
};
inline XZ operator-(const Vec3 &a, const Vec3 &b) { return KR2(XZ(a.x - b.x, a.z - b.z)); }
inline XZ sub1(int z0, const Vec3 &a, const Vec3 &b) { z0 = z0; return KR2(XZ(a.x - b.x, a.z - b.z)); }
inline XZ sub2(int z0, int z1, const Vec3 &a, const Vec3 &b) { z0 = z0; z1 = z1; return KR1(XZ(a.x - b.x, a.z - b.z)); }
/* run 426: KR5 return (chain temp num 2); the dummy count picks the pooled word of the ctor's `this` result */
inline XZ subr1(int z0, const Vec3 &a, const Vec3 &b) { z0 = z0; return KR5(XZ(a.x - b.x, a.z - b.z)); }
/* dir keeps the plain return: its chain temp lies before the pool's first use, so the 8 ctor blocks cost the pool
 * nothing (3 / 5); the KR5 form there scores 99.564, this 99.567 */
inline XZ subp2(int z0, int z1, const Vec3 &a, const Vec3 &b) { z0 = z0; z1 = z1; return XZ(a.x - b.x, a.z - b.z, 0); }
/* agent-c run 426: the return's subtraction with the kept dummy AFTER the operands. edg reserves an inline's
 * parameters in order before it evaluates the arguments, and the pool is one sequence of words for every type, so
 * b (the w->pos pointer) is the statement's 2nd word, M-108 as in the target; the trailing int keeps the ctor-`this`
 * in an a0 word */
inline XZ subz(const Vec3 &a, const Vec3 &b, int z0) { z0 = z0; return KR1(XZ(a.x - b.x, a.z - b.z)); }
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
/* agent-c run 426: one kept pointer parameter each, which grows the pointer pool by the words the target has after
 * d2's object (lenf: 7 -> 9) and after the e * dot product (the inner +=: 2 -> 3, frame 528) */
inline float dot2p(const Vec3 &u, const Vec3 &v, const Vec3 *p0) { p0 = p0; return u.x * v.x + u.z * v.z; }
inline float hdotp(const Vec3 &h, const Vec3 &e, const Vec3 *p0) { p0 = p0; return e.x * h.x + e.z * h.z; } /* run 425: e first (load order) */
/* layout only (agent-h run 428, the B3C recipe): the target's `lw t9,0x100(at)` is an in-place %lo (the game_uso Yay0
 * block is raw .text, no reloc bake), so the phase table must sit at donor .rodata +0x100. These 64 distinct literals
 * (none with a zero low half, so none loads by lui) stand in for game_uso RoData +0x0 .. +0x100 (0000's 1/6, 940's
 * tables, B3C's table and strings, the 3ED4 / 43D8 / 44F4 / 7C1C floats). The splice copies only
 * SnowAI_aimWeave; its renamed .rodata reloc (SnowAI_aimWeave_rodata) is pinned to 0. */
extern "C" void game_uso_func_00008CD8_lay(volatile float *o) {
    float v = *o;
    v = v * 1.10f; v = v * 1.20f; v = v * 1.30f; v = v * 1.40f; v = v * 1.60f; v = v * 1.70f; v = v * 1.80f; v = v * 1.90f;
    v = v * 2.10f; v = v * 2.20f; v = v * 2.30f; v = v * 2.40f; v = v * 2.60f; v = v * 2.70f; v = v * 2.80f; v = v * 2.90f;
    v = v * 3.10f; v = v * 3.20f; v = v * 3.30f; v = v * 3.40f; v = v * 3.60f; v = v * 3.70f; v = v * 3.80f; v = v * 3.90f;
    v = v * 4.10f; v = v * 4.20f; v = v * 4.30f; v = v * 4.40f; v = v * 4.60f; v = v * 4.70f; v = v * 4.80f; v = v * 4.90f;
    v = v * 5.10f; v = v * 5.20f; v = v * 5.30f; v = v * 5.40f; v = v * 5.60f; v = v * 5.70f; v = v * 5.80f; v = v * 5.90f;
    v = v * 6.10f; v = v * 6.20f; v = v * 6.30f; v = v * 6.40f; v = v * 6.60f; v = v * 6.70f; v = v * 6.80f; v = v * 6.90f;
    v = v * 7.10f; v = v * 7.20f; v = v * 7.30f; v = v * 7.40f; v = v * 7.60f; v = v * 7.70f; v = v * 7.80f; v = v * 7.90f;
    v = v * 8.10f; v = v * 8.20f; v = v * 8.30f; v = v * 8.40f; v = v * 8.60f; v = v * 8.70f; v = v * 8.80f; v = v * 8.90f;
    *o = v;
}
extern "C" XZ SnowAI_aimWeave(Self *s, Node *a, Node *b, int flag) {
    if (flag != 0) {
        XZ dir(subp2((int)a, (int)a, XZ((int)a, (int)a, b->pos), a->pos));
        XCALL(game_uso_func_071028, math_vec3Normalize)(&dir);
        /* run 418: d2 a temporary, lenf M-100; agent-c run 426: a member call on d2 (conversion `v` = word 3, M-112) */
        float lenf = (XZ(s->w->pos, &a->pos, &a->pos) - K3(a)->pos).dotp(dir, &a->pos);
        /* run 425: chained (kit CDXCIX); agent-c run 426: no named dot (the target's M-104 is a pool word), the outer
         * assignment through asg5 (its parameter temp at the target's M-244), K1 on the first += receiver */
        s->p84.asg5(s->p90 = (const Vec3 &)XZ((int)a, (int)a, a->pos), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        K1(s)->p84 += KR2(dir.mvf(lenf + (s->w->speed * s->kDC) / 2));
        s->p90 += dir * (lenf - (s->w->speed * s->kDC) / 2);
        s->phase = 0;
    } else {
        XZ e(XZ((int)a, (int)a, (int)a, b->pos) - a->pos);
        XZ f = subr1((int)a, s->p84, s->p90); /* run 418: a named value at the target's M-336 */
        /* agent-c run 426: g a named value and no named wpos view (the target's e-324 f-336 g-348 with three block
         * temps above), written plainly: operator-'s a / b take the statement's first two pool words, so the
         * w->pos pointer is the 3rd (M-112, v1) as in the target */
        XZ g = XZ(s->w->pos) - s->p90;
        if (e.x * f.x + e.z * f.z) {} /* run 422: code-free test, the e.f sum becomes a uopt CSE (target f0, no ring pop) */
        if ((e.x * f.x + e.z * f.z) * 0.5f < e.x * g.x + e.z * g.z) {
            s->p90 = s->p84;
            XCALL(game_uso_func_071028, math_vec3Normalize)(&e);
            XZ side = perp(e);
            s->p84 = (const Vec3 &)XZ((int)a, (int)a, (int)a, a->pos);
            /* run 425: h is a temporary of the += statement (the target's += operand temp M-408 sits above h's
             * M-420, and h's subtraction takes struct pool word 1, so e * dot returns through word 2 = M-244) */
            s->p84 += e * (hdotp(subr1((int)a, s->p90, a->pos), e, &a->pos) + s->w->speed * s->kDC); /* frame 528 */
            side *= (250.0f * b->width) * s->kE0; /* the target muls 250 * width first, then kE0 (run 422: as the left operand) */
            s->phase++; /* agent-h run 361: the target stores phase twice (phase + 1, then & 3), 91.182 -> 91.288 */
            s->phase &= 3; /* run 425: a statement of its own, then switch on the field: the masked value stays a */
            switch (s->phase) { /* ugen temp (target t8) instead of a uopt LR (v0), and the return copy recolours */
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
    return subz(s->p84, s->w->pos, (int)a); /* agent-c run 426: the return `v` at M-108, v1 (target) */
}
