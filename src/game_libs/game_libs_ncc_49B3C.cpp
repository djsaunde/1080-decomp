/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00049B3C (0x280 / 160 words).
 * agent-c run 162 rewrite: the by-value array iterator of 4B620 (AIter(Arr *) with a kept parameter,
 * Arr::iter() returning by value) over self->0x84 calling each element's +0x5C table slot, a fifth
 * registration, the {cur, next} list iterator over self->0x2C->0x10 calling the +0x28 table, banner, log.
 * 96.95 (previous draft) -> 97.375, length = target, frame 0x80 via a block of two unused volatiles before the
 * iterator. Residual: next() / idx / base colours in the array loop (target v0 / a1 / v1), the list-iterator
 * homes 8 lower, the iterator construct temp 4 lower. Still NON_MATCHING. */
extern "C" {
void *gl_func_00000000(...);
void gl_49B3C_regf(char *, char *, char *, float, float, int);
extern char D_00000000[];
extern char D_49B3C_s1[], D_49B3C_s2[], D_49B3C_s3[], D_49B3C_s4[], D_49B3C_s5[], D_49B3C_s6[];
}
#define S(o) (D_00000000 + (o))
inline void *operator new(unsigned n) { return gl_func_00000000(n); }
struct VT { char pad[8]; short d8; short p2; void (*fn12)(void *); char pad10[8]; short d24; short p3; void (*fn28)(void *); };
struct Elem { char pad[0x28]; VT *vt28; char pad2c[0x5C - 0x2C]; VT *vt5C; };
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
    Elem **next() { return idx < last ? &(*a)[++idx] : 0; }
    Elem *cur() { return (*a)[idx]; }
};
inline AIter Arr::iter() { return AIter(this); }
struct LNode { Elem *data; LNode *next; };
struct LIter {
    LNode *cur, *nx;
    LIter(LNode *h) { nx = h; }
    Elem *next() { return (cur = nx) ? (nx = nx->next, cur->data) : 0; } /* run 173: 34890 spelling (nx->next, no != 0) */
};
struct Sub { char pad[0x10]; LNode *head; };
struct Obj { char pad[0x2C]; Sub *sub; char pad30[0x40 - 0x30]; int x40; int x44; char pad48[0x84 - 0x48]; Arr arr; char pad94[0x9C - 0x94]; int x9C; char padA0[4]; int xA4; };
inline void cb5c(Elem *e) { if (0) { e = 0; } e->vt5C->fn28(e->vt5C->d24 + (char *)e); } /* run 173: the per-element callback through a kept Elem * parameter */
inline AIter iter(Arr *a) { return AIter(a); } /* run 173: kit CXVI free iter() */
extern "C" void gl_func_00049B3C(Obj *self) {
    gl_func_00000000(D_49B3C_s1, S(0x1FFD4), 0);
    gl_func_00000000(D_49B3C_s2, S(0x1FFE0), &self->x40, 0);
    gl_func_00000000(D_49B3C_s3, S(0x1FFE8), &self->x44, 0);
    gl_49B3C_regf(D_49B3C_s4, S(0x1FFF4), (char *)&self->x9C, 0.0f, 10.0f, 0);
    { volatile int pz, py; }
    AIter it = iter(&self->arr);
    while (it.next()) {
        Elem *e = it.cur();
        cb5c(e);
    }
    gl_func_00000000(D_49B3C_s5, S(0x20004), &self->xA4, -1, self->arr.pad8, 0);
    if (self->sub != 0) {
        LIter li(self->sub->head);
        Elem *e;
        while ((e = li.next()) != 0) {
            Elem *d = li.cur->data;
            d->vt28->fn12(d->vt28->d8 + (char *)d);
        }
    }
    gl_func_00000000(D_49B3C_s6);
    gl_func_00000000(self);
}
