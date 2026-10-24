/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for timproc_uso_b5_func_0000B368
 * (174 words, target 173). NM object only (build/non_matching); the ROM build keeps
 * INCLUDE_ASM. The cfe NM body lifted under NCC with the offset / scale vector read
 * from file-local statics (kit LX: x / y one object through references, z a second
 * one), as in the B624 donor: 175 -> 174 words, 87.47 -> 89.3 fuzzy (agent-g run 142).
 * RESIDUAL: head load order / shared-lui slot, FP colours in the rotate tail.
 * Run 156 (kit LXXXVI): head differences through held block locals (target keeps them in
 * f2 / f12 / f14 and loads the statics first): 89.58 -> 92.71, 174 -> 173 words = target.
 * Run 157: rewritten as C++ vector code. Every m2c "struct copy chain" (d_272 -> c1_292 ->
 * c2_308, s92 -> c1 -> s240, s64 -> c76 -> s224) is an inline function returning a named
 * non-POD V by value (r -> return temp -> destination); the three results are held in
 * locals; the second scale takes its factor by const reference, which gives it its own
 * return temp (sp+0x4C) as in the target; pos += through V::operator+=; the frame-only
 * locals declared in the if arm in target order: 92.71 -> 95.71, 173 words.
 * RESIDUAL: homes (target has 13 more frame words in the arm), arg0 copied to s0 before the
 * head, the 1.0f axis constant in a temp (target f12), FP ring offset in the tail. */
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef double f64;
#define M2C_FIELD(ptr, type, off) (*(type)((char *)(ptr) + (off)))
typedef struct { f32 f0; f32 f4; f32 f8; } Vec154;
extern "C" int func_00000000(...);
extern "C" {
extern char D_00000000;
extern void timproc_uso_b5_func_00006E08(char *arg0, s32 arg1);
extern int timproc_uso_b5_func_0546DC(char *, char *, f32, f32, f32);
extern void *timproc_uso_b5_func_071028(...);
extern void timproc_uso_b5_func_071AE0(void *quat, void *axis, f32 angle);
extern void timproc_uso_b5_func_054CAC(void *obj);

extern f32 timb5_B368_gscale;
extern void timproc_uso_b5_func_06970C_r(...);
}
static f32 sXY[2];
static f32 sZ;
struct V { f32 x, y, z; V() {} V &operator+=(const V &v) { x += v.x; y += v.y; z += v.z; return *this; } };
static inline V relPos(const V &a) {
    f32 &x = sXY[0]; f32 &y = sXY[1];
    V r; f32 dx = a.x - x; f32 dy = a.y - y; f32 dz = a.z - sZ;
    r.x = dx; r.y = dy; r.z = dz;
    return r;
}
static inline V poolScaled(const f32 &s) {
    f32 &x = sXY[0]; f32 &y = sXY[1];
    V r; f32 rx = x * s; f32 ry = y * s; f32 rz = sZ * s;
    r.x = rx; r.y = ry; r.z = rz;
    return r;
}
static inline V scaled3(const V &a, const f32 &s) {
    V r; f32 rx = a.x * s; f32 ry = a.y * s; f32 rz = a.z * s;
    r.x = rx; r.y = ry; r.z = rz;
    return r;
}
extern "C" void timproc_uso_b5_func_0000B368(char *arg0) {
    V d;

    d = relPos(*(V *) (arg0 + 0xDC));
    timproc_uso_b5_func_071028(&d);
    if (M2C_FIELD(arg0, s32 *, 0x140) != 0) {
        V s240;
        V s224;
        V ax1;
        V ax2;
        V ax3;
        V s120;
        V s104;
        f32 ang;
        M2C_FIELD(arg0, f32 *, 0xF4) = 0.0f;
        M2C_FIELD(arg0, f32 *, 0xF8) = 0.0f;
        M2C_FIELD(arg0, f32 *, 0xFC) = 0.0f;
        M2C_FIELD(arg0, f32 *, 0x100) = 1.0f;
        ang = M2C_FIELD(arg0, f32 *, 0x110);
        ax1.z = 0.0f;
        ax1.y = 0.0f;
        ax1.x = 1.0f;
        timproc_uso_b5_func_071AE0(arg0 + 0xF4, &ax1, ang);
        ang = M2C_FIELD(arg0, f32 *, 0x10C);
        ax2.x = 0.0f;
        ax2.y = 0.0f;
        ax2.z = 1.0f;
        timproc_uso_b5_func_071AE0(arg0 + 0xF4, &ax2, ang);
        ang = M2C_FIELD(arg0, f32 *, 0x114);
        ax3.x = 0.0f;
        ax3.z = 0.0f;
        ax3.y = 1.0f;
        timproc_uso_b5_func_071AE0(arg0 + 0xF4, &ax3, ang);
        if (M2C_FIELD(arg0, s32 *, 0x13C) != 0) {
            func_00000000(M2C_FIELD(*(char **) &D_00000000, char **, 0x70) + 0xB4, &s120);
            func_00000000(&s120, &s104);
            func_00000000(arg0 + 0xF4, &s104, M2C_FIELD(arg0, s32 *, 0x128), 0);
        }
        M2C_FIELD(arg0, f32 *, 0xE8) = M2C_FIELD(arg0, f32 *, 0x118);
        M2C_FIELD(arg0, f32 *, 0xEC) = M2C_FIELD(arg0, f32 *, 0x118);
        M2C_FIELD(arg0, f32 *, 0xF0) = M2C_FIELD(arg0, f32 *, 0x118);
        {
            s224 = scaled3(s240 = poolScaled(timb5_B368_gscale), M2C_FIELD(arg0, f32 *, 0x128));
            *(V *) (arg0 + 0xDC) += s224;
        }
        timproc_uso_b5_func_06970C_r(M2C_FIELD(arg0, void **, 0x108), M2C_FIELD(arg0, s32 *, 0x124));
        timproc_uso_b5_func_054CAC(arg0);
        M2C_FIELD(arg0, s32 *, 0x12C) = 1;
    }
}
