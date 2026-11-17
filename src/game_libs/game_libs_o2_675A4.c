/* IDO 7.1 -O2 donor for pad_update -- input-debounce / flag-update
 * routine (0x418 / 262 words). PROVEN EXACT 262/262 words; the link-resolved
 * words are the blank jal and the HI/LO pair of the dead-zone literal.
 *
 * WIRED via REPLACE_FUNC_BODY donor splice into game_libs_post1b.c.o (the
 * 3C0C / 2B94 recipe): the stick dead-zone threshold is the DOUBLE LITERAL
 * 0.2 (bootup.uso RoData +0x2200 holds 0x3FC999999999999A after the
 * section's leading marker word), not a data variable. As a literal it is a
 * uopt constant candidate coloured after the 0.0f literal (target zero f12 /
 * threshold f14); the old named `f64 temp_f14 = gl_D_2200_thr.v` web was
 * coloured first and swapped them (the 8-word residue). The donor-local
 * .rodata reloc is renamed pad_update_rodata and pinned to +0x2200.
 *
 * Load-bearing shapes kept from the NM body (docs/IDO_CODEGEN.md
 * #store-forward-ring-across-bb-and-scaled-index-first-675a4): the 0x18
 * store + re-read, the scaled-index-first ==1 cursor (in field form that is
 * `&p->replayFrames[p->replayFrame]`), `(cur | 0) & ~prev`, and the dead
 * `while (0) { temp_f2 = y; }` anchor. */
#include "common.h"
#include "gdl/gl_data.h"
#include "gdl/pad.h"
#include "uso_call.h"
#ifdef PORT
#include "port_game.h" /* port/include: the trace hook */
#endif

extern int D_00000000;
extern int gl_func_00000000();

/* agent-b run 521: typed over gdl/pad.h PadCtl; identical objdump -dr. Spelling notes: a
 * field read is lowered in the opposite order from the old `FW(arg0, off)` form, so the
 * commutative pairs that were `X | FW(0x10)` are written `p->held | X` here (PLAY arm,
 * injected input) and the doubleTap test is `lastPressed & pressed`. D_00000000 below is
 * the dirIndex table (one u8 per direction-bit pattern). */
