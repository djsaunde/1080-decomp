/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_0004B0A8 (0x24C / 147 words), agent-c run 164 (was the
 * run-134 C lift, 94.76). The function is the constructor `Obj::Obj(Src *s) : Base(name)` (Makefile rename of
 * __ct__3ObjFP3Src): cfront `this == 0 -> new(0x44)`, out-of-line Base(char *) (Base = 0x28 data bytes + the
 * virtual, so Obj's own vptr store lands at +0x28), a Pair copied from the vt+0x140 template, f0 = 2, f40 from an
 * out-of-line `ctor(0, name, w)` call, four f40 calls with &pair, then 41D40's by-value AIter over s->arr: every
 * element whose +0x6C is set gets a gSPDisplayList packet on f40's DL (named-d next()) whose w1 is the 0x34458
 * result for (*t->base + k * 8, t, k). Load-bearing: t and k read from TWO it.cur() calls passed to an inline
 * res(Elem *e1, Elem *e2) (the target re-reads the element pointer for k; one named e is 95.1, 576 -> 588 bytes);
 * Tex::base is a char ** (the target dereferences it); IBase writes `a = arr; idx = -1;` (emitted idx first, as the
 * target); five end-of-body block volatiles = frame 0xA8; AIter::next() as an if / return (the first expansion's colours match:
 * 98.73 -> 98.80 local); the
 * iterator built by a free `iter(Arr *)` instead of the member Arr::iter() puts the first iterator temp at the
 * target's sp+0x78 (98.81). 94.76 -> 98.81, length = target. Residual: the
 * colours of the iterator ctor this (a3 vs v1), the two element pointers and the next() results. Still NON_MATCHING. */
extern "C" {
void *gl_func_00034458(...);
extern char D_00000000[];
extern char gl_d_4b0a8_tbl, gl_d_4b0a8_vt, gl_d_4b0a8_w214;
}
inline void *operator new(unsigned n) { return gl_func_00034458(n); }
struct Pair { int a, b; };
struct Gfx { unsigned w0, w1; };
struct DLHead { Gfx *buf; int count; };
struct DL { char pad[0xC]; DLHead *dl; Gfx *next() { DLHead *d = dl; int n = d->count; d->count = n + 1; return &dl->buf[n]; } };
struct Tex { char pad[0xC]; char **base; };
struct Elem { char pad[0x6C]; Tex *t; char pad70[0x8A - 0x70]; short k; };
struct AIter;
struct Arr { Elem **data; int pad4, pad8; int count; Elem *&operator[](int i) { return data[i]; } AIter iter(); };
struct IBase {
    Arr *a;
    int idx;
    int last;
    IBase(Arr *arr) { a = arr; idx = -1; last = arr->count - 1; }
};
struct AIter : IBase {
    AIter(Arr *arr) : IBase(arr) { if (0) { arr = 0; } } /* run 190: kept parameter */
    Elem **next() { if (idx < last) return &(*a)[++idx]; return 0; } /* run 190: back to ++idx (with the two levers around it) */
    Elem *cur() { Arr &r = *a; return r[idx]; } /* run 190: local-reference read (kit CXLIV) */
    AIter *me() { return this; } /* agent-c run 238: zero-code pool hold for the loop increment (kit CCLIV) */
};
inline AIter iter(Arr *a) { return AIter(a); }
inline void *res(Elem *e1, Elem *e2) { Tex *t = e1->t; int k = e2->k; return gl_func_00034458(*t->base + k * 8, t, k); }
struct Src { char pad[0x84]; Arr arr; };
struct Base { int f0; char pad[0x24]; Base(char *name); virtual void f(); };
struct Obj : Base {
    virtual void f();
    char pad2c[0x40 - 0x2C];
    DL *f40;
    Obj(Src *s);
};
Obj::Obj(Src *s) : Base(D_00000000 + 0x200EC) {
    Pair p = *(Pair *)(&gl_d_4b0a8_vt + 0x140);
    f0 = 2;
    f40 = (DL *)gl_func_00034458(0, D_00000000 + 0x200F4, *(int *)(&gl_d_4b0a8_w214 + 0x214));
    gl_func_00034458(f40, &p);
    gl_func_00034458(f40, &p);
    gl_func_00034458(f40, &p);
    gl_func_00034458(f40, &p);
    AIter it = iter(&s->arr);
    /* agent-c run 238: the increment holds three pool slots while edg emits the rotated bottom copy of
     * it.next(), so its result / pointer / ++idx temps take the target's a0 / v1 / v0 (99.56 -> 99.71); the
     * three hold homes replace the four end-of-body volatile pads (frame 0xA8 kept, 99.83) */
    for (; it.next(); it.me()->me()->me()) {
        if (it.cur()->t != 0) {
            DL *d = f40;
            void *r = res(it.cur(), it.cur());
            Gfx *g = d->next();
            g->w0 = 0x06000000;
            g->w1 = (unsigned)r;
        }
    }
    gl_func_00034458(f40);
}
