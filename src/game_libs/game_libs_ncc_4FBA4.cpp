/* IDO 7.1 NCC (C++) EXACT donor for gl_func_0004FBA4 (0x15C / 87 words), agent-c run 163.
 * Factory: o = new Der (edg's new + inline ctors: Base re-tests this == 0 with a 0x60 fallback,
 * stores the Base vptr at +0x5C, builds its M8 / M4 members through the this==0 protocol, calls
 * its out-of-line init; Der then stores its own vptr), copies src's M8 member, and walks the
 * halfword key list at *pp with an {key, p} iterator, making virtual call 4 (vtbl entry +0x20)
 * with &it for every nonzero key; *pp = it.p after. Load-bearing: the iterator's inline
 * `int next() { return key = *p++ & 0xFFFF; }` (the key in v0 like the target), Der overriding v4
 * (its own vtable store), and `if (0) { gl(&src); }` keeping src in its arg home (not an s-reg).
 * The cfe body reached 77.78. The Makefile renames both vtables (symbol names only). Spliced
 * over an empty placeholder in both build trees. */
extern "C" void *gl_func_00000000(...);
inline void *operator new(unsigned n) { return gl_func_00000000(n); }
struct M8 { int a, b; M8() { b = 0; a = 0; } };
struct M4 { int a; M4() { a = 0; } };
struct It { int key; unsigned *p; int next() { return key = *p++ & 0xFFFF; } };
struct Base {
    char p0[0x34]; M8 m; char p3c[0x58 - 0x3C]; M4 n;
    virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(It *);
    Base() { gl_func_00000000(this); }
};
struct Der : Base { char p60[0x90 - 0x60]; void v4(It *); Der() {} };
extern "C" Der *gl_func_0004FBA4(Der *src, unsigned **pp) {
    Der *o = new Der;
    if (0) { gl_func_00000000(&src); }
    o->m = src->m;
    It it;
    it.p = *pp;
    while (it.next()) {
        o->v4(&it);
    }
    *pp = it.p;
    return o;
}
