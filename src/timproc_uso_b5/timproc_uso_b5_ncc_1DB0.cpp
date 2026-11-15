/* IDO 7.1 NCC (C++) donor for SelectionMgr_setSplit_00001DB0 (0x164 / 89 words).
 * timproc_b5 is C++: edg keeps the SOURCE operand order of commutative FP ops
 * (cfe canonicalizes the constant to the right), so `C0 + B8 * t` and
 * `41.0f + 5.0f * f` emit the target's const-first mul/add operands.
 * Spliced into timproc_uso_b5.c.o via REPLACE_FUNC_BODY. */
extern "C" {
extern char timproc_uso_b5_D_807FF0D8, timproc_uso_b5_D_807FF0E0;
#define D_dbl_1DB0_B8 (*(double *)(&timproc_uso_b5_D_807FF0D8 + 0xB8))
#define D_dbl_1DB0_C0 (*(double *)(&timproc_uso_b5_D_807FF0E0 + 0xC0))
typedef int s32; typedef float f32; typedef double f64;
#include "../../include/game/select.h"  /* NCC donors build without -I include */
#define MGR ((SelectionMgr *)a0)
void SelectionMgr_setSplit_00001DB0(char *a0) {
    char *t;
    char *u;

    MGR->wins[0]->centerX = MGR->split;
    t = (char *)MGR->wins[0];
    if (SELWIN(t)->centerX == 0) {
        SELWIN_VIEW(SELWIN(t))->height = 0;
    } else {
        SELWIN_VIEW(SELWIN(t))->originY = 0;
        SELWIN_VIEW(MGR->wins[0])->vpY = 0;
        SELWIN_VIEW(MGR->wins[0])->height = MGR->split;
        SELWIN_VIEW(MGR->wins[0])->vpH = MGR->split;
        SELWIN_VIEW(MGR->wins[0])->aspect =
            D_dbl_1DB0_C0 + D_dbl_1DB0_B8 * (f64)(1.0f - MGR->splitT);
        MGR->wins[0]->fovBase = 41.0f + 5.0f * MGR->splitT;
    }
    MGR->wins[1]->centerX = 0xF0 - MGR->split;
    u = (char *)MGR->wins[1];
    if (SELWIN(u)->centerX == 0) {
        SELWIN_VIEW(SELWIN(u))->height = 0;
        return;
    }
    SELWIN_VIEW(SELWIN(u))->originY = MGR->split;
    SELWIN_VIEW(MGR->wins[1])->vpY = MGR->split;
    SELWIN_VIEW(MGR->wins[1])->height = 0xF0 - MGR->split;
    SELWIN_VIEW(MGR->wins[1])->vpH = 0xF0 - MGR->split;
}
}
