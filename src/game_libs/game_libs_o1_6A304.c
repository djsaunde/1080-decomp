#include "uso_call.h"
#include "common.h"
#include "libultra/os_task.h"
/* -O1 donor for gl__VirtualToPhysicalTask — 71-insn pointer-relocation fixup driver
 * (frame 0x20, ra only). PROVEN EXACT 71/71 at IDO 7.1 -O1 (2026-07-08
 * standalone probe: 69/71 raw with the 2 remaining words being the UNBAKED
 * HI16/LO16 of D_00041710, which link to the target's lui 0x4 / addiu 0x1710;
 * final-ROM byte gate confirms). post1b builds -O2, which constant-folds every
 * field access into absolute lui/lw (the old "deferred: symbol plumbing" cap
 * note). Levers:
 *   (1) real relocated symbol D_00041710 (= 0x41710 in undefined_syms_auto.txt)
 *       so the base stays a symbol address held in a reg/stack home — the
 *       0x41710 int-constant form emits lui/ori (target has lui/addiu);
 *   (2) plain -O1: obj homed at sp+0x1C and reloaded `lw tN,0x1C(sp)` before
 *       every field test/store, a0 homed+reloaded for the first call. */
extern int gl_func_00000000();
extern char D_00041710[];

int gl__VirtualToPhysicalTask(int a0) {
    OSTask_t *obj;

    obj = (OSTask_t *)D_00041710;
    CALL(gl_bcopy)(a0, obj, sizeof(OSTask_t));
    if (obj->ucode) {
        obj->ucode = (u64 *)CALL(gl_osVirtualToPhysical)(obj->ucode);
    }
    if (obj->ucode_data) {
        obj->ucode_data = (u64 *)CALL(gl_osVirtualToPhysical)(obj->ucode_data);
    }
    if (obj->dram_stack) {
        obj->dram_stack = (u64 *)CALL(gl_osVirtualToPhysical)(obj->dram_stack);
    }
    if (obj->output_buff) {
        obj->output_buff = (u64 *)CALL(gl_osVirtualToPhysical)(obj->output_buff);
    }
    if (obj->output_buff_size) {
        obj->output_buff_size = (u64 *)CALL(gl_osVirtualToPhysical)(obj->output_buff_size);
    }
    if (obj->data_ptr) {
        obj->data_ptr = (u64 *)CALL(gl_osVirtualToPhysical)(obj->data_ptr);
    }
    if (obj->yield_data_ptr) {
        obj->yield_data_ptr = (u64 *)CALL(gl_osVirtualToPhysical)(obj->yield_data_ptr);
    }
    return (int)obj;
}
