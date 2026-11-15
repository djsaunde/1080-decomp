#include "../../include/uso_call.h"  /* NCC donors build without -I include */
/* IDO 7.1 NCC (C++) EXACT donor for App_softReset (0x24C / 147 words), agent-c run 173, spliced over an
 * empty placeholder in game_libs_post0b.c in both build trees
 * (from the run-164 draft, kit CXVI item 5). `void Self::init()` (rename init__4SelfFv): D_41820_e = new Arr(10)
 * (inline ctor cap / n / data = new int[c]), Self : SB (0x28 data bytes + 12 virtuals, m2C at +0x2C), the attach
 * blocks as the member Self::link(Rec *q) with a KEPT q (`if (0) { q = 0; }`: the q reload from its pooled home
 * and the this reload into v0 like the target; draft 93.98 -> 97.57 at 588 bytes = target), the third attach on
 * `new O44` (out-of-line base ctor B44(name), vptr = Node_vtbl_1F748_41820_h) through a keep-free linkN (97.73), and the closing
 * virtual v12() through the vptr at +0x28; two end-of-body block volatiles give frame 0x40.
 * The first two attaches as a FREE inline lnk(Rec *q, Self *s) with q first and kept: a member call on this
 * allocates a pooled `this` temp that took the Arr temp's slot, so q sat one slot lower (0x2C); q-first puts q in
 * the target's sp+0x30 slot and colours it a2 (97.73 -> 98.16 local).
 * A trailing dead `if (s) {}` in lnk (kit XLVI lever E; it acts through the SECOND attach) makes that attach's
 * this reload a coloured v0 and realigns the O44 tail's t-ring (98.16 -> 98.81 local).
 * The first attach through lnkB, which keeps the call result `int r = gl(...)` with a trailing `if (r) {}`: v0 stays
 * live through that attach, so its this reload takes v1 like the target; one end-of-body volatile (not two) then puts
 * the &m10 spill back at sp+0x1C. cfe body 94.72. */
extern "C" {
int gl_func_00000000(...);
void gl_func_00000000_41820v();
extern char D_00000000[];
extern int D_41820_a;
extern int import_80000310;
extern char gSysLib_3c740[];
extern char gGfxLib_41820_c[], D_41820_d[];
extern int D_41820_g;
}
inline void *operator new(unsigned n) { return (void *)CALL(mem_new)(n); }
struct Rec { int f0; int f4; int f8; int fC; int f10; void *f14; };
struct Arr { int *data; int f4; int cap; int n; int f10; Arr(int c) { cap = c; n = 0; data = new int[c]; } };
struct B44 : Rec { char pad[0x28 - 0x18]; B44(char *name); };
struct O44 : B44 { O44() : B44(D_00000000 + 0x1F630) {} virtual void f(); };
extern "C" { extern Rec *gConsoleNode_3EF04_41820_b; extern Arr *D_41820_e; extern Rec *D_41820_f; }
struct SB {
    char pad[0x10];
    int m10;
    char pad14[0x28 - 0x14];
    virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6();
    virtual void v7(); virtual void v8(); virtual void v9(); virtual void v10(); virtual void v11(); virtual void v12();
};
struct Self : SB {
    int m2C;
    void init();
    void link(Rec *q) { if (0) { q = 0; } gl_func_00000000(&m10, q); if (q->f14) q->f4 = 1; q->f14 = this; }
    void linkN(Rec *q) { gl_func_00000000(&m10, q); if (q->f14) q->f4 = 1; q->f14 = this; }
};
inline void lnk(Rec *q, Self *s) { if (0) { q = 0; } CALL(list_append)(&s->m10, q); if (q->f14) q->f4 = 1; q->f14 = s; if (s) {} }
inline void lnkB(Rec *q, Self *s) { if (0) { q = 0; } int r = CALL(list_append)(&s->m10, q); if (q->f14) q->f4 = 1; q->f14 = s; if (s) {} if (r) {} }
void Self::init() {
    int local[3];
    CALL(dbg_printf)(D_00000000 + 0x1F5F0, D_41820_a++);
    if (import_80000310 != 0x17D7) {
        CALL(sys_fatal)(D_00000000 + 0x1F600);
    }
    CALL(dbg_printf)(D_00000000 + 0x1F618);
    CALL(ConsoleMgr_closeOwnerWins)(gConsoleNode_3EF04_41820_b, 1);
    CALL(dbg_printf)(D_00000000 + 0x1F624);
    CALL(sys_listRemove_00061D1C)(gSysLib_3c740, 1);
    local[0] = 0x10;
    CALL(Node_broadcastMsg)(this, local);
    CALL(dma_waitAll)(gSysLib_3c740);
    CALL(gfx_waitRcpIdle)(gGfxLib_41820_c);
    CALL(app_freeAllOfType)(1);
    CALL(app_setHeapMode)(1);
    CALL(DynArr_init_000665A4)(D_41820_d, 100);
    D_41820_e = new Arr(10);
    CALL(app_setHeapMode)(1);
    CALL(MemTracker_walkAll)(D_41820_f, 1);
    XCALL(gl_func_00000000_41820v, BasePolyShape_initGeomGlobals_00052674)();
    m10 = 0;
    m2C = 0;
    D_41820_g = 0;
    lnkB(D_41820_f, this);
    lnk(gConsoleNode_3EF04_41820_b, this);
    CALL(sys_pollDevices_00034B64)(gSysLib_3c740);
    CALL(GfxLib_reset)(gGfxLib_41820_c);
    linkN(new O44);
    v12();
    { volatile int e0; }
}
