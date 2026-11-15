#include "common.h"
#include "game/input.h"
#include "gdl/backup.h"
#include "uso_call.h"

extern int func_00000000();
extern void stream_readS32_0000F1F0(int *dst);
extern void stream_readF32_0000F23C(int *dst);
extern void stream_readVec3_0000F2EC(int *dst);

/* drop the recordings: replayData[0] is the shared recordBuf, replayData[1] (player 2)
 * was allocated by replay_beginRecord2P and is freed. */
void replay_freeRecord(InputMgr *mgr) {
    mgr->replayData[0] = 0;
    if (mgr->replayData[1] != 0) {
        FCALL(mem_delete)(mgr->replayData[1]);
    }
    mgr->replayData[1] = 0;
}

/* save player 1's recording as the ghost: Backup_openSession(0, backup, 3, -1), copy
 * the frames into a fresh buffer, Backup_ghostSetHeader(ghost, mode, rec* bytes, data,
 * count), Backup_writeGhost(backup, ghost), Backup_closeSession(session, 3).
 * `mode` is the caller's BACKUP_MODE_* (the ghost header bits 0-1). */
void replay_saveGhost(InputMgr *mgr, int mode) {
    int v0;
    mgr->session = (struct BackupSession *)FCALL(Backup_openSession)(0, mgr->backup, 3, -1);
    v0 = FCALL(mem_new)(mgr->replayLen[0] << 2);
    FCALL(memcpy)(v0, mgr->replayData[0], mgr->replayLen[0] << 2);
    FCALL(Backup_ghostSetHeader)(
        mgr->session->ghost,
        mode,
        mgr->recCourse,
        mgr->recRider,
        mgr->recFlags,
        mgr->recBoard,
        v0,
        mgr->replayLen[0]
    );
    FCALL(Backup_writeGhost)(mgr->backup, mgr->session->ghost);
    FCALL(Backup_closeSession)(mgr->session, 3);
    mgr->unk184 = 0;
    mgr->demoIndex = -1;
}

void boot_readFloatParm_00012D64(char *dst) {
    int tmp;
    stream_readS32_0000F1F0(&tmp);
    stream_readF32_0000F23C((int*)(dst + 0x10));
}

void QuitProcess_stub_00012DA4(int a0) {}

void boot_readVec3Parm_00012DB8(char *dst) {
    int tmp;
    stream_readS32_0000F1F0(&tmp);
    stream_readVec3_0000F2EC((int*)(dst + 0x10));
}
