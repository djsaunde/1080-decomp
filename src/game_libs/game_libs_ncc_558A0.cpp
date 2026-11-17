#include "../../include/uso_call.h"  /* NCC donors build without -I include */
/* IDO 7.1 NCC (C++) donor for nodelist_dispatchList_000558A0 (0x7C / 31 words): command
 * stream walker. The by-value parameter s = {pp, w, ...} is homed at sp+32
 * and &s goes to every callee; each command word is stored into s.w.
 * The cursor step is an inline MEMBER `int next() { return *(*pp)++; }`:
 * expanded under NCC it gives the target's colouring (cursor p in v1, the
 * s.pp reload in v0). Every cfe / NCC spelling of the step as statements
 * (p = *s.pp; *s.pp = p + 1; w = *p) or a free inline function puts p in
 * v0 (kit LXIII).
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY (agent-g run 142). */
extern "C" int gl_func_00000000(...);
struct St558A0 {
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
extern "C" void nodelist_dispatchList_000558A0(St558A0 s) {
    int w;
    w = s.next();
    s.w = w;
    while (w != 0) {
        s.w &= 0xFFFF;
        CALL(nodelist_dispatch)(&s);
        w = s.next();
        s.w = w;
    }
}
