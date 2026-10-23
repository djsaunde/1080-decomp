/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00027804: the NM C body lifted under NCC + one parameter empty test from a fz sweep (agent-c run 134, cfe 77.59; a second parameter empty test from round 2 gives 78.51).
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
#undef NULL
#define NULL 0
#define FW(p, o) (*(int *)((char *)(p) + (o)))
#define FF(p, o) (*(f32 *)((char *)(p) + (o)))
typedef struct { int w0, w1; } GfxPair1EE78;
extern "C" {
extern int D_00000000;
extern char *gl_1dcb4_f5(char *, s32, s32);
extern char *gl_1dcb4_f4(char *, s16, s16);
extern GfxPair1EE78 *p;
extern unsigned char b0, b1;
extern unsigned char b0, b1, b2, b3, b4;
extern unsigned char b0, b1, b2, b3, b4;
extern char pad[0x15C];
extern int gl_ref_0003C2B4(...);
}
extern "C" void gl_func_00027804(char *arg0, char *arg1) {
    union {
        s32 w;
        struct {
            u8 b3;
            u8 pad;
            u8 b0;
            u8 b2;
        } b;
    } sc;
    s32 temp_v0;
    f32 var_f2;
    f32 var_f12;
    f32 var_f14;
    f32 temp_f0;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_a2;
    s32 v1s;
    s8 mode;
    char *var_v0;

    if (0) { char **pp27804 = &arg1; }
    temp_v0 = *(u8 *)(arg0 + 0x35);
    gl_ref_0003C2B4(arg0, FW(arg1, 0x4));
    var_f2 = FF(arg1, 0x8);
    sc.b.b2 = *(u8 *)(arg1 + 0x2);
    sc.b.b0 = *(u8 *)(arg1 + 0x0);
    *(u8 *)&sc = *(u8 *)(arg1 + 0x3);
    sc.b.b2 &= 0x7F;
    *(u8 *)(arg0 + 0xB0) &= ~8;
    *(u8 *)(arg0 + 0xB0) &= ~4;
    *(u8 *)(arg0 + 0xB0) = ((((u32) (sc.w << 6) >> 0x1F) << 1) & 2) | (*(u8 *)(arg0 + 0xB0) & 0xFD);
    *(u8 *)(arg0 + 0xB0) = (*(u8 *)&sc & 1) | (*(u8 *)(arg0 + 0xB0) & 0xFE);
    if ((temp_v0 != 0) && (*(s8 *)((char *)&D_00000000 + 0x2076) == 1)) {
        var_a1 = (s32) sc.b.b2 >> 1;
        var_v0 = arg0 + 0xB0;
        if (var_a1 >= 0x40) {
            var_a1 = 0x3F;
        }
        v1s = var_a1 * 2;
        *(u8 *)(var_v0 + 0x4) = *(u16 *)((char *)&D_00000000 + v1s);
        *(u8 *)(var_v0 + 0x1) = *(u8 *)(var_v0 + 0x1) | 1;
        *(u8 *)(var_v0 + 0x3) = *(u16 *)((char *)&D_00000000 + 0x7E - v1s);
        var_f12 = *(f32 *)((char *)&D_00000000 + sc.b.b2 * 4);
        var_f14 = *(f32 *)((char *)&D_00000000 + 0x1FC - sc.b.b2 * 4);
    } else {
        mode = *(s8 *)((char *)&D_00000000 + 0x2076);
        if ((temp_v0 != 0) && (mode == 0)) {
            var_v0 = arg0 + 0xB0;
            *(u8 *)(var_v0 + 0x3) = 0;
            *(u8 *)(var_v0 + 0x4) = 0;
            *(u8 *)(var_v0 + 0x1) = *(u8 *)(var_v0 + 0x1) & 0xFFFE;
            var_a1_2 = 0;
            var_f12 = *(f32 *)((char *)&D_00000000 + sc.b.b2 * 4);
            var_f14 = *(f32 *)((char *)&D_00000000 + 0x1FC - sc.b.b2 * 4);
            var_a2 = 0;
            if ((s32) sc.b.b2 < 0x20) {
                var_a2 = 1;
            } else if ((s32) sc.b.b2 >= 0x61) {
                var_a1_2 = 1;
            }
            *(u8 *)(var_v0 + 0x0) = ((var_a1_2 << 3) & 8) | (*(u8 *)(var_v0 + 0x0) & 0xFFF7);
            *(u8 *)(var_v0 + 0x0) = ((var_a2 << 2) & 4) | (*(u8 *)(var_v0 + 0x0) & 0xFB);
            switch ((u32) (sc.w << 2) >> 0x1E) {
            case 0:
                break;
            case 1:
                *(u8 *)(var_v0 + 0x0) = ((((u32) (sc.w << 4) >> 0x1F) << 3) & 8) | (*(u8 *)(var_v0 + 0x0) & 0xFFF7);
                *(u8 *)(var_v0 + 0x0) = ((((u32) (sc.w << 5) >> 0x1F) << 2) & 4) | (*(u8 *)(var_v0 + 0x0) & 0xFB);
                break;
            case 2:
                *(u8 *)(var_v0 + 0x0) = (((((u32) (sc.w << 4) >> 0x1F) | var_a1_2) << 3) & 8) | (*(u8 *)(var_v0 + 0x0) & 0xFFF7);
                *(u8 *)(var_v0 + 0x0) = (((((u32) (sc.w << 5) >> 0x1F) | var_a2) << 2) & 4) | (*(u8 *)(var_v0 + 0x0) & 0xFB);
                break;
            case 3:
                *(u8 *)(var_v0 + 0x0) = (((((u32) (sc.w << 4) >> 0x1F) ^ var_a1_2) << 3) & 8) | (*(u8 *)(var_v0 + 0x0) & 0xFFF7);
                *(u8 *)(var_v0 + 0x0) = (((((u32) (sc.w << 5) >> 0x1F) ^ var_a2) << 2) & 4) | (*(u8 *)(var_v0 + 0x0) & 0xFB);
                break;
            }
        } else if (mode == 3) {
            var_v0 = arg0 + 0xB0; if (arg0) {}
            var_f14 = *(f32 *)((char *)&D_00000000 + 0xFC0);
            *(u8 *)(var_v0 + 0x0) &= ~2;
            *(u8 *)(var_v0 + 0x0) &= ~1;
            var_f12 = var_f14;
        } else {
            var_v0 = arg0 + 0xB0; if (arg0) {}
            *(u8 *)(var_v0 + 0x0) = ((((u32) (sc.w << 4) >> 0x1F) << 3) & 8) | (*(u8 *)(var_v0 + 0x0) & 0xFFF7);
            *(u8 *)(var_v0 + 0x0) = ((((u32) (sc.w << 5) >> 0x1F) << 2) & 4) | (*(u8 *)(var_v0 + 0x0) & 0xFB);
            var_f12 = *(f32 *)((char *)&D_00000000 + sc.b.b2 * 4);
            var_f14 = *(f32 *)((char *)&D_00000000 + 0x1FC - sc.b.b2 * 4);
        }
    }
    if (var_f2 < 0.0f) {
        var_f2 = 0.0f;
    }
    if (var_f2 > 1.0f) {
        var_f2 = 1.0f;
    }
    temp_f0 = *(f32 *)((char *)&D_00000000 + 0xFC4);
    *(s16 *)(var_v0 + 0x8) = (s16) (s32) (var_f2 * var_f12 * temp_f0);
    *(s16 *)(var_v0 + 0xA) = (s16) (s32) (var_f2 * var_f14 * temp_f0);
    *(u8 *)(var_v0 + 0x2) = *(u8 *)(arg1 + 0x1);
    FW(var_v0, 0x14) = FW(arg1, 0x10);
    *(u8 *)(var_v0 + 0x7) = *(u8 *)(arg1 + 0x14);
    *(u16 *)(var_v0 + 0xE) = *(u16 *)(arg1 + 0x16);
    *(u8 *)(var_v0 + 0x5) = sc.b.b0;
}
