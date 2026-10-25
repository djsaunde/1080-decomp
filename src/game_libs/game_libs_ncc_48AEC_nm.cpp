/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00048AEC (0x81C / 519 words), agent-c run 159.
 * `void Obj::draw()` (renamed to gl_func_00048AEC with objcopy): the matrix concat into a local f32[4][4],
 * Ctx::mtx() / next() appends on the GState context (next() with a named count `n`: the target's dl
 * pointer in a caller-saved register, re-read for the buf; mtx() as `&mh->buf[mh->count++]`, frame 0x160 = target), load(m) / prim(hi, lo) / dlcall(p) DL helpers
 * (named `g` / `t` per expansion, like the target's per-arm disp homes), three `AIter it = arr.iter();` loops, one per arm (block-scoped `it`, the
 * return temp pooled across arms like the target's shared sp+196 copy temp), Elem's vptr after 0x5C data
 * bytes (`it.cur()->draw(this)` = the slot-5 call through +0x5C), the arm-2 Gfx copy of g34.
 * Declaration order gctx, A, B, mtx, i, j, k, mt, cmd puts gctx / mtx / cmd at the target's homes.
 * Elem's virtuals sit in a 0x5C-byte base (EBase) so the derived members keep their offsets.
 * The arm-1 setup call is an inline `Ctx::setup(Gfx *, int)` (its `this` = the GState context pool temp).
 * The two matrix arms call one inline `Ctx::push(src, cmd)` (mtx() + the copy call + load()) on the named gctx:
 * gctx is then stored to its home before the copy call and reloaded after it, word for word the target (kit CVII:
 * a pointer stored once and reloaded around a call is a temp shared by an inline across that call).
 * load() and prim() append through `&dl->buf[dl->count++]` (no named count; dlcall keeps next()'s named n).
 * The arm-2 Gfx copy is masked through `Gfx *gp = &gbi` (the target's &gbi in s0).
 * push() assigns the matrix slot inside the copy call's argument (`func(src, c = mtx())`), 94.92 -> 95.21.
 * The else arm writes `(gctx = GS->ctx)->push(A, cmd);` (assignment as the object expression), 95.21 -> 95.67.
 * cfe body 88.11 -> 95.67; length 2052 -> 2072 (target 2076).
 * agent-c run 160: the else arm is `Mtx *c2; gctx = GS->ctx; func(A, c2 = gctx->mtx()); gctx->load(c2);` (a named
 * by-value c2 instead of the shared `Mtx *&` cmd: the target holds it in s0 across the copy call), 95.67 -> 96.13, frame
 * 0x158 -> 0x160 = target; the Node flag test as an inline `Flags::test(int m)` with an `if (0) { m = 0; }` parameter keep
 * (the target's `addiu a3,a3,0x18` this temp), 96.13 -> 96.57, length 2072 -> 2076 = target.
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
    Gfx *next() { int n = dl->count++; return &dl->buf[n]; } /* run 173: n-only post-increment (kit CXVII) */
    void load(Mtx *m) { Gfx *g = &dl->buf[dl->count++]; g->w0 = 0x01020040; g->w1 = (u32)func_00000000(m); }
    void prim(u32 hi, u32 lo) { Gfx *g = &dl->buf[dl->count++]; g->w0 = hi; g->w1 = lo; }
    void dlcall(void *p) { u32 t = (u32)func_00000000(p); Gfx *g = next(); g->w0 = 0x06000000; g->w1 = t; }
    void setup(Gfx *g, int z) { func_00000000(this, g, z); }
    void push(void *src, Mtx *&c) { func_00000000(src, c = mtx()); load(c); } void push2(void *src, Mtx *c) { func_00000000(src, c); load(c); } void push5(void *src, Mtx *&c) { func_00000000(src, c); load(c); }
    Mtx *mtx() { return &mh->buf[mh->count++]; }
};
struct GState { char pad[0x158]; Ctx *ctx; };
#define GS (*(GState **)(D_00000000 + 0x254))
struct Flags { int v; int test(int m) { if (0) { m = 0; } return v & m; } };
struct Node { char pad[0x18]; Flags flags; char pad1c[2]; s16 m1e; };
struct EBase {
    char pad0[0x5C];
    virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void draw(void *);
};
struct Elem : EBase {
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
        gctx->push(mt, cmd);
    } else {
        Mtx *c2; gctx = GS->ctx; func_00000000(A, c2 = gctx->mtx()); gctx->load(c2);
    }
    func_00000000(GS->ctx);
    if (*(s32 *)((char *)&D_00000000_g + 0x1C4) & 2) {
        if ((flags30 & 8) && n14 != 0) {
            GS->ctx->setup(&g34, n14->m1e);
            AIter it = arr.iter();
            while (it.next()) {
                if (it.cur()->ctx6C != 0) {
                    GS->ctx->dlcall(&it.cur()->ctx6C->dl->buf[it.ref()->n8A]);
                }
            }
        }
    } else if (n14->flags.test(0x80) && *(s32 *)D_00000000 != 0) {
        Gfx gbi = g34;
        Gfx *gp = &gbi;
        gp->w1 = (gp->w1 & ~0xF00) | 0x400;
        gp->w1 = gp->w1 & ~0x80000;
        func_00000000(GS->ctx, &gbi);
        func_00000000(GS->ctx, &gbi);
        func_00000000(GS->ctx, &gbi);
        func_00000000(GS->ctx, &gbi);
        GS->ctx->prim(0xFA000000, 0xFFFF00FF);
        AIter it = arr.iter();
        while (it.next()) {
            if (it.cur()->ctx6C != 0) {
                GS->ctx->dlcall(&it.cur()->ctx6C->dl->buf[it.ref()->n8A]);
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
