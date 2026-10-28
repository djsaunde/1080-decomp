/* IDO 7.1 NCC (C++) donor for timproc_uso_b5_func_000032C8 (0x5C8 / 370 words, composite panel
 * constructor: 6 text-row children, two vec3 children, two value children). Spliced into
 * timproc_uso_b5.c.o via REPLACE_FUNC_BODY (agent-g run 202). Ported from the EXACT sibling 2B74
 * donor (timproc_uso_b5_ncc_2B74.cpp): the same TimB5Nm name-struct copies, braced child blocks and
 * the `(cA = alloc(0x20)), (h = ...), (cA != 0)` fallback that re-assigns the zero vector value h
 * on both paths (the target's two `mtc1 zero,$f0` after the calls). The vec3 constants are source
 * literals (RoData +0x134..+0x148: -2500, -1500, 2500, 1500, -pi, pi); the donor's .rodata relocs
 * are renamed timproc_uso_b5_func_000032C8_rodata and pinned to +0x134. Levers over the plain port:
 * h = (f32)iz() (a distinct uopt value, not a folded constant: h keeps $f0 across the first vec
 * child), the decl list of the cfe body (cB above padF0 / padF1, no padF2: cB / t homes 0x50 / 0x3C),
 * and the first vec block as ONE source line with `if (1) {}` after the name load and after vA
 * (statement / split grid: the l13C load then precedes the h zero as in the target). The `if (1) {}`
 * after vC2 and after vD keep the -pi / pi loads one at a time in $f0. */
