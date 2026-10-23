/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00025C54: the NM C body lifted under NCC + one parameter empty test from a fz sweep (agent-c run 134, cfe 76.23).
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
#undef NULL
#define NULL 0
#define FW(p, o) (*(int *)((char *)(p) + (o)))
typedef struct { int w0, w1; } GfxPair1EE78;
extern "C" char DB[];
extern "C" {
extern int gl_func_00000000(...);
extern int *a1;
extern char *rec;
extern char *gl_1dcb4_f4(char *, s16, s16);
extern char *gl_1dcb4_f5(char *, s32, s32);
extern GfxPair1EE78 *p;
extern GfxPair1EE78 *r;
extern int gl_ref_0003959C(...);
extern int gl_ref_0003A268(...);
}
extern "C" void gl_func_00025C54(s32 arg0, s32 arg1, char *arg2) {
    s32 sp4C;
    s32 var_s1;
    s32 var_s2;
    s32 var_t0;
    s32 i;
    s32 n;
    char *temp_v0;
    char *rec;
    s32 w;
    u32 typ;

    sp4C = 0;
    if (FW(DB, 0x1034) != 0) {
        sp4C = 1;
    }
    FW(DB, 0x1030) = 0;
    var_s2 = 0;
    temp_v0 = (char *)FW(DB, 0x2030) + (arg0 * 0x14);
    i = *(u8 *)(temp_v0 + 0x0);
    n = *(u8 *)(temp_v0 + 0x1);
    if (i > 0) {
        do {
            char *e = (char *)gl_func_00000000(arg0, var_s2);
            if (e != 0) {
                if (*(u8 *)(e + 0x1) != 0) {
                    gl_ref_0003A268(e + 8); if (arg1) {}
                }
                if (*(u8 *)(e + 0x2) != 0x7F) {
                    gl_ref_0003A268(e + 0x18);
                }
                gl_ref_0003A268(e + 0x10);
            }
            var_s2 += 1;
        } while (var_s2 != i);
        var_s2 = 0;
    }
    if (n > 0) {
        do {
            char *e = (char *)gl_func_00000000(arg0, var_s2);
            if (e != 0) {
                gl_ref_0003A268(e + 4);
            }
            var_s2 += 1;
        } while (var_s2 != n);
    }
    {
        s32 a1 = FW(DB, 0x1030);
        var_s1 = 0;
        var_s2 = 0;
        if (a1 != 0) {
            var_t0 = FW(DB, 0x1034);
            if (a1 > 0) {
                char *p = DB + 0x430;
                char *end = DB + 0x430 + (a1 * 4);
                s32 total = 0;
                do {
                    total += ((FW(*(int *)p, 0x0) & 0xFFFFFF) + 15) & -16;
                    p += 4;
                } while ((u32) p < (u32) end);
                var_s2 = 0;
            }
            if (a1 > 0) {
                var_s1 = FW(DB, 0x5C);
                do {
                    if (var_t0 == 0x78) {
                        break;
                    }
                    rec = (char *)FW(DB + (var_s2 * 4), 0x430);
                    w = FW(rec, 0x0);
                    typ = (u32) (w << 4) >> 0x1E;
                    if (typ != 0) {
                        switch (arg1) {
                        case 0:
                            if (FW(arg2, 0x10) == typ) {
                                var_s1 = gl_func_00000000(w & 0xFFFFFF, FW(arg2, 0x0), FW(rec, 0x4), (s8) typ);
                            } else if (FW(arg2, 0x14) == typ) {
                                var_s1 = gl_func_00000000(w & 0xFFFFFF, FW(arg2, 0x4), FW(rec, 0x4), (s8) typ);
                            }
                            break;
                        case 1:
                            if (FW(arg2, 0x10) == typ) {
                                var_s1 = gl_func_00000000(w & 0xFFFFFF, FW(arg2, 0x0), FW(rec, 0x4), (s8) typ);
                            } else if (FW(arg2, 0x14) == typ) {
                                var_s1 = gl_func_00000000(w & 0xFFFFFF, FW(arg2, 0x4), FW(rec, 0x4), (s8) typ);
                            }
                            break;
                        }
                        if (var_s1 == 0) {
                            var_t0 = FW(DB, 0x1034);
                        } else {
                            switch (arg1) {
                            case 0:
                                w = FW(rec, 0x0);
                                typ = (u32) (w << 4) >> 0x1E;
                                if (typ == 1) {
                                    gl_func_00000000(FW(rec, 0x4), var_s1, w & 0xFFFFFF, *(s16 *)((char *)FW(DB, 0x2024) + 0x2));
                                } else {
                                    gl_func_00000000(FW(rec, 0x4), var_s1, w & 0xFFFFFF, (s8) typ);
                                }
                                FW(rec, 0x4) = var_s1;
                                *(s8 *)(rec + 0x0) = (u8) *(u8 *)(rec + 0x0) & ~0x0C;
                                var_t0 = FW(DB, 0x1034);
                                break;
                            case 1:
                                var_t0 = FW(DB, 0x1034);
                                FW(DB + (var_t0 * 0x14), 0x634) = (s32) rec;
                                FW(DB + (FW(DB, 0x1034) * 0x14), 0x638) = var_s1;
                                var_t0 = FW(DB, 0x1034);
                                FW(DB + (var_t0 * 0x14), 0x63C) = (s32) ((var_t0 << 0x18) | 0xFFFFFF);
                                FW(DB + (FW(DB, 0x1034) * 0x14), 0x640) = 0;
                                w = FW(rec, 0x0);
                                FW(DB + (FW(DB, 0x1034) * 0x14), 0x630) =
                                    FW(rec, 0x4) + (w & 0xFFFFFF) + ((u32) (w << 4) >> 0x1E);
                                var_t0 = FW(DB, 0x1034);
                                FW(DB, 0x1034) = var_t0 + 1;
                                var_t0 = FW(DB, 0x1034);
                                break;
                            default:
                                var_t0 = FW(DB, 0x1034);
                                break;
                            }
                        }
                    }
                    var_s2 += 1;
                } while (var_s2 < FW(DB, 0x1030));
                FW(DB, 0x5C) = var_s1;
            }
            FW(DB, 0x1030) = 0;
            if ((var_t0 != 0) && (sp4C == 0)) {
                char *r = (char *)FW(DB + (var_t0 * 0x14), 0x620);
                w = FW(r, 0x0);
                gl_ref_0003959C(FW(r, 0x4), FW(DB + (var_t0 * 0x14), 0x624),
                                w & 0xFFFFFF, (u32) (w << 4) >> 0x1E,
                                ((u32) (w & 0xFFFFFF) >> 0xC) + 1, DB + 0x1684,
                                FW(DB + (var_t0 * 0x14), 0x628));
            }
        }
    }
}
