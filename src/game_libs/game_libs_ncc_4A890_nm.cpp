/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_0004A890 (0x160 / 88 words), agent-c run 160.
 * Rewritten as real C++ (was the run-134 C lift under NCC, 86.41): `s16 *Map::build(s32 n, s16 *ids)` (renamed to
 * gl_func_0004A890 with objcopy): for every id, the id then the indices of the map entries equal to it (the entry count
 * `hi - lo` through an inline `count()` re-read in the loop condition: the target re-loads 0xD4 / 0xD6 only after a
 * hit), each run closed by -1; the end pointer from a copy of the id cursor; then a heap copy of the list.
 * 86.41 -> 88.70 (length unchanged, 344 vs 352). Residual: the target holds the id copy and the byte size in s0 / s1
 * across the calls (ours: one s-register, the size spilled).
 * Still NON_MATCHING: build/non_matching/ only.
 * agent-c run 179: the byte size `volatile s32 sz` (the target keeps it in s1; ours spills it either way, the volatile reload lines up the call tail): 88.70 -> 91.0 local.
 * agent-c run 179: `ids++` after the run terminator store (adjacent-statement swap sweep): 91.19 -> 93.47.
 * agent-c run 216: the loop walks the COPY `s` and the end pointer comes from the parameter (`end = ids + n`):
 * the parameter's live range then takes s0 from entry (the target's leading `or s0,a2,zero`, s0 reused for
 * the list base after the loop): 93.47 -> 94.73. Left: the byte size in s1 (ours spills it) + t-reg rotation.
 */
typedef short s16; typedef int s32;
extern "C" {
extern char D_00000000[];
void *gl_func_00034458(...);
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
        s16 *end = ids + n;
        do {
        list[t0++] = *s;
        for (int idx = 0; idx < count(); idx++) {
            if (*s == tbl[idx]) {
                list[t0++] = idx;
            }
        }
        list[t0++] = -1;
        s++;
        } while (s != end);
    }
    list[t0++] = -1;
    if (t0 >= 299) {
        gl_func_00034458(D_00000000 + 0x20058);
    }
    volatile s32 sz = t0 * 2 + 0 * (int)(n); /* agent-g run 252: folded counted use of n (zero-use sweep): 94.73 -> 94.77, 348 -> 352 bytes = target */
    s16 *r = (s16 *)gl_func_00034458(sz);
    gl_func_00034458(r, list, sz);
    return r;
}
