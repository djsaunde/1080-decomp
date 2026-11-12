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
 * agent-g run 190: appends 3..9 bump the count through an inline `DLHead::inc() { return count++; }`
 * (`&head->buf[head->inc()]`, no keep): edg's `this` temp is the target's head-in-s0 / old-count-in-s3
 * pair; append 2 keeps the direct `count++` with the hi / lo keep (addOK). 98.98 -> 99.37 (objdiff, NM tree).
 * Left: appends 1-2 old count s3 (target s0), the loop's ++idx / ref temps s0<->s3, appends 5-6 g a1/v0.
 * Still NON_MATCHING: spliced into build/non_matching/ only. */
extern "C" {
void *gl_func_00000000(...);
extern char D_00000000[];
}
#define S(o) (D_00000000 + (o))
inline void *operator new(unsigned n) { return gl_func_00000000(n); }
template <class T> inline T *K3(T *p) { p = p; return p; } /* agent-g run 362: value-preserving identity (pool word, no block split) */
struct Gfx { unsigned w0, w1; };
struct DLHead { Gfx *buf; int count; int inc() { return count++; } };

struct DL {
    int pad[3];
    DLHead *head;
    void addE(unsigned hi, unsigned lo) { Gfx *g = &head->buf[head->inc()]; g->w0 = hi; g->w1 = lo; } /* run 173: no keep on the 4th packet (per-site grid) */
    void addO(unsigned hi, unsigned lo) { Gfx *g = &head->buf[head->count++]; g->w0 = hi; g->w1 = lo; }
    /* agent-g run 362: the count through a self-assigning identity (`K3(this)->head->count++`, only hi kept): one pool
     * word between count and head WITHOUT K1's `if (0)` block split, so g stays in append 1's block (its uopt live range
     * interferes with append 1's v0 / v1 / a0 and takes a1) and the old count takes s0 like the target, 99.773 -> 99.822 */
    void addOK(unsigned hi, unsigned lo) { Gfx *g = &head->buf[K3(this)->head->count++]; g->w0 = hi; g->w1 = lo; if (0) { hi = 0; } }
    void addIK(unsigned hi, unsigned lo) { Gfx *g = &head->buf[head->inc()]; g->w0 = hi; g->w1 = lo; if (0) { hi = lo = 0; } }
    void add(unsigned hi, unsigned lo) { Gfx *g = &head->buf[head->inc()]; g->w0 = hi; g->w1 = lo; }
};
template <class T> inline T *K1(T *p) { if (0) { p = 0; } return p; } /* agent-g run 359: kept-parameter identity */
/* run 173: the first packet as a free inline, DL first. agent-g run 359: the count through a kept-parameter DL
 * (`K1(d)->head->count++`) with only hi kept puts the old count on the target's s0 word and the head on v1
 * (one pool word between the count and head temps), 99.725 -> 99.773 */
inline void addG(DL *d, unsigned hi, unsigned lo) { if (0) { hi = 0; } Gfx *g = &d->head->buf[K1(d)->head->count++]; g->w0 = hi; g->w1 = lo; }
template <class T> inline T *me(T *p) { return p; } /* zero-code accessor (kit CCLXXXIV) */
struct Elem { char pad[0x34]; int m34; char pad38[0x64 - 0x38]; DL *m64; };
struct AIter;
struct Arr { Elem **data; int pad4, pad8; int count; Elem *&operator[](int i) { return data[i]; } AIter iter(); };
struct IBase {
    Arr *a;
    int idx;
    int last;
    /* agent-g run 310: kept ctor parameter (dead `if (0)` store) together with `idx += 1` in next() below -- a random
     * joint draw over the iterator / append lever families (the `+=` alone is 98.62, the keep alone flat): 99.693 -> 99.725 */
    IBase(Arr *arr) { if (0) { arr = 0; } a = arr; idx = -1; last = arr->count - 1; }
};
struct AIter : IBase {
    AIter(Arr *arr) : IBase(arr) { if (0) { arr = 0; } }
    Elem **next() { return idx < last ? &(*a)[idx += 1] : 0; } /* agent-g run 310: `idx += 1` (with the IBase keep above);
                                                                    * was, agent-c run 272: `idx = idx + 1` (not ++idx): the loop's
                                                                    * new-index temp s0 and the &it reload s3 like the target, 99.66 -> 99.68 */
    Elem *cur() { return (*a)[idx]; }
    Elem *&ref() { return (*a)[idx]; }
};
inline void addGN(DL *d, unsigned hi, unsigned lo) { Gfx *g = &d->head->buf[d->head->count++]; g->w0 = hi; g->w1 = lo; }
inline void addGI(DL *d, unsigned hi, unsigned lo) { if (0) { hi = lo = 0; } Gfx *g = &d->head->buf[d->head->inc()]; g->w0 = hi; g->w1 = lo; }
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
inline void g1(Elem *e) { if (0) { e = 0; } gl_func_00000000(e->m64); } /* run 173: the last per-element call through a kept parameter */
Obj::Obj(void *a1) : Base(a1) {
    *(char **)((char *)this + 0x28) = S(0);
    d0 = 0;
    d4 = 0;
    cc = (DL *)gl_func_00000000(0, S(0x20104), 0x14, 0, 0, 0, 0);
    addG(cc, 0xE7000000, 0);
    cc->addOK(0xBA001402, 0);
    cc->add(0xB9000201, 4);
    cc->addE(0xBA000C02, 0x2000);
    cc->add(0xBA001301, 0); if (1) {}
    cc->add(0xBB000001, 0x80008000);
    gl_func_00000000(cc);
    AIter it = arr.iter();
    while (it.next()) {
        /* agent-g run 292: the first two element accesses through zero-code accessors (kit CCLXXXIV grid over the
         * nine it. sites, 163): 99.676 -> 99.693 */
        me(me(&it))->cur()->m64 = (DL *)gl_func_00000000(0, S(0x20110), 0x14, 0, 0, 0, 0);
        gl_func_00000000(it.cur()->m64, &me(&it)->ref()->m34);
        gl_func_00000000(it.cur()->m64, &it.ref()->m34);
        gl_func_00000000(it.cur()->m64, it.cur(), 0);
        g1(it.cur());
    }
    d0 = (DL *)gl_func_00000000(0, S(0x2011C), 0xA, 0, 0, 0, 0);
    d0->add(0xBA001301, 0x80000);
    d0->add(0xB9000201, 0);
    d0->add(0xBB000000, 0x80008000);
    gl_func_00000000(d0);
}
