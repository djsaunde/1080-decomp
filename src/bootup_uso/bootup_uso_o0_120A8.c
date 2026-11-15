#include "common.h"
#include "game/input.h"
#include "gdl/backup.h"

/* Run 8 of bootup_uso's -O0 region (0x120A8..0x122C3). 5 functions.
 * See project_1080_bootup_uso_o0_runs.md + feedback_ido_o0_empty_stub.md.
 * input_initDefaults_00012188 (0x12188, 0xBC) moved here from bootup_uso_tail3b_top.c:
 * it is -O0 codegen (fresh addiu rN,zero,1 per `=1` store, stack-stored
 * loop counter `sw zero,4(sp)`, unfilled-delay epilogue). `register int v`
 * reuses $a1 across the value-pair stores; plain `for` keeps `i` on stack.
 * settings_setBit_00012244 (0x12244, 0x80) also moved here from tail3b_top: -O0 void
 * set-bit-and-call helper. Match needs the body's RMW pointer in $s0 — a
 * `register unsigned short *q` forces that (the condition's deref stays an
 * inline fresh temp; a shared/stack `p` regresses). Byte-exact at -O0;
 * trailing align nop dropped by TRUNCATE 0x21C. */

extern int func_00000000();
extern int gInputMgr_00000150;

void snd_playSeq_000120A8(char *a0, int a1, int a2, int a3) {
    GAMEFLOW(a0)->seq = a1;
    GAMEFLOW(a0)->seqArg = a2;
    func_00000000(GAMEFLOW(a0)->seq, GAMEFLOW(a0)->seqArg, 0);
    GAMEFLOW(a0)->seqArg = 0;
}

void GameFlow_releaseFiles_00012110(int a0) {
    func_00000000(gInputMgr_00000150);
    func_00000000(7);
}

void settings_save_00012150(char *a0) {
    func_00000000(GAMEFLOW(a0)->backup, GAMEFLOW(a0)->options);
}

void input_initDefaults_00012188(char *a0) {
    int i;
    register int v;
    GAMEFLOW(a0)->level = 0;
    GAMEFLOW(a0)->playerRider = 1;
    GAMEFLOW(a0)->playerFlags = 0;
    GAMEFLOW(a0)->playerBoard = 1;
    GAMEFLOW(a0)->rider2 = 1;
    GAMEFLOW(a0)->flags2 = 0;
    GAMEFLOW(a0)->board2 = 1;
    GAMEFLOW(a0)->course = 2;
    GAMEFLOW(a0)->unk80 = 0;
    for (i = 0; i < 3; i++) { *(a0 + i + 0x158) = 0x2A; }
    GAMEFLOW(a0)->unk15C = 0;
    v = 4; GAMEFLOW(a0)->order90[0] = v; GAMEFLOW(a0)->orderA4[4] = v;
    v = 3; GAMEFLOW(a0)->order90[1] = v; GAMEFLOW(a0)->orderA4[3] = v;
    v = 1; GAMEFLOW(a0)->order90[2] = v; GAMEFLOW(a0)->orderA4[1] = v;
    v = 2; GAMEFLOW(a0)->order90[3] = v; GAMEFLOW(a0)->orderA4[2] = v;
    v = 5; GAMEFLOW(a0)->order90[4] = v; GAMEFLOW(a0)->orderA4[0] = v;
}

void settings_setBit_00012244(int *a0, int a1) {
    if ((SETTINGS(GAMEFLOW(a0)->options)->unlocks & (1 << a1)) == 0) {
        register unsigned short *q = &SETTINGS(GAMEFLOW(a0)->options)->unlocks;
        *q = *q | (1 << a1);
        func_00000000(a0);
    }
}
