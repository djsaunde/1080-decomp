/* IDO 7.1 NCC (C++) donor for timproc_uso_b1_func_00001340 (0x3B8 / 238 words), EXACT:
 * timer HUD draw with the mm:ss.cc split (the 23FC / timproc_b3 13B8 / arcproc 16F4 family). agent-g
 * run 172 (kit CXVII): the cfe NM body as plain C++ (228 of 238 words, 86.53).
 * agent-g run 184 (kit CXXXIII): ported from the 13B8 C++ rewrite -- sprite member objects and the inline
 * `Spr::width()` with a named image pointer, no volatile pads: 239 words (target 238), 94.05 standalone.
 * Then the tim panel's width / height through the members (`tim.width() / 12`, `tim.height()`) and `if (1) {}`
 * after its begin call: 238 words = target, 98.10 standalone; xmid de-named (lever-sweep).
 * agent-g run 196 (kit CLIX): 13B8's run-185 nested block (x / pos4 / posA1 / pos6 block locals, panels named
 * there) and run-196 Img members (height / panel widths through `img->gh()` / `img->gw()`; tim's width keeps
 * the named local; three volatile pads in the block; the old `sp0` pad dropped): 98.18 -> 99.62 standalone.
 * agent-g run 198 (kit CLXI / CLXIII): EXACT. 13B8's tie-order fix (`+ 0 * arg0->tim.pw()` on the glyph
 * statement, loop height through `tim.img->gh()`, x / pos4 / posA1 / pos6 declared before the panel
 * pointers) fixes the pB-width / pA-copy pair; one[] stored in index order. The count spill (target 152)
 * is count's own frontend home: edg allocates a scope's named locals at scope entry and pooled inline
 * temporaries at first use, so declaring count at the top put it above the five pool temps (172). Declared
 * as the first local of the nested block it lands after them, directly above x (152).
 * Spliced over the placeholder in timproc_uso_b1.c via REPLACE_FUNC_BODY. */
typedef float f32; typedef int s32; typedef short s16;
extern "C" {
extern char D_00000000;
int timproc_uso_b1_func_000000B0(...);
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
    char p0[0x2A8]; char c2A8[0x24]; char c2CC[0x6B4 - 0x2CC];
    Spr sB; Spr sA; Spr sX; Spr sC; Spr tim;
    f32 alpha;
};
#define FL(o) (*(Flags **)((char *)(o) + 0x528))
extern "C" void timproc_uso_b1_func_00001340(Obj *arg0, s32 arg1, s32 arg2) {
    f32 one[4];
    char buf[128];
    s32 min;
    s32 sec;
    s32 glyph;
    s32 i;

    one[0] = 1.0f;
    one[1] = 1.0f;
    one[2] = 1.0f;
    one[3] = 1.0f;
    if (FL(arg0)->f & 1) {
        min = arg2 / 60000;
        arg2 -= min * 60000;
        sec = arg2 / 1000;
        arg2 -= sec * 1000;
        arg2 = arg2 / 10;
        timproc_uso_b1_func_000000B0(&D_00000000, (s32)(255.0f * arg0->alpha), arg0->c2A8, arg0->c2CC);
        timproc_uso_b1_func_000000B0(&arg0->tim); if (1) {}
        glyph = arg0->tim.width() / 12 + 0 * arg0->tim.pw();
        timproc_uso_b1_func_000000B0(&arg0->sA);
        timproc_uso_b1_func_000000B0(&arg0->sB);
        timproc_uso_b1_func_000000B0(&arg0->sC);
        timproc_uso_b1_func_000000B0(buf, &D_00000000 + 0x3F4, min, sec, arg2);
        {
        s32 count = timproc_uso_b1_func_000000B0(buf);
        s32 x, pos4, posA1, pos6;
        Spr *pC = &arg0->sC, *pA = &arg0->sA, *pB = &arg0->sB;
        x = 160 - (pC->pw() + count * 13 + pA->pw() + pB->pw() + 16) / 2;
        pos4 = pB->pw() + x + 4;
        posA1 = pA->pw() + pos4 + 4;
        pos6 = pC->pw() + posA1 + 8;
        volatile s32 pad0, pad1, pad2;
        timproc_uso_b1_func_000000B0(&arg0->sC, posA1, arg1 + 0x32, 2);
        for (i = 0; i < count; i++) {
            if (buf[i] == 0x5B) {
                buf[i] = 0xA;
            } else if (buf[i] == 0x5D) {
                buf[i] = 0xB;
            } else {
                buf[i] = buf[i] - 0x30;
            }
            timproc_uso_b1_func_000000B0(&arg0->tim, pos6 + i * 13, (arg1 - arg0->tim.img->gh() / 2) + 0x32, buf[i] * glyph, glyph);
        }
        timproc_uso_b1_func_000000B0(&arg0->sA, (s32)(255.0f * arg0->alpha), one, 0xFF);
        timproc_uso_b1_func_000000B0(&arg0->sA, pos4, arg1 + 0x32, 2);
        timproc_uso_b1_func_000000B0(&arg0->sB, (s32)(255.0f * arg0->alpha), one, 0xFF);
        timproc_uso_b1_func_000000B0(&arg0->sB, x, arg1 + 0x32, 2);
        }
    }
}
