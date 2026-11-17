/* Native original-compiler source port from src/game_libs/game_libs_post.c (the snd_HandleSeqEvent_30AF4 NON_MATCHING body,
 * comments dropped; see that file for the per-line lever history). agent-g run 288: the in-tree body compiles to the
 * target's 528 words once linked with the callees / arena addresses at their own values and the TU's .rodata (switch-1
 * table, the 0.1f literal, switch-2 / switch-3 tables) at the USO RoData 0x1874 (scripts/match-origins-snd_HandleSeqEvent_30AF4.ld).
 * Void prototypes gl_func_emit_30AF4v / gl_func_00030AF4_{44F18,42364,460D0}v: calls whose result is never read. */
#include "../../include/uso_call.h"
/* PC port (agent-a run 817, plan 18.9): the callee of each `gl_func_emit` / `gl_func_emit_30AF4v`
 * jal and the object of each `gl_state_30AF4` HI16 / LO16 pair, from bootup.uso's TextReloc at
 * that site (snd_HandleSeqEvent_30AF4 = bootup Text 0x45160).  Call sites were paired with their
 * jals by a verify copy that spells each call token uniquely (identical .text bytes and reloc
 * offsets); gl_state_30AF4's six lui / addiu pairs (Text +0x4, +0x40, +0x80, +0xB4, +0x4B8,
 * +0x5EC) are read off the object directly: Data 0x1C09C, 0x1C0A8 (lines prev / f0 = mode share
 * the pair), 0x1C080 (the five clears and the first sv share two pairs), 0x1C0B0, Bss 0x338. */
typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
extern int gl_func_00041D0C();
extern int gl_func_00042364();
extern int gl_func_000460D0();
extern int gl_func_00041ED4();
extern int gl_func_00041F0C();
extern int gl_func_00041F44();
extern int gl_func_00044F18();
extern int gl_func_emit();
typedef struct {
    s32 f0, f4, f8, fC, f10, f14, f18;
} GlState30AF4;
extern GlState30AF4 gl_state_30AF4;
extern u32 gl_cmd_30AF4;
extern s32 D_30AF4_b, D_30AF4_c, D_30AF4_d, D_30AF4_e;
extern s32 D_30AF4_f, D_30AF4_g, D_30AF4_h, D_30AF4_i, D_30AF4_j, D_30AF4_k;
extern u32 D_30AF4_l;
extern s32 D_30AF4_m, D_30AF4_n, D_30AF4_o, D_30AF4_p, D_30AF4_q;
extern u32 D_30AF4_r;
extern s32 D_30AF4_s, D_30AF4_t, D_30AF4_v, D_30AF4_w, D_30AF4_x;
extern char gl_data_30AF4_45010, gl_data_30AF4_45F78;
#ifdef PORT
/* the command word's low byte: byte 3 on the cartridge (big-endian), byte 0 on the host.  The
 * word (bootup Data 0x1C0AC) is typed u32 in port/gen/blob_types.tsv (plan 18.1). */
