/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_0004B0A8: the NM C body lifted under NCC (agent-c run 134 sweep, cfe 93.36).
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
#undef NULL
#define NULL 0
#define FW(p, o) (*(int *)((char *)(p) + (o)))
typedef struct { int w; } Word3E968;
typedef struct { char *src; s32 idx; s32 end; } Q_0004B0A8;
extern "C" char qa[];
extern "C" char qb[];
extern "C" char qc[];
typedef struct { int a, b; } P2_0004B0A8;
extern "C" {
extern int D_00000000;
extern Word3E968 *p;
extern void gl_49B3C_regf(char *, char *, char *, float, float, int);
extern char *o;
extern int gl_func_00034458(...);
extern int  idx;
extern int arg1;
extern int  end;
extern u8 gl_d_4b0a8_tbl;
extern u8 gl_d_4b0a8_vt;
extern u8 gl_d_4b0a8_w214;
}
extern "C" int *gl_func_0004B0A8(int *arg0, char *arg1) {
    int spA0[2];
    Q_0004B0A8 sp94;
    s32 pad90;
    Q_0004B0A8 sp84;
    Q_0004B0A8 sp78;
    int *self;
    int *ps;
    int *res;
    int **cur;
    int **nxt;
    int *vp;
    Q_0004B0A8 *qa;
    Q_0004B0A8 *qb;
    Q_0004B0A8 *qc;
    char *tmp;
    s32 iv;
    s32 ni;
    int *cbres;
    char *rec;
    s32 cnt;
    char *slot;
    s32 a2v;
    char *base2;
    char *e1;
    char *e2;
    char *a1v;

    self = arg0;
    if ((arg0 != 0) || (res = (int *)gl_func_00034458(0x44), self = res, (res != 0))) {
        gl_func_00034458(self, (char *)&D_00000000 + 0x200EC);
        FW(self, 0x28) = (int)&gl_d_4b0a8_tbl;
        ps = spA0;
        *(P2_0004B0A8 *)ps = *(P2_0004B0A8 *)((char *)&gl_d_4b0a8_vt + 0x140);
        self[0] = 2;
        res = (int *)gl_func_00034458(0, (char *)&D_00000000 + 0x200F4, *(s32 *)((char *)&gl_d_4b0a8_w214 + 0x214));
        FW(self, 0x40) = (int)res;
        gl_func_00034458(res, ps);
        gl_func_00034458(FW(self, 0x40), ps);
        gl_func_00034458(FW(self, 0x40), ps);
        gl_func_00034458(FW(self, 0x40), ps);
        ps = (int *)&sp78;
        qa = (Q_0004B0A8 *)ps;
        if (((u32)ps > 0U) || (qa = (Q_0004B0A8 *)gl_func_00034458(0xC), (qa != 0))) {
            tmp = arg1 + 0x84;
            ((volatile Q_0004B0A8 *)qa)->idx = -1;
            ((volatile Q_0004B0A8 *)qa)->src = tmp;
            ((volatile Q_0004B0A8 *)qa)->end = FW(tmp, 0xC) - 1;
        }
        qb = &sp84;
        *qb = *(Q_0004B0A8 *)ps;
        qc = &sp94;
        *qc = *qb;
        if (sp94.idx < sp94.end) {
            ni = sp94.idx + 1;
            sp94.idx = ni;
            cur = (int **)*(int **)sp94.src + ni;
        } else {
            cur = 0;
        }
        if (cur != 0) {
            do {
                iv = sp94.idx * 4;
                if (FW(*(int **)((char *)*(int **)sp94.src + iv), 0x6C) != 0) {
                    base2 = *(char *volatile *)sp94.src;
                    ps = (int *)FW(self, 0x40);
                    e1 = (char *)*(int **)(base2 + iv);
                    e2 = *(char *volatile *)((int)base2 + iv);
                    a1v = (char *)FW(e1, 0x6C); if (1) {}
                    a2v = *(s16 *)(e2 + 0x8A);
                    cbres = (int *)gl_func_00034458(*(int *)FW(a1v, 0xC) + (a2v * 8), a1v, a2v);
                    vp = (int *)FW(ps, 0xC);
                    cnt = vp[1];
                    vp[1] = cnt + 1;
                    tmp = (char *)(FW(FW(ps, 0xC), 0x0) + (cnt * 8)); if (1) {}
                    FW(tmp, 0x0) = 0x06000000;
                    FW(tmp, 0x4) = (int)cbres;
                }
                if (sp94.idx < sp94.end) {
                    ni = sp94.idx + 1;
                    sp94.idx = ni;
                    nxt = (int **)*(int **)sp94.src + ni;
                } else {
                    nxt = 0;
                }
            } while (nxt != 0);
        }
        gl_func_00034458(FW(self, 0x40));
    }
    return self;
}
