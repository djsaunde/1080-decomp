#ifndef GAME_INPUT_H
#define GAME_INPUT_H

/*
 * The input manager: bootup.uso's exported Bss object at Bss 0xF8 (agent-h run 466).
 *
 * Every mode module (titproc, n64proc, h2hproc, mgrproc, arcproc, timproc, trkproc) imports
 * it as one symbol and passes it as arg 0 to the bootup input_* functions
 * (input_checkPressed_00011E00 / input_checkHeld_00011ED4 / input_checkReleased_00011FA8,
 * input_setActivePad_0001207C, input_setActive_00011CD8). Besides the pad list it holds the
 * game-flow words the mode state machines run on, so the mode modules read and write
 * those fields through the same import (relocations against bootup Bss 0xF8 + offset).
 *
 * Only the fields with evidence are named; the rest is padding. The size is not known
 * (the object reaches at least +0x194, past the Fader pointer).
 *
 * Mode modules model the import as `extern InputMgr D_00000000;` (or cast the import
 * address), which keeps the relocated symbol and offset unchanged (READABILITY_PLAN 9, rule 4e).
 */

/* one controller as the input manager sees it (the objects in InputMgr.pads[]) */
typedef struct InputPad {
    char pad00[0x10];
    /* 0x10 */ s32 held;      /* button mask tested by input_checkHeld_00011ED4 */
    char pad14[0x18 - 0x14];
    /* 0x18 */ s32 pressed;   /* tested by input_checkPressed_00011E00 */
    /* 0x1C */ s32 released;  /* tested by input_checkReleased_00011FA8 */
    char pad20[0x4C - 0x20];
    /* Input recording / playback (pad_update, game_libs 0x675A4; pad_startRecord /
     * pad_startPlay). One InputFrame per frame. */
    /* 0x4C */ s32 replayMode;   /* PAD_REPLAY_OFF / PLAY / RECORD; drops to OFF at replayLen */
    /* 0x50 */ s32 replayFrame;  /* frame index, counted up by pad_update */
    /* 0x54 */ s32 replayLen;    /* frames to play, or the record capacity */
    /* 0x58 */ s32 *replayLenOut; /* RECORD: receives the frame count each frame */
    /* 0x5C */ struct InputFrame *replayFrames;
    char pad60[0x6C - 0x60];
    /* 0x6C */ s32 recordMask;   /* ANDed with held when recording; the replay_begin* functions
                                  * set 0xFFFBFFFF */
    /* 0x70 */ s32 mirrorX;      /* nonzero: pad_update negates the stick x byte */
} InputPad;

/* One recorded frame (4 bytes): pad_update RECORD stores the raw stick bytes and
 * (recordMask & held & 0x1700); PLAY loads the stick bytes back and ORs the buttons into held. */
typedef struct InputFrame {
    /* 0x0 */ signed char stickX;   /* plain C types: the NCC .cpp users have no s8 / s16 */
    /* 0x1 */ signed char stickY;
    /* 0x2 */ short buttons;
} InputFrame;

#define PAD_REPLAY_OFF      0
#define PAD_REPLAY_PLAY     1   /* pad_startPlay */
#define PAD_REPLAY_RECORD   2   /* pad_startRecord */

