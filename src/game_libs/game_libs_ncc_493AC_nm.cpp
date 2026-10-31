/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_000493AC (0x790 / 484 words).
 * agent-c run 159: rewritten as real C++ (was the run-152 C lift under NCC, 92.20). `int Mgr::run()`
 * (renamed to gl_func_000493AC with objcopy): BB declares the virtuals (vptr at 0x28, slot 12 = the per-hit
 * callback, edg {d, f} entries at 0x60 / 0x64), two `AIter` loops over `arr` (the 4B620 iterator: by-value
 * iter() through a return temp into `it`, the second one an assignment to the same `it`), a `Rec` with its
 * vptr AFTER 0x598 data bytes (`Rec rec;` declared where the target stores the vtable), Ctx::count() /
 * noop() display-list appends on o->ctx6C, and `this->hit(0, &rec, j, o)` virtual calls.
 * Home layout (all sp offsets = target): b8 = b4 = b0 = 0 chained (two pooled temps before the
 * iterator return temp), the m38 test as an inline `Elem::test()` (its `this` is the edg pool temp: the
 * target's s2), the m34 argument through an inline `p34()`, next() as `if (idx < last) return ...; return 0;`
 * (the result in v0 like the target), DLHead appends without a named count (`&head->buf[head->count++]`), `hit` declared after `rowStart`, c / e / j / m in their blocks,
 * `o->rows7C[k]` in a for loop (uopt's strength-reduced offset lands at sp+0x48 like the target).
 * AIter(Arr *) keeps its parameter with `if (0) { arr = 0; }` (the IBase this temp then takes v1 like the target, 96.52 -> 96.63).
 * Seven `pad` words at the top are still unexplained named homes (target: 10 words above `it`; o, j, i are the other three).
 * Still NON_MATCHING: build/non_matching/ only.
 * agent-c run 179: `hit = 1` after the hit() call (adjacent-statement swap sweep): 96.63 -> 96.90.
 * agent-c run 206 (agent-g kit CLXXIX `+ 0` lever): the second loop's test as `(it.next() + 0)` -- no code, but edg
 * shares its temporaries differently (local 96.56 -> 96.81, length unchanged). The first loop's test does not move.
 */
typedef short s16; typedef unsigned short u16; typedef int s32; typedef unsigned int u32; typedef unsigned char u8;
extern "C" {
extern char D_00000000[];
extern int D_493AC_a, D_493AC_b, D_493AC_c, D_493AC_d;
void *gl_func_00000000(...);
}
#define S(o) (D_00000000 + (o))
#define G1C4(S_) (*(s32 *)((char *)&S_ + 0x1C4))
inline void *operator new(unsigned n) { return gl_func_00000000(n); }
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
struct Elem {
    int test(int m) { return m38 & m; }
    int *p34() { return &m34; }
    char pad0[0x14]; int f14; int key18; char pad1c[0x34 - 0x1C]; int m34; int m38; char pad3c[0x50 - 0x3C];
    int lo50; int hi54; char pad58[0x6C - 0x58]; Ctx *ctx6C; char pad70[0x7C - 0x70]; s16 *rows7C; s16 n80; char pad82[8]; s16 n8A;
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
    AIter(Arr *arr) : IBase(arr) { if (0) { arr = 0; } }
    Elem **next() { if (idx < last) return &(*a)[++idx]; return 0; }
    Elem *cur() { return (*a)[idx]; }
    Elem *&ref() { return (*a)[idx]; }
};
inline AIter Arr::iter() { return AIter(this); }

struct Grid { int w; int n; char pad8[0x18 - 0x8]; s16 *tbl18; int *row1C; };
struct Ent { u16 key; char pad[6]; };
struct BB {
    char pad0[0xC]; int m0c; char pad10[0x28 - 0x10];
    virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6();
    virtual void v7(); virtual void v8(); virtual void v9(); virtual void v10(); virtual void v11();
    virtual void hit(int, Rec *, int, Elem *);
};
struct Mgr : BB {
    int m2c; int flags30; char pad34[0x44 - 0x34]; u32 n44; char pad48[4]; s16 *remap4C; int m50; char pad54[0x68 - 0x54];
    Ent *tab68; char pad6c[0x84 - 0x6C]; Arr arr; char pad94[0x9C - 0x94]; int m9c; Grid *grid; char pada4[0xB0 - 0xA4];
    int b0, b4, b8;
    int run();
};
int Mgr::run() {
    Elem *o;
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
    gl_func_00000000(m0c);
    gl_func_00000000(S(0x1FF54), m0c);
    b8 = b4 = b0 = 0;
    gl_func_00000000(S(0x1FF58));
    gl_func_00000000(S(0x1FF60), 0);
    gl_func_00000000();
    gl_func_00000000(S(0x1FF6C));
    gl_func_00000000(S(0x1FF74));
    AIter it = arr.iter();
    while (it.next() + 0) { /* agent-c run 247: the folded + 0 moved from the second loop's test to this one (99.36 -> 99.43: the second loop's next() result leaves the call-crossing s2 pool temp, target v0) */
        gl_func_00000000(it.cur());
        if (m50 == 0) {
            if (it.cur()->test(0x2000)) {
                gl_func_00000000(it.cur(), m9c);
            }
        }
        gl_func_00000000(it.cur(), it.cur()->p34());
    }
    gl_func_00000000();
    gl_func_00000000(S(0x1FF7C));
    gl_func_00000000(S(0x1FF88));
    int total = 0;
    u8 *buf;
    if (G1C4(D_493AC_a) & 8) {
        pad1 = (int)this; if (1) { pad1 += 0x30; } *(int *)pad1 |= 0x80; /* agent-g run 221: pointer mutation behind if (1) = the target's addiu v0,s6,48 + lw/sw 0(v0) */
        buf = (u8 *)gl_func_00000000(D_00000000, n44);
        for (u32 i2 = 0; i2 < n44; i2++) {
            buf[i2] = 0;
        }
    }
    Rec rec;
    it = arr.iter();
    while (it.next()) { /* run 206 had a folded + 0 here (kit CLXXIX); run 247 moved it to the first loop */
        o = it.cur();
        /* agent-g run 233 (kit CCXLIV): the context pointer is a ugen temp (t6) in the target, not a coloured
         * local: stored to o->ctx6C and re-read through the member (uopt forwards the store), rec.ctx taken before
         * the rec stores that would kill the forwarding; `g` kept as a home-keeping local (frame offsets) */
        Ctx *g = 0;
        o->ctx6C = *(Ctx **)((char *)&D_493AC_d + 0x214);
        o->n8A = o->ctx6C->count();
        rec.ctx = o->ctx6C;
        rec.n58C = 0;
        rec.f590 = 32; rec.f594 = 3; /* agent-g run 233: one line (32 materialised first, stores 0x674 then 0x670 as the target) */
        gl_func_00000000(&rec);
        int cnt = 0; /* agent-g run 233: after the call (the target's `or s5,zero,zero` follows the &rec argument) */
        if (G1C4(D_493AC_b) & 8) {
            int k = 0;
            if (grid == 0) {
                gl_func_00000000(S(0x1FF90), S(0x1FFA0), 3287);
            }
            o->n80 = o->ctx6C->count();
            o->ctx6C->noop();
            o->lo50 = 0x80;
            o->hi54 = 0;
            for (k = 0; k < grid->n; k++) {
                {
                    int rowStart = grid->w * k;
                    int hit = 0;
                    for (int c = rowStart; c < rowStart + grid->w; c++) {
                        int e = grid->row1C[c];
                        if (e != -1) {
                            int j;
                            while ((j = grid->tbl18[e++]) != -1) {
                                u8 *m = buf + j;
                                if (tab68[j].key == o->key18 && *m == 0) { /* agent-g run 233: table key first (target load order) */
                                    o->f14 = 1;
                                    cnt++;
                                    this->hit(0, &rec, j, o);
                                    hit = 1;
                                    *m = 1;
                                }
                            }
                        }
                    }
                    if (hit) {
                        if (k < o->lo50) o->lo50 = k;
                        if (o->hi54 < k) o->hi54 = k;
                    }
                    gl_func_00000000(&rec); if (1) {}
                    o->rows7C[k] = o->ctx6C->count();
                    o->ctx6C->noop();
                }
            }
        } else {
            o->lo50 = -1;
            for (i = 0; i < n44; i++) {
                if (remap4C != 0) {
                    j = remap4C[i];
                } else {
                    j = i;
                }
                if (tab68[j].key == o->key18) {
                    o->f14 = 1;
                    cnt++;
                    this->hit(0, &rec, j, o);
                }
            }
        }
        gl_func_00000000(&rec);
        total += rec.n58C;
        gl_func_00000000(o->ctx6C);
        if (cnt == 0) {
            o->ctx6C = 0;
        }
    }
    gl_func_00000000(S(0x1FFAC), m0c, total);
    if (G1C4(D_493AC_c) & 8) {
        gl_func_00000000(D_00000000, buf);
    }
    gl_func_00000000();
    gl_func_00000000(S(0x1FFB4));
    gl_func_00000000();
    gl_func_00000000();
    gl_func_00000000(S(0x1FFC4));
    return 1;
}
