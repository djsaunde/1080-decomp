/* IDO 7.1 NCC (C++) donor for RiderAnim_stateRideDeep (0x10C / 67 words).
 *
 * State-entry handler, sibling of 10694: self->0xB4->0x3DC = 1, D8A8(self),
 * D8EC(self); f0 = self->0xB4->0x970; if |f0| exceeds the double at
 * D_807FFB78+0x258, DB78(self, 0x20002, 0x20003, f0, 0, 1), else
 * DB28(self, 0x20008, 0, 1, 6, 1); E1FC(self); if self->0xB4->0x9CC == 0,
 * D5BC(self, PMF at D_807FF400+0xE10); 11750(self).
 *
 * The cfe wrap (87.67%) had two wrong guesses: the `mfc1 a3,f0` is just
 * DB78's float 4th parameter (as in F13C), and the "dead arg-prep" before
 * the abs branch (a1 = 0x20008, a2 = 0, a3 = 1) is the else-arm's DB28
 * argument list, hoisted. With both fixed the NCC compile matches on the
 * first try.
 * Spliced into game_uso.c.o via REPLACE_FUNC_BODY
 * (docs/MATCHING_WORKFLOW.md#game-uso-ncc-cplusplus-donor-recipe). Every
 * symbol is extern "C". */
typedef int s32; typedef unsigned short u16; typedef float f32; typedef double f64;
typedef short s16; typedef unsigned char u8; typedef unsigned int u32;
#include "../../include/game/rideranim.h" /* NCC has no -I include */
#include "../../include/game/game_uso_data.h"
struct A { char pad[40]; virtual void m(); };
typedef void (A::*PM)();
extern "C" {
extern char game_uso_D_807FFB78;
extern char game_uso_D_807FF400;
void import_0010DB28(void *, int, int, int, int, int);
void import_0010DB78(void *, int, int, float, int, int);
void RiderAnim_checkBrake_0000D8A8(void *);
void RiderAnim_checkJump_0000D8EC(void *);
void RiderAnim_checkRow_0000E1FC(void *);
void RiderAnim_enterState_0000D5BC(void *, PM);
void RiderAnim_checkBackwardSwitch_00011750(void *);
}
#define AT(T,b,o) (*(T *)((char *)(b) + (o)))
extern "C" void RiderAnim_stateRideDeep(int *a0) {
    float f0;
    RA(a0)->rider->riding = 1;
    RiderAnim_checkBrake_0000D8A8(a0);
    RiderAnim_checkJump_0000D8EC(a0);
    f0 = RA(a0)->rider->edge;
    if ((double)(f0 < 0.0f ? -f0 : f0) > AT(double, &game_uso_D_807FFB78, kRoD_0p06_258)) {
        import_0010DB78(a0, RANIM_ID(2, RANIM_TURN_B), RANIM_ID(2, RANIM_TURN_F), f0, 0, 1);
    } else {
        import_0010DB28(a0, RANIM_ID(2, RANIM_UP_DOWN), 0, 1, 6, 1);
    }
    RiderAnim_checkRow_0000E1FC(a0);
    if (RA(a0)->rider->useDParms == 0) {
        RiderAnim_enterState_0000D5BC(a0, AT(PM, &game_uso_D_807FF400, kPmf_RiderAnim_stateRideBlendIn));
    }
    RiderAnim_checkBackwardSwitch_00011750(a0);
}
