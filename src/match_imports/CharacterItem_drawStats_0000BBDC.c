/* Native original-compiler source port from src/timproc_uso_b5/timproc_uso_b5.c. Target bytes unchanged. */
#include "context_timproc_uso_b5.h"
void CharacterItem_drawStats_0000BBDC(TimB5EntryFields *a0, B5TimingScreen *a1) {
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
    if (a0->barCount < 10) {
        if (--(a0->barTimer) < 0) {
            a0->barTimer = 1;
            a0->barCount = a0->barCount + 2;
        }
    }
    caps[0] = a0->am0;
    caps[1] = a0->am1;
    caps[2] = a0->am2;
    caps[3] = a0->am3;
    caps[4] = a0->am4;
    idxp[0] = (int *)a1->cc0;
    idxp[1] = (int *)a1->cc1;
    idxp[2] = (int *)a1->cc2;
    idxp[3] = (int *)a1->cc3;
    idxp[4] = (int *)a1->cc4;
    p230 = a1->cc7;
    tbl = (char *)&timproc_uso_b5_D_807FF4C0 + 0x40;
    i = 0;
    while (0) { i += 1; }
    cp = caps;
    do {
        v = a0->barCount;
        v1 = (v < *cp) ? v : *cp;
        v = v1;
        if (a1->numPlayers == 1) {
            SelItem_drawStatBar_00003A4C(
                (char *)a0,
                a0->xp0,
                /* *(int *)&a0->yp0: the plain member read a0->yp0 re-colours the loop temps (run 592) */
                i * a1->ysp + *(int *)&a0->yp0,
                tbl,
                v1,
                (f32 *)idxp[i],
                (f32 *)p230,
                (char *)a1);
        } else {
            v1 = 0;
            SelItem_drawStatBarRight_00003C8C(
                (char *)a0,
                a0->xp0,
                i * a1->ysp + *(int *)&a0->yp0, /* (as above) */
                tbl,
                v,
                (f32 *)idxp[i],
                (f32 *)p230,
                (char *)a1);
        }
        cp++;
        tbl += 0x18;
        i++;
    } while (i < 5);
}
