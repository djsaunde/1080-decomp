/* IDO 7.1 NCC (C++) EXACT donor for gl_func_00045178 (0x21C / 135 words), agent-c run 162.
 * History-lane clamp (-32700 / 32700 literals) + one Mtx slot from the context's matrix pool + one
 * stock-gbi gDma1p packet (0x01030040 = F3DEX gSPMatrix(m, 3, 64)) + three camera Vec3 calls.
 * The cfe body (99.99, 2 words) could not place the packet pointer's spill: the target spills it at
 * sp+0x24, one slot BELOW the last named local -- under NCC that is the macro's block-scoped `_g`,
 * homed after the one pooled temp of the `n3 = c->dl->count++` post-increment (kit CXII: named block
 * locals are homed after the temps allocated before them). Decl order t, a, b, q, i, c, n3, m, v, r =
 * the target's homes; the upper clamp bound is a literal (a named `hi` costs a frame word); the w1 call
 * is int-returning (gl_func_00000000_i), the others void. Spliced over an empty placeholder in both
 * build trees; the three literals' .rodata reloc is renamed gl_func_00045178_rodata (pinned 0x1B60). */
typedef float f32; typedef unsigned int u32;
struct Gfx { u32 w0, w1; };
struct Mtx { int m[16]; };
struct DLHead { Gfx *buf; int count; };
struct MHead { Mtx *buf; int count; };
struct Ctx {
    char pad[0xC];
    DLHead *dl;
    char pad10[0x3C - 0x10];
    MHead *mh;
};
struct V3 { f32 x, y, z; };
struct Cam { char pad[0xB4]; f32 q[12]; };
struct Own { char pad[0x70]; Cam *cam; char pad74[0x158 - 0x74]; Ctx *ctx; };
struct Obj { char pad[0x90]; f32 h[3]; char pad9c[0x254 - 0x9C]; Own *own; };
extern "C" {
void gl_func_00000000(...);
u32 gl_func_00000000_i(...);
extern char D_00000000[];
extern char D_45178_c1, D_45178_c2, D_45178_c3;
}
#define _SHIFTL(v, s, w) (((u32)(v) & ((1 << (w)) - 1)) << (s))
#define gDma1p(pkt, c, s, l, p) { Gfx *_g = (Gfx *)(pkt); _g->w0 = (_SHIFTL((c), 24, 8) | _SHIFTL((p), 16, 8) | _SHIFTL((l), 0, 16)); _g->w1 = (u32)(s); }
extern "C" void gl_func_00045178(Obj *o) {
    Cam *t;
    V3 a, b;
    f32 *q;
    int i;
    Ctx *c;
    int n3;
    Mtx *m;
    f32 v, r;
    v = o->h[0];
    if (v < -32700.0f) { r = -32700.0f; } else { if (32700.0f < v) { r = 32700.0f; } else { r = v; } }
    v = o->h[1];
    o->h[0] = r;
    if (v < -32700.0f) { r = -32700.0f; } else if (32700.0f < v) { r = 32700.0f; } else { r = v; }
    v = o->h[2];
    o->h[1] = r;
    if (v < -32700.0f) { r = -32700.0f; } else if (32700.0f < v) { r = 32700.0f; } else { r = v; }
    o->h[2] = r;
    c = o->own->ctx;
    i = c->mh->count++; m = &c->mh->buf[i];
    gl_func_00000000(D_00000000 + 0x60, m);
    n3 = c->dl->count++;
    gDma1p(&c->dl->buf[n3], 1, gl_func_00000000_i(m), 64, 3);
    t = o->own->cam;
    q = t->q;
    if (t != 0) {
        a.x = q[0]; a.y = q[4]; a.z = q[8];
        b.x = q[1]; b.y = q[5]; b.z = q[9];
        gl_func_00000000(&D_45178_c1, &a, &b);
        gl_func_00000000(&D_45178_c2, &a, &b);
        a.y = 0.0f; a.z = 0.0f; b.x = 0.0f; b.y = 0.0f; a.x = 1.0f; b.z = -1.0f;
        gl_func_00000000(&D_45178_c3, &a, &b);
    }
}
