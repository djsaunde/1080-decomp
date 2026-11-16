/* IDO 7.1 NCC (C++) donor for SnowBoarder_loadBoardModel_0000B1B4 (0x2E8 / 186 words) -- sound /
 * resource bank lazy loader: clamp (cat, idx), on the banked categories 6/7
 * build the bank object once and register it, then load + play.
 *
 * The bank object is `new Bank`: a cfront chain Bank : Mid : Base where Base
 * has an out-of-line ctor, two member objects with their own `this == 0`
 * alloc protocol (Pair at +0x34, Word at +0x58) and a vptr at +0x5C. Only
 * Bank's vtable is a real bootup symbol (Snower_hostioParms + 0x70); Base and
 * Mid's are blank USO refs.
 *
 * The four words that kept the cfe/NCC ports off (the counter read into v0,
 * target a1) are the registry: a 5-slot table `{ Base *tbl[5]; int n; }` at
 * func_00000148 with an inline add() that does the overflow check and the
 * post-increment store itself. The expression temp of `tbl[n++]` colours a1;
 * a named `n` local colours v0. The `Base *o = b;` copy is dead code but
 * buys the edg frame slot that puts the later v1 spill at sp+24
 * (docs/IDO_CODEGEN.md kit XVI #6).
 * Spliced into bootup_uso.c.o via REPLACE_FUNC_BODY. Every symbol is
 * extern "C"; blank refs go through bu_b1b4_* aliases (0) in
 * undefined_syms_auto.txt; the three vtables are aliases too. */
typedef short s16; typedef int s32; typedef unsigned char u8; typedef float f32;
#include "../../include/gdl/material.h"
extern "C" {
int func_00000000(...);
extern char D_00000000;
extern char func_000089FC[], func_00008A40[];
extern int gBoardModelUsoPaths_b1b4_tblA[], gBoarderUsoPaths_b1b4_tblB[];
}
struct Bb4Pair { int a, b; Bb4Pair() { b = 0; a = 0; } };
struct Bb4Word { int a; Bb4Word() { a = 0; } };
struct Bb4Base { char p[0x34]; Bb4Pair m; char q[0x58 - 0x3C]; Bb4Word n; Bb4Base() { func_00000000(this); } virtual void v(); };
struct Bb4Mid : Bb4Base { char x[0x30]; Bb4Mid() {} virtual void v(); };
/* agent-g run 555: the real classes. Bb4Base = libgdl BaseMat (its out-of-line ctor is BaseMat_init_000517E4,
 * vtable BaseMat_vtbl Data+0x20640), Bb4Mid = Mat (0x90, Mat_vtbl Data+0x1FBEC), and Bb4Bank = a Mat subclass
 * whose vtable is Data+0x8440 (gl_data.h's "SnowBoarder_vtbl + 0x70"; a separate table after SnowBoarder's):
 * slot 0 = SnowBoarder_grabScreenThumb_0000E740, then BaseMat_hostioParms, Mat_print, gfx_decodeMatCmd,
 * BaseMat_vslot4_000437C0. add() puts it in gGfxLib.frameStartCbs (Bss+0x3C168 +0x148 / count +0x15C), so
 * gfx_start calls slot 0 every frame: a 64x32 RGBA16 texture (texels = mem_new(0x1000)) that E740 refills
 * with every 4th pixel of the main screen. The stores below are BaseMat fields (gdl/material.h). */
#define BM ((BaseMat *)this)
struct Bb4Bank : Bb4Mid {
    Bb4Bank() { BM->format = MAT_FMT_RGBA16; BM->texels = (void *)func_00000000(0x1000); BM->width = 0x40; BM->height = 0x20; BM->texFlags = MAT_TEX_WRAP_S | MAT_TEX_WRAP_T; BM->geomFlags = 0x402C4B; BM->unk34 = 0; BM->id = 0; }
    virtual void v();
};
struct Bb4Reg { Bb4Base *tbl[5]; int n; void add(Bb4Base *b) { if (n >= 5) func_00000000(); Bb4Base *o = b; tbl[n++] = o; } };
extern "C" Bb4Reg func_00000148;
struct Bb4Req { int pad[3]; int a4a, a4b; int pad2[11]; int cat, idx, a2; };
extern "C" void SnowBoarder_loadBoardModel_0000B1B4(unsigned int cat, unsigned int idx, int a2, int a3, int a4) {
    Bb4Req o;
    if (idx >= 10) idx = 9;
    if (cat >= 9) cat = 8;
    o.a4a = a4;
    o.a4b = a4;
    o.a2 = a2;
    o.cat = cat;
    o.idx = idx;
    if (cat == 7 || cat == 6) {
        if (func_00000148.tbl[0] == 0) {
            func_00000000(func_000089FC + 0x2C);
            Bb4Bank *r = new Bb4Bank;
            func_00000148.add(r);
        }
    }
    func_00000000(func_00008A40 + 0x8, 0);
    func_00000000(func_00008A40 + 0x14, 0);
    if (idx != 0) {
        if (func_00000000(gBoardModelUsoPaths_b1b4_tblA[idx]) == 0) func_00000000(&D_00000000, gBoardModelUsoPaths_b1b4_tblA[idx]);
    }
    if (func_00000000(gBoarderUsoPaths_b1b4_tblB[cat]) == 0) func_00000000(&D_00000000, gBoarderUsoPaths_b1b4_tblB[cat]);
    func_00000000();
    func_00000000(&D_00000000, a4, a3, &o);
    func_00000000();
}
