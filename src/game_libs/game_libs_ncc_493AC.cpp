#include "../../include/uso_call.h"  /* NCC donors build without -I include */
/* IDO 7.1 NCC (C++) EXACT donor for PolyShape_init (0x790 / 484 words), REPLACE_FUNC_BODY-spliced over the
 * placeholder in game_libs_post0b.c on both build paths (agent-g run 312, kit CDIV: virtual int hit(), cur2() on
 * one p34-call argument, me(me()) on the k-loop's closing noop, the AIter ctor's dead block dropped and three
 * zero-code me0() levels inside Arr::iter()).
 * agent-c run 159: rewritten as real C++ (was the run-152 C lift under NCC, 92.20). `int Mgr::run()`
 * (renamed to PolyShape_init with objcopy): BB declares the virtuals (vptr at 0x28, slot 12 = the per-hit
 * callback, edg {d, f} entries at 0x60 / 0x64), two `AIter` loops over `arr` (the 4B620 iterator: by-value
 * iter() through a return temp into `it`, the second one an assignment to the same `it`), a `Rec` with its
 * vptr AFTER 0x598 data bytes (`Rec rec;` declared where the target stores the vtable), Ctx::count() /
 * noop() display-list appends on o->triDL, and `this->hit(0, &rec, j, o)` virtual calls.
 * Home layout (all sp offsets = target): b8 = b4 = b0 = 0 chained (two pooled temps before the
 * iterator return temp), the geomFlags test as an inline `Mat::test()` (its `this` is the edg pool temp: the
 * target's s2), the unk34 argument through an inline `p34()`, next() as `if (idx < last) return ...; return 0;`
 * (the result in v0 like the target), DLHead appends without a named count (`&head->buf[head->count++]`), `hit` declared after `rowStart`, c / e / j / m in their blocks,
 * `o->rowGfx[k]` in a for loop (uopt's strength-reduced offset lands at sp+0x48 like the target).
 * AIter(Arr *) kept its parameter with `if (0) { arr = 0; }` (96.52 -> 96.63; dropped in agent-g run 312, see iter()).
 * Seven `pad` words at the top are still unexplained named homes (target: 10 words above `it`; o, j, i are the other three).
 * agent-c run 179: `hit = 1` after the hit() call (adjacent-statement swap sweep): 96.63 -> 96.90.
 * agent-c run 206 (agent-g kit CLXXIX `+ 0` lever): the second loop's test as `(it.next() + 0)` -- no code, but edg
 * shares its temporaries differently (local 96.56 -> 96.81, length unchanged). The first loop's test does not move.
 */
typedef short s16; typedef unsigned short u16; typedef int s32; typedef unsigned int u32; typedef unsigned char u8;
extern "C" {
extern char D_00000000[];
extern int gGfxLib_493AC_a, gGfxLib_493AC_b, gGfxLib_493AC_c, gGfxLib_493AC_d;
void *gl_func_00000000(...);
}
#define S(o) (D_00000000 + (o))
#define G1C4(S_) (*(s32 *)((char *)&S_ + 0x1C4))
inline void *operator new(unsigned n) { return CALL(mem_new)(n); }
template <class T> inline T *me0(T *p) { return p; } /* agent-g run 312: zero-code accessor declared before Arr::iter() */
struct Gfx { u32 w0, w1; };
inline void gnoop(Gfx *g) { g->w0 = 0xC0000000; g->w1 = 0; }
struct DLHead { Gfx *buf; int count; };
struct Ctx {
    char pad[0xC];
    DLHead *head;
    Gfx *next() { return &head->buf[head->count++]; }
    int count() { return head->count; }
    void noop() { Gfx *g = next(); g->w0 = 0xC0000000; g->w1 = 0; }
};
struct Rec {
    Ctx *ctx;
    char pad[0x588];
    int n58C;
    int f590;
    int f594;
    Rec() {}
    virtual void f();
};
/* Mat (include/gdl/material.h; was `Elem` here, agent-b run 553: same offsets, names only) */
struct Mat {
    int test(int m) { return geomFlags & m; }
    int *p34() { return &unk34; }
    char pad0[0x14]; int unk14; int id; char pad1c[0x34 - 0x1C]; int unk34; int geomFlags; char pad3c[0x50 - 0x3C];
    int minCut; int maxCut; char pad58[0x6C - 0x58]; Ctx *triDL; char pad70[0x7C - 0x70]; s16 *rowGfx; s16 rowStart; char pad82[8]; s16 triStart;
};
struct AIter;
struct Arr { Mat **data; int pad4, pad8; int count; Mat *&operator[](int i) { return data[i]; } AIter iter(); };
struct IBase {
    Arr *a;
    int idx;
    int last;
    IBase(Arr *arr) { a = arr; idx = -1; last = arr->count - 1; }
};
struct AIter : IBase {
    /* agent-g run 312: the run-1xx `if (0) { arr = 0; }` dropped -- the kept parameter put BOTH iter() constructions'
     * IBase `this` into the -1536 pool var, diluting its uopt priority below the -1532 var's; the first loop's
     * iter() now gets that slot through me(me(&arr)) instead (see run()) */
    AIter(Arr *arr) : IBase(arr) {}
    Mat **next() { if (idx < last) return &(*a)[++idx]; return 0; }
    /* agent-c run 299: the first loop's own copy of next() (same body): its result leaves the s2 pool temp (target v0) */
    Mat **next1() { if (idx < last) return &(*a)[++idx]; return 0; }
    Mat *cur() { return (*a)[idx]; }
    /* agent-g run 312: cur() with a named copy of `a` (a code-free inline local = one more edg pool entry), used for
     * the first p34-call argument: the second argument's p34 `this` then shares the operator[] index slot (the
     * target's a2, ours was the `a` slot's v1): 99.917 -> 99.938, same length */
    Mat *cur2() { Arr *x = a; return (*x)[idx]; }
    Mat *&ref() { return (*a)[idx]; }
};
/* agent-g run 312 (EXACT): the IBase-this pool slot is picked by accessor depth INSIDE iter() -- three zero-code
 * me0() levels on the ctor argument put both constructions' IBase `this` copy in the target's v1 var without any
 * call-site pool words ahead of the by-value return temps (the call-site me(me(me(me(&arr)))) form left them 8 low) */
