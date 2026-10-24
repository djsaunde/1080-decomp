#include "common.h"

/* game_libs_func_FFFFFFE0: the FIRST function of the game_libs text, bootup.uso
 * Text section offset 0x1464C (USO Sym export 871, ROM 0xDE50B8). Its first 8
 * words (the 255 / 235 / 80 / 0 FP constants and the first div.s, hoisted above
 * the prologue) sit 0x20 bytes BEFORE game_libs VRAM 0, so the function used to
 * be split across the old bootup_uso_post / game_libs_pre bins and the INCLUDE_ASM
 * gl_func_00000000 (0x1CC). It now lives in its own unit, linked as its own
 * output section ahead of .game_libs (which now starts at VRAM 0x1CC); the
 * blank-call placeholder gl_func_00000000 is pinned to 0 in
 * undefined_syms_auto.txt.
 *
 * Colour fade toward (235, 80, 80) / 255 by o->0x74 / 255, then three draw calls
 * on o->0xB0 and (if set) o->0xA0. cfe -O2, one pass:
 * - base[] / col[] are arrays (memory: base[3] is a dead store, base[1] / [2]
 *   are reloaded); r / g / b get homes (stored, then reloaded into col);
 * - the declaration order base, col, d255, r, g, b, t plus an unused int pad[4]
 *   gives the 0x70 frame (b / g / r at 64 / 68 / 72, col at 80, base at 96);
 * - every sum is written `product + field` (cfe emits `add.s field, product`);
 * - the 5-argument call takes a float at 16(sp): prototyped gl_func_00000000f. */
extern int gl_func_00000000();
extern int gl_func_00000000f(char *, int, int, int, f32, char *);
extern f32 D_00000000;

void game_libs_func_FFFFFFE0(char *o) {
    f32 base[4];
    f32 col[4];
    f32 d255 = 255.0f;
    f32 r, g, b;
    f32 t;
    int pad[4];

    base[0] = 235.0f / d255;
    base[1] = 80.0f / d255;
    base[2] = 80.0f / d255;
    base[3] = 0.0f / d255;
    t = (f32)*(int *)(o + 0x74);
    r = (base[0] - *(f32 *)(o + 0x64)) * t / d255 + *(f32 *)(o + 0x64);
    g = (base[1] - *(f32 *)(o + 0x68)) * t / d255 + *(f32 *)(o + 0x68);
    b = (base[2] - *(f32 *)(o + 0x6C)) * t / d255 + *(f32 *)(o + 0x6C);
    col[0] = r;
    col[1] = g;
    col[2] = b;
    col[3] = 1.0f;
    gl_func_00000000(*(char **)(o + 0xB0));
    gl_func_00000000(*(char **)(o + 0xB0), (int)((f32)*(int *)(o + 0x78) * D_00000000), col, 255);
    gl_func_00000000f(*(char **)(o + 0xB0), (int)(*(f32 *)(o + 0x44) + 2.0f), (int)*(f32 *)(o + 0x5C),
                      *(int *)(o + 0x88), *(f32 *)(o + 0x8C), o + 0xB4);
    if (*(char **)(o + 0xA0) != 0) {
        gl_func_00000000(*(char **)(o + 0xA0));
        gl_func_00000000(*(char **)(o + 0xA0), (int)((f32)*(int *)(o + 0x78) * D_00000000), col, 255);
        gl_func_00000000(*(char **)(o + 0xA0), (int)((f32)*(int *)(o + 0x80) + *(f32 *)(o + 0x44)),
                         (int)((f32)*(int *)(o + 0x84) + *(f32 *)(o + 0x5C)), 3);
    }
}
