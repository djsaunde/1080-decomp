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
    s16 *v0; /* agent-c run 285: v0 takes the dead-stored opad's word and ey shares ex's word: the declared-word
              * count below off falls to the target's (zdbug:1 lowest M -124 -> -116), frame 240 -> 232 (96.52 -> 96.65) */
    ColB9 *col;
    FaceB9 *f;
    V3B9 *n;
    f32 dn;
    f32 t;
    s32 ok;
    V3B9 p;
    s32 ib;
    s32 iav; /* agent-c run 387: ia named again (one tris[i].a load, the target's v1 role) */
    f32 num; /* agent-c run 286: the t numerator (was nz, agent-g run 248: function scope = one more edg home word between p and d -> d at sp+160 like the target, 95.14 -> 95.19) */
    V3B9 d;
    f32 *pp; /* run 194: the target indexes a held &p (addiu sp,184; addu idx,base) */
    s16 ey;
    volatile s32 vpad; /* agent-c run 387: unused volatile pad -- the home word the dropped sg / qa / qb names held (frame 224 -> 232 = target) */
    s16 ex; /* run 191 lever-sweep hoist: ex declared before vt (78.55 -> 78.65) */
    s16 (*vt)[3];
    s16 *nx;
    u16 *fv;
    s32 off;
    s16 *v1;

    cnt = 0;
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
        if (dn > -0.0001) { /* agent-g run 327: agent-c run 284's folded n->z use dropped (n->z back at 15, coloured after n->x / n->y: f20 like the target) */
            continue;
        }
        ok = 1; /* agent-c run 180 (lever-sweep bbsplit) */
        /* agent-c run 286: the target keeps the numerator before the negation in its own LR (sub.s f2 / neg.s / div.s);
         * a named num with n->z read inline (run 235's named nz dropped: its copy-propagated variable was a phantom
         * float LR across the loop that kept the 0.0f constant off f14) re-aligns the t / p block: 97.13 -> 98.94;
         * the d subtraction outside num: frame 240 -> 232 (target), 98.94 -> 99.00 */
        /* agent-g run 343 (kit CDXXXII term-order lever, kit CDV / CDVI num form): d subtracted INSIDE num (`sub.s f2`
         * = num's own LR, then neg / div: every FP register of the target), with a code-free `if (1) {}` before the
         * remap test (BB split: the remap block's shape and length back to the target's) and dx / dy carried in
         * self_al / pp below (no s16 homes: frame 240 -> 232 = target): 99.211 -> 99.319 */
        num = a->x * n->x + a->y * n->y + a->z * n->z - self->tris[i].d; t = -num / dn;
        p.x = a->x + d.x * t; /* agent-c run 235 (lever-sweep bbsplit, 94.90 -> 95.00) */
        p.y = a->y + d.y * t;
        p.z = a->z + d.z * t;
        unused = (int)self->col; col = self->col; /* agent-c run 287: run 235's `if (1) {}` here and its + n use below dropped (stale, removal sweep flat at 99.07);
                                                   * the self->col value copied into the unused parameter (kit CCCXLVII (b): fresh web, no home) and folded on
                                                   * the nx line in col's place (kit CCCXLVIII phantom): col's own LR now ends at its last real use, before
                                                   * &tris[i], and takes v0 like the target (99.07 -> 99.12) */
        if (1) {} if (col->remap != 0) {
            f = &col->faces[col->remap[i]];
        } else {
            f = &col->faces[i];
        }
        iav = (&self->tris[i])->a;
        ib = (&self->tris[i])->b + 0 * (int)(self_al->tris) + 0 * (int)(col->remap); /* agent-g run 210: second pass: 92.70 -> 93.89; agent-c run 284: + col->remap (zero-use --exprs re-sweep after the off split, 96.00 -> 96.05) */
            if (0) { f = 0; } /* agent-c run 285: dead multi-def of f (no code; insertion sweep): the i * 12 base a1 like the target, 96.65 -> 96.77 */
