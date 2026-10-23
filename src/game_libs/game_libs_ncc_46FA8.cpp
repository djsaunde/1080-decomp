/* IDO 7.1 NCC (C++) donor for gl_func_00046FA8 (0x13C / 79 words): emit a
 * gDPFillRectangle + gDPPipeSync pair through the object's display-list
 * context, after up to three pre-dispatch calls; the else arm calls out.
 * - C++: the context's inline next() append (lw dl; lw n; n+1; sw; reload dl)
 *   and the memory-homed parameters (edg keeps x/y/w/h in their arg homes).
 * - `(g = a0->mgr->ctx)->next()` makes the context the pooled next() `this`
 *   (v1) for both appends; a named `Gfx *g` result in next() puts the
 *   second append's list/index/slot webs in a2/v0/a3 (one fewer counted
 *   variable load before uopt's 20-load block cut, kit XXXV).
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY (agent-c run 126). */
extern "C" void gl_func_00000000(...);
struct Gfx { unsigned int w0, w1; };
struct DL { Gfx *buf; int n; };
struct Ctx { char pad[0xC]; DL *dl; Gfx *next() { int n = dl->n++; Gfx *g = &dl->buf[n]; return g; } };
struct Mgr { char pad[0x158]; Ctx *ctx; };
struct Obj { char pad[0x254]; Mgr *mgr; char pad2[0x28C - 0x258]; int done; };
#define _SHIFTL(v, s, w) ((unsigned int)(((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))
extern "C" void gl_func_00046FA8(Obj *a0, int x, int y, int w, int h, int c) {
    if (a0->done == 0) {
        if (c != 0) {
            gl_func_00000000(a0->mgr->ctx, c);
            gl_func_00000000(a0->mgr->ctx, c);
            gl_func_00000000(a0->mgr->ctx, c);
        }
        Ctx *g;
        Gfx *p = (g = a0->mgr->ctx)->next();
        p->w0 = _SHIFTL(0xF6, 24, 8) | _SHIFTL(x + w - 1, 14, 10) | _SHIFTL(y + h - 1, 2, 10);
        p->w1 = _SHIFTL(x, 14, 10) | _SHIFTL(y, 2, 10);
        p = g->next();
        p->w0 = _SHIFTL(0xE7, 24, 8);
        p->w1 = 0;
    } else {
        gl_func_00000000(x, y, w, h);
    }
}
