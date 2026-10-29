/* IDO 7.1 NCC (C++) NM-only comparison donor for func_00008BD4 (collision impulse, 209 words).
 * agent-g run 157 / 182: the cfe NM body under NCC (amt in $f20), 97.77.
 * agent-g run 184 (kit CXXXIII): rewritten as C++ -- `lwc1 180(a0); addiu a0,a0,180` = an inline
 * V3::operator+=(const V3 &) on b->ctx->vel (and ctx->acc), `addiu v1,v1,2408 / 2576` = edg's compound-assignment
 * lvalue temps (`ctx->imp += amt`, `ctx->fl |= b->fl`), ctx->cnt as `cnt = cnt + 1`; frame 184 = target (was 200);
 * t declared in its own block; `q = (t < 0.0f) ? -t : t`. The surface-code call returns an unsigned char
 * (per-site alias func_8bd4_c): that brings back the target's `move a1,v0` copy of the code before the 0x62 test
 * (as a0 here). 97.77 -> 97.92. LEFT: the code copy register (a0 vs a1), the second scaledBy result 16 high.
 * agent-c run 204: the 0x61/0x62/0x63 skip test is ONE `||` expression on an int code (the target's
 * xori/sltiu/bnez chain with the code copy in a1 and v0 consumed by the 0x62 test), and ctx->acc accumulates
 * through a reference-returning V3::add (the acc lvalue temp then takes v0 with the target's addiu order):
 * 97.84 -> 98.73 standalone. LEFT: the vel += temp (a0 in the target; a ref-returning += there fixes it but
 * grows the frame by 8) and the second scaledBy result 16 high.
 * agent-c run 238: EXACT (98.83 -> 100). (1) The second scaling is its own inline (scaledBy2) whose result r is
 * declared AFTER hx / hy / hz (`f32 hx, hy, hz; V3 r; hx = ...`): the three scalar homes then sit above r, so r
 * lands 16 bytes lower (target sp+64); (2) the else arm's |f970| is one variable (`q = f970; q = q < 0 ? -q : q;`),
 * one home fewer below r, so the frame stays 184; (3) vel += impA through a named reference
 * (`V3 &v = b->ctx->vel; v += impA;`): the vel pointer takes a0 with the target's lwc1 / addiu order.
 * Spliced over a placeholder on both build paths via REPLACE_FUNC_BODY. */
typedef float f32;
typedef unsigned short u16;
extern "C" {
extern char D_00000000;
extern int func_00000000(...);
extern unsigned char func_8bd4_c(...);
extern void func_8bd4_a(void *, void *, void *, void *, float);
extern void func_8bd4_b(void *, float, void *, float);
extern void func_8bd4_heavy(void *, void *, void *);
}
struct V3 {
    f32 x, y, z;
    V3() {}
    void operator+=(const V3 &o) { x += o.x; y += o.y; z += o.z; }
    V3 &add(const V3 &o) { x += o.x; y += o.y; z += o.z; return *this; }
};
static inline V3 scaledBy(const V3 &d, f32 s) {
    V3 r; f32 hx = d.x * s; f32 hy = d.y * s; f32 hz = d.z * s;
    r.x = hx; r.y = hy; r.z = hz;
    return r;
}
static inline V3 scaledBy2(const V3 &d, f32 s) {
    f32 hx, hy, hz; V3 r; hx = d.x * s; hy = d.y * s; hz = d.z * s;
    r.x = hx; r.y = hy; r.z = hz;
    return r;
}
struct Ctx {
    char p0[0xB4]; V3 vel; char pC0[0x938 - 0xC0];
    int cnt; V3 acc; char p948[0x968 - 0x948]; f32 imp; char p96C[4]; f32 f970;
    char p974[0x9A4 - 0x974]; short code; char p9A6[0xA10 - 0x9A6]; u16 fl;
};
struct G { char p[0x84]; int h; };
struct Body { char p0[0x114]; char s114[0xC]; char s120[0x2C]; int k14C; char p150[0x170 - 0x150]; Ctx *ctx; u16 fl; };
extern "C" void func_00008BD4(Body *b, float amt, V3 *dir, int a3) {
    u16 fl = b->fl;
    int skip;
    int code;
    V3 impA, nnA;

    if (fl & 0x1F0) {
        if (amt < 0.0f) {
            code = func_8bd4_c((*(G **)&D_00000000)->h, b->k14C);
            skip = code == 0x61 || code == 0x62 || code == 0x63;
            if (!skip) {
                b->ctx->cnt++;
                b->ctx->code = code;
                if (amt < -30.0f) amt = -30.0f;
                b->ctx->fl |= b->fl;
                impA = scaledBy(*dir, (float)((double)-amt * 0.5));
                { V3 &v = b->ctx->vel; v += impA; }
                b->ctx->acc.add(*dir);
                b->ctx->imp += amt;
                nnA = scaledBy2(*dir, -amt);
                func_8bd4_heavy(b->ctx, b->s120, &nnA);
            }
        }
        return;
    }
    if (fl & 0xF) {
        if (amt < 50.0f) {
            b->ctx->cnt++;
            b->ctx->fl |= b->fl;
            b->ctx->imp += amt;
            b->ctx->acc.add(*dir);
            f32 q = b->ctx->f970;
            q = (q < 0.0f) ? -q : q;
            func_8bd4_a(b->ctx, b->s120, b->s114, dir, q * q);
            func_8bd4_b(b, amt, dir, 0.1f);
        }
    }
}
