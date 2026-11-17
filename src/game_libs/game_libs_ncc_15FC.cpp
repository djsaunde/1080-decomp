#include "../../include/uso_call.h"  /* NCC donors build without -I include */
typedef int s32; typedef short s16; typedef float f32;
#include "../../include/gdl/gadget.h" /* CoolGadget */
/* IDO 7.1 NCC (C++) donor for CoolGadget_ctor (0x19C / 103 words), EXACT.
 * The function is a cfront-style constructor G::G(int) of a 0x198-byte class:
 * edg's ctor protocol gives the get-or-create head (`if (this == 0) this =
 * new(0x198)`, operator new forwarded inline to the alloc), the out-of-line
 * base ctor B(&D[0xCC58]) runs first, then the vptr at +0x28 (B declares the
 * virtual after 0x28 bytes of data), then the member ctor at +0x2C, then the
 * body. agent-g run 163 (docs/IDO_CODEGEN.md kit CII): as cfe C the li 100
 * kept v0 (target v1) under every spelling; as a real ctor with a named
 * `char *self = (char *)this` the return-value LR is live through the body
 * block and v0 is forbidden for the constant. The 100 stores are two
 * statements (a chained assignment costs an edg temp home, frame 64 vs 56),
 * and the last three init calls take ((char *)this) instead of self, which
 * orders the two call-crossing address spills like the target (0x180 at
 * sp+40, 0x168 at sp+44).
 * Symbols renamed after compile (names only): __ct__1GFi -> CoolGadget_ctor,
 * __ct__1BFPc -> gl_func_00000000_15fc_base, __ct__1MFv ->
 * gl_func_00000000_15fc_m, __vtbl__1G -> gl_data_00000000_15fc. The data
 * words are pinned at their USO offsets (D_15FC_* in undefined_syms_auto.txt). */
extern "C" {
void *gl_func_00000000(...);
extern char sStr_Node_CC58[], sStr_CoolGadget_CC60[];
extern float kRo_CoolGadget_ctor_C7C, kRo_CoolGadget_ctor_C80, kRo_CoolGadget_ctor_C84, kRo_CoolGadget_ctor_C88, kRo_CoolGadget_ctor_C8C, kRo_CoolGadget_ctor_C90, kRo_CoolGadget_ctor_C94;
}
inline void *operator new(unsigned n) { return CALL(mem_new)(n); }
#ifdef PORT /* cfront layout (port_cfront.h, plan 17.8): explicit vptr, the NCC vtable store written out */
#include "port_cfront.h"
extern "C" char gl_data_00000000_15fc[]; /* __vtbl__1G = CoolGadget_vtbl, bootup Data 0xC974 */
/* B::B = Node_ctor, M::M = CoolGadgetParms_ctor (__ct__1BFPc / __ct__1MFv -> gl_func_00000000_15fc_base / _m) */
CF_EXT(void *, gl15fc_Node_ctor, Node_ctor, (void *, char *));
CF_EXT(void *, gl15fc_CoolGadgetParms_ctor, CoolGadgetParms_ctor, (void *));
struct B { char p[0x28]; char *vptr; CF_INL B(char *name) { gl15fc_Node_ctor(this, name); } };
struct M { char p[0xA8]; CF_INL M() { gl15fc_CoolGadgetParms_ctor(this); } };
struct G : B { M m; char rest[0x198 - 0xD4]; G(int arg1); };
#else
struct B { char p[0x28]; B(char *); virtual void g(); };
struct M { char p[0xA8]; M(); };
struct G : B { M m; char rest[0x198 - 0xD4]; G(int arg1); virtual void g(); };
#endif
G::G(int arg1) : B(sStr_Node_CC58) {
#ifdef PORT
    vptr = gl_data_00000000_15fc;
#endif
    CoolGadget *self = (CoolGadget *)this;
    float temp_f2 = kRo_CoolGadget_ctor_C7C;
    self->node.name = (const char *)sStr_CoolGadget_CC60;
    self->lowColour.r = temp_f2;
    self->midColour.r = temp_f2;
    self->lowColour.g = 0.0f; self->lowColour.b = 0.0f; self->lowColour.a = 0.0f; self->midColour.g = 0.0f; self->midColour.b = 0.0f; self->midColour.a = 0.0f;
    self->pulseLo.r = kRo_CoolGadget_ctor_C80;
    self->pulseHi.r = 1.0f;
    self->pulseLo.a = 0.0f;
    self->pulseLo.b = 0.0f;
    self->pulseLo.g = 0.0f;
    self->pulseHi.g = kRo_CoolGadget_ctor_C84;
    self->pulseHi.a = 0.0f;
    self->pulseHi.b = 0.0f;
    self->colour[0] = kRo_CoolGadget_ctor_C88;
    self->colour[1] = kRo_CoolGadget_ctor_C8C;
    self->colour[2] = kRo_CoolGadget_ctor_C90;
    self->colour[3] = 0.0f;
    self->total = arg1;
    self->phase = 0.0f;
    self->target = 100;
    self->cur = 100;
    self->step = kRo_CoolGadget_ctor_C94;
    self->labelAlpha = 0x82;
    CALL(gui_spriteInit)(self->barSprite, 0x5000A);
    CALL(gui_spriteInit)(self->capSprite, 0x50009);
    CALL(gui_spriteInit)(((CoolGadget *)this)->label2Sprite, 0x5000B);
    CALL(gui_spriteInit)(((CoolGadget *)this)->label1Sprite, 0x5001C);
    CALL(CoolGadget_setLabels)(((CoolGadget *)this), ((CoolGadget *)this)->label1Sprite, ((CoolGadget *)this)->label2Sprite);
    CALL(CoolGadget_setLabelPos)(self, -0x49, -2, -0x32, -1);
}
