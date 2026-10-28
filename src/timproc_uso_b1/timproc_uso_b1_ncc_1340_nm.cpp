/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for timproc_uso_b1_func_00001340 (0x3B8 / 238 words):
 * timer HUD draw with the mm:ss.cc split (the 23FC / timproc_b3 13B8 / arcproc 16F4 family). agent-g
 * run 172 (kit CXVII): the cfe NM body as plain C++ (228 of 238 words, 86.53).
 * agent-g run 184 (kit CXXXIII): ported from the 13B8 C++ rewrite -- sprite member objects and the inline
 * `Spr::width()` with a named image pointer, no volatile pads: 239 words (target 238), 94.05 standalone.
 * Then the tim panel's width / height through the members (`tim.width() / 12`, `tim.height()`) and `if (1) {}`
 * after its begin call: 238 words = target, 98.10 standalone; xmid de-named (lever-sweep).
 * NM object only. */
typedef float f32; typedef int s32; typedef short s16;
extern "C" {
extern char D_00000000;
int timproc_uso_b1_func_000000B0(...);
}
struct Img { char p[0x20]; s16 w; s16 h; };
struct Spr {
    s32 height() { Img *i = img; return i->h; }
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
extern "C" void timproc_uso_b1_func_00001340(Obj *arg0, s32 arg1, s32 arg2) {
    f32 one[4];
    volatile s32 sp0; /* frame word above buf (the target's one / buf sit 8 bytes lower) */
    char buf[128];
    s32 min, sec;
    s32 glyph;
    s32 x, pos4, posA1, pos6;
    s32 i;
    s32 count;

    one[1] = 1.0f;
    one[2] = 1.0f;
    one[0] = 1.0f;
    one[3] = 1.0f;
    if (FL(arg0)->f & 1) {
        min = arg2 / 60000;
        arg2 -= min * 60000;
        sec = arg2 / 1000;
        arg2 -= sec * 1000;
        arg2 = arg2 / 10;
        timproc_uso_b1_func_000000B0(&D_00000000, (s32)(255.0f * arg0->alpha), arg0->c2A8, arg0->c2CC);
        timproc_uso_b1_func_000000B0(&arg0->tim); if (1) {}
        glyph = arg0->tim.width() / 12;
        timproc_uso_b1_func_000000B0(&arg0->sA);
        timproc_uso_b1_func_000000B0(&arg0->sB);
        timproc_uso_b1_func_000000B0(&arg0->sC);
        timproc_uso_b1_func_000000B0(buf, &D_00000000 + 0x3F4, min, sec, arg2);
        count = timproc_uso_b1_func_000000B0(buf);
        x = 160 - (arg0->sC.width() + count * 13 + arg0->sA.width() + arg0->sB.width() + 16) / 2;
        pos4 = arg0->sB.width() + x + 4;
        posA1 = arg0->sA.width() + pos4 + 4;
        pos6 = arg0->sC.width() + posA1 + 8;
        timproc_uso_b1_func_000000B0(&arg0->sC, posA1, arg1 + 0x32, 2);
        for (i = 0; i < count; i++) {
            if (buf[i] == 0x5B) {
                buf[i] = 0xA;
            } else if (buf[i] == 0x5D) {
                buf[i] = 0xB;
            } else {
                buf[i] = buf[i] - 0x30;
            }
            timproc_uso_b1_func_000000B0(&arg0->tim, pos6 + i * 13, (arg1 - arg0->tim.height() / 2) + 0x32, buf[i] * glyph, glyph);
        }
        timproc_uso_b1_func_000000B0(&arg0->sA, (s32)(255.0f * arg0->alpha), one, 0xFF);
        timproc_uso_b1_func_000000B0(&arg0->sA, pos4, arg1 + 0x32, 2);
        timproc_uso_b1_func_000000B0(&arg0->sB, (s32)(255.0f * arg0->alpha), one, 0xFF);
        timproc_uso_b1_func_000000B0(&arg0->sB, x, arg1 + 0x32, 2);
    }
}
