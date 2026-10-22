/* IDO 7.1 NCC (C++) -O0 donors for func_00011E00 / func_00011ED4 / func_00011FA8
 * (0xD4 / 53 words each): "first slot whose mask word (+0x18 / +0x10 / +0x1C)
 * has an a1 bit" (slot[] = a0 + 0xE0, a0->[0x128] == -1 scans all a0->[0x120]
 * slots). bootup_uso is C++: under cfe -O0 every fn carried an extra dead
 * `b epi; nop` pair before the epilogue (the closing-brace marker of the
 * return-value dead-double-b gap); edg's -O0 ucode has no such marker, so the
 * same body compiles to the shipped 53 words.
 * Spliced into bootup_uso_o0_11E00.c.o via REPLACE_FUNC_BODY. */
extern "C" int func_00011E00(register int *a0, register int a1) {
    int i;
    register int *p;
    if (a0[0x18C/4] == 0) {
        if (a0[0x128/4] == -1) {
            i = 0;
            if (a0[0x120/4] > 0) do {
                p = (int*)a0[0xE0/4 + i];
                if (p[0x18/4] & a1) return 1 << i;
            } while (++i < a0[0x120/4]);
            goto ret0;
        }
        p = (int*)a0[0xE0/4 + a0[0x128/4]];
        if (p[0x18/4] & a1) return 1 << a0[0x128/4];
    }
ret0:
    return 0;
}

extern "C" int func_00011ED4(register int *a0, register int a1) {
    int i;
    register int *p;
    if (a0[0x18C/4] == 0) {
        if (a0[0x128/4] == -1) {
            i = 0;
            if (a0[0x120/4] > 0) do {
                p = (int*)a0[0xE0/4 + i];
                if (p[0x10/4] & a1) return 1 << i;
            } while (++i < a0[0x120/4]);
            goto ret0;
        }
        p = (int*)a0[0xE0/4 + a0[0x128/4]];
        if (p[0x10/4] & a1) return 1 << a0[0x128/4];
    }
ret0:
    return 0;
}

extern "C" int func_00011FA8(register int *a0, register int a1) {
    int i;
    register int *p;
    if (a0[0x18C/4] == 0) {
        if (a0[0x128/4] == -1) {
            i = 0;
            if (a0[0x120/4] > 0) do {
                p = (int*)a0[0xE0/4 + i];
                if (p[0x1C/4] & a1) return 1 << i;
            } while (++i < a0[0x120/4]);
            goto ret0;
        }
        p = (int*)a0[0xE0/4 + a0[0x128/4]];
        if (p[0x1C/4] & a1) return 1 << a0[0x128/4];
    }
ret0:
    return 0;
}
