/* Native original-compiler source port from src/timproc_uso_b5/timproc_uso_b5.c. Target bytes unchanged. */
#include "context_timproc_uso_b5.h"
void timproc_uso_b5_func_0000C98C(C98CObject *arg0, C98CContext *arg1, volatile int arg2, int arg3) {
    float ones[4];
    int a2v;
    int sp30;
    int v0r, v1;
    ones[0] = 1.0f;
    ones[1] = 1.0f;
    ones[2] = 1.0f;
    ones[3] = 1.0f;
    a2v = (arg1->link->drawable->field_C4 + arg1->link->drawable->field_BC) - arg0->field_5C;
    timproc_uso_b5_func_000040BC((int)arg0, arg0->field_44, a2v, arg2, arg3,
        arg0->field_1AC * 24 + (int)(&timproc_uso_b5_D_807FF640 + 0x1C0), 0xFF);
    if (*(int *)&import_8002022C != 0) {
        timproc_uso_b5_func_00003FF8((int)arg0, arg0->field_44 + arg0->field_74,
            a2v + arg0->field_8C,
            arg0->field_1AC * 24 + (int)(&timproc_uso_b5_D_807FF700 + 0x280), 0xFF);
    }
    sp30 = 0;
    v0r = timproc_uso_b5_func_01EEE4(*(char **)(&import_80020098 + 0x148) + 4);
    v1 = *(int *)(&import_80020098 + 0x34);
    if (v1 == 6) {
        if (v0r == 2) {
            if (timproc_uso_b5_func_01EFC8(*(char **)(&import_80020098 + 0x148) + 4) == arg0->field_2B0) {
                sp30 = 1;
            }
        }
    } else if (v1 == 4) {
        if (v0r == 1) {
            if (timproc_uso_b5_func_01EFC8(*(char **)(&import_80020098 + 0x148) + 4) == arg0->field_2B0) {
                sp30 = 1;
            }
        }
    }
    if (sp30 != 0) {
        timproc_uso_b5_func_00003FF8((int)arg0, arg0->field_44 + 0x54, a2v - 0x78,
            (int)(&timproc_uso_b5_D_807FFB50 + 0x6D0), 0xFF);
    }
}
