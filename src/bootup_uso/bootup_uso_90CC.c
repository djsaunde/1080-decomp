#include "gdl/gl_data.h"
#include "uso_call.h"
/* IDO 7.1 cfe (C) EXACT donor for SnowBoarder_ctor (agent-c run 226 decode, exact in run 227).
 * The target is cfe output (cfront-style C), not edg: the draw-call this-adjust is `addu a0, delta, ptr`
 * and the site-2 vptr base is loaded once, which only cfe gives. Mechanical port of the NCC donor
 * bootup_uso_ncc_90CC_nm.cpp (98.10 as plain C), then the edg-only fixes were reverted or redone for cfe
 * (docs/IDO_CODEGEN.md kit CCXXXII): if-goto dispatch with `if (1) {}` in both case bodies (beq 7 first),
 * compare operands in target load order, the 0xB4 block written as offsets then `var_s0 += 0xB4`
 * with `temp_f0 = 0;` (the literal `0`, not `0.0f`: one shared f0 zero), no `if (1) {}` after the
 * site-2 vptr load, an empty `if (1) {}` closing the `arg2 == 6` arm (gives the arg1 reload its own
 * join block, so as1 stops copying the `arg8 &= ~4` join and hoisting the d148 / &D_8880 address
 * loads), `var_s0 = temp_s1_8; var_s0 += ...` for the base-first addu, and each draw-argument global
 * loaded into var_s0 first (the target's `lui s0, %hi(g); lw v0, %lo(g)(s0)`).
 * Site 1: `if (1) {}` after the preceding call + a value variable other than var_v1 (v0), the d148 list
 * walk's id in cg (v1). Run 227 (as1 orders equal-ready instructions by source line): cg's load on the
 * `do` line, and vpos.z stored in a goto block (`if (1) {}` keeps its own line, after the call's line)
 * so the 5th-argument store goes first. 2106 words, exact. REPLACE_FUNC_BODY-spliced on both paths. */
#include "common.h"
#include "game/snowboarder.h"
#include "gdl/coll.h"
#include "gdl/light.h"
#include "game/input.h"
#include "game/course.h"
typedef struct { float x, y, z; } Vec3;
extern char D_00000000;
extern char *func_00000000();
extern f32 kRo_SnowBoarder_ctor_868, kRo_SnowBoarder_ctor_86C, kRo_SnowBoarder_ctor_870, kRo_SnowBoarder_ctor_874, kRo_SnowBoarder_ctor_878, kRo_SnowBoarder_ctor_87C, kRo_SnowBoarder_ctor_880, kRo_SnowBoarder_ctor_884, kRo_SnowBoarder_ctor_888, kRo_SnowBoarder_ctor_88C, kRo_SnowBoarder_ctor_890, kRo_SnowBoarder_ctor_894, kRo_SnowBoarder_ctor_898, kRo_SnowBoarder_ctor_89C, kRo_SnowBoarder_ctor_8A0, kRo_SnowBoarder_ctor_8A4, kRo_SnowBoarder_ctor_8A8, kRo_SnowBoarder_ctor_8AC, kRo_SnowBoarder_ctor_8B0, kRo_SnowBoarder_ctor_8B4;
extern char sStr_SnowB_8730;
extern char sStr_Node_8738;
extern char sStr_Parms_8740;
extern char FloatParm_vtbl_8018;
/* per-site aliases of FloatParm_vtbl_8018: bust cross-cascade addr GCSE (target rematerializes lui/addiu per +0xC store, post-jal temp not pre-jal s-reg) */
extern char D_00008018_s02;
extern char D_00008018_s03;
extern char D_00008018_s04;
extern char D_00008018_s05;
extern char D_00008018_s06;
extern char D_00008018_s07;
extern char D_00008018_s08;
extern char D_00008018_s09;
extern char D_00008018_s10;
extern char D_00008018_s11;
extern char D_00008018_s12;
extern char D_00008018_s13;
extern char D_00008018_s14;
extern char D_00008018_s15;
extern char D_00008018_s16;
extern char D_00008018_s17;
extern char D_00008018_s18;
extern char D_00008018_s19;
extern char D_00008018_s20;
extern char D_00008018_s21;
extern char D_00008018_s22;
extern char D_00008018_s23;
extern char D_00008018_s24;
extern char D_00008018_s25;
extern char D_00008018_s26;
extern char D_00008018_s27;
extern char D_00008018_s28;
extern char D_00008018_s29;
extern char D_00008018_s30;
extern char D_00008018_s31;
extern char D_00008018_s32;
extern char D_00008018_s33;
extern char D_00008018_s34;
extern char D_00008018_s35;
extern char D_00008018_s36;
extern char D_00008018_s37;
extern char D_00008018_s38;
extern char IntParm_vtbl_8060;
extern char D_00008060_s2;
extern char D_00008060_s3;
extern char SnowBChild_vtbl_8108;
extern char sStr_Trails_8800;
extern char sStr_DispSpray_8808;
extern char sStr_DispTrails_8814;
extern char sStr_DispTrails_8820;
extern char sStr_Node_882C;
extern char sStr_Node_8834;
extern char sStr_Node_883C;
extern char sStr_Node_8844;
extern char sStr_AnimationData_884C;
extern char sStr_lighttarget_885C;
extern char sFmt_Global_out_of_range_8868;
extern char sStr_Trails_8880;
extern char sStr_Trails_8888;
/* SnowBoarder_ctor alloc-cascade +0x28 child-vtable pointers: USO relocs blanked in the
 * .s (lui/addiu 0x0). Resolved from bootup.uso TextReloc @0xd9fe28: symIdx 545 =
 * shared 0x174-fallback vtable (imports), 546/547/548 = per-class vtables (imports),
 * symIdx 312 = internal data +0x207DC. All =0 in undefined_syms_auto.txt (bytes match). */
extern char bu_90cc_vt545;
extern char bu_90cc_vt545_b;
extern char bu_90cc_vt545_c;
extern char bu_90cc_vt545_d;  /* per-cascade base-0 aliases of symIdx 545 -- bust cross-cascade addr GCSE (target rematerializes per store site) */
extern char bu_90cc_vt546;
extern char bu_90cc_vt547;
extern char bu_90cc_vt548;
extern char bu_90cc_vt548_b;
extern char LightTarget_vtbl_90cc_d207dc;
extern char bu_90cc_imp30;  /* +0x30 import ptr in the 0x878 cascade (target 0xA2F8 lui/addiu/sw pair; was decoded as literal 0) */
/* target 0xAC34-0xAD3C per-site blank globals (each site = own symbol; busts base GCSE).
 * Scalar/array form (not cast-arith) so the lo16 folds into the load offset (lui rd; lw rd,4(rd))
 * and stores go through $at -- matches target's direct-global access shape. */
extern char gPolyFlags_3EEC4_90cc_g8_ld, gPolyFlags_3EEC4_90cc_g8_st;      /* load/store alias pair: same address, distinct syms kill the addr CSE (lui rd/lw + lui at/sw shape) */


extern s32 bu_90cc_gown;
extern char gNodeGlobals_90cc_gtbl;
/* gPolyGeomMode_3EEC8 (gl_data.h): two mode words, [1] = the PolyShape geometry-mode bits */
typedef struct { s32 mode0; s32 geomMode; } GeomModePair_90cc;
extern GeomModePair_90cc gPolyGeomMode_3EEC8_90cc_gpair;
extern char gGfxLib_90cc_gdev;
extern char SnowBoarder_vtbl_90cc_vt_self;  /* self +0x28 vtable (was decoded as literal 0; expected has lui/addiu reloc pair) */
/* vtable draw slot (+0x5C): trailing two args are single-precision f32.
 * Prototyping defeats the K&R float->double promotion so single swc1
 * arg stores emit (matches target; no cvt.d.s/sdc1). */
typedef void (*Vfn_ff)(char *, s32, s32, void *, s32, s32, f32, f32);
typedef struct { s32 w; } W90;
extern char sStr_SnowBoarder_Parms_8778;
extern void bu_90cc_call_p4f(void *, char *, s32, PORT64_FT, f32); /* CollObj_initCylinder: PORT64_FT, the O32 float bits (include/uso_call.h) */
extern void bu_90cc_call_iipif(s32, s32, void *, f32, f32);
/* PASS-2 2026-06-10 (big-swing): FULL 2106-insn m2c graft replacing the
 * 30-insn stub. Raw-word pipeline: .s words -> objdump blob -> label-
 * resolved .s -> m2c (1084 lines, 0 errors, 163 labels). All callees
 * collapsed to func_00000000, all D-refs to D_00000000/absolutes;
 * types s32-defaulted. Expect a large structural jump from 2.74; the
 * refinement passes (callee identities, float pool symbols, struct
 * shapes) follow.
 * PASS-3 2026-07-11 (agent-g): field-width decode fixes (fuzzy
 * 56.79->57.27%). var_s2->0x900/0x902/0x88C/0x88E/0x904 are halfword
 * fields (target sh), were s32 -> s16. temp_s0->0x58 (4 draw-vfn base
 * reads) are halfword (target lh) -> s16. &D+0x17D is a byte flag (target
 * lbu) -> u8. (RGB triples at +0x8688.. already u8.) Remainder is the
 * ~198-insn m2c-collapsed callee/cascade reconstruction gap + regalloc. */
