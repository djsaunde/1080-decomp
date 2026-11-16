/* Native original-compiler source port from src/timproc_uso_b5/timproc_uso_b5.c. Target bytes unchanged. */
#include "context_timproc_uso_b5.h"
void SelectWin_init_00005BF0(B5TimingScreen *screen, int *arg1, int *arg2, int *arg3, int *arg4, int *arg5) {
    extern B5TimingEntry *timproc_uso_b5_func_00008A38_entry(B5TimingScreen *);
    extern s32 import_0024CCF4(s32, char *, s32, s32, s32, s32);
    B5TimingRow *row, *row2;
    B5TimingEntry *e, *e2, *e3;
    B5TimingVtable *vt;
    int *tbl;
    int i, idx, n, j;
    float divisor;
    f32 (*records)[4];
    screen->out[0] = arg1;
    screen->out[1] = arg2;
    screen->out[2] = arg3;
    screen->out[3] = arg4;
    screen->out[4] = arg5;
    /* RAW: data-table: per-character / per-board save tables indexed off timproc_uso_b5 Data symbols; p414 is an untyped SelectWin child */
    screen->cheatChar = *(int *)(&timproc_uso_b5_D_807FE6E4 + *arg2 * 4 + 0xE74);
    screen->chosenItem[0] = *(int *)(&timproc_uso_b5_D_807FE6E4_b + *arg2 * 4 + 0xE54); /* RAW: data-table (as above) */
    screen->cheatBoard = *(int *)(&timproc_uso_b5_D_807FE6C4 + *arg4 * 4 + 0xE94); /* RAW: data-table (as above) */
    screen->chosenItem[1] = SelectWin_findItem_00008988((char *)screen, 1, *arg4);
    row = screen->itemsMgr->lists[0];
    i = 0;
    if (row->count > 0) {
        tbl = (int *)(&import_80020150 + 0xB8);
        do {
            if (screen->numPlayers == 2) {
                if (tbl[screen->player] != 0) {
                    row->items[i]->flags = 0x20000;
                } else {
                    row->items[i]->flags = 0;
                }
            } else {
                row->items[i]->flags = 0;
            }
            i++;
            row = screen->itemsMgr->lists[0];
        } while (i < row->count);
    }
    row2 = screen->itemsMgr->lists[1];
    j = 0;
    if (row2->count > 0) {
        do {
            row2->items[j]->flags = *screen->out[2] & 0x20;
            j++;
            row2 = screen->itemsMgr->lists[1];
        } while (j < row2->count);
    }
    screen->chosenItem[2] = SelectWin_findItem_00008988((char *)screen, 2, *screen->out[4]);
    idx = SelectWin_findItem_00008988((char *)screen, 3, *screen->out[0]);
    screen->chosenItem[3] = idx;
    screen->curItem[3] = idx;
    n = screen->chosenItem[0];
    screen->curItem[0] = n;
    screen->curItem[1] = screen->chosenItem[1];
    screen->curItem[2] = screen->chosenItem[2];
    e = screen->itemsMgr->lists[0]->items[n];
    vt = e->vt;
    vt->setChosen((char *)(vt->setChosenDelta + (int)e), 0);
    e2 = screen->itemsMgr->lists[1]->items[screen->chosenItem[1]];
    vt = e2->vt;
    vt->setChosen((char *)(vt->setChosenDelta + (int)e2), 0);
    e3 = screen->itemsMgr->lists[2]->items[screen->chosenItem[2]];
    vt = e3->vt;
    vt->setChosen((char *)(vt->setChosenDelta + (int)e3), 0);
    records = screen->colors;
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
    screen->font = import_0024CCF4(0, &import_80074E78, 0x48, 0xDD, 3, 0xD);
    e = timproc_uso_b5_func_00008A38_entry(screen);
    vt = e->vt;
    vt->focus((char *)(vt->focusDelta + (int)e));
    {
        char *p414;
        e = timproc_uso_b5_func_00008A38_entry(screen);
        p414 = screen->scene;
        SelectWin_setCamToItem_00003A28((int *)e, (int *)*(int *)(p414 + 0x10), *(int *)(p414 + 0x14)); /* RAW: data-table (as above) */
    }
    SelectWin_beginTransition_00008688((char *)screen, 0);
    screen->idleTimer = screen->lfw;
    screen->transCountdownInit = 0x40;
    screen->transition = 1.0f;
}
