#include "common.h"
#include "gdl/gl_data.h"
#include "uso_call.h"

extern int func_00000000();
extern char D_00000000;

/* -O0 split of Process_resume_0000FAE8 from bootup_uso_tail1.c (target is -O0; the
 * bit-manip intermediates land in callee-saved s0-s3 via `register`). */
void Process_resume_0000FAE8(int *self) {
    register int *q, *r;
    register int flag;
    register int *x;

    FCALL(snd_unmuteAll)();
    if (self[0x38 / 4] == 0) {
        FCALL(input_unpauseRacePads)(&gInputMgr);
    }
    FCALL(Process_stop)(self[0x48 / 4]);
    self[0x30 / 4] = 0;
    self[0x34 / 4] = 0;
    if (self[0x4C / 4] != 0) {
        q = (int*)((char*)self[0x4C / 4] + 0x18);
        r = q;
        flag = 1;
        if (flag) {
            x = r;
            *x |= 8;
        } else {
            x = r;
            *x &= ~8;
        }
    }
}
