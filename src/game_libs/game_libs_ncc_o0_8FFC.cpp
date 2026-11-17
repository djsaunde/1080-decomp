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
#include "../../include/uso_call.h"  /* NCC donors build without -I include */
extern int gInputMgr_0000004C, D_00000050, gInputMgr_00000064; /* UNNAMED: data-base D_00000050 = 0x50 is Bss sym272 +0x50 here (the &= 0x20 in app_createGame_00008FFC) but RoData +0x50 in bootup_uso.c (snd_calcEmitterDistPan_000003F8); one pinned value for two objects, so a pure rename would merge them: it needs a per-site split */
extern unsigned char gInputMgr_0000017D;
extern unsigned char gInputMgr_00000179, gInputMgr_0000017A, gInputMgr_0000017B, gInputMgr_00000181, gInputMgr_00000182, gInputMgr_00000183;
extern char gProcUsoPaths_8FFC_base;

int app_createGame_00008FFC(int a0, int a1, int a2) {
    int obj;
    register char *base;
    register int *o;
    register char *tmp;

    CALL(game_loadUso)(&gInputMgr, (int)((char **)&gProcUsoPaths_8FFC_base)[2]); /* gProcUsoPaths[2] = "rom/gamegfx.uso" */
    CALL(snd_startPlayer2Seq_0000027C)(gInputMgr_0000004C, gInputMgr_0000017D);
    D_00000050 = D_00000050 & 0x20;
    obj = CALL(game_init)(&gInputMgr, gInputMgr_00000064, 5, a2);
    base = (char *)&gInputMgr;
    o = (int *)obj;
    tmp = base + 0x10;
    CALL(list_append)(tmp, o);
    if (o[0x14 / 4] != 0) {
        o[0x4 / 4] = 1;
    }
    tmp = base;
    o[0x14 / 4] = (int)tmp;
    CALL(input_setRacePadsActive)(&gInputMgr, 0);
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
    CALL(snd_startPlayer2Seq_0000027C)(gInputMgr_0000004C, gInputMgr_0000004C);
    obj = CALL(game_init)(&gInputMgr, gInputMgr_00000064, 4, a2);
    base = (char *)&gInputMgr;
    o = (int *)obj;
    tmp = base + 0x10;
    CALL(list_append)(tmp, o);
    if (o[0x14 / 4] != 0) {
        o[0x4 / 4] = 1;
    }
    tmp = base;
    o[0x14 / 4] = (int)tmp;
    CALL(input_setRacePadsActive)(&gInputMgr, 0);
    (void)a0;
    (void)a1;
    return obj;
}
}
