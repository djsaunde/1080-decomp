/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_000454C4 (0x7EC / 507 words), agent-c run 160.
 * The light setup: `void Obj::setLights(Ctx *c, f32 *s, f32 k)` (renamed to gl_func_000454C4 with objcopy) fills a
 * Lights record (Ambient + Light[4], 0x48 bytes) taken from the context's light pool with `&lp->buf[lp->count++]`,
 * the ambient colour from the 0x194 element scaled by s[] and clamped by k, then walks the 0x18C node list with a
 * by-value list iterator (`LIter`: cur / nxt in memory, the target's sp+0x3C / 0x40 pair) and emits one stock-gbi
 * gSPLight per lit element (macro form: gDma1p with _SHIFTL, the Gfx pointer `_g` per expansion is the target's s0),
 * then gSPLight(ambient, n + 1) + gSPNumLights, or the one-light default (ambient copied into l[0]).
 * The element flags are read volatile (the target re-loads +0xC4 for the second test). The cfe body was 63.85
 * (2020 bytes); this is 91.40 at 2032 bytes (target 2028). Residual: the 255.0f constant is held in fs1 for the
 * whole function (the target keeps it in f0 and rematerialises it after the loop's call: four words of frame / save
 * and every sp offset shifted by 8), the 0x03000000 gSPLight constant is hoisted into s5 (target `lui at` per site),
 * so n / s / rec / c sit one s-register lower than the target.
 * The direction bytes carry a folded `& 0xFF` (one ugen ring slot each, the 42F4C idiom): 91.40 -> 91.48.
 * Still NON_MATCHING: build/non_matching/ only. */
typedef float f32; typedef unsigned int u32; typedef int s32; typedef unsigned char u8; typedef signed char s8;
extern "C" {
u32 gl_func_00034458(...);
}
struct Gfx { u32 w0, w1; };
struct DLHead { Gfx *buf; int count; };
struct Light { u8 col[3]; char p3; u8 colc[3]; char p7; s8 dir[3]; char pb; int pc; };
struct Ambient { u8 col[3]; char p3; u8 colc[3]; char p7; };
struct Lights { Ambient a; Light l[4]; };
struct LPool { Lights *buf; int count; };
struct Ctx {
    char pad[0xC];
    DLHead *dl;
    char pad10[0x2C - 0x10];
    LPool *lp;
    Gfx *next() { return &dl->buf[dl->count++]; }
    Lights *lights() { return &lp->buf[lp->count++]; }
};
#define _SHIFTL(v, s, w) (((u32)(v) & ((1 << (w)) - 1)) << (s))
#define gDma1p(pkt, c, s, l, p) { Gfx *_g = (Gfx *)(pkt); _g->w0 = (_SHIFTL((c), 24, 8) | _SHIFTL((p), 16, 8) | _SHIFTL((l), 0, 16)); _g->w1 = (u32)(s); }
#define gSPLight(pkt, l, n) gDma1p((pkt), 0x03, (l), sizeof(Light), ((n) - 1) * 2 + 0x86)
#define NUML(n) (((n) + 1) * 32 + 0x80000000)
#define gMoveWd(pkt, index, offset, data) { Gfx *_g = (Gfx *)(pkt); _g->w0 = (_SHIFTL(0xBC, 24, 8) | _SHIFTL((offset), 8, 16) | _SHIFTL((index), 0, 8)); _g->w1 = (u32)(data); }
#define gSPNumLights(pkt, n) gMoveWd(pkt, 2, 0, NUML(n))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
struct LElem { char pad[0xC4]; volatile int flags; char padc8[0xE4 - 0xC8]; f32 dir[3]; char padf0[0xFC - 0xF0]; f32 col[3]; };
struct Node { LElem *data; Node *next; };
struct LIter {
    Node *cur;
    Node *nxt;
    LIter(Node *h) { nxt = h; }
    LElem *next() { cur = nxt; if (cur) { nxt = cur->next; return cur->data; } return 0; }
};
struct Obj {
    char pad[0x18C]; Node *lights; char pad190[4]; LElem *amb;
    void setLights(Ctx *c, f32 *s, f32 k);
};
void Obj::setLights(Ctx *c, f32 *s, f32 k) {
    Lights *rec = c->lights();
    LElem *v = amb;
    u32 r = (u32)(v->col[0] * s[0] * 255.0f) & 0xFF;
    u32 g = (u32)(v->col[1] * s[1] * 255.0f) & 0xFF;
    u32 b = (u32)(v->col[2] * s[2] * 255.0f) & 0xFF;
    if (k != 1.0) {
        r = (u32)MIN(r * k, 255.0f) & 0xFF;
        g = (u32)MIN(g * k, 255.0f) & 0xFF;
        b = (u32)MIN(b * k, 255.0f) & 0xFF;
    }
    rec->a.col[0] = r; rec->a.col[1] = g; rec->a.col[2] = b;
    rec->a.colc[0] = r; rec->a.colc[1] = g; rec->a.colc[2] = b;
    LIter it(lights);
    int n = 0;
    LElem *e;
    while ((e = it.next()) != 0) {
        if (!(e->flags & 0x80) && (e->flags & 2)) {
            Light *l = &rec->l[n];
            s32 cv;
            cv = (s32)(s[0] * e->col[0] * 255.0f); l->col[0] = cv; l->colc[0] = cv;
            cv = (s32)(s[1] * e->col[1] * 255.0f); l->col[1] = cv; l->colc[1] = cv;
            cv = (s32)(s[2] * e->col[2] * 255.0f); l->col[2] = cv; l->colc[2] = cv;
            l->dir[0] = (s32)(e->dir[0] * 120.0f) & 0xFF;
            l->dir[1] = (s32)(e->dir[1] * 120.0f) & 0xFF;
            l->dir[2] = (s32)(e->dir[2] * 120.0f) & 0xFF;
            gSPLight(c->next(), gl_func_00034458(l), n + 1);
            n++;
        }
    }
    if (n != 0) {
        gSPLight(c->next(), gl_func_00034458(&rec->a), n + 1);
        gSPNumLights(c->next(), n <= 0 ? 1 : n);
        return;
    }
    rec->l[0].col[0] = rec->a.col[0]; rec->l[0].col[1] = rec->a.col[1]; rec->l[0].col[2] = rec->a.col[2];
    rec->l[0].colc[0] = rec->a.colc[0]; rec->l[0].colc[1] = rec->a.colc[1]; rec->l[0].colc[2] = rec->a.colc[2];
    gSPLight(c->next(), gl_func_00034458(&rec->l[0]), 1);
    gSPLight(c->next(), gl_func_00034458(&rec->a), 2);
    gSPNumLights(c->next(), 1);
}
