/* IDO 7.1 NCC (C++) donor for gl_func_0005534C (0x9C / 39 words): command
 * stream walker that calls the first command word as a function, then
 * dispatches the rest (low 16 bits) through the shared handler. Same
 * by-value St / inline member cursor step as gl_func_000558A0
 * (`int next() { return *(*pp)++; }`), which gives the target's p = v1 /
 * s.pp reload = v0 colouring under NCC (kit LXIII).
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY (agent-g run 142). */
extern "C" int gl_func_00000000(...);
struct St5534C {
    int **pp;
    int w;
    int x;
    int y;
    int next() { return *(*pp)++; }
};
extern "C" void gl_func_0005534C(St5534C s) {
    int w;
    w = s.next();
    s.w = w;
    ((int (*)(St5534C *))w)(&s);
    w = s.next();
    s.w = w;
    while (w != 0) {
        s.w &= 0xFFFF;
        gl_func_00000000(&s);
        w = s.next();
        s.w = w;
    }
}
