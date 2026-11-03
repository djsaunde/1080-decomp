/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00041D40 (0x19C / 103 words), agent-c run 162.
 * Registry dump: banner, clear the global array's count, log, then walk the global array with the by-value
 * index iterator of 4B620 / 49B3C (AIter(Arr *) with a kept parameter, Arr::iter() returning by value),
 * printing each element's +0xC and notifying its +0x14 owner. cfe body 92.99 (408 of 412 bytes) -> 96.11 at
 * 412 bytes = target with a block of six unused volatiles at the end (frame 0x88). Residual: the array pointer
 * load before the iterator construct (target: in the bnez delay slot), next()'s result in s1 (target v1 / a0:
 * the pooled return temp merges with a call-crossing slot) and the s1 / s2 order. Still NON_MATCHING. * agent-c run 174: the walk is a `for (AIter it = iter(..); it.next(); )` -- EDG keeps the for-init's pooled Arr*
 * temp alive for the whole statement, so the condition's next() gets fresh temps (target: next()'s array load in
 * v0 / v1, not the body's call-crossing s1); its two extra frame words come out of the dead volatile block (6 -> 4),
 * and next() is `return &(*a)[++idx]` again. 99.17 -> 99.26 (objdiff diff). Left: next()'s index temp a0 (target
 * a1) in the entry copy, and the bottom copy's array / index temps swap v0 / v1.
 * agent-c run 164: the iterator from a free `inline AIter iter(Arr *a) { return AIter(a); }` instead of the member
 * Arr::iter() (the member's implicit this was an extra pooled temp between the iterator objects): 95.76 -> 98.97 local.
 * agent-c run 262: EXACT (99.90 -> 100). The walk is written as `{ AIter it = ..; if (it.first() + 0) do { .. }
 * while (it.adv() + 0); }` (the two tests as separate source expressions) with the bottom advance naming its index
 * and array pointer; the dead volatile block shrinks 4 -> 3 for the frame. Spliced over the post0b placeholder on
 * both build paths; the gl_func_00034458 calls are renamed to the blank gl_func_00041D40_c34458 = 0 (the target's
 * jal words are blank USO calls).
 */
extern "C" {
void *gl_func_00034458(...);
extern char D_00000000[];
}
#define S(o) (D_00000000 + (o))
inline void *operator new(unsigned n) { return gl_func_00034458(n); }
struct Elem { char pad[0xC]; int xC; char pad10[4]; char *x14; };
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
    /* agent-c run 262: the entry test and the loop-bottom test are separate source expressions (explicit
     * `if (..) do { } while (..)`): the entry copy is the ?: form, the bottom copy names the index and the
     * array pointer (`int i = ++idx; Arr *p = a;`), so the bottom array load is its own uopt live range (v1)
     * instead of sharing the edg temp that holds the entry copy's idx + 1 (a1). */
    Elem **first() { return idx < last ? &(*a)[++idx] + 0 : 0; }
    Elem **adv() { if (idx < last) { int i = ++idx; Arr *p = a; return &(*p)[i] + 0; } return 0; }
    Elem *cur() { return (*a)[idx]; }
};
inline AIter iter(Arr *a) { return AIter(a); }
extern "C" void gl_func_00041D40(int arg0) {
    gl_func_00034458(S(0x1F6B4));
    (*(Arr **)D_00000000)->count = 0;
    gl_func_00034458(arg0);
    { AIter it = iter(*(Arr **)D_00000000); if (it.first() + 0) do {
        Elem *e = it.cur();
        gl_func_00034458(S(0x1F6C0), e->xC);
        e = it.cur(); if (1) {}
        if (e->x14 != 0) {
            gl_func_00034458(e->x14 + 0x10, e);
        }
    } while (it.adv() + 0); }
    { volatile int q0, q1, q2; }
    gl_func_00034458();
}
