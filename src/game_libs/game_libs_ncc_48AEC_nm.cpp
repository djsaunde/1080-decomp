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
template <class T> inline T *me(T *p) { return p; } /* agent-c run 252: zero-code accessor (kit CCLXXXIV) */
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
    void prim(u32 hi, u32 lo) { Gfx *g = nextQ(); g->w0 = hi; g->w1 = lo; } /* agent-g run 245: w1 first (member permutation sweep, 97.235 -> 97.260); agent-g run 294: slot through next() */
    void dlcall(void *p) { DLHead *dd = dl; u32 t = (u32)func_00000000(p) + 0 * (int)(dl); /* run 209: folded counted use */ Gfx *g = next(); g->w0 = 0x06000000; g->w1 = t; }
    void setup(Gfx *g, int z) { func_00000000(this, g, z); }
    void push(void *src, Mtx *&c) { func_00000000(src, c = mtx()); load(c); } void push2(void *src, Mtx *c) { func_00000000(src, c); load(c); } void push5(void *src, Mtx *&c) { func_00000000(src, c); load(c); }
    Mtx *mtx() { if (mh->count) {} return &mh->buf[mh->count++]; } /* agent-g run 407: code-free count test, with arm 1's GS->ctx test 98.397 -> 98.426 */ /* agent-g run 266: named post-increment count (kit CXVII form): gctx a3 / mh v1 like the target in both arms, 97.29 -> 97.45 (frame 0x160 -> 0x168: n's edg home per expansion) */
    /* agent-g run 293: code-free dead loads of the head pointer first (kit CCCLXIII lever) in the push() matrix
     * allocation and the first loop's dlcall() packet, push() through a depth-2 accessor: 97.532 -> 97.669 */
    /* agent-g run 417: the loop bodies' 0x06 packet as an inline taking the element's ctx and n8A as parameters
     * (`GS->ctx->dcQ(it.cur()->ctx6C, it.cur()->n8A)`): the target's ctx6C / n8A are uopt variables (v0 / v1), not
     * ugen temps. With the plain loop tests and the plain Flags::test, 98.484 -> 99.071 (dcQ reusing n for the call
     * result). Then: arm 2's gbi masked in place (no gp pointer: one word fewer, so dcQ keeps its own t and loop 2's
     * dc3 its unused dd word, homes = target), 99.071 -> 99.100; a folded `n + 0 * n` in dcQ and a code-free
     * `if (c) {}` in dc3 raise c / n (pooled across both loops) over the cur() temps: c v0 / n v1 like the target,
     * 99.100 -> 99.119; n's element through `me(&it)->cur()` (its own pool word, not next()'s): 99.119 -> 99.283 */
    void dcQ(Ctx *c, int n) { u32 t = (u32)func_00000000(&c->dl->buf[n + 0 * n]) + 0 * (int)(dl); Gfx *g = nextQ(); g->w0 = 0x06000000; g->w1 = t; }
    void dc3(Ctx *c, int n) { if (c) {} DLHead *dd = dl; u32 t = (u32)func_00000000(&c->dl->buf[n]) + 0 * (int)(dl); Gfx *g = next(); g->w0 = 0x06000000; g->w1 = t; }
    Gfx *nextQ() { return &dl->buf[dl->count++]; } /* agent-g run 294: n-free slot (with prim() via next(): 97.697 -> 97.715) */
    void dlcallQ(void *p) { u32 t = (u32)func_00000000(p) + 0 * (int)(dl); Gfx *g = nextQ(); g->w0 = 0x06000000; g->w1 = t; }
    /* agent-g run 354: the run-352 frame-0x160 body (h5_17: arm-1 copy through cp() / ld() with a kept parameter, else-arm
     * ld2(), no i / j / k / cmd at function scope) with the committed gains ported, plus loop 1's ctx6C through cur():
     * 98.033 (frame 0x170) -> 98.037 (frame 0x160 = target) */
    void ld(Mtx *m) { if (0) { m = 0; } Gfx *g = &dl->buf[dl->count++]; g->w0 = 0x01020040; g->w1 = (u32)func_00000000(m); }
    void ld2(int z0, Mtx *m) { if (1) {} if (*(u32 *)&dl) {} Gfx *g = dl->buf + ((DLHead *)*(u32 *)&dl)->count++; g->w0 = 0x01020040; g->w1 = (u32)func_00000000(m); } /* agent-g run 407: code-free test of dl (98.329 -> 98.368) */
    /* agent-g run 413: the test and the count's head read through a u32 view of dl (kit CDLXXXIX): the else-arm
     * dl is no longer the same uopt expression as arm 1's, so arm 1's dl LR takes v0 like the target (98.455 -> 98.484) */
    void cp(void *src, Mtx *c) { func_00000000(src, c); ld(c); }
    /* agent-g run 407: arm 1 as cS() (the copy call) + lE3() (the 0x01 packet). The count is a by-value parameter
     * behind three unused non-constant leading parameters (kit CDXC), so it lands on the else arm's c2 word: one
     * variable that the target keeps in s0 (mt / arm-1 count / else c2), with c spilled to its home like the target */
    Gfx *nq3(int z0, int z1, int z2, int n) { dl->count = n + 1; return &dl->buf[n]; }
    void cS(void *src, Mtx *c) { func_00000000((char *)src, c); }
    void lE3(Mtx *m) { Gfx *g = nq3((int)m, (int)m, (int)m, dl->count); g->w0 = 0x01020040; g->w1 = (u32)func_00000000(m); }
};
/* agent-g run 352: kept-parameter wrapper (kit CDXXIX I2 form) on the loops' element reads: the two ctx6C tests and the
 * third loop's draw() receiver, 97.821 -> 97.869 */
