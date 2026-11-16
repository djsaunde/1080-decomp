/* tit_procMain (0x590 / 356 words) -- 12-case jumptable state
 * machine. EXACT under IDO 7.1 cfe -O2 (agent-g run 178). Spliced over the
 * unit's INCLUDE_ASM by REPLACE_FUNC_BODY (Makefile TITPROC_5DC_DONOR) so the
 * switch's donor-local .rodata jumptable reloc is renamed
 * tit_procMain_rodata and pinned to the table the USO ships at
 * RoData +0x0. Decode history: see the note above the placeholder in
 * titproc_uso.c. */
#include "common.h"
#include "game/input.h" /* d = the bootup input manager import */
#include "game/modeproc.h" /* a0 = the titproc flow object (ModeFlowProc: +0 the record session) */
#include "uso_call.h"
/* States 7 / 9 (vs ghost): copy the saved ghost's rider / board flag /
 * board into InputMgr ghostRider / ghostFlags / ghostBoard for game_setupVsGhost and
 * take its course; `FLOWPROC(a0)->session->ghost` is the open BackupSession's ghost. */
extern char import_00020098;
extern int titproc_uso_func_0120A8();
extern int titproc_uso_func_0027E8();
extern int titproc_uso_func_000C54();
extern int titproc_uso_func_01DA48();
extern int titproc_uso_func_000B6C();
extern int titproc_uso_func_000C0C();
extern int titproc_uso_func_000418();
extern int titproc_uso_func_011CD8();
extern int titproc_uso_func_001E9C();
extern int tit_imp_Backup_ghostGetRider();
extern int tit_imp_Backup_ghostGetBoardFlag();
extern int tit_imp_Backup_ghostGetBoard();
extern int tit_imp_Backup_ghostGetCourse();
extern int import_000B3268();


