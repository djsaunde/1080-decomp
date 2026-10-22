/* IDO 7.1 NCC (C++) donor for game_uso_func_0000D9CC (0x830 / 524 words).
 *
 * Per-frame landing/turn-state classifier. Under NCC two edg spellings fix
 * the cfe "andi temp coalesced into a1" t-ring cascade (kit XXI #2): the
 * 0x108 field is re-read through memory (no value local) in the `| 0x60000`
 * store and the `!= 0` test, and the three `vol * FF(arg0, K)` products of
 * the 0xA1C..0xA24 block are written field-first (edg keeps source operand
 * order).
 * The first gate is TWO nested ifs (x > 30.0f, then the 0x1F0 mask), not one
 * `&&`, and the early goto lands after BOTH closing braces. The mask test and
 * the 0x22C test branch to the inner end label, which is directly followed by
 * the outer one, so as1 finds no instruction to steal and keeps `beqz; nop` /
 * `bc1f; nop`; only the first gate (whose target starts with the 500.0f lui)
 * becomes `bc1fl` with the lui duplicated. Found with a branch-offset-aware
 * diff (docs/IDO_CODEGEN.md kit XXV); the masked diffs showed it as a
 * 4-word as1 cap.
 * The two doubles are data-symbol reads whose addends carry the %lo offsets
 * (0x1F8/0x200; the game_uso block is extracted unlinked); the host
 * placeholder in game_uso.c re-emits the .rodata words.
 * Spliced via REPLACE_FUNC_BODY into both trees. */
