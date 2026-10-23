/* IDO 7.1 NCC (C++) donor for timproc_uso_b5_func_00001F14 (0x89C / 551 words,
 * per-frame two-player transition update). Spliced into timproc_uso_b5.c.o via
 * REPLACE_FUNC_BODY; its .rodata literal pool (0.05f steps, 1.333f + 1.3333 * t
 * blends) is pinned to RoData +0xC8 as timproc_uso_b5_func_00001F14_rodata.
 * The inner 87A0 call of each copy pair takes three args (a3 keeps 2). */
typedef int s32;
typedef float f32;
typedef double f64;
extern "C" {
extern void timproc_uso_b5_func_00001DB0(char *);
extern int timproc_uso_b5_func_000087A0(...);
extern void timproc_uso_b5_func_04CD94(char *);
}
extern "C" void timproc_uso_b5_func_00001F14(char *arg0) {
    s32 dst;
    s32 frame;
    s32 frame2;
    s32 frame3;
    char *r34;
    char *r34b;
    char *r34c;
    char *r38;
    char *r38b;
    char *r38c;

    *(s32 *)(arg0 + 0x3C) = 0;
    if (*(s32 *)(arg0 + 0x30) == 2) {
        if (*(s32 *)(*(char **)(arg0 + 0x34) + 0x3CC) == 8) {
            *(s32 *)(arg0 + 0x3C) |= 1;
        }
        if (*(s32 *)(*(char **)(arg0 + 0x38) + 0x3CC) == 8) {
            *(s32 *)(arg0 + 0x3C) = *(s32 *)(arg0 + 0x3C) | 2;
        }
        if (*(s32 *)(arg0 + 0x3C) != 0) {
            if (*(s32 *)(arg0 + 0x3C) == 3) {
                if (*(s32 *)(arg0 + 0x44) == 0x78) {
                    if (*(s32 *)(arg0 + 0x40) == 1) {
                        *(s32 *)(arg0 + 0x4C) = 1;
                        *(s32 *)(arg0 + 0x48) = 1;
                        *(f32 *)(arg0 + 0x54) = 0.0f;
                    } else if (*(s32 *)(arg0 + 0x40) == 2) {
                        *(s32 *)(arg0 + 0x4C) = 1;
                        *(s32 *)(arg0 + 0x48) = 2;
                        *(f32 *)(arg0 + 0x54) = 0.0f;
                    }
                }
                if (*(s32 *)(arg0 + 0x48) == 1) {
                    dst = timproc_uso_b5_func_000087A0(*(char **)(arg0 + 0x38), 4, arg0);
                    *(s32 *)(*(s32 *)(arg0 + 0x38) + (dst * 4) + 0x3D0) =
                        ((s32 *)(*(s32 *)(arg0 + 0x34) + 0x3D0))[timproc_uso_b5_func_000087A0(*(char **)(arg0 + 0x34), 4, arg0)];
                } else {
                    dst = timproc_uso_b5_func_000087A0(*(char **)(arg0 + 0x34), 4, arg0);
                    *(s32 *)(*(s32 *)(arg0 + 0x34) + (dst * 4) + 0x3D0) =
                        ((s32 *)(*(s32 *)(arg0 + 0x38) + 0x3D0))[timproc_uso_b5_func_000087A0(*(char **)(arg0 + 0x38), 4, arg0)];
                }
                if (*(s32 *)(arg0 + 0x4C) != 0) {
                    if (*(s32 *)(arg0 + 0x48) == 1) {
                        *(f32 *)(arg0 + 0x54) = *(f32 *)(arg0 + 0x54) + 0.05f;
                        if (*(f32 *)(arg0 + 0x54) >= 1.0f) {
                            *(f32 *)(arg0 + 0x54) = 1.0f;
                            *(s32 *)(arg0 + 0x4C) = 0;
                        }
                        *(s32 *)(arg0 + 0x44) = (s32) (120.0f + (*(f32 *)(arg0 + 0x54) * 120.0f));
                        timproc_uso_b5_func_00001DB0(arg0);
                    }
                    if (*(s32 *)(arg0 + 0x48) == 2) {
                        *(f32 *)(arg0 + 0x54) = *(f32 *)(arg0 + 0x54) + 0.05f;
                        if (*(f32 *)(arg0 + 0x54) >= 1.0f) {
                            *(f32 *)(arg0 + 0x54) = 1.0f;
                            *(s32 *)(arg0 + 0x4C) = 0;
                        }
                        frame = (s32) (120.0f - (*(f32 *)(arg0 + 0x54) * 120.0f)); if (arg0) {}
                        *(s32 *)(arg0 + 0x44) = frame;
                        *(s32 *)(*(char **)(arg0 + 0x34) + 0x4D8) = frame;
                        r34 = *(char **)(arg0 + 0x34);
                        if (*(s32 *)(r34 + 0x4D8) == 0) {
                            *(s32 *)(*(s32 *)(*(s32 *)(r34 + 0x414) + 0xC) + 0xBC) = 0;
                        } else {
                            *(s32 *)(*(s32 *)(*(s32 *)(r34 + 0x414) + 0xC) + 0xC4) = 0;
                            *(s32 *)(*(s32 *)(*(s32 *)(*(char **)(arg0 + 0x34) + 0x414) + 0xC) + 0xD4) = 0;
                            *(s32 *)(*(s32 *)(*(s32 *)(*(char **)(arg0 + 0x34) + 0x414) + 0xC) + 0xBC) = *(s32 *)(arg0 + 0x44);
                            *(s32 *)(*(s32 *)(*(s32 *)(*(char **)(arg0 + 0x34) + 0x414) + 0xC) + 0xCC) = *(s32 *)(arg0 + 0x44);
                        }
                        *(s32 *)(*(char **)(arg0 + 0x38) + 0x4D8) = 0xF0 - *(s32 *)(arg0 + 0x44);
                        r38 = *(char **)(arg0 + 0x38);
                        if (*(s32 *)(r38 + 0x4D8) == 0) {
                            *(s32 *)(*(s32 *)(*(s32 *)(r38 + 0x414) + 0xC) + 0xBC) = 0;
                        } else {
                            *(s32 *)(*(s32 *)(*(s32 *)(r38 + 0x414) + 0xC) + 0xC4) = *(s32 *)(arg0 + 0x44);
                            *(s32 *)(*(s32 *)(*(s32 *)(*(char **)(arg0 + 0x38) + 0x414) + 0xC) + 0xD4) = *(s32 *)(arg0 + 0x44);
                            *(s32 *)(*(s32 *)(*(s32 *)(*(char **)(arg0 + 0x38) + 0x414) + 0xC) + 0xBC) = 0xF0 - *(s32 *)(arg0 + 0x44);
                            *(s32 *)(*(s32 *)(*(s32 *)(*(char **)(arg0 + 0x38) + 0x414) + 0xC) + 0xCC) = 0xF0 - *(s32 *)(arg0 + 0x44);
                            *(f32 *)(*(s32 *)(*(s32 *)(*(char **)(arg0 + 0x38) + 0x414) + 0xC) + 0xB4) =
                                1.333f + (1.3333 * (f64) (1.0f - *(f32 *)(arg0 + 0x54)));
                            *(f32 *)(*(char **)(arg0 + 0x38) + 0x49C) = 41.0f + (5.0f * *(f32 *)(arg0 + 0x54));
                        }
                    }
                }
            } else {
                if ((*(s32 *)(arg0 + 0x44) == 0) || (*(s32 *)(arg0 + 0x44) == 0xF0)) {
                    if (*(s32 *)(arg0 + 0x3C) == 1) {
                        *(s32 *)(arg0 + 0x4C) = 1;
                        *(s32 *)(arg0 + 0x48) = 1;
                        *(f32 *)(arg0 + 0x54) = 0.0f;
                    }
                    if (*(s32 *)(arg0 + 0x3C) == 2) {
                        *(s32 *)(arg0 + 0x4C) = 1;
                        *(s32 *)(arg0 + 0x48) = 2;
                        *(f32 *)(arg0 + 0x54) = 0.0f;
                    }
                }
                if (*(s32 *)(arg0 + 0x4C) != 0) {
                    if (*(s32 *)(arg0 + 0x48) == 1) {
                        *(f32 *)(arg0 + 0x54) = *(f32 *)(arg0 + 0x54) + 0.05f;
                        if (*(f32 *)(arg0 + 0x54) >= 1.0f) {
                            *(f32 *)(arg0 + 0x54) = 1.0f;
                            *(s32 *)(arg0 + 0x4C) = 0;
                            *(s32 *)(arg0 + 0x48) = 0;
                        }
                        frame2 = (s32) (240.0f - (*(f32 *)(arg0 + 0x54) * 120.0f)); if (arg0) {}
                        *(s32 *)(arg0 + 0x44) = frame2;
                        *(s32 *)(*(char **)(arg0 + 0x34) + 0x4D8) = frame2;
                        r34b = *(char **)(arg0 + 0x34);
                        if (*(s32 *)(r34b + 0x4D8) == 0) {
                            *(s32 *)(*(s32 *)(*(s32 *)(r34b + 0x414) + 0xC) + 0xBC) = 0;
                        } else {
                            *(s32 *)(*(s32 *)(*(s32 *)(r34b + 0x414) + 0xC) + 0xC4) = 0;
                            *(s32 *)(*(s32 *)(*(s32 *)(*(char **)(arg0 + 0x34) + 0x414) + 0xC) + 0xD4) = 0;
                            *(s32 *)(*(s32 *)(*(s32 *)(*(char **)(arg0 + 0x34) + 0x414) + 0xC) + 0xBC) = *(s32 *)(arg0 + 0x44);
                            *(s32 *)(*(s32 *)(*(s32 *)(*(char **)(arg0 + 0x34) + 0x414) + 0xC) + 0xCC) = *(s32 *)(arg0 + 0x44);
                            *(f32 *)(*(s32 *)(*(s32 *)(*(char **)(arg0 + 0x34) + 0x414) + 0xC) + 0xB4) =
                                1.333f + (1.3333 * (f64) *(f32 *)(arg0 + 0x54));
                            *(f32 *)(*(char **)(arg0 + 0x34) + 0x49C) = 41.0f + (5.0f * (1.0f - *(f32 *)(arg0 + 0x54)));
                        }
                        *(s32 *)(*(char **)(arg0 + 0x38) + 0x4D8) = 0xF0 - *(s32 *)(arg0 + 0x44);
                        r38b = *(char **)(arg0 + 0x38);
                        if (*(s32 *)(r38b + 0x4D8) == 0) {
                            *(s32 *)(*(s32 *)(*(s32 *)(r38b + 0x414) + 0xC) + 0xBC) = 0;
                        } else {
                            *(s32 *)(*(s32 *)(*(s32 *)(r38b + 0x414) + 0xC) + 0xC4) = *(s32 *)(arg0 + 0x44);
                            *(s32 *)(*(s32 *)(*(s32 *)(*(char **)(arg0 + 0x38) + 0x414) + 0xC) + 0xD4) = *(s32 *)(arg0 + 0x44);
                            *(s32 *)(*(s32 *)(*(s32 *)(*(char **)(arg0 + 0x38) + 0x414) + 0xC) + 0xBC) = 0xF0 - *(s32 *)(arg0 + 0x44);
                            *(s32 *)(*(s32 *)(*(s32 *)(*(char **)(arg0 + 0x38) + 0x414) + 0xC) + 0xCC) = 0xF0 - *(s32 *)(arg0 + 0x44);
                            *(f32 *)(*(s32 *)(*(s32 *)(*(char **)(arg0 + 0x38) + 0x414) + 0xC) + 0xB4) =
                                1.333f + (1.3333 * (f64) *(f32 *)(arg0 + 0x54));
                            *(f32 *)(*(char **)(arg0 + 0x38) + 0x49C) = 41.0f + (5.0f * (1.0f - *(f32 *)(arg0 + 0x54)));
                        }
                    }
                    if (*(s32 *)(arg0 + 0x48) == 2) {
                        *(f32 *)(arg0 + 0x54) = *(f32 *)(arg0 + 0x54) + 0.05f;
                        if (*(f32 *)(arg0 + 0x54) >= 1.0f) {
                            *(f32 *)(arg0 + 0x54) = 1.0f;
                            *(s32 *)(arg0 + 0x4C) = 0;
                            *(s32 *)(arg0 + 0x48) = 0;
                        }
                        frame3 = (s32) (*(f32 *)(arg0 + 0x54) * 120.0f); if (arg0) {}
                        *(s32 *)(arg0 + 0x44) = frame3;
                        *(s32 *)(*(char **)(arg0 + 0x34) + 0x4D8) = frame3;
                        r34c = *(char **)(arg0 + 0x34);
                        if (*(s32 *)(r34c + 0x4D8) == 0) {
                            *(s32 *)(*(s32 *)(*(s32 *)(r34c + 0x414) + 0xC) + 0xBC) = 0;
                        } else {
                            *(s32 *)(*(s32 *)(*(s32 *)(r34c + 0x414) + 0xC) + 0xC4) = 0;
                            *(s32 *)(*(s32 *)(*(s32 *)(*(char **)(arg0 + 0x34) + 0x414) + 0xC) + 0xD4) = 0;
                            *(s32 *)(*(s32 *)(*(s32 *)(*(char **)(arg0 + 0x34) + 0x414) + 0xC) + 0xBC) = *(s32 *)(arg0 + 0x44);
                            *(s32 *)(*(s32 *)(*(s32 *)(*(char **)(arg0 + 0x34) + 0x414) + 0xC) + 0xCC) = *(s32 *)(arg0 + 0x44);
                            *(f32 *)(*(s32 *)(*(s32 *)(*(char **)(arg0 + 0x34) + 0x414) + 0xC) + 0xB4) =
                                1.333f + (1.3333 * (f64) *(f32 *)(arg0 + 0x54));
                            *(f32 *)(*(char **)(arg0 + 0x34) + 0x49C) = 41.0f + (5.0f * (1.0f - *(f32 *)(arg0 + 0x54)));
                        }
                        *(s32 *)(*(char **)(arg0 + 0x38) + 0x4D8) = 0xF0 - *(s32 *)(arg0 + 0x44);
                        r38c = *(char **)(arg0 + 0x38);
                        if (*(s32 *)(r38c + 0x4D8) == 0) {
                            *(s32 *)(*(s32 *)(*(s32 *)(r38c + 0x414) + 0xC) + 0xBC) = 0;
                        } else {
                            *(s32 *)(*(s32 *)(*(s32 *)(r38c + 0x414) + 0xC) + 0xC4) = *(s32 *)(arg0 + 0x44);
                            *(s32 *)(*(s32 *)(*(s32 *)(*(char **)(arg0 + 0x38) + 0x414) + 0xC) + 0xD4) = *(s32 *)(arg0 + 0x44);
                            *(s32 *)(*(s32 *)(*(s32 *)(*(char **)(arg0 + 0x38) + 0x414) + 0xC) + 0xBC) = 0xF0 - *(s32 *)(arg0 + 0x44);
                            *(s32 *)(*(s32 *)(*(s32 *)(*(char **)(arg0 + 0x38) + 0x414) + 0xC) + 0xCC) = 0xF0 - *(s32 *)(arg0 + 0x44);
                            *(f32 *)(*(s32 *)(*(s32 *)(*(char **)(arg0 + 0x38) + 0x414) + 0xC) + 0xB4) =
                                1.333f + (1.3333 * (f64) *(f32 *)(arg0 + 0x54));
                            *(f32 *)(*(char **)(arg0 + 0x38) + 0x49C) = 41.0f + (5.0f * (1.0f - *(f32 *)(arg0 + 0x54)));
                        }
                    }
                }
            }
        }
        if (*(s32 *)(*(char **)(arg0 + 0x34) + 0x4D8) != 0) {
            *(s32 *)(arg0 + 0x50) = *(s32 *)(arg0 + 0x50) | 1;
        } else {
            *(s32 *)(arg0 + 0x50) = *(s32 *)(arg0 + 0x50) & ~1;
        }
        if (*(s32 *)(*(char **)(arg0 + 0x38) + 0x4D8) != 0) {
            *(s32 *)(arg0 + 0x50) = *(s32 *)(arg0 + 0x50) | 2;
        } else {
            *(s32 *)(arg0 + 0x50) = *(s32 *)(arg0 + 0x50) & ~2;
        }
        *(s32 *)(arg0 + 0x40) = *(s32 *)(arg0 + 0x3C);
    }
    timproc_uso_b5_func_04CD94(arg0);
}