void tit_procMain(char *a0, int a1) {
    int d;
    int done;
    int s5v;
    int *r;
    int t;
    int mask;
    d = (int)&import_00020098;
    done = 0;
    do {
        switch (a1) {
        case 0: {
            XCALL(titproc_uso_func_0120A8, snd_playSeq_000120A8)(d, 0, 0, 0);
            done = 1;
            ((InputMgr *)d)->level = 0;
            *(int *)(d + 0x8C) = 0x820000;
            r = (int *)XCALL(titproc_uso_func_0027E8, game_init)(d, 2, 9, 1);
            t = (int)r;
            XCALL(titproc_uso_func_07ACE0, list_append)((char *)&import_00020098 + 0x10, r); if (a0) {}
            if (NODE(t)->parent != 0) {
                NODE(t)->unk04 = 1;
            }
            NODE(t)->parent = (Node *)d;
            XCALL(titproc_uso_func_01DA48, app_runProcess)(a0, 0, XCALL(titproc_uso_func_000C54, TitleDemoProcess_ctor)(0, r));
            break;
        }
        case 1:
            XCALL(titproc_uso_func_000B6C, tit_openSession_00000B6C)(a0, -1);
            done = 1;
            import_000B3268(a0, 0x9FFF0, 0x10000, FLOWPROC(a0)->session);
            ((InputMgr *)d)->nextProcState = 2;
            break;
        case 2:
            XCALL(titproc_uso_func_000C0C, tit_closeSession_00000C0C)(a0);
            ((InputMgr *)d)->procState = 0;
            break;
        case 3: {
            ((InputMgr *)d)->procState = ((InputMgr *)d)->nextProcState, ((InputMgr *)d)->nextProcState = 0;
            s5v = 0x800000;
            break;
        }
        case 4: {
            mask = ((s5v | 0x10000) | 0x80000) | 0x898;
            t = XCALL(titproc_uso_func_000418, tit_randomUnlocked_00000418)();
            done = 1;
            XCALL(titproc_uso_func_0120A8, snd_playSeq_000120A8)(d, 4, t, 0);
            r = (int *)XCALL(titproc_uso_func_0027E8, game_init)(d, t, 10, 1);
            t = (int)r;
            XCALL(titproc_uso_func_07ACE0, list_append)((char *)&import_00020098 + 0x10, r);
            if (NODE(t)->parent != 0) {
                NODE(t)->unk04 = 1;
            }
            NODE(t)->parent = (Node *)d;
            XCALL(titproc_uso_func_011CD8, input_setRacePadsActive)(d, 1);
            XCALL(titproc_uso_func_01DA48, app_runProcess)(a0, 0, XCALL(titproc_uso_func_001E9C, GameDemoProcess_ctor)(0, mask, r, 0));
            ((InputMgr *)d)->procState = 0;
            break;
        }
        case 5: {
            mask = ((s5v | 0x1010000) | 0x80000) | 0x898;
            t = XCALL(titproc_uso_func_000418, tit_randomUnlocked_00000418)();
            done = 1;
            XCALL(titproc_uso_func_0120A8, snd_playSeq_000120A8)(d, 4, t, 0);
            r = (int *)XCALL(titproc_uso_func_0027E8, game_init)(d, t, 11, 2);
            t = (int)r;
            XCALL(titproc_uso_func_07ACE0, list_append)((char *)&import_00020098 + 0x10, r);
            if (NODE(t)->parent != 0) {
                NODE(t)->unk04 = 1;
            }
            NODE(t)->parent = (Node *)d;
            XCALL(titproc_uso_func_011CD8, input_setRacePadsActive)(d, 1);
            XCALL(titproc_uso_func_01DA48, app_runProcess)(a0, 0, XCALL(titproc_uso_func_001E9C, GameDemoProcess_ctor)(0, mask, r, 0));
            ((InputMgr *)d)->procState = 0;
            break;
        }
        case 6:
            XCALL(titproc_uso_func_000C0C, tit_closeSession_00000C0C)(a0);
            s5v = 0x2100000;
            ((InputMgr *)d)->procState = 10;
            break;
        case 7: {
            t = s5v & 0x2000000;
            mask = (s5v | 0x8020000) | 0x80000;
            XCALL(titproc_uso_func_000B6C, tit_openSession_00000B6C)(a0, 2);
            done = 1;
            ((InputMgr *)d)->ghostRider = tit_imp_Backup_ghostGetRider(FLOWPROC(a0)->session->ghost);
            ((InputMgr *)d)->ghostFlags = tit_imp_Backup_ghostGetBoardFlag(FLOWPROC(a0)->session->ghost);
            ((InputMgr *)d)->ghostBoard = tit_imp_Backup_ghostGetBoard(FLOWPROC(a0)->session->ghost);
            if (t != 0) {
                XCALL(titproc_uso_func_0120A8, snd_playSeq_000120A8)(d, 10, tit_imp_Backup_ghostGetCourse(FLOWPROC(a0)->session->ghost), 2);
            } else {
                XCALL(titproc_uso_func_0120A8, snd_playSeq_000120A8)(d, 4, tit_imp_Backup_ghostGetCourse(FLOWPROC(a0)->session->ghost), 0);
            }
            ((InputMgr *)d)->nextFlowMode = 6;
            t = tit_imp_Backup_ghostGetCourse(FLOWPROC(a0)->session->ghost);
            ((InputMgr *)d)->course = t;
            r = (int *)XCALL(titproc_uso_func_0027E8, game_init)(d, t, 4, 1);
            ((InputMgr *)d)->nextFlowMode = 1;
            t = (int)r;
            XCALL(titproc_uso_func_07ACE0, list_append)((char *)&import_00020098 + 0x10, r);
            if (NODE(t)->parent != 0) {
                NODE(t)->unk04 = 1;
            }
            NODE(t)->parent = (Node *)d;
            XCALL(titproc_uso_func_011CD8, input_setRacePadsActive)(d, 1);
            t = (int)FLOWPROC(a0)->session;
            XCALL(titproc_uso_func_01DA48, app_runProcess)(a0, 0, XCALL(titproc_uso_func_001E9C, GameDemoProcess_ctor)(0, (int)&((char (*)[2])mask)[0][((BackupSession *)t)->ghost->count], r, t));
            ((InputMgr *)d)->procState = 11;
            break;
        }
        case 8:
            XCALL(titproc_uso_func_000C0C, tit_closeSession_00000C0C)(a0);
            s5v = 0x2100000;
            ((InputMgr *)d)->procState = 9;
            break;
        case 9: {
            t = s5v & 0x2000000;
            mask = (s5v | 0x8020000) | 0x80000;
            XCALL(titproc_uso_func_000B6C, tit_openSession_00000B6C)(a0, 1);
            done = 1;
            ((InputMgr *)d)->ghostRider = tit_imp_Backup_ghostGetRider(FLOWPROC(a0)->session->ghost);
            ((InputMgr *)d)->ghostFlags = tit_imp_Backup_ghostGetBoardFlag(FLOWPROC(a0)->session->ghost);
            ((InputMgr *)d)->ghostBoard = tit_imp_Backup_ghostGetBoard(FLOWPROC(a0)->session->ghost);
            if (t != 0) {
                XCALL(titproc_uso_func_0120A8, snd_playSeq_000120A8)(d, 10, tit_imp_Backup_ghostGetCourse(FLOWPROC(a0)->session->ghost), 1);
            } else {
                XCALL(titproc_uso_func_0120A8, snd_playSeq_000120A8)(d, 4, tit_imp_Backup_ghostGetCourse(FLOWPROC(a0)->session->ghost), 0);
            }
            t = tit_imp_Backup_ghostGetCourse(FLOWPROC(a0)->session->ghost);
            ((InputMgr *)d)->course = t;
            r = (int *)XCALL(titproc_uso_func_0027E8, game_init)(d, t, 4, 1);
            t = (int)r;
            XCALL(titproc_uso_func_07ACE0, list_append)((char *)&import_00020098 + 0x10, r);
            if (NODE(t)->parent != 0) {
                NODE(t)->unk04 = 1;
            }
            NODE(t)->parent = (Node *)d;
            XCALL(titproc_uso_func_011CD8, input_setRacePadsActive)(d, 1);
            t = (int)FLOWPROC(a0)->session;
            XCALL(titproc_uso_func_01DA48, app_runProcess)(a0, 0, XCALL(titproc_uso_func_001E9C, GameDemoProcess_ctor)(0, (int)&((char (*)[2])mask)[0][((BackupSession *)t)->ghost->count], r, t));
            ((InputMgr *)d)->procState = 11;
            break;
        }
        case 10:
            break;
        case 11:
            XCALL(titproc_uso_func_000C0C, tit_closeSession_00000C0C)(a0);
            ((InputMgr *)d)->procState = ((InputMgr *)d)->nextProcState;
            break;
        }
        a1 = ((InputMgr *)d)->procState;
    } while (done == 0);
    /* t is uninitialised on the case-1 exit path. Allowed only because the target
     * itself relies on an uninitialised local in this same function: its prologue
     * loads s5v from its never-written home (`lw s5,0x5C(sp)`, 0x8FB5005C at
     * 0x620; s5v is only assigned in cases 3/6/8 and read in 4/5/7/9). This empty
     * test keeps t live around the loop so the address constant outranks it
     * (d = s0, t = s1 as in the target). */
    if (t) {}
}
