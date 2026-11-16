#include "common.h"
#include "gdl/backup.h"
#include "uso_call.h"

extern int gl_func_00000000();
extern int gInputMgr_backup_148, gInputMgr_session_14C, D_00000068;

/* -O0 donor for arc_openSession_000005C8 (48-insn / 0xC0 alloc wrapper).
 * Spliced into the -O2 tail1 object via REPLACE_FUNC_BODY (real -O0 output).
 * arcproc is non-Yay0, so the imported R_MIPS HI16/LO16 vs gInputMgr_backup_148/14C/68
 * and the jal-0 + R_MIPS_26 gl_func_00000000 relocs are resolved by the linker
 * (same as the arcproc_uso_o0_748 donor).
 *
 * KEY LEVER (breaks the 2026-06-24 "NOT C-steerable temp-slot" cap): the single
 * reused temp `saved` (holding the two gl_func_00000000(2) results) must home to
 * sp+0x24, but the prior body declared `register int s0;` FIRST, which at -O0
 * grabbed the top local slot 0x24 (unused, s0-resident) and pushed `saved` down
 * to 0x20. Declaring the stack temp `saved` BEFORE the register var gives it the
 * 0x24 slot -> byte-exact. Decl-order slot lever (docs/IDO_CODEGEN.md -O0 kit). */
void arc_openSession_000005C8(int *a0) {
    int saved;
    register int s0;
    saved = CALL(app_setHeapMode)(2);
    a0[2] = CALL(mem_new)(0x38);
    CALL(app_setHeapMode)(saved);
    saved = CALL(app_setHeapMode)(2);
    s0 = CALL(Backup_openSession)(0, gInputMgr_backup_148, -1, -1);
    a0[0] = s0;
    gInputMgr_session_14C = s0;
    CALL(app_setHeapMode)(saved);
    ((BackupSession *)a0[0])->modes = 4;
    D_00000068 = 0;
}