typedef int s32;
typedef float f32;
#define FW(p, o) (*(int *)((char *)(p) + (o)))
struct TimB5V3 { f32 x, y, z; };
struct TimB5Nm { char *v; };
#define B5_NAME(sym, lo) (*(char **)((char *)&sym + (lo)))
extern "C" {
extern int timproc_uso_b5_func_055750(...);
extern int timproc_uso_b5_func_04A188(...);
extern char timproc_uso_b5_D_807FE9E8;
extern char timproc_uso_b5_D_807FE9F8, timproc_uso_b5_D_807FE9FC, timproc_uso_b5_D_807FEA00, timproc_uso_b5_D_807FEA04,
    timproc_uso_b5_D_807FEA08, timproc_uso_b5_D_807FEA0C, timproc_uso_b5_D_807FEA10, timproc_uso_b5_D_807FEA14,
    timproc_uso_b5_D_807FEA18, timproc_uso_b5_D_807FEA1C;
extern char D_timb5_32C8_vt0[], D_timb5_32C8_vt1[], D_timb5_32C8_vt2[], D_timb5_32C8_vt3[], D_timb5_32C8_vt4[], D_timb5_32C8_vt5[];
extern char D_timb5_32C8_vv0, D_timb5_32C8_vv1;
extern char D_timb5_32C8_vw0, D_timb5_32C8_vw1;
}
inline int iz() { return 0; }
extern "C" char *timproc_uso_b5_func_000032C8(char *arg0) {
    struct TimB5Nm u0;
    struct TimB5Nm u1;
    struct TimB5Nm u2;
    struct TimB5Nm u3;
    struct TimB5Nm u4;
    struct TimB5Nm u5;
    struct TimB5V3 vC;
    struct TimB5V3 vA;
    struct TimB5V3 vB;
    struct TimB5Nm u12;
    struct TimB5V3 vC2;
    struct TimB5V3 vD;
    struct TimB5V3 vE;
    struct TimB5Nm u13;
    struct TimB5Nm u14;
    struct TimB5Nm u15;
    volatile s32 padA1;
    volatile s32 padA2;
    volatile s32 padA3;
    char *c;
    struct TimB5Nm nm;
    volatile s32 padB0;
    volatile s32 padB1;
    volatile s32 padB2;
    volatile s32 padB3;
    volatile s32 padB4;
    volatile s32 padB5;
    volatile s32 padB6;
    char *cA;
    volatile s32 padC0;
    struct TimB5V3 cp0;
    volatile s32 padD0;
    struct TimB5V3 cp1;
    volatile s32 padE0;
    struct TimB5V3 cp2;
    char *cB;
    volatile s32 padF0;
    volatile s32 padF1;
    struct TimB5V3 t;
    volatile s32 padG0;
    volatile s32 padG1;
    volatile s32 padG2;
    f32 h;
    char *vt;

    if (1) { struct TimB5Nm *q = &u0; q = &u1; q = &u2; q = &u3; q = &u4; q = &u5; q = &u12; q = &u13; q = &u14; q = &u15; }

    if (arg0 != 0 || (arg0 = (char *)timproc_uso_b5_func_055750(0x108)) != 0) {
        vt = arg0;
        if (vt != 0 || (vt = (char *)timproc_uso_b5_func_055750(8)) != 0) {
            FW(vt, 0x0) = (int)((char *)&timproc_uso_b5_D_807FE9E8 + 0x1178);
            FW(vt, 0x4) = 0;
        }
        u0.v = B5_NAME(timproc_uso_b5_D_807FE9F8, 0x1188); nm = u0;
        {
            if ((c = arg0 + 0x8) != 0 || (c = (char *)timproc_uso_b5_func_055750(0x18)) != 0) {
                timproc_uso_b5_func_04A188(c, arg0, nm, 1);
                (FW(c, 0xC) = (int)((char *)&D_timb5_32C8_vt0 + 0x1A0), FW(c, 0x14) = 0, FW(c, 0x10) = 0);
            }
        }
        u1.v = B5_NAME(timproc_uso_b5_D_807FE9FC, 0x118C); nm = u1;
        {
            if ((c = arg0 + 0x20) != 0 || (c = (char *)timproc_uso_b5_func_055750(0x18)) != 0) {
                timproc_uso_b5_func_04A188(c, arg0, nm, 1);
                (FW(c, 0xC) = (int)((char *)&D_timb5_32C8_vt1 + 0x1A0), FW(c, 0x10) = 2, FW(c, 0x14) = 0);
            }
        }
        u2.v = B5_NAME(timproc_uso_b5_D_807FEA00, 0x1190); nm = u2;
        {
            if ((c = arg0 + 0x38) != 0 || (c = (char *)timproc_uso_b5_func_055750(0x18)) != 0) {
                timproc_uso_b5_func_04A188(c, arg0, nm, 1);
                (FW(c, 0xC) = (int)((char *)&D_timb5_32C8_vt2 + 0x1A0), FW(c, 0x10) = 2, FW(c, 0x14) = 0);
            }
        }
        u3.v = B5_NAME(timproc_uso_b5_D_807FEA04, 0x1194); nm = u3;
        {
            if ((c = arg0 + 0x50) != 0 || (c = (char *)timproc_uso_b5_func_055750(0x18)) != 0) {
                timproc_uso_b5_func_04A188(c, arg0, nm, 1);
                (FW(c, 0xC) = (int)((char *)&D_timb5_32C8_vt3 + 0x1A0), FW(c, 0x10) = 2, FW(c, 0x14) = 0);
            }
        }
        u4.v = B5_NAME(timproc_uso_b5_D_807FEA08, 0x1198); nm = u4;
        {
            if ((c = arg0 + 0x68) != 0 || (c = (char *)timproc_uso_b5_func_055750(0x18)) != 0) {
                timproc_uso_b5_func_04A188(c, arg0, nm, 1);
                (FW(c, 0xC) = (int)((char *)&D_timb5_32C8_vt4 + 0x1A0), FW(c, 0x10) = 2, FW(c, 0x14) = 0);
            }
        }
        u5.v = B5_NAME(timproc_uso_b5_D_807FEA0C, 0x119C); nm = u5;
        {
            if ((c = arg0 + 0x80) != 0 || (c = (char *)timproc_uso_b5_func_055750(0x18)) != 0) {
                timproc_uso_b5_func_04A188(c, arg0, nm, 1);
                (FW(c, 0xC) = (int)((char *)&D_timb5_32C8_vt5 + 0x1A0), FW(c, 0x10) = 2, FW(c, 0x14) = 0);
            }
        }
        h = (f32)iz(); u12 = *(struct TimB5Nm *)((char *)&timproc_uso_b5_D_807FEA10 + 0x11A0); if (1) {} vC.x = h; vC.y = h; vC.z = h; vA.x = -2500.0f; vA.y = -1500.0f; vA.z = -2500.0f; if (1) {} vB.x = 2500.0f; vB.y = 1500.0f; vB.z = 2500.0f;
        cp0 = vC; cp1 = vA; cp2 = vB;
        nm.v = u12.v;
        {
            if ((cA = arg0 + 0x98) != 0 ||
                ((cA = (char *)timproc_uso_b5_func_055750(0x20)),
                 (h = (f32)iz()), (cA != 0))) {
                timproc_uso_b5_func_04A188(cA, arg0, nm, 1);
                h = (f32)iz();
                FW(cA, 0xC) = (int)((char *)&D_timb5_32C8_vv0 + 0x170);
                FW(cA, 0x1C) = 0;
                t = cp0;
                *(f32 *)(cA + 0x10) = t.x;
                *(f32 *)(cA + 0x14) = t.y;
                *(f32 *)(cA + 0x18) = t.z;
            }
        }
        u13 = *(struct TimB5Nm *)((char *)&timproc_uso_b5_D_807FEA14 + 0x11A4);
        vC2.x = h; vC2.y = h; vC2.z = h;
        if (1) {}
        vD.x = -3.1415927f; vD.y = -3.1415927f; vD.z = -3.1415927f;
        if (1) {}
        vE.x = 3.1415927f; vE.y = 3.1415927f; vE.z = 3.1415927f;
        cp0 = vC2;
        cp1 = vD;
        cp2 = vE;
        nm.v = u13.v;
        {
            if ((cB = arg0 + 0xB8) != 0 || (cB = (char *)timproc_uso_b5_func_055750(0x20)) != 0) {
                timproc_uso_b5_func_04A188(cB, arg0, nm, 1);
                FW(cB, 0xC) = (int)((char *)&D_timb5_32C8_vv1 + 0x170);
                FW(cB, 0x1C) = 0;
                t = cp0;
                *(f32 *)(cB + 0x10) = t.x;
                *(f32 *)(cB + 0x14) = t.y;
                *(f32 *)(cB + 0x18) = t.z;
            }
        }
        u14.v = B5_NAME(timproc_uso_b5_D_807FEA18, 0x11A8); nm = u14;
        {
            if ((c = arg0 + 0xD8) != 0 || (c = (char *)timproc_uso_b5_func_055750(0x18)) != 0) {
                timproc_uso_b5_func_04A188(c, arg0, nm, 1);
                FW(c, 0xC) = (int)((char *)&D_timb5_32C8_vw0 + 0x158);
                FW(c, 0x14) = 0;
                *(f32 *)(c + 0x10) = 0.0f;
            }
        }
        u15.v = B5_NAME(timproc_uso_b5_D_807FEA1C, 0x11AC); nm = u15;
        {
            if ((c = arg0 + 0xF0) != 0 || (c = (char *)timproc_uso_b5_func_055750(0x18)) != 0) {
                timproc_uso_b5_func_04A188(c, arg0, nm, 1);
                FW(c, 0xC) = (int)((char *)&D_timb5_32C8_vw1 + 0x158);
                FW(c, 0x14) = 0;
                *(f32 *)(c + 0x10) = 0.0f;
            }
        }
    }
    return arg0;
}
