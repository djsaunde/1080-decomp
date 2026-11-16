/* IDO 7.1 -O2 donor for Node_doCommand -- opcode-record interpreter step
 * (0x144 / 81 words; 81/81 word-exact apart from reloc fields).
 *
 * WIRED via REPLACE_FUNC_BODY donor splice into game_libs_post0b.c.o (the
 * 29CCC / 52CD4 / 3AC5C recipe, docs/MATCHING_WORKFLOW.md#post0b-jumptable-
 * donor-52cd4). The C switch over opcodes 0x64..0x6C puts its 9-entry
 * jumptable in the donor's local .rodata; the splice renames that reloc to
 * Node_doCommand_rodata, pinned to 0x1A1C in undefined_syms_auto.txt (the
 * target's `lw t6,0x1A1C(at)`). The three baked lui/addiu message addresses
 * are the existing sFmt_Global_out_of_range_1EC6C / sFmt_Global_out_of_range_1EC84 / sFmt_unknown_Node_cmd_for_1EC9C pins; the callbacks
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
#include "gdl/gl_data.h"
#include "gdl/node.h"
#include "uso_call.h"

extern int gl_func_00000000();
extern int D_00000000;
extern char sFmt_Global_out_of_range_1EC6C[], sFmt_Global_out_of_range_1EC84[], sFmt_unknown_Node_cmd_for_1EC9C[];

void Node_doCommand(char *o, char *c) {
    unsigned int key;
    int code;
    volatile int pad[4];
    char *r;
    int *p;
    switch (code = *(int *)c) {
        case 0x64:
            key = *(unsigned int *)(c + 0x4);
            *(unsigned int *)(c + 0x4) = key + 4;
            NODE(o)->handle = *(int *)key;
            return;
        case 0x65:
            p = *(int **)(c + 0x4);
            *(int **)(c + 0x4) = p + 1;
            key = *p;
            NODE(o)->name = key;
            return;
        case 0x66:
            p = *(int **)(c + 0x4);
            *(int **)(c + 0x4) = p + 1;
            key = *p;
            if (key >= 40) {
                CALL(sys_fatal)(sFmt_Global_out_of_range_1EC6C, key);
            }
            r = *(char **)((char *)&gNodeGlobals + key * 4);
            CALL(list_append)(&NODE(r)->children, o);
            if (NODE(o)->parent != 0) {
                NODE(o)->unk04 = 1;
            }
            NODE(o)->parent = NODE(r);
            return;
        case 0x67:
            p = *(int **)(c + 0x4);
            *(int **)(c + 0x4) = p + 1;
            key = *p;
            if (key >= 40) {
                CALL(sys_fatal)(sFmt_Global_out_of_range_1EC84, key);
            }
            *(char **)((char *)&gNodeGlobals + key * 4) = o;
            return;
        case 0x6C:
            return;
    }
    key = code; code = 0;
    p = (int *)NODE(o)->name; CALL(sys_fatal)(sFmt_unknown_Node_cmd_for_1EC9C, key, p);
}
