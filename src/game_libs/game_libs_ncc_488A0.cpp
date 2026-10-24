/* IDO 7.1 NCC (C++) donor for gl_func_000488A0 (EXACT, agent-c run 155): the NM C body lifted under NCC (agent-c run 134 sweep, cfe 81.4 -> NCC 98.6, 112 words = target). Run 136: the post-increment split `n = ctx->dl->count; ctx->dl->count = n + 1;` moves count to v1.
 * Run 155 (uopt dump, -Wo,-zdbug:5): ctx, n and the ctx->dl load all have adjsave 31.5, so they colour in
 * itab-bit order. A named DL cursor d plus a dead `while (0) { d = 0; n = 0; }` at the top gives d and n bits
 * before ctx (d -> v0, n -> v1, ctx -> a0 like the target). c2 is read before the `if (obj) {}` BB split so it
 * shares the header's unit (v0/v1/a0/a1 taken -> a2). Spliced into both build trees via REPLACE_FUNC_BODY. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
#undef NULL
#define NULL 0
typedef struct { float x, y, z; } Vec3;
typedef struct VT3FC58 { char pad[8]; short off8; char padA[2]; void (*fnC)(); char pad10[0x18]; short off28; char pad2A[2]; void (*fn2C)(); } VT3FC58;
typedef struct { VT3FC58 *vt0; char pad[0x24]; VT3FC58 *vt28; } O3FC58;
typedef struct { unsigned int w0, w1; } Gfx488A0;
typedef struct { Gfx488A0 *base; int count; } DLBuf488A0;
typedef struct { short ob[3]; unsigned short flag; short tc[2]; unsigned char cn[4]; } Vtx488A0;
typedef struct { char pad[0xC]; DLBuf488A0 *dl; char pad2[0xC]; Vtx488A0 **vtx; } Ctx488A0;
typedef struct { Ctx488A0 *ctx; int nv; int nline; int vbase; int pad; short ln[236][3]; short wd[1]; } Obj488A0;

extern "C" char c2[];

extern "C" char g[];
extern "C" char ctx[];




























extern "C" {
extern int gl_func_00000000(...);
extern void gl_proto_41148(int, int, int, int, int, double);
extern short count;
extern Vec3 vtx;
extern f32 n[3];
extern O3FC58 *obj;
}
extern "C" void gl_func_000488A0(Obj488A0 *obj) {
    Ctx488A0 *c2; Gfx488A0 *g; int i; Ctx488A0 *ctx; int n;
    DLBuf488A0 *d;
    while (0) { d = 0; n = 0; }
    if (obj->nv != 0) {
        ctx = obj->ctx; d = ctx->dl; n = d->count; d->count = n + 1;
        g = &ctx->dl->base[n];
        g->w0 = 0x04000000 | (((obj->nv << 10) | ((obj->nv << 4) - 1)) & 0xFFFF); c2 = obj->ctx; if (obj) {}
        g->w1 = gl_func_00000000(*c2->vtx + obj->vbase);
        *(int *)((char *)obj + 0x58C) += obj->nv;
        for (i = 0; i < obj->nline; i++) {
            if (obj->wd[i] != 1) {
                ctx = obj->ctx; d = ctx->dl; n = d->count; d->count = n + 1;
                g = &ctx->dl->base[n];
                g->w0 = 0xB5000000;
                g->w1 = ((((obj->ln[i][0] - obj->vbase) * 2) & 0xFF) << 16) | ((((obj->ln[i][1] - obj->vbase) * 2) & 0xFF) << 8) | (obj->wd[i] & 0xFF);
            } else {
                ctx = obj->ctx; d = ctx->dl; n = d->count; d->count = n + 1;
                g = &ctx->dl->base[n];
                g->w0 = 0xB5000000;
                g->w1 = ((((obj->ln[i][0] - obj->vbase) * 2) & 0xFF) << 16) | ((((obj->ln[i][1] - obj->vbase) * 2) & 0xFF) << 8);
            }
        }
    }
    gl_func_00000000(obj);
}
