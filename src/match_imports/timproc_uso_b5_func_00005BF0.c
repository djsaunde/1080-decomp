/* Native original-compiler source port from src/timproc_uso_b5/timproc_uso_b5.c. Target bytes unchanged. */
#include "context_timproc_uso_b5.h"
void timproc_uso_b5_func_00005BF0(B5TimingScreen *screen, int *arg1, int *arg2, int *arg3, int *arg4, int *arg5) {
    extern B5TimingEntry *timproc_uso_b5_func_00008A38_entry(B5TimingScreen *);
    extern s32 import_0024CCF4(s32, char *, s32, s32, s32, s32);
    B5TimingRow *row, *row2;
    B5TimingEntry *e, *e2, *e3;
    B5TimingVtable *vt;
    int *tbl;
    int i, idx, n, j;
    float divisor;
    f32 (*records)[4];
    screen->field_4DC[0] = arg1;
    screen->field_4DC[1] = arg2;
    screen->field_4DC[2] = arg3;
    screen->field_4DC[3] = arg4;
    screen->field_4DC[4] = arg5;
    screen->field_4A4 = *(int *)(&timproc_uso_b5_D_807FE6E4 + *arg2 * 4 + 0xE74);
    screen->field_3E0[0] = *(int *)(&timproc_uso_b5_D_807FE6E4_b + *arg2 * 4 + 0xE54);
    screen->field_4A8 = *(int *)(&timproc_uso_b5_D_807FE6C4 + *arg4 * 4 + 0xE94);
    screen->field_3E0[1] = timproc_uso_b5_func_00008988((char *)screen, 1, *arg4);
    row = screen->field_40C->field_40[0];
    i = 0;
    if (row->field_6C > 0) {
        tbl = (int *)(&import_80020150 + 0xB8);
        do {
            if (screen->field_3BC == 2) {
                if (tbl[screen->field_3B8] != 0) {
                    row->field_3C[i]->field_2B4 = 0x20000;
                } else {
                    row->field_3C[i]->field_2B4 = 0;
                }
            } else {
                row->field_3C[i]->field_2B4 = 0;
            }
            i++;
            row = screen->field_40C->field_40[0];
        } while (i < row->field_6C);
    }
    row2 = screen->field_40C->field_40[1];
    j = 0;
    if (row2->field_6C > 0) {
        do {
            row2->field_3C[j]->field_2B4 = *screen->field_4DC[2] & 0x20;
            j++;
            row2 = screen->field_40C->field_40[1];
        } while (j < row2->field_6C);
    }
    screen->field_3E0[2] = timproc_uso_b5_func_00008988((char *)screen, 2, *screen->field_4DC[4]);
    idx = timproc_uso_b5_func_00008988((char *)screen, 3, *screen->field_4DC[0]);
    screen->field_3E0[3] = idx;
    screen->field_3D0[3] = idx;
    n = screen->field_3E0[0];
    screen->field_3D0[0] = n;
    screen->field_3D0[1] = screen->field_3E0[1];
    screen->field_3D0[2] = screen->field_3E0[2];
    e = screen->field_40C->field_40[0]->field_3C[n];
    vt = e->field_28;
    vt->field_84((char *)(vt->field_80 + (int)e), 0);
    e2 = screen->field_40C->field_40[1]->field_3C[screen->field_3E0[1]];
    vt = e2->field_28;
    vt->field_84((char *)(vt->field_80 + (int)e2), 0);
    e3 = screen->field_40C->field_40[2]->field_3C[screen->field_3E0[2]];
    vt = e3->field_28;
    vt->field_84((char *)(vt->field_80 + (int)e3), 0);
    records = screen->field_420;
    divisor = 255.0f;
    records[0][0] = 90.0f / divisor;
    records[0][1] = 90.0f / divisor;
    records[0][2] = 90.0f / divisor;
    records[0][3] = (float)255 / divisor;
    records[1][0] = (float)255 / divisor;
    records[1][1] = 145.0f / divisor;
    records[1][2] = 58.0f / divisor;
    records[1][3] = (float)255 / divisor;
    records[2][0] = (float)255 / divisor;
    records[2][1] = (float)255 / divisor;
    records[2][2] = (float)255 / divisor;
    records[2][3] = (float)255 / divisor;
    records[3][0] = 13.0f / divisor;
    records[3][1] = 13.0f / divisor;
    records[3][2] = 13.0f / divisor;
    records[3][3] = (float)255 / divisor;
    records[4][0] = 161.0f / divisor;
    records[4][1] = 91.0f / divisor;
    records[4][2] = 35.0f / divisor;
    records[4][3] = (float)255 / divisor;
    records[5][0] = 70.0f / divisor;
    records[5][1] = 70.0f / divisor;
    records[5][2] = 70.0f / divisor;
    records[5][3] = (float)255 / divisor;
    screen->field_4D0 = import_0024CCF4(0, &import_80074E78, 0x48, 0xDD, 3, 0xD);
    e = timproc_uso_b5_func_00008A38_entry(screen);
    vt = e->field_28;
    vt->field_64((char *)(vt->field_60 + (int)e));
    {
        char *p414;
        e = timproc_uso_b5_func_00008A38_entry(screen);
        p414 = screen->field_414;
        timproc_uso_b5_func_00003A28((int *)e, (int *)*(int *)(p414 + 0x10), *(int *)(p414 + 0x14));
    }
    timproc_uso_b5_func_00008688((char *)screen, 0);
    screen->field_488 = screen->field_104;
    screen->field_404 = 0x40;
    screen->field_4BC = 1.0f;
}
