/* IDO 7.1 NCC (C++) -O0 donors for app_createGame_00008FFC and app_createGame_00009100
 * (0x104 / 65 words each). game_libs is C++: compiled by edg the -O0 bodies
 * end without cfe's closing-brace `b epilogue; nop` marker, so the
 * "-O0 return-value dead-double-b" gap of the cfe build (67 words) is gone.
 * The bodies are the cfe NM bodies unchanged (plain obj home + register
 * s0/s1/s2 aliases; gProcUsoPaths_8FFC_base defeats the &D CSE for the +8 read).
 * Spliced into game_libs_mid.c.o via REPLACE_FUNC_BODY. */
extern "C" {
int gl_func_00000000(...);
extern char D_00000000;
#include "../../include/gdl/gl_data.h"  /* object aliases (NCC donors build without -I include) */
extern int gInputMgr_0000004C, D_00000050, gInputMgr_00000064;
extern unsigned char gInputMgr_0000017D;
extern unsigned char gInputMgr_00000179, gInputMgr_0000017A, gInputMgr_0000017B, gInputMgr_00000181, gInputMgr_00000182, gInputMgr_00000183;
extern char gProcUsoPaths_8FFC_base;

int app_createGame_00008FFC(int a0, int a1, int a2) {
    int obj;
    register char *base;
    register int *o;
    register char *tmp;

    /* RAW: donor-mirror: C++ donor (NCC Cfront); its local mirror structs cannot include the C headers */
    gl_func_00000000(&gInputMgr, *(int *)((char *)&gProcUsoPaths_8FFC_base + 8));
    gl_func_00000000(gInputMgr_0000004C, gInputMgr_0000017D);
    D_00000050 = D_00000050 & 0x20;
    obj = gl_func_00000000(&gInputMgr, gInputMgr_00000064, 5, a2);
    base = (char *)&gInputMgr;
    o = (int *)obj;
    tmp = base + 0x10;
    gl_func_00000000(tmp, o);
    if (o[0x14 / 4] != 0) {
        o[0x4 / 4] = 1;
    }
    tmp = base;
    o[0x14 / 4] = (int)tmp;
    gl_func_00000000(&gInputMgr, 0);
    (void)a0;
    (void)a1;
    return obj;
}

int app_createGame_00009100(int a0, int a1, int a2) {
    int obj;
    register char *base;
    register int *o;
    register char *tmp;

    gInputMgr_00000181 = gInputMgr_00000179;
    gInputMgr_00000182 = gInputMgr_0000017A;
    gInputMgr_00000183 = gInputMgr_0000017B;
    gl_func_00000000(gInputMgr_0000004C, gInputMgr_0000004C);
    obj = gl_func_00000000(&gInputMgr, gInputMgr_00000064, 4, a2);
    base = (char *)&gInputMgr;
    o = (int *)obj;
    tmp = base + 0x10;
    gl_func_00000000(tmp, o);
    if (o[0x14 / 4] != 0) {
        o[0x4 / 4] = 1;
    }
    tmp = base;
    o[0x14 / 4] = (int)tmp;
    gl_func_00000000(&gInputMgr, 0);
    (void)a0;
    (void)a1;
    return obj;
}
}
