#include "../../include/uso_call.h"  /* NCC donors build without -I include */
/* IDO 7.1 NCC (C++) donor for nodelist_walk_0005534C (0x9C / 39 words): command
 * stream walker that calls the first command word as a function, then
 * dispatches the rest (low 16 bits) through the shared handler. Same
 * by-value St / inline member cursor step as nodelist_dispatchList_000558A0
 * (`int next() { return *(*pp)++; }`), which gives the target's p = v1 /
 * s.pp reload = v0 colouring under NCC (kit LXIII).
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY (agent-g run 142). */
extern "C" int gl_func_00000000(...);
struct St5534C {
    int **pp;
    int w;
    int x;
    int y;
#ifdef PORT64
    /* plan 21.7: the callers pass the whole NodeListState (0x4C bytes, gdl/nodelist.h) and the handlers
     * read past these four words through &s.  On MIPS / i386 every byte of it is in memory after the
     * homes; x86-64 passes a 16-byte struct in two registers and a 0x4C-byte one in memory, so the
     * callee must declare the full size to read the caller's copy. */
    int rest[(0x4C - 16) / 4];
#endif
    int next() { return *(*pp)++; }
};
extern "C" void nodelist_walk_0005534C(St5534C s) {
    int w;
    w = s.next();
    s.w = w;
    ((int (*)(St5534C *))w)(&s);
    w = s.next();
    s.w = w;
    while (w != 0) {
        s.w &= 0xFFFF;
        CALL(nodelist_dispatch)(&s);
        w = s.next();
        s.w = w;
    }
}
