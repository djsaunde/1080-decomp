#include "gdl/gl_data.h"
#include "uso_call.h"
/* -O1 donor for gl_osContInit — 92-insn one-shot shutdown/timer-wait driver
 * (frame 0x70, no s-regs). PROVEN EXACT 92/92 at IDO 7.1 -O1 (2026-07-08
 * standalone probe; plain -O1, no -g3). Shape: guard on a state flag, flag=1,
 * time = u64_now(); if (time < 0x165A0BC) { osCreateMesgQueue(&q,&msg,1);
 * osSetTimer(&timer, 0x165A0BC - time, 0, &q, &msg); osRecvMesg(&q,&msg,1); }
 * then a byte-state=4 / callback teardown ladder, byte-state=0, final
 * register call, return ret (the osSetTimer 64-bit countdown pair (a2,a3) +
 * 0LL interval in stack slots 16/20 + &q/&msg at 24/28 match exactly).
 * Levers:
 *   (1) decl order msg/ret/time/timer[8]/q[6] = homes 0x6C/0x68/0x60/0x40/0x28;
 *   (2) u64 `time` local + (u64)0x165A0BC compare/subtract = the hi/lo
 *       bne+sltu pair and the 64-bit subu/sltu/subu countdown;
 *   (3) byte stores need the DIRECT char global D_00000000_b ($at-fused sb;
 *       *(char*)&D_00000000 materializes the address, +1 insn);
 *   (4) final call takes two DISTINCT blank syms (&D_00000000_c, &D_00000000_d)
 *       to defeat lui/addiu CSE (one sym = single materialization + or copy);
 *   (5) gl_func_00000000_ll = distinct blank extern with u64 return (v0/v1).
 * Spliced into game_libs_post1b.c.o via REPLACE_FUNC_BODY. */
extern int gl_func_00000000();
extern unsigned long long gl_func_00000000_ll();
extern int D_00000000;
extern char D_00000000_b;
extern int D_00000000_c;
extern int D_00000000_d;

int gl_osContInit(int a0, int a1, int a2) {
    int msg;
    int ret;
    unsigned long long time;
    int timer[8];
    int q[6];

    ret = 0;
    if (gl___osContinitialized != 0) {
        return 0;
    }
    gl___osContinitialized = 1;
    time = XCALL(gl_func_00000000_ll, osGetTime)();
    if (time < (unsigned long long)0x165A0BC) {
        CALL(gl_osCreateMesgQueue)(q, &msg, 1);
        CALL(gl_osSetTimer)(timer, (unsigned long long)0x165A0BC - time, (unsigned long long)0, q, &msg);
        CALL(gl_osRecvMesg)(q, &msg, 1);
    }
    D_00000000_b = 4;
    CALL(__osPackReadData)(0);
    ret = CALL(gl___osSiRawStartDma)(1, &gl___osContPifRam);
    CALL(gl_osRecvMesg)(a0, &msg, 1);
    ret = CALL(gl___osSiRawStartDma)(0, &gl___osContPifRam);
    CALL(gl_osRecvMesg)(a0, &msg, 1);
    CALL(osContGetReadData)(a1, a2);
    D_00000000_b = 0;
    CALL(gl___osSiCreateAccessQueue)();
    CALL(gl_osCreateMesgQueue)(&D_00000000_c, &D_00000000_d, 1);
    return ret;
}
