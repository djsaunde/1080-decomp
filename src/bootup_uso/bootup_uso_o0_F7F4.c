#include "common.h"
#include "gdl/node.h"
#include "uso_call.h"

/* -O0 run: 0xF6C4..0xFBCB. These functions compile at -O0, not -O2.
 * See project_1080_bootup_uso_o0_runs.md + feedback_ido_o0_empty_stub.md. */

extern int func_00000000();
/* UNNAMED: data-base D_00000030 = 0x30 is one pinned value for two Bss objects (sym324 +0x30 in game_create3DWindow / game_createProcessWindow, sym326 +0x30 in Process_exit); a pure rename would merge them, it needs a per-site split. */
extern void *D_00000030;
extern char D_00000000;

/* Process_display: -O0 init+dispatch wrapper (sibling of gui_initRows_000100F0).
 * RE-MATCHED 2026-06-10 (previous body was a silent false match: it passed
 * NULL where the target passes &D_00000000 — lui/addiu vs move a0,zero —
 * and used a separate p-statement, leaving 8 wrong words at same length).
 * Match keys: (1) first call arg is &D_00000000 (reloc-bearing %hi/%lo pair);
 * (2) `register char *p` in $s0 with the p-assignment EMBEDDED in the call's
 * argument expression so one $t6 a0-load serves the p-load and the addu
 * (same lever as gui_initRows_000100F0 in bootup_uso_o0_100F0.c). */
void Process_display(char *a0) {
    register char *p;
    FCALL(gui_beginFrame_000016A4)(&D_00000000);
    VT_SLOT(p, 13).fn(
        VT_SLOT(p = NODE(a0)->vptr, 13).delta + (int)a0);
    FCALL(Node_display)(a0);
}

void Process_exit(int a0) {
    D_00000030 = (void*)func_00000000; /* UNNAMED: no-reloc not a call, so no jal: stores &GameFlow_restart_000116C8 (hi/lo TextReloc T+0xF7D4 / F7D8); CALL names only R_MIPS_26 jals */
}

void Process_onQuit(int a0) {}

void Process_stub_0000F808(int a0) {}