typedef struct InputMgr {
    char pad00[0x34];
    /* 0x34 */ s32 nextFlowMode;   /* index into bootup Data 0xC450 (0 N64DEMO .. 8 EDITORS);
                                    * written by the title menu rows */
    char pad38[0x40 - 0x38];
    /* 0x40 */ s32 procState;      /* state of the running mode's <pfx>_procMain loop */
    /* 0x44 */ s32 nextProcState;  /* state to enter after the current step (select screen, fade) */
    /* 0x48 */ s32 thenProcState;  /* the state after that (arc: 10 / 9, h2h: 6) */
    /* 0x4C */ s32 playerRider;    /* player 1's rider; mgr_pickRivals never picks it */
    /* 0x50 */ s32 playerFlags;    /* SelectWin out: character item flags | board item flags
                                    * (0x20000 / 0x20 = the toggleVariant bits) */
    /* 0x54 */ s32 playerBoard;    /* player 1's board (the three are SelectWin_init's out pointers;
                                    * game_setupVsGhost builds the player rider from them) */
    /* 0x58 */ s32 rider2;         /* player 2's rider / flags / board (the second SelectWin) */
    /* 0x5C */ s32 flags2;
    /* 0x60 */ s32 board2;
    /* 0x64 */ s32 course;         /* course id of the next race (mode procMain race state) */
    /* 0x68 */ s32 level;          /* difficulty 1..3 (mgr_initMatchRace) */
    /* 0x6C */ s32 demoIndex;      /* which attract demo n64proc / titproc runs next */
    char pad70[0xE0 - 0x70];
    /* 0xE0 */ InputPad *pads[8];  /* input_addPad_00011C70 appends; numPads counts */
    /* 0x100 */ InputPad *racePads[8]; /* the race's pads (replay_addInput, every game_setup*
                                    * builder): input_pauseAll / unpauseAll call pad_setPause on each;
                                    * pads[] above are the controllers (GameFlow_addPads) */
    /* 0x120 */ s32 numPads;
    /* 0x124 */ s32 numRacePads;
    /* 0x128 */ s32 activePad;     /* -1 = any pad; input_setActivePad / input_clearActivePad */
    char pad12C[0x134 - 0x12C];
    /* 0x134 */ char *game;        /* boot Bss 0x22C: the game world object. +0xC4 / +0xCC the
                                    * two players (their +0x800 is the pad), +0x108 / +0x10C the
                                    * camera windows (RaceProcess replayCam / savedCam swap in) */
    /* 0x138 */ struct RaceTracker *dataMgr; /* the running race's DataManager (include/gdl/race.h:
                                    * a RaceTracker subclass, node "DataManager"); every race /
                                    * demo ctor stores it */
    char pad13C[0x148 - 0x13C];
    /* 0x148 */ void *backup;      /* the save-data object, arg 2 of Backup_openSession */
    /* 0x14C */ struct BackupSession *session; /* include/gdl/backup.h (tag only: the NCC .cpp
                                    * user has no u8 / s8); the open record session (<pfx>_openSession /
                                    * closeSession); its ghost's count / data feed replayLen / replayData */
    char pad150[0x154 - 0x150];
    /* 0x154 */ char *unlocks;     /* the unlock / option record: +0 u16 unlock bits (1 << n),
                                    * +4 u16 option bits (8 = the title language menu),
                                    * +0x12..0x14 u8 settings */
    char pad158[0x160 - 0x158];
    /* 0x160 */ s32 recordBuf;     /* InputFrame buffer of recordCap frames, allocated by the
                                    * ctor (GameFlow, tail3a 10C8C); player 1 records into it */
    /* 0x164 */ s32 recordCap;     /* record capacity in frames: the first word of the backup
                                    * object (Backup.unk0 = 6400, 4 bytes a frame) */
    /* 0x168 */ s32 replayData[2]; /* per player: recorded input, pad_startPlay arg 2 */
    /* 0x170 */ s32 replayLen[2];  /* per player: pad_startPlay arg 3; the replay timeouts
                                    * (h2h / mgr / arc onTimeout) are built from it */
    /* 0x178 */ unsigned char recCourse;      /* replay_beginRecord*: the race being recorded (course, */
    /* 0x179 */ unsigned char recRider;       /* playerRider, playerFlags, playerBoard as bytes); */
    /* 0x17A */ unsigned char recFlags;       /* replay_saveGhost writes them into the ghost header */
    /* 0x17B */ unsigned char recBoard;
    char pad17C[0x181 - 0x17C];
    /* 0x181 */ unsigned char ghostRider;     /* the saved ghost's rider / flags / board (titproc vs-ghost */
    /* 0x182 */ unsigned char ghostFlags;     /* states, Backup_ghostGetRider / BoardFlag / Board); */
    /* 0x183 */ unsigned char ghostBoard;     /* game_setupVsGhost builds the ghost rider from them */
    /* 0x184 */ s32 unk184;        /* replay_beginRecord* / replay_saveGhost clear it */
    /* 0x188 */ s32 racePadsPaused; /* input_pauseAll sets it (returns the old value), unpauseAll clears */
    /* 0x18C */ s32 locked;        /* nonzero: every input_check* returns 0 */
    /* 0x190 */ void *fader;       /* the shared screen Fader (Fader_start / Fader_isDone /
                                    * Fader_checkDone); boot Bss 0x288 */
} InputMgr;

