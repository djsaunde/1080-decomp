/* IDO 7.1 NCC (C++) donor for gl_func_0004E244 (0x138 / 78 words): call the
 * third virtual of every element of a pointer array, through the array's
 * by-value index iterator (12-byte {arr, idx, last}).
 * - The iterator's fields live in a BASE class whose ctor is reached through
 *   the derived ctor: only then does edg emit cfront's `this == 0 -> new(12)`
 *   protocol the target has around the construction (sp+60 is never 0).
 * - Arr::iter() returns AIter(self) by value: construct (sp+60) -> return temp
 *   (sp+76) -> it (sp+92), the two lw/sw copy ladders; the named `self` is the
 *   4-byte slot at sp+72, and the dead `if (0) { self = this; }` recolours the
 *   construction block (&tmp v1 / this a1 / array a2) like the target.
 * - Arr::operator[] makes the array pointer and the pre-incremented index
 *   inline-parameter webs (v0/v1/a0/a1 candidates instead of ring temps).
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY (agent-c run 124). */
extern "C" {
void *gl_func_00000000(...);
}
inline void *operator new(unsigned n) { return gl_func_00000000(n); }
struct Pad40 { char pad[0x28]; };
struct Elem : Pad40 { virtual void v1(); virtual void v2(); virtual void v3(); };
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
inline AIter Arr::iter() { Arr *self = this; if (0) { self = this; } return AIter(self); }
extern "C" void gl_func_0004E244(Arr *src) {
    AIter it = src->iter();
    while (it.next()) it.cur()->v3();
}
