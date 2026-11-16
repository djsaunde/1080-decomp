/* Native original-compiler source port from src/game_libs/game_libs_post.c. Target bytes unchanged. */
#include "context_game_libs_post.h"
void AudioSeq_InitSequencePlayer(Obj26D64 *a0) {
    int i;
    int j;
    for (i = 0; i != 16; i++) {
        *((s32 **)&a0->w38[i]) = (s32 *)((char *)&D_00000000 + 0x5280);
    }
#ifdef PORT
    a0->flags &= ~AUDIO_FLAGBIT(0); /* enabled */
    a0->flags &= ~AUDIO_FLAGBIT(2); /* muted */
    a0->flags &= ~AUDIO_FLAGBIT(4); /* fontDmaInProgress */
    a0->flags &= ~AUDIO_FLAGBIT(3); /* seqDmaInProgress */
    a0->flags &= ~AUDIO_FLAGBIT(7); /* applyBend */
#else
    a0->flags &= ~0x80;
    a0->flags &= 0xDF;
    a0->flags &= 0xF7;
    a0->flags &= 0xEF;
    a0->flags &= 0xFE;
#endif
    for (j = 0; j < 8; j++) {
        a0->b158[j] = -1;
    }
    *((u8 *)a0 + 3) = 96;
    a0->f2C = 1.0f;
    a0->f34 = 1.0f;
    gl_2D224_init_links((char *)a0 + 0x9C);
    gl_2D224_reset_record(a0);
}
