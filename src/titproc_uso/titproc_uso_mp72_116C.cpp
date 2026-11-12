/* MIPSpro 7.2.1 C++ (o32 edgcpfe, EDG 2.29) NON_MATCHING donor for titproc_uso_func_0000116C (290 words),
 * compiled by tools/ncc721.sh and spliced into the non_matching object only (agent-h run 355). The ROM
 * build keeps INCLUDE_ASM.
 *
 * The target's `switch (s0->0x40)` has 4 labels lowered as a 4-entry jump table (sltiu 4), which only
 * EDG 2.29's switch rule produces; the cfe NM body in titproc_uso.c needs a phantom `case 4:` (sltiu 5).
 * This is that body (agent-g / agent-c runs 145..234) with the phantom label dropped and the vtable
 * this-adjust written `(*(short *)(vt + 0x58) + (int)v1)` (edg puts `ptr + short` pointer-first).
 * 99.514 (cfe) -> 99.586. lay_5DC only puts this function's table at .rodata+0x30 (the target's %lo:
 * 5DC's 12-word table is titproc RoData +0x0); it is not spliced.
 * LEFT (22 words, the cfe residual set): the constant 40 colours a1 (target v0) at its three sites, and
 * in case 3 the s0->0x64 object loads take v0 with `c` in v1 (target: v1 / v0). */
