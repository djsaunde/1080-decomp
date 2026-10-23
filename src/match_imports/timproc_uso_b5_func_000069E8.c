/* Native original-compiler source port from src/timproc_uso_b5/timproc_uso_b5.c. Target bytes unchanged. */
#include "context_timproc_uso_b5.h"
int timproc_uso_b5_func_000069E8(char *scr, int a1) {
    B5TimingPanels *C;
    int a4;
    B5TimingScreen *screen = (B5TimingScreen *)scr;
    import_000A53B4(scr, a1);
    timproc_uso_b5_func_00F954(*(char **)(scr + 0x418), -1, 0);
    *(int *)(timproc_uso_b5_69E8_global + 0x40) = *(int *)(timproc_uso_b5_69E8_global + 0x44);
    *screen->field_4DC[1] = screen->field_40C->field_40[0]->field_3C[screen->field_3D0[0]]->field_2B0;
    a4 = screen->field_4A4;
    if (a4 != 0 && a4 < 4) {
        timproc_uso_b5_69E8_print(&timproc_uso_b5_D_807FEB74 + 0x1304, a4);
        *screen->field_4DC[1] = ((int *)(&timproc_uso_b5_D_807FE794 + 0xF24))[screen->field_4A4] - 1;
    }
    if (*(int *)&import_80000310 == 0x17D7) {
        *screen->field_4DC[0] = screen->field_40C->field_40[3]->field_3C[screen->field_3D0[3]]->field_2B0;
    } else {
        *screen->field_4DC[0] = 1;
    }
    *screen->field_4DC[3] = screen->field_40C->field_40[1]->field_3C[screen->field_3D0[1]]->field_2B0;
    if (screen->field_4A8 == 4) {
        *screen->field_4DC[3] = 8;
    }
    C = screen->field_40C;
    *screen->field_4DC[2] = C->field_40[0]->field_3C[screen->field_3D0[0]]->field_2B4 | C->field_40[1]->field_3C[screen->field_3D0[1]]->field_2B4;
    if (a1 != 0) {
        *screen->field_4DC[4] = screen->field_40C->field_40[2]->field_3C[screen->field_3D0[2]]->field_2B0;
    }
    if (*(int *)(timproc_uso_b5_69E8_global + 0x34) != 2) {
        ((int *)(&import_8002013C + 0xA4))[screen->field_3D0[0]] = *screen->field_4DC[3];
    } else {
        ((int *)(&import_80020150 + 0xB8))[screen->field_3B8] =
            (screen->field_40C->field_40[0]->field_3C[screen->field_3D0[0]]->field_2B4 & 0x20000) != 0;
    }
    return 1;
}
