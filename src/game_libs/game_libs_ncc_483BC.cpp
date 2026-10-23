/* IDO 7.1 NCC (C++) donor for gl_func_000483BC: G_VTX + per-triple G_TRI1 packets (488A0 sibling).
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
/* (agent-c run 134) 483BC = emit a G_VTX packet for the object's nv vertices
 * (0x04 | nv<<10 | nv*16-1, address from the context's vertex table + vbase),
 * bump the 0x58C vertex counter, then one G_TRI1 (0xBF) packet per 6-byte index
 * triple (indices relative to vbase, doubled), then the trailing call; nothing
 * when nv == 0. Sibling of 488A0 (same packet allocator, line packets there).
 * cfe 88.9 -> NCC lift 92.5 -> EXACT with the 488A0 struct form, the early
 * return, and the loop packet in its own pointer h (one shared g colours the
 * loop packet t0 and shifts every temp). */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef float f32; typedef double f64;
typedef struct { unsigned int w0, w1; } Gfx488A0;
typedef struct { Gfx488A0 *base; int count; } DLBuf488A0;
typedef struct { short ob[3]; unsigned short flag; short tc[2]; unsigned char cn[4]; } Vtx488A0;
typedef struct { char pad[0xC]; DLBuf488A0 *dl; char pad2[0xC]; Vtx488A0 **vtx; } Ctx488A0;
typedef struct { Ctx488A0 *ctx; int nv; int nline; int vbase; int pad; short ln[236][3]; short wd[1]; } Obj488A0;






























#undef NULL
#define NULL 0
extern "C" {
extern int gl_func_00000000(...);
}
extern "C" void gl_func_000483BC(Obj488A0 *obj) {
    Ctx488A0 *c2; Gfx488A0 *g; int i; Ctx488A0 *ctx; int n;
    if (obj->nv == 0) return;
    {
        ctx = obj->ctx; n = ctx->dl->count++;
        g = &ctx->dl->base[n];
        g->w0 = 0x04000000 | (((obj->nv << 10) | ((obj->nv << 4) - 1)) & 0xFFFF);
        g->w1 = gl_func_00000000(*(c2 = obj->ctx)->vtx + obj->vbase);
        *(int *)((char *)obj + 0x58C) += obj->nv;
        for (i = 0; i < obj->nline; i++) {
            Gfx488A0 *h;
            ctx = obj->ctx; n = ctx->dl->count++;
            h = &ctx->dl->base[n];
            h->w0 = 0xBF000000;
            h->w1 = ((((obj->ln[i][0] - obj->vbase) * 2) & 0xFF) << 16) | ((((obj->ln[i][1] - obj->vbase) * 2) & 0xFF) << 8) | (((obj->ln[i][2] - obj->vbase) * 2) & 0xFF);
        }
    }
    gl_func_00000000(obj);
}
