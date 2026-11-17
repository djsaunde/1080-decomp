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
 *  - the handle arm's cursor is `key` (the key arms' a3), the other arms' cursor p;
 *  - the default's third argument goes through p: its call-argument
 *    preference (a2) puts every case cursor in a2, so key takes a3;
 *  - `key = code; code = 0;` in the default: the selector's web ends at the
 *    copy (no a1 argument preference of its own), so it colours v0; the
 *    report argument is the copy's own web (a1);
 *  - the p assignment and the call share one source line (as1 order
 *    lui/addiu/lw before the jal);
 *  - declaration order + pad[4] give the 64-byte frame and the 60 / 36 homes.
 * Case labels (agent-c run 829) read off the ROM table (port/tools/jumptable-audit.py):
 * 0x64 name, 0x65 parent (gNodeGlobals[key]'s child), 0x67 global slot, 0x6C handle; 0x66 and
 * 0x68..0x6B are the default (fatal "unknown Node cmd"). The PC port (gcc builds its table
 * from the labels) uses those. The matching build keeps the old labels (0x64 handle, 0x65 name,
 * 0x66 parent, plus an empty `case 0x6C: return;`): they link the same ROM bytes (the table is
 * the ROM's: same 0x64..0x6C range, same arm order), but with the true ones the objdiff report
 * loses the exact (the donor's own table is what it compares). */
#include "common.h"
#include "gdl/gl_data.h"
#include "gdl/node.h"
#include "uso_call.h"

extern int gl_func_00000000();
extern int D_00000000;
extern char sFmt_Global_out_of_range_1EC6C[], sFmt_Global_out_of_range_1EC84[], sFmt_unknown_Node_cmd_for_1EC9C[];

#define CMD(c) ((NodeMsg *)(c))
#ifdef PORT /* the ROM table's values (see above) */
#define NODECMD_NAME 0x64
#define NODECMD_PARENT 0x65
#define NODECMD_HANDLE 0x6C
#else
#define NODECMD_NAME 0x65
#define NODECMD_PARENT 0x66
#define NODECMD_HANDLE 0x64
#endif

void Node_doCommand(char *o, char *c) {
    unsigned int key;
    int code;
    volatile int pad[4];
    char *r;
    int *p;
    switch (code = *(int *)c) {
        case NODECMD_HANDLE:
            /* c is the NodeMsg (node.h): tag = opcode, arg.cursor = the stream read cursor */
            key = (unsigned int)CMD(c)->arg.cursor;
            CMD(c)->arg.cursor = (s32 *)(key + 4);
            NODE(o)->handle = *(int *)key;
            return;
        case NODECMD_NAME:
            p = (int *)CMD(c)->arg.cursor;
            CMD(c)->arg.cursor = (s32 *)(p + 1);
            key = *p;
            NODE(o)->name = (const char *)key;
            return;
        case NODECMD_PARENT:
            p = (int *)CMD(c)->arg.cursor;
            CMD(c)->arg.cursor = (s32 *)(p + 1);
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
        case NDC_SETGLOBAL:
            p = (int *)CMD(c)->arg.cursor;
            CMD(c)->arg.cursor = (s32 *)(p + 1);
            key = *p;
            if (key >= 40) {
                CALL(sys_fatal)(sFmt_Global_out_of_range_1EC84, key);
            }
            *(char **)((char *)&gNodeGlobals + key * 4) = o;
            return;
#ifndef PORT
        case 0x6C:
            return;
#endif
    }
    key = code; code = 0;
    p = (int *)NODE(o)->name; CALL(sys_fatal)(sFmt_unknown_Node_cmd_for_1EC9C, key, p);
}
