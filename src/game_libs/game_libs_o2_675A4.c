/* IDO 7.1 -O2 donor for gl_func_000675A4 -- input-debounce / flag-update
 * routine (0x418 / 262 words). PROVEN EXACT 262/262 words; the link-resolved
 * words are the blank jal and the HI/LO pair of the dead-zone literal.
 *
 * WIRED via REPLACE_FUNC_BODY donor splice into game_libs_post1b.c.o (the
 * 3C0C / 2B94 recipe): the stick dead-zone threshold is the DOUBLE LITERAL
 * 0.2 (bootup.uso RoData +0x2200 holds 0x3FC999999999999A after the
 * section's leading marker word), not a data variable. As a literal it is a
 * uopt constant candidate coloured after the 0.0f literal (target zero f12 /
 * threshold f14); the old named `f64 temp_f14 = gl_D_2200_thr.v` web was
 * coloured first and swapped them (the 8-word residue). The donor-local
 * .rodata reloc is renamed gl_func_000675A4_rodata and pinned to +0x2200.
 *
 * Load-bearing shapes kept from the NM body (docs/IDO_CODEGEN.md
 * #store-forward-ring-across-bb-and-scaled-index-first-675a4): the 0x18
 * store + re-read, the scaled-index-first ==1 cursor, `(cur | 0) & ~prev`,
 * and the dead `while (0) { temp_f2 = y; }` anchor. */
#include "common.h"

extern int D_00000000;
extern int gl_func_00000000();

