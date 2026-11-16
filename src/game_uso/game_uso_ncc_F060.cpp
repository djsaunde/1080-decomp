/* IDO 7.1 NCC (C++) donor for RiderAnim_statePreJump (0xDC / 55 words).
 *
 * Flag-dispatch state handler: clear self->0xF4, run 11888, then if the
 * 0xB4 object's 0x800 child has (!(flags & 0x100) || (flags & 0x200)) and
 * the 0xB4 object's 0x938 is set, F360(self) and return. Otherwise DB28,
 * D5F8 with the PMF at D_807FF4A0+0xEB0 passed by value, clear 0xE6,
 * bump 0xE4 and call 966AC(self->0xB4 + 0x808).
 *
 * The 0x800 child is read through an inlined member `V::child()`. edg keeps
 * the inline return temp in v0, so the 0xB4 pointer colours v1 as in the
 * target; the same expression written inline (cfe or NCC) colours it v0
 * (99.64%). `((x & 0x100) != 0) == 0` keeps the materialized bool
 * (`sltu; beqzl`), and the `unsigned *` re-read keeps the flag reload.
 * Spliced into game_uso.c.o via REPLACE_FUNC_BODY
 * (docs/MATCHING_WORKFLOW.md#game-uso-ncc-cplusplus-donor-recipe). Every
 * symbol is extern "C". */
typedef int s32; typedef unsigned short u16; typedef float f32; typedef double f64;
typedef short s16; typedef unsigned char u8; typedef unsigned int u32;
#include "../../include/game/rideranim.h" /* NCC has no -I include */
struct A { char pad[40]; virtual void m(); };
typedef void (A::*PM)();
extern "C" {
extern char game_uso_D_807FF4A0;
void import_0010DB28(void *, int, int, int, int, int);
void import_000966AC(void *);
void RiderAnim_jumpTakeoff_0000F360(void *);
void RiderAnim_clearTricks_00011888(void *);
void RiderAnim_queueState_0000D5F8(void *, PM, int);
}
#define AT(T,b,o) (*(T *)((char *)(b) + (o)))
struct V { int *child() { return AT(int *, this, 0x800); } };
extern "C" void RiderAnim_statePreJump(int *a0) {
    int *s0 = a0;
    RA(a0)->curTrick = 0;
    RiderAnim_clearTricks_00011888(a0);
    a0 = ((V *)RA(s0)->rider)->child(); /* rider->input */
    /* the u32 view of held makes edg load it again for the 0x200 test (a plain second read is CSEd) */
    if ((((((RiderInput *)a0)->held & 0x100) != 0) == 0 || (*(u32 *)&((RiderInput *)a0)->held & 0x200)) && RA(s0)->rider->contactCount != 0) {
        RiderAnim_jumpTakeoff_0000F360(s0);
        return;
    }
    import_0010DB28(s0, RANIM_ID(1, RANIM_PRE_JUMP), 0, 0, 0x100, RA(s0)->parms.jct.value.i);
    RiderAnim_queueState_0000D5F8(s0, AT(PM, &game_uso_D_807FF4A0, 0xEB0), -1);
    RA(s0)->holdFrames = 0;
    RA(s0)->jumpCharge = RA(s0)->jumpCharge + 1;
    import_000966AC(RA(s0)->rider->sound);
}
