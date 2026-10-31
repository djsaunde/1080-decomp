/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00046050 (agent-c run 152): the NM C body (EMIT path as the then-arm of `== 0`, call in the else arm) lifted under NCC; NCC colours the early _q/_n DL temps v0/a1 like the target where cfe gives a1/a2; the first character is read through a copy of the cursor (target `or v1,s3; lbu a1,0(v1)`), both the entry read and the loop-bottom read, 86.48 -> 86.91.
 * agent-c run 182: the character dispatch is a `switch (ch)` (edg compare chain `beq 10 / beq 9 / beql 0x30 / b default`
 * with the case bodies out of line, as the target) whose newline case jumps past the advance (`goto next`), and x is read
 * after the switch: 86.91 -> 89.54, length 1832 -> 1840 (target 1848); charw read before lineh (s1 / s2 as the
 * target): -> 89.63.
 * agent-g run 220: rewritten as real C++ (the 2DE0 / 329C kit): every display-list append is the gfx context's
 * inline Ctx::next() reached through o->sub->ctx, the fixed packets are plain word pairs, and the glyph is the
 * stock gbi.h gSPTextureRectangle with `CTX->next()` as its pkt argument (its two gImmp1 re-evaluations ARE the
 * RDPHALF_1 / RDPHALF_2 appends; s = (cc & 0xF) << 8, t = ((cc & 0xF0) >> 1) << 5, dsdx / dtdy = 0x2000 / charw
 * / lineh): length 1840 -> 1848 = target. The right-margin word is read as an ARRAY global (`D_46050_m[0]`):
 * through `&D_00000000` / a held `int *` uopt folds it into `lui; lw %lo` per iteration, as an array element
 * the address is a loop-invariant web hoisted into s8 as in the target (which then also stops the 0xE4000000
 * constant from taking s8). 89.71 -> 92.20 standalone. LEFT: the first two prologue appends read o from a0
 * (target) and use v0 / a1 for the dl / count temps; cc is a3 (target a0); the E4 w1 / B4 / B3 schedule.
 * agent-c run 252: s is the texel column scaled to 10.5 in two steps, `((cc & 0xF) * 8) << 5` (same `andi 0xF` +
 * one `sll 24` after folding, but the deeper tree makes ugen evaluate s BEFORE t, as the target does): 92.43 ->
 * 92.78 standalone, instruction shape unchanged. Then the glyph's pkt is `CTX->me()->next()`: routing `this` through
 * a zero-code inline accessor (any of me() / me()->me() / ref() / a local copy) gives the target's whole texrect
 * schedule (y before x in w1, 0x2000 / lineh after the RDPHALF_1 ctx load, the RDPHALF_2 word first) -- 92.78 ->
 * 96.71; per-site pkt grid on top (me() on the 0xBA000C02 append, folded `+ 0` on the 0xE6000000 and the first
 * epilogue append): -> 98.44. Superseded by ONE site: the glyph pkt `o->me()->sub->ctx->next() + 0` (Font::me()
 * on the outer object + a folded `+ 0` on the pointer; every other append plain) -> 99.10 with the t-ring equal
 * to the target everywhere. LEFT: a consistent swap of two next() pool colours (target ctx v1 / n a2, ours a2 /
 * v1), the arg1 increment above the first test (delay slot), cc a0 (ours a2). Then a greedy over per-site pkt
 * spellings with a template zero-code `me(p)` ({CTX, me(CTX), me(me(CTX)), me(o)->sub->ctx, ...} x {next, next2}
 * x {"", + 0}): glyph `me(CTX)->next() + 0`, first prologue and first epilogue appends `me(me(CTX))->next()`, last
 * epilogue append `me(CTX)->next() + 0`: -> 99.36 (instruction multiset = target).
 * agent-c run 253: the texrect macro takes its three appends as separate pkt arguments (the stock macro re-evaluates
 * one pkt expression three times, so the per-site lever could not reach the RDPHALF appends): glyph TEXRECT append
 * `me(CTX)->next() + 0`, RDPHALF_1 / RDPHALF_2 appends plain `CTX->next()` -> 99.58 (best-improvement greedy over 22
 * sites x 15 accessor spellings, plateau). LEFT: prologue append result colours, F5801000 ctx t0, s3++ delay slot,
 * `beq 10,ch` operand order, texrect dl / n / pkt rotation (target a1 / a2 / v0, ours v0 / a1 / a2).
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32;
typedef float f32;
extern "C" {
extern int D_00000000; extern int D_46050_m[];
extern int gl_func_00034458(...);
}
#define _SHIFTL(v, s, w) ((unsigned int) (((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))
struct Gwords { unsigned int w0, w1; };
union Gfx { Gwords words; long long force_structure_alignment; };
#define gImmp1(pkt, c, p0) { Gfx *_g = (Gfx *)(pkt); _g->words.w0 = _SHIFTL((c), 24, 8); _g->words.w1 = (unsigned int)(p0); }
#define gW(pkt, a, b) { Gfx *_g = (Gfx *)(pkt); _g->words.w0 = (a); _g->words.w1 = (b); }
#define G_TEXRECT 0xe4
#define G_RDPHALF_1 0xb4
#define G_RDPHALF_2 0xb3
#define gSPTextureRectangle(pkt, pkt2, pkt3, xl, yl, xh, yh, tile, s, t, dsdx, dtdy) \
{ Gfx *_g = (Gfx *)(pkt); \
  _g->words.w0 = (_SHIFTL(G_TEXRECT, 24, 8) | _SHIFTL(xh, 12, 12) | _SHIFTL(yh, 0, 12)); \
  _g->words.w1 = (_SHIFTL(tile, 24, 3) | _SHIFTL(xl, 12, 12) | _SHIFTL(yl, 0, 12)); \
  gImmp1(pkt2, G_RDPHALF_1, (_SHIFTL(s, 16, 16) | _SHIFTL(t, 0, 16))); \
  gImmp1(pkt3, G_RDPHALF_2, (_SHIFTL(dsdx, 16, 16) | _SHIFTL(dtdy, 0, 16))); }
