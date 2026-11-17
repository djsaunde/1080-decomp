/* IDO 7.1 NCC (C++) donor for SnowBoard_staticCtor_0000E9FC (0x30 / 12 words): the static
 * initializer (edg `__sti__<stamp>`) NCC generates for a namespace-scope
 * object of a class Dv whose inline default constructor calls the out-of-line
 * base constructor B::B() and then stores Dv's vtable address into the vptr.
 * B holds 0x28 bytes of data, so the vptr (declared by Dv's first virtual)
 * sits at +0x28: `sw t6, 0x28(at)` with the object at 0.
 *
 * Same mechanism as boot_staticInit_00006204 (see bootup_uso_ncc_6204.cpp): the __sti__
 * is emitted at the end of the translation unit and its prologue carries the
 * last source line, while the constructor call carries the earlier object
 * definition line, which is why `lui a0` is scheduled above `sw ra`.
 *
 * Symbols renamed after compile (names only, no bytes change):
 * __sti__<timestamp> -> SnowBoard_staticCtor_0000E9FC, __ct__9BuE9FCBasFv -> func_00000000,
 * __vtbl__8BuE9FCOb -> SnowBoard_vtbl_func_0000E9FC_arg2. The object keeps C linkage under
 * its pinned name gSnowBoard_func_0000E9FC_arg1. */
#ifdef PORT /* BuE9FCBas::BuE9FCBas = App_ctor (the ROM relocation of the __sti__'s jal) */
#include "port_cfront.h"
CF_EXT(void *, buE9FC_App_ctor, App_ctor, (void *));
struct BuE9FCBas {
    CF_INL BuE9FCBas() { buE9FC_App_ctor(this); }
    int w[10];
};
#else
struct BuE9FCBas {
    BuE9FCBas();
    int w[10];
};
#endif

#ifdef PORT /* cfront layout (port_cfront.h, plan 17.8): explicit vptr, the NCC vtable store written out */
extern "C" char SnowBoard_vtbl_func_0000E9FC_arg2[]; /* __vtbl__8BuE9FCOb, bootup Data 0x8BA8 */
struct BuE9FCOb : BuE9FCBas {
    char *vptr;
    BuE9FCOb() { vptr = SnowBoard_vtbl_func_0000E9FC_arg2; }
};
#else
struct BuE9FCOb : BuE9FCBas {
    BuE9FCOb() {}
    virtual void v();
};
#endif

extern "C" {
BuE9FCOb gSnowBoard_func_0000E9FC_arg1;
}

/* end of the translation unit: the __sti__ prologue takes this later line */
