#include "common.h"
#include "game/course.h"
#include "gdl/node.h"
#include "gdl/race.h"
#include "game/input.h"
#include "uso_call.h"

extern int gl_func_00000000();
extern char D_00000000;

typedef struct { int a, b, c; } TrnTri3i;
typedef struct { int a, b, c, d; } TrnQuad4;

/* trnproc.uso's own Bss (relocations against the Bss section base, in-place addend = the
 * offset). trn_bss is pinned at 0 (undefined_syms_auto.txt): a symbol of its own, so uopt
 * does not share a lui with the bootup imports spelled off D_00000000. */
extern char trn_bss[];
/* Bss+0x1B0: the running TrnRace process (stored by onPauseSelect before the menu callback). */
#define gTrnRace (*(char **)(trn_bss + 0x1B0))

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00000000);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00000008);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00000010);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00000194);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_000001A4);

/* trnproc_uso_func_000001B0: pause menu "continue" (as TimRace_pauseContinue_000006B0, without
 * the input_clearActivePad). */
void trnproc_uso_func_000001B0(void) {
    CALL(Process_resume_0000FAE8)(gTrnRace);
    CALL(Node_resumeUpdate_0000E450)(TIMRACE(gTrnRace)->dataManager);
}

/* trnproc_uso_func_000001E4: pause menu row 1 (restart): resume, then the restart helper 9A0. */
void trnproc_uso_func_000001E4(void) {
    CALL(Process_resume_0000FAE8)(gTrnRace);
    CALL(trnproc_uso_func_000009A0)(gTrnRace);
}

/* trnproc_uso_func_00000214: pause menu "quit": next flow state 3 / 4, save the setting at
 * +0x6B0, fade the process out (as TrkRace_pauseQuit_00000818). */
void trnproc_uso_func_00000214(void) {
    INPUT_MGR(D_00000000).procState = 3;
    *(int *)((char *)&D_00000000 + 0x44) = 4;
    CALL(settings_set8_00012818)(&D_00000000, gTrnRace + 0x6B0);
    CALL(Process_fadeOut_0000F954)(gTrnRace, -1, 0);
}

/* trnproc_uso_func_00000268: read an int from the gdl file (cross-USO template, as checkpoint
 * checkpoint_uso_func_00000034 / game_uso stream_readS32_00000280). */
void trnproc_uso_func_00000268(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

/* trnproc_uso_func_000002A4: read a float from the gdl file (template, as balloon_uso_func_00000104). */
void trnproc_uso_func_000002A4(float *dst) {
    float buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

/* trnproc_uso_func_000002E0: read a 16-byte quad from the gdl file (template, as
 * tim_streamReadQuad_000008B4). */
void trnproc_uso_func_000002E0(TrnQuad4 *dst) {
    TrnQuad4 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 16);
    *dst = buf;
}

/* trnproc_uso_func_00000338: read a Vec3 from the gdl file (template, as balloon_uso_func_00000094 /
 * game_uso stream_readVec3_000001D4). */
void trnproc_uso_func_00000338(float *dst) {
    int pad_top[1];
    TrnTri3i raw;
    int pad_mid[2];
    TrnTri3i tmp;
    int pad_bot[2];
    CALL(fs_gdlReadFile)(&D_00000000, &raw, 12);
    tmp = raw;
    dst[0] = *(float *)&tmp.a;
    dst[1] = *(float *)&tmp.b;
    dst[2] = *(float *)&tmp.c;
}

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_000003A8);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_000006CC);

/* trnproc_uso_func_00000858: TrnRace onPauseSelect (as TimRace_onPauseSelect, plus the
 * dataManager +0xFC guard). */
void trnproc_uso_func_00000858(int *self) {
    int *v0;
    int new_var;
    int v1;
    int stride;
    if (*(int *)((char *)TIMRACE(self)->dataManager + 0xFC) != 0) {
        return;
    }
    if (CALL(Fader_isDone_00013980)(INPUT_MGR(D_00000000).fader, self) == 0) {
        return;
    }
    v0 = (int *) self[0x48 / 4];
    v1 = v0[0x7C / 4];
    if (v1 != 0) {
        CALL(snd_postEvent)(5);
        if (1) {
        }
        v0 = (int *) self[0x48 / 4];
        v1 = v0[0x7C / 4];
    }
    stride = 40;
    if (((void (*)(void)) (*((int *) ((((char *) v0) + (v1 * stride)) + 0x90)))) != 0) {
        gTrnRace = (char *)self;
        if (1) {
        }
        v0 = (int *) self[(0x48 / 4) ^ 0];
        (*((void (**)(void)) ((((char *) v0) + (v0[0x7C / 4] * stride)) + (new_var = 0x90))))();
    }
}

/* trnproc_uso_func_00000908: clear the exit step (+0x504). */
void trnproc_uso_func_00000908(char *self) {
    *(int *)(self + 0x504) = 0;
}

