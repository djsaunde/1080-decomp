#include "gdl/gl_data.h"
#include "uso_call.h"
#include "libultra/os_thread.h"
/* IDO 7.1 -O2 donor for dbg_faultThread (0x13C / 79 words, the
 * subsystem poll loop) and sys_hang (0x20, the hang stub).
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY; only those two
 * functions are imported.
 *
 * Why a donor: the loop is infinite, and IDO places the unreachable
 * epilogue after an infinite loop on a 32-byte boundary of the .text
 * section (`for (;;) g();` epilogues land at 0x20, 0x60, 0xA0, ... in a
 * test file). The target's epilogue sits at 0x34654 (2 nops after the
 * back branch), which is 32-aligned relative to 0x343F4: the original
 * translation unit starts at dbg_threadStateName. splat cut that unit
 * at 0x34458, so in game_libs_post0b.c the same C gets 3 nops (320
 * bytes). This file is the original unit's head: the real C of its two
 * leading functions (343F4 = game_libs_o2_343F4.c, 34458 =
 * game_libs_post0b.c, both exact) followed by 34548, which then compiles
 * to the target's 316 bytes.
 *
 * 34548 itself (34458 sibling recipe, agent-c run 147): the loop is
 * infinite, the poll result is stored BEFORE the zero test (target sw in
 * the beqz delay) and re-read after every call; the slot global is a
 * pinned symbol at 0x3BB34 (s1 = &slot, lw 0(s1)); the vtable record is a
 * distinct base-0 extern; homes 92/84/80 = sp5C, s0, the out pair in
 * that declaration order. */
extern char D_00000000;
extern int gl_func_00000000();

/* The labels are game_libs_o2_343F4.c's (agent-c run 829: the ROM table, libultra's thread states). */
int dbg_threadStateName(int a0) {
    switch (a0) {
    case 1:
        return (int)(GL_DATA_STR(0x1E3BC, "Stopped "));
    case 2:
        return (int)(GL_DATA_STR(0x1E3C8, "Runnable"));
    case 4:
        return (int)(GL_DATA_STR(0x1E3D4, "Running "));
    case 8:
        return (int)(GL_DATA_STR(0x1E3E0, "Waiting "));
    case 3: case 5: case 6: case 7: /* no state: IDO's table density */
    default:
        return (int)(GL_DATA_STR(0x1E3EC, "Unknown"));
    }
}

extern char *gl___osActiveQueue_34458_head;
extern char *gSysApi_34458_obj;
int dbg_printThreads() {
    char *n = gl___osActiveQueue_34458_head;
    volatile int pad;
    int outB, outA;
    int s1;
    int vB;
    CALL(dbg_printf)(GL_DATA_STR(0x1E3F4, "Threads: \n"));
    while (((OSThread *)n)->priority != -1) {
        (*(int (**)(int, int *, int *))(gSysApi_34458_obj + 0x70))(((OSThread *)n)->context.pc, &outA, &outB);
        s1 = CALL(dbg_threadName)(((OSThread *)n)->id);
        vB = CALL(dbg_threadStateName)(((OSThread *)n)->state);
        CALL(dbg_printf)(GL_DATA_STR(0x1E400, "%s (%d): %s : (%s,0x%x) PC=0x%x \n"), s1, ((OSThread *)n)->id, vB,
                         outA, outB, ((OSThread *)n)->context.pc);
        n = (char *)((OSThread *)n)->tlnext;
    }
}

extern char *gSysApi_34548_obj;
extern char *D_34548_slot;
void dbg_faultThread(void *a0) {
    int sp5C;
    char *s0;
    int sp54, sp50;
    int r5;
    CALL(gl_osSetEventMesg)(0xC, GL_BSS(0x3BB18), 0x10);
    *(int *)(GL_BSS(0x3BB38)) = 0;
    for (;;) {
        CALL(gl_osRecvMesg)(GL_BSS(0x3BB18), &sp5C, 1);
        if ((D_34548_slot = (char *)CALL(gl___osGetCurrFaultedThread)()) == 0) {
            continue;
        }
        s0 = D_34548_slot + 0x20;
        (*(int (**)())(gSysApi_34548_obj + 0x70))(((__OSThreadContext *)s0)->pc, &sp50, &sp54);
        CALL(dbg_printf)(GL_DATA_STR(0x1E424, "****************\n"));
        CALL(fault_printThread)(D_34548_slot);
        r5 = CALL(dbg_threadName)(((OSThread *)D_34548_slot)->id);
        CALL(sys_fatal)(GL_DATA_STR(0x1E438, "Fault (%s,0x%x) 0x%x in Thread %s (%d)\n"), sp50, sp54, ((__OSThreadContext *)s0)->pc, r5,
                         ((OSThread *)D_34548_slot)->id);
    }
}

extern int func_00000000();
void dbg_startFaultThread(void) {
    FCALL(gl_osCreateMesgQueue)(GL_BSS(0x3BB18), GL_BSS(0x3BB30), 1);
    FCALL(gl_osCreateThread)(GL_BSS(0x3B968), 5, &D_00000000, 0,
                   (char*)&D_00000000 + 0x3C040, 12);
    FCALL(gl_osStartThread)(GL_BSS(0x3B968));
}

int dbg_printfSink(int a0, char *src, int n) {
    int i;
    int k2;
    char buf[256];
    int j;
    int g = *(int *)((char *)&D_00000000 + 8);
    int k;
    if (g != 0) {
        CALL(TextWin_write)(g, 0, src, n);
    }
    if (n >= 255) {
        n = 255;
    }
    j = 0; for (i = 0; i < n; i++) { k = j++; buf[k] = src[i]; } k2 = j++; buf[k2] = 0;
    if (!i) {
    }
    buf[j] = 0;
    return 1;
}

void dbg_printf(int a0, ...) {
    int aligned = (((int)((char *)&a0 + 4)) + 3) & ~3;
    CALL(_Printf)((char *)&D_00000000 + 0x48D5C, 0, a0, aligned);
}

/* 3485C: hang stub (8/8 words). `&a0` homes the argument at 0(sp) with no
 * frame; the epilogue after the infinite loop lands on the 32-byte boundary
 * at unit offset 0x480 (3 nops), exactly as in the target. 34684 / 346F0 /
 * 34810 above are the real (exact) C of the functions in between, copied
 * from game_libs_post0b.c so the offsets agree. */
void sys_hang(int a0) {
    int *p = &a0;
    for (;;) {
    }
}
