/* IDO 7.1 NCC (C++) EXACT donor for gl_func_000500EC (0x2B8 / 174 words), agent-c run 175, spliced over an empty
 * placeholder in game_libs_post0b.c in both build trees (Makefile renames: paint__4MeshFUi, and the out-of-line
 * adjacency test adj__4MeshFiT1 -> gl_func_00000000 = the target's baked jal 0).
 * `int Mesh::paint(unsigned t)`: the cfe body (91.66: goto layout, two constant-6 carriers, hand-spelt *6) is a
 * plain C++ graph-colouring step -- for every other triangle i sharing a vertex position with triangle t (S3
 * inline `int operator==` = the target's xor / sltiu results and the this-pointer copies), if adj(i, t): give i a
 * colour from the wrapping counter (D_500EC_cnt, pinned 0) when it has none, check no adjacent triangle shares it,
 * and copy it to t. Load-bearing: the candidate written on the LEFT (`pos[tris[i].i[n]] == pos[tris[t].i[k]]`),
 * `col[j] == col[i]` in the conflict scan. */
extern "C" int D_500EC_cnt;
struct S3 {
    short x, y, z;
    int operator==(const S3 &o) const { return x == o.x && y == o.y && z == o.z; }
};
struct Tri { short f0; unsigned short i[3]; };
struct Mesh {
    char pad[0x44];
    unsigned ntri;
    char pad48[0x18];
    S3 *pos;
    int pad64;
    Tri *tris;
    char pad6C[0xC];
    unsigned char *col;
    int adj(int a, int b);
    int paint(unsigned t);
};
int Mesh::paint(unsigned t) {
    for (unsigned i = 0; i < ntri; i++) {
        if (i == t) continue;
        for (int k = 0; k < 3; k++) {
            if (pos[tris[i].i[0]] == pos[tris[t].i[k]] || pos[tris[i].i[1]] == pos[tris[t].i[k]] || pos[tris[i].i[2]] == pos[tris[t].i[k]]) {
                if (adj(i, t)) {
                    int ok = 1;
                    if (col[i] == 0) {
                        col[i] = D_500EC_cnt;
                        if (++D_500EC_cnt >= 13) D_500EC_cnt = 1;
                    }
                    for (unsigned j = 0; j < ntri; j++) {
                        if (col[j] == col[i] && adj(j, i) == 0) {
                            ok = 0;
                            break;
                        }
                    }
                    if (ok) {
                        col[t] = col[i];
                        return 1;
                    }
                }
                break;
            }
        }
    }
    return 0;
}
