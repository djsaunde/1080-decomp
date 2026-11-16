/* IDO 7.1 NCC (C++) donor for mgr_drawResults_000023FC, EXACT (run 198, see the end of this comment)
 * (0x454 / 277 words): results-panel draw (alpha panels, mm:ss.cc timer text
 * rendered glyph by glyph, flag panel). agent-g run 172: the cfe NM body
 * ported to C++ (mgrproc is C++, like the other proc USOs).
 *
 * Under NCC the digit loop is a plain `for (i = 0; i < n; i++)` over buf[i]
 * with `x + i * 13`: uopt strength-reduces it to the target's 13*i counter
 * against a 13*n bound with n kept live (sw/lw t2 around the in-loop call)
 * and the `blez n` guard -- the cfe body needed a hand-written do-while and
 * still lost n. 277/277 words (cfe 275); the frame homes follow NCC's
 * declaration-order rule (every referenced named local is homed in decl
 * order), with volatile scalars for the unnamed words; `255.0f * f` keeps the
 * target's constant-first mul operand order. Left (~78 words): the int temp
 * ring runs one register behind the target from the first 0x508 increment on,
 * and the loop counter / bound / n take v1 / t0 / t1 where the target has
 * t0 / t1 / t2 (the target keeps the per-glyph 0x798 reload in v1 / v0).
 * Run 172 later: the glyph `c` as u8 (was s32) -> the loop counter / bound / n take the target's t0 / t1 / t2
 * (78 -> 54 words, 98.20 -> 98.70); left: c in v1 (target v0), the 0x798 reload pair and the t-ring.
 * Run 184 (kit CXXXIII): the two sprite widths through an inline `Spr::width()` that copies the image pointer
 * into a named local (the timer-HUD family rewrite): its temporaries take the frame words three of the four
 * volatile pads stood in for (pad0 / pad0b / pad1 dropped), then `if (1) {}` after `flag = 0` (lever-sweep):
 * 98.62 -> 99.25 standalone. Then `tim` de-named (SPR(9) at both uses) and the volatile pad between n and x
 * replaced by two volatile pads just above min / sec (their homes 96 / 92 = target): 99.27. Then the glyph
 * height through `Spr::height()` (same named-image-pointer member) and `if (1) {}` after `c = buf[i]`: 99.49;
 * its inline temporaries then replace the last two volatile pads (dropped: no pads left).
 * Run 194 (kit CLIII): the run-184 `if (1) {}` after `flag = 0` dropped again -- with the later member rewrites it
 * made the flag test's `& 1` a v0 candidate (lw v0 / andi v0,v0,1); without the split the t-ring takes it (t9 / t3)
 * and the rest of the block's ring lines up: 99.54 -> 99.86 standalone. Left: the 0x798 image load in v0 (target
 * t0) and the n / x homes (target 84 / 76, ours 104 / 100).
 * Run 198 (kit CLXIII, edg home order): a scope's named locals are homed at scope entry, pooled inline
 * temporaries at first use. n / x moved into nested blocks opened after the glyph-width / vt statements
 * (n after the first pooled temp, x after the second) and min / sec into the timer block, plus a folded
 * `+ 0 * SPR(9)->width()` in the first panel test (two function-scope pool temps above min / sec, no
 * code): min / sec / n / x homes 96 / 92 / 84 / 76 and the a0 spill 68 = target; the loop height through
 * `img->gh()` (one pool temp) keeps the frame at 288. 99.85 -> 99.88 standalone. Left: the glyph-width image
 * load in v0 (target t0) -- its pool variable gets v0 first (zdbug:6), nothing occupies v0 over its range.
 * Then EXACT: the glyph width and vt through `Spr::pw()` (`img->gw()`: pooled temporaries, reusable, where
 * width()'s named inline local `Img *i` is a fresh variable per expansion), the loop height back through
 * `height()`, and one unreferenced volatile word in the n block (restores the x home / spill words):
 * the glyph image load takes t0. Spliced over the placeholder in mgrproc_uso.c via REPLACE_FUNC_BODY. */
