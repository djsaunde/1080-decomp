/* IDO 7.1 NCC (C++) donor for RiderAnim_statePreJumpHold (0x144 / 81 words).
 *
 * Same opening flag block as F060: if the 0xB4 object's 0x800 child has
 * (!(flags & 0x100) || (flags & 0x200)) and the 0xB4 object's 0x938 is set,
 * F360(self). Otherwise DB78 (float 4th arg = 0xB4->0x970) when |0x970|
 * exceeds the D_807FFB58+0x238 double, else DB28; then 1001C(self) when
 * 0xB4->0x938 is clear. Finally ++self->0xE6 and D5BC(self, PMF by value)
 * once the old counter passes self->0x184.
 *
 * The inlined `V::child()` member puts the 0xB4 pointer in v1 (edg keeps the
 * inline return temp in v0); written inline it colours v0 (cfe 99.57%).
 * Spliced into game_uso.c.o via REPLACE_FUNC_BODY
 * (docs/MATCHING_WORKFLOW.md#game-uso-ncc-cplusplus-donor-recipe). Every
 * symbol is extern "C". */
typedef int s32; typedef unsigned short u16; typedef float f32; typedef double f64;
typedef short s16; typedef unsigned char u8; typedef unsigned int u32;
#include "../../include/game/rideranim.h" /* NCC has no -I include */
struct A { char pad[40]; virtual void m(); };
typedef void (A::*PM)();
extern "C" {
extern char game_uso_D_807FFB58;
extern char game_uso_D_807FF498;
void import_0010DB28(void *, int, int, int, int, int);
void import_0010DB78(void *, int, int, float, int, int);
void RiderAnim_jumpTakeoff_0000F360(void *);
void RiderAnim_startHop_0001001C(void *);
void RiderAnim_setNextState_0000D5BC(void *, PM);
}
#define AT(T,b,o) (*(T *)((char *)(b) + (o)))
struct V { int *child() { return AT(int *, this, 0x800); } };
extern "C" void RiderAnim_statePreJumpHold(int *a0, int *a1) {
    int *s0 = a0;
    a1 = ((V *)RA(s0)->rider)->child(); /* rider->input */
    /* the u32 view of held makes edg load it again for the 0x200 test (a plain second read is CSEd) */
    if ((((((RiderInput *)a1)->held & 0x100) != 0) == 0 || (*(u32 *)&((RiderInput *)a1)->held & 0x200)) && RA(s0)->rider->contactCount != 0) {
        RiderAnim_jumpTakeoff_0000F360(s0);
    } else {
        float f0 = RA(s0)->rider->edge;
        float abs_f0 = (f0 < 0.0f) ? -f0 : f0;
        if (AT(double, &game_uso_D_807FFB58, 0x238) < (double)abs_f0) {
            import_0010DB78(s0, RANIM_ID(1, RANIM_PRE_JUMP_DEEP_B), RANIM_ID(1, RANIM_PRE_JUMP_DEEP_F), f0, 0, 1);
        } else {
            import_0010DB28(s0, RANIM_ID(1, RANIM_PRE_JUMP), 0, 1, 6, 1);
        }
        if (RA(s0)->rider->contactCount == 0) {
            RiderAnim_startHop_0001001C(s0);
        }
    }
    {
        short v0 = RA(s0)->holdFrames;
        RA(s0)->holdFrames = v0 + 1;
        if (RA(s0)->parms.jht.value.i < v0) {
            RiderAnim_setNextState_0000D5BC(s0, AT(PM, &game_uso_D_807FF498, 0xEA8));
        }
    }
}
