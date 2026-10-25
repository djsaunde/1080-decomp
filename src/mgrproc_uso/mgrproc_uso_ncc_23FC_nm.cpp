/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for mgrproc_uso_func_000023FC
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
 * Spliced into the NON_MATCHING mgrproc_uso object only. */
typedef float f32;
typedef int s32;
typedef unsigned char u8;
typedef short s16;
#define FW(p, o) (*(int *)((char *)(p) + (o)))
extern "C" {
extern char import_800201D0, import_80263D30, import_8024CAF8, import_80264A10;
void import_0024E608(...);
void import_0024F2C8(...);
void import_0024F34C(...);
void import_0024EAA0(...);
void import_0024F75C(...);
int mgrproc_uso_func_07C610(...);
int mgrproc_uso_func_00000140(...);
void mgrproc_uso_func_083574(...);
void mgrproc_uso_func_000023FC(char *arg0) {
    f32 one[4];
    u8 buf[0x80];
    char *p770;
    s32 t;
    s32 w;
    s32 xoff;
    s32 vt;
    s32 c;
    s32 flag;
    char *panel;
    s32 i;
    volatile s32 pad0;
    volatile s32 pad0b;
    s32 min;
    s32 sec;
    volatile s32 pad1;
    s32 n;
    volatile s32 pad2;
    s32 x;
    char *tim;

    one[0] = 1.0f;
    one[1] = 1.0f;
    one[2] = 1.0f;
    one[3] = 1.0f;
    if (FW(arg0, 0x4F8) != 2) {
        import_0024F2C8(&import_80263D30);
        import_0024E608(&import_80263D30, (s32) (255.0f * *(f32 *)(arg0 + 0x7A0)), one);
        import_0024F34C(&import_80263D30, 0xA0, 0x8C, 3);
    }
    import_0024EAA0(&import_8024CAF8, (s32) (255.0f * *(f32 *)(arg0 + 0x7A0)), arg0 + 0x338, arg0 + 0x35C);
    FW(arg0, 0x508) = FW(arg0, 0x508) + 1;
    if (FW(arg0, 0x4F8) == 0) {
        panel = arg0 + 0x728;
        if (mgrproc_uso_func_00000140(FW(arg0, 0x6A8)) != 0) {
            panel = arg0 + 0x740;
            if (FW(arg0, 0x508) & 8) {
                import_0024F2C8(panel);
                import_0024F34C(panel, 0xA0, 0x78, 3);
            }
        } else {
            import_0024F2C8(panel);
            import_0024F34C(panel, 0xA0, 0x78, 3);
        }
    }
    if ((FW(arg0, 0x508) & 8) && (FW((char *) FW(arg0, 0x528), 0x14) & 1)) {
        t = FW((char *) FW(&import_800201D0, 0x138), 0x30);
        min = t / 60000;
        t -= min * 60000;
        sec = t / 1000;
        t -= sec * 1000;
        t = t / 10;
        import_0024EAA0(&import_8024CAF8, (s32) (255.0f * *(f32 *)(arg0 + 0x7A0)), arg0 + 0x2A8, arg0 + 0x2CC);
        tim = arg0 + 0x788;
        import_0024F2C8(tim);
        w = *(s16 *)((char *) FW(arg0, 0x798) + 0x20) / 12;
        p770 = arg0 + 0x770;
        import_0024F2C8(p770);
        mgrproc_uso_func_083574(buf, &import_80264A10 + 0x640, min, sec, t);
        n = mgrproc_uso_func_07C610(buf);
        vt = *(s16 *)((char *) FW(arg0, 0x780) + 0x20);
        xoff = 0xA0 - ((vt + (n * 13) + 8) / 2);
        x = vt + xoff + 8;
        import_0024F34C(p770, xoff, 0xBE, 2);
        for (i = 0; i < n; i++) {
            c = buf[i];
            if (c == 0x5B) {
                buf[i] = 10;
            } else if (c == 0x5D) {
                buf[i] = 11;
            } else {
                buf[i] = c - 0x30;
            }
            import_0024F75C(tim, x + i * 13, 0xBE - (*(s16 *)((char *) FW(arg0, 0x798) + 0x22) / 2), buf[i] * w, w);
        }
    }
    flag = 0;
    if ((FW((char *) FW(arg0, 0x528), 0x14) & 1) && (FW(arg0, 0x4FC) != 0) && (FW((char *) FW((char *) FW(&import_800201D0, 0x138), 0x44), 0x38) < 3)) {
        flag = 1;
    }
    import_0024EAA0(&import_8024CAF8, (s32) (255.0f * *(f32 *)(arg0 + 0x7A0)), arg0 + 0x2F0, arg0 + 0x314);
    panel = arg0 + 0x758;
    if (flag != 0) {
        import_0024F2C8(panel);
        if (FW(arg0, 0x508) & 8) {
            import_0024F34C(panel, 0xA0, 0xA0, 3);
        }
    }
}
}
