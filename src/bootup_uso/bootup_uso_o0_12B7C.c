#include "common.h"
#include "gdl/gl_data.h"
#include "game/input.h"
#include "gdl/backup.h"

/* Run 10 members (now covers 0x12818..0x12BF8). */

extern int func_00000000();
extern int D_00000000;
extern int D_A0000200;

/* settings_set8_00012818: 99.73% NON_MATCHING. Sole diff (0x5C/0x64): the
 * `if (D_A0000200 == 0xAC290000)` value-load. IDO -O0 emits
 *   lui $t5,%hi; lw $t5,%lo($t5); bne $t5,$at   (REUSES the %hi base $t5)
 * but the target uses a FRESH dest:
 *   lui $t5,%hi; lw $t6,%lo($t5); bne $t6,$at
 * Pure -O0 value-load reg-reuse, NOT C-reachable here. Tried:
 *  - operand swap (0xAC29... == D_A0000200): IDO canonicalizes, no change.
 *  - struct member at offset 0 (D_A0000200.v): same reuse.
 *  - array form (D_A0000200[0]): DOES give fresh $t6, but materializes the
 *    full base (lui+addiu+lw) -> +1 insn, wrong size (0x98 vs 0x94).
 * No same-size C produces fresh-dest-with-folded-%lo, so this is also
 * permuter-immune (can't flip the reg without adding an instruction).
 * (Was an INSN_PATCH target; INSN_PATCH removed 2026-05-23 as match-faking.)
 * 2026-06-09 EXTENDED NEGATIVE (do not re-try): volatile D_A0000200 at 7.1
 * AND 5.3 (reuse persists); IDO 5.3 -O0 plain (identical 2 diffs -- the
 * value-load reuse is version-invariant); chaining the unused-assignment
 * into the condition via && (+3 insns, worse) or comma operator (same 2
 * diffs -- the -O0 temp counter resets per value, not per statement).
 * Every compiler-version/qualifier/expression-chaining axis now exhausted.
 * Genuine -O0 codegen cap. */
/* 2026-06-11 permuter CONCLUDED: base 20 (1 true diff) flat through
 * 551k iterations -- allocator-internal (-O0 temp allocator class).
 * 99.73 stands.
 * 2026-06-11 wave-2: two more axes dead — `D_A0000000[0x80]` zero-lo array
 * base gives fresh $t6 + folded 0x200 offset but -O0 keeps the `addiu
 * base,base,0` (+1 insn); switch-form adds a dispatch beq block. */
/* 2026-07-15 (agent-g) CAP RETRACTED — EXACT via LITERAL cast-pointer
 * deref `*(int*)0xA0000200`: with a CONSTANT address (not the extern
 * symbol) -O0 gives the address its own temp (lui t5,0xA000) and the
 * value a FRESH temp (lw t6,0x200(t5)) with the low16 folded into the
 * load offset — the exact target words 3C0DA000/8DAE0200, reloc-free.
 * All prior negatives had probed only SYMBOL spellings. objdiff shows
 * 99.73 as an artifact (expected .o carries %hi/%lo relocs with zeroed
 * fields); the linked ROM is byte-identical (cmp clean). */
void settings_set8_00012818(char *a0, char *a1) {
    int i;
    register int *unused;

    i = 0;
    do {
        (*(char**)(a0 + 0x154))[i + 6] = a1[i];
    } while (++i < 8);

    unused = &gSysLib;

    if (*(int*)0xA0000200 == 0xAC290000) {
        func_00000000(a0);
    }
}

/* The replay recorder (docs/1080_REPLAY.md). `mgr` is the InputMgr, the pads are
 * InputPad objects; func_00000000 here is pad_startPlay (pad, frames, len) or
 * pad_startRecord (pad, frames, capacity, &lenOut) (game_libs 0x67A10 / 0x679BC) and,
 * in beginRecord2P, the allocator. recordMask 0xFFFBFFFF clears one held bit while
 * recording. */

/* time trial against the saved ghost: `ghost` plays the session's ghost, `pad`
 * records player 1 into replayData[0] (the preallocated recordBuf). */
void replay_beginRecordWithGhost(InputMgr *mgr, InputPad *pad, InputPad *ghost) {
    if (ghost != 0) {
        ghost->recordMask = 0xFFFBFFFF;
        func_00000000(ghost, mgr->session->ghost->data, mgr->session->ghost->count);
    }
    if (pad != 0) {
        if (mgr->replayData[0] == 0) {
            mgr->replayData[0] = mgr->recordBuf;
        }
        mgr->recCourse = mgr->course;
        mgr->recRider = mgr->playerRider;
        mgr->recFlags = mgr->playerFlags;
        mgr->recBoard = mgr->playerBoard;
        pad->recordMask = 0xFFFBFFFF;
        func_00000000(pad, mgr->replayData[0], mgr->recordCap, &mgr->replayLen[0]);
        mgr->unk184 = 0;
    }
}

/* two players: player 2 records into replayData[1], allocated on first use. */
void replay_beginRecord2P(InputMgr *mgr, InputPad *pad1, InputPad *pad2) {
    if (pad1 != 0) {
        if (mgr->replayData[0] == 0) {
            mgr->replayData[0] = mgr->recordBuf;
        }
        mgr->recCourse = mgr->course;
        mgr->recRider = mgr->playerRider;
        mgr->recFlags = mgr->playerFlags;
        mgr->recBoard = mgr->playerBoard;
        pad1->recordMask = 0xFFFBFFFF;
        func_00000000(pad1, mgr->replayData[0], mgr->recordCap, &mgr->replayLen[0]);
        mgr->unk184 = 0;
    }
    if (pad2 != 0) {
        if (mgr->replayData[1] == 0) {
            mgr->replayData[1] = func_00000000(mgr->recordCap << 2);
        }
        pad2->recordMask = 0xFFFBFFFF;
        func_00000000(pad2, mgr->replayData[1], mgr->recordCap, &mgr->replayLen[1]);
        mgr->unk184 = 0;
    }
}

void replay_beginRecord1P(InputMgr *mgr, InputPad *pad, int unused) {
    if (pad != 0) {
        if (mgr->replayData[0] == 0) {
            mgr->replayData[0] = mgr->recordBuf;
        }
        mgr->recCourse = mgr->course;
        mgr->recRider = mgr->playerRider;
        mgr->recFlags = mgr->playerFlags;
        mgr->recBoard = mgr->playerBoard;
        pad->recordMask = 0xFFFBFFFF;
        func_00000000(pad, mgr->replayData[0], mgr->recordCap, &mgr->replayLen[0]);
        mgr->unk184 = 0;
    }
}

/* replay the last recording on `pad` and/or the saved ghost on `ghost`. */
void replay_beginPlayback(InputMgr *mgr, InputPad *pad, InputPad *ghost) {
    if (pad != 0) {
        func_00000000(pad, mgr->replayData[0], mgr->replayLen[0]);
    }
    if (ghost != 0) {
        func_00000000(ghost, mgr->session->ghost->data, mgr->session->ghost->count);
    }
}
