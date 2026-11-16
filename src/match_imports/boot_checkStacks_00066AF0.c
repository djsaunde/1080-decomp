/* Native original-compiler source port from src/game_libs/game_libs_post1b.c. Target bytes unchanged. */
#include "context_game_libs_post1b.h"
void boot_checkStacks_00066AF0(void) {
#ifdef PORT
    /* The port runs the N64 threads on the host stack: no thread stacks, no canaries to check
     * (boot_startThread_00066B64, which writes them, is never run; plan 17.8). */
    return;
#endif
    if (gl_storage_66B64_threadRegion[0x3F310 / 8] != 0x1234567812345678ULL) {
        gl_import_66AF0_report(gl_ro_66AF0 + 0x21BC);
    }
    if (gl_storage_66B64_threadRegion[0x414C0 / 8] != 0x1234567812345678ULL) {
        gl_import_66AF0_report(gl_ro_66AF0 + 0x21D8);
    }
}
