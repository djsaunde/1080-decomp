#ifndef GAME_TIMPROC_USO_B5_DATA_H
#define GAME_TIMPROC_USO_B5_DATA_H

/* genproc.uso (Text = src/timproc_uso_b5) data: named SECTION OFFSETS (agent-g run 593).
 *
 * Same scheme as game_uso_data.h / mode_uso_data.h: a site keeps its per-site zero symbol
 * (timproc_uso_b5_D_<addr>, or an import_<addr> in the src/match_imports ports) and the
 * section offset K gets a name. Plain integers: a respelled site preprocesses to the same
 * expression. `scripts/uso-data-sites.py --uso genproc check` verifies every constant here:
 * the value claim (f32 V) against the genproc section bytes, and each function that uses it
 * (src/timproc_uso_b5 and its src/match_imports ports) against a section-base LO16 reloc of
 * that section with that K. Macros only: C and C++ safe. (Owner c's earlier genproc offsets,
 * kRoF_gen_* / kTag_gen_*, are in include/game/select.h.) */

/* ---- RoData ---- */
#define kRoF_gen_1e7_3A0 0x3A0       /* RoData+0x3A0: f32 1e7, CharacterList_createItem's LightTarget
                                      * unk108[0..2] (the model's light-target scale) */
#define kRoF_gen_0p1_3A4 0x3A4       /* RoData+0x3A4: f32 0.1, CharacterList_createItem's
                                      * SelBoardModel.highlight */
#define kRoF_gen_1e7_3B4 0x3B4       /* RoData+0x3B4: f32 1e7, BoardList_createItem's LightTarget
                                      * unk108[0..2] */
#define kRoF_gen_m28p48_3B8 0x3B8    /* RoData+0x3B8: f32 -28.48, BoardList_createItem's item cfv */

/* ---- Data ---- */
#define kData_gen_charItemSlot_E54 0xE54 /* Data+0xE54: s32[8] {4, 3, 1, 2, 0, 1, 3, 0}: SelectWin_init's
                                          * chosenItem[0] (the character list slot) per character */
#define kData_gen_charCheat_E74 0xE74    /* Data+0xE74: s32[8] {0, 0, 0, 0, 0, 1, 2, 3}: SelectWin_init's
                                          * cheatChar per character (characters 5..7 start as the
                                          * matched cheat sequence + 1) */
#define kData_gen_boardCheat_E94 0xE94   /* Data+0xE94: s32[] (all 0 in the ROM): SelectWin_init's
                                          * cheatBoard per board */

#endif
