/* Native original-compiler source port from src/game_libs/game_libs_post1b.c. Target bytes unchanged. */
#include "context_game_libs_post1b.h"
void gl_func_000671E4(GlControllerState *self) {
    char *message = (char *)&D_00000000;
    if (1) {
        message += 0x416E0;
    }
    self->field_13E8 = 0;
    *(int *)message = 1;
    gl_671E4_osSendMesg(&self->queue_11B0, message, 1);
}
