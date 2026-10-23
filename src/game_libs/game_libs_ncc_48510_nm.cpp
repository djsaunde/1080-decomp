/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00048510: the NM C body lifted under NCC (agent-c run 134 sweep, cfe 80.23; if (arg0) {} after the temp_t8 load from a parameter-only fz sweep).
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
#undef NULL
#define NULL 0
#define FW(p, o) (*(int *)((char *)(p) + (o)))
typedef struct { int w; } Word3E968;
typedef char *(*GP_00048510)(...);
extern "C" {
extern Word3E968 *p;
extern void gl_proto_41148(int, int, int, int, int, double);
extern char *o;
extern int gl_func_00034458(...);
extern int arg3;
extern int arg4;
extern int arg1;
}
extern "C" void gl_func_00048510(char *arg0, char *arg1, char *arg2, s32 *arg3, s32 arg4) {
    s32 sp6C;
    s32 *var_s3;
    s32 *var_v0;
    s32 temp_a0;
    s32 temp_t6;
    s32 temp_t8;
    s32 temp_v0;
    s32 var_a0;
    s32 var_a1;
    s32 var_s1;
    s32 var_t0;
    s32 var_v1;
    char *temp_a1;
    char *temp_a1_2;
    char *temp_v0_2;
    char *temp_v1;
    char *var_a1_2;
    char *var_s2;

    *arg3 = 0;
    var_t0 = FW(arg0, 0x594);
    var_s1 = 0;
    if (var_t0 > 0) {
        var_s2 = arg1;
        var_s3 = &sp6C;
        do {
            var_a1 = var_s1 == 0;
            if (var_a1 == 0) {
                var_a1 = var_t0 == 2;
            }
            temp_v0 = gl_func_00034458(arg0, var_a1, var_s2);
            *var_s3 = temp_v0;
            if (temp_v0 < 0) {
                if (FW(arg0, 0x4) >= (FW(arg0, 0x590) - 1)) {
                    temp_a1 = (char *)(FW(arg0, 0x598));
                    var_s1 = 0;
                    ((GP_00048510)FW(temp_a1, 0xC))(FW(temp_a1, 0x8) + (int)arg0, temp_a1);
                    gl_func_00034458(arg0);
                    var_s2 = arg1;
                    var_s3 = &sp6C;
                }
                if (var_s1 == 0) {
                    *arg3 = 1;
                }
                *var_s3 = FW(FW(FW(arg0, 0x0), 0x1C), 0x4);
                temp_v0_2 = (char *)(FW(arg0, 0x0));
                temp_v1 = (char *)(FW(temp_v0_2, 0x1C));
                temp_a0 = FW(temp_v1, 0x4);
                FW(temp_v1, 0x4) = (s32) (temp_a0 + 1);
                temp_a1_2 = (char *)(FW(FW(temp_v0_2, 0x1C), 0x0) + (temp_a0 * 0x10));
                FW(temp_a1_2, 0x0) = (s32) FW(var_s2, 0x0);
                FW(temp_a1_2, 0x4) = (s32) FW(var_s2, 0x4);
                FW(temp_a1_2, 0x8) = (s32) FW(var_s2, 0x8);
                FW(temp_a1_2, 0xC) = (s32) FW(var_s2, 0xC);
                FW(((int)arg0 + (FW(arg0, 0x4) * 2)), 0x4C4) = (u16) *(int*)((int)arg4 + (var_s1 * 2));
                FW(arg0, 0x4) = (s32) (FW(arg0, 0x4) + 1);
            }
            var_t0 = FW(arg0, 0x594);
            var_s1 += 1;
            var_s2 += 0x10;
            var_s3 += 4;
        } while (var_s1 < var_t0);
    }
    var_v1 = 0;
    if (var_t0 > 0) {
        var_a0 = 0;
        var_a1_2 = arg2;
        var_v0 = &sp6C;
        do {
            var_v1 += 1;
            var_a1_2 += 2;
            FW(var_a1_2, -0x2) = (s16) *var_v0;
            temp_t8 = *(int*)var_v0; if (arg0) {}
            var_v0 += 4;
            FW(((int)arg0 + (FW(arg0, 0x8) * 6) + var_a0), 0x14) = (s16) temp_t8;
            var_a0 += 2;
        } while (var_v1 < FW(arg0, 0x594));
    }
    temp_t6 = FW(arg0, 0x8) + 1;
    FW(arg0, 0x8) = temp_t6;
    if (temp_t6 >= 0xC8) {
        gl_func_00034458((char *)0x1FF44);
    }
}
