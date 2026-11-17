#include "../../include/uso_call.h"  /* NCC donors build without -I include */
/* IDO 7.1 NCC (C++) donor for nodelist_dispatch (0x430 / 268 words): the command-stream
 * opcode dispatcher. A `switch` under edg is a compare chain in CASE order (1014, 1012,
 * ... 1018), which is why the cfe goto-ladder NM needed `if (1) goto` tricks. Shapes:
 * - `next()` = `return cmd = *(*cp)++;` (inline, its result temp is the v1 value);
 *   1012 calls it as a statement and tests `f18 & cmd` (gives `and t8,t7,v1`);
 * - the flag cases are inline Flags members on the D_00000000 block (this = a0, m = a1);
 * - 1017 / 1003 pass the 76-byte Strm by value (edg copy loops);
 * - default: a REAL cfront virtual call (vptr at +0x28 behind a 40-byte base, 7th
 *   virtual = entry +0x38: `lh delta; lw fn; jalr; addu a0,delta,obj`), the {cmd, cur}
 *   pair `vt` declared at function scope after a dead `Two pt` (frame 0x130: NCC drops
 *   unused scalars but keeps a class object), filled by an inline set() through a block
 *   int `c` and followed by `if (1) {}`.
 * gl_ref_55470_d is a distinct zero alias (the plain D_00000000 store would CSE with
 * the Flags base). Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
extern "C" {
extern char D_00000000[];
extern int gl_ref_55470_d;
int gl_func_00000000(...);
}
struct Two { int a, b; Two() {} Two(int x, int y) { a = x; b = y; } void set(int x, int y) { a = x; b = y; } };
struct Obj;
struct ObjPad { char pad[40]; };
#ifdef PORT /* cfront layout: explicit vptr, virtuals as slot calls (port_cfront.h, plan 17.8) */
#include "port_cfront.h"
struct Obj : ObjPad {
    char *vptr; /* cfront vptr */
    CF_V0(void, m1, 0) CF_V0(void, m2, 1) CF_V0(void, m3, 2) CF_V0(void, m4, 3) CF_V0(void, m5, 4)
    CF_V0(void, m6, 5) CF_V1(void, m7, 6, int *)
};
#else
struct Obj : ObjPad { virtual void m1(); virtual void m2(); virtual void m3(); virtual void m4(); virtual void m5(); virtual void m6(); virtual void m7(int *); };
#endif
struct Flags { int a; int b;
    void orA(int m) { a |= m; } void clrA(int m) { a &= ~m; }
    void orB(int m) { b |= m; } void clrB(int m) { b &= ~m; } };
struct Strm {
    int **cp; int cmd; int w2; int w3; Obj *obj; int w5, w6, w7, w8, w9, w10, w11, w12;
    int f13; int f14; int w15; int f16; int f17; int f18;
    int next() { return cmd = *(*cp)++; }
};
#ifdef PORT /* one object for the bytes these tokens share, so gcc keeps the order (scripts/port-alias-audit.py, plan 18.7) */
extern "C" char port_bootup_bss[];
extern "C" char port_bootup_data[];
#undef XDATA
#define XDATA(tok, sym, off) (*(__typeof__(tok) *)((char *)&sym + (off)))
#endif
extern "C" void nodelist_dispatch(Strm *s) {
    Two pt;
    Two vt;
    switch (s->cmd) {
    case 1014:
        break;
    case 1012:
        s->next(); if (!(s->f18 & s->cmd)) CALL(nodelist_skipBlock)(s);
        break;
    case 1015:
        CALL(nodelist_skipBlock)(s);
        break;
    case 1010:
        if (s->next() != s->f16) CALL(nodelist_skipBlock)(s);
        break;
    case 1011:
        if (s->next() != s->f17) CALL(nodelist_skipBlock)(s);
        break;
    case 1016:
        s->f14 = s->next();
        break;
    case 1000:
        break;
    case 1008:
        gl_ref_55470_d = s->next();
        break;
    case 1019:
        break;
    case 1004:
        ((Flags *)XDATA(D_00000000, port_bootup_bss, 0x3EEC8))->orB(s->next());
        break;
    case 1005:
        ((Flags *)XDATA(D_00000000, port_bootup_bss, 0x3EEC8))->b &= ~s->next();
        break;
    case 1006:
        ((Flags *)XDATA(D_00000000, port_bootup_bss, 0x3EEC4))->orA(s->next());
        break;
    case 1007:
        ((Flags *)XDATA(D_00000000, port_bootup_bss, 0x3EEC4))->a &= ~s->next();
        break;
    case 1017: {
        Strm t = *s;
        int v = s->next();
        t.cp = (int **)&v;
        CALL(nodelist_dispatchList_000558A0)(t);
        break;
    }
    case 1003: {
        Strm t = *s;
        t.w3 = 0;
        t.obj = 0;
        t.w15 = (int)s;
        CALL(nodelist_walk_0005534C)(t);
        break;
    }
    case 113:
        s->f13 = 1;
    default: {
        CALL(nodelist_checkNode)(s);
        { int c = s->cmd; vt.set(c, (int)*s->cp); }
        if (1) {}
        s->obj->m7(&vt.a);
        *s->cp = (int *)vt.b;
        break;
    }
    case 1018:
        CALL(dbg_printf)(XDATA(D_00000000, port_bootup_data, 0x0) + 0x211B0, s->next());
        break;
    }
}
#ifdef PORT
#undef XDATA
#define XDATA(tok, sym, off) PORT_XDATA(tok, sym, off, __COUNTER__)
#endif
