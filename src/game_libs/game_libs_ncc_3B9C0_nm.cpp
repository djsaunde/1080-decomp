/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_0003B9C0: the NM C body lifted under NCC + one parameter empty test from a fz sweep (agent-c run 134, cfe 72.25; a second parameter empty test from round 2 gives 75.08).
 * run 152 (agent-c, 75.15 -> 76.55): `while (ok && (fv++, nx != end))` puts fv++ in the bne delay slot after the ok exit as in the target (no bnel back edge).
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
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
extern "C" s32 gl_func_0003B9C0(ObjB9 *self, s32 max, f32 *dist, TriB9 **out, V3B9 *a, V3B9 *b, s32 unused, V3B9 *pts) {
    s32 cnt;
    u32 i;
    s32 off;
    ColB9 *col;
    FaceB9 *f;
    V3B9 *n;
    f32 dn;
    f32 t;
    s32 ok;
    V3B9 p;
    s32 ia;
    s32 ib;
    f32 qa;
    V3B9 d;
    f32 qb;
    f32 sg;
    s16 (*vt)[3];
    s16 *nx;
    u16 *fv;
    s16 *v0;
    s16 *v1;
    s16 ex;
    s16 ey;
    s16 dx;
    s16 dy;

    cnt = 0;
    if (self->col == 0) {
        return 0;
    }
    d.x = b->x - a->x;
    d.y = b->y - a->y;
    d.z = b->z - a->z;
    if (sqrtf(d.x * d.x + d.y * d.y + d.z * d.z) > 0.0001) {
        gl_func_00000000_vnorm3(&d);
    } else {
        d.x = 0.0f;
        d.z = 0.0f;
        d.y = -1.0f;
    }
    for (i = 0, off = 0; i < self->col->ntri; i++, off += 12) {
        if (((TriB9 *)((char *)self->tris + off))->grp < 0) {
            continue;
        }
        n = (V3B9 *)((char *)self->col->nrm + off);
        dn = n->x * d.x + n->y * d.y + n->z * d.z;
        if (dn > -0.0001) {
            continue;
        }
        ok = 1;
        t = -(a->x * n->x + a->y * n->y + a->z * n->z - self->tris[i].d) / dn;
        p.x = a->x + d.x * t;
        p.y = a->y + d.y * t;
        p.z = a->z + d.z * t;
        col = self->col;
        if (col->remap != 0) {
            f = &col->faces[col->remap[i]];
        } else {
            f = &col->faces[i];
        }
        ia = ((TriB9 *)((char *)self->tris + off))->a;
        ib = ((TriB9 *)((char *)self->tris + off))->b;
        qa = (&p.x)[ia];
        qb = (&p.x)[ib];
        sg = ((TriB9 *)((char *)self->tris + off))->sgn;
        vt = col->verts;
        nx = (s16 *)&D_00000000;
        fv = (u16 *)f;
        do {
            v0 = vt[fv[1]];
            v1 = vt[f->vi[*nx++]];
            ex = v1[ia] - v0[ia];
            ey = v1[ib] - v0[ib]; if (self) {}
            dx = (s32)(qa - v0[ia]); if (self) {}
            dy = (s32)(qb - v0[ib]);
            if (((f32)ex * (f32)dy - (f32)ey * (f32)dx) * sg > 0.0f) {
                ok = 0;
            }
        } while (ok && (fv++, nx != (s16 *)((char *)&D_3B9C0_c + 6)));
        if (ok) {
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