typedef float f32;
typedef int s32;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
#include "../../include/game/modeproc.h" /* MgrRace: arg0 (agent-h run 552) */
#define FW(p, o) (*(int *)((char *)(p) + (o)))
extern "C" {
extern char import_800201D0, import_80263D30, import_8024CAF8, import_80264A10;
void import_0024E608(...);
void import_0024F2C8(...);
void import_0024F34C(...);
void import_0024EAA0(...);
void import_0024F75C(...);
int mgrproc_uso_func_07C610(...);
int mgr_isLastCourse_00000140(...);
void mgrproc_uso_func_083574(...);
}
struct Img { char p[0x20]; s16 w; s16 h; s32 gh() { return h; } s32 gw() { return w; } };
struct Spr { char p[0x10]; Img *img; char p14[4]; s32 width() { Img *i = img; return i->w; } s32 height() { Img *i = img; return i->h; } s32 pw() { return img->gw(); } };
#define SPR(k) ((Spr *)&MGRRACE(arg0)->sprites[k])
extern "C" {
void mgr_drawResults_000023FC(char *arg0) {
    f32 one[4];
    u8 buf[0x80];
    char *p770;
    s32 t;
    s32 w;
    s32 xoff;
    s32 vt;
    u8 c;
    s32 flag;
    char *panel;
    s32 i;

    one[0] = 1.0f;
    one[1] = 1.0f;
    one[2] = 1.0f;
    one[3] = 1.0f;
    if (MGRRACE(arg0)->race.result != 2 + 0 * SPR(9)->width()) {
        import_0024F2C8(&import_80263D30);
        import_0024E608(&import_80263D30, (s32) (255.0f * MGRRACE(arg0)->overlayAlpha), one);
        import_0024F34C(&import_80263D30, 0xA0, 0x8C, 3);
    }
    import_0024EAA0(&import_8024CAF8, (s32) (255.0f * MGRRACE(arg0)->overlayAlpha), MGRRACE(arg0)->race.colours.pair[RACECOL_AD][0].value, MGRRACE(arg0)->race.colours.pair[RACECOL_AD][1].value);
    MGRRACE(arg0)->race.unk508 = MGRRACE(arg0)->race.unk508 + 1;
    if (MGRRACE(arg0)->race.result == 0) {
        panel = (char *)&MGRRACE(arg0)->sprites[5];
        if (mgr_isLastCourse_00000140(MGRRACE(arg0)->courseList) != 0) {
            panel = (char *)&MGRRACE(arg0)->sprites[6];
            if (MGRRACE(arg0)->race.unk508 & 8) {
                import_0024F2C8(panel);
                import_0024F34C(panel, 0xA0, 0x78, 3);
            }
        } else {
            import_0024F2C8(panel);
            import_0024F34C(panel, 0xA0, 0x78, 3);
        }
    }
    if ((MGRRACE(arg0)->race.unk508 & 8) && (FW((char *) MGRRACE(arg0)->race.session, 0x14) & 1)) {
        t = FW((char *) FW(&import_800201D0, 0x138), 0x30);
        s32 min, sec;
        min = t / 60000;
        t -= min * 60000;
        sec = t / 1000;
        t -= sec * 1000;
        t = t / 10;
        import_0024EAA0(&import_8024CAF8, (s32) (255.0f * MGRRACE(arg0)->overlayAlpha), MGRRACE(arg0)->race.colours.pair[RACECOL_RS][0].value, MGRRACE(arg0)->race.colours.pair[RACECOL_RS][1].value);
        import_0024F2C8(SPR(9));
        w = SPR(9)->pw() / 12;
        {
        s32 n;
        volatile s32 pn;
        p770 = (char *)&MGRRACE(arg0)->sprites[8];
        import_0024F2C8(p770);
        mgrproc_uso_func_083574(buf, &import_80264A10 + 0x640, min, sec, t);
        n = mgrproc_uso_func_07C610(buf);
        vt = SPR(8)->pw();
        {
        s32 x;
        xoff = 0xA0 - ((vt + (n * 13) + 8) / 2);
        x = vt + xoff + 8;
        import_0024F34C( p770, xoff, 0xBE, 2);
        for (i = 0; i < n; i++) {
            c = buf[i]; if (1) {}
            if (c == 0x5B) {
                buf[i] = 10;
            } else if (c == 0x5D) {
                buf[i] = 11;
            } else {
                buf[i] = c - 0x30;
            }
            import_0024F75C(SPR(9), x + i * 13, 0xBE - (SPR(9)->height() / 2), buf[i] * w, w);
        }
        }
        }
    }
    flag = 0;
    if ((FW((char *) MGRRACE(arg0)->race.session, 0x14) & 1) && (MGRRACE(arg0)->race.unk4FC != 0) && (FW((char *) FW((char *) FW(&import_800201D0, 0x138), 0x44), 0x38) < 3)) {
        flag = 1;
    }
    import_0024EAA0(&import_8024CAF8, (s32) (255.0f * MGRRACE(arg0)->overlayAlpha), MGRRACE(arg0)->race.colours.pair[RACECOL_NR][0].value, MGRRACE(arg0)->race.colours.pair[RACECOL_NR][1].value);
    panel = (char *)&MGRRACE(arg0)->sprites[7];
    if (flag != 0) {
        import_0024F2C8(panel);
        if (MGRRACE(arg0)->race.unk508 & 8) {
            import_0024F34C(panel, 0xA0, 0xA0, 3);
        }
    }
}
}
