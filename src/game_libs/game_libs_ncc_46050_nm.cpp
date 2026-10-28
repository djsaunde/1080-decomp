/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00046050 (agent-c run 152): the NM C body (EMIT path as the then-arm of `== 0`, call in the else arm) lifted under NCC; NCC colours the early _q/_n DL temps v0/a1 like the target where cfe gives a1/a2; the first character is read through a copy of the cursor (target `or v1,s3; lbu a1,0(v1)`), both the entry read and the loop-bottom read, 86.48 -> 86.91.
 * agent-c run 182: the character dispatch is a `switch (ch)` (edg compare chain `beq 10 / beq 9 / beql 0x30 / b default`
 * with the case bodies out of line, as the target) whose newline case jumps past the advance (`goto next`), and x is read
 * after the switch: 86.91 -> 89.54, length 1832 -> 1840 (target 1848); charw read before lineh (s1 / s2 as the
 * target): -> 89.63.
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32;
typedef float f32;
extern "C" {
extern int D_00000000;
extern int gl_func_00034458(...);
}
#define DI(o) (*(int *)((char *)&D_00000000 + (o)))
#define FI(p, o) (*(int *)((char *)(p) + (o)))
#define FP(p, o) (*(void **)((char *)(p) + (o)))
/* held GfxCtx DL node: (arg0->0x254)->0x158, member 0xC = {head@0, count@4} */
#define DLCTX (FP(FP(arg0, 0x254), 0x158))
#define EMIT(w0, w1) {                                             \
    register void *_e;                                             \
    _c = DLCTX;                                                    \
    _q = FP(_c, 0xC);                                              \
    _n = FI(_q, 0x4);                                              \
    FI(_q, 0x4) = _n + 1;                                          \
    _e = (char *)FP(FP(_c, 0xC), 0x0) + (_n * 8);                  \
    FI(_e, 0x0) = (w0); FI(_e, 0x4) = (w1);                        \
}

extern "C" s32 gl_func_00046050(void *arg0, u8 *arg1, s32 arg2) {
    register s32 startx = FI(arg0, 0x21C);
    register s32 charw;
    register s32 lineh;
    int *margin = &D_00000000;
    s32 ch;
    s32 cc;
    void *_c;
    void *_q;
    int _n;

    charw = FI(arg0, 0x224);
    lineh = FI(arg0, 0x228);

    if (FI(arg0, 0x28C) == 0) {
        EMIT(0xE7000000, 0); if (1) {}
        EMIT(0xBA001402, 0);
        EMIT(0xB6000000, 1);
        EMIT(0xB900031D, 0x0C087008);
        EMIT(0xF9000000, 0x80);
        EMIT(0xBB000001, 0x80008000);
        EMIT(0xFCFFFFFF, 0xFFFCF279);
        EMIT(0xBA000C02, 0);
        EMIT(0xBA001301, 0);
        { register void *_e;
          _c = DLCTX;
          _q = FP(_c, 0xC);
          _n = FI(_q, 0x4); FI(_q, 0x4) = _n + 1;
          _e = (char *)FP(FP(_c, 0xC), 0x0) + (_n * 8);
          FI(_e, 0x0) = 0xFD900000; FI(_e, 0x4) = gl_func_00034458((void *)&D_00000000); }
        EMIT(0xF5900000, 0x07000000);
        EMIT(0xE6000000, 0);
        EMIT(0xF3000000, 0x077FF100);
        EMIT(0xE7000000, 0);
        EMIT(0xF5801000, 0);
        EMIT(0xF2000000, 0x1FC0FC);
    }

    { u8 *p = arg1; arg1 = p + 1; ch = *p; }
    if (ch != 0) {
        do {
            cc = ch;
             
            switch (ch) {
            case 10:
                FI(arg0, 0x21C) = startx;
                FI(arg0, 0x220) = FI(arg0, 0x220) + lineh;
                goto next;
            case 9:
                ch = 0x20;
                cc = 0x20;
                break;
            case 0x30:
                ch = 0x4F;
                cc = 0x4F;
                break;
            }
            {
                /* run 190: x (o->0x21C) de-named into the test: 89.63 -> 89.66 standalone */
                if ((*margin - 0x14) < (FI(arg0, 0x21C) + charw)) {
                    FI(arg0, 0x21C) = startx;
                    FI(arg0, 0x220) = FI(arg0, 0x220) + lineh;
                }
                if (cc < 0x80) {
                    if (FI(arg0, 0x28C) == 0) {
                        EMIT((((((FI(arg0, 0x21C) + charw) * 4) & 0xFFF) << 0xC) | 0xE4000000) | (((FI(arg0, 0x220) + lineh) * 4) & 0xFFF),
                             (((FI(arg0, 0x21C) * 4) & 0xFFF) << 0xC) | ((FI(arg0, 0x220) * 4) & 0xFFF));  if (1) {} 
                        EMIT(0xB4000000,
                            ((cc & 0xF) << 0x18) | (((((s32) (cc & 0xF0)) >> 1) << 5) & 0xFFFF));
                        EMIT(0xB3000000,
                            ((0x2000 / charw) << 0x10) | ((0x2000 / lineh) & 0xFFFF));
                    } else {
                        gl_func_00034458(arg0, ch, FI(arg0, 0x21C), FI(arg0, 0x220));
                    }
                }
                FI(arg0, 0x21C) = FI(arg0, 0x21C) + charw;
            }
        next:
            if (!(FI(arg0, 0x21C) < arg2)) {
                break;
            }
            { u8 *p = arg1; arg1 = p + 1; ch = *p; }
        } while (ch != 0);
    }

    if (FI(arg0, 0x28C) == 0) {
        EMIT(0xE7000000, 0);
        EMIT(0xBB000000, -1);
        EMIT(0xBA001402, 0x300000);
    }
    return 0;
}