#ifndef FW
#define FW(p, o) (*(int *)((char *)(p) + (o)))
#endif
void gl_func_000675A4(char *arg0) {
    f32 temp_f0;
    f32 temp_f2;
    f32 var_f0;
    s16 temp_v0;
    s32 temp_a0;
    char *temp_t0;
    s32 temp_t4;
    s32 temp_v0_2;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 temp_v0_6;
    s32 temp_v0_7;
    s32 temp_v1;
    s32 temp_v1_2;
    u32 temp_t3;
    char *temp_v0_3;

    temp_v1 = FW(arg0, 0x34);
    if ((temp_v1 != 0) && (FW(arg0, 0x68) == 0)) {
        temp_t0 = (char *) FW(arg0, 0x78);
        if (temp_t0 != 0) {
            temp_v0 = (*(s16*)((char*)arg0 + 0x74));
            (*(s16*)((char*)arg0 + 0x74)) = (s16) (temp_v0 + 1);
            gl_func_00000000(&D_00000000, temp_v1 - 1, ((u8*)temp_t0)[temp_v0], arg0);
            if (*(u8*)((*(s16*)((char*)arg0 + 0x74)) + FW(arg0, 0x78)) == 0xFF) {
                FW(arg0, 0x78) = 0;
                (*(s16*)((char*)arg0 + 0x76)) = 0;
            }
        }
    }
    FW(arg0, 0x14) = FW(arg0, 0x10);
    FW(arg0, 0x10) = 0;
    if (FW(arg0, 0x68) == 0) {
        if (FW(arg0, 0x34) != 0) {
            gl_func_00000000(&D_00000000, arg0);
        }
        if (FW(arg0, 0x70) != 0) {
            (*(s8*)((char*)arg0 + 0x8)) = (s8) -(*(s8*)((char*)arg0 + 0x8));
        }
        if (FW(arg0, 0x4C) != 0) {
            FW(arg0, 0x50) = FW(arg0, 0x50) + 1;
            if ((u32) FW(arg0, 0x50) >= (u32) FW(arg0, 0x54)) {
                FW(arg0, 0x4C) = 0;
            }
            temp_v0_2 = FW(arg0, 0x4C);
            if (temp_v0_2 == 2) {
                *(s8*)(FW(arg0, 0x5C) + (FW(arg0, 0x50) * 4)) = (*(s8*)((char*)arg0 + 0x8));
                *(s8*)((FW(arg0, 0x5C) + (FW(arg0, 0x50) * 4)) + 0x1) = (*(s8*)((char*)arg0 + 0x9));
                temp_t4 = FW(arg0, 0x6C) & FW(arg0, 0x10) & 0x1700;
                *(s16*)((FW(arg0, 0x5C) + (FW(arg0, 0x50) * 4)) + 0x2) = temp_t4;
                *(int*)(FW(arg0, 0x58)) = FW(arg0, 0x50);
            } else if (temp_v0_2 == 1) {
                temp_v0_3 = (char *)((FW(arg0, 0x50) * 4) + FW(arg0, 0x5C));
                FW(arg0, 0x10) = (s32) (*(s16*)((char*)temp_v0_3 + 0x2) | FW(arg0, 0x10));
                (*(s8*)((char*)arg0 + 0x8)) = *(s8*)((char*)temp_v0_3 + 0x0);
                (*(s8*)((char*)arg0 + 0x9)) = *(s8*)((char*)temp_v0_3 + 0x1);
            }
        }
        while (0) { temp_f2 = (*(f32*)((char*)arg0 + 0x4)); }
        temp_f0 = (f32) (*(s8*)((char*)arg0 + 0x8)) / 80.0f;
        (*(f32*)((char*)arg0 + 0x0)) = temp_f0;
        (*(f32*)((char*)arg0 + 0x4)) = (f32) ((f32) (*(s8*)((char*)arg0 + 0x9)) / 80.0f);
        if (temp_f0 < 0.0f) {
            var_f0 = -(*(f32*)((char*)arg0 + 0x0));
        } else {
            var_f0 = (*(f32*)((char*)arg0 + 0x0));
        }
        if ((f64) var_f0 < 0.2) {
            (*(f32*)((char*)arg0 + 0x0)) = 0.0f;
        }
        temp_f2 = (*(f32*)((char*)arg0 + 0x4));
        if (temp_f2 < 0.0f) {
            var_f0 = -temp_f2;
        } else {
            var_f0 = temp_f2;
        }
        if ((f64) var_f0 < 0.2) {
            (*(f32*)((char*)arg0 + 0x4)) = 0.0f;
        }
    }
    if (FW(arg0, 0x3C) != 0) {
        FW(arg0, 0x10) = (s32) (FW(arg0, 0x40) | FW(arg0, 0x10));
        FW(arg0, 0x3C) = 0;
        (*(f32*)((char*)arg0 + 0x0)) = (f32) (*(f32*)((char*)arg0 + 0x44));
        (*(f32*)((char*)arg0 + 0x4)) = (f32) (*(f32*)((char*)arg0 + 0x48));
    }
    if ((f64) (*(f32*)((char*)arg0 + 0x0)) > 0.5) {
        FW(arg0, 0x10) = (s32) (FW(arg0, 0x10) | 0x4000);
    } else {
        FW(arg0, 0x10) = (s32) (FW(arg0, 0x10) & ~0x4000);
    }
    if ((f64) (*(f32*)((char*)arg0 + 0x0)) < -0.5) {
        FW(arg0, 0x10) = (s32) (FW(arg0, 0x10) | 0x10000);
    } else {
        FW(arg0, 0x10) = (s32) (FW(arg0, 0x10) & 0xFFFEFFFF);
    }
    if ((f64) (*(f32*)((char*)arg0 + 0x4)) > 0.5) {
        FW(arg0, 0x10) = (s32) (FW(arg0, 0x10) | 0x2000);
    } else {
        FW(arg0, 0x10) = (s32) (FW(arg0, 0x10) & ~0x2000);
    }
    if ((f64) (*(f32*)((char*)arg0 + 0x4)) < -0.5) {
        FW(arg0, 0x10) = (s32) (FW(arg0, 0x10) | 0x8000);
    } else {
        FW(arg0, 0x10) = (s32) (FW(arg0, 0x10) & 0xFFFF7FFF);
    }
    temp_v0_4 = FW(arg0, 0x10);
    temp_v1_2 = FW(arg0, 0x14);
    temp_a0 = FW(arg0, 0x38);
    FW(arg0, 0x18) = (temp_v0_4 | 0) & ~temp_v1_2;
    FW(arg0, 0x1C) = (s32) (~temp_v0_4 & temp_v1_2);
    if (temp_a0 != 0) {
        temp_v0_5 = temp_a0 - 1;
        FW(arg0, 0x38) = temp_v0_5;
        if (temp_v0_5 > 0) {
            temp_v0_6 = FW(arg0, 0x18) & FW(arg0, 0x24);
            FW(arg0, 0x20) = temp_v0_6;
            if (temp_v0_6 != 0) {
                FW(arg0, 0x38) = 0;
            }
        }
    } else {
        temp_v0_7 = FW(arg0, 0x18);
        FW(arg0, 0x20) = 0;
        FW(arg0, 0x24) = temp_v0_7;
        if (temp_v0_7 != 0) {
            FW(arg0, 0x38) = 0xA;
        }
    }
    (*(s16*)((char*)arg0 + 0x7C)) = (s16) *(u8*)((char*)&D_00000000 + ((u32)(FW(arg0, 0x10) & 0x1E000) >> 0xD));
}
