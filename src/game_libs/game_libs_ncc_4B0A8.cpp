/* IDO 7.1 NCC (C++) EXACT donor for Shadow_ctor_0004B0A8 (0x24C / 147 words), agent-c run 164 (was the
 * run-134 C lift, 94.76). The function is the constructor `Obj::Obj(Src *s) : Base(name)` (Makefile rename of
 * __ct__3ObjFP3Src): cfront `this == 0 -> new(0x44)`, out-of-line Base(char *) (Base = 0x28 data bytes + the
 * virtual, so Obj's own vptr store lands at +0x28), a Pair copied from the vt+0x140 template, f0 = 2, f40 from an
 * out-of-line `ctor(0, name, w)` call, four f40 calls with &pair, then 41D40's by-value AIter over s->arr: every
 * element whose +0x6C is set gets a gSPDisplayList packet on f40's DL (named-d next()) whose w1 is the 0x34458
 * result for (*t->base + k * 8, t, k). Load-bearing: t and k read from TWO it.cur() calls passed to an inline
 * res(Elem *e1, Elem *e2) (the target re-reads the element pointer for k; one named e is 95.1, 576 -> 588 bytes);
 * Tex::base is a char ** (the target dereferences it); IBase writes `a = arr; idx = -1;` (emitted idx first, as the
 * target); five end-of-body block volatiles = frame 0xA8; AIter::next() as an if / return (the first expansion's colours match:
 * 98.73 -> 98.80 local); the
 * iterator built by a free `iter(Arr *)` instead of the member Arr::iter() puts the first iterator temp at the
 * target's sp+0x78 (98.81). 94.76 -> 98.81, length = target. Residual: the
 * colours of the iterator ctor this (a3 vs v1), the two element pointers and the next() results.
 * agent-c run 240: EXACT (99.83 -> 100). The iterator ctor `this` (lr -60, t0) reached the loop through the ++idx temps
 * and met cur()'s `Arr &r` temp (a3, priority 20); cur() as `return (*a)[idx];` and next() through `*me()->a` move the
 * pool temps so `this` takes a3 (12.5) with every other colour unchanged; the two dropped homes are paid back by an
 * initialised `{ int p1 = 0, p2 = 0; }` after the loop (frame 0xA8). Spliced over a placeholder in both build trees;
 * the blank jal / lui-addiu relocations go to the 0-pinned gl_4b0a8_c34458 / gl_4b0a8_base / Shadow_vtbl_4b0a8_vtbl (Makefile). */
