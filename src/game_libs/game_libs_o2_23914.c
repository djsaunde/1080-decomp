/* IDO 7.1 -O2 donor for gl_func_00023914 (0x1F4 / 125 words) -- resource
 * acquire: begin (baked jal 0x381B0) short-circuits, else fetch the table
 * entry (baked jal 0x38204), allocate by mode (5-entry switch), fill it,
 * then notify per a0. Word-exact standalone apart from relocations.
 *
 * WIRED via REPLACE_FUNC_BODY donor splice into game_libs_post.c.o (the
 * 29CCC / 343F4 recipe): the C switch puts its 5-entry jumptable in the
 * donor's local .rodata; the splice renames that reloc to
 * gl_func_00023914_rodata, pinned in undefined_syms_auto.txt to the table
 * the USO ships in game_libs rodata at +0xEA0.
 *
 * Levers (agent-c run 114 decode + run 122 fix): the two leading callees
 * are the baked in-unit jals 0x381B0 / 0x38204 (gl_ref_ pins; the run-114
 * NM called the in-file 23B44 / 23B98, which the raw diff exposed);
 * `case 4: break;` keeps the 5-entry table; `(char *)((int)tbl + a1 *
 * 0x10)` gives addu tbl-first; the size is computed into one local twice
 * (the target keeps the dead first store); `if (mode == 0) k = 5; else
 * k = 2;` gives the b-around-b tail. */
#include "common.h"

extern int gl_func_00000000();
extern int gl_ref_000381B0();
extern int gl_ref_00038204();

int gl_func_00023914(int a0, int a1, int *a2) {
    int size;
    char *tbl;
    char *e;
    int kind;
    int mode;
    int src;
    int r;
    int k;

    r = gl_ref_000381B0(a0, a1, a2);
    if (r != 0) {
        *a2 = 0;
        k = 2;
    } else {
        tbl = (char *)gl_ref_00038204(a0);
        e = (char *)((int)tbl + a1 * 0x10);
        size = *(int *)(e + 0x14);
        size = (size + 0xF) & ~0xF;
        kind = *(s8 *)(e + 0x18);
        mode = *(s8 *)(e + 0x19);
        src = *(int *)(e + 0x10);
        switch (mode) {
        case 0:
            r = gl_func_00000000(a0, a1, size);
            if (r == 0) return r;
            break;
        case 1:
            r = gl_func_00000000(a0, size, 1, a1);
            if (r == 0) return r;
            break;
        case 2:
            r = gl_func_00000000(a0, size, 0, a1);
            if (r == 0) return r;
            break;
        case 3:
            r = gl_func_00000000(a0, size, 2, a1);
            if (r == 0) return r;
            break;
        case 4:
            break;
        }
        *a2 = 1;
        if (kind == 1) {
            gl_func_00000000(src, r, size, *(s16 *)(tbl + 2));
        } else {
            gl_func_00000000(src, r, size, kind);
        }
        if (mode == 0) {
            k = 5;
        } else {
            k = 2;
        }
    }
    switch (a0) {
    case 0:
        gl_func_00000000(a1, k);
        break;
    case 1:
        gl_func_00000000(a1, k);
        break;
    case 2:
        gl_func_00000000(a1, k);
        break;
    }
    return r;
}
