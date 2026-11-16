/* Native original-compiler source port from src/h2hproc_uso/h2hproc_uso.c. Target bytes unchanged. */
#include "context_h2hproc_uso.h"
#include "game/modeproc.h"

/* the display's H2HRace (H2HWinsDisplay.race) */
#define RACE H2HRACE(H2HWINS(self)->race)
extern char snw_h2h_1360_text_data;
extern char snw_h2h_1360_import149;
void h2h_WinsDisplay_display_00001360(int *self) {
    char scratch[64];
    int *vtable;
    int *v_call;
    int retval;
    int gate;
    char c;
    H2HWINS(self)->frame += 1;
    gate = NODE(H2HWINS(self)->fader)->vptr;
    ((void(*)(int))VT_SLOT(gate, 2).fn)(H2HWINS(self)->fader + VT_SLOT(gate, 2).delta);
    h2hproc_1360_import_157(RACE->race.spriteSet);
    h2hproc_1360_import_158(RACE->race.spriteSet);
    gate = (RACE->winner == 0)
         ? H2HWINS(self)->frame : 0x10;
    if (gate & 0x10) {
        h2hproc_1360_import_159(scratch, &snw_h2h_1360_text_data + 0x3FC);
        retval = h2hproc_1360_import_160(RACE->race.spriteSet, 80, scratch);
        h2hproc_1360_import_161(RACE->race.spriteSet, retval, 30, scratch);
        {
            retval = ((int *)RACE->wins)[0]; if (retval != 1) c = 115; else c = 0; h2hproc_1360_import_159(scratch, &snw_h2h_1360_text_data + 0x400, retval, c);
        }
        retval = h2hproc_1360_import_160(RACE->race.spriteSet, 80, scratch);
        h2hproc_1360_import_161(RACE->race.spriteSet, retval, 80, scratch);
    }
    gate = (RACE->winner == 1)
         ? H2HWINS(self)->frame : 0x10;
    if (gate & 0x10) {
        h2hproc_1360_import_159(scratch, &snw_h2h_1360_text_data + 0x40C);
        retval = h2hproc_1360_import_160(RACE->race.spriteSet, 240, scratch);
        h2hproc_1360_import_161(RACE->race.spriteSet, retval, 30, scratch);
        {
            retval = ((int *)RACE->wins)[1]; if (retval != 1) c = 115; else c = 0; h2hproc_1360_import_159(scratch, &snw_h2h_1360_text_data + 0x410, retval, c);
        }
        retval = h2hproc_1360_import_160(RACE->race.spriteSet, 240, scratch);
        h2hproc_1360_import_161(RACE->race.spriteSet, retval, 80, scratch);
    }
    h2hproc_1360_import_162(&snw_h2h_1360_import149);
    v_call = (int*)&H2HWINS(self)->sprites[0];
    h2hproc_1360_import_163(v_call);
    h2hproc_1360_import_164(v_call, 60, 64, 3);
    v_call = (int*)&H2HWINS(self)->sprites[1];
    h2hproc_1360_import_163(v_call);
    vtable = (int *)H2HWINS(self)->sprites[0].image;
    if (1) {}
    retval = *(short*)((char*)vtable + 0x20);
    h2hproc_1360_import_164(v_call, retval + 68, 64, 3);
    v_call = (int*)&H2HWINS(self)->sprites[2];
    h2hproc_1360_import_163(v_call);
    h2hproc_1360_import_164(v_call, 220, 64, 3);
    v_call = (int*)&H2HWINS(self)->sprites[3];
    h2hproc_1360_import_163(v_call);
    vtable = (int *)H2HWINS(self)->sprites[2].image;
    if (1) {}
    retval = *(short*)((char*)vtable + 0x20);
    h2hproc_1360_import_164(v_call, retval + 228, 64, 3);
    h2hproc_1360_import_165(self);
}
