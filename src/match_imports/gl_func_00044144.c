/* Native original-compiler source port from src/game_libs/game_libs_post0b.c. Target bytes unchanged. */
#include "context_game_libs_post0b.h"
void gl_func_00044144(void *arg0) {
    extern char gl_d44144_k1, gl_d44144_k2, gl_d44144_k2c, gl_d44144_k3,
        gl_d44144_k3c, gl_d44144_k4, gl_d44144_k5, gl_d44144_k5c,
        gl_d44144_k6, gl_d44144_r1, gl_d44144_r2, gl_d44144_r3, gl_d44144_r4,
        gl_d44144_s1a, gl_d44144_s1b, gl_d44144_s2a, gl_d44144_s2b,
        gl_d44144_s3a, gl_d44144_s3b, gl_d44144_s4a, gl_d44144_s4b,
        gl_d44144_gp, gl_d44144_t2, gl_d44144_t3, gl_d44144_t4;
    extern char gl_d44144_f730a, gl_d44144_f730b, gl_d44144_f730c, gl_d44144_f730d;
    extern char gl_d44144_t1;
    extern int gl_d44144_v13;
    char sp38[0x100];
    s32 *var_v1;
    s32 *var_a2;
    struct St4C928 *temp_v0_9;
    gl_44144_register_int(&gl_d44144_k1, (&gl_44144_key_FD34 + 0x1FD34), 0);
    gl_44144_register_float(&gl_d44144_k2, (&gl_44144_key_FD40 + 0x1FD40), (float *)(&gl_d44144_k2c + 0x24C), 0.0f, 1.0f, 0);
    gl_44144_register_word(&gl_d44144_k3, (&gl_44144_key_FD50 + 0x1FD50), &gl_d44144_k3c + 0x250);
    gl_44144_register_word(&gl_d44144_k4, (&gl_44144_key_FD58 + 0x1FD58), (&gl_44144_key_F970 + 0x1F970));
    gl_44144_register_range(&gl_d44144_k5, (&gl_44144_key_FD64 + 0x1FD64), &gl_d44144_k5c + 0x0, 0x400, 0x19000, 0);
    gl_44144_setup(&gl_d44144_k6 + 0x294);
    var_a2 = gl_44144_alloc(8);
    if (var_a2 != 0) {
        var_v1 = var_a2;
        if ((var_a2 != 0) || (var_v1 = gl_44144_alloc(4), var_v1 != 0)) {
            *var_v1 = (s32)(&gl_d44144_f730a + 0x1F730);
        }
        ((*(s32 *)((char *)var_a2 + 0x0)) = (s32)&gl_d44144_s1a,
         (*(s32 *)((char *)var_a2 + 0x4)) = (s32)&gl_d44144_s1b);
    }
    gl_44144_register_binding(&gl_d44144_r1, (&gl_44144_key_FD6C + 0x1FD6C), var_a2, 1);
    var_a2 = gl_44144_alloc(8);
    if (var_a2 != 0) {
        var_v1 = var_a2;
        if ((var_a2 != 0) || (var_v1 = gl_44144_alloc(4), var_v1 != 0)) {
            *var_v1 = (s32)(&gl_d44144_f730b + 0x1F730);
        }
        ((*(s32 *)((char *)var_a2 + 0x0)) = (s32)&gl_d44144_s2a,
         (*(s32 *)((char *)var_a2 + 0x4)) = (s32)&gl_d44144_s2b);
    }
    gl_44144_register_binding(&gl_d44144_r2, (&gl_44144_key_FD78 + 0x1FD78), var_a2, 1);
    var_a2 = gl_44144_alloc(8);
    if (var_a2 != 0) {
        var_v1 = var_a2;
        if ((var_a2 != 0) || (var_v1 = gl_44144_alloc(4), var_v1 != 0)) {
            *var_v1 = (s32)(&gl_d44144_f730c + 0x1F730);
        }
        ((*(s32 *)((char *)var_a2 + 0x0)) = (s32)&gl_d44144_s3a,
         (*(s32 *)((char *)var_a2 + 0x4)) = (s32)&gl_d44144_s3b);
    }
    gl_44144_register_binding(&gl_d44144_r3, (&gl_44144_key_FD88 + 0x1FD88), var_a2, 0);
    var_a2 = gl_44144_alloc(8);
    if (var_a2 != 0) {
        var_v1 = var_a2;
        if ((var_a2 != 0) || (var_v1 = gl_44144_alloc(4), var_v1 != 0)) {
            *var_v1 = (s32)(&gl_d44144_f730d + 0x1F730);
        }
        ((*(s32 *)((char *)var_a2 + 0x0)) = (s32)&gl_d44144_s4a,
         (*(s32 *)((char *)var_a2 + 0x4)) = (s32)&gl_d44144_s4b);
    }
    gl_44144_register_binding(&gl_d44144_r4, (&gl_44144_key_FD94 + 0x1FD94), var_a2, 1);
    temp_v0_9 = *(struct St4C928 **)(&gl_d44144_gp + 0x218);
    gl_44144_format(sp38, (&gl_44144_key_FDA4 + 0x1FDA4), temp_v0_9->p3C, temp_v0_9->p40, temp_v0_9->p24, temp_v0_9->p1C, temp_v0_9->p20, *(volatile s32 *)&gl_d44144_t1);
    gl_44144_message(&gl_d44144_t2, sp38);
    gl_44144_format(sp38, (&gl_44144_key_FDD4 + 0x1FDD4), gl_d44144_v13);
    gl_44144_message(&gl_d44144_t3, sp38);
    gl_44144_finish(&gl_d44144_t4);
    gl_44144_dispatch(arg0);
}
