#include "../../include/uso_call.h"  /* donors build without -I include */
/* MIPSpro 7.2.1 C++ (o32 edgcpfe, EDG 2.29) EXACT donor for SoundOptions_update (279 words), built by
 * tools/ncc721.sh and spliced over the placeholder in viewrec_uso.c on both build paths (agent-g run 652).
 * Its two 4-label `switch (self->cursor)` are native 4-entry jump tables only under EDG 2.29 (cfe tables
 * from 5 labels, IDO 7.1 NCC never). The tables are viewrec RoData +0x38 / +0x48 (in-place %lo); this
 * donor's .rodata holds just the two tables, at +0x0 / +0x10, and the splice-renamed reloc
 * SoundOptions_update_rodata is pinned to 0x38 in undefined_syms_auto.txt.
 * `self->mode--;` as its own statement (not `--self->mode` as the call arg) burns the ugen temp the target
 * skips in the sound-mode arms. */
extern "C" int gl_func_00000000(...);
extern "C" void viewrec_uso_func_00000000(...);
struct VrNode { char node[0x28]; int vptr; };
/* GameFlow (include/game/input.h InputMgr, bootup Bss 0xF8): the settings record at +0x154 */
struct VrGameFlow { char pad[0x154]; char *options; };
extern "C" VrGameFlow gInputMgr_vr4990;  /* 0-pinned GameFlow import */
#define gInputMgr_vr gInputMgr_vr4990
extern "C" int vr_sndTbl_4990[];  /* viewrec Data+0x318: per-row sound ids (in-place 0, 0-pinned) */
typedef unsigned char u8;
typedef unsigned short u16;
typedef int s32;
/* the sound options screen (BGM / SE / voice levels and the sound mode); the rows are cursor 0..3 */
struct VrSoundOptions {
    /* 0x00 */ VrNode node;
    /* 0x2C */ char pad2C[0x84 - 0x2C];
    /* 0x84 */ s32 cursor;
    /* 0x88 */ s32 mode;            /* options flags & 3 */
    /* 0x8C */ s32 levels[3];       /* options +0x12..+0x14 */
};
extern "C" void SoundOptions_update(VrSoundOptions *self) {
    self->levels[0] = ((u8 *)gInputMgr_vr.options)[0x12];
    self->levels[1] = ((u8 *)gInputMgr_vr.options)[0x13];
    self->levels[2] = ((u8 *)gInputMgr_vr.options)[0x14];
    self->mode = *(u16 *)(gInputMgr_vr.options + 4) & 3;
    if (self->cursor > 0 && CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x2004)) {
        CALL(snd_postEvent)(1);
        self->cursor--;
    }
    if (self->cursor < 3 && CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x8008)) {
        CALL(snd_postEvent)(1);
        self->cursor++;
    }
    if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x10001)) {
        switch (self->cursor) {
        case 0:
            if (self->levels[self->cursor] > 0) {
                self->levels[self->cursor]--;
                CALL(settings_vol1Down_000123F0)(&gInputMgr_vr);
                CALL(snd_postEvent)(vr_sndTbl_4990[self->cursor]);
            } else {
                CALL(snd_postEvent)(165);
            }
            break;
        case 1:
            if (self->levels[self->cursor] > 0) {
                self->levels[self->cursor]--;
                CALL(settings_vol0Down_000124C4)(&gInputMgr_vr);
                CALL(snd_postEvent)(vr_sndTbl_4990[self->cursor]);
            } else {
                CALL(snd_postEvent)(165);
            }
            break;
        case 2:
            if (self->levels[self->cursor] > 0) {
                self->levels[self->cursor]--;
                CALL(settings_vol2Down_00012598)(&gInputMgr_vr);
                CALL(snd_postEvent)(vr_sndTbl_4990[self->cursor]);
            } else {
                CALL(snd_postEvent)(165);
            }
            break;
        case 3:
            if (self->mode > 0) {
                self->mode--;
                CALL(settings_applySoundMode_0001266C)(&gInputMgr_vr, self->mode);
                CALL(snd_postEvent)(vr_sndTbl_4990[self->cursor]);
            } else {
                CALL(snd_postEvent)(165);
            }
            break;
        }
    }
    if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x4002)) {
        switch (self->cursor) {
        case 0:
            if (self->levels[self->cursor] < 10) {
                self->levels[self->cursor]++;
                CALL(settings_vol1Up_00012458)(&gInputMgr_vr);
                CALL(snd_postEvent)(vr_sndTbl_4990[self->cursor]);
            } else {
                CALL(snd_postEvent)(165);
            }
            break;
        case 1:
            if (self->levels[self->cursor] < 10) {
                self->levels[self->cursor]++;
                CALL(settings_vol0Up_0001252C)(&gInputMgr_vr);
                CALL(snd_postEvent)(vr_sndTbl_4990[self->cursor]);
            } else {
                CALL(snd_postEvent)(165);
            }
            break;
        case 2:
            if (self->levels[self->cursor] < 10) {
                self->levels[self->cursor]++;
                CALL(settings_vol2Up_00012600)(&gInputMgr_vr);
                CALL(snd_postEvent)(vr_sndTbl_4990[self->cursor]);
            } else {
                CALL(snd_postEvent)(165);
            }
            break;
        case 3:
            if (self->mode < 2) {
                self->mode++;
                CALL(settings_applySoundMode_0001266C)(&gInputMgr_vr, self->mode);
                CALL(snd_postEvent)(vr_sndTbl_4990[self->cursor]);
            } else {
                CALL(snd_postEvent)(165);
            }
            break;
        }
    }
    if (CALL(input_checkPressed_00011E00)(&gInputMgr_vr, 0x300)) {
        CALL(snd_postEvent)(0x802);
        CALL(settings_save_00012150)(&gInputMgr_vr);
        CALL(viewrec_uso_func_00000000)();
    }
    CALL(Node_update)(self);
}
