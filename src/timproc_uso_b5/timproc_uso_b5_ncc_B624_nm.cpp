/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for timproc_uso_b5_func_0000B624
 * (139 words = target). NM object only (build/non_matching); the ROM build keeps
 * INCLUDE_ASM. The cfe NM body lifted under NCC with the head's offset vector read
 * from file-local statics (kit LX: x / y one object read through references, z a
 * second one): 140 -> 139 words, 88.57 -> 89.7 fuzzy (agent-g run 142).
 * RESIDUAL: the target hoists the shared `lui at` to the first slot and orders the
 * head loads differently; FP colours in the rotate / scale tail. An exact landing
 * would pin the renamed .bss relocs to the USO data offsets 0 / 8.
 * Run 156 (kit LXXXVI): the head offset vector is computed into three held locals and stored
 * afterwards (the target keeps the differences in f2 / f0 / f12 and loads all three statics
 * first): 89.84 -> 93.72, length unchanged (139 words). The reference form of x / y on top
 * gives 94.78 but 138 words (away from the target), not kept.
 * Run 157: the head is an inline helper returning a named non-POD V (relPos below), the
 * three subtracts held in dx / dy / dz and the frame-only locals of the if arm moved
 * into the arm: 93.72 -> 95.57, 139 words. RESIDUAL: arg0 goes to s0 before the head
 * (target: after it), 1.0f colour. The 0x144 countdown written as
 * `if (c) c -= 1; if (c == 0)` (fixes the t5 temp), sp28 dropped for arg0 + 0xF4 (fixes the frame,
 * 0xA8 -> 0x98 = target): 95.57 -> 96.07. */
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

/* The head is an inline helper returning a named V by value: NCC keeps the
 * argument address (arg0 + 0xDC) in v0, holds the three differences and makes
 * the ctor-temp -> return-slot -> sp8C copy chain of the target (agent-g run 157). */
struct V { f32 x, y, z; V() {} };   /* the user-declared ctor matters (POD V = 94.63) */
static inline V relPos(const V &a) {
    f32 &x = sXY[0]; f32 &y = sXY[1];
    V r; f32 dx = a.x - x; f32 dy = a.y - y; f32 dz = a.z - sZ;
    r.x = dx; r.y = dy; r.z = dz;
    return r;
}
extern "C" void timproc_uso_b5_func_0000B624(char *arg0) {
    VecF sp8C;

    *(V *) &sp8C = relPos(*(V *) (arg0 + 0xDC));
    timproc_uso_b5_func_071028(&sp8C);
    if ((f64) ((M2C_FIELD((&timproc_uso_b5_D_807FFBB0), f32 *, 0) * sp8C.f0) + (M2C_FIELD((&timproc_uso_b5_D_807FFBB0), f32 *, 4) * sp8C.f4) + (M2C_FIELD((&timproc_uso_b5_D_807FFBB0), f32 *, 8) * sp8C.f8)) < M2C_FIELD((&timproc_uso_b5_D_807FF370), f64 *, 0x350)) {
        VecF sp48;
        f32 ang;
        VecF sp38;
        VecF sp2C;
        M2C_FIELD(arg0, f32 *, 0xF4) = 0.0f;
        M2C_FIELD(arg0, f32 *, 0xF8) = 0.0f;
        M2C_FIELD(arg0, f32 *, 0xFC) = 0.0f;
        M2C_FIELD(arg0, f32 *, 0x100) = 1.0f;
        ang = M2C_FIELD(arg0, f32 *, 0x114);
        sp48.f8 = 0.0f;
        sp48.f4 = 0.0f;
        sp48.f0 = 1.0f;
        timproc_uso_b5_func_071AE0(arg0 + 0xF4, &sp48, ang);
        ang = M2C_FIELD(arg0, f32 *, 0x110);
        sp38.f0 = 0.0f;
        sp38.f4 = 0.0f;
        sp38.f8 = 1.0f;
        timproc_uso_b5_func_071AE0(arg0 + 0xF4, &sp38, ang);
        ang = M2C_FIELD(arg0, f32 *, 0x118);
        sp2C.f0 = 0.0f;
        sp2C.f8 = 0.0f;
        sp2C.f4 = 1.0f;
        timproc_uso_b5_func_071AE0(arg0 + 0xF4, &sp2C, ang);
        timproc_uso_b5_func_06970C_i(M2C_FIELD(arg0, s32 *, 0x108), M2C_FIELD(arg0, s32 *, 0x130));
        if ((M2C_FIELD(arg0, s32 *, 0x144) != 0) || (M2C_FIELD(arg0, s32 *, 0x138) != 0)) {
            timproc_uso_b5_func_054FE0(arg0);
            timproc_uso_b5_D_807FFBBC += 1;
            if (M2C_FIELD(arg0, s32 *, 0x144) != 0) M2C_FIELD(arg0, s32 *, 0x144) -= 1;
            if (M2C_FIELD(arg0, s32 *, 0x144) == 0) {
                if (M2C_FIELD(M2C_FIELD(arg0, char **, 0x10C), s32 *, 0xB0) != 0) {
                    timproc_uso_b5_func_077C44(M2C_FIELD(arg0, char **, 0x10C), M2C_FIELD(arg0, s32 *, 0x140), 0, 2, 2, 1);
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
