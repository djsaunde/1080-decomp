/* IDO 7.1 NCC (C++) donor for func_80000598 (0x40 / 16 words): byte copy.
 *
 * The plain `while (n--) *d++ = *s++;` over unsigned-char locals. Under cfe
 * uopt folds the post-increment / post-decrement temporaries (12 words with
 * params, 15 with locals); the edg front end materialises each one, which is
 * exactly the target: the dead `or a3,a2` count copy before the test and the
 * per-iteration `or a0,v0` / `or a3,v1` / `or a1,a2` snapshots (the old
 * "redundant ABI-dance move" cap). Spliced into kernel_000.c.o. */
extern "C" void func_80000598(void *src, void *dst, int n) {
    unsigned char *s = (unsigned char *)src;
    unsigned char *d = (unsigned char *)dst;
    while (n--) {
        *d++ = *s++;
    }
}
