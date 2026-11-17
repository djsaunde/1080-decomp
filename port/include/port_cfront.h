/* port/include/port_cfront.h -- cfront (IDO NCC) virtual dispatch for the C++ donors on the port
 * (docs/1080_PC_PORT_PLAN.md 17.8, agent-g run 623).
 *
 * The game's C++ objects carry a cfront vptr: a plain data member placed after the data members
 * of the class that introduces the virtuals (+0x28 for the usual 40-byte Node prefix), pointing
 * at a ROM vtable (a module blob on the port) whose slot N is {s16 delta, pad, fn} at vtbl + 8 + 8*N
 * (include/gdl/node.h VT_SLOT). g++ would put its own vptr at +0 with 4-byte entries, shift every
 * data member by 4 and store `_ZTV*` addresses in the constructors.
 *
 * So under PORT a donor class with virtuals is declared WITHOUT `virtual`: an explicit
 * `char *vptr` member where cfront puts it, and one CF_Vk line per virtual it calls. CF_Vk defines
 * an always-inlined member function with the virtual's name and signature that calls slot N the
 * way the C code does (VT_SLOT(vt, N).fn(VT_SLOT(vt, N).delta + obj, ...)), so the call sites
 * (`root->f()`, `a1->v4()`) stay as written. Constructors that installed a cfront vtable store
 * the ROM vtable explicitly under PORT (see the donors).
 *
 * The dispatchers are always_inline: the donors' local classes reuse names (VO, Base, Obj) with
 * different slot numbers, so an out-of-line COMDAT copy could be merged across units. */
#ifndef PORT_CFRONT_H
#define PORT_CFRONT_H

struct PortCfVtEnt {
    short delta;
    short pad;
    int (*fn)();
};

/* The cfront constructor protocol (plan 18.11): a cfront ctor called with this == 0 allocates the
 * object itself (`if (this == 0) this = new(size)`) and returns this; the ROM calls them that way
 * (`BGBox_ctor(0, name, ..)`).  A gcc ctor neither allocates nor returns anything, so each donor
 * whose ctor the ROM calls by name defines an extern "C" port_ctor_<name> entry with
 * CF_CTOR_NEW, and port/sourcemap.tsv renames that entry (not the mangled ctor) to <name>. */
inline void *operator new(unsigned, void *p) { return p; }
#define CF_CTOR_NEW(self, Cls, args) ((self) != 0 ? ::new ((void *)(self)) Cls args : new Cls args)

#define CF_SLOT(vt, n) (((PortCfVtEnt *)((char *)(vt) + 8))[n])
#define CF_THIS(n) ((char *)this + CF_SLOT(this->vptr, n).delta)
#define CF_FN(n, T) ((T)CF_SLOT(this->vptr, n).fn)
#define CF_INL __attribute__((always_inline)) inline

/* virtual R name(T1, ..) as slot n of this->vptr */
#define CF_V0(R, name, n) \
    CF_INL R name() { return CF_FN(n, R (*)(char *))(CF_THIS(n)); }
#define CF_V1(R, name, n, T1) \
    CF_INL R name(T1 a1) { return CF_FN(n, R (*)(char *, T1))(CF_THIS(n), a1); }
#define CF_V2(R, name, n, T1, T2) \
    CF_INL R name(T1 a1, T2 a2) { return CF_FN(n, R (*)(char *, T1, T2))(CF_THIS(n), a1, a2); }
#define CF_V3(R, name, n, T1, T2, T3) \
    CF_INL R name(T1 a1, T2 a2, T3 a3) { return CF_FN(n, R (*)(char *, T1, T2, T3))(CF_THIS(n), a1, a2, a3); }
#define CF_V4(R, name, n, T1, T2, T3, T4) \
    CF_INL R name(T1 a1, T2 a2, T3 a3, T4 a4) { \
        return CF_FN(n, R (*)(char *, T1, T2, T3, T4))(CF_THIS(n), a1, a2, a3, a4); }
