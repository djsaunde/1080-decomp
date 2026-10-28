/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for timproc_uso_b3_func_000013B8 (0x2A8 / 170 words):
 * timer HUD draw (twin of arcproc_uso_func_000016F4 / timproc_uso_b1_func_00001340, same family as
 * mgrproc 23FC). agent-g run 172 (kit CXVII): the cfe NM body as plain C++, 162 of 171 words (89.18).
 * agent-g run 184 (kit CXXXIII): the five panels are sprite MEMBER objects of the owner (0x18 bytes each,
 * image pointer at +0x10, image width / height s16 at +0x20 / +0x22) and the widths come from an inline
 * `Spr::width()` that copies the image pointer into a named local first (`Img *i = img; return i->w;`):
 * those inline locals are the target's register copies (`move ra,t3` ...) and re-reads of the widths after
 * the signed /2; no volatile pads (frame 320, target 328). Length = target (680), 89.18 -> 97.57 standalone.
 * LEFT: xoff / xa / x homes 24 bytes high (the target has nine more 4-byte vreg homes above them) and the
 * last two panel draws' argument reload order.
 * NM object only. */
typedef float f32; typedef int s32; typedef unsigned char u8; typedef short s16;
extern "C" {
extern char D_b3_1240_base;
int timproc_uso_b3_func_000000B0(...);
}
struct Img { char p[0x20]; s16 w; s16 h; };
struct Spr {
    char p[0x10]; Img *img; char p14[4];
    s32 width() { Img *i = img; return i->w; }
};
struct Flags { char p[0x14]; int f; };
struct Obj {
    char p0[0x2A8]; char c2A8[0x24]; char c2CC[0x6B4 - 0x2CC];
    Spr sB; Spr sA; Spr sX; Spr sC; Spr tim;
    f32 alpha;
};
#define FL(o) (*(Flags **)((char *)(o) + 0x528))
extern "C" void timproc_uso_b3_func_000013B8(Obj *arg0, s32 arg1, f32 *arg2) {
    f32 one[4];
    u8 buf[0x80];
    s32 x;
    s32 w;
    s32 n;
    s32 i;
    s32 xoff;
    s32 xb;
    s32 xa;

    one[0] = 1.0f;
    one[1] = 1.0f;
    one[2] = 1.0f;
    one[3] = 1.0f;
    if (FL(arg0)->f & 2) {
        timproc_uso_b3_func_000000B0(&D_b3_1240_base, (s32)(255.0f * arg0->alpha), arg0->c2A8, arg0->c2CC);
        timproc_uso_b3_func_000000B0(&arg0->tim);
        w = arg0->tim.img->w / 12;
        timproc_uso_b3_func_000000B0(&arg0->sA);
        timproc_uso_b3_func_000000B0(&arg0->sB);
        timproc_uso_b3_func_000000B0(&arg0->sC);
        timproc_uso_b3_func_000000B0(buf, &D_b3_1240_base + 0x3D8, arg2);
        n = timproc_uso_b3_func_000000B0(buf);
        xoff = 0xA0 - (arg0->sC.width() + n * 13 + arg0->sA.width() + arg0->sB.width() + 0x10) / 2;
        xb = arg0->sB.width() + xoff + 4;
        xa = arg0->sA.width() + xb + 4;
        x = arg0->sC.width() + xa + 8;
        timproc_uso_b3_func_000000B0(&arg0->sC, xa, arg1 + 0x32, 2);
        for (i = 0; i < n; i++) {
            buf[i] -= 0x30;
            timproc_uso_b3_func_000000B0(&arg0->tim, x + i * 13, arg1 - arg0->tim.img->h / 2 + 0x32, buf[i] * w, w);
        }
        timproc_uso_b3_func_000000B0(&D_b3_1240_base, (s32)(255.0f * arg0->alpha), one);
        timproc_uso_b3_func_000000B0(&arg0->sA, xb, arg1 + 0x32, 2);
        timproc_uso_b3_func_000000B0(&arg0->sB, xoff, arg1 + 0x32, 2);
    }
}
