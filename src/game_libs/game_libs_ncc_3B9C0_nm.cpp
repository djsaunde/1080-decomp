/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_0003B9C0: the NM C body lifted under NCC + one parameter empty test from a fz sweep (agent-c run 134, cfe 72.25; a second parameter empty test from round 2 gives 75.08).
 * run 152 (agent-c, 75.15 -> 76.55): `while (ok && (fv++, nx != end))` puts fv++ in the bne delay slot after the ok exit as in the target (no bnel back edge).
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY.
 * agent-c run 179: the next-vertex table pointer set before the verts load (adjacent-statement swap sweep): 76.54 -> 76.72.
 * agent-c run 284: the target's b / a / sgn reads use the i * 12 base it keeps in a1 for tris[i].d (off is spilled to
 * sp+128 at the body top and reloaded for the += 12), so they are written as tris[i] (verified decode fix); with the
 * inner-loop `if (self) {}` / `if (qa != 0.0f) {}` and the ok / p.x `if (1) {}` splits dropped, off's priority falls below
 * every callee-saved candidate and uopt splits it like the target (a1 header piece + home spill): 1108 -> 1116 bytes =
 * target, 95.72 -> 96.00. off is declared after fv (home = frame - 104, the target's slot) with a dead-stored opad in
 * its old place keeping the d / p homes. LEFT: frame 240 (target 232: 8 bytes of hidden edg temp area), colours.
 */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
#undef NULL
#define NULL 0
typedef struct { f32 x, y, z; } V3B9;
typedef struct { s8 a; s8 b; s8 ax; s8 sgn; s16 grp; s16 pad6; f32 d; } TriB9;
typedef struct { u16 mat; u16 vi[3]; } FaceB9;
typedef struct {
    char pad0[0x44]; u32 ntri; char pad48[4]; s16 *remap; char pad50[4]; V3B9 *nrm; char pad58[8];
    s16 (*verts)[3]; char pad64[4]; FaceB9 *faces;
} ColB9;
typedef struct { char pad0[0x2C]; TriB9 *tris; char pad30[0x54]; ColB9 *col; } ObjB9;
extern "C" {
extern int D_00000000;
extern signed char b;
extern char *p;
extern void gl_func_00034458_36F0C(f32, f32 *, f32 *);
extern float sqrtf(float);
extern int D_3B9C0_a, D_3B9C0_b, D_3B9C0_c;
extern void gl_func_00000000_vnorm3(V3B9 *);
}
inline void zf(f32 &f, int z) { f = z; }
extern "C" s32 gl_func_0003B9C0(ObjB9 *self, s32 max, f32 *dist, TriB9 **out, V3B9 *a, V3B9 *b, s32 unused, V3B9 *pts) {
    s32 cnt;
    u32 i;
    s32 opad;
    ColB9 *col;
    FaceB9 *f;
    V3B9 *n;
    f32 dn;
    f32 t;
    s32 ok;
    V3B9 p;
    s32 ib;
    f32 qa;
    f32 nz; /* agent-g run 248 (frame scan): function scope = one more edg home word between p and d -> d at sp+160 like the target (95.14 -> 95.19) */
    V3B9 d;
    f32 qb;
    f32 sg;
    f32 *pp; /* run 194: the target indexes a held &p (addiu sp,184; addu idx,base) */
    s16 ex; /* run 191 lever-sweep hoist: ex declared before vt (78.55 -> 78.65) */
    s16 (*vt)[3];
    s16 *nx;
    u16 *fv;
    s32 off;
    s16 *v0;
    s16 *v1;
    s16 ey;
    s16 dx;
    s16 dy;

    cnt = 0;
    opad = 0;
    if (self->col == 0) {
        return 0;
    }
    d.x = b->x - a->x;
    d.y = b->y - a->y;
    d.z = b->z - a->z;
    if (sqrtf(d.x * d.x + d.y * d.y + d.z * d.z) > 0.00001) {
        gl_func_00000000_vnorm3(&d);
    } else {
        zf(d.x, 0); /* run 190: zeros through an int inline parameter (77.78 -> 78.55); run 193: x first = target store order */
        zf(d.z, 0);
        d.y = -1.0f;
    }
    for (i = 0, off = 0; i < self->col->ntri; i++, off += 12) {
        n = (V3B9 *)((char *)self->col->nrm + off); /* run 194: normal pointer before the group test (target lw 84(s5) ahead of the bltzl) */
        ObjB9 *self_al = self;
        if (((TriB9 *)((char *)self_al->tris + off))->grp < 0) {
            continue;
        }
        dn = n->x * d.x + n->y * d.y + n->z * d.z;
        if (dn > -0.0001) {
            continue;
        }
        ok = 1; /* agent-c run 180 (lever-sweep bbsplit) */
        nz = n->z; /* agent-c run 235: n->z named for the t sum (fs reg like the target: fs2 save, 94.27 -> 94.90, 1096 -> 1104 of 1116) */
        t = -(a->x * n->x + a->y * n->y + a->z * nz - self->tris[i].d) / dn;
        p.x = a->x + d.x * t; /* agent-c run 235 (lever-sweep bbsplit, 94.90 -> 95.00) */
        p.y = a->y + d.y * t;
        p.z = a->z + d.z * t;
        col = self->col; if (1) {} /* agent-c run 235 (lever-sweep bbsplit, 95.06 -> 95.08) */
        if (col->remap != 0) {
            f = &col->faces[col->remap[i]] + 0 * (int)(n); /* agent-c run 235: folded counted use (zero-use-sweep, 95.08 -> 95.14) */
        } else {
            f = &col->faces[i];
        }
        ib = (&self->tris[i])->b + 0 * (int)(self_al->tris) + 0 * (int)(col->remap); /* agent-g run 210: second pass: 92.70 -> 93.89; agent-c run 284: + col->remap (zero-use --exprs re-sweep after the off split, 96.00 -> 96.05) */
#define ia ((&self->tris[i])->a) /* agent-c run 235: ia de-named (lever-sweep denaming, 95.00 -> 95.07) */
        /* agent-c run 266: qa / qb loads in ONE basic block (the `if (1) {}` moved after qb): the target's held
         * `addiu t9,sp,184` &p base is CSE'd for both loads inside the iteration (1104 -> 1108 of 1116); folded
         * counted uses of ib / ia on pp and of pp on sg re-colour it (zero-use sweep: 94.19 -> 95.52) */
        pp = &p.x + 0 * (int)(ib) + 0 * (int)(ia);
        qa = pp[ia]; qb = pp[ib]; if (1) {}
        sg = (&self->tris[i])->sgn + 0 * (int)(self->tris) + 0 * (int)(pp); if (1) {} /* agent-g run 210: folded counted use (int add before the s8 -> f32 conversion): 92.55 -> 92.70; agent-c run 273: the dx split moved here */
        nx = (s16 *)&D_00000000;
        vt = col->verts;
        fv = (u16 *)f;
        do {
            v0 = vt[fv[1]]; /* agent-c run 273: a code-free 0.0f use inside the inner loop (qa set before the loop, the test after the dx line) raises
                             * the hoisted 0.0f constant's priority over the n->x / n->y / n->z loads: f14 like the target (95.52 -> 95.72) */
            v1 = vt[f->vi[*nx]]; nx++; /* run 194: no `*nx++` copy (the target steps nx in place; loop block aligns up to colours) */
            dx = (s32)(qa - v0[ia]); /* run 193: dx / dy before ex / ey (3BE1C twin decode) */
            dy = (s32)(qb - v0[ib]);
            ex = v1[ia] - v0[ia];
            ey = v1[ib] - v0[ib];
            if (((f32)ex * (f32)dy - (f32)ey * (f32)dx) * sg > 0.0f) {
                ok = 0 + 0 * (int)(i) + 0 * (int)(self); /* agent-g run 210: third pass: 93.89 -> 93.96; run 231: + self, 93.96 -> 94.27 */
            }
        } while (ok && (fv++, nx != (s16 *)((char *)&D_3B9C0_c + 6)));
        if (ok + 0 * (int)(vt)) { /* agent-g run 258: folded counted use of vt (zero-use re-sweep): 95.19 -> 95.24 */
            dist[cnt] = t;
            out[cnt] = &self->tris[i];
            if (pts != 0) {
                pts[cnt].x = p.x;
                pts[cnt].y = p.y;
                pts[cnt].z = p.z;
            }
            cnt++;
            if (cnt >= max) {
                return cnt;
            }
        }
    }
    return cnt;
}
