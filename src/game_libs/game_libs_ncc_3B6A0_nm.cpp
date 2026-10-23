/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_0003B6A0: the NM C body lifted under NCC (agent-c run 134 sweep, cfe 65.26).
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
#undef NULL
#define NULL 0
extern "C" {
extern signed char b;
extern float gl_func_00000000_3a58cs(float);
extern int gl_proto_39E24(int, int, float, float, float, int, float);
extern char *o;
extern int count;
extern short D_00000000_3b6a0tbl[3];
}
extern "C" int gl_func_0003B6A0(char *o, int max, float *outd, char **outp, float *pt) {
    char *s8;
    int i;
    int off;
    unsigned int count;
    int nfound;
    int inside;
    short *rem;
    char *tri;
    char *face;
    char *verts;
    char *fp;
    short *tp;
    int a, b;
    int ax, ay;
    short ex, ey, px, py;
    float dir;
    float qa, qb;
    float cross;
    float d;
    char *v;
    char *w;
    float fzero;
    float lim;

    nfound = 0;
    s8 = *(char **)(o + 0x84);
    if (s8 == 0) {
        return 0;
    }
    count = *(int *)(s8 + 0x44);
    if (count != 0) {
        lim = -50.0f;
        fzero = 0.0f;
        off = 0;
        for (i = 0; i < count; i++) {
            rem = *(short **)(s8 + 0x4C);
            tri = *(char **)(o + 0x2C) + off;
            inside = 1;
            verts = *(char **)(s8 + 0x60);
            if (rem != 0) {
                face = *(char **)(s8 + 0x68) + rem[i] * 8;
            } else {
                face = *(char **)(s8 + 0x68) + i * 8;
            }
            dir = (float)*(signed char *)(tri + 3);
            a = *(signed char *)(tri + 0);
            b = *(signed char *)(tri + 1);
            qa = pt[a];
            qb = pt[b];
            tp = D_00000000_3b6a0tbl;
            fp = face;
            do {
                v = verts + *(unsigned short *)(fp + 2) * 6;
                w = verts + *(unsigned short *)(face + *tp * 2 + 2) * 6;
                tp++;
                ax = *(short *)(v + a * 2);
                ay = *(short *)(v + b * 2);
                ex = *(short *)(w + a * 2) - ax;
                ey = *(short *)(w + b * 2) - ay;
                px = (int)(qa - (float)ax);
                py = (int)(qb - (float)ay);
                cross = (float)ex * (float)py - (float)ey * (float)px;
                if (fzero < cross * dir) {
                    inside = 0;
                }
                fp += 2;
            } while (inside && tp != D_00000000_3b6a0tbl + 3);
            if (inside) {
                float *nrm = (float *)(*(char **)(s8 + 0x54) + off);
                d = pt[0] * nrm[0] + pt[1] * nrm[1] + pt[2] * nrm[2] - *(float *)(tri + 8);
                if (d < fzero && lim < d) {
                    if (nfound < max) {
                        outd[nfound] = d;
                        outp[nfound++] = *(char **)(o + 0x2C) + i * 12;
                        s8 = *(char **)(o + 0x84);
                        count = *(int *)(s8 + 0x44);
                    } else {
                        return -1;
                    }
                }
            }
            off += 12;
        }
    }
    return nfound;
}
