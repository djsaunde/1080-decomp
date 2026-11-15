#ifndef GAME_RIDERANIM_ANIMS_H
#define GAME_RIDERANIM_ANIMS_H

/*
 * Rider animation ids in rom/anims.bin (agent-g run 488). Only #defines, so the C TUs and the
 * NCC C++ donors can both include it.
 *
 * An anim id is (group << 16) | index (include/gdl/anim.h). rom/anims.bin (ROM 0x923C02, after the
 * 0x2C-byte Info record and the data record header) is the AnimDataMgr_loadFile stream: version
 * 0x128, 9 groups. Every anim record starts with cmd 4 (skip 0x20 bytes), and those 0x20 bytes are
 * the anim's name and its "<n>-0.dsc" source. Entries whose name starts with a 0 byte (or is all
 * 0xFF) are empty: numFrames 0 ("Empty Anim"). The full list is docs/1080_GAME_CLASSES.md section 17.
 *
 * RiderAnim.animSet holds the stance group (RANIM_GROUP_REGULAR or RANIM_GROUP_SWITCH);
 * groups 1 and 5 have the same layout, so `animSet | RANIM_TURN_B` picks the stance's version.
 */
#define RANIM_ID(group, index)  (((group) << 16) | (index))

#define RANIM_GROUP_REGULAR     0x10000 /* group 1: regular stance riding / crashes (48 anims) */
#define RANIM_GROUP_DEEP        0x20000 /* group 2: crouched ("dp", deep position) variants */
#define RANIM_GROUP_AIR         0x30000 /* group 3: ollie and the grab / spin tricks (TrickTable +0x20) */
#define RANIM_GROUP_SELECT      0x40000 /* group 4: character-select poses */
#define RANIM_GROUP_SWITCH      0x50000 /* group 5: switch stance ("fk", fakie) version of group 1 */
#define RANIM_GROUP_DEAD        0x60000 /* group 6: crash_*_DEAD sets (RiderAnim_selectAnim's | 0x60000) */
#define RANIM_GROUP_EVENT       0x70000 /* group 7: start, goal, time up, ending */

/* groups 1 / 5 (OR into animSet); group 2 uses the same indices for its deep versions */
#define RANIM_ROW               0x00    /* boarder_row */
#define RANIM_TURN_B            0x02    /* boarder_turn_normal_b66 / fk_turn_norm_b / turn_deep_b2 */
#define RANIM_TURN_F            0x03    /* boarder_turn_normal_f55 / fk_turn_norm_f / turn_deep_f1 */
#define RANIM_BRAKE_B           0x04    /* boarder_brake_b / fk_brake_b / brake_dp_b */
#define RANIM_BRAKE_F           0x05    /* boarder_brake_f / fk_brake_f / brake_dp_f */
#define RANIM_WEIGHT_F          0x06    /* boarder_weight_f (empty) */
#define RANIM_WEIGHT_B          0x07    /* boarder_weight_b (empty) */
#define RANIM_UP_DOWN           0x08    /* boarder_up_down / fk_up_down / up_down_dp */
#define RANIM_LANDING           0x09    /* boarder_landing_s (empty in groups 1 / 5) */
#define RANIM_CRASH_SLIDE       0x0A    /* new01_17_crashing_f_slide */
#define RANIM_PRE_JUMP          0x11    /* boarder_pre_jump_AZ */
#define RANIM_BUMP_R_BREAK      0x16    /* boarder_bump_r_break */
#define RANIM_TO_FAKIE_180F     0x18    /* open_norm_fk_F02 */
#define RANIM_HOP               0x19    /* new_hop07 / new_fk_hop06_1 */
#define RANIM_TO_FAKIE_180B     0x1A    /* blind_norm_fk_B02 */
#define RANIM_TO_FAKIE_360F     0x1B    /* front_360_norm_fk_360_F01 */
#define RANIM_TO_FAKIE_360B     0x1C    /* blind_360_norm_fk_360_B01 */
#define RANIM_SWITCH_STANCE     0x1D    /* swith_stanceNF / swith_stanceFN */
#define RANIM_DEEP_SWITCH       0x24    /* deep_pos_sw_stNF / deep_pos_sw_stFN */
#define RANIM_KNEE_CUSHION      0x25    /* knee_cushion06 / knee_cushion05 */
#define RANIM_PRE_JUMP_DEEP_B   0x28    /* boarder_turn_dp_preJ_b */
#define RANIM_PRE_JUMP_DEEP_F   0x29    /* boarder_turn_dp_preJ_f */
#define RANIM_FLIP_CRASH_F      0x2A    /* F_flip_crash4 */
#define RANIM_FLIP_CRASH_B      0x2B    /* normal_B_flip_crash3_1 */

/* group 3 */
#define RANIM_OLLIE             0x01    /* boarder_ollie_arm (an empty entry in the retail file) */

/* group 7 */
#define RANIM_STAND             0x00    /* start_a */
#define RANIM_GOAL_WIN          0x03    /* boarder_goal_w3 */
#define RANIM_GOAL_LOSE         0x04    /* boarder_goal_l2 */
#define RANIM_GOAL_NORMAL       0x05    /* boarder_goal_n_r5 */
#define RANIM_TIME_UP           0x07    /* boarder_time_up6 (not started by RiderAnim) */
#define RANIM_ENDING_WIN        0x08    /* ending_WIN_480d */
#define RANIM_ENDING_LOSE       0x09    /* ending_LOSE_475b */
#define RANIM_START_INTRO       0x0A    /* start_A_type02; + character for B..E */
#define RANIM_GOAL_WIN_OM       0x0F    /* HP_OM_goal_WIN5_2 */

#endif /* GAME_RIDERANIM_ANIMS_H */
