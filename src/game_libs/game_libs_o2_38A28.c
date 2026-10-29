/* IDO 7.1 -O2 donor for gl_func_00038A28 -- opcode-record interpreter step
 * (0x144 / 81 words; 81/81 word-exact apart from reloc fields).
 *
 * WIRED via REPLACE_FUNC_BODY donor splice into game_libs_post0b.c.o (the
 * 29CCC / 52CD4 / 3AC5C recipe, docs/MATCHING_WORKFLOW.md#post0b-jumptable-
 * donor-52cd4). The C switch over opcodes 0x64..0x6C puts its 9-entry
 * jumptable in the donor's local .rodata; the splice renames that reloc to
 * gl_func_00038A28_rodata, pinned to 0x1A1C in undefined_syms_auto.txt (the
 * target's `lw t6,0x1A1C(at)`). The three baked lui/addiu message addresses
 * are the existing D_0001EC6C / D_0001EC84 / D_0001EC9C pins; the callbacks
 * are `jal 0` in the ROM (0-pinned gl_func_00000000).
 *
 * Register levers (agent-g runs 207-209, docs/IDO_CODEGEN.md kit CXCI):
 *  - case 0x64's cursor is `key` (the key arms' a3), the other arms' cursor p;
 *  - the default's third argument goes through p: its call-argument
 *    preference (a2) puts every case cursor in a2, so key takes a3;
 *  - `key = code; code = 0;` in the default: the selector's web ends at the
 *    copy (no a1 argument preference of its own), so it colours v0; the
 *    report argument is the copy's own web (a1);
 *  - the p assignment and the call share one source line (as1 order
 *    lui/addiu/lw before the jal);
 *  - declaration order + pad[4] give the 64-byte frame and the 60 / 36 homes. */
#include "common.h"

extern int gl_func_00000000();
extern int D_00000000;
extern char D_0001EC6C[], D_0001EC84[], D_0001EC9C[];

void gl_func_00038A28(char *o, char *c) {
    unsigned int key;
    int code;
    volatile int pad[4];
    char *r;
    int *p;
    switch (code = *(int *)c) {
        case 0x64:
            key = *(unsigned int *)(c + 0x4);
            *(unsigned int *)(c + 0x4) = key + 4;
            *(int *)(o + 0x20) = *(int *)key;
            return;
        case 0x65:
            p = *(int **)(c + 0x4);
            *(int **)(c + 0x4) = p + 1;
            key = *p;
            *(int *)(o + 0x0C) = key;
            return;
        case 0x66:
            p = *(int **)(c + 0x4);
            *(int **)(c + 0x4) = p + 1;
            key = *p;
            if (key >= 40) {
                gl_func_00000000(D_0001EC6C, key);
            }
            r = *(char **)((char *)&D_00000000 + key * 4);
            gl_func_00000000(r + 0x10, o);
            if (*(int *)(o + 0x14) != 0) {
                *(int *)(o + 0x4) = 1;
            }
            *(char **)(o + 0x14) = r;
            return;
        case 0x67:
            p = *(int **)(c + 0x4);
            *(int **)(c + 0x4) = p + 1;
            key = *p;
            if (key >= 40) {
                gl_func_00000000(D_0001EC84, key);
            }
            *(char **)((char *)&D_00000000 + key * 4) = o;
            return;
        case 0x6C:
            return;
    }
    key = code; code = 0;
    p = *(int **)(o + 0xC); gl_func_00000000(D_0001EC9C, key, p);
}
