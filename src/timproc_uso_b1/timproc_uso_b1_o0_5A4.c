#include "common.h"
#include "gdl/backup.h"
#include "uso_call.h"

extern int gl_func_00000000();
extern int gInputMgr_backup_148;
extern int gInputMgr_session_14C;
/* UNNAMED: data-base D_00000068 = 0x68 is RoData +0x68 in bootup snd_soundObjSetPan_00000B2C but InputMgr +0x68 (import mod0) in the arcproc / timproc b1 / b3 openSession helpers; a pure rename would merge them, it needs a per-site split. */
extern int D_00000068;

void tim_openSession_000005A4(int **arg0, int arg1, int arg2) {
    int handle = CALL(app_setHeapMode)(2);
    register int new_obj = CALL(Backup_openSession)(0, gInputMgr_backup_148, 1, arg2);
    *arg0 = (int*)new_obj;
    gInputMgr_session_14C = new_obj;
    CALL(app_setHeapMode)(handle);
    if (arg1 != 0) {
        ((BackupSession *)*arg0)->modes = 1;
    } else {
        ((BackupSession *)*arg0)->modes = 0;
    }
    D_00000068 = 0;
}
