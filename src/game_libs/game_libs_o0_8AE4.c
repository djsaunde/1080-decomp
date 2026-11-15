#include "common.h"
#include "gdl/gl_data.h"
#include "uso_call.h"

extern int gl_func_00000000();
extern char D_00000000;
extern int D_00000000_a;
extern int gInputMgr_8AE4_mode;  /* = 0x34 in undefined_syms_auto.txt; bare-symbol read
 * is reloc-aware MATCHED (fuzzy 100%). The HI16/LO16 reloc to gInputMgr_8AE4_mode bakes
 * %lo=0x34 into the lw at link; objdiff resolves it. (objdump -d on the .o shows
 * `lw 0` because it doesn't apply the LO16 reloc -- that is NOT a real diff.) */

void app_launchGenProc(int a0, int a1, int a2, int a3) {
    register int rv;

    CALL(game_loadUso)(&gInputMgr, *(int*)((char*)&D_00000000_a + 0x0));
    CALL(game_loadUso)(&gInputMgr, *(int*)((char*)&D_00000000_a + 0xC));

    if (gInputMgr_8AE4_mode == 3) {
        CALL(game_loadUso)(&gInputMgr, *(int*)((char*)&D_00000000_a + 0x10));
    }

    if (gInputMgr_8AE4_mode != 2) {
        CALL(game_loadUso)(&gInputMgr, *(int*)((char*)&D_00000000_a + 0x14));
    }

    if ((gInputMgr_8AE4_mode != 3) && (gInputMgr_8AE4_mode != 5)) {
        CALL(game_loadUso)(&gInputMgr, *(int*)((char*)&D_00000000_a + 0x18));
    }

    CALL(snd_playSeq_000120A8)(&gInputMgr, 3, 0, 0);
    rv = CALL(GenSelectChar_ctor)(0, a1, a2, a3);
    CALL(app_runProcess)(a0, 0, rv);
}
