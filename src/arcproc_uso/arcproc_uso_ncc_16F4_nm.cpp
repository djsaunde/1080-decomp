/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for arcproc_uso_func_000016F4 (twin of timproc_uso_b3_func_000013B8;
 * see that donor). agent-g run 184 (kit CXXXIII): ported from the b3 C++ rewrite -- sprite member objects
 * (sA 0x704, sB 0x71C, sC 0x734, tim 0x74C, alpha 0x77C) and the inline `Spr::width()` with a named image pointer.
 * Same run, later: the tim panel's width / height through the members and the BB split after its begin call.
 * agent-g run 185 (kit CXXXV): x / xb / xoff are block locals of a NESTED block opened after the count call,
 * with the three width panels named there (`Spr *pC = &arg0->sC, ...`; the width() calls go through them, the
 * draw calls keep `&arg0->sX`): edg allocates a block's named locals at block entry, below the enclosing
 * block's temporaries, so the homes land on the target's 136 / 144 / 148. LEFT: spill slots 4 low, t-ring.
 * NM object only. */
typedef float f32; typedef int s32; typedef unsigned char u8; typedef short s16;
extern "C" {
extern char D_00000000;
int arcproc_uso_func_00001B88(...);
}
struct Img { char p[0x20]; s16 w; s16 h; };
struct Spr {
    s32 height() { Img *i = img; return i->h; }
    char p[0x10]; Img *img; char p14[4];
    s32 width() { Img *i = img; return i->w; }
};
struct Flags { char p[0x14]; int f; };
struct Obj {
    char p0[0x2A8]; char c2A8[0x24]; char c2CC[0x704 - 0x2CC];
    Spr sA; Spr sB; Spr sC; Spr tim; char p764[0x77C - 0x764];
    f32 alpha;
};
#define FL(o) (*(Flags **)((char *)(o) + 0x528))
extern "C" void arcproc_uso_func_000016F4(Obj *arg0, s32 arg1, f32 *arg2) {
    f32 one[4];
    u8 buf[0x80];
    s32 i;

    one[0] = 1.0f;
    one[1] = 1.0f;
    one[2] = 1.0f;
    one[3] = 1.0f;
    if (FL(arg0)->f & 4) {
        arcproc_uso_func_00001B88(&D_00000000, (s32)(255.0f * arg0->alpha), arg0->c2A8, arg0->c2CC);
        arcproc_uso_func_00001B88(&arg0->tim); if (1) {}
        s32 w = arg0->tim.width() / 12;
        arcproc_uso_func_00001B88(&arg0->sA);
        arcproc_uso_func_00001B88(&arg0->sB);
        arcproc_uso_func_00001B88(&arg0->sC);
        arcproc_uso_func_00001B88(buf, &D_00000000 + 0x3D8, arg2);
        s32 n = arcproc_uso_func_00001B88(buf);
        {
        Spr *pC = &arg0->sC, *pA = &arg0->sA, *pB = &arg0->sB;
        s32 xoff = 0xA0 - (pC->width() + n * 13 + pA->width() + pB->width() + 0x10) / 2;
        s32 xb = pB->width() + xoff + 4;
        s32 xa = pA->width() + xb + 4;
        s32 x = pC->width() + xa + 8;
        arcproc_uso_func_00001B88(&arg0->sC, xa, arg1 + 0x32, 2);
        for (i = 0; i < n; i++) {
            buf[i] -= 0x30;
            arcproc_uso_func_00001B88(&arg0->tim, x + i * 13, arg1 - arg0->tim.height() / 2 + 0x32, buf[i] * w, w);
        }
        arcproc_uso_func_00001B88(&D_00000000, (s32)(255.0f * arg0->alpha), one);
        arcproc_uso_func_00001B88(&arg0->sA, xb, arg1 + 0x32, 2);
        arcproc_uso_func_00001B88(&arg0->sB, xoff, arg1 + 0x32, 2);
        }
    }
}
