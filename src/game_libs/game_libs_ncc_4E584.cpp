/* IDO 7.1 NCC (C++) donor for gl_func_0004E584 (0x84 / 33 words): it is the
 * static-initializer (__sti__) of a global object -- an 8-byte member zeroed by its
 * ctor at +0 and a wrapped 4-byte member at +0x28, each built through the cfront
 * this==0 allocation protocol that edg emits for every constructor. The object
 * declaration's `;` sits on its own line: as1 ties the sti code to that line, which
 * schedules the protocol's `or v1,a1` above `sw ra` like the target. The Makefile
 * renames the __sti__ symbol to gl_func_0004E584 (a symbol name only). Spliced into
 * game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
extern "C" void *gl_func_00000000(...);
inline void *operator new(unsigned n) { return gl_func_00000000(n); }
struct M8 { int a, b; M8() { b = 0; a = 0; } };
struct M4 { int a; M4() { a = 0; } };
struct W4 { M4 m; W4() {} };
struct G { M8 m8; char pad[0x28 - 8]; W4 w; G() {} };
extern "C" G D_4E584_arr;
G D_4E584_arr
;
