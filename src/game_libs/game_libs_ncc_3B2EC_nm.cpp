/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_0003B2EC (0x3B4 / 237 words), agent-c run 177.
 * Real C++ `int Obj::height(int max, f32 *out, f32 x, f32 z)` (ground heights under (x, z)), not a C lift:
 * the cell lookup through inline Obj members toCell(const V3 &, int &, int &) / cellOf() / list() (reference
 * parameters = the target's memory p and cx / cz homes and the self->col reloads), Col::face(i) with the remap
 * pointer in a local (one remap load, the `this` copy), the face before verts, the edge walk as
 * `for (k = 0; k < 3; k++)` over the next-vertex table (LFTR'd to the target's pointer walk), ten unused
 * volatiles for the 0x98 frame. cfe body 91.56 -> 95.57 (952 B vs target 948, cfe 936).
 * Residual: the post-loop `k != 3` end constant is a register copy of the loop end (s6) where the target
 * rematerialises it; that s-register shifts the saves / cx / cz homes. Still NON_MATCHING. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef float f32;
extern "C" {
extern char D_00000000[];
void gl_func_00000000(...);
extern s16 D_3B2EC_a[], D_3B2EC_b[], D_3B2EC_c[];
}
#define K(o) (*(f32 *)(D_00000000 + (o)))
struct V3 { f32 x, y, z; };
struct Tri { s8 a, b, ax, sgn; s16 grp, pad6; f32 d; };
struct Face { u16 mat; u16 vi[3]; };
struct Grid {
    int w, h, x0, z0; f32 cw, ch; s16 *lists; int *cells;
};
struct Col {
    char pad0[0x4C]; s16 *remap; char pad50[4]; V3 *nrm; char pad58[8]; s16 (*verts)[3];
    char pad64[4]; Face *faces; char pad6c[0x34]; Grid *grid;
    Face *face(int i) { s16 *r = remap; return r ? &faces[r[i]] : &faces[i]; }
};
struct Obj {
    char pad0[0x2C]; Tri *tris; char pad30[0x54]; Col *col;
    int height(int max, f32 *out, f32 x, f32 z);
    void toCell(const V3 &p, int &cx, int &cz) {
        cx = (p.x - col->grid->x0) / col->grid->cw;
        cz = (p.z - col->grid->z0) / col->grid->ch;
    }
    int cellOf(const V3 &p) {
        int cx, cz;
        toCell(p, cx, cz);
        if (cx >= 0 && cx < col->grid->w && cz >= 0 && cz < col->grid->h) return cz * col->grid->w + cx;
        return -1;
    }
    s16 *list(int cell) { return col->grid->cells[cell] != -1 ? &col->grid->lists[col->grid->cells[cell]] : 0; }
};

int Obj::height(int max, f32 *out, f32 x, f32 z) {
    V3 p;
    volatile int v0, v1, v2, v3, v4, v5, v6, v7, v8, v9;
    int cnt;
    int cell;
    int cx, cz;
    s16 *list;
    int i;
    x *= K(0x128);
    z *= K(0x130);
    cnt = 0;
    if (col == 0) return 0;
    if (col->grid == 0) gl_func_00000000(D_00000000 + 0x1EE8C);
    p.x = x; p.z = z; p.y = 0.0f;
    cell = cellOf(p);
    if (cell < 0) return 0;
    list = this->list(cell);
    if (list == 0) return 0;
    while ((i = *list++) != -1) {
        int off = ((i << 2) - i) << 2;
        V3 *n = (V3 *)((char *)col->nrm + off);
        if (!(n->y > 0.0f)) continue;
        Face *f = col->face(i);
        s16 (*vt)[3] = col->verts;
        int k;
        for (k = 0; k < 3; k++) {
            s16 *v0 = vt[f->vi[k]];
            s16 *v1 = vt[f->vi[D_3B2EC_c[k]]];
            s16 ex = v1[0] - v0[0];
            s16 ez = v1[2] - v0[2];
            s16 dx = (int)(x - v0[0]);
            s16 dz = (int)(z - v0[2]);
            if (dx * ez - dz * ex < -1000) break;
        }
        if (k != 3) continue;
        f32 h = (n->x * x + n->z * z - ((Tri *)((char *)tris + off))->d) / n->y;
        if (cnt >= max) return -1;
        out[cnt++] = -h / K(0x12C);
    }
    return cnt;
}
