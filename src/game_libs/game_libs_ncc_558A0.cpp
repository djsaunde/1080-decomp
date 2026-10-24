/* IDO 7.1 NCC (C++) donor for gl_func_000558A0 (0x7C / 31 words): command
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
    int next() { return *(*pp)++; }
};
extern "C" void gl_func_000558A0(St558A0 s) {
    int w;
    w = s.next();
    s.w = w;
    while (w != 0) {
        s.w &= 0xFFFF;
        gl_func_00000000(&s);
        w = s.next();
        s.w = w;
    }
}
