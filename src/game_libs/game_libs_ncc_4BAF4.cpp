/* IDO 7.1 NCC (C++) EXACT donor for gl_func_0004BAF4 (0x69C / 423 words; exact since agent-c run 405), lifted in agent-c run 157:
 * the cfe NM body (billboard sprite draw) lifted under NCC with the display-list appends as cfront
 * inlines (Ctx4B::next / Ctx4B::add). What moved it (cfe 92.03 -> 95.58, length 1680 -> 1692 = target):
 * - the viewport clip test reads the half-extents 0xB8 / 0xBC as VOLATILE ints: the target re-loads them
 *   in every test (one register, a3) while the centres 0xC0 / 0xC4 stay CSE'd -- a plain int lets uopt
 *   keep both half-extents live (two words short);
 * - no `gs` copy of the global (GS4B read directly): no `or v0,v1` copy;
 * - the 0xEE, 0xB3 and final 0x06 appends go through add(hi, lo) (two edg parameter temps: the target's
 *   0xF0 frame); s / r declared before x so x..h home at sp+0x80..0x74 like the target.
 * Residual: t-register colours shifted by one (self t3 / iz t5 / spr t2 in the target) and the MAX / MIN
 * merge copies in the scissor packets (run 157; all closed by run 405). Spliced into both build trees. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef float f32; typedef double f64;
extern "C" {
extern int D_00000000;
extern int D_4BAF4_flags;
int gl_func_00000000(...);
}
typedef struct { u32 w0, w1; } Gfx4B;
typedef struct { Gfx4B *buf; int n; } GP4B;
template <class T> inline T *me2(T *p) { return p; } /* agent-g run 335: zero-code accessor for the Ctx4B inlines below */
/* agent-g run 336: put4B() stores through a parameter temp (edg reuses those pool words) instead of addF()'s local g
 * (one fresh word per inline site), which pays for nextF()'s named count n: the ctx packets' count is now its own
 * per-site LR, no longer the expression packets' count pool word: 99.409 -> 99.444, same length and frame */
inline void put4B(Gfx4B *g, u32 hi, u32 lo) { g->w0 = hi; g->w1 = lo; }
/* agent-g run 335 (kit CDXXVII): nextF() / addF() for the three ctx-variable packets (first, else arm, final). The
 * four me2() on the count side hold edg pool words before the `n++` temp, so these packets' count / gfx words are no
 * longer the words the expression-receiver packets (0xEE, TEXRECT, B4, B3) use for their receiver: 99.303 -> 99.409 */
struct Ctx4B { char pad[0xC]; GP4B *gfx; Gfx4B *nextP(int n) { gfx->n = n + 1; return &gfx->buf[n]; } void dlS(char *p) { u32 r = gl_func_00000000(*(int *)(p + 0x60)); put4B(me2(this)->nextP(gfx->n), 0x06000000, r); } int ex() { return (int)this; } Gfx4B *nQ4(int z0, int z1, int z2, int z3, int n) { gfx->n = n + 1; return &gfx->buf[n]; } void dW4(char *p) { u32 r = gl_func_00000000(*(int *)(p + 0x60)); put4B(nQ4((int)r, (int)r, (int)r, (int)r, gfx->n), 0x06000000, r); } void dM4(char *p) { u32 r = gl_func_00000000(*(int *)(p + 0x60)); put4B(me2(this)->nQ4((int)r, (int)r, (int)r, (int)r, gfx->n), 0x06000000, r); } int gt(int v, int c) { return v > c + (int)this; } void dlc(char *p) { u32 r = gl_func_00000000(*(int *)(p + 0x60)); put4B(nextF(), 0x06000000, r); } void dlG(char *p) { u32 r = gl_func_00000000(*(int *)(p + 0x60)); put4B(nextG(), 0x06000000, r); } void dlM(char *p) { u32 r = gl_func_00000000(*(int *)(p + 0x60)); put4B(me2(this)->nextG(), 0x06000000, r); } Gfx4B *next() { return &gfx->buf[gfx->n++]; } Gfx4B *nextF() { int n = gfx->n; gfx->n = n + 1; return &gfx->buf[n]; } Gfx4B *nextG() { int n = gfx->n; gfx->n = n + 1; return &gfx->buf[n + 0 * n]; } void addG(u32 hi, u32 lo) { put4B(me2(this)->nextG(), hi, lo); } void addF(u32 hi, u32 lo) { put4B(me2(this)->nextF(), hi, lo); } Gfx4B *nextO() { return &this->gfx->buf[me2(me2(me2(me2(this))))->gfx->n++]; } void addO(u32 hi, u32 lo) { Gfx4B *g = nextO(); g->w0 = hi; g->w1 = lo; } void add(u32 hi, u32 lo) { Gfx4B *g = next(); g->w0 = hi; g->w1 = lo; } };
typedef struct { f32 x, y, z; } V4B;
typedef struct { char pad[0x20]; s16 w; s16 h; char pad24[0x40]; char *dl64; } Spr4B;
#define GS4B (*(char **)((char *)&D_00000000 + 0x254))
#define F4B(p, o, T) (*(T *)((char *)(p) + (o)))
template <class T> inline T *me(T *p) { return p; } /* agent-g run 296: zero-code accessor (kit CCLXXXIV) */
#define PKT4B(c) ((c)->next())
#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif
#define SHL4B(v, s, w) ((unsigned int)(((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))

