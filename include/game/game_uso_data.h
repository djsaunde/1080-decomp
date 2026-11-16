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

/* const-table:begin (uso-data-sites.py --uso game const-table --write; do not edit by hand) */
/* strings (class names, "Parms" group names, assert texts, file names) */
#define kStr_Parms_318 0x318 /* Data+0x318: "Parms" */
#define kStr_SnowChaseActorCam_340 0x340 /* Data+0x340: "SnowChaseActorCam" */
#define kStr_Parms_354 0x354 /* Data+0x354: "Parms" */
#define kStr_SnowChaseActorLook_360 0x360 /* Data+0x360: "SnowChaseActorLook" */
#define kStr_Parms_374 0x374 /* Data+0x374: "Parms" */
#define kStr_SnowChaseActor_3A0 0x3A0 /* Data+0x3A0: "SnowChaseActor" */
#define kStr_Node_6D0 0x6D0 /* Data+0x6D0: "<Node>" */
#define kStr_SnowAI_Parms_6D8 0x6D8 /* Data+0x6D8: "SnowAI::Parms" */
#define kStr_0_markerType_markerType_11_78C 0x78C /* Data+0x78C: "0 <= markerType && markerType <= 11" */
#define kStr_snowai_cpp_7B0 0x7B0 /* Data+0x7B0: "snowai.cpp" */
#define kStr_enCm_0_7D4 0x7D4 /* Data+0x7D4: "enCm != 0" */
#define kStr_snowai_cpp_7E0 0x7E0 /* Data+0x7E0: "snowai.cpp" */
#define kStr_nearBoarderNorNearMrk_0_7EC 0x7EC /* Data+0x7EC: "nearBoarderNorNearMrk != 0" */
#define kStr_snowai_cpp_808 0x808 /* Data+0x808: "snowai.cpp" */
#define kStr_nearBoarderDivNearMrk_0_814 0x814 /* Data+0x814: "nearBoarderDivNearMrk != 0" */
#define kStr_snowai_cpp_830 0x830 /* Data+0x830: "snowai.cpp" */
#define kStr_Parms_D54 0xD54 /* Data+0xD54: "Parms" */
#define kStr_MNode_D70 0xD70 /* Data+0xD70: "<MNode>" */
#define kStr_LensFlare_Parms_D78 0xD78 /* Data+0xD78: "LensFlare::Parms" */
#define kStr_LensFlare_D98 0xD98 /* Data+0xD98: "LensFlare" */
#define kStr_Parms_1224 0x1224 /* Data+0x1224: "Parms" */
/* 3-letter parm tags (Parms_addParm / the parms-file keys) */
#define kTag_cof_320 0x320 /* Data+0x320: "cof" */
#define kTag_ch0_35C 0x35C /* Data+0x35C: "ch0" */
#define kTag_off_37C 0x37C /* Data+0x37C: "off" */
#define kTag_sun_D50 0xD50 /* Data+0xD50: "sun" */
#define kTag_rot_D5C 0xD5C /* Data+0xD5C: "rot" */
#define kTag_ele_D60 0xD60 /* Data+0xD60: "ele" */
#define kTag_scl_D64 0xD64 /* Data+0xD64: "scl" */
#define kTag_mat_D68 0xD68 /* Data+0xD68: "mat" */
#define kTag_col_D6C 0xD6C /* Data+0xD6C: "col" */
#define kTag_fin_D8C 0xD8C /* Data+0xD8C: "fin" */
#define kTag_dlv_D90 0xD90 /* Data+0xD90: "dlv" */
#define kTag_dfm_D94 0xD94 /* Data+0xD94: "dfm" */
#define kTag_lnc_122C 0x122C /* Data+0x122C: "lnc" */
#define kTag_jct_1230 0x1230 /* Data+0x1230: "jct" */
#define kTag_jht_1234 0x1234 /* Data+0x1234: "jht" */
#define kTag_jot_1238 0x1238 /* Data+0x1238: "jot" */
#define kTag_jca_123C 0x123C /* Data+0x123C: "jca" */
#define kTag_jlt_1240 0x1240 /* Data+0x1240: "jlt" */
#define kTag_jli_1244 0x1244 /* Data+0x1244: "jli" */
#define kTag_rfc_1248 0x1248 /* Data+0x1248: "rfc" */
#define kTag_tkt_124C 0x124C /* Data+0x124C: "tkt" */
#define kTag_b1f_1250 0x1250 /* Data+0x1250: "b1f" */
#define kTag_b2f_1254 0x1254 /* Data+0x1254: "b2f" */
#define kTag_xf1_1258 0x1258 /* Data+0x1258: "xf1" */
#define kTag_f1f_125C 0x125C /* Data+0x125C: "f1f" */
#define kTag_f2f_1260 0x1260 /* Data+0x1260: "f2f" */
#define kTag_f3f_1264 0x1264 /* Data+0x1264: "f3f" */
#define kTag_f4f_1268 0x1268 /* Data+0x1268: "f4f" */
#define kTag_l1f_126C 0x126C /* Data+0x126C: "l1f" */
#define kTag_l2f_1270 0x1270 /* Data+0x1270: "l2f" */
#define kTag_l3f_1274 0x1274 /* Data+0x1274: "l3f" */
#define kTag_r1f_1278 0x1278 /* Data+0x1278: "r1f" */
#define kTag_r2f_127C 0x127C /* Data+0x127C: "r2f" */
#define kTag_dm1_1280 0x1280 /* Data+0x1280: "dm1" */
#define kTag_dm2_1284 0x1284 /* Data+0x1284: "dm2" */
#define kTag_dm3_1288 0x1288 /* Data+0x1288: "dm3" */
#define kTag_dm4_128C 0x128C /* Data+0x128C: "dm4" */
#define kTag_st1_1290 0x1290 /* Data+0x1290: "st1" */
#define kTag_st2_1294 0x1294 /* Data+0x1294: "st2" */
#define kTag_st3_1298 0x1298 /* Data+0x1298: "st3" */
#define kTag_sl1_129C 0x129C /* Data+0x129C: "sl1" */
#define kTag_sl2_12A0 0x12A0 /* Data+0x12A0: "sl2" */
#define kTag_slm_12A4 0x12A4 /* Data+0x12A4: "slm" */
#define kTag_stm_12A8 0x12A8 /* Data+0x12A8: "stm" */
/* float / double constants (the type is the one the site reads) */
#define kDataF_60_1098 0x1098 /* Data+0x1098: f32 60.0 */
#define kRoF_FLT_MAX_94 0x94 /* RoData+0x94: f32 3.4028235e+38 */
#define kRoF_mFLT_MAX_98 0x98 /* RoData+0x98: f32 -3.4028235e+38 */
#define kRoF_m10000_9C 0x9C /* RoData+0x9C: f32 -10000.0 */
#define kRoF_m10000_A0 0xA0 /* RoData+0xA0: f32 -10000.0 */
#define kRoF_m10000_A4 0xA4 /* RoData+0xA4: f32 -10000.0 */
#define kRoF_m3000_A8 0xA8 /* RoData+0xA8: f32 -3000.0 */
#define kRoF_m15000_AC 0xAC /* RoData+0xAC: f32 -15000.0 */
#define kRoF_m15000_B0 0xB0 /* RoData+0xB0: f32 -15000.0 */
#define kRoF_m10000_B4 0xB4 /* RoData+0xB4: f32 -10000.0 */
#define kRoF_m10000_B8 0xB8 /* RoData+0xB8: f32 -10000.0 */
#define kRoF_m10000_BC 0xBC /* RoData+0xBC: f32 -10000.0 */
#define kRoF_m10000_C0 0xC0 /* RoData+0xC0: f32 -10000.0 */
#define kRoF_m10000_C4 0xC4 /* RoData+0xC4: f32 -10000.0 */
#define kRoF_m10000_C8 0xC8 /* RoData+0xC8: f32 -10000.0 */
#define kRoF_m10000_CC 0xCC /* RoData+0xCC: f32 -10000.0 */
#define kRoF_m10000_D0 0xD0 /* RoData+0xD0: f32 -10000.0 */
#define kRoF_m10000_D4 0xD4 /* RoData+0xD4: f32 -10000.0 */
#define kRoF_1p1_D8 0xD8 /* RoData+0xD8: f32 1.1 */
#define kRoF_m6000_DC 0xDC /* RoData+0xDC: f32 -6000.0 */
#define kRoF_m6000_E0 0xE0 /* RoData+0xE0: f32 -6000.0 */
#define kRoF_m6000_E4 0xE4 /* RoData+0xE4: f32 -6000.0 */
#define kRoF_2p4_E8 0xE8 /* RoData+0xE8: f32 2.4 */
#define kRoF_0p4_EC 0xEC /* RoData+0xEC: f32 0.4 */
#define kRoF_0p3_F0 0xF0 /* RoData+0xF0: f32 0.3 */
#define kRoF_0p8_F4 0xF4 /* RoData+0xF4: f32 0.8 */
#define kRoF_m100000_F8 0xF8 /* RoData+0xF8: f32 -100000.0 */
#define kRoF_FLT_MAX_110 0x110 /* RoData+0x110: f32 3.4028235e+38 */
#define kRoF_FLT_MAX_114 0x114 /* RoData+0x114: f32 3.4028235e+38 */
#define kRoF_16000000_140 0x140 /* RoData+0x140: f32 16000000.0 */
#define kRoF_0p01_150 0x150 /* RoData+0x150: f32 0.01 */
#define kRoD_0p1_158 0x158 /* RoData+0x158: f64 0.1 */
#define kRoF_0p8_160 0x160 /* RoData+0x160: f32 0.8 */
#define kRoF_0p6_164 0x164 /* RoData+0x164: f32 0.6 */
#define kRoD_0p8_1F8 0x1F8 /* RoData+0x1F8: f64 0.8 */
#define kRoD_m0p4_200 0x200 /* RoData+0x200: f64 -0.4 */
#define kRoD_0p1_208 0x208 /* RoData+0x208: f64 0.1 */
#define kRoD_0p06_238 0x238 /* RoData+0x238: f64 0.06 */
#define kRoD_0p06_250 0x250 /* RoData+0x250: f64 0.06 */
#define kRoD_0p06_258 0x258 /* RoData+0x258: f64 0.06 */
/* const-table:end */

#endif
