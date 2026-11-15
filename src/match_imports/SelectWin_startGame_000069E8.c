/* Native original-compiler source port from src/timproc_uso_b5/timproc_uso_b5.c. Target bytes unchanged. */
#include "context_timproc_uso_b5.h"
int SelectWin_startGame_000069E8(char *scr, int a1) {
    B5TimingPanels *C;
    int a4;
    B5TimingScreen *screen = (B5TimingScreen *)scr;
    import_000A53B4(scr, a1);
    gen_imp_Process_fadeOut_0000F954(*(char **)(scr + 0x418), -1, 0);
    *(int *)(timproc_uso_b5_69E8_global + 0x40) = *(int *)(timproc_uso_b5_69E8_global + 0x44);
    *screen->out[1] = screen->itemsMgr->lists[0]->items[screen->curItem[0]]->id;
    a4 = screen->cheatChar;
    if (a4 != 0 && a4 < 4) {
        timproc_uso_b5_69E8_print(&timproc_uso_b5_D_807FEB74 + 0x1304, a4);
        *screen->out[1] = ((int *)(&timproc_uso_b5_D_807FE794 + 0xF24))[screen->cheatChar] - 1;
    }
    if (*(int *)&import_80000310 == 0x17D7) {
        *screen->out[0] = screen->itemsMgr->lists[3]->items[screen->curItem[3]]->id;
    } else {
        *screen->out[0] = 1;
    }
    *screen->out[3] = screen->itemsMgr->lists[1]->items[screen->curItem[1]]->id;
    if (screen->cheatBoard == 4) {
        *screen->out[3] = 8;
    }
    C = screen->itemsMgr;
    *screen->out[2] = C->lists[0]->items[screen->curItem[0]]->flags | C->lists[1]->items[screen->curItem[1]]->flags;
    if (a1 != 0) {
        *screen->out[4] = screen->itemsMgr->lists[2]->items[screen->curItem[2]]->id;
    }
    if (*(int *)(timproc_uso_b5_69E8_global + 0x34) != 2) {
        ((int *)(&import_8002013C + 0xA4))[screen->curItem[0]] = *screen->out[3];
    } else {
        ((int *)(&import_80020150 + 0xB8))[screen->player] =
            (screen->itemsMgr->lists[0]->items[screen->curItem[0]]->flags & 0x20000) != 0;
    }
    return 1;
}