#define CF_V5(R, name, n, T1, T2, T3, T4, T5) \
    CF_INL R name(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) { \
        return CF_FN(n, R (*)(char *, T1, T2, T3, T4, T5))(CF_THIS(n), a1, a2, a3, a4, a5); }

/* Cross-donor members (agent-a run 810). A donor that uses a class whose constructor / method is
 * defined OUT OF LINE elsewhere references it by its mangled name; the matching build renames that
 * reference (Makefile objcopy --redefine-sym) to a token the ROM relocation binds to the real body:
 * a C function (Node_ctor, MCamera_ctor, ...) or another donor's renamed body. g++ ignores asm labels
 * on constructors, so under PORT the member gets an inline body that calls the real body through
 * CF_EXT: `local` is a C++-side name for the C symbol `sym` (a GNU asm label), so the declaration
 * can carry the donor's own parameter types without clashing with port/gen/decls_cpp.h.
 * cfront constructors take `this` first and allocate when it is 0; the inline bodies always pass a
 * live `this` (base / member subobjects; for `new T` the class is given its real size, so g++'s
 * operator new -- the donor's, mem_new -- allocates the same block the C ctor would). The bodies are
 * CF_INL: the donors reuse class names (B, M, BB, Base) for different classes, and an out-of-line COMDAT
 * copy of one unit's B::B could be merged into another unit. */
#define CF_EXT(R, local, sym, params) extern "C" R local params __asm__(#sym)

/* cfront pointers to member functions (agent-a run 812, plan 18.7). cfront / edg keep a PMF as
 * 8 bytes {s16 delta, s16 index, fn-or-vptr-offset}; g++ would read the same 8 bytes as its own
 * {ptr, adj}. Under PORT a donor's PMF type is `typedef PortCfPmf PM;` and the call / compare sites
 * stay as written: `(obj->*pm)(..)` picks the operator->* below, `pm != null` the operators here.
 * The decode is the ROM's (the edg dispatch in RiderAnim_update_0000D204, identical to the C
 * MemberCallback_invoke / _00007BF4):
 *   index < 0   non-virtual: fn is the function, called with this + delta;
 *   index >= 0  virtual: vptrOff is the offset of the vptr in the (this + delta) object, where
 *               vptrOff 0 with delta 0 means the Node vptr at +0x28; the slot is the
 *               {s16 delta, fn} entry at vptr + 8 * index (index is 1-based: CF_SLOT(vt, index - 1)),
 *               called with this + delta + the slot's delta.
 * Equality is the edg 2-word compare. The ROM's PMF records (game Data 0x840, 0xDB0..0xF58) are all
 * non-virtual {0, -1, fn}; the null PMF is {0, 0, 0}. */
struct PortCfPmf {
    short delta;
    short index;
    union { int (*fn)(); int vptrOff; } u;
    CF_INL bool operator==(const PortCfPmf &o) const {
        return delta == o.delta && index == o.index && u.vptrOff == o.u.vptrOff;
    }
    CF_INL bool operator!=(const PortCfPmf &o) const { return !(*this == o); }
};
struct PortCfPmfBound {
    char *self;
    int (*fn)(char *);
    CF_INL int operator()() const { return fn(self); }
};
template <class T> CF_INL PortCfPmfBound operator->*(T *obj, const PortCfPmf &pm) {
    PortCfPmfBound b;
    b.self = (char *)obj + pm.delta;
    if (pm.index < 0) {
        b.fn = (int (*)(char *))pm.u.fn;
    } else {
        int off = (pm.u.vptrOff == 0 && pm.delta == 0) ? 0x28 : pm.u.vptrOff;
        PortCfVtEnt *e = (PortCfVtEnt *)(*(char **)(b.self + off) + 8 * pm.index);
        b.self += e->delta;
        b.fn = (int (*)(char *))e->fn;
    }
    return b;
}

#endif /* PORT_CFRONT_H */
