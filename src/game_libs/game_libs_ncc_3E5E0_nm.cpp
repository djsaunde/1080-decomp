/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_0003E5E0 (0x260 / 152 words), agent-c run 157.
 * The cfe body's nested alloc-or-given blocks are three `new` expressions of member-function callback
 * objects: a 4-byte polymorphic root K (vptr only), CB : K holding {obj, PMF} (no virtuals of its own, so its
 * ctor stores no vptr), and the two leaf classes CB1 / CB2 whose empty ctors store their vptr LAST -- the
 * target's order (K vptr, obj, pmf, leaf vptr). Global operator new (a class-level operator new adds a
 * protocol level); the PMF argument by const reference (copied straight from its static, no stack temp).
 * cfe 75.74 -> 91.78. Residual: the new result is an s-register variable here (target: a2, spilled to the
 * temp area at sp+0x2C around the inner allocs, with rec / key in s0 / s1), and the arg0 home is read back
 * for the first FW(arg0, 0xC). Length 604 vs target 608 (cfe body 644). The vtable symbols are renamed to
 * zero-pinned blank aliases in the Makefile. Still NON_MATCHING: spliced into build/non_matching/ only.
 * agent-c run 161: the first two registrations as inline `reg0(char *a)` / `reg1(char *a)` with a kept parameter and
 * the new-expression passed straight into the call: each new result gets its own size-pooled temp slot instead of
 * the one home shared with the loop's new (kit CXII), so it stays a short a2 LR spilled around the ctor call like the
 * target: 94.32 -> 99.36, length 600 -> 608 = target, frame 0x38 = target. Residual: the spill slot (target sp+0x2C,
 * ours 0x20) and the inner ctor this / loop temps (s1 / a0 / v1 vs v1 / a2).
 * agent-c run 270 (kit CCCXV, zdbug:1 entry map): the loop as a free inline `loop(it, d0, d1, d2)` whose three dummy int
 * parameters are assigned in a dead block (each takes a pool temp; constant arguments, dead stores deleted) and the
 * loop's new-expression passed straight into the call; reg0 / reg1 without the keep. The dummies hold the first three
 * pool entries during the loop body (allocated before the expansion), so the loop's new-temp lands on reg0's CB / K
 * `this` entry: one variable live across the loop calls -> s1 in both places (target), and the reg0 / reg1 new results
 * spill to M-12 like the target. 99.36 -> 99.90, length 608 = target. Left: frame 0x40 vs 0x38 (the loop's CB2 `this` /
 * o / PMF / CB `this` temps take three fresh entries M-32 .. M-44; the target's CB2 `this` sits on M-24), so every
 * sp offset above the args is +8.
 */
typedef int s32;
extern "C" {
int gl_func_00034458(...);
extern char D_00000000_e5e0a, D_00000000_e5e0c, D_00000000_e5e0e, D_00000000_e5e0f, D_00000000_e5e0h,
    D_00000000_e5e0j, D_00000000_e5e0k, D_00000000_e5e0l, D_00000000_e5e0m, D_00000000_e5e0o,
    D_00000000_e5e0q, D_00000000_e5e0r, D_00000000_e5e0s;
}
#define FW(p, o) (*(int *)((char *)(p) + (o)))
struct PMF { s32 w0, w1; };
inline void *operator new(unsigned n) { return (void *)gl_func_00034458(n); }
struct K { virtual void f(); };
struct CB : K {
    void *obj;
    PMF pmf;
    CB(void *o, const PMF &p) { obj = o; pmf = p; }
};
struct CB1 : CB { CB1(void *o, const PMF &p) : CB(o, p) {} virtual void f(); };
struct CB2 : CB { CB2(void *o, const PMF &p) : CB(o, p) {} virtual void f(); };
inline void reg0(char *a) { gl_func_00034458(&D_00000000_e5e0e, &D_00000000_e5e0f + 0x1F318, new CB1(a, *(PMF *)(&D_00000000_e5e0c + 0x1EEC0)), 0); }
inline void reg1(char *a) { gl_func_00034458(&D_00000000_e5e0j, &D_00000000_e5e0k + 0x1F324, new CB1(a, *(PMF *)(&D_00000000_e5e0h + 0x1EEC8)), 1); }
inline void loop(char *it, int d0, int d1, int d2) {
    if (0) { it = 0; d0 = 0; d1 = 0; d2 = 0; } /* run 270: the three dummies hold pool entries during the body */
    if (it != 0) {
        do {
            gl_func_00034458(&D_00000000_e5e0q, it + 0x2C, new CB2(it, *(PMF *)(&D_00000000_e5e0o + 0x1EED0)));
            it = (char *)FW(it, 0x30);
        } while (it != 0);
    }
}
extern "C" void gl_func_0003E5E0(char *arg0) {

    { int x = FW(arg0, 0xC); gl_func_00034458(&D_00000000_e5e0a, x, 0); } /* run 158: read before the call setup = the target's `lw a1,12(a0)` straight from a0 (no home reload); 91.78 -> 94.32, length 604 -> 600 (target 608: away, flagged) */
    reg0(arg0);
    reg1(arg0);
    gl_func_00034458(&D_00000000_e5e0l, &D_00000000_e5e0m + 0x1F330, 0);
    loop((char *)FW(arg0, 0x2C), 1, 1, 1);
    gl_func_00034458(&D_00000000_e5e0r);
    gl_func_00034458(&D_00000000_e5e0s);
    gl_func_00034458(arg0);
}
