/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00043D38: the NM C body lifted under NCC (agent-c run 134 sweep, cfe 79.55; if (arg0) {} after var_v1 += 1 from a parameter-only fz sweep, 80.25).
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
#undef NULL
#define NULL 0
#define FW(p, o) (*(int *)((char *)(p) + (o)))
typedef struct { int w; } Word3E968;
extern "C" {
extern Word3E968 *p;
extern void gl_proto_41148(int, int, int, int, int, double);
extern char *o;
extern int gl_func_00034458(...);
}
extern "C" void gl_func_00043D38(char *arg0) {
    char *sp1C;
    s16 temp_a2;
    s16 temp_a2_2;
    s16 temp_a2_3;
    s32 var_a0;
    s32 var_t2;
    s32 var_v1;
    char *temp_a1;
    char *temp_v0;
    char *temp_v0_2;
    char *temp_v0_3;
    char *temp_v0_4;
    char *temp_v1;

    if (FW(arg0, 0x7C) != 0) {
        temp_v0 = *(char **)0x254;
        var_t2 = FW(temp_v0, 0x10C);
        if (var_t2 > 0) {
            var_t2 -= 1;
        }
        var_v1 = var_t2;
        var_a0 = var_v1 * 2;
        if (var_t2 < FW(temp_v0, 0x110)) {
            do {
                var_v1 += 1; if (arg0) {}
                temp_a2 = *(int*)(FW(arg0, 0x7C) + var_a0);
                var_a0 += 2;
                temp_v0_2 = (char *)(*(int*)(FW(FW(arg0, 0x6C), 0xC)) + (temp_a2 * 8));
                FW(temp_v0_2, 0x0) = 0xC0000000;
                FW(temp_v0_2, 0x4) = 0;
            } while (var_v1 < FW((*(char **)0x254), 0x110));
        }
        if (var_t2 != 0) {
            temp_a1 = (char *)(FW(arg0, 0x6C));
            temp_a2_2 = *(int*)(FW(arg0, 0x7C) + (var_t2 * 2));
            temp_v1 = (char *)(*(int*)(FW(temp_a1, 0xC)) + (FW(arg0, 0x80) * 8));
            FW(temp_v1, 0x0) = 0x06010000;
            sp1C = temp_v1;
            FW(temp_v1, 0x4) = gl_func_00034458(*(int*)(FW(FW(arg0, 0x6C), 0xC)) + (temp_a2_2 * 8), temp_a1, temp_a2_2, arg0);
        } else {
            temp_v0_3 = (char *)(*(int*)(FW(FW(arg0, 0x6C), 0xC)) + (FW(arg0, 0x80) * 8));
            FW(temp_v0_3, 0x0) = 0xC0000000;
            FW(temp_v0_3, 0x4) = 0;
        }
        temp_a2_3 = *(int*)(FW(arg0, 0x7C) + (FW((*(char **)0x254), 0x110) * 2));
        if (temp_a2_3 != -1) {
            temp_v0_4 = (char *)(*(int*)(FW(FW(arg0, 0x6C), 0xC)) + (temp_a2_3 * 8));
            FW(temp_v0_4, 0x0) = 0xB8000000;
            FW(temp_v0_4, 0x4) = 0;
        }
    }
}
