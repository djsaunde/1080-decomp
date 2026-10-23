/* Native original-compiler source port from src/game_libs/game_libs_post0b.c. Target bytes unchanged. */
#include "context_game_libs_post0b.h"
void gl_func_00044918(GlResetState *arg0) {
    int *p;
    int i;
    int busy;
    p = arg0->table_2AC;
    arg0->field_5D0 = 0;
    for (i = 0; i < 50; i++) { p[i] = 0; }
    p[50] = 0;
    arg0->field_210 = 0;
    gl_44918_init_43EAC(&arg0->field_294);
    gl_44918_set_46788(arg0, 0);
    p = (int *)arg0->field_218;
    do { do { do { do { busy = (((GlPendingWork *)p)->p1C != 0 || ((GlPendingWork *)p)->p04 != 0 || ((GlPendingWork *)p)->p0C != 0); } while (0); } while (0); } while (0); } while (0);
    while (busy) {
        do { do { do { do { do { do { do { do { gl_44918_yield_732C4(); } while (0); } while (0); } while (0); } while (0); } while (0); } while (0); } while (0); } while (0);
        busy = (((GlPendingWork *)p)->p1C != 0 || ((GlPendingWork *)p)->p04 != 0 || ((GlPendingWork *)p)->p0C != 0);
    }
    p = (int *)&arg0->flags_138;
    gl_44918_reset_4E00C(arg0);
    arg0->field_24C = *(f32 *)(D_44918_flt + 0x1B38);
    arg0->field_250 = 0;
    arg0->field_230 = 0;
    arg0->flags_138.a = 0;
    arg0->flags_138.b = 0;
    ((Pair2 *)p)->b &= ~0x10000;
    D_44918_fzero = 0.0f;
    ((Pair2 *)p)->b &= ~2;
    ((Pair2 *)p)->b &= ~0x1000;
    gl_44918_reset_56814(arg0->field_214);
    ((Pair2 *)p)->b |= 1;
    ((Pair2 *)p)->b |= 0x4000;
    ((Pair2 *)p)->b |= 0x8000;
    ((Pair2 *)p)->b |= 0x20000;
    ((Pair2 *)p)->b |= 0x80;
    gl_44918_notify_4DD4C((char *)&D_00000000 + 0x1FE70);
    gl_44918_range_3523C(&D_44918_arg0c, arg0->field_240->field_FC,
                     D_44918_ma * D_44918_mb * 6);
}
