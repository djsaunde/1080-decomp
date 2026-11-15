/* IDO 7.1 NCC (C++) donor for SelectionMgr_update_00001F14 (0x89C / 551 words,
 * per-frame two-player transition update). Spliced into timproc_uso_b5.c.o via
 * REPLACE_FUNC_BODY; its .rodata literal pool (0.05f steps, 1.333f + 1.3333 * t
 * blends) is pinned to RoData +0xC8 as SelectionMgr_update_00001F14_rodata.
 * The inner 87A0 call of each copy pair takes three args (a3 keeps 2). */
typedef int s32;
typedef float f32;
typedef double f64;
extern "C" {
extern void SelectionMgr_setSplit_00001DB0(char *);
extern int gen_stageBitToIndex_000087A0(...);
extern void gen_imp_Node_update(char *);
}
#include "../../include/game/select.h"  /* NCC donors build without -I include */
#define MGR ((SelectionMgr *)arg0)

extern "C" void SelectionMgr_update_00001F14(char *arg0) {
    s32 dst;
    s32 frame;
    s32 frame2;
    s32 frame3;
    char *r34;
    char *r34b;
    char *r34c;
    char *r38;
    char *r38b;
    char *r38c;

    MGR->waitMask = 0;
    if (MGR->numPlayers == 2) {
        if (MGR->wins[0]->state == 8) {
            MGR->waitMask |= 1;
        }
        if (MGR->wins[1]->state == 8) {
            MGR->waitMask = MGR->waitMask | 2;
        }
        if (MGR->waitMask != 0) {
            if (MGR->waitMask == 3) {
                if (MGR->split == 0x78) {
                    if (MGR->prevWaitMask == 1) {
                        MGR->busy = 1;
                        MGR->zoomWin = 1;
                        MGR->splitT = 0.0f;
                    } else if (MGR->prevWaitMask == 2) {
                        MGR->busy = 1;
                        MGR->zoomWin = 2;
                        MGR->splitT = 0.0f;
                    }
                }
                if (MGR->zoomWin == 1) {
                    dst = gen_stageBitToIndex_000087A0((char *)MGR->wins[1], 4, arg0);
                    MGR->wins[1]->curItem[dst] =
                        MGR->wins[0]->curItem[gen_stageBitToIndex_000087A0((char *)MGR->wins[0], 4, arg0)];
                } else {
                    dst = gen_stageBitToIndex_000087A0((char *)MGR->wins[0], 4, arg0);
                    MGR->wins[0]->curItem[dst] =
                        MGR->wins[1]->curItem[gen_stageBitToIndex_000087A0((char *)MGR->wins[1], 4, arg0)];
                }
                if (MGR->busy != 0) {
                    if (MGR->zoomWin == 1) {
                        MGR->splitT = MGR->splitT + 0.05f;
                        if (MGR->splitT >= 1.0f) {
                            MGR->splitT = 1.0f;
                            MGR->busy = 0;
                        }
                        MGR->split = (s32) (120.0f + (MGR->splitT * 120.0f));
                        SelectionMgr_setSplit_00001DB0(arg0);
                    }
                    if (MGR->zoomWin == 2) {
                        MGR->splitT = MGR->splitT + 0.05f;
                        if (MGR->splitT >= 1.0f) {
                            MGR->splitT = 1.0f;
                            MGR->busy = 0;
                        }
                        frame = (s32) (120.0f - (MGR->splitT * 120.0f)); if (arg0) {}
                        MGR->split = frame;
                        MGR->wins[0]->centerX = frame;
                        r34 = (char *)MGR->wins[0];
                        if (SELWIN(r34)->centerX == 0) {
                            SELWIN_VIEW(SELWIN(r34))->height = 0;
                        } else {
                            SELWIN_VIEW(SELWIN(r34))->originY = 0;
                            SELWIN_VIEW(MGR->wins[0])->vpY = 0;
                            SELWIN_VIEW(MGR->wins[0])->height = MGR->split;
                            SELWIN_VIEW(MGR->wins[0])->vpH = MGR->split;
                        }
                        MGR->wins[1]->centerX = 0xF0 - MGR->split;
                        r38 = (char *)MGR->wins[1];
                        if (SELWIN(r38)->centerX == 0) {
                            SELWIN_VIEW(SELWIN(r38))->height = 0;
                        } else {
                            SELWIN_VIEW(SELWIN(r38))->originY = MGR->split;
                            SELWIN_VIEW(MGR->wins[1])->vpY = MGR->split;
                            SELWIN_VIEW(MGR->wins[1])->height = 0xF0 - MGR->split;
                            SELWIN_VIEW(MGR->wins[1])->vpH = 0xF0 - MGR->split;
                            SELWIN_VIEW(MGR->wins[1])->aspect =
                                1.333f + (1.3333 * (f64) (1.0f - MGR->splitT));
                            MGR->wins[1]->fovBase = 41.0f + (5.0f * MGR->splitT);
                        }
                    }
                }
            } else {
                if ((MGR->split == 0) || (MGR->split == 0xF0)) {
                    if (MGR->waitMask == 1) {
                        MGR->busy = 1;
                        MGR->zoomWin = 1;
                        MGR->splitT = 0.0f;
                    }
                    if (MGR->waitMask == 2) {
                        MGR->busy = 1;
                        MGR->zoomWin = 2;
                        MGR->splitT = 0.0f;
                    }
                }
                if (MGR->busy != 0) {
                    if (MGR->zoomWin == 1) {
                        MGR->splitT = MGR->splitT + 0.05f;
                        if (MGR->splitT >= 1.0f) {
                            MGR->splitT = 1.0f;
                            MGR->busy = 0;
                            MGR->zoomWin = 0;
                        }
                        frame2 = (s32) (240.0f - (MGR->splitT * 120.0f)); if (arg0) {}
                        MGR->split = frame2;
                        MGR->wins[0]->centerX = frame2;
                        r34b = (char *)MGR->wins[0];
                        if (SELWIN(r34b)->centerX == 0) {
                            SELWIN_VIEW(SELWIN(r34b))->height = 0;
                        } else {
                            SELWIN_VIEW(SELWIN(r34b))->originY = 0;
                            SELWIN_VIEW(MGR->wins[0])->vpY = 0;
                            SELWIN_VIEW(MGR->wins[0])->height = MGR->split;
                            SELWIN_VIEW(MGR->wins[0])->vpH = MGR->split;
                            SELWIN_VIEW(MGR->wins[0])->aspect =
                                1.333f + (1.3333 * (f64) MGR->splitT);
                            MGR->wins[0]->fovBase = 41.0f + (5.0f * (1.0f - MGR->splitT));
                        }
                        MGR->wins[1]->centerX = 0xF0 - MGR->split;
                        r38b = (char *)MGR->wins[1];
                        if (SELWIN(r38b)->centerX == 0) {
                            SELWIN_VIEW(SELWIN(r38b))->height = 0;
                        } else {
                            SELWIN_VIEW(SELWIN(r38b))->originY = MGR->split;
                            SELWIN_VIEW(MGR->wins[1])->vpY = MGR->split;
                            SELWIN_VIEW(MGR->wins[1])->height = 0xF0 - MGR->split;
                            SELWIN_VIEW(MGR->wins[1])->vpH = 0xF0 - MGR->split;
                            SELWIN_VIEW(MGR->wins[1])->aspect =
                                1.333f + (1.3333 * (f64) MGR->splitT);
                            MGR->wins[1]->fovBase = 41.0f + (5.0f * (1.0f - MGR->splitT));
                        }
                    }
                    if (MGR->zoomWin == 2) {
                        MGR->splitT = MGR->splitT + 0.05f;
                        if (MGR->splitT >= 1.0f) {
                            MGR->splitT = 1.0f;
                            MGR->busy = 0;
                            MGR->zoomWin = 0;
                        }
                        frame3 = (s32) (MGR->splitT * 120.0f); if (arg0) {}
                        MGR->split = frame3;
                        MGR->wins[0]->centerX = frame3;
                        r34c = (char *)MGR->wins[0];
                        if (SELWIN(r34c)->centerX == 0) {
                            SELWIN_VIEW(SELWIN(r34c))->height = 0;
                        } else {
                            SELWIN_VIEW(SELWIN(r34c))->originY = 0;
                            SELWIN_VIEW(MGR->wins[0])->vpY = 0;
                            SELWIN_VIEW(MGR->wins[0])->height = MGR->split;
                            SELWIN_VIEW(MGR->wins[0])->vpH = MGR->split;
                            SELWIN_VIEW(MGR->wins[0])->aspect =
                                1.333f + (1.3333 * (f64) MGR->splitT);
                            MGR->wins[0]->fovBase = 41.0f + (5.0f * (1.0f - MGR->splitT));
                        }
                        MGR->wins[1]->centerX = 0xF0 - MGR->split;
                        r38c = (char *)MGR->wins[1];
                        if (SELWIN(r38c)->centerX == 0) {
                            SELWIN_VIEW(SELWIN(r38c))->height = 0;
                        } else {
                            SELWIN_VIEW(SELWIN(r38c))->originY = MGR->split;
                            SELWIN_VIEW(MGR->wins[1])->vpY = MGR->split;
                            SELWIN_VIEW(MGR->wins[1])->height = 0xF0 - MGR->split;
                            SELWIN_VIEW(MGR->wins[1])->vpH = 0xF0 - MGR->split;
                            SELWIN_VIEW(MGR->wins[1])->aspect =
                                1.333f + (1.3333 * (f64) MGR->splitT);
                            MGR->wins[1]->fovBase = 41.0f + (5.0f * (1.0f - MGR->splitT));
                        }
                    }
                }
            }
        }
        if (MGR->wins[0]->centerX != 0) {
            MGR->shownMask = MGR->shownMask | 1;
        } else {
            MGR->shownMask = MGR->shownMask & ~1;
        }
        if (MGR->wins[1]->centerX != 0) {
            MGR->shownMask = MGR->shownMask | 2;
        } else {
            MGR->shownMask = MGR->shownMask & ~2;
        }
        MGR->prevWaitMask = MGR->waitMask;
    }
    gen_imp_Node_update(arg0);
}
