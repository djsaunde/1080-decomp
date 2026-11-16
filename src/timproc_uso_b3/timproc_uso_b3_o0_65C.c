#include "common.h"
#include "uso_call.h"

extern int gl_func_00000000();
extern int gInputMgr_session_14C;

/* -O0 donor for trk_closeSession_0000065C — byte-identical sibling of
 * tim_closeSession_0000065C (same 21-insn -O0 cleanup wrapper at the same
 * 0x65C offset). Spliced into the -O2 main object via REPLACE_FUNC_BODY (real
 * -O0 compiler output, not instruction-forcing). The `gInputMgr_session_14C` extern form
 * gives the 2-insn symbol-store whose %lo field the reloc-blind expected/.o
 * bakes literal; see timproc_uso_b1_o0_65C.c for the full reloc-blind-donor
 * rationale and docs/MATCHING_WORKFLOW.md#feedback-replace-func-body-o0-donor. */
void trk_closeSession_0000065C(int *a0) {
    register int zero;
    CALL(Backup_closeSession)((int*)a0[0], 3);
    zero = 0;
    a0[0] = zero;
    gInputMgr_session_14C = zero;
}
