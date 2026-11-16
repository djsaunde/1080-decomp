/* IDO 7.1 -O2 donor for game_setupMode -- bootup_uso subsystem init + 12-way
 * command dispatch (0x1EC / 123 words; word-exact standalone apart from the
 * reloc fields).
 *
 * WIRED via REPLACE_FUNC_BODY donor splice into bootup_uso.c.o (the game_libs
 * 7344 / 52CD4 recipe, docs/MATCHING_WORKFLOW.md#post0b-jumptable-donor-52cd4):
 * the switch's donor-local .rodata jumptable reloc is renamed
 * game_setupMode_rodata and pinned in undefined_syms_auto.txt to 0x4F4, the
 * table bootup_uso ships in its RoData (module 0xD9FE28; file offset 0x4F8 =
 * imm + 4, the word at 0x4F4 is the preceding 0.04f literal). Table words
 * (cases 1..12): 2F10 2E98 2ED4 2EAC 2EE8 2EFC 2F74 2F74 2F38 2F4C 2F60 2EC0,
 * so the arms lay out in SOURCE order 2, 4, 12, 3, 5, 6, 1, 9, 10, 11 and
 * cases 7/8 share the default (&sStr_unknown_GAME_MODE_7408). The 2026-07-10 NM decode had the
 * table one entry late (3,5,1,4,...); extract-uso-jumptable.py drops the
 * LAST entry here (0x2EC0), not the first.
 *
 * Load-bearing shapes:
 *  - distinct base-0 globals (docs/IDO_CODEGEN.md#lui-count-placeholder-
 *    sweep-44de4): the created object is stored through D_2DA4_obj and
 *    re-read at once through D_2DA4_obj_rd (one symbol forwards the stored
 *    v0 instead of the target's `lui a1; lw a1,0(a1)`); the config word is
 *    read through gPolyGeomMode_3EEC8_2DA4_cfg+4 and stored through D_00000000+4 (one symbol
 *    CSEs the lui).
 *  - calls whose result is unused go through the void alias
 *    func_00000000_2DA4_v: with the int-returning blank, the dead $v0 def
 *    pushes the named config value `v` to v1 (target v0).
 *  - case 1: `a1 = obj; if (1) { a1 += 0x18; } *(int *)a1 |= 4` reusing the
 *    a1 variable gives `lw t0,24(a1); addiu a1,a1,24; ... sw t1,0(a1)`.
 *  - `a1` declared before `v1` puts their spill homes at sp+0x1C / sp+0x18.
 *  - `v | 0x80000 | 0x2000 | 0x20000` stays unfolded (lui/or, ori, lui/or). */
#include "common.h"
#include "gdl/gl_data.h"
#include "gdl/node.h"
#include "game/course.h"
#include "uso_call.h"

extern int func_00000000();
extern void func_00000000_2DA4_v();
extern char sStr_ImgFileMgr_73EC, sStr_rom_tricks_bin_73F8, sStr_unknown_GAME_MODE_7408;
extern int D_2DA4_obj, D_2DA4_obj_rd;
extern char D_00000000;
extern int gPolyGeomMode_3EEC8_2DA4_cfg[]; /* gPolyGeomMode_3EEC8 read alias: mode words [0], [1] */

void game_setupMode(char *arg0, s32 arg1, s32 arg2) {
    char *a1;
    char *v1;
    int v;
    XCALL(func_00000000_2DA4_v, memtrack_begin)(&sStr_ImgFileMgr_73EC, 0);
    XCALL(func_00000000_2DA4_v, memtrack_end)();
    D_2DA4_obj = FCALL(TrickTable_ctor_0000C2D4)(0, &sStr_rom_tricks_bin_73F8);
    v1 = (char *)WORLDREG(arg0)->bgRoot;
    a1 = (char *)D_2DA4_obj_rd;
    XCALL(func_00000000_2DA4_v, list_append)(v1 + 0x10, a1);
    if (NODE(a1)->parent != 0) {
        NODE(a1)->unk04 = 1;
    }
    NODE(a1)->parent = (Node *)v1;
    XCALL(func_00000000_2DA4_v, AnimDataMgr_releaseFrames)(D_2DA4_obj);
    v = gPolyGeomMode_3EEC8_2DA4_cfg[1]; /* [1]: the geometry mode word */
    v = v | 0x80000 | 0x2000 | 0x20000;
    ((int *)&gPolyGeomMode_3EEC8)[1] = v;
    switch (arg1) {
    case 2: XCALL(func_00000000_2DA4_v, game_setupSolo)(arg0, arg2); break;
    case 4: XCALL(func_00000000_2DA4_v, game_setupReplay)(arg0, arg2); break;
    case 12: XCALL(func_00000000_2DA4_v, game_setupReplay)(arg0, arg2); break;
    case 3: XCALL(func_00000000_2DA4_v, game_setupVsGhost)(arg0, arg2); break;
    case 5: XCALL(func_00000000_2DA4_v, game_setupVsCpu)(arg0, arg2); break;
    case 6: XCALL(func_00000000_2DA4_v, game_setupVs2P)(arg0, arg2); break;
    case 1:
        XCALL(func_00000000_2DA4_v, game_setupEmpty)(arg0, 0);
        a1 = (char *)D_2DA4_obj; if (1) { a1 += 0x18; } *(int *)a1 = *(int *)a1 | 4;
        break;
    case 9: XCALL(func_00000000_2DA4_v, game_setupDemoSolo)(arg0, arg2); break;
    case 10: XCALL(func_00000000_2DA4_v, game_setupDemoRace)(arg0, arg2); break;
    case 11: XCALL(func_00000000_2DA4_v, game_setupDemoSplit)(arg0, arg2); break;
    default:
        XCALL(func_00000000_2DA4_v, sys_fatal)(&sStr_unknown_GAME_MODE_7408);
        break;
    }
}