/* PASS-4 2026-10-16 (agent-c, 94.19 -> 95.2x fuzzy): FRAME 0x2D8 -> 0x178 exact.
 * With any address-taken / memory local in the function, IDO gives EVERY
 * declared local its own frame slot (register-allocated and even unused ones),
 * laid out downward in declaration order, and a spilled variable uses its own
 * slot. The m2c graft declared ~110 locals; the target has 19 non-sp ones.
 * Fixes: the 41 per-cascade var_a0_N collapse into sp108 / sp80 / sp8C (the
 * target's spill homes 264/128/140); unused temps deleted; the block-scoped
 * rgb temps hoisted (prgb stays `register`); single-use temps merged where
 * live ranges are disjoint; sp174 IS the flag (spill home 372) and sp58 IS
 * the 0..0x22 loop counter (home 88); decls reordered so the register-only
 * locals fill the target's gaps (4 above sp144, var_a2 + 2 below sp118, ...).
 * Self is the PARAM (move s2,a0 inside the save sequence, IDO_CODEGEN
 * #prologue-move-param-vs-copy-62368). The four rgb table bases are per-site
 * symbols (target lui/addiu %hi/%lo(Tree_handleMsg+0x24..0x30) + lbu 0/1/2).
 * Every sp-relative word now lands on the target slot. Residual: the arg2
 * 7/6 dispatch order (target beq 7 first; switch / if-goto / split-switch
 * spellings all scored lower), arg1 a3-vs-t4, the u32->f32 `lui at,0x4f80`
 * schedule, vt548 hoist, and the 0x1BE0..0x1F94 call-tail scheduling. */
/* data aliases of splat function-anchored bases (target relocs: Tree_handleMsg+40.., stream_readQuad_000000F0+0x44, stream_readF32_00000044/80/08+K, snd_seqEventDbg_00000148+24, SndHandle_setScaleIdx15+4/8, TreeTmpl_create+0x5C/0x64).
 * A fn-typed symbol never folds its addend into %lo (separate addiu) and cfe rejects a data redeclaration; a data alias folds. */
extern u8 bu_90cc_rgb28[], bu_90cc_rgb24[], bu_90cc_rgb30[], bu_90cc_rgb2C[];
extern u8 bu_90cc_d8664[], bu_90cc_d87A4[], gInputMgr_90cc_dF0[], kRo_SnowBoarder_ctor_8B8[], kRo_SnowBoarder_ctor_8BC[];
/* RoData offsets of the two f32 393.0 words SnowBoarder_ctor passes to the trail / spray vslot 10 calls
 * (uso-data-sites: RoData-base+0x8B8 / +0x8BC). Both per-site symbols are valued 0x8B4 (the start of
 * that float run, undefined_syms_auto.txt), so each site is symbol + addend; these name the addends. */
#define kRoF_SnowBoarder_ctor_8B8 4 /* RoData+0x8B8 = 0x8B4 + 4: f32 393.0 */
#define kRoF_SnowBoarder_ctor_8BC 8 /* RoData+0x8BC = 0x8B4 + 8: f32 393.0 */
/* per-site aliases (one symbol per access) bust the shared-base GCSE that turns lui/%lo-folded loads into base+K */
extern u8 gGfxLib_90cc_d148_01[], bu_90cc_d148_02[], bu_90cc_d148_03[];
extern u8 gGfxLib_90cc_d148_18[];
extern void bu_90cc_d00_b();  /* second read of D_00000000.0 in the sp174 list walk: distinct symbol keeps the target's reload (else CSE'd into a copy) */
typedef struct { s32 f00, f04, f08, f0C, f10, f14, f18, f1C, f20, f24, f28, f2C, f30, f34, f38; } BuW90;
/* GameFlow through the stream_readQuad_000000F0 + 0x44 alias (as bootup_uso.c's FLOW_74E8_H): ->game = +0x134 */
#define FLOW_90CC GAMEFLOW((char *)gInputMgr_90cc_dF0 - 0xF0)
extern BuW90 gPolyGeomMode_3EEC8_90cc_gcfg_ld, gPolyGeomMode_3EEC8_90cc_gcfg_st, gPolyGeomMode_3EEC8_90cc_gcfg_b_ld, gPolyGeomMode_3EEC8_90cc_gcfg_b_st;
/* scalar-field form: a named global field read is a uopt candidate (target lui/lw temps land in s0/s1/a2, not the t-ring) */
extern BuW90 gNodeGlobals_90cc_d44_01, gNodeGlobals_90cc_d44_02, gNodeGlobals_90cc_d44_03, gNodeGlobals_90cc_d44_04, gNodeGlobals_90cc_d44_05, gNodeGlobals_90cc_d44_06, gNodeGlobals_90cc_d44_07, gNodeGlobals_90cc_d44_08, gNodeGlobals_90cc_d44_09, gInputMgr_90cc_d44_10, gNodeGlobals_90cc_d08_01, gNodeGlobals_90cc_d08_02, gNodeGlobals_90cc_d08_03, gNodeGlobals_90cc_d08_04, gNodeGlobals_90cc_d80_01, gNodeGlobals_90cc_d80_02, gNodeGlobals_90cc_d80_03;
#ifdef PORT64 /* QNode_ctor(self, name, x, y, z): the ROM's 0.0 double fills x and y (a2 / a3), x86-64 passes three floats */
extern void bu_90cc_call_p4d(void *, char *, f32, f32, f32);
#else
extern void bu_90cc_call_p4d(void *, char *, f64, f32);
#endif
extern void gSkinMgrs_41700_90cc_fz();  /* 0x814/818/81C source base: target lui v0/addiu v0 (no reloc) then lw 0/8/4(v0) -- a fn-typed symbol keeps the %lo in an addiu so the base is CSEd in one register */
/* bu_90cc_fz2 rows (0x18 bytes, zero-rider terminated): rider id, ddm slot, then the ddm data
 * whose address SnowBoarder_ctor stores into the slot when the id matches its rider (arg2) */
typedef struct { s32 rider; s32 slot; char ddm[0x10]; } RiderDdmRow_90cc;
extern void bu_90cc_fz2();  /* sp174 list-walk head (same blank address), fn-typed for the same reason */

#ifdef PORT /* one object for the bytes these tokens share, so gcc keeps the order (scripts/port-alias-audit.py, plan 18.7) */
extern char port_bootup_bss[];
#define gGfxLib_90cc_d148_01 (*(__typeof__(gGfxLib_90cc_d148_01) *)(port_bootup_bss + 0x3C2B0))
#define gGfxLib_90cc_d148_18 (*(__typeof__(gGfxLib_90cc_d148_18) *)(port_bootup_bss + 0x3C2C8))
#define gNodeGlobals_90cc_d80_01 (*(__typeof__(gNodeGlobals_90cc_d80_01) *)(port_bootup_bss + 0x3C0D8))
#define gNodeGlobals_90cc_gtbl (*(__typeof__(gNodeGlobals_90cc_gtbl) *)(port_bootup_bss + 0x3C058))
#define gPolyFlags_3EEC4_90cc_g8_ld (*(__typeof__(gPolyFlags_3EEC4_90cc_g8_ld) *)(port_bootup_bss + 0x3EEC4))
#define gPolyFlags_3EEC4_90cc_g8_st (*(__typeof__(gPolyFlags_3EEC4_90cc_g8_st) *)(port_bootup_bss + 0x3EEC4))
#define gPolyGeomMode_3EEC8_90cc_gcfg_b_ld (*(__typeof__(gPolyGeomMode_3EEC8_90cc_gcfg_b_ld) *)(port_bootup_bss + 0x3EEC8))
#define gPolyGeomMode_3EEC8_90cc_gcfg_b_st (*(__typeof__(gPolyGeomMode_3EEC8_90cc_gcfg_b_st) *)(port_bootup_bss + 0x3EEC8))
#define gPolyGeomMode_3EEC8_90cc_gcfg_ld (*(__typeof__(gPolyGeomMode_3EEC8_90cc_gcfg_ld) *)(port_bootup_bss + 0x3EEC8))
#define gPolyGeomMode_3EEC8_90cc_gcfg_st (*(__typeof__(gPolyGeomMode_3EEC8_90cc_gcfg_st) *)(port_bootup_bss + 0x3EEC8))
#define gPolyGeomMode_3EEC8_90cc_gpair (*(__typeof__(gPolyGeomMode_3EEC8_90cc_gpair) *)(port_bootup_bss + 0x3EEC8))
#endif
/* PC port (plan 18.8): the XDATA sites are hand-bound from the ROM TextRelocs (the verify compile
 * of this donor-recipe body differs): Bss+0xF8 (gInputMgr; the addiu bases at +0x12CC / +0x12EC
 * that the +0x34 / +0x4C / +0x58 / +0x17D field reads share), then the three word loads
 * Bss+0x10 (+0x16BC, CollObjMgr_addDynamic a0), Bss+0x50 (+0x16D4, a0), Bss+0xD8 (+0x1B30, a1). */
