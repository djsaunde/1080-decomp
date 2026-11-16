#ifndef GAME_GAME_USO_DATA_H
#define GAME_GAME_USO_DATA_H

/* game.uso data: named SECTION OFFSETS (agent-g run 538).
 *
 * game_uso is linked at VRAM 0 like every USO, so each data reference is a
 * zero symbol plus the section offset K the ROM instruction carries
 * (`&game_uso_D_807FF430 + 0xE40`, `AT(PM, &game_uso_D_807FF430, 0xE40)`, or
 * `&D_00000000 + K`). The symbol is part of the matched spelling and stays;
 * these constants name K. They are plain integers, so a respelled site
 * preprocesses to the same expression.
 *
 * `scripts/uso-data-sites.py check` verifies each one: every function that uses
 * it has a section-base LO16 site of that section with that K (game.uso TextReloc
 * table), and a `PMF -> FUNC` comment names the function the DataReloc at K + 4
 * points at. Lines below are `uso-data-sites.py --uso game pmf-table` output;
 * `--uso game spell FILE --write` respells the sites. Macros only, so the header
 * is C and C++ safe (NCC donors include it by relative path). */

/* RiderAnim states: the 8-byte PMF records {s16 delta, s16 index, fn} at
 * Data 0xDB0..0xF60 that RiderAnim_enterState / queueState take
 * (rideranim.h: queuedState 0xC0, state 0xC8). Data 0xDC8 is the null PMF
 * (all zero) that clearState stores and the update loop compares with. */
