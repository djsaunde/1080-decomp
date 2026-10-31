/* IDO 7.1 NCC (C++) donor for mgrproc_uso_func_00002B7C (0x2B8 / 174 words), EXACT (agent-c run 244, see the
 * end of this comment). History: agent-c run 243 (NM donor).
 * The in-tree cfe body (mgrproc_uso.c, 98.24) compiled verbatim under NCC scores 98.17; the four volatile 1.0f
 * "ghost" stores of the cfe body are an unused C++ colour object with an inline constructor (`Col col;` -- the
 * ctor's `this` keeps the object in memory, so the stores survive without volatile): 99.39, and the first
 * `lw a3,0xD4(a0)` now schedules after the four swc1 like the target. Frame (sp offsets only): every named
 * local edg keeps is a home word -- n1 declared above col gives the unused top word (col at 0xA4..0xB0, frame
 * 0xB8), the n1 / n2 string-argument names and the split case-4 panel2 give the two extra low homes (the case-1
 * spill lands at 0x5C, sp60 at 0x60, sub at 0xA0 = target). 99.45, length = target.
 * Then the glyph pointer is advanced where it is loaded (`s1 = table[idx] + 1; digit = s1[-1] - 0x30;`): one
 * definition of s1 instead of two, the counter takes s0 and the glyph pointer s1 as in the target, and the lbu /
 * addiu pair stays in target order (99.45 -> 99.88).
 * Run 244 (kit CCLXVII), EXACT: case 1's panel pointer is its own variable p (homed at sp+0x5C, declared right
 * after sp60), the case-1 value the target keeps in v0 across the D108 call. A plain `p = a3 + 0x490` is
 * copy-propagated into p + 0x10 = a3 + 0x4A0 (a3 is then the value held across the call); a dead re-read of a3
 * after the D108 call stops that, and p then takes a callee-saved register because s0..s3 are already in use
 * (cost 0) by the case-3 loop variables and nothing in case 1 interferes with them. The target's v0 needs p to
 * interfere with the s0..s3 owners: empty tests on s1 / s2 / sub / s0 before the E608 call and on sub after it
 * keep those loop variables live (unset, from function entry) through case 1, `if (p) {}` raises p's weight,
 * and D108 is declared void (its return value is never used anywhere), which takes v0 off p's forbidden set.
 * The empty tests on unset locals rely on the uninitialised-local exception (docs/MATCHING_WORKFLOW.md): the
 * target itself reads the never-written home of `sub` (`lw s3,0xA0(sp)` at 0x2CA4, before the case-3 loop; the
 * only store to 0xA0(sp) is `sw s3,0xA0(sp)` at 0x2D58, after it). `sub` is the loop-carried
 * conditionally-assigned value of the target (no default arm), as in the cfe body.
 * Spliced over the placeholder in mgrproc_uso.c via REPLACE_FUNC_BODY (both build paths). */
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
extern void import_0024D108(...);
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
    char *p;
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
        p = a3 + 0x490;
        if (p) {}
        import_0024D108(*(int *)(a3 + 0x568));
        a3 = *(char **)(arg0 + 0xD4);
        /* Uninitialised-local exception: the target reads sub's never-written home (`lw s3,0xA0(sp)` at 0x2CA4). */
        if (s1) {}
        if (s2) {}
        if (sub) {}
        if (s0) {}
        import_0024E608(&import_8024CAF8, 0xFF, p + 0x10);
        /* Uninitialised-local exception: the target reads sub's never-written home (`lw s3,0xA0(sp)` at 0x2CA4). */
        if (sub) {}
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
            s1 = ((char **)&import_8025CAF8)[idx] + 1;
            digit = s1[-1] - 0x30;
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
