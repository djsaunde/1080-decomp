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
typedef int s32;
typedef unsigned int u32;
#include "../../include/game/tricktable.h" /* NCC has no -I include */
#include "../../include/uso_call.h"  /* NCC donors build without -I include */
struct A { char pad[40]; virtual void m(); };
typedef void (A::*PM)();
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
void RiderAnim_setState_0000D5F8(void *, PM, int);
void RiderAnim_setNextState_0000D5BC(void *, PM);
}
#define AT(T,b,o) (*(T *)((char *)(b) + (o)))
#define I(b,o) AT(int,b,o)
#define P(b,o) AT(char *,b,o)
/* RiderAnim members (include/game/rideranim.h; the donor keeps byte-offset views) */
#define QUEUED_TRICK(s) AT(TrickEntry *, s, 0xF0)   /* RiderAnim.queuedTrick */
#define CUR_TRICK(s)    AT(TrickEntry *, s, 0xF4)   /* RiderAnim.curTrick */
#define LAST_TRICK(s)   AT(TrickEntry *, s, 0xF8)   /* RiderAnim.lastTrick */
#define COMBO_COUNT(s)  I(s, 0x100)                 /* RiderAnim.comboCount */
#define RIDER(s)        P(s, 0xB4)                  /* RiderAnim.rider */
#define RIDER_FLAGS(s)  I(RIDER(s), 0xA58)          /* SnowBoarder.flags */
extern "C" void RiderAnim_stateTrickStart(char *s0) {
    TrickEntry *f4;
    if (CUR_TRICK(s0)->flags & TRICK_FLAG_HALF_TURN) {
        RiderAnim_flipStance_0000D6E4(s0);
    }
    RiderAnim_addTrickPoints_0000EE30(s0, QUEUED_TRICK(s0)->points, 1);
    if (RIDER_FLAGS(s0) & 0x20) {
        int base = (int)game_uso_D_807FFBC0;
        import_000B8518(I(&import_800201D0, 0x138), RIDER(s0),
            ((base + (QUEUED_TRICK(s0)->mirrorIndex << 6)) - base) / 64);
    } else {
        import_000B8518(I(&import_800201D0, 0x138), RIDER(s0),
            ((int)QUEUED_TRICK(s0) - (int)game_uso_D_807FFBC0) / 64);
    }
    COMBO_COUNT(s0) = COMBO_COUNT(s0) + 1;
    CUR_TRICK(s0) = QUEUED_TRICK(s0);
    QUEUED_TRICK(s0) = 0;
    if (LAST_TRICK(s0) == CUR_TRICK(s0)) { COMBO_COUNT(s0) = 0; }
    LAST_TRICK(s0) = CUR_TRICK(s0);
    RiderAnim_playSpinSound_0000E564(s0);
    if ((RIDER_FLAGS(s0) & 0x40) && CUR_TRICK(s0)->animSwitch != 0) {
        CUR_TRICK(s0)->anim = CUR_TRICK(s0)->animSwitch;
    } else {
        CUR_TRICK(s0)->anim = CUR_TRICK(s0)->animRegular;
    }
    f4 = CUR_TRICK(s0);
    if (f4->flags & TRICK_FLAG_GRAB) {
        XCALL(game_uso_func_077C44, AnimPlayer_start)(s0, f4->anim, 4, 2, 1, 1);
        RiderAnim_setState_0000D5F8(s0, AT(PM, &game_uso_D_807FF4B0, 0xEC0), 2);
    } else {
        XCALL(game_uso_func_077C44, AnimPlayer_start)(s0, f4->anim, 4, 3, 1, 1);
        RiderAnim_setState_0000D5F8(s0, AT(PM, &game_uso_D_807FF4B8, 0xEC8), 3);
    }
    RiderAnim_setNextState_0000D5BC(s0, AT(PM, &game_uso_D_807FF4C0, 0xED0));
}
