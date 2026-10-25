/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00048AEC (0x81C / 519 words), agent-c run 159.
 * `void Obj::draw()` (renamed to gl_func_00048AEC with objcopy): the matrix concat into a local f32[4][4],
 * Ctx::mtx() / next() appends on the GState context (mtx() with a named count: the target's v0 / v1, next()
 * as `&dl->buf[dl->count++]`), three `AIter it = arr.iter();` loops, one per arm (block-scoped `it`, the
 * return temp pooled across arms like the target's shared sp+196 copy temp), Elem's vptr after 0x5C data
 * bytes (`it.cur()->draw(this)` = the slot-5 call through +0x5C), the arm-2 Gfx copy of g34.
 * Declaration order gctx, A, B, mtx, i, j, k, mt, cmd puts gctx / mtx / cmd at the target's homes.
 * cfe body 88.11 -> 91.75; length 2052 -> 2096 (target 2076: overshoots, distance 24 -> 20).
 * Still NON_MATCHING: build/non_matching/ only. */
typedef short s16; typedef unsigned short u16; typedef int s32; typedef unsigned int u32; typedef unsigned char u8; typedef float f32;
extern "C" {
extern char D_00000000[];
extern int D_00000000_g, D_00000000_h, D_00000000_i, D_00000000_j;
void *func_00000000(...);
}
struct Gfx { u32 w0, w1; };
struct DLHead { Gfx *buf; int count; };
struct Mtx { int m[16]; };
struct MtxHead { Mtx *buf; int count; };
struct Ctx {
    char pad[0xC];
    DLHead *dl;
    char pad10[0x3C - 0x10];
    MtxHead *mh;
    Gfx *next() { return &dl->buf[dl->count++]; }
    Mtx *mtx() { int n = mh->count; mh->count = n + 1; return &mh->buf[n]; }
};
struct GState { char pad[0x158]; Ctx *ctx; };
#define GS (*(GState **)(D_00000000 + 0x254))
struct Node { char pad[0x18]; int flags; char pad1c[2]; s16 m1e; };
struct Elem {
    char pad0[0x5C];
    virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void draw(void *);
    char pad60[0x6C - 0x60]; Ctx *ctx6C; char pad70[0x8A - 0x70]; s16 n8A;
};
struct AIter;
struct Arr { Elem **data; int pad4, pad8; int count; Elem *&operator[](int i) { return data[i]; } AIter iter(); };
struct IBase {
    Arr *a;
    int idx;
    int last;
    IBase(Arr *arr) { a = arr; idx = -1; last = arr->count - 1; }
};
struct AIter : IBase {
    AIter(Arr *arr) : IBase(arr) {}
    Elem **next() { if (idx < last) return &(*a)[++idx]; return 0; }
    Elem *cur() { return (*a)[idx]; }
    Elem *&ref() { return (*a)[idx]; }
};
inline AIter Arr::iter() { return AIter(this); }
struct Obj {
    char pad0[0x14]; Node *n14; char pad18[0x30 - 0x18]; int flags30; Gfx g34; char pad3c[0x84 - 0x3C];
    Arr arr; char pad94[0xA8 - 0x94]; int a8; int ac;
    void draw();
};
void Obj::draw() {
    Ctx *gctx;
    f32 (*A)[4];
    f32 (*B)[4];
    f32 mtx[4][4];
    int i, j, k;
    Mtx *mt;
    Mtx *cmd;
    Gfx *g;

    *(s32 *)((char *)&D_00000000_h + 0x3C7C4) = 0;
    if (flags30 & 0x80) {
        func_00000000(GS, this);
    }
    if (*(s32 *)((char *)&D_00000000_g + 0x1C4) & 1) {
        return;
    }
    B = *(f32 (**)[4])(D_00000000 + 0xE4);
    A = (f32 (*)[4])((char *)&D_00000000_j + 0xA0);
    if (B != 0) {
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                mtx[i][j] = 0.0f;
                for (k = 0; k < 4; k++) {
                    mtx[i][j] += A[i][k] * B[k][j];
                }
            }
        }
        mt = (Mtx *)((char *)&D_00000000_i + 0x3C8A0);
        func_00000000(mtx, mt);
        gctx = GS->ctx;
        cmd = gctx->mtx();
        func_00000000(mt, cmd);
        g = gctx->next();
        g->w0 = 0x01020040;
        g->w1 = (u32)func_00000000(cmd);
    } else {
        gctx = GS->ctx;
        cmd = gctx->mtx();
        func_00000000(A, cmd);
        g = gctx->next();
        g->w0 = 0x01020040;
        g->w1 = (u32)func_00000000(cmd);
    }
    func_00000000(GS->ctx);
    if (*(s32 *)((char *)&D_00000000_g + 0x1C4) & 2) {
        if ((flags30 & 8) && n14 != 0) {
            func_00000000(GS->ctx, &g34, n14->m1e);
            AIter it = arr.iter();
            while (it.next()) {
                if (it.cur()->ctx6C != 0) {
                    Elem *e = it.cur();
                    Ctx *c = GS->ctx;
                    u32 t = (u32)func_00000000(&e->ctx6C->dl->buf[it.cur()->n8A]);
                    Gfx *d = c->next();
                    d->w0 = 0x06000000;
                    d->w1 = t;
                }
            }
        }
    } else if ((n14->flags & 0x80) && *(s32 *)D_00000000 != 0) {
        Gfx gbi = g34;
        gbi.w1 = (gbi.w1 & ~0xF00) | 0x400;
        gbi.w1 = gbi.w1 & ~0x80000;
        func_00000000(GS->ctx, &gbi);
        func_00000000(GS->ctx, &gbi);
        func_00000000(GS->ctx, &gbi);
        func_00000000(GS->ctx, &gbi);
        g = GS->ctx->next();
        g->w0 = 0xFA000000;
        g->w1 = 0xFFFF00FF;
        AIter it = arr.iter();
        while (it.next()) {
            if (it.cur()->ctx6C != 0) {
                Elem *e = it.cur();
                Ctx *c = GS->ctx;
                u32 t = (u32)func_00000000(&e->ctx6C->dl->buf[it.cur()->n8A]);
                Gfx *d = c->next();
                d->w0 = 0x06000000;
                d->w1 = t;
            }
        }
    } else {
        if (a8 != 0) {
            func_00000000(ac, a8, g34.w1 & 0x20000000, g34.w1);
        }
        AIter it = arr.iter();
        while (it.next()) {
            it.cur()->draw(this);
        }
    }
}
