/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_000493AC (agent-c run 152): the NM C body lifted under NCC.
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32;
typedef float f32; typedef double f64;
extern "C" {
extern int D_00000000;
extern int func_00000000(...);
extern int gl_func_00000000(...);
extern int gl_func_00034458(...);
extern int D_493AC_a, D_493AC_b, D_493AC_c, D_493AC_d;
}
/* 2026-10-17 run 110 re-decode (agent-c): cfront registration pass. Two
 * inlined Array iterators over self+0x84 ({arr, idx, last}, returned by
 * value through a temp), a 0x59C builder record at sp+0xE0, per-row
 * vcalls through self->0x28 vtable entry {s16 delta @0x60, fptr @0x64}. */
typedef struct { char **buf; int pad4; int pad8; int count; } Arr493;
typedef struct { Arr493 *arr; int idx; int last; } It493;
typedef struct { u32 w0, w1; } Gfx493;
typedef struct { Gfx493 *buf; int n; } Pool493;
typedef struct { char pad[0xC]; Pool493 *gfx; } Ctx493;
typedef struct { Ctx493 *ctx; char pad[0x588]; int n58C; int f590; int f594; char *p598; } Rec493;
typedef struct { int w; int n; char pad[0x10]; s16 *tbl18; int *row1C; } Grid493;
typedef struct { s16 d; s16 pad; int (*f)(...); } VEnt493;
typedef struct {
    char pad0[0x14]; int f14; int key18; char pad1c[0x34]; int lo50; int hi54;
    char pad58[0x14]; Ctx493 *ctx6C; char pad70[0xC]; s16 *rows7C; s16 n80; char pad82[8]; s16 n8A;
} Obj493;
#define STR493(o) ((char *)&D_00000000 + (o))
#define G1C4(S) (*(s32 *)((char *)&S + 0x1C4))
#define G214 (*(Ctx493 **)((char *)&D_493AC_d + 0x214))
#define SELF(o, T) (*(T *)(self + (o)))
/* run 151 (agent-c, 86.86 -> 89.86): the 0x214 context read goes through its own
 * zero-pinned alias D_493AC_d (target `lui t6; lw t6,532(t6)`; through &D_00000000
 * it shared a held lui/addiu base with rec.p598 and pushed &rec and the 1 into
 * s7 / s8); the +0x34 argument of the per-node call is a second element spelling
 * (target re-adds base + i*4). Length 1904 -> 1908 (target 1936).
 * Later in run 151 (89.86 -> 90.60): the 0x4C remap pick is an if/else with
 * `j = i` in the else arm (kit XCIV `b .+8` fingerprint; 1908 -> 1912). */