extern "C" {
typedef int s32; typedef unsigned short u16; typedef float f32; typedef double f64;
typedef struct { int a, b; } Pair2;
#define FW(p, o) (*(int *)((char *)(p) + (o)))
#define FF(p, o) (*(f32 *)((char *)(p) + (o)))
int game_uso_func_077C44(...);
void game_uso_func_0000D5DC(char *a0);
void game_uso_func_0000D5F8(char *a0, Pair2 pair, int a3);
void game_uso_func_0000D63C(char *a0, int a1);
void game_uso_func_07C0FC(...);
void game_uso_func_0000EDCC(int *a0, int a1);
void game_uso_func_0000EDD4(int *a0, unsigned int a1, char *a2);
void game_uso_func_0001189C(int *a0, int a1, char *a2);
extern char game_uso_D_807FF478;
extern char game_uso_D_807FF480;
extern char game_uso_D_807FF3B8;
extern char game_uso_D_807FF68C;
extern char game_uso_D_807FFB18;
extern char game_uso_D_807FFB20;
void game_uso_func_0000D9CC(char *arg0) {
    s32 var_a3;
    s32 sp30;
    char *sp2C;
    s32 sp28;
    f32 temp_f0;
    f32 var_f0;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 var_f0_4;
    f32 var_f12;
    f32 var_f2;
    char *temp_v0;
    char *temp_v0_2;
    char *temp_v0_3;
    char *var_v1;

    FW(arg0, 0x108) = 0;
    sp30 = 1;
    sp2C = 0;
    sp28 = 0;
    var_v1 = (char *)FW(arg0, 0xB4);
    var_a3 = 0;
    if (FF(var_v1, 0x348) > 30.0f) { if (*(u16 *)(var_v1 + 0xA10) & 0x1F0) {
        temp_f2 = FF(var_v1, 0xA1C);
        if (temp_f2 < 0.0f) {
            var_f0 = -temp_f2;
        } else {
            var_f0 = temp_f2;
        }
        if (FF(arg0, 0x244) < var_f0) {
            sp30 = 0;
            sp28 = 2;
            game_uso_func_0000EDCC((int *)arg0, 1);
            var_a3 = 3;
            if (FF(FW(arg0, 0xB4), 0xA1C) > 0.0f) {
                FW(arg0, 0x108) = (s32) (FW(arg0, 0xFC) | 0x16);
                sp2C = (char *)1;
            } else {
                FW(arg0, 0x108) = (s32) (FW(arg0, 0xFC) | 0x17);
                sp2C = (char *)3;
            }
            var_v1 = (char *)FW(arg0, 0xB4);
            goto after_24;
        }
        if (temp_f2 < 0.0f) {
            var_f0 = -temp_f2;
        } else {
            var_f0 = temp_f2;
        }
        if (FF(arg0, 0x22C) < var_f0) {
            sp30 = 0;
            game_uso_func_0000EDCC((int *)arg0, 4);
            var_v1 = (char *)FW(arg0, 0xB4);
            var_a3 = 2;
            temp_f2_2 = FF(var_v1, 0x970);
            if (temp_f2_2 < 0.0f) {
                var_f0 = -temp_f2_2;
            } else {
                var_f0 = temp_f2_2;
            }
            if (*(f64 *)((char *)&game_uso_D_807FFB18 + 0x1F8) < (f64) var_f0) {
                if (temp_f2_2 <= 0.0f) {
                    FW(arg0, 0x108) = (s32) (FW(arg0, 0xFC) | 0xE);
                    sp2C = (char *)3;
                } else {
                    FW(arg0, 0x108) = (s32) (FW(arg0, 0xFC) | 0xF);
                    sp2C = (char *)1;
                }
            } else if (FF(var_v1, 0xA1C) > 0.0f) {
                FW(arg0, 0x108) = (s32) (FW(arg0, 0xFC) | 0xC);
                sp2C = (char *)1;
            } else {
                FW(arg0, 0x108) = (s32) (FW(arg0, 0xFC) | 0xD);
                sp2C = (char *)3;
            }
            var_v1 = (char *)FW(arg0, 0xB4);
        }
    }
after_24:;
    }
    if (FF(var_v1, 0x9D0) < 500.0f) {
        temp_v0 = var_v1 + 0xCC;
        if ((f64) FF(var_v1, 0xA0C) < *(f64 *)((char *)&game_uso_D_807FFB20 + 0x200)) {
            FF(temp_v0, 0) = 0.0f; FF(temp_v0, 0x4) = 0.0f; FF(temp_v0, 0x8) = 0.0f; FF(temp_v0, 0xC) = 1.0f;
            sp28 = 2;
            var_a3 = 4;
            game_uso_func_0000EDCC((int *)arg0, 2);
            if (FF(FW(arg0, 0xB4), 0x3CC) < 0.0f) {
                FW(arg0, 0x108) = 0x1002A;
            } else {
                FW(arg0, 0x108) = 0x1002B;
            }
            var_v1 = (char *)FW(arg0, 0xB4);
        }
    }
    temp_v0_2 = var_v1 + 0x528;
    if (!(FW(var_v1, 0x9A8) & 1)) {
        var_f12 = FF(var_v1, 0xA1C) * FF(arg0, 0x11C);
        var_f2 = FF(var_v1, 0xA20) * FF(arg0, 0x11C);
        var_f0_4 = FF(var_v1, 0xA24) * FF(arg0, 0x11C);
        if (!(FW(var_v1, 0xA58) & 0x4000)) {
            var_f0_4 = 0.0f;
            var_f2 = 0.0f;
            var_f12 = 0.0f;
        }
        if ((FF(arg0, 0x304) * (*(volatile f32 *)(temp_v0_2 + 0x10))) <= var_f2) {
            sp28 = 3;
            sp30 = 0;
            game_uso_func_0000EDCC((int *)arg0, 2);
            var_a3 = 5;
            FW(arg0, 0x108) = (s32) (FW(arg0, 0xFC) | 0x15);
        } else if ((FF(arg0, 0x2EC) * (*(volatile f32 *)(temp_v0_2 + 0x10))) <= var_f2) {
            sp28 = 0;
            sp30 = 0;
            game_uso_func_0000EDCC((int *)arg0, 5);
            var_a3 = 1;
            FW(arg0, 0x108) = (s32) (FW(arg0, 0xFC) | 0x2D);
        } else if ((FF(arg0, 0x2D4) * (*(volatile f32 *)(temp_v0_2 + 0x10))) <= var_f2) {
            sp28 = 0;
            sp30 = 0;
            game_uso_func_0000EDCC((int *)arg0, 4);
            var_a3 = 1;
            FW(arg0, 0x108) = (s32) (FW(arg0, 0xFC) | 0x2C);
        } else if (FF(arg0, 0x2BC) <= var_f0_4) {
            sp28 = 4;
            game_uso_func_0000EDCC((int *)arg0, 3);
            var_a3 = 5;
            FW(arg0, 0x108) = (s32) (FW(arg0, 0xFC) | 0x15);
        } else if (FF(arg0, 0x2A4) <= var_f0_4) {
            sp28 = 3;
            game_uso_func_0000EDCC((int *)arg0, 2);
            var_a3 = 4;
            FW(arg0, 0x108) = (s32) (FW(arg0, 0xFC) | 0xB);
        } else if (FF(arg0, 0x28C) <= var_f0_4) {
            sp28 = 2;
            game_uso_func_0000EDCC((int *)arg0, 1);
            var_a3 = 3;
            FW(arg0, 0x108) = (s32) (FW(arg0, 0xFC) | 0xA);
        } else if (FF(arg0, 0x274) <= var_f0_4) {
            sp28 = 0;
            sp30 = 0;
            game_uso_func_0000EDCC((int *)arg0, 1);
            var_a3 = 1;
            FW(arg0, 0x108) = (s32) (FW(arg0, 0xFC) | 0x2C);
        } else if (var_f0_4 <= -FF(arg0, 0x334)) {
            sp28 = 2;
            sp2C = (char *)2;
            var_a3 = 3;
            sp30 = 0; game_uso_func_0000EDCC((int *)arg0, 5);
            FW(arg0, 0x108) = (s32) (FW(arg0, 0xFC) | 0x16);
        } else if (var_f0_4 <= -FF(arg0, 0x31C)) {
            sp28 = 0;
            sp2C = (char *)2;
            sp30 = 0;
            game_uso_func_0000EDCC((int *)arg0, 5);
            var_a3 = 1;
            FW(arg0, 0x108) = (s32) (FW(arg0, 0xFC) | 0x14);
        } else {
            temp_f0 = FF(arg0, 0x25C);
            if (temp_f0 <= var_f12) {
                sp28 = 3;
                game_uso_func_0000EDCC((int *)arg0, 2);
                var_a3 = 2;
                FW(arg0, 0x108) = (s32) (FW(arg0, 0xFC) | 0x13);
            } else if (var_f12 <= -temp_f0) {
                sp28 = 3;
                game_uso_func_0000EDCC((int *)arg0, 2);
                var_a3 = 2;
                FW(arg0, 0x108) = (s32) (FW(arg0, 0xFC) | 0x12);
            } else if ((*(f32 *)((char *)&game_uso_D_807FF68C + 4248) <= var_f2) && ((f64) FF(arg0, 0x11C) == 1.0) && (FW(var_v1, 0x9CC) == 0)) {
                sp28 = 0;
                sp30 = 0;
                FW(arg0, 0x108) = (s32) (FW(arg0, 0xFC) | 0x2F);
                game_uso_func_07C0FC(FW(var_v1, 0x800),
                                     (char *)&game_uso_D_807FF68C + 4252, 0xA);
            }
        }
    }
    if ((FW(arg0, 0x114) == 1) && (sp28 == 0)) {
        FW(arg0, 0x108) = 0;
    }
    if (FW(arg0, 0x108) != 0) {
        game_uso_func_0001189C((int *)arg0, (s32) var_a3, sp2C);
        FF(arg0, 0x11C) = 1.0f;
        game_uso_func_0000EDD4((int *)arg0, sp28, sp2C);
        temp_v0_3 = (char *)FW(arg0, 0xB4) + 0x2FC;
        FF(temp_v0_3, 0) = 0.0f; FF(temp_v0_3, 0x4) = 0.0f; FF(temp_v0_3, 0x8) = 0.0f; FF(temp_v0_3, 0xC) = 1.0f;
        game_uso_func_0000D63C(arg0, (sp28 == 0));
        FW(FW(arg0, 0xB4), 0x3DC) = 0;
        if (sp30 != 0) {
            game_uso_func_077C44(arg0, FW(arg0, 0x108), 0, 2, 1, 1);
            game_uso_func_0000D5F8(arg0, *(Pair2 *)((char *)&game_uso_D_807FF478 + 0xE88), 2);
        } else {
            game_uso_func_077C44(arg0, FW(arg0, 0x108), 0, 1, 1, 1);
            game_uso_func_0000D5F8(arg0, *(Pair2 *)((char *)&game_uso_D_807FF480 + 0xE90), 1);
        }
        game_uso_func_0000D5DC(arg0);
        FW(arg0, 0x114) = 0;
    }
    if (FW(FW(arg0, 0xB4), 0xA14) <= 0) {
        FW(arg0, 0x114) = 0;
        if (FW(arg0, 0x108) != 0) {
            FW(arg0, 0x108) = (FW(arg0, 0x108) & 0xFFFF) | 0x60000;
            game_uso_func_077C44(arg0, FW(arg0, 0x108), 0, 1, 1, 1);
            game_uso_func_0000D5F8(arg0, *(Pair2 *)((char *)&game_uso_D_807FF3B8 + 0xDC8), 1);
        }
        FW(FW(arg0, 0xB4), 0x960) = 0x64;
        game_uso_func_0000D5DC(arg0);
    }
}
}