/* View a generic USO import placeholder (extern char / int D_00000000, import_XXXXXXXX) as
 * the input manager. Only for references whose relocation resolves to bootup Bss 0xF8: the
 * symbol and the offset stay the same, so the relocations do not change. */
#define INPUT_MGR(sym) (*(InputMgr *)&(sym))

#ifndef __cplusplus /* edg (NCC donors) rejects the &((T *)0)->f constant form */
typedef char InputPad_held_check[(int)&((InputPad *)0)->held == 0x10 ? 1 : -1];
typedef char InputPad_pressed_check[(int)&((InputPad *)0)->pressed == 0x18 ? 1 : -1];
typedef char InputPad_released_check[(int)&((InputPad *)0)->released == 0x1C ? 1 : -1];
typedef char InputPad_replayMode_check[(int)&((InputPad *)0)->replayMode == 0x4C ? 1 : -1];
typedef char InputPad_recordMask_check[(int)&((InputPad *)0)->recordMask == 0x6C ? 1 : -1];
typedef char InputPad_mirrorX_check[(int)&((InputPad *)0)->mirrorX == 0x70 ? 1 : -1];
typedef char InputFrame_size_check[sizeof(InputFrame) == 4 ? 1 : -1];
typedef char InputMgr_recordBuf_check[(int)&((InputMgr *)0)->recordBuf == 0x160 ? 1 : -1];
typedef char InputMgr_nextFlowMode_check[(int)&((InputMgr *)0)->nextFlowMode == 0x34 ? 1 : -1];
typedef char InputMgr_procState_check[(int)&((InputMgr *)0)->procState == 0x40 ? 1 : -1];
typedef char InputMgr_thenProcState_check[(int)&((InputMgr *)0)->thenProcState == 0x48 ? 1 : -1];
typedef char InputMgr_board2_check[(int)&((InputMgr *)0)->board2 == 0x60 ? 1 : -1];
typedef char InputMgr_ghost_check[(int)&((InputMgr *)0)->ghostBoard == 0x183 ? 1 : -1];
typedef char InputMgr_course_check[(int)&((InputMgr *)0)->course == 0x64 ? 1 : -1];
typedef char InputMgr_demoIndex_check[(int)&((InputMgr *)0)->demoIndex == 0x6C ? 1 : -1];
typedef char InputMgr_pads_check[(int)&((InputMgr *)0)->pads == 0xE0 ? 1 : -1];
typedef char InputMgr_racePads_check[(int)&((InputMgr *)0)->racePads == 0x100 ? 1 : -1];
typedef char InputMgr_racePadsPaused_check[(int)&((InputMgr *)0)->racePadsPaused == 0x188 ? 1 : -1];
typedef char InputMgr_numPads_check[(int)&((InputMgr *)0)->numPads == 0x120 ? 1 : -1];
typedef char InputMgr_activePad_check[(int)&((InputMgr *)0)->activePad == 0x128 ? 1 : -1];
typedef char InputMgr_game_check[(int)&((InputMgr *)0)->game == 0x134 ? 1 : -1];
typedef char InputMgr_unlocks_check[(int)&((InputMgr *)0)->unlocks == 0x154 ? 1 : -1];
typedef char InputMgr_dataMgr_check[(int)&((InputMgr *)0)->dataMgr == 0x138 ? 1 : -1];
typedef char InputMgr_backup_check[(int)&((InputMgr *)0)->backup == 0x148 ? 1 : -1];
typedef char InputMgr_session_check[(int)&((InputMgr *)0)->session == 0x14C ? 1 : -1];
typedef char InputMgr_replayData_check[(int)&((InputMgr *)0)->replayData == 0x168 ? 1 : -1];
typedef char InputMgr_replayLen_check[(int)&((InputMgr *)0)->replayLen == 0x170 ? 1 : -1];
typedef char InputMgr_fader_check[(int)&((InputMgr *)0)->fader == 0x190 ? 1 : -1];
typedef char InputMgr_locked_check[(int)&((InputMgr *)0)->locked == 0x18C ? 1 : -1];
#endif

#endif
