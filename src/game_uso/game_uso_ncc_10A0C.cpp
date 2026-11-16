/* IDO 7.1 NCC (C++) donor for RiderAnim_stateBrakeDeep (0xBC / 47 words).
 *
 * EE84-family state handler, near-twin of 1094C: DB78(self, 0x20004,
 * 0x20005, self->0xB4->0x970, 0, 1); if self->0xB4->0x990 == 0 call
 * D5BC(self, PMF at D_807FF438+0xE48); if self->0xB4->0x9CC == 0 call
 * D5BC(self, PMF at D_807FF460+0xE70); then D8EC(self) and D7F4(self).
 *
 * The cfe C (93.4%) had every temp renumbered (t6/t7/t9/t3 vs the target's
 * v0/t6/t8/t1) and a late `or s0,a0`. The same logic under NCC, with the
 * PMFs passed by value (pointer-to-member type) and the param used
 * directly, matches on the first compile.
 * Spliced into game_uso.c.o via REPLACE_FUNC_BODY
 * (docs/MATCHING_WORKFLOW.md#game-uso-ncc-cplusplus-donor-recipe). Every
 * symbol is extern "C". */
typedef int s32; typedef unsigned short u16; typedef float f32; typedef double f64;
typedef short s16; typedef unsigned char u8; typedef unsigned int u32;
#include "../../include/game/rideranim.h" /* NCC has no -I include */
struct A { char pad[40]; virtual void m(); };
typedef void (A::*PM)();
extern "C" {
extern char game_uso_D_807FF438;
extern char game_uso_D_807FF460;
void import_0010DB78(void *, int, int, int, int, int);
void RiderAnim_setNextState_0000D5BC(void *, PM);
void RiderAnim_checkJump_0000D8EC(void *);
void RiderAnim_checkReverseBrake_0000D7F4(void *);
}
#define AT(T,b,o) (*(T *)((char *)(b) + (o)))
extern "C" void RiderAnim_stateBrakeDeep(int *a0) {
    SnowBoarder *p;
    import_0010DB78(a0, RANIM_ID(2, RANIM_BRAKE_B), RANIM_ID(2, RANIM_BRAKE_F), *(int *)&RA(a0)->rider->edge, 0, 1);
    p = RA(a0)->rider;
    if (p->crouch == 0) {
        RiderAnim_setNextState_0000D5BC(a0, AT(PM, &game_uso_D_807FF438, 0xE48));
        p = RA(a0)->rider;
    }
    if (p->useDParms == 0) {
        RiderAnim_setNextState_0000D5BC(a0, AT(PM, &game_uso_D_807FF460, 0xE70));
    }
    RiderAnim_checkJump_0000D8EC(a0);
    RiderAnim_checkReverseBrake_0000D7F4(a0);
}
