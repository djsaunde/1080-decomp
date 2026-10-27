/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_0003B6A0 (0x320 / 200 words), agent-c run 160.
 * Rewritten as real C++ (was the run-134 C lift under NCC with two parameter empty tests, 72.22): `int Obj::below(int max,
 * f32 *outd, TriB **outp, f32 *pt)` (renamed to gl_func_0003B6A0 with objcopy): for every collision triangle, the 2D
 * edge-sign test on the triangle's dropped axis pair (`{a, b}` from the Tri record, the {1, 2, 0} next-vertex table) and,
 * when inside, the plane distance below the point within 50 units, collected up to max (-1 when full; the triangle count
 * re-read after each hit). Typed records instead of byte offsets, the face pick as an if / else (the target re-reads
 * col->faces in each arm), no empty tests. 72.22 -> 73.63, length 796 -> 800 = target. Residual: the target keeps the
 * triangle offset and count in frame homes (frame 0x80 vs 0x40; `ra` used as a variable register) where ours colours
 * them.
 * agent-c run 178: the triangle record through an inline `triAt(const int &off)` (a reference parameter keeps
 * off memory-homed like the target's frame home): 73.61 -> 74.51, length = target.
 * Still NON_MATCHING: build/non_matching/ only.
 * agent-c run 179: de-named `a` (t->a at each use) and the -50.0f limit constant: 74.53 -> 74.59.
 */
typedef float f32; typedef short s16; typedef unsigned short u16; typedef signed char s8;
struct TriB { s8 a; s8 b; s8 ax; s8 sgn; s16 grp; s16 pad6; f32 d; };
struct Face { u16 mat; u16 vi[3]; };
struct V3 { f32 x, y, z; };
struct Col { char pad[0x44]; unsigned int ntri; char pad48[4]; s16 *remap; char pad50[4]; V3 *nrm; char pad58[8]; s16 (*verts)[3]; char pad64[4]; Face *faces; };
extern "C" { extern s16 D_00000000_3b6a0tbl[3]; }
struct Obj {
    char pad[0x2C]; TriB *tris; char pad30[0x84 - 0x30]; Col *col;
    int below(int max, f32 *outd, TriB **outp, f32 *pt);
    TriB *triAt(const int &o) { return (TriB *)((char *)tris + o); }
};
int Obj::below(int max, f32 *outd, TriB **outp, f32 *pt) {
    int nfound = 0;
    Col *c = col;
    if (c == 0) {
        return 0;
    }
    unsigned int count = c->ntri;
    if (count != 0) {
        int off = 0;
        for (unsigned int i = 0; i < count; i++, off += 12) {
            TriB *t = triAt(off);
            int inside = 1;
            s16 (*verts)[3] = c->verts;
            Face *f;
            if (c->remap != 0) {
                f = &c->faces[c->remap[i]];
            } else {
                f = &c->faces[i];
            }
            f32 dir = t->sgn;
            int b = t->b;
            f32 qa = pt[t->a];
            f32 qb = pt[b];
            s16 *tp = D_00000000_3b6a0tbl;
            u16 *fp = (u16 *)f;
            do {
                s16 *v = verts[fp[1]];
                s16 *w = verts[f->vi[*tp++]];
                int ax = v[t->a];
                int ay = v[b];
                s16 ex = w[t->a] - ax;
                s16 ey = w[b] - ay;
                s16 px = (int)(qa - (f32)ax);
                s16 py = (int)(qb - (f32)ay);
                if (0.0f < ((f32)ex * (f32)py - (f32)ey * (f32)px) * dir) {
                    inside = 0;
                }
            } while (inside && (fp++, tp != D_00000000_3b6a0tbl + 3));
            if (inside) {
                V3 *n = (V3 *)((char *)c->nrm + off);
                f32 d = pt[0] * n->x + pt[1] * n->y + pt[2] * n->z - t->d;
                if (d < 0.0f && -50.0f < d) {
                    if (nfound >= max) {
                        return -1;
                    }
                    outd[nfound] = d;
                    outp[nfound++] = &tris[i];
                    c = col;
                    count = c->ntri;
                }
            }
        }
    }
    return nfound;
}
