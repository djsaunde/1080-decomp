/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_0003BE1C (0x620 / 392 words), agent-c run 178.
 * Real C++ `int Obj::query(int max, f32 *dist, Tri **out, V3 p)` (vertical ray / ground query, sibling of
 * 3B2EC Obj::height and 3B9C0), ported from the run-110..152 cfe body (77.27): the grid lookup through the
 * same inline Obj members toCell(const V3 &, int &, int &) / cellOf() / list() as 3B2EC (the cell-index
 * homes and per-statement self->col reloads come for free), the K triple through its own symbol
 * D_3BE1C_128 (held base, as the target), off computed BEFORE the grp test, the material callback through a
 * `Col *cc` copy of self->col (off then spills around the call like the target), the 0x100 test as a nested
 * block instead of `continue` (no duplicated loop test), t->a / t->b read before the 0x200 branch, and the
 * edge walk as the cfe do-while with `fv++` inside the condition and the table read `*nx` then `nx++` as
 * its own statement (`*nx++` reads through a copy of the old pointer that takes an s-register); self->col
 * read once into a function-scope `Col *cc` (the target's s1) and re-read into it per triangle; the
 * single-use locals ia / ib / ic, vt, qa / qb and sg DE-NAMED (macros: each named scalar costs an NCC home
 * and a colour; de-naming them took the frame to the target's 0xF0 with s0..s7 only, 83.38 -> 87.55).
 * The face() argument written as an rvalue (`i + 0`; `i | 0` is the same, a plain `i` or a named copy is
 * not): edg then passes it through a parameter temp and the expansion's `this` copy lands in the target's v0
 * (87.55 -> 88.59). cfe 77.27 -> 88.59 local, 1560 bytes (target 1568, cfe 1528). Residual: the face pointer
 * is not copied into s0 after the face() expansion, off spills to sp+76 (target sp+84), r is not copied into a0, the first
 * inner MAX is evaluated as two arms (the target pre-assigns one before the branch) and the edge-loop
 * schedule. agent-c run 193 (decode): the edge loop computes dx / dy (qa / qb minus the first vertex) BEFORE
 * ex / ey: the target loads v0[a] and v0[b] together and converts both before the v1[] reads; the loop block
 * now aligns word-for-word (up to colours): 88.59 -> 94.9 local, 1560 -> 1552 bytes (target 1568: away,
 * flagged; the deficit is the missing face / r copies and the two-arm inner MAX listed above).
 * agent-c run 194: face() called on the member `col` (plain `i`), `cc = col` after it: 94.93 -> 95.40 local,
 * 1552 -> 1556 bytes (toward 1568).
 * Still NON_MATCHING. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef float f32;
extern "C" {
extern char D_00000000[];
int gl_func_00000000(...);
extern s16 D_3BE1C_c[];
extern f32 D_3BE1C_128[];
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
#define MAXS(a, b) ((a) > (b) ? (s16)(a) : (s16)(b))
#define MINS(a, b) ((a) < (b) ? (s16)(a) : (s16)(b))
struct Obj {
    char pad0[0x2C]; Tri *tris; char pad30[0x54]; Col *col;
    int query(int max, f32 *dist, Tri **out, V3 p);
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

int Obj::query(int max, f32 *dist, Tri **out, V3 p) {
    Col *cc;
    int cnt;
    int cell;
    s16 *list;
    int i;
    cc = col;
    cnt = 0;
    if (cc == 0) return 0;
    if (cc->grid == 0) gl_func_00000000(D_00000000 + 0x1EE9C);
    p.x *= D_3BE1C_128[0];
    p.y *= D_3BE1C_128[1];
    p.z *= D_3BE1C_128[2];
    cell = cellOf(p);
    if (cell < 0) return 0;
    list = this->list(cell);
    if (list == 0) return 0;
    while ((i = *list++) != -1) {
        int off = ((i << 2) - i) << 2;
        if (((Tri *)((char *)tris + off))->grp < 0) continue;
        Face *f = col->face(i) + 0 * (int)(*list); /* agent-g run 210: fourth pass (list points at the next entry or the -1 terminator): 97.26 -> 97.33, 1564 -> 1568 bytes = target */
        cc = col;
        int r = gl_func_00000000(cc, f->mat) + 0 * (int)(cc); /* agent-g run 210: folded counted use (zero-use sweep): 95.48 -> 96.38, 1556 -> 1560 bytes (target 1568) */
        if (r & 0x100) {
            int ok = 1;
            V3 *n = (V3 *)((char *)col->nrm + off);
            Tri *t = (Tri *)((char *)tris + off);
            V3 *n_al = n; /* agent-c run 195: alias copy (colour tie) */
            f32 d = p.x * n_al->x + p.y * n_al->y + p.z * n_al->z - t->d;
            f32 q[3];
            q[0] = p.x - n_al->x * d;
            q[1] = p.y - n_al->y * d;
            q[2] = p.z - n->z * d;
            t = (Tri *)((char *)tris + off);
            int a = t->a;
            int b = t->b;
            if (r & 0x200) {
                s16 *va = col->verts[f->vi[0]];
                s16 *vb = col->verts[f->vi[1]];
                s16 *vc = col->verts[f->vi[2]];
                int k = 2 - t->ax;
#define ia va[k]
#define ib vb[k]
#define ic vc[k]
                s16 hi = MAXS(MAXS(ia, ib), ic) + 0 * (int)(vb); /* agent-g run 210: third pass: 97.05 -> 97.26 */
                s16 lo = MINS(MINS(ia, ib), ic);
                if (q[k] < (f32)lo || (f32)hi < q[k]) ok = 0 + 0 * (int)(va); /* agent-g run 210: second pass: 96.38 -> 97.05, 1560 -> 1564 bytes (target 1568) */
            } else {
#define qa q[a]
#define qb q[b]
#define sg ((f32)t->sgn)
#define vt (col->verts)
                s16 *nx = D_3BE1C_c;
                u16 *fv = (u16 *)f;
                do {
                    s16 *v0 = vt[fv[1]];
                    s16 *v1 = vt[f->vi[*nx]];
                    nx++;
                    s16 dx = (int)(qa - v0[a]);
                    s16 dy = (int)(qb - v0[b]);
                    s16 ex = v1[a] - v0[a];
                    s16 ey = v1[b] - v0[b];
                    if (((f32)ex * (f32)dy - (f32)ey * (f32)dx) * sg > 10.0f) ok = 0;
                } while (ok && (fv++, nx < D_3BE1C_c + 3));
            }
            if (ok) {
                dist[cnt] = d / K(0x12C);
                out[cnt] = &tris[i];
                cnt++;
                if (cnt >= max) return cnt;
            }
        }
    }
    return cnt;
}
