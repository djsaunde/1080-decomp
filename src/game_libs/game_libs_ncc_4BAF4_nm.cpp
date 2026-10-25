/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_0004BAF4 (0x69C / 423 words), agent-c run 157:
 * the cfe NM body (billboard sprite draw) lifted under NCC with the display-list appends as cfront
 * inlines (Ctx4B::next / Ctx4B::add). What moved it (cfe 92.03 -> 95.58, length 1680 -> 1692 = target):
 * - the viewport clip test reads the half-extents 0xB8 / 0xBC as VOLATILE ints: the target re-loads them
 *   in every test (one register, a3) while the centres 0xC0 / 0xC4 stay CSE'd -- a plain int lets uopt
 *   keep both half-extents live (two words short);
 * - no `gs` copy of the global (GS4B read directly): no `or v0,v1` copy;
 * - the 0xEE, 0xB3 and final 0x06 appends go through add(hi, lo) (two edg parameter temps: the target's
 *   0xF0 frame); s / r declared before x so x..h home at sp+0x80..0x74 like the target.
 * Residual: t-register colours shifted by one (self t3 / iz t5 / spr t2 in the target) and the MAX / MIN
 * merge copies in the scissor packets. Still NON_MATCHING: spliced into build/non_matching/ only. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef float f32; typedef double f64;
extern "C" {
extern int D_00000000;
extern int D_4BAF4_flags;
int gl_func_00000000(...);
}
typedef struct { u32 w0, w1; } Gfx4B;
typedef struct { Gfx4B *buf; int n; } GP4B;
struct Ctx4B { char pad[0xC]; GP4B *gfx; Gfx4B *next() { return &gfx->buf[gfx->n++]; } void add(u32 hi, u32 lo) { Gfx4B *g = next(); g->w0 = hi; g->w1 = lo; } };
typedef struct { f32 x, y, z; } V4B;
typedef struct { char pad[0x20]; s16 w; s16 h; char pad24[0x40]; char *dl64; } Spr4B;
#define GS4B (*(char **)((char *)&D_00000000 + 0x254))
#define F4B(p, o, T) (*(T *)((char *)(p) + (o)))
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
extern "C" void gl_func_0004BAF4(char *self) {
    V4B pos;
    f32 out[17];
    int iz;
    f32 sx;
    f32 sy;
    Spr4B *spr;
    Ctx4B *ctx;
    f32 s;
    u32 r;
    int x;
    int y;
    int w;
    int h;
    Gfx4B *g;
    volatile int pad0; /* run 158: two more homes below h = the target's frame 0xF0 (every sp offset then matches) */
    volatile int pad1;

    {
        Cam4B *cam = &D_4BAF4_cam;
        pos.x = cam->p.x;
        pos.y = cam->p.y;
        pos.z = cam->p.z;
    }
    if (gl_func_00000000(GS4B, &pos, 1, out) == 0) {
        return;
    }
    iz = (pos.z + 1.0f) * 16384.0f + F4B(self, 0xC8, f32);
    if (F4B(self, 0xC4, int) != 0) {
        s = (f32)((f64)(f32)F4B(GS4B, 0xBC, int) * 0.5 * (f64)out[0]);
        sx = F4B(self, 0xB0, f32) * s;
        sy = F4B(self, 0xB4, f32) * s;
    } else {
        sx = F4B(self, 0xB0, f32);
        sy = F4B(self, 0xB4, f32);
    }
    spr = F4B(self, 0x84, Spr4B **)[F4B(self, 0xC0, s16)];
    pos.x -= (f32)(spr->w >> 1) * sx;
    pos.y -= (f32)(spr->h >> 1) * sy;
    pos.x += F4B(self, 0xB8, f32);
    pos.y += F4B(self, 0xBC, f32);
    x = pos.x;
    y = pos.y;
    w = spr->w;
    h = spr->h;
    if (F4B(GS4B, 0xC0, int) + F4B(GS4B, 0xB8, volatile int) < x || F4B(GS4B, 0xC4, int) + F4B(GS4B, 0xBC, volatile int) < y ||
        x < F4B(GS4B, 0xC0, int) - F4B(GS4B, 0xB8, volatile int) || y < F4B(GS4B, 0xC4, int) - F4B(GS4B, 0xBC, volatile int)) {
        return;
    }
    ctx = F4B(GS4B, 0x158, Ctx4B *);
    r = gl_func_00000000(F4B(F4B(self, 0xCC, char *), 0x60, int));
    g = PKT4B(ctx);
    g->w0 = 0x06000000;
    g->w1 = r;
    F4B(GS4B, 0x158, Ctx4B *)->add(0xEE000000, (iz & 0x7FFF) << 16);
    if (*(s32 *)((char *)&D_4BAF4_flags + 0x1C4) & 2) {
        if ((F4B(self, 0x30, int) & 8) && F4B(self, 0x14, char *) != 0) {
            gl_func_00000000(F4B(GS4B, 0x158, Ctx4B *), (char *)spr + 0x34, F4B(F4B(self, 0x14, char *), 0x1E, s16));
        }
    } else {
        ctx = F4B(GS4B, 0x158, Ctx4B *);
        r = gl_func_00000000(F4B(spr->dl64, 0x60, int));
        g = PKT4B(ctx);
        g->w0 = 0x06000000;
        g->w1 = r;
    }
    {
        Gfx4B *_g = (Gfx4B *)(PKT4B(F4B(GS4B, 0x158, Ctx4B *)));
        _g->w0 = (SHL4B(0xE4, 24, 8) |
                  SHL4B(MAX((s16)((x + (int)(w * sx)) << 2), 0), 12, 12) |
                  SHL4B(MAX((s16)((y + (int)(h * sy) - 1) << 2), 0), 0, 12));
        _g->w1 = (SHL4B(0, 24, 3) |
                  SHL4B(MAX((s16)(x << 2), 0), 12, 12) |
                  SHL4B(MAX((s16)(y << 2), 0), 0, 12)); if (1) {}
    } if (1) {}
    {
        Gfx4B *_g = (Gfx4B *)(PKT4B(F4B(GS4B, 0x158, Ctx4B *)));
        _g->w0 = SHL4B(0xB4, 24, 8);
        _g->w1 = (SHL4B((0 -
                   (((s16)(x << 2) < 0) ?
                   (((s16)((u16)(s32)(1024.0f / sx)) < 0) ?
                    (MAX((((s16)(x << 2) * (s16)((u16)(s32)(1024.0f / sx))) >> 7), 0)) :
                    (MIN((((s16)(x << 2) * (s16)((u16)(s32)(1024.0f / sx))) >> 7), 0))) : 0)), 16, 16) |
                  SHL4B((0 -
                   (((y << 2) < 0) ?
                   (((s16)((u16)(s32)(1024.0f / sy)) < 0) ?
                    (MAX((((s16)(y << 2) * (s16)((u16)(s32)(1024.0f / sy))) >> 7), 0)) :
                    (MIN((((s16)(y << 2) * (s16)((u16)(s32)(1024.0f / sy))) >> 7), 0))) : 0)), 0, 16));
    }
    {
        F4B(GS4B, 0x158, Ctx4B *)->add(SHL4B(0xB3, 24, 8), (SHL4B(((u16)(s32)(1024.0f / sx)), 16, 16) | SHL4B(((u16)(s32)(1024.0f / sy)), 0, 16)));
    }
    ctx = F4B(GS4B, 0x158, Ctx4B *);
    r = gl_func_00000000(F4B(F4B(self, 0xD0, char *), 0x60, int));
    ctx->add(0x06000000, r);
}
