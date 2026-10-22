/* IDO 7.1 NCC (C++) donor for func_00000A9C: mode letter -> flag bit.
 * A switch whose cases assign one result variable: edg lowers it to a compare
 * chain (NCC never emits jumptables) and the 'n'/'s' arm's `r = 8` is hoisted
 * into the 'n' compare's delay slot (v1), returned through `or v0,v1`.
 * Spliced into bootup_uso.c.o via REPLACE_FUNC_BODY (agent-c run 121). */
extern "C" int func_00000A9C(int a0, int a1) {
    int r;
    switch (a1) {
    case 0: r = 1; break;
    case 'e': r = 4; break;
    case 'd': r = 0x10; break;
    case 'm': r = 0x40; break;
    case 't': r = 0x20; break;
    case 'n': case 's': r = 8; break;
    default: r = 2;
    }
    return r;
}
