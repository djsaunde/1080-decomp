/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for gl_func_0005F984: the existing cfe NM body
 * recompiled unchanged under NCC (85.13 -> 88.01, same length). NM object only; ROM build keeps INCLUDE_ASM. */
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
extern "C" void gl_func_0005F984(register f32 (*m)[4], f32 (*rot)[4], f32 x, f32 y, f32 z, s32 flag) {
    f32 R[4][4];
    f32 len;
    V3_5F984 v;
    f32 pad[2];
    f32 out[4][4];
    f32 t[2];
    f32 h;
    f32 c;
    f32 cs, sn;
    s32 i, j, k;

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
        v.x = rot[1][0] * b + rot[2][0] * d + rot[0][0] * a;
        v.y = rot[1][1] * b + rot[2][1] * d + a * rot[0][1];
        v.z = a * rot[0][2] + rot[1][2] * b + rot[2][2] * d;
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
