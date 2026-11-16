#ifndef GAME_MODE_USO_DATA_H
#define GAME_MODE_USO_DATA_H

/* Mode-USO data: named SECTION OFFSETS (agent-h run 588).
 *
 * The mode modules (arcproc, mgrproc, timproc = timproc_uso_b1, trkproc = timproc_uso_b3) link
 * at VRAM 0 like every USO, so a reference to their own Data / RoData / Bss is a zero symbol
 * (a per-site alias such as D_b1_11d8_b, or &D_00000000) plus the section offset K that the
 * ROM instruction carries. gl_data.h's GL_DATA names cover bootup.uso only, and
 * scripts/uso-data-sites.py reads only bootup.uso / game.uso, so these constants name K the
 * game_uso_data.h way: the per-site symbol stays (it is part of the matched spelling) and
 * the offset gets a name. Plain integers: a respelled site preprocesses to the same
 * expression.
 *
 * Checked by hand against the ROM (run 588): each site's reloc is a section-base LO16 of that
 * section (`scripts/uso-imports.py sites <module>`: "RoData(base)" / "Data(base)" /
 * "Bss(base)"), and the value is the word at that offset of the module's section payload
 * (scripts/uso_original.py modules()). Macros only: C and C++ safe. */

/* ---- arcproc.uso ---- */
#define kRoF_arc_resultFadeStep_44 0x44   /* RoData+0x44: f32 0.1: ArcNameEntry_drawExitOverlay's
                                           * per-frame ArcRace.resultFade decrement */

/* ---- mgrproc.uso ---- */
#define kBss_mgr_raceProc_30 0x30         /* Bss+0x30: the running MGRRaceProcess (MGR_STATE_PTR; the
                                           * pause-menu callbacks resume / end it) */
#define kData_mgr_endingPhase_5F0 0x5F0   /* Data+0x5F0: s32[3] {0, 0, 2}: MgrRace.endingPhase for
                                           * course 5 / 6 / 7 (mgr_startEnding_00001BE4, idx = course - 5) */
#define kData_mgr_endingTimer_5FC 0x5FC   /* Data+0x5FC: s32[3] {750, 750, 900}: MgrRace.endingTimer,
                                           * same index */

/* ---- timproc.uso (timproc_uso_b1) ---- */
#define kRoF_tim_recordFadeMax_40 0x40    /* RoData+0x40: f32 0.975: TimRace.recordFade fade-in cap */
#define kRoF_tim_recordFadeIn_44 0x44     /* RoData+0x44: f32 0.025: recordFade fade-in step */
#define kRoF_tim_recordFadeOut_48 0x48    /* RoData+0x48: f32 0.1: TimNameEntry_drawExitOverlay's
                                           * recordFade fade-out step */

/* ---- trkproc.uso (timproc_uso_b3; the same three floats as timproc) ---- */
#define kRoF_trk_recordFadeMax_40 0x40    /* RoData+0x40: f32 0.975 */
#define kRoF_trk_recordFadeIn_44 0x44     /* RoData+0x44: f32 0.025 */
#define kRoF_trk_recordFadeOut_48 0x48    /* RoData+0x48: f32 0.1 (TrkNameEntry_drawExitOverlay) */

#endif
