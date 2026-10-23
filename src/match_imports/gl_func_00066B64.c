/* Native original-compiler source port from src/game_libs/game_libs_post1b.c. Target bytes unchanged. */
#include "context_game_libs_post1b.h"
void gl_func_00066B64(int argument) {
    gl_import_66B64_osInitialize();
    gl_import_66B64_osCreateThread((char *)&gl_storage_66B64_threadRegion + 0x41310, 1,
                                 gl_import_66B64_threadEntry, argument,
                                 (char *)&gl_storage_66B64_threadRegion + 0x415C0, 1);
    gl_storage_66B64_threadRegion[0x414C0 / 8] = 0x1234567812345678ULL;
    gl_import_66B64_osStartThread((char *)&gl_storage_66B64_threadRegion + 0x41310);
}
