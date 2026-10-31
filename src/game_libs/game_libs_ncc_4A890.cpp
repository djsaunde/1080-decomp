/* IDO 7.1 NCC (C++) donor for gl_func_0004A890 (0x160 / 88 words): EXACT, REPLACE_FUNC_BODY-spliced over the
 * game_libs_post0b.c placeholder in both build trees (agent-g run 257). `s16 *Map::build(s32 n, s16 *ids)` (renamed
 * to gl_func_0004A890 with objcopy, name only): for every id, the id then the indices of the map entries equal to it
 * (the entry count `hi - lo` through an inline `count()` re-read in the loop condition: the target re-loads
 * 0xD4 / 0xD6 only after a hit), each run closed by -1; then a heap copy of the list.
 * History (NM): agent-c run 160 real C++ (86.41 -> 88.70); run 179 `ids++` after the terminator store (93.47);
 * run 216 the loop walks the COPY `s`, the end pointer from the parameter (the parameter's live range takes s0
 * from entry: 94.73); agent-g run 252 spelling / zero-use levers (95.91).
 * agent-g run 257 (95.91 -> 100):
 *  - the byte size is the PARAMETER n reused (`n = t0 * 2 + 0 * (int)(n)`, n is dead after the end pointer):
 *    uopt colours n's webs together, and with the entry web live across nothing the size web takes s1 like the
 *    target (a separate `sz`, named / volatile / function-scope, always spilled); the target's function-scope
 *    home for it stays as an unused `volatile s32 sz;` (frame 0x2A0; without it 0x298);
 *  - the end pointer as integer arithmetic `(s16 *)(n * 2 + (int)ids)` gives the target's `addu t1,t6,s0`
 *    operand order (every pointer spelling -- ids + n, n + ids, &n[ids], += -- emits s0 first);
 *  - the first store is the plain `*s` again (the folded second read only helped the spilled-size shape).
 * Calls go through gl_func_00000000 (the target's jal fields are 0); D_00000000 + 0x20058 is the target's
 * absolute lui 0x2 / addiu 0x58 pair. No rodata.
 */
typedef short s16; typedef int s32;
extern "C" {
extern char D_00000000[];
void *gl_func_00000000(...);
}
struct Map {
    char pad[0xD0]; s16 *tbl; s16 lo; s16 hi;
    int count() { return hi - lo; }
    s16 *build(s32 n, s16 *ids);
};
s16 *Map::build(s32 n, s16 *ids) {
    s16 list[300];
    s16 t0 = 0;
    s16 *s; /* agent-g run 195 (hoist sweep): function scope */
    if (n > 0) {
        s = ids;
        s16 *end = (s16 *)(n * 2 + (int)ids);
        do {
        list[t0++] = *s;
        for (int idx = 0; idx < count(); idx++) {
            if (tbl[idx] == *s) { /* agent-g run 252: operand order (spelling grid) */
                list[t0++] = idx;
            }
        }
        list[t0++] = -1;
        s++;
        } while (s != end);
    }
    list[t0++] = -1;
    if (t0 >= 299) {
        gl_func_00000000(D_00000000 + 0x20058);
    }
    volatile s32 sz;
    n = t0 * 2 + 0 * (int)(n);
    s16 *r = (s16 *)gl_func_00000000(n);
    gl_func_00000000(r, list, n);
    return r;
}
