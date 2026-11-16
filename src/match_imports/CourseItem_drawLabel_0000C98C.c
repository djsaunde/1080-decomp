/* Native original-compiler source port from src/timproc_uso_b5/timproc_uso_b5.c. Target bytes unchanged. */
#include "context_timproc_uso_b5.h"
#include "game/input.h" /* InputMgr (GameFlow): nextFlowMode, backup */
void CourseItem_drawLabel_0000C98C(char *arg0, char *arg1, volatile int arg2, int arg3) {
    float ones[4];
    int a2v;
    int sp30;
    int v0r, v1;
    ones[0] = 1.0f;
    ones[1] = 1.0f;
    ones[2] = 1.0f;
    ones[3] = 1.0f;
    a2v = (SELWIN_VIEW(SELWIN(arg1))->originY + SELWIN_VIEW(SELWIN(arg1))->height) - SELITEM(arg0)->parms.ypn.value;
    SelItem_drawSpriteColorEx_000040BC((int)arg0, SELITEM(arg0)->parms.xpn.value, a2v, arg2, arg3,
        SELITEM(arg0)->info.ixn.value * 24 + (int)(&timproc_uso_b5_D_807FF640 + 0x1C0), 0xFF);
    if (*(int *)&import_8002022C != 0) {
        SelItem_drawSpriteWhite_00003FF8((int)arg0, SELITEM(arg0)->parms.xpn.value + SELITEM(arg0)->parms.jnx.value,
            a2v + (unsigned int)SELITEM(arg0)->parms.jny.value, /* u32 add: the signed one swaps t1 / t2 */
            SELITEM(arg0)->info.ixn.value * 24 + (int)(&timproc_uso_b5_D_807FF700 + 0x280), 0xFF);
    }
    sp30 = 0;
    v0r = gen_imp_Backup_ghostGetMode((char *)INPUT_MGR(import_80020098).backup + 4);
    v1 = INPUT_MGR(import_80020098).nextFlowMode;
    if (v1 == 6) {
        if (v0r == 2) {
            if (gen_imp_Backup_ghostGetCourse((char *)INPUT_MGR(import_80020098).backup + 4) == SELITEM(arg0)->id) {
                sp30 = 1;
            }
        }
    } else if (v1 == 4) {
        if (v0r == 1) {
            if (gen_imp_Backup_ghostGetCourse((char *)INPUT_MGR(import_80020098).backup + 4) == SELITEM(arg0)->id) {
                sp30 = 1;
            }
        }
    }
    if (sp30 != 0) {
        SelItem_drawSpriteWhite_00003FF8((int)arg0, SELITEM(arg0)->parms.xpn.value + 0x54, a2v - 0x78,
            (int)(&timproc_uso_b5_D_807FFB50 + 0x6D0), 0xFF);
    }
}
