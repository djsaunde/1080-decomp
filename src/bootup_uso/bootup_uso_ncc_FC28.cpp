/* IDO 7.1 NCC (C++) -O0 donor for QuitProcess_ctor0 QuitProcess_ctor PauseMenu_create: the NM C body of
 * bootup_uso_tail1_bot.c compiled as C++ (bootup_uso is C++). cfe -O0 left a dead
 * closing-brace branch the shipped code lacks; edg's -O0 ucode has none.
 * Spliced into bootup_uso_tail1_bot.c.o via REPLACE_FUNC_BODY.
 * agent-g run 543: the Node vptr / name stores spelled through include/gdl/node.h (NODE(p)->vptr / ->name; relative
 * include, its offset asserts are C-only). Same bytes.
 * agent-g run 546: QuitProcess unk44 / menu (local view) and PauseMenu on game/guimenu.h GuiMenu (locked, the two
 * row-colour quads in unk4C). Same bytes.
 * agent-g run 558: the quads are GuiMenu.selColour / rowColour (agent-h run 556); s walks each as an f32[4].
 * Same bytes. */
typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;
typedef signed char s8; typedef signed short s16; typedef signed int s32; typedef float f32; typedef double f64;
#include "../../include/gdl/node.h"
#include "../../include/game/guimenu.h" /* PauseMenu = GuiMenu (0x304) */
#include "../../include/uso_call.h"  /* NCC donors build without -I include */
/* QuitProcess (0x50): the in-place Node (0x2C) -> Process (0x50) cascade, then QuitProcess_vtbl. No shared
 * Process header yet; this view names the two slots QuitProcess_ctor clears. +0x48 is the Process child-menu
 * slot (RaceProcess.menu, MenuProcess.menu: a GuiMenu); +0x44 is the word before it. */
