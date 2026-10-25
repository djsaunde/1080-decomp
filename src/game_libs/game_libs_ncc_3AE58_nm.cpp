/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_0003AE58 (0x17C / 95 words), agent-c run 162.
 * Mesh-vertex table build: the cfront `this == 0 -> alloc(0x90)` protocol, two registrations, the count * 12
 * array, then a for loop over the mesh entries (optional short index table) converting each entry's short
 * vertex to a float triple passed by address. The cfe body (92.91) had the loop rotated wrong (beq / beq vs
 * the target's `bnezl at, loop; lw v0, 0x4C(s2)`) and a 0x20 volatile-array pad; under NCC the plain for loop
 * gives the target's rotation and delay-slot reload, `f32 fv[3]` at function top with one volatile scalar
 * above and six below gives frame 0x70 / fv at sp+0x60: 98.37, 95 words = target. Residual: the entry-pointer
 * merge variable colours v1 (target t0) and the t-ring is one register early. Still NON_MATCHING. */
typedef float f32;
extern "C" {
int gl_func_00000000(...);
extern char gl_data_0001EE5C, gl_data_0001EE70;
}
struct Ent { short a; unsigned short vi; int pad; };
struct SV { short x, y, z; };
struct Mesh { char pad[0x44]; unsigned n; char pad48[4]; short *tbl; char pad50[0x60 - 0x50]; SV *verts; char pad64[4]; Ent *ents; };
struct Own { char pad[0x54]; char *x54; };
struct Obj { char pad[0x2C]; char *x2C; char pad30[0x84 - 0x30]; Mesh *m; };
extern "C" Obj *gl_func_0003AE58(Obj *a0, Mesh *a1) {
    volatile int p0;
    f32 fv[3];
    volatile int q0, q1, q2, q3, q4, q5;
    if (a0 != 0 || (a0 = (Obj *)gl_func_00000000(0x90)) != 0) {
        gl_func_00000000(a0, &gl_data_0001EE5C);
        gl_func_00000000(&gl_data_0001EE70, 0);
        a0->m = a1;
        a0->x2C = (char *)gl_func_00000000(a1->n * 12);
        unsigned i;
        int off;
        for (i = 0, off = 0; i < a1->n; i++, off += 12) {
            SV *v = &a1->verts[(a1->tbl ? &a1->ents[a1->tbl[i]] : &a1->ents[i])->vi];
            fv[0] = v->x;
            fv[1] = v->y;
            fv[2] = v->z;
            gl_func_00000000(a0->x2C + off, fv, ((Own *)a0->m)->x54 + off, i);
        }
        gl_func_00000000();
    }
    return a0;
}
