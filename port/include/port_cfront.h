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

#endif /* PORT_CFRONT_H */
