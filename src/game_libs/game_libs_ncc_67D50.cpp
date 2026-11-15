/* IDO 7.1 NCC (C++) donor for str_fill: memset (12 words). The plain while (n--) *p++ = c; under NCC gives the target's or a3,a2 n-- copy ahead of the sb; cfe capped at 2 raw (40 spellings).
 * Spliced into game_libs_post1b.c.o via REPLACE_FUNC_BODY. */
extern "C" void *str_fill(void *s, int c, int n) {
    unsigned char *p = (unsigned char *)s;
    while (n--) *p++ = c;
    return s;
}
