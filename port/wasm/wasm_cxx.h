/* port/wasm/wasm_cxx.h -- the C++ donors' C-linkage calls on the wasm32 build (plan section 30).
 *
 * port/gen/decls_cpp.h declares each C callee the donors name with the ABI types of its definition (`int`,
 * `void *`) plus a `...` tail, and g++ -fpermissive accepts the donors' calls that pass a pointer for an
 * int, an int for a pointer, or more arguments than the callee takes. clang has no -fpermissive, and a wasm
 * `...` is not a register tail: the extra arguments would go in a memory block whose address the callee
 * reads as its next parameter. So the wasm build compiles the donors against decls_cpp_wasm.h
 * (port/wasm/gen-decls-wasm.py), where
 *   - an int-class or pointer parameter is a PortW<T>: a one-member struct (passed as its member: the wasm
 *     C ABI passes a single-scalar struct directly) that converts from any integer, enum, pointer or array;
 *   - the `...` tail of a callee that is not variadic is PORT_WASM_TAIL extra PortWA parameters defaulted
 *     to 0: LTO's FixFunctionBitcasts drops the extra arguments at the call (the K&R call of the ROM);
 *   - a 4-byte struct passed by value (BYVALUE_CALLERS) becomes its bits (PortWA copies a class's bytes).
 * Only the donors' calls change type; the C definitions keep theirs. */
#ifndef PORT_WASM_CXX_H
#define PORT_WASM_CXX_H
#ifdef __cplusplus

template <bool B> struct PortWTag {};

template <class P> struct PortW {
    P v;
    template <class T> static P conv(const T &x, PortWTag<false>) { return (P)(long)x; }
    template <class T> static P conv(const T &x, PortWTag<true>) {
        unsigned int w = 0;
        __builtin_memcpy(&w, &x, sizeof(T) < 4 ? sizeof(T) : 4);
        return (P)(long)w;
    }
    PortW() : v(0) {}
    template <class T> PortW(const T &x) : v(conv(x, PortWTag<__is_class(T) || __is_union(T)>())) {}
    template <class T> PortW(T *x) : v((P)(long)x) {}
    PortW(float x) : v((P)(long)x) {}
    PortW(double x) : v((P)(long)x) {}
};
/* pointers: an integer is an address (the N64 / ILP32 meaning) */
template <> struct PortW<void *> {
    void *v;
    PortW() : v(0) {}
    template <class T> PortW(const T &x) : v((void *)(long)x) {}
    template <class T> PortW(T *x) : v((void *)x) {}
    template <class T> PortW(const T *x) : v((void *)x) {}
};

typedef PortW<int> PortWA;

/* an int-class or pointer result (returned as its one word): reads as an int or as any pointer, as the donors'
 * -fpermissive reads did */
struct PortWR {
    int v;
    operator int() const { return v; }
    template <class T> operator T *() const { return (T *)(long)v; }
};
#define PORT_WASM_TAIL_DEFAULTS PortWA = PortWA(), PortWA = PortWA(), PortWA = PortWA(), PortWA = PortWA(), \
    PortWA = PortWA(), PortWA = PortWA(), PortWA = PortWA(), PortWA = PortWA()

#endif
#endif
