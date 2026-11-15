/* IDO 7.1 NCC (C++) donor for RiderAnim_startQueuedTrick_0000E35C (0x208 / 130 words).
 *
 * Slot/queue advance, sibling of F6D4 (same body inside `if (self->0xF0)`,
 * no D6E4 pre-flush, 077C44 third arg 0, returns 1 / 0). Built with the
 * F6D4 recipe and exact on the first compile: no named prev local (the
 * F0 -> F4 move goes through the members, and the F8 compare and store
 * re-read F4), counter increment first, `F8 == F4`, base-first ceil-div
 * addu (edg), D5F8 / D5BC pairs are pointers to members by value. The
 * cfe wrap capped at 95.3%.
 * See docs/IDO_CODEGEN.md#ncc-no-named-copy-local-f6d4. Spliced into
 * game_uso.c.o via REPLACE_FUNC_BODY
 * (docs/MATCHING_WORKFLOW.md#game-uso-ncc-cplusplus-donor-recipe). Every
 * symbol is extern "C". */
typedef int s32;
typedef unsigned int u32;
#include "../../include/game/tricktable.h" /* NCC has no -I include */
#include "../../include/uso_call.h"  /* NCC donors build without -I include */
#include "../../include/game/input.h"  /* NCC donors build without -I include */
struct A { char pad[40]; virtual void m(); };
typedef void (A::*PM)();
extern "C" {
extern int *game_uso_D_807FFBC0;
extern char import_800201D0;
extern char game_uso_D_807FF4B0;
extern char game_uso_D_807FF4B8;
extern char game_uso_D_807FF4C0;
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
extern "C" int RiderAnim_startQueuedTrick_0000E35C(char *s0) {
    TrickEntry *f4;
    if (QUEUED_TRICK(s0) != 0) {
        RiderAnim_addTrickPoints_0000EE30(s0, QUEUED_TRICK(s0)->points, 1);
        if (RIDER_FLAGS(s0) & 0x20) {
            int base = (int)game_uso_D_807FFBC0;
            import_000B8518(*(int *)&INPUT_MGR(import_800201D0).dataMgr, RIDER(s0),
                ((base + (QUEUED_TRICK(s0)->mirrorIndex << 6)) - base) / 64);
        } else {
            import_000B8518(*(int *)&INPUT_MGR(import_800201D0).dataMgr, RIDER(s0),
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
            XCALL(game_uso_func_077C44, AnimPlayer_start)(s0, f4->anim, 0, 2, 1, 1);
            RiderAnim_setState_0000D5F8(s0, AT(PM, &game_uso_D_807FF4B0, 0xEC0), 2);
        } else {
            XCALL(game_uso_func_077C44, AnimPlayer_start)(s0, f4->anim, 0, 3, 1, 1);
            RiderAnim_setState_0000D5F8(s0, AT(PM, &game_uso_D_807FF4B8, 0xEC8), 3);
        }
        RiderAnim_setNextState_0000D5BC(s0, AT(PM, &game_uso_D_807FF4C0, 0xED0));
        return 1;
    }
    return 0;
}