#define CMD_B3 ((u8)gl_cmd_30AF4)
#else
#define CMD_B3 (((u8 *)&gl_cmd_30AF4)[3])
#endif
extern void gl_func_emit_30AF4v();
extern void gl_func_00030AF4_44F18v();
extern void gl_func_00030AF4_42364v();
extern void gl_func_00030AF4_460D0v();
#ifdef PORT /* one object for the bytes these tokens share, so gcc keeps the order (scripts/port-alias-audit.py, plan 18.7) */
extern char port_bootup_bss[];
extern char port_bootup_data[];
#define D_30AF4_f (*(__typeof__(D_30AF4_f) *)(port_bootup_data + 0x1C0B4))
#define D_30AF4_g (*(__typeof__(D_30AF4_g) *)(port_bootup_data + 0x1C0A8))
#define D_30AF4_h (*(__typeof__(D_30AF4_h) *)(port_bootup_data + 0x1C0B0))
#define D_30AF4_i (*(__typeof__(D_30AF4_i) *)(port_bootup_data + 0x1C0B4))
#define D_30AF4_j (*(__typeof__(D_30AF4_j) *)(port_bootup_data + 0x1C0B0))
#define D_30AF4_k (*(__typeof__(D_30AF4_k) *)(port_bootup_bss + 0x308))
#define D_30AF4_l (*(__typeof__(D_30AF4_l) *)(port_bootup_bss + 0x308))
#define D_30AF4_m (*(__typeof__(D_30AF4_m) *)(port_bootup_data + 0x1C0B4))
#define D_30AF4_n (*(__typeof__(D_30AF4_n) *)(port_bootup_bss + 0x308))
#define D_30AF4_o (*(__typeof__(D_30AF4_o) *)(port_bootup_bss + 0x308))
#define D_30AF4_p (*(__typeof__(D_30AF4_p) *)(port_bootup_data + 0x1C0B0))
#define D_30AF4_q (*(__typeof__(D_30AF4_q) *)(port_bootup_bss + 0x308))
#define D_30AF4_r (*(__typeof__(D_30AF4_r) *)(port_bootup_bss + 0x308))
#define D_30AF4_t (*(__typeof__(D_30AF4_t) *)(port_bootup_data + 0x1C0A8))
#define D_30AF4_w (*(__typeof__(D_30AF4_w) *)(port_bootup_data + 0x1C0B0))
#define D_30AF4_x (*(__typeof__(D_30AF4_x) *)(port_bootup_data + 0x1C0B4))
#endif
void snd_HandleSeqEvent_30AF4(mode, sub, arg2)
s32 mode;
u32 sub;
s32 arg2;
{
    s32 flag;
    s32 wv;
    s32 prev;
    s32 w;
    s32 sv;
    flag = 0;
    if (XDATA(gl_state_30AF4, port_bootup_data, 0x1C09C).f0 == 1) {
        gl_func_00041D0C();
    }
    XCALL(gl_func_emit_30AF4v, AudioThread_QueueCmdS32)(0xFB000000, &gl_data_30AF4_45010);
    prev = XDATA(gl_state_30AF4, port_bootup_data, 0x1C0A8).f0;
    XDATA(gl_state_30AF4, port_bootup_data, 0x1C0A8).f0 = mode;
    XCALL(gl_func_emit, snd_soundObjChannelsOff)(0);
    XCALL(gl_func_emit_30AF4v, snd_soundObjChannelsOff)(1);
    D_30AF4_b = 0;
    D_30AF4_c = 0;
    D_30AF4_d = 0;
    D_30AF4_e = 0;
    XDATA(gl_state_30AF4, port_bootup_data, 0x1C080).f4 = 0;
    XDATA(gl_state_30AF4, port_bootup_data, 0x1C080).f8 = 0;
    XDATA(gl_state_30AF4, port_bootup_data, 0x1C080).f14 = 0;
    XDATA(gl_state_30AF4, port_bootup_data, 0x1C080).f0 = 0;
    XDATA(gl_state_30AF4, port_bootup_data, 0x1C080).f18 = 0;
    XCALL(gl_func_emit, snd_InitCueQueues)();
    gl_func_00030AF4_460D0v();
    sv = (s32)&XDATA(gl_state_30AF4, port_bootup_data, 0x1C080) + 0 * (int)(flag);
    if (sv) {}
    switch (mode) { /* labels (both switches) from the ROM tables at RoData 0x1874 / 0x18B0 (port/tools/jumptable-audit.py, run 831) */
    case 1:
        gl_func_00042364(1);
        flag = 1;
        break;
    case 0:
        D_30AF4_f = 0;
        gl_func_00042364(0);
        XCALL(gl_func_emit, snd_PlayPlayer1SeqDefault)(2);
        flag = 1;
        break;
    case 2:
        gl_func_00042364(2);
        gl_cmd_30AF4 = 16;
        XCALL(gl_func_emit, snd_PlayPlayer1SeqDefault)(CMD_B3);
        flag = 1;
        break;
    case 3:
        if (prev != 0) {
            gl_func_00042364(5);
            flag = 1;
        }
        XCALL(gl_func_emit, snd_PlayPlayer1SeqDefault)(3);
        break;
    case 4:
        if (prev == 0) {
            D_30AF4_g = 12;
            break;
        }
        if (prev == 3) {
            for (;;) {
                if (XCALL(gl_func_emit, AudioThread_GetLayerSamplesLeft)(2, 14, 0) < 100) {
                    break;
                }
                XCALL(gl_func_emit, AudioThread_WaitForAudioTask)();
            }
        }
        D_30AF4_h = sub + 0 * (int)(flag);
        sv = D_30AF4_h;
        gl_cmd_30AF4 = D_30AF4_h + 4; if (1) {}
        if (D_30AF4_i != 0 && D_30AF4_h == 6) {
            gl_cmd_30AF4 = 7 + 0 * (int)(D_30AF4_i + D_30AF4_i);
        }
        if (D_30AF4_i == 4) {
            gl_func_00042364(4);
            gl_cmd_30AF4 = D_30AF4_j + 18; if (1) {}
        } else if (sv + 0 * (int)(sv + sv + sv + sv + sv + sv) == 0) {
            gl_func_00042364(6);
        } else {
            gl_func_00042364(3);
        }
        flag = 1;
        D_30AF4_k = 14;
        break;
    case 7:
        if (gl_cmd_30AF4 == 13) {
            break;
        }
        if (gl_cmd_30AF4 == 15) {
            break;
        }
        if (gl_cmd_30AF4 == 14) {
            break;
        }
        if (prev == 8) {
            break;
        }
        if (D_30AF4_l == 14) {
            gl_cmd_30AF4 = 14;
        } else if (D_30AF4_l == 16) {
            gl_cmd_30AF4 = 16;
        } else {
            gl_cmd_30AF4 = 13;
        }
        XCALL(gl_func_emit, snd_PlayPlayer1SeqDefault)(CMD_B3);
        break;
    case 5:
        {
        w = 35;
        if (sv) {}
        if (D_30AF4_m == 4) {
            gl_cmd_30AF4 = 13;
            wv = 523 + 0 * (int)(sub);
        } else {
            switch (sub) {
            case 0:
                wv = 21;
                if (D_30AF4_m != 0) {
                    *(s32 *)&gl_cmd_30AF4 = 14;
                }
                break;
            case 1:
                gl_cmd_30AF4 = 13;
                wv = 20;
                break;
            case 2:
                gl_cmd_30AF4 = 13;
                wv = 22;
                w = 60;
                break;
            case 3:
            default:
                *(s32 *)&gl_cmd_30AF4 = 14;
                wv = 256;
                break;
            }
        }
        D_30AF4_n = gl_cmd_30AF4 + 0 * (int)(sv);
        if (sv) {}
        if (D_30AF4_m == 0 && sub == 0) {
            ((GlState30AF4 *)sv)->fC = 256;
            XCALL(gl_func_emit, AudioThread_QueueCmdS32)(0x83010000, 2000);
            D_30AF4_o = 14;
        } else if (D_30AF4_p == 1) {
            XCALL(gl_func_emit, AudioThread_QueueCmdS8)(0x46010000, 0);
            XCALL(gl_func_emit, snd_ScheduleSeqAfter)(CMD_B3, w + 10);
        } else {
            XCALL(gl_func_emit, AudioThread_QueueCmdS32)(0x83010000, 80);
            XCALL(gl_func_emit, snd_ScheduleSeqAfter)(CMD_B3, w);
        }
        XCALL(gl_func_emit, snd_PostEvent)(wv);
        }
        break;
    case 6:
        D_30AF4_q = 14;
        break;
    case 8:
        if (gl_cmd_30AF4 != 15) {
            gl_cmd_30AF4 = 16;
            D_30AF4_r = 16;
            XCALL(gl_func_emit, snd_PlayPlayer1SeqDefault)(CMD_B3);
        }
        break;
    case 10:
        gl_func_00030AF4_42364v(3);
        sv = (u32)&XDATA(gl_state_30AF4, port_bootup_data, 0x1C0B0);
        if (sv) {}
        *(s32 *)sv = sub;
        if (*(u32 *)sv == 1) {
            gl_cmd_30AF4 = 5;
        } else {
            gl_cmd_30AF4 = 17;
        }
        XCALL(gl_func_emit, snd_ScheduleSeqAfter)(CMD_B3, 35);
        flag = 1;
        break;
    case 11:
        gl_func_00042364(7);
        gl_cmd_30AF4 = 18;
        XCALL(gl_func_emit, AudioThread_QueueCmdS32)(0x81000F00, 0);
        sv = sub;
        D_30AF4_s = sv;
        if (sv == 2) {
            XCALL(gl_func_emit, AudioThread_QueueCmdS32)(0x81001300, 0);
        }
        XCALL(gl_func_emit, snd_PlayPlayer1SeqDefault)(CMD_B3);
        flag = 1;
        break;
    case 13:
        gl_cmd_30AF4 = 19;
        XCALL(gl_func_emit, snd_ScheduleSeqAfter)(CMD_B3, 80);
        break;
    case 9:
    case 12:
        break;
    }
    if (flag == 1) {
        gl_func_00041ED4();
        XCALL(gl_func_emit, snd_PlayPlayer0Seq0)();
        gl_func_00041F0C();
        gl_func_00041F44();
        if (mode != 4) {
            XCALL(gl_func_emit, snd_PlayPlayer2Seq)(0, 0);
        }
    }
    sv = (u32)&XDATA(gl_state_30AF4, port_bootup_bss, 0x338);
    if (D_30AF4_t != 5) {
        gl_func_00030AF4_44F18v(0);
    } else {
        *(f32 *)&((GlState30AF4 *)sv)->f14 = 0.1f;
        XCALL(gl_func_emit, snd_TriggerChannel8Cue)(0, 100, ((GlState30AF4 *)sv)->f4);
        D_30AF4_v = 4;
    }
    switch (mode) {
    case 4:
        if (prev == 0) {
            break;
        }
        if (D_30AF4_w == 1) {
            gl_func_00030AF4_44F18v(1);
        } else if (D_30AF4_w == 0) {
            gl_func_00030AF4_44F18v(2);
        }
        if (D_30AF4_x == 1 || D_30AF4_x == 3) {
            switch (D_30AF4_w) {
            case 0:
                XCALL(gl_func_emit_30AF4v, snd_PostEvent)(0x1F);
                break;
            case 1:
                XCALL(gl_func_emit_30AF4v, snd_PostEvent)(0x1B);
                break;
            case 2:
                XCALL(gl_func_emit_30AF4v, snd_PostEvent)(0x1D);
                break;
            case 3:
                XCALL(gl_func_emit_30AF4v, snd_PostEvent)(0x19);
                break;
            case 4:
                XCALL(gl_func_emit_30AF4v, snd_PostEvent)(0x1E);
                break;
            case 5:
                XCALL(gl_func_emit_30AF4v, snd_PostEvent)(0x1C);
                break;
            case 6:
                XCALL(gl_func_emit_30AF4v, snd_PostEvent)(0x1A);
                break;
            case 7:
                XCALL(gl_func_emit_30AF4v, snd_PostEvent)(0x18);
                break;
            }
        }
        if (D_30AF4_w == 8) {
            XCALL(gl_func_emit, snd_PostEvent)(0xBC);
        }
        break;
    case 0: case 2: case 5: case 7: case 8: case 9: case 10: case 11:
        break;
    case 1:
        XCALL(gl_func_emit, AudioThread_QueueCmdS32)(0xF8000000, 0);
        XCALL(gl_func_emit, snd_PostEvent)(0xA3);
        break;
    case 3:
        XCALL(gl_func_emit, AudioThread_QueueCmdS32)(0xF8000000, 0);
#ifdef PORT64 /* 0.3f: the definition takes a float (abi-scan X64KR, plan 21.10) */
        PORT64_CALLP(void (int, float, signed char), snd_UpdateChannel10Emitter)(0, PORT64_F(0x3E99999A), 0);
#else
        XCALL(gl_func_emit, snd_UpdateChannel10Emitter)(0, 0x3E99999A, 0);
#endif
        break;
    case 6:
        switch (sub) {
        case 0:
            XCALL(gl_func_emit, snd_PostEvent)(14);
            break;
        case 1:
            XCALL(gl_func_emit, snd_PostEvent)(15);
            break;
        }
        break;
    }
    XCALL(gl_func_emit, AudioThread_QueueCmdS32)(0xFB000000, 0);
    XCALL(gl_func_emit, snd_SetLevelRampTarget)(0);
    XCALL(gl_func_emit, AudioThread_QueueCmdS32)(0xFB000000, &gl_data_30AF4_45F78);
}
#ifdef PORT
#undef D_30AF4_f
#undef D_30AF4_g
#undef D_30AF4_h
#undef D_30AF4_i
#undef D_30AF4_j
#undef D_30AF4_k
#undef D_30AF4_l
#undef D_30AF4_m
#undef D_30AF4_n
#undef D_30AF4_o
#undef D_30AF4_p
#undef D_30AF4_q
#undef D_30AF4_r
#undef D_30AF4_t
#undef D_30AF4_w
#undef D_30AF4_x
#endif