typedef struct QuitProcess {
    Node node;                  /* 0x00 */
    char unk2C[0x44 - 0x2C];
    s32 unk44;                  /* 0x44: 0 (QuitProcess_ctor) */
    struct GuiMenu *menu;       /* 0x48: the Process child menu; 0 (QuitProcess_ctor) */
    s32 unk4C;
} QuitProcess;
typedef char QuitProcess_size_check[sizeof(QuitProcess) == 0x50 ? 1 : -1];
extern "C" {
/* bootup_uso 0xFC28..0x100F0 region, split off bootup_uso_tail1.c on
 * 2026-05-28 so that Process_setWindowFlag8_0000FBCC could move to a -O0 sub-file
 * (bootup_uso_o0_FBCC.c) and land as a byte-exact match. These four
 * functions stay INCLUDE_ASM NM-wraps (documented -O0 alloc-cascade /
 * dead-BB / t-reg caps — see per-function notes). TRUNCATE_TEXT clips
 * this file's .text to FEE8's true end (0x4C8) so o0_100F0.c.o abuts at
 * bootup offset 0x100F0. */

extern int func_00000000(...);
extern struct { char _p2C[0x2C]; int f2C; char _p38[0x8]; int f38; } stream_readS32_00000008; /* USO-base reloc; struct-typed for $at-fused member stores */
#ifdef PORT /* plan 17.8: on the port the code-symbol token is a function in .text; uso-data-sites.py gives the data: f2C / f38 are GameFlow + 0x34 / + 0x40 */
#include "port_game.h"
struct PortFC28Gf { char _p2C[0x2C]; int f2C; char _p38[0x8]; int f38; };
#define stream_readS32_00000008 (*(PortFC28Gf *)(PORT_GAMEFLOW + 8))
#endif
extern char D_00000000;
#include "../../include/gdl/gl_data.h"  /* object aliases (NCC donors build without -I include) */
extern char sStr_Node_C58C;
extern char sStr_Quit_Process_C594;
extern char sStr_Node_C5A4;
extern char sStr_Quit_Process_C5AC;
extern char sStr_Node_C5BC;
extern char sStr_PauseMenu_C5C4;
extern int D_bootup_scratch0;         /* int-typed base-0 alias of D_00000000; $at-fused scratch store/reload */

/* QuitProcess_ctor0: 73-insn (0x124) constructor + init chain, returns a0.
 *
 * 2026-07-10: 56.23% -> 97.06%. 2026-09-05: -> 73/75 insns byte-exact;
 * the ONLY residual is the documented -O0 return-value dead-double-b
 * toolchain gap (see FD4C below for the full lever writeup — FC28 is the
 * smaller sibling, same recipe).
 * Levers: (1) SHORT-CIRCUIT alloc-fallback `if (x != 0 || (x = alloc(N))
 * != 0) { arm }` (docs/IDO_CODEGEN.md -O0 alloc-cascade lever #2). The
 * 2026-07-10 body used nested `if(x==0){x=alloc;} if(x!=0){...}` and
 * mis-reported "byte-exact except the double-b": that shape re-tests x
 * on the no-alloc path too, so the target's `bnez t6,.L64` (straight to
 * `lw s0`) came out as `bnez t6,.L58` (into the `lw t7; beqz t7` re-test)
 * — 3 wrong branch targets (t6/s0/s1 bnez). `||` puts the null-test on
 * the alloc path only: `bnez x,.arm; alloc; beqz x,.skip` exactly. A
 * `goto` at -O0 would emit a 2-insn branch-over (bnez;b), so no gotos;
 * the outer `||`'s false-exit IS the a0-alloc-fail `beqz t7,end`.
 * (2) `register void*
 * s0,s1` (callee-saved cascade temps, no home spill). (3) TAIL recycles
 * the now-dead s0/s1: `s0=a0; s1=&sStr_Quit_Process_C594; *(int*)(s0+0xC)=(int)s1;`
 * reproduces `lw s0; lui s1; sw s1,0xC(s0)`, and `s0=a0; f(scratch,s0)`
 * the final-call arg. (4) D_bootup_scratch0 (int-typed base-0 alias of
 * D_00000000, undefined_syms) $at-fuses the *&D store/reload (lui/sw)
 * vs the address-of-cast's lui/addiu/sw. RESIDUAL = the dead second
 * `b epilogue` only (value-return -O0 cap, NOT C-fixable; re-probed
 * 2026-09-05: switch/if(1)/for(;;)/goto-end/same-line-brace/ternary tails
 * all keep or worsen it). NM stays. */
void *QuitProcess_ctor0(void *a0) {
    register void *s0, *s1;

    if (a0 != 0 || (a0 = (void*)FCALL(mem_new)(0x50)) != 0) {
        s0 = a0;
        if (s0 != 0 || (s0 = (void*)FCALL(mem_new)(0x50)) != 0) {
            s1 = s0;
            if (s1 != 0 || (s1 = (void*)FCALL(mem_new)(0x2C)) != 0) {
                FCALL(Node_ctor)(s1, &sStr_Node_C58C);
                NODE(s1)->vptr = (char *)&XDATA(Process_vtbl, port_bootup_data, 0xC338);
            }
            NODE(s0)->vptr = (char *)&XDATA(Process_vtbl_C3A0, port_bootup_data, 0xC3A0);
        }
        NODE(a0)->vptr = (char *)&XDATA(QuitProcess_vtbl, port_bootup_data, 0xC2B8);
        s0 = a0;
        s1 = (void*)&sStr_Quit_Process_C594;
        NODE(s0)->name = (const char *)s1;
        D_bootup_scratch0 = FCALL(game_createProcessWindow)(a0, 0);
        s0 = a0;
        FCALL(node_linkBack_0004E888)(D_bootup_scratch0, s0);
    }
    return a0;
}

/* QuitProcess_ctor: 85-insn (0x154) alloc-chain + multi-init helper, returns a0.
 * Larger sibling of QuitProcess_ctor0 (+ a1/a2 global stashes, 0x44/0x48 zeroing).
 *
 * 2026-07-10: 58.18% -> 97.47%. 2026-09-05: -> 85/87 insns byte-exact; the
 * ONLY residual is the documented -O0 return-value dead-double-b toolchain
 * gap: our cc emits `lw v0; b epi; nop; b epi(DEAD); nop`, the target has a
 * single b — value-return -O0 cap,
 * docs/IDO_CODEGEN.md#feedback-ido-o0-return-value-dead-double-b, NOT C-fixable.
 *
 * LEVER KIT (the reusable part — cracked all diffs except the double-b):
 * (1) SHORT-CIRCUIT ALLOC-FALLBACK: `if (x != 0 || (x = alloc(N)) != 0) { arm }`
 *     (docs/IDO_CODEGEN.md -O0 alloc-cascade lever #2). IDO -O0 renders
 *     `if(E) goto L;` as a 2-insn branch-over (`b!E over; b L`), so no gotos;
 *     but nested `if(x==0){x=alloc;} if(x!=0){arm}` (the 2026-07-10 body,
 *     which mis-reported "byte-exact except the double-b") re-tests x on the
 *     no-alloc path, so each cascade `bnez x` landed on the `lw t;beqz t`
 *     re-test instead of skipping past it (3 wrong bnez targets: t6/s0/s1).
 *     `||` puts the null-test on the alloc path only: `bnez x,.arm; alloc;
 *     beqz x,.skip` exactly; the outer `||` false-exit IS the a0-alloc-fail
 *     `beqz t7,end`.
 * (2) `register void *s0,s1;` = the callee-saved cascade temps (or s0,v0 / or
 *     s1,s0 — no home spill; plain locals would stack-home and lose the shape).
 * (3) TAIL RE-USE of the now-dead register vars as scratch (the target recycles
 *     $s0/$s1 once the cascade vars die): `s0=a0; s1=&sStr_Quit_Process_C5AC;
 *     *(int*)(s0+0xC)=(int)s1;` -> `lw s0; lui s1; sw s1,0xC(s0)`; and
 *     `s0=a0; f(scratch,s0)` for the last call's arg.
 * (4) struct-typed stream_readS32_00000008 (member .f2C/.f38) $at-fuses the global stashes
 *     (lui at; sw ...(at)) vs the (char*)&sym+off cast's lui/addiu/sw.
 * (5) D_bootup_scratch0 = int-typed base-0 alias of D_00000000 (undefined_syms)
 *     $at-fuses the *&D scratch store + reload.
 * RESIDUAL = the dead double-b only. NM stays. */
void *QuitProcess_ctor(void *a0, int a1, int a2) {
    register void *s0, *s1;

    if (a0 != 0 || (a0 = (void*)FCALL(mem_new)(0x50)) != 0) {
        s0 = a0;
        if (s0 != 0 || (s0 = (void*)FCALL(mem_new)(0x50)) != 0) {
            s1 = s0;
            if (s1 != 0 || (s1 = (void*)FCALL(mem_new)(0x2C)) != 0) {
                FCALL(Node_ctor)(s1, &sStr_Node_C5A4);
                NODE(s1)->vptr = (char *)&XDATA(Process_vtbl, port_bootup_data, 0xC338);
            }
            NODE(s0)->vptr = (char *)&XDATA(Process_vtbl_C3A0, port_bootup_data, 0xC3A0);
        }
        NODE(a0)->vptr = (char *)&XDATA(QuitProcess_vtbl, port_bootup_data, 0xC2B8);
        s0 = a0;
        s1 = (void*)&sStr_Quit_Process_C5AC;
        NODE(s0)->name = (const char *)s1;
        stream_readS32_00000008.f2C = a1;
        stream_readS32_00000008.f38 = a2;
        ((QuitProcess *)a0)->unk44 = 0;
        ((QuitProcess *)a0)->menu = 0;
        D_bootup_scratch0 = FCALL(game_createProcessWindow)(a0, 0);
        s0 = a0;
        FCALL(node_linkBack_0004E888)(D_bootup_scratch0, s0);
    }
    return a0;
}

/* QuitProcess_onQuit: 18-insn indirect-call wrapper:
 *   p = a0->field_28; (*p->field_64)(a0 + (short)p->field_60).
 * MATCHED 2026-07-10 (w50 island sweep, 18/18): the long-documented
 * "double-reload residual cap" (19 vs 18 insns) was the cfe DAG-share
 * lever from Process_update — single expression statement with the
 * register-p assignment EMBEDDED IN THE ARG:
 *   (*(void(**)(...))(p + 0x64))(*(short*)((p = *(char**)(a0+0x28)) + 0x60) + a0);
 * The arg's `+ a0` DAG-shares the assignment chain's a0 home reload
 * (t6) -> `addu a0,t7,t6` with no second lw; fn-ptr load lands in $t9.
 * Required flipping this file to -O0 (all other members stay INCLUDE_ASM
 * so the flag only affects this compiled body). Old exhaustive-variant
 * notes retired: all four 2026-05-08 variants were two-statement forms. */
void QuitProcess_onQuit(char *a0) {
    register char *p;
#ifdef PORT
    p = *(char **)&NODE(a0)->vptr; /* the callee is read through p: gcc evaluates the function designator before the
     * arguments that assign it (IDO the other way round; plan 18.9) */
#endif
    (*(void(**)(...))&VT_SLOT(p, 11).fn)(VT_SLOT((p = *(char **)&NODE(a0)->vptr), 11).delta + a0);
}

/* PauseMenu_create - verified structural decode (0x208, 130 insns,
 * get-or-create constructor + normalized-colour init). bootup_uso
 * constructor family (cf. item_createBal / FloatPosParms_ctor).
 *   void *PauseMenu_create(void *a0) {
 *       o = a0 ? a0 : func_00000000(0x304);
 *       if (!o) return 0;
 *       s = func_00000000(0x2C);
 *       if (!s) return ...;
 *       func_00000000(s, &sStr_Node_C5BC);            // init sub
 *       s->0x28 = &D_a;                            // descriptor
 *       o->0x28 = &D_b;                            // descriptor
 *       o->0xC  = &sStr_PauseMenu_C5C4;                     // type/vtable
 *       f32 *c = (f32*)((char*)o + 0x4C);
 *       c[0] = 0x77 / 255.0f;                      // 119/255
 *       c[1] = 0x75 / 255.0f;                      // 117/255
 *       c[2] = 0xE5 / 255.0f;                      // 229/255
 *       c[3] = ... / 255.0f;                       // (RGBA-ish)
 *       ... (further normalized float fields)
 *   }
 * Struct-typing reference: o = 0x304-byte object; o->0x28 (40)
 * descriptor (&D), o->0xC (12) type/vtable ptr (&sStr_PauseMenu_C5C4).
 * s = 0x2C-byte sub-object (s->0x28 descriptor, init datum
 * sStr_Node_C5BC). o+0x4C (76) = a float array initialized from u8
 * literals divided by 255.0f (0x437F0000) - a 0..1 normalized
 * colour / parameter vector (the int-byte / 255.0 idiom). Caps
 * <80: get-or-create + alloc-cascade (~3 reloc) + repeated
 * cvt.s.w + div.s by 255.0 chain + &D descriptors. Full body
 * INCLUDE_ASM-preserved (.s = source of truth). INCLUDE_ASM (no
 * episode; tautology-trap rule). */
/* 2026-09-04 REBUILD 43.3 -> cap (128/130 words, 2 = the -O0 value-return
 * dead-double-b toolchain gap shared with FC28/FD4C above; everything else
 * byte-exact incl. relocs). Levers: fused-|| get-or-create cascades (outer
 * on the a0 home, inner on register s -> s0); second register d -> s1 for
 * the 0xC descriptor store; s re-based to a0+0x4C / a0+0x5C for the two
 * colour quads; and the /255 channels spelled `(float)(0, N) / 255.0f` --
 * the COMMA expression is the only spelling found that stops cfe folding
 * an int/float constant quotient at -O0 (li N; mtc1; cvt.s.w; lui 0x437f;
 * div.s, incl. the 255/255 = 1.0 one). All plain casts/pointer casts/
 * sizeof/char literals fold to a .rodata lwc1. */
void *PauseMenu_create(char *a0) {
    register char *s;
    register char *d;
    if (a0 != 0 || (a0 = (char *)FCALL(mem_new)(0x304)) != 0) {
        if ((s = a0) != 0 || (s = (char *)FCALL(mem_new)(0x2C)) != 0) {
            FCALL(Node_ctor)(s, &sStr_Node_C5BC);
            NODE(s)->vptr = (char *)&XDATA(Process_vtbl, port_bootup_data, 0xC338);
        }
        NODE(a0)->vptr = (char *)&XDATA(PauseMenu_vtbl, port_bootup_data, 0xC47C);
        s = a0;
        d = &sStr_PauseMenu_C5C4;
        NODE(s)->name = (const char *)d;
        s = (char *)GUIMENU(a0)->selColour;   /* 0x4C: the cursor row's colour (PauseMenu_display's q4C) */
        ((f32 *)s)[0] = (float)(0, 119) / 255.0f;
        ((f32 *)s)[1] = (float)(0, 117) / 255.0f;
        ((f32 *)s)[2] = (float)(0, 229) / 255.0f;
        ((f32 *)s)[3] = (float)(0, 255) / 255.0f;
        s = (char *)GUIMENU(a0)->rowColour;   /* 0x5C: the other rows' colour (PauseMenu_display's q5C) */
        ((f32 *)s)[0] = (float)(0, 192) / 255.0f;
        ((f32 *)s)[1] = (float)(0, 194) / 255.0f;
        ((f32 *)s)[2] = (float)(0, 229) / 255.0f;
        ((f32 *)s)[3] = (float)(0, 255) / 255.0f;
        GUIMENU(a0)->locked = 0;
        FCALL(gui_clearRows_00010A9C)(a0);
    }
    return a0;
}
}