/* trnproc_uso_func_00000910: TrnRace onTimeout: step 0 fades out (Fader_start(3, 1, self)),
 * step 1 waits for the fader, restarts (9F0) and fades back in. */
void trnproc_uso_func_00000910(char *self) {
    switch (*(int *)(self + 0x504)) {
    case 0:
        CALL(Fader_start)(INPUT_MGR(D_00000000).fader, 3, 1, self);
        *(int *)(self + 0x504) = 1;
        break;
    case 1:
        if (CALL(Fader_checkDone_00013924)(INPUT_MGR(D_00000000).fader) != 0) {
            CALL(trnproc_uso_func_000009F0)(self);
            CALL(Fader_start)(INPUT_MGR(D_00000000).fader, 1, 1);
        }
        break;
    }
}

/* trnproc_uso_func_000009A0: pause the race: race pads off, DataManager update paused,
 * sub-state 3, state flag 1, counter 0. */
void trnproc_uso_func_000009A0(char *self) {
    CALL(input_setRacePadsActive)(&D_00000000, 0);
    CALL(Node_pauseUpdate_0000E464)(TIMRACE(self)->dataManager);
    *(int *)(self + 0x4E0) = 3;
    *(int *)(self + 0x4DC) = 1;
    *(int *)(self + 0x4E4) = 0;
}

/* trnproc_uso_func_000009F0: restart: reset the pad of the GameFlow's current player
 * (+0x134 -> +0xC4 -> +0x800), then the RaceProcess global slot-11 call. */
void trnproc_uso_func_000009F0(char *self) {
    char *p = *(char **)(*(char **)((char *)&D_00000000 + 0x134) + 0xC4);
    char *pad = *(char **)(p + 0x800);
    CALL(pad_reset_000673FC)(pad, *(int *)(pad + 0x34));
    CALL(RaceProcess_callGlobalSlot11_00008674)(self);
}

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00000A2C);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00000B44);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00000B78);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00000DD4);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00001098);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00001228);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00001284);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_000012AC);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00001324);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_000013A0);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_000013C4);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_000016C8);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00001778);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_0000180C);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_0000188C);

INCLUDE_ASM("asm/nonmatchings/trnproc_uso/trnproc_uso", trnproc_uso_func_00001A98);

/* trnproc_uso_func_00001B84: Parm<Int>::read: skip the tag int, read the value at +0x10
 * (template, as tit_readIntParm). */
void trnproc_uso_func_00001B84(char *dst) {
    int tmp;
    trnproc_uso_func_00000268(&tmp);
    trnproc_uso_func_00000268((int *)(dst + 0x10));
}

/* trnproc_uso_func_00001BB4: Parm<Float>::read: skip the tag int, read the value at +0x10
 * (template, as tit_readFloatParm). */
void trnproc_uso_func_00001BB4(char *dst) {
    int tmp;
    trnproc_uso_func_00000268(&tmp);
    trnproc_uso_func_000002A4((float *)(dst + 0x10));
}

/* trnproc_uso_func_00001BE4: Parm<Quad>::read: skip the tag int, read the value at +0x10
 * (template, as tit_readQuadParm). */
void trnproc_uso_func_00001BE4(char *dst) {
    int tmp;
    trnproc_uso_func_00000268(&tmp);
    trnproc_uso_func_000002E0((TrnQuad4 *)(dst + 0x10));
}

/* trnproc_uso_func_00001C14: ItemType create: new ColourItem subclass (0x40 bytes, vtable
 * bootup Data+0x75A0), linked under the template's list head at +0x40 (template, as
 * h2h_ItemType_create_00001A6C). */
void *trnproc_uso_func_00001C14(int *arg0) {
    int *p2;
    int *head;
    int *p1;
    p1 = (int*)CALL(mem_new)(0x40);
    if (p1 != 0) {
        CALL(ColourItem_ctor)(p1);
        NODE(p1)->vptr = (char *)&D_00000000;
        ((ColourSubItem *)p1)->unk3C = 0;
    }
    p2 = p1;
    p1 = arg0;
    head = (int*)p1[0x40 / 4];
    if ((int*)p1[0x40 / 4] != 0) {
        CALL(list_append)((char*)p2 + 0x10, head);
        if (NODE(head)->parent != 0) {
            NODE(head)->unk04 = 1;
        }
        NODE(head)->parent = (Node *)p2;
    }
    return p2;
}

/* trnproc_uso_func_00001CA4: Parm<Vec3>::read: skip the tag int, read the value at +0x10
 * (template, as tit_readVec3Parm). */
void trnproc_uso_func_00001CA4(char *dst) {
    int tmp;
    trnproc_uso_func_00000268(&tmp);
    trnproc_uso_func_00000338((float *)(dst + 0x10));
}

