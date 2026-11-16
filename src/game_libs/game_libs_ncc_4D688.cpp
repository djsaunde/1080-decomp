#include "../../include/uso_call.h"  /* NCC donors build without -I include */
#include "../../include/gdl/sysapi.h"  /* SysLib, SYSLIB_AT */
typedef int s32; typedef unsigned int u32; typedef unsigned long long u64;
#include "../../include/gdl/rcp.h" /* RcpTask / RcpMgr */
/* IDO 7.1 NCC (C++) donor for RcpMgr_service -- EXACT (agent-g run 256), spliced over the post0b placeholder on
 * both build paths via REPLACE_FUNC_BODY (only RcpMgr_service = `Obj::run()` is taken from this object).
 * The debug-console event loop (agent-c run 137 NCC goto form; agent-g runs 192 / 204: member function, for(;;) loop
 * holding &g2 / 1, source-order edg switch = beq chain, static guard pair, k homed above e, dead multi-def on e;
 * agent-g run 256: the old-n copy of the 'd' toggle through the dead e, `e = (Ev *)n; n++; if ((int)e & 1)`, which
 * puts it in a0 like the target).
 * TRANSLATION UNIT: the loop never returns, so ugen emits `.align 5` before the dead epilogue. In a standalone object
 * that pads 5 nops (run() at offset 0, epilogue at 0x38C); the target has none because run() was NOT first in its
 * file. The guard static is shared with RcpMgr_ctor (it stores the 0x1234567812345678 magic that run() checks),
 * so this file reconstructs the unit from RcpMgr_loadTask_0004D354 (the nearest preceding function whose start puts run()'s
 * epilogue on a 32-byte boundary: 0x334 + 0x38C = 0x6C0): 4D354 / 4D39C / 4D3E4 (the post0b C bodies as extern "C"),
 * 4D468 (the exact NCC donor body, see game_libs_ncc_4D468.cpp for its notes), 4D658, then run(). Every compiled
 * size equals the target's (4D3E4 scores 99.70 under NCC; only run() is spliced). The guard's .bss reloc is renamed
 * RcpMgr_service_bss by the splice (pinned to 0x3CB00, the address RcpMgr_ctor_bss already uses); calls go
 * through gl_func_00000000 (the USO's zero jal). */
