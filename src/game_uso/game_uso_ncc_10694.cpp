/* IDO 7.1 NCC (C++) donor for RiderAnim_stateRide (0x1AC / 107 words).
 *
 * State-entry handler: flag the 0xB4 object, set state 2, sample
 * 0xB4->0x970, run the E1FC/D8A8/D8EC enter hooks, pick the DB78 / DB28
 * import by |r| vs the D_807FFB70+0x250 double, then two SetState-style
 * calls RiderAnim_enterState_0000D5BC(this, pm) taking a pointer-to-member BY VALUE
 * (a1/a2 + home-slot stores): once with the null-ish PMF at
 * D_807FF438+0xE48, once with this->pm at +0xD0 when it is non-null (the
 * same edg 2-word `at`/`addi` PMF compare as D204). Spliced into
 * game_uso.c.o via REPLACE_FUNC_BODY (docs/MATCHING_WORKFLOW.md#game-uso-
 * ncc-cplusplus-donor-recipe). The float 4th arg of DB78 goes out as
 * `mfc1 a3,$f0` straight from the prototype -- the "mfc1-from-C cap" of the
 * old C wrap was a missing prototype, not an IDO limit. Every symbol is
 * extern "C". */
typedef int s32; typedef unsigned short u16; typedef float f32; typedef double f64;
typedef short s16; typedef unsigned char u8; typedef unsigned int u32;
#include "../../include/game/rideranim.h" /* NCC has no -I include */
#include "../../include/game/game_uso_data.h"
struct A { char pad[40]; virtual void m(); };
typedef void (A::*PM)();
extern "C" {
extern char game_uso_D_807FFB70;
extern char game_uso_D_807FF438;
extern char game_uso_D_807FF3B8;
void RiderAnim_checkRow_0000E1FC(void *);
void RiderAnim_checkBrake_0000D8A8(void *);
void RiderAnim_checkJump_0000D8EC(void *);
void import_0010DB78(void *, int, int, float, int, int);
void import_0010DB28(void *, int, int, int, int, int);
void RiderAnim_checkGroundSpin_0000E91C(void *);
void RiderAnim_checkBackwardSwitch_00011750(void *);
void RiderAnim_enterState_0000D5BC(void *, PM);
}
#define AT(T,b,o) (*(T *)((char *)(b) + (o)))
#define PMF(pair) (*(PM *)&(pair)) /* a Pair2 field read as the PMF it holds */
extern "C" void RiderAnim_stateRide(A *a0) {
    float r;
    float ar;
    RA(a0)->rider->unkA18 = 1;
    RA(a0)->trickPhase = 2;
    RA(a0)->rider->riding = 1;
    RA(a0)->hitScale = 1.0f;
    r = RA(a0)->rider->edge;
    RiderAnim_checkRow_0000E1FC(a0);
    RA(a0)->rider->slowdown = 0;
    RiderAnim_checkBrake_0000D8A8(a0);
    RiderAnim_checkJump_0000D8EC(a0);
    ar = (r < 0.0f) ? -r : r;
    if ((double)ar > AT(double, &game_uso_D_807FFB70, 0x250)) {
        int v = RA(a0)->animSet;
        import_0010DB78(a0, v | RANIM_TURN_B, v | RANIM_TURN_F, r, 0, 1);
    } else {
        import_0010DB28(a0, RA(a0)->animSet | RANIM_UP_DOWN, 0, 1, 6, 1);
    }
    if (RA(a0)->rider->useDParms != 0) {
        RiderAnim_enterState_0000D5BC(a0, AT(PM, &game_uso_D_807FF438, kPmf_RiderAnim_stateRideDeepBlendIn));
    }
    RiderAnim_checkGroundSpin_0000E91C(a0);
    RiderAnim_checkBackwardSwitch_00011750(a0);
    RA(a0)->rideContacts = RA(a0)->rider->contactCount;
    if (PMF(RA(a0)->pendingState) != AT(PM, &game_uso_D_807FF3B8, kPmfNull_DC8)) { /* the null PMF */
        RiderAnim_enterState_0000D5BC(a0, PMF(RA(a0)->pendingState));
    }
}
