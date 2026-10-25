/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_0004B620 (0x4D4 / 309 words), agent-c run 157.
 * The function is a C++ constructor Obj::Obj(void *a1) : Base(a1): cfront's `this == 0 -> new(0xD8)`
 * protocol, the inline Base ctor (its own `new(0xCC)` fallback, external BB ctor, FP / field defaults),
 * the vptr store at 0x28, two display lists built with an inline DL::add, and a walk of the element array
 * at 0x84 through the by-value index iterator (4E244 / 49B3C family). What the NCC form gives that the
 * cfe C body could not: self in s4 with five saved registers, the 0xB0 frame with the iterator copies at
 * sp+96 / 108 / 164 (edg pool temps + per-expansion `g` homes), and the per-append rotation of g.
 * - `Gfx *g` is a named local of DL::add (one variable per expansion: v0 / a0 / a1 rotation like the target);
 * - `if (0) { hi = lo = 0; }` keeps hi / lo as edg parameter temps (two pool slots, 98.82 vs 97.34);
 * - the `&elem->m34` argument goes through a reference accessor (`Elem *&ref()`): the address, not the
 *   loaded value, is the pool temp (97.40 vs 96.09);
 * - IBase stores `a` before `idx` (the target's sw order).
 * Residual (98.82): the DL head / count temps of the first two appends (target v1 / s0, then s0 / s3) and
 * the &it / ++idx colours. agent-c run 159: AIter(Arr *) keeps its parameter (`if (0) { arr = 0; }`), the
 * IBase construct temp then colours like the target (98.82 -> 99.00, length unchanged).
 * Still NON_MATCHING: spliced into build/non_matching/ only. */
extern "C" {
void *gl_func_00000000(...);
extern char D_00000000[];
}
#define S(o) (D_00000000 + (o))
inline void *operator new(unsigned n) { return gl_func_00000000(n); }
struct Gfx { unsigned w0, w1; };
struct DLHead { Gfx *buf; int count; };

struct DL {
    int pad[3];
    DLHead *head;
    void addE(unsigned hi, unsigned lo) { Gfx *g = &head->buf[head->count++]; g->w0 = hi; g->w1 = lo; } /* run 173: no keep on the 4th packet (per-site grid) */
    void add(unsigned hi, unsigned lo) { Gfx *g = &head->buf[head->count++]; g->w0 = hi; g->w1 = lo; if (0) { hi = lo = 0; } }
};
struct Elem { char pad[0x34]; int m34; char pad38[0x64 - 0x38]; DL *m64; };
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
    Elem *&ref() { return (*a)[idx]; }
};
inline AIter Arr::iter() { return AIter(this); }
struct BB {
    char pad[0x84];
    BB(char *name, void *a1, int z);
};
struct Base : BB {
    Arr arr;            /* 0x84 */
    char pad94[0xB0 - 0x94];
    float b0, b4, b8, bc;
    short c0;
    int c4;
    float c8;
    Base(void *a1) : BB(S(0x200FC), a1, 0) { c0 = 0; c4 = 0; b8 = 0.0f; bc = 0.0f; c8 = 0.0f; b0 = 1.0f; b4 = 1.0f; }
};
struct Obj : Base {
    DL *cc;
    DL *d0;
    int d4;
    Obj(void *a1);
};
Obj::Obj(void *a1) : Base(a1) {
    *(char **)((char *)this + 0x28) = S(0);
    d0 = 0;
    d4 = 0;
    cc = (DL *)gl_func_00000000(0, S(0x20104), 0x14, 0, 0, 0, 0);
    cc->add(0xE7000000, 0);
    cc->add(0xBA001402, 0);
    cc->add(0xB9000201, 4);
    cc->addE(0xBA000C02, 0x2000);
    cc->add(0xBA001301, 0);
    cc->add(0xBB000001, 0x80008000);
    gl_func_00000000(cc);
    AIter it = arr.iter();
    while (it.next()) {
        it.cur()->m64 = (DL *)gl_func_00000000(0, S(0x20110), 0x14, 0, 0, 0, 0);
        gl_func_00000000(it.cur()->m64, &it.ref()->m34);
        gl_func_00000000(it.cur()->m64, &it.ref()->m34);
        gl_func_00000000(it.cur()->m64, it.cur(), 0);
        gl_func_00000000(it.cur()->m64);
    }
    d0 = (DL *)gl_func_00000000(0, S(0x2011C), 0xA, 0, 0, 0, 0);
    d0->add(0xBA001301, 0x80000);
    d0->add(0xB9000201, 0);
    d0->add(0xBB000000, 0x80008000);
    gl_func_00000000(d0);
}