template <class T> inline T I2(T v) { if (0) { v = 0; } return v; }
struct GState { char pad[0x158]; Ctx *ctx; };
#define GS (*(GState **)(D_00000000 + 0x254))
struct Flags { int v; int test(int m) { return v & m; } }; /* agent-g run 417: plain (the run-209 folded use of v / kept m made v a uopt LR in v0; the target reads it into a ugen temp, t4) */
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
    Elem *curM() { return (*me(this)->a)[idx]; } /* agent-g run 369: per-site accessor with an internal zero-code me(this) (kit CDLII 6.) */
    Elem *&ref() { return (*a)[idx]; }
};
inline AIter Arr::iter() { return AIter(this); }
struct Obj {
    char pad0[0x14]; Node *n14; char pad18[0x30 - 0x18]; int flags30; Gfx g34; char pad3c[0x84 - 0x3C];
    Arr arr; char pad94[0xA8 - 0x94]; int a8; int ac;
    void draw();
};
void Obj::draw() {
    /* agent-g run 294: -g3 homes vs the target's offsets put gctx at the frame top (vfp-4) with A / B below it; the
     * 120-order grid with gctx first: mtx declared last, 97.715 -> 97.719 (frame still 0x170, target 0x160).
     * agent-c run 307 (full 720-order grid, 150 random): i / j / k and cmd ahead of gctx (mtx still last): cmd's home
     * at sp+348 / gctx sp+352 under the frame top, 97.719 -> 97.726, same length */
    Ctx *gctx;
    f32 (*A)[4];
    f32 (*B)[4];
    f32 mtx[4][4];

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
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                mtx[i][j] = 0.0f;
                for (int k = 0; k < 4; k++) {
                    mtx[i][j] += A[i][k] * B[k][j];
                }
            }
        }
#define mt ((Mtx *)((char *)&D_00000000_i + 0x3C8A0)) /* agent-c run 180: de-named (lever-sweep) */
        func_00000000(mtx, mt); /* agent-g run 293: code-free test (random insertion search on the mtxQ body): 97.669 -> 97.697 */
        gctx = GS->ctx + 0 * ((int)(mt) + (int)(mt)); /* agent-c run 288: double folded use of the mt address (zero-use --exprs sweep), 97.51 -> 97.53 */
        { Mtx *c; if (GS->ctx) {} c = me(gctx)->mtx(); gctx->cS(mt, c); if (c) {} gctx->lE3(c); } /* agent-g run 407: 98.297 -> 98.329 */
#undef mt
    } else {
        Mtx *c2; gctx = GS->ctx + 0 * (int)B; func_00000000(A, c2 = me(gctx)->mtx()); me(me(gctx))->ld2((int)c2, c2); /* agent-g run 407: m one pool word later (unused leading parameter, kit CDXC), 98.368 -> 98.397 */ /* agent-c run 252: accessor sweep 97.26 -> 97.29; agent-c run 288: folded use of B (set on every path above; zero-use --exprs sweep), 97.48 -> 97.51 */
    }
    func_00000000(GS->ctx);
    if (*(s32 *)((char *)&D_00000000_g + 0x1C4) & 2) {
        if ((flags30 & 8) && n14 != 0) {
            GS->ctx->setup(&g34, n14->m1e); if (1) {}
            AIter it = arr.iter();
            while (it.next()) {
                /* agent-g run 369: the ctx6C test through curM() and n8A back through cur(), and the second loop's test
                 * through curM() (per-site accessor rounds, 200 + 200): 98.037 -> 98.085 -> 98.297, same length */
                if (me(&it)->cur()->ctx6C != 0) { /* agent-g run 417: plain test (kit CDXCVIII 4.); loop 1's test and loop 2's ctx element through me(&it)->cur() (random accessor round), 99.312 -> 99.389 */
                    GS->ctx->dcQ(it.cur()->ctx6C, it.cur()->n8A); /* agent-g run 352: ctx6C read through ref() (loop-lever random round), 97.734 -> 97.821; agent-g run 354: back through cur() on the frame-0x160 body (per-read wrapper grid), 98.017 -> 98.037 */
                }
            }
        }
    } else if (n14->flags.test(0x80) && *(s32 *)D_00000000 != 0) {
        Gfx gbi = g34;
        gbi.w1 = (gbi.w1 & ~0xF00) | 0x400; if (1) {} /* agent-c run 180 (lever-sweep bbsplit) */
        gbi.w1 = gbi.w1 & ~0x80000;
        func_00000000(GS->ctx, &gbi);
        func_00000000(GS->ctx, &gbi);
        func_00000000(GS->ctx, &gbi);
        func_00000000(GS->ctx, &gbi); if (this) {} /* agent-c run 276: code-free test after the fourth copy call (sweep), 97.447 -> 97.476 */
        me(GS->ctx)->prim(0xFA000000, 0xFFFF00FF);
        AIter it = arr.iter();
        while (it.next()) {
            if (it.cur()->ctx6C != 0) {
                GS->ctx->dc3(me(&it)->cur()->ctx6C, it.cur()->n8A); /* agent-g run 352: n8A through cur() (loop-lever round on the I2 body), 97.869 -> 98.033 */
            }
        }
    } else {
        if (a8 != 0) {
            func_00000000(ac, a8, g34.w1 & 0x20000000, g34.w1);
        }
        AIter it = arr.iter();
        while (it.next()) {
            it.cur()->draw(this); if (1) {} /* agent-c run 181: lever-sweep bbsplit hit */
        }
    }
}