#define kPmf_RiderAnim_stateStartIntro 0xDB0 /* Data+0xDB0: PMF -> RiderAnim_stateStartIntro */
#define kPmf_RiderAnim_stateStartWait 0xDB8 /* Data+0xDB8: PMF -> RiderAnim_stateStartWait */
#define kPmf_RiderAnim_stateStopped 0xDC0 /* Data+0xDC0: PMF -> RiderAnim_stateStopped */
#define kPmfNull_DC8 0xDC8 /* Data+0xDC8: PMF -> null */
#define kPmf_RiderAnim_stateHopStart 0xDD0 /* Data+0xDD0: PMF -> RiderAnim_stateHopStart */
#define kPmf_RiderAnim_stateHopRise 0xDD8 /* Data+0xDD8: PMF -> RiderAnim_stateHopRise */
#define kPmf_RiderAnim_stateCushionIn 0xDE0 /* Data+0xDE0: PMF -> RiderAnim_stateCushionIn */
#define kPmf_RiderAnim_stateHopDrop 0xDE8 /* Data+0xDE8: PMF -> RiderAnim_stateHopDrop */
#define kPmf_RiderAnim_stateHopAir 0xDF0 /* Data+0xDF0: PMF -> RiderAnim_stateHopAir */
#define kPmf_RiderAnim_stateHopLand 0xDF8 /* Data+0xDF8: PMF -> RiderAnim_stateHopLand */
#define kPmf_RiderAnim_stateCushionEnd 0xE00 /* Data+0xE00: PMF -> RiderAnim_stateCushionEnd */
#define kPmf_RiderAnim_stateCushion 0xE08 /* Data+0xE08: PMF -> RiderAnim_stateCushion */
#define kPmf_RiderAnim_stateRideBlendIn 0xE10 /* Data+0xE10: PMF -> RiderAnim_stateRideBlendIn */
#define kPmf_RiderAnim_stateOllieRise 0xE18 /* Data+0xE18: PMF -> RiderAnim_stateOllieRise */
#define kPmf_RiderAnim_stateOllieTakeoff 0xE20 /* Data+0xE20: PMF -> RiderAnim_stateOllieTakeoff */
#define kPmf_RiderAnim_stateOllieLand 0xE28 /* Data+0xE28: PMF -> RiderAnim_stateOllieLand */
#define kPmf_RiderAnim_stateOllieHang 0xE30 /* Data+0xE30: PMF -> RiderAnim_stateOllieHang */
#define kPmf_RiderAnim_stateOllieDrop 0xE38 /* Data+0xE38: PMF -> RiderAnim_stateOllieDrop */
#define kPmf_RiderAnim_stateRide 0xE40 /* Data+0xE40: PMF -> RiderAnim_stateRide */
#define kPmf_RiderAnim_stateRideDeepBlendIn 0xE48 /* Data+0xE48: PMF -> RiderAnim_stateRideDeepBlendIn */
#define kPmf_RiderAnim_stateRow 0xE50 /* Data+0xE50: PMF -> RiderAnim_stateRow */
#define kPmf_RiderAnim_stateRowOut 0xE58 /* Data+0xE58: PMF -> RiderAnim_stateRowOut */
#define kPmf_RiderAnim_stateRideDeep 0xE60 /* Data+0xE60: PMF -> RiderAnim_stateRideDeep */
#define kPmf_RiderAnim_stateRowIn 0xE68 /* Data+0xE68: PMF -> RiderAnim_stateRowIn */
#define kPmf_RiderAnim_stateBrakeBlendIn 0xE70 /* Data+0xE70: PMF -> RiderAnim_stateBrakeBlendIn */
#define kPmf_RiderAnim_stateBrakeDeep 0xE78 /* Data+0xE78: PMF -> RiderAnim_stateBrakeDeep */
#define kPmf_RiderAnim_stateBrake 0xE80 /* Data+0xE80: PMF -> RiderAnim_stateBrake */
#define kPmf_RiderAnim_stateCrashRecover 0xE88 /* Data+0xE88: PMF -> RiderAnim_stateCrashRecover */
#define kPmf_RiderAnim_stateResumeRide 0xE90 /* Data+0xE90: PMF -> RiderAnim_stateResumeRide */
#define kPmf_RiderAnim_stateSwitchStanceEnd 0xE98 /* Data+0xE98: PMF -> RiderAnim_stateSwitchStanceEnd */
#define kPmf_RiderAnim_stateSwitchStance 0xEA0 /* Data+0xEA0: PMF -> RiderAnim_stateSwitchStance */
#define kPmf_RiderAnim_statePreJumpCancel 0xEA8 /* Data+0xEA8: PMF -> RiderAnim_statePreJumpCancel */
#define kPmf_RiderAnim_statePreJumpHold 0xEB0 /* Data+0xEB0: PMF -> RiderAnim_statePreJumpHold */
#define kPmf_RiderAnim_statePreJump 0xEB8 /* Data+0xEB8: PMF -> RiderAnim_statePreJump */
#define kPmf_RiderAnim_stateTrickWindup 0xEC0 /* Data+0xEC0: PMF -> RiderAnim_stateTrickWindup */
#define kPmf_RiderAnim_stateTrickRelease 0xEC8 /* Data+0xEC8: PMF -> RiderAnim_stateTrickRelease */
#define kPmf_RiderAnim_stateTrickAir 0xED0 /* Data+0xED0: PMF -> RiderAnim_stateTrickAir */
#define kPmf_RiderAnim_stateTrickHold 0xED8 /* Data+0xED8: PMF -> RiderAnim_stateTrickHold */
#define kPmf_RiderAnim_stateTrickStart 0xEE0 /* Data+0xEE0: PMF -> RiderAnim_stateTrickStart */
#define kPmf_RiderAnim_stateTrickEnd 0xEE8 /* Data+0xEE8: PMF -> RiderAnim_stateTrickEnd */
#define kPmf_RiderAnim_stateFlipToRide 0xEF0 /* Data+0xEF0: PMF -> RiderAnim_stateFlipToRide */
#define kPmf_RiderAnim_stateStartIntroEnd 0xEF8 /* Data+0xEF8: PMF -> RiderAnim_stateStartIntroEnd */
#define kPmf_RiderAnim_stateIdle 0xF00 /* Data+0xF00: PMF -> RiderAnim_stateIdle */
#define kPmf_RiderAnim_stateEnding2 0xF08 /* Data+0xF08: PMF -> RiderAnim_stateEnding2 */
#define kPmf_RiderAnim_stateEnding3 0xF10 /* Data+0xF10: PMF -> RiderAnim_stateEnding3 */
#define kPmf_RiderAnim_stateEnding1 0xF18 /* Data+0xF18: PMF -> RiderAnim_stateEnding1 */
#define kPmf_RiderAnim_stateFinish 0xF20 /* Data+0xF20: PMF -> RiderAnim_stateFinish */
#define kPmf_RiderAnim_stateGoalNormal 0xF28 /* Data+0xF28: PMF -> RiderAnim_stateGoalNormal */
#define kPmf_RiderAnim_stateGoalWinOM 0xF30 /* Data+0xF30: PMF -> RiderAnim_stateGoalWinOM */
#define kPmf_RiderAnim_stateGoalWin 0xF38 /* Data+0xF38: PMF -> RiderAnim_stateGoalWin */
#define kPmf_RiderAnim_stateGoalLose 0xF40 /* Data+0xF40: PMF -> RiderAnim_stateGoalLose */
#define kPmf_RiderAnim_stateTimeUp 0xF48 /* Data+0xF48: PMF -> RiderAnim_stateTimeUp */
#define kPmf_RiderAnim_stateTimeUpLose 0xF50 /* Data+0xF50: PMF -> RiderAnim_stateTimeUpLose */
#define kPmf_RiderAnim_stateStand 0xF58 /* Data+0xF58: PMF -> RiderAnim_stateStand */

#endif
