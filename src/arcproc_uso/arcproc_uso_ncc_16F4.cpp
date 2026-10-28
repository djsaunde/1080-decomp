/* IDO 7.1 NCC (C++) donor for arcproc_uso_func_000016F4, EXACT (twin of timproc_uso_b3_func_000013B8;
 * see that donor). agent-g run 184 (kit CXXXIII): ported from the b3 C++ rewrite -- sprite member objects
 * (sA 0x704, sB 0x71C, sC 0x734, tim 0x74C, alpha 0x77C) and the inline `Spr::width()` with a named image pointer.
 * Same run, later: the tim panel's width / height through the members and the BB split after its begin call.
 * agent-g run 185 (kit CXXXV): x / xb / xoff are block locals of a NESTED block opened after the count call,
 * with the three width panels named there (`Spr *pC = &arg0->sC, ...`; the width() calls go through them, the
 * draw calls keep `&arg0->sX`): edg allocates a block's named locals at block entry, below the enclosing
 * block's temporaries, so the homes land on the target's 136 / 144 / 148. LEFT: spill slots 4 low, t-ring.
 * agent-g run 196 (kit CLIX): b3 13B8's Img-member port (height / panel widths through `img->gh()` / `img->gw()`,
 * tim's width keeps the named local, three volatile pads in the nested block).
 * agent-g run 197 (kit CLXI): EXACT with b3 13B8's fix (`+ 0 * arg0->tim.pw()` on the w statement sets the
 * edg pool-temp order, loop height through `tim.img->gh()`, x locals declared before the panel pointers).
 * Spliced over the placeholder in arcproc_uso_tail1.c via REPLACE_FUNC_BODY. */
typedef float f32; typedef int s32; typedef unsigned char u8; typedef short s16;
extern "C" {
extern char D_00000000;
int gl_func_00000000(...);
}
struct Img { char p[0x20]; s16 w; s16 h; s32 gh() { return h; } s32 gw() { return w; } };
struct Spr {
    s32 height() { return img->gh(); }
    char p[0x10]; Img *img; char p14[4];
    s32 width() { Img *i = img; return i->w; }
    s32 pw() { return img->gw(); }
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
        gl_func_00000000(&D_00000000, (s32)(255.0f * arg0->alpha), arg0->c2A8, arg0->c2CC);
        gl_func_00000000(&arg0->tim); if (1) {}
        s32 w = arg0->tim.width() / 12 + 0 * arg0->tim.pw();
        gl_func_00000000(&arg0->sA);
        gl_func_00000000(&arg0->sB);
        gl_func_00000000(&arg0->sC);
        gl_func_00000000(buf, &D_00000000 + 0x3D8, arg2);
        s32 n = gl_func_00000000(buf);
        {
        s32 xoff, xb, xa, x;
        Spr *pC = &arg0->sC, *pA = &arg0->sA, *pB = &arg0->sB;
        xoff = 0xA0 - (pC->pw() + n * 13 + pA->pw() + pB->pw() + 0x10) / 2;
        xb = pB->pw() + xoff + 4;
        xa = pA->pw() + xb + 4;
        x = pC->pw() + xa + 8;
        volatile s32 pad0, pad1, pad2;
        gl_func_00000000(&arg0->sC, xa, arg1 + 0x32, 2);
        for (i = 0; i < n; i++) {
            buf[i] -= 0x30;
            gl_func_00000000(&arg0->tim, x + i * 13, arg1 - arg0->tim.img->gh() / 2 + 0x32, buf[i] * w, w);
        }
        gl_func_00000000(&D_00000000, (s32)(255.0f * arg0->alpha), one);
        gl_func_00000000(&arg0->sA, xb, arg1 + 0x32, 2);
        gl_func_00000000(&arg0->sB, xoff, arg1 + 0x32, 2);
        }
    }
}
