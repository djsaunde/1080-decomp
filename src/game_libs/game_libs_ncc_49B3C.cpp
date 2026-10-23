/* IDO 7.1 NCC (C++) NM donor for gl_func_00049B3C (0x280 / 160 words): child
 * registration + 4E244-style by-value array iterator over self->arr. Still
 * NON_MATCHING (32 raw words, cfe NM body 60; agent-c run 124-127 drafts):
 * the target's pooled scratch is a1 (construct `this`, idx + 1, vptr) where
 * this colours v1/v0, and two spills land one slot off. Frame-exact.
 * Spliced into the NON_MATCHING comparison object only; the ROM keeps
 * INCLUDE_ASM. */
extern "C" {
void *gl_func_00000000(...);
void gl_49B3C_regf(char *, char *, char *, float, float, int);
extern char D_00000000[];
extern char D_49B3C_s1[], D_49B3C_s2[], D_49B3C_s3[], D_49B3C_s4[], D_49B3C_s5[], D_49B3C_s6[];
}
inline void *operator new(unsigned n) { return gl_func_00000000(n); }
struct Pad92 { char pad[0x5C]; };
struct Elem : Pad92 { virtual void v1(); virtual void v2(); virtual void v3(); };
struct Pad40 { char pad[0x28]; };
struct Obj : Pad40 { virtual void v1(); };
struct AIter;
struct Arr { Elem **data; int pad4, pad8; int count; Elem *&operator[](int i) { return data[i]; } AIter iter(); };
struct IBase {
    Arr *a;
    int idx;
    int last;
    IBase(Arr *arr) { idx = -1; a = arr; last = arr->count - 1; }
};
struct AIter : IBase {
    AIter(Arr *arr) : IBase(arr) {}
    Elem **next() { return idx < last ? &(*a)[++idx] : 0; }
    Elem *cur() { return (*a)[idx]; }
};
inline AIter Arr::iter() { return AIter(this); }
struct Pad12 { int a, b, c; Pad12() {} };
struct Link { Obj *data; Link *next; };
struct Iter {
    Link *cur, *nxt;
    Iter(Link *head) { nxt = head; }
    Obj *next() { if ((cur = nxt) != 0) { nxt = nxt->next; return cur->data; } return 0; }
};
struct List { char pad[0x10]; Link *head; };
struct Self {
    char pad[0x2C];
    List *list;        /* 0x2C */
    char pad30[0x40 - 0x30];
    int f40, f44;      /* 0x40, 0x44 */
    char pad48[0x84 - 0x48];
    Arr arr;           /* 0x84 */
    int f94[2];
    int f9C;           /* 0x9C */
    char padA0[4];
    int fA4;           /* 0xA4 */
    AIter iter() { return AIter(&arr); }
};
#define S(o) (D_00000000 + (o))
extern "C" void gl_func_00049B3C(Self *self) {
    gl_func_00000000(D_49B3C_s1, S(0x1FFD4), 0);
    gl_func_00000000(D_49B3C_s2, S(0x1FFE0), &self->f40, 0);
    gl_func_00000000(D_49B3C_s3, S(0x1FFE8), &self->f44, 0);
    gl_49B3C_regf(D_49B3C_s4, S(0x1FFF4), (char *)&self->f9C, 0.0f, 10.0f, 0);
    { AIter it = self->iter(); Pad12 dead; while (it.next()) it.cur()->v3(); }
    gl_func_00000000(D_49B3C_s5, S(0x20004), &self->fA4, -1, *(int *)((char *)self + 0x8C), 0);
    if (self->list) {
        Iter li(self->list->head);
        while (li.next()) li.cur->data->v1();
    }
    gl_func_00000000(D_49B3C_s6);
    gl_func_00000000(self);
}
