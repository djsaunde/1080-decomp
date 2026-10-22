/* IDO 7.1 NCC (C++) donor for timproc_uso_b5_func_00001DB0 (0x164 / 89 words).
 * timproc_b5 is C++: edg keeps the SOURCE operand order of commutative FP ops
 * (cfe canonicalizes the constant to the right), so `C0 + B8 * t` and
 * `41.0f + 5.0f * f` emit the target's const-first mul/add operands.
 * Spliced into timproc_uso_b5.c.o via REPLACE_FUNC_BODY. */
extern "C" {
extern char timproc_uso_b5_D_807FF0D8, timproc_uso_b5_D_807FF0E0;
#define D_dbl_1DB0_B8 (*(double *)(&timproc_uso_b5_D_807FF0D8 + 0xB8))
#define D_dbl_1DB0_C0 (*(double *)(&timproc_uso_b5_D_807FF0E0 + 0xC0))
typedef int s32; typedef float f32; typedef double f64;
void timproc_uso_b5_func_00001DB0(char *a0) {
    char *t;
    char *u;

    *(s32 *)(*(char **)(a0 + 0x34) + 0x4D8) = *(s32 *)(a0 + 0x44);
    t = *(char **)(a0 + 0x34);
    if (*(s32 *)(t + 0x4D8) == 0) {
        *(s32 *)(*(char **)(*(char **)(t + 0x414) + 0xC) + 0xBC) = 0;
    } else {
        *(s32 *)(*(char **)(*(char **)(t + 0x414) + 0xC) + 0xC4) = 0;
        *(s32 *)(*(char **)(*(char **)(*(char **)(a0 + 0x34) + 0x414) + 0xC) + 0xD4) = 0;
        *(s32 *)(*(char **)(*(char **)(*(char **)(a0 + 0x34) + 0x414) + 0xC) + 0xBC) = *(s32 *)(a0 + 0x44);
        *(s32 *)(*(char **)(*(char **)(*(char **)(a0 + 0x34) + 0x414) + 0xC) + 0xCC) = *(s32 *)(a0 + 0x44);
        *(f32 *)(*(char **)(*(char **)(*(char **)(a0 + 0x34) + 0x414) + 0xC) + 0xB4) =
            D_dbl_1DB0_C0 + D_dbl_1DB0_B8 * (f64)(1.0f - *(f32 *)(a0 + 0x54));
        *(f32 *)(*(char **)(a0 + 0x34) + 0x49C) = 41.0f + 5.0f * *(f32 *)(a0 + 0x54);
    }
    *(s32 *)(*(char **)(a0 + 0x38) + 0x4D8) = 0xF0 - *(s32 *)(a0 + 0x44);
    u = *(char **)(a0 + 0x38);
    if (*(s32 *)(u + 0x4D8) == 0) {
        *(s32 *)(*(char **)(*(char **)(u + 0x414) + 0xC) + 0xBC) = 0;
        return;
    }
    *(s32 *)(*(char **)(*(char **)(u + 0x414) + 0xC) + 0xC4) = *(s32 *)(a0 + 0x44);
    *(s32 *)(*(char **)(*(char **)(*(char **)(a0 + 0x38) + 0x414) + 0xC) + 0xD4) = *(s32 *)(a0 + 0x44);
    *(s32 *)(*(char **)(*(char **)(*(char **)(a0 + 0x38) + 0x414) + 0xC) + 0xBC) = 0xF0 - *(s32 *)(a0 + 0x44);
    *(s32 *)(*(char **)(*(char **)(*(char **)(a0 + 0x38) + 0x414) + 0xC) + 0xCC) = 0xF0 - *(s32 *)(a0 + 0x44);
}
}
