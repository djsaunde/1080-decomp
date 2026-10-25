/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00041D40 (0x19C / 103 words), agent-c run 162.
 * Registry dump: banner, clear the global array's count, log, then walk the global array with the by-value
 * index iterator of 4B620 / 49B3C (AIter(Arr *) with a kept parameter, Arr::iter() returning by value),
 * printing each element's +0xC and notifying its +0x14 owner. cfe body 92.99 (408 of 412 bytes) -> 96.11 at
 * 412 bytes = target with a block of six unused volatiles at the end (frame 0x88). Residual: the array pointer
 * load before the iterator construct (target: in the bnez delay slot), next()'s result in s1 (target v1 / a0:
 * the pooled return temp merges with a call-crossing slot) and the s1 / s2 order. Still NON_MATCHING. * agent-c run 164: the iterator from a free `inline AIter iter(Arr *a) { return AIter(a); }` instead of the member
 * Arr::iter() (the member's implicit this was an extra pooled temp between the iterator objects): 95.76 -> 98.97 local.
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
    Elem **next() { return idx < last ? &(*a)[++idx] : 0; }
    Elem *cur() { return (*a)[idx]; }
};
inline AIter iter(Arr *a) { return AIter(a); }
extern "C" void gl_func_00041D40(int arg0) {
    gl_func_00034458(S(0x1F6B4));
    (*(Arr **)D_00000000)->count = 0;
    gl_func_00034458(arg0);
    AIter it = iter(*(Arr **)D_00000000);
    while (it.next()) {
        Elem *e = it.cur();
        gl_func_00034458(S(0x1F6C0), e->xC);
        e = it.cur();
        if (e->x14 != 0) {
            gl_func_00034458(e->x14 + 0x10, e);
        }
    }
    { volatile int q0, q1, q2, q3, q4, q5; }
    gl_func_00034458();
}
