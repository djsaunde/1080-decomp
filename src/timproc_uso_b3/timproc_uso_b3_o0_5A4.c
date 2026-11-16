#include "common.h"
#include "gdl/backup.h"
#include "uso_call.h"

extern int gl_func_00000000();
extern int gInputMgr_backup_148;
extern int gInputMgr_session_14C;
/* UNNAMED: data-base D_00000068 = 0x68 is RoData +0x68 in bootup snd_soundObjSetPan_00000B2C but InputMgr +0x68 (import mod0) in the arcproc / timproc b1 / b3 openSession helpers; a pure rename would merge them, it needs a per-site split. */
extern int D_00000068;

/* -O0 donor for trk_openSession_000005A4 — masked-shape TWIN of the landed
 * tim_openSession_000005A4 (46-insn -O0 allocator-wrapper at the same 0x5A4
 * offset). The two target functions are instruction-for-instruction identical
 * except for two immediate constants (b1 uses 0x1, b3 uses 0x2). Spliced into
 * the -O2 main object via REPLACE_FUNC_BODY (real -O0 compiler output, not
 * instruction-forcing); the splice imports the donor HI16/LO16/26 relocs.
 * See timproc_uso_b1_o0_5A4.c + feedback_splice_import_donor_relocs_recipe. */
void trk_openSession_000005A4(int **arg0, int arg1, int arg2) {
    int handle = CALL(app_setHeapMode)(2);
    register int new_obj = CALL(Backup_openSession)(0, gInputMgr_backup_148, 2, arg2);
    *arg0 = (int*)new_obj;
    gInputMgr_session_14C = new_obj;
    CALL(app_setHeapMode)(handle);
    if (arg1 != 0) {
        ((BackupSession *)*arg0)->modes = 2;
    } else {
        ((BackupSession *)*arg0)->modes = 0;
    }
    D_00000068 = 0;
}
