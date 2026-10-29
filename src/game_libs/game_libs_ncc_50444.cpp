/* IDO 7.1 NCC (C++) EXACT donor for gl_func_00050444 (0x1E8 / 122 words), REPLACE_FUNC_BODY-spliced over the empty
 * placeholder in game_libs_post0b.c in both build trees (symbol `intern` renamed in the Makefile).
 * Vertex intern: truncate the position (3 floats) and uv (2) to s16 keys, scan the mesh's position / uv tables
 * from `start` for an equal pair (S3 / S2 inline `operator==` returning int = the target's xor / sltiu results),
 * else append both (member-wise inline operator=) and bump the count, warning at max. The position key is built
 * by an inline S3(float, float, float) ctor (all three loads first, like the target), the uv key member-wise;
 * `int start` (signed) gives the loop counter straight in a0. agent-c run 175: cfe 86.72 -> NCC 91.83, length = target.
 * agent-c run 209 (kit CXCIV lever b, then a spelling grid): 91.94 -> 100.
 * - a folded `+ 0 * (int)(pos)` on the final return is zero code, but counts as a use of pos for uopt: pos then
 *   keeps its memory home (`sw a2,0x38(sp)`, base reloaded into t7) as in the target, re-phasing the t-ring (98.30);
 * - operator== written member-of-the-argument first (`o.x == x && ...`): the target loads the key from its home
 *   before the table element (98.73);
 * - the count bump `(m->n += 1) >= max` instead of `++m->n >= max`: the element pointer and the count take v0 and
 *   the == results v1, as in the target (100). */
extern "C" {
void gl_func_00000000(...);
extern char D_00000000[];
}
struct S3 {
    short x, y, z;
    S3() {}
    S3(float a, float b, float c) { x = a; y = b; z = c; }
    void operator=(const S3 &o) { x = o.x; y = o.y; z = o.z; }
    int operator==(const S3 &o) const { return o.x == x && o.y == y && o.z == z; }
};
struct S2 {
    short s, t;
    S2() {}
    S2(float a, float b) { s = a; t = b; }
    void operator=(const S2 &o) { s = o.s; t = o.t; }
    int operator==(const S2 &o) const { return o.s == s && o.t == t; }
};
struct Mesh { char pad[0x40]; unsigned n; char pad44[0x1C]; S3 *pos; S2 *uv; };
extern "C" unsigned intern(int start, Mesh *m, float *pos, float *uv, unsigned max) {
    if (m->uv == 0) gl_func_00000000(D_00000000 + 0x20E3C);
    S3 p(pos[0], pos[1], pos[2]);
    S2 t;
    t.s = uv[0];
    t.t = uv[1];
    for (int i = start; i < m->n; i++) {
        if (p == m->pos[i]) {
            if (t == m->uv[i]) return i;
        }
    }
    m->uv[m->n] = t;
    m->pos[m->n] = p;
    if ((m->n += 1) >= max) gl_func_00000000(D_00000000 + 0x20E4C);
    return m->n - 1 + 0 * (int)(pos);
}
