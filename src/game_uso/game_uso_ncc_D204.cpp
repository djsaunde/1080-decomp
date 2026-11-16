#include "../../include/uso_call.h"  /* NCC donors build without -I include */
/* IDO 7.1 NCC (C++) donor for RiderAnim_update_0000D204 (0x214 / 133 words).
 *
 * game_uso was written in C++ and compiled by IDO 7.1's own C++ driver NCC
 * (edg front end). The `if (pm != null) (this->*pm)();` block below is an
 * edg-lowered cfront-layout pointer-to-member-function test + call (memberwise
 * 2-word compare loop through `at` with `addi`, then the delta/index/vptr
 * dispatch with the class's vptr at +40); cfe cannot emit it (the old C NM
 * wrap capped at 85.8%). Built by the explicit NCC rule in the Makefile
 * (no asm-processor, no GLOBAL_ASM) and spliced into game_uso.c.o via
 * REPLACE_FUNC_BODY; this unit is never linked on its own.
 * docs/IDO_CODEGEN.md#game-uso-is-cplusplus-ncc-pmf-compare-d204,
 * docs/MATCHING_WORKFLOW.md#game-uso-ncc-cplusplus-donor-recipe.
 *
 * Every symbol is extern "C" so names stay unmangled for the splice's reloc
 * import. Comparing against the extern null PMF at game_uso_D_807FF3B8+0xDC8
 * (not literal 0) makes edg reference the USO's null object instead of
 * emitting its own .data copy. NCC wants `0x104 != 0x74` (cfe wanted the
 * reverse) for the same `beq a1,t9`. */
extern "C" {
typedef int s32; typedef unsigned short u16; typedef float f32; typedef double f64;
typedef short s16; typedef unsigned char u8; typedef unsigned int u32;
#include "../../include/game/rideranim.h"
#include "../../include/game/input.h"
extern char import_80020098;
extern char game_uso_D_807FF68C;
extern char game_uso_D_807FF3B8;
extern char D_00000000;
void game_uso_func_07C0C0(int, char *, int);
void RiderAnim_selectAnim_0000D9CC(void *);
void game_uso_func_0000ECE8(void *);
void game_uso_func_076E78(int, int);
void game_uso_func_0775F8(void *);
}
struct A { char pad[40]; virtual void m(); };
typedef void (A::*PM)();
struct H : A {}; /* RiderAnim as the class its state PMFs bind to (vptr at +40) */
#define PMF(pair) (*(PM *)&(pair)) /* a Pair2 field read as the PMF it holds */
#define F(b,o) (*(float *)((char *)(b) + (o)))
extern "C" void RiderAnim_update_0000D204(H *a0) {
    H *a3 = a0;
    /* import_80020098 is GameFlow (include/game/input.h); on course 0 its dataMgr's hudFlags get
     * RTF_FLUSH | RTF_POINTS while endTrickFlag is set and the rider is past the RoData z threshold */
    if (INPUT_MGR(import_80020098).course == 0) {
        float th = F(&D_00000000, 0x1F4);
        if (RA(a0)->rider->dnode.vqs.trans[2] < th) {
            RA(a0)->endTrickFlag = 0;
        }
        if (RA(a0)->rider->dnode.vqs.trans[2] < th) goto clear_bits;
        if (RA(a3)->endTrickFlag == 0) goto set_bits;
    clear_bits:
        { s32 *flags = &INPUT_MGR(import_80020098).dataMgr->hudFlags; *flags &= ~(RTF_FLUSH | RTF_POINTS); }
        goto bits_done;
    set_bits:
        { s32 *flags = &INPUT_MGR(import_80020098).dataMgr->hudFlags; *flags |= RTF_FLUSH | RTF_POINTS; }
    bits_done:;
    }
    {
        SnowBoarder *v0 = RA(a3)->rider;
        if (v0->contactCount == 0 && v0->groundDist < 700.0f && v0->dnode.vel[1] < -100.0f) {
            XCALL(game_uso_func_07C0C0, pad_startRumble)((int)v0->input, &game_uso_D_807FF68C + 4252, 1);
        }
    }
    if (RA(a3)->trickPhase != 0) RiderAnim_selectAnim_0000D9CC(a3);
    if (PMF(RA(a3)->state) != *(PM *)(&game_uso_D_807FF3B8 + 0xDC8)) (a3->*PMF(RA(a3)->state))(); /* run the state */
    XCALL(game_uso_func_0000ECE8, RiderAnim_updateSlowdown_0000ECEC)(a3);
    if (RA(a3)->player.anim != 0) {
        XCALL(game_uso_func_076E78, AnimDataMgr_lookup)(RA(a3)->player.mgr, RA(a3)->player.anim);
        if (RA(a3)->loadedAnim != RA(a3)->player.anim) RA(a3)->loadedAnim = RA(a3)->player.anim;
    }
    XCALL(game_uso_func_0775F8, AnimPlayer_advance)(a3);
}
