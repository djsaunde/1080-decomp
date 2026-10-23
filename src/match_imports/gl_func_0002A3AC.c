/* Native original-compiler source port from src/game_libs/game_libs_post.c. Target bytes unchanged. */
#include "context_game_libs_post.h"
int gl_func_0002A3AC(Gl2AD1CState *base, int idx) {
    char *slot = (char *)base + idx * 4;
    Gl2AD1C *no;
    if (*(Gl2AD1C **)(slot + 0x50) == 0) {
        *(Gl2AD1C **)(slot + 0x50) = gl_a3ac_pool_take((char *)&D_00000000 + 0x5368);
        if (*(Gl2AD1C **)(slot + 0x50) == 0) {
            *(Gl2AD1C **)(slot + 0x50) = 0;
            return -1;
        }
    } else {
        gl_a3ac_operation6(*(Gl2AD1C **)(slot + 0x50));
    }
    no = *(Gl2AD1C **)(slot + 0x50);
    no->w50 = base;
    *(GlCommandBinding *)&no->b18 = *(GlCommandBinding *)&base->b8C;
    ((GlPooledFlags *)&no->b0)->bit7 = 1;
    ((GlPooledFlags *)&no->b0)->bit6 = 0;
    ((GlPooledFlags *)&no->b0)->bit5 = 0;
    ((GlPooledFlags *)&no->b0)->bit4 = 0;
    ((GlPooledFlags *)&no->b0)->bit3 = 0;
    ((GlPooledFlags *)&no->b0)->bit2 = 0;
    ((GlPooledFlags *)&no->b0)->bit1 = 0;
    no->b18 = 0;
    ((GlPooledFlags *)&no->b0)->bit0 = 0;
    no->b1 = 0;
    no->b20 = 0;
    no->b6C = 0;
    no->b3 = 128;
    no->b5 = 64;
    no->h10 = 0;
    no->h8 = 0;
    no->hA = 0;
    no->hC = 0;
    no->w2C = 0;
    no->w48 = 0;
    no->b2 = 255;
    no->f30 = 1.0f;
    no->f34 = 1.0f;
    no->f38 = 0.0f;
    return 0;
}
