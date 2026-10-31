/* IDO 7.1 NCC (C++) NM-only comparison donor for mgrproc_uso_func_00002B7C (0x2B8 / 174 words), agent-c run 243.
 * The in-tree cfe body (mgrproc_uso.c, 98.24) compiled verbatim under NCC scores 98.17; the four volatile 1.0f
 * "ghost" stores of the cfe body are an unused C++ colour object with an inline constructor (`Col col;` -- the
 * ctor's `this` keeps the object in memory, so the stores survive without volatile): 99.39, and the first
 * `lw a3,0xD4(a0)` now schedules after the four swc1 like the target. Frame (sp offsets only): every named
 * local edg keeps is a home word -- n1 declared above col gives the unused top word (col at 0xA4..0xB0, frame
 * 0xB8), the n1 / n2 string-argument names and the split case-4 panel2 give the two extra low homes (the case-1
 * spill lands at 0x5C, sp60 at 0x60, sub at 0xA0 = target). 99.45, length = target.
 * Left: case 1's derived pointer (target keeps a3 + 0x490 in v0 across the D108 call; every held / reference /
 * derived-to-base spelling folds it back onto a3 or adds a null check) and the loop counter / glyph pointer
 * s0 / s1 swap. `sub` is the loop-carried conditionally-assigned value of the target (no default arm), as in
 * the cfe body. Still NON_MATCHING: build/non_matching/ only. */
typedef float f32;
extern "C" {
extern char import_8024CAF8;
extern void import_0024E608(...);
extern void import_0024F2C8(...);
extern void import_0024F34C(...);
extern char import_8025CAF8;
extern char import_80264A28;
extern char import_80264A3C;
extern char import_80263D48;
extern int import_0024D108(...);
extern int import_0024E060(...);
extern int import_0024D704(...);
extern int mgrproc_uso_func_083574(...);
void mgrproc_uso_func_00002B7C(char *arg0);
}
struct Col { f32 r, g, b, a; Col() { r = 1.0f; g = 1.0f; b = 1.0f; a = 1.0f; } };
void mgrproc_uso_func_00002B7C(char *arg0) {
    char *n1;
    Col col;
    char *sub;
    char sp60[64];
    char *a3;
    char *panel;
    int s0;
    int s2;
    char *s1;
    int digit;
    int idx;
    char *panel2;
    char *n2;

    a3 = *(char **)(arg0 + 0xD4);
    switch (*(int *)(a3 + 0x7E4)) {
    case 1:
        a3 += 0x490;
        import_0024D108(*(int *)(a3 + 0xD8));
        import_0024E608(&import_8024CAF8, 0xFF, a3 + 0x10);
        mgrproc_uso_func_083574(&sp60, (n1 = &import_80264A28 + 0x658));
        import_0024D704(*(int *)(*(char **)(arg0 + 0xD4) + 0x568),
                        import_0024E060(*(int *)(*(char **)(arg0 + 0xD4) + 0x568), 0xA0, &sp60),
                        0x46, &sp60);
        mgrproc_uso_func_083574(&sp60, (n2 = &import_80264A3C + 0x66C));
        import_0024D704(*(int *)(*(char **)(arg0 + 0xD4) + 0x568),
                        import_0024E060(*(int *)(*(char **)(arg0 + 0xD4) + 0x568), 0xA0, &sp60),
                        0x64, &sp60);
        return;
    case 3:
        s2 = -1 - *(int *)(a3 + 0x7E0);
        import_0024D108(*(int *)(a3 + 0x568));
        s0 = 0;
        do {
            a3 = *(char **)(arg0 + 0xD4);
            idx = *(int *)(a3 + 0x7DC) + s0;
            s1 = ((char **)&import_8025CAF8)[idx];
            digit = *s1 - 0x30;
            s1++;
            switch (digit) {
            case 0:
                sub = a3 + 0x458;
                break;
            case 1:
                sub = a3 + 0x47C;
                break;
            case 2:
                sub = a3 + 0x4A0;
                break;
            }
            import_0024E608(&import_8024CAF8, 0xFF, sub);
            import_0024D704(*(int *)(*(char **)(arg0 + 0xD4) + 0x568),
                            import_0024E060(*(int *)(*(char **)(arg0 + 0xD4) + 0x568), 0xA0, s1),
                            s2, s1);
            s0 += 1;
            s2 += 0x10;
        } while (s0 != 0x10);
        a3 = *(char **)(arg0 + 0xD4);
        if (*(int *)(a3 + 0x7E8) < 0x96) {
            if (*(int *)(arg0 + 0x170) >= 0x79) {
                *(int *)(arg0 + 0x170) = *(int *)(arg0 + 0x170) - 1; if (arg0) {}
                a3 = *(char **)(arg0 + 0xD4);
            }
            import_0024E608(&import_8024CAF8, 0xFF, a3 + 0x4C4);
            panel = &import_80263D48 + 0x18;
            import_0024F2C8(panel);
            import_0024F34C(panel, 0xA0, *(int *)(arg0 + 0x170), 3);
        }
        return;
    case 4:
        if (*(int *)(arg0 + 0x170) >= 0x79) {
            *(int *)(arg0 + 0x170) = *(int *)(arg0 + 0x170) - 1; if (arg0) {}
            a3 = *(char **)(arg0 + 0xD4);
        }
        import_0024E608(&import_8024CAF8, 0xFF, a3 + 0x4C4);
        panel2 = &import_80263D48 + 0x18;
        import_0024F2C8(panel2);
        import_0024F34C(panel2, 0xA0, *(int *)(arg0 + 0x170), 3);
        break;
    }
}