extern "C" {
int gl_func_00000000(...);
extern char D_00000000[];
extern char gSysLib_4D688_g1[];
extern int D_4D688_g2;
extern char gSysDevBlock_3A4E8_4D688_g3[];
extern int D_4D468_x, D_4D468_y, D_4D468_z;
extern char gSysLib_4D468_w[];
extern char D_4D468_a2[];
}
static long long sG;
extern "C" void RcpMgr_loadTask_0004D354(RcpTask *task) {
    CALL(sys_writebackDCacheAll)(task);
    CALL(osSpTaskLoad)(&task->task);
    CALL(gl_osSpTaskStartGo)(&task->task);
    task->unk60 = 1;
    task->unk64 = 0;
}
/* append task to the circular queue at *head (before the head) */
extern "C" void RcpMgr_insertTask(RcpTask *task, RcpTask **head) {
    if (*head != 0) {
        task->next = *head;
        task->prev = (*head)->prev;
        (*head)->prev->next = task;
        (*head)->prev = task;
    } else {
        task->next = task;
        task->prev = task;
        *head = task;
    }
}
extern "C" void RcpMgr_removeTask(RcpTask *task, RcpTask **head) {
    if (*head == 0) {
        gl_func_00000000(D_00000000 + 0x201E4, head, task); /* "RcpTask remove" */ /* UNNAMED: no-reloc TU-context copy: only run() is spliced from this donor (REPLACE_FUNC_BODY RcpMgr_service), so this body never reaches the ROM; the live RcpMgr_removeTask (game_libs_post0b.c) makes this call as sys_fatal */
    }
    if (task->next == task->prev && task == task->prev) {
        *head = 0;
        return;
    }
    *head = (*head)->next;
    task->prev->next = task->next;
    task->next->prev = task->prev;
}
struct E { E() {} char pad[0x78]; };
void *operator new(unsigned int n, int al);
struct S4D { int w0, w4, w8, wC; int w10; int w14; int w18; int w1C, w20, w24, w28, w2C; char pad30[0x14]; int w44; };
inline void bind(int k, int h, int id) { if (0) { h = 0; } CALL(gl_osSetEventMesg)(k, h, id); }
extern "C" S4D *RcpMgr_ctor(S4D *s) {
    if (s != 0 || (s = (S4D *)CALL(mem_new)(0x50)) != 0) {
    CALL(memtrack_begin)(D_00000000 + 0x201F4, 0);
    CALL(dma_read)(D_00000000 + 0x3EB00, 0xB0000B70, 0x100);
    s->w44 = 0;
    s->w0 = 0;
    s->w4 = 0;
    s->w8 = 0;
    s->wC = 0;
    for (int i = 0; i < 16; i++) {
#ifdef PORT
        int p = mem_newAligned(0x78, 16u); RcpMgr_insertTask((RcpTask *)(p ? p : 0), (RcpTask **)s);
#else
        int p = CALL(mem_newAligned)(0x78, 16u); CALL(RcpMgr_insertTask)(p ? p : 0, s);
#endif
    }
    { int p = CALL(mem_newAligned)(0x78, 16); s->w14 = p ? p : 0; }
    D_4D468_x = CALL(mem_newAligned)(0xC00, 16);
    D_4D468_y = 0x19000;
    D_4D468_z = CALL(mem_newAligned)(D_4D468_y + 4, 64);
    ((unsigned *)D_4D468_z)[(unsigned)D_4D468_y / 4] = 0x12345678u;
    s->w2C = CALL(mem_newAligned)(0x400, 16);
    CALL(gl_osCreateThread)(D_00000000 + 0x3C950, 4, D_4D468_a2, s, D_00000000 + 0x3EB00, 11);
    sG = 0x1234567812345678LL;
    /* gSysLib.msgQueue: the system event queue, bound to SP / DP / VI events here */
    bind(9, (int)SYSLIB_AT(gSysLib_4D468_w)->msgQueue, 102);
    { int h = (int)SYSLIB_AT(gSysLib_4D468_w)->msgQueue; CALL(gl_osSetEventMesg)(4, h, 101); }
    { int h = (int)SYSLIB_AT(gSysLib_4D468_w)->msgQueue; CALL(gl_osSetEventMesg)(14, h, 108); }
    CALL(gl_osStartThread)(D_00000000 + 0x3C950);
    s->w20 = 0;
    s->w1C = 0;
    s->w24 = 0;
    s->w28 = 0;
    CALL(memtrack_end)();
    s->w20 = 0;
    s->w1C = 0;
    }
    return s;
}

