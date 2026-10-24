/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for timproc_uso_b5_func_0000B624
 * (139 words = target). NM object only (build/non_matching); the ROM build keeps
 * INCLUDE_ASM. The cfe NM body lifted under NCC with the head's offset vector read
 * from file-local statics (kit LX: x / y one object read through references, z a
 * second one): 140 -> 139 words, 88.57 -> 89.7 fuzzy (agent-g run 142).
 * RESIDUAL: the target hoists the shared `lui at` to the first slot and orders the
 * head loads differently; FP colours in the rotate / scale tail. An exact landing
 * would pin the renamed .bss relocs to the USO data offsets 0 / 8. */
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef double f64;
#define M2C_FIELD(ptr, type, off) (*(type)((char *)(ptr) + (off)))
typedef struct { f32 f0; f32 f4; f32 f8; } VecF;
extern "C" char timproc_uso_b5_D_807FFBB0_ld4[];
extern "C" char timproc_uso_b5_D_807FFBB0_ld8[];
extern "C" {
extern void timproc_uso_b5_func_00006E08(char *arg0, s32 arg1);
extern int timproc_uso_b5_func_0546DC(char *, char *, f32, f32, f32);
extern void *timproc_uso_b5_func_071028(...);
extern void timproc_uso_b5_func_071AE0(void *quat, void *axis, f32 angle);
extern void timproc_uso_b5_func_054CAC(void *obj);
extern char timproc_uso_b5_D_807FF370;
extern char timproc_uso_b5_D_807FFBB0;
extern f32 timproc_uso_b5_D_807FFBB0_ld0;
extern int timproc_uso_b5_D_807FFBBC;
extern int timproc_uso_b5_func_054FE0(...);
extern int timproc_uso_b5_func_077C44(...);
extern void timproc_uso_b5_func_06970C_i(...);
}
static f32 sXY[2];
static f32 sZ;
extern "C" void timproc_uso_b5_func_0000B624(char *arg0) {
    VecF sp8C;
    union { VecF v; s32 w[4]; } sp7C;
    s32 var_v0;
    s32 temp_t5;
    VecF sp68;
    volatile s32 pad9C[3];
    char *temp_a0_2;
    VecF sp48;
    f32 ang;
    VecF sp38;
    VecF sp2C;
    f32 *sp28;

    sp28 = (f32 *) (arg0 + 0xDC);
    do {
        { f32 &x = sXY[0]; f32 &y = sXY[1];
        sp68.f0 = sp28[0] - x;
        sp68.f4 = sp28[1] - y; }
        sp68.f8 = sp28[2] - sZ;
    } while (0);
    sp7C.v = sp68;
    sp8C = sp7C.v;
    timproc_uso_b5_func_071028(&sp8C);
    sp28 = (f32 *) (arg0 + 0xF4);
    if ((f64) ((M2C_FIELD((&timproc_uso_b5_D_807FFBB0), f32 *, 0) * sp8C.f0) + (M2C_FIELD((&timproc_uso_b5_D_807FFBB0), f32 *, 4) * sp8C.f4) + (M2C_FIELD((&timproc_uso_b5_D_807FFBB0), f32 *, 8) * sp8C.f8)) < M2C_FIELD((&timproc_uso_b5_D_807FF370), f64 *, 0x350)) {
        M2C_FIELD(arg0, f32 *, 0xF4) = 0.0f;
        M2C_FIELD(arg0, f32 *, 0xF8) = 0.0f;
        M2C_FIELD(arg0, f32 *, 0xFC) = 0.0f;
        M2C_FIELD(arg0, f32 *, 0x100) = 1.0f;
        ang = M2C_FIELD(arg0, f32 *, 0x114);
        sp48.f8 = 0.0f;
        sp48.f4 = 0.0f;
        sp48.f0 = 1.0f;
        timproc_uso_b5_func_071AE0(sp28, &sp48, ang);
        ang = M2C_FIELD(arg0, f32 *, 0x110);
        sp38.f0 = 0.0f;
        sp38.f4 = 0.0f;
        sp38.f8 = 1.0f;
        timproc_uso_b5_func_071AE0(sp28, &sp38, ang);
        ang = M2C_FIELD(arg0, f32 *, 0x118);
        sp2C.f0 = 0.0f;
        sp2C.f8 = 0.0f;
        sp2C.f4 = 1.0f;
        timproc_uso_b5_func_071AE0(sp28, &sp2C, ang);
        timproc_uso_b5_func_06970C_i(M2C_FIELD(arg0, s32 *, 0x108), M2C_FIELD(arg0, s32 *, 0x130));
        if ((M2C_FIELD(arg0, s32 *, 0x144) != 0) || (M2C_FIELD(arg0, s32 *, 0x138) != 0)) {
            timproc_uso_b5_func_054FE0(arg0);
            timproc_uso_b5_D_807FFBBC += 1;
            var_v0 = M2C_FIELD(arg0, s32 *, 0x144); if (arg0) {}
            temp_t5 = var_v0 - 1;
            if (var_v0 != 0) {
                M2C_FIELD(arg0, s32 *, 0x144) = temp_t5;
                var_v0 = temp_t5;
            }
            if (var_v0 == 0) {
                temp_a0_2 = M2C_FIELD(arg0, char **, 0x10C);
                if (M2C_FIELD(temp_a0_2, s32 *, 0xB0) != 0) {
                    timproc_uso_b5_func_077C44(temp_a0_2, M2C_FIELD(arg0, s32 *, 0x140), 0, 2, 2, 1);
                    M2C_FIELD(M2C_FIELD(arg0, char **, 0x10C), s32 *, 0xB0) = 0;
                }
            }
        }
        timproc_uso_b5_func_054CAC(arg0);
        M2C_FIELD(arg0, s32 *, 0x134) = 1;
        return;
    }
    M2C_FIELD(arg0, s32 *, 0x134) = 0;
}
