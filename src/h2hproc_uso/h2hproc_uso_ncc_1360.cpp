/* IDO 7.1 NCC (C++) NM donor for h2hproc_uso_func_00001360 (per-frame menu
 * draw, 0x290 / 164 words). C++: the edg-order virtual call at the top. 6 raw
 * words left (agent-c run 125): in the two `move(prev->font->w + dx, ...)`
 * calls the target holds the font pointer in v1 and the width in v0; here the
 * pointer is a t-ring temp and the width lands in v1. Recipe:
 * - the blink gates as `(cond ? counter : 16) & 0x10` (one pooled ?: temp, v0);
 * - the colour as an `unsigned char c` with `if (v != 1) c = 115; else c = 0;`
 *   (keeps the target's `or a3,v0` argument copy), and `v = ...; if ...; call`
 *   on ONE source line (as1 then puts the buffer address before the v load);
 * - the four items through one `Item *it` (the spilled sp+32 pointer), with
 *   inline reset()/move() members.
 * Spliced into the NON_MATCHING comparison object only. */
extern "C" {
int gl_func_00000000(...);
extern char D_00000000[];
}
struct Pad40 { char pad[0x28]; };
struct VObj : Pad40 { virtual void v1(); virtual void v2(); virtual void v3(); };
struct Font { char pad[0x20]; short w; };
struct Item {
    char pad[0x10];
    Font *font;
    char pad14[4];
    void reset() { gl_func_00000000(this); }
    void move(int x, int y, int n) { gl_func_00000000(this, x, y, n); }
};
struct Ctx { char pad[0x568]; int f568; char pad56c[0x6A8 - 0x56C]; int *f6A8; char pad6ac[0x6B8 - 0x6AC]; int f6B8; };
struct Self {
    char pad[0x2C];
    Ctx *ctx;
    char pad30[4];
    VObj *obj;
    Item a, b, c, d;
    int counter;
};
extern "C" void h2hproc_uso_func_00001360(Self *self) {
    char buf[64];
    int r;
    Item *it;
    int v;
    unsigned char c;
    self->counter++;
    self->obj->v3();
    gl_func_00000000(self->ctx->f568);
    gl_func_00000000(self->ctx->f568);
    if (((self->ctx->f6B8 == 0) ? self->counter : 16) & 0x10) {
        gl_func_00000000(buf, D_00000000 + 0x3FC);
        r = gl_func_00000000(self->ctx->f568, 80, buf);
        gl_func_00000000(self->ctx->f568, r, 30, buf);
        v = self->ctx->f6A8[0]; if (v != 1) c = 115; else c = 0; gl_func_00000000(buf, D_00000000 + 0x400, v, c);
        r = gl_func_00000000(self->ctx->f568, 80, buf);
        gl_func_00000000(self->ctx->f568, r, 80, buf);
    }
    if (((self->ctx->f6B8 == 1) ? self->counter : 16) & 0x10) {
        gl_func_00000000(buf, D_00000000 + 0x40C);
        r = gl_func_00000000(self->ctx->f568, 240, buf);
        gl_func_00000000(self->ctx->f568, r, 30, buf);
        v = self->ctx->f6A8[1]; if (v != 1) c = 115; else c = 0; gl_func_00000000(buf, D_00000000 + 0x410, v, c);
        r = gl_func_00000000(self->ctx->f568, 240, buf);
        gl_func_00000000(self->ctx->f568, r, 80, buf);
    }
    gl_func_00000000(D_00000000);
    it = &self->a;
    it->reset();
    it->move(60, 64, 3);
    it = &self->b;
    it->reset();
    it->move((r = self->a.font->w) + 68, 64, 3);
    it = &self->c;
    it->reset();
    it->move(220, 64, 3);
    it = &self->d;
    it->reset();
    it->move((r = self->c.font->w) + 228, 64, 3);
    gl_func_00000000(self);
}