extern "C" {
extern int gl_func_00000000(...);
extern char D_00000000;
extern char import_00020098;
extern char import_00263D30;
extern void gl_func_00000000_116c_v(...);

void titproc_uso_func_000005DC_lay(int x, int *o) {
    switch (x) {
    case 0: o[0] = 10; break; case 1: o[1] = 11; break; case 2: o[2] = 12; break; case 3: o[3] = 13; break;
    case 4: o[4] = 14; break; case 5: o[5] = 15; break; case 6: o[6] = 16; break; case 7: o[7] = 17; break;
    case 8: o[8] = 18; break; case 9: o[9] = 19; break; case 10: o[10] = 20; break; case 11: o[11] = 21; break;
    }
}

void titproc_uso_func_0000116C(char *s0) {
    char *v1, *vt;
    char *d3;
    char *vt2;
    char *vt3;
    char *vt4;
    char *vt5;
    char *vt6;
    char *vt7;
    char *vt8;
    void (*fn)(int);
    int v;

    switch (*(int *)(s0 + 0x40)) {
    case 0: {
        *(int *)(s0 + 0x3C) = *(int *)(s0 + 0x3C) - 1;
        if (*(int *)(s0 + 0x3C) >= 0) {
            if (gl_func_00000000((char *)&D_00000000, 0x40300) == 0) {
                return;
            }
        }
        vt = *(char **)(*(char **)(s0 + 0x58) + 0x28);
        v1 = *(char **)(s0 + 0x58);
        (*(void (**)(int))(vt + 0x5C))((*(short *)(vt + 0x58) + (int)v1));
        gl_func_00000000_116c_v(*(int *)(s0 + 0x58));
        *(int *)(s0 + 0x40) = *(int *)(s0 + 0x40) + 1;
        *(int *)(s0 + 0x3C) = *(int *)(s0 + 0x70) * 30;
        *(int *)(s0 + 0x68) = 40;
        gl_func_00000000_116c_v(13);
        break;
    }
    case 1: {
        v = *(int *)(s0 + 0x68);
        if (v != 0) {
            *(int *)(s0 + 0x68) = v - 1; if (s0) {}
            v = *(int *)(s0 + 0x68);
        }
        if (v == 0) {
            vt2 = *(char **)(*(char **)(s0 + 0x54) + 0x28);
            v1 = *(char **)(s0 + 0x54);
            (*(void (**)(int))(vt2 + 0x5C))((*(short *)(vt2 + 0x58) + (int)v1));
        }
        *(int *)(s0 + 0x3C) = *(int *)(s0 + 0x3C) - 1;
        if (*(int *)(s0 + 0x3C) == 0) goto fire;
        if (*(int *)(*(char **)(*(char **)(s0 + 0x50) + 0x44) + 0x34) != 0) {
fire:
            *(int *)((char *)&D_00000000 + 0x34) = 1;
            v = gl_func_00000000();
            gl_func_00000000_116c_v(s0, v, 0);
            return;
        }
        if (*(int *)(s0 + 0x68) != 0) {
            return;
        }
        if (gl_func_00000000((char *)&D_00000000, 0x40300) == 0) {
            return;
        }
        gl_func_00000000_116c_v(*(int *)(s0 + 0x58));
        gl_func_00000000_116c_v(*(int *)(s0 + 0x54));
        if (*(unsigned short *)(*(char **)((char *)&D_00000000 + 0x154) + 4) & 8) {
            vt3 = *(char **)(*(char **)(s0 + 0x60) + 0x28);
            v1 = *(char **)(s0 + 0x60);
            (*(void (**)(int))(vt3 + 0x5C))((*(short *)(vt3 + 0x58) + (int)v1));
            *(int *)(s0 + 0x40) = 2;
            gl_func_00000000_116c_v(144);
        } else {
            vt4 = *(char **)(*(char **)(s0 + 0x5C) + 0x28);
            v1 = *(char **)(s0 + 0x5C);
            (*(void (**)(int))(vt4 + 0x5C))((*(short *)(vt4 + 0x58) + (int)v1));
            *(int *)(s0 + 0x40) = 3;
            *(int *)(s0 + 0x3C) = *(int *)(s0 + 0x74) * 30;
            gl_func_00000000_116c_v(144);
        }
        break;
    }
    case 2: {
        gl_func_00000000_116c_v(*(int *)(s0 + 0x60));
        if (gl_func_00000000((char *)&D_00000000, 0x40100) != 0) {
            gl_func_00000000_116c_v(5);
            if (1) {
                char *z = *(char **)((char *)&D_00000000 + 0x154) + 4;
                *(short *)z = (short)(*(unsigned short *)z & 0xFFF7);
            }
            gl_func_00000000_116c_v((char *)&D_00000000);
            gl_func_00000000_116c_v(*(int *)(s0 + 0x60));
            vt5 = *(char **)(*(char **)(s0 + 0x5C) + 0x28);
            v1 = *(char **)(s0 + 0x5C);
            (*(void (**)(int))(vt5 + 0x5C))((*(short *)(vt5 + 0x58) + (int)v1));
            *(int *)(s0 + 0x40) = 3;
            *(int *)(s0 + 0x3C) = *(int *)(s0 + 0x74) * 30;
        }
        break;
    }
    case 3: {
        int *c;
        if (*(int *)(s0 + 0x3C) == 0) {
            *(int *)((char *)&D_00000000 + 0x34) = 1;
            v = gl_func_00000000();
            gl_func_00000000_116c_v(s0, v, 0);
        }
        if (*(int *)(*(char **)(*(char **)(s0 + 0x50) + 0x44) + 0x34) != 0) {
            c = (int *)(*(char **)(s0 + 0x64) + 0x3C);
            if (*(int *)(*(char **)(s0 + 0x64) + 0x3C) < 255) {
                *c = *c + 16;
                if (*(int *)(*(char **)(s0 + 0x64) + 0x3C) >= 256) {
                    *(int *)(*(char **)(s0 + 0x64) + 0x3C) = 720;
                }
            } else {
                d3 = (char *)&D_00000000;
                vt6 = *(char **)(d3 + 0x28);
                (*(void (**)(int))(vt6 + 0x64))((*(short *)(vt6 + 0x60) + (int)d3));
            }
        } else {
            int av;
            v1 = *(char **)(s0 + 0x64);
            av = *(int *)(v1 + 0x3C);
            c = (int *)(v1 + 0x3C);
            if (av > 0) {
                *c = *c - 16;
                v1 = *(char **)(s0 + 0x64);
                av = *(int *)(v1 + 0x3C);
                if (av < 0) {
                    *(int *)(v1 + 0x3C) = 0;
                    av = *(int *)(*(char **)(s0 + 0x64) + 0x3C);
                }
            }
            if (av == 0) {
                if (gl_func_00000000((char *)&D_00000000, 512) != 0) {
                    vt7 = *(char **)(*(char **)(s0 + 0x58) + 0x28);
                    v1 = *(char **)(s0 + 0x58);
                    (*(void (**)(int))(vt7 + 0x5C))((*(short *)(vt7 + 0x58) + (int)v1));
                    gl_func_00000000_116c_v(*(int *)(s0 + 0x58));
                    vt8 = *(char **)(*(char **)(s0 + 0x54) + 0x28);
                    v1 = *(char **)(s0 + 0x54);
                    (*(void (**)(int))(vt8 + 0x5C))((*(short *)(vt8 + 0x58) + (int)v1));
                    gl_func_00000000_116c_v(*(int *)(s0 + 0x5C));
                    *(int *)(s0 + 0x40) = 1;
                    *(int *)(s0 + 0x3C) = *(int *)(s0 + 0x70) * 30;
                    gl_func_00000000_116c_v(2050);
                }
            }
            if (gl_func_00000000((char *)&D_00000000, 0x40100) != 0) {
                gl_func_00000000_116c_v(*(int *)(s0 + 0x5C));
                gl_func_00000000_116c_v(5); if (*(char **)(s0 + 0x5C)) {}
                v1 = *(char **)(s0 + 0x5C);
                if (*(int *)(v1 + *(int *)(v1 + 0x7C) * 40 + 0x90) != 0) {
                    *(char **)((char *)&import_00263D30 + 0xA8) = s0;
                    *(int *)((char *)&import_00020098 + 0x84) = *(int *)(*(char **)(s0 + 0x5C) + 0x7C); if (*(char **)(s0 + 0x5C)) {}
                    v1 = *(char **)(s0 + 0x5C);
                    ((void (*)(void))(*(int *)(v1 + *(int *)(v1 + 0x7C) * 40 + 0x90)))();
                }
            }
        }
        break;
    }
    }
}

}