typedef struct { char pad[0x30]; V4B p; } Cam4B;
/* the 0x1C4 flag word is a separate blank USO data ref (per-site lui in the target) */

extern "C" Cam4B D_4BAF4_cam;
template <class T> inline T idv(T v) { return v; } /* agent-g run 300: zero-code by-value identity inline (one-site wrap sweep) */
/* agent-c run 402 (kit CDLXXXVI): the B4 x clamp's MAX through a by-value int inline -- its parameter is an edg
 * pool word, so the merge temps move onto the target's v1 / a1 (99.693 -> 99.835) */
inline int mx4B(int v) { return MAX(v, 0); }
extern "C" void gl_func_0004BAF4(char *self) {
    V4B pos;
    f32 out[17];
    int iz;
    f32 sx;
    f32 sy;
    Spr4B *spr;
    /* run 158: two more homes below h = the target's frame 0xF0 (every sp offset then matches); agent-g run 335: g and
     * pad0 dropped -- the addF() parameter words of the ctx packets take their 8 bytes (frame still 0xF0) */
    /* agent-g run 233: the clip half-extent e, in pad1's home slot (frame kept); agent-g run 266: one local d for the half-extent AND the self->0xCC pointer */
    /* agent-c runs 391 / 400 / 402 (kits CDLXXVIII, CDLXXXV): the inline-ctx basin -- no named ctx / s / cam; r / d open
     * an inner block after pos.x, x..h a second one; the three 0x06 packets are ONE Ctx4B inline member each (dlG / dlc /
     * dlM) so their `this` is the single a3 pool word; the clip extents are read through Ctx4B gt() / ex() `this`; the
     * first packet as dlG and the 0xEE append as add() at depth 2: 99.586 -> 99.681; the else-arm packet as dlS
     * (count = nextP()'s by-value int parameter) and the B4 x MAX through mx4B(): -> 99.894, same length */

    pos.x = F4B(me((Ctx4B *)&D_4BAF4_cam), 0x30, f32);
    {
    u32 r;
    char *d;
    pos.y = D_4BAF4_cam.p.y;
    pos.z = D_4BAF4_cam.p.z;
    if ((d = (char *)gl_func_00000000(GS4B, &pos, 1, out)) == 0) { /* agent-g run 332: live call result in r (kit CDXXII) */
        return;
    }
    iz = (pos.z + 1.0f) * 16384.0f + F4B(me((Ctx4B *)self), 0xC8, f32);
    if (1) {} /* agent-g run 262 (kit CCC random code-free test search): 97.03 -> 97.39 (agent-c run 288: the paired iz / r tests below dropped, byte-identical -- any two of the four levers are redundant; agent-c run 307: the second `if (1) {}` here dropped, byte-identical) */
    if (F4B(self, 0xC4, int) != 0) {
        sx = F4B(self, 0xB0, f32) * (f32)((f64)(f32)F4B(GS4B, 0xBC, int) * 0.5 * (f64)out[0]);
        sy = F4B(self, 0xB4, f32) * (f32)((f64)(f32)F4B(GS4B, 0xBC, int) * 0.5 * (f64)out[0]);
    } else {
        sx = F4B(self, 0xB0, f32);
        sy = F4B(self, 0xB4, f32);
    } if (self) {}
    spr = F4B(self, 0x84, Spr4B **)[F4B(self, 0xC0, s16)];
    pos.x -= (f32)(spr->w >> 1) * sx;
    pos.y -= (f32)(spr->h >> 1) * sy;
    pos.x += F4B(self, 0xB8, f32);
    pos.y += F4B(self, 0xBC, f32);
    {
    int x;
    int y;
    int w;
    int h;
    x = pos.x;
    y = pos.y;
    w = spr->w; /* agent-g run 233: the folded `+ 0 * y` dropped (x / y back in a0 / a1 after the e change) */
    h = spr->h;
    /* agent-g run 233 (kit CCXLIV): each volatile half-extent read assigned to ONE local e -- the target's four
     * reloads share one coloured register (a3), and the centre 0xC0 now loads before 0xB8 like the target */
    /* agent-g run 266: the half-extent reads go into d (int -> char * -> int round trip, 32-bit), and d is then
     * re-assigned self->0xCC for the dlist call: the pointer takes v0 like the target (97.39 -> 97.54) */
    /* agent-g run 332: the half-extents through ctx (the target's ext / ctx share a3), operand order `x > ext + C0`;
     * agent-g run 335: the second test as `y > C4 + ext` with the idv() dropped (`addu t9,t0,a3` order): 99.291 -> 99.303 */
    /* agent-g run 369: the first / third half-extent reads through d and the second through r (only the last one stays
     * in ctx): the C0 centre takes a2 like the target (ext / ctx / centre variable grid, 161): 99.515 -> 99.539 */
    /* agent-g run 372: with the first packet's count coloured before ctx (addG below), the extent carriers re-gridded
     * (ctx / d / r / by-value idv(idv()) pool temp per read, `x > C0 + ext` order): 99.539 -> 99.586, same length */
    if ((((Ctx4B *)F4B(GS4B, 0xB8, volatile int))->gt(x, F4B(GS4B, 0xC0, int) + 0 * (int)d))) {
        return;
    }
    if ((((Ctx4B *)F4B(GS4B, 0xBC, volatile int))->gt(y, F4B(GS4B, 0xC4, int)))) {
        return;
    }
    if ((x < F4B(GS4B, 0xC0, int) - ((Ctx4B *)F4B(GS4B, 0xB8, volatile int))->ex())) {
        return;
    }
    if ((y < F4B(GS4B, 0xC4, int) - ((Ctx4B *)F4B(GS4B, 0xBC, volatile int))->ex())) {
        return;
    } r = (u32)d; if (r) {}
    /* agent-g run 332: this and the else-arm ctx load through two me() (depth grid over the three ctx loads, 63): 99.243 -> 99.291; agent-g run 266: the folded `+ 0 * (int)(spr->h)` (runs 209-233) dropped with the d change */
    F4B(GS4B, 0x158, Ctx4B *)->dW4(F4B(self, 0xCC, char *)); /* agent-c run 405: dW4 (see the final append); agent-g run 372: addG's named count with a folded counted use (`n + 0 * n`, adjsave 3.0 -> 4.0) is coloured
                               * BEFORE ctx: this packet's ctx / gfx / count = the target's a3 / a0 / a2 (was a2 / a0 / v1 via run 336's addO) */
    /* agent-c run 306 (kit CCCLXXXVIII ring lever): one doubled (value-preserving) iz mask here = one ugen ring slot, so
     * the target's `addu t1; andi t6; sll t9; sw w0; sw w1` WAR order comes out; the three extra `& 8` below (period 4)
     * put the ring back for the rest of the body: 98.021 -> 98.617, same length */
    me(me(F4B(GS4B, 0x158, Ctx4B *)))->add(0xEE000000, ((iz & 0x7FFF) & 0x7FFF) << 16); /* agent-g run 332: 0xEE receiver through two me() (receiver-depth re-grid on the else-arm-d base, final add depth 0): 99.137 -> 99.196 */
    if (*(s32 *)((char *)&D_4BAF4_flags + 0x1C4) & 2) {
        if ((F4B(self, 0x30, int) & 8) && F4B(self, 0x14, char *) != 0) { /* agent-g run 332: one `& 8` again (ring re-tuned for the else arm's d) */
            gl_func_00000000(F4B(GS4B, 0x158, Ctx4B *), (char *)spr + 0x34, F4B(F4B(self, 0x14, char *), 0x1E, s16));
        }
    } else {
        /* agent-g run 332: spr->dl64 through d like the first packet's 0xCC pointer -- the target holds it in v0 (a
         * coloured LR, `lw v0,100(t2); lw a0,96(v0)`), not a ugen temp: 99.007 -> 99.137, same length */
        me(me(F4B(GS4B, 0x158, Ctx4B *)))->dM4(d = spr->dl64); /* agent-c run 402: the count as a by-value parameter (a reused edg pool word, kit CDLXXXVI); run 405: dM4 (see the final append) */
    }
    {
        Gfx4B *_g = (Gfx4B *)(PKT4B(me(me(me(F4B(GS4B, 0x158, Ctx4B *)))))); /* agent-g run 332: both TEXRECT packets' receivers at depth 3 (grid 0-4 x 0-4 x 0xEE 1-3): 99.196 -> 99.243 */
        _g->w0 = (SHL4B(0xE4, 24, 8) |
                  SHL4B(MAX((s16)((x + (int)(w * sx)) << 2), 0), 12, 12) |
                  SHL4B(((((s16)((y + (int)(h * sy) - 1) << 2)) > 0 ? (d = (char *)(int)((s16)((y + (int)(h * sy) - 1) << 2))) : (d = (char *)0)), (int)d), 0, 12)); /* agent-c run 288 (fork C): this MAX as `E <= 0 ? 0 : E` -- the zero is no longer set before the branch, 97.54 -> 97.88. agent-c run 310 (kit CCCXCIV one-variable reading): the target's v0 holds d (the 0xCC pointer) AND this MAX result AND the w1 yl MAX result -- both clamps assigned into d in each arm (int <-> pointer casts), plain `E > 0` order again: 98.617 -> 98.960, same length */
        _g->w1 = (SHL4B(0, 24, 3) |
                  SHL4B(MAX((s16)(x << 2), 0), 12, 12) |
                  SHL4B(((((s16)(y << 2)) > 0 ? (d = (char *)(int)((s16)(y << 2))) : (d = (char *)0)), (int)d), 0, 12)); /* agent-c run 287: this line's `if (1) {}` and the one after the block dropped (stale: instruction stream identical, 97.54) */
    }
    {
        Gfx4B *_g = (Gfx4B *)(PKT4B(me(me(me(F4B(GS4B, 0x158, Ctx4B *))))));
        _g->w0 = SHL4B(0xB4, 24, 8);
        _g->w1 = (SHL4B((0 -
                   (((s16)(x << 2) < 0) ?
                   (((s16)((u16)(s32)(1024.0f / sx)) < 0) ?
                    (mx4B((((s16)(x << 2) * (s16)((u16)(s32)(1024.0f / sx))) >> 7))) :
                    (MIN((((s16)(x << 2) * (s16)((u16)(s32)(1024.0f / sx))) >> 7), 0))) : 0)), 16, 16) |
                  SHL4B((0 -
                   (((y << 2) < 0) ?
                   (((s16)((u16)(s32)(1024.0f / sy)) < 0) ?
                    (MAX((((s16)(y << 2) * (s16)((u16)(s32)(1024.0f / sy))) >> 7), 0)) :
                    (MIN((((s16)(y << 2) * (s16)((u16)(s32)(1024.0f / sy))) >> 7), 0))) : 0)), 0, 16));
    }
    /* agent-g run 296: the RDPHALF_2 append's receiver through five nested zero-code me() accessors and the final
     * append's through two (receiver-depth grid 0-7 over the three appends, 200 + local 176): the RDPHALF_2 / final
     * appends' dl / count colours move toward the target's (97.879 -> 98.009, same length); agent-g run 332: the final
     * append at depth 0 and the 0xEE append at depth 2 on the else-arm-d base; agent-g run 336: this one at depth 2 */
    {
        me(me(F4B(GS4B, 0x158, Ctx4B *)))->add(SHL4B(0xB3, 24, 8), (SHL4B(((u16)(s32)(1024.0f / sx)), 16, 16) | SHL4B(((u16)(s32)(1024.0f / sy)), 0, 16)));
    }
    /* agent-g run 321: the self->0xD0 pointer through d with a BB split before the call (kit CDXII): the pointer is a
     * coloured LR (the target's v0) instead of a ugen temp, 98.960 -> 99.007, same length; agent-g run 336: the BB split
     * dropped together with the B3 receiver at me() depth 2 (random mutation round on the put4B base, both needed):
     * 99.444 -> 99.480 */
    /* agent-c run 405 (kit CDXC): the three 0x06 packets' counts as the 5th by-value parameter of nQ4() (four unused
     * leading parameters fed the call result): edg hands out parameter pool words in creation order, so the 5th one is the
     * SAME word at the first, else-arm and final packet AND the B4 y-MIN merge temp -- one variable, one live range, which
     * also spans block 20 where the a1 gfx word lives, so it colours a2 like the target: 99.894 -> 100 */
    F4B(GS4B, 0x158, Ctx4B *)->dM4(F4B(self, 0xD0, char *));
    }
    }
}
