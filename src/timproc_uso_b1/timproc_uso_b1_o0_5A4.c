#include "common.h"
#include "gdl/backup.h"
#include "uso_call.h"

extern int gl_func_00000000();
extern int D_00000148;
extern int D_0000014C;
extern int D_00000068;

void tim_openSession_000005A4(int **arg0, int arg1, int arg2) {
    int handle = CALL(app_setHeapMode)(2);
    register int new_obj = CALL(Backup_openSession)(0, D_00000148, 1, arg2);
    *arg0 = (int*)new_obj;
    D_0000014C = new_obj;
    CALL(app_setHeapMode)(handle);
    if (arg1 != 0) {
        ((BackupSession *)*arg0)->modes = 1;
    } else {
        ((BackupSession *)*arg0)->modes = 0;
    }
    D_00000068 = 0;
}
