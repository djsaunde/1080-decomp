/* IDO 7.1 NCC (C++) NM-only comparison donor for func_000090CC (agent-g run 176).
 * Port of the cfe NM body (97.15) to C++: 98.50, 2106 words = target. edg-specific fixes
 * (docs/IDO_CODEGEN.md kit CXXIII): unreferenced fillers kept with `= 0`, the 3-word zero
 * loop as an index loop (edg's pointer loop unrolls), literals through fi(int), arg8 is the
 * flag candidate itself, temp_s1 carries the late +0x70 draw argument (split m2c variable),
 * per-site vt548 alias, fn-typed second D_0 base, cfg RMW through a local.
 * RESIDUAL: t-ring one behind the target after the 0x808 cascade, case-7 constant hoist,
 * `lui s0; lw v0` draw-argument globals, dead `addiu s0, s0, 0xB4`, arg load order at the
 * 5-arg call, &D_8880 delay slot, wpos.y store schedule.
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
typedef double f64;
typedef struct { float x, y, z; } Vec3;
extern "C" {
extern char D_00000000;
extern char *func_00000000(...);
extern f32 D_00000868, D_0000086C, D_00000870, D_00000874, D_00000878, D_0000087C, D_00000880, D_00000884, D_00000888, D_0000088C, D_00000890, D_00000894, D_00000898, D_0000089C, D_000008A0, D_000008A4, D_000008A8, D_000008AC, D_000008B0, D_000008B4;
extern char D_00008730;
extern char D_00008738;
extern char D_00008740;
extern char D_00008018;
/* per-site aliases of D_00008018: bust cross-cascade addr GCSE (target rematerializes lui/addiu per +0xC store, post-jal temp not pre-jal s-reg) */
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
extern char D_00008060;
extern char D_00008060_s2;
extern char D_00008060_s3;
extern char D_00008108;
extern char D_00008800;
extern char D_00008808;
extern char D_00008814;
extern char D_00008820;
extern char D_0000882C;
extern char D_00008834;
extern char D_0000883C;
extern char D_00008844;
extern char D_0000884C;
extern char D_0000885C;
extern char D_00008868;
extern char D_00008880;
extern char D_00008888;
/* func_000090CC alloc-cascade +0x28 child-vtable pointers: USO relocs blanked in the
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
extern char bu_90cc_d207dc;
extern char bu_90cc_imp30;  /* +0x30 import ptr in the 0x878 cascade (target 0xA2F8 lui/addiu/sw pair; was decoded as literal 0) */
/* target 0xAC34-0xAD3C per-site blank globals (each site = own symbol; busts base GCSE).
 * Scalar/array form (not cast-arith) so the lo16 folds into the load offset (lui rd; lw rd,4(rd))
 * and stores go through $at -- matches target's direct-global access shape. */
extern char bu_90cc_g8_ld, bu_90cc_g8_st;      /* load/store alias pair: same address, distinct syms kill the addr CSE (lui rd/lw + lui at/sw shape) */


extern s32 bu_90cc_gown;
extern char bu_90cc_gtbl;
extern char bu_90cc_gpair;
extern char bu_90cc_gdev;
extern char bu_90cc_vt_self;  /* self +0x28 vtable (was decoded as literal 0; expected has lui/addiu reloc pair) */
/* vtable draw slot (+0x5C): trailing two args are single-precision f32.
 * Prototyping defeats the K&R float->double promotion so single swc1
 * arg stores emit (matches target; no cvt.d.s/sdc1). */
typedef void (*Vfn_ff)(char *, s32, s32, void *, s32, s32, f32, f32);
typedef struct { s32 w; } W90;
extern char D_00008778;
extern void func_00000000_p4f(void *, char *, s32, s32, f32);
extern void func_00000000_iipif(s32, s32, void *, f32, f32);
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
 * symbols (target lui/addiu %hi/%lo(func_00008664+0x24..0x30) + lbu 0/1/2).
 * Every sp-relative word now lands on the target slot. Residual: the arg2
 * 7/6 dispatch order (target beq 7 first; switch / if-goto / split-switch
 * spellings all scored lower), arg1 a3-vs-t4, the u32->f32 `lui at,0x4f80`
 * schedule, vt548 hoist, and the 0x1BE0..0x1F94 call-tail scheduling. */
/* data aliases of splat function-anchored bases (target relocs: func_00008664+40.., func_000000F0+0x44, func_00000044/80/08+K, func_00000148+24, func_000008B4+4/8, func_000087A4+0x5C/0x64).
 * A fn-typed symbol never folds its addend into %lo (separate addiu) and cfe rejects a data redeclaration; a data alias folds. */