#include "../../include/gdl/gfxlib.h" /* GfxLib: unk140 (the Pair template), staticDL */
#include "../../include/uso_call.h"  /* NCC donors build without -I include */
extern "C" {
void *gl_4b0a8_c34458(...);
extern char D_00000000[];
extern char gl_d_4b0a8_tbl, gGfxLib_4b0a8_vt, gGfxLib_4b0a8_w214;
}
inline void *operator new(unsigned n) { return XCALL(gl_4b0a8_c34458, mem_new)(n); }
struct Pair { int a, b; };
struct Gfx { unsigned w0, w1; };
struct DLHead { Gfx *buf; int count; };
struct DL { char pad[0xC]; DLHead *dl; Gfx *next() { DLHead *d = dl; int n = d->count; d->count = n + 1; return &dl->buf[n]; } };
struct Tex { char pad[0xC]; char **base; };
struct Elem { char pad[0x6C]; Tex *t; char pad70[0x8A - 0x70]; short k; };
struct AIter;
struct Arr { Elem **data; int pad4, pad8; int count; Elem *&operator[](int i) { return data[i]; } AIter iter(); };
struct IBase {
    Arr *a;
    int idx;
    int last;
    IBase(Arr *arr) { a = arr; idx = -1; last = arr->count - 1; }
};
struct AIter : IBase {
    AIter(Arr *arr) : IBase(arr) { if (0) { arr = 0; } } /* run 190: kept parameter */
    Elem **next() { if (idx < last) return &(*me()->a)[++idx]; return 0; } /* run 240: through me() */
    Elem *cur() { return (*a)[idx]; } /* run 240: no named reference (was kit CXLIV's local-reference read) */
    AIter *me() { return this; } /* agent-c run 238: zero-code pool hold for the loop increment (kit CCLIV) */
};
inline AIter iter(Arr *a) { return AIter(a); }
inline void *res(Elem *e1, Elem *e2) { Tex *t = e1->t; int k = e2->k; return XCALL(gl_4b0a8_c34458, gl_osVirtualToPhysical)(*t->base + k * 8, t, k); }
struct Src { char pad[0x84]; Arr arr; };
#ifdef PORT /* cfront layout (port_cfront.h, plan 17.8): explicit vptr, the NCC vtable stores written out */
#include "port_cfront.h"
extern "C" char Shadow_vtbl_4b0a8_vtbl[]; /* __vtbl__3Obj, bootup Data 0x1FB84 */
/* Base::Base = ShapeNode_ctor_000521F8 (__ct__4BaseFPc -> gl_4b0a8_base) */
CF_EXT(void *, gl4b0a8_ShapeNode_ctor, ShapeNode_ctor_000521F8, (void *, char *));
struct Base { int f0; char pad[0x24]; char *vptr; CF_INL Base(char *name) { gl4b0a8_ShapeNode_ctor(this, name); } };
struct Obj : Base {
    char pad2c[0x40 - 0x2C];
    DL *f40;
    Obj(Src *s);
};
#else
struct Base { int f0; char pad[0x24]; Base(char *name); virtual void f(); };
struct Obj : Base {
    virtual void f();
    char pad2c[0x40 - 0x2C];
    DL *f40;
    Obj(Src *s);
};
#endif
Obj::Obj(Src *s) : Base(D_00000000 + 0x200EC) {
#ifdef PORT
    vptr = Shadow_vtbl_4b0a8_vtbl;
#endif
    Pair p = *(Pair *)GFXLIB(&gGfxLib_4b0a8_vt)->unk140;
    f0 = 2;
    f40 = (DL *)XCALL(gl_4b0a8_c34458, gfx_newDLChild_0005AB84)(0, D_00000000 + 0x200F4, (int)GFXLIB(&gGfxLib_4b0a8_w214)->staticDL); /* "MainStaticDL" */
    XCALL(gl_4b0a8_c34458, gfx_emitCycleType)(f40, &p);
    XCALL(gl_4b0a8_c34458, gfx_emitGeometryMode)(f40, &p);
    XCALL(gl_4b0a8_c34458, gfx_emitCombineMode)(f40, &p);
    XCALL(gl_4b0a8_c34458, gfx_emitRenderMode)(f40, &p);
    AIter it = iter(&s->arr);
    /* agent-c run 238: the increment holds three pool slots while edg emits the rotated bottom copy of
     * it.next(), so its result / pointer / ++idx temps take the target's a0 / v1 / v0 (99.56 -> 99.71); the
     * three hold homes replace the four end-of-body volatile pads (frame 0xA8 kept, 99.83) */
    for (; it.next(); it.me()->me()->me()) {
        if (it.cur()->t != 0) {
            DL *d = f40;
            void *r = res(it.cur(), it.cur());
            Gfx *g = d->next();
            g->w0 = 0x06000000;
            g->w1 = (unsigned)r;
        }
    }
    { int p1 = 0, p2 = 0; }
    XCALL(gl_4b0a8_c34458, gfx_endDL)(f40);
}
#ifdef PORT /* the cfront ctor protocol: the ROM calls it with this == 0 (port_cfront.h, plan 18.11) */
extern "C" Obj *port_ctor_Shadow_ctor_0004B0A8(Obj *self, Src *s) { return CF_CTOR_NEW(self, Obj, (s)); }
#endif