void *SnowBoarder_ctor(char *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5, f32 arg6, s32 arg7, s32 arg8) {
    s32 sp174;
    Vec3 vpos;
    char *sp164;
    char *sp160;
    char *sp15C;
    char *sp158;
    s32 var_t1 = 0;
    char *var_s0;
    char *var_s1;
    char *temp_v0_5;
    W90 sp144;
    W90 sp140;
    W90 sp13C;
    W90 sp138;
    W90 sp134;
    W90 sp130;
    W90 sp12C;
    W90 sp128;
    W90 sp124;
    W90 sp120;
    W90 sp11C;
    W90 sp118;
    s32 var_a2;
    char *temp_s1;
    char *temp_s1_8;
    char *sp108;
    W90 sp104;
    W90 sp100;
    W90 spFC;
    W90 spF8;
    W90 spF4;
    W90 spF0;
    W90 spEC;
    W90 spE8;
    W90 spE4;
    W90 spE0;
    W90 spDC;
    W90 spD8;
    W90 spD4;
    W90 spD0;
    W90 spCC;
    W90 spC8;
    W90 spC4;
    W90 spC0;
    W90 spBC;
    W90 spB8;
    W90 spB4;
    W90 spB0;
    W90 spAC;
    W90 spA8;
    W90 spA4;
    W90 spA0;
    W90 sp9C;
    W90 sp98;
    W90 sp94;
    W90 sp90;
    char *sp8C;
    s32 *fq;
    s32 *var_s0_3 = 0;
    char *sp80;
    W90 sp7C;
    register u8 *prgb;
    u32 cr;
    u32 cg;
    u32 cb;
    f32 inv;
    Vec3 wpos;
    u32 sp58;
    f32 temp_f0;
    s32 var_v1;
    s32 temp_v0_51;

    if ((arg0 != 0) || (temp_v0_5 = FCALL(mem_new)(0xA8C), arg0 = temp_v0_5, (temp_v0_5 != 0))) {
        FCALL(DNode_ctor)(arg0, &sStr_SnowB_8730);
        NODE(arg0)->vptr = (char *)&SnowBoarder_vtbl_90cc_vt_self;
        var_s0 = arg0 + 0x3EC;
        if (((arg0 != (void *)-0x3EC) || ((var_s0 = FCALL(mem_new)(0x154)) != 0)) && ((FCALL(Node_ctor)(var_s0, &sStr_Node_8738), NODE(var_s0)->vptr = (char *)&SnowBChild_vtbl_8108, var_s1 = var_s0 + 0x2C, (var_s0 != (void *)-0x2C)) || (temp_v0_5 = FCALL(mem_new)(0x128), var_s1 = temp_v0_5, (temp_v0_5 != 0)))) {
            var_a2 = (s32)var_s1;
            if ((var_s1 != 0) || (temp_v0_5 = FCALL(mem_new)(8), var_a2 = (s32)temp_v0_5, (temp_v0_5 != 0))) {
                PARMS(var_a2)->name = &sStr_Parms_8740;
                PARMS(var_a2)->first = 0;
            }
            sp144.w = *(s32 *)(GL_DATA_STR(0x8748, "ied"));
            sp108 = var_s1 + 8;
            sp104 = sp144;
            if ((var_s1 != (void *)-8) || (temp_v0_5 = FCALL(mem_new)(0x18), sp108 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp108, var_s1, sp104, 1);
                PARMF(sp108)->type = &FloatParm_vtbl_8018;
                PARMF(sp108)->unk14 = 0;
                PARMF(sp108)->value = kRo_SnowBoarder_ctor_868;
            }
            sp140.w = *(s32 *)(GL_DATA_STR(0x874C, "ifr"));
            sp108 = var_s1 + 0x20;
            sp104 = sp140;
            if ((var_s1 != (void *)-0x20) || (temp_v0_5 = FCALL(mem_new)(0x18), sp108 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp108, var_s1, sp104, 1);
                PARMF(sp108)->type = &D_00008018_s02;
                PARMF(sp108)->unk14 = 0;
                PARMF(sp108)->value = kRo_SnowBoarder_ctor_86C;
            }
            sp13C.w = *(s32 *)(GL_DATA_STR(0x8750, "ded"));
            sp108 = var_s1 + 0x38;
            sp104 = sp13C;
            if ((var_s1 != (void *)-0x38) || (temp_v0_5 = FCALL(mem_new)(0x18), sp108 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp108, var_s1, sp104, 1);
                PARMF(sp108)->type = &D_00008018_s03;
                PARMF(sp108)->unk14 = 0;
                PARMF(sp108)->value = 2.0f;
            }
            sp138.w = *(s32 *)(GL_DATA_STR(0x8754, "dfr"));
            sp108 = var_s1 + 0x50;
            sp104 = sp138;
            if ((var_s1 != (void *)-0x50) || (temp_v0_5 = FCALL(mem_new)(0x18), sp108 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp108, var_s1, sp104, 1);
                PARMF(sp108)->type = &D_00008018_s04;
                PARMF(sp108)->unk14 = 0;
                PARMF(sp108)->value = kRo_SnowBoarder_ctor_870;
            }
            sp134.w = *(s32 *)(GL_DATA_STR(0x8758, "efr"));
            sp108 = var_s1 + 0x68;
            sp104 = sp134;
            if ((var_s1 != (void *)-0x68) || (temp_v0_5 = FCALL(mem_new)(0x18), sp108 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp108, var_s1, sp104, 1);
                PARMF(sp108)->type = &D_00008018_s05;
                PARMF(sp108)->unk14 = 0;
                PARMF(sp108)->value = kRo_SnowBoarder_ctor_874;
            }
            sp130.w = *(s32 *)(GL_DATA_STR(0x875C, "nbf"));
            sp108 = var_s1 + 0x80;
            sp104 = sp130;
            if ((var_s1 != (void *)-0x80) || (temp_v0_5 = FCALL(mem_new)(0x18), sp108 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp108, var_s1, sp104, 1);
                PARMF(sp108)->type = &D_00008018_s06;
                PARMF(sp108)->unk14 = 0;
                PARMF(sp108)->value = kRo_SnowBoarder_ctor_878;
            }
            sp12C.w = *(s32 *)(GL_DATA_STR(0x8760, "nel"));
            sp108 = var_s1 + 0x98;
            sp104 = sp12C;
            if ((var_s1 != (void *)-0x98) || (temp_v0_5 = FCALL(mem_new)(0x18), sp108 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp108, var_s1, sp104, 1);
                PARMF(sp108)->type = &D_00008018_s07;
                PARMF(sp108)->unk14 = 0;
                PARMF(sp108)->value = kRo_SnowBoarder_ctor_87C;
            }
            sp128.w = *(s32 *)(GL_DATA_STR(0x8764, "dbf"));
            sp108 = var_s1 + 0xB0;
            sp104 = sp128;
            if ((var_s1 != (void *)-0xB0) || (temp_v0_5 = FCALL(mem_new)(0x18), sp108 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp108, var_s1, sp104, 1);
                PARMF(sp108)->type = &D_00008018_s08;
                PARMF(sp108)->unk14 = 0;
                PARMF(sp108)->value = 0.0f;
            }
            sp124.w = *(s32 *)(GL_DATA_STR(0x8768, "del"));
            sp108 = var_s1 + 0xC8;
            sp104 = sp124;
            if ((var_s1 != (void *)-0xC8) || (temp_v0_5 = FCALL(mem_new)(0x18), sp108 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp108, var_s1, sp104, 1);
                PARMF(sp108)->type = &D_00008018_s09;
                PARMF(sp108)->unk14 = 0;
                PARMF(sp108)->value = kRo_SnowBoarder_ctor_880;
            }
            sp120.w = *(s32 *)(GL_DATA_STR(0x876C, "res"));
            sp108 = var_s1 + 0xE0;
            sp104 = sp120;
            if ((var_s1 != (void *)-0xE0) || (temp_v0_5 = FCALL(mem_new)(0x18), sp108 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp108, var_s1, sp104, 1);
                PARMF(sp108)->type = &D_00008018_s10;
                PARMF(sp108)->unk14 = 0;
                PARMF(sp108)->value = 1.0f;
            }
            sp11C.w = *(s32 *)(GL_DATA_STR(0x8770, "flx"));
            sp108 = var_s1 + 0xF8;
            sp104 = sp11C;
            if ((var_s1 != (void *)-0xF8) || (temp_v0_5 = FCALL(mem_new)(0x18), sp108 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp108, var_s1, sp104, 1);
                PARMF(sp108)->type = &D_00008018_s11;
                PARMF(sp108)->unk14 = 0;
                PARMF(sp108)->value = 1.0f;
            }
            sp118.w = *(s32 *)(GL_DATA_STR(0x8774, "sta"));
            sp108 = var_s1 + 0x110;
            sp104 = sp118;
            if ((var_s1 != (void *)-0x110) || (temp_v0_5 = FCALL(mem_new)(0x18), sp108 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp108, var_s1, sp104, 1);
                PARMF(sp108)->type = &D_00008018_s12;
                PARMF(sp108)->unk14 = 0;
                PARMF(sp108)->value = 1.0f;
            }
        }
        var_s0 = arg0 + 0x540;
        if ((arg0 != (void *)-0x540) || (temp_v0_5 = FCALL(mem_new)(0x2C0), var_s0 = temp_v0_5, (temp_v0_5 != 0))) {
            var_s1 = var_s0;
            if ((var_s0 != 0) || (temp_v0_5 = FCALL(mem_new)(8), var_s1 = temp_v0_5, (temp_v0_5 != 0))) {
                PARMS(var_s1)->name = &sStr_SnowBoarder_Parms_8778;
                PARMS(var_s1)->first = 0;
            }
            sp100.w = *(s32 *)(GL_DATA_STR(0x878C, "hpf"));
            sp8C = var_s0 + 8;
            sp104 = sp100;
            if ((var_s0 != (void *)-8) || (temp_v0_5 = FCALL(mem_new)(0x18), sp8C = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp8C, var_s0, sp104, 1);
                PARMF(sp8C)->type = &D_00008018_s13;
                PARMF(sp8C)->unk14 = 0;
                PARMF(sp8C)->value = kRo_SnowBoarder_ctor_884;
            }
            spFC.w = *(s32 *)(GL_DATA_STR(0x8790, "snf"));
            var_s1 = (char *)&sp7C;
            sp80 = var_s0 + 0x20;
            *(W90 *)var_s1 = spFC;
            if ((var_s0 != (void *)-0x20) || (temp_v0_5 = FCALL(mem_new)(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp80, var_s0, *(W90 *)var_s1, 1);
                PARMF(sp80)->type = &D_00008018_s14;
                PARMF(sp80)->unk14 = 0;
                PARMF(sp80)->value = 1.0f;
            }
            spF8.w = *(s32 *)(GL_DATA_STR(0x8794, "dsf"));
            sp80 = var_s0 + 0x38;
            *(W90 *)var_s1 = spF8;
            if ((var_s0 != (void *)-0x38) || (temp_v0_5 = FCALL(mem_new)(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp80, var_s0, *(W90 *)var_s1, 1);
                PARMF(sp80)->type = &D_00008018_s15;
                PARMF(sp80)->unk14 = 0;
                PARMF(sp80)->value = 1.0f;
            }
            spF4.w = *(s32 *)(GL_DATA_STR(0x8798, "icf"));
            sp80 = var_s0 + 0x50;
            *(W90 *)var_s1 = spF4;
            if ((var_s0 != (void *)-0x50) || (temp_v0_5 = FCALL(mem_new)(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp80, var_s0, *(W90 *)var_s1, 1);
                PARMF(sp80)->type = &D_00008018_s16;
                PARMF(sp80)->unk14 = 0;
                PARMF(sp80)->value = 1.0f;
            }
            spF0.w = *(s32 *)(GL_DATA_STR(0x879C, "tfc"));
            sp80 = var_s0 + 0x68;
            *(W90 *)var_s1 = spF0;
            if ((var_s0 != (void *)-0x68) || (temp_v0_5 = FCALL(mem_new)(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp80, var_s0, *(W90 *)var_s1, 1);
                PARMF(sp80)->type = &D_00008018_s17;
                PARMF(sp80)->unk14 = 0;
                PARMF(sp80)->value = 1.0f;
            }
            spEC.w = *(s32 *)(GL_DATA_STR(0x87A0, "edf"));
            sp80 = var_s0 + 0x80;
            *(W90 *)var_s1 = spEC;
            if ((var_s0 != (void *)-0x80) || (temp_v0_5 = FCALL(mem_new)(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp80, var_s0, *(W90 *)var_s1, 1);
                PARMF(sp80)->type = &D_00008018_s18;
                PARMF(sp80)->unk14 = 0;
                PARMF(sp80)->value = 1.0f;
            }
            spE8.w = *(s32 *)(GL_DATA_STR(0x87A4, "bdf"));
            sp80 = var_s0 + 0x98;
            *(W90 *)var_s1 = spE8;
            if ((var_s0 != (void *)-0x98) || (temp_v0_5 = FCALL(mem_new)(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp80, var_s0, *(W90 *)var_s1, 1);
                PARMF(sp80)->type = &D_00008018_s19;
                PARMF(sp80)->unk14 = 0;
                PARMF(sp80)->value = 1.0f;
            }
            spE4.w = *(s32 *)(GL_DATA_STR(0x87A8, "ure"));
            sp80 = var_s0 + 0xB0;
            *(W90 *)var_s1 = spE4;
            if ((var_s0 != (void *)-0xB0) || (temp_v0_5 = FCALL(mem_new)(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp80, var_s0, *(W90 *)var_s1, 1);
                PARMF(sp80)->type = &D_00008018_s20;
                PARMF(sp80)->unk14 = 0;
                PARMF(sp80)->value = kRo_SnowBoarder_ctor_888;
            }
            spE0.w = *(s32 *)(GL_DATA_STR(0x87AC, "mro"));
            sp80 = var_s0 + 0xC8;
            *(W90 *)var_s1 = spE0;
            if ((var_s0 != (void *)-0xC8) || (temp_v0_5 = FCALL(mem_new)(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp80, var_s0, *(W90 *)var_s1, 1);
                PARMF(sp80)->type = &D_00008018_s21;
                PARMF(sp80)->unk14 = 0;
                PARMF(sp80)->value = kRo_SnowBoarder_ctor_88C;
            }
            spDC.w = *(s32 *)(GL_DATA_STR(0x87B0, "mpi"));
            sp80 = var_s0 + 0xE0;
            *(W90 *)var_s1 = spDC;
            if ((var_s0 != (void *)-0xE0) || (temp_v0_5 = FCALL(mem_new)(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp80, var_s0, *(W90 *)var_s1, 1);
                PARMF(sp80)->type = &D_00008018_s22;
                PARMF(sp80)->unk14 = 0;
                PARMF(sp80)->value = kRo_SnowBoarder_ctor_890;
            }
            spD8.w = *(s32 *)(GL_DATA_STR(0x87B4, "mdr"));
            sp80 = var_s0 + 0xF8;
            *(W90 *)var_s1 = spD8;
            if ((var_s0 != (void *)-0xF8) || (temp_v0_5 = FCALL(mem_new)(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp80, var_s0, *(W90 *)var_s1, 1);
                PARMF(sp80)->type = &D_00008018_s23;
                PARMF(sp80)->unk14 = 0;
                PARMF(sp80)->value = 1.0f;
            }
            spD4.w = *(s32 *)(GL_DATA_STR(0x87B8, "axr"));
            sp80 = var_s0 + 0x110;
            *(W90 *)var_s1 = spD4;
            if ((var_s0 != (void *)-0x110) || (temp_v0_5 = FCALL(mem_new)(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp80, var_s0, *(W90 *)var_s1, 1);
                PARMF(sp80)->type = &D_00008018_s24;
                PARMF(sp80)->unk14 = 0;
                PARMF(sp80)->value = kRo_SnowBoarder_ctor_894;
            }
            spD0.w = *(s32 *)(GL_DATA_STR(0x87BC, "ayr"));
            sp80 = var_s0 + 0x128;
            *(W90 *)var_s1 = spD0;
            if ((var_s0 != (void *)-0x128) || (temp_v0_5 = FCALL(mem_new)(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp80, var_s0, *(W90 *)var_s1, 1);
                PARMF(sp80)->type = &D_00008018_s25;
                PARMF(sp80)->unk14 = 0;
                PARMF(sp80)->value = kRo_SnowBoarder_ctor_898;
            }
            spCC.w = *(s32 *)(GL_DATA_STR(0x87C0, "azr"));
            sp80 = var_s0 + 0x140;
            *(W90 *)var_s1 = spCC;
            if ((var_s0 != (void *)-0x140) || (temp_v0_5 = FCALL(mem_new)(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp80, var_s0, *(W90 *)var_s1, 1);
                PARMF(sp80)->type = &D_00008018_s26;
                PARMF(sp80)->unk14 = 0;
                PARMF(sp80)->value = kRo_SnowBoarder_ctor_89C;
            }
            spC8.w = *(s32 *)(GL_DATA_STR(0x87C4, "tlt"));
            sp80 = var_s0 + 0x158;
            *(W90 *)var_s1 = spC8;
            if ((var_s0 != (void *)-0x158) || (temp_v0_5 = FCALL(mem_new)(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp80, var_s0, *(W90 *)var_s1, 1);
                PARMF(sp80)->type = &D_00008018_s27;
                PARMF(sp80)->unk14 = 0;
                PARMF(sp80)->value = kRo_SnowBoarder_ctor_8A0;
            }
            spC4.w = *(s32 *)(GL_DATA_STR(0x87C8, "wfr"));
            sp80 = var_s0 + 0x170;
            *(W90 *)var_s1 = spC4;
            if ((var_s0 != (void *)-0x170) || (temp_v0_5 = FCALL(mem_new)(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp80, var_s0, *(W90 *)var_s1, 1);
                PARMF(sp80)->type = &D_00008018_s28;
                PARMF(sp80)->unk14 = 0;
                PARMF(sp80)->value = 0.0f;
            }
            spC0.w = *(s32 *)(GL_DATA_STR(0x87CC, "sfr"));
            sp80 = var_s0 + 0x188;
            *(W90 *)var_s1 = spC0;
            if ((var_s0 != (void *)-0x188) || (temp_v0_5 = FCALL(mem_new)(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp80, var_s0, *(W90 *)var_s1, 1);
                PARMF(sp80)->type = &D_00008018_s29;
                PARMF(sp80)->unk14 = 0;
                PARMF(sp80)->value = 0.0f;
            }
            spBC.w = *(s32 *)(GL_DATA_STR(0x87D0, "dts"));
            sp80 = var_s0 + 0x1A0;
            *(W90 *)var_s1 = spBC;
            if ((var_s0 != (void *)-0x1A0) || (temp_v0_5 = FCALL(mem_new)(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp80, var_s0, *(W90 *)var_s1, 1);
                PARMF(sp80)->type = &D_00008018_s30;
                PARMF(sp80)->unk14 = 0;
                PARMF(sp80)->value = kRo_SnowBoarder_ctor_8A4;
            }
            spB8.w = *(s32 *)(GL_DATA_STR(0x87D4, "nts"));
            sp80 = var_s0 + 0x1B8;
            *(W90 *)var_s1 = spB8;
            if ((var_s0 != (void *)-0x1B8) || (temp_v0_5 = FCALL(mem_new)(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp80, var_s0, *(W90 *)var_s1, 1);
                PARMF(sp80)->type = &D_00008018_s31;
                PARMF(sp80)->unk14 = 0;
                PARMF(sp80)->value = kRo_SnowBoarder_ctor_8A8;
            }
            spB4.w = *(s32 *)(GL_DATA_STR(0x87D8, "trt"));
            sp80 = var_s0 + 0x1D0;
            *(W90 *)var_s1 = spB4;
            if ((var_s0 != (void *)-0x1D0) || (temp_v0_5 = FCALL(mem_new)(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp80, var_s0, *(W90 *)var_s1, 1);
                PARMF(sp80)->type = &D_00008018_s32;
                PARMF(sp80)->unk14 = 0;
                PARMF(sp80)->value = kRo_SnowBoarder_ctor_8AC;
            }
            spB0.w = *(s32 *)(GL_DATA_STR(0x87DC, "twt"));
            sp80 = var_s0 + 0x1E8;
            *(W90 *)var_s1 = spB0;
            if ((var_s0 != (void *)-0x1E8) || (temp_v0_5 = FCALL(mem_new)(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp80, var_s0, *(W90 *)var_s1, 1);
                PARM(sp80)->type = &IntParm_vtbl_8060; PARM(sp80)->value = 0x19;
                PARM(sp80)->unk14 = 0;
            }
            spAC.w = *(s32 *)(GL_DATA_STR(0x87E0, "min"));
            sp80 = var_s0 + 0x200;
            *(W90 *)var_s1 = spAC;
            if ((var_s0 != (void *)-0x200) || (temp_v0_5 = FCALL(mem_new)(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp80, var_s0, *(W90 *)var_s1, 1);
                PARMF(sp80)->type = &D_00008018_s33;
                PARMF(sp80)->unk14 = 0;
                PARMF(sp80)->value = 50.0f;
            }
            spA8.w = *(s32 *)(GL_DATA_STR(0x87E4, "max"));
            sp80 = var_s0 + 0x218;
            *(W90 *)var_s1 = spA8;
            if ((var_s0 != (void *)-0x218) || (temp_v0_5 = FCALL(mem_new)(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp80, var_s0, *(W90 *)var_s1, 1);
                PARMF(sp80)->type = &D_00008018_s34;
                PARMF(sp80)->unk14 = 0;
                PARMF(sp80)->value = 150.0f;
            }
            spA4.w = *(s32 *)(GL_DATA_STR(0x87E8, "jmp"));
            sp80 = var_s0 + 0x230;
            *(W90 *)var_s1 = spA4;
            if ((var_s0 != (void *)-0x230) || (temp_v0_5 = FCALL(mem_new)(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp80, var_s0, *(W90 *)var_s1, 1);
                PARMF(sp80)->type = &D_00008018_s35;
                PARMF(sp80)->unk14 = 0;
                PARMF(sp80)->value = 100.0f;
            }
            spA0.w = *(s32 *)(GL_DATA_STR(0x87EC, "djp"));
            sp80 = var_s0 + 0x248;
            *(W90 *)var_s1 = spA0;
            if ((var_s0 != (void *)-0x248) || (temp_v0_5 = FCALL(mem_new)(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp80, var_s0, *(W90 *)var_s1, 1);
                PARMF(sp80)->type = &D_00008018_s36;
                PARMF(sp80)->unk14 = 0;
                PARMF(sp80)->value = 100.0f;
            }
            sp9C.w = *(s32 *)(GL_DATA_STR(0x87F0, "ufs"));
            sp80 = var_s0 + 0x260;
            *(W90 *)var_s1 = sp9C;
            if ((var_s0 != (void *)-0x260) || (temp_v0_5 = FCALL(mem_new)(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp80, var_s0, *(W90 *)var_s1, 1);
                PARMF(sp80)->type = &D_00008018_s37;
                PARMF(sp80)->unk14 = 0;
                PARMF(sp80)->value = 0.0f;
            }
            sp98.w = *(s32 *)(GL_DATA_STR(0x87F4, "bdm"));
            sp80 = var_s0 + 0x278;
            *(W90 *)var_s1 = sp98;
            if ((var_s0 != (void *)-0x278) || (temp_v0_5 = FCALL(mem_new)(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp80, var_s0, *(W90 *)var_s1, 1);
                PARM(sp80)->type = &D_00008060_s2; PARM(sp80)->value = 0xA;
                PARM(sp80)->unk14 = 0;
            }
            sp94.w = *(s32 *)(GL_DATA_STR(0x87F8, "cdm"));
            sp80 = var_s0 + 0x290;
            *(W90 *)var_s1 = sp94;
            if ((var_s0 != (void *)-0x290) || (temp_v0_5 = FCALL(mem_new)(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp80, var_s0, *(W90 *)var_s1, 1);
                PARM(sp80)->type = &D_00008060_s3; PARM(sp80)->value = 0xF;
                PARM(sp80)->unk14 = 0;
            }
            sp90.w = *(s32 *)(GL_DATA_STR(0x87FC, "tch"));
            sp80 = var_s0 + 0x2A8;
            *(W90 *)var_s1 = sp90;
            if ((var_s0 != (void *)-0x2A8) || (temp_v0_5 = FCALL(mem_new)(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                FCALL(Parms_addParm)(sp80, var_s0, *(W90 *)var_s1, 1);
                PARMF(sp80)->type = &D_00008018_s38 + 0 * (int)(arg2); /* agent-c run 212: folded counted use (zero code) */
                PARMF(sp80)->unk14 = 0;
                PARMF(sp80)->value = 1.0f;
            }
        }
        var_s0 = arg0 + 0x808; if (1) {}
        if ((arg0 != (void *)-0x808) || (var_s0 = FCALL(mem_new)(0xC), (var_s0 != 0))) {
            *(s32 *)var_s0 = 0;
        }
        temp_s1 = arg0 + 0x878;
        var_s0 = temp_s1;
        if ((arg0 != (void *)-0x878) || (temp_v0_5 = FCALL(mem_new)(0x40), var_s0 = temp_v0_5, (temp_v0_5 != 0))) {
            FCALL(coll_boxCtor_0003CAA0)(var_s0);
            COLLOBJ(var_s0)->vptr = (char *)&bu_90cc_imp30;
        }
        SNOWB(arg0)->rival = 0;
        SNOWB(arg0)->controller = arg1;
        SNOWB(arg0)->character = arg2;
        fq = (s32 *)arg0;
        fq += 6;
        if (1) { *fq &= ~2; }
        SNOWB(arg0)->camera = 0;
        /* target 0xA330-0xA388: beq-chain to out-of-line cases (default inline first,
         * case 7, then case 6 falling through) with per-case var_t1 = arg8|1 -- switch shape.
         * (goto-layout variant re-normalized by IDO and scored worse; keep switch) */
        if (arg2 == 7) goto sw_c7;
        if (arg2 == 6) goto sw_c6;
        SNOWB(arg0)->unk900 = 4;
        SNOWB(arg0)->unk902 = 7;
        
        goto sw_end;
sw_c7:
        if (1) {}
        SNOWB(arg0)->unk900 = 0x19; SNOWB(arg0)->unk902 = 0x1A;
        arg8 |= 1;
        goto sw_end;
sw_c6:
        if (1) {}
        SNOWB(arg0)->unk900 = 0x17; SNOWB(arg0)->unk902 = 0x18;
        arg8 |= 1;
sw_end:
        sp174 = 0 + 0 * (int)(arg8); /* agent-c run 212: folded counted use (zero code) */
        if ((arg1 == 1) && (*(s32 *)((char *)&XDATA(D_00000000, port_bootup_bss, 0xF8) + 0x58) == *(s32 *)((char *)&XDATA(D_00000000, port_bootup_bss, 0xF8) + 0x4C))) {
            sp174 = 1;
        }
        if ((*(s32 *)((char *)&XDATA(D_00000000, port_bootup_bss, 0xF8) + 0x34) == 3) && (*(u8 *)((char *)&XDATA(D_00000000, port_bootup_bss, 0xF8) + 0x17D) == *(s32 *)((char *)&XDATA(D_00000000, port_bootup_bss, 0xF8) + 0x4C)) && (arg1 == 2)) {
            sp174 = 1 + 0 * (int)(sp174); /* agent-c run 212: folded counted use (zero code) */
        }
        if ((arg2 == 7) || (arg2 == 6)) {
            if (arg7 & 8) {
                sp174 = 1;
            }
            if (arg2 == 7) {
                if (sp174 != 0) {
                    inv = 255.0f;
                    prgb = bu_90cc_rgb28;
                    cr = prgb[0];
                    cg = prgb[1];
                    cb = prgb[2];
                    SNOWB(arg0)->tint[0] = (f32) cr / inv;
                    SNOWB(arg0)->tint[1] = (f32) cg / inv;
                    SNOWB(arg0)->tint[2] = (f32) cb / inv;
                    SNOWB(arg0)->tint[3] = (f32)255 / inv;
                } else {
                    {
                    inv = 255.0f;
                    prgb = bu_90cc_rgb24;
                    cr = prgb[0];
                    cg = prgb[1];
                    cb = prgb[2];
                    SNOWB(arg0)->tint[0] = (f32) cr / inv;
                    SNOWB(arg0)->tint[1] = (f32) cg / inv;
                    SNOWB(arg0)->tint[2] = (f32) cb / inv;
                    SNOWB(arg0)->tint[3] = (f32)255 / inv;
                    }
                }
            }
            if (arg2 == 6) {
                if (sp174 != 0) {
                    inv = 255.0f;
                    prgb = bu_90cc_rgb30;
                    cr = prgb[0];
                    cg = prgb[1];
                    cb = prgb[2];
                    SNOWB(arg0)->tint[0] = (f32) cr / inv;
                    SNOWB(arg0)->tint[1] = (f32) cg / inv;
                    SNOWB(arg0)->tint[2] = (f32) cb / inv;
                    SNOWB(arg0)->tint[3] = 164.0f / inv;
                } else {
                    inv = 255.0f;
                    prgb = bu_90cc_rgb2C;
                    cr = prgb[0];
                    cg = prgb[1];
                    cb = prgb[2];
                    SNOWB(arg0)->tint[0] = (f32) cr / inv;
                    SNOWB(arg0)->tint[1] = (f32) cg / inv;
                    SNOWB(arg0)->tint[2] = (f32) cb / inv;
                    SNOWB(arg0)->tint[3] = 164.0f / inv;
                }
                if (1) {}
            }
            arg8 &= ~4;
        } else {
            if (arg7 & 8) {
                inv = 255.0f;
                temp_f0 = (f32)255 / inv;
                SNOWB(arg0)->tint[0] = temp_f0;
                SNOWB(arg0)->tint[1] = temp_f0;
                SNOWB(arg0)->tint[2] = temp_f0;
                SNOWB(arg0)->tint[3] = 117.0f / inv;
            } else {
                inv = 255.0f;
                temp_f0 = (f32)255 / inv;
                SNOWB(arg0)->tint[0] = temp_f0;
                SNOWB(arg0)->tint[1] = temp_f0;
                SNOWB(arg0)->tint[2] = temp_f0;
                SNOWB(arg0)->tint[3] = 70.0f / inv;
                if (*(s32 *)((char *)&XDATA(D_00000000, port_bootup_bss, 0xF8) + 0x34) != 2) {
                    arg7 |= 0x1000;
                }
            }
        }
        temp_v0_51 = arg7 | 0x4800;
        SNOWB(arg0)->initFlags = temp_v0_51;
        /* run 179: this store inside if (1) { } (a BB boundary before the 0x8B8 store)
         * stops as1 copying the `arg8 &= ~4` join into the rgb30 arm (length 8436 ->
         * 8424 = target) and gives arg2 the target t3 */
        if (1) { SNOWB(arg0)->flags = temp_v0_51; }
        SNOWB(arg0)->unk8B8 = arg8 + 0 * (int)(arg7); /* agent-c run 212: folded counted use (zero code) */
        bu_90cc_call_p4f(temp_s1, arg0, 0x6D, PORT64_F(0x43160000), 200.0f);
        if (SNOWB(arg0)->flags & 8) {
            SNOWB(arg0)->collHitMask = 4;
            SNOWB(arg0)->collCategory = 2;
        } else {
            SNOWB(arg0)->collCategory = 1; SNOWB(arg0)->collHitMask = 5; /* agent-c run 221: 0x88E first on the line = the target's 1 / 5 constant order */
        }
        FCALL(CollObjMgr_addDynamic)(*(s32 *)((char *)&XDATA(D_00000000, port_bootup_bss, 0x10) + 0), temp_s1);
        vpos.x = arg5;
        vpos.y = 0.0f;
        goto bu_90cc_lz;
    bu_90cc_lc: bu_90cc_call_iipif(*(s32 *)((char *)&XDATA(D_00000000, port_bootup_bss, 0x50) + 0), 1, &vpos.y, arg5, arg6); goto bu_90cc_le;
    bu_90cc_lz: if (1) {} vpos.z = arg6; goto bu_90cc_lc;
    bu_90cc_le: ;
        vpos.y += kRo_SnowBoarder_ctor_8B0;
        wpos = vpos;
        SNOWB(arg0)->dnode.unk3A4[0] = wpos.x;
        SNOWB(arg0)->dnode.unk3A4[1] = wpos.y;
        SNOWB(arg0)->dnode.unk3A4[2] = wpos.z;
        SNOWB(arg0)->unk8C0 = (s32) (1 << arg2);
        SNOWB(arg0)->dnode.unk38C = 1;
        SNOWB(arg0)->trails = 0;
        SNOWB(arg0)->dispTrails = 0;
        SNOWB(arg0)->dispSpray = 0;
        FCALL(snd_soundObjCreate_0000057C)(arg0 + 0x808, arg1, arg2);
        if (SNOWB(arg0)->flags & 3) {
            FCALL(memtrack_begin)(bu_90cc_d87A4 + 0x5C, 0);
            SNOWB(arg0)->dispSpray = FCALL(Node_ctor)(0, bu_90cc_d87A4 + 0x64);
            SNOWB(arg0)->dispTrails = FCALL(Node_ctor)(0, &sStr_DispTrails_8814);
            var_s0 = SNOWB(arg0)->dispSpray;
            var_s0 += 0x18;
            *(s32 *)var_s0 &= ~8;
            var_s1 = SNOWB(arg0)->dispSpray;
            var_s0 = (char *)((WorldRegistry *)FLOW_90CC->game)->egRoot;
            FCALL(list_append)(var_s0 + 0x10, var_s1);
            if (NODE(var_s1)->parent != 0) {
                NODE(var_s1)->unk04 = 1;
            }
            NODE(var_s1)->parent = (Node *) var_s0;
            temp_v0_5 = FCALL(Node_ctor)(0, &sStr_DispTrails_8820);
            SNOWB(arg0)->dispTrails = temp_v0_5;
            fq = (s32 *)temp_v0_5;
            if (1) { fq += 6; }
            *fq &= ~8;
            var_s1 = SNOWB(arg0)->dispTrails;
            var_s0 = (char *)((WorldRegistry *)FLOW_90CC->game)->bgRoot;
            FCALL(list_append)(var_s0 + 0x10, var_s1);
            if (NODE(var_s1)->parent != 0) {
                NODE(var_s1)->unk04 = 1;
            }
            NODE(var_s1)->parent = (Node *) var_s0;
            FCALL(Node_setNewNodeMask)(1);
            var_s0 = FCALL(mem_new)(0x23C);
            if (var_s0 != 0) {
                var_s1 = var_s0;
                if ((var_s1 != 0) || (var_s1 = FCALL(mem_new)(0x174), (var_s1 != 0))) {
                    FCALL(Node_ctor)(var_s1, &sStr_Node_882C);
                    NODE(var_s1)->vptr = (char *)&bu_90cc_vt545;
                }
                NODE(var_s0)->vptr = (char *)&bu_90cc_vt546;
            }
            sp164 = var_s0;
            var_s1 = SNOWB(arg0)->dispTrails;
            FCALL(list_append)(var_s1 + 0x10, var_s0);
            if (NODE(var_s0)->parent != 0) {
                NODE(var_s0)->unk04 = 1;
            }
            NODE(var_s0)->parent = (Node *) var_s1;
            if (SNOWB(arg0)->flags & 1) {
                var_s1 = FCALL(mem_new)(0X1F4);
                if (var_s1 != 0) {
                    var_a2 = (s32) var_s1;
                    if ((var_a2 != 0) || (var_a2 = (s32)FCALL(mem_new)(0x174), (var_a2 != 0))) {
                        FCALL(Node_ctor)((char *)var_a2, &sStr_Node_8834);
                        NODE(var_a2)->vptr = (char *)&bu_90cc_vt545_b;
                    }
                    NODE(var_s1)->vptr = (char *)&bu_90cc_vt547;
                }
                sp160 = var_s1;
                var_s0 = var_s1;
                var_s1 = SNOWB(arg0)->dispSpray;
                FCALL(list_append)(var_s1 + 0x10, var_s0);
                if (NODE(var_s0)->parent != 0) {
                    NODE(var_s0)->unk04 = 1;
                }
                NODE(var_s0)->parent = (Node *) var_s1;
            }
            var_s1 = FCALL(mem_new)(0X174);
            if (var_s1 != 0) {
                var_a2 = (s32) var_s1;
                if ((var_a2 != 0) || (var_a2 = (s32)FCALL(mem_new)(0x174), (var_a2 != 0))) {
                    FCALL(Node_ctor)((char *)var_a2, &sStr_Node_883C);
                    NODE(var_a2)->vptr = (char *)&bu_90cc_vt545_c;
                }
                NODE(var_s1)->vptr = (char *)&bu_90cc_vt548;
            }
            sp15C = var_s1;
            var_s0 = var_s1;
            var_s1 = SNOWB(arg0)->dispTrails;
            FCALL(list_append)(var_s1 + 0x10, var_s0);
            if (NODE(var_s0)->parent != 0) {
                NODE(var_s0)->unk04 = 1;
            }
            NODE(var_s0)->parent = (Node *) var_s1;
            var_s1 = FCALL(mem_new)(0X174);
            if (var_s1 != 0) {
                var_a2 = (s32) var_s1;
                if ((var_a2 != 0) || (var_a2 = (s32)FCALL(mem_new)(0x174), (var_a2 != 0))) {
                    FCALL(Node_ctor)((char *)var_a2, &sStr_Node_8844);
                    NODE(var_a2)->vptr = (char *)&bu_90cc_vt545_d;
                }
                NODE(var_s1)->vptr = (char *)&bu_90cc_vt548_b;
            }
            sp158 = var_s1;
            var_s0 = var_s1;
            var_s1 = SNOWB(arg0)->dispTrails;
            FCALL(list_append)(var_s1 + 0x10, var_s0);
            if (NODE(var_s0)->parent != 0) {
                NODE(var_s0)->unk04 = 1;
            }
            NODE(var_s0)->parent = (Node *) var_s1;
            FCALL(Node_setNewNodeMask)(0xFFFF);
            FCALL(memtrack_end)();
        }
        FCALL(memtrack_begin)(&sStr_AnimationData_884C, 0);
        var_s1 = FCALL(mem_new)(0x16C);
        if (var_s1 != 0) {
#ifdef PORT64
            bu_90cc_call_p4d(var_s1, &sStr_lighttarget_885C, 0.0f, 0.0f, 0.0f);
#else
            bu_90cc_call_p4d(var_s1, &sStr_lighttarget_885C, 0.0, 0.0f);
#endif
            temp_f0 = kRo_SnowBoarder_ctor_8B4;
            NODE(var_s1)->vptr = (char *)&LightTarget_vtbl_90cc_d207dc; ((LightTarget *)var_s1)->mask = 0xFFFF;
            ((LightTarget *)var_s1)->unk108[0] = temp_f0;
            ((LightTarget *)var_s1)->unk108[1] = temp_f0;
            ((LightTarget *)var_s1)->unk108[2] = temp_f0;
            ((LightTarget *)var_s1)->scale = 1.0f;
        }
        SNOWB(arg0)->light = var_s1;
        FCALL(MNode_addChild)(arg0, var_s1);
        var_s0 = SNOWB(arg0)->light;
        temp_f0 = 0;
        var_s0 += 0xB4;
        ((SnowBVec3 *)var_s0)->x = temp_f0;
        ((SnowBVec3 *)var_s0)->z = temp_f0;
        ((SnowBVec3 *)var_s0)->y = 100.0f;
        SNOWB(arg0)->anim = (struct RiderAnim *) FCALL(RiderAnim_ctor_0000C48C)(0, *(s32 *)((char *)&XDATA(D_00000000, port_bootup_bss, 0xD8) + 0), arg0);
        FCALL(memtrack_end)();
        SNOWB(arg0)->input = (RiderInput *) arg4;
        var_v1 = ((s32) SNOWB(arg0)->anim); FCALL(MNode_addChild)(arg0, var_v1);
        var_s0 = ((char *) SNOWB(arg0)->anim);
        var_s0 += 0x18;
        *(s32 *)var_s0 &= ~8;
        /* target 0xAC34-0xAC98: each global access is its own symbol (fresh lui per
         * site, reload between the |= and &= statements) -- per-site blank aliases
         * bust the m2c &D_00000000 base GCSE that had cached one addiu base reg. */
        *(s32 *)&gPolyFlags_3EEC4_90cc_g8_st = *(s32 *)&gPolyFlags_3EEC4_90cc_g8_ld | 8;
        /* agent-c run 221 (kit CCXXI): the two RMW chains on the dummy-home var_t1 (not the shared var_v1 name)
         * inside do { } while (0) blocks get v0 as the target */
        do { var_t1 = gPolyGeomMode_3EEC8_90cc_gcfg_ld.f04; var_t1 |= 0x20000; gPolyGeomMode_3EEC8_90cc_gcfg_st.f04 = var_t1; } while (0);
        do { var_t1 = gPolyGeomMode_3EEC8_90cc_gcfg_b_ld.f04; var_t1 &= 0xFFF7FFFF; var_t1 &= ~2; gPolyGeomMode_3EEC8_90cc_gcfg_b_st.f04 = var_t1; } while (0);
        bu_90cc_gown = (s32) arg0;
        for (sp58 = 0; sp58 != 0x23; sp58++) {
            var_a2 = sp58;
            if (sp58 >= 0x28U) {
                FCALL(sys_fatal)(&sFmt_Global_out_of_range_8868, sp58);
            }
            *(s32 *)((char *)&gNodeGlobals_90cc_gtbl + var_a2 * 4) = 0;
        }
        gPolyGeomMode_3EEC8_90cc_gpair.geomMode = 0x2A003;
        gPolyGeomMode_3EEC8_90cc_gpair.mode0 = 0;
        SNOWB(arg0)->unk8F4[0] = (SnowBRec3 *) FCALL(gfx_addDdm)(&gGfxLib_90cc_gdev, 1, 0);
        SNOWB(arg0)->unk8F4[1] = (SnowBRec3 *) FCALL(gfx_addDdm)(&gGfxLib_90cc_gdev, 2, 0);
        SNOWB(arg0)->unk8F4[2] = (SnowBRec3 *) FCALL(gfx_addDdm)(&gGfxLib_90cc_gdev, 3, 0);
        if (sp174 != 0) {
            temp_v0_5 = (char *)bu_90cc_fz2; /* target lui v1 / addiu v0,v1,0 (blank) then lw 0(v0): fn-typed base */
            if (*(s32 *)temp_v0_5 != 0) {
                temp_s1_8 = (char *)(gGfxLib_90cc_d148_01 + 24);
                cg = *(s32 *)(char *)bu_90cc_d00_b; do { /* run 227: cg's load on the `do` line = as1 issues it after the d148 address */
                    if (arg2 == cg) {
                        /* (temp_v0_5->unk4 * 4)->unk160 store: scaled-index table write */
                        var_s0 = temp_s1_8; var_s0 += ((RiderDdmRow_90cc *)temp_v0_5)->slot * 4 + 0 * (int)(arg2);
                        *(s32 *)var_s0 = (s32)(temp_v0_5 + 8);
                    }
                    cg = ((RiderDdmRow_90cc *)temp_v0_5)[1].rider;
                    temp_v0_5 += 0x18;
                } while (cg != 0);
            }
        } else {
            { s32 i; for (i = 0; i < 3; i++) ((s32 *)gGfxLib_90cc_d148_18)[i] = 0; }
        }
        if (SNOWB(arg0)->flags & 8) {
            FCALL(SnowBoarder_setDefaultDdms)(arg0);
        } else {
            FCALL(SnowBoarder_setRiderDdms)(arg0);
        }
        SNOWB(arg0)->board = (s16) arg3;
        { s32 v840 = ((s32) SNOWB(arg0)->anim); FCALL(SnowBoarder_loadBoardModel_0000B1B4)(arg2, arg3, SNOWB(arg0)->unk8B8, v840, arg0); }
        FCALL(SnowBoarder_createColliders_0000CCE0)(arg0, SNOWB(arg0)->flags & 1);
        if (SNOWB(arg0)->flags & 3) {
            FCALL(memtrack_begin)(&sStr_Trails_8880, 0);
            temp_v0_5 = FCALL(Node_ctor)(0, &sStr_Trails_8888);
            SNOWB(arg0)->trails = temp_v0_5;
            fq = (s32 *)temp_v0_5;
            if (1) { fq += 6; }
            *fq &= ~4;
            FCALL(MNode_addChild)(arg0, ((s32) SNOWB(arg0)->trails));
            var_s1 = SNOWB(arg0)->trails;
            FCALL(list_append)(var_s1 + 0x10, sp164); if (1) {}
            if (NODE(sp164)->parent != 0) {
                NODE(sp164)->unk04 = 1;
            }
            NODE(sp164)->parent = (Node *) var_s1;
            if (SNOWB(arg0)->flags & 1) {
                var_s1 = SNOWB(arg0)->trails;
                FCALL(list_append)(var_s1 + 0x10, sp160);
                if (NODE(sp160)->parent != 0) {
                    NODE(sp160)->unk04 = 1;
                }
                NODE(sp160)->parent = (Node *) var_s1;
            }
            var_s1 = SNOWB(arg0)->trails;
            FCALL(list_append)(var_s1 + 0x10, sp15C); if (1) {}
            if (NODE(sp15C)->parent != 0) {
                NODE(sp15C)->unk04 = 1;
            }
            NODE(sp15C)->parent = (Node *) var_s1;
            var_s1 = SNOWB(arg0)->trails;
            FCALL(list_append)(var_s1 + 0x10, sp158); if (1) {}
            if (NODE(sp158)->parent != 0) {
                NODE(sp158)->unk04 = 1;
            }
            NODE(sp158)->parent = (Node *) var_s1;
            FCALL(Node_setNewNodeMask)(1); if (1) {}
            /* a call does not end a uopt basic block: without the split, the sp158 temp's v0 live range
             * covers this block and the site-1 value cannot take v0. Each draw-argument global goes
             * through var_s0 (overwritten next) for the target's `lui s0, %hi(g); lw v0, %lo(g)(s0)`. */
            var_s0 = (char *)gNodeGlobals_90cc_d44_01.f28;
            var_s0_3 = (s32 *)var_s0;
            var_s0 = ((char *) NODE(sp164)->vptr);
            temp_s1 = (char *)((s32)var_s0_3 + 0x70);
            ((Vfn_ff)VT_SLOT(var_s0, 10).fn)(VT_SLOT(var_s0, 10).delta + sp164, ((s32) SNOWB(arg0)->trails), ((s32) SNOWB(arg0)->dispSpray), arg0, 0x10, (s32)temp_s1, *(f32 *)(kRo_SnowBoarder_ctor_8B8 + kRoF_SnowBoarder_ctor_8B8), 100.0f);
            if (SNOWB(arg0)->flags & 1) {
                var_s0 = ((char *) NODE(sp160)->vptr); 
                ((Vfn_ff)VT_SLOT(var_s0, 10).fn)(VT_SLOT(var_s0, 10).delta + sp160, ((s32) SNOWB(arg0)->trails), ((s32) SNOWB(arg0)->dispSpray), arg0, 0xC, (s32)temp_s1, *(f32 *)(kRo_SnowBoarder_ctor_8BC + kRoF_SnowBoarder_ctor_8BC), 100.0f);
                FCALL(BoardTrail_createPowder_00006144)(sp164); /* jal T+0xAFE0 -> import mod60 sym120 = snoweffects.uso BoardTrail_createPowder_00006144 (TextReloc import, proven by name-placeholder-calls) */
                FCALL(BoardTrail_createSpec_00006248)(sp164); /* jal T+0xAFE8 -> import mod60 sym122 = snoweffects.uso BoardTrail_createSpec_00006248 (TextReloc import, proven by name-placeholder-calls) */
            }
            var_s0 = (char *)gNodeGlobals_90cc_d08_01.f2C;
            temp_v0_51 = (s32)var_s0;
            var_s0 = ((char *) NODE(sp15C)->vptr);
            ((Vfn_ff)VT_SLOT(var_s0, 10).fn)(VT_SLOT(var_s0, 10).delta + sp15C, ((s32) SNOWB(arg0)->trails), ((s32) SNOWB(arg0)->dispSpray), arg0, 8, temp_v0_51 + 0x70, 32.0f, 74.0f);
            var_s0 = (char *)gNodeGlobals_90cc_d44_02.f04;
            var_t1 = (s32)var_s0;
            var_s0 = ((char *) NODE(sp158)->vptr);
            ((Vfn_ff)VT_SLOT(var_s0, 10).fn)(VT_SLOT(var_s0, 10).delta + sp158, ((s32) SNOWB(arg0)->trails), ((s32) SNOWB(arg0)->dispSpray), arg0, 8, (s32)var_t1 + 0x70, 32.0f, 74.0f);
            FCALL(Node_setNewNodeMask)(0xFFFF);
            FCALL(memtrack_end)();
        }
        SNOWB(arg0)->unk814 = (char *)((BuW90 *)gSkinMgrs_41700_90cc_fz)->f00;
        SNOWB(arg0)->unk818 = (char *)((BuW90 *)gSkinMgrs_41700_90cc_fz)->f08;
        SNOWB(arg0)->unk81C = (char *)((BuW90 *)gSkinMgrs_41700_90cc_fz)->f04;
        var_s0 = (char *)gNodeGlobals_90cc_d44_03.f28;
        SNOWB(arg0)->bodyNodes[1] = var_s0;
        var_s0 = (char *)gNodeGlobals_90cc_d80_01.f08;
        SNOWB(arg0)->bodyNodes[0] = var_s0;
        var_s1 = (char *)gNodeGlobals_90cc_d08_02.f00;
        SNOWB(arg0)->bodyNodes[2] = var_s1;
        var_s1 = (char *)gNodeGlobals_90cc_d08_03.f14;
        SNOWB(arg0)->bodyNodes[4] = var_s1;
        var_s1 = (char *)((BuW90 *)&gNodeGlobals)->f04;
        SNOWB(arg0)->bodyNodes[3] = var_s1;
        var_s1 = (char *)gNodeGlobals_90cc_d08_04.f10;
        SNOWB(arg0)->bodyNodes[5] = var_s1;
        var_s1 = (char *)gNodeGlobals_90cc_d44_04.f0C;
        SNOWB(arg0)->bodyNodes[6] = var_s1;
        var_s0 = (char *)gNodeGlobals_90cc_d44_05.f2C;
        SNOWB(arg0)->top = var_s0;
        var_s0 = (char *)gNodeGlobals_90cc_d80_02.f04;
        SNOWB(arg0)->unk860 = var_s0;
        SNOWB(arg0)->unk854 = (char *) gNodeGlobals_90cc_d44_06.f30;
        var_s0 = (char *)gNodeGlobals_90cc_d44_07.f34;
        SNOWB(arg0)->bodySkin = var_s0;
        var_s0 = (char *)gNodeGlobals_90cc_d44_08.f38;
        SNOWB(arg0)->headShape = var_s0;
        var_s0 = (char *)gNodeGlobals_90cc_d80_03.f00;
        SNOWB(arg0)->boardNode = var_s0;
        var_s0 = (char *)gNodeGlobals_90cc_d44_09.f14;
        SNOWB(arg0)->camSource = var_s0;

        SNOWB(arg0)->ai = (struct SnowAI *) FCALL(SnowAI_ctor)(0, arg0, gInputMgr_90cc_d44_10.f24);
        FCALL(SnowBoarder_reset)(arg0);
    }
    return arg0;
}
#ifdef PORT
#undef gGfxLib_90cc_d148_01
#undef gGfxLib_90cc_d148_18
#undef gNodeGlobals_90cc_d80_01
#undef gNodeGlobals_90cc_gtbl
#undef gPolyFlags_3EEC4_90cc_g8_ld
#undef gPolyFlags_3EEC4_90cc_g8_st
#undef gPolyGeomMode_3EEC8_90cc_gcfg_b_ld
#undef gPolyGeomMode_3EEC8_90cc_gcfg_b_st
#undef gPolyGeomMode_3EEC8_90cc_gcfg_ld
#undef gPolyGeomMode_3EEC8_90cc_gcfg_st
#undef gPolyGeomMode_3EEC8_90cc_gpair
#endif
