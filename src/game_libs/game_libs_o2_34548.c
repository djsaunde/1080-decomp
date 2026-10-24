/* IDO 7.1 -O2 donor for gl_func_00034548 (0x13C / 79 words, the
 * subsystem poll loop). Spliced into game_libs_post0b.c.o via
 * REPLACE_FUNC_BODY; only gl_func_00034548 is imported.
 *
 * Why a donor: the loop is infinite, and IDO places the unreachable
 * epilogue after an infinite loop on a 32-byte boundary of the .text
 * section (`for (;;) g();` epilogues land at 0x20, 0x60, 0xA0, ... in a
 * test file). The target's epilogue sits at 0x34654 (2 nops after the
 * back branch), which is 32-aligned relative to 0x343F4: the original
 * translation unit starts at game_libs_func_000343F4. splat cut that unit
 * at 0x34458, so in game_libs_post0b.c the same C gets 3 nops (320
 * bytes). This file is the original unit's head: the real C of its two
 * leading functions (343F4 = game_libs_o2_343F4.c, 34458 =
 * game_libs_post0b.c, both exact) followed by 34548, which then compiles
 * to the target's 316 bytes.
 *
 * 34548 itself (34458 sibling recipe, agent-c run 147): the loop is
 * infinite, the poll result is stored BEFORE the zero test (target sw in
 * the beqz delay) and re-read after every call; the slot global is a
 * pinned symbol at 0x3BB34 (s1 = &slot, lw 0(s1)); the vtable record is a
 * distinct base-0 extern; homes 92/84/80 = sp5C, s0, the out pair in
 * that declaration order. */
extern char D_00000000;
extern int gl_func_00000000();

int game_libs_func_000343F4(int a0) {
    switch (a0) {
    case 2:
        return (int)(&D_00000000 + 0x1E3BC);
    case 3:
        return (int)(&D_00000000 + 0x1E3C8);
    case 5:
        return (int)(&D_00000000 + 0x1E3D4);
    case 1:
        return (int)(&D_00000000 + 0x1E3E0);
    case 8:
    default:
        return (int)(&D_00000000 + 0x1E3EC);
    }
}

extern char *D_34458_head;
extern char *D_34458_obj;
int gl_func_00034458() {
    char *n = D_34458_head;
    volatile int pad;
    int outB, outA;
    int s1;
    int vB;
    gl_func_00000000((char *)&D_00000000 + 0x1E3F4);
    while (*(int *)(n + 4) != -1) {
        (*(int (**)(int, int *, int *))(D_34458_obj + 0x70))(*(int *)(n + 0x11C), &outA, &outB);
        s1 = gl_func_00000000(*(int *)(n + 0x14));
        vB = gl_func_00000000(*(unsigned short *)(n + 0x10));
        gl_func_00000000((char *)&D_00000000 + 0x1E400, s1, *(int *)(n + 0x14), vB,
                         outA, outB, *(int *)(n + 0x11C));
        n = *(char **)(n + 0xC);
    }
}

extern char *D_34548_obj;
extern char *D_34548_slot;
void gl_func_00034548(void *a0) {
    int sp5C;
    char *s0;
    int sp54, sp50;
    int r5;
    gl_func_00000000(0xC, (char *)&D_00000000 + 0x3BB18, 0x10);
    *(int *)((char *)&D_00000000 + 0x3BB38) = 0;
    for (;;) {
        gl_func_00000000((char *)&D_00000000 + 0x3BB18, &sp5C, 1);
        if ((D_34548_slot = (char *)gl_func_00000000()) == 0) {
            continue;
        }
        s0 = D_34548_slot + 0x20;
        (*(int (**)())(D_34548_obj + 0x70))(*(int *)(s0 + 0xFC), &sp50, &sp54);
        gl_func_00000000((char *)&D_00000000 + 0x1E424);
        gl_func_00000000(D_34548_slot);
        r5 = gl_func_00000000(*(int *)(D_34548_slot + 0x14));
        gl_func_00000000((char *)&D_00000000 + 0x1E438, sp50, sp54, *(int *)(s0 + 0xFC), r5,
                         *(int *)(D_34548_slot + 0x14));
    }
}
