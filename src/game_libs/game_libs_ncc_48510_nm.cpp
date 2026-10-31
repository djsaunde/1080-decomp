/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00048510 (0x210 / 132 words), agent-c run 160.
 * Rewritten as real C++ (was the run-134 C lift under NCC, 91.11): `void Strip::add(Vtx *v, s16 *out, int *full,
 * u16 *id)` (renamed to gl_func_00048510 with objcopy). Strip: mesh at 0, vertex / triangle counts at 4 / 8, 200
 * 3-index triangles at 0x14, vertex ids at 0x4C4, limits at 0x590 / 0x594 and its vptr at 0x598 (a class that introduces
 * virtuals puts the vptr after its own data: the cfront `flush()` call through vtbl+8). Per vertex: look up an existing
 * index (`i == 0 || n594 == 2` as the flag), else flush when full (and restart at i = 0), append the vertex to the
 * mesh's pool through a named `Mesh *m` (the target holds the mesh in v0 for both vp reads) and record its id; then
 * copy the indices out and into the next triangle (a separate loop counter k: the target's v1 / a0 / v0 IVs).
 * 91.11 -> 97.21, length = target, frame 0x78 = target. Residual: t-register colours, idx[] at sp+0x68 (target 0x6C).
 * The out[] halfword store carries a folded `& 0xFFFF` (one ugen ring slot): 97.21 -> 97.29.
 * agent-c run 161: `int idx[3]` (the target array ends at the frame top, 0x6C) + a volatile pad for the 0x78 frame,
 * the count read and the append through inline `Mesh::count()` / `Mesh::next()` with a `Vtx::set(const Vtx &)` copy
 * (the pooled pointer temp a1 holds the destination like the target), and `nt = nt + 1; if (nt >= 200)` (store-forwarded
 * ring temps instead of a coloured ++nt): 97.29 -> 98.71. Residual: ugen operand order of `nv >= max590 - 1` and
 * `&buf[n]` (t6 / t7 / t8 permuted) and one ring slot from there on.
 * Still NON_MATCHING: build/non_matching/ only. */
typedef short s16; typedef unsigned short u16; typedef int s32;
extern "C" {
int gl_func_00034458(...);
extern char D_00000000[];
}
struct Vtx { int w[4]; void set(const Vtx &o) { *this = o; } };
struct VPool { Vtx *buf; int count; };
struct Mesh { char pad[0x1C]; VPool *vp; int count() { return vp->count; } Vtx *next() { return &vp->buf[vp->count++]; } };
struct Tri { s16 v[3]; };
struct Strip {
    Mesh *mesh;
    int nv;
    int nt;
    char padc[0x14 - 0xC];
    Tri tris[200];
    u16 ids[0x590 / 2 - 0x4C4 / 2];
    int max590;
    int n594;
    virtual void flush();
    void add(Vtx *v, s16 *out, int *full, u16 *id);
};
void Strip::add(Vtx *v, s16 *out, int *full, u16 *id) {
    int idx[3]; volatile int pad;
    int i;
    *full = 0;
    for (i = 0; i < n594; i++) {
        idx[i] = gl_func_00034458(this, i == 0 || n594 == 2, &v[i]);
        if (idx[i] < 0) {
            if (nv + 1 >= max590) { /* agent-g run 258: compare spelling (max590 loaded first like the target): 98.71 -> 98.73 */
                flush();
                gl_func_00034458(this);
                i = 0;
            }
            if (i == 0) {
                *full = 1;
            }
            idx[i] = mesh->count();
            mesh->next()->set(v[i]);
            ids[nv] = id[i];
            nv++;
        }
    }
    for (int k = 0; k < n594; k++) {
        out[k] = idx[k] & 0xFFFF;
        tris[nt].v[k] = idx[k];
    }
    nt = nt + 1; if (nt >= 200) {
        gl_func_00034458(D_00000000 + 0x1FF44);
    }
}