void pad_update(PadCtl *p) {
    f32 temp_f0;
    f32 temp_f2;
    f32 var_f0;
    s16 pos;
    s32 temp_a0;
    char *rumble;  /* read back through a u8 * cast: a plain u8 * local swaps the addu */
    s32 temp_t4;
    s32 temp_v0_2;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 temp_v0_6;
    s32 temp_v0_7;
    s32 port;
    s32 temp_v1_2;
    InputFrame *frame;

    port = p->port;
    if ((port != 0) && (p->paused == 0)) {
        rumble = (char *)p->rumble;
        if (rumble != 0) {
            pos = p->rumblePos;
            p->rumblePos = (s16) (pos + 1);
            CALL(pad_setSlotValueGlobal)(&XDATA(gSysLib, port_bootup_bss, 0x3C740), port - 1, ((u8 *)rumble)[pos], p);
            if (p->rumble[p->rumblePos] == 0xFF) {
                p->rumble = 0;
                p->rumblePrio = 0;
            }
        }
    }
    p->prevHeld = p->held;
    p->held = 0;
    if (p->paused == 0) {
        if (p->port != 0) {
            CALL(pad_decodeButtonMap_00034CC8)(&XDATA(gSysLib, port_bootup_bss, 0x3C740), p);
#ifdef PORT
            /* trace input hook (plan 9.6.4): log, or inject on replay, the decoded held word
             * and the raw stick bytes, before mirroring and the game's own replay block */
            {
                unsigned int held = (unsigned int)p->held;
                port_trace_pad(p->port - 1, port_game_frame_count(), p->replayFrame, &held,
                               &p->rawStickX, &p->rawStickY);
                p->held = (int)held;
            }
#endif
        }
        if (p->mirrorX != 0) {
            p->rawStickX = (s8) -p->rawStickX;
        }
        if (p->replayMode != PAD_REPLAY_OFF) {
            p->replayFrame = p->replayFrame + 1;
            if ((u32) p->replayFrame >= (u32) p->replayLen) {
                p->replayMode = PAD_REPLAY_OFF;
            }
            temp_v0_2 = p->replayMode;
            if (temp_v0_2 == PAD_REPLAY_RECORD) {
                p->replayFrames[p->replayFrame].stickX = p->rawStickX;
                p->replayFrames[p->replayFrame].stickY = p->rawStickY;
                temp_t4 = p->held & p->recordMask & 0x1700;
                p->replayFrames[p->replayFrame].buttons = temp_t4;
                *p->replayLenOut = p->replayFrame;
            } else if (temp_v0_2 == PAD_REPLAY_PLAY) {
                frame = &p->replayFrames[p->replayFrame];
                p->held = (s32) (p->held | frame->buttons);
                p->rawStickX = frame->stickX;
                p->rawStickY = frame->stickY;
            }
        }
        while (0) { temp_f2 = p->stickY; }
        temp_f0 = (f32) p->rawStickX / 80.0f;
        p->stickX = temp_f0;
        p->stickY = (f32) ((f32) p->rawStickY / 80.0f);
        if (temp_f0 < 0.0f) {
            var_f0 = -p->stickX;
        } else {
            var_f0 = p->stickX;
        }
        if ((f64) var_f0 < 0.2) {
            p->stickX = 0.0f;
        }
        temp_f2 = p->stickY;
        if (temp_f2 < 0.0f) {
            var_f0 = -temp_f2;
        } else {
            var_f0 = temp_f2;
        }
        if ((f64) var_f0 < 0.2) {
            p->stickY = 0.0f;
        }
    }
    if (p->injectPending != 0) {
        p->held = (s32) (p->held | p->injectButtons);
        p->injectPending = 0;
        p->stickX = (f32) p->injectStickX;
        p->stickY = (f32) p->injectStickY;
    }
    if ((f64) p->stickX > 0.5) {
        p->held = (s32) (p->held | 0x4000);
    } else {
        p->held = (s32) (p->held & ~0x4000);
    }
    if ((f64) p->stickX < -0.5) {
        p->held = (s32) (p->held | 0x10000);
    } else {
        p->held = (s32) (p->held & 0xFFFEFFFF);
    }
    if ((f64) p->stickY > 0.5) {
        p->held = (s32) (p->held | 0x2000);
    } else {
        p->held = (s32) (p->held & ~0x2000);
    }
    if ((f64) p->stickY < -0.5) {
        p->held = (s32) (p->held | 0x8000);
    } else {
        p->held = (s32) (p->held & 0xFFFF7FFF);
    }
    temp_v0_4 = p->held;
    temp_v1_2 = p->prevHeld;
    temp_a0 = p->tapTimer;
    p->pressed = (temp_v0_4 | 0) & ~temp_v1_2;
    p->released = (s32) (~temp_v0_4 & temp_v1_2);
    if (temp_a0 != 0) {
        temp_v0_5 = temp_a0 - 1;
        p->tapTimer = temp_v0_5;
        if (temp_v0_5 > 0) {
            temp_v0_6 = p->lastPressed & p->pressed;
            p->doubleTap = temp_v0_6;
            if (temp_v0_6 != 0) {
                p->tapTimer = 0;
            }
        }
    } else {
        temp_v0_7 = p->pressed;
        p->doubleTap = 0;
        p->lastPressed = temp_v0_7;
        if (temp_v0_7 != 0) {
            p->tapTimer = 0xA;
        }
    }
    p->dirIndex = (s16) *(u8*)((char*)&XDATA(D_00000000, port_bootup_data, 0x225C8) + ((u32)(p->held & 0x1E000) >> 0xD));
}
