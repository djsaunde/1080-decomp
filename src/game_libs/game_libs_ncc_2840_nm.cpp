/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for gl_func_00002840 (110 words;
 * this body 110 = target). NM object only; the ROM build keeps INCLUDE_ASM. cfe body
 * under NCC + `if (arg2) {}`: 89.75 -> 90.81. Run 139: the target stores dy into its
 * stack home before the sqrt call and reloads it after (no spill-slot copy): declaring
 * sp38 VOLATILE reproduces that and fixes the length (111 -> 110); sp3C / sp38 moved
 * after f18 and len / dx / dy before p put both homes at 60 / 56 (target). Two
 * parameter empty tests (after the sqrt call and after vx = sp3C) 92.45 -> 92.82.
 * RESIDUAL: sp3C is still register-allocated (spill at 32, home 92; the target
 * keeps it in its home at 60 like sp38); making sp3C volatile too fixes that but
 * shifts the whole FP colouring (f16 / f18 locals -> f14 / f16): 83.1. p spills at
 * 32 (target 40). */
typedef int s32;
typedef float f32;
#define FW(p, o) (*(int *)((char *)(p) + (o)))
extern "C" {
extern int D_00000000;
extern int gl_func_00000000(...);
extern f32 gl_func_00000000_sqf(f32);
}
extern "C" void gl_func_00002840(char *arg0, s32 arg1, f32 arg2, f32 arg3, s32 arg4) {
    f32 vec[4];
    f32 vx;
    f32 vy;
    f32 b4;
    f32 d4;
    f32 f0;
    f32 f16;
    f32 f2;
    f32 f18;
    f32 sp3C;
    volatile f32 sp38;
    f32 len;
    f32 dx;
    f32 dy;
    char *p;
    char *temp_v0;

    temp_v0 = (char *)FW(FW(((*(s32*)((char*)&D_00000000 + 0x134)) + (arg1 * 4)), 0x108), 0x70);
    f0 = (*(f32*)((char*)temp_v0 + 0xBC));
    f16 = (*(f32*)((char*)temp_v0 + 0xA0));
    f2 = (*(f32*)((char*)temp_v0 + 0xDC));
    f18 = (*(f32*)((char*)temp_v0 + 0xA8));
    if ((((f16 * f0) + (f18 * f2)) < ((f0 * arg2) + (f2 * arg3))) && ((((f0 * arg2) + (f2 * arg3)) - ((f16 * f0) + (f18 * f2))) < 4000.0f)) {
        b4 = (*(f32*)((char*)temp_v0 + 0xB4));
        dx = arg2 - f16;
        dy = arg3 - f18;
        sp3C = dx;
        d4 = (*(f32*)((char*)temp_v0 + 0xD4));
        sp38 = dy;
        len = gl_func_00000000_sqf((dx * dx) + (dy * dy)); if (arg2) {}
        vx = sp3C; if (arg2) {}
        vy = sp38;
        if (len != 0.0f) {
            vx /= len;
            vy /= len;
        }
        p = arg0 + 0x2E4;
        vec[0] = 1.0f;
        vec[2] = 0.0f;
        vec[3] = 0.0f;
        sp3C = vx;
        sp38 = vy;
        vec[1] = (*(f32*)((char*)&D_00000000 + 0xCB4));
        gl_func_00000000(p);
        gl_func_00000000(p, (s32) (255.0f * (*(f32*)&D_00000000)), vec, 0xFF);
        gl_func_00000000(p, (s32) ((f32) (FW(arg0, 0x164) - 0xC) + (((b4 * sp3C) + (d4 * sp38)) * 150.0f)), arg4 - 8, 0);
    }
}
