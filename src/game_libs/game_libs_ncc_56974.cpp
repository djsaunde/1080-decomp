/* IDO 7.1 NCC (C++) donor for gl_func_00056974 (0x3A0) and gl_func_00056D14 (0x2E0):
 * gDPSetCombineMode-style selectors. `switch (mode & 0xF00)` lowers under edg to a
 * compare chain in CASE order (the cfe NMs needed if-goto ladders + if (1) {} and still
 * lost the last test's polarity and a temp-ring phase); each arm appends one 8-byte
 * SETCOMBINE through an inline Ctx::next() (n = dl->n++, then &dl->buf[n]). The mode is
 * read once into a named `k`. Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
extern "C" {
extern char D_00000000[];
int gl_func_00000000(...);
}
struct G { int w0, w1; };
struct DL { G *buf; int n; };
struct Ctx { char pad[0xC]; DL *dl;
    G *next() { int n = dl->n++; return &dl->buf[n]; } };
struct Cmd { int w0, w1; };
#define SET(a, b) { G *g = c->next(); g->w0 = a; g->w1 = b; }
extern "C" void gl_func_00056974(Ctx *c, Cmd *m) {
    int k = m->w1 & 0xF00;
    switch (k) {
    case 0x0: SET(0xFCFFFFFF, 0xFFFE793C); break;
    case 0x400: SET(0xFCFFFFFF, 0xFFFDF6FB); break;
    case 0x700: SET(0xFC41C683, 0xFF8FFFFF); break;
    case 0x100: SET(0xFCFFFFFF, 0xFFFCF279); break;
    case 0x600: SET(0xFC60FEC1, 0x44FE773B); break;
    case 0x200: SET(0xFC30B261, 0x44664924); break;
    case 0x300: SET(0xFC121824, 0xFF33FFFF); break;
    case 0x500: SET(0xFC30B261, 0xFF67FFFF); break;
    case 0x900: SET(0xFC30B261, 0x5566FF7F); break;
    case 0xA00: SET(0xFC40B281, 0x3365FEFF); break;
    case 0x800: SET(0xFC127E24, -0x805); break;
    case 0xC00: SET(0xFC11FE23, -0x805); break;
    case 0xD00: SET(0xFCFFB3FF, 0xFF65FEFF); break;
    default:
        gl_func_00000000(D_00000000 + 0x21980, k);
        break;
    }
}

extern "C" void gl_func_00056D14(Ctx *c, Cmd *m) {
    int k = m->w1 & 0xF00;
    switch (k) {
    case 0: SET(0xFCFFFFFF, 0xFFFE7838); break;
    case 0x400: SET(0xFCFFFFFF, 0xFFFDF638); break;
    case 0x100: SET(0xFCFFFFFF, 0xFFFCF238); break;
    case 0x200:
    case 0x600: SET(0xFC30B3FF, 0x4FFE4838); break;
    case 0x500: SET(0xFC30B3FF, 0xFFFFFE38); break;
    case 0xA00: SET(0xFC40B3FF, 0x3FFDFE38); break;
    case 0xB00: SET(0xFC40E3FF, 0x3FFDCE38); break;
    case 0x900: SET(0xFC30B3FF, 0x5FFEFE38); break;
    case 0x300:
        if (m->w1 & 0x80000) SET(0xFC127FFF, 0xFFFFF238)
        else SET(0xFC257E04, 0x1FFCF3F8)
        break;
    default:
        gl_func_00000000(D_00000000 + 0x2199C, k);
        break;
    }
}
