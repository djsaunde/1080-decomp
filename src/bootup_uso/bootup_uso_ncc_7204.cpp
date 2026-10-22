/* IDO 7.1 NCC (C++) donor for func_00007204: allocate a two-field record
 * (new Pair(a1, p): word + M4 member with its own cfront this==0 protocol)
 * and hand it to the list at a0 + 0x2C. 33/33 words.
 *
 * The hand-off is an INLINE member of the list, `L::add(Pair *n)`, that
 * forwards to the blank import: its parameter `n` is a separate web from the
 * new-expression result, so the result stays in the ctor's register (a2) and
 * is copied into a1 only at the call (`or a1,a2`). Calling the import
 * directly with the result (every spelling tried in run 122: named copies,
 * casts, prototypes, operator new forms) coalesces the result into a1.
 * Spliced into bootup_uso.c.o via REPLACE_FUNC_BODY (agent-c run 123). */
extern "C" void *func_00000000(...);
inline void *operator new(unsigned n) { return func_00000000(n); }
struct Val { int v; };
struct M4 { int a; M4() { a = 0; } };
struct Pair {
    int a;
    M4 b;
    Pair(int x, Val p) { a = x; *(Val *)&b = p; }
};
struct L { void add(Pair *n) { func_00000000(this, n); } };
extern "C" void func_00007204(char *a0, int a1, Val p) {
    Pair *o = new Pair(a1, p);
    ((L *)(a0 + 0x2C))->add(o);
}
