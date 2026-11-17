#include "../../include/uso_call.h"  /* NCC donors build without -I include */
/* IDO 7.1 NCC (C++) EXACT donor for app_staticInit_000412E8 (0x23C / 143 words), agent-c run 163.
 * It is the static initializer (__sti__) of four global objects, each built through edg's cfront
 * `this == 0 -> allocate` protocol for every user-declared constructor:
 *  - gGfxLib_412E8_a: a holder with NO user ctor (so no protocol of its own) whose members A (0x1E0: two
 *    8-byte pairs, a word, a wrapped word), E (0xC), Fog (0x18, ctor calls an out-of-line init) and G
 *    (0x25C: El[50] -> __vec_new(this, 50, 12, El::El)) sit at +0 / +0x234 / +0x294 / +0x378. A member
 *    at holder+off compares `&holder != -off` (the target's `li at, -0x234; bne`); a TU-defined object
 *    itself would have its own this==0 test folded away, which is why every protocol object is a member.
 *  - gSysLib_412E8_i: holder of I, whose W4 is a BASE (built before the vptr store at +0x18).
 *  - gSysLib_412E8_l: L has a vptr and no user ctor -> a bare vtable store. In the ROM both
 *    _i and _l relocate to gSysLib (Sym 73): one object whose base-class ctor stores
 *    SysLibBase_vtbl at +0x18 and whose own implicit ctor then stores SysLib_vtbl_1E1B8 there.
 *  - gHostio_412E8_m: holder of a W4.
 * The cfe body (92.24) needed per-site section aliases. The Makefile renames the __sti__ symbol and
 * the El ctor / vtables / __vec_new references (symbol names only). Spliced over an empty
 * placeholder in both build trees. */
extern "C" void *gl_func_00000000(...);
inline void *operator new(unsigned n) { return CALL(mem_new)(n); }
struct M8 { int a, b; M8() { b = 0; a = 0; } };
struct M4 { int a; M4() { a = 0; } };
struct W4 { M4 m; W4() {} };
struct A { char p0[0x138]; M8 m138; M8 m140; char p148[0x18C - 0x148]; M4 m18C; char p190[0x1C4 - 0x190]; W4 w1C4; char p1C8[0x1E0 - 0x1C8]; A() {} };
struct E { M8 m; int x; E() {} };
/* GfxLib.fog (+0x294, FogParms, gfxlib.h): its ctor calls the out-of-line init */
struct Fog { char p[0x18]; Fog() { CALL(FogParms_setDefault_00043EAC)(this); } };
#ifdef PORT /* El::El = GfxDdm_ctor (__ct__2ElFv -> D_412E8_h, the ROM's __vec_new ctor argument) */
#include "port_cfront.h"
CF_EXT(void *, gl412e8_GfxDdm_ctor, GfxDdm_ctor, (void *));
struct El { char p[0xC]; CF_INL El() { gl412e8_GfxDdm_ctor(this); } };
#else
struct El { char p[0xC]; El(); };
#endif
struct G { El e[50]; int n; G() {} };
#ifdef PORT /* cfront layout (port_cfront.h, plan 17.8): explicit vptr, the NCC vtable stores written out */
extern "C" char SysLibBase_vtbl_412E8_j[], SysLib_vtbl_1E1B8_412E8_k[]; /* __vtbl__1I, __vtbl__1L */
struct I : W4 { char p[0x18 - 4]; char *vptr; I() { vptr = SysLibBase_vtbl_412E8_j; } };
#else
struct I : W4 { char p[0x18 - 4]; virtual void f(); I() {} };
#endif
struct HI { I i; };
struct HM { W4 w; };
#ifdef PORT
struct L { char p[0x18]; char *vptr; L() { vptr = SysLib_vtbl_1E1B8_412E8_k; } };
#else
struct L { char p[0x18]; virtual void f(); };
#endif
struct Big { A a; char p1[0x234 - 0x1E0]; E e; char p2[0x294 - 0x240]; Fog f; char p3[0x378 - 0x2AC]; G g; };
Big gGfxLib_412E8_a;
HI gSysLib_412E8_i;
L gSysLib_412E8_l;
HM gHostio_412E8_m;