template <class T> inline T *me(T *p) { return p; }
struct DL { Gfx *buf; int n; };
/* agent-c run 242: next2() = the explicit `n = dl->n; dl->n = n + 1` slot (kit CV) for the first two prologue appends
 * and the last epilogue append (greedy per-site grid): their dl temp takes the target's v0, 92.27 -> 92.43 standalone */
struct Ctx { char pad[0xC]; DL *dl; Gfx *next() { return &dl->buf[dl->n++]; } Gfx *next2() { int n = dl->n; dl->n = n + 1; return &dl->buf[n]; } };
struct Sub { char pad[0x158]; Ctx *ctx; };
struct Font { char pad[0x21C]; s32 x; s32 y; s32 cw; s32 lh; char pad22c[0x254 - 0x22C]; Sub *sub; char pad258[0x28C - 0x258]; s32 mode; };
#define CTX (o->sub->ctx)
extern "C" s32 gl_func_00046050(Font *o, u8 *arg1, s32 arg2) {
    s32 startx = o->x;
    s32 charw;
    s32 lineh;
    s32 ch;
    s32 cc;

    charw = o->cw; lineh = o->lh; /* agent-g run 228: one line (kit CCXXXIV line order): 92.20 -> 92.23 */

    if (o->mode == 0) {
        gW(me(me(CTX))->next(), 0xE7000000, 0); if (1) {}
        gW(CTX->next2(), 0xBA001402, 0);
        gW(CTX->next(), 0xB6000000, 1);
        gW(CTX->next(), 0xB900031D, 0x0C087008);
        gW(CTX->next(), 0xF9000000, 0x80);
        gW(CTX->next(), 0xBB000001, 0x80008000);
        gW(CTX->next(), 0xFCFFFFFF, 0xFFFCF279);
        gW(CTX->next(), 0xBA000C02, 0);
        gW(CTX->next(), 0xBA001301, 0);
        gW(CTX->next(), 0xFD900000, gl_func_00034458((void *)&D_00000000));
        gW(CTX->next(), 0xF5900000, 0x07000000);
        gW(CTX->next(), 0xE6000000, 0);
        gW(CTX->next(), 0xF3000000, 0x077FF100);
        gW(CTX->next(), 0xE7000000, 0);
        gW(CTX->next(), 0xF5801000, 0);
        gW(CTX->next(), 0xF2000000, 0x1FC0FC);
    }

    { u8 *p = arg1 + 0 * (int)(arg2); arg1 = p + 1; ch = *p; } /* agent-g run 231: folded counted use of arg2 (zero-use sweep): 92.23 -> 92.25 */
    if (ch != 0) {
        do {
            cc = ch;
            switch (ch) {
            case 10:
                o->x = startx;
                o->y = o->y + lineh;
                goto next;
            case 9:
                ch = 0x20;
                cc = 0x20;
                break;
            case 0x30:
                ch = 0x4F;
                cc = 0x4F;
                break;
            }
            if ((D_46050_m[0] - 0x14) < (o->x + charw)) {
                o->x = startx;
                o->y = o->y + lineh;
            }
            if (cc < 0x80) {
                if (o->mode == 0) {
                    gSPTextureRectangle(me(CTX)->next() + 0, CTX->next(), CTX->next(), o->x * 4, o->y * 4, (o->x + charw) * 4, (o->y + lineh) * 4, 0,
                        ((cc & 0xF) * 8) << 5, ((cc & 0xF0) >> 1) << 5, 0x2000 / charw, 0x2000 / lineh);
                } else {
                    gl_func_00034458(o, ch, o->x, o->y);
                }
            }
            o->x = o->x + charw;
        next:
            if (!(o->x < arg2)) {
                break;
            }
            { u8 *p = arg1 + 0 * (int)(o); arg1 = p + 1; ch = *p; }
        } while (ch != 0);
    }

    if (o->mode == 0) {
        gW(me(me(CTX))->next(), 0xE7000000, 0);
        gW(CTX->next(), 0xBB000000, -1);
        gW(me(CTX)->next() + 0, 0xBA001402, 0x300000);
    }
    return 0;
}
