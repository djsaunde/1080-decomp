/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00050444 (0x1E8 / 122 words), agent-c run 175.
 * Vertex intern: truncate the position (3 floats) and uv (2) to s16 keys, scan the mesh's position / uv tables
 * from `start` for an equal pair (S3 / S2 inline `operator==` returning int = the target's xor / sltiu results),
 * else append both (member-wise inline operator=) and bump the count, warning at max. The position key is built
 * by an inline S3(float, float, float) ctor (all three loads first, like the target), the uv key member-wise;
 * `int start` (signed) gives the loop counter straight in a0. cfe 86.72 -> 91.83 local, length = target.
 * Residual: pos stays a register candidate (target: memory home, base reloaded into t7), shifting the t-ring by
 * one, and the == temps take v0 / v1 swapped. Still NON_MATCHING: build/non_matching/ only (symbol `intern`
 * renamed in the Makefile). */
extern "C" {
void gl_func_00000000(...);
extern char D_00000000[];
}
struct S3 {
    short x, y, z;
    S3() {}
    S3(float a, float b, float c) { x = a; y = b; z = c; }
    void operator=(const S3 &o) { x = o.x; y = o.y; z = o.z; }
    int operator==(const S3 &o) const { return x == o.x && y == o.y && z == o.z; }
};
struct S2 {
    short s, t;
    S2() {}
    S2(float a, float b) { s = a; t = b; }
    void operator=(const S2 &o) { s = o.s; t = o.t; }
    int operator==(const S2 &o) const { return s == o.s && t == o.t; }
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
    if (++m->n >= max) gl_func_00000000(D_00000000 + 0x20E4C);
    return m->n - 1;
}
