/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00034890 (0x150 / 84 words), agent-c run 164.
 * `new X(o)` with inline cfront-protocol constructors: X : Mid : Base (Base = vptr only, 4 bytes; Mid stores the
 * owner at +4 and copies the 8-byte template D+0x1E1B0 to +8; X stores its vptr). Under NCC each inlined
 * constructor gets the `this == 0 -> operator new(sizeof)` fallback, which is exactly the target's alloc
 * cascade (0x10 / 0x10 / 4) with the nested null skips. Then register(&D, h, a, o->flags & 2), a by-value
 * {cur, nxt} iterator over o->0x10 calling f(it.cur->data), and f(&D). h is a KEPT inline parameter of make()
 * (`if (0) { h = 0; }`): a named h stays memory-homed; the kept pooled temp is coloured a1 with the target's
 * spill / reload around each alloc. cfe body 90.35 (0x148 bytes) -> 98.10, length 0x150 = target.
 * Residual: h's spill slot 0x28 (target 0x2C, our top slot is make's o parameter), the Mid / Base this
 * colours (target a3 / v1, ours v1 / a0: zdbug:5 priorities Mid 2.0 > Base 1.5) and the next() result
 * (h's pooled slot is reused for it, so both are a1; target a2 then v1). Still NON_MATCHING. */
extern "C" void *gl_func_00034458(...);
inline void *operator new(unsigned n) { return gl_func_00034458(n); }
struct Pair { int a, b; };
extern "C" Pair D_34890_tmpl;
extern "C" char D_34890_d[], D_34890_e[];
struct Link { void *data; Link *next; };
struct O { char pad[0xC]; int h; Link *list; char pad14[4]; int flags; };
struct LIter {
    Link *cur, *nxt;
    LIter(Link *l) { nxt = l; }
    void *next() { return (cur = nxt) ? (nxt = nxt->next, cur->data) : 0; }
};
struct Base { Base() {} virtual void f(); };
struct Mid : Base { O *o; Pair p; Mid(O *oo) { o = oo; p = D_34890_tmpl; } };
struct X : Mid { X(O *oo) : Mid(oo) {} virtual void f(); };
inline void make(O *o, int h) { if (0) { h = 0; } X *a = new X(o); gl_func_00034458(D_34890_d, h, a, o->flags & 2); }
extern "C" void gl_func_00034890(O *o) {
    make(o, o->h);
    LIter it(o->list);
    while (it.next()) {
        gl_func_00034458(it.cur->data);
    }
    gl_func_00034458(D_34890_e);
}
