/* IDO 7.1 NCC (C++) donor for gl_func_0005F984 (rotate-by-drag matrix, 184 words). EXACT (agent-g run 156):
 * spliced into both build/src and build/non_matching post0b with REPLACE_FUNC_BODY.
 * History: the cfe NM body recompiled under NCC (85.13 -> 88.01); run 155 param copy `m = arg0` + volatile
 * q0 / q1 around R (92.46). Run 156, the last two residuals:
 * - the rot block's register cycle through sp+0x20 / 0x24: the target computes the three rotated
 *   components into registers (f0 / f12 / f14) and stores them only afterwards (y, x, z), so ugen runs
 *   out of FP temps and spills the inputs a / b / d. Written with dead locals reused for the results
 *   (c, h, len: no new frame words) and every product as rot[r][c] * var in row order.
 * - homes: NCC gives each declared local one frame word in declaration order; two scalars between q1 /
 *   len / v and the other six between v and out put len at 0xB0, v at 0xA0 and out at 0x48. */
typedef float f32; typedef double f64; typedef int s32; typedef unsigned int u32; typedef short s16; typedef unsigned short u16; typedef signed char s8; typedef unsigned char u8; 
#define NULL 0
typedef struct { f32 x, y, z; } V3_5F984;
typedef struct { s32 a, b; } Pair_51AD8;
extern "C" {
extern int gl_func_00000000(...);
extern int w, h;
extern void gl_func_00000000_d414(float t, int *a, int *b);
extern short pad;
extern void gl_func_00000000_rot5F984(f32 (*)[4], V3_5F984 *, f32, f32);
extern char *c;
extern Pair_51AD8 m;
extern f32 v;
extern void gl_func_00000000_d414(float t, int *a, int *b);
extern void gl_func_00000000_d414(float t, int *a, int *b);
extern void gl_func_00000000_5eaa4(f32 (*)[4], s32, f32, s32);
extern f32 gl_func_00000000_sqrt5F984(f32);
extern void gl_func_00000000_rot5F984(f32 (*)[4], V3_5F984 *, f32, f32);
}
extern "C" void gl_func_0005F984(f32 (*arg0)[4], f32 (*rot)[4], f32 x, f32 y, f32 z, s32 flag) {
    volatile f32 q0;
    f32 R[4][4];
    volatile f32 q1;
    f32 h;
    f32 len;
    f32 c;
    V3_5F984 v;
    f32 cs;
    f32 sn;
    s32 i;
    s32 j;
    s32 k;
    f32 (*m)[4];
    f32 out[4][4];

    m = arg0;
    len = gl_func_00000000_sqrt5F984(x * x + y * y + z * z);
    if (len == 0.0f) {
        if (flag != 0) {
            gl_func_00000000(m, rot);
        }
        return;
    }
    c = 100.0f;
    h = gl_func_00000000_sqrt5F984(c * c + len * len);
    cs = 100.0f / h;
    v.x = -y / len;
    v.y = -x / len;
    v.z = -z / len;
    sn = len / h;
    if (rot != NULL) {
        f32 a = v.x, b = v.y, d = v.z;
        c = rot[0][0] * a + rot[1][0] * b + rot[2][0] * d;
        h = rot[0][1] * a + rot[1][1] * b + rot[2][1] * d;
        len = rot[0][2] * a + rot[1][2] * b + rot[2][2] * d;
        v.y = h;
        v.x = c;
        v.z = len;
    }
    gl_func_00000000_rot5F984(R, &v, sn, cs);
    if (flag == 0) {
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                out[i][j] = 0.0f;
                for (k = 0; k < 4; k++) {
                    out[i][j] += m[i][k] * R[k][j];
                }
            }
        }
        gl_func_00000000(out, m);
    } else {
        gl_func_00000000(R, m);
    }
}
