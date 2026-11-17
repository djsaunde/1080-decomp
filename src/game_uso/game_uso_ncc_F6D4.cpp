/* IDO 7.1 NCC (C++) donor for RiderAnim_stateTrickStart (0x210 / 132 words).
 *
 * Node-process / queue-advance handler, sibling of E35C: optional D6E4
 * pre-flush (cur->0x38 & 1), EE30(self, active->0x30, 1), B8518 index call
 * (ceil-div-by-64 of the node offset, two forms on obj->0xA58 & 0x20),
 * counter++ / F4 = F0 / F0 = 0, reset the counter when F8 == F4, F8 = F4,
 * E564(self), then the 0xA58 & 0x40 / cur->0x28 select into cur->0x20 and
 * the 077C44 + D5F8 (PMF by value) pair on cur->0x38 & 2, then D5BC.
 *
 * C++ / edg facts this body depends on (the cfe wrap capped at 95.4%):
 *  - edg emits the ceil-div (base + (n << 6)) - base addu base-first
 *    (cfe always put the shift operand first).
 *  - There is NO named prev local: the target's t0 / t1 are expression temps.
 *    The F0 -> F4 move is written through the members, and the F8 compare
 *    and store re-read F4. A named local colours v0 and shifts every later
 *    temp by one register.
 *  - The compare is F8 == F4 (gives bne t0,t1); F4 == F8 swaps them.
 *  - The pairs passed to D5F8 / D5BC are pointers to members (by value).
 * Spliced into game_uso.c.o via REPLACE_FUNC_BODY
 * (docs/MATCHING_WORKFLOW.md#game-uso-ncc-cplusplus-donor-recipe). Every
 * symbol is extern "C". */
typedef int s32; typedef unsigned short u16; typedef float f32; typedef double f64;
typedef short s16; typedef unsigned char u8; typedef unsigned int u32;
#include "../../include/game/rideranim.h" /* NCC has no -I include */
#include "../../include/uso_call.h"  /* NCC donors build without -I include */
#include "../../include/game/input.h"  /* NCC donors build without -I include */
#include "../../include/game/game_uso_data.h"
#ifdef PORT /* cfront PMF {s16 delta, s16 index, fn}, A without g++ polymorphism (port_cfront.h, plan 18.7) */
#include "port_cfront.h"
struct A { char pad[40]; char *vptr; /* cfront vptr */ };
typedef PortCfPmf PM;
#else
struct A { char pad[40]; virtual void m(); };
typedef void (A::*PM)();
#endif
extern "C" {
extern int *game_uso_D_807FFBC0;
extern char import_800201D0;
extern char game_uso_D_807FF4B0;
extern char game_uso_D_807FF4B8;
extern char game_uso_D_807FF4C0;
void RiderAnim_flipStance_0000D6E4(void *);
void RiderAnim_addTrickPoints_0000EE30(void *, int, int);
void import_000B8518(int, void *, int);
void RiderAnim_playSpinSound_0000E564(void *);
void game_uso_func_077C44(void *, int, int, int, int, int);
void RiderAnim_queueState_0000D5F8(void *, PM, int);
void RiderAnim_enterState_0000D5BC(void *, PM);
}
#define AT(T,b,o) (*(T *)((char *)(b) + (o)))
extern "C" void RiderAnim_stateTrickStart(char *s0) {
    TrickEntry *f4;
    if (RA(s0)->curTrick->flags & TRICK_FLAG_HALF_TURN) {
        RiderAnim_flipStance_0000D6E4(s0);
    }
    RiderAnim_addTrickPoints_0000EE30(s0, RA(s0)->queuedTrick->points, 1);
    if (RA(s0)->rider->flags & SNOWB_FLAG_20) {
        int base = (int)game_uso_D_807FFBC0;
        import_000B8518(*(int *)&INPUT_MGR(import_800201D0).dataMgr, RA(s0)->rider,
            ((base + (RA(s0)->queuedTrick->mirrorIndex << 6)) - base) / 64);
    } else {
        import_000B8518(*(int *)&INPUT_MGR(import_800201D0).dataMgr, RA(s0)->rider,
            ((int)RA(s0)->queuedTrick - (int)game_uso_D_807FFBC0) / 64);
    }
    RA(s0)->comboCount = RA(s0)->comboCount + 1;
    RA(s0)->curTrick = RA(s0)->queuedTrick;
    RA(s0)->queuedTrick = 0;
    if (RA(s0)->lastTrick == RA(s0)->curTrick) { RA(s0)->comboCount = 0; }
    RA(s0)->lastTrick = RA(s0)->curTrick;
    RiderAnim_playSpinSound_0000E564(s0);
    if ((RA(s0)->rider->flags & 0x40) && RA(s0)->curTrick->animSwitch != 0) {
        RA(s0)->curTrick->anim = RA(s0)->curTrick->animSwitch;
    } else {
        RA(s0)->curTrick->anim = RA(s0)->curTrick->animRegular;
    }
    f4 = RA(s0)->curTrick;
    if (f4->flags & TRICK_FLAG_GRAB) {
        XCALL(game_uso_func_077C44, AnimPlayer_start)(s0, f4->anim, 4, 2, 1, 1);
        RiderAnim_queueState_0000D5F8(s0, AT(PM, &game_uso_D_807FF4B0, kPmf_RiderAnim_stateTrickWindup), 2);
    } else {
        XCALL(game_uso_func_077C44, AnimPlayer_start)(s0, f4->anim, 4, 3, 1, 1);
        RiderAnim_queueState_0000D5F8(s0, AT(PM, &game_uso_D_807FF4B8, kPmf_RiderAnim_stateTrickRelease), 3);
    }
    RiderAnim_enterState_0000D5BC(s0, AT(PM, &game_uso_D_807FF4C0, kPmf_RiderAnim_stateTrickAir));
}