extern u8 bu_90cc_rgb28[], bu_90cc_rgb24[], bu_90cc_rgb30[], bu_90cc_rgb2C[];
extern u8 bu_90cc_d8664[], bu_90cc_d87A4[], bu_90cc_dF0[], bu_90cc_d8B4_01[], bu_90cc_d8B4_02[];
/* per-site aliases (one symbol per access) bust the shared-base GCSE that turns lui/%lo-folded loads into base+K */
extern u8 bu_90cc_d148_01[], bu_90cc_d148_02[], bu_90cc_d148_03[];
extern u8 bu_90cc_d148_18[];
extern void bu_90cc_d00_b(...);  /* second read of D_00000000.0 in the sp174 list walk: distinct symbol keeps the target's reload (else CSE'd into a copy) */
typedef struct { s32 f00, f04, f08, f0C, f10, f14, f18, f1C, f20, f24, f28, f2C, f30, f34, f38; } BuW90;
extern BuW90 bu_90cc_gcfg_ld, bu_90cc_gcfg_st, bu_90cc_gcfg_b_ld, bu_90cc_gcfg_b_st;
/* scalar-field form: a named global field read is a uopt candidate (target lui/lw temps land in s0/s1/a2, not the t-ring) */
extern BuW90 bu_90cc_d44_01, bu_90cc_d44_02, bu_90cc_d44_03, bu_90cc_d44_04, bu_90cc_d44_05, bu_90cc_d44_06, bu_90cc_d44_07, bu_90cc_d44_08, bu_90cc_d44_09, bu_90cc_d44_10, bu_90cc_d08_01, bu_90cc_d08_02, bu_90cc_d08_03, bu_90cc_d08_04, bu_90cc_d80_01, bu_90cc_d80_02, bu_90cc_d80_03;
extern void func_00000000_p4d(void *, char *, f64, f32);
extern void bu_90cc_fz(...);  /* 0x814/818/81C source base: target lui v0/addiu v0 (no reloc) then lw 0/8/4(v0) -- a fn-typed symbol keeps the %lo in an addiu so the base is CSEd in one register */
extern void bu_90cc_fz2(...);  /* sp174 list-walk head (same blank address), fn-typed for the same reason */
}
static inline f32 fi(int v) { return v; }
extern "C" void *func_000090CC(char *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5, f32 arg6, s32 arg7, s32 arg8) {
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

    if ((arg0 != 0) || (temp_v0_5 = func_00000000(0xA8C), arg0 = temp_v0_5, (temp_v0_5 != 0))) {
        func_00000000(arg0, &D_00008730);
        *(s32 *)((char *)(arg0) + 0x28) = (s32) &bu_90cc_vt_self;
        var_s0 = arg0 + 0x3EC;
        if (((arg0 != (void *)-0x3EC) || ((var_s0 = func_00000000(0x154)) != 0)) && ((func_00000000(var_s0, &D_00008738), *(s32 *)((char *)(var_s0) + 0x28) = (s32) &D_00008108, var_s1 = var_s0 + 0x2C, (var_s0 != (void *)-0x2C)) || (temp_v0_5 = func_00000000(0x128), var_s1 = temp_v0_5, (temp_v0_5 != 0)))) {
            var_a2 = (s32)var_s1;
            if ((var_s1 != 0) || (temp_v0_5 = func_00000000(8), var_a2 = (s32)temp_v0_5, (temp_v0_5 != 0))) {
                *(s32 *)((char *)(var_a2) + 0x0) = (s32) &D_00008740;
                *(s32 *)((char *)(var_a2) + 0x4) = 0;
            }
            sp144.w = *(s32 *)((char *)&D_00000000 + 0x8748);
            sp108 = var_s1 + 8;
            sp104 = sp144;
            if ((var_s1 != (void *)-8) || (temp_v0_5 = func_00000000(0x18), sp108 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp108, var_s1, sp104, 1);
                *(s32 *)((char *)(sp108) + 0xC) = (s32) &D_00008018;
                *(s32 *)((char *)(sp108) + 0x14) = 0;
                *(f32 *)((char *)(sp108) + 0x10) = (f32) D_00000868;
            }
            sp140.w = *(s32 *)((char *)&D_00000000 + 0x874C);
            sp108 = var_s1 + 0x20;
            sp104 = sp140;
            if ((var_s1 != (void *)-0x20) || (temp_v0_5 = func_00000000(0x18), sp108 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp108, var_s1, sp104, 1);
                *(s32 *)((char *)(sp108) + 0xC) = (s32) &D_00008018_s02;
                *(s32 *)((char *)(sp108) + 0x14) = 0;
                *(f32 *)((char *)(sp108) + 0x10) = (f32) D_0000086C;
            }
            sp13C.w = *(s32 *)((char *)&D_00000000 + 0x8750);
            sp108 = var_s1 + 0x38;
            sp104 = sp13C;
            if ((var_s1 != (void *)-0x38) || (temp_v0_5 = func_00000000(0x18), sp108 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp108, var_s1, sp104, 1);
                *(s32 *)((char *)(sp108) + 0xC) = (s32) &D_00008018_s03;
                *(s32 *)((char *)(sp108) + 0x14) = 0;
                *(f32 *)((char *)(sp108) + 0x10) = 2.0f;
            }
            sp138.w = *(s32 *)((char *)&D_00000000 + 0x8754);
            sp108 = var_s1 + 0x50;
            sp104 = sp138;
            if ((var_s1 != (void *)-0x50) || (temp_v0_5 = func_00000000(0x18), sp108 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp108, var_s1, sp104, 1);
                *(s32 *)((char *)(sp108) + 0xC) = (s32) &D_00008018_s04;
                *(s32 *)((char *)(sp108) + 0x14) = 0;
                *(f32 *)((char *)(sp108) + 0x10) = (f32) D_00000870;
            }
            sp134.w = *(s32 *)((char *)&D_00000000 + 0x8758);
            sp108 = var_s1 + 0x68;
            sp104 = sp134;
            if ((var_s1 != (void *)-0x68) || (temp_v0_5 = func_00000000(0x18), sp108 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp108, var_s1, sp104, 1);
                *(s32 *)((char *)(sp108) + 0xC) = (s32) &D_00008018_s05;
                *(s32 *)((char *)(sp108) + 0x14) = 0;
                *(f32 *)((char *)(sp108) + 0x10) = (f32) D_00000874;
            }
            sp130.w = *(s32 *)((char *)&D_00000000 + 0x875C);
            sp108 = var_s1 + 0x80;
            sp104 = sp130;
            if ((var_s1 != (void *)-0x80) || (temp_v0_5 = func_00000000(0x18), sp108 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp108, var_s1, sp104, 1);
                *(s32 *)((char *)(sp108) + 0xC) = (s32) &D_00008018_s06;
                *(s32 *)((char *)(sp108) + 0x14) = 0;
                *(f32 *)((char *)(sp108) + 0x10) = (f32) D_00000878;
            }
            sp12C.w = *(s32 *)((char *)&D_00000000 + 0x8760);
            sp108 = var_s1 + 0x98;
            sp104 = sp12C;
            if ((var_s1 != (void *)-0x98) || (temp_v0_5 = func_00000000(0x18), sp108 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp108, var_s1, sp104, 1);
                *(s32 *)((char *)(sp108) + 0xC) = (s32) &D_00008018_s07;
                *(s32 *)((char *)(sp108) + 0x14) = 0;
                *(f32 *)((char *)(sp108) + 0x10) = (f32) D_0000087C;
            }
            sp128.w = *(s32 *)((char *)&D_00000000 + 0x8764);
            sp108 = var_s1 + 0xB0;
            sp104 = sp128;
            if ((var_s1 != (void *)-0xB0) || (temp_v0_5 = func_00000000(0x18), sp108 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp108, var_s1, sp104, 1);
                *(s32 *)((char *)(sp108) + 0xC) = (s32) &D_00008018_s08;
                *(s32 *)((char *)(sp108) + 0x14) = 0;
                *(f32 *)((char *)(sp108) + 0x10) = 0.0f;
            }
            sp124.w = *(s32 *)((char *)&D_00000000 + 0x8768);
            sp108 = var_s1 + 0xC8;
            sp104 = sp124;
            if ((var_s1 != (void *)-0xC8) || (temp_v0_5 = func_00000000(0x18), sp108 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp108, var_s1, sp104, 1);
                *(s32 *)((char *)(sp108) + 0xC) = (s32) &D_00008018_s09;
                *(s32 *)((char *)(sp108) + 0x14) = 0;
                *(f32 *)((char *)(sp108) + 0x10) = (f32) D_00000880;
            }
            sp120.w = *(s32 *)((char *)&D_00000000 + 0x876C);
            sp108 = var_s1 + 0xE0;
            sp104 = sp120;
            if ((var_s1 != (void *)-0xE0) || (temp_v0_5 = func_00000000(0x18), sp108 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp108, var_s1, sp104, 1);
                *(s32 *)((char *)(sp108) + 0xC) = (s32) &D_00008018_s10;
                *(s32 *)((char *)(sp108) + 0x14) = 0;
                *(f32 *)((char *)(sp108) + 0x10) = 1.0f;
            }
            sp11C.w = *(s32 *)((char *)&D_00000000 + 0x8770);
            sp108 = var_s1 + 0xF8;
            sp104 = sp11C;
            if ((var_s1 != (void *)-0xF8) || (temp_v0_5 = func_00000000(0x18), sp108 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp108, var_s1, sp104, 1);
                *(s32 *)((char *)(sp108) + 0xC) = (s32) &D_00008018_s11;
                *(s32 *)((char *)(sp108) + 0x14) = 0;
                *(f32 *)((char *)(sp108) + 0x10) = 1.0f;
            }
            sp118.w = *(s32 *)((char *)&D_00000000 + 0x8774);
            sp108 = var_s1 + 0x110;
            sp104 = sp118;
            if ((var_s1 != (void *)-0x110) || (temp_v0_5 = func_00000000(0x18), sp108 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp108, var_s1, sp104, 1);
                *(s32 *)((char *)(sp108) + 0xC) = (s32) &D_00008018_s12;
                *(s32 *)((char *)(sp108) + 0x14) = 0;
                *(f32 *)((char *)(sp108) + 0x10) = 1.0f;
            }
        }
        var_s0 = arg0 + 0x540;
        if ((arg0 != (void *)-0x540) || (temp_v0_5 = func_00000000(0x2C0), var_s0 = temp_v0_5, (temp_v0_5 != 0))) {
            var_s1 = var_s0;
            if ((var_s0 != 0) || (temp_v0_5 = func_00000000(8), var_s1 = temp_v0_5, (temp_v0_5 != 0))) {
                *(s32 *)((char *)(var_s1) + 0x0) = (s32) &D_00008778;
                *(s32 *)((char *)(var_s1) + 0x4) = 0;
            }
            sp100.w = *(s32 *)((char *)&D_00000000 + 0x878C);
            sp8C = var_s0 + 8;
            sp104 = sp100;
            if ((var_s0 != (void *)-8) || (temp_v0_5 = func_00000000(0x18), sp8C = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp8C, var_s0, sp104, 1);
                *(s32 *)((char *)(sp8C) + 0xC) = (s32) &D_00008018_s13;
                *(s32 *)((char *)(sp8C) + 0x14) = 0;
                *(f32 *)((char *)(sp8C) + 0x10) = (f32) D_00000884;
            }
            spFC.w = *(s32 *)((char *)&D_00000000 + 0x8790);
            var_s1 = (char *)&sp7C;
            sp80 = var_s0 + 0x20;
            *(W90 *)var_s1 = spFC;
            if ((var_s0 != (void *)-0x20) || (temp_v0_5 = func_00000000(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp80, var_s0, *(W90 *)var_s1, 1);
                *(s32 *)((char *)(sp80) + 0xC) = (s32) &D_00008018_s14;
                *(s32 *)((char *)(sp80) + 0x14) = 0;
                *(f32 *)((char *)(sp80) + 0x10) = 1.0f;
            }
            spF8.w = *(s32 *)((char *)&D_00000000 + 0x8794);
            sp80 = var_s0 + 0x38;
            *(W90 *)var_s1 = spF8;
            if ((var_s0 != (void *)-0x38) || (temp_v0_5 = func_00000000(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp80, var_s0, *(W90 *)var_s1, 1);
                *(s32 *)((char *)(sp80) + 0xC) = (s32) &D_00008018_s15;
                *(s32 *)((char *)(sp80) + 0x14) = 0;
                *(f32 *)((char *)(sp80) + 0x10) = 1.0f;
            }
            spF4.w = *(s32 *)((char *)&D_00000000 + 0x8798);
            sp80 = var_s0 + 0x50;
            *(W90 *)var_s1 = spF4;
            if ((var_s0 != (void *)-0x50) || (temp_v0_5 = func_00000000(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp80, var_s0, *(W90 *)var_s1, 1);
                *(s32 *)((char *)(sp80) + 0xC) = (s32) &D_00008018_s16;
                *(s32 *)((char *)(sp80) + 0x14) = 0;
                *(f32 *)((char *)(sp80) + 0x10) = 1.0f;
            }
            spF0.w = *(s32 *)((char *)&D_00000000 + 0x879C);
            sp80 = var_s0 + 0x68;
            *(W90 *)var_s1 = spF0;
            if ((var_s0 != (void *)-0x68) || (temp_v0_5 = func_00000000(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp80, var_s0, *(W90 *)var_s1, 1);
                *(s32 *)((char *)(sp80) + 0xC) = (s32) &D_00008018_s17;
                *(s32 *)((char *)(sp80) + 0x14) = 0;
                *(f32 *)((char *)(sp80) + 0x10) = 1.0f;
            }
            spEC.w = *(s32 *)((char *)&D_00000000 + 0x87A0);
            sp80 = var_s0 + 0x80;
            *(W90 *)var_s1 = spEC;
            if ((var_s0 != (void *)-0x80) || (temp_v0_5 = func_00000000(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp80, var_s0, *(W90 *)var_s1, 1);
                *(s32 *)((char *)(sp80) + 0xC) = (s32) &D_00008018_s18;
                *(s32 *)((char *)(sp80) + 0x14) = 0;
                *(f32 *)((char *)(sp80) + 0x10) = 1.0f;
            }
            spE8.w = *(s32 *)((char *)&D_00000000 + 0x87A4);
            sp80 = var_s0 + 0x98;
            *(W90 *)var_s1 = spE8;
            if ((var_s0 != (void *)-0x98) || (temp_v0_5 = func_00000000(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp80, var_s0, *(W90 *)var_s1, 1);
                *(s32 *)((char *)(sp80) + 0xC) = (s32) &D_00008018_s19;
                *(s32 *)((char *)(sp80) + 0x14) = 0;
                *(f32 *)((char *)(sp80) + 0x10) = 1.0f;
            }
            spE4.w = *(s32 *)((char *)&D_00000000 + 0x87A8);
            sp80 = var_s0 + 0xB0;
            *(W90 *)var_s1 = spE4;
            if ((var_s0 != (void *)-0xB0) || (temp_v0_5 = func_00000000(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp80, var_s0, *(W90 *)var_s1, 1);
                *(s32 *)((char *)(sp80) + 0xC) = (s32) &D_00008018_s20;
                *(s32 *)((char *)(sp80) + 0x14) = 0;
                *(f32 *)((char *)(sp80) + 0x10) = (f32) D_00000888;
            }
            spE0.w = *(s32 *)((char *)&D_00000000 + 0x87AC);
            sp80 = var_s0 + 0xC8;
            *(W90 *)var_s1 = spE0;
            if ((var_s0 != (void *)-0xC8) || (temp_v0_5 = func_00000000(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp80, var_s0, *(W90 *)var_s1, 1);
                *(s32 *)((char *)(sp80) + 0xC) = (s32) &D_00008018_s21;
                *(s32 *)((char *)(sp80) + 0x14) = 0;
                *(f32 *)((char *)(sp80) + 0x10) = (f32) D_0000088C;
            }
            spDC.w = *(s32 *)((char *)&D_00000000 + 0x87B0);
            sp80 = var_s0 + 0xE0;
            *(W90 *)var_s1 = spDC;
            if ((var_s0 != (void *)-0xE0) || (temp_v0_5 = func_00000000(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp80, var_s0, *(W90 *)var_s1, 1);
                *(s32 *)((char *)(sp80) + 0xC) = (s32) &D_00008018_s22;
                *(s32 *)((char *)(sp80) + 0x14) = 0;
                *(f32 *)((char *)(sp80) + 0x10) = (f32) D_00000890;
            }
            spD8.w = *(s32 *)((char *)&D_00000000 + 0x87B4);
            sp80 = var_s0 + 0xF8;
            *(W90 *)var_s1 = spD8;
            if ((var_s0 != (void *)-0xF8) || (temp_v0_5 = func_00000000(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp80, var_s0, *(W90 *)var_s1, 1);
                *(s32 *)((char *)(sp80) + 0xC) = (s32) &D_00008018_s23;
                *(s32 *)((char *)(sp80) + 0x14) = 0;
                *(f32 *)((char *)(sp80) + 0x10) = 1.0f;
            }
            spD4.w = *(s32 *)((char *)&D_00000000 + 0x87B8);
            sp80 = var_s0 + 0x110;
            *(W90 *)var_s1 = spD4;
            if ((var_s0 != (void *)-0x110) || (temp_v0_5 = func_00000000(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp80, var_s0, *(W90 *)var_s1, 1);
                *(s32 *)((char *)(sp80) + 0xC) = (s32) &D_00008018_s24;
                *(s32 *)((char *)(sp80) + 0x14) = 0;
                *(f32 *)((char *)(sp80) + 0x10) = (f32) D_00000894;
            }
            spD0.w = *(s32 *)((char *)&D_00000000 + 0x87BC);
            sp80 = var_s0 + 0x128;
            *(W90 *)var_s1 = spD0;
            if ((var_s0 != (void *)-0x128) || (temp_v0_5 = func_00000000(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp80, var_s0, *(W90 *)var_s1, 1);
                *(s32 *)((char *)(sp80) + 0xC) = (s32) &D_00008018_s25;
                *(s32 *)((char *)(sp80) + 0x14) = 0;
                *(f32 *)((char *)(sp80) + 0x10) = (f32) D_00000898;
            }
            spCC.w = *(s32 *)((char *)&D_00000000 + 0x87C0);
            sp80 = var_s0 + 0x140;
            *(W90 *)var_s1 = spCC;
            if ((var_s0 != (void *)-0x140) || (temp_v0_5 = func_00000000(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp80, var_s0, *(W90 *)var_s1, 1);
                *(s32 *)((char *)(sp80) + 0xC) = (s32) &D_00008018_s26;
                *(s32 *)((char *)(sp80) + 0x14) = 0;
                *(f32 *)((char *)(sp80) + 0x10) = (f32) D_0000089C;
            }
            spC8.w = *(s32 *)((char *)&D_00000000 + 0x87C4);
            sp80 = var_s0 + 0x158;
            *(W90 *)var_s1 = spC8;
            if ((var_s0 != (void *)-0x158) || (temp_v0_5 = func_00000000(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp80, var_s0, *(W90 *)var_s1, 1);
                *(s32 *)((char *)(sp80) + 0xC) = (s32) &D_00008018_s27;
                *(s32 *)((char *)(sp80) + 0x14) = 0;
                *(f32 *)((char *)(sp80) + 0x10) = (f32) D_000008A0;
            }
            spC4.w = *(s32 *)((char *)&D_00000000 + 0x87C8);
            sp80 = var_s0 + 0x170;
            *(W90 *)var_s1 = spC4;
            if ((var_s0 != (void *)-0x170) || (temp_v0_5 = func_00000000(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp80, var_s0, *(W90 *)var_s1, 1);
                *(s32 *)((char *)(sp80) + 0xC) = (s32) &D_00008018_s28;
                *(s32 *)((char *)(sp80) + 0x14) = 0;
                *(f32 *)((char *)(sp80) + 0x10) = 0.0f;
            }
            spC0.w = *(s32 *)((char *)&D_00000000 + 0x87CC);
            sp80 = var_s0 + 0x188;
            *(W90 *)var_s1 = spC0;
            if ((var_s0 != (void *)-0x188) || (temp_v0_5 = func_00000000(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp80, var_s0, *(W90 *)var_s1, 1);
                *(s32 *)((char *)(sp80) + 0xC) = (s32) &D_00008018_s29;
                *(s32 *)((char *)(sp80) + 0x14) = 0;
                *(f32 *)((char *)(sp80) + 0x10) = 0.0f;
            }
            spBC.w = *(s32 *)((char *)&D_00000000 + 0x87D0);
            sp80 = var_s0 + 0x1A0;
            *(W90 *)var_s1 = spBC;
            if ((var_s0 != (void *)-0x1A0) || (temp_v0_5 = func_00000000(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp80, var_s0, *(W90 *)var_s1, 1);
                *(s32 *)((char *)(sp80) + 0xC) = (s32) &D_00008018_s30;
                *(s32 *)((char *)(sp80) + 0x14) = 0;
                *(f32 *)((char *)(sp80) + 0x10) = (f32) D_000008A4;
            }
            spB8.w = *(s32 *)((char *)&D_00000000 + 0x87D4);
            sp80 = var_s0 + 0x1B8;
            *(W90 *)var_s1 = spB8;
            if ((var_s0 != (void *)-0x1B8) || (temp_v0_5 = func_00000000(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp80, var_s0, *(W90 *)var_s1, 1);
                *(s32 *)((char *)(sp80) + 0xC) = (s32) &D_00008018_s31;
                *(s32 *)((char *)(sp80) + 0x14) = 0;
                *(f32 *)((char *)(sp80) + 0x10) = (f32) D_000008A8;
            }
            spB4.w = *(s32 *)((char *)&D_00000000 + 0x87D8);
            sp80 = var_s0 + 0x1D0;
            *(W90 *)var_s1 = spB4;
            if ((var_s0 != (void *)-0x1D0) || (temp_v0_5 = func_00000000(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp80, var_s0, *(W90 *)var_s1, 1);
                *(s32 *)((char *)(sp80) + 0xC) = (s32) &D_00008018_s32;
                *(s32 *)((char *)(sp80) + 0x14) = 0;
                *(f32 *)((char *)(sp80) + 0x10) = (f32) D_000008AC;
            }
            spB0.w = *(s32 *)((char *)&D_00000000 + 0x87DC);
            sp80 = var_s0 + 0x1E8;
            *(W90 *)var_s1 = spB0;
            if ((var_s0 != (void *)-0x1E8) || (temp_v0_5 = func_00000000(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp80, var_s0, *(W90 *)var_s1, 1);
                *(s32 *)((char *)(sp80) + 0xC) = (s32) &D_00008060; *(s32 *)((char *)(sp80) + 0x10) = 0x19;
                *(s32 *)((char *)(sp80) + 0x14) = 0;
            }
            spAC.w = *(s32 *)((char *)&D_00000000 + 0x87E0);
            sp80 = var_s0 + 0x200;
            *(W90 *)var_s1 = spAC;
            if ((var_s0 != (void *)-0x200) || (temp_v0_5 = func_00000000(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp80, var_s0, *(W90 *)var_s1, 1);
                *(s32 *)((char *)(sp80) + 0xC) = (s32) &D_00008018_s33;
                *(s32 *)((char *)(sp80) + 0x14) = 0;
                *(f32 *)((char *)(sp80) + 0x10) = 50.0f;
            }
            spA8.w = *(s32 *)((char *)&D_00000000 + 0x87E4);
            sp80 = var_s0 + 0x218;
            *(W90 *)var_s1 = spA8;
            if ((var_s0 != (void *)-0x218) || (temp_v0_5 = func_00000000(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp80, var_s0, *(W90 *)var_s1, 1);
                *(s32 *)((char *)(sp80) + 0xC) = (s32) &D_00008018_s34;
                *(s32 *)((char *)(sp80) + 0x14) = 0;
                *(f32 *)((char *)(sp80) + 0x10) = 150.0f;
            }
            spA4.w = *(s32 *)((char *)&D_00000000 + 0x87E8);
            sp80 = var_s0 + 0x230;
            *(W90 *)var_s1 = spA4;
            if ((var_s0 != (void *)-0x230) || (temp_v0_5 = func_00000000(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp80, var_s0, *(W90 *)var_s1, 1);
                *(s32 *)((char *)(sp80) + 0xC) = (s32) &D_00008018_s35;
                *(s32 *)((char *)(sp80) + 0x14) = 0;
                *(f32 *)((char *)(sp80) + 0x10) = 100.0f;
            }
            spA0.w = *(s32 *)((char *)&D_00000000 + 0x87EC);
            sp80 = var_s0 + 0x248;
            *(W90 *)var_s1 = spA0;
            if ((var_s0 != (void *)-0x248) || (temp_v0_5 = func_00000000(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp80, var_s0, *(W90 *)var_s1, 1);
                *(s32 *)((char *)(sp80) + 0xC) = (s32) &D_00008018_s36;
                *(s32 *)((char *)(sp80) + 0x14) = 0;
                *(f32 *)((char *)(sp80) + 0x10) = 100.0f;
            }
            sp9C.w = *(s32 *)((char *)&D_00000000 + 0x87F0);
            sp80 = var_s0 + 0x260;
            *(W90 *)var_s1 = sp9C;
            if ((var_s0 != (void *)-0x260) || (temp_v0_5 = func_00000000(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp80, var_s0, *(W90 *)var_s1, 1);
                *(s32 *)((char *)(sp80) + 0xC) = (s32) &D_00008018_s37;
                *(s32 *)((char *)(sp80) + 0x14) = 0;
                *(f32 *)((char *)(sp80) + 0x10) = 0.0f;
            }
            sp98.w = *(s32 *)((char *)&D_00000000 + 0x87F4);
            sp80 = var_s0 + 0x278;
            *(W90 *)var_s1 = sp98;
            if ((var_s0 != (void *)-0x278) || (temp_v0_5 = func_00000000(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp80, var_s0, *(W90 *)var_s1, 1);
                *(s32 *)((char *)(sp80) + 0xC) = (s32) &D_00008060_s2; *(s32 *)((char *)(sp80) + 0x10) = 0xA;
                *(s32 *)((char *)(sp80) + 0x14) = 0;
            }
            sp94.w = *(s32 *)((char *)&D_00000000 + 0x87F8);
            sp80 = var_s0 + 0x290;
            *(W90 *)var_s1 = sp94;
            if ((var_s0 != (void *)-0x290) || (temp_v0_5 = func_00000000(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp80, var_s0, *(W90 *)var_s1, 1);
                *(s32 *)((char *)(sp80) + 0xC) = (s32) &D_00008060_s3; *(s32 *)((char *)(sp80) + 0x10) = 0xF;
                *(s32 *)((char *)(sp80) + 0x14) = 0;
            }
            sp90.w = *(s32 *)((char *)&D_00000000 + 0x87FC);
            sp80 = var_s0 + 0x2A8;
            *(W90 *)var_s1 = sp90;
            if ((var_s0 != (void *)-0x2A8) || (temp_v0_5 = func_00000000(0x18), sp80 = temp_v0_5, (temp_v0_5 != 0))) {
                func_00000000(sp80, var_s0, *(W90 *)var_s1, 1);
                *(s32 *)((char *)(sp80) + 0xC) = (s32) &D_00008018_s38;
                *(s32 *)((char *)(sp80) + 0x14) = 0;
                *(f32 *)((char *)(sp80) + 0x10) = 1.0f;
            }
        }
        var_s0 = arg0 + 0x808; if (1) {}
        if ((arg0 != (void *)-0x808) || (var_s0 = func_00000000(0xC), (var_s0 != 0))) {
            *(s32 *)var_s0 = 0;
        }
        temp_s1 = arg0 + 0x878;
        var_s0 = temp_s1;
        if ((arg0 != (void *)-0x878) || (temp_v0_5 = func_00000000(0x40), var_s0 = temp_v0_5, (temp_v0_5 != 0))) {
            func_00000000(var_s0);
            *(s32 *)((char *)(var_s0) + 0x30) = (s32) &bu_90cc_imp30;
        }
        *(s32 *)((char *)(arg0) + 0x908) = 0;
        *(s32 *)((char *)(arg0) + 0x848) = arg1;
        *(s32 *)((char *)(arg0) + 0x8C4) = arg2;
        fq = (s32 *)arg0;
        fq += 6;
        if (1) { *fq &= ~2; }
        *(s32 *)((char *)(arg0) + 0x8DC) = 0;
        /* target 0xA330-0xA388: beq-chain to out-of-line cases (default inline first,
         * case 7, then case 6 falling through) with per-case var_t1 = arg8|1 -- switch shape.
         * (goto-layout variant re-normalized by IDO and scored worse; keep switch) */
        switch (arg2) {
        case 7: goto sw_c7;
        case 6: goto sw_c6;
        }
        *(s16 *)((char *)(arg0) + 0x900) = 4;
        *(s16 *)((char *)(arg0) + 0x902) = 7;
        
        goto sw_end;
sw_c7:
        *(s16 *)((char *)(arg0) + 0x900) = 0x19;
        *(s16 *)((char *)(arg0) + 0x902) = 0x1A;
        arg8 |= 1;
        goto sw_end;
sw_c6:
        *(s16 *)((char *)(arg0) + 0x900) = 0x17; *(s16 *)((char *)(arg0) + 0x902) = 0x18;
        arg8 |= 1;
sw_end:
        sp174 = 0 + 0 * (int)(arg8); /* agent-c run 212: folded counted use (zero code) */
        if ((arg1 == 1) && (*(s32 *)((char *)&D_00000000 + 0x4C) == *(s32 *)((char *)&D_00000000 + 0x58))) {
            sp174 = 1;
        }
        if ((*(s32 *)((char *)&D_00000000 + 0x34) == 3) && (*(s32 *)((char *)&D_00000000 + 0x4C) == *(u8 *)((char *)&D_00000000 + 0x17D)) && (arg1 == 2)) {
            sp174 = 1;
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
                    *(f32 *)((char *)(arg0) + 0x8E0) = (f32) cr / inv;
                    *(f32 *)((char *)(arg0) + 0x8E4) = (f32) cg / inv;
                    *(f32 *)((char *)(arg0) + 0x8E8) = (f32) cb / inv;
                    *(f32 *)((char *)(arg0) + 0x8EC) = fi(255) / inv;
                } else {
                    {
                    inv = 255.0f;
                    prgb = bu_90cc_rgb24;
                    cr = prgb[0];
                    cg = prgb[1];
                    cb = prgb[2];
                    *(f32 *)((char *)(arg0) + 0x8E0) = (f32) cr / inv;
                    *(f32 *)((char *)(arg0) + 0x8E4) = (f32) cg / inv;
                    *(f32 *)((char *)(arg0) + 0x8E8) = (f32) cb / inv;
                    *(f32 *)((char *)(arg0) + 0x8EC) = fi(255) / inv;
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
                    *(f32 *)((char *)(arg0) + 0x8E0) = (f32) cr / inv;
                    *(f32 *)((char *)(arg0) + 0x8E4) = (f32) cg / inv;
                    *(f32 *)((char *)(arg0) + 0x8E8) = (f32) cb / inv;
                    *(f32 *)((char *)(arg0) + 0x8EC) = 164.0f / inv;
                } else {
                    inv = 255.0f;
                    prgb = bu_90cc_rgb2C;
                    cr = prgb[0];
                    cg = prgb[1];
                    cb = prgb[2];
                    *(f32 *)((char *)(arg0) + 0x8E0) = (f32) cr / inv;
                    *(f32 *)((char *)(arg0) + 0x8E4) = (f32) cg / inv;
                    *(f32 *)((char *)(arg0) + 0x8E8) = (f32) cb / inv;
                    *(f32 *)((char *)(arg0) + 0x8EC) = 164.0f / inv;
                }
            }
            arg8 &= ~4;
        } else {
            if (arg7 & 8) {
                inv = 255.0f;
                temp_f0 = fi(255) / inv;
                *(f32 *)((char *)(arg0) + 0x8E0) = temp_f0;
                *(f32 *)((char *)(arg0) + 0x8E4) = temp_f0;
                *(f32 *)((char *)(arg0) + 0x8E8) = temp_f0;
                *(f32 *)((char *)(arg0) + 0x8EC) = 117.0f / inv;
            } else {
                inv = 255.0f;
                temp_f0 = fi(255) / inv;
                *(f32 *)((char *)(arg0) + 0x8E0) = temp_f0;
                *(f32 *)((char *)(arg0) + 0x8E4) = temp_f0;
                *(f32 *)((char *)(arg0) + 0x8E8) = temp_f0;
                *(f32 *)((char *)(arg0) + 0x8EC) = 70.0f / inv;
                if (*(s32 *)((char *)&D_00000000 + 0x34) != 2) {
                    arg7 |= 0x1000;
                }
            }
        }
        temp_v0_51 = arg7 | 0x4800;
        *(s32 *)((char *)(arg0) + 0x8BC) = temp_v0_51;
        /* run 179: this store inside if (1) { } (a BB boundary before the 0x8B8 store)
         * stops as1 copying the `arg8 &= ~4` join into the rgb30 arm (length 8436 ->
         * 8424 = target) and gives arg2 the target t3 */
        if (1) { *(s32 *)((char *)(arg0) + 0xA58) = temp_v0_51; }
        *(s32 *)((char *)(arg0) + 0x8B8) = arg8 + 0 * (int)(arg7); /* agent-c run 212: folded counted use (zero code) */
        func_00000000_p4f(temp_s1, arg0, 0x6D, 0x43160000, 200.0f);
        if (*(s32 *)((char *)(arg0) + 0xA58) & 8) {
            *(s16 *)((char *)(arg0) + 0x88C) = 4;
            *(s16 *)((char *)(arg0) + 0x88E) = 2;
        } else {
            *(s16 *)((char *)(arg0) + 0x88C) = 5; *(s16 *)((char *)(arg0) + 0x88E) = 1;
        }
        func_00000000(*(s32 *)((char *)&D_00000000 + 0), temp_s1);
        vpos.x = arg5;
        vpos.z = arg6;
        vpos.y = 0.0f;
        func_00000000_iipif(*(s32 *)((char *)&D_00000000 + 0), 1, &vpos.y, arg5, arg6);
        vpos.y += D_000008B0;
        wpos = vpos;
        *(f32 *)((char *)(arg0) + 0x3A4) = wpos.x;
        *(f32 *)((char *)(arg0) + 0x3A8) = wpos.y;
        *(f32 *)((char *)(arg0) + 0x3AC) = wpos.z;
        *(s32 *)((char *)(arg0) + 0x8C0) = (s32) (1 << arg2);
        *(s32 *)((char *)(arg0) + 0x38C) = 1;
        *(s32 *)((char *)(arg0) + 0x870) = 0;
        *(s32 *)((char *)(arg0) + 0x868) = 0;
        *(s32 *)((char *)(arg0) + 0x86C) = 0;
        func_00000000(arg0 + 0x808, arg1, arg2);
        if (*(s32 *)((char *)(arg0) + 0xA58) & 3) {
            func_00000000(bu_90cc_d87A4 + 0x5C, 0);
            *(char **)((char *)(arg0) + 0x86C) = func_00000000(0, bu_90cc_d87A4 + 0x64);
            *(char **)((char *)(arg0) + 0x868) = func_00000000(0, &D_00008814);
            var_s0 = *(char **)((char *)(arg0) + 0x86C);
            var_s0 += 0x18;
            *(s32 *)var_s0 &= ~8;
            var_s1 = *(char **)((char *)(arg0) + 0x86C);
            var_s0 = *(char **)(*(char **)(bu_90cc_dF0 + 0x44) + 0x88);
            func_00000000(var_s0 + 0x10, var_s1);
            if (*(s32 *)((char *)(var_s1) + 0x14) != 0) {
                *(s32 *)((char *)(var_s1) + 0x4) = 1;
            }
            *(s32 *)((char *)(var_s1) + 0x14) = (s32) var_s0;
            temp_v0_5 = func_00000000(0, &D_00008820);
            *(char **)((char *)(arg0) + 0x868) = temp_v0_5;
            fq = (s32 *)temp_v0_5;
            if (1) { fq += 6; }
            *fq &= ~8;
            var_s1 = *(char **)((char *)(arg0) + 0x868);
            var_s0 = *(char **)(*(char **)(bu_90cc_dF0 + 0x44) + 0x84);
            func_00000000(var_s0 + 0x10, var_s1);
            if (*(s32 *)((char *)(var_s1) + 0x14) != 0) {
                *(s32 *)((char *)(var_s1) + 0x4) = 1;
            }
            *(s32 *)((char *)(var_s1) + 0x14) = (s32) var_s0;
            func_00000000(1);
            var_s0 = func_00000000(0x23C);
            if (var_s0 != 0) {
                var_s1 = var_s0;
                if ((var_s1 != 0) || (var_s1 = func_00000000(0x174), (var_s1 != 0))) {
                    func_00000000(var_s1, &D_0000882C);
                    *(s32 *)((char *)(var_s1) + 0x28) = (s32) &bu_90cc_vt545;
                }
                *(s32 *)((char *)(var_s0) + 0x28) = (s32) &bu_90cc_vt546;
            }
            sp164 = var_s0;
            var_s1 = *(char **)((char *)(arg0) + 0X868);
            func_00000000(var_s1 + 0x10, var_s0);
            if (*(s32 *)((char *)(var_s0) + 0x14) != 0) {
                *(s32 *)((char *)(var_s0) + 0x4) = 1;
            }
            *(s32 *)((char *)(var_s0) + 0x14) = (s32) var_s1;
            if (*(s32 *)((char *)(arg0) + 0xA58) & 1) {
                var_s1 = func_00000000(0X1F4);
                if (var_s1 != 0) {
                    var_a2 = (s32) var_s1;
                    if ((var_a2 != 0) || (var_a2 = (s32)func_00000000(0x174), (var_a2 != 0))) {
                        func_00000000((char *)var_a2, &D_00008834);
                        *(s32 *)((char *)(var_a2) + 0x28) = (s32) &bu_90cc_vt545_b;
                    }
                    *(s32 *)((char *)(var_s1) + 0x28) = (s32) &bu_90cc_vt547;
                }
                sp160 = var_s1;
                var_s0 = var_s1;
                var_s1 = *(char **)((char *)(arg0) + 0X86C);
                func_00000000(var_s1 + 0x10, var_s0);
                if (*(s32 *)((char *)(var_s0) + 0x14) != 0) {
                    *(s32 *)((char *)(var_s0) + 0x4) = 1;
                }
                *(s32 *)((char *)(var_s0) + 0x14) = (s32) var_s1;
            }
            var_s1 = func_00000000(0X174);
            if (var_s1 != 0) {
                var_a2 = (s32) var_s1;
                if ((var_a2 != 0) || (var_a2 = (s32)func_00000000(0x174), (var_a2 != 0))) {
                    func_00000000((char *)var_a2, &D_0000883C);
                    *(s32 *)((char *)(var_a2) + 0x28) = (s32) &bu_90cc_vt545_c;
                }
                *(s32 *)((char *)(var_s1) + 0x28) = (s32) &bu_90cc_vt548;
            }
            sp15C = var_s1;
            var_s0 = var_s1;
            var_s1 = *(char **)((char *)(arg0) + 0X868);
            func_00000000(var_s1 + 0x10, var_s0);
            if (*(s32 *)((char *)(var_s0) + 0x14) != 0) {
                *(s32 *)((char *)(var_s0) + 0x4) = 1;
            }
            *(s32 *)((char *)(var_s0) + 0x14) = (s32) var_s1;
            var_s1 = func_00000000(0X174);
            if (var_s1 != 0) {
                var_a2 = (s32) var_s1;
                if ((var_a2 != 0) || (var_a2 = (s32)func_00000000(0x174), (var_a2 != 0))) {
                    func_00000000((char *)var_a2, &D_00008844);
                    *(s32 *)((char *)(var_a2) + 0x28) = (s32) &bu_90cc_vt545_d;
                }
                *(s32 *)((char *)(var_s1) + 0x28) = (s32) &bu_90cc_vt548_b;
            }
            sp158 = var_s1;
            var_s0 = var_s1;
            var_s1 = *(char **)((char *)(arg0) + 0X868);
            func_00000000(var_s1 + 0x10, var_s0);
            if (*(s32 *)((char *)(var_s0) + 0x14) != 0) {
                *(s32 *)((char *)(var_s0) + 0x4) = 1;
            }
            *(s32 *)((char *)(var_s0) + 0x14) = (s32) var_s1;
            func_00000000(0xFFFF);
            func_00000000();
        }
        func_00000000(&D_0000884C, 0);
        var_s1 = func_00000000(0x16C);
        if (var_s1 != 0) {
            func_00000000_p4d(var_s1, &D_0000885C, 0.0, 0.0f);
            temp_f0 = D_000008B4;
            *(s32 *)((char *)(var_s1) + 0x28) = (s32) &bu_90cc_d207dc; *(s32 *)((char *)(var_s1) + 0x120) = 0xFFFF;
            *(f32 *)((char *)(var_s1) + 0x108) = temp_f0;
            *(f32 *)((char *)(var_s1) + 0x10C) = temp_f0;
            *(f32 *)((char *)(var_s1) + 0x110) = temp_f0;
            *(f32 *)((char *)(var_s1) + 0x124) = 1.0f;
        }
        *(char **)((char *)(arg0) + 0x850) = var_s1;
        func_00000000(arg0, var_s1);
        var_s0 = *(char **)((char *)(arg0) + 0x850);
        temp_f0 = fi(0);
        var_s0 += 0xB4;
        *(f32 *)((char *)(var_s0) + 0x0) = temp_f0;
        *(f32 *)((char *)(var_s0) + 0x8) = temp_f0;
        *(f32 *)((char *)(var_s0) + 0x4) = 100.0f;
        *(char **)((char *)(arg0) + 0x840) = func_00000000(0, *(s32 *)((char *)&D_00000000 + 0), arg0);
        func_00000000();
        *(s32 *)((char *)(arg0) + 0x800) = arg4;
        var_v1 = *(s32 *)((char *)(arg0) + 0x840); func_00000000(arg0, var_v1);
        var_s0 = *(char **)((char *)(arg0) + 0x840);
        var_s0 += 0x18;
        *(s32 *)var_s0 &= ~8;
        /* target 0xAC34-0xAC98: each global access is its own symbol (fresh lui per
         * site, reload between the |= and &= statements) -- per-site blank aliases
         * bust the m2c &D_00000000 base GCSE that had cached one addiu base reg. */
        *(s32 *)&bu_90cc_g8_st = *(s32 *)&bu_90cc_g8_ld | 8;
        var_v1 = bu_90cc_gcfg_ld.f04; var_v1 |= 0x20000; bu_90cc_gcfg_st.f04 = var_v1;
        var_v1 = bu_90cc_gcfg_b_ld.f04; var_v1 &= 0xFFF7FFFF; var_v1 &= ~2; bu_90cc_gcfg_b_st.f04 = var_v1;
        bu_90cc_gown = (s32) arg0;
        for (sp58 = 0; sp58 != 0x23; sp58++) {
            var_a2 = sp58;
            if (sp58 >= 0x28U) {
                func_00000000(&D_00008868, sp58);
            }
            *(s32 *)((char *)&bu_90cc_gtbl + var_a2 * 4) = 0;
        }
        *(s32 *)((char *)&bu_90cc_gpair + 4) = 0x2A003;
        *(s32 *)&bu_90cc_gpair = 0;
        *(char **)((char *)(arg0) + 0x8F4) = func_00000000(&bu_90cc_gdev, 1, 0);
        *(char **)((char *)(arg0) + 0x8F8) = func_00000000(&bu_90cc_gdev, 2, 0);
        *(char **)((char *)(arg0) + 0x8FC) = func_00000000(&bu_90cc_gdev, 3, 0);
        if (sp174 != 0) {
            temp_v0_5 = (char *)bu_90cc_fz2; /* target lui v1 / addiu v0,v1,0 (blank) then lw 0(v0): fn-typed base */
            if (*(s32 *)temp_v0_5 != 0) {
                var_v1 = *(s32 *)(char *)bu_90cc_d00_b;
                temp_s1_8 = (char *)(bu_90cc_d148_01 + 24);
                do {
                    if (arg2 == var_v1) {
                        /* (temp_v0_5->unk4 * 4)->unk160 store: scaled-index table write */
                        var_s0 = temp_s1_8 + *(s32 *)((char *)temp_v0_5 + 0x4) * 4;
                        *(s32 *)var_s0 = (s32)(temp_v0_5 + 8);
                    }
                    var_v1 = *(s32 *)((char *)(temp_v0_5) + 0x18);
                    temp_v0_5 += 0x18;
                } while (var_v1 != 0);
            }
        } else {
            for (var_v1 = 0; var_v1 < 3; var_v1++) ((s32 *)bu_90cc_d148_18)[var_v1] = 0;
        }
        if (*(s32 *)((char *)(arg0) + 0xA58) & 8) {
            func_00000000(arg0);
        } else {
            func_00000000(arg0);
        }
        *(s16 *)((char *)(arg0) + 0x904) = (s16) arg3;
        { s32 v840 = *(s32 *)((char *)(arg0) + 0x840); func_00000000(arg2, arg3, *(s32 *)((char *)(arg0) + 0x8B8), v840, arg0); }
        func_00000000(arg0, *(s32 *)((char *)(arg0) + 0xA58) & 1);
        if (*(s32 *)((char *)(arg0) + 0xA58) & 3) {
            func_00000000(&D_00008880, 0);
            temp_v0_5 = func_00000000(0, &D_00008888);
            *(char **)((char *)(arg0) + 0x870) = temp_v0_5;
            fq = (s32 *)temp_v0_5;
            if (1) { fq += 6; }
            *fq &= ~4;
            func_00000000(arg0, *(s32 *)((char *)(arg0) + 0x870));
            var_s1 = *(char **)((char *)(arg0) + 0x870);
            func_00000000(var_s1 + 0x10, sp164); if (1) {}
            if (*(s32 *)((char *)(sp164) + 0x14) != 0) {
                *(s32 *)((char *)(sp164) + 0x4) = 1;
            }
            *(char **)((char *)(sp164) + 0x14) = var_s1;
            if (*(s32 *)((char *)(arg0) + 0xA58) & 1) {
                var_s1 = *(char **)((char *)(arg0) + 0x870);
                func_00000000(var_s1 + 0x10, sp160);
                if (*(s32 *)((char *)(sp160) + 0x14) != 0) {
                    *(s32 *)((char *)(sp160) + 0x4) = 1;
                }
                *(char **)((char *)(sp160) + 0x14) = var_s1;
            }
            var_s1 = *(char **)((char *)(arg0) + 0x870);
            func_00000000(var_s1 + 0x10, sp15C); if (1) {}
            if (*(s32 *)((char *)(sp15C) + 0x14) != 0) {
                *(s32 *)((char *)(sp15C) + 0x4) = 1;
            }
            *(char **)((char *)(sp15C) + 0x14) = var_s1;
            var_s1 = *(char **)((char *)(arg0) + 0x870);
            func_00000000(var_s1 + 0x10, sp158); if (1) {}
            if (*(s32 *)((char *)(sp158) + 0x14) != 0) {
                *(s32 *)((char *)(sp158) + 0x4) = 1;
            }
            *(char **)((char *)(sp158) + 0x14) = var_s1;
            func_00000000(1);
            var_s0 = *(char **)((char *)(sp164) + 0x28);
            var_v1 = bu_90cc_d44_01.f28;
            temp_s1 = (char *)(var_v1 + 0x70);
            ((Vfn_ff)*(s32 *)((char *)(var_s0) + 0x5C))(*(s16 *)((char *)(var_s0) + 0x58) + sp164, *(s32 *)((char *)(arg0) + 0x870), *(s32 *)((char *)(arg0) + 0x86C), arg0, 0x10, (s32)temp_s1, *(f32 *)(bu_90cc_d8B4_01 + 4), 100.0f);
            if (*(s32 *)((char *)(arg0) + 0xA58) & 1) {
                var_s0 = *(char **)((char *)(sp160) + 0x28); if (1) {}
                ((Vfn_ff)*(s32 *)((char *)(var_s0) + 0x5C))(*(s16 *)((char *)(var_s0) + 0x58) + sp160, *(s32 *)((char *)(arg0) + 0x870), *(s32 *)((char *)(arg0) + 0x86C), arg0, 0xC, (s32)temp_s1, *(f32 *)(bu_90cc_d8B4_02 + 8), 100.0f);
                func_00000000(sp164);
                func_00000000(sp164);
            }
            temp_v0_51 = bu_90cc_d08_01.f2C;
            var_s0 = *(char **)((char *)(sp15C) + 0x28);
            ((Vfn_ff)*(s32 *)((char *)(var_s0) + 0x5C))(*(s16 *)((char *)(var_s0) + 0x58) + sp15C, *(s32 *)((char *)(arg0) + 0x870), *(s32 *)((char *)(arg0) + 0x86C), arg0, 8, temp_v0_51 + 0x70, 32.0f, 74.0f);
            var_v1 = bu_90cc_d44_02.f04;
            var_s0 = *(char **)((char *)(sp158) + 0x28);
            ((Vfn_ff)*(s32 *)((char *)(var_s0) + 0x5C))(*(s16 *)((char *)(var_s0) + 0x58) + sp158, *(s32 *)((char *)(arg0) + 0x870), *(s32 *)((char *)(arg0) + 0x86C), arg0, 8, var_v1 + 0x70, 32.0f, 74.0f);
            func_00000000(0xFFFF);
            func_00000000();
        }
        *(s32 *)((char *)(arg0) + 0x814) = (s32) *(s32 *)((char *)bu_90cc_fz + 0);
        *(s32 *)((char *)(arg0) + 0x818) = (s32) *(s32 *)((char *)bu_90cc_fz + 8);
        *(s32 *)((char *)(arg0) + 0x81C) = (s32) *(s32 *)((char *)bu_90cc_fz + 4);
        var_s0 = (char *)bu_90cc_d44_03.f28;
        *(s32 *)((char *)(arg0) + 0x824) = (s32)var_s0;
        var_s0 = (char *)bu_90cc_d80_01.f08;
        *(s32 *)((char *)(arg0) + 0x820) = (s32)var_s0;
        var_s1 = (char *)bu_90cc_d08_02.f00;
        *(s32 *)((char *)(arg0) + 0x828) = (s32)var_s1;
        var_s1 = (char *)bu_90cc_d08_03.f14;
        *(s32 *)((char *)(arg0) + 0x830) = (s32)var_s1;
        var_s1 = (char *)*(s32 *)((char *)&D_00000000 + 4);
        *(s32 *)((char *)(arg0) + 0x82C) = (s32)var_s1;
        var_s1 = (char *)bu_90cc_d08_04.f10;
        *(s32 *)((char *)(arg0) + 0x834) = (s32)var_s1;
        var_s1 = (char *)bu_90cc_d44_04.f0C;
        *(s32 *)((char *)(arg0) + 0x838) = (s32)var_s1;
        var_s0 = (char *)bu_90cc_d44_05.f2C;
        *(s32 *)((char *)(arg0) + 0x844) = (s32)var_s0;
        var_s0 = (char *)bu_90cc_d80_02.f04;
        *(s32 *)((char *)(arg0) + 0x860) = (s32)var_s0;
        *(s32 *)((char *)(arg0) + 0x854) = (s32) bu_90cc_d44_06.f30;
        var_s0 = (char *)bu_90cc_d44_07.f34;
        *(s32 *)((char *)(arg0) + 0x85C) = (s32)var_s0;
        var_s0 = (char *)bu_90cc_d44_08.f38;
        *(s32 *)((char *)(arg0) + 0x858) = (s32)var_s0;
        var_s0 = (char *)bu_90cc_d80_03.f00;
        *(s32 *)((char *)(arg0) + 0x864) = (s32)var_s0;
        var_s0 = (char *)bu_90cc_d44_09.f14;
        *(s32 *)((char *)(arg0) + 0x874) = (s32)var_s0;

        *(char **)((char *)(arg0) + 0x804) = func_00000000(0, arg0, bu_90cc_d44_10.f24);
        func_00000000(arg0);
    }
    return arg0;
}
