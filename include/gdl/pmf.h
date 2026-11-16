#ifndef GDL_PMF_H
#define GDL_PMF_H

/* Cfront pointer-to-member function (the __mptr record), and the member-callback object the gdl
 * engine dispatches through it (agent-b run 583).
 *
 * GdlPmf is Cfront's 8-byte {short d; short i; union { fn f; short o; }}: d is the this-delta added
 * to the object; i < 0 means a non-virtual member and the word at +4 is the function itself;
 * i >= 0 is a virtual slot and the word at +4 is the object's vptr offset (0 together with d == 0
 * selects the class's default vptr offset, 0x28 for the MemberCallback_invoke family and 0x5C for
 * CMod_callbackInvoke_000551E0). Cfront vtable entries have the same layout, so the slot entry read
 * at vtbl + i * 8 is a GdlPmf as well: its d adjusts this again and its +4 word is the function.
 *
 * GdlPmfCallback is the 16-byte callback object (vptr, target object, pmf) of game/sun.h's
 * MemberCallback / game/envobj.h's Tv1Callback, plus the two argument words that
 * MemberCallback_invoke_0003A0C4 passes after this. Evidence: the six dispatchers in
 * game_libs_post0b.c (MemberCallback_invoke, _0003A044, _0003A0C4, _0003EBDC, _0003EC5C,
 * CMod_callbackInvoke_000551E0) read exactly these words. Plain C types only. */

typedef struct GdlPmf {
    /* 0x0 */ short delta;              /* this-adjustment */
    /* 0x2 */ short vslot;              /* < 0: non-virtual (fn below); else the vtable slot index */
    /* 0x4 */ union {
        void *fn;                       /* vslot < 0 (and in a vtable entry): the member function */
        int vptrOffset;                 /* vslot >= 0: where the object keeps its vptr (0 = default) */
    } u;
} GdlPmf;

/* GdlFunctor (vFP1): a one-argument function object. Its base vtable Data+0x1FC24 has the "vFP1" stub
 * vFP1_call (fatal: pure virtual) in slot 0; the derived vtable Data+0x1FC3C has vFP1_invokeFn_0004D084
 * there, which calls fn(arg, this) (docs/1080_ENGINE_CORE.md). */
typedef struct GdlFunctor {
    /* 0x0 */ void *vptr;
    /* 0x4 */ int (*fn)();
    /* 0x8 */ int arg;
} GdlFunctor;

typedef struct GdlPmfCallback {
    /* 0x00 */ void *vptr;
    /* 0x04 */ char *obj;               /* the target object (this before pmf.delta) */
    /* 0x08 */ GdlPmf pmf;
    /* 0x10 */ int arg0;                /* MemberCallback_invoke_0003A0C4: the two words passed after this */
    /* 0x14 */ int arg1;
} GdlPmfCallback;

#endif /* GDL_PMF_H */