#define ia iav /* agent-c run 235: ia de-named (95.00 -> 95.07); agent-c run 387: named again with the qa / qb change below */
        /* agent-c run 266: qa / qb loads in ONE basic block (the `if (1) {}` moved after qb): the target's held
         * `addiu t9,sp,184` &p base is CSE'd for both loads inside the iteration (1104 -> 1108 of 1116); folded
         * counted uses of ib / ia on pp and of pp on sg re-colour it (zero-use sweep: 94.19 -> 95.52) */
        pp = &p.x + 0 * (int)(ib); /* agent-c run 287: the + ia fold dropped (stale after the col phantom, removal sweep flat) */
        /* agent-c run 387 (re-decode, the exact 3BE1C sibling's q[a] form): no qa / qb / sg variables -- p[ia] / p[ib]
         * and the sgn read are written IN the edge loop and uopt hoists them to the preheader; the &p lda is then an
         * uncoloured ugen base (`addiu t9,sp,184` reused for both loads, kits CDLXVIII 4. / CDLXXII said unreachable with
         * named qa / qb) and the sgn load schedules between them like the target; the old sg line's slot keeps a
         * code-free `if (pp) {}`: 99.319 -> 99.660, same length */
        if (1) {}
        if (pp) {}
        nx = (s16 *)&D_00000000 + 0 * (unused + unused); /* agent-c run 287: the phantom pair in col's place; agent-c run 286: folded col use (zero-use sweep after the vt change, 95.30 -> 99.01) */
        vt = self->col->verts; /* agent-c run 286: the target loads verts from the self->col web (lw t5,96(s5)), not from col */
        fv = (u16 *)f;
        do {
            v0 = vt[fv[1]]; /* agent-c run 273: a code-free 0.0f use inside the inner loop (qa set before the loop, the test after the dx line) raises
                             * the hoisted 0.0f constant's priority over the n->x / n->y / n->z loads: f14 like the target (95.52 -> 95.72) */
            v1 = vt[f->vi[*nx]]; nx++; /* run 194: no `*nx++` copy (the target steps nx in place; loop block aligns up to colours) */
            /* agent-g run 343 (kit CDVI carriers): dx in self_al (block local, dead after the grp test / ib fold), dy in
             * pp (dead after the qa / qb / sg reads above), both re-written here before every read: value-preserving s16
             * truncation; dx in b first (99.247: b's top LR then shared the loop colour t0, target v0) -> self_al 99.319 */
            self_al = (ObjB9 *)(int)(s16)(s32)((&p.x)[ia] - v0[ia]); /* run 193: dx / dy before ex / ey (3BE1C twin decode) */
            pp = (f32 *)(int)(s16)(s32)((&p.x)[ib] - v0[ib]);
            ex = v1[ia] - v0[ia];
            ey = v1[ib] - v0[ib];
            if (((f32)ex * (f32)(int)pp - (f32)ey * (f32)(int)self_al) * (f32)(&self->tris[i])->sgn > 0.0f) {
                ok = 0 + 0 * (int)(i) + 0 * ((int)(f) + (int)(f)); if (p.x != 0.0f) {} /* agent-c run 387: the 0.0f test on p.x (qa is gone) */ /* agent-g run 327: the + num fold dropped (num no longer spans
                                                                     * the inner loop) and a code-free 0.0f test here (raises the hoisted 0.0f over n->x / n->y / n->z):
                                                                     * 0.0f f14 / n->z f20 like the target, 99.122 -> 99.211 (grid r8, 156); agent-c run 286: the run-285 nz use dropped with nz; + num (set before the loop on every path; zero-use --keep-float, 99.01 -> 99.07); agent-c run 286: two folded f uses in the inner-loop arm (a hoisted loop-invariant use counts outer-loop weight only) raise f's adjsave 32.5 -> 82.5, between vt 73.3 and the 100 group: f t4 / vt t5 / ia, ib ra, s0 like the target, 96.83 -> 97.13; agent-c run 285: + nz (zero-use --keep-float, set before the loop on every path, 96.77 -> 96.83); agent-g run 210: third pass: 93.89 -> 93.96; run 231: + self, 93.96 -> 94.27; agent-c run 284: + self dropped (stale after the off split, rm2 removal sweep 96.11 -> 96.52) */
            }
        } while (ok && (fv++, nx != (s16 *)((char *)&D_3B9C0_c + 6)));
        if (ok) { /* agent-c run 287: agent-g run 258's folded vt use dropped (stale) */
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