extern "C" void RcpMgr_update(int a0) {
    CALL(dbg_printf)(D_00000000 + 0x000201FC);
    CALL(RcpMgr_service)(a0);
}
#define DSTR(o) (D_00000000 + (o))
struct Ev { char pad[0x1C]; int key; };
struct Q { char pad[0x1C]; Ev *ev; };
struct Child { char pad[0x5C]; int m5C; };
struct Obj {
    int m0, m4, m8;
    Child *mC;
    int m10, m14;
    int m18, m1C, m20;
    volatile int m24;
    int m28;
    char pad2c[0x3C - 0x2C];
    char *m3C, *m40;
    void *m44;
    int m48;
    void run();
};
void Obj::run()
{
    int n, pend, cur, keep;
    m48 = 0;
    m18 = 0;
    m40 = DSTR(0x2021C);
    m10 = 0;
    n = 0;
    cur = 0;
    pend = 0;
    keep = 0;
    CALL(pad_requestGlobal)(gSysLib_4D688_g1);
    for (;;) {
        if (sG != 0x1234567812345678LL) {
            CALL(sys_fatal)(DSTR(0x20224));
        }
        int k; /* run 204: homed above e */
        Ev *e = ((Q *)gSysLib_4D688_g1)->ev;
        D_4D688_g2 = 0;
        CALL(gl_osRecvMesg)(e, &e->key, 1);
        k = e->key;
        if (0) { e = 0; } /* run 204: dead multi-def (register LR spilled to its home), not &e */
        D_4D688_g2 = 1;
        switch (k) {
        case 'e':
            m20 = 0;
            if (m18 != 0) {
                CALL(prof_timerStop)(DSTR(0x20240), k);
                Child *c = mC;
                if (c != 0 && c->m5C != 0) {
                    m3C = DSTR(0x20248);
#ifdef PORT
                    RcpMgr_loadTask_0004D354((RcpTask *)c); /* Child is a mirror of RcpTask */
#else
                    CALL(RcpMgr_loadTask_0004D354)(c);
#endif
                    m40 = DSTR(0x20250);
                    m20 = 1;
                } else {
                    CALL(RcpMgr_insert_0004DCD4)(this);
                    CALL(RcpMgr_startTask)(this);
                }
                m18 = 0;
            } else if (keep != 0) {
                keep = 0;
                mC->m5C = CALL(gl_osSpTaskYielded)((char *)mC + 0x10, k);
                m18 = pend;
                CALL(sys_writebackDCacheAll)();
                m3C = DSTR(0x2025C);
                CALL(prof_timerStart)(DSTR(0x20260));
                CALL(osSpTaskLoad)(m18);
                CALL(gl_osSpTaskStartGo)(m18);
                m40 = DSTR(0x20268);
                m20 = 1;
            } else {
                CALL(RcpMgr_insert_0004DCD4)(this, k);
                CALL(RcpMgr_startTask)(this);
            }
            break;
        case 'f':
            m1C = 0;
            CALL(prof_timerStop)(DSTR(0x2026C), k);
            break;
        case 'd':
            e = (Ev *)n; /* agent-g run 256: the old n through the dead e (its poll web reloads into a0; kit CCLXX / CCLXXX) */
            n++;
            if ((int)e & 1) {
                CALL(pad_waitIdleGlobal)(gSysLib_4D688_g1, k);
                CALL(pad_requestGlobal)(gSysLib_4D688_g1);
            }
            pend = cur;
            if (cur != 0) {
                if (mC != 0) {
                    CALL(gl_osSpTaskYield)();
                    keep = 1; /* after the call (local; lever-sweep order 95.45 -> 95.71) */
                } else {
                    m18 = cur;
                    CALL(sys_writebackDCacheAll)();
                    m3C = DSTR(0x20270);
                    CALL(prof_timerStart)(DSTR(0x20274));
                    CALL(osSpTaskLoad)(m18);
                    CALL(gl_osSpTaskStartGo)(m18);
                    m40 = DSTR(0x2027C);
                    m20 = 1;
                }
            }
            m3C = DSTR(0x20280);
            CALL(prof_timerStart)(DSTR(0x20288));
            cur = CALL(snd_AudioUpdate)();
            CALL(prof_timerStop)(DSTR(0x20290));
            {
                void *p = m44;
                m3C = DSTR(0x20298); /* run 204: back before m24++ */
                m24++;
                if (p != 0 && (m24 & 1)) {
                    CALL(gl_osSendMesg)(p, 'd', 1);
                }
            }
            break;
        case 'g':
        case 'h':
            if (mC != 0) {
                CALL(sys_fatal)(DSTR(0x202A0), k);
            }
            if (m4 != 0) {
                CALL(sys_fatal)(DSTR(0x202B4));
            }
            m4 = m8; /* run 204: no named t (target reads m8 into a ring temp) */
            m8 = 0;
            m28 = 0;
            if (m18 == 0) {
                CALL(RcpMgr_startTask)(this);
            }
            CALL(prof_timerStart)(DSTR(0x202D0));
            break;
        case 'l':
            CALL(pad_sendMsg_00067168)(gSysDevBlock_3A4E8_4D688_g3, k);
            CALL(snd_PreNMI)();
            m48 = 1;
            break;
        default:
            CALL(sys_fatal)(DSTR(0x202D4), k);
            break;
        }
    }
}
