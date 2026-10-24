#include "common.h"

/* IDO 7.1 -O2 donor for game_libs_func_0002A07C (0x1DC / 119 words) = OoT's
 * AudioSeq_HandleScriptFlowControl (seqplayer.c). The CALL / LOOP stack pushes are
 * written as an s32 copy of the depth, the store(s), then a separate increment (OoT's
 * stack[depth++] colours the depth t1, target v0). Spliced into game_libs_post.c.o via
 * REPLACE_FUNC_BODY; the splice renames the donor-local .rodata jumptable reloc
 * (opcodes 0xF2..0xFF) to game_libs_func_0002A07C_rodata, pinned to the USO table at
 * +0x1020. */
typedef struct { u8 *pc; u8 *stack[4]; u8 remLoopIters[4]; u8 depth; s8 value; } gl_2A07C_SeqScriptState;
typedef struct { char pad[0x18]; u8 *seqData; } gl_2A07C_SeqPlayer;
extern int gl_func_0003F05C();

s32 game_libs_func_0002A07C(gl_2A07C_SeqPlayer *seqPlayer, gl_2A07C_SeqScriptState *state, s32 cmd, s32 cmdArg) {
    s32 d;
    switch (cmd) {
        case 0xFF:
            if (state->depth == 0) {
                return -1;
            }
            state->pc = state->stack[--state->depth];
            break;

        case 0xFD:
            return gl_func_0003F05C(state);

        case 0xFE:
            return 1;

        case 0xFC:
            d = state->depth;
            state->stack[d] = state->pc;
            state->depth++;
            state->pc = seqPlayer->seqData + (u16)cmdArg;
            break;

        case 0xF8:
            d = state->depth;
            state->remLoopIters[d] = cmdArg;
            state->stack[state->depth] = state->pc;
            state->depth++;
            break;

        case 0xF7:
            state->remLoopIters[state->depth - 1]--;
            if (state->remLoopIters[state->depth - 1] != 0) {
                state->pc = state->stack[state->depth - 1];
            } else {
                state->depth--;
            }
            break;

        case 0xF6:
            state->depth--;
            break;

        case 0xF5:
        case 0xF9:
        case 0xFA:
        case 0xFB:
            if (cmd == 0xFA && state->value != 0) {
                break;
            }
            if (cmd == 0xF9 && state->value >= 0) {
                break;
            }
            if (cmd == 0xF5 && state->value < 0) {
                break;
            }
            state->pc = seqPlayer->seqData + (u16)cmdArg;
            break;

        case 0xF2:
        case 0xF3:
        case 0xF4:
            if (cmd == 0xF3 && state->value != 0) {
                break;
            }
            if (cmd == 0xF2 && state->value >= 0) {
                break;
            }
            state->pc += (s8)(cmdArg & 0xFF);
            break;
    }

    return 0;
}
