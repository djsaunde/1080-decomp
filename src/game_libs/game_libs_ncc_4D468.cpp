#include "../../include/uso_call.h"  /* NCC donors build without -I include */
/* IDO 7.1 NCC (C++) donor for RcpMgr_ctor (0x1F0 / 124
 * words): module init -- allocate the 0x50 owner if none, clear it, register 16
 * new 0x78-byte nodes, allocate the 0x19000-byte heap with a 0x12345678 guard
 * word, start the 0x3C950 task, set the 0x3CB00 guard pair, three bind calls.
 * agent-c run 137 (cfe NM 78.6 -> NCC 124/124 words, 9 raw left). Load-bearing:
 * each node is `p = alloc(0x78, 16u); reg(p ? p : 0, s)` (the edg new-expression
 * null test; 16u keeps the loop bound 16 out of the argument); the heap guard is
 * an UNSIGNED store (an int store CSEs 0x12345678 into s2 across the calls); the
 * 0x3CB00 guard pair is ONE static `long long = 0x1234567812345678` (one ugen
 * temp for both words, one shared `lui at`: file-local object, kit LX); each
 * bind call reads its D+0x1C argument into a block-scope int first (target loads
 * a1 before a0). Residual: the node pointer's merge register (a2 here, target a3,
 * both sites); an exact donor would also pin the renamed .bss reloc to 0x3CB00.
 * agent-c run 161: EXACT. The first bind call goes through `inline void bind(int k, int h, int id)` with the
 * handle parameter kept (`if (0) { h = 0; }`): its pooled int temp reuses the node ternary's slot (edg pools 4-byte
 * temps by size), the merged live range now crosses a2 argument setups, so the ternary result takes the target's a3
 * at both merge sites (binding all three calls that way re-ranks s1 / s2 instead). The static guard pair's .bss
 * reloc is renamed RcpMgr_ctor_bss by the splice and pinned to 0x3CB00. Spliced into both build trees. */
extern "C" {
extern int gl_func_00000000(...);
extern char D_00000000[];
extern int D_4D468_x, D_4D468_y, D_4D468_z;
extern char gSysLib_4D468_w[];
extern char D_4D468_a2[];
}
struct E { E() {} char pad[0x78]; };
void *operator new(unsigned int n, int al);
static long long sCanL;
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
        int p = CALL(mem_newAligned)(0x78, 16u); CALL(RcpMgr_insertTask)(p ? p : 0, s);
    }
    { int p = CALL(mem_newAligned)(0x78, 16); s->w14 = p ? p : 0; }
    D_4D468_x = CALL(mem_newAligned)(0xC00, 16);
    D_4D468_y = 0x19000;
    D_4D468_z = CALL(mem_newAligned)(D_4D468_y + 4, 64);
    ((unsigned *)D_4D468_z)[(unsigned)D_4D468_y / 4] = 0x12345678u;
    s->w2C = CALL(mem_newAligned)(0x400, 16);
    CALL(gl_osCreateThread)(D_00000000 + 0x3C950, 4, D_4D468_a2, s, D_00000000 + 0x3EB00, 11);
    sCanL = 0x1234567812345678LL;
    /* RAW: unknown-object: gSysLib +0x1C is the system OSMesgQueue * (the event queue bound to SP / DP / VI events here); no SysLib struct on this branch yet (main: sysapi.h SysLib.msgQueue, agent-b run 583) */
    bind(9, *(int *)(gSysLib_4D468_w + 0x1C), 102);
    { int h = *(int *)(gSysLib_4D468_w + 0x1C); CALL(gl_osSetEventMesg)(4, h, 101); } /* RAW: unknown-object (as above) */
    { int h = *(int *)(gSysLib_4D468_w + 0x1C); CALL(gl_osSetEventMesg)(14, h, 108); } /* RAW: unknown-object (as above) */
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