extern "C" s32 gl_func_000493AC(char *self) {
    Obj493 *o;
    Grid493 *gr;
    int cnt;
    int hit;
    int c;
    int j;
    int e;
    u32 i;
    It493 *q;
    char **v;
    char *tp;
    Gfx493 *d;
    int n;
    It493 it;
    int total;
    u8 *buf;
    Rec493 rec;
    int pad1[2];
    It493 tmp;
    It493 t1;
    int pad2[11];
    It493 t2;
    int pad3[2];
    int k;
    int pad4;
    int rowStart;
    int pad5[10];
    int rowOff;

    if (!(SELF(0x30, s32) & 1)) {
        return 1;
    }
    gl_func_00000000(SELF(0xC, int));
    gl_func_00000000(STR493(0x1FF54), SELF(0xC, int));
    SELF(0xB0, int) = 0;
    SELF(0xB4, int) = 0;
    SELF(0xB8, int) = 0;
    gl_func_00000000(STR493(0x1FF58));
    gl_func_00000000(STR493(0x1FF60), 0);
    gl_func_00000000();
    gl_func_00000000(STR493(0x1FF6C));
    gl_func_00000000(STR493(0x1FF74));
    if (1) { q = &t1; }
    if (q != 0 || (q = (It493 *)gl_func_00000000(12)) != 0) {
        q->arr = (Arr493 *)(self + 0x84);
        q->idx = -1;
        q->last = q->arr->count - 1;
    }
    tmp = t1;
    it = tmp;
    v = (it.idx < it.last) ? (++it.idx, &it.arr->buf[it.idx]) : 0;
    while (v != 0) {
        gl_func_00000000(it.arr->buf[it.idx]);
        if (SELF(0x50, int) == 0) {
            tp = it.arr->buf[it.idx];
            if (*(int *)(tp + 0x38) & 0x2000) {
                gl_func_00000000(it.arr->buf[it.idx], SELF(0x9C, int));
            }
        }
        gl_func_00000000(it.arr->buf[it.idx], *(char **)((char *)it.arr->buf + it.idx * 4) + 0x34);
        v = (it.idx < it.last) ? (++it.idx, &it.arr->buf[it.idx]) : 0;
    }
    gl_func_00000000();
    gl_func_00000000(STR493(0x1FF7C));
    gl_func_00000000(STR493(0x1FF88));
    total = 0;
    if (G1C4(D_493AC_a) & 8) {
        SELF(0x30, s32) |= 0x80;
        buf = (u8 *)gl_func_00000000(&D_00000000, SELF(0x44, u32));
        for (i = 0; i < SELF(0x44, u32); i++) {
            buf[i] = 0;
        }
    }
    rec.p598 = (char *)&D_00000000;
    if (1) { q = &t2; }
    if (q != 0 || (q = (It493 *)gl_func_00000000(12)) != 0) {
        q->arr = (Arr493 *)(self + 0x84);
        q->idx = -1;
        q->last = q->arr->count - 1;
    }
    tmp = t2;
    it = tmp;
    v = (it.idx < it.last) ? (++it.idx, &it.arr->buf[it.idx]) : 0;
    while (v != 0) {
        Ctx493 *g;
        o = (Obj493 *)it.arr->buf[it.idx];
        g = G214;
        o->ctx6C = g;
        cnt = 0;
        o->n8A = g->gfx->n;
        rec.n58C = 0;
        rec.f594 = 3;
        rec.f590 = 32;
        rec.ctx = g;
        gl_func_00000000(&rec);
        if (G1C4(D_493AC_b) & 8) {
            k = 0;
            if (SELF(0xA0, int) == 0) {
                gl_func_00000000(STR493(0x1FF90), STR493(0x1FFA0), 3287);
            }
            {
                tp = (char *)o->ctx6C;
                o->n80 = ((Ctx493 *)tp)->gfx->n;
                n = ((Ctx493 *)tp)->gfx->n;
                ((Ctx493 *)tp)->gfx->n = n + 1;
                d = &((Ctx493 *)tp)->gfx->buf[n];
                d->w0 = 0xC0000000;
                d->w1 = 0;
            }
            o->lo50 = 0x80;
            o->hi54 = 0;
            gr = SELF(0xA0, Grid493 *);
            if (gr->n > 0) {
                rowOff = 0;
                do {
                    hit = 0;
                    rowStart = gr->w * k;
                    for (c = rowStart; c < rowStart + gr->w; c++) {
                        e = gr->row1C[c];
                        if (e != -1) {
                            while ((j = gr->tbl18[e++]) != -1) {
                                u8 *m = buf + j;
                                if (o->key18 == *(u16 *)(SELF(0x68, char *) + j * 8) && *m == 0) {
                                    o->f14 = 1;
                                    tp = SELF(0x28, char *);
                                    cnt++;
                                    hit = 1;
                                    ((VEnt493 *)tp)[12].f(((VEnt493 *)tp)[12].d + self, 0, &rec, j, o);
                                    *m = 1;
                                }
                                gr = SELF(0xA0, Grid493 *);
                            }
                        }
                    }
                    if (hit) {
                        if (k < o->lo50) o->lo50 = k;
                        if (o->hi54 < k) o->hi54 = k;
                    }
                    gl_func_00000000(&rec);
                    {
                        tp = (char *)o->ctx6C;
                        *(s16 *)((char *)o->rows7C + rowOff) = ((Ctx493 *)tp)->gfx->n;
                        tp = (char *)o->ctx6C;
                        n = ((Ctx493 *)tp)->gfx->n;
                        ((Ctx493 *)tp)->gfx->n = n + 1;
                        d = &((Ctx493 *)tp)->gfx->buf[n];
                        d->w0 = 0xC0000000;
                        d->w1 = 0;
                    }
                    rowOff += 2;
                    k++;
                    gr = SELF(0xA0, Grid493 *);
                } while (k < gr->n);
            }
        } else {
            o->lo50 = -1;
            for (i = 0; i < SELF(0x44, u32); i++) {
                if (SELF(0x4C, s16 *) != 0) {
                    j = SELF(0x4C, s16 *)[i];
                } else {
                    j = i;
                }
                if (o->key18 == *(u16 *)(SELF(0x68, char *) + j * 8)) {
                    o->f14 = 1;
                    tp = SELF(0x28, char *);
                    cnt++;
                    ((VEnt493 *)tp)[12].f(((VEnt493 *)tp)[12].d + self, 0, &rec, j, o);
                }
            }
        }
        gl_func_00000000(&rec);
        total += rec.n58C;
        gl_func_00000000(o->ctx6C);
        if (cnt == 0) {
            o->ctx6C = 0;
        }
        v = (it.idx < it.last) ? (++it.idx, &it.arr->buf[it.idx]) : 0;
    }
    gl_func_00000000(STR493(0x1FFAC), SELF(0xC, int), total);
    if (G1C4(D_493AC_c) & 8) {
        gl_func_00000000(&D_00000000, buf);
    }
    gl_func_00000000();
    gl_func_00000000(STR493(0x1FFB4));
    gl_func_00000000();
    gl_func_00000000();
    gl_func_00000000(STR493(0x1FFC4));
    return 1;
}
