/* Native original-compiler source port from src/timproc_uso_b5/timproc_uso_b5.c. Target bytes unchanged. */
#include "context_timproc_uso_b5.h"
void timproc_uso_b5_func_0000BBDC(TimB5EntryFields *a0, B5TimingScreen *a1) {
    volatile int pad0;
    int caps[5];
    int *idxp[5];
    volatile int pad2[5];
    int *cp;
    char *tbl;
    char *p230;
    int i;
    int v;
    int v1;
    if (a0->field_2BC < 10) {
        if (--(a0->field_2C0) < 0) {
            a0->field_2C0 = 1;
            a0->field_2BC = a0->field_2BC + 2;
        }
    }
    caps[0] = a0->field_1C4;
    caps[1] = a0->field_1DC;
    caps[2] = a0->field_1F4;
    caps[3] = a0->field_20C;
    caps[4] = a0->field_224;
    idxp[0] = (int *)a1->field_134;
    idxp[1] = (int *)a1->field_158;
    idxp[2] = (int *)a1->field_17C;
    idxp[3] = (int *)a1->field_1A0;
    idxp[4] = (int *)a1->field_1C4;
    p230 = a1->field_230;
    tbl = (char *)&timproc_uso_b5_D_807FF4C0 + 0x40;
    i = 0;
    while (0) { i += 1; }
    cp = caps;
    do {
        v = a0->field_2BC;
        v1 = (v < *cp) ? v : *cp;
        v = v1;
        if (a1->field_3BC == 1) {
            timproc_uso_b5_func_00003A4C(
                (int)a0,
                a0->field_A4,
                i * a1->field_74 + *(int *)((char *)a0 + 0xBC),
                (int)tbl,
                v1,
                (int)idxp[i],
                (int)p230,
                (char *)a1);
        } else {
            v1 = 0;
            timproc_uso_b5_func_00003C8C(
                (int)a0,
                a0->field_A4,
                i * a1->field_74 + *(int *)((char *)a0 + 0xBC),
                (char *)tbl,
                v,
                (int)idxp[i],
                (int)p230,
                (char *)a1);
        }
        cp++;
        tbl += 0x18;
        i++;
    } while (i < 5);
}
