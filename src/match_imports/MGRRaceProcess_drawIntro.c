/* Native original-compiler source port from src/mgrproc_uso/mgrproc_uso.c. Target bytes unchanged. */
#include "context_mgrproc_uso.h"
#include "game/modeproc.h" /* MgrRace (agent-h run 552) */
void MGRRaceProcess_drawIntro(char *arg0) {
    volatile int pt_a, pt_b, pt_c, pt_d;
    float sp50[4];
    volatile int pmid;
    int sp48;
    char *p6a8;
    char *p6d8;
    int w;
    char *q;
    volatile int pad_a, pad_b, pad_c;
    if (MGRRACE(arg0)->race.flags & 0x10000) {
        sp50[0] = 1.0f;
        sp50[1] = 1.0f;
        sp50[2] = 1.0f;
        sp50[3] = 1.0f;
        if (MGRRACE(arg0)->race.timer >= 0xB) {
            float max = mgrproc_1C90_pool_5CC.field_5CC;
            if (MGRRACE(arg0)->overlayAlpha < max) {
                MGRRACE(arg0)->overlayAlpha = MGRRACE(arg0)->overlayAlpha + mgrproc_1C90_pool_5D0.field_5D0;
            }
            if (MGRRACE(arg0)->fade7A4 < max) {
                MGRRACE(arg0)->fade7A4 = MGRRACE(arg0)->fade7A4 + mgrproc_1C90_pool_5D4.field_5D4;
            }
        } else {
            if (MGRRACE(arg0)->overlayAlpha > 0.0f) {
                MGRRACE(arg0)->overlayAlpha = MGRRACE(arg0)->overlayAlpha - mgrproc_1C90_pool_5D8.field_5D8;
            }
        }
        import_0024E608(&import_8024CAF8, (int)((0, (float)255) * MGRRACE(arg0)->overlayAlpha), sp50);
        import_0024F2C8(&import_80263D30);
        import_0024F34C(&import_80263D30, 0xA0, 0x9C, 3);
        import_0024EAA0(&import_8024CAF8, (int)((0, (float)255) * MGRRACE(arg0)->overlayAlpha), MGRRACE(arg0)->race.colours.pair[RACECOL_CS][0].value, MGRRACE(arg0)->race.colours.pair[RACECOL_CS][1].value);
        import_0024F2C8(&MGRRACE(arg0)->sprites[0]);
        import_0024F34C(&MGRRACE(arg0)->sprites[0], 0xA0, 0xB0, 3);
        while (0) { w = *(short *)(arg0 + 0x20); }
        q = (char *)MGRRACE(arg0)->sprites[2].image;
        w = ((RaceSpriteImage *)q)->width;
        p6d8 = (char *)MGRRACE(arg0)->sprites[1].image;
        sp48 = 0xA0 - ((((RaceSpriteImage *)p6d8)->width + w + 4) / 2);
        import_0024EAA0(&import_8024CAF8, (int)((0, (float)255) * MGRRACE(arg0)->overlayAlpha), MGRRACE(arg0)->race.colours.pair[RACECOL_RN][0].value, MGRRACE(arg0)->race.colours.pair[RACECOL_RN][1].value);
        p6a8 = (char *)MGRRACE(arg0)->courseList;
        if (*(int *)(p6a8 + 8) == (*(int *)(p6a8 + 4) + 1) /* MgrCourseList count == idx + 1 (member form reorders the loads) */) {
            import_0024F2C8(&MGRRACE(arg0)->sprites[2]);
            import_0024F34C(&MGRRACE(arg0)->sprites[2], sp48, 0x88, 2);
            import_0024F2C8(&MGRRACE(arg0)->sprites[1]);
            q = (char *)MGRRACE(arg0)->sprites[2].image;
            w = ((RaceSpriteImage *)q)->width;
            import_0024F34C(&MGRRACE(arg0)->sprites[1], sp48 + w + 4, 0x88, 2);
            return;
        }
        import_0024F2C8(&MGRRACE(arg0)->sprites[1]);
        import_0024F34C(&MGRRACE(arg0)->sprites[1], sp48, 0x88, 2);
        import_0024F2C8(&MGRRACE(arg0)->sprites[2]);
        q = (char *)MGRRACE(arg0)->sprites[1].image;
        w = ((RaceSpriteImage *)q)->width;
        import_0024F34C(&MGRRACE(arg0)->sprites[2], sp48 + w + 4, 0x88, 2);
    }
}