inline AIter Arr::iter() { return AIter(me0(me0(me0(this)))); }

template <class T> inline T *me(T *p) { return p; }
struct Grid { int w; int n; char pad8[0x18 - 0x8]; s16 *tbl18; int *row1C; };
struct Ent { u16 key; char pad[6]; };
struct BB {
    char pad0[0xC]; int m0c; char pad10[0x28 - 0x10];
    virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6();
    virtual void v7(); virtual void v8(); virtual void v9(); virtual void v10(); virtual void v11();
    /* agent-g run 312: int return (result never read here): the target colours the grid loop as if v0 were
     * clobbered by a value-returning call inside the j loop (tbl18 v1, grid a0, end v0); with a void hit()
     * uopt keeps v0 for the tbl18 load and rotates grid / w / end (99.680 -> 99.917, same length) */
    virtual int hit(int, Rec *, int, Mat *);
};
struct Mgr : BB {
    int m2c; int flags30; char pad34[0x44 - 0x34]; u32 n44; char pad48[4]; s16 *remap4C; int m50; char pad54[0x68 - 0x54];
    Ent *tab68; char pad6c[0x84 - 0x6C]; Arr arr; char pad94[0x9C - 0x94]; int m9c; Grid *grid; char pada4[0xB0 - 0xA4];
    int b0, b4, b8;
    int run();
};
int Mgr::run() {
    Mat *o;
    int j;
    int pad6;
    int pad7;
    int pad8;
    u32 i;
    int pad1;
    int pad2;
    int pad3;
    int pad4;
    pad1 = 0; pad2 = 0; pad3 = 0; pad4 = 0; pad6 = pad7 = pad8 = 0; if (pad1 + pad2 + pad3 + pad4 + pad6 + pad7 + pad8) {}
    if (!(flags30 & 1)) {
        return 1;
    }
    CALL(sys_imin)(m0c);
    CALL(sys_printStub_00067AE8)(S(0x1FF54), m0c);
    b8 = b4 = b0 = 0;
    CALL(sys_imin)(S(0x1FF58));
    CALL(memtrack_begin)(S(0x1FF60), 0);
    CALL(sys_imout)();
    CALL(sys_printStub_00067AE8)(S(0x1FF6C));
    CALL(sys_imin)(S(0x1FF74));
    AIter it = arr.iter();
    while (it.next1()) { /* agent-c run 247: the folded + 0 moved from the second loop's test to this one (99.36 -> 99.43: the second loop's next() result leaves the call-crossing s2 pool temp, target v0). agent-c run 299: next1() instead (both loop results in v0); the s1/s2 and s0/s1 colour order then fixed by the folded e / m uses in the grid loop (99.43 -> 99.52) */
        { Mat *e1 = it.cur(); CALL(Mat_reset_00043584)(e1); } /* agent-c run 299: block-scoped named copy (its edg home replaces the old `Ctx *g` home-keeping local below): 99.62 -> 99.65 */
        if (m50 == 0) {
            if (it.cur()) {} /* agent-c run 302: code-free test of the current element (random test sweep): 99.6488 -> 99.6798 */
            if (it.cur()->test(0x2000)) {
                CALL(PolyShape_allocLightDL_000435FC)(it.cur(), m9c);
            }
        }
        CALL(PolyShape_buildStateDL_00043654)(it.cur2(), it.cur()->p34());
    }
    CALL(sys_imout)();
    CALL(sys_printStub_00067AE8)(S(0x1FF7C));
    CALL(sys_imin)(S(0x1FF88));
    int total = 0;
    u8 *buf;
    if (G1C4(gGfxLib_493AC_a) & 8) {
        pad1 = (int)this; if (1) { pad1 += 0x30; } *(int *)pad1 |= 0x80; /* agent-g run 221: pointer mutation behind if (1) = the target's addiu v0,s6,48 + lw/sw 0(v0) */
        buf = (u8 *)CALL(heap_tempAlloc)(D_00000000, n44);
        for (pad2 = 0; pad2 < n44; pad2++) { /* agent-c run 302: the clear loop counts in the (zeroed, otherwise dead) pad2 word, not a block-scoped u32 i2: i2's edg temp word sat between the two iter() return temps (second one at sp+132, target sp+136); with it gone and the `Ctx *g = 0;` home-keeping local back in the second loop every sp offset is the target's (pad2 < n44 compares unsigned like i2 did): 99.6467 -> 99.6488, same length */
            buf[pad2] = 0;
        }
    }
    Rec rec;
    it = arr.iter();
    while (it.next()) { /* run 206 had a folded + 0 here (kit CLXXIX); run 247 moved it to the first loop */
        Ctx *g = 0;
        o = it.cur();
        /* agent-g run 233 (kit CCXLIV): the context pointer is a ugen temp (t6) in the target, not a coloured
         * local: stored to o->triDL and re-read through the member (uopt forwards the store), rec.ctx taken before
         * the rec stores that would kill the forwarding; `g` kept as a home-keeping local (frame offsets) --
         * agent-c run 299: g dropped, the first loop's block-scoped e1 now holds that home word */
        o->triDL = *(Ctx **)((char *)&gGfxLib_493AC_d + 0x214);
        o->triStart = o->triDL->count();
        rec.ctx = o->triDL;
        rec.n58C = 0;
        rec.f590 = 32; rec.f594 = 3; /* agent-g run 233: one line (32 materialised first, stores 0x674 then 0x670 as the target) */
        CALL(gfx_vtxBatchBegin_000483A0)(&rec);
        int cnt = 0; /* agent-g run 233: after the call (the target's `or s5,zero,zero` follows the &rec argument) */
        if (G1C4(gGfxLib_493AC_b) & 8) {
            int k = 0;
            if (grid == 0) {
                CALL(sys_assertFail)(S(0x1FF90), S(0x1FFA0), 3287);
            }
            o->rowStart = o->triDL->count();
            me(me(o->triDL))->noop(); /* agent-c run 299: zero-code accessor depth 2 on this receiver (random depth search, kit CCCLXXI lever): 99.56 -> 99.62 */
            o->minCut = 0x80;
            o->maxCut = 0; if (o) {} /* agent-c run 299: code-free tests of o here and of k after the row block (random test sweep): 99.52 -> 99.56 */
            for (k = 0; k < grid->n; k++) {
                {
                    int rowStart = grid->w * k;
                    int hit = 0;
                    for (int c = rowStart; c < rowStart + grid->w; c++) {
                        int e = grid->row1C[c];
                        if (e != -1) {
                            int j;
                            while ((j = grid->tbl18[e++]) != -1) {
                                u8 *m = buf + j + 0 * e; /* agent-c run 299: folded counted use: e outranks the pool temp (target s1) */
                                if (tab68[j].key == o->id && *m == 0) { /* agent-g run 233: table key first (target load order) */
                                    o->unk14 = 1;
                                    cnt++;
                                    this->hit(0, &rec, j, o);
                                    hit = 1;
                                    *m = 1 + 0 * (int)m; /* agent-c run 299: folded use: m stays above e (target s0) */
                                }
                            }
                        }
                    }
                    if (hit) {
                        if (k < o->minCut) o->minCut = k;
                        if (o->maxCut < k) o->maxCut = k;
                    }
                    CALL(PolyShape_emitTriBatch_000483BC)(&rec); if (1) {}
                    o->rowGfx[k] = o->triDL->count();
                    me(me(o->triDL))->noop(); /* agent-g run 312: accessor depth 2 = the head / count temps in the -1536 / -1532 pool vars like the rowStart append (target v1 / a2); with the iter() change above their uopt order is the target's */
                } if (k) {}
            }
        } else {
            o->minCut = -1;
            for (i = 0; i < n44; i++) {
                if (remap4C != 0) {
                    j = remap4C[i];
                } else {
                    j = i;
                }
                if (tab68[j].key == o->id) {
                    o->unk14 = 1;
                    cnt++;
                    this->hit(0, &rec, j, o);
                }
            }
        }
        CALL(PolyShape_emitTriBatch_000483BC)(&rec);
        total += rec.n58C;
        CALL(gfx_endDL)(o->triDL);
        if (cnt == 0) {
            o->triDL = 0;
        }
    }
    CALL(sys_printStub_00067AE8)(S(0x1FFAC), m0c, total);
    if (G1C4(gGfxLib_493AC_c) & 8) {
        CALL(heap_tempFree)(D_00000000, buf);
    }
    CALL(sys_imout)();
    CALL(sys_printStub_00067AE8)(S(0x1FFB4));
    CALL(memtrack_end)();
    CALL(sys_imout)();
    CALL(sys_printStub_00067AE8)(S(0x1FFC4));
    return 1;
}
