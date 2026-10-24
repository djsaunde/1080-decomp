#include "common.h"
/* bootup.uso RoData section base (USO symIdx 1): every FP literal load is
 * `lui at,0; lwc1 OFF(at)` with OFF = the RoData pool offset, so a DATA
 * symbol pinned at 0 indexed by OFF gives the lui-at/lwc1 shape (the old
 * `(char *)&func_00000940 + OFF` cast materialized the function address). */
extern float D_bu_ro_pool[];
extern double D_bu_ro_dpool[];  /* same base, f64 view (8-aligned pool offsets) */
extern f32 D_00000128, D_00000868, D_0000086C, D_00000870, D_00000874, D_00000878, D_0000087C, D_00000880, D_00000884, D_00000888, D_0000088C, D_00000890, D_00000894, D_00000898, D_0000089C, D_000008A0, D_000008A4, D_000008A8, D_000008AC, D_000008B0, D_000008B4, D_000008B8, D_000008BC, D_000008E0, D_000008F0, D_00000910, D_00000920, D_00000924, D_00000928, D_00000938, D_0000093C, D_00000940, D_00000944, D_00000948, D_0000094C;

/* func_00000000 is splat's synthetic symbol for unrelocated JAL target 0x0.
 * Raw bytes are `jr $ra; nop`. IDO -O2 emits exactly that for an empty
 * function with implicit int return (per docs/IDO_CODEGEN.md#feedback-ido-
 * empty-void-matchable). K&R empty arg list + implicit int keeps every
 * same-TU caller compatible (some use return value, some discard). */
func_00000000() {
}

/* File-scope K&R decl, int return. Void-discarding callers just ignore
 * the value; value-using callers get it directly. Same underlying symbol. */

extern char D_00000000;

#ifdef NON_MATCHING
/* Macro definitions for NM-wrap bodies. Auto-managed by /struct-name-tick.
 * Default build never sees these — wrap bodies aren't compiled.
 * BOOT_SELF_PTR: pointer to the self/context struct (read into `self`/`dst`/
 * `t6` locals across wraps). bootup_uso D+0x254.
 */
#endif
/* BOOT_SELF_PTR graduated to unconditional 2026-07-03: promoted exact bodies
 * (func_000082F8/func_000050A0) use it in the default build. */
#define BOOT_SELF_PTR (*(int**)((char*)&D_00000000 + 0x254))

void func_00000008(int *dst) {
    int buf[2];
    func_00000000(&D_00000000, buf, 4);
    *dst = buf[0];
}

void func_00000044(float *dst) {
    float buf[2];
    func_00000000(&D_00000000, buf, 4);
    *dst = buf[0];
}

typedef struct { int a, b, c; } Tri3i;
typedef struct { float x, y, z; } Vec3;
void func_00000080(Vec3 *dst) {
    int pad_top[1];
    Tri3i raw;
    int pad_mid[2];
    Tri3i tmp;
    int pad_bot[2];
    func_00000000(&D_00000000, &raw, 12);
    tmp = raw;
    dst->x = *(float*)&tmp.a;
    dst->y = *(float*)&tmp.b;
    dst->z = *(float*)&tmp.c;
}

typedef struct { int a, b, c, d; } Quad4;
void func_000000F0(Quad4 *dst) {
    Quad4 buf;
    func_00000000(&D_00000000, &buf, 16);
    *dst = buf;
}

extern char D_00006538;
void func_00000148(int a0, int a1, int a2) {
    func_00000000(&D_00006538);
    func_00000000(a0, a1, a2);
}

extern char D_00006544;
void func_00000188(int a0, int a1, int a2) {
    func_00000000(&D_00006544);
    func_00000000(a0, a1, a2);
}

void func_000001C8(int a0) {
    if (a0 != 0) {
        func_00000000(2);
    } else {
        func_00000000(0);
    }
}

void func_000001FC(void) {
    func_00000000();
}

void func_0000021C(void) {
    func_00000000();
}

void func_0000023C(void) {
    func_00000000();
}

void func_0000025C(void) {
    func_00000000();
}

void func_0000027C(void) {
    func_00000000();
}

void func_0000029C(void) {
    func_00000000();
}

void func_000002BC(void) {
    func_00000000();
}

void func_000002DC(void) {
    func_00000000();
}

void func_000002FC(void) {
    func_00000000();
}

void func_0000031C(int a0, int a1, int a2) {
    int local1, local2;
    func_00000000(a1, &local1, &local2, a2);
    func_00000000(a0, local1, (signed char)local2);
}

extern char D_00006550;
void func_00000360(void) {
    func_00000000();
    func_00000000(&D_00006550);
}

extern char D_00006568;
void func_0000038C(void) {
    func_00000000();
    func_00000000(&D_00006568);
}

void func_000003B8(void) {
    func_00000000();
}

void func_000003D8(void) {
    func_00000000();
}

// func_000003F8(Vec3 *world, float *outDist, int *outProj, Obj *obj):
// camera-relative projection / distance helper (billboard or HUD-marker math).
//
//   Cam *cam = obj->0x70;            // view/camera sub-struct
//   Vec3 v = *world;                 // local copy (sp+0x3C..0x44)
//   v.x -= cam->0xA0;  v.y -= cam->0xA4;  v.z -= cam->0xA8;   // eye-relative
//   float lenSq = v.x*v.x + v.y*v.y + v.z*v.z;
//   float dist  = func_00000000(lenSq);          // = sqrtf(lenSq)
//   *outDist = dist;
//   float inv = (float)(1.0 / (double)dist);     // reciprocal (done in f64)
//   v.x *= inv;  v.y *= inv;  v.z *= inv;         // normalize
//   float d = v.x*cam->0xB4 + v.y*cam->0xC4 + v.z*cam->0xD4;  // dot w/ basis row
//   *outProj = (int)(d * 63.0f);                  // 0x427C0000 = 63.0f
//   *outDist = *outDist / K;                      // K = func_00000044+0xC literal
//   if ((double)*outDist > 1.0) *outDist = 1.0f;  // clamp to [.,1]
//
// Struct-typing reference:
//   world = arg0 Vec3 (0x0 x, 0x4 y, 0x8 z);  outDist = arg1 f32*;
//   outProj = arg2 s32*;  obj = arg3, obj->0x70 = Cam:
//     0xA0/0xA4/0xA8 eye position Vec3;
//     0xB4/0xC4/0xD4 a basis row Vec3 (stride 0x10 -> 4x4 matrix rows).
#ifndef FW
#define FW(p, o) (*(int *)((char *)(p) + (o)))
#endif
/* EXACT (agent-c run 115). The delta is a real 12-byte struct copy of *pos
 * (pointer-based lw/sw); the camera column is an f32 ARRAY (stays in memory:
 * store-then-reload per element, 0x48 frame with four ghost f32 decls); sqrtf
 * is a typed float->float jal-0 callee; the 1.0 double divisor is reused for
 * the clamp. The dot product is written last-term-first with the inner pair
 * reversed and an assignment lever on d.x, which gives the target's
 * running-sum-as-fs add.s and the f18 literal register; the range divisor is
 * the f32 at func_00000044+0xC (D_00000050). */
typedef struct { f32 x, y, z; } V3_3F8;
extern f32 func_00000000_3F8_f(f32);
extern f32 D_00000050;
void func_000003F8(V3_3F8 *pos, f32 *dist, s32 *proj, char *obj) {
    V3_3F8 d;
    f32 n[3];
    f32 q0, q1, q2, q3;
    f32 inv;
    f32 t;

    d = *pos;
    d.x = d.x - *(f32 *)(*(char **)(obj + 0x70) + 0xA0);
    d.y = d.y - *(f32 *)(*(char **)(obj + 0x70) + 0xA4);
    d.z = d.z - *(f32 *)(*(char **)(obj + 0x70) + 0xA8);
    *dist = func_00000000_3F8_f(d.x * d.x + d.y * d.y + d.z * d.z);
    inv = 1.0 / *dist;
    d.x *= inv;
    d.y *= inv;
    d.z *= inv;
    n[0] = *(f32 *)(*(char **)(obj + 0x70) + 0xB4);
    n[1] = *(f32 *)(*(char **)(obj + 0x70) + 0xC4);
    n[2] = *(f32 *)(*(char **)(obj + 0x70) + 0xD4);
    *proj = (s32)((d.z * n[2] + (d.y * n[1] + n[0] * (t = d.x))) * 63.0f);
    *dist = *dist / D_00000050;
    if (*dist > 1.0) {
        *dist = 1.0f;
    }
}

/* func_0000057C: alloc obj = func(0x64) into arg0->0x0; switch(arg1) selects
 * 0/1/2; init obj->0x22=(s8)arg2, obj->0x10=0.0f, arg0->0x8=0; then call obj's
 * handler with the selector. Byte-exact (incl. the switch branch-likely + dead
 * store); func_00000000 = cross-segment placeholder (jal 0). Key: `sel=0` must
 * be set AFTER the alloc call so it stays in $a2 (before-the-call spills it). */
void func_0000057C(int *arg0, int arg1, int arg2) {
    int sel;
    arg0[0] = (int)func_00000000(0x64);
    sel = 0;
    switch (arg1) {
    case 0: break;
    case 1: sel = 1; break;
    case 2: sel = 2; break;
    }
    *(char *)(arg0[0] + 0x22) = (char)arg2;
    *(float *)(arg0[0] + 0x10) = 0.0f;
    arg0[2] = 0;
    func_00000000(arg0[0], sel);
}

/* func_00000610 - corrected full decode (0x24C, 147 insns, FP param
 * normalize / clamp / scale + 5 func_00000000 dispatch calls).
 * Signature recovered from arg-home slots: a0=int* (struct-ptr ptr),
 * then 5 f32 (arg1..arg5) + 3 s32 (arg6..arg8). o = arg0[0]; bails if 0.
 *   - o->0xC  = clamp01( (f32)((f64)arg5 / *(f64*)(&func_00000044+0x14)) )
 *   - o->0x4  = (f32)((f64)fabsf(arg4) + *(f64*)(&func_00000044+0x1C))
 *   - o->0x18 = (s8)(s32)(arg4 * 63.0f)            // 0x427C0000
 *   - o->0x1E = (arg8 & 0x80)   != 0
 *   - switch(arg6){0,'a','b','c','d'} -> o->0x15 / o->0x14 tags
 *   - o->0x0  = arg2 / 4.0f ;  o->0x8 = arg3 / 4.0f
 *   - o->0x20 = 1 ;  o->0x1C = (arg8 & 0x8000) != 0
 *   - if(o->0x1C) { func_00000000((int)arg1bits,&sp28,&sp24,arg7);
 *                   func_00000000(arg0[0],*(int*)&sp28,(s8)sp24); }
 *   - func_00000000(&D_00006584); func_00000000(arg0[0],0);
 *     func_00000000();
 * The &func_00000044+off ldc1 doubles are f64 consts embedded in
 * func_00000044's literal pool (same idiom as func_000024B8 line ~1794,
 * which emits the matching R_MIPS_HI16/LO16 func_00000044 relocs).
 * NM body now reproduces all relocs + the full call/switch/clamp shape
 * (100 residual diff words, down from 125). Residual classes — both
 * permuter-immune, hence INCLUDE_ASM (no episode):
 *   1) div.s-vs-mul.s: arg2/4.0f & arg3/4.0f. -O2 -r4300_mul ALWAYS
 *      reciprocal-muls a foldable /4.0f (lui 0x3e80; mul.s). Target
 *      keeps lui 0x4080; mtc1; div.s twice (no stack store) — only
 *      reachable from a NON-foldable 4.0 (verified: needs a branch-fed
 *      local or a global load; both add insns/stores the target lacks).
 *      Cannot be produced from honest /4.0f literal C.
 *   2) +16B frame / FP-register coloring cascade: &sp28/&sp24
 *      address-taken locals land 16B higher than target's sp+0x24/0x28
 *      slots, shifting arg-home + $f-register numbering throughout.
 * INCLUDE_ASM-preserved (.s = source of truth). */
#ifdef NON_MATCHING
extern char D_00006584;
/* Run 109 (agent-c, 93.18 -> 99.75): per-address pool doubles; arg5 is
 * reassigned with the scaled value (target keeps it in f2); ONE float local
 * carries both the clamp and the fabs results (f0); `/ (f32)4` keeps the
 * div.s (a 4.0f literal folds to * 0.25). Residual: frame 0x38 vs 0x30
 * (sp28/sp24 sit 8 bytes higher; pads only grow it). */
extern f64 D_bu_ro_d058, D_bu_ro_d060;  /* RoData pool doubles at 0x58 / 0x60 (func_00000044 + 0x14 / 0x1C); one symbol each, the target re-luis per load */
void func_00000610(int *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    int o;
    f32 sp28;
    int sp24;
    f32 var_f0;

    o = arg0[0];
    if (o != 0) {
        arg5 = (f32) ((f64) arg5 / D_bu_ro_d058);
        if (arg5 < 0.0f) {
            var_f0 = 0.0f;
        } else if (1.0f < arg5) {
            var_f0 = 1.0f;
        } else {
            var_f0 = arg5;
        }
        *(f32*)((char*)o + 0xC) = var_f0;
        if (arg4 < 0.0f) {
            var_f0 = -arg4;
        } else {
            var_f0 = arg4;
        }
        *(f32*)((char*)arg0[0] + 0x4) = (f32) ((f64) var_f0 + D_bu_ro_d060);
        *(s8*)((char*)arg0[0] + 0x18) = (s8) (s32) (arg4 * 63.0f);
        *(s8*)((char*)arg0[0] + 0x1E) = (s8) ((arg8 & 0x80) != 0);
        switch (arg6) {
        case 0x0:
            *(s8*)((char*)arg0[0] + 0x15) = 1;
            *(s8*)((char*)arg0[0] + 0x14) = 0;
            break;
        case 0x61:
            *(s8*)((char*)arg0[0] + 0x15) = 1;
            *(s8*)((char*)arg0[0] + 0x14) = 1;
            break;
        case 0x62:
            *(s8*)((char*)arg0[0] + 0x15) = 3;
            *(s8*)((char*)arg0[0] + 0x14) = 1;
            break;
        case 0x63:
            *(s8*)((char*)arg0[0] + 0x15) = 2;
            *(s8*)((char*)arg0[0] + 0x14) = 1;
            break;
        case 0x64:
            *(s8*)((char*)arg0[0] + 0x15) = 2;
            *(s8*)((char*)arg0[0] + 0x14) = 2;
            break;
        }
        *(f32*)((char*)arg0[0] + 0x0) = arg2 / (f32)4;
        *(f32*)((char*)arg0[0] + 0x8) = arg3 / (f32)4;
        *(s8*)((char*)arg0[0] + 0x20) = 1;
        *(u8*)((char*)arg0[0] + 0x1C) = (u8) ((arg8 & 0x8000) != 0);
        if (*(u8*)((char*)arg0[0] + 0x1C) != 0) {
            func_00000000(*(int*)&arg1, &sp28, &sp24, arg7);
            func_00000000(arg0[0], *(int*)&sp28, (s8) sp24);
        }
        func_00000000(&D_00006584);
        func_00000000(arg0[0], 0);
        func_00000000();
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/bootup_uso", func_00000610);
#endif

void func_0000085C(int *a0) {
    int a1 = *a0;
    if (a1 != 0) {
        func_00000000(a1);
    }
}

void func_00000888(int *a0) {
    int a1 = *a0;
    if (a1 != 0) {
        func_00000000(a1);
    }
}

void func_000008B4(int *a0) {
    func_00000000(*a0);
}

void func_000008D4(int *a0) {
    func_00000000(*a0);
}

void func_000008F4(int *a0, int a1_orig) {
    int ret = func_00000000(a0, a0[2]);
    int node = *a0;
    if (node != 0) {
        func_00000000(node, ret, 0, a1_orig);
    }
}

void func_00000940(int *a0, int a1_orig) {
    int ret = func_00000000(a0, a0[2]);
    int node = *a0;
    if (node != 0) {
        func_00000000(node, ret, 3, a1_orig);
    }
}

void func_0000098C(int *a0, int a1_orig) {
    int ret = func_00000000(a0, a0[2]);
    int node = *a0;
    if (node != 0) {
        func_00000000(node, ret, 1, a1_orig);
    }
}

void func_000009D8(int *a0, int a1_orig) {
    int ret = func_00000000(a0, a0[2]);
    int node = *a0;
    if (node != 0) {
        func_00000000(node, ret, 2, a1_orig);
    }
}

void func_00000A24(int *a0) {
    int a1 = *a0;
    if (a1 != 0) {
        func_00000000(a1);
    }
}

void func_00000A50(int **a0, int a1) {
    int *v0 = *a0;
    if (v0 != 0) {
        ((char*)v0)[0x1F] = (char)a1;
    }
}

void func_00000A68(int *a0) {
    int a1 = *a0;
    if (a1 != 0) {
        func_00000000(a1);
    }
}

void func_00000A94(int *a0, int a1) {
    a0[2] = a1;
}

/* NATURAL CEILING: 97.83% NM (register-pick cap). IDO emits
 * `addiu v0,zero,8` in the 'n' case's delay slot; target uses
 * `addiu v1,zero,8` then `or v0,v1,zero` at the shared return — pure
 * regalloc swap with the value flowing to the return register. 5+
 * variants tried (named local, register hint, volatile, goto-split,
 * ||-fusion); IDO's allocator always picks $v0 when the value flows to
 * v0 directly. The previous INSN_PATCH bridge (2 fixed offsets 0x30 +
 * 0x74) was REMOVED 2026-05-23 as match-faking.
 * 2026-06-11 wave-2 (10 more variants): the v1-staging + `or v0,v1` IS
 * C-producible — `return (a1 != 'n' && a1 != 's') ? 2 : 8;` stages the
 * else-arm 8 in $v1 exactly (select arms don't coalesce with the result
 * pseudo) — BUT the ternary places the or-block ADJACENT to its tests
 * (target has it LAST, after the label blocks) and triggers a t-case
 * delay-hoist; net no better than 2 diffs. goto-form + named r always
 * coalesces (placement sweep x5, dead-init, two-def phi, const-prop
 * interference r-6 all fold back). Needs select-staging + label-block
 * placement simultaneously; not reachable from one C shape found so far.
 * 2026-06-11 ugen-crack session: NEW LEVER got within 1 as1 decision.
 * `r = 8; if (n) goto L_B0C; if (s) goto L_B0C; ... L_B0C: return r | 0;`
 * — `| 0` SURVIVES cfe+uopt (`+ 0` folds) and emits the exact
 * `or v0,v1,zero` word, with li v1,8 landing in the n-beq delay = target
 * words at both diff sites. Remaining single blocker: as1's branch-delay
 * filler. Its priority (derived empirically, see docs/IDO_CODEGEN as1
 * entry): (1) fill from-before; (2) STEAL first insn of an EXCLUSIVE
 * target block (plain beq, vZ9 probe); (3) for a SHARED target block,
 * COPY first insn into a LIKELY slot (beq->beql) and retarget past it;
 * (4) nop. Our s-beq hits rule 3 (B0C shared by n+s, first insn = the
 * or) -> beql + 31 insns. The original's as1 left beq+nop, which only
 * happens when the shared block's first insn is non-hoistable (jr) —
 * i.e. its B0C reached as1 already as [jr][or-in-delay]. No C shape
 * reaches that (the or must carry B0C's label, so as1's own jr-fill
 * can't move it; duplicate-block forms leave a ghost twin, 32 insns).
 * = the documented "branch-likely vs regular branch" as1 cap class
 * (docs/IDO_CODEGEN branch-likely entry). Stays NM at 2 diffs. */
/* func_00000A9C — best NM body (28/30 words, 97.83%). CAP CONFIRMED 2026-07-02
 * (agent-e deep session, ~25 new C shapes + 4 phase-split configs + IDO 5.3 + -g1/2/3).
 *
 * The two residual words: target stages the n/s-case 8 in $v1
 * ([0xACC] addiu v1,zero,8 in the n-beq delay) and returns it via a
 * genuine MOVE in the jr delay ([0xB10] or v0,v1,zero); our build
 * coalesces to v0 (li v0,8 + nop).
 *
 * COMPLETE MECHANISM MAP (new this session — supersedes the old
 * "as1 branch-delay filler" theory in the previous comment):
 *  1. The beql conversion on the s-beq is NOT triggered when the s-beq's
 *     ORIGINAL ugen target is a label block that the n-beq EMPTIES by a
 *     shared-block steal (li v1,8 -> n-delay); the s-beq is then
 *     retargeted to the next block with its delay finalized as NOP.
 *     Reproduced exactly (t11 shape: n/s -> L_ELSE{r=8} falling into
 *     L_MERGE{return r}). Real ugen MOVEs at the retarget point do NOT
 *     re-trigger likely-copy; ALU `or` from the `|0` lever DOES.
 *  2. The or-in-jr-delay sink at B0C works (label moves onto jr) iff the
 *     merge block has ONLY conditional-branch preds; any unconditional-b
 *     pred pins it (t11 vs ternary/t13 A/B).
 *  3. The move v0,v1 survives uopt ONLY as a >=2-def web (select/phi with
 *     two distinct reaching values). Single-def const webs ALWAYS coalesce
 *     (probed: named r, dead-init, r=r+6 web-chaining, if(1)-dead-edge,
 *     a0==a0 edge, do-while-break, undef-path phi -> uopt spills to stack).
 *  4. A 2nd def costs an insn that must be DELETED for the 30-word layout.
 *     Only deleter is as1's b->return-block dup (ternary/t13/w1-w4), whose
 *     window is <=9 insns / <=5 blocks: probed w1-w4 ok (3..9 insns),
 *     w5/p10 fail (>=10-11). Target geometry needs the dup to span
 *     5 label blocks + ELSE = 11 insns. MECHANICALLY EXCLUDED.
 *     Routing the extra def through a label case (y3) gets uopt-threaded
 *     into bnel at the test site. Phase splits (uopt-O2+ugen/as1-O1,
 *     uopt-O1+ugen-O2), -g1/g2/g3, -Wb/-Wo overrides (driver -O2 wins),
 *     and IDO 5.3 all fail identically or worse ("Binasm file dictates -O"
 *     pins as1 to ugen's level; as1 -O2 on ugen -O1 binasm asserts).
 *  The closest shapes: ternary `return (a1!='n'&&a1!='s')?2:8` = all
 *  mechanism right, merge block placed 2nd instead of last (+t-steal);
 *  t11 goto-form = layout right, [16/17] b+li v1,2 vs jr+li v0,2 and
 *  or unsunk. No C shape reconciles both under IDO 5.3/7.1 -O2.
 *  Target's plain-beq+nop into a shared [jr][move] tail is UNIQUE in the
 *  entire ROM (full-asm scan) — likely an uopt pass-ordering state not
 *  reachable from any input we can construct. Stays NM at 2 word diffs.
 *  2026-07-15 (agent-g wave-3 re-check): return-capture precolor and
 *  void-alias FD0-exclusion CANNOT apply (leaf fn, no calls). Two new
 *  zero-cost 2-def-web spellings probed, both byte-identical to baseline
 *  (still li v0,8 + nop): `a1 = 8; return a1;` (live-incoming + 8 = real
 *  2-def web) and `a0 = 8; return a0;` (dead-incoming + 8). Param webs
 *  are dead-def-pruned/split before coloring, so the const def coalesces
 *  exactly like a fresh local. MECHANICALLY EXCLUDED verdict stands. */
/* EXACT via an IDO 7.1 NCC (C++) donor: src/bootup_uso/bootup_uso_ncc_A9C.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder (agent-c run 121). The old
 * goto chain returned constants directly; the target is a switch that assigns
 * one result variable (the 'n'/'s' value is hoisted into v1). */
int func_00000A9C(int a0, int a1) {
    volatile int r = 0;
    return r;
}

void func_00000B14(int **a0, int a1) {
    int *v0 = *a0;
    if (v0 != 0) {
        ((char*)v0)[0x1D] = (char)a1;
    }
}

extern float D_00000068;
void func_00000B2C(void **a0, float a1) {
    a1 = a1 / D_00000068;
    if (*a0 != 0) {
        *(float*)((char*)*a0 + 0x10) = a1;
    }
}

void func_00000B50(char *a0) {
    int scratch;
    func_00000008(&scratch);
    func_00000044((float*)(a0 + 0x10));
}

void func_00000B80(char *a0) {
    int scratch;
    func_00000008(&scratch);
    func_00000080(a0 + 0x10);
}

void func_00000BB0(char *a0) {
    int scratch;
    func_00000008(&scratch);
    func_000000F0(a0 + 0x10);
}

void func_00000BE0(char *a0) {
    int scratch;
    func_00000008(&scratch);
    func_00000008((int*)(a0 + 0x10));
}

/* func_00000C10: mirror of func_000046EC / func_00007B08 / func_0000E690.
 * BYTE-EXACT twin of donor eddproc_uso_func_000003BC, via the donor's clean C
 * form (p2/head/p1 decl order + char*-cast field access + p1[0x40/4] re-read).
 * Was once force-matched via 22-word INSN_PATCH (banned 2026-05-23, recipe
 * deprecated); this is the genuine C match. */
extern int func_00000000();
void *func_00000C10(int *arg0) {
    int *p2;
    int *head;
    int *p1;
    p1 = (int*)func_00000000(0x40);
    if (p1 != 0) {
        func_00000000(p1);
        *(int*)((char*)p1 + 0x28) = (int)&D_00000000;
        *(int*)((char*)p1 + 0x3C) = 0;
    }
    p2 = p1;
    p1 = arg0;
    head = (int*)p1[0x40 / 4];
    if ((int*)p1[0x40 / 4] != 0) {
        func_00000000((char*)p2 + 0x10, head);
        if (*(int*)((char*)head + 0x14) != 0) {
            *(int*)((char*)head + 0x4) = 1;
        }
        *(int*)((char*)head + 0x14) = (int)p2;
    }
    return p2;
}

/* func_00000CA0 - verified structural decode (0xF4, 61 insns,
 * get-or-create constructor + box-corner table generator).
 *   void *func_00000CA0(void *a0, int a1, int a2, int a3) {
 *       o = a0;
 *       if (o == 0) {
 *           o = alloc(0x40);                      // func_00000000
 *           if (o == 0) return 0;
 *           init(o, &D_000066A8);                 // func_00000000
 *       }
 *       o->0x28 = &D_desc;
 *       o->0x34 = 1.0f; o->0x30 = 1.0f;           // scale = (1,1,1)
 *       o->0x2C = 1.0f; o->0x38 = 0.0f;
 *       o->0xC  = a1;
 *       o->0x3C = a2;
 *       *(int*)&D_g0 = a3;
 *       *(int*)(func_00000000 + 4) = a3;          // 2nd global
 *       h  = (a3 >= 0) ? (a3 >> 1) : ((a3 + 1) >> 1);  // a3/2
 *       nh = -h;
 *       // global s16 table D_v: 4 verts, stride 0x10, lanes +0/+2/+4
 *       v[0]={ h, h, h};  v[1]={nh, h, h};
 *       v[2]={nh,nh, h};  v[3]={ h,nh, h};
 *       return o;
 *   }
 * Struct-typing reference: o = 0x40-byte transform object. o->0x28
 * (40) descriptor/vtable ptr (&D), o->0x2C/0x30/0x34 (44/48/52) f32
 * scale vector (init (1,1,1)), o->0x38 (56) f32 = 0.0, o->0xC (12)
 * s32 = a1, o->0x3C (60) s32 = a2. a3 = a half-extent: a3/2 (signed-
 * round) is written as a box/quad corner pattern into a global s16
 * vertex table (4 entries, 0x10 stride, x/y/z at +0/+2/+4 - all z =
 * +h, xy = the 4 (+-h) corners); a3 also stored to two global int
 * slots (&D and func_00000000+4). D_000066A8 = init datum.
 * 2026-06-20 reconstruction 61->79%: (1) control flow rewritten to
 * `if (a0 || (o=alloc())) { body }` + shared-epilogue `return o`
 * (was a separate `return 0` path); (2) added the missing
 * `*(func_00000000+4)=a3` store; (3) `a3/2` (IDO signed-div emit)
 * puts the half-extent in $a0 matching target (manual >>1 ternary used
 * $v0). RESIDUAL caps: descriptor-symbol reg, prologue move/sw-a2
 * scheduling, and the 2 single-int global stores want $at-fused form
 * but IDO holds D_00000000 in $v1 (shared with the 12-entry box table).
 * Stays INCLUDE_ASM (no episode). */
/* func_00000CA0: EXACT via an IDO 7.1 NCC (C++) donor,
 * src/bootup_uso/bootup_uso_ncc_CA0.cpp, REPLACE_FUNC_BODY-spliced over this
 * placeholder (agent-g run 150). It is the real constructor D::D(a1, a2, a3):
 * edg's this==0 -> new(0x40) protocol, out-of-line base ctor B(&D_000066A8)
 * (import), D's vptr at +0x28 (the stores now schedule as in the target), and
 * the last vertex corner written one store per source line (as1 reorders only
 * inside a .loc line group; the cfe body colored h/-h v0/a0, 97.2). */
void func_00000CA0(int a0, int a1, int a2, int a3) {
    volatile int r = 0;
}

/* func_00000D94 - verified structural decode (0xD4, 53 insns,
 * FP-threshold-gated state update).
 * Struct-typing reference: g = &D_00000000 global; g->0x2A0 (672) f32
 * value compared (promoted to f64) against the f64 constant at
 * func_00000044+0x2C; g->0x254 (596) ptr to a state object whose
 * +0x78 (120) is a flags word (bit 2 / mask 0x4 = an enable, set or
 * cleared by a0->0x3C in the below-threshold path, always set in the
 * at/above path) and +0xDC..+0xE8 (220..232) a 4x int param block
 * filled from func_0000027C+0x18; a0->0x3C (60) selector.
 * Caps <80: FP cvt.d.s + c.lt.d double-promote + ldc1 f64 const +
 * bc1f branch + 2x reloc + 2x cross-symbol data ref.
 * INCLUDE_ASM remains build path. */
extern void func_00000044();  /* used as data-symbol base */
extern void func_0000027C();  /* used as data-symbol base */
extern int D_00007324;
/* EXACT (agent-c run 115). Globals block import_0005C108 (alias bu_d94_g),
 * threshold = RoData f64 at +0x70 (func_00000044 + 0x2C), record at 0x294
 * (func_0000027C + 0x18, alias bu_d94_src); callees func_05B750(g, rec) and
 * func_04CCA8(a0). The a0->0x3C test goes through a named int (target v1),
 * the flag word through a pointer, and the 16-byte record is a struct copy. */
typedef struct { int w[4]; } S16_D94;
extern char bu_d94_g[], bu_d94_src[];
void func_00000D94(char *a0) {
    int *p;
    int on;
    if ((double)*(float *)(bu_d94_g + 0x2A0) < D_bu_ro_dpool[0x70 / 8]) {
        func_00000000(bu_d94_g, bu_d94_src);
        func_00000000(a0);
        p = (int *)(*(char **)(bu_d94_g + 0x254) + 0x78);
        on = *(int *)(a0 + 0x3C);
        if (on != 0) {
            *p |= 4;
        } else {
            *p &= ~4;
        }
    } else {
        p = (int *)(*(char **)(bu_d94_g + 0x254) + 0x78);
        *p |= 4;
        *(S16_D94 *)(*(char **)(bu_d94_g + 0x254) + 0xDC) = *(S16_D94 *)bu_d94_src;
    }
}

// func_00000E68 — FIRST STRUCTURAL PASS (0xBDC / 759 insns, multi-run target,
// no episode). Procedural tiled quad-mesh + index-buffer builder/constructor
// (terrain/water-surface grid for the bootup/title scene).
//
//   void *func_00000E68(void *self, Cfg *a1, void *owner2, void *owner3,
//                       arg5)  // arg5 @ sp+0x1E8; a1->s0 owner2->s1 owner3->s2
//     if (!self) { self = func_00000000(0x4C); if (!self) return 0; }  // alloc
//     func_00000000(self, &D_000066B0);          // register / base-init
//     self->0x28 = 0;
//     self->0x0C = &D_000066B8;                   // vtable/template
//     self->0x2C = owner2;  self->0x30 = owner3;  self->0x34 = arg5;
//     self->0x3C = self->0x40 = self->0x44 = self->0x48 = 1.0f;  // scale/uv
//     // cache a1->0x8, a1->0xC into locals (grid dims)
//     buf = func_00000000(0x800);                 // vertex scratch (sp+0x1CC)
//     func_00000000(buf, 0x800, 0);               // zero-init
//     // Phase 1: nested grid build — outer (dimA>>5) rows x inner (dimB>>6)
//     //   cols; per cell emit a tessellated quad as 0x10-stride short records
//     //   (pos x = colBase±0x20, y = rowBase±0x10; uv consts 0x7C0/0xFC0;
//     //   the ±/>>1 idioms are signed ceil-divide rounding).
//     // Phase 2: flatten into owner2->0xC index list (data @ +0x0,
//     //   count @ +0x4): 4 vert indices per quad, s0 steps by 4, outer s6
//     //   bounded by (dim>>6); appends s2(owner3) + per-vert payloads.
//     return self;
//   }
//
// Struct-typing reference:
//   self: 0x0C vtable(&D_000066B8), 0x28 fnptr=0, 0x2C/0x30 owner ptrs,
//         0x34 arg5, 0x3C/0x40/0x44/0x48 f32=1.0 (scale/uv defaults), size 0x4C.
//   owner2(s1)->0x0C = a {data,count} index-list handle.
//   D_000066B0 base-init descriptor; D_000066B8 instance vtable.
//   func_00000000 + 0x4 = a placeholder/folded data ref (same JAL-target-0
//   + folded-pool family as the func_0000098C/func_00000044 literal-pool bug,
//   docs/N64_FORENSICS.md#bootup-uso-fp-literal-pool-folded-into-func-0000098C).
// Caps (DEFERRED): 759-insn dual-phase builder, multi-run target —
//   byte-match deferred. Real-C STRUCTURAL body below — procedural
//   tiled quad-mesh + index-buffer builder skeleton. Name pre-checked:
//   no extern reuse.
#ifdef NON_MATCHING
/* PASS-9 2026-10-20 (agent-c run 114): FRESH HAND DECODE 73.5 -> 86.9.
 * The body is three phases of real libultra F3DEX GBI macros, written out
 * here as E68_* clones of the 2.0I gbi.h expansions (scaled-down _SHIFTL,
 * gDma1p, gSPCullDisplayList, gSP1Triangle, gDPSetTile, LoadTileGeneric):
 *   1. a vertex grid (dims.h/32 rows x dims.w/64 cols, 4 Vtx per cell);
 *   2. one sub-display-list per cell: CullDL + gDPLoadTextureTile (the
 *      2.0J form whose SetTileSize is (0,0,(lrs-uls)<<2,(lrt-ult)<<2): IDO
 *      reassociates that into the target's odd hoisted "63" and
 *      "(t1-t1)+31" spill slots and the held 71 = line before >>3) + two
 *      TRI1 + ENDDL, with the head pointer of each kept in arr[];
 *   3. gSPVertex + gSPDisplayList(arr[i*n+j]) per row, then ENDDL.
 * Levers: (1) the DL append is `(_n = m->dl->count++, &m->dl->buf[_n])`.
 * With a plain `&buf[count++]`, IDO loads buf before the count store.
 * The target re-reads m->dl after the store, which is what the comma
 * temp gives (57.9 -> 85.1). (2) The dims w and h reads go through
 * separate zero aliases (w = [0], h = [1], so the +4 stays in the lw).
 * (3) Phase 3 walks a separate vp pointer, not &buf[i*n*4]. Residual: k
 * (the arr[] index) is homed instead of held in s5, buf is held in s1
 * where the target spills it to 0x1CC, the frame is 0x10 short, and the
 * phase-2 vidx*2 web takes ra/s-regs where the target uses t5. Real
 * identities: dims = import_8001FFA0 {w, h}, mgr = *(import_8005C108 +
 * 0x18), vtx quad = bootup data sym163. */
extern char D_000066B0, D_000066B8;
extern char D_e68_vtx;  /* bootup.uso data sym163 (D_0065AC): 4-vertex quad */
typedef struct { u32 w0, w1; } GwE68;
typedef struct { GwE68 *buf; s32 count; } DlE68;
typedef struct { char pad[0xC]; DlE68 *dl; } MgrE68;
typedef struct { s16 x, y, z; u16 flag; s16 s, t; u8 cn[4]; } VtxE68;
extern s32 import_8001FFA0[];   /* screen/grid dims {w, h}: w reads */
extern s32 import_8001FFA0_h[]; /* same import, h reads ([1] = +4) */
extern int import_8005C108[];
#define E68_MGR (*(MgrE68 **)((char *)import_8005C108 + 0x18))
#define E68_NEXT(m) (_n = (m)->dl->count++, &(m)->dl->buf[_n])
#define E68_SHL(v, s, w) ((u32)(((u32)(v) & ((0x01 << (w)) - 1)) << (s)))
#define E68_W(pkt, a, b) { GwE68 *_g = (GwE68 *)(pkt); _g->w0 = (a); _g->w1 = (b); }
#define E68_TILEGEN(pkt, c, tile, uls, ult, lrs, lrt) \
    E68_W(pkt, E68_SHL(c, 24, 8) | E68_SHL(uls, 12, 12) | E68_SHL(ult, 0, 12), \
          E68_SHL(tile, 24, 3) | E68_SHL(lrs, 12, 12) | E68_SHL(lrt, 0, 12))
#define E68_SETTILE(pkt, fmt, siz, line, tmem, tile, pal, cmt, maskt, shiftt, cms, masks, shifts) \
    E68_W(pkt, E68_SHL(0xF5, 24, 8) | E68_SHL(fmt, 21, 3) | E68_SHL(siz, 19, 2) | E68_SHL(line, 9, 9) | E68_SHL(tmem, 0, 9), \
          E68_SHL(tile, 24, 3) | E68_SHL(pal, 20, 4) | E68_SHL(cmt, 18, 2) | E68_SHL(maskt, 14, 4) | \
          E68_SHL(shiftt, 10, 4) | E68_SHL(cms, 8, 2) | E68_SHL(masks, 4, 4) | E68_SHL(shifts, 0, 4))
#define E68_LOADTEXTILE(pkt, timg, fmt, siz, width, height, uls, ult, lrs, lrt, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ \
    E68_W(pkt, E68_SHL(0xFD, 24, 8) | E68_SHL(fmt, 21, 3) | E68_SHL(siz, 19, 2) | E68_SHL((width) - 1, 0, 12), (u32)(timg)); \
    E68_SETTILE(pkt, fmt, siz, (((((lrs) - (uls) + 1) * 1) + 7) >> 3), 0, 7, 0, cmt, maskt, shiftt, cms, masks, shifts); \
    E68_W(pkt, E68_SHL(0xE6, 24, 8), 0); \
    E68_TILEGEN(pkt, 0xF4, 7, (uls) << 2, (ult) << 2, (lrs) << 2, (lrt) << 2); \
    E68_W(pkt, E68_SHL(0xE7, 24, 8), 0); \
    E68_SETTILE(pkt, fmt, siz, (((((lrs) - (uls) + 1) * 1) + 7) >> 3), 0, 0, pal, cmt, maskt, shiftt, cms, masks, shifts); \
    E68_TILEGEN(pkt, 0xF2, 0, 0, 0, ((lrs) - (uls)) << 2, ((lrt) - (ult)) << 2); \
}
#define E68_DMA1P(pkt, c, s, l, p) E68_W(pkt, E68_SHL(c, 24, 8) | E68_SHL(p, 16, 8) | E68_SHL(l, 0, 16), (u32)(s))
#define E68_VTX(pkt, v, n, v0) E68_DMA1P(pkt, 0x04, v, ((n) << 10) | (sizeof(VtxE68) * (n) - 1), (v0) * 2)
#define E68_CULL(pkt, vs, ve) E68_W(pkt, E68_SHL(0xBE, 24, 8) | E68_SHL((vs) * 2, 0, 16), E68_SHL((ve) * 2, 0, 16))
#define E68_TRI1(pkt, a, b, c) E68_W(pkt, E68_SHL(0xBF, 24, 8), E68_SHL((a) * 2, 16, 8) | E68_SHL((b) * 2, 8, 8) | E68_SHL((c) * 2, 0, 8))
#define E68_END(pkt) E68_W(pkt, E68_SHL(0xB8, 24, 8), 0)
void *func_00000E68(char *self, char *cfg, s32 arg2, s32 arg3, s32 arg4) {
    u32 timg;
    u32 tlut;
    VtxE68 *buf;
    s32 row;
    s32 k2;
    volatile int padq[4]; /* run 138: frame = target */
    s32 col, x, y, vidx;
    s32 ult;
    GwE68 *arr[32];
    s32 k, uls;
    s32 i;
    GwE68 *head;
    s32 j, n;
    MgrE68 *m;
    VtxE68 *vp;
    s32 _n;

    if ((self != 0) || (self = (char *)func_00000000(0x4C)) != 0) {
        func_00000000(self, &D_000066B0);
        *(void **)(self + 0x28) = &D_00000000;
        *(void **)(self + 0xC) = &D_000066B8;
        timg = *(u32 *)(cfg + 0x8);
        tlut = *(u32 *)(cfg + 0xC);
        *(s32 *)(self + 0x2C) = arg2;
        *(s32 *)(self + 0x30) = arg3;
        *(s32 *)(self + 0x34) = arg4;
        *(f32 *)(self + 0x48) = 1.0f;
        *(f32 *)(self + 0x44) = 1.0f;
        *(f32 *)(self + 0x40) = 1.0f;
        *(f32 *)(self + 0x3C) = 1.0f;
        buf = (VtxE68 *)func_00000000(0x800, 16);
        func_00000000(buf, 0x800, 0);
        vidx = 0;
        y = import_8001FFA0_h[1] / 2 - 16;
        k = 0;
        for (row = 0; row < import_8001FFA0_h[1] / 32; row++) {
            x = import_8001FFA0[0] / 2 - 32;
            for (col = 0; col < import_8001FFA0[0] / 64; col++) {
                buf[vidx + 0].x = x + 32;
                buf[vidx + 0].y = y + 16;
                buf[vidx + 0].s = 0;
                buf[vidx + 0].z = import_8001FFA0[0] / 2;
                buf[vidx + 0].t = 0;
                buf[vidx + 1].x = x - 32;
                buf[vidx + 1].y = y + 16;
                buf[vidx + 1].s = 0xFC0;
                buf[vidx + 1].t = 0;
                buf[vidx + 1].z = import_8001FFA0[0] / 2;
                buf[vidx + 2].x = x - 32;
                buf[vidx + 2].y = y - 16;
                buf[vidx + 2].s = 0xFC0;
                buf[vidx + 2].t = 0x7C0;
                buf[vidx + 2].z = import_8001FFA0[0] / 2;
                buf[vidx + 3].x = x + 32;
                buf[vidx + 3].y = y - 16;
                buf[vidx + 3].s = 0;
                buf[vidx + 3].t = 0x7C0;
                buf[vidx + 3].z = import_8001FFA0[0] / 2;
                x -= 64;
                vidx += 4;
            }
            y -= 32;
        }
        m = E68_MGR;
        ult = 0;
        for (i = 0; i < import_8001FFA0_h[1] / 64; i++) {
            vidx = 0;
            for (k2 = 0; k2 != 2; k2++) {
                uls = 0;
                for (j = 0; j < import_8001FFA0[0] / 64; j++) {
                    E68_CULL(arr[k++] = E68_NEXT(m), vidx, vidx + 3);
                    E68_LOADTEXTILE(E68_NEXT(m), timg, 2, 1, import_8001FFA0[0], 0, uls, ult, uls + 63, ult + 31, 0, 0, 0, 6, 5, 0, 0);
                    E68_TRI1(E68_NEXT(m), vidx, vidx + 1, vidx + 2);
                    E68_TRI1(E68_NEXT(m), vidx, vidx + 2, vidx + 3);
                    E68_END(E68_NEXT(m));
                    vidx += 4;
                    uls += 64;
                }
                ult += 32;
            }
        }
        m = E68_MGR;
        head = E68_NEXT(m);
        E68_VTX(head, func_00000000(&D_e68_vtx), 4, 0);
        E68_W(E68_NEXT(m), 0xBE000000, 6);
        E68_W(E68_NEXT(m), 0xE7000000, 0);
        E68_W(E68_NEXT(m), 0xBA001402, 0);
        E68_W(E68_NEXT(m), 0xBB000001, 0x80008000);
        E68_W(E68_NEXT(m), 0xB6000000, 0x33205);
        E68_W(E68_NEXT(m), 0xB900031D, 0x0C084000);
        E68_W(E68_NEXT(m), 0xFC357E6A, 0x11FCF67B);
        E68_W(E68_NEXT(m), 0xBA000E02, 0x8000);
        E68_W(E68_NEXT(m), 0xFD100000, tlut);
        E68_W(E68_NEXT(m), 0xE8000000, 0);
        E68_W(E68_NEXT(m), 0xF5000100, 0x07000000);
        E68_W(E68_NEXT(m), 0xE6000000, 0);
        E68_W(E68_NEXT(m), 0xF0000000, 0x073FC000);
        E68_W(E68_NEXT(m), 0xE7000000, 0);
        vp = buf;
        n = import_8001FFA0[0] / 32;
        for (i = 0; i < import_8001FFA0_h[1] / 64; i++) {
            E68_VTX(E68_NEXT(m), func_00000000(vp), n * 4, 0);
            for (j = 0; j < n; j++) {
                E68_DMA1P(E68_NEXT(m), 0x06, arr[i * n + j], 0, 0);
            }
            vp += n * 4;
        }
        E68_END(E68_NEXT(m));
        *(GwE68 **)(self + 0x38) = head;
    }
    return self;
}
#else
INCLUDE_ASM("asm/nonmatchings/bootup_uso", func_00000E68);
#endif

/* func_00001A44 - verified structural decode (0x1CC, 115 insns,
 * FP transform/draw + DL list-append). Same family as
 * func_0000485C / func_000076F4 (global root vector scaled by the
 * Dc->0x128/0x12C/0x130 triple, reloc draw helper, command-list
 * append).
 *   void func_00001A44(Obj *a0) {
 *       if (*(int*)(func_00000188 + 0x3C) & 0x2) return;  // gate
 *       st  = (*(int*)&D_a)->0x254;
 *       n   = st->0x158;
 *       e   = n->0x3C;                            // entry
 *       base = e->0x0;  stride6 = e->0x4 << 6;
 *       root = *(void**)&D_root;
 *       v0  = root->0x70;
 *       f32 sx = v0->0xA0 * Dc->0x128;
 *       f32 sy = v0->0xA4 * Dc->0x12C;
 *       f32 sz = v0->0xA8 * Dc->0x130;
 *       reloc_draw(base + stride6,                // func_00000000
 *                  (f32)a0->0x2C, (f32)a0->0x30, (f32)a0->0x34,
 *                  1.0f, sx, sy, sz);             // stack args
 *       l = n->0xC;                                // command list
 *       i = l->0x4; l->0x4 = i + 1;                // bump count
 *       cmd = l->0x0 + i*8;                        // 8-byte entry
 *       cmd[0] = 0x01020040;                       // packed marker
 *       ... (entry payload writes follow)
 *   }
 * Struct-typing reference: global gate *(func_00000188+0x3C) bit 1
 * (0x2) = suppress/already-emitted. *(&D_a)->0x254 (596) -> state
 * st; st->0x158 (344) -> node n; n->0x3C (60) -> a renderable
 * entry e (e->0x0 base ptr, e->0x4 << 6 = byte offset); n->0xC
 * (12) -> a command list {0x0 buffer, 0x4 u32 count}, 8-byte
 * entries, first word = 0x01020040 marker. root = *(&D_root),
 * root->0x70 (112) -> transform obj, ->0xA0/0xA4/0xA8 (160/164/
 * 168) f32 Vec3 scaled per-component by Dc->0x128/0x12C/0x130
 * (296/300/304) (the func_0000485C scale triple). a0->0x2C/0x30/
 * 0x34 (44/48/52) s32 -> converted to f32 draw params; 1.0f
 * const. Caps <80: FP-heavy cvt.s.w/mul.s + global gate + 2x &D
 * + reloc draw + list-append (count bump + 8-byte stride).
 * INCLUDE_ASM remains build path.
 *
 * 2026-06-02 (13.5->17.2%): the reloc_draw call passes 7 single-floats — routed
 * through a typed-float proto (func_1a44_emit, 0x0-alias) so they emit swc1 to
 * the outgoing stack-arg slots instead of K&R cvt.d.s+sdc1 double-promotion.
 * Remaining ~49 insns (the "entry payload writes" after cmd[0]) are still
 * undecoded — body is 66/115 insns. Next pass: decode the 8-byte-entry payload
 * stores + any trailing FP from the target asm. */
extern void func_00000188();  /* used as data-symbol base */
#ifdef NON_MATCHING
/* PASS-3 2026-10-20 (agent-c run 114): fresh decode against the target
 * words and the bootup.uso reloc table. It is a gSPMatrix + gSPDisplayList
 * draw: gate = import_8005C108[0x3C] bit 1; gfx ctx = G->0x254->0x158; the
 * matrix stack ctx->0x3C = {Mtx *buf, count} (64-byte Mtx). The 7-float
 * builder is a typed-float proto (floats go in a1..a3 + stack slots). w1
 * of the G_MTX word = osVirtualToPhysical of the same &buf[count], then
 * count++. DL appends use the comma-temp reload macro (see E68). The
 * camera pointer must stay inline (a named local costs a frame slot and
 * re-colors the a0 reloads: 81.7 -> 91.4 when inlined). */
extern int import_8005C108[];
extern int import_8005C108_g1a44[];   /* gate read alias (same import) */
extern char *D_bu_205CC;              /* bootup data sym161: camera root */
extern void func_1a44_emit(void *, float, float, float, float, float, float, float);
extern int func_1a44_emit2(void *);
typedef struct { u32 w0, w1; } Gw1A44;
typedef struct { Gw1A44 *buf; s32 count; } Dl1A44;
typedef struct { char m[64]; } Mtx1A44;
typedef struct { Mtx1A44 *buf; s32 count; } Ms1A44;
#define NEXT1A44(d) (_n = (d)->count++, &(d)->buf[_n])
#define W1A44(pkt, a, b) { Gw1A44 *_g = (Gw1A44 *)(pkt); _g->w0 = (a); _g->w1 = (b); }
void func_00001A44(char *a0) {
    char *g;
    char *gc;
    s32 _n;

    if (import_8005C108_g1a44[0x3C / 4] & 2) {
        return;
    }
    g = (char *)import_8005C108;
    gc = *(char **)(*(char **)(g + 0x254) + 0x158);
    func_1a44_emit(&(*(Ms1A44 **)(gc + 0x3C))->buf[(*(Ms1A44 **)(gc + 0x3C))->count],
                   (f32)*(s32 *)(a0 + 0x2C), (f32)*(s32 *)(a0 + 0x30), (f32)*(s32 *)(a0 + 0x34),
                   1.0f,
                   *(f32 *)(*(char **)(D_bu_205CC + 0x70) + 0xA0) * *(f32 *)(g + 0x128),
                   *(f32 *)(*(char **)(D_bu_205CC + 0x70) + 0xA4) * *(f32 *)(g + 0x12C),
                   *(f32 *)(*(char **)(D_bu_205CC + 0x70) + 0xA8) * *(f32 *)(g + 0x130));
    W1A44(NEXT1A44(*(Dl1A44 **)(gc + 0xC)), 0x01020040,
          func_1a44_emit2(&(*(Ms1A44 **)(gc + 0x3C))->buf[(*(Ms1A44 **)(gc + 0x3C))->count]));
    _n = (*(Ms1A44 **)(gc + 0x3C))->count;
    (*(Ms1A44 **)(gc + 0x3C))->count = _n + 1;
    W1A44(NEXT1A44(*(Dl1A44 **)(gc + 0xC)), 0x06000000, *(u32 *)(a0 + 0x38)); if (g) {}
    W1A44(NEXT1A44(*(Dl1A44 **)(gc + 0xC)), 0xE7000000, 0);
    W1A44(NEXT1A44(*(Dl1A44 **)(gc + 0xC)), 0xBA000E02, 0);
}
#else
INCLUDE_ASM("asm/nonmatchings/bootup_uso", func_00001A44);
#endif

void func_00001C10(int *dst) {
    int buf[2];
    func_00000000(&D_00000000, buf, 4);
    *dst = buf[0];
}

void func_00001C4C(float *dst) {
    float buf[2];
    func_00000000(&D_00000000, buf, 4);
    *dst = buf[0];
}

extern void func_00001C88();
void func_00001C88(Vec3 *dst) {
    int pad_top[1];
    Tri3i raw;
    int pad_mid[2];
    Tri3i tmp;
    int pad_bot[2];
    func_00000000(&D_00000000, &raw, 12);
    tmp = raw;
    dst->x = *(float*)&tmp.a;
    dst->y = *(float*)&tmp.b;
    dst->z = *(float*)&tmp.c;
}

void func_00001CF8(int *dst) {
    int buf[2];
    func_00000000(&D_00000000, buf, 4);
    *dst = buf[0];
}

void func_00001D34(Quad4 *dst) {
    Quad4 buf;
    func_00000000(&D_00000000, &buf, 16);
    *dst = buf;
}

void *func_00001D8C(void *a0) {
    if (a0 == 0) {
        a0 = (void*)func_00000000(0x48);
    }
    return a0;
}

void *func_00001DB8(void *a0, int a1, int a2) {
    if (a0 == 0) {
        a0 = (void*)func_00000000(0x48);
        if (a0 == 0) goto end;
    }
    func_00000000(a0, a1);
    *(int*)((char*)a0 + 0x40) = a2;
end:
    return a0;
}

/* func_00001E08 - verified structural decode (0x170, 92 insns,
 * get-or-create constructor + bulk property registration).
 * (reg = func_00000000(obj, &descriptor, 0).) Struct-typing
 * reference: o = 0x118-byte object. o->0x28 (40) descriptor ptr
 * (&D), o->0xC (12) type/vtable ptr (&D_00006E38), o->0x70 (112) =
 * 0, o->0x78 (120) = 0, o->0x74 (116) = &o[0x2C] (an embedded
 * sub-struct self-pointer; o+0x2C is init'd by the second reloc
 * call). D_00006E48..D_00006EC4 (and the .L00006EA8 entry) = ~10
 * property/field descriptor data blobs registered into o via the
 * reg helper (a property-table builder; arg3 = 0 default).
 * EXACT 2026-10-16 agent-c (run 97): arg3 is NOT 0 but a per-site
 * base-0 address (lui/addiu 0 each; one shared name would CSE into an
 * s-reg) -> D_1E08_a..j, o->0x28 = D_1E08_d, all pinned 0; the param is
 * reused as `o` (prologue or s0 / bnez / sw ra delay, as in the exact
 * 8F18 sibling) and an unused `pad` local gives frame 0x30 + the 0x24
 * spill slot for `sub`; .L00006EA8 -> D_00006EA8 pinned to 0x6EA8. */
extern char D_00006E30, D_00006E38;
extern char D_00006E48, D_00006E54, D_00006E60, D_00006E70, D_00006E80;
extern char D_00006E8C, D_00006E98, D_00006EA8, D_00006EB8, D_00006EC4;
/* per-site base-0 placeholders (target lui 0 / addiu 0 each, no CSE) */
extern char D_1E08_d, D_1E08_a, D_1E08_b, D_1E08_c, D_1E08_e, D_1E08_f;
extern char D_1E08_g, D_1E08_h, D_1E08_i, D_1E08_j;
void *func_00001E08(char *o) {
    char *pad;
    char *sub;
    if (o == 0) {
        o = (char*)func_00000000(0x118);
        if (o == 0) goto end;
    }
    func_00000000(o, &D_00006E30);
    *(void**)(o + 0x28) = &D_1E08_d;
    sub = o + 0x2C;
    func_00000000(sub);
    *(void**)(o + 0xC) = &D_00006E38;
    *(int*)(o + 0x70) = 0;
    *(int*)(o + 0x78) = 0;
    *(char**)(o + 0x74) = sub;
    func_00000000(o, &D_00006E48, &D_1E08_a);
    func_00000000(o, &D_00006E54, &D_1E08_b);
    func_00000000(o, &D_00006E60, &D_1E08_c);
    func_00000000(o, &D_00006E70, &D_1E08_d);
    func_00000000(o, &D_00006E80, &D_1E08_e);
    func_00000000(o, &D_00006E8C, &D_1E08_f);
    func_00000000(o, &D_00006E98, &D_1E08_g);
    func_00000000(o, &D_00006EA8, &D_1E08_h);
    func_00000000(o, &D_00006EB8, &D_1E08_i);
    func_00000000(o, &D_00006EC4, &D_1E08_j);
end:
    return o;
}

// Linked-list insert-head: alloc node via func_00000000(0), splice after
// obj->0x74 (head), advance head, bump obj->0x78 count. Body byte-matches at
// 98%; the sole diff is IDO -O2 saving obj across the jal in $a1 while the
// target uses $a3 — documented-unflippable from C, see
// docs/IDO_CODEGEN.md#feedback-ido-arg-save-reg-pick (this func is that
// entry's canonical example). Permuter-only; no episode (tautology-trap rule).
void func_00001F78(void *obj, int d1, int d2) {
    void *n;
    n = func_00000000(0, d1, d2, obj);
    *(void **)((char *)n + 0x44) = *(void **)((char *)(*(void **)((char *)obj + 0x74)) + 0x44);
    *(void **)((char *)(*(void **)((char *)obj + 0x74)) + 0x44) = n;
    *(void **)((char *)obj + 0x74) = n;
    *(int *)((char *)obj + 0x78) += 1;
}

void *func_00001FC8(char *a0, int a1) {
    void *p = *(void**)(a0 + 0x70);
    int i;
    for (i = 0; i < a1; i++) {
        p = *(void**)((char*)p + 0x44);
    }
    return p;
}

void *func_00002014(char *a0, int a1) {
    void *p = *(void**)(a0 + 0x70);
    int i;
    for (i = 0; i < a1; i++) {
        p = *(void**)((char*)p + 0x44);
    }
    return p;
}

void func_00002060(char *a0, int a1, int a2, int a3, int a4, int a5) {
    *(int*)(a0 + 0x80) = a1;
    *(int*)(a0 + 0x84) = a2;
    *(int*)(a0 + 0x88) = a4;
    *(int*)(a0 + 0x7C) = a5;
    *(int*)(a0 + 0x8C) = a3;
}

void func_00002080(int *a0) { *(int*)((char*)a0 + 0x104) = 0; }

/* append to count+entries list at 0x104 (count) + 0x108 (4-byte entries).
 * MATCHED 2026-06-21: the long-standing `addu`-before-`sw count` cap was an
 * as1 SCHEDULER TIE keyed on per-statement debug line numbers. Collapsing the
 * count-store and the entry-store onto a SINGLE source line lets IDO's as1
 * scheduler interleave them by data-readiness (it hoists `addu t8,a0,t7` so
 * t8 is ready for the delay-slot `sw a1,0x108(t8)`), producing the target's
 * addu-before-sw order. Same-line lever (cf. func_00005F34). Sibling
 * func_000020AC cracked identically. */
void func_00002088(char *a0, int a1) {
    int idx = *(int *)(a0 + 0x104);
    *(int *)(a0 + 0x104) = idx + 1; *(int *)(a0 + idx * 4 + 0x108) = a1;
}

void func_000020A4(int *a0) { *(int*)((char*)a0 + 0xC0) = 0; }

/* append-pair to a count+entries list at offset 0xC0 (count) + 0xC4 (pairs
 * of 8). MATCHED 2026-06-21: same as1 SCHEDULER TIE as func_00002088 — the
 * `sw t9,0xC0(a0)`-before-`addu t1,a0,t0` cap was a per-statement-line
 * tie-break. Collapsing the count-store and entry-store onto one source line
 * lets as1 hoist the addu (t1 needed for the delay-slot store), matching the
 * target's addu-before-sw order. */
void func_000020AC(int *a0, int a1, int a2) {
    int idx;
    *(int*)((char*)a0 + *(int*)((char*)a0 + 0xC0) * 8 + 0xC8) = a2;
    idx = *(int*)((char*)a0 + 0xC0);
    *(int*)((char*)a0 + 0xC0) = idx + 1; *(int*)((char*)a0 + idx * 8 + 0xC4) = a1;
}

void func_000020D8(char *a0, int a1) {
    int i;
    char *p;
    if (*(int*)(a0 + 0xC0) > 0) {
        i = 0;
        p = a0;
        do {
            func_00000000(a1, *(int*)(p + 0xC4), *(int*)(p + 0xC8));
            i++;
            p += 8;
        } while (i < *(int*)(a0 + 0xC0));
    }
}

void func_0000214C(int a0) {
}

void func_00002154(char *a0) {
    int scratch;
    func_00001C10(&scratch);
    func_00001C4C((float*)(a0 + 0x10));
}

void func_00002184(char *a0) {
    int scratch;
    func_00001C10(&scratch);
    func_00001C88(a0 + 0x10);
}

void func_000021B4(char *a0) {
    int scratch;
    func_00001C10(&scratch);
    func_00001CF8((int*)(a0 + 0x10));
}

void func_000021E4(char *a0) {
    int scratch;
    func_00001C10(&scratch);
    func_00001D34((Quad4*)(a0 + 0x10));
}

void func_00002214(char *a0) {
    int scratch;
    func_00001C10(&scratch);
    func_00001C10((int*)(a0 + 0x10));
}

/* func_00002244: byte-identical twin of func_0000EBE8 (alloc-or-fail +
 * linked-list-prepend). EXACT 2026-07-15 (82.4% -> 36/36, ROM byte-exact)
 * via the EBE8 recipe: ret=p + destructive p+=4 in-place advance (2nd def
 * blocks copyprop, p colors $a0), condition-tests-expression with the
 * assignment CSE'd inside the branch ($v0 carrier, plain beqz + or a1,v0
 * delay), reverse-decl-order spill slots (decls ret,target,p ->
 * p 0x1C / target 0x20 / ret 0x24). 3 trailing alignment nops restored via
 * the standalone GLOBAL_ASM orphan block below (SUFFIX injection is unsafe
 * this early in the file — see the orphan .s header). Retires the old
 * volatile-int-pp NM ceiling verdict. */
extern int func_00000000();
int *func_00002244(int *caller_a0) {
    int *ret;
    int *target;
    int *p;
    if ((p = (int*)func_00000000(0x40)) != 0) {
        func_00000000(p);
        p[0x28/4] = (int)&D_00000000;
        p[0x3C/4] = 0;
    }
    ret = p;
    p += 0x10/4;
    if (caller_a0[0x40/4] != 0) {
        target = (int*)caller_a0[0x40/4];
        func_00000000(p, target);
        if (target[0x14/4] != 0) {
            target[0x4/4] = 1;
        }
        target[0x14/4] = (int)ret;
    }
    return ret;
}

#pragma GLOBAL_ASM("asm/nonmatchings/bootup_uso/bootup_orphan_000022D4.s")

void func_000022E0(int *dst) {
    int buf[2];
    func_00000000(&D_00000000, buf, 4);
    *dst = buf[0];
}

void func_0000231C(float *dst) {
    float buf[2];
    func_00000000(&D_00000000, buf, 4);
    *dst = buf[0];
}

extern void func_00002358();
void func_00002358(Vec3 *dst) {
    int pad_top[1];
    Tri3i raw;
    int pad_mid[2];
    Tri3i tmp;
    int pad_bot[2];
    func_00000000(&D_00000000, &raw, 12);
    tmp = raw;
    dst->x = *(float*)&tmp.a;
    dst->y = *(float*)&tmp.b;
    dst->z = *(float*)&tmp.c;
}

void func_000023C8(Quad4 *dst) {
    Quad4 buf;
    func_00000000(&D_00000000, &buf, 16);
    *dst = buf;
}

/* MATCHED 2026-06-21: the s4/s5-store schedule swap is C-reachable after all.
 * The target emits the s4 store (sp-loaded) then the s5 store BEFORE the four
 * zero/const stores; the prior C ordered s5 LAST (after the zeros), and IDO's
 * list scheduler then deferred the s4 store past the cheap zero-stores. Moving
 * the `a0->0x34 = s5;` statement up to be adjacent to `a0->0x30 = s4;` pins
 * both sp-loaded stores together ahead of the zero/const block, matching the
 * target schedule exactly (0 non-reloc diffs vs expected; reloc symbol set =
 * func_00000000 x2 + D_0000731C HI/LO; the a0+0x28 store's `&D_00000000`
 * materialises as `lui 0; addiu 0` post-link, byte-identical to the target's
 * reloc-stripped `lui (0x0>>16); addiu 0x0`). */
extern char D_0000731C;
int *func_00002420(int *a0, int a1, int a2, int a3, int s4, int s5) {
    if (a0 == 0) {
        a0 = (int*)func_00000000(0x50);
        if (a0 == 0) goto end;
    }
    func_00000000(a0, &D_0000731C);
    *(int*)((char*)a0 + 0x28) = (int)&D_00000000;
    *(int*)((char*)a0 + 0x38) = a1;
    *(int*)((char*)a0 + 0x40) = a2;
    *(int*)((char*)a0 + 0x2C) = a3;
    *(int*)((char*)a0 + 0x30) = s4;
    *(int*)((char*)a0 + 0x34) = s5;
    *(int*)((char*)a0 + 0x3C) = 0;
    *(int*)((char*)a0 + 0x44) = 0;
    *(int*)((char*)a0 + 0x48) = 0x2710;
    *(int*)((char*)a0 + 0x4C) = 0;
end:
    return a0;
}

// func_000024B8(obj): per-frame state/animation tick for a bootup_uso scene object.
// Sibling of the obj->0x28 vtable-dispatch family (the 0x6C/0x68 jalr idiom appears 3x,
// same shape as func_0000485C draw-delegate path).
//
// Pseudocode:
//   cfg = obj->0x38;  world = obj->0x2C;  cam = world->0x70;
//   active = obj->0x4C;
//   if (READ32(func_00000044 + 0x20) == 1) {
//       active = func_00000000((cfg->0x10 & 0x400) != 0, (cfg->0x10 & 0x1000) != 0,
//                              (s8)cfg->0x8, (s8)cfg->0x9);   // result via sp+0x4C slot
//   }
//   if (cfg->0x18 & 0x40) { obj->0x4C = (obj->0x4C == 0); t0 = 1; }   // edge-toggle latch
//   if (obj->0x4C) {
//       if (t0) {
//           func_00000000(obj, 0);
//           d = obj->0x30->0x28;  (d->0x6C)(d->0x68 + func_00000000(obj,3,0));  // vtable
//           cam->0x14C = 85.0f;                                                 // activate
//       }
//       if (cfg->0x10 & 0x8  && cam->0x14C < 120.0f) cam->0x14C += 2.0f;         // ramp up
//       if (cfg->0x10 & 0x4  && cam->0x14C > 45.0f)   cam->0x14C -= 2.0f;        // ramp dn
//       cam->0x134 += cfg->0x4 * 2.0f;
//       cam->0x138 += cfg->0x0 * 2.0f;
//   } else {
//       if (t0) obj->0x48 = 10000;                          // 0x2710 retry timeout
//       if (++obj->0x48 >= 0x3C) {                           // 60-frame poll gate
//           obj->0x48 = obj->0x48 + 1;
//           p = func_00000000(READ32(0), D_00007324, obj->0x40 + 0xB4, 0);
//           if (p && p != obj->0x44) {
//               k = p->0x3C;
//               sel = (k >= 3 && k < 0x13) ? obj->0x30 : obj->0x34;  // in-range vs default
//               func_00000000(obj, (k in-range ? 0 : ...), k);
//               d = sel->0x28;  (d->0x6C)(d->0x68 + func_00000000(obj,k,p));     // vtable
//               obj->0x48 = 0;  obj->0x44 = p;
//           }
//       }
//   }
//
// Struct-typing reference (obj = arg0):
//   0x2C world ptr (->0x70 cam: FP 0x134 pan, 0x138 tilt, 0x14C fov-like, slewed +-2.0,
//        clamp [45.0,120.0], reset 85.0); 0x30/0x34 delegate objs (->0x28 vtable;
//        vtable->0x6C fnptr, vtable->0x68 s16 base-id; dispatch a0 = base + retval);
//        0x38 cfg (0x0/0x4 float rates, 0x8/0x9 s8 params, 0x10 flag-bits 0x4/0x8/
//        0x400/0x1000, 0x18 flag 0x40 latch-enable); 0x40 id (poll key + 0xB4),
//        0x44 cached poll result, 0x48 frame/timeout counter, 0x4C active latch.
//   func_00000044+0x20 == 1 = global-mode gate;  D_00007324 = poll-table word arg.
// NM body now structurally faithful: gate read folds via FW(&func_00000044,0x20)
//   HI16/LO16 reloc; 0x68 delegate base is s16 (lh, FH macro); cam->0x14C activate
//   is a FLOAT store (FF, was wrongly FW=int 85); D_00007324 is the poll-table
//   global (extern int); 2.0f hoisted to `two` local so cfg->0x4/0x0 emit mul.s
//   (not strength-reduced x+x); cam field updates via &cam->field pointer temps.
// Reg-agnostic similarity ~82% (166/175 insns). RESIDUAL CAP = register coloring +
//   frame-size: target saves TWO callee regs (s0,s1) for arg0+held-temp across the
//   4 dispatcher calls; IDO here colors arg0 into a single saved reg (frame 0x40 vs
//   target 0x50). Permuter 3205->2810 plateau (only 0x14C-hoist nudge, no crack) —
//   classic coloring/two-saved-reg barrier. Genuine type/value/CSE bugs all fixed.
// Full body INCLUDE_ASM-preserved (.s = source of truth). INCLUDE_ASM (no episode; tautology-trap rule).
#ifdef NON_MATCHING
/* 2026-10-16 agent-c rewrite: gate read through D_00000064 (pinned 0x64 ==
 * func_00000044+0x20, folds into the lw like the target); the two cfg->0x10
 * reads stay separate because the second is read as u32 (one type CSEs them);
 * D_00007324 is passed as a 4-byte struct by value (`tb.w = D; f(.., tb, ..)`
 * gives the target `sw a1,56(sp); sw a1,4(sp)`); ONE reused `s0` pointer var
 * for cfg / vtable desc / FP field pointers (the target keeps it in s0 and
 * does not fold `&cam->f` into the lwc1/swc1). Residual: that var colours a3
 * here (target s0, second saved reg) and the FP pointers fold; the `r`
 * latch slot lands at sp+0x30 (target 0x4C). Decl order p/pad/tb/r/t0/k
 * puts p, tb and the k spill at the target 0x40/0x38/0x2C. 80.6 -> 90.0. */
#ifndef FW
#define FW(p, o) (*(int *)((char *)(p) + (o)))
#endif
#ifndef FF
#define FF(p, o) (*(f32 *)((char *)(p) + (o)))
#endif
#ifndef FH
#define FH(p, o) (*(s16 *)((char *)(p) + (o)))
#endif
extern int D_00000064;
extern int D_24B8_tbl;
typedef struct { int w; } W24B8;
typedef void (*VF_24B8)();
void func_000024B8(char *arg0) {
    char *s0;
    char *v0;
    char *v1;
    char *p;
    volatile int pad;
    W24B8 tb;
    int r;
    int t0;
    int k;

    t0 = 0;
    if (D_00000064 == 1) {
        v0 = (char *)FW(arg0, 0x38);
        r = 0;
        func_00000000((FW(v0, 0x10) & 0x400) != 0, (*(u32 *)(v0 + 0x10) & 0x1000) != 0, (s8)v0[8], (s8)v0[9]);
        t0 = r;
    }
    s0 = (char *)FW(arg0, 0x38);
    if (FW(s0, 0x18) & 0x40) {
        t0 = 1;
        FW(arg0, 0x4C) = (FW(arg0, 0x4C) != 0) == 0;
    }
    if (FW(arg0, 0x4C) != 0) {
        f32 two = 2.0f;
        if (t0 != 0) {
            func_00000000(arg0, 0);
            v0 = (char *)FW(arg0, 0x30);
            s0 = (char *)FW(v0, 0x28);
            ((VF_24B8)FW(s0, 0x6C))(FH(s0, 0x68) + v0, 3, 0);
            FF(FW(FW(arg0, 0x2C), 0x70), 0x14C) = 85.0f;
        }
        s0 = (char *)FW(arg0, 0x38); if (arg0) {}
        if (FW(s0, 0x10) & 8) {
            v1 = (char *)FW(FW(arg0, 0x2C), 0x70);
            if (FF(v1, 0x14C) < 120.0f) {
                s0 = v1 + 0x14C; if (1) {}
                *(f32 *)s0 = *(f32 *)s0 + two;
            }
        }
        s0 = (char *)FW(arg0, 0x38);
        if (FW(s0, 0x10) & 4) {
            v1 = (char *)FW(FW(arg0, 0x2C), 0x70);
            if (FF(v1, 0x14C) > 45.0f) {
                s0 = v1 + 0x14C; if (1) {}
                *(f32 *)s0 = *(f32 *)s0 - two;
            }
        }
        v1 = (char *)FW(FW(arg0, 0x2C), 0x70);
        s0 = v1 + 0x134; if (1) {}
        *(f32 *)s0 = *(f32 *)s0 + FF(FW(arg0, 0x38), 4) * two;
        s0 = (char *)FW(FW(arg0, 0x2C), 0x70);
        s0 += 0x138;
        *(f32 *)s0 = *(f32 *)s0 + FF(FW(arg0, 0x38), 0) * two;
        return;
    }
    if (t0 != 0) {
        FW(arg0, 0x48) = 10000;
    }
    s0 = (char *)FW(arg0, 0x48);
    FW(arg0, 0x48) = (int)s0 + 1;
    if ((int)s0 >= 0x3C) {
        tb.w = D_00007324;
        p = (char *)func_00000000(D_24B8_tbl, tb, FW(arg0, 0x40) + 0xB4, 0);
        if (p != 0 && (int)p != FW(arg0, 0x44)) {
            k = FW(p, 0x3C);
            if ((u32)k >= 3 && (u32)k < 0x13) {
                func_00000000(arg0, 0);
                v0 = (char *)FW(arg0, 0x30);
                s0 = (char *)FW(v0, 0x28);
                ((VF_24B8)FW(s0, 0x6C))(FH(s0, 0x68) + v0, k, p);
            } else {
                func_00000000(arg0, 1);
                v0 = (char *)FW(arg0, 0x34);
                s0 = (char *)FW(v0, 0x28);
                ((VF_24B8)FW(s0, 0x6C))(FH(s0, 0x68) + v0, k, p);
            }
            FW(arg0, 0x48) = 0; if (p) {}
            FW(arg0, 0x44) = (int)p;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/bootup_uso", func_000024B8);
#endif

void func_00002774(char *a0, int a1) {
    if (*(int*)(a0 + 0x18) & 8) {
        char *p;
        *(int*)(a0 + 0x3C) = a1;
        *(int*)(*(char**)(a0 + 0x2C) + 0x70) = ((int*)a0)[a1 + 12];
        p = *(char**)(*(char**)(a0 + 0x2C) + 0x70) + 0x134;
        *(float*)(p + 8) = 0.0f;
        *(float*)(p + 4) = 0.0f;
        *(float*)p = 0.0f;
    }
}

void func_000027C0(char *a0, int *a1) {
    int msg_type = *a1;
    int *flags = (int*)(a0 + 0x18);
    if (msg_type == 0x3ED) {
        *flags &= ~0x8;
    }
}

// func_000027E8 — RECONSTRUCTED 95.21% fuzzy (0x4AC / 299 insns, no episode).
// 2026-06-22 (agent-b): cracked the +8 frame inflation (now -0x50 = target) by
// dropping a dead `base` local + reordering the `g->0x14=s4C` store before the
// trailing call (lands the store in the call's DELAY SLOT, killing a redundant
// g spill/reload pair); fixed the handle-slot coloring (s3C/s4C/s48/s44/s40 now
// land at 0x3C/0x4C/0x48/0x44/0x40 exactly) by declaring s3C LAST in the ptr
// list, and g LAST overall (g spill now at 0x34 = target). Function is now
// BYTE-IDENTICAL through the first ~73 insns; length matches (299).
// REMAINING RESIDUAL (~5%, coloring/addressing, NOT structural):
//   (1) the `*D_0 = g` singleton store: target emits the 3-insn HELD-$v1 form
//       (lui v1; addiu v1; sw v0,0(v1)); IDO emits the 2-insn la-macro
//       (lui at; sw v0,0(at)) for a direct symbol store. `*(volatile int*)`
//       DOES force the 3-insn held form but mis-colors it ($t9 not $v1) and
//       shifts downstream regs (nets -0.5%); plain la-macro scores higher.
//   (2) the +0x98 store base: target loads scene->0x134 right before the
//       m-call and stores in the delay slot (no spill); our C spills the base
//       across the call (+1 sw/lw in the ratio region). Neither nested-deref
//       nor a held local reproduces IDO's pre-call late-load schedule.
//   (3) `sv` spill slot 0x30 vs target 0x38 + two as1-scheduler ties (a
//       0x4F800000 `lui` position swap; a trailing `jal` swap) — register-
//       numerical / scheduler-tie class, no C-level lever found.
// These are the documented coloring/addressing-mode cap (docs/IDO_CODEGEN.md
// line ~126); permuter-class. Earlier note (was 93.97%):
// UI/scene setup constructor for the bootup/title screen. Body below is the
// true C structure (verified vs expected .o): correct call sequence, all 3
// u32->float ratio blocks emit byte-aligned (denom loaded via D_00000000+0x20C
// object-fold so the +0x10 folds into the lw; per-block `fd` temp forces the
// denominator to be evaluated FIRST, matching IDO's divisor-first scheduling),
// correct +0x98 store via held base ptr, correct return (s3C = slot 0x3C).
// Size gap driven from -99 insns (old stub) to -1.
// RESIDUAL CAP (~6%, NOT byte-exact, no episode): IDO register/slot coloring +
// addressing-mode in the `g` singleton region — target keeps g live in $a1
// across the inner func() call and stores the singleton via a held $v1 pointer
// (lui+addiu+sw, 3 insns); our C yields the la-macro sw (2 insns, the -1) plus
// a redundant g spill/reload, inflating the frame 80->88 (+2 slots) which
// renumbers nearly every stack slot. This is the documented coloring/
// addressing-mode cap class, not a structural/decode bug.
//
//   void *func_000027E8(Scene *scene, A a1, B a2, C a3) {  // args spilled
//                                                          // sp+0x50/54/58/5C
//     v0 = func_00000000(&D_00000000);  scene->0x3C = a2;  // (sp+0x38)
//     func_00000000(&func_00007328, 0);                    // register descs:
//     func_00000000(&D_00007334, scene->0x134);            // 7328/7334/7344/
//     func_00000000(&D_00007344, 0);                       // .L7350/7360/7374
//     func_00000000(.L00007350); func_00000000(&D_00007360);
//     func_00000000(); func_00000000(&D_00007374, 0);
//     // create widget objects from descriptor table, stash handles:
//     h4C = func_00000000(0, &D_00007380);  // sp+0x4C
//     h3C = func_00000000(0, &D_0000738C);  // sp+0x3C  (root, returned)
//     h48 = func_00000000(0, &D_00007390);  // sp+0x48
//     h44 = func_00000000(0, &D_00007394);  // sp+0x44
//     h40 = func_00000000(0, .L00007398);   // sp+0x40
//     g   = func_00000000(0); *(int*)D_0=g; // global singleton store
//     func_00000000(h4C+0x10, g);
//     if (g->0x14) g->0x4 = 1;  g->0x14 = h4C;             // re-parent idiom
//     // 3x normalized-ratio: a,b,c = func_00000000(...) ints; each via the
//     // u32->float fixup (bgez + add 0x4F800000 when sign bit set);
//     //   ratio = (float)(b - c) / (float)a;  passed as split-double a3:a2
//     //   to func_00000000 (anim/layout interp setup), once per axis,
//     //   reading scene->0x134 + descriptors D_739C/73B4/73CC.
//     // final: link h44/h40/h4C/h3C children — the
//     //   `if(h->0x14) h->0x4=1; h->0x14=child` dirty-and-attach idiom x4.
//     return scene->0x3C;  // = h3C
//   }
//
// Struct-typing reference:
//   scene = arg0 (spill sp+0x50): 0x3C result/root widget, 0x134 = UI
//     container/parent passed to child registrations.
//   widget handle H: 0x04 dirty-flag (set 1 when 0x14 was non-null),
//     0x14 attached-child/content ptr (re-parent target).
//   D_00007328..D_000073CC = descriptor/template table (labels, widget
//     specs); .L00007350/7398 are in-table sub-labels.
//   func_000001FC + 0x10 (lw, 3x) = folded int ref — same JAL-target-0 /
//   folded-symbol family as the func_0000098C literal-pool bug
//   (docs/N64_FORENSICS.md#bootup-uso-fp-literal-pool-folded-into-func-0000098C);
//   here it's the integer denominator of the ratio fixups.
// Caps (DEFERRED): 299-insn dispatcher-heavy ctor; raw-word USO +
//   placeholder cb's. Real-C STRUCTURAL body below — bootup/title
//   scene constructor skeleton. Byte-match deferred. Name pre-checked:
//   no extern reuse. D_00000000 reuses file-scope extern char.
#ifdef NON_MATCHING
void *func_000027E8(char *scene, int a1, int a2, int a3) {
    char *s4C, *s48, *s44, *s40, *s3C, *p, *g;
    int sv, m;
    f32 fd;
    sv = (int)func_00000000(&D_00000000);
    *(int *)(scene + 0x3C) = a2;
    func_00000000((char *)&D_00000000 + 0x7328, 0);
    func_00000000(&D_00000000, (char *)&D_00000000 + 0x7334);
    func_00000000(*(int *)(scene + 0x134), a2);
    func_00000000((char *)&D_00000000 + 0x7344, 0);
    func_00000000(&D_00000000, (char *)&D_00000000 + 0x7350);
    func_00000000(&D_00000000, (char *)&D_00000000 + 0x7360);
    func_00000000();
    func_00000000((char *)&D_00000000 + 0x7374, 0);
    s3C = (char *)func_00000000(0, (char *)&D_00000000 + 0x7380);
    s4C = (char *)func_00000000(0, (char *)&D_00000000 + 0x738C);
    s48 = (char *)func_00000000(0, (char *)&D_00000000 + 0x7390);
    s44 = (char *)func_00000000(0, (char *)&D_00000000 + 0x7394);
    s40 = (char *)func_00000000(0, (char *)&D_00000000 + 0x7398);
    g = (char *)func_00000000(0);
    *(int *)&D_00000000 = (int)g;
    func_00000000(s4C + 0x10, g);
    if (*(int *)(g + 0x14)) *(int *)(g + 0x4) = 1;
    *(int *)(g + 0x14) = (int)s4C;
    func_00000000();
    func_00000000(*(int *)(scene + 0x134), s44, s4C, s3C, s48, s40);
    func_00000000(*(int *)(scene + 0x134));
    func_00000000(*(int *)(scene + 0x134));
    func_00000000(*(int *)(scene + 0x134));
    m = (int)func_00000000(&D_00000000);
    fd = (f32)(u32)*(int *)((char *)&D_00000000 + 0x20C);
    func_00000000((char *)&D_00000000 + 0x739C,
                  ((f32)(u32)m - (f32)(u32)sv) / fd);
    sv = (int)func_00000000(&D_00000000);
    func_00000000(*(int *)(scene + 0x134), func_00000000(*(int *)(scene + 0x134), a1), a2);
    g = (char *)*(int *)(scene + 0x134);
    m = (int)func_00000000(&D_00000000);
    *(int *)(g + 0x98) = m;
    fd = (f32)(u32)*(int *)((char *)&D_00000000 + 0x20C);
    func_00000000((char *)&D_00000000 + 0x73B4,
                  ((f32)(u32)m - (f32)(u32)sv) / fd);
    sv = (int)func_00000000(&D_00000000);
    func_00000000(*(int *)(scene + 0x134), a2, a3);
    m = (int)func_00000000(&D_00000000);
    fd = (f32)(u32)*(int *)((char *)&D_00000000 + 0x20C);
    func_00000000((char *)&D_00000000 + 0x73CC,
                  ((f32)(u32)m - (f32)(u32)sv) / fd);
    p = s48 + 0x10;
    func_00000000(p, s44);
    if (*(int *)(s44 + 0x14)) *(int *)(s44 + 0x4) = 1;
    *(int *)(s44 + 0x14) = (int)s48;
    func_00000000(p, s40);
    if (*(int *)(s40 + 0x14)) *(int *)(s40 + 0x4) = 1;
    *(int *)(s40 + 0x14) = (int)s48;
    p = s3C + 0x10;
    func_00000000(p, s4C);
    if (*(int *)(s4C + 0x14)) *(int *)(s4C + 0x4) = 1;
    *(int *)(s4C + 0x14) = (int)s3C;
    func_00000000(p, s48);
    if (*(int *)(s48 + 0x14)) *(int *)(s48 + 0x4) = 1;
    *(int *)(s48 + 0x14) = (int)s3C;
    return s3C;
}
#else
INCLUDE_ASM("asm/nonmatchings/bootup_uso", func_000027E8);
#endif

/* func_00002C94 - verified structural decode (0x110, 68 insns,
 * subsystem init: scratch-vec setup + global flag config + table
 * walk).
 * Struct-typing reference: func_000003F8+0xF8 = f32 default value;
 * func_000000F0+0x38 = a global scratch Vec3 (set to (k,k,k));
 * func_00000080+0x68 = a paired context object (reloc setup args).
 * Global flag words: &D_mode = 6, &D_flags bit3 (0x8) cleared,
 * config word at func_00000000+4 = (old & ~0x2) | 0x2000 | 0x80000
 * (clears bit1, sets the 0x2000 and 0x80000 feature bits). &D_tbl =
 * a {int a; int b;} stride-8 table (next-probe reads p[0] after the
 * advance) walked until a zero terminator, each (a,b) passed to the
 * reloc helper with a fixed &D_ctx. D_000073E8 = init datum.
 * Caps <80: ~6 reloc calls + cross-symbol data refs (func_000003F8/
 * 000000F0/00000080/00000000+off) + multi-global flag RMW + table-
 * walk branch-likely. INCLUDE_ASM remains build path. */
extern char D_000073E8;
extern void func_000003F8();  /* used as data-symbol base */
/* EXACT (agent-c run 115). Identities/offsets from the TextReloc and the
 * asm addends: scale = RoData pool +0x4F0, dst = 0x128 / src = 0xE8 (both
 * import_0005C108 fields), mode D_01EBDC, flags import_0005EE64 (separate
 * load/store aliases), cfg word import_0005EE6C + 4 through a named int v
 * (target keeps it in v0), table/gate D_00719C (two aliases so the gate is
 * its own load), ctx import_0005C108; callees func_072E58, func_072F1C,
 * func_074710(&D_000073E8, 0), func_059354 per entry, func_074840. */
extern f32 bu_2c94_k4f0;
extern f32 bu_2c94_dst[];
extern char bu_2c94_src[], bu_2c94_ctx[], D_000073E8;
extern int bu_2c94_mode, bu_2c94_x_ld, bu_2c94_x_st, bu_2c94_y_ld[], bu_2c94_y_st[];
extern int bu_2c94_tbl[], bu_2c94_gate;
void func_00002C94(void *a0, void *a1) {
    f32 k;
    int *p;
    int e;
    int v;

    k = bu_2c94_k4f0;
    bu_2c94_dst[0] = k;
    bu_2c94_dst[1] = k;
    bu_2c94_dst[2] = k;
    func_00000000(bu_2c94_src);
    func_00000000(bu_2c94_src, bu_2c94_dst);
    bu_2c94_mode = 6;
    bu_2c94_x_st = bu_2c94_x_ld & ~8;
    v = bu_2c94_y_ld[1];
    v &= ~2;
    v |= 0x2000;
    v |= 0x80000;
    bu_2c94_y_st[1] = v;
    p = bu_2c94_tbl;
    func_00000000(&D_000073E8, 0);
    if (bu_2c94_gate != 0) {
        do {
            func_00000000(bu_2c94_ctx, p[0], p[1]);
            p += 2;
        } while (p[0] != 0);
    }
    func_00000000();
}

/* func_00002DA4: bootup_uso subsystem init + 12-way command dispatch
 * (0x1EC / 123 words). init(&D_000073EC, 0); create -> global; attach the
 * new object under arg0->0x84 (+0x10 list, 0x14 back-link, 0x4 linked flag);
 * config word |= 0x82000 | 0x20000; then switch (arg1) 1..12 -> per-mode
 * handler(arg0, arg2) (case 1: handler(arg0, 0) + obj->0x18 |= 4; cases
 * 7/8 and out-of-range: func(&D_00007408)).
 * Exact C (identical to the IDO 7.1 -O2 donor unit bootup_uso_o2_2DA4.c,
 * 123/123 words; levers documented there). WIRED via REPLACE_FUNC_BODY donor
 * splice: the in-unit compile's .rodata jumptable is discarded by the link, so
 * the splice swaps in the donor body with its .rodata reloc renamed
 * func_00002DA4_rodata (pinned to the bootup_uso RoData table at +0x4F4).
 * The in-unit body keeps the .text size exact: a smaller placeholder shifts
 * the section-relative %lo(.L...) addends of later INCLUDE_ASM functions
 * (replace-function-body.py does not rebase those). */
extern void func_00000000_2DA4_v();
extern char D_000073EC, D_000073F8, D_00007408;
extern int D_2DA4_obj, D_2DA4_obj_rd;
extern char D_2DA4_cfg;
#define FW2DA4(p, o) (*(int *)((char *)(p) + (o)))
void func_00002DA4(char *arg0, s32 arg1, s32 arg2) {
    char *a1;
    char *v1;
    int v;
    func_00000000_2DA4_v(&D_000073EC, 0);
    func_00000000_2DA4_v();
    D_2DA4_obj = func_00000000(0, &D_000073F8);
    v1 = (char *)FW2DA4(arg0, 0x84);
    a1 = (char *)D_2DA4_obj_rd;
    func_00000000_2DA4_v(v1 + 0x10, a1);
    if (FW2DA4(a1, 0x14) != 0) {
        FW2DA4(a1, 0x4) = 1;
    }
    FW2DA4(a1, 0x14) = (int)v1;
    func_00000000_2DA4_v(D_2DA4_obj);
    v = *(int *)((char *)&D_2DA4_cfg + 4);
    v = v | 0x80000 | 0x2000 | 0x20000;
    *(int *)((char *)&D_00000000 + 4) = v;
    switch (arg1) {
    case 2: func_00000000_2DA4_v(arg0, arg2); break;
    case 4: func_00000000_2DA4_v(arg0, arg2); break;
    case 12: func_00000000_2DA4_v(arg0, arg2); break;
    case 3: func_00000000_2DA4_v(arg0, arg2); break;
    case 5: func_00000000_2DA4_v(arg0, arg2); break;
    case 6: func_00000000_2DA4_v(arg0, arg2); break;
    case 1:
        func_00000000_2DA4_v(arg0, 0);
        a1 = (char *)D_2DA4_obj; if (1) { a1 += 0x18; } *(int *)a1 = *(int *)a1 | 4;
        break;
    case 9: func_00000000_2DA4_v(arg0, arg2); break;
    case 10: func_00000000_2DA4_v(arg0, arg2); break;
    case 11: func_00000000_2DA4_v(arg0, arg2); break;
    default:
        func_00000000_2DA4_v(&D_00007408);
        break;
    }
}
#undef FW2DA4

/* func_00002F90 - verified structural decode (0x228, 138 insns,
 * multi-widget builder + per-child init). Builder+register family
 * (cf. func_00001E08 / func_00006734).
 *   void func_00002F90(St *s1) {
 *       func_00000000(&D_0000741C, 0);            // pre-init
 *       c = func_00000000(0, &D_00007424);        // build child 0
 *       s1->0x90 = c;
 *       f32 k = *(f32*)(func_000003F8 + 0x12C);
 *       c->0x124 = c->0x128 = c->0x12C = k;       // colour rgb
 *       c->0x130 = 1.0f;                          // alpha
 *       c->0xD0  = 1;                             // type tag
 *       c->0xC4 |= 0x40;  c->0xC4 |= 0x80;        // flag bits
 *       c = func_00000000(0, &D_00007434);        // build child 1
 *       s1->0x94 = c;
 *       c->0x124..0x130 = 1.0f;                   // white
 *       c->0xD0  = 2;
 *       c->0xC4 |= 0x40; ...
 *       ... (repeats for D_00007444 / D_00007454 children) ...
 *   }
 * Struct-typing reference: s1->0x90 (144) / s1->0x94 (148) / ... =
 * slots holding the built child widgets (D_00007424/7434/7444/
 * 7454 = each child's key/descriptor data). Per child: ->0x124
 * (292) / 0x128 (296) / 0x12C (300) f32 = RGB colour (child 0 uses
 * the global func_000003F8+0x12C value; later children use 1.0),
 * ->0x130 (304) f32 = 1.0 alpha, ->0xD0 (208) s32 = type tag
 * (1,2,...), ->0xC4 (196) u32 flags (bits 0x40 then 0x80 OR'd in).
 * D_0000741C = pre-init datum. EXACT (see the comment on the body). */
/* EXACT 2026-10-16 (agent-c, 75.4 -> 100). The old "held float-pool base"
 * cap was the shared func_000003F8 base: three scalar f32 globals pinned at
 * the real offsets (D_00000524/528/52C = func_000003F8+0x12C/0x130/0x134)
 * each get their own `lui at; lwc1`. Other levers:
 *  - the three registration calls take a base-0 object (D_2F90_reg, one
 *    symbol: the target holds it in s0), not a literal 0; m->0x28 is the
 *    base-0 address D_2F90_m28, and the s1->0x90 link call passes p90 as a1;
 *  - `p = obj; if (1) { p += 0xC4; } *(int *)p |= K;` for the addiu-base RMWs,
 *    with ONE reused pointer (p90, colours a2) for every s1->0x90/0x94 reload
 *    and the link block; a second reused pointer `o` (s0) for the third child,
 *    the s1->0x84 node and the 0x16C alloc;
 *  - the alloc init takes three floats through the prototyped blank alias
 *    func_00000000_2F90_f (mfc1 a2/a3 + swc1 16(sp)); unused-result calls go
 *    through the void alias func_00000000_2F90_v (the int blank pushed n2 to
 *    v1);
 *  - `*(o+0x28) = &D; *(o+0x120) = 0xFFFF;` on one line gives the target ring
 *    order; s1->0x90 store after the k load;
 *  - slots: volatile pads pad0 / pad..pad3 put p90's spill at sp+0x58, n2 at
 *    0x48 and vec at 0x3C (frame 0x60). */
extern char D_0000741C, D_00007424, D_00007434, D_00007444, D_00007454;
extern float D_00000524, D_00000528, D_0000052C;
extern char D_2F90_reg, D_2F90_m28;
extern void func_00000000_2F90_v();
extern void func_00000000_2F90_f(char *, char *, float, float, float);
void func_00002F90(char *s1) {
    volatile int pad0;
    char *p90;
    volatile int pad;
    volatile int pad2;
    volatile int pad3;
    int n2;
    float vec[3];
    float k2;
    char *o;
    char *c;
    func_00000000_2F90_v(&D_0000741C, 0);
    c = (char *)func_00000000(0, &D_00007424);
    {
        float k = D_00000524;
        *(char **)(s1 + 0x90) = c;
        *(float *)(c + 0x12C) = k;
        *(float *)(c + 0x128) = k;
        *(float *)(c + 0x124) = k;
        *(float *)(c + 0x130) = 1.0f;
    }
    p90 = *(char **)(s1 + 0x90); *(int *)(p90 + 0xD0) = 1;
    p90 = *(char **)(s1 + 0x90); if (1) { p90 += 0xC4; } *(int *)p90 |= 0x40;
    p90 = *(char **)(s1 + 0x90); if (1) { p90 += 0xC4; } *(int *)p90 |= 0x80;
    c = (char *)func_00000000(0, &D_00007434);
    *(char **)(s1 + 0x94) = c;
    *(float *)(c + 0x130) = 1.0f;
    *(float *)(c + 0x12C) = 1.0f;
    *(float *)(c + 0x128) = 1.0f;
    *(float *)(c + 0x124) = 1.0f;
    p90 = *(char **)(s1 + 0x94); *(int *)(p90 + 0xD0) = 2;
    p90 = *(char **)(s1 + 0x94); if (1) { p90 += 0xC4; } *(int *)p90 |= 0x40;
    o = (char *)func_00000000(0, &D_00007444);
    vec[0] = 0.0f;
    vec[2] = 0.0f;
    vec[1] = D_00000528;
    func_00000000_2F90_v(o + 0x70, vec);
    c = o; if (1) { c += 0xC4; } *(int *)c |= 0x40;
    func_00000000_2F90_v(&D_2F90_reg, *(int *)(s1 + 0x94));
    func_00000000_2F90_v(&D_2F90_reg, *(int *)(s1 + 0x90));
    func_00000000_2F90_v(&D_2F90_reg, *(int *)(s1 + 0x94));
    p90 = *(char **)(s1 + 0x90);
    o = *(char **)(s1 + 0x84);
    func_00000000_2F90_v(o + 0x10, p90);
    if (*(int *)(p90 + 0x14) != 0) {
        *(int *)(p90 + 4) = 1;
    }
    *(int *)(p90 + 0x14) = (int)o;
    o = (char *)func_00000000(0x16C);
    if (o != 0) {
        func_00000000_2F90_f(o, &D_00007454, 0.0f, 0.0f, 0.0f);
        k2 = D_0000052C;
        *(int *)(o + 0x28) = (int)&D_2F90_m28; *(int *)(o + 0x120) = 0xFFFF;
        *(float *)(o + 0x108) = k2;
        *(float *)(o + 0x10C) = k2;
        *(float *)(o + 0x110) = k2;
        *(float *)(o + 0x124) = 1.0f;
    }
    n2 = *(int *)(s1 + 0x84);
    func_00000000_2F90_v(n2 + 0x10, o);
    if (*(int *)(o + 0x14) != 0) {
        *(int *)(o + 4) = 1;
    }
    *(int *)(o + 0x14) = n2;
    func_00000000_2F90_v();
}

void func_000031B8(int a0) {
}

// func_000031C0 — STRUCTURAL PASS (0x328 / 202 insns, no episode).
// 3-mode UI/scene element constructor (bootup/title).
//
//   void *func_000031C0(A a0, int mode, P parent3, Q a3) {  // spill 0x90..0x9C
//     obj = func_00000000(0xB8);  if (!obj) obj = func_00000000(0xB4);
//     if (!obj) goto post;        // alloc primary record (fallback size)
//     func_00000000(obj, &D_00007460);                 // base-init
//     obj->0x28 = 0;
//     sub = (obj == -0x2C) ? func_00000000(4) : obj+0x2C;   // sub-alloc/embed
//     if (!sub) sub2 = func_00000000(4);
//     if (sub) *sub = 0;
//     func_00000000(obj);                                   // finalize
//     // Vec3-ish default at sp+0x48: {0.0, 500.0f(0x43FA0000), 0.0};
//     func_00000000(obj+0x30, &sp48);
//   post:
//     v = arg1(mode);
//     switch (mode) {
//       case 0: case 2:   // .L32CC: build widget kind A
//         w = func_00000000(0,arg3,obj, 0, 500.0f, func_000003F8+0x138,
//                           arg_at_0xA0);                    // folded f32
//         func_00000000(0, arg3->0x874, &D_00007480);
//         func_00000000(w, ret, &.L00007490);
//         child = func_00000000(arg1b, ...);  arg1b->0xB4 = child;
//         child->0xC = &D_000074A4;  break;
//       case 1:           // .L3384: build widget kind B
//         e = func_00000000(0x15C);
//         if (e) { func_00000000(e, obj, 0, 500.0f, func_000003F8+0x13C);
//                  e->0x28 = &D_00007120;  e->0x158 = 0;
//                  e->0x154 = arg3;  e->0x150 = e->0x14C; }
//         func_00000000(0, &D_000074B0, arg3);
//         f = func_00000000(arg1b);  f->0xC = &D_000074C4;  break;
//       default: ;        // .L341C
//     }
//     // common tail: x2 dirty-and-attach re-parent
//     //   (if h->0x14: h->0x4=1; h->0x14 = parent3);
//     // copy Vec3 arg3->{0x3A4,0x3A8,0x3AC} into widget->{0x60,0x64,0x68}
//     // and into parent3+0x30; consts 500.0f / -1000.0f (0xC47A0000).
//     return obj;
//   }
//
// Struct-typing reference:
//   obj sizes 0xB8 / 0xB4 / (kind-B) 0x15C; 0x28 vtable/desc ptr,
//     0x2C embedded-sub, 0xB4 child, 0x14C/0x150 f32 pair, 0x154 owner,
//     0x158 flag, 0x60/0x64/0x68 Vec3 (from arg3->0x3A4/0x3A8/0x3AC).
//   widget handle: 0x04 dirty-flag, 0x14 attached-child (re-parent).
//   D_00007460/746C/7480/.L7490/74A4/7120/74B0/74C4 = per-mode desc table.
//   func_000003F8 + 0x138 / + 0x13C (lwc1) = folded f32 consts — yet
//   another fold target in the bootup_uso literal-pool bug family
//   (docs/N64_FORENSICS.md#bootup-uso-fp-literal-pool-folded-into-func-0000098C).
// Caps (DEFERRED, fuzzy 27.5%->42.0% 2026-07): full faithful body incl.
//   3-mode switch + common re-parent/Vec3-copy tail reconstructed; typed
//   float protos suppress K&R double-promote (swc1 not cvt.d.s/sdc1).
//   Remaining byte-cap: target keeps `obj` in $a2 spilled/reloaded around
//   each call (0x6C slot) vs the natural callee-saved $s0 here (regalloc
//   divergence), plus folded f32 consts at func_000003F8+0x138/+0x13C
//   (bootup_uso literal-pool bug). Name pre-checked: no extern reuse.
#ifdef NON_MATCHING
/* PASS-3 2026-10-20 (agent-c run 114): fresh decode of the target words.
 * cfront-style constructor code: obj = alloc(0xB8); the inlined ctor
 * re-tests `this` (dead alloc(0xB4) fallback), and the member sub-object at
 * +0x2C has two nested inlined ctors (the outer tests &this->sub == 0, i.e.
 * this == -0x2C). Then a mode switch (0/2 -> widget kind A, 1 -> kind B)
 * and the common attach + Vec3-copy tail. Float args sit in a2/a3 as
 * bit patterns (`addiu a3,zero,0` = 0.0f, `lui a3,0x43fa` = 500.0f), so the
 * builder protos take float params at those slots. Two `if (1) { }`
 * blocks before the return raise the BB count so uopt keeps every local in
 * its frame home instead of s0/s1 (74.3 -> 88.5). `vec1[0] = vec1[2] =
 * 0.0f` shares one mtc1 (-> 91.6). Residual: target keeps an obj2 copy in
 * its own slot (volatile reproduces it but costs the dispatch order), the
 * switch tests 0,2,1 + an explicit b-default, and the frame is 0x20 short. */
extern void *fn_31c0_alloc(int);
extern void  fn_31c0_init(void *, void *);
extern void  fn_31c0_fin(void *);
extern void  fn_31c0_vec(void *, float *);
extern void *fn_31c0_A1(int, void *, void *, float, float, float, int, void *);
extern void *fn_31c0_A2(int, void *, int, void *);
extern void  fn_31c0_link(void *, void *);
extern void *fn_31c0_A4(int, void *, void *, void *);
extern void  fn_31c0_B2(void *, void *, float, float, float);
extern void *fn_31c0_B3(int, void *, void *, void *);
extern void  fn_31c0_tail(void *, void *);
extern char D_31c0_a, D_31c0_b, D_31c0_c, D_31c0_d, D_31c0_e, D_31c0_f, D_31c0_g, D_31c0_h;
typedef struct { int w; } SC4_; /* run 131 struct-copy lever (31C0) */
void *func_000031C0(int a0, int mode, char *parent, char *a3v, int p4) {
    char *w;
    char *h;
    char *v;
    char *r2;
    char *r3;
    char *obj2;
    char *obj;
    char *p;
    char *e;
    volatile int pqa[6]; /* run 138: frame 112 -> 144 = target (vec1 at 72) */
    f32 vec1[3];
    f32 vec2[3];
    int *m;
    volatile int pqb[2];

    obj = (char *)fn_31c0_alloc(0xB8); if (mode) {}
    if (obj != 0) {
        p = obj;
        if (p == 0) {
            p = (char *)fn_31c0_alloc(0xB4);
            if (p == 0) goto post;
        }
        fn_31c0_init(p, &D_31c0_a);
        *(void **)(p + 0x28) = &D_31c0_b;
        m = (int *)(p + 0x2C);
        if (m == 0) {
            m = (int *)fn_31c0_alloc(4);
            if (m == 0) goto skip;
        }
        if (m == 0) {
            m = (int *)fn_31c0_alloc(4);
            if (m == 0) goto skip;
        }
        *m = 0;
    skip:
        fn_31c0_fin(p);
        vec1[0] = vec1[2] = 0.0f;
        vec1[1] = 500.0f;
        fn_31c0_vec(p + 0x30, vec1);
    }
post:
    *(SC4_ *)&(obj2) = *(SC4_ *)&(obj); /* run 131: 4-byte struct copy (both address-taken) -> length 202 = target */
    switch (mode) {
    case 0:
    case 2:
        h = w = (char *)fn_31c0_A1(0, a3v, obj, 0.0f, 500.0f, 1.0e-20f, p4, &D_31c0_c);
        r2 = (char *)fn_31c0_A2(0, a3v, *(int *)(a3v + 0x874), &D_31c0_d);
        fn_31c0_link(w, r2);
        *(char **)(w + 0x154) = r2;
        r3 = (char *)fn_31c0_A4(0, w, r2, (char *)&D_31c0_e + 0x7490);
        fn_31c0_link(obj2, r3);
        *(char **)(obj2 + 0xB4) = r3;
        *(void **)(r3 + 0xC) = &D_31c0_f;
        break;
    case 1:
        e = (char *)fn_31c0_alloc(0x15C);
        if (e != 0) {
            fn_31c0_B2(e, obj, 0.0f, 500.0f, 1.0e-21f);
            *(void **)(e + 0x28) = &D_31c0_g;
            *(int *)(e + 0x158) = 0;
            *(char **)(e + 0x154) = a3v;
            *(f32 *)(e + 0x150) = *(f32 *)(e + 0x14C);
        }
        h = e;
        v = (char *)fn_31c0_B3(0, a3v, a3v, &D_31c0_h);
        fn_31c0_link(obj2, v);
        *(void **)(v + 0xC) = &D_31c0_f;
        break;
    }
    fn_31c0_tail(parent + 0x10, h);
    if (*(int *)(h + 0x14) != 0) {
        *(int *)(h + 0x4) = 1;
    }
    *(char **)(h + 0x14) = parent;
    fn_31c0_tail(parent + 0x10, obj2);
    if (*(int *)(obj2 + 0x14) != 0) {
        *(int *)(obj2 + 0x4) = 1;
    }
    *(char **)(obj2 + 0x14) = parent;
    *(f32 *)(obj2 + 0x60) = *(f32 *)(a3v + 0x3A4);
    *(f32 *)(obj2 + 0x64) = *(f32 *)(a3v + 0x3A8);
    *(f32 *)(obj2 + 0x68) = *(f32 *)(a3v + 0x3AC);
    *(f32 *)(h + 0x60) = *(f32 *)(a3v + 0x3A4);
    *(f32 *)(h + 0x64) = *(f32 *)(a3v + 0x3A8);
    *(f32 *)(h + 0x68) = *(f32 *)(a3v + 0x3AC);
    vec2[1] = 0.0f;
    vec2[0] = 0.0f;
    vec2[2] = -1000.0f;
    fn_31c0_vec(h + 0x30, vec2);
    if (1) { } if (1) { }
    return h;
}
#else
INCLUDE_ASM("asm/nonmatchings/bootup_uso", func_000031C0);
#endif

/* func_000034E8 - verified structural decode (0x150, 84 insns,
 * tagged-arg-block object builder + wire). Cross-symbol data refs:
 * func_00000008+0x28 (global Vec3 slot), func_00000008+0x24 (global
 * pointer slot), func_000003F8+0x140 (source Vec3). The C body uses
 * &func_NNNNNNNN + offset casts to recreate the data references.
 * Struct-typing reference: func_000003F8+0x140 = a global source
 * Vec3 (3 f32) copied (with a 0.0 4th lane) into the global slot
 * func_00000008+0x28. a1 = an int[] params source: indices 0,1,2,
 * 3,4,5 and 6 (a1[6] OR'd with 0x2) feed the builder arg block,
 * interleaved with id tags 0x64/0x6D/0x6E/0x6F/0x70/0x71/0x73/0x74
 * (the func_00000000 builder is a tagged-vararg constructor; tag
 * 0x64 head, 0x6D terminator-ish). Result r: r->0x70 (112) = a2,
 * and r is published to the global func_00000008+0x24. a0->0x84
 * (132) / a0->0x80 (128) = optional-callback gates; final
 * func_00000000(a0, r) attaches r to a0. D_000074D0 = builder datum.
 * 2026-06-20 correctness fixes (67->68.6%): removed a spurious
 * `*(+0x24)=r` store (not in target); `r->0x70=a2` placed correctly
 * (2nd-call delay slot); the two optional-callback calls take TWO args
 * `func(r, a0->0x84)` / `func(r, a0->0x80)` (the 2nd arg was missing).
 * 2026-10-16 EXACT (lui-count sweep, docs/IDO_CODEGEN.md#lui-count-
 * placeholder-sweep-44de4: target 6 luis vs 3): the three source floats are
 * distinct scalar globals pinned at 0x538/0x53C/0x540 (one shared
 * &func_000003F8 base was CSE'd into one held register), and the Vec4 slot is
 * the zero-offset D_00000030 (== func_00000008+0x28), so its address is held
 * in v0 with 0/4/8/C displacements as in the target. Each optional callback
 * reads its gate once (`if ((p = a0->0x84) != 0) f(r, p)`), and the r->0x70
 * store precedes the publish call (it lands in that jal's delay slot). */
extern char D_000074D0;
extern f32 D_00000538, D_0000053C, D_00000540;
extern float D_00000030[4];
extern char D_0000002C;
extern void func_00000008();  /* used as data-symbol base */
extern void func_000003F8();  /* used as data-symbol base */
void *func_000034E8(char *a0, int *a1, int a2) {
    float *g;
    char *r;
    int p;
    g = D_00000030;
    g[0] = D_00000538;
    g[1] = D_0000053C;
    g[2] = D_00000540;
    g[3] = 0.0f;
    r = (char *)func_00000000(0, 0x64, &D_000074D0, 0x6D,
                              2, 0x6E, a1[0], a1[1], 0x6F, a1[2], a1[3],
                              0x70, a1[6] | 2, 0x71, 1, 0x73, a1[4],
                              0x74, a1[5], 0);
    *(int *)(r + 0x70) = a2;
    func_00000000(&D_0000002C, r);
    if ((p = *(int *)(a0 + 0x84)) != 0) func_00000000(r, p);
    if ((p = *(int *)(a0 + 0x80)) != 0) func_00000000(r, p);
    func_00000000(a0, r);
    return r;
}

/* func_00003638 - verified structural decode (0xFC, 63 insns,
 * object factory + link + notify).
 *   void *func_00003638(void *a0, int a1, int a2, int a3,
 *                       int e0, f32 e1, f32 e2, int flags,
 *                       int e4, void *parent) {   // e* = stack args
 *       reloc_init(&D_000074DC);
 *       flags |= 0x2;                              // force bit 1
 *       r = reloc_create(0, a1, a2 + 1, a3 + 1,
 *                        e0, e1, e2, e4);          // big factory call
 *       reloc_attach((char*)parent + 0x10, r);
 *       if (r->0x14 == 0) {                        // not yet linked
 *           r->0x4  = 1;
 *           r->0x14 = parent;
 *       }
 *       if (flags & 0xC) reloc_notify(a0, ..., 0);
 *       else             reloc_notify2(..., r);
 *       reloc_finalize();
 *       return r;
 *   }
 * Struct-typing reference: created object r - r->0x4 (4) s32 =
 * linked-flag (set 1 when first attached), r->0x14 (20) = back-link
 * to `parent` (0 = unlinked). `parent` (a stack arg, spilled
 * sp+0x4C) is the owning container; parent+0x10 is the attach
 * point passed to the second reloc helper. `flags` (stack arg,
 * sp+0x58) gets bit 1 (0x2) forced on, and bits 0xC select the
 * post-link notify path. a2/a3 are incremented by 1 before the
 * factory call (1-based -> 0-based or count+1). e1/e2 are f32
 * stack args (lwc1/swc1 through sp+0x14/0x18). D_000074DC = init
 * datum. Caps <80: 5x func_00000000 reloc calls + f32 stack-arg
 * passing + beql branch-likely link guard + flags-bit branch.
 * Full body INCLUDE_ASM-preserved (.s = source of truth).
 * INCLUDE_ASM (no episode; tautology-trap rule). */
#ifdef NON_MATCHING
/* func_00003638: 5-call orchestrator. All 10 args home-spilled at entry; calls
 * cb(&D_74DC); node = cb(0, arg1, arg2+1, arg3+1, arg4, arg6, arg7, arg8|2, arg9);
 * cb(arg5+0x10, node); if (arg5->0x14) arg5->0x4=1; arg5->0x14 = arg4(0x4C slot);
 * then by (arg8|2)&0xC cb(arg0, node, 0/1); cb(); return node. m2c-assisted.
 * 2026-06-02 (49.6->93.7%): two fixes.
 *   (1) the node call's arg6/arg7 are single floats on the stack (target swc1
 *       0x14/0x18) — routed through a typed-float proto func_3638_node so they
 *       don't K&R double-promote (cvt.d.s+sdc1).
 *   (2) the `arg8 |= 2` must be a real mutation placed AFTER the cb(&D_74DC)
 *       call (target writes the |2 result to BOTH arg8's home slot AND the
 *       node-call outgoing slot, deferred past the first jal). Computing it at
 *       the top (or inline-only `arg8|2`) misaligned the whole schedule.
 * 2026-07-15 (agent-g, 93.7 -> 95.9): 38C0-family sweep. TWO DECODE ERRORS +
 * one lever: (1) the link-guard operates on the NODE (reloaded into $v1 from
 * a 0x30 spill of a `p = node` copy taken BEFORE the attach jal), not on
 * arg5; (2) the back-link store is arg5 (parent), not arg4;
 * (a) void-alias the init call — its dead $v0 def was excluding $v0, flags
 * web colored $v1 (dead-$v0 exclusion, 38C0 recipe). All 63 insns + regs now
 * match except ONE as1 delay-slot tie: target fills the first jal delay with
 * `sw a3` (addiu %lo early), build fills it with the addiu (sw a2/a3 early).
 * Probed if(1)-wrap / array-decay / named-address-local — all inert.
 * AS1-TIE CAP (li-at/lui-slot class); stays NM (INCLUDE_ASM build path). */
extern char D_000074DC;
/* typed-float proto (0x0-alias): args 6,7 are single floats on the stack
 * (target swc1 0x14/0x18); K&R func_00000000 double-promotes them. */
extern void *func_3638_node(int, void *, int, int, int, float, float, int, int);
/* void-returning alias for the init call: kills the dead $v0 def so flags colors $v0 (38C0 recipe) */
extern void func_00000000_3638_v(void *);
void *func_00003638(int *arg0, void *arg1, int arg2, int arg3, int arg4, char *arg5,
                    float arg6, float arg7, int arg8, int arg9) {
    void *node;
    char *p;
    func_00000000_3638_v(&D_000074DC);
    arg8 |= 2;
    node = func_3638_node(0, arg1, arg2 + 1, arg3 + 1, arg4, arg6, arg7, arg8, arg9);
    p = node;   /* copy spilled at 0x30 before the attach jal; guard reads NODE via p */
    func_00000000(arg5 + 0x10, node);
    if (*(int *)(p + 0x14) != 0) {
        *(int *)(p + 0x4) = 1;
    }
    *(char **)(p + 0x14) = arg5;   /* back-link = arg5 (parent), NOT arg4 */
    if (arg8 & 0xC) {
        func_00000000(arg0, node, 0);
    } else {
        func_00000000(arg0, node, 1);
    }
    func_00000000();
    return node;
}
#else
INCLUDE_ASM("asm/nonmatchings/bootup_uso", func_00003638);
#endif

/* func_00003734 - verified structural decode (0x18C, 99 insns,
 * multi-stage builder chain). LARGER SIBLING of func_000038C0
 * (same alloc-0x80 + tagged-arg func_00000000 builder family,
 * idx*0x1C descriptor table, link via ->0x14/->0x4, r->0x8DC =
 * result).
 * Struct-typing reference (same family as func_000038C0): s1->0x80
 * (128) handle passed through; s1->0x98 (152) -> cfg with f32
 * 0xC4 (196)/0xCC (204) builder params; s1->0x84 (132) = parent
 * for the r5 list-link (r5->0x14 (20) back-link, r5->0x4 (4) =1
 * when already-linked). r1->0x8DC (2268) = slot receiving the
 * idx-table build result r4. &D + a1*0x1C = a 28-byte descriptor
 * table indexed by a1; func_00000080+0x14 = a static datum.
 * func_00000000 = the tagged-vararg builder (tags 2/1/0/0x58005/
 * 0x1B). Caps <80: ~12 reloc + FP stack-arg + a1*0x1C index
 * (sll/subu/sll) + beql link-guard + 2 alloc(0x80).
 *
 * 2026-06-02 (68.6->72.5%): the r1 builder call's args 7,8 are single floats
 * on the stack (cfg->0xC4/0xCC, target swc1 0x18/0x1C) — routed through typed-
 * float proto func_3734_r1 so they don't K&R double-promote. Residual ~27% =
 * arg-home-slot + scheduling regalloc (a1 spilled to sp+0x64 vs 0x5C, cascades
 * the frame). Third double-promote-scan hit (1A44/3638/3734).
 * 2026-07-15 (agent-g, 72.5 -> objdiff-100): 38C0/39D8 recipe backport, 3rd
 * family member cracked. FOUR DECODE ERRORS + two levers:
 *  (1) alloc-fail does NOT return — beqz +4 skips only the init call (x2);
 *  (2) arg4 of the r1 builder is a VALUE load *(func_00000080+0x14)
 *      (target lui/lw), not the address — D_3734_v alias;
 *  (3)+(4) the r2 AND r3 builder calls have a FIFTH arg s0 (sw s0, 0x10(sp)
 *      before each jal), matching sibling 39D8's b3=builder(...,o1);
 *  (a) de-name cfg/row + merge o/sub/r5 into ONE $s0 web + void-alias the
 *      reg call (dead $v0 def freed so cfg colors $v0) -> all 99 insns and
 *      the whole temp ring matched at frame 0x50;
 *  (b) target frame is 0x60 with 4 ghost home words at 0x3C-0x48 (the
 *      original's named locals) — 4x `volatile int pad` phantom slots
 *      restore them with zero insns. Word-exact vs target .s except the
 *      %lo placeholder addend (reloc).
 * Unwrapped EXACT 2026-10-16 (run 96): the one differing .o word (lw %lo
 * addend 0 vs 0x14) links to the ROM's 0x94 because D_3734_v is pinned to
 * func_00000080+0x14 = 0x94; every other reloc resolves to 0. */
/* typed-float proto (0x0-alias): args 7,8 are single floats on the stack
 * (target swc1 0x18/0x1C); K&R func_00000000 double-promotes them. */
extern char *func_3734_r1(void *, int, int, int, void *, int, float, float, int, int);
extern int D_3734_v;  /* VALUE load at func_00000080+0x14 (target lui/lw, not lui/addiu) */
/* void-returning alias for the reg call: kills the dead $v0 def so cfg colors $v0 (38C0 recipe) */
extern void func_00000000_3734_v(void *, void *);
void func_00003734(char *s1, int a1) {
    char *s0;
    char *r1, *r2, *r3, *r4;
    volatile int pad1, pad2, pad3, pad4;  /* 4 ghost home words 0x3C-0x48 (phantom-slot lever) */
    char *p;
    s0 = (char*)func_00000000(0x80);
    if (s0 != 0) {
        func_00000000(s0, 0);   /* alloc-fail SKIPS ONLY THIS (target beqz +4, 38C0 recipe) */
    }
    func_00000000_3734_v(&D_00000000, s0);
    r1 = func_3734_r1(s1, 2, 1, D_3734_v,
                      s0, *(int*)(s1 + 0x80),
                      *(float*)(*(char**)(s1 + 0x98) + 0xC4),
                      *(float*)(*(char**)(s1 + 0x98) + 0xCC),
                      0x58005, 0x1B);
    r2 = (char*)func_00000000(s1, 0, *(int*)(s1 + 0x80), r1, s0);
    r3 = (char*)func_00000000(s1, 1, *(int*)(s1 + 0x80), r1, s0);
    r4 = (char*)func_00000000(s1, (char*)&D_00000000 + a1 * 0x1C, r2);
    s0 = (char*)func_00000000(0x80);   /* merged $s0 web: o -> sub -> r5 */
    if (s0 != 0) {
        func_00000000(s0, 1);
    }
    s0 = (char*)func_00000000(0, s0, r1, r4, r2, r3);
    p = *(char**)(s1 + 0x84);
    func_00000000(p + 0x10, s0);
    if (*(int*)(s0 + 0x14) != 0) *(int*)(s0 + 0x4) = 1;
    *(char**)(s0 + 0x14) = p;
    *(char**)(r1 + 0x8DC) = r4;
}

/* func_000038C0 - verified structural decode (0x118, 70 insns,
 * composite constructor + 3-stage builder chain).
 * Struct-typing reference: o = alloc'd 0x80-byte object. s1->0x80
 * (128) = a handle passed through; s1->0x98 (152) = ptr to a config
 * whose 0xC4 (196) / 0xCC (204) are f32 params (passed as stack
 * args to build1). flags word = global D_z->0x50 OR'd with 0x1 |
 * 0x10000 | 0x40000 feature bits; D_z->0x4C (76) / 0x54 (84) =
 * global build params. &D_w = a 0x1C-stride (28-byte) descriptor
 * table indexed by a1. r (= build1 result) ->0x8DC (2268) = slot
 * receiving the final build3 result (r2 + table row).
 * Caps <80: ~8 reloc + FP stack-arg passing + packed-flag lui/ori
 * build + a1*0x1C table index.
 *
 * 2026-06-02 (70.5->80.1%): the build1 call's args 7,8 are single floats
 * (cfg->0xC4/0xCC, target swc1 0x18) — typed-float proto func_38c0_r avoids
 * the K&R double-promote. Sibling of func_00003734 (double-promote-scan vein).
 * 2026-07-15 (80.1->91.3, agent-g): TWO DECODE ERRORS + one lever:
 *  (1) alloc-fail does NOT return — target beq v0,zero,+4 skips ONLY the
 *      func(o,1) init call and continues the whole builder chain;
 *  (2) the r2 builder call has FIVE args: func(s1, 2, s1->0x80, r, o) —
 *      arg3 = r (or a3,v0 in the jal delay), o goes to the stack slot;
 *      matches sibling 39D8's decoded b3=builder(s2,2,s2->0x80,r1,o1);
 *  (3) array-decay retype `extern int D_38c0_z[]` + indexed loads shares the
 *      Dz base in ONE lui/addiu register pair across 0x4C/0x50/0x54 (the
 *      char*-held + per-use-cast form folded per-use).
 * 2026-07-15 (agent-g wave 3 round 2, 91.3 -> objdiff-100 / byte-exact vs
 * target .s): (a) DE-NAME flags + cfg (fold the OR-chain and both float loads
 * inline into the r-call args) — expression temps number through the FRESH
 * ring t7->t8->t9->t0 exactly and the two dead ghost homes vanish (frame
 * -0x48 -> -0x40 exact, r-spill 0x38); (b) the reg_b call (last jal before
 * the temp region) gets a VOID-returning alias func_00000000_38C0_v — its
 * dead $v0 def was excluding $v0 from the CSE temps (cfg stole v1, Dz fell
 * to ring t0); with it gone Dz=v1/cfg=v0 as in target (082F8/102CC lever).
 * Unwrapped EXACT 2026-10-16 (run 96): every callee/data name resolves to 0
 * (D_38c0_z pinned at the end of undefined_syms_auto.txt). */
/* typed-float proto (0x0-alias): args 7,8 are single floats on the stack
 * (target swc1 0x18); K&R func_00000000 double-promotes them. */
extern char *func_38c0_r(void *, int, int, int, void *, int, float, float, int, int);
extern int D_38c0_z[];   /* D_z global — array-typed so the base is held in ONE reg */
/* void-returning alias for reg_b: kills the dead $v0 def so Dz/cfg color v1/v0 */
extern void func_00000000_38C0_v(void *, void *, int);
void func_000038C0(char *s1, int a1) {
    char *o = (char*)func_00000000(0x80);
    char *r;
    char *r2;
    int *row;
    char *res;
    if (o != 0) {
        func_00000000(o, 1);   /* alloc-fail SKIPS ONLY THIS (target beq +4) */
    }
    func_00000000(&D_00000000, o);          /* reg_a — D_x placeholder */
    func_00000000_38C0_v(&D_00000000, o, 0); /* reg_b — D_y placeholder (void alias) */
    r = func_38c0_r(s1, 0, D_38c0_z[0x4C / 4], D_38c0_z[0x54 / 4],
                    o, *(int*)(s1 + 0x80),
                    *(float*)(*(char**)(s1 + 0x98) + 0xC4),
                    *(float*)(*(char**)(s1 + 0x98) + 0xCC),
                    (D_38c0_z[0x50 / 4] | 1) | 0x10000 | 0x40000, 0x1B);
    r2 = (char*)func_00000000(s1, 2, *(int*)(s1 + 0x80), r, o);
    row = (int*)((char*)&D_00000000 + a1 * 0x1C);  /* D_w placeholder */
    res = (char*)func_00000000(s1, row, r2);
    *(char**)(r + 0x8DC) = res;
}

/* func_000039D8 - verified structural decode (0x1A0, 104 insns,
 * multi-stage builder chain). MEMBER of the func_000038C0 /
 * func_00003734 builder-chain family (same alloc-0x80 + tagged-arg
 * func_00000000 builder pattern; flags = global|1|0x10000|0x40000,
 * tag 0x1B; s2 source, cfg = s2->0x98).
 * Struct-typing reference (same family as func_000038C0 /
 * func_00003734): s2->0x80 (128) pass-through handle; s2->0x98
 * (152) -> cfg with f32 0xC4 (196) / 0xCC (204) builder params;
 * D_g (a global, lui/addiu 0) ->0x50 (80) base flags OR'd with
 * 0x1|0x10000|0x40000 feature bits, ->0x4C (76) / ->0x54 (84)
 * global build params. o1/o2 = 0x80-byte builder objects
 * cross-wired through the tagged func_00000000 builder. a1 indexes
 * a 0x1C-stride descriptor table (per the family pattern).
 * Caps <80: ~12 reloc + FP stack-arg passing + packed-flag lui/ori
 * + a1*0x1C index + list-link beql.
 *
 * 2026-06-02 FULL CHAIN DECODED 37.4->54.3% (+16.9pp): r1=builder(flags=
 * Dg->0x50|1|0x10000|0x40000, tag 0x1B); r2=builder with BYTE args (Dg->0x181/
 * 0x183 lbu, flags2=Dg->0x182|0x8008, tag 0x14); r2->0x38C=0; b3=builder(s2,2,
 * s2->0x80,r1,o1); row=&D+a1*0x1C; f1=builder(s2,row,b3); r1/r2->0x8DC=f1. Both
 * register calls are func(Dg,o1)/func(Dg,o2) [not o2,o1]. All field stores
 * byte-match; residual ~46% = regalloc (saved-reg renumber + &D const-fold).
 * Later closed to objdiff-100 (merged o1/o2 $s1 web + address-escaped o1).
 * Unwrapped EXACT 2026-10-16 (run 96): every reloc resolves to 0. */
/* typed-float proto (0x0-alias): 10-arg builders, args 7,8 single floats. */
extern char *func_39d8_r(char *, int, int, int, char *, int, float, float, int, int);
extern unsigned char D_39d8_g[];  /* D_g placeholder alias — array-typed so base lives in $s0 */
void func_000039D8(char *s2, int a1) {
    char *o1;
    char *o2;
    char *r1, *r2, *b3, *f1;
    o2 = (char*)func_00000000(0x80);  /* o2 is the MERGED $s1 web: o1-carrier, then o2 */
    if (o2 != 0) {
        func_00000000(o2, 1);   /* alloc-fail SKIPS ONLY THIS (target beqz +4, 38C0 recipe) */
    }
    if (0) { func_00000000(&o1); }  /* address-escape: o1 memory-homed at 0x4C */
    o1 = o2;  /* carrier ($s1) -> home store at the branch join */
    o2 = (char*)func_00000000(0x80);
    if (o2 != 0) {
        func_00000000(o2, 0);
    }
    func_00000000(D_39d8_g, o1);
    func_00000000(D_39d8_g, o2);
    func_00000000(D_39d8_g, o1, o2);
    r1 = func_39d8_r(s2, 0, *(int*)(D_39d8_g + 0x4C), *(int*)(D_39d8_g + 0x54),
                     o1, *(int*)(s2 + 0x80),
                     *(float*)(*(char**)(s2 + 0x98) + 0xC4),
                     *(float*)(*(char**)(s2 + 0x98) + 0xCC),
                     (*(int*)(D_39d8_g + 0x50) | 1) | 0x10000 | 0x40000, 0x1B);
    r2 = func_39d8_r(s2, 0, *(unsigned char*)(D_39d8_g + 0x181), *(unsigned char*)(D_39d8_g + 0x183),
                     o2, *(int*)(s2 + 0x80),
                     *(float*)(*(char**)(s2 + 0x98) + 0xC4),
                     *(float*)(*(char**)(s2 + 0x98) + 0xCC),
                     (*(unsigned char*)(D_39d8_g + 0x182) | 0x8) | 0x8000, 0x14);
    *(int*)(r2 + 0x38C) = 0;
    b3 = (char*)func_00000000(s2, 2, *(int*)(s2 + 0x80), r1, o1);
    f1 = (char*)func_00000000(s2, (char*)&D_00000000 + a1 * 0x1C, b3);
    *(char**)(r1 + 0x8DC) = f1;
    *(char**)(r2 + 0x8DC) = f1;
}

/* func_00003B78 - verified structural decode (0x1C4, 113 insns,
 * multi-stage builder chain). MEMBER of the func_000038C0 /
 * func_00003734 / func_000039D8 builder-chain family (same
 * alloc-0x80 + tagged-arg func_00000000 builder pattern).
 * Struct-typing reference (same family as func_000038C0 /
 * func_00003734 / func_000039D8): s1->0x80 (128) pass-through
 * handle; s1->0x98 (152) -> cfg with f32 0xC4 (196) / 0xCC (204)
 * builder params. CFG = a global byte-config block (lui/addiu 0):
 * CFG->0x181 (385) / 0x182 (386) / 0x183 (387) u8 fields supply
 * arg2/flag-base/arg3 (0x182 OR'd with 0x8001|0x10000|0x40000
 * feature bits). o = the 0x80-byte builder object; a1 indexes the
 * 0x1C-stride descriptor table per the family pattern.
 * Caps <80: ~12 reloc + FP stack-arg passing + packed-flag lui/ori
 * (with global u8 inputs) + a1*0x1C index + list-link beql.
 *
 * 2026-06-02 FULL CHAIN DECODED: single-alloc variant.
 * r1=builder(byte args CFG->0x181/0x183, flags=CFG->0x182|0x8001|0x10000|
 * 0x40000, tag 0x1B); b2=builder(s1,0,s1->0x80,r1,o); b3=builder(s1,1,...);
 * f1=builder(s1,&D+a1*0x1C,b2); b2->0x14C=85.0f; then a CONDITIONAL o2=alloc:
 * if(o2){ func(o2,1); big=builder(0,o2,r1,f1,b2,b3); func(s1->0x84+0x10,big);
 * linked-set finalizer on big (big->0x14/0x4 vs s1->0x84); } r1->0x8DC=f1.
 *
 * 2026-06-23 RAW-BYTE-POSITION RE-MEASURE + decode progress 20.4% -> 52.2%
 * (NOT 82.8% -- the prior figure was windowed/fuzzy which masks reloc + the
 * sp-slot-offset cascade). Real levers applied (all kept):
 *   (1) frame now -0x60 (matched) via spilling the conditional v1_84 local.
 *   (2) SHARED CFG BASE (IDO_CODEGEN deferred-assign+named-locals lever):
 *       register u8 *C; C=CFG; c182=C[0x182]; ... uses C[0x181]/C[0x183]
 *       -> one held base reg + 3 lbu (was 3 separate lui/addiu). +28pp.
 *   (3) v1_84 = s1->0x84 read AFTER the big-builder call (target order).
 * RESIDUAL ~48% = register-COLORING cascade: held base C lands in t1/t9
 * not target's $v1, cascading every spill-slot assignment off by 8 (same
 * 0x60 frame, different slot numbering) + as1 tail scheduling. Permuter
 * RAN (base score 1215, every mutation +score / 1-error) -> CANNOT crack;
 * objdiff normalizes sp-offsets so it is blind to the slot cascade. This is
 * the documented coloring-cap class for the 38C0/3734/39D8/3B78 family.
 * 2026-10-20 (run 113 agent-c): EXACT. 3734/38C0 recipe backport plus one new
 * lever:
 *  (1) void-typed alias for the second CFG reg call (frees $v0 so cfg colours
 *      v0 and the CFG base v1, as in target);
 *  (2) o / o2 / big merged into ONE $s0 web; register C dropped (the array-
 *      typed CFG_3b78 base is CSE'd into v1 by itself) -> r1/b2/b3/f1 spill
 *      homes 0x58..0x4C exact;
 *  (3) four unused volatile pads between f1 and v1_84 = the ghost words that
 *      put v1_84's home at 0x38;
 *  (4) NEW: the flag constant spelled `| 1 | 0x8000` (not `| 0x8001`): the
 *      folded intermediate burns one t-ring register, so the lbu/ori/or chain
 *      numbers t8/t9/t0/t1 like the target instead of t7.. (every later
 *      temp was one ring slot early). See docs/IDO_CODEGEN.md
 *      #split-or-constant-ring-burn-3b78. */
extern char *func_3b78_r(char *, int, int, int, char *, int, float, float, int, int);
extern unsigned char CFG_3b78[];
extern void func_00000000_3B78_v(void *, void *, int);
void func_00003B78(char *s1, int a1) {
    char *o;
    char *r1, *b2, *b3, *f1;
    volatile int pad1, pad2, pad3, pad4;  /* ghost home words (phantom-slot lever) */
    int v1_84;
    o = (char*)func_00000000(0x80);
    if (o != 0) {
        func_00000000(o, 0);
    }
    func_00000000(CFG_3b78, o);
    func_00000000_3B78_v(CFG_3b78, o, 0);
    r1 = func_3b78_r(s1, 0,
                     CFG_3b78[0x181],
                     CFG_3b78[0x183],
                     o, *(int*)(s1 + 0x80),
                     *(float*)(*(char**)(s1 + 0x98) + 0xC4), *(float*)(*(char**)(s1 + 0x98) + 0xCC),
                     (CFG_3b78[0x182] | 1 | 0x8000) | 0x10000 | 0x40000, 0x1B);
    b2 = (char*)func_00000000(s1, 0, *(int*)(s1 + 0x80), r1, o);
    b3 = (char*)func_00000000(s1, 1, *(int*)(s1 + 0x80), r1, o);
    f1 = (char*)func_00000000(s1, (char*)&D_00000000 + a1 * 0x1C, b2);
    *(float*)(b2 + 0x14C) = 85.0f;
    o = (char*)func_00000000(0x80);
    if (o != 0) {
        func_00000000(o, 1);
    }
    o = (char*)func_00000000(0, o, r1, f1, b2, b3);
    v1_84 = *(int*)(s1 + 0x84);
    func_00000000(v1_84 + 0x10, o);
    if (*(int*)(o + 0x14) != 0) {
        *(int*)(o + 0x4) = 1;
    }
    *(int*)(o + 0x14) = v1_84;
    *(char**)(r1 + 0x8DC) = f1;
}

/* func_00003D3C - verified structural decode (0x1C4, 113 insns,
 * multi-stage builder chain). MEMBER of the func_000038C0 /
 * func_00003734 / func_000039D8 / func_00003B78 builder-chain
 * family (same alloc-0x80 x2 + register-&D + tagged-arg
 * func_00000000 builder pattern; variant params).
 * Struct-typing reference (same family as func_000038C0 et al.):
 * s1->0x80 (128) pass-through handle; s1->0x98 (152) -> cfg with
 * f32 0xC4 (196) / 0xCC (204) builder params; o1/o2 = 0x80-byte
 * builder objects cross-wired through the tagged func_00000000
 * builder (saved at sp+0x4C across the alloc pair); a1 indexes
 * the 0x1C-stride descriptor table per the family pattern. */
/* EXACT (agent-c run 115). Identities from the bootup.uso TextReloc: alloc =
 * func_055750, ctor = func_07BA00, list adds = func_011CA4 / func_0128AC on
 * import_000200FC (= &D_00000000, whose +0x4C/0x50/0x54/0x17D/0x17F fields
 * are read through the same held base), spawn = func_00003638 (x2), link =
 * func_000031C0, attach = func_000034E8 with &D_007294 + a1 * 0x1C (zero
 * alias bu_3d3c_d7294). The two new-expression results go through ONE temp t (target
 * s2) with o1 copied out to the frame; the camera reads are inline per arg
 * (a cfg local hoists the load); decl order puts o1 at sp+76 and r at sp+64
 * (seven declared locals, cfg unused but it holds the 0x58 frame). */
extern char *func_00000000_3d3cm(char *, int, int, int, char *, int, float, float, int, int);
extern char bu_3d3c_d7294[];
void func_00003D3C(char *s1, int a1) {
    char *o2, *o3, *o1, *o4, *t, *r, *cfg;
    t = (char*)func_00000000(0x80);
    if (t != 0) func_00000000(t, 1);
    o1 = t;
    t = (char*)func_00000000(0x80);
    if (t != 0) func_00000000(t, 0);
    o2 = t;
    func_00000000(&D_00000000, o1);
    func_00000000(&D_00000000, o2);
    func_00000000(&D_00000000, o1, 0);
    r = func_00000000_3d3cm(s1, 0, *(int*)((char*)&D_00000000 + 0x4C), *(int*)((char*)&D_00000000 + 0x54),
                             o1, *(int*)(s1 + 0x80), *(float*)(*(char**)(s1 + 0x98) + 0xC4) - 500.0f, *(float*)(*(char**)(s1 + 0x98) + 0xCC),
                             *(int*)((char*)&D_00000000 + 0x50) | 1 | 0x10000 | 0x40000, 0x1B);
    o3 = func_00000000_3d3cm(s1, 2, *(unsigned char*)((char*)&D_00000000 + 0x17D),
                              *(unsigned char*)((char*)&D_00000000 + 0x17F), o2, *(int*)(s1 + 0x80),
                              *(float*)(*(char**)(s1 + 0x98) + 0xC4) + 500.0f, *(float*)(*(char**)(s1 + 0x98) + 0xCC), 0x8004, 0x1D);
    *(int*)(o3 + 0xA14) = 0x989680;
    *(char**)(r + 0x908) = o3;
    *(char**)(o3 + 0x908) = r;
    o4 = (char*)func_00000000(s1, bu_3d3c_d7294 + a1 * 0x1C, func_00000000(s1, 2, *(int*)(s1 + 0x80), r, o1));
    *(char**)(r + 0x8DC) = o4;
    if (1) { }
    if (1) { }
    *(char**)(o3 + 0x8DC) = o4;
}

/* func_00003F00 - verified structural decode (0x21C, 135 insns,
 * multi-stage builder chain). MEMBER of the func_000038C0 /
 * func_00003734 / func_000039D8 / func_00003B78 / func_00003D3C
 * builder-chain family (same alloc-0x80 x2 + register-&D +
 * tagged-arg func_00000000 builder pattern; variant params).
 * Struct-typing reference (same family as func_000038C0 et al.):
 * s1->0x80 (128) pass-through handle; s1->0x98 (152) -> cfg with
 * f32 0xC4 (196) builder param, here biased by -500.0f
 * (0x43FA0000); D_g (a global) ->0x4C (76)/0x50 (80)/0x54 (84)
 * global build params; tag 0x15. o1/o2 = 0x80-byte builder
 * objects (init types 1 and 2) cross-wired via the tagged builder;
 * a1 indexes the 0x1C-stride descriptor table per the family.
 */
/* EXACT (agent-c run 115). Identities from the bootup.uso TextReloc (same
 * chain as func_00003D3C): alloc func_055750, ctor func_07BA00, list adds
 * func_011CA4 x2 + func_0129A0 on import_000200FC, spawn func_00003638 x2,
 * link func_000031C0 x2, attach func_000034E8 x2 on &D_007294 + a1 * 0x1C
 * (zero alias), r->0xC = D_000074E4 / D_000074EC (baked). No early returns:
 * the two new-expression results share temp t and are copied into an object
 * ARRAY o[2] (slots 104/108, re-read from memory for the link calls); b3/f1
 * are their own variables (sharing t's register drops the param to s2); the
 * if (1) { } after the copies lowers t below r1 (s3 vs s2). */
extern char D_000074E4[], D_000074EC[], bu_3f00_d7294[];
extern char *func_00000000_3d3cm(char *, int, int, int, char *, int, float, float, int, int);
void func_00003F00(char *s1, int a1) {
    char *o[2], *t, *r1, *r2, *x, *y, *b4, *f2, *row;
    t = (char*)func_00000000(0x80);
    if (t != 0) func_00000000(t, 1);
    o[0] = t;
    t = (char*)func_00000000(0x80);
    if (t != 0) func_00000000(t, 2);
    o[1] = t;
    if (1) { }
    func_00000000(&D_00000000, o[0]);
    func_00000000(&D_00000000, t);
    func_00000000(&D_00000000, o[0], t);
    r1 = func_00000000_3d3cm(s1, 0, *(int*)((char*)&D_00000000 + 0x4C), *(int*)((char*)&D_00000000 + 0x54),
                             o[0], *(int*)(s1 + 0x80), *(float*)(*(char**)(s1 + 0x98) + 0xC4) - 500.0f, *(float*)(*(char**)(s1 + 0x98) + 0xCC),
                             *(int*)((char*)&D_00000000 + 0x50), 0x15);
    r2 = func_00000000_3d3cm(s1, 1, *(int*)((char*)&D_00000000 + 0x58), *(int*)((char*)&D_00000000 + 0x60),
                             t, *(int*)(s1 + 0x80), *(float*)(*(char**)(s1 + 0x98) + 0xC4) + 500.0f, *(float*)(*(char**)(s1 + 0x98) + 0xCC),
                             *(int*)((char*)&D_00000000 + 0x5C), 0x15);
    *(char**)(r1 + 0xC) = D_000074E4;
    *(char**)(r2 + 0xC) = D_000074EC;
    *(char**)(r1 + 0x908) = r2;
    *(char**)(r2 + 0x908) = r1;
    x = (char*)func_00000000(s1, 2, *(int*)(s1 + 0x80), r1, o[0]);
    b4 = (char*)func_00000000(s1, 2, *(int*)(s1 + 0x80), r2, o[1]);
    *(float*)(x + 0x14C) = 55.0f;
    *(float*)(b4 + 0x14C) = 55.0f;
    row = bu_3f00_d7294 + a1 * 0x1C;
    y = (char*)func_00000000(s1, row, x);
    f2 = (char*)func_00000000(s1, row + 0x1C, b4);
    *(char**)(r1 + 0x8DC) = y;
    *(char**)(r2 + 0x8DC) = f2;
    *(char**)(f2 + 0x1B8) = y;
}

/* func_0000411C - verified structural decode (0x2B8, 174 insns,
 * randomized multi-stage builder chain). MEMBER of the
 * func_000038C0 / func_00003734 / func_000043D4 builder-chain
 * family (alloc-0x80 x2 + register-&D + tagged-arg func_00000000
 * builder; rand-scaled params, same shape as func_000043D4).
 * Struct-typing reference (same family as func_000038C0 /
 * func_000043D4): o1/o2 = 0x80-byte builder objects (sp+0x68);
 * rand01() = func_00000000() 0..1 f32; func_000003F8+0x14C /
 * +0x150 = f32 scale consts (one -> int table index via trunc,
 * one -> float param); func_00000080+0x10 = descriptor table
 * indexed by idx*4; tag 0x58005; flag 0x3. Per-call random
 * descriptor selection (same as func_000043D4).
 * Caps <80: ~10 reloc (incl. RNG) + FP mul.s/trunc.w.s + idx*4
 * table index + cross-symbol refs + FP stack-arg.
 *
 * 2026-06-02 FULL CHAIN DECODED 27.0->62.9% (RNG-variant of 43D4 + 3B78 tail):
 * r1/r2 RNG-builders (idx1/idx2, descriptor rowN, cfg->0xC4 -/+ 500, flags
 * 0x58005/0x48024, tag 0x1B) + 0x908 cross-link; b3=builder(s1,0,s1->0x80,r1,
 * o1); b4=builder(s1,1,...); f1=builder(s1,&D+a1*0x1C,b3); conditional o3=alloc
 * { func(o3,1); big=builder(0,o3,r1,f1,b3,b4); func(s1->0x84+0x10,big);
 * linked-set finalizer on big; } r1/r2->0x8DC=f1. All field stores byte-match.
 *
 * 2026-06-23 62.9->69.6% (correct-C bugfixes, agent-e):
 *  (1) rand callee returns FLOAT in $f0 — K&R int-return proto emitted spurious
 *      mtc1/cvt.s.w per call; fixed with `func_411c_rf` float-RETURN 0x0-alias
 *      (docs/MATCHING_WORKFLOW K&R-float-return). +6.5pp.
 *  (2) finalizer reuses s1->0x84 via single `int p84` local (was reloaded twice).
 *
 * 2026-07-30 69.6->88.0 (agent-g structural pass; retracts most of the old
 *  "~30% not C-fixable" verdict):
 *  DECODE FIXES: (a) NO early returns — `if (obj) init(obj,N);` guards ONLY the
 *  init call, flow continues with obj==0 (all three allocs); (b) r2 call a1=2
 *  (not 1); (c) big-block is UNCONDITIONAL (only init(o3,1) guarded);
 *  (d) descriptor table offset is func_00000080+0x10 (not +0x90).
 *  LEVERS: o1/o2/o3/big = ONE serial temp `p` (s0, target's or-s0 beq delays);
 *  homed locals f1/b4/b3/idx2/idx1/r1/r2/o1 = local ARRAY A[8] (IDO never
 *  promotes array elements — struct fields DO get scalarized/promoted, plain
 *  locals stole s-regs); frame gap filled with volatile padA[1] above / padB[2]
 *  below A (single volatile int gets trimmed; arrays don't); per-region
 *  `q = A[5]` temps reproduce block-granular r1 reloads + a3 reuse; E68-style
 *  reloc-identity split (&func_0000057C-0x34 / &func_00000044+0x4C) busts the
 *  in-TU pool/table base CSE across the 2 uses (frees s1 -> param, kills s2).
 *  RESIDUAL (~12%, hard cap, BF8C class): expected pool/table loads are
 *  DIRECT-form `lui at,%hi(sym+K); lwc1 %lo` — in-TU-defined-function base
 *  means our C spells pointer arith -> addiu-form (+1 insn per use) and the
 *  2nd-use anchors have wrong reloc name/lo16 (semantically same address);
 *  downstream f10/f18 ring swap + mfc1 t8-vs-a2 idx-temp coloring couple to
 *  that. Struct-cast ((P*)func)->field does NOT fold either. */
/* EXACT (agent-c run 115). The func_000043D4 shape (reloc identities,
 * per-site pinned aliases for the RoData scales +0x544/+0x548 and the 0x90
 * descriptor table, zero alias for the second list add, spawn results in
 * r[2], block temps for the cross link) plus the tail: a third new-expression
 * (ctor flag 1) passed to func_002420(0, t, r[0], f, b3, b4), linked into
 * s1->0x84 + 0x10 by func_07ACE0. */
extern char bu_411c_d7294[], bu_411c_d00_b;
extern int bu_411c_t90[];
extern f32 bu_411c_k544, bu_411c_k548;
extern f32 func_00000000_411c_f(void);
extern char *func_00000000_3d3cm(char *, int, int, int, char *, int, float, float, int, int);
void func_0000411C(char *s1, int a1) {
    char *t, *o1, *r[2];
    int idx1, idx2;
    char *b3, *b4, *f;
    int unused[4];  /* frame words between f (sp+76) and the p84 home (sp+56) */
    char *p84;
    t = (char*)func_00000000(0x80);
    if (t != 0) func_00000000(t, 0);
    o1 = t;
    t = (char*)func_00000000(0x80);
    if (t != 0) func_00000000(t, 0);
    func_00000000(&D_00000000, o1);
    func_00000000(&bu_411c_d00_b, t);
    idx1 = (int)(func_00000000_411c_f() * bu_411c_k544);
    idx2 = (int)(func_00000000_411c_f() * bu_411c_k548 + (float)idx1 + 1.0f) % 3;
    r[0] = func_00000000_3d3cm(s1, 0, idx1, bu_411c_t90[idx1], o1, *(int*)(s1 + 0x80),
                             *(float*)(*(char**)(s1 + 0x98) + 0xC4) - 500.0f, *(float*)(*(char**)(s1 + 0x98) + 0xCC), 0x58005, 0x1B);
    r[1] = func_00000000_3d3cm(s1, 2, idx2, bu_411c_t90[idx2], t, *(int*)(s1 + 0x80),
                             *(float*)(*(char**)(s1 + 0x98) + 0xC4) + 500.0f, *(float*)(*(char**)(s1 + 0x98) + 0xCC), 0x48024, 0x1B);
    { char *a = r[0], *b = r[1]; *(char**)(a + 0x908) = b; *(char**)(b + 0x908) = a;
    b3 = (char*)func_00000000(s1, 0, *(int*)(s1 + 0x80), a, o1); }
    b4 = (char*)func_00000000(s1, 1, *(int*)(s1 + 0x80), r[0], o1);
    f = (char*)func_00000000(s1, bu_411c_d7294 + a1 * 0x1C, b3);
    t = (char*)func_00000000(0x80);
    if (t != 0) func_00000000(t, 1);
    t = (char*)func_00000000(0, t, r[0], f, b3, b4);
    p84 = *(char**)(s1 + 0x84);
    func_00000000(p84 + 0x10, t);
    if (*(int*)(t + 0x14) != 0) *(int*)(t + 0x4) = 1;
    *(char**)(t + 0x14) = p84;
    *(char**)(r[0] + 0x8DC) = f;
    *(char**)(r[1] + 0x8DC) = f;
}

/* func_000043D4 - verified structural decode (0x258, 150 insns,
 * multi-stage builder chain w/ randomized params). MEMBER of the
 * func_000038C0 / func_00003734 / func_000039D8 / func_00003B78
 * builder-chain family (alloc-0x80 x2 + register-&D + tagged-arg
 * func_00000000 builder).
 * Struct-typing reference (same family as func_000038C0 et al.):
 * o1/o2 = 0x80-byte builder objects (saved sp+0x58); rand01() =
 * func_00000000() returning a 0..1 f32; func_000003F8+0x154 /
 * +0x158 = f32 scale constants (one yields an int table index via
 * trunc, one a float param); func_00000080+0x10 = a descriptor
 * table indexed by idx*4; tags 0x8004 / 0x15 + flag bits drive the
 * builder. Randomized variant of the family.
 * (Historical notes below; see the EXACT comment for the final shape.)
 *
 *
 * 2026-06-02 FULL CHAIN DECODED 27.6->55.5% (+27.9pp): two rng() calls give
 * idx1=(int)(r*F3F8->0x154) and idx2=(int)(rr*F3F8->0x158 + idx1 + 1.0f) % 3;
 * row1/row2 = *(func_00000080 + idxN*4 + 0x90) (descriptor table); r1/r2 =
 * builder(s0, 0/1, idxN, rowN, oN, s0->0x80, cfg->0xC4 -/+ 500.0f, cfg->0xCC,
 * 0x8004/0x8024, 0x15); r1<->r2 0x908 cross-link; b3/b4=builder(s0,2,s0->0x80,
 * rN,oN); bN->0x14C=55.0f; rowF=&D+a1*0x1C; f1/f2=builder(s0,rowF[+0x1C],bN);
 * rN->0x8DC=fN. All field stores byte-match; residual ~45% = regalloc. */
/* EXACT (agent-c run 115). Same reloc-identified chain as func_00003D3C /
 * func_00003F00 (alloc, ctor, list add x2, RNG func_075EE4 x2, spawn
 * func_00003638 x2, link func_000031C0 x2, attach func_000034E8 x2). The
 * RoData scales (pool +0x54C / +0x550) and the descriptor table (0x90) are
 * per-site data aliases pinned to their real offsets, and the second list
 * add gets its own zero alias of import_000200FC, so nothing is held in an
 * s-reg. The two spawn results are an ARRAY r[2] (frame 80/84); the cross
 * link and the first link call go through block temps a/b (a = a3). */
extern char bu_43d4_d7294[], bu_43d4_d00_b;
extern int bu_43d4_t90[];
extern f32 bu_43d4_k54c, bu_43d4_k550;
extern f32 func_00000000_43d4_f(void);
extern char *func_00000000_3d3cm(char *, int, int, int, char *, int, float, float, int, int);
void func_000043D4(char *s0, int a1) {
    char *t, *o1, *r[2];
    int idx1, idx2;
    char *f1, *row, *b3, *b4;
    t = (char*)func_00000000(0x80);
    if (t != 0) func_00000000(t, 0);
    o1 = t;
    t = (char*)func_00000000(0x80);
    if (t != 0) func_00000000(t, 0);
    func_00000000(&D_00000000, o1);
    func_00000000(&bu_43d4_d00_b, t);
    idx1 = (int)(func_00000000_43d4_f() * bu_43d4_k54c);
    idx2 = (int)(func_00000000_43d4_f() * bu_43d4_k550 + (float)idx1 + 1.0f) % 3;
    r[0] = func_00000000_3d3cm(s0, 0, idx1, bu_43d4_t90[idx1], o1, *(int*)(s0 + 0x80),
                             *(float*)(*(char**)(s0 + 0x98) + 0xC4) - 500.0f, *(float*)(*(char**)(s0 + 0x98) + 0xCC), 0x8004, 0x15);
    r[1] = func_00000000_3d3cm(s0, 1, idx2, bu_43d4_t90[idx2], t, *(int*)(s0 + 0x80),
                             *(float*)(*(char**)(s0 + 0x98) + 0xC4) + 500.0f, *(float*)(*(char**)(s0 + 0x98) + 0xCC), 0x8024, 0x15);
    { char *a = r[0], *b = r[1]; *(char**)(a + 0x908) = b; *(char**)(b + 0x908) = a;
    b3 = (char*)func_00000000(s0, 2, *(int*)(s0 + 0x80), a, o1); }
    b4 = (char*)func_00000000(s0, 2, *(int*)(s0 + 0x80), r[1], t);
    *(float*)(b3 + 0x14C) = 55.0f;
    *(float*)(b4 + 0x14C) = 55.0f;
    row = bu_43d4_d7294 + a1 * 0x1C;
    f1 = (char*)func_00000000(s0, row, b3);
    b4 = (char*)func_00000000(s0, row + 0x1C, b4);
    *(char**)(r[0] + 0x8DC) = f1;
    *(char**)(r[1] + 0x8DC) = b4;
}

void func_0000462C(char *a0) {
    int scratch;
    func_000022E0(&scratch);
    func_0000231C((int*)(a0 + 0x10));
}

void func_0000465C(char *a0) {
    int scratch;
    func_000022E0(&scratch);
    func_00002358((int*)(a0 + 0x10));
}

void func_0000468C(char *a0) {
    int scratch;
    func_000022E0(&scratch);
    func_000023C8((int*)(a0 + 0x10));
}

void func_000046BC(char *a0) {
    int scratch;
    func_000022E0(&scratch);
    func_000022E0((int*)(a0 + 0x10));
}

extern char D_00000000;

/* func_000046EC: 36-insn (0x90) entry-list constructor. BYTE-EXACT, ported
 * from its masked-hash twin donor eddproc_uso_func_000003BC (byte-identical
 * body, 0 non-reloc word diffs). The donor's clean C form (p2/head/p1 decl
 * order + char*-cast field access + p1[0x40/4] re-read) reproduces the target's
 * $a0/$v1 regalloc + 0x28 frame that the prior node[]/head[] array-index NM
 * body (89.31% ceiling) couldn't. Was once force-matched via 22-entry
 * INSN_PATCH (banned 2026-05-23); this is the genuine C match. */
void *func_000046EC(int *arg0) {
    int *p2;
    int *head;
    int *p1;
    p1 = (int*)func_00000000(0x40);
    if (p1 != 0) {
        func_00000000(p1);
        *(int*)((char*)p1 + 0x28) = (int)&D_00000000;
        *(int*)((char*)p1 + 0x3C) = 0;
    }
    p2 = p1;
    p1 = arg0;
    head = (int*)p1[0x40 / 4];
    if ((int*)p1[0x40 / 4] != 0) {
        func_00000000((char*)p2 + 0x10, head);
        if (*(int*)((char*)head + 0x14) != 0) {
            *(int*)((char*)head + 0x4) = 1;
        }
        *(int*)((char*)head + 0x14) = (int)p2;
    }
    return p2;
}

/* func_0000477C - verified structural decode (0xE0, 56 insns,
 * slider/parameter registration). SIBLING of func_00006734 (same
 * registration-builder shape, here with float range/step params).
 * (reg = func_00000000; a3 = min/step as f32 bits, 5th/6th args =
 * max + flag on the stack at sp+0x10 / sp+0x14.)
 * Constants decoded: 0xC4FA0000 = -2000.0f, 0x44FA0000 = 2000.0f,
 * 0x3C23D70A = 0.01f, 0x3F800000 = 1.0f.
 * Struct-typing reference: D_000074F4/7500/7508/7510 = four
 * slider/param key data; bound object fields a0->0x2C (44) [range
 * +-2000.0, step 0], a0->0x34 (52) and a0->0x30 (48) [step 0.01,
 * max 1.0] - i.e. one wide unbounded-ish axis plus two fine 0..1
 * normalized params (entry 0 binds no field). Caps <80: 6x reloc +
 * 4x &D reloc + f32-const-as-(a3/stack)-arg passing.
 * INCLUDE_ASM remains build path. */
extern char D_000074F4, D_00007500, D_00007508, D_00007510;
/* MATCHED 2026-06-02: typed-float proto (0x0-alias) makes args 4,5 pass as
 * single floats (a3 reg + swc1 0x10 stack) instead of K&R double-promote. */
extern void func_477c_reg(void *, void *, void *, float, float, int);
void func_0000477C(char *a0) {
    func_00000000(&D_00000000, &D_000074F4, 0);
    func_477c_reg(&D_00000000, &D_00007500, a0 + 0x2C, -2000.0f,  2000.0f, 0);
    func_477c_reg(&D_00000000, &D_00007508, a0 + 0x34,    0.01f,    1.0f, 0);
    func_477c_reg(&D_00000000, &D_00007510, a0 + 0x30,    0.01f,    1.0f, 0);
    func_00000000(&D_00000000);
    func_00000000(a0);
}

/* func_0000485C - verified structural decode (0xB8, 46 insns,
 * FP transform/draw helper).
 * Struct-typing reference: a0 = object; a0->0x2C/0x30/0x34 (44/48/52)
 * f32 source vector components; global root *(&D_a) ->0x70 (112) ptr
 * to a transform/state obj whose ->0xA0/0xA4/0xA8 (160/164/168) is an
 * f32 Vec3 scaled per-component by Dc->0x128/0x12C/0x130 (296/300/304)
 * f32 scale constants; func_00000080+0x20 = static string/param datum
 * passed to the reloc draw/print helper. Caps <80: FP-heavy mul.s/
 * add.s chains + 4x func_00000000 reloc calls + 2x &D reloc +
 * func_00000080+0x20 cross-symbol data ref. INCLUDE_ASM remains build
 * path. */
extern void func_00000080();  /* used as data-symbol base */
/* EXACT via an IDO 7.1 NCC (C++) donor: src/bootup_uso/bootup_uso_ncc_485C.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder (agent-c run 116): V3 ctor
 * forwarding to set(), b2 in an inner block (allocated after b1's pool slots). */
void func_0000485C(char *a0) {
    volatile char *r = a0;
}

void func_00004914(char *a0, int a1, char *a2) {
    *(float*)(a0 + 0x60) = *(float*)(a2 + 0x30);
    *(float*)(a0 + 0x64) = *(float*)(a2 + 0x34);
    *(float*)(a0 + 0x68) = *(float*)(a2 + 0x38);
    *(int*)(a0 + 0x158) = a1;
    *(float*)(a0 + 0x64) += 100.0f;
}

/* func_00004948: camera transition tick keyed by obj->0x158 (snap / distance-table
 * blend / view-angle blend). EXACT via an IDO 7.1 NCC (C++) donor:
 * src/bootup_uso/bootup_uso_ncc_4948.cpp, REPLACE_FUNC_BODY-spliced over this
 * placeholder (agent-c run 125); the trailing all-zero pad word is a
 * SUFFIX_BYTES_FORCE word. */
extern float func_00000000_4948_ff(float); /* also used by a later NM body */
void func_00004948(char *obj) {
    volatile int r = 0;
}

void func_00004FF0(int *dst) {
    int buf[2];
    func_00000000(&D_00000000, buf, 4);
    *dst = buf[0];
}

void func_0000502C(int *dst) {
    int buf[2];
    func_00000000(&D_00000000, buf, 4);
    *dst = buf[0];
}

/* BYTE-EXACT 2026-05-31. 2-call wrapper: func(&D_7D94); func(0, a0). The
 * documented "3rd-arg-copy-vs-dead-home cap" was wrong — it never tried
 * struct-by-value. The target homes a1 to its outgoing slot (`sw a1,4(sp)`,
 * dead) which a single-int STRUCT-BY-VALUE arg (`func(0, *(struct OneI*)&a0)`)
 * reproduces exactly (same lever as the game_uso pair-homing, applied to one
 * int). Sibling func_000054A0 identical. See docs/IDO_CODEGEN.md
 * #feedback-ido-struct-by-value-homes-arg-pair. */
extern char D_00007D94;
struct OneI { int x; };
void func_00005068(int a0) {
    func_00000000(&D_00007D94);
    func_00000000(0, *(struct OneI*)&a0);
}

/* 33-insn / 0x84 init+alloc+populate.
 *   func_00000000(&D_00007DA4);       ; init call
 *   buf[4] = {0.0f, 0.0f, 0.0f, 0.0f}  ; 16-byte stack-local cleared via mtc1+swc1
 *   p = func_00000000(0x58);           ; alloc 0x58 bytes
 *   if (p == 0) return 0;
 *   func_00000000(p, a0, D[0], &buf);  ; init call (populates p from a0+template)
 *   p->[0x28] = (int)&D_00000000;      ; store data ptr at offset 0x28
 *   return p;
 *
 * Initial decode — multi-pass refinement expected. The two `lui+addiu` pairs
 * at 0x48/0x4C and 0x64/0x68 (loading &D_00000000 + offset) are USO
 * runtime-patched relocations; structural shape may diverge from C-only emit
 * per docs/PATTERNS.md uso-multi-placeholder-wrapper.
 *
 * 2026-05-07 progress (74.06% -> 77.33%, +3.27pp):
 *   (a) Re-ordered statements: init-call FIRST, then buf-clear, matching
 *       target asm's emit order.
 *   (b) Added `char pad[24]` (declared AFTER buf) to bump frame to -0x48
 *       AND place buf at 0x38(sp). Without pad: frame -0x30, buf at 0x20.
 *   (c) Switched if-NULL-return-0 to if-not-NULL-populate-then-return.
 *       Target's beqz-skip-populate-then-fallthrough-to-epilogue pattern
 *       only emits when the populate branch is the if-true arm.
 *
 * 2026-05-16 progress (77.33% -> 89.45%, +12.12pp):
 *   `volatile int saved_a0; saved_a0 = a0;` makes IDO reload a0 through
 *   t6 and spill it to 0x1C(sp) in the alloc-jal delay slot, matching the
 *   target's first hard scheduler shape. Per docs/IDO_CODEGEN.md
 *   feedback-ido-write-only-volatile-keeps-frame-without-deadload.
 *
 * Remaining cap (~11%): target's third call reloads a1 from 0x1C before
 * the D_00000000 load, spills v0 to 0x30(sp) before the jal, then fills the
 * jal delay slot with a defensive `sw a1, 0x4(sp)`. Best natural C still
 * spills a0/v0 to 0x18(sp) in the delay slot. Tested and rejected:
 * plain saved-a0 local (DCE/reloads caller slot), post-call `(void)saved_a0`
 * (adds dead `lw zero,0x1C(sp)`), scoped call_a1 temp and aliased saved_p
 * (both grow frame/buf to 0x50), and a 5-arg call (96.45% but wrong ABI
 * stack shape: ra at 0x1C and 5th arg at 0x10). Remaining signature matches
 * the documented low-offset precall-spill cap in docs/PATTERNS.md
 * feedback-game-uso-precall-spill-family / docs/IDO_CODEGEN.md
 * feedback-ido-volatile-local-frame-placement.
 *
 * 2026-05-18: had promoted to byte-exact via SUFFIX_BYTES_FORCE / INSN_PATCH
 * on the third-call scheduler cap; both mechanisms REMOVED 2026-05-23 as
 * match-faking. Function is at honest 89.45% NM now.
 *
 * 2026-06-21 (98.82% -> 99.09%): the pad local was 24 bytes, over-reserving
 * the frame by 8 (frame -80, buf at sp+0x40). Shrinking to `char pad[16]`
 * drops buf to sp+0x38 and the frame to -72 = target EXACTLY, collapsing 9
 * of the 17 diffs (all the frame/buf-offset ones). RESIDUAL 8 diffs are pure
 * spill-slot-coloring + register-renumber: target spills saved_a0 to the LOW
 * home 0x1C and p(v0) to the HIGH home 0x30, with the defensive `sw a1,0x4(sp)`
 * delay store going to the arg-build area (0x4); mine inverts the two home
 * slots (saved_a0 at 0x24, p at 0x1C) and puts the defensive store at 0x20,
 * plus the consequent t8-vs-t7 renumber. The bodies are otherwise instruction-
 * identical. This is the documented spill-slot-coloring cap — the
 * earlier-spilled value taking the lower home is an IDO allocator choice not
 * reachable by reordering C (declaration-order + embedded-assign permutations
 * all regress, retested). Stays NM at 99.09%. */
extern char D_00007DA4;
/* func_000050A0 — EXACT (33/33 post-link) 2026-07-03 agent-e. CRACKED the
 * documented "spill-slot-coloring cap" (wrap note 2026-06-21, 8 residual diffs).
 *
 * TWO levers, applied on top of the old volatile-saved_a0 body:
 *  1) SINGLE-INT STRUCT BY-VALUE ARG (PATTERNS Pair2-crack, 1-word variant):
 *     pass the saved param as `*(S50A0*)&saved_a0` (typedef struct {int v;})
 *     in arg position 2. This alone emitted the dead `sw a1,0x4(sp)` outgoing
 *     arg-shadow store in the jal delay (previously faked via `new_var =
 *     saved_a0` at a WRONG local slot 0x20), flipped the p-spill to v0@high
 *     slot, and fixed the t7->t8 renumber: 8 real diffs -> 5 (all layout -8).
 *  2) OVERSIZED BUFFER + &buf[1] (docs "8-byte dead slot below a stack buffer
 *     is a &buf[N] offset"): float buf[5] zeroing buf[1..4], passing &buf[1].
 *     Claims the 0x34 gap word; decl order buf,p,pad,saved_a0 then lands
 *     buf@0x38(zeros 0x38-0x44), p@0x30, pad 0x20-0x2F, saved_a0@0x1C,
 *     align@0x18 -> frame -0x48 exact. 5 -> 0.
 *
 * .o-level residual = 1 reloc-class word [5]: addiu a0,a0,%lo(D_00007DA4);
 * D_00007DA4=0x7DA4 in undefined_syms_auto.txt -> links to 24847DA4 = target.
 * All jals zero-target K&R func_00000000. No new wiring needed.
 */
typedef struct { int v; } S50A0;
int func_000050A0(int a0) {
  float buf[5];
  int *p;
  char pad[16];
  volatile int saved_a0;
  (void) pad;
  func_00000000(&D_00007DA4);
  buf[1] = (0, 0.0f);
  buf[2] = 0.0f;
  buf[3] = 0.0f;
  buf[4] = 0.0f;
  saved_a0 = a0;
  p = (int *) func_00000000(0x58);
  if (p != 0)
  {
    func_00000000(p, *(S50A0*)&saved_a0, *((int *) (&D_00000000)), &buf[1]);
    *((int *) (((char *) p) + 0x28)) = (int) (&D_00000000);
  }
  return (int) p;
}

/* func_00005124 - verified structural decode (0xB0, 44 insns,
 * alloc-cascade constructor with defensive-dead-check).
 * Struct-typing reference: object = 0x4C bytes; obj->0x28 (40) and
 * obj->0x48 (72) descriptor/vtable ptrs (&D, runtime-patched); arg
 * passed through (spilled sp+0x2C / sp+0x1C / sp+0x4) into the dead
 * arm's sub2. D_00007DB4 = named init datum (label/string). Caps <80:
 * alloc-cascade + defensive-dead-check (bnez after beqz makes the
 * 0x48 arm unreachable, documented ceiling) + 3-4 func_00000000
 * reloc calls + 3x &D descriptor-store relocs. To MATCH this family
 * (5124/51D4/5284/5334) you need: (a) the 3 distinct descriptor symbols
 * — they are lui-0+addiu-0 (reloc'd, addend 0) in the raw-word .s, so the
 * names live only in the USO reloc sidecar, not recoverable from the .s
 * alone; (b) the redundant-goto + dead-0x48-arm written out so -g -O2
 * keeps it; (c) the obj->0x28 double-write (&D_x then &D_y) kept (no
 * dead-store elim under -g).
 * 2026-09-04 RETRACTED: MATCHED at plain -O2 (100, ROM byte-identical) —
 * the "dead arm" is a get-or-create fallback on a memory-homed copy, the
 * descriptor stores are base-0 placeholder externs (same bytes), see
 * docs/IDO_CODEGEN.md#alloc-cascade-ctor-family-kit-5124. */
extern char D_00007DB4;
/* func_00005124 - alloc-cascade get-or-create constructor.
 * Reconstructed missing logic: the three descriptor stores
 * (obj->0x28 = A; obj->0x28 = B; obj->0x48 = C) target DISTINCT
 * registers (t9/t0/t1) -> three DISTINCT externs to defeat the
 * &D_00000000 address-CSE that collapses them onto one base reg.
 * The 0x48 second-alloc arm is provably-dead (kept under -g) and
 * is written as a get-or-create fall-through so IDO keeps it. */
extern char D_00007DB4;
extern char D_00000000_a;
extern char D_00000000_b;
extern char D_00000000_c;
/* 2026-09-04 (agent-f, pass 2): frame 0x38 slot layout = three `volatile int`
 * pads interleaved in decl order (pad,obj,b1,pad,o2,pad,b2 -> homes
 * 0x30/0x2C/0x24/0x1C); the beqz-on-v0 + delay-slot `sw a1,4(sp)` schedule
 * comes from folding the get-or-create into one condition
 * (`o2 != 0 || (o2 = alloc(0x48)) != 0`); tail addiu order = temp-first `t = &c`;
 * obj-reload placement = call+store on ONE source line. Standalone -O2 44/44 exact. */
typedef struct { int v; } Box5124;
void *func_00005124(Box5124 arg) {
    volatile int p0;
    char *obj;
    Box5124 b1;
    volatile int p1;
    char *o2;
    volatile int p2;
    Box5124 b2;
    func_00000000(&D_00007DB4);
    b1 = arg;
    obj = (char *)func_00000000(0x4C);
    if (obj != 0) {
        b2 = b1;
        o2 = obj;
        if (o2 != 0 || (o2 = (char *)func_00000000(0x48)) != 0) {
            func_00000000(o2, b2, 0); *(char **)(o2 + 0x28) = &D_00000000_a; /* same-line: keeps obj reload after the addiu */
        }
        /* temp-first `t = &c` -> addiu(c) before addiu(b); whole tail on ONE line (line-number scheduling hint) */
        { char *t = &D_00000000_c; *(char **)(obj + 0x28) = &D_00000000_b; *(char **)(obj + 0x48) = t; }
    }
    return obj;
}

/* func_000051D4 - verified structural decode (0xB0, 44 insns).
 * BYTE-STRUCTURALLY IDENTICAL SIBLING of func_00005124 (same
 * alloc-cascade constructor with defensive-dead-check; the ONLY
 * difference is the init datum D_00007DC4 here vs D_00007DB4 there).
 * Struct-typing reference: identical to func_00005124 - object 0x4C
 * bytes, obj->0x28 (40) / obj->0x48 (72) descriptor/vtable ptrs (&D
 * runtime-patched). Caps <80: alloc-cascade + defensive-dead-check +
 * 3-4 reloc calls + 3x &D-store reloc. MATCHED 2026-09-04 (100, ROM-exact; kit in docs/IDO_CODEGEN.md#alloc-cascade-ctor-family-kit-5124).
 * NM body captures live behavior only — dead-arm artifacts not
 * recreated (same convention as func_00005124). */
extern char D_00007DC4;
/* func_000051D4 - alloc-cascade get-or-create constructor.
 * Reconstructed missing logic (sibling of func_00005124, init datum
 * D_00007DC4): the three descriptor stores (obj->0x28 = A; obj->0x28
 * = B; obj->0x48 = C) target DISTINCT registers (t9/t0/t1) -> three
 * DISTINCT externs to defeat the &D_00000000 address-CSE that
 * collapses them onto one base reg. The 0x48 second-alloc arm is
 * provably-dead (kept under -g) and is written as a get-or-create
 * fall-through so IDO keeps it. */
extern char D_00007DC4;
extern char D_00000000_a;
extern char D_00000000_b;
extern char D_00000000_c;
/* 2026-09-04: struct-by-value shape ported from func_00005124 (see its note). */
/* 2026-09-04 (agent-f, pass 2): frame 0x38 slot layout = three `volatile int`
 * pads interleaved in decl order (pad,obj,b1,pad,o2,pad,b2 -> homes
 * 0x30/0x2C/0x24/0x1C); the beqz-on-v0 + delay-slot `sw a1,4(sp)` schedule
 * comes from folding the get-or-create into one condition
 * (`o2 != 0 || (o2 = alloc(0x48)) != 0`); tail addiu order = temp-first `t = &c`;
 * obj-reload placement = call+store on ONE source line. Standalone -O2 44/44 exact. */
typedef struct { int v; } Box51D4;
void *func_000051D4(Box51D4 arg) {
    volatile int p0;
    char *obj;
    Box51D4 b1;
    volatile int p1;
    char *o2;
    volatile int p2;
    Box51D4 b2;
    func_00000000(&D_00007DC4);
    b1 = arg;
    obj = (char *)func_00000000(0x4C);
    if (obj != 0) {
        b2 = b1;
        o2 = obj;
        if (o2 != 0 || (o2 = (char *)func_00000000(0x48)) != 0) {
            func_00000000(o2, b2, 0); *(char **)(o2 + 0x28) = &D_00000000_a; /* same-line: keeps obj reload after the addiu */
        }
        /* temp-first `t = &c` -> addiu(c) before addiu(b); whole tail on ONE line (line-number scheduling hint) */
        { char *t = &D_00000000_c; *(char **)(obj + 0x28) = &D_00000000_b; *(char **)(obj + 0x48) = t; }
    }
    return obj;
}

/* func_00005284 - verified structural decode (0xB0, 44 insns).
 * THIRD IDENTICAL SIBLING of the func_00005124 / func_000051D4
 * alloc-cascade-ctor family (instruction-for-instruction identical
 * except the init datum D_00007DD4 and local label addresses).
 * Struct-typing reference: identical to func_00005124 — object 0x4C
 * bytes, obj->0x28 (40) / obj->0x48 (72) descriptor ptrs (&D runtime-
 * patched). D_00007DD4 = named init datum (family: D_00007DB4/DC4/DD4
 * at +0x10 stride). Caps <80: alloc-cascade + defensive-dead-check +
 * 3-4 reloc calls + 3x &D-store reloc. MATCHED 2026-09-04 (100, ROM-exact; kit in docs/IDO_CODEGEN.md#alloc-cascade-ctor-family-kit-5124).
 * NM body captures live behavior only — dead-arm artifacts not
 * recreated. */
extern char D_00007DD4;
/* func_00005284 - alloc-cascade get-or-create constructor.
 * THIRD IDENTICAL SIBLING of func_00005124 / func_000051D4
 * (instruction-for-instruction identical except the init datum
 * D_00007DD4 and local label addresses).
 * Reconstructed missing logic: the three descriptor stores
 * (obj->0x28 = A; obj->0x28 = B; obj->0x48 = C) target DISTINCT
 * registers (t9/t0/t1) -> three DISTINCT externs to defeat the
 * &D_00000000 address-CSE that collapses them onto one base reg.
 * The 0x48 second-alloc arm is provably-dead (kept under -g) and
 * is written as a get-or-create fall-through so IDO keeps it. */
extern char D_00007DD4;
/* 2026-09-04 (agent-f, pass 2): frame 0x38 slot layout = three `volatile int`
 * pads interleaved in decl order (pad,obj,b1,pad,o2,pad,b2 -> homes
 * 0x30/0x2C/0x24/0x1C); the beqz-on-v0 + delay-slot `sw a1,4(sp)` schedule
 * comes from folding the get-or-create into one condition
 * (`o2 != 0 || (o2 = alloc(0x48)) != 0`); tail addiu order = temp-first `t = &c`;
 * obj-reload placement = call+store on ONE source line. Standalone -O2 44/44 exact. */
typedef struct { int v; } Box5284;
void *func_00005284(Box5284 arg) {
    volatile int p0;
    char *obj;
    Box5284 b1;
    volatile int p1;
    char *o2;
    volatile int p2;
    Box5284 b2;
    func_00000000(&D_00007DD4);
    b1 = arg;
    obj = (char *)func_00000000(0x4C);
    if (obj != 0) {
        b2 = b1;
        o2 = obj;
        if (o2 != 0 || (o2 = (char *)func_00000000(0x48)) != 0) {
            func_00000000(o2, b2, 0); *(char **)(o2 + 0x28) = &D_00000000_a; /* same-line: keeps obj reload after the addiu */
        }
        /* temp-first `t = &c` -> addiu(c) before addiu(b); whole tail on ONE line (line-number scheduling hint) */
        { char *t = &D_00000000_c; *(char **)(obj + 0x28) = &D_00000000_b; *(char **)(obj + 0x48) = t; }
    }
    return obj;
}

/* func_00005334 - verified structural decode (0xB4, 45 insns).
 * NEAR-SIBLING of the func_00005124/51D4/5284 alloc-cascade-ctor +
 * defensive-dead-check family. Variant: object size 0x50 (vs 0x4C),
 * init datum D_00007DE8, one extra insn (0xB4 vs 0xB0).
 * Struct-typing reference: object = 0x50 bytes (this variant);
 * obj->0x28 (40) / obj->0x48 (72) descriptor ptrs (&D runtime-
 * patched). Caps <80: alloc-cascade + defensive-dead-check + 3-4
 * reloc + 3x &D-store reloc. MATCHED 2026-09-04 (100, ROM-exact; kit in docs/IDO_CODEGEN.md#alloc-cascade-ctor-family-kit-5124). NM body
 * captures live behavior only — dead-arm artifacts not recreated. */
extern char D_00007DE8;
/* func_00005334 - alloc-cascade get-or-create constructor (0x50 variant).
 * Reconstructed to the func_00005124 family shape: object 0x50 bytes,
 * init datum D_00007DE8. Four descriptor stores target DISTINCT
 * registers (t9 -> obj->0x28; t0 -> obj->0x28; t1 -> obj->0x4C; zero ->
 * obj->0x48) so the &D_00000000 address-CSE does not collapse them onto
 * one base register -> distinct lui/addiu per store. The 0x48 second-
 * alloc arm is provably dead (kept under -g) and written as a get-or-
 * create fall-through so IDO retains it. */
extern char D_00007DE8;
extern char D_00000000_a;
extern char D_00000000_b;
extern char D_00000000_c;
/* 2026-09-04 (agent-f, pass 2): frame 0x38 slot layout = three `volatile int`
 * pads interleaved in decl order (pad,obj,b1,pad,o2,pad,b2 -> homes
 * 0x30/0x2C/0x24/0x1C); the beqz-on-v0 + delay-slot `sw a1,4(sp)` schedule
 * comes from folding the get-or-create into one condition
 * (`o2 != 0 || (o2 = alloc(0x48)) != 0`); tail addiu order = temp-first `t = &c`;
 * obj-reload placement = call+store on ONE source line. Standalone -O2 44/44 exact. */
typedef struct { int v; } Box5334;
void *func_00005334(Box5334 arg) {
    volatile int p0;
    char *obj;
    Box5334 b1;
    volatile int p1;
    char *o2;
    volatile int p2;
    Box5334 b2;
    func_00000000(&D_00007DE8);
    b1 = arg;
    obj = (char *)func_00000000(0x50);
    if (obj != 0) {
        b2 = b1;
        o2 = obj;
        if (o2 != 0 || (o2 = (char *)func_00000000(0x48)) != 0) {
            func_00000000(o2, b2, 0); *(char **)(o2 + 0x28) = &D_00000000_a; /* same-line: keeps obj reload after the addiu */
        }
        /* temp-first `t = &c` -> addiu(c) before addiu(b); whole tail on ONE line (line-number scheduling hint) */
        { char *t = &D_00000000_c; *(char **)(obj + 0x28) = &D_00000000_b; *(char **)(obj + 0x4C) = t; *(int *)(obj + 0x48) = 0; }
    }
    return obj;
}

/* func_000053E8 - verified structural decode (0xB8, 46 insns).
 * NEAR-SIBLING of the func_00005124/51D4/5284/5334 alloc-cascade-ctor
 * family. Variant: object 0x50 (like func_00005334), init datum
 * D_00007DFC, 0xB8 (one extra descriptor store vs 5334's 0xB4; uses
 * t2 reg where 5124 uses t1).
 * Struct-typing reference: object = 0x50 bytes; obj->0x28 (40) /
 * obj->0x48 (72) descriptor ptrs (&D runtime-patched); D_00007DFC =
 * named init datum (family data run D_00007DB4/DC4/DD4/DE8/DFC).
 * Caps <80: alloc-cascade + defensive-dead-check + 3-4 reloc calls +
 * &D-store reloc. MATCHED 2026-09-04 (100, ROM-exact; kit in docs/IDO_CODEGEN.md#alloc-cascade-ctor-family-kit-5124). NM body captures
 * live behavior only — dead-arm artifacts not recreated. */
extern char D_00007DFC;
/* func_000053E8 - alloc-cascade get-or-create constructor.
 * Reconstructed missing logic: the three descriptor stores
 * (obj->0x28 = A in the live arm; obj->0x28 = B; obj->0x4C = C in
 * the tail) target DISTINCT registers (t9/t0/t2) -> three DISTINCT
 * externs to defeat the &D_00000000 address-CSE that collapses them
 * onto one base reg. The obj->0x48 store is the immediate 1. The
 * 0x48 second-alloc arm is provably-dead (kept under -g) and is
 * written as a get-or-create fall-through so IDO keeps it. */
extern char D_00007DFC;
extern char D_00000000_a;
extern char D_00000000_b;
extern char D_00000000_c;
/* 2026-09-04 (agent-f, pass 2): frame 0x38 slot layout = three `volatile int`
 * pads interleaved in decl order (pad,obj,b1,pad,o2,pad,b2 -> homes
 * 0x30/0x2C/0x24/0x1C); the beqz-on-v0 + delay-slot `sw a1,4(sp)` schedule
 * comes from folding the get-or-create into one condition
 * (`o2 != 0 || (o2 = alloc(0x48)) != 0`); tail addiu order = temp-first `t = &c`;
 * obj-reload placement = call+store on ONE source line. Standalone -O2 44/44 exact. */
typedef struct { int v; } Box53E8;
void *func_000053E8(Box53E8 arg) {
    volatile int p0;
    char *obj;
    Box53E8 b1;
    volatile int p1;
    char *o2;
    volatile int p2;
    Box53E8 b2;
    func_00000000(&D_00007DFC);
    b1 = arg;
    obj = (char *)func_00000000(0x50);
    if (obj != 0) {
        b2 = b1;
        o2 = obj;
        if (o2 != 0 || (o2 = (char *)func_00000000(0x48)) != 0) {
            func_00000000(o2, b2, 0); *(char **)(o2 + 0x28) = &D_00000000_a; /* same-line: keeps obj reload after the addiu */
        }
        /* temp-first `t = &c` -> addiu(c) before addiu(b); store order b,1,c = temp numbering t0/t1/t2; whole tail on ONE line (multi-line tail perturbs the li/addiu schedule) */
        { char *t = &D_00000000_c; *(char **)(obj + 0x28) = &D_00000000_b; *(int *)(obj + 0x48) = 1; *(char **)(obj + 0x4C) = t; }
    }
    return obj;
}

/* func_000054A0: 14-insn 2-call wrapper. Sibling of func_00005068 (same
 * struct-by-value one-int recipe). Passing `*(struct OneI*)&a0` homes the
 * value to the outgoing a1 slot and reproduces the target's dead
 * `sw a1, 0x4(sp)` without instruction patching. */
extern char D_00007E10;
void func_000054A0(int a0) {
    func_00000000(&D_00007E10);
    func_00000000(0, *(struct OneI*)&a0);
}

extern char D_000078D8;

/* 25-insn constructor wrapper: alloc 0x58 bytes, init via runtime-patched
 * callee, set field 0x28 to &D_000078D8.  BYTE-EXACT (2026-06-20).
 * Byte-identical sibling of func_0000553C — same three IDO levers stacked
 * (struct-by-value PARAM homes a0; struct-by-value ARG homes outgoing a1 in
 * the jal delay slot; volatile int pad grows the frame 0x20 -> 0x28). The
 * only difference from 553C is the field-0x28 data symbol (D_000078D8 here
 * vs D_000079C8 there). D_54D8_init_value/_arg are base-0 externs. */
extern char D_54D8_init_value;
extern char D_54D8_init_arg;
int *func_000054D8(struct OneI arg0) {
    volatile int pad;
    int *p;
    int saved_a0 = arg0.x;
    p = (int*)func_00000000(0x58);
    if (p == 0) goto end;
    func_00000000(p, *(struct OneI*)&saved_a0, *(int*)&D_54D8_init_value, &D_54D8_init_arg);
    p[10] = (int)&D_000078D8;
end:
    return p;
}

/* 25-insn constructor wrapper: alloc 0x58 bytes, init via runtime-patched
 * callee, set field 0x28 to &D_000079C8.  BYTE-EXACT (2026-06-20).
 *
 * The match needed three IDO levers stacked:
 *  - struct-by-value PARAM (`struct OneI arg0`) homes the incoming a0 to its
 *    caller slot (`sw a0,0x28(sp)`) AND copies it to a local (`sw a0,0x1C`).
 *  - struct-by-value ARG (`*(struct OneI*)&saved_a0`) homes the outgoing a1
 *    to its arg slot in the jal delay slot (`sw a1,0x4(sp)`) —
 *    docs/IDO_CODEGEN.md#feedback-ido-struct-by-value-homes-arg-pair.
 *  - `volatile int pad;` adds the 8-byte bottom phantom slot to grow the
 *    frame 0x20 -> 0x28 (target is bigger) —
 *    docs/IDO_CODEGEN.md VOLATILE-PAD phantom-slot maker.
 * D_553C_init_value/_arg are base-0 externs (init args read/passed at 0x0). */
extern char D_000079C8;
extern char D_553C_init_value;
extern char D_553C_init_arg;
int *func_0000553C(struct OneI arg0) {
    volatile int pad;
    int *p;
    int saved_a0 = arg0.x;
    p = (int*)func_00000000(0x58);
    if (p == 0) goto end;
    func_00000000(p, *(struct OneI*)&saved_a0, *(int*)&D_553C_init_value, &D_553C_init_arg);
    p[10] = (int)&D_000079C8;
end:
    return p;
}

#ifdef NON_MATCHING
/* func_000055A0 - STRUCTURAL PASS (big-swing 2026-06-02).
 * bootup_uso scene/HUD constructor, 0x958 (598 insns), ~52 folded jal-0
 * placeholder calls + folded D_00000000 data-pool refs.
 *
 * Shape:
 *  - poke control regs at D+0/D+4 (D+4 = 0xAA002), clear root.
 *  - zero a ~40-float transform-scratch block on the stack; set the
 *    diagonal/identity entries (1.0f) and the 0.5f bias entries.
 *  - compute a mode/flag word var_s0_2 from arg1, arg2, and globals at
 *    D+0x34 / D+0x64 (combinations of bits 0x704/1/2/8; forced 0xFFFF
 *    when arg1==1); store it at root->0x34.
 *  - if (flags & 4): build three 0x74-byte actor objects + one 0x6C-byte
 *    object, each via the inlined get-or-alloc(0x74/0x6C/0x48/0x18/0x10/4)
 *    helper, init'd from a per-actor Vec (D+0x20/0x24/0x28..) + 1.0f, then
 *    registered into root (func(root, obj, ...)).
 *  - always: build 0x48 / 0x58-byte objects and register four transform
 *    groups (&sp144/&sp114/&spD4/&spC4/&spE4) into root.
 *
 * Placeholder calls are func_00000000 (file convention, K&R int return,
 * cast to ptr for allocs). intra-/cross-USO split of each jal-0 and the
 * exact folded data symbols are a next-pass concern (uso-reloc-symbolize).
 *
 * NOT matched: folded-pool refs + ~52-call register allocation are a
 * multi-tick grind (project_1080_cap_analysis_2026-05-28). This body
 * replaces the prior "too big to decode" bail comment with the full
 * decoded structure for the next pass.
 *
 * RELOC-BLIND CAP (2026-06-21, agent-i): MATCH is unreachable from C. The
 * target .s has 44 zero-page USO-GOT refs (`lui rX,(0x0>>16); lw rX,0(rX)`)
 * with NO relocs (splat could not resolve the GOT syms; expected/.o is
 * post-link stripped). Any C expression carries an HI16/LO16 reloc our .o
 * keeps -> reloc_mismatch can never reach 0. Same for the float const pool
 * at func_0000057C+0x1C..0x30 (6 uses): target emits a fresh
 * `lui %hi(func_0000057C+N); lwc1 %lo(..+N)` per use (distinct addend per
 * %hi), but IDO CSEs our shared `&func_0000057C` base into one callee-saved
 * reg -> the splat-fold per-use form is C-irreproducible. The real fix is
 * the spimdisasm USO-reloc migration (expected/ becomes reloc-aware), not a
 * per-fn C change.
 *
 * Best achievable structure (this pass): scratch block 0xC4..0x163 modeled
 * as one address-taken float m[40] (forces the contiguous stack reservation
 * the target has; size 429->504 words of 598); gRoot as a distinct base-0
 * pointer extern (breaks the &D_00000000 CSE-into-saved-reg for the ~12
 * root-reload registration sites); CF() = func_0000057C const pool. Prefix
 * still 0 (word-0 frame -0x180 not yet reproduced: depends on reconstructing
 * all 44 distinct globals so the 52-call spill schedule converges). */
extern void *gRoot; /* zero-page USO-GOT global; reloaded per use (no CSE) */
/* tail-block init globals (decode-progress 2026-06-26): the four transform-group
 * registrations + final 0x58 block pass a distinct per-group global as arg2
 * (D_00007E58..68), the first 0x48 node uses &D_00007E48 and func_00007E50's
 * word value, the 2nd 0x48 uses D_00007E54, and obj+0x28 stores &D_00007600. */
extern int D_00007E48, D_00007E54, D_00007E58, D_00007E5C, D_00007E60, D_00007E64, D_00007E68;
extern char D_00007600;
extern void func_00007E50(int *a0);
extern int D_00007E20, D_00007E24; /* arg1==1 list-init globals (decode 2026-06-26) */
extern int D_00007E34; /* actor-0 sub-init preload value (decode 2026-06-26) */
extern char D_00007AB8; /* actor-0 obj+0x28 symbol store (decode 2026-06-26) */
#define DROOT ((int *)gRoot)
#define DI(o) (*(int *)((char *)&D_00000000 + (o)))
#define DF(o) (*(float *)((char *)&D_00000000 + (o)))
extern f32 D_bu_ro_598, D_bu_ro_59C, D_bu_ro_5A0, D_bu_ro_5A4, D_bu_ro_5A8, D_bu_ro_5AC;  /* per-address RoData pool scalars (func_0000057C + K): target re-emits lui at per load */
#define DP(o) ((void *)((char *)&D_00000000 + (o)))
#define OI(p, o) (*(int *)((char *)(p) + (o)))
#define OF(p, o) (*(float *)((char *)(p) + (o)))

/* scratch[0] == sp+0xC4. Indices below are byte-offset/4 from 0xC4. */
#define M(off) m[((off) - 0xC4) / 4]
/* RUN 107 (agent-c, byvalue-struct-scan `out` tell): 51.9 -> 87.6. Every
 * sub-object ctor takes a 4-byte struct BY VALUE (`lw a1,92(sp); sw a1,4(sp)`):
 * zero-page global word -> per-object temp (v0..v3 / t4..t10, kept alive by
 * an address escape so their dead stores stay) -> inline-param copies p1/p2
 * shared by all objects, copied as `.w` ints (a struct assignment goes through
 * addiu-addressed copies). Each reloc-blind zero-page site is its own extern
 * (D_55A0_*), so uopt cannot CSE or dead-store them; one merged flags variable
 * (s0); address-taken list iterator; actor zeros as (f32)0 so 0.0f is not
 * hoisted into $f20. Residual: frame 0x170 vs 0x180, arg1 home reloads, the
 * list-iterator spill shape, t-register numbering. */
typedef struct { int w; } S4_55A0;
typedef struct { float x, y, z, w; } V4_55A0;
extern S4_55A0 D_55A0_v0, D_55A0_v1, D_55A0_v2, D_55A0_v3;
extern char D_55A0_s28_0, D_55A0_s28_1, D_55A0_s28_2, D_55A0_s28_3;
extern char D_55A0_o28_0, D_55A0_o28_1, D_55A0_o28_2, D_55A0_o28_3;
extern char D_55A0_o48_0, D_55A0_o48_1, D_55A0_o48_2;
typedef struct { int a, b; } P8_55A0;
extern P8_55A0 D_55A0_pair;
extern char D_55A0_la, D_55A0_lb, D_55A0_n4, D_55A0_n2;
extern char D_55A0_tb, D_55A0_o28_4, D_55A0_s28_5, D_55A0_o28_5;
extern int D_55A0_g5, D_55A0_g6, D_55A0_g7, D_55A0_g8, D_55A0_g9, D_55A0_g10;
typedef struct { int a, b; } G2_55A0;
typedef struct { int w, x, y; } R12_55A0;
extern G2_55A0 D_55A0_G;
extern int D_55A0_w0, D_55A0_r0, D_55A0_z0, D_55A0_list, D_55A0_listchk, D_55A0_list2;
extern char D_55A0_lb0, D_55A0_fmt, D_55A0_cfg;
#define CFG55A0(o) (*(int *)(&D_55A0_cfg + (o)))
void func_000055A0(int arg0, int arg1, int arg2) {
    S4_55A0 t4, t5, t6, t7, t8, t9, t10;
    float m[40]; /* sp+0xC4 .. sp+0x163 */
    S4_55A0 v0, v1, v2, v3;
    S4_55A0 p1;
    V4_55A0 vec0;
    S4_55A0 p2;
    V4_55A0 vec1, vec2;
    int var_s0, mode;
    int cnt;
    char *obj, *sub, *node;
    char *nl;
    char *n2, *n3, *n4;
    R12_55A0 *it;

    if (1) { S4_55A0 *q0 = &v0, *q1 = &v1, *q2 = &v2, *q3 = &v3, *q4 = &t4, *q5 = &t5, *q10 = &t10; }
    D_55A0_w0 = D_55A0_r0 | 8; if (1) {}
    D_55A0_G.b = 0xAA002;
    D_55A0_G.a = 0;
    D_55A0_z0 = 0;
    var_s0 = arg2 | 0x10;
    M(0x154) = 0.0f; M(0x158) = 0.0f; M(0x15C) = 0.0f; M(0x160) = 0.0f;
    M(0x144) = 0.0f; M(0x148) = 0.0f; M(0x150) = 0.0f;
    M(0x134) = 0.0f; M(0x138) = 0.0f; M(0x140) = 0.0f; M(0x130) = 0.0f;
    M(0x114) = 0.0f; M(0x11C) = 0.0f; M(0x120) = 0.0f;
    M(0x104) = 0.0f; M(0x10C) = 0.0f; M(0x110) = 0.0f;
    M(0xF4) = 0.0f; M(0xFC) = 0.0f; M(0x100) = 0.0f;
    M(0xEC) = 0.0f; M(0xF0) = 0.0f;
    M(0xD8) = 0.0f; M(0xDC) = 0.0f; M(0xE0) = 0.0f;
    M(0xC8) = 0.0f; M(0xCC) = 0.0f; M(0xD0) = 0.0f;
    M(0x14C) = 1.0f; M(0x124) = 1.0f; M(0x128) = 1.0f; M(0x12C) = 1.0f; M(0x118) = 1.0f; M(0xC4) = 1.0f;
    M(0x13C) = 0.5f; M(0x108) = 0.5f; M(0xE4) = 0.5f; M(0xE8) = 0.5f; M(0xD4) = 0.5f;
    M(0xF8) = D_bu_ro_598;

    if (arg1 == 1) {
        D_55A0_G.b = 0x28002;
        D_55A0_G.a = 0;
        D_55A0_list = func_00000000(0, &D_00007E20, &D_55A0_lb0, 0);
        if (D_55A0_listchk != 0) {
            if (1) { R12_55A0 **q = &it; }
            it = (R12_55A0 *)&D_00007E24; if (1) {}
            do {
                func_00000000(&D_55A0_fmt, it->w, it);
                func_00000000(D_55A0_list2, it);
                it++; if (1) {}
            } while (it->w != 0);
        }
    }

    if (arg1 != 6) {
        var_s0 = var_s0 | 0x20;
    }
    var_s0 |= 4;
    if ((arg1 == 6) || (arg1 == 0xB)) {
        var_s0 &= ~0x704;
    }
    mode = CFG55A0(0x34);
    if (mode == 5) {
        var_s0 |= 1;
    }
    if (mode == 6) {
        var_s0 |= 2;
    }
    if ((arg1 == 2) || (arg1 == 3) || (arg1 == 4) || (arg1 == 9) || (CFG55A0(0x64) == 7)) {
        var_s0 |= 8;
    }
    if (arg1 == 1) {
        var_s0 = 0xFFFF;
    }
    OI(DROOT, 0x34) = var_s0;

    if (var_s0 & 4) {
        /* actor 0 (0x74) */
        v0.w = D_55A0_v0.w;
        p1.w = v0.w;
        obj = (char *)func_00000000(0x74);
        if (obj != 0) {
            p2.w = p1.w;
            sub = obj;
            if (sub != 0 || (sub = (char *)func_00000000(0x48)) != 0) {
                func_00000000(sub, p2, 0);
                OI(sub, 0x28) = (int)&D_55A0_s28_0;
            }
            OI(obj, 0x28) = (int)&D_55A0_o28_0;
            node = obj + 0x5C;
            if (node != 0 || (node = (char *)func_00000000(0x18)) != 0) {
                func_00000000(node);
            }
            OF(obj, 0x4C) = (f32)0; OF(obj, 0x50) = (f32)0; OF(obj, 0x54) = (f32)0; OF(obj, 0x58) = 1.0f; if (1) {}
            vec0.w = 1.0f; vec0.x = vec0.y = vec0.z = D_bu_ro_59C;
            func_00000000(obj + 0x5C, &vec0, 993, 997);
            OI(obj, 0x38) = 1;
            OI(obj, 0x48) = (int)&D_55A0_o48_0;
        }
        func_00000000(DROOT, obj, 4);

        /* actor 1 (0x74) */
        v1.w = D_55A0_v1.w;
        p1.w = v1.w;
        obj = (char *)func_00000000(0x74);
        if (obj != 0) {
            p2.w = p1.w;
            sub = obj;
            if (sub != 0 || (sub = (char *)func_00000000(0x48)) != 0) {
                func_00000000(sub, p2, 0);
                OI(sub, 0x28) = (int)&D_55A0_s28_1;
            }
            OI(obj, 0x28) = (int)&D_55A0_o28_1;
            node = obj + 0x5C;
            if (node != 0 || (node = (char *)func_00000000(0x18)) != 0) {
                func_00000000(node);
            }
            OF(obj, 0x4C) = (f32)0; OF(obj, 0x50) = (f32)0; OF(obj, 0x54) = (f32)0; OF(obj, 0x58) = 1.0f; if (1) {}
            vec1.w = 1.0f; vec1.x = vec1.y = vec1.z = D_bu_ro_5A0;
            func_00000000(obj + 0x5C, &vec1, 993, 997);
            OI(obj, 0x38) = 1;
            OI(obj, 0x48) = (int)&D_55A0_o48_1;
        }
        func_00000000(DROOT, obj, 4);

        /* actor 2 (0x74) */
        v2.w = D_55A0_v2.w;
        p1.w = v2.w;
        obj = (char *)func_00000000(0x74);
        if (obj != 0) {
            p2.w = p1.w;
            sub = obj;
            if (sub != 0 || (sub = (char *)func_00000000(0x48)) != 0) {
                func_00000000(sub, p2, 0);
                OI(sub, 0x28) = (int)&D_55A0_s28_2;
            }
            OI(obj, 0x28) = (int)&D_55A0_o28_2;
            node = obj + 0x5C;
            if (node != 0 || (node = (char *)func_00000000(0x18)) != 0) {
                func_00000000(node);
            }
            OF(obj, 0x58) = 1.0f; OF(obj, 0x4C) = (f32)0; OF(obj, 0x50) = (f32)0; OF(obj, 0x54) = (f32)0;
            vec2.x = D_bu_ro_5A4; vec2.y = D_bu_ro_5A8; vec2.z = D_bu_ro_5AC; vec2.w = 1.0f;
            func_00000000(obj + 0x5C, &vec2, 993, 997);
            OI(obj, 0x38) = 1;
            OI(obj, 0x48) = (int)&D_55A0_o48_2;
        }
        func_00000000(DROOT, obj, 4);

        /* list object (0x6C) */
        DI(4) = 0x28002;
        DI(0) = 0;
        nl = (char *)func_00000000(0, &D_55A0_la, &D_55A0_lb, 0);
        v3.w = D_55A0_v3.w;
        p1.w = v3.w;
        obj = (char *)func_00000000(0x6C);
        if (obj != 0) {
            p2.w = p1.w;
            sub = obj;
            if (sub != 0 || (sub = (char *)func_00000000(0x48)) != 0) {
                func_00000000(sub, p2, nl);
                OI(sub, 0x28) = (int)&D_55A0_s28_3;
            }
            OI(obj, 0x28) = (int)&D_55A0_o28_3;
            n2 = obj + 0x48;
            if (n2 != 0 || (n2 = (char *)func_00000000(0x10)) != 0) {
                n3 = n2;
                if (n3 != 0 || (n3 = (char *)func_00000000(0x10)) != 0) {
                    n4 = n3;
                    if (n4 != 0 || (n4 = (char *)func_00000000(4)) != 0) {
                        OI(n4, 0) = (int)&D_55A0_n4;
                    }
                    OI(n3, 0x4) = (int)obj;
                    *(P8_55A0 *)(n3 + 8) = D_55A0_pair;
                }
                OI(n2, 0) = (int)&D_55A0_n2;
            }
            DI(0) = 0;
            OI(obj, 0x5C) = 2;
            OI(obj, 0x64) = 0;
            OI(obj, 0x60) = 0;
            OI(obj, 0x58) = 0;
            OI(obj, 0x68) = 0;
        }
        func_00000000(DROOT, obj, 0xFFFF);
    }

    func_00000000(DROOT, DP(0));

    /* trailing 0x48 / 0x58 objects + transform-group registration */
    nl = (char *)func_00000000(0, &D_00007E48, &D_55A0_tb, 0);
    *(SC4_ *)&(t4.w) = *(SC4_ *)&(*(int *)&func_00007E50);
    p1.w = t4.w;
    obj = (char *)func_00000000(0x48);
    if (obj != 0) {
        func_00000000(obj, p1, nl);
        OI(obj, 0x28) = (int)&D_55A0_o28_4;
    }
    func_00000000(DROOT, obj, 0xFFFF);

    t5.w = *(int *)&D_00007E54;
    p1.w = t5.w;
    obj = (char *)func_00000000(0x48);
    if (obj != 0) {
        p2.w = p1.w;
        sub = obj;
        if (sub != 0 || (sub = (char *)func_00000000(0x48)) != 0) {
            func_00000000(sub, p2, D_55A0_g5);
            OI(sub, 0x28) = (int)&D_55A0_s28_5;
        }
        OI(obj, 0x28) = (int)&D_55A0_o28_5;
    }
    func_00000000(DROOT, obj, 0xFFFF);

    func_00000000(DROOT, func_00000000(0, (t6.w = *(int *)&D_00007E58, t6), D_55A0_g6, &M(0x144)), 0xFFFF);
    func_00000000(DROOT, func_00000000(0, (t7.w = *(int *)&D_00007E5C, t7), D_55A0_g7, &M(0x114)), 0xFFFF);
    func_00000000(DROOT, func_00000000(0, (t8.w = *(int *)&D_00007E60, t8), D_55A0_g8, &M(0xD4)), 0xFFFF);
    func_00000000(DROOT, func_00000000(0, (t9.w = *(int *)&D_00007E64, t9), D_55A0_g9, &M(0xC4)), 0xFFFF);

    t10.w = *(int *)&D_00007E68;
    p1.w = t10.w;
    obj = (char *)func_00000000(0x58);
    if (obj != 0) {
        func_00000000(obj, p1, D_55A0_g10, &M(0xE4));
        OI(obj, 0x28) = (int)&D_00007600;
    }
    func_00000000(DROOT, obj, 0xFFFF);
}
#else
INCLUDE_ASM("asm/nonmatchings/bootup_uso", func_000055A0);
#endif

void func_00005EF8(int *dst) {
    int buf[2];
    func_00000000(&D_00000000, buf, 4);
    *dst = buf[0];
}

/* func_00005F34 - SIX-element list builder. 7 params (a0 unused + a1,a2,a3
 * in regs + a4,a5,a6 on the caller stack at sp+80/84/88). s1 = a container
 * object built from D_00007E6C; list = s1+0x10 is its child attach point.
 * Per element: r = func_00000000(0, payload, A2, A3, 0) with an angle constant
 * threaded into a3 (0/90/180/270) for elements 0-3 and into a2 (-90/90) for
 * elements 4-5; then func_00000000(list, r) attaches it (return discarded),
 * then back-link: if (r->0x14 != 0) r->0x4 = 1 (linked flag), r->0x14 = s1
 * (owner). Element 5 (a6) is gated on a6 != 0 and uses a beql back-link
 * (duplicated store) since it precedes the epilogue. Returns s1.
 * Prologue cracked (was 42->95.52->99.x%): the target homes the unused a0 to
 * its caller home slot AND emits a dead `or a3,a0,zero` copy — reproduced by
 * passing a0 as a dead 4th arg to the s1 constructor (func_00000000 is K&R,
 * 3 args used). The +8 frame (64 vs 56) with list pinned at sp+0x28 needs
 * `volatile int pad[2]` declared LAST (8-aligned pad takes the bottom dead
 * space, leaving list at the top local slot). Result: 113/115 .text words
 * byte-exact (the remaining word is a R_MIPS_LO16 reloc to D_00007E6C, equal
 * once relocated).
 * MATCH (permuter as1-tie crack, 2026-06-21): the residual was a 2-insn
 * as1-scheduler tie — the target schedules `sw list,0x28(sp)` (home the
 * `list` stack temp) BEFORE `or s0,v0` (save r to callee-saved s0), and the
 * default statement layout picks the opposite tie. The permuter cracked it by
 * COLLAPSING `r = func_00000000(0,a1,...)`, `list = s1 + 0x10`, and the first
 * `func_00000000(list, r)` onto a SINGLE source line: IDO's as1 scheduler uses
 * the per-statement debug line number as a tie-break, so giving the three
 * statements one shared line re-anchors the tie to emit the list-home store
 * first, exactly matching the target. (Whitespace-only steer — the C is
 * identical; the SAME line is load-bearing, do not re-split it.) 115/115 .text
 * words byte-exact (the lone reloc word to D_00007E6C resolves equal).
 * Frame 64 with list at sp+0x28. */
extern char D_00007E6C;
char *func_00005F34(int a0, int a1, int a2, int a3, int a4, int a5, int a6) {
  char *s1 = (char *) func_00000000(0, &D_00007E6C, 0, a0);
  char *list;
  char *r;
  volatile int pad[2];
 r = (char *) func_00000000(0, a1, 0, 0, 0); list = s1 + 0x10; func_00000000(list, r); if ((*((int *) (r + 0x14))) != 0) {
    *((int *) (r + 0x4)) = 1;
  }
  *((void **) (r + 0x14)) = s1;
  r = (char *) func_00000000(0, a2, 0, 90, 0);
  func_00000000(list, r);
  if ((*((int *) (r + 0x14))) != 0)
  {
    *((int *) (r + 0x4)) = 1;
  }
  *((void **) (r + 0x14)) = s1;
  r = (char *) func_00000000(0, a3, 0, 180, 0);
  func_00000000(list, r);
  if ((*((int *) (r + 0x14))) != 0)
  {
    *((int *) (r + 0x4)) = 1;
  }
  *((void **) (r + 0x14)) = s1;
  r = (char *) func_00000000(0, a4, 0, 270, 0);
  func_00000000(list, r);
  if ((*((int *) (r + 0x14))) != 0)
  {
    *((int *) (r + 0x4)) = 1;
  }
  *((void **) (r + 0x14)) = s1;
  r = (char *) func_00000000(0, a5, -90, 0, 0);
  func_00000000(list, r);
  if ((*((int *) (r + 0x14))) != 0)
  {
    *((int *) (r + 0x4)) = 1;
  }
  *((void **) (r + 0x14)) = s1;
  if (a6 != 0)
  {
    r = (char *) func_00000000(0, a6, 90, 0, 0);
    func_00000000(list, r);
    if ((*((int *) (r + 0x14))) != 0)
    {
      *((int *) (r + 0x4)) = 1;
      *((int *) (r + 0x4)) = 1;
    }
    *((void **) (r + 0x14)) = s1;
  }
  return s1;
}

void func_00006100(float *dst) {
    float buf[2];
    func_00000000(&D_00000000, buf, 4);
    *dst = buf[0];
}

void func_0000613C(Quad4 *dst) {
    Quad4 buf;
    func_00000000(&D_00000000, &buf, 16);
    *dst = buf;
}

extern void func_00006194();
void func_00006194(Vec3 *dst) {
    int pad_top[1];
    Tri3i raw;
    int pad_mid[2];
    Tri3i tmp;
    int pad_bot[2];
    func_00000000(&D_00000000, &raw, 12);
    tmp = raw;
    dst->x = *(float*)&tmp.a;
    dst->y = *(float*)&tmp.b;
    dst->z = *(float*)&tmp.c;
}

/* 77.78% NM (7/9 words): target "lui a0; sw ra; jal", C "sw ra; lui a0;
 * jal". No matching C found; this is NOT proof of an unreachable cap.
 * IDO 7.1 -Wab,-R traces the same-line probe: sw ra and lui have equal
 * aftercycles/latency (2/1); the newly ready sw ra is visited first.
 * An unused int argument reverses that tie through its home-store
 * dependency, but adds a non-target instruction. Prototype, pointer,
 * control-flow, neighboring-function and IDO 5.3 probes did not match.
 * See docs/IDO_CODEGEN.md#as1-ready-list-wrapper-6204 in the agent repo.
 * The old 2-word INSN_PATCH was removed as match-faking; do not restore. */
#ifdef NON_MATCHING
void func_00006204(void) {
    func_00000000(&D_00000000);
}
#else
INCLUDE_ASM("asm/nonmatchings/bootup_uso", func_00006204);
#endif

void func_00006228(char *a0) {
    func_0000502C((int*)(a0 + 0x3C));
    func_00000000(a0);
}

/* func_00006254 - verified structural decode (0x160, 88 insns,
 * get-or-create constructor + sub-object + Vec3 init).
 * Struct-typing reference: o = 0xDC-byte object. o->0x28 (40)
 * descriptor ptr (&D), o->0xC (12) type/vtable ptr (&D_00007E7C),
 * o->0xB4 (180) = a1 (a parent/owner handle; o->0xB4->0xA0 (160)
 * gated for an extra reloc registration), o->0xB8 (184)=a3, o->0xBC
 * (188)=a2, o->0xC0 (192)=a4. s = 0xB4-byte sub-object: s->0x28 (40)
 * descriptor (&D), s+0x30 (48) a Vec3f zero-initialized via the
 * reloc helper; plus a defensive 4-byte child alloc (*(int*)c = 0).
 * D_00007E74/7E7C/7E84/7E98 = init datums.
 * Caps <80: get-or-create + ~8 reloc + &D descriptors + defensive-
 * dead alloc guards + FP Vec3 + bnel branch-likely. INCLUDE_ASM
 * remains build path.
 * 2026-05-31: applied the alloc-or-passthrough cascade dead-arm form to the
 * child alloc (c = s+0x2C; if(s==-44) c=alloc(4); if(!c) c=alloc(4); if(c)
 * *c=0) -> 64.9->67.3%. Remaining residual is the sub-object saved-reg(s1)-
 * vs-stack-spill(76sp) choice + the ~8 &D %hi/%lo reloc scheduling. */
extern char D_00007E74, D_00007E7C, D_00007E84, D_00007E98;
/* run 111 (agent-c): cfront ctor shape -- if (this || (this = new(0xDC))), the inlined base ctor
 * if (s || (s = new(0xB4))) and the doubly-inlined member ctor for the +0x2C cell; the param
 * itself is the object (no copy local); pads restore the 0x50 frame / home offsets; block-count
 * lever (IDO_CODEGEN#hold-vs-remat-firstusecost-run111). EXACT: z[1]/z[2] stored in the call's
 * argument (comma expression) puts the s+0x30 setup between the z[0] and z[1] stores as in the target. */
void *func_00006254(char *a0, int a1, int a2, int a3, int a4) {
    char *s;
    volatile int pad[3];
    float z[3];
    volatile int pad2[1];
    if (a0 != 0 || (a0 = (char*)func_00000000(0xDC)) != 0) {
    s = a0;
    if (s != 0 || (s = (char*)func_00000000(0xB4)) != 0) {
        func_00000000(s, &D_00007E74);
        *(void**)(s + 0x28) = &D_00000000;
        {
            int *c = (int*)(s + 0x2C);
            if (c != 0 || (c = (int*)func_00000000(4)) != 0) {
                int *c2 = c;
                if (c2 != 0 || (c2 = (int*)func_00000000(4)) != 0) {
                    *c2 = 0;
                }
            }
        }
        func_00000000(s);
        z[0] = 0.0f;
        func_00000000(s + 0x30, (z[1] = 0.0f, z[2] = 0.0f, z));
    }
    *(void**)(a0 + 0x28) = &D_00000000;
    *(void**)(a0 + 0xC) = &D_00007E7C;
    *(int*)(a0 + 0xB4) = a1;
    func_00000000(a0, a1);
    if (*(int*)(*(char**)(a0 + 0xB4) + 0xA0) == 0)
        func_00000000(&D_00007E84, &D_00007E98, 0x3E4);
    *(int*)(a0 + 0xB8) = a3;
    *(int*)(a0 + 0xBC) = a2;
    *(int*)(a0 + 0xC0) = a4;
    }
    if (1) { }
    if (1) { }
    if (1) { }
    if (1) { }
    return a0;
}

// func_000063B4 — 0x380 / 224 insns. Minimap marker projection. 23.4 -> 76.0
// (agent-f 2026-09-04, hand reconstruction from the target .s; old body was
// a 78-insn stub that dropped the whole 3x3 transform + clamp cascade).
// Shape recovered: obj homed at 0x90(sp) (no s-regs); K = folded const
// func_0000057C+0x34 read via DATA ALIAS bu_63b4_c57c (fn-typed base never
// folds %lo addends); call 1 = float f(float) -> f0; call 2 = (float, &A.x,
// &A.z); A/B/q live in ONE escaping frame struct Bu63b4F (0x5C..0x8F, passed
// to call 3 as &fr) — this is what keeps B's x/y transform alive (as plain
// locals uopt DCEs B.x/B.y since only B.z is read) and makes every field a
// memory var (per-use lwc1/swc1); XFORM = in-place M*v with named x_/y_/z_
// input temps + rx_/ry_/rz_ result temps (Vec3 temp + struct copy = memcpy
// form, wrong); MIN/MAX = ternary into a FLOAT temp then (int) cast (cast on
// the ternary duplicates trunc into both arms); fn = (float)(int)(...).
// Frame: 0x98 vs 0x90 — one float-home over (floats cost 8B homes here; the
// x_,y_,z_,rx_,ry_ + z-direct/y,x,z-store-order variant lands the frame
// EXACT and materializes the D_00000000 base (lui/addiu + lw 0x254(base))
// but re-colors base->a0 / obj->v0 and scores 63 — coloring cascade, open).
// Residual: target reloads ctx (*(D+0x254)) 3x via a held a2 base (kills =
// pointer-ish stores; fn-typed &func_00000000 base blocks the CSE but demotes
// obj to per-use home reloads), bc1fl-form ternaries (target: bc1fl + else
// in delay + dead mov after b), A.z store sunk into the MAX arms vs stored
// immediately + reloaded (f14) in the target.
// Struct-typing reference:
//   obj->0xB8 / 0xBC = grid/index counts; obj->0xC0 = pixel origin offset;
//   obj->0xB4 = record ptr (->0xA0 -> {0xC int, 0x14 float});
//   D_0 + 0x254 = ctx; ctx->0x70 = View; View+0xB4 = 3x3 (0x10 stride),
//   View->0xA8 float; D_0 + 0x130 = screen scale;
//   ctx->0x10C / 0x110 = marker pixel X/Y (clamped 0..0x7F).
#ifdef NON_MATCHING
extern u8 bu_63b4_c57c[];                                   /* data alias of func_0000057C (const pool) */
extern float func_00000000_63b4_ff(float);                  /* range helper: f12 -> f0 */
extern void func_00000000_63b4_fpp(float, float *, float *); /* sincos-like: (ang, &x, &z) */
typedef struct { float q[4]; float pad[3]; Vec3 B; Vec3 A; } Bu63b4F; /* 0x5C..0x8F frame block, escapes via call 3 */
extern void func_00000000_63b4_pq(void *, Bu63b4F *);
extern void func_00000000_63b4_p(void *);
#define BU63B4_CTX() (*(char **)((char *)&D_00000000 + 0x254))
#define BU63B4_M() ((float (*)[4])(*(char **)(BU63B4_CTX() + 0x70) + 0xB4))
#define BU63B4_MC(c) ((float (*)[4])(*(char **)((c) + 0x70) + 0xB4))
#define BU63B4_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define BU63B4_MAX(a, b) (((b) < (a)) ? (a) : (b))
#define BU63B4_XFORM(m, v)                                                   \
    {                                                                          \
        x_ = (v).x; y_ = (v).y; z_ = (v).z;                                    \
        rx_ = (m)[0][0] * x_ + (m)[1][0] * y_ + (m)[2][0] * z_;               \
        ry_ = (m)[0][1] * x_ + (m)[1][1] * y_ + (m)[2][1] * z_;               \
        rz_ = (m)[0][2] * x_ + (m)[1][2] * y_ + (m)[2][2] * z_;               \
        (v).x = rx_;                                                           \
        (v).y = ry_;                                                           \
        (v).z = rz_;                                                           \
    }
void func_000063B4(char *obj) {
    Bu63b4F fr;
    float inv;
    float fn, t;
    int ix, iy;
    float (*m)[4];
    char *ctx;
    char *v1;
    float x_, y_, z_, rx_, ry_, rz_;
    char *base;

    inv = func_00000000_63b4_ff((float)*(int *)(obj + 0xBC) / *(float *)(bu_63b4_c57c + 0x34));
    fr.A.y = 0.0f;
    fr.A.x = 0.0f;
    fr.A.z = (float)(-*(int *)(obj + 0xB8)) / inv;
    func_00000000_63b4_fpp((float)*(int *)(obj + 0xBC), &fr.A.x, &fr.A.z);
    base = (char *)&D_00000000;
    ctx = *(char **)(base + 0x254);
    fr.B.y = 0.0f;
    fr.B.x = -fr.A.x;
    fr.B.z = fr.A.z;
    m = BU63B4_MC(ctx);
    BU63B4_XFORM(m, fr.A);
    m = BU63B4_MC(ctx);
    ctx = *(char **)(base + 0x254);
    BU63B4_XFORM(m, fr.B);
    v1 = *(char **)(*(char **)(obj + 0xB4) + 0xA0);
    fn = (float)(int)((*(float *)(*(char **)(ctx + 0x70) + 0xA8) * *(float *)(base + 0x130) -
                       (float)*(int *)(v1 + 0xC)) / *(float *)(v1 + 0x14));
    fr.A.z += fn;
    fr.B.z += fn;
    t = BU63B4_MIN(fr.A.z, fn);
    ix = (int)t;
    t = BU63B4_MIN((float)ix, fr.B.z);
    ix = (int)t;
    t = BU63B4_MAX(fr.A.z, fn);
    iy = (int)t;
    t = BU63B4_MAX((float)iy, fr.B.z);
    iy = (int)t;
    ix -= *(int *)(obj + 0xC0);
    iy += *(int *)(obj + 0xC0);
    if (ix < 0) ix = 0;
    if (ix >= 0x80) ix = 0x7F;
    if (iy < 0) iy = 0;
    if (iy >= 0x80) iy = 0x7F;
    *(int *)(ctx + 0x10C) = ix;
    *(int *)(*(char **)(base + 0x254) + 0x110) = iy;
    fr.q[2] = 0.0f;
    fr.q[1] = 0.0f;
    fr.q[0] = 0.0f;
    fr.q[3] = 1.0f;
    func_00000000_63b4_pq(base, &fr);
    func_00000000_63b4_p(obj);
}
#else
INCLUDE_ASM("asm/nonmatchings/bootup_uso", func_000063B4);
#endif

/* func_00006734 - verified structural decode (0xD4, 53 insns,
 * 4-entry registration/builder).
 * (reg = func_00000000; the 5th/6th args are passed on the stack at
 * sp+0x10 / sp+0x14, the documented IDO stack-arg slots.)
 * Struct-typing reference: D_00007EA4/EA8/EAC/EB0 = four registration
 * key data (label/descriptor); the bound targets are object fields
 * a0->0xB8 (184), a0->0xC0 (192), a0->0xBC (188) respectively (entry
 * 0 binds no target). Per-entry trailing params: (a3=0, 0x7F, 1) for
 * EA8/EAC and (a3=0, 0x59, 0) for EB0 - look like {range/limit, flag}
 * pairs. Caps <80: 6x reloc calls + 4x &D reloc + 5th/6th stack-arg
 * passing (sp+0x10/0x14). INCLUDE_ASM remains build path. */
extern char D_00007EA4, D_00007EA8, D_00007EAC, D_00007EB0;
void func_00006734(char *a0) {
    func_00000000(&D_00000000, &D_00007EA4, 0);
    func_00000000(&D_00000000, &D_00007EA8, a0 + 0xB8, 0, 0x7F, 1);
    func_00000000(&D_00000000, &D_00007EAC, a0 + 0xC0, 0, 0x7F, 1);
    func_00000000(&D_00000000, &D_00007EB0, a0 + 0xBC, 0, 0x59, 0);
    func_00000000(&D_00000000);
    func_00000000(a0);
}

// func_00006808 — STRUCTURAL PASS (0x948 / 594 insns, no episode).
// Large scene/screen constructor, bracketed by a global mode-flag
// set/clear; builds a widget tree from the obj's sub-object table via
// ~65 func_00000000 dispatcher calls.
//
//   void *func_00006808(Scene *obj, Ctx *a1, void *a2) {  // obj->s7
//     s1 = a1->0x40;
//     *(int*)(func_00000000+0x4) |= 0x20000;          // set global mode bit
//     // stack widget descriptor: {flag=1, h=0x14, w=0x28, 0} @ sp+0xB0..BC
//     // zero f32 triple @ sp+0xC0/C4/C8 (a default Vec/color)
//     func_00000000(&.L00007EB8, 0);
//     root = func_00000000();  *(void**)D_0 = root;   // singleton store
//     s2 = root;
//     func_00000000(obj->0x84 + 0x10, root);
//     if (root->0x14) { ... }                          // attach guard
//     // main body: iterate obj sub-objects 0x7C/0x80/0x84/0x88/0x90/0x94
//     //   building child widgets/labels; descriptor tables .L00007EB8 /
//     //   .L00007F5C / func_00000188+0x3C (6x); ~65 dispatcher calls
//     //   total — create / configure / re-parent each element.
//     // teardown: v = (*(int*)D_0) & s6;  *(int*)D_0b = v;  // clear flag
//     return s0;                                        // built root/handle
//   }
//
// Struct-typing reference:
//   obj(s7): 0x7C/0x80/0x84/0x88 child/source sub-object ptrs,
//     0x90/0x94 config words/handles; a1->0x40 = s1 seed.
//   widget descriptor on stack: {enable, 0x14, 0x28, 0} (h/w spec).
//   D_0 = global scene singleton (root stored, flag masked on exit).
//   Folded refs (same JAL-target-0 / literal-pool fold family, see
//   docs/N64_FORENSICS.md#bootup-uso-fp-literal-pool-folded-into-func-0000098C):
//     func_00000000 + 0x4  = a WRITABLE global mode word (RMW |0x20000);
//     func_00000188 + 0x3C = a folded table (read 6x);
//     func_0000057C + 0x38 = a folded f32 const (sibling of the
//                            func_0000057C+0x34 ref in func_000063B4).
// Caps (DEFERRED): 594-insn dispatcher-heavy ctor w/ folded globals
//   — byte-match blocked by deferred pool symbolization + GOT-base
//   coloring (arg0->s7, two module globals pinned in s4/s5, the running
//   record-opcode index in s6, across the whole 13-case dispatch loop).
//   IDO emits these from genuinely long-lived values; m2c-style
//   fragmented temporaries re-color and grow the frame. Real-C
//   STRUCTURAL body below — large scene/screen constructor skeleton.
//   Name pre-checked: no extern reuse.
//
// PASS-3 2026-06-21 (agent-b deep reconstruction): rewrote with
//   long-lived register-intent locals (obj kept live -> s7-class; the
//   two module-global table pointers g_root / g_mode kept live ->
//   s4/s5-class; record stride/index -> s6-class), real rodata symbol
//   refs (&D_00007Exx) instead of bare 0x7Exx ints, and explicit
//   goto-on-terminator loop control matching the `opcode==0xC` exit.
//   Folded literal-pool refs (func_00000000+0x4 mode word,
//   func_00000188+0x3C table, func_0000057C+0x38 jumptable) kept as
//   offset reads — they cannot be symbolized in this build (USO GOT).
// PASS-4 2026-06-21 (agent-i): DOUBLY-CAPPED, confirmed empirically vs
//   expected .o. (1) Reloc cap: target materializes its held s4/s5
//   globals as bare `lui 0; addiu 0` with NO reloc (USO-GOT base patched
//   at load; reloc consumed in post-processing). C cannot emit a
//   zero-immediate lui without a HI16/LO16 reloc, so &D_ refs diverge.
//   (2) Coloring cap: the two s4/s5 globals are BOTH address-0 (same
//   symbol) — a register pointer CSEs to ONE reg, so the s4/s5 split is
//   structurally unreproducible; frame can't reach -0xF0 (built -0x110).
//   `register` hints had no effect (weight-driven alloc). NOT landable
//   without the deferred splat/GOT-symbolization infra pass + original cc.
//   Fixed two m2c-unset-temp literals in the tail (sw 6 to s5 global;
//   0xA8002 mode word) — faithful corrections, prefix still 0% (frame).
// PASS-5 2026-07-17 (agent-g): NEGATIVE RESULT — E68 identity-split
//   recipe probed and REVERTED (all three variants lower fuzzy from
//   60.28: +0x1C4 RMWs -> true-identity func_00000188+0x3C form 58.82;
//   + mode-word RMW -> func_00000000+0x4 form 57.93; + gc alias for the
//   s5-class 0x68/0x3C reads 56.56), despite each moving insn count
//   toward target (545->553 of 594) and matching the target's per-site
//   %hi/%lo shapes (target: no held base for the func_00000188+0x3C
//   |8/&~8 RMWs; s4 AND s5 both re-materialized lui0/addiu0 mid-body).
//   At this alignment distance the fuzzy LCS is dominated by the s-reg
//   under-coloring (built saves 5 regs vs target 9, seed a1->0x40
//   spilled to 0x3C(sp) vs held in s1) — identity splits can't help
//   until the register-role skeleton (obj->s7, ga->s4, gc->s5, seed->s1,
//   idx/mask->s6) is rebuilt first, i.e. this needs the E68-style
//   de-name/role-local pass BEFORE the alias split, not after.
// PASS-6 2026-10-20 (agent-c run 114): FRESH HAND DECODE 63.4 -> 91.4.
//   Real identities from the bootup.uso TextReloc table + emu symnames
//   (scratch rel.py over uso-reloc-symbolize internals): A = import_
//   8001FFC0 (root slot, s4 pre-loop), C = import_800200FC (s5), B =
//   D_7770 (current obj), post-loop s4/s5 = D_776C / D_1EBDC, mode word
//   import_8005EE6C, flags import_8005EE64 / import_8005C108+0x3C. Every
//   `lui a0; addiu a0,0` string arg is a DISTINCT symbol (no &sym CSE in
//   target). Levers: two shared object temps o(s0)/t(s2) + one list temp
//   L(s3) used by an ATTACH macro (no per-site copies; explicit `t = o;`
//   where target shows the s0+s2 pair); record header read as
//   `count = *p >> 16; t = p++; op = *t & 0xFFFF` (t reused as the
//   record pointer gives the target's or s2,s1 + reload); p reused for
//   the post-loop object (s1); RMWs read through per-site zero aliases
//   (target folds every RMW to lui/lw + lui/sw, no held base). Residual:
//   post-loop target holds &D/&E in s4/s5 + 6/0xA0000/&I as hoisted
//   caller-saved constants; 3 phantom frame words below v4C; t-ring +1.
// PASS-7 2026-10-22 (agent-c run 119): frame map from a -g3 .mdebug dump
//   (cfe homes EVERY named local, even unreferenced ones, in declaration
//   order): target = two dummy slots above e4, one more before va/vb, op
//   declared before v4C, and no block-scope m/q/q slots -> every stack
//   offset now matches the target relative to the frame top (frame 240).
//   gD/gE through `(T *)(int)&sym` DO get held in s4/s5 like the target
//   (uopt no longer folds the store back to lui/sw), but the frame then
//   grows to 248 (an unused 8-byte home at 72..80) and the t-ring shifts;
//   kept the plain form (258 diff words vs 280).
#ifdef NON_MATCHING
extern int import_8005EE6C[];   /* mode words: [0], [1] */
extern int import_8005EE6C_r[];  /* read alias (same USO import) */
extern char *import_8001FFC0;     /* root object slot */
extern int import_800200FC[];   /* settings: [0x3C/4], [0x68/4] */
extern int import_8005EE64;     /* flags word */
extern int import_8005EE64_r;
extern int import_8005C108[];   /* [0x3C/4] flags */
extern int import_8005C108_r[];
extern int import_8001FFF0;
extern char *D_bu_7770;
extern char *D_bu_776C[];
extern int D_bu_1EBDC[];
typedef struct { int count : 16; unsigned op : 16; } Rec6808;
typedef struct { f32 x, y, z; } V3_6808;
#define ATTACH6808(list, x) { \
    L = (list); \
    func_00000000(L + 0x10, x); \
    if (*(int *)(x + 0x14) != 0) *(int *)(x + 0x4) = 1; \
    *(int *)(x + 0x14) = (int)L; }
extern char D_6808_00, D_6808_01, D_6808_02, D_6808_03, D_6808_04, D_6808_05, D_6808_06, D_6808_07, D_6808_08, D_6808_09, D_6808_10, D_6808_11, D_6808_12, D_6808_13, D_6808_14, D_6808_15, D_6808_16, D_6808_17, D_6808_18, D_6808_19, D_6808_20, D_6808_21, D_6808_22, D_6808_23;
extern int import_8005C108_0[], import_8005C108_1[], import_8005C108_2[], import_8005C108_3[], import_8005C108_4[], import_8005C108_5[], import_8005EE64_0, import_8005EE64_1, import_8005EE64_2, import_8005EE64_3, import_8005EE64_4, import_8005EE64_5, import_8005EE64_6, import_8005EE64_7;
void *func_00006808(char *obj, char *arg1, s32 arg2) {
    int g0, g00;
    S4_55A0 e4, e0;
    int dc;
    char *g1;
    int d4, d0;
    char *g2;
    f32 c8, c4, c0;
    int bC, b8, b4, b0;
    char *g3;
    int count;
    f32 bA4, bA0, b9C;
    char *g4;
    char *va, *vb;
    char *L, *t;
    int *rec;
    char **gD;
    int *gE;
    S4_55A0 t78, t74, t70, t6C;
    int *p;
    int idx;
    char *o;
    char *res;
    int op;
    V3_6808 v4C;

    dc = 0;
    d4 = 0;
    p = *(int **)(arg1 + 0x40);
    bC = 0;
    b8 = 40;
    b4 = 20;
    b0 = 1;
    import_8005EE6C[1] = import_8005EE6C_r[1] | 0x20000;
    c8 = 0.0f;
    c4 = 0.0f;
    c0 = 0.0f;
    func_00000000(&D_6808_00, 0);
    t = (char *)func_00000000(0);
    import_8001FFC0 = t;
    ATTACH6808(*(char **)(obj + 0x84), t);
    bA0 = 0.0f;
    bA4 = 0.0f;
    b9C = 1.0f;
    D_bu_7770 = (char *)func_00000000(0, &D_6808_01, -1);
    idx = 0;
    do {
        count = *p >> 16;
        t = (char *)p++;
        switch (op = *(int *)t & 0xFFFF) {
        case 12:
            break;
        case 0: {
            func_00000000(&D_6808_02, 0);
            o = (char *)func_00000000(0);
            t = (char *)func_00000000(0, &D_6808_03, o);
            func_00000000(o, t);
            t78.w = *(int *)&D_6808_04;
            func_00000000(import_8001FFC0, o, t78);
            t74.w = *(int *)&D_6808_05;
            func_00000000(import_8001FFC0, t, t74);
            ATTACH6808(*(char **)(obj + 0x7C), o);
            func_00000000(o, *(int *)(obj + 0x94));
            t70.w = *(int *)&D_6808_06;
            func_00000000(import_8001FFC0, *(int *)(obj + 0x90), t70);
            t6C.w = *(int *)&D_6808_07;
            func_00000000(import_8001FFC0, *(int *)(obj + 0x94), t6C);
            func_00000000();
            break;
        }
        case 1:
            idx = p[1];
            func_00000000(import_8001FFC0, 1, p[0]);
            break;
        case 2:
            if (import_800200FC[0x68 / 4] < 2) {
                idx = p[1];
                func_00000000(import_8001FFC0, 1, p[0]);
            }
            break;
        case 3:
            if (import_800200FC[0x68 / 4] == 2) {
                func_00000000(import_8001FFC0, 1, p[0]);
                idx = p[1];
            }
            break;
        case 4:
            if (import_800200FC[0x68 / 4] == 3) {
                func_00000000(import_8001FFC0, 1, p[0]);
                idx = p[1];
            }
            break;
        case 5:
            bC = 1;
            b8 = p[0];
            b4 = p[1];
            b0 = p[2];
            break;
        case 6:
            if (arg2 != 6) {
                func_00000000(&D_6808_08, 0);
                func_00000000(import_800200FC, p[0]);
                o = (char *)func_00000000(p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
                t = o;
                ATTACH6808(*(char **)(obj + 0x84), t);
                *(char **)(import_8001FFC0 + 0x3C) = o;
                func_00000000();
            }
            break;
        case 7:
            func_00000000(&D_6808_09, 0);
            func_00000000(import_800200FC, p[0]);
            d0 = p[1];
            func_00000000();
            break;
        case 8:
            dc = p[0];
            break;
        case 9:
            d4 = p[0];
            break;
        case 10:
            c8 = p[0];
            c4 = p[1];
            c0 = p[2];
            break;
        case 11:
            bA4 = p[0];
            bA0 = p[1];
            b9C = p[2];
            break;
        }
        p += count;
    } while (op != 12);
    func_00000000();
    if (import_800200FC[0x3C / 4] == 6 || import_800200FC[0x3C / 4] == 11) {
        b4 = 5;
        b0 = 2;
    }
    gD = D_bu_776C;
    gE = D_bu_1EBDC;
    *gD = 0;
    *gE = 6;
    func_00000000(D_bu_7770, arg2, idx, *gE);
    {
        t = D_bu_7770;
        ATTACH6808(*(char **)(obj + 0x88), t);
    }
    func_00000000(&D_6808_10, 0);
    if (bC != 0) {
        import_8005C108_0[0x3C / 4] = import_8005C108_1[0x3C / 4] | 8;
    } else {
        import_8005C108_2[0x3C / 4] = import_8005C108_3[0x3C / 4] & ~8;
    }
    *gE = (import_8005EE64_0 = (import_8005EE64_1 & ~8) | 0x100, 6);
    import_8005EE6C[1] = 0xA8002;
    import_8005EE6C[0] = 0;
    func_00000000(&D_6808_11, 0);
    p = (int *)func_00000000(0, &D_6808_12, 0, 0);
    func_00000000();
    func_00000000(&D_6808_13, 0);
    func_00000000(((char *)p), d0);
    func_00000000();
    func_00000000(&D_6808_14, 0);
    *(int *)(((char *)p) + 0x50) = func_00000000(*(int *)(((char *)p) + 0x40) * 4);
    func_00000000();
    func_00000000(&D_6808_15, 0);
    func_00000000(((char *)p));
    func_00000000();
    import_8005EE64_2 = import_8005EE64_3 & ~0x100;
    *(int *)((char *)p + 0x30) |= 0x40;
    import_8005C108_4[0x3C / 4] = import_8005C108_5[0x3C / 4] & ~8;
    func_00000000();
    func_00000000(&D_6808_16, 0);
    {
        o = (char *)func_00000000(0, ((char *)p), *(int *)(obj + 0x90), *(int *)(obj + 0x94));
        t = o;
        ATTACH6808(*(char **)(obj + 0x84), t);
        func_00000000(o);
    }
    func_00000000();
    *gE = 1;
    res = (char *)func_00000000(0, ((char *)p), b8, b4, b0);
    *(int *)(res + 0x18) &= ~0x40;
    import_8001FFF0 = func_00000000(0, ((char *)p));
    func_00000000(&D_6808_17, 0);
    if (dc != 0) {
        func_00000000(D_bu_7770, 1, dc);
    }
    func_00000000();
    *(f32 *)(res + 0xC4) = c8;
    *(f32 *)(res + 0xC8) = c4;
    *(f32 *)(res + 0xCC) = c0;
    *(f32 *)(res + 0xD0) = bA4;
    *(f32 *)(res + 0xD4) = bA0;
    *(f32 *)(res + 0xD8) = b9C;
    {
        e4.w = *(int *)&D_6808_18;
        va = (char *)func_00000000(D_bu_7770, e4, 0);
        if (va != 0) {
            v4C = *(V3_6808 *)(va + 0x30);
            *(f32 *)(res + 0xC4) = v4C.x;
            *(f32 *)(res + 0xC8) = v4C.y;
            *(f32 *)(res + 0xCC) = v4C.z;
        } else {
            func_00000000((char *)&D_6808_19 + 0x7F5C);
        }
    }
    {
        e0.w = *(int *)&D_6808_20;
        vb = (char *)func_00000000(D_bu_7770, e0, 0);
        if (vb != 0) {
            v4C = *(V3_6808 *)(vb + 0x30);
            *(f32 *)(res + 0xD0) = v4C.x;
            *(f32 *)(res + 0xD4) = v4C.y;
            *(f32 *)(res + 0xD8) = v4C.z;
        } else {
            func_00000000(&D_6808_21);
        }
    }
    ATTACH6808(*(char **)(obj + 0x84), res);
    *gE = 6;
    func_00000000(&D_6808_22, 0);
    *gD = (char *)func_00000000(0, &D_6808_23, -1);
    import_8005EE64_4 = import_8005EE64_5 | 8;
    func_00000000(*gD);
    if (d4 != 0) {
        func_00000000(*gD, 1, d4);
    }
    if (arg2 == 1) {
        t = *gD;
        ATTACH6808(*(char **)(obj + 0x80), t);
    }
    func_00000000();
    import_8005EE64_6 = import_8005EE64_7 & ~8;
    return res;
}
#else
INCLUDE_ASM("asm/nonmatchings/bootup_uso", func_00006808);
#endif

/* func_00007150 - get-or-create constructor (0xB4, 45 insns).
 * EXACT 2026-07-15 (84.0% -> 45/45, ROM byte-exact, C is build path).
 * Object = 0x44 bytes: ->0x18 u32 flags (bit 3 cleared), ->0x28
 * descriptor ptr (&D runtime-patched), ->0x2C sub-ptr (inline &o+0x2C,
 * defensive alloc(4) arm when o == -0x2C), ->0x30 = 4, ->0x34 = 0xB00,
 * ->0x38 = 1, ->0x3C/0x40 = 0; D_00007F84 = init datum.
 * Levers: (1) destructive a0 reuse (no local copy) -> spill goes to the
 * a0 HOME slot 0x18(sp), frame 0x18 not 0x20; (2) the flag RMW uses the
 * if(1)-pointer-mutation idiom `q = a0; if (1) { q += 0x18; } *(int*)q
 * &= ~8;` to materialize `addiu v0,a2,0x18` + 0(v0) accesses (plain
 * `*(int*)(a0+0x18) &= ~8` folds into 0x18(a2) addressing, -1 insn);
 * (3) alloc(4) arm written `if ((v1 = r) == 0) goto after_v1;` so the
 * or v1,v0 copy fills the beqz delay slot (reload lw a2 hoists above
 * the branch). The if(1) braces and shared lines are LOAD-BEARING. */
extern char D_00007F84;
void *func_00007150(char *a0) {
    if (a0 == 0) {
        a0 = (char*)func_00000000(0x44);
        if (a0 == 0) goto end;
    }
    func_00000000(a0, &D_00007F84);
    *(void**)(a0 + 0x28) = &D_00000000;
    {
        int *v1 = (int *)(a0 + 0x2C);
        if (v1 == 0) {
            int *r = (int *)func_00000000(4);
            if ((v1 = r) == 0) goto after_v1;
        }
        *v1 = 0;
    after_v1:;
    }
    {
        char *q = a0;
        if (1) { q += 0x18; }
        *(int *)q &= ~8;
    }
    *(int*)(a0 + 0x40) = 0;
    *(int*)(a0 + 0x3C) = 0;
    *(int*)(a0 + 0x30) = 4;
    *(int*)(a0 + 0x34) = 0xB00;
    *(int*)(a0 + 0x38) = 1;
end:
    return a0;
}

/* func_00007204: 33-insn alloc/link helper. NATURAL CEILING: 90.15% NM.
 * Built emits 0x28 frame / 31 insns; target needs 0x30 frame / 33 insns
 * with defensive a2 dead-spill at jal-delay + a1<->a2 regalloc swap +
 * base-pointer form for a2 reload. The historical SUFFIX_BYTES_FORCE
 * (+8 bytes, 2 nops) + 31-entry INSN_PATCH promotion was REMOVED
 * 2026-05-23 as match-faking. Last of the 3-function bootup_uso regalloc
 * cluster (7C74/7B08/7204) per project_1080_bootup_regalloc_cluster.md. */
/* func_00007204: allocate a {word, M4} record (cfront new + inline ctors with
 * the member this==0 protocol) and add it to the list at a0 + 0x2C.
 * EXACT via an IDO 7.1 NCC (C++) donor: src/bootup_uso/bootup_uso_ncc_7204.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder (agent-c run 123). The add is
 * an inline list member forwarding to the import, which keeps the new-expression
 * result a separate web from the call argument (target a2, copied to a1). */
void func_00007204(int a0, int a1, int a2) {
    volatile int r = 0;
}

/* func_00007288: 40-insn (0xA0) two-conditional state-init helper.
 *
 * Structure:
 *   self = D[0x254];   // global state pointer at &D_00000000 + 0x254
 *   if (a0->0x38 & 2) {
 *       self->0xAC = (float)(int)a0->0x30;   // cvt.s.w via int→float regs
 *       self->0xB0 = (float)(int)a0->0x34;
 *   }
 *   if (a0->0x3C == 0) {
 *       self->0x78 |= 4;
 *       copy 4 words from (func_0000027C + 0x18..0x24) to self->0xDC..0xE8;
 *   }
 *
 * The `func_0000027C + 0x18` is the splat-fold-into-nearest-func pattern
 * (per docs/MATCHING_WORKFLOW.md): a real D_<addr> rodata symbol was folded
 * into the preceding function. */
/* EXACT (agent-c run 121): the record is a 16-byte struct copy from a pinned alias (bu_7288_src = func_0000027C + 0x18), and the flags RMW goes through the advanced pointer q (*q |= 4). */
/* 2026-07-31 (agent-g newest-lever sweep): 94.6 -> 95.4. Levers landed:
 * de-named self (per-use *(base+0x254) deref -> t-ring reload temps t9/t1
 * exact, kills the old named-self $v0 web) + named per-region base local +
 * named single-load `d` for the 4-store tail (one lw like target) + q
 * pointer-bump RMW (lw 120(v0)/addiu v0,v0,120/ori/sw 0(v0) shape exact).
 * Body is now insn-for-insn isomorphic to target (40 words, all opcodes
 * + offsets aligned); RESIDUAL is a pure register-name rotation web:
 * base colors $a2 vs target $v1, d $a1 vs t5, RMW temps $a0 vs t3/t4,
 * data temps t3-t6 vs t8/t7 ping-pong, plus the folded-rodata src-base
 * addend form — target addiu carries +0x18 (25ce0018 = %lo(func_0000027C)
 * +24: the ORIGINAL had its own D_00000294 rodata symbol, splat folded it
 * into func_0000027C) where C can only produce %lo+0 with the 0x18 folded
 * into the load offsets (24..36); if(1) pointer-mutation does NOT bake
 * addends on FUNCTION symbols (probed — emits a separate addiu+24, unlike
 * the data-symbol C14/2BE4 cases). Coloring probes: if(1) BB-split flips
 * base a2->v1 and makes region 1 + region-2 head BYTE-EXACT but costs +2
 * insns in the tail (split addiu + d remat, net fuzzy 91.3 — rejected);
 * def-order swap src<->RMW = 95.275; register kw inert. a-reg-vs-t-ring
 * named-web free-pool tie class. NM. */
typedef struct { int w[4]; } Q7288;
extern Q7288 bu_7288_src; /* = func_0000027C + 0x18 (asm %lo addend), pinned at 0x294 */
void func_00007288(int *a0) {
    if (a0[0x38/4] & 2) {
        char *base = (char*)&D_00000000;
        *(float*)(*(char**)(base + 0x254) + 0xAC) = (float)(int)a0[0x30/4];
        *(float*)(*(char**)(base + 0x254) + 0xB0) = (float)(int)a0[0x34/4];
    }
    if (a0[0x3C/4] == 0) {
        char *b2 = (char*)&D_00000000;
        int *q;
        q = *(int**)(b2 + 0x254);
        q = (int*)((char*)q + 0x78);
        *q |= 4;
        *(Q7288 *)(*(char **)(b2 + 0x254) + 0xDC) = bu_7288_src;
    }
}

/* func_00007328 - FULL structural decode (0x1C0, 112 insns, node-list
 * search + match-dispatch loop + tail state-set). fuzzy 24 -> 69 (2026-07,
 * agent-f). Logic + all reloc callees are correct; the FP-literal-pool
 * premise does NOT apply here (the single float 1.0f is emitted inline via
 * lui 0x3F800000 / mtc1, no pool).
 *
 * Structure (verified vs .s):
 *   while (func_00005EF8(&flag), flag != 0) {
 *       func_0000502C(&outv);
 *       node = s3->0x2C; cval = node ? node->0x4 : node;
 *       for (; node && node->0x0; node = cval, cval = node->0x4)
 *           if (flag == node->0x0->0x4) { match = node->0x0; break; }
 *       if (match) { vt = match->0x0->0x28;                 // vtable
 *                    (*(vt->0x4C))((char*)(match->0x0) + (short)vt->0x48); }
 *       else { func_00000000(&D_00007FA0,&flag);
 *              func_00000000(&D,1,outv); }
 *   }
 *   func_0000502C(&s3->0x38); ...flag-1 -> reg-desc; flag-2 -> free 0x30/0x34;
 *   mode=*(int*)(0x3C); if(mode==6||mode==0xB){ *(float*)0x2A0=1.0f;
 *       s3->0x34=0x320; s3->0x38|=2; }
 *
 * RESIDUAL CAP (why not EXACT, ~31% of words): correct-logic / divergent
 * register-allocation coloring, NOT a reloc-form or pool issue. My build
 * frame = -0x60, target = -0x58 (IDO spills the two cursor pointers
 * cval/node to distinct 0x44/0x48 stack slots AND holds &flag(0x54) +
 * &keybuf(0x34) in SAVED regs s0/s1 across every call — i.e. both are
 * treated address-taken; my C keeps them in caller-temp regs so the
 * allocator picks s2/s1 and grows the frame +8). This is the documented
 * permuter-immune coloring cap (project_1080_cap_analysis_2026-05-28):
 * same logic, spill-slot + saved-reg-number divergence, solvable only by
 * per-function exact-structure RE of the original stack-buffer shape
 * (func_00005EF8/keybuf likely wrote/compared a small stack STRUCT, not a
 * scalar). All three folded-symbol refs (func_0000027C+0x18,
 * func_00000008+0x34, func_0000029C+0x4) and the jal-0 callees ARE
 * reloc-correct here. INCLUDE_ASM remains build path. */
extern char D_00007F90;
extern char D_00007FA0;
/* func_00005EF8 / func_0000502C already file-scope-declared. */
extern void func_0000027C();   /* K&R data-symbol base (0x27C) */
extern void func_0000029C();   /* K&R data-symbol base (0x29C) */
extern void func_00000008();   /* K&R data-symbol base (0x08)  */
/* EXACT via an IDO 7.1 NCC (C++) donor: src/bootup_uso/bootup_uso_ncc_7328.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder (agent-c run 117). Handler list walk through the C++ {cur, nxt} iterator; see the .cpp header. */
void func_00007328(char *s3) {
}

/* func_000074E8 - verified structural decode (0x138, 78 insns,
 * guarded constructor + list-link + vtable + finalize).
 * Struct-typing reference: global gate *(func_00000008+0x2C) == 8
 * skips construction (already-done / disabled state). r = 0x134-byte
 * object, sub = 0x48-byte aux; both get a &D descriptor at ->0x28
 * (r's is the obj-0x28 vtable, 0x5C/0x58 variant: fn@0x5C (92),
 * s16@0x58 (88), dispatched with arg 0x96). a0->0x40 (64) = the
 * owning slot for r. r linked into the global list whose head is
 * (*(func_000000F0+0x44))->0x88 via r->0x14 (back-link) / r->0x4
 * (=1 when already-linked). r->0x48 (72) s32 toggled 1 then 0,
 * r->0x4C (76) f32 = 0.0 (init state/flags). D_00007FB0 / D_00007FBC
 * = init datums. Caps <80: global gate + ~6 reloc (alloc/init/
 * finalize) + &D descriptors + list-link beql + obj-0x28 vtable
 * jalr + FP. INCLUDE_ASM remains build path. */
extern char D_00007FB0, D_00007FBC;
extern void func_000000F0();  /* used as data-symbol base */
/* 2026-10-22 (agent-c run 120) EXACT: the final call takes NO argument (the target only stores p->0x48 = 0 in its delay slot; a0 is whatever the previous call left), and the child's init call and its +0x28 store share one source line (as1 line tie). */
/* run 111 (agent-c): cfront ctor shape (r = new(0x134); if (r) { sub = r; if (sub || (sub = new(0x48))) ... }),
 * no early returns, data symbols for the two %lo-folded globals, a0 escaped so every a0->0x40 is re-read,
 * v reused for the tail stores, block-count lever (IDO_CODEGEN#hold-vs-remat-firstusecost-run111). */
extern char D_74e8_g, D_74e8_h;
void func_000074E8(char *a0) {
    int *v;
    char *r;
    char *sub;
    char *head;
    if (*(int*)((char*)&D_74e8_g + 0x2C) == 8) return;
    func_00000000(&D_00007FB0, 0);
    r = (char*)func_00000000(0x134);
    if (r != 0) {
        sub = r;
        if (sub != 0 || (sub = (char*)func_00000000(0x48)) != 0) {
            func_00000000(sub, &D_00007FBC); *(void**)(sub + 0x28) = &D_00000000;
        }
        *(void**)(r + 0x28) = &D_00000000;
    }
    *(char**)(a0 + 0x40) = r;
    head = *(char**)((*(char**)((char*)&D_74e8_h + 0x44)) + 0x88);
    if (1) { }
    if (1) { }
    if (1) { }
    if (1) { }
    if (1) { }
    if (1) { }
    if (1) { }
    if (1) { }
    if (1) { char **pa = &a0; }
    func_00000000(head + 0x10, r);
    if (*(int*)(r + 0x14) != 0) *(int*)(r + 0x4) = 1;
    *(char**)(r + 0x14) = head;
    v = *(int**)(*(char**)(a0 + 0x40) + 0x28);
    (*(void (**)(char*, int))((char*)v + 0x5C))(
        (char*)((int)*(char**)(a0 + 0x40) + (int)(short)*(short*)((char*)v + 0x58)),
        0x96);
    v = *(int**)(a0 + 0x40); *(int*)((char*)v + 0x48) = 1;
    v = *(int**)(a0 + 0x40); *(float*)((char*)v + 0x4C) = 0.0f;
    *(int*)(*(char**)(a0 + 0x40) + 0x48) = 0;
    func_00000000();
}

/* func_00007620 - verified structural decode (0xD4, 53 insns,
 * enable/disable toggle on a substate object).
 * Struct-typing reference: a0->0x40 (64) = substate object (NULL =
 * no-op). substate->0x18 (24) flags word: bit 2 (0x4) and bit 3
 * (0x8) = the enable pair (set together on enable, cleared on
 * disable); substate->0x48 (72) = bound handle (= a1 on enable);
 * substate->0x4C (76) f32 = D_000005E8 on enable / 0.0 on disable;
 * substate->0xBC (188) = a gate that forces the enable path even
 * when a1==0. D_000005E8 = f32 constant (default value). [Caps note below RETRACTED 2026-09-04 — matched, see body.] Caps <80:
 * beql/bnel branch-likely + mtc1/swc1 FP + D_000005E8 f32 reloc +
 * reloc notify call. INCLUDE_ASM remains build path. */
extern float D_000005E8;
/* 2026-09-04 MATCHED 53/53 (agent-f): the old body merged `a1 != 0 ||
 * st->0xBC != 0` into one enable test; the target gates only the two
 * flag-CLEARs on 0xBC and always writes 0.0f + handle on the a1==0 path.
 * Levers: substate ptr reloaded per access (no cached local); each flag RMW
 * through an explicit `int *f = st + 0x18; *f op= k` (materializes the
 * addiu + lw/sw 0(v0) pairs, direct `*(int*)(st+0x18) |= k` folds the
 * displacement); enable-path swc1 base = the same `f` var re-pointed at st
 * with the 0x4C folded into the store (colors v0). */
#define ST_7620 (*(char**)(a0 + 0x40))
void func_00007620(char *a0, int a1) {
    int *f;
    if (ST_7620 == 0) return;
    func_00000000(a1);
    if (a1 == 0) {
        if (*(int*)(ST_7620 + 0xBC) == 0) {
            f = (int*)(ST_7620 + 0x18); *f &= ~0x8;
            f = (int*)(ST_7620 + 0x18); *f &= ~0x4;
        }
        *(float*)(ST_7620 + 0x4C) = 0.0f;
    } else {
        f = (int*)(ST_7620 + 0x18); *f |= 0x8;
        f = (int*)(ST_7620 + 0x18); *f |= 0x4;
        f = (int*)ST_7620; *(float*)((char*)f + 0x4C) = D_000005E8;
    }
    *(int*)(ST_7620 + 0x48) = a1;
}
#undef ST_7620

/* func_000076F4 - verified structural decode (0xDC, 55 insns).
 * Same FP transform/draw family as func_0000485C (shared globals:
 * root *(func_0000023C+0x18), scratch vector func_00000080+0x20,
 * scale consts &D+0x128/0x12C/0x130, a global emit counter).
 * Struct-typing reference: s0 = drawable/emitter object. s0->0x5C
 * (92) / 0x60 (96) s32 params, s0->0x64 (100) f32 (stack arg),
 * s0->0x70 (112) f32 * s0->0x80 (128) f32 = a derived f32 stack
 * arg, s0->0x6C (108) f32 y-offset, s0+0x2C = a sub-struct pointer
 * arg. root = *(func_0000023C+0x18) global context; func_00000080+
 * 0x20 = shared f32 scratch vector {0x30,0x34,0x38} scaled per-frame
 * by the &D+0x128/0x12C/0x130 scale triple (same as func_0000485C).
 * reloc_emit returns nonzero to proceed with the transform+draw.
 * Caps <80: FP mul.s/add.s chains + 3x reloc + cross-symbol data
 * refs (func_0000023C+0x18, func_00000080+0x20) + f32 stack-arg.
 *
 * 2026-06-02 (69.1->75.2%): the reloc_emit call's args 5,7 are single floats
 * (s0->0x64, s0->0x70*s0->0x80; target swc1 0x10/0x18) — typed-float proto
 * func_76f4_q avoids K&R double-promote. Residual ~25%: target FOLDS the root
 * load (lui%hi/lw%lo of func_0000023C+0x18) but the function-symbol arithmetic
 * here materializes it (extra addiu) + the 0x70/0x80 mul-operand load order.
 * Double-promote-scan vein. INCLUDE_ASM stays. */
extern void func_0000023C();  /* used as data-symbol base */
#ifdef NON_MATCHING
/* 2026-10-21 (agent-c run 115): reloc-identity re-decode (root = 0x254
 * (func_0000023C + 0x18), globals import_0005C108, v = the same import +
 * 0xA0, count import_0001FFC4; callees func_063D70 (typed floats),
 * func_05C444, func_04CCA8 as void). The count increment is inside the if.
 * Residual: in the target v and the scale base are the SAME symbol held in
 * two registers, so the scale loads stay after each store; two aliases let
 * IDO hoist them. Also the 0x70/0x80 load order and the hoisted G def.
 * Run 137 (81.75 -> 87.33): the root pointer is G+0x18 read through the same G
 * base (target `lw a0,24(a0)`; G then stays in a2 across the test call), and
 * the product is written 0x70 * 0x80 (load order). */
extern int func_00000000_76f4_q(void *, int, int, int, float, int, float);
extern char *bu_76f4_root;
extern char bu_76f4_g[], bu_76f4_v[];
extern int bu_76f4_cnt;
extern void func_00000000_76f4_v();
/* Run 150 (agent-g, 87.33 -> 3 words): the vector is read through a
 * FUNCTION-typed alias (bu_76f4_vf, target lo16 0xA0 = import + 0xA0): a data
 * array lets as1 prove it cannot alias the scale base and hoist the three
 * scale loads above the vector stores; the function symbol keeps them after
 * each store, as in the target. Left: lui v0 / lui a2 order after the call
 * (G is the rematerialised arg constant) and the pin (+0x20 addend). */
extern void bu_76f4_vf();
void func_000076F4(char *s0) {
    if (func_00000000_76f4_q(*(char **)(bu_76f4_g + 0x18), 3, *(int *)(s0 + 0x5C), *(int *)(s0 + 0x60),
                    *(float *)(s0 + 0x64), 0,
                    *(float *)(s0 + 0x70) * *(float *)(s0 + 0x80)) != 0) {
        func_00000000_76f4_v(bu_76f4_g, s0 + 0x2C);
        *(float *)((char *)bu_76f4_vf + 0x30) *= *(float *)(bu_76f4_g + 0x128);
        *(float *)((char *)bu_76f4_vf + 0x34) *= *(float *)(bu_76f4_g + 0x12C);
        *(float *)((char *)bu_76f4_vf + 0x38) *= *(float *)(bu_76f4_g + 0x130);
        *(float *)((char *)bu_76f4_vf + 0x34) += *(float *)(s0 + 0x6C);
        func_00000000_76f4_v(s0);
        bu_76f4_cnt += 1;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/bootup_uso", func_000076F4);
#endif

/* func_000077D0 - verified structural decode (0x278, 158 insns,
 * glyph/tile blit with mode dispatch). Font/HUD text renderer.
 *   void func_000077D0(St *a0, u16 *dst, int mode) {
 *       u16 *glyphs = a0->0x64;
 *       if (glyphs == NULL) return;
 *       base = a0->0x58;
 *       code = mode ? (base + 0x60) : base;       // char code
 *       switch (a0->0x5C) {                        // sub-mode
 *           case 0: stride = 0x140; ...            // plain
 *           case 2: stride = 0x140; ...            // variant
 *           case 1: ...                            // variant
 *           default: return;
 *       }
 *       c = code & 0xFF;
 *       if (c < 0xF0) {                            // valid glyph
 *           u16 *g = glyphs + c * stride;          // multu index
 *           for (row = 0; row < 0x40; row += 4) {
 *               *dst++ = g[0x00/2];
 *               *dst++ = g[0x0A/2];
 *               *dst++ = g[0x14/2];
 *               *dst++ = g[0x1E/2];
 *               g += 0x28/2;                       // next row
 *           }
 *       }
 *   }
 * Struct-typing reference: a0->0x64 (100) = glyph/tile pixel table
 * (NULL -> no-op); a0->0x58 (88) s32 = the character/tile code
 * (mode!=0 offsets it by +0x60 - e.g. alternate page); a0->0x5C
 * (92) s32 = render sub-mode (0/1/2 select stride 0x140 + the
 * per-row copy pattern; other = abort). Glyph cell layout: c <
 * 0xF0 valid range; cell = glyphs + c*stride; each of 16 rows
 * (loop 0..0x40 step 4) reads 4 u16 lanes at cell offsets 0x00/
 * 0x0A/0x14/0x1E and advances the cell by 0x28 bytes - i.e. a
 * 4-wide swizzled glyph blitted into the dst framebuffer/DL.
 * Caps <80: sub-mode switch (beq chain) + multu glyph index +
 * unrolled per-row halfword copy + branch-likely guards. Full
 * body INCLUDE_ASM-preserved (.s = source of truth). INCLUDE_ASM
 * (no episode; tautology-trap rule). */
/* EXACT 2026-10-17 (agent-c): the last 8 words (tbl and the 320 stride
 * swapped t0/t1) fall to a dead `if (tbl) {}` at the top of the mode-0 outer
 * loop: its loop-weighted refs lift tbl's compute_save above the stride
 * constant, so tbl colors first and takes t0. (Outside the loop it is inert.)
 * PASS-2 2026-10-16 (agent-c, 0 -> 99.75): source rewrite.
 * - stores go through a copy: `t = dst; dst++; *t = v;` is the target's
 *   `or v0,a1,zero; addiu a1,a1,2; sh tN,0(v0)` (a plain *dst++ = v gets
 *   strength-reduced to one +8 bump and sh -8/-6/-4/-2), same for the mode-1
 *   source pointer (`g = p; p++;`).
 * - goto loops: `for` / `do` loops get unrolled x4 by IDO again.
 * - stride is a variable (= 0x140): the target multiplies c * 320 with multu
 *   and doubles with sll; a literal folds to * 640.
 * - mode test order 0, 2, 1 as an if-goto chain (`if (mode != 1) return;
 *   goto m1;`), and `if (j == 0x40) goto next; goto l0;` keeps the mode-0
 *   copy loop's bne at the bottom.
 * - mode 1 rereads a float from func_411c_rf() (no args; the regs m2c passed
 *   are stale). `if (1) {}` before the loop and after the call splits dst and
 *   c around the call (sw a1,76 / sw a3,32) instead of s-regs.
 * - separate counters per loop (k, n, m); the 0x58 read goes through `mode`
 *   before the +0x60 adjust (target lw v0 + or a3,v0). The frame depends on
 *   the declaration list (every local takes a slot): `s32 pad[5]` and the
 *   unused `u` give 0x48 with the call spill at 32(sp). */
extern float func_411c_rf(void);
#define PUT77(v) { t = dst; dst++; *t = (v); }
void func_000077D0(char *arg0, u16 *dst, s32 arg2) {
    s32 pad[5];
    u16 *tbl;
    u16 *g;
    u16 *t;
    u16 *u;
    s32 c;
    s32 mode;
    s32 i;
    s32 j;
    s32 stride;
    s32 m;
    s32 n;
    s32 k;
    u16 *p;

    tbl = *(u16 **)(arg0 + 0x64);
    if (tbl == NULL) {
        return;
    }
    mode = *(s32 *)(arg0 + 0x58);
    c = mode;
    if (arg2 != 0) {
        c = mode + 0x60;
    }
    mode = *(s32 *)(arg0 + 0x5C);
    i = 0;
    stride = 0x140;
    if (mode == 0) goto m0;
    if (mode == 2) goto m2;
    if (mode != 1) return;
    goto m1;
m0:
    do {
        if (tbl) {}
        c &= 0xFF;
        i++;
        if (c < 0xF0) {
            g = tbl;
            g += c * stride;
            j = 0;
        l0:
            PUT77(g[0]);
            PUT77(g[5]);
            PUT77(g[10]);
            PUT77(g[15]);
            j += 4;
            g += 20;
            if (j == 0x40) goto next;
            goto l0;
        } else {
            k = 0;
        l1:
            PUT77(0);
            PUT77(0);
            PUT77(0);
            PUT77(0);
            k += 4;
            if (k != 0x40) goto l1;
        }
    next:
        c += 3;
    } while (i != 0x20);
    return;
m2:
    g = tbl;
    g += c * stride;
    n = 0;
    do {
        j = 0;
    l2:
        PUT77(g[0]);
        PUT77(g[5]);
        PUT77(g[10]);
        PUT77(g[15]);
        j += 4;
        g += 20;
        if (j != 0x40) goto l2;
        n++;
        g += 0x280;
    } while (n != 0x20);
    return;
m1:
    c = 0;
    if (1) {}
    do {
        p = (u16 *)&D_00000000 + (s32)(func_411c_rf() * 64.0f);
        if (1) {}
        m = 0;
        c++;
    l3:
        g = p; p++; PUT77(*g);
        g = p; p++; PUT77(*g);
        g = p; p++; PUT77(*g);
        g = p; p++; PUT77(*g);
        m += 4;
        if (m != 0x40) goto l3;
    } while (c != 0x20);
}

void func_00007A48(char *a0) {
    int scratch;
    func_00004FF0(&scratch);
    func_00006100((int*)(a0 + 0x10));
}

void func_00007A78(char *a0) {
    int scratch;
    func_00004FF0(&scratch);
    func_0000613C((int*)(a0 + 0x10));
}

void func_00007AA8(char *a0) {
    int scratch;
    func_00004FF0(&scratch);
    func_00004FF0((int*)(a0 + 0x10));
}

void func_00007AD8(char *a0) {
    int scratch;
    func_00004FF0(&scratch);
    func_00006194((int*)(a0 + 0x10));
}

/* func_00007B08: 36-insn (0x90) alloc/init/link constructor. BYTE-EXACT,
 * ported from its masked-hash twin donor eddproc_uso_func_000003BC — the two
 * bodies are byte-identical (0 non-reloc word diffs). The donor's clean C
 * form (int* param + p2/head/p1 decl order + p1[0x40/4] int-array indexing)
 * is what produces the target's frame (0x28) and $a0/$v1 register allocation,
 * which the prior permuter-mangled char* NM body (93.72% ceiling) could not
 * reach. Was once force-matched via 25-entry INSN_PATCH (banned 2026-05-23);
 * this is the genuine C match. */
void *func_00007B08(int *arg0) {
    int *p2;
    int *head;
    int *p1;
    p1 = (int*)func_00000000(0x40);
    if (p1 != 0) {
        func_00000000(p1);
        *(int*)((char*)p1 + 0x28) = (int)&D_00000000;
        *(int*)((char*)p1 + 0x3C) = 0;
    }
    p2 = p1;
    p1 = arg0;
    head = (int*)p1[0x40 / 4];
    if ((int*)p1[0x40 / 4] != 0) {
        func_00000000((char*)p2 + 0x10, head);
        if (*(int*)((char*)head + 0x14) != 0) {
            *(int*)((char*)head + 0x4) = 1;
        }
        *(int*)((char*)head + 0x14) = (int)p2;
    }
    return p2;
}

void func_00007B98(char *a0) {
    int scratch;
    func_00004FF0(&scratch);
    func_0000502C((int*)(a0 + 0x10));
}

void func_00007BC8(char *a0) {
    func_00004FF0((int*)(a0 + 0x3C));
    func_00000000(a0);
}

/* func_00007BF4 - EXACT (run 113 agent-c): twin port of gl_func_00035A18
 * (same vtable-dispatch thunk). Keys from the twin: plain if/else idx select
 * (no goto / branch-likely), loaded-value-LAST entry add, and the arg load +
 * accumulate joined on one line (hoists the lh above the lw). */
int func_00007BF4(char *a0) {
    char *arg;
    char *v1;
    char *entry;
    int (*fnptr)(char *);
    int idx;
    v1 = a0 + 8;
    arg = *(char**)(a0 + 4); arg = *(short*)(a0 + 8) + arg;
    if (*(short*)(a0 + 0xA) < 0) {
        fnptr = *(int (**)(char *))(a0 + 0xC);
    } else {
        int selector = *(int*)(v1 + 4);
        if (selector == 0 && *(short*)v1 == 0) idx = 0x28; else idx = selector;
        entry = (*(short*)(v1 + 2) << 3) + *(char**)(arg + idx);
        arg = *(short*)entry + arg;
        fnptr = *(int (**)(char *))(entry + 4);
    }
    return fnptr(arg);
}

extern float D_000005EC;
/* func_00007C74: 36-insn alloc/init/link helper. EXACT 2026-10-16 (was NM spill-slot "cap", fix at the end).
 * 6 residual diffs, all frame-slot offsets. ROOT CAUSE (re-decoded 2026-05-28):
 * BOTH mine and target spill `ret` to sp+0x1C; the ONLY difference is the
 * `link` spill slot — mine reuses the gap BELOW ret (sp+0x18, frame 0x20),
 * target allocates a FRESH HIGHER slot (sp+0x20) leaving 0x18 empty (frame 0x28).
 * This is IDO's spill-slot reuse-vs-fresh choice, not a missing pad.
 * NEGATIVE RESULTS (do not repeat): `volatile int pad[2]` is DCE'd (no &-anchor,
 * frame stays 0x20); `(void)&ret` is a no-op (ret already spilled); the
 * remove-local-inline-recompute lever REGRESSES to 19 diffs / 38 insns because
 * `link` has 4 uses across a jal so IDO adds a reload instead of CSE-ing to one
 * spill. PERMUTER tried 2026-05-28 (150s, -j4): floored at base score 10 (the
 * 6 spill-slot diffs), no crack — the higher-slot allocation is genuinely not
 * reachable. Member of project_1080_bootup_regalloc_cluster (7C74/7B08/7204).
 * Previously INSN_PATCH'd (removed 2026-05-23 as match-faking); now honestly NM.
 * See feedback_no_instruction_forcing_matches_policy.
 *
 * 2026-05-30 UCODE regalloc-dump (-Wo,-zdbug:6 -> uoptlist) — first application
 * of the unlocked dump to this cap. CONFIRMS the mechanism at UCODE level: the
 * fn has exactly 3 spilled live ranges (rlods=3 rstrs=3 numcalls=3), surfacing
 * as "-ve save" candidates 23/24/25 in the reg-alloc-prep section, plus the
 * param a0 (cand 12) "split out 26". The 3 spills are ret (across all 3 calls),
 * a0 (across the first 2 calls, until `link = *(a0+0x40)` is loaded), and link
 * (across the 3rd call). a0 spills to 0x18 then DIES at the link-load; mine
 * REUSES a0's freed 0x18 slot for link, target ABANDONS 0x18 and gives link a
 * fresh 0x20 (frame 0x28). This reuse-vs-abandon of a dead spill slot is pure
 * IDO spill-slot-coalescing policy, NOT C-reachable: loading link earlier to
 * change its candidate number would hoist its `lw` before the calls, breaking
 * instruction order. Dump-confirmed intractable; leave NM.
 *
 * 2026-07-15 re-probe with the B49C dead-scalar-home lever kit (NEGATIVE, do
 * not repeat): the target's empty 0x18 is NOT a reproducible dead scalar home.
 * In this dense scalar-only frame IDO -O2 elides dead homes entirely:
 * plain `int dead;`, `volatile int dead;`, coalesced `char *tmp = ret` (+
 * dead-if ref boost), and a live named `float f = D_000005EC` (which also
 * parks in $f0 via user-var reuse, 1C74-style) ALL leave frame -32 with dense
 * 0x18/0x1C spills. Dead homes materialize only in aggregate-bearing frames
 * (titproc 1710 class). Decl order does control which spill gets the higher
 * slot (link-first -> link above ret) but cannot skip a slot. Cap stands.
 *
 * 2026-10-16 EXACT (agent-c): the empty 0x18 IS a dead aggregate home, it just
 * needs BOTH halves at once: `char pad[4];` declared FIRST (gets the lowest
 * slot, 0x18, and grows the frame to 0x28) AND link declared before ret (link
 * spill above ret: ret 0x1C, link 0x20). Either alone stays off (pad-first with
 * ret-first decls puts ret at 0x20; link-first without pad keeps frame 0x20).
 * The 07-15 probes tried pad and decl order separately. */
void *func_00007C74(char *a0) {
    char pad[4];
    int *link;
    char *ret;

    ret = (char*)func_00000000(0x88);
    if (ret != 0) {
        func_00000000(ret);
        *(void**)(ret + 0x28) = &D_00000000;
    }

    *(float*)(ret + 0x80) = D_000005EC;
    link = *(int**)(a0 + 0x40);
    func_00000000(ret + 0x10, link);
    if (link[5] != 0) {
        link[1] = 1;
    }
    link[5] = (int)ret;

    return ret;
}

/* func_00007D04 - verified structural decode (0x14C, 83 insns,
 * phased animation/timer state machine).
 * Struct-typing reference: s0->0x5C (92) s32 = animation state /
 * next-state (0/1/2 phases); s0->0x60 (96) s32 = per-phase frame
 * counter (thresholds 0x1F / 0x3D / 0x5B per state); s0->0x58 (88)
 * s32 = a timer/position adjusted on phase transition (-0xA for
 * state1->next, =0x18 reset on state2 wrap); s0->0x64 (100) =
 * stored result of the obj-0x28 vtable dispatch (0x64/0x60
 * variant, root = *(func_0000023C+0x18)); s0->0x40 (64) -> obj
 * whose ->0x84 (132) is a message/anim list (entries -> ->0x1C
 * payload) driven by the two func_00000000 calls (idx 0 and 1,
 * arg = next state a3). State 2 wrap randomizes the next state
 * via rand01()*3.0. Caps <80: obj-0x28 vtable jalr + switch
 * (beq/beql chain) + FP rand*3.0 trunc + func_0000023C+0x18
 * reloc + reloc dispatch calls. INCLUDE_ASM remains build path. */
/* RUN 107 (agent-c): 58.0 -> 96.1. The target compares 1, 0, 2 with the
 * bodies out of line = nested-!= goto chain (IDO sorts switch compares).
 * The arms were swapped in the old body (kind 1 -> 31 / kind 0 -> 61 and
 * the -10 is unconditional / kind 2 -> 91 and the =24 is unconditional), each
 * compares the OLD counter (`slti at,old,K` -- an explicit load / store
 * pair, since `(*p)++ >= K` materializes slti/xori), and the dispatch value
 * and the counter share one variable (both v1). The rand helper returns a
 * float in $f0. Residual: the target's two list calls are an unrolled loop
 * (dead `addiu v0,v0,4`, a3 re-spilled before each call); every loop form
 * tried stays rolled (s-reg counter).
 * RUN 126 (agent-c): EXACT. The list pointer is its own variable `l`, not the
 * reused `k` (k = obj stays in v1, the list base goes to v0 as in the target);
 * `+ 1` on the int * gives the dead `addiu v0,v0,4`. */
extern char *D_7D04_root[];
extern float func_7D04_randf();
void func_00007D04(char *s0) {
    int a3 = *(int *)(s0 + 0x5C);
    int k;
    *(int *)(s0 + 0x64) =
        ((int (*)(char *))*((unsigned int *)((char *)*(int **)(D_7D04_root[6] + 0x28) + 0x64)))(
            (char *)((int)D_7D04_root[6] + (int)(short)*(short *)((char *)*(int **)(D_7D04_root[6] + 0x28) + 0x60)));
    k = *(int *)(s0 + 0x5C);
    if (k != 1) {
        if (k != 0) {
            if (k != 2) {
            } else goto c2;
        } else goto c0;
    } else goto c1;
    goto end;
c1:
    k = *(int *)(s0 + 0x60);
    *(int *)(s0 + 0x60) = k + 1;
    if (k >= 0x1F) a3 = 2;
    goto end;
c0:
    k = *(int *)(s0 + 0x60);
    *(int *)(s0 + 0x60) = k + 1;
    if (k >= 0x3D) a3 = 2;
    *(int *)(s0 + 0x58) -= 0xA;
    goto end;
c2:
    k = *(int *)(s0 + 0x60);
    *(int *)(s0 + 0x60) = k + 1;
    if (k >= 0x5B) {
        a3 = (int)(func_7D04_randf() * 3.0f);
        *(int *)(s0 + 0x60) = 0;
    }
    *(int *)(s0 + 0x58) = 0x18;
end:
    { int *l;
    k = *(int *)(s0 + 0x40); l = *(int **)(k + 0x84);
    func_00000000(s0, *(int *)(*l + 0x1C), 0, a3); if (1) {}
    k = *(int *)(s0 + 0x40); l = *(int **)(k + 0x84) + 1;
    func_00000000(s0, *(int *)(*l + 0x1C), 1, a3); }
    *(int *)(s0 + 0x5C) = a3;
}

void func_00007E50(int *a0) {
    *(int*)((char*)a0 + 0x68) = 0;
    func_00000000(a0);
}

extern int D_func_00007E70_a;
extern int D_func_00007E70_b;
void func_00007E70(char *a0) {
    D_func_00007E70_a = 0;
    func_00000000(a0);
    if (D_func_00007E70_b != 0) {
        int counter = *(int*)(a0 + 0x68) + 1;
        *(int*)(a0 + 0x68) = counter;
        if (counter == 1) {
            func_00000000();
        }
    }
}

/* func_00007EC8 - verified structural decode (0x1B0, 108 insns,
 * FP 1D collision / threshold-crossing test).
 * Struct-typing reference: s0->0x38 (56) f32 = position, s0->0x54
 * (84) f32 = velocity, s0->0x6C (108) f32 = a derived value,
 * s0->0x8C (140) s32 = a latched collision/zone state (compared
 * to the new hit flag - mismatch triggers the transition + the
 * func_0000027C+0x18 transform-block copy to a global). g =
 * *(&D_g); g->0x130 (304) f32 = world scale, g->0x254 (596) ->
 * 0x70 (112) -> 0xA8 (168) f32 = the boundary/limit position
 * (scaled). The (limit - p) / vel is the time-of-impact fraction.
 * Global gate *(func_00000008+0x2C) == 8 disables the test.
 * Likely a course/wall boundary crossing detector.
 * Caps <80: FP-heavy mul.s/c.lt.s/add.s/sub.s/div.s + bc1fl
 * branch-likely chain + &D + func_0000027C+0x18 cross-symbol block
 * copy. INCLUDE_ASM remains build path. */
/* EXACT (agent-c run 115). Identities from the bootup.uso TextReloc: the
 * mode word is import_000200FC + 0x2C (= D_00000000), the globals block is
 * import_0005C108 (zero alias bu_7ec8_g: +0x130 scale, +0x254 game object,
 * +0x18 a 24-byte record), the backup record is import_0001FFC8 (bu_7ec8_h);
 * (the asm spells the mode word func_00000008 + 0x2C = 0x34, alias
 * bu_7ec8_m, and the record func_0000027C + 0x18 = 0x294, alias
 * bu_7ec8_src: the baked words carry those offsets); callees func_0586D8 (char *, char *, float: frac goes out by mfc1 a2),
 * func_063D70 (four ints then float/int/float on the stack) and func_051D88.
 * The record copies are 24-byte struct assignments; the scale is re-read
 * inline (a scale local colours f2/f0 the wrong way round) and written
 * scale * field for the target mul.s operand order. */
typedef struct { int a, b, c, d, e, f; } S24_7EC8;
extern char bu_7ec8_g[], bu_7ec8_h[], bu_7ec8_src[];
extern int bu_7ec8_m;
extern void func_00000000_7ec8_c(char *, char *, float);
extern int func_00000000_7ec8_t(int, int, int, int, float, int, float);
void func_00007EC8(char *s0) {
    float scale, limit, p, end, frac;
    int hit;
    if (bu_7ec8_m != 8) {
        frac = 0.0f;
        limit = *(float *)(bu_7ec8_g + 0x130) * *(float *)(*(char **)(*(char **)(bu_7ec8_g + 0x254) + 0x70) + 0xA8);
        hit = 0;
        p = *(float *)(bu_7ec8_g + 0x130) * *(float *)(s0 + 0x38);
        end = *(float *)(s0 + 0x54) + p;
        if (p < limit && limit < end) {
            hit = 1;
            frac = (limit - p) / *(float *)(s0 + 0x54);
        } else if (end < limit && limit < *(float *)(s0 + 0x6C) + p) {
            frac = 1.0f;
            hit = 1;
        }
        if (hit != *(int *)(s0 + 0x8C)) {
            *(S24_7EC8 *)bu_7ec8_h = *(S24_7EC8 *)(bu_7ec8_src);
            *(int *)(s0 + 0x8C) = hit;
        }
        if (hit != 0) {
            *(S24_7EC8 *)(bu_7ec8_src) = *(S24_7EC8 *)bu_7ec8_h;
            func_00000000_7ec8_c(bu_7ec8_src, s0 + 0x74, frac);
        }
    } else {
        if (func_00000000_7ec8_t(*(int *)(bu_7ec8_g + 0x254), 3, *(int *)(s0 + 0x30),
                          *(int *)(s0 + 0x34), *(float *)(s0 + 0x38), 0, 0.0f) != 0) {
            func_00000000(s0);
        }
    }
}

void func_00008078(char *a0) {
    func_00000000(a0 + 0x3C, 0);
    func_00000000(a0 + 0x74);
    func_00000000(a0);
}

void func_000080B4(char *a0) {
    func_00000000((int*)(a0 + 0x3C));
    func_00000000((int*)(a0 + 0x74));
    func_00000000(a0);
}

void func_000080EC(char *a0) {
    func_00000000((int*)(a0 + 0x3C));
    func_00000000((int*)(a0 + 0x74));
    func_00000000(a0);
}

/* func_00008124 - verified structural decode (0x1D4, 117 insns).
 * NEAR-SIBLING of func_0001438C / func_000084A0 (bootup_uso
 * get-or-create constructor + child sub-objects family); variant
 * params: object 0x90, init datum D_00007878, child descriptor
 * D_00007FC4, child type D_00007540, child->0x10 = 200.0f
 * (0x43480000), and FP/&D constants from D_00007FCC/0x7FD0.
 *   void *func_00008124(void *a0) {
 *       o = a0 ? a0 : alloc(0x90); if (!o) return 0;
 *       init(o, &D_00007878);
 *       o->0x28 = &D_desc;
 *       s = alloc(0x38);
 *       c = alloc(8);                              // defensive arm
 *       if (c) { c->0x0 = &D_00007FC4; c->0x4 = 0; }
 *       v = *(int*)&D_00007FCC;
 *       if (s != (void*)-8) {
 *           c2 = alloc(0x18);
 *           if (c2) {
 *               init2(c2, s, v, 1);                // func_00000000
 *               c2->0xC  = &D_00007540;            // type/vtable
 *               c2->0x14 = 0;
 *               c2->0x10 = 200.0f;                 // 0x43480000
 *           }
 *       }
 *       ... (further child from D_00007FD0, same family shape)
 *   }
 * Struct-typing reference (same as func_0001438C): o = 0x90-byte
 * object, o->0x28 (40) descriptor (&D_00007878). s = 0x38-byte
 * sub; c = 8-byte aux (c->0x0 = &D_00007FC4 descriptor, c->0x4=0);
 * c2 = 0x18-byte child: c2->0xC (12) type ptr (&D_00007540),
 * c2->0x10 (16) f32 = 200.0 / 300.0, c2->0x14 (20) = 0. The u !=
 * -0x3C / -0x8 / -0x20 / -0x74 tests are the `c = u+k; if (c != 0
 * || alloc)` get-or-create guards folded by IDO. D_00007FCC /
 * D_00007FD0 = global init values feeding the children.
 * 2026-07-30 agent-g: 13D40 recipe applied (69.5 -> 94.5,
 * 116/117 insns, locals+frame-slot layout fully matched: p home
 * 0x24, w(c-save) 0x2C, int A[5] dead stores 0x40/0x3C base 0x30,
 * u caller-save spill, arg0 homed 0x50): Val8124 struct-by-value
 * arg3, char * volatile w save slot, TWO dead ptr decls q2/q1
 * flanking p shift locals +8 to place the 0x28 hole. NEW tail
 * lever: o reused for the loaded child d after `v = o` copy
 * (move v1,s0 / s0=load shape); a separate d var + o=d instead
 * spills o off s0 entirely (-5 insns worse). Residual = 13D40
 * allocator-web cap: 2 p.v table webs coalesce to v0 (vs t8/t2),
 * t-cycle renumber (t9/t1 vs t1/t5), u spill 0x4C + frame 0x58 vs
 * 0x44/0x50 (reserved spill words), [sw a0,0x2C] window rotation
 * x2, tail lw-v0/move-s0 two-step elided (116 vs 117). Sub-100 ->
 * NM wrap per policy. */
#ifdef NON_MATCHING
/* Run 123 (agent-c, raw 40 -> 31 words): frame 80 = declared locals v (sp+76,
 * the result's spill home), o, u (sp+68), A[5], w, q2, p, c; the size-8 inner
 * member reuses c (s was one word too many). Left: that member's this colours
 * a0 (target v1), the Val reads colour v0 (target t-ring temps), the a3 = 1
 * schedule, and the tail loads the parent into v0 before copying it to s0/a1
 * (the 7204 inline-member separation; a cfe temp copy lengthens it by 4). */
typedef struct { int v; } Val8124;
extern char D_00007878;
extern char D_00007FC4;
extern int D_00007FCC;
extern int D_00007FD0;
extern char D_00007540;
char *func_00008124(char *arg0) {
    char *v;
    char *o;
    char *u;
    int A[5];
    char * volatile w;
    char *q2;
    Val8124 p;
    char *c;

    o = (char *)func_00000000(0x90);
    if (o != 0) {
        func_00000000(o);
        *(char **)(o + 0x28) = &D_00007878;
        u = o + 0x3C;
        if (u != 0 || (u = (char *)func_00000000(0x38)) != 0) {
            c = u;
            if (c != 0 || (c = (char *)func_00000000(8)) != 0) {
                *(char **)(c + 0) = &D_00007FC4;
                *(int *)(c + 4) = 0;
            }
            p.v = D_00007FCC; A[4] = p.v;
            c = u + 8;
            if (c != 0 || (c = (char *)func_00000000(0x18)) != 0) {
                func_00000000(w = c, u, p, 1);
                c = w;
                *(char **)(c + 0xC) = &D_00007540;
                *(int *)(c + 0x14) = 0;
                *(float *)(c + 0x10) = 200.0f;
            }
            p.v = D_00007FD0; A[3] = p.v;
            c = u + 0x20;
            if (c != 0 || (c = (char *)func_00000000(0x18)) != 0) {
                func_00000000(w = c, u, p, 1);
                c = w;
                *(char **)(c + 0xC) = &D_00007540;
                *(int *)(c + 0x14) = 0;
                *(float *)(c + 0x10) = 300.0f;
            }
        }
        u = o + 0x74;
        if (u != 0 || (u = (char *)func_00000000(0x18)) != 0) {
            func_00000000(u);
        }
        *(int *)(o + 0x8C) = 0;
    }
    v = o;
    o = *(char **)(arg0 + 0x40);
    if (o != 0) {
        func_00000000(v + 0x10, o);
        if (*(int *)(o + 0x14) != 0) {
            *(int *)(o + 4) = 1;
        }
        *(char **)(o + 0x14) = v;
    }
    return v;
}
#else
INCLUDE_ASM("asm/nonmatchings/bootup_uso", func_00008124);
#endif

/* func_000082F8 — EXACT (54/54 post-link) 2026-07-03 agent-e. CRACKED the
 * documented "base $v0-vs-$v1 caller-saved renumber cap" (header note B, 2026-06-20).
 *
 * LEVER (NEW): call-1's callee was the K&R int-returning func_00000000 — its
 * UNUSED return created a dead $v0 def at the jal. The CSE'd &D base candidate
 * (live [0]..[41], rematerialized after call-1) interfered with that dead def,
 * excluding $v0 -> uopt colored base $v1 (zdbug:6 "R=1 reserved"). Declaring a
 * DISTINCT VOID-RETURNING extern for call-1 (func_00000000_082F8_v, same style
 * as the existing 7-arg func_00000000_082F8) removes the dead def; base takes
 * $v0. 8 diffs -> 1 in one edit.
 *
 * .o-level residual = 1 reloc-class word [31]: lw t8,0x2C(t8) + R_MIPS_LO16
 * D_func_00000008_data (=0x8) -> links to 0x34 = target word 8F180034. All
 * lui/addiu D-words reloc-resolve to 0 (D_00000000=0); all jals zero-target.
 * WIRING REQUIRED to land: add `func_00000000_082F8_v = 0x00000000;` to
 * undefined_syms_auto.txt (next to func_00000000_082F8, line ~1038).
 */
extern int func_00000000_082F8(int, int, int, int, float, int, float);
extern void func_00000000_082F8_v(int, int);
extern char D_00000000_a;
extern char D_func_00000008_data;
void func_000082F8(int *a0) {
    float f0 = *(float*)((char*)&D_00000000 + 0x130);
    float f2 = *(float*)((char*)*(int**)((char*)BOOT_SELF_PTR + 0x70) + 0xA8) * f0;
    float f12 = *(float*)((char*)a0 + 0x38) * f0;

    if (f12 < f2) {
        if (*(float*)((char*)a0 + 0x54) + f12 > f2) {
            func_00000000_082F8_v(*(int*)&D_00000000_a, *(int*)((char*)a0 + 0x6C));
        }
    }

    if (*(int*)(&D_func_00000008_data + 0x2C) == 8) {
        if (func_00000000_082F8(
            *(int*)((char*)&D_00000000 + 0x254),
            3,
            *(int*)((char*)a0 + 0x30),
            *(int*)((char*)a0 + 0x34),
            *(float*)((char*)a0 + 0x38),
            0,
            0.0f) != 0) {
            func_00000000(a0);
        }
    }
}

/* func_000083D0: 30-insn (0x78) 5-call init/dispatch wrapper. Callee
 * func_00000000 is the K&R cross-USO placeholder.
 *
 * Decoded: single int* arg, makes 5 sequential calls, no return:
 *   call(&D, &D_00007FD4, 0)
 *   call(&D, &D_00007FDC, (int)a0+0x6C, 1)
 *   call((int)a0+0x3C, 0)
 *   call(&D)
 *   call(a0)
 *
 * Pattern: $a0 saved at +0x18(sp) (= caller's a0 slot above this 0x18-byte
 * frame) at function entry, reloaded at every call that needs it (calls
 * 2/3/5). All delay slots are filled with addiu/move/lw arg setup. */
extern char D_00007FD4;
extern char D_00007FDC;
void func_000083D0(int *a0) {
    func_00000000(&D_00000000, &D_00007FD4, 0);
    func_00000000(&D_00000000, &D_00007FDC, (int)a0 + 0x6C, 1);
    func_00000000((int)a0 + 0x3C, 0);
    func_00000000(&D_00000000);
    func_00000000(a0);
}
/* Status 2026-05-18: build-path tested. 30/30 insns, build/.o vs
 * expected/.o differ at EXACTLY ONE word (w5, 0x14): the call-1
 * arg2. Target's .s uses %hi/%lo of LOCAL label `.L00007FD4`
 * (assembled-in intra-segment → expected/.o bakes addiu a1,a1,0x7FC4,
 * NO reloc). This NM uses extern `&D_00007FD4` (D_00007FD4=0x7FD4 in
 * undefined_syms) → emits a reloc with placeholder 0 (build/.o
 * 24A50000). The 0x10 delta (0x7FD4 vs 0x7FC4) = lui/addiu %hi-carry
 * of a local-label pair, NOT a flat symbol. w12 (&D_00007FDC) is
 * CORRECT (matches .o-vs-.o). Fix path: reference the local
 * `.L00007FD4` segment data symbol at the value that bakes 0x7FC4 in
 * %lo (account for %hi carry), not the flat extern. Reloc word →
 * INSN_PATCH-unsafe. One-word symbol-form cap; rest byte-exact. */

void func_00008448(char *a0) {
    func_00000000((int*)(a0 + 0x3C));
    func_00000000(a0);
}

void func_00008474(char *a0) {
    func_00000000((int*)(a0 + 0x3C));
    func_00000000(a0);
}

/* func_000084A0 - verified structural decode (0x1C4, 113 insns,
 * guarded singleton constructor). Member of the bootup_uso
 * alloc-cascade-ctor + defensive-dead-check family (cf.
 * func_00005124 / func_0001438C / func_00006254).
 *   void func_000084A0(void *a0) {
 *       g = *(St**)&D_root;
 *       if (g->0x40 != 0) return;                 // already built
 *       func_00000000(g);                          // pre-init
 *       o = alloc(0x74); if (!o) return;
 *       init(o, &D_00007968);
 *       o->0x28 = &D_desc;
 *       s = alloc(0x38); if (!s) return;
 *       c = alloc(8);                              // defensive arm
 *       if (c) { c->0x0 = &D_00007FE4; c->0x4 = 0; }
 *       v = *(int*)&D_00007FEC;
 *       if (s != (void*)-8) {                      // defensive guard
 *           c2 = alloc(0x18);
 *           if (c2) init2(c2, s, v, 1);            // func_00000000
 *       }
 *       ... (chain continues: more sub-allocs, FP 300.0f
 *           (0x43960000) param, &D_00007FF0/D_00007540 datums,
 *           func_00007620 helper - same shape as the family).
 *   }
 * Struct-typing reference: D_root global -> g, g->0x40 (64) =
 * built-once latch (nonzero = skip). o = 0x74-byte object,
 * o->0x28 (40) descriptor (&D_00007968 init datum). s = 0x38-byte
 * sub-object; c = 8-byte aux (c->0x0 = &D_00007FE4 descriptor,
 * c->0x4 = 0); c2 = 0x18-byte child init'd from the global
 * D_00007FEC value. The `u != -0x3C / -0x8 / -0x20` tests are the
 * `c = u+k; if (c != 0 || alloc)` get-or-create guards folded by
 * IDO. FP const 0x43960000 = 300.0f; block2 kind a3=0 and child2
 * gets {0x10=1 (int), 0xC=&func_00007620+0x70, 0x14=0} instead of
 * the float form.
 * 2026-07-30 agent-g: 13D40 recipe applied (62.5 -> 93.3, 113/113
 * structure, ALL homed slots exact via int A[6] + q2 dead decl
 * (9-word odd parity pads 0x20, p 0x24, q2 0x28, w 0x2C, A base
 * 0x30); v=o tail-reuse lever as 8124. Residual = 13D40
 * allocator-web cap (p.v webs coalesce to v0, t-cycle renumber,
 * u-spill/frame +0x10 reserved words, sw-a0 window x2, tail
 * lw-v0/move-s0 elide) + `(char *)func_00007620 + 0x70` emits
 * separate addiu 0x70 (addend does not fold into %lo since the
 * symbol is a real fn in this TU, unlike the extern-array
 * func_0000CACC form in 1438C) + head g = *(char**)&D_00000000
 * carries HI/LO relocs where target's lui/lw pair is baked
 * reloc-free (baked-USO-symbol cap; bytes identical). Sub-100 ->
 * NM wrap per policy. */
#ifdef NON_MATCHING
typedef struct { int v; } Val84A0;
extern char D_00007968;
extern char D_00007FE4;
extern int D_00007FEC;
extern int D_00007FF0;
extern char D_00007540;
char *func_000084A0(char *arg0) {
    char *o;
    char *v;
    char *u;
    char *s;
    char *c;
    char *g;
    int A[6];
    char * volatile w;
    char *q2;
    Val84A0 p;

    g = *(char **)&D_00000000;
    if (*(int *)(g + 0x40) == 0) {
        func_00000000(g);
    }
    o = (char *)func_00000000(0x74);
    if (o != 0) {
        func_00000000(o);
        *(char **)(o + 0x28) = &D_00007968;
        u = o + 0x3C;
        if (u != 0 || (u = (char *)func_00000000(0x38)) != 0) {
            s = u;
            if (s != 0 || (s = (char *)func_00000000(8)) != 0) {
                *(char **)(s + 0) = &D_00007FE4;
                *(int *)(s + 4) = 0;
            }
            p.v = D_00007FEC; A[4] = p.v;
            c = u + 8;
            if (c != 0 || (c = (char *)func_00000000(0x18)) != 0) {
                func_00000000(w = c, u, p, 1);
                c = w;
                *(char **)(c + 0xC) = &D_00007540;
                *(int *)(c + 0x14) = 0;
                *(float *)(c + 0x10) = 300.0f;
            }
            p.v = D_00007FF0; A[3] = p.v;
            c = u + 0x20;
            if (c != 0 || (c = (char *)func_00000000(0x18)) != 0) {
                func_00000000(w = c, u, p, 0);
                c = w;
                *(int *)(c + 0x10) = 1;
                *(char **)(c + 0xC) = (char *)func_00007620 + 0x70;
                *(int *)(c + 0x14) = 0;
            }
        }
    }
    v = o;
    o = *(char **)(arg0 + 0x40);
    if (o != 0) {
        func_00000000(v + 0x10, o);
        if (*(int *)(o + 0x14) != 0) {
            *(int *)(o + 4) = 1;
        }
        *(char **)(o + 0x14) = v;
    }
    return v;
}
#else
INCLUDE_ASM("asm/nonmatchings/bootup_uso", func_000084A0);
#endif

void func_00008664(char *a0, int *a1) {
    func_00000000(a0);
    if (*a1 == 7 || *a1 == 19) {
        *(float*)(a0 + 0xA0) = *(float*)(a0 + 0x5C);
        *(float*)(a0 + 0xA4) = *(float*)(a0 + 0x60);
        *(float*)(a0 + 0xA8) = *(float*)(a0 + 0x64);
    }
}

/* func_000086C0: 33-insn (0x84) two-init + linked-list dispatcher.
 *
 * Matching notes: reusing arg0 as the list iterator makes IDO keep it in
 * $s0 across the two init calls; the separate `node` null-test produces the
 * target's `lw v0` + branch-delay `or s0,v0` loop entry. The direct function
 * pointer call keeps the callback in $t9. */
extern int func_00000000();
extern char gl_ref_00000294;

void func_000086C0(char *arg0) {
    char *node;
    char *obj;

    func_00000000(&D_00000000, arg0 + 0x4C);
    func_00000000(arg0 + 0x5C);

    node = *(char**)(arg0 + 0x34);
    if (node != 0) {
        arg0 = node;
        do {
            if ((*(int*)(arg0 + 0x18) & 4) != 0) {
                obj = *(char**)(arg0 + 0x28);
                ((int (*)(char *))*(int*)(obj + 0x1C))(arg0 + *(short*)(obj + 0x18));
            }
            arg0 = *(char**)(arg0 + 0x84);
        } while (arg0 != 0);
    }
    func_00000000(&gl_ref_00000294);  /* alt-entry tail call to func_0000027C+0x18 */
}

extern char D_func_00008744_arg1;
extern char D_func_00008744_arg2;
void func_00008744(char *a0) {
    int arg = *(int*)(a0 + 0xC);
    func_00000000(&D_func_00008744_arg1, arg, 0);
    func_00000000(a0 + 0x4C, 0);
    func_00000000(a0 + 0x5C);
    func_00000000(&D_func_00008744_arg2);
    func_00000000(a0);
}

extern char D_00007FF4;
extern float D_000005F0;

/* func_000087A4: allocator/init sibling of func_000086C0. EXACT 2026-10-16
 * (agent-c). The old caps were all C-reachable:
 *  - the three bare `lui r,0` bases at the top are three DISTINCT base-0
 *    globals (D_87A4_rd read, D_87A4_wr OR-store, D_87A4_cfg +4/+0 stores),
 *    pinned to 0. An empty `if (1) {}` after the OR-store makes IDO hoist the
 *    rd/wr luis above the bnez and put the cfg lui in its delay slot (without
 *    it the cfg lui/addiu pair is hoisted instead).
 *  - the single-float 5th arg (swc1, no cvt.d.s) comes from a prototyped
 *    alias of the blank, func_00000000_087A4_f(char *, char *, int, float,
 *    float); 150.0f rides in a3 as bits. Unused-result calls go through the
 *    void alias func_00000000_087A4.
 *  - &func_00007A48 + 0x10 through the data alias D_087A4_vtable (pinned to
 *    0x7A48) folds the addend into %lo (a fn-typed symbol emits a separate
 *    addiu).
 *  - frame 0x50 with kind at sp+0x44: no named `desc` local (each extra named
 *    pointer took a ghost home) plus `int pad[5]` declared after kind.
 *  - the three trailing nops are file-end alignment pad (all-zero
 *    SUFFIX_BYTES_FORCE). */
extern int D_87A4_rd, D_87A4_wr, D_87A4_tail;
extern char D_87A4_cfg, D_87A4_vt;
extern char D_087A4_vtable[];
extern void func_00000000_087A4();
extern void func_00000000_087A4_f(char *, char *, int, float, float);
void *func_000087A4(char *arg0) {
    char *s1;
    char *s0;
    int kind; int pad[5];

    if (*(int *)(arg0 + 0x40) == 0) {
        D_87A4_wr = D_87A4_rd | 8;
        if (1) {}
        *(int *)(&D_87A4_cfg + 4) = 0xAA002;
        *(int *)(&D_87A4_cfg + 0) = 0;
        *(int *)(arg0 + 0x40) = func_00000000(0, &D_00007FF4, *(int *)(arg0 + 0x48), 0);
    }
    s1 = (char *)func_00000000(0xC8);
    if (s1 != 0) {
        func_00000000_087A4(s1);
        *(int *)(s1 + 0x28) = (int)(D_087A4_vtable + 0x10);
        s0 = s1 + 0x88;
        if (s0 == 0) {
            s0 = (char *)func_00000000(0x40);
            if (s0 == 0) {
                goto skip;
            }
        }
        func_00000000_087A4(s0);
        *(int *)(s0 + 0x30) = (int)&D_87A4_vt;
    }
skip:
    *(float *)(s1 + 0x70) = 10.0f;
    kind = 0x13;
    s0 = s1;
    ((void (*)(char *, int *))*(int *)(*(int *)(s1 + 0x28) + 0x2C))(s1 + *(short *)(*(int *)(s1 + 0x28) + 0x28), &kind);
    s1 = (char *)*(int *)(arg0 + 0x40);
    func_00000000_087A4(s0 + 0x10, s1);
    if (*(int *)(s1 + 0x14) != 0) {
        *(int *)(s1 + 4) = 1;
    }
    *(int *)(s1 + 0x14) = (int)s0;
    func_00000000_087A4_f(s0 + 0x88, s0, 0x74, 150.0f, D_000005F0);
    *(int *)(s0 + 0x98) |= 1;
    func_00000000_087A4(D_87A4_tail, s0 + 0x88);
    return s0;
}

extern char *D_00000130;
int func_00008920(int a0) {
    char *p = D_00000130;
    while (p != 0) {
        if (func_00000000(*(int*)p, a0) == 0) {
            return 1;
        }
        p = *(char**)(p + 4);
    }
    return 0;
}

int func_00008980(int arg0) {
    if (func_00000000(arg0) == 0) {
        func_00000000(&D_00000000, arg0);
        return 1;
    }
    return 0;
}

void func_000089C0(int *dst) {
    int buf[2];
    func_00000000(&D_00000000, buf, 4);
    *dst = buf[0];
}

void func_000089FC(short *dst) {
    short buf[4];
    func_00000000(&D_00000000, buf, 2);
    *dst = buf[0];
}

void func_00008A38(int a0) {
}

void func_00008A40(float *dst) {
    float buf[2];
    func_00000000(&D_00000000, buf, 4);
    *dst = buf[0];
}

extern void func_00008A7C();
void func_00008A7C(Vec3 *dst) {
    int pad_top[1];
    Tri3i raw;
    int pad_mid[2];
    Tri3i tmp;
    int pad_bot[2];
    func_00000000(&D_00000000, &raw, 12);
    tmp = raw;
    dst->x = *(float*)&tmp.a;
    dst->y = *(float*)&tmp.b;
    dst->z = *(float*)&tmp.c;
}

void func_00008AEC(Quad4 *dst) {
    Quad4 buf;
    func_00000000(&D_00000000, &buf, 16);
    *dst = buf;
}

/* func_00008B44: 36-insn (0x90) alloc-or-given init. BYTE-EXACT (0 non-reloc
 * diffs, 36/36 insns). func_0000D900 (still INCLUDE_ASM) takes the address of
 * two interior points of this function (8B44+0x28, 8B44+0x2C); its .s now
 * references them as `func_00008B44 + 0x28 / + 0x2C` (identical relocation
 * bytes) instead of the now-removed local labels `.L00008B6C` / `D_00008B70`.
 *
 * Two levers that took the prior 87.2% NM to byte-exact:
 *  - the (u16)a2 store must read the saved int slot's low halfword by MEMORY
 *    (`((unsigned short*)&a2)[1]` -> lhu 0x2A(sp)), not reload+truncate (lw).
 *  - the short store (0x174) must be emitted BEFORE the float stores in C so
 *    as1 schedules the 1.0f/10.0f lui/mtc1/swc1 materialization to match.
 * typed-float proto func_8b44_init keeps the 5th arg a single swc1. */
extern void func_8b44_init(char *, int, int, int, float);
char* func_00008B44(char *a0, int a1, int a2, int a3, int a4, float f_arg) {
    if (a0 == 0) {
        a0 = (char*)func_00000000(0x178);
        if (a0 == 0) goto end;
    }
    func_8b44_init(a0, a1, a3, a4, f_arg);
    *(int*)(a0 + 0x28) = (int)&D_00000000;
    *(int*)(a0 + 0x170) = a1;
    *(unsigned short*)(a0 + 0x174) = ((unsigned short*)&a2)[1];
    *(float*)(a0 + 0x150) = 1.0f;
    *(float*)(a0 + 0x154) = 10.0f;
end:
    return a0;
}

// func_00008BD4 — STRUCTURAL PASS (0x344 / 209 insns, no episode).
// Collision impulse / force-application onto a physics body, gated by
// the body's flag-class (body->0x174) and the impulse magnitude.
//
//   void func_00008BD4(Body *b, float amt, Vec3 *dir, x a3) {
//     u16 fl = b->0x174;
//     if (fl & 0x1F0) {                      // heavy-contact class
//       if (amt < 0.0f) return;
//       int code = func_00000000((*(G**)D_0)->0x84, b->0x14C);
//       if (code==0x61 || code==0x62 || code==0x63) return;  // 'a''b''c' skip
//       Ctx *cx = b->0x170;  cx->0x938++;                    // hit counter
//       if (amt < -30.0f) amt = -30.0f;       // clamp; cx->0x9A4 = (s16)code
//       cx->0xA10 |= fl;                                       // merge flags
//       float s = (float)(-(double)amt * 0.5);                 // scaled mag
//       Vec3 imp = { dir->x*s, dir->y*s, dir->z*s };
//       cx->0xB4  += imp;        // force/vel accum  (0xB4/B8/BC)
//       cx->0x93C += *dir;       // torque/pos accum (0x93C/940/944)
//       cx->0x968 += amt;        // scalar accum
//       Vec3 n = { dir->x*-amt, dir->y*-amt, dir->z*-amt };
//       func_00000000(cx, b+0x120, &n, ...);                  // apply
//       return;
//     }
//     if (fl & 0xF) {                         // light-contact class
//       if (amt < 50.0f) return;
//       Ctx *cx = b->0x170;  cx->0x938++;  cx->0xA10 |= fl;
//       cx->0x968 += amt;  cx->0x93C += *dir;
//       float q = fabsf(cx->0x970);
//       func_00000000(b+0x120, b+0x114, dir, q*q);            // apply
//       func_00000000(b, amt, dir, 0.1f);                     // restitution
//     }
//   }
//
// Struct-typing reference:
//   b->0x114/0x120 = sub-handlers' anchor offsets; b->0x14C lookup key;
//   b->0x170 = physics Ctx; b->0x174 = u16 contact-flag class
//     (bits 0x1F0 heavy / 0x0F light).
//   Ctx: 0xB4/B8/BC force-accum Vec3, 0x93C/940/944 torque-accum Vec3,
//     0x938 hit counter, 0x968 scalar accum, 0x970 a limit, 0x9A4 last
//     contact code (s16), 0xA10 merged flag bits.
//   Consts: -30.0 max-neg clamp, 0.5 half, 50.0 light threshold,
//     0.1f (0x3DCCCCCD) restitution; code 0x61/62/63 = skip set.
// Caps (64.1%->83.9% 2026-07-30 agent-g): structural residuals closed —
//   single-deref D_0 arg (*(char**)&D_0 + 0x84, was double), value-form ||
//   skip test (xori/sltiu chain + one bnel), de-named per-block pointer
//   locals for each cx accumulation group (fp/ap/tp/mp + light gp/np/wp;
//   kills the cx s-reg web -> v0/v1/a0 reloads like target), nnA = impB
//   (second marshal reads the staging copy, not nn), light-branch flag
//   merge re-reads *(u16*)(b+0x174) (not fl), two-var ternary fabs
//   (t/q -> bc1fl+mov/b+neg/dead-mov shape). b->s0, dir->s1 now exact.
//   REMAINING CAP (sole driver of every residual class): target parks amt
//   in callee-saved $f20 from the prologue (sdc1/ldc1, mtc1 a1,f20, frame
//   0xB8); our recomp IDO 7.1 spills the param to its arg-slot home around
//   each call (swc1/lwc1, f14) instead. Probe battery all-negative:
//   param copy-local (splits f12/f14 +mov.s), register kw, while(0)
//   ref-multiplier in 5 placements (assign-amt, dead-def-dup titproc
//   pattern), if(1)/else dead use, call-return web, multidef. Only a REAL
//   loop (for i<1, bne kept) promotes fp params to f20 in this toolchain
//   at depth 0 = the documented "live-across-call float in $f20" toolchain
//   cap (docs/IDO_CODEGEN.md 19555 class). Cascade: f2/f12/f14 free in
//   target = grouped 3-load/3-mul/3-store vec schedules, ||-ring a0-first,
//   entry lhu v0, mfc1-before-lui tail; all pressure-coupled to f20.
//   Frame-gap note: target locals impA@A0 nnA@94 impB@84 imp@74 nn@40
//   (+0x28 mid-gap, +0x10 low-gap); padded-carrier-struct probe ballooned
//   frame 0xF0 (cfe no-overlay) — offsets stay shifted, weigh light.
#ifdef NON_MATCHING
/* typed-float protos (0x0-aliases): float args (q*q; amt, 0.1f) pass single,
 * not K&R double-promote. */
extern void func_8bd4_a(void *, void *, void *, float *, float);
extern void func_8bd4_b(char *, float, float *, float);
extern void func_8bd4_heavy(void *, void *, void *, void *);
typedef struct { float x, y, z; } BD4V;
void func_00008BD4(char *b, float amt, float *dir, int a3) {
    unsigned short fl = *(unsigned short *)(b + 0x174);
    int code;
    int skip;
    float s;
    float negamt;
    BD4V impA, nnA, impB, imp, nn;
    float t;
    float q;
    if (fl & 0x1F0) {
        if (amt < 0.0f) {
            code = (int)func_00000000(*(int *)(*(char **)&D_00000000 + 0x84), *(int *)(b + 0x14C));
            skip = (code == 0x61 || code == 0x62 || code == 0x63);
            if (!skip) {
                *(int *)((*(char **)(b + 0x170)) + 0x938) += 1;
                *(short *)((*(char **)(b + 0x170)) + 0x9A4) = (short)code;
                if (amt < -30.0f) amt = -30.0f;
                negamt = -amt;
                {
                    unsigned short *fp = (unsigned short *)(*(char **)(b + 0x170) + 0xA10);
                    *fp |= *(unsigned short *)(b + 0x174);
                }
                s = (float)((double)negamt * 0.5);
                imp.x = dir[0] * s;
                imp.y = dir[1] * s;
                imp.z = dir[2] * s;
                impB = imp;
                impA = impB;
                {
                    float *ap = (float *)(*(char **)(b + 0x170) + 0xB4);
                    ap[0] += impA.x;
                    ap[1] += impA.y;
                    ap[2] += impA.z;
                }
                {
                    float *tp = (float *)(*(char **)(b + 0x170) + 0x93C);
                    tp[0] += dir[0];
                    tp[1] += dir[1];
                    tp[2] += dir[2];
                }
                {
                    float *mp = (float *)(*(char **)(b + 0x170) + 0x968);
                    *mp += amt;
                }
                nn.x = dir[0] * negamt;
                nn.y = dir[1] * negamt;
                nn.z = dir[2] * negamt;
                impB = nn;
                nnA = impB;
                func_8bd4_heavy(*(char **)(b + 0x170), b + 0x120, &nnA, &impB);
            }
        }
        return;
    }
    if (fl & 0xF) {
        if (amt < 50.0f) {
            *(int *)(*(char **)(b + 0x170) + 0x938) += 1;
            {
                unsigned short *gp = (unsigned short *)(*(char **)(b + 0x170) + 0xA10);
                *gp |= *(unsigned short *)(b + 0x174);
            }
            {
                float *np = (float *)(*(char **)(b + 0x170) + 0x968);
                *np += amt;
            }
            {
                float *wp = (float *)(*(char **)(b + 0x170) + 0x93C);
                wp[0] += dir[0];
                wp[1] += dir[1];
                wp[2] += dir[2];
            }
            {
                char *cxa = *(char **)(b + 0x170);
                t = *(float *)(cxa + 0x970);
                if (t < 0.0f) {
                    q = -t;
                } else {
                    q = t;
                }
                func_8bd4_a(cxa, b + 0x120, b + 0x114, dir, q * q);
            }
            func_8bd4_b(b, amt, dir, 0.1f);
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/bootup_uso", func_00008BD4);
#endif

/* func_00008F18: 36-insn (0x90) optional-alloc + multi-call init. */
extern char D_00008708;
extern char D_00008710;
extern char D_8F18_vtable;
extern char D_8F18_call_arg;
char *func_00008F18(char *self) {
    int handle;
    if (self == 0) {
        self = (char*)func_00000000(0x3D8);
        if (self == 0) goto end;
    }
    func_00000000(self, &D_00008708);
    *(char**)(self + 0x28) = &D_8F18_vtable;
    handle = func_00000000(0, &D_00008710, &D_8F18_call_arg, 0);
    func_00000000(self, handle);
    *(int*)(self + 0x3D4) = 100;
    *(float*)(self + 0x260) = 50.0f;
end:
    return self;
}

/* func_00008FA8: 73-insn (0x124) "scatter-bitcast-int-to-float-vec3-fields"
 * + counter decrement + global-table update. Sibling of recently-matched
 * func_00008F18 (alloc + multi-call init).
 *
 * Structural shape (decoded):
 *   1. 4-stage Vec3 scatter via stack int-to-float bit-cast:
 *      read 3 ints from self->[0xB4..0xBC], write as floats to self->[0xDC..0xE4]
 *      read 3 ints from self->[0xC0..0xC8], write as floats to self->[0xE8..0xF0]
 *      read 3 ints from self->[0xCC..0xD4], write as floats to self->[0xF4..0xFC]
 *      read 1 float from self->[0xD8], write to self->[0x100]
 *      Each int-to-float goes through stack scratch sp+0x2C; bit-pattern
 *      preserved (no conversion).
 *   2. Cross-USO call gl_func_00000000(self) at 0x904C.
 *   3. Counter decrement (self->[0x3D4]) + global-table append on
 *      counter zero-cross. Table at *(D_00000000) with [0x8]=max [0xC]=count.
 *      Realloc-via-call when count >= max, then append self.
 *
 * RECONSTRUCTED 2026-06-20: faithful body now ~94% (13 word diffs, was 71.9%).
 * Vec3i struct-copy idiom reproduces the v0-store/sp-load scatter exactly;
 * return is void; 0xFC written before 0x100; tbl spilled across realloc
 * (count re-read, tbl not). volatile pad_hi/pad_lo size the 0x48 frame
 * (zero-instruction phantom slots). RESIDUAL CAP: IDO as1 hoists the
 * &D_00008714 address into the pre-branch delay region, forcing `count`
 * off $a0 onto $v1 (coloring-renumber), plus the tbl-spill slot lands at
 * 0x1C instead of 0x40. Pure scheduler-tie + coloring; not C-fixable.
 * Run 129 (agent-c) 15 -> 13 raw: tbl declared at function scope between
 * `volatile int pad_t[1]` and `pad_hi[2]` -- its home IS the spill slot, now
 * sp+0x40 as the target. LEFT: count v1 (target a0) lets the &D_8714 lui
 * hoist above the compare; while (0) / trailing if (count) {} are flat.
 * Run 131 (agent-c) EXACT: the pre-realloc compare reads tbl[3] directly and
 * count is assigned only AFTER the if (one load, not a count copy reloaded in
 * the then-arm) -> count colours a0 and the &D_8714 lui stays in the arm. */
extern char D_00008714;
typedef struct { int a, b, c; } Vec3i_8FA8;
void func_00008FA8(int *self) {
    volatile int pad_t[1];
    int *tbl;
    volatile int pad_hi[2];
    Vec3i_8FA8 scratch;
    volatile int pad_lo[2];
    scratch = *(Vec3i_8FA8*)((char*)self + 0xB4);
    *(float*)((char*)self + 0xDC) = *(float*)&scratch.a;
    *(float*)((char*)self + 0xE0) = *(float*)&scratch.b;
    *(float*)((char*)self + 0xE4) = *(float*)&scratch.c;
    scratch = *(Vec3i_8FA8*)((char*)self + 0xC0);
    *(float*)((char*)self + 0xE8) = *(float*)&scratch.a;
    *(float*)((char*)self + 0xEC) = *(float*)&scratch.b;
    *(float*)((char*)self + 0xF0) = *(float*)&scratch.c;
    scratch = *(Vec3i_8FA8*)((char*)self + 0xCC);
    *(float*)((char*)self + 0xF4) = *(float*)&scratch.a;
    *(float*)((char*)self + 0xF8) = *(float*)&scratch.b;
    *(float*)((char*)self + 0xFC) = *(float*)&scratch.c;
    *(float*)((char*)self + 0x100) = *(float*)((char*)self + 0xD8);
    func_00000000(self);
    {
        int counter = self[0x3D4/4];
        self[0x3D4/4] = counter - 1;
        if (counter <= 0) {
            int count;
            tbl = *(int**)&D_00000000;
            if (tbl[3] >= tbl[2]) {
                func_00000000(&D_00008714, self);
            }
            count = tbl[3];
            tbl[3] = count + 1;
            *(int*)(tbl[0] + count * 4) = (int)self;
        }
    }
}

#ifdef NON_MATCHING
/* func_000090CC: 2106-insn (0x20E8) alloc-cascade constructor.
 * Sibling of recently-matched func_00008F18 (alloc + multi-call init,
 * 36 insns) — much larger version with ~10+ alloc-or-use cascades.
 *
 * Strategy-memo spine class: bigger constructor with many sub-object
 * allocs. This is a multi-pass-decomp candidate; expect 30-50% NM on
 * initial pass, refining per-block over multiple iterations.
 *
 * Frame -0x178 (376 bytes). Saves s0/s1/s2/ra. Args saved to caller
 * slots at sp+0x17C/0x180/0x184 (a1/a2/a3).
 *
 * Initial decoded structure (first ~30 of 2106 insns):
 *   if (self == 0) {
 *       self = func(0xA8C);              // alloc 2700 bytes
 *       if (!self) return 0;
 *   }
 *   func(self, &D_00008730);              // init self
 *   self->[0x28] = &D_<reloc>;            // vtable
 *
 *   // cascade sub1: at self+0x3EC if self exists, else alloc 0x154
 *   if (self != (int*)0xFFFFFC14) {
 *       sub1 = self + 0x3EC;
 *   } else {
 *       sub1 = func(0x154);
 *       if (!sub1) return 0;
 *   }
 *   func(sub1, &D_00008738);
 *   sub1->[0x28] = &func_000080EC + 0x1C; // child vtable
 *
 *   // cascade sub2: at sub1+0x2C if sub1 exists, else alloc 0x128
 *   if (sub1 != (int*)0xFFFFFFD4) {
 *       sub2 = sub1 + 0x2C;
 *   } else {
 *       sub2 = func(0x128);
 *       if (!sub2) return 0;
 *   }
 *
 *   // cascade sub3: alloc 8 if sub2 exists; init sub2 fields
 *   if (sub2 != 0) {
 *       sub3 = func(8);
 *       if (sub3) {
 *           sub3->[0] = &D_00008740;
 *           sub3->[4] = 0;
 *       }
 *   }
 *
 *   ... (~2000 more instructions of similar cascade-and-init patterns) ...
 *
 *   end:
 *       return self;
 *
 * Stub C body — 30-50% structural match expected. Multi-pass NM. */
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
extern char bu_90cc_d207dc;
extern char bu_90cc_imp30;  /* +0x30 import ptr in the 0x878 cascade (target 0xA2F8 lui/addiu/sw pair; was decoded as literal 0) */
/* target 0xAC34-0xAD3C per-site blank globals (each site = own symbol; busts base GCSE).
 * Scalar/array form (not cast-arith) so the lo16 folds into the load offset (lui rd; lw rd,4(rd))
 * and stores go through $at -- matches target's direct-global access shape. */
extern char bu_90cc_g8_ld, bu_90cc_g8_st;      /* load/store alias pair: same address, distinct syms kill the addr CSE (lui rd/lw + lui at/sw shape) */
extern char bu_90cc_gcfg_ld, bu_90cc_gcfg_st;
extern char bu_90cc_gcfg_b_ld, bu_90cc_gcfg_b_st;
extern s32 bu_90cc_gown;
extern char bu_90cc_gtbl;
extern char bu_90cc_gpair;
extern char bu_90cc_gdev;
extern char bu_90cc_vt_self;  /* self +0x28 vtable (was decoded as literal 0; expected has lui/addiu reloc pair) */
/* vtable draw slot (+0x5C): trailing two args are single-precision f32.
 * Prototyping defeats the K&R float->double promotion so single swc1
 * arg stores emit (matches target; no cvt.d.s/sdc1). */
typedef void (*Vfn_ff)(s32, s32, s32, void *, s32, s32, f32, f32);
typedef struct { s32 w; } W90;
extern char D_00008778;
extern void func_00000000_p4f(void *, s32, s32, s32, f32);
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
extern char bu_90cc_d00_b;  /* second read of D_00000000.0 in the sp174 list walk: distinct symbol keeps the target's reload (else CSE'd into a copy) */
typedef struct { s32 f00, f04, f08, f0C, f10, f14, f18, f1C, f20, f24, f28, f2C, f30, f34, f38; } BuW90;
/* scalar-field form: a named global field read is a uopt candidate (target lui/lw temps land in s0/s1/a2, not the t-ring) */
extern BuW90 bu_90cc_d44_01, bu_90cc_d44_02, bu_90cc_d44_03, bu_90cc_d44_04, bu_90cc_d44_05, bu_90cc_d44_06, bu_90cc_d44_07, bu_90cc_d44_08, bu_90cc_d44_09, bu_90cc_d44_10, bu_90cc_d08_01, bu_90cc_d08_02, bu_90cc_d08_03, bu_90cc_d08_04, bu_90cc_d80_01, bu_90cc_d80_02, bu_90cc_d80_03;
extern void func_00000000_p4d(void *, s32, f64, f32);
extern void bu_90cc_fz();  /* 0x814/818/81C source base: target lui v0/addiu v0 (no reloc) then lw 0/8/4(v0) -- a fn-typed symbol keeps the %lo in an addiu so the base is CSEd in one register */
extern void bu_90cc_fz2();  /* sp174 list-walk head (same blank address), fn-typed for the same reason */
void *func_000090CC(char *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5, f32 arg6, s32 arg7, s32 arg8) {
    s32 sp174;
    Vec3 vpos;
    char *sp164;
    char *sp160;
    char *sp15C;
    char *sp158;
    s32 var_t1;
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
    s32 *var_s0_3;
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
    s32 *sp40;

    if ((arg0 != 0) || (temp_v0_5 = func_00000000(0xA8C), arg0 = temp_v0_5, (temp_v0_5 != 0))) {
        func_00000000(arg0, &D_00008730);
        *(s32 *)((char *)(arg0) + 0x28) = (s32) &bu_90cc_vt_self;
        var_s0 = arg0 + 0x3EC;
        if (((arg0 != (void *)-0x3EC) || ((var_s0 = func_00000000(0x154)) != 0)) && ((func_00000000(var_s0, &D_00008738), *(s32 *)((char *)(var_s0) + 0x28) = (s32) &D_00008108, var_s1 = var_s0 + 0x2C, (var_s0 != (void *)-0x2C)) || (temp_v0_5 = func_00000000(0x128), var_s1 = temp_v0_5, (temp_v0_5 != 0)))) {
            var_a2 = var_s1;
            if ((var_s1 != 0) || (temp_v0_5 = func_00000000(8), var_a2 = temp_v0_5, (temp_v0_5 != 0))) {
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
                var_s1 = (char *)&sp7C;
            }
            spFC.w = *(s32 *)((char *)&D_00000000 + 0x8790);
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
        sp40 = var_s0;
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
        if (1) { fq += 6; }
        *fq &= ~2;
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
        var_t1 = arg8;
        goto sw_end;
sw_c7:
        *(s16 *)((char *)(arg0) + 0x902) = 0x1A;
        *(s16 *)((char *)(arg0) + 0x900) = 0x19;
        var_t1 = arg8 | 1;
        goto sw_end;
sw_c6:
        *(s16 *)((char *)(arg0) + 0x902) = 0x18; *(s16 *)((char *)(arg0) + 0x900) = 0x17;
        var_t1 = arg8 | 1;
sw_end:
        sp174 = 0;
        if ((arg1 == 1) && (*(s32 *)((char *)&D_00000000 + 0x58) == *(s32 *)((char *)&D_00000000 + 0x4C))) {
            sp174 = 1;
        }
        if ((*(s32 *)((char *)&D_00000000 + 0x34) == 3) && (*(u8 *)((char *)&D_00000000 + 0x17D) == *(s32 *)((char *)&D_00000000 + 0x4C)) && (arg1 == 2)) {
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
                    *(f32 *)((char *)(arg0) + 0x8EC) = (f32) 255 / inv;
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
                    *(f32 *)((char *)(arg0) + 0x8EC) = (f32) 255 / inv;
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
            var_t1 &= ~4;
        } else {
            if (arg7 & 8) {
                inv = 255.0f;
                temp_f0 = (f32) 255 / inv;
                *(f32 *)((char *)(arg0) + 0x8E0) = temp_f0;
                *(f32 *)((char *)(arg0) + 0x8E4) = temp_f0;
                *(f32 *)((char *)(arg0) + 0x8E8) = temp_f0;
                *(f32 *)((char *)(arg0) + 0x8EC) = 117.0f / inv;
            } else {
                inv = 255.0f;
                temp_f0 = (f32) 255 / inv;
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
        *(s32 *)((char *)(arg0) + 0xA58) = temp_v0_51;
        *(s32 *)((char *)(arg0) + 0x8B8) = var_t1;
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
        *(s32 *)((char *)(arg0) + 0x8C0) = (s32) (1 << arg2);
        *(s32 *)((char *)(arg0) + 0x38C) = 1;
        *(s32 *)((char *)(arg0) + 0x870) = 0;
        *(s32 *)((char *)(arg0) + 0x868) = 0;
        *(s32 *)((char *)(arg0) + 0x86C) = 0;
        *(f32 *)((char *)(arg0) + 0x3AC) = wpos.z;
        func_00000000(sp40, arg1, arg2);
        if (*(s32 *)((char *)(arg0) + 0xA58) & 3) {
            func_00000000(bu_90cc_d87A4 + 0x5C, 0);
            *(s32 *)((char *)(arg0) + 0x86C) = func_00000000(0, bu_90cc_d87A4 + 0x64);
            *(s32 *)((char *)(arg0) + 0x868) = func_00000000(0, &D_00008814);
            var_s0 = *(s32 *)((char *)(arg0) + 0x86C);
            var_s0 += 0x18;
            *(s32 *)var_s0 &= ~8;
            var_s1 = *(s32 *)((char *)(arg0) + 0x86C);
            var_s0 = *(s32 *)(*(char **)(bu_90cc_dF0 + 0x44) + 0x88);
            func_00000000(var_s0 + 0x10, var_s1);
            if (*(s32 *)((char *)(var_s1) + 0x14) != 0) {
                *(s32 *)((char *)(var_s1) + 0x4) = 1;
            }
            *(s32 *)((char *)(var_s1) + 0x14) = (s32) var_s0;
            temp_v0_5 = func_00000000(0, &D_00008820);
            *(s32 *)((char *)(arg0) + 0x868) = temp_v0_5;
            fq = (s32 *)temp_v0_5;
            if (1) { fq += 6; }
            *fq &= ~8;
            var_s1 = *(s32 *)((char *)(arg0) + 0x868);
            var_s0 = *(s32 *)(*(char **)(bu_90cc_dF0 + 0x44) + 0x84);
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
            var_s1 = *(s32 *)((char *)(arg0) + 0X868);
            func_00000000(var_s1 + 0x10, var_s0);
            if (*(s32 *)((char *)(var_s0) + 0x14) != 0) {
                *(s32 *)((char *)(var_s0) + 0x4) = 1;
            }
            *(s32 *)((char *)(var_s0) + 0x14) = (s32) var_s1;
            if (*(s32 *)((char *)(arg0) + 0xA58) & 1) {
                var_s1 = func_00000000(0X1F4);
                if (var_s1 != 0) {
                    var_a2 = (s32) var_s1;
                    if ((var_a2 != 0) || (var_a2 = func_00000000(0x174), (var_a2 != 0))) {
                        func_00000000((char *)var_a2, &D_00008834);
                        *(s32 *)((char *)(var_a2) + 0x28) = (s32) &bu_90cc_vt545_b;
                    }
                    *(s32 *)((char *)(var_s1) + 0x28) = (s32) &bu_90cc_vt547;
                }
                sp160 = var_s1;
                var_s0 = var_s1;
                var_s1 = *(s32 *)((char *)(arg0) + 0X86C);
                func_00000000(var_s1 + 0x10, var_s0);
                if (*(s32 *)((char *)(var_s0) + 0x14) != 0) {
                    *(s32 *)((char *)(var_s0) + 0x4) = 1;
                }
                *(s32 *)((char *)(var_s0) + 0x14) = (s32) var_s1;
            }
            var_s1 = func_00000000(0X174);
            if (var_s1 != 0) {
                var_a2 = (s32) var_s1;
                if ((var_a2 != 0) || (var_a2 = func_00000000(0x174), (var_a2 != 0))) {
                    func_00000000((char *)var_a2, &D_0000883C);
                    *(s32 *)((char *)(var_a2) + 0x28) = (s32) &bu_90cc_vt545_c;
                }
                *(s32 *)((char *)(var_s1) + 0x28) = (s32) &bu_90cc_vt548;
            }
            sp15C = var_s1;
            var_s0 = var_s1;
            var_s1 = *(s32 *)((char *)(arg0) + 0X868);
            func_00000000(var_s1 + 0x10, var_s0);
            if (*(s32 *)((char *)(var_s0) + 0x14) != 0) {
                *(s32 *)((char *)(var_s0) + 0x4) = 1;
            }
            *(s32 *)((char *)(var_s0) + 0x14) = (s32) var_s1;
            var_s1 = func_00000000(0X174);
            if (var_s1 != 0) {
                var_a2 = (s32) var_s1;
                if ((var_a2 != 0) || (var_a2 = func_00000000(0x174), (var_a2 != 0))) {
                    func_00000000((char *)var_a2, &D_00008844);
                    *(s32 *)((char *)(var_a2) + 0x28) = (s32) &bu_90cc_vt545_d;
                }
                *(s32 *)((char *)(var_s1) + 0x28) = (s32) &bu_90cc_vt548;
            }
            sp158 = var_s1;
            var_s0 = var_s1;
            var_s1 = *(s32 *)((char *)(arg0) + 0X868);
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
            *(s32 *)((char *)(var_s1) + 0x120) = 0xFFFF; *(s32 *)((char *)(var_s1) + 0x28) = (s32) &bu_90cc_d207dc;
            *(f32 *)((char *)(var_s1) + 0x108) = temp_f0;
            *(f32 *)((char *)(var_s1) + 0x10C) = temp_f0;
            *(f32 *)((char *)(var_s1) + 0x110) = temp_f0;
            *(f32 *)((char *)(var_s1) + 0x124) = 1.0f;
        }
        *(s32 *)((char *)(arg0) + 0x850) = var_s1;
        func_00000000(arg0, var_s1);
        var_s0 = *(s32 *)((char *)(arg0) + 0x850);
        temp_f0 = 0.0f;
        *(f32 *)((char *)(var_s0) + 0xB4) = temp_f0;
        *(f32 *)((char *)(var_s0) + 0xBC) = temp_f0;
        *(f32 *)((char *)(var_s0) + 0xB8) = 100.0f;
        var_s0 += 0xB4;
        *(s32 *)((char *)(arg0) + 0x840) = func_00000000(0, *(s32 *)((char *)&D_00000000 + 0), arg0);
        func_00000000();
        *(s32 *)((char *)(arg0) + 0x800) = arg4;
        func_00000000(arg0, *(s32 *)((char *)(arg0) + 0x840));
        var_s0 = *(s32 *)((char *)(arg0) + 0x840);
        sp58 = 0;
        var_s0 += 0x18;
        *(s32 *)var_s0 &= ~8;
        /* target 0xAC34-0xAC98: each global access is its own symbol (fresh lui per
         * site, reload between the |= and &= statements) -- per-site blank aliases
         * bust the m2c &D_00000000 base GCSE that had cached one addiu base reg. */
        *(s32 *)&bu_90cc_g8_st = *(s32 *)&bu_90cc_g8_ld | 8;
        *(s32 *)((char *)&bu_90cc_gcfg_st + 4) = *(s32 *)((char *)&bu_90cc_gcfg_ld + 4) | 0x20000;
        *(s32 *)((char *)&bu_90cc_gcfg_b_st + 4) = *(s32 *)((char *)&bu_90cc_gcfg_b_ld + 4) & 0xFFF7FFFF & ~2;
        bu_90cc_gown = (s32) arg0;
        while (sp58 != 0x23) {
            var_a2 = sp58;
            if (sp58 >= 0x28U) {
                func_00000000(&D_00008868, sp58); /* 2-arg call: a2 holds var_a2 only by colouring (target: one or a2,a1 in the bnez delay) */
            }
            *(s32 *)((char *)&bu_90cc_gtbl + var_a2 * 4) = 0;
            sp58 += 1;
        }
        *(s32 *)((char *)&bu_90cc_gpair + 4) = 0x2A003;
        *(s32 *)&bu_90cc_gpair = 0;
        *(s32 *)((char *)(arg0) + 0x8F4) = func_00000000(&bu_90cc_gdev, 1, 0);
        *(s32 *)((char *)(arg0) + 0x8F8) = func_00000000(&bu_90cc_gdev, 2, 0);
        *(s32 *)((char *)(arg0) + 0x8FC) = func_00000000(&bu_90cc_gdev, 3, 0);
        if (sp174 != 0) {
            temp_v0_5 = (char *)bu_90cc_fz2; /* target lui v1 / addiu v0,v1,0 (blank) then lw 0(v0): fn-typed base */
            if (*(s32 *)temp_v0_5 != 0) {
                var_v1 = *(s32 *)&bu_90cc_d00_b;
                do {
                    if (arg2 == var_v1) {
                        /* (temp_v0_5->unk4 * 4)->unk160 store: scaled-index table write */
                        *(s32 *)(bu_90cc_d148_01 + 24 + *(s32 *)((char *)temp_v0_5 + 0x4) * 4) = (s32)(temp_v0_5 + 8);
                    }
                    var_v1 = *(s32 *)((char *)(temp_v0_5) + 0x18);
                    temp_v0_5 += 0x18;
                } while (var_v1 != 0);
            }
        } else {
            temp_v0_5 = bu_90cc_d148_02 + 24;
            do {
                temp_v0_5 += 4;
                *(s32 *)((char *)(temp_v0_5) - 0x4) = 0;
            } while (temp_v0_5 != bu_90cc_d148_03 + 36);
        }
        if (*(s32 *)((char *)(arg0) + 0xA58) & 8) {
            func_00000000(arg0);
        } else {
            func_00000000(arg0);
        }
        *(s16 *)((char *)(arg0) + 0x904) = (s16) arg3;
        func_00000000(arg2, arg3, *(s32 *)((char *)(arg0) + 0x8B8), *(s32 *)((char *)(arg0) + 0x840), arg0);
        func_00000000(arg0, *(s32 *)((char *)(arg0) + 0xA58) & 1);
        if (*(s32 *)((char *)(arg0) + 0xA58) & 3) {
            func_00000000(&D_00008880, 0);
            temp_v0_5 = func_00000000(0, &D_00008888);
            *(s32 *)((char *)(arg0) + 0x870) = temp_v0_5;
            fq = (s32 *)temp_v0_5;
            if (1) { fq += 6; }
            *fq &= ~4;
            func_00000000(arg0, *(s32 *)((char *)(arg0) + 0x870));
            var_s1 = *(s32 *)((char *)(arg0) + 0x870);
            func_00000000(var_s1 + 0x10, sp164); if (1) {}
            if (*(s32 *)((char *)(sp164) + 0x14) != 0) {
                *(s32 *)((char *)(sp164) + 0x4) = 1;
            }
            *(s32 *)((char *)(sp164) + 0x14) = var_s1;
            if (*(s32 *)((char *)(arg0) + 0xA58) & 1) {
                var_s1 = *(s32 *)((char *)(arg0) + 0x870);
                func_00000000(var_s1 + 0x10, sp160);
                if (*(s32 *)((char *)(sp160) + 0x14) != 0) {
                    *(s32 *)((char *)(sp160) + 0x4) = 1;
                }
                *(s32 *)((char *)(sp160) + 0x14) = var_s1;
            }
            var_s1 = *(s32 *)((char *)(arg0) + 0x870);
            func_00000000(var_s1 + 0x10, sp15C); if (1) {}
            if (*(s32 *)((char *)(sp15C) + 0x14) != 0) {
                *(s32 *)((char *)(sp15C) + 0x4) = 1;
            }
            *(s32 *)((char *)(sp15C) + 0x14) = var_s1;
            var_s1 = *(s32 *)((char *)(arg0) + 0x870);
            func_00000000(var_s1 + 0x10, sp158); if (1) {}
            if (*(s32 *)((char *)(sp158) + 0x14) != 0) {
                *(s32 *)((char *)(sp158) + 0x4) = 1;
            }
            *(s32 *)((char *)(sp158) + 0x14) = var_s1;
            func_00000000(1);
            var_s0 = *(char **)((char *)(sp164) + 0x28);
            var_t1 = bu_90cc_d44_01.f28 + 0x70;
            ((Vfn_ff)*(s32 *)((char *)(var_s0) + 0x5C))(*(s16 *)((char *)(var_s0) + 0x58) + sp164, *(s32 *)((char *)(arg0) + 0x870), *(s32 *)((char *)(arg0) + 0x86C), arg0, 0x10, var_t1, *(f32 *)(bu_90cc_d8B4_01 + 4), 100.0f);
            if (*(s32 *)((char *)(arg0) + 0xA58) & 1) {
                var_s0 = *(char **)((char *)(sp160) + 0x28);
                ((Vfn_ff)*(s32 *)((char *)(var_s0) + 0x5C))(*(s16 *)((char *)(var_s0) + 0x58) + sp160, *(s32 *)((char *)(arg0) + 0x870), *(s32 *)((char *)(arg0) + 0x86C), arg0, 0xC, var_t1, *(f32 *)(bu_90cc_d8B4_02 + 8), 100.0f);
                func_00000000(sp164);
                func_00000000(sp164);
            }
            var_s0 = *(char **)((char *)(sp15C) + 0x28);
            ((Vfn_ff)*(s32 *)((char *)(var_s0) + 0x5C))(*(s16 *)((char *)(var_s0) + 0x58) + sp15C, *(s32 *)((char *)(arg0) + 0x870), *(s32 *)((char *)(arg0) + 0x86C), arg0, 8, bu_90cc_d08_01.f2C + 0x70, 32.0f, 74.0f);
            var_s0 = *(char **)((char *)(sp158) + 0x28);
            ((Vfn_ff)*(s32 *)((char *)(var_s0) + 0x5C))(*(s16 *)((char *)(var_s0) + 0x58) + sp158, *(s32 *)((char *)(arg0) + 0x870), *(s32 *)((char *)(arg0) + 0x86C), arg0, 8, bu_90cc_d44_02.f04 + 0x70, 32.0f, 74.0f);
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

        *(s32 *)((char *)(arg0) + 0x804) = func_00000000(0, arg0, bu_90cc_d44_10.f24);
        func_00000000(arg0);
    }
    return arg0;
}
#else
INCLUDE_ASM("asm/nonmatchings/bootup_uso", func_000090CC);
#endif

// func_0000B1B4 — STRUCTURAL PASS (0x2E8 / 186 insns, no episode).
// Sound/resource-bank lazy loader + global-table registrar + play
// dispatcher.
//
//   void func_0000B1B4(int cat, int idx, a2, a3) {
//     if (idx >= 0xA) idx = 9;               // clamp [0,9]
//     if (cat >= 9)   cat = 8;               // clamp [0,8]
//     if (cat == 7 || cat == 6) {            // banked categories
//       if (*(int*)(func_00000148) == 0) {   // not-yet-loaded guard
//         func_00000000(&func_000089FC+0x2C);              // a name/path
//         R *r = func_00000000(0x90);  if (!r) goto reg;    // alloc (retry)
//         S *s = func_00000000(0x60);
//         s->0x5C = &D_0;                                   // vtable
//         // optional sub-allocs keyed by sentinel addrs (-0x34/-0x58):
//         //   alloc 8 -> {0,0} ; alloc 4 -> {0}
//         func_00000000(s);  s2->0x5C = &D_0;
//         r->0x24 = 0x110;  r->0x5C = &func_000083D0+0x70;  // folded vtbl
//         void *buf = func_00000000(0x1000);
//         r->0x28 = 0x1100;  r->0x22 = (s16)0x20;  r->0x20 = (s16)0x40;
//         r->0x38 = 0x402C4B;  r->0x1C = buf;  r->0x34 = 0;  r->0x18 = 0;
//       reg:
//         if (*(int*)(func_00000148+0x14) >= 5) func_00000000(r);  // evict
//         int n = *(int*)(func_00000148+0x14);
//         *(int*)(func_00000148+0x14) = n + 1;
//         ((void**)func_00000148)[n] = r;                   // register
//       }
//     }
//     func_00000000(&func_00008A40+0x8, 0);
//     func_00000000(&func_00008A40+0x14, 0);
//     if (idx) { t = tableA[idx]; if (!func_00000000(t->0x0))
//                  func_00000000(D_x, t->0x0); }              // play by idx
//     t = tableB[cat]; if (!func_00000000(t->0x0))
//                  func_00000000(D_y, t->0x0);                 // play by cat
//     func_00000000();
//     func_00000000(D_z, a3, a2, &sp34);   func_00000000();    // finalize
//   }
//
// Struct-typing reference:
//   cat/idx = category & sub indices (clamped); a2/a3 forwarded to the
//     final play call. Resource R (0x90): 0x18 state, 0x1C buffer ptr,
//     0x20/0x22 s16 dims (0x40/0x20), 0x24=0x110 mode, 0x28=0x1100,
//     0x34 flag, 0x38=0x402C4B id, 0x5C vtable(&func_000083D0+0x70).
//   func_00000148 = WRITABLE global resource table: [0]=loaded-root,
//     [0x14]=entry count, [n] = registered R ptrs. tableA/tableB =
//     other global index->entry arrays.
//   Folded refs (multi-class fold family — writable globals + tables +
//   vtables + name data; see
//   docs/N64_FORENSICS.md#bootup-uso-fp-literal-pool-folded-into-func-0000098C):
//     func_00000148 (writable table), func_000089FC+0x2C /
//     func_00008A40+0x8/+0x14 (name/string data), func_000083D0+0x70
//     (vtable).
// Caps (DEFERRED): 186-insn lazy-init + table-register + multi-dispatch.
//   2026-05-31 partial completion 55.9->67.1%: added the two sentinel-keyed
//   sub-allocs (alloc(8)->s+0x34={0,0} guarded by s==-52; alloc(4)->s+0x58={0}
//   guarded by s==-88, dead-arm passthrough form) and fixed the clamps to
//   UNSIGNED (sltiu: (unsigned)idx>=0xA, (unsigned)cat>=9).
//   2026-06-23 decode pass (agent-t) 161->177 insns (target 186), sim
//   35.7->37.5%: (1) CORRECTNESS FIX — replaced all D_00000000+off named-table
//   refs with the REAL symbols the .s relocs name: func_00000148 (writable
//   table+flag+count), func_000089FC+0x2C (name data), func_000083D0+0x70
//   (vtable), func_00008A40+0x8/+0x14 (string data). Build reloc set now
//   matches target's named relocs. (2) Reconstructed the nested ||-allocator-
//   fallback cascade (m2c shape): r=alloc(0x90); r||(r=alloc) ; s=r||(s=alloc
//   0x60) ; the s+0x34 / s+0x58 sentinel sub-allocs. (3) 5th stack arg a4 +
//   a2/a4 home copies. RESIDUAL (37.5->100), genuine IDO -O2 caps:
//   * CSE-COLLAPSE: target re-materializes &func_00000148 at 5 sites (5x HI16);
//     IDO -O2 here CSEs ours into 1 reg held across calls (1x HI16). 4 missing
//     re-lui's. C-level CSE defeat (volatile addr) not faithful.
//   * ARG-HOME FRAME: target -0x80 homes cat/idx to BOTH arg slots (0x80/0x84)
//     AND local copies (0x74/0x78), reloading memory for the 7/6 test while
//     keeping arg-home for the end dispatch; a4 double-stored 0x40/0x44, a2
//     0x7C. Ours -0x50, fewer homes — memory-vs-register alloc divergence.
//   * REG NUMBERING: target threads r in a2 / s in a3 where ours uses a1/a2.
//   Permuter unlikely to bridge (structural, not small coloring residual).
//   2026-07-31 (agent-g) 93.1->96.1: (1) FRAME CRACKED via decl order — o[19]
//   declared FIRST claims the TOP block 0x34-0x7F, named scalars ghost-descend
//   below (0x30..0x1C) with the CSE temp at 0x18; t/e MERGED (both color $v1,
//   disjoint regions) to hit the target's 6-scalar count — frame -0x88->-0x80,
//   every sp offset now exact. (2) Sentinel polarity was a DECODE FIX: target
//   allocs on s2==-0x34 fallthrough, so the ||-form is `s2 != -0x34 || alloc`
//   (bne emitted, not beq). (3) Tail tables CSE the element ADDRESS:
//   e=(char*)&tbl[idx] + *(int*)e twice (full lui/addiu base, addr saved/
//   reloaded at 0x18) — not e=tbl[idx] (extra deref level). (4) n's $a1 via
//   dead-web reuse of s2 (last colored $a1); plain named n colored $v0.
//   RESIDUAL (96.1->100, real bytes ~10 rows): post-call constant-group
//   MATERIALIZATION REVERSAL — target mats the store-group constants in
//   source order but emits the STORES reversed within the group (vt/0x110
//   pair + 0x1100/0x20/0x40 triple + ori-t2 adjacency); ours always couples
//   mat order = store order = source order, both spellings probed. as1/ugen
//   pair-order class (probe-immune). Rest of diff = split-name/base-0 reloc
//   naming only (NM-only externs, masked class).
//   Name pre-checked. Build via INCLUDE_ASM; ROM byte-identical (NM diff only).
/* EXACT via an IDO 7.1 NCC (C++) donor: src/bootup_uso/bootup_uso_ncc_B1B4.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder (agent-c run 117). The
 * last residual (counter in v0, target a1) was the registry: an inline
 * Reg::add() on the 5-slot table at func_00000148 doing `tbl[n++] = b`. */
void func_0000B1B4(unsigned int cat, unsigned int idx, int a2, int a3, int a4) {
    volatile unsigned int r = cat;
}

/* func_0000B49C: 33-insn (0x84) clamped-index dispatcher. EXACT 2026-07-15.
 *
 *   j = clamp(idx, 9); v = table1[j];
 *   if (gl_func(v, j) == 0) gl_func(&table1, v);
 *   gl_func(0, (char*)func_00008A40 + 0x18, table2[j], 0);   // dispatch
 *
 * The dispatched function ptr is `func_00008A40 + 0x18` — a code address
 * inside an existing function (alt-entry); gl_ref_00008A58 folds the +0x18.
 *
 * CROSSED 2026-07-15 (was 99.94% "frame -40 vs -32 spill-allocator cap").
 * The +8 frame was the dead-scalar-home block for the named locals (j/elem):
 * ANY named local scalar (int or pointer, register or not, any scope depth)
 * costs a 4-byte home rounded to 8 — the target has ZERO in-frame homes; its
 * two spill slots 0x18/0x1C are pad-region CSE temps. Levers that crossed it:
 *   1. `j` as an EXTRA (unpassed) PARAM — param homes live in the CALLER's
 *      arg area, not the frame; `j = idx` on the incoming-$a1 param emits
 *      exactly the target's `or $a1,$a0` delay-slot copy with no home store.
 *   2. NO named elem — spell `table[j]` twice; uopt CSEs the ADDRESS into a
 *      pad temp (sw $v1,0x18) exactly like the target.
 *   3. Distinct base-0 alias D_00000000_b49c for the 3rd table so it shares
 *      only the `sll j,2` temp (sw $v0,0x1C reused via $t7) instead of
 *      full-address CSE — matches the target's separate lui for table2.
 * (A 3rd `int *elem` param kills the frame but leaks `sw $a2,0x28(sp)` —
 * K&R param home-store; dead-redefinition doesn't DCE it. Named-local forms
 * all stay -40: local/register/inner-scope/named-off variants probed.) */
extern char gl_ref_00008A58;
extern int D_00000000_b49c[];  /* base-0 alias: 3rd table's separate lui (no CSE with 1st table's addr temp) */
void func_0000B49C(int idx, int j) {
    j = idx;
    if (j >= 10) j = 9;
    if (gl_func_00000000(((int*)&D_00000000)[j], j) == 0) {
        gl_func_00000000(&D_00000000, ((int*)&D_00000000)[j]);
    }
    gl_func_00000000(0, &gl_ref_00008A58, D_00000000_b49c[j], 0);  /* func_00008A40+0x18 */
}

/* func_0000B520 - verified structural decode (0x23C, 143 insns,
 * FP directional-nudge / input-driven position update).
 *   void func_0000B520(St *a1) {
 *       if (!(a1->0xA58 & 0x100)) return;          // enable gate
 *       blk = a1->0x800;
 *       f32 mag = 0.0f;
 *       if (blk->0x10 & 0x4) mag += 10.0f;         // 0x41200000
 *       if (blk->0x10 & 0x8) mag -= 10.0f;
 *       Vec3f off = { a1->0x3C8[0] * mag,           // sp+0x50
 *                     a1->0x3C8[1] * mag,
 *                     a1->0x3C8[2] * mag };
 *       // working copies sp+0x50 -> sp+0x5C -> sp+0x74
 *       a1->0x318 += off.x;                         // position Vec3
 *       a1->0x31C += off.y;
 *       a1->0x320 += off.z;
 *       ... (re-reads a1->0xA58, applies further updates) ...
 *   }
 * Struct-typing reference: a1->0xA58 (2648) u32 flags - bit 8
 * (0x100) enables this update. a1->0x800 (2048) -> input/state
 * block whose ->0x10 (16) carries direction bits 0x4 (forward/+)
 * and 0x8 (back/-), each contributing +-10.0 to the step
 * magnitude. a1->0x3C8 (968) f32[3] = the unit direction vector;
 * a1->0x318 (792) f32[3] = the accumulated position Vec3 advanced
 * by dir*mag. Likely the player/camera lateral or strafe nudge
 * (snowboard movement). Caps <80: FP-heavy mul.s/add.s/sub.s
 * Vec3 math + beql branch-likely on the input bits + a1->0x800
 * deref chain + Vec3 stack-copy shuffles. Full body INCLUDE_ASM-
 * preserved (.s = source of truth). INCLUDE_ASM (no episode;
 * tautology-trap rule). */
#ifndef FW
#define FW(p, o) (*(int *)((char *)(p) + (o)))
#endif
/* EXACT 2026-10-17 (agent-c): the f0/f2 swap goes away when the step
 * magnitude reuses `t` (the 0xA58-bit-14 target value): one variable for both
 * webs, as in the target (s and t both f2). `if (1) {}` before the v50 stores
 * splits the block so the v5C copy base reuses v0 after q. The pool reads are
 * pinned scalars (bu_b520_k48/k44/kc = 0x48/0x44/0xC in
 * undefined_syms_auto.txt; target func_00008124 + K / func_000008B4 + 0xC).
 * PASS-2 2026-10-16 (agent-c, 36.90 -> 99.34): m2c had the Vec3 copies as
 * int structs with (f32)/(s32) casts (4 cvt + 4 trunc too many) and a K&R call
 * carrying stale f12/f14. 143/143 words; 19 differ: the 0.0 constant / s
 * swap f0<->f2, the v5C copy base v1 (target v0), and the four pool addends
 * (k48/k44/kc need pins for an exact). Same shape as func_0000D900: named
 * products, v50 -> v5C -> v74 struct copies, decl-order frame (an unused
 * Vec3 between v74 and v5C, pad[3] below v50). Also: q = arg0 + 0x3C8 assigned
 * before the if (target re-forms it on both join edges, addiu v0,a1,968); the
 * second flag test re-reads p->0x10 (volatile read; plain CSEs the lw); the
 * 0x48 limit read inline at both uses; the call is (arg0 + 0x318, arg0). */
extern f32 bu_b520_k48, bu_b520_k44;  /* rodata f32 (target func_00008124 + 0x48 / + 0x44) */
extern f64 bu_b520_kc;               /* rodata f64 (target func_000008B4 + 0xC) */
#define F520(p, o) (*(f32 *)((char *)(p) + (o)))
void func_0000B520(char *arg0) {
    Vec3 v74;
    Vec3 vB;
    Vec3 v5C;
    Vec3 v50;
    s32 pad[3];
    f32 s;
    f32 px;
    f32 py;
    f32 pz;
    f32 t;
    f32 d;
    f32 k;
    char *p;
    char *q;

    q = arg0 + 0x3C8;
    t = 0.0f;
    if (FW(arg0, 0xA58) & 0x100) {
        p = (char *)FW(arg0, 0x800);
        if (FW(p, 0x10) & 4) {
            t += 10.0f;
        }
        if (*(volatile int *)(p + 0x10) & 8) {
            t -= 10.0f;
        }
    }
    px = F520(q, 0) * t;
    py = F520(q, 4) * t;
    pz = F520(q, 8) * t;
    if (1) {}
    v50.x = px; v50.y = py; v50.z = pz;
    v5C = v50;
    v74 = v5C;
    F520(arg0, 0x318) = F520(arg0, 0x318) + v74.x;
    F520(arg0, 0x31C) = F520(arg0, 0x31C) + v74.y;
    F520(arg0, 0x320) = F520(arg0, 0x320) + v74.z;
    if ((FW(arg0, 0xA58) << 14) < 0) {
        p = (char *)FW(arg0, 0x908);
        t = F520(arg0, 0x768);
        if (p != NULL) {
            d = F520(p, 0xBC) - F520(arg0, 0xBC);
            if (bu_b520_k48 < d) {
                t += bu_b520_k44;
            } else if (d < -bu_b520_k48) {
                t -= bu_b520_k44;
            }
        }
        k = F520(arg0, 0xA78);
        F520(arg0, 0xA78) = (f64)k + (f64)(t - k) * bu_b520_kc;
    } else {
        F520(arg0, 0xA78) = F520(arg0, 0x768);
    }
    if (F520(arg0, 0xA78) < F520(arg0, 0x348)) {
        func_00000000(arg0 + 0x318, arg0);
        k = F520(arg0, 0xA78);
        F520(arg0, 0x318) = F520(arg0, 0x318) * k;
        F520(arg0, 0x31C) = F520(arg0, 0x31C) * k;
        F520(arg0, 0x320) = F520(arg0, 0x320) * k;
    }
    if (FW(arg0, 0x938) == 0) {
        k = 1.0f - F520(arg0, 0x6C0);
        F520(arg0, 0x318) = F520(arg0, 0x318) * k;
        F520(arg0, 0x320) = F520(arg0, 0x320) * k;
    }
}

// func_0000B75C — STRUCTURAL PASS (0x830 / 524 insns, no episode).
// Per-frame camera/transform interpolation update over a large scene-
// state struct (compute-heavy: f64 lerp/curve math, only 11 dispatcher
// calls; ~70 accesses to the state struct via a3 = a0).
//
//   void func_0000B75C(State *st) {           // st kept in a3
//     // local default Vec @ sp+0x6C..0x74 = {0.0, 1.0f, 0.0}
//     float t = *(float*)st->0x800;            // a normalized phase/time
//     double k0 = DBL(func_000008B4+0x14), k1 = DBL(func_000008B4+0x1C);
//     // curve eval: scale t (as double) by the folded f64 coefficient
//     //   bank, producing eased blend factors;
//     if (st->0x960 != 0x64) { ... uses st->0x974 ... }   // state guard
//     // main body: many double mul/add/sub chains driven by the
//     //   func_000008B4 / func_000008D4 / func_000008F4 coefficient
//     //   table (each symbol = 4 f64 at +0x4/+0xC/+0x14/+0x1C) plus
//     //   func_00008124+0x4C — interpolates a transform/orientation
//     //   from st fields and writes the result Vec to st->0x300.. .
//     st->0x304 = (float)result_y;             // epilogue writes
//     st->0x308 = (float)result_z;             //   the eased transform
//   }
//
// Struct-typing reference:
//   st(a3=a0): 0x300/0x304/0x308 output transform Vec3 (eased result);
//     0x800 -> f32 phase/time input; 0x960 a state/mode word
//     (compared to 0x64=100); 0x974 a sub-field used when 0x960!=0x64;
//     many other 0x300+ fields read as f32 sources.
//   Folded f64 coefficient bank (literal-pool fold family — and this
//   function shows the pool is a CONTIGUOUS 0x20-stride double table,
//   not scattered singletons: func_000008B4 / func_000008D4 /
//   func_000008F4 each carry 4 f64 at +0x4/+0xC/+0x14/+0x1C, plus
//   func_00008124+0x4C; see
//   docs/N64_FORENSICS.md#bootup-uso-fp-literal-pool-folded-into-func-0000098C).
// Caps (DEFERRED): 524-insn f64 interpolation w/ folded coefficient
//   table — byte-match blocked by deferred pool symbolization.
//   Real-C STRUCTURAL body below — per-frame camera/transform
//   interpolation skeleton. Name pre-checked: no extern reuse.
#ifdef NON_MATCHING
/* PASS-2 2026-06-10 (big-swing): FULL m2c graft (524 insns, table-free,
 * 7 f64 ops -- mild f64, tempered expectations).
 * PASS-3 2026-07-11 (agent-g): st->0xA6C is a halfword field (target 1x lh
 * + 3x sh), was s32 -> retyped s16 (read+3 writes). Fuzzy 66.37->67.21%.
 * PASS-4 2026-07-30 (agent-g): 67.21->70.32.
 *   (1) Pool symbolization per expected relocs -- DISTINCT base-0 undefined
 *       externs (D_000008C8/D0/D8/E8/F8, D_00000900/908 f64; D_00008170 call
 *       arg = func_00008124+0x4C) so each site emits the target's per-use
 *       lui-at + %lo ldc1 (func_+off spelling CSEs the addiu-base into a
 *       long-lived v0/v1 web: 65.5, WORSE than the old absolute loads).
 *   (2) {0,1,0} default vec respelled as f32 sp6C[3] (array keeps the 3
 *       prologue swc1s + sp reloads; scalars const-propped to inline mtc1).
 *   NEGATIVE probes: if(0){f(&arg0)} address-escape (target's a3+arg-home-
 *   spill-around-calls shape) memory-homes arg0 per-BB -> 0x998/47.9;
 *   self-assign and staged-copy multi-defs both VN/copy-prop away (s0 stays).
 *   Residual: arg0 s0-promotion vs target a3+home (11 calls), frame 256 vs
 *   120 (named m2c FP-temp ghost homes), as1 const-hoist order (20.0f/30.0f),
 *   store-forward vs reload at 0x974.
 * PASS-5 2026-10-16 (agent-c): 69.24 -> 88.15. CALL DECODE: the nine
 *   float-carrying calls are (ptr, ptr, f32, self) -- target a0/a1 = arg0+K,
 *   a2 = mfc1 of ONE single float, a3 = arg0 -- through the prototyped blank
 *   alias bu_b75c_mvf. m2c had invented `var_f12_2, 0` / `0, temp_f14` lead
 *   args and K&R double promotion (12 extra cvt.d.s + 4 sdc1). Passing self
 *   as the 4th arg is also why the target keeps arg0 in a3 (or a3,a0 early,
 *   sw/lw a3,120(sp) around every call; IDO_CODEGEN #hidden-arg-register-
 *   base-a88). The 0x808 call is just (arg0+0x808, arg0->0xB4) and the
 *   D_00008170 call is (st->0x800, D_00008170, 1, arg0). st800->0x0 is an
 *   f32 in the A70 accumulate (target lwc1 + cvt.d.s, not lw + cvt.d.w).
 *   Residual: 20.0f/30.0f hoist, 0x974 store+reload, `sp60 *= 2.0f`
 *   (target mul.s, build add.s), sp1C/sp6C[2] reload placement, t-ring. */
extern void bu_b75c_mvf(char *, char *, f32, char *);  /* (dst, src, single float in a2, self in a3); NM-only blank alias */
extern f64 D_000008C8, D_000008D0, D_000008D8, D_000008E8, D_000008F8, D_00000900, D_00000908;
extern char D_00008170[];
void func_0000B75C(char *arg0) {
    f32 sp6C[3];
    f32 sp60;
    f32 sp5C;
    f32 sp58;
    f32 sp54;
    char *sp28;
    char *sp24;
    f32 sp1C;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 temp_f0_5;
    f32 temp_f0_6;
    f32 temp_f0_7;
    f32 temp_f12;
    f32 temp_f12_2;
    f32 temp_f12_3;
    f32 temp_f14;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f2_3;
    f32 temp_f2_4;
    f32 temp_f2_5;
    f32 temp_f2_6;
    f32 temp_f2_7;
    f32 temp_f2_8;
    f32 temp_f2_9;
    f32 var_f0;
    f32 var_f0_2;
    f32 var_f0_3;
    f32 var_f0_4;
    f32 var_f0_5;
    f32 var_f12;
    f32 var_f12_2;
    f32 var_f2;
    f32 var_f2_2;
    f64 temp_f0;
    s16 temp_v0_2;
    s32 temp_v0;
    s32 var_v0;
    s32 var_v0_2;
    char *temp_a0;
    char *temp_a1;
    char *var_a0;

    sp6C[0] = 0;
    sp6C[2] = 0;
    sp6C[1] = 1.0f;
    temp_f0 = (f64) *(f32 *)(*(s32 *)((char *)(arg0) + 0x800));
    var_f12 = (f32) (temp_f0 * D_000008C8);
    sp60 = (f32) (temp_f0 * D_000008D0);
    if (*(s32 *)((char *)(arg0) + 0x960) == 0x64) {
        var_f12 = 0;
        sp60 = 0;
    }
    temp_f0_2 = *(f32 *)((char *)(arg0) + 0x974);
    temp_f2 = *(f32 *)((char *)(arg0) + 0xA00);
    temp_v0 = *(s32 *)((char *)(arg0) + 0x990);
    *(f32 *)((char *)(arg0) + 0x974) = (f32) (temp_f0_2 - (temp_f0_2 * *(f32 *)((char *)(arg0) + 0x720) * temp_f2));
    *(f32 *)((char *)(arg0) + 0x974) = (f32) (*(f32 *)((char *)(arg0) + 0x974) + (var_f12 * temp_f2));
    if (temp_v0 != 0) {
        sp5C = 0;
        sp1C = *(f32 *)((char *)(arg0) + 0x3C4);
    } else {
        temp_f0_3 = *(f32 *)((char *)(arg0) + 0x3C4);
        sp1C = temp_f0_3;
        sp5C = -((*(f32 *)((char *)(arg0) + 0x3BC) * sp6C[0]) + (*(f32 *)((char *)(arg0) + 0x3C0) * sp6C[1]) + (temp_f0_3 * sp6C[2]));
    }
    temp_f0_4 = *(f32 *)((char *)(arg0) + 0x978);
    temp_f2_2 = *(f32 *)((char *)(arg0) + 0x30C);
    temp_f12 = *(f32 *)((char *)(arg0) + 0x310);
    temp_f14 = *(f32 *)((char *)(arg0) + 0x314);
    *(f32 *)((char *)(arg0) + 0x978) = (f32) ((f64) temp_f0_4 + ((f64) (sp5C - temp_f0_4) * D_000008D8));
    sp58 = (*(f32 *)((char *)(arg0) + 0x3C8) * temp_f2_2) + (*(f32 *)((char *)(arg0) + 0x3CC) * temp_f12) + (*(f32 *)((char *)(arg0) + 0x3D0) * temp_f14);
    sp54 = (*(f32 *)((char *)(arg0) + 0x3BC) * temp_f2_2) + (*(f32 *)((char *)(arg0) + 0x3C0) * temp_f12) + (sp1C * temp_f14);
    if (temp_v0 != 0) {
        sp60 = (f32) ((f64) sp60 * 0.5);
    }
    var_f12_2 = -1.0f;
    *(f32 *)((char *)(arg0) + 0x970) = (f32) (*(f32 *)((char *)(arg0) + 0x974) + *(f32 *)((char *)(arg0) + 0x978));
    if (*(s32 *)((char *)(arg0) + 0xA58) & 0x80) {
        *(f32 *)((char *)(arg0) + 0x970) = (f32) -*(f32 *)((char *)(arg0) + 0x970);
    }
    temp_f0_5 = *(f32 *)((char *)(arg0) + 0x970);
    if (temp_f0_5 < -1.0f) {
        var_f2 = -1.0f;
    } else if (temp_f0_5 > 1.0f) {
        var_f2 = 1.0f;
    } else {
        var_f2 = temp_f0_5;
    }
    *(f32 *)((char *)(arg0) + 0x970) = var_f2;
    if ((*(s32 *)((char *)(arg0) + 0x9A8) & 1) || (*(s32 *)((char *)(arg0) + 0xA50) == 0)) {
        *(f32 *)((char *)(arg0) + 0xA04) = (f32) *(f32 *)((char *)(arg0) + 0x3CC);
        *(f32 *)((char *)(arg0) + 0xA08) = (f32) -*(f32 *)((char *)(arg0) + 0x3C0);
        *(f32 *)((char *)(arg0) + 0xA0C) = (f32) *(f32 *)((char *)(arg0) + 0x3B4);
    } else {
        temp_f0_6 = *(f32 *)((char *)(arg0) + 0x9D8);
        temp_f2_3 = *(f32 *)((char *)(arg0) + 0x9DC);
        var_f12_2 = *(f32 *)((char *)(arg0) + 0x9E0);
        *(f32 *)((char *)(arg0) + 0xA04) = (f32) ((*(f32 *)((char *)(arg0) + 0x3C8) * temp_f0_6) + (*(f32 *)((char *)(arg0) + 0x3CC) * temp_f2_3) + (*(f32 *)((char *)(arg0) + 0x3D0) * var_f12_2));
        *(f32 *)((char *)(arg0) + 0xA08) = (f32) -((*(f32 *)((char *)(arg0) + 0x3BC) * temp_f0_6) + (*(f32 *)((char *)(arg0) + 0x3C0) * temp_f2_3) + (*(f32 *)((char *)(arg0) + 0x3C4) * var_f12_2));
        *(f32 *)((char *)(arg0) + 0xA0C) = (f32) ((*(f32 *)((char *)(arg0) + 0x3B0) * temp_f0_6) + (*(f32 *)((char *)(arg0) + 0x3B4) * temp_f2_3) + (*(f32 *)((char *)(arg0) + 0x3B8) * var_f12_2));
    }
    if (*(s32 *)((char *)(arg0) + 0x938) == 0) {
        *(s32 *)((char *)(arg0) + 0xA5C) = 0;
        *(f32 *)((char *)(arg0) + 0x2FC) = 0;
        *(f32 *)((char *)(arg0) + 0x300) = 0;
        *(f32 *)((char *)(arg0) + 0x304) = 0;
        *(f32 *)((char *)(arg0) + 0x308) = 1.0f;
        if ((*(s32 *)((char *)(*(s32 *)((char *)(arg0) + 0x800)) + 0x10) & 0x200) && (*(s32 *)((char *)(arg0) + 0x9A8) & 1)) {
            temp_v0_2 = *(s16 *)((char *)(arg0) + 0xA6C);
            *(s16 *)((char *)(arg0) + 0xA6C) = (s16) (temp_v0_2 + 1);
            if (temp_v0_2 >= 0xB) {
                temp_f12_2 = D_000008E0;
                temp_f0_7 = (f32) ((f64) *(f32 *)((char *)(arg0) + 0xA70) + ((f64) *(f32 *)(*(s32 *)((char *)(arg0) + 0x800)) * D_000008E8));
                if (temp_f0_7 < temp_f12_2) {
                    var_f2_2 = temp_f12_2;
                } else {
                    temp_f12_3 = D_000008F0;
                    if (temp_f12_3 < temp_f0_7) {
                        var_f2_2 = temp_f12_3;
                    } else {
                        var_f2_2 = temp_f0_7;
                    }
                }
                *(f32 *)((char *)(arg0) + 0xA70) = var_f2_2;
            }
        } else {
            *(s16 *)((char *)(arg0) + 0xA6C) = 0;
            if (*(s32 *)((char *)(arg0) + 0x9A8) & 1) {
                bu_b75c_mvf(arg0 + 0xCC, arg0 + 0x3B0, -*(f32 *)(*(s32 *)((char *)(arg0) + 0x800)) * *(f32 *)((char *)(arg0) + 0x678), arg0);
            } else {
                temp_a0 = arg0 + 0xCC;
                temp_a1 = arg0 + 0x3C8;
                sp28 = temp_a1;
                sp24 = temp_a0;
                bu_b75c_mvf(temp_a0, temp_a1, (f32) ((f64) ((*(f32 *)((char *)(arg0) + 0x3BC) * sp6C[0]) + (*(f32 *)((char *)(arg0) + 0x3C0) * sp6C[1]) + (*(f32 *)((char *)(arg0) + 0x3C4) * sp6C[2])) * D_000008F8), arg0);
                if (*(s32 *)((char *)(arg0) + 0xA18) != 0) {
                    sp24 = temp_a0;
                    bu_b75c_mvf(temp_a0, temp_a1, *(f32 *)(*(s32 *)((char *)(arg0) + 0x800)) * *(f32 *)((char *)(arg0) + 0x690), arg0);
                    bu_b75c_mvf(temp_a0, arg0 + 0x3BC, *(f32 *)((char *)(*(s32 *)((char *)(arg0) + 0x800)) + 0x4) * *(f32 *)((char *)(arg0) + 0x660), arg0);
                    bu_b75c_mvf(temp_a0, arg0 + 0x3B0, -*(f32 *)(*(s32 *)((char *)(arg0) + 0x800)) * *(f32 *)((char *)(arg0) + 0x678), arg0);
                }
            }
        }
        var_v0 = *(s32 *)((char *)(arg0) + 0x9A8) & 1;
        if (var_v0 != 0) {
            if ((f64) *(f32 *)((char *)(arg0) + 0x3B4) < D_00000900) {
                *(f32 *)((char *)(arg0) + 0x318) = 0;
            }
            bu_b75c_mvf(arg0 + 0xCC, arg0 + 0x3B0, -*(f32 *)((char *)(arg0) + 0xA70), arg0);
            var_v0_2 = *(s32 *)((char *)(arg0) + 0x9A8);
            goto block_67;
        }
    } else {
        temp_f2_4 = *(f32 *)((char *)(arg0) + 0x970);
        *(s16 *)((char *)(arg0) + 0xA6C) = 0;
        *(f32 *)((char *)(arg0) + 0xA74) = 0;
        *(f32 *)((char *)(arg0) + 0xA70) = 0;
        if (temp_f2_4 < 0.0f) {
            var_f0 = -temp_f2_4;
        } else {
            var_f0 = temp_f2_4;
        }
        *(s32 *)((char *)(arg0) + 0xA5C) = (s32) (20.0f + (30.0f * var_f0));
        if (*(s32 *)((char *)(arg0) + 0x9A8) & 1) {
            func_00000000(arg0 + 0x808, *(s32 *)((char *)(arg0) + 0xB4));
            if (*(s32 *)((char *)(arg0) + 0x960) == 0) {
                temp_f2_5 = *(f32 *)((char *)(arg0) + 0x3C8);
                if (temp_f2_5 < 0.0f) {
                    var_f0_2 = -temp_f2_5;
                } else {
                    var_f0_2 = temp_f2_5;
                }
                temp_f2_6 = *(f32 *)((char *)(arg0) + 0x318);
                *(f32 *)((char *)(arg0) + 0x318) = (f32) (temp_f2_6 + (temp_f2_6 * *(f32 *)((char *)(arg0) + 0x558) * var_f0_2));
            }
            if ((f64) *(f32 *)((char *)(arg0) + 0x3B4) < D_00000908) {
                sp60 *= 2.0f;
            }
            var_a0 = arg0 + 0xCC;
        } else {
            if ((sp58 < *(f32 *)((char *)(arg0) + 0x648)) && (((sp54 > 0.0f) && (sp60 < 0.0f)) || ((sp54 < 0.0f) && (sp60 > 0.0f)))) {
                *(s32 *)((char *)(arg0) + 0xA5C) = (s32) (*(s32 *)((char *)(arg0) + 0xA5C) + 0x14);
                sp60 = 0;
                func_00000000(*(s32 *)((char *)(arg0) + 0x800), D_00008170, 1, arg0);
            }
            temp_f2_7 = *(f32 *)((char *)(arg0) + 0xA08);
            if (temp_f2_7 < 0.0f) {
                var_f0_3 = -temp_f2_7;
            } else {
                var_f0_3 = temp_f2_7;
            }
            if (*(f32 *)((char *)(arg0) + 0x618) < var_f0_3) {
                bu_b75c_mvf(arg0 + 0xCC, arg0 + 0x3C8, temp_f2_7 * *(f32 *)((char *)(arg0) + 0x600), arg0);
            }
            temp_f2_8 = *(f32 *)((char *)(arg0) + 0xA04);
            var_a0 = arg0 + 0xCC;
            if (temp_f2_8 < 0.0f) {
                var_f0_4 = -temp_f2_8;
            } else {
                var_f0_4 = temp_f2_8;
            }
            if (*(f32 *)((char *)(arg0) + 0x630) < var_f0_4) {
                sp24 = var_a0;
                bu_b75c_mvf(var_a0, arg0 + 0x3BC, temp_f2_8 * *(f32 *)((char *)(arg0) + 0x600), arg0);
            }
        }
        bu_b75c_mvf(var_a0, arg0 + 0x3B0, sp60, arg0);
        var_v0_2 = *(s32 *)((char *)(arg0) + 0x9A8);
block_67:
        var_v0 = var_v0_2 & 1;
    }
    if (var_v0 != 0) {
        var_f0_5 = D_00000910;
    } else {
        var_f0_5 = *(f32 *)((char *)(arg0) + 0x7F8);
    }
    temp_f2_9 = *(f32 *)((char *)(arg0) + 0x308);
    *(f32 *)((char *)(arg0) + 0x2FC) = (f32) (*(f32 *)((char *)(arg0) + 0x2FC) * var_f0_5);
    *(f32 *)((char *)(arg0) + 0x300) = (f32) (*(f32 *)((char *)(arg0) + 0x300) * var_f0_5);
    *(f32 *)((char *)(arg0) + 0x304) = (f32) (*(f32 *)((char *)(arg0) + 0x304) * var_f0_5);
    *(f32 *)((char *)(arg0) + 0x308) = (f32) ((f64) temp_f2_9 + ((f64) (1.0f - temp_f2_9) * (1.0 - (f64) var_f0_5)));
}
#else
INCLUDE_ASM("asm/nonmatchings/bootup_uso", func_0000B75C);
#endif

/* func_0000BF8C - verified structural decode (0x2A8, 170 insns,
 * large FP per-frame parameter selector/update).
 *   void func_0000BF8C(St *s0) {
 *       in = s0->0x800;
 *       s0->0x990 = ((f64)in->0x4 < 0.0) ? 1 : 0;  // sign flag
 *       int m = (in->0x10 & 0x400) != 0;            // mode flag
 *       s0->0x9CC = m;
 *       s0->0x9F8 = s0->0x490;
 *       if (m) {                                    // param set A
 *           s0->0x9FC = s0->0x4F0;
 *           s0->0x360 = s0->0x4D8;
 *           s0->0xA00 = s0->0x6F0 * s0->0x508;
 *       } else {                                    // param set B
 *           s0->0x9FC = s0->0x4C0;
 *           s0->0x360 = s0->0x4A8;
 *           s0->0xA00 = s0->0x708 * s0->0x508;
 *       }
 *       if (s0->0x990) {                            // signed branch
 *           if (s0->0x9CC) { ... 0.5f (0x3F000000) ... }
 *           ... uses func_000008F4+0x20 / func_00008A40 ...
 *       }
 *       ... s0->0x9A2 (s16) further updates ...
 *   }
 * Struct-typing reference: s0->0x800 (2048) -> input/state block
 * (in->0x4 f32 signed value -> sign flag, in->0x10 u32 bit 10 /
 * 0x400 -> mode). Flags: s0->0x990 (2448) s32 sign latch, s0->
 * 0x9CC (2508) s32 mode latch. Param sets selected by mode: set A
 * {0x6F0 (1776), 0x4F0 (1264), 0x4D8 (1240)} vs set B {0x708
 * (1800), 0x4C0 (1216), 0x4A8 (1192)}, with shared scale s0->0x508
 * (1288); outputs s0->0x9FC (2556) / s0->0x360 (864) / s0->0xA00
 * (2560); s0->0x9F8 (2552) = s0->0x490 (1168). s0->0x9A2 (2466)
 * s16, 0.5 const for the signed sub-path. Likely a carve/lean
 * direction (left vs right) parameter swap per frame.
 *
 * RECONSTRUCTED 2026-06-23: faithful full-body decode now uses the REAL
 * resolved reloc symbols (func_000008F4+0x20 f32 / +0x24 f64 const-pool;
 * func_00008A40+0x20 / +0x2C dispatch-target addresses passed to the
 * allocator func_00000000) in place of the old &D_00000000 placeholders.
 * Build emits EXACTLY 186 instructions = target's 186 (same opcode
 * multiset, all relocs correct: 2x func_000008F4 HI/LO, 2x func_00008A40
 * HI/LO, 8x func_00000000 R_MIPS_26).
 *
 * RESIDUAL CAP (~35 mnemonic-position diffs, permuter-immune — floored at
 * score ~2015 over 7-thread x10min, never approached 0): three clusters,
 * all scheduler/coloring, none C-controllable:
 *   (1) sign-flag block: target uses bc1fl/beqzl BRANCH-LIKELY with the
 *       mtc1/lwc1 const-load hoisted into the delay slot; build emits the
 *       plain-branch + separate-slot shape.
 *   (2) switch default arm: target's 3rd compare is `beql` (likely) with
 *       lwc1 0x360 in delay + ldc1/add.d/cvt ordering; build differs only
 *       in as1 instruction scheduling.
 *   (3) RESOLVED 2026-07-30 (agent-g): the func_00008A40+0x2C tail ref is
 *       anchor-split to &func_00008A38+0x34 (same absolute 0x8A6C, distinct
 *       in-TU anchor — E68/CCE0 lever) which kills the base CSE-collapse:
 *       frame back to -32 (exact), spill gone. Reloc-name diff at that one
 *       site is the accepted fuzzy residual.
 * 2026-07-30 (agent-g) 86.9->88.3: also fixed commutative mul.s operand
 * order at 6 sites (source A*B emits B-first; write B*A) + volatile on the
 * per-arm *(0x508) read (defeats the cross-arm PRE hoist; target loads it
 * in each arm) + add.d order in default arm (pool + (f64)x). Switch compare
 * order (target tests 0x63 first, we emit 0x61 first) is NOT source-
 * steerable: case reorder = identical output; if-chain/hybrid = worse shape
 * (154 -> 189/171 diff lines). Remaining = t-reg-vs-v0/a1 web coloring +
 * FP ring rotation + branch-likely shaping coupled to the direct-form cap.
 * Logic + structure are byte-faithful; INCLUDE_ASM remains the build path
 * (no episode; not 0-diff).
 * RUN 108 (agent-c): switch -> nested-!= goto chain (0x63, 0x61, 0x62; default
 * innermost, chain tell); const-pool / dispatch addresses through DATA-symbol
 * views (lui at + lwc1 lo(at) instead of the function-symbol lui/addiu/lwc1);
 * the 0x990 flag as a direct boolean store; the 0x9CC flag/0x9F8 copy/test as
 * one comma condition; mid block 9FC=0, 360 +=, then the 1-x temp. Only
 * residual: the first if-arm's FP ring starts at f4 vs target f16. */
#ifdef NON_MATCHING
extern f32 D_BF8C_pool8F4[]; /* data views of the func_000008F4 const pool (NM only; pins = func_000008F4 / func_000008F4 + 0x24 if promoted) */
extern f64 D_BF8C_pool8F4_24;
extern char D_BF8C_8A40[]; /* data view of func_00008A40 (dispatch-target addresses; pin = func_00008A40 if promoted) */
void func_0000BF8C(char *arg0) {
    f32 temp_f0;
    f32 temp_f0_2;
    f32 var_f12;
    f32 var_f2;
    s16 temp_v0_2;
    s32 temp_t9;
    s32 temp_v0_3;
    s32 var_t6;
    char *temp_v0;

    var_f12 = 0.0f;
    temp_v0 = *(char **)((char *)arg0 + 0x800);
    *(s32 *)((char *)arg0 + 0x990) = (f64) (*(f32 *)((char *)temp_v0 + 0x4)) < -0.75;
    if ((*(s32 *)((char *)arg0 + 0x9CC) = (*(s32 *)((char *)temp_v0 + 0x10) & 0x400) != 0, *(f32 *)((char *)arg0 + 0x9F8) = *(f32 *)((char *)arg0 + 0x490), *(s32 *)((char *)arg0 + 0x9CC)) != 0) {
        *(f32 *)((char *)arg0 + 0x9FC) = *(f32 *)((char *)arg0 + 0x4F0);
        *(f32 *)((char *)arg0 + 0x360) = *(f32 *)((char *)arg0 + 0x4D8);
        *(f32 *)((char *)arg0 + 0xA00) = (f32) ((*(volatile f32 *)((char *)arg0 + 0x508)) * (*(f32 *)((char *)arg0 + 0x6F0)));
    } else {
        *(f32 *)((char *)arg0 + 0x9FC) = *(f32 *)((char *)arg0 + 0x4C0);
        *(f32 *)((char *)arg0 + 0x360) = *(f32 *)((char *)arg0 + 0x4A8);
        *(f32 *)((char *)arg0 + 0xA00) = (f32) ((*(volatile f32 *)((char *)arg0 + 0x508)) * (*(f32 *)((char *)arg0 + 0x708)));
    }
    if (*(s32 *)((char *)arg0 + 0x990) != 0) {
        if (*(s32 *)((char *)arg0 + 0x9CC) != 0) {
            var_f12 = D_BF8C_pool8F4[8];
        } else {
            var_f12 = 0.5f;
        }
        temp_f0 = *(f32 *)((char *)arg0 + 0x970);
        if (temp_f0 < 0.0f) {
            var_f2 = -temp_f0;
        } else {
            var_f2 = temp_f0;
        }
        *(f32 *)((char *)arg0 + 0x360) = (f32) ((*(f32 *)((char *)arg0 + 0x360)) + (var_f2 * var_f12));
    }
    temp_v0_2 = *(s16 *)((char *)arg0 + 0x9A2);
    if (temp_v0_2 != 0x63) {
        if (temp_v0_2 != 0x61) {
            if (temp_v0_2 != 0x62) {
        *(f32 *)((char *)arg0 + 0x360) = (f32) (D_BF8C_pool8F4_24 + (f64) (*(f32 *)((char *)arg0 + 0x360)));
        *(f32 *)((char *)arg0 + 0x9FC) = 0.0f;
        *(f32 *)((char *)arg0 + 0x9F8) = 0.0f;
            } else goto L62;
        } else goto Lend;
    } else goto L63;
    goto Lend;
L63:
        *(f32 *)((char *)arg0 + 0x360) = (f32) ((*(f32 *)((char *)arg0 + 0x448)) * (*(f32 *)((char *)arg0 + 0x360)));
        *(f32 *)((char *)arg0 + 0x9FC) = (f32) ((*(f32 *)((char *)arg0 + 0x430)) * (*(f32 *)((char *)arg0 + 0x9FC)));
    goto Lend;
L62:
        *(f32 *)((char *)arg0 + 0x360) = (f32) ((*(f32 *)((char *)arg0 + 0x478)) * (*(f32 *)((char *)arg0 + 0x360)));
        *(f32 *)((char *)arg0 + 0x9FC) = (f32) ((*(f32 *)((char *)arg0 + 0x460)) * (*(f32 *)((char *)arg0 + 0x9FC)));
Lend:
    temp_v0_3 = *(s32 *)((char *)arg0 + 0x960);
    *(f32 *)((char *)arg0 + 0x9FC) = (f32) ((*(f32 *)((char *)arg0 + 0x5D0)) * (*(f32 *)((char *)arg0 + 0x9FC)));
    *(f32 *)((char *)arg0 + 0x360) = (f32) ((*(f32 *)((char *)arg0 + 0x5E8)) * (*(f32 *)((char *)arg0 + 0x360)));
    if (temp_v0_3 != 0) {
        *(f32 *)((char *)arg0 + 0x9FC) = 0.0f;
        *(f32 *)((char *)arg0 + 0x360) = (f32) ((*(f32 *)((char *)arg0 + 0x360)) + ((f32) temp_v0_3 / 100.0f));
        temp_f0_2 = 1.0f - (*(f32 *)((char *)arg0 + 0x6D8));
        *(f32 *)((char *)arg0 + 0x318) = (f32) ((*(f32 *)((char *)arg0 + 0x318)) * temp_f0_2);
        *(f32 *)((char *)arg0 + 0x320) = (f32) ((*(f32 *)((char *)arg0 + 0x320)) * temp_f0_2);
    }
    func_00000000(arg0);
    func_00000000(arg0);
    func_00000000(D_BF8C_8A40 + 0x20);
    func_00000000(arg0);
    /* anchor-split (E68/CCE0 lever): same absolute addr 0x8A6C as
     * &func_00008A40+0x2C but distinct in-TU anchor kills the base
     * CSE-collapse (frame bloat cascade); reloc-name diff at this site
     * is the accepted fuzzy residual. */
    func_00000000(D_BF8C_8A40 + 0x2C);
    func_00000000(arg0);
    if ((*(s32 *)((char *)arg0 + 0x960) == 0x64) && ((*(f32 *)((char *)arg0 + 0x348)) < 30.0f)) {
        *(f32 *)((char *)arg0 + 0x318) = 0.0f;
        *(f32 *)((char *)arg0 + 0x320) = 0.0f;
    }
    func_00000000(arg0);
    func_00000000(arg0);
}
#else
INCLUDE_ASM("asm/nonmatchings/bootup_uso", func_0000BF8C);
#endif

// func_0000C234 — STRUCTURAL PASS (0x898 / 550 insns, no episode).
// Per-frame entity physics/state update over a large state struct
// (st = a0 = s0; ~152 field accesses, 16 dispatcher sub-calls).
//
//   void func_0000C234(State *st) {
//     if (st->0xA80) st->0xA80--;              // cooldown/timer decrement
//     Vec3 p = { st->0xB4, st->0xB8, st->0xBC };   // -> sp+0xB4 local
//     st->0x890 = p.x;  st->0x894 = p.y;       // snapshot prev position
//     // const 0x457A0000 = 4000.0f (a range/limit); 1.0f baseline;
//     // a1 = &st->0xDC (a sub-record), a3 = sp+0xCC scratch.
//     // main body: physics/animation integration driven by folded
//     //   constant tables —
//     //     func_00000940 + {0x4,0x8,0xC,0x10}  (4-word table),
//     //     func_00008A7C + {0x14,0x18,0x28,0x34} (a folded struct),
//     //     func_00008A40 + 0x38, func_000008F4 + {0x2C,0x30,0x34}
//     //   (same literal-pool fold family) — plus 16 func_00000000
//     //   sub-calls (sub-system updates) operating on st->0x300.. and
//     //   st->0xA00.. blocks.
//     st->0xA24 -= (delta_a + delta_b);        // decay/damp accumulate
//   }
//
// Struct-typing reference:
//   st(a0=s0): 0xA80 cooldown counter; 0xB4/0xB8/0xBC current pos Vec3;
//     0x890/0x894 prev-pos snapshot; 0xDC a sub-record (passed by ptr);
//     0x318 a source field; 0xA24 a decayed accumulator; many 0x300+/
//     0xA00+ fields as physics state.
//   Folded refs (literal-pool fold family — incl. another contiguous
//   strided table at func_00000940 +0x4/0x8/0xC/0x10; see
//   docs/N64_FORENSICS.md#bootup-uso-fp-literal-pool-folded-into-func-0000098C):
//     func_00000940 (4-word table), func_00008A7C (folded struct),
//     func_00008A40+0x38, func_000008F4+{0x2C,0x30,0x34} (f-pool).
// Caps (DEFERRED): 550-insn folded-table-driven physics integrator —
//   byte-match blocked by deferred pool symbolization. Real-C
//   STRUCTURAL body below — per-frame entity physics/state update
//   skeleton. Name pre-checked: no extern reuse.
#ifdef NON_MATCHING
/* PASS-2 2026-06-10 (big-swing): FULL m2c graft (550 insns, table-free,
 * 0 M2C_ERRORs).
 * PASS-3 2026-07-11 (agent-g): width decode fixes (fuzzy 57.95->63.93%).
 *   st->0x9A0/0x9A2 are halfword fields (target sh/lh), were s32 -> retyped
 *   s16 (store+reads). st->0xA30..0xA33 are the u8 RGB+pad quad (target sb),
 *   were s32 -> retyped u8. Callee-output stack halfwords spDC/spDE (0xDC/
 *   0xDE, target lh) were f32 -> retyped s16.
 * PASS-4 2026-07-30 (agent-g): 63.93->68.06%.
 *   (1) sp+0xCC..0xF3 is ONE stack record (C234Rec) whose address escapes
 *       into the probe call -> post-call reads reload from memory.
 *   (2) $f20 zero-web killed via (f32)0-vs-0.0f web split (region-alternating
 *       spellings; 14 mtc1-zero remats vs target 13). Reverting the two
 *       compare-site casts regresses to 66.05 -- keep them.
 *   (3) Pool symbolization per expected relocs: D_00000920/24/28 ->
 *       func_000008F4+0x2C/30/34; D_00000128 -> func_000000F0+0x38;
 *       D_00000940/44/48/4C -> func_00000940+0/4/8/C; *(f64*)0x950 ->
 *       func_00000940+0x10; *(f64*)0x930 -> extern D_00000930 (target reloc
 *       is local .L00000930 -- name mismatch tolerated by objdiff);
 *       0x8A7C/8A94/8A90/8AA4/8AB0 call args -> (char*)func_00008A7C+off;
 *       D_00000000+0x8A78 -> func_00008A40+0x38; D_00000000+0x34 ->
 *       func_00000008+0x2C; 0x81C0 table -> extern s32 D_000081C0[idx].
 *   (4) f32 (not s32) loads for the 0x93C vec multiplies (kills cvt.s.w);
 *       int-typed 0x91C call arg (target lw a1, no f64 promotion); dropped
 *       bogus "0 f64-2nd-half" placeholder arg on the 8A94 call (f64#1
 *       belongs in a2/a3 via mfc1).
 *   Residual: frame 264 vs 248 (target has 80B of slot gaps: 4+12+4+36+8+8+8
 *   ghost region; volatile-pad fill attempt landed pads BELOW the aggregates,
 *   frame 328, fuzzy-neutral -- cfe does not slot volatile scalar pads
 *   between aggregates here), early-store scheduling, FP temp-ring numbering,
 *   var_t0/sp8C int-web coloring. */
/* PASS-6 2026-10-20 (agent-c run 114, 70.75 -> 77.19): every 3-word vec
 * shuffle (int-pair copies through spB4/sp54/sp68/sp40/sp7C) is a real
 * STRUCT ASSIGNMENT of a 12-byte f32 vec (V3C234). spB4 takes >= 9 copies,
 * so IDO pointer-izes them (`addiu v0,sp,0xB4; sw 0/4/8(v0)`), the target
 * shape. The temps are declared between the vec aggregates to land the
 * target's frame map (0x40/0x54/0x68/0x7C/0xB4/0xCC with ghost-slot gaps).
 * The fuzzy score is blind to sp offsets, so that part only matters for a
 * later exact match. */
typedef struct { f32 x, y, z; } V3C234;
typedef struct {
    /* +0x00 (sp 0xCC) */ s32 unk0;
    /* +0x04 */ s32 unk4;
    /* +0x08 */ s32 unk8;
    /* +0x0C (sp 0xD8) */ f32 d8;
    /* +0x10 (sp 0xDC) */ s16 dc;
    /* +0x12 (sp 0xDE) */ s16 de;
    /* +0x14 */ s32 e0;
    /* +0x18 (sp 0xE4) */ f32 e4;
    /* +0x1C */ f32 e8;
    /* +0x20 */ f32 ec;
    /* +0x24 (sp 0xF0) */ f32 f0;
} C234Rec; /* 0x28 — stack probe record; address escapes into the probe call,
            * so post-call reads reload from memory (kills the $f20 zero web) */
/* PASS-5 2026-08-22 (agent-f): NEGATIVE RESULT — pool-base-CSE web confirmed
 * as the dominant residual and NOT C-fixable here. Target rematerializes
 * every func_00000940+N / func_00008A7C+N ref as a fresh per-use lui %hi
 * (+addend) pair — build CSEs the shared base into a reg AND spills it
 * (0x38(sp), reloaded for the +0x28/+0x34 call args). Probed the
 * distinct-extern respelling (D_00000940/44/48/4C + f64 D_00000950 for the
 * five 940-pool sites): shape moves toward target but fuzzy REGRESSES
 * 67.99->67.32 (objdiff penalizes the D_-name/addend-0 relocs vs the
 * target's func_00000940+addend relocs more than the per-use-lui shape
 * gains). Same splat-fold family as the 55A0/6808 reloc caps: unreachable
 * without the spimdisasm USO-reloc migration (real per-symbol relocs in
 * expected/). Remaining non-reloc residual (arm-order transposition at the
 * 0x62-case + early-store scheduling) is downstream of the same web. */
extern f64 D_00000930;
extern s32 D_000081C0[];
void func_0000C234(char *arg0) {
    C234Rec spCC;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f12;
    V3C234 spB4;
    f32 temp_f12_2;
    f32 temp_f14;
    f32 temp_f16;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f2_3;
    f32 var_f0;
    f32 var_f2;
    f32 var_f2_2;
    s32 sp8C;
    s16 temp_v0_2;
    V3C234 sp7C;
    s16 temp_v1;
    s32 temp_a1_2;
    V3C234 sp68;
    s32 temp_v0;
    s32 temp_v0_4;
    V3C234 sp54;
    s32 temp_v1_2;
    s32 var_t0;
    V3C234 sp40;
    char *sp3C;
    s32 var_v0;
    char *temp_a1;
    char *temp_v0_3;
    char *var_v0_2;


    temp_v0 = *(s32 *)((char *)(arg0) + 0xA80);
    if (temp_v0 != 0) {
        *(s32 *)((char *)(arg0) + 0xA80) = (s32) (temp_v0 - 1);
    }
    spB4 = *(V3C234 *)((char *)(arg0) + 0xB4);
    temp_a1 = arg0 + 0xDC;
    *(f32 *)((char *)(arg0) + 0x890) = spB4.x;
    *(f32 *)((char *)(arg0) + 0x894) = spB4.y;
    *(f32 *)((char *)(arg0) + 0x898) = spB4.z;
    spB4 = *(V3C234 *)((char *)(arg0) + 0x318);
    *(f32 *)((char *)(arg0) + 0x89C) = spB4.x;
    *(f32 *)((char *)(arg0) + 0x8A0) = spB4.y;
    *(f32 *)((char *)(arg0) + 0x8A4) = spB4.z;
    spCC.f0 = 0.0f;
    spCC.ec = 0.0f;
    spCC.e4 = 0.0f;
    spCC.e8 = 1.0f;
    *(s16 *)((char *)(arg0) + 0x9A2) = 0;
    *(f32 *)((char *)(arg0) + 0x9D0) = D_bu_ro_pool[0x920 / 4];
    sp3C = temp_a1;
    if (func_00000000(*(s32 *)((char *)&D_00000000 + 0), temp_a1, 0x457A0000, &spCC) != 0) {
        *(s16 *)((char *)(arg0) + 0x9A0) = func_00000000(*(s32 *)((char *)((*(s32 *)((char *)&D_00000000 + 0))) + 0x84), spCC.dc);
        if (spCC.d8 < D_bu_ro_pool[0x924 / 4]) {
            *(s16 *)((char *)(arg0) + 0x9A2) = *(s16 *)((char *)(arg0) + 0x9A0);
        } else {
            *(s16 *)((char *)(arg0) + 0x9A2) = 0;
        }
        *(s32 *)((char *)(arg0) + 0x9A8) = func_00000000(*(s32 *)((char *)((*(s32 *)((char *)&D_00000000 + 0))) + 0x84), spCC.dc);
        *(f32 *)((char *)(arg0) + 0x9D0) = spCC.d8;
        if ((spCC.f0 < ((D_bu_ro_pool[0x128 / 4] * ((*(f32 *)((char *)(arg0) + 0xDC) * spCC.e4) + ((*(f32 *)((char *)(arg0) + 0xE0) + D_bu_ro_pool[0x928 / 4]) * spCC.e8) + (*(f32 *)((char *)(arg0) + 0xE4) * spCC.ec))) + 4.0f)) || (*(s32 *)((char *)(arg0) + 0x938) != 0)) {
            *(s32 *)((char *)(arg0) + 0xA4C) = 1;
            spB4 = *(V3C234 *)&spCC.e4;
            var_t0 = 0;
            *(f32 *)((char *)(arg0) + 0x9D8) = spB4.x;
            *(f32 *)((char *)(arg0) + 0x9DC) = spB4.y;
            *(f32 *)((char *)(arg0) + 0x9E0) = spB4.z;
            *(f32 *)((char *)(arg0) + 0x9E4) = spCC.f0;
            if (D_00000930 < (f64) *(f32 *)((char *)(arg0) + 0x9DC)) {
                var_t0 = 1;
            }
            *(s32 *)((char *)(arg0) + 0xA50) = var_t0;
        } else {
            *(s32 *)((char *)(arg0) + 0xA4C) = 0;
            *(f32 *)((char *)(arg0) + 0x9D8) = (f32) 0;
            *(f32 *)((char *)(arg0) + 0x9E0) = (f32) 0;
            *(f32 *)((char *)(arg0) + 0x9E4) = (f32) 0;
            *(s32 *)((char *)(arg0) + 0xA50) = 0;
            *(f32 *)((char *)(arg0) + 0x9DC) = 1.0f;
        }
        if ((*(f32 *)((char *)(arg0) + 0x9D0) < (f32) 0) && (spCC.e8 > (f32) 0)) {
            *(f32 *)((char *)(arg0) + 0x31C) = 10.0f;
        }
        if ((*(s32 *)((char *)(arg0) + 0x938) != 0) && (*(f32 *)((char *)(arg0) + 0x348) > 10.0f)) {
            temp_v1 = *(s16 *)((char *)(arg0) + 0x9A0);
            var_v0 = 0;
            if (temp_v1 == 0x64) {
                *(s32 *)((char *)(arg0) + 0xA5C) = 0x64;
                var_v0 = 2;
            } else if (temp_v1 == 0x63) {
                *(s32 *)((char *)(arg0) + 0xA5C) = 0x46;
                var_v0 = 1;
            }
            if (var_v0 != 0) {
                temp_f2 = *(f32 *)((char *)(arg0) + 0x970);
                if (temp_f2 < 0.0f) {
                    var_f0 = -temp_f2;
                } else {
                    var_f0 = temp_f2;
                }
                func_00000000(*(s32 *)((char *)(arg0) + 0x800), D_000081C0[(s32) ((f32) var_v0 + (var_f0 * 3.0f))], 2);
            }
        }
        /* Run 109 (agent-c, 69.25 -> 70.94): target block order is default,
         * 0x61, 0x62 (flat goto ladder); the 0x940/0x944/0x948 pool reads are
         * per-address scalars (D_bu_ro_pool[] here CSEs the base, -0.1). */
        temp_v0_2 = *(s16 *)((char *)(arg0) + 0x9A0);
        if (temp_v0_2 == 0x61) goto c61;
        if (temp_v0_2 == 0x62) goto c62;
        *(f32 *)((char *)(arg0) + 0x288) = (f32) D_00000938;
        *(f32 *)((char *)(arg0) + 0x28C) = (f32) D_0000093C;
        *(u8 *)((char *)(arg0) + 0xA33) = 0;
        *(u8 *)((char *)(arg0) + 0xA32) = 0;
        *(u8 *)((char *)(arg0) + 0xA31) = 0;
        *(u8 *)((char *)(arg0) + 0xA30) = 0;
        *(f32 *)((char *)(arg0) + 0x910) = (f32) *(f32 *)((char *)(arg0) + 0x5A0);
        *(f32 *)((char *)(arg0) + 0x290) = D_00000940;
        goto cend;
c61:
        *(f32 *)((char *)(arg0) + 0x288) = (f32) *(f32 *)((char *)(arg0) + 0x120);
        *(f32 *)((char *)(arg0) + 0x28C) = (f32) *(f32 *)((char *)(arg0) + 0x138);
        *(f32 *)((char *)(arg0) + 0x290) = (f32) *(f32 *)((char *)(arg0) + 0x1B0);
        *(f32 *)((char *)(arg0) + 0x910) = (f32) *(f32 *)((char *)(arg0) + 0x570);
        func_00000000(*(s32 *)((char *)((*(s32 *)((char *)&D_00000000 + 0))) + 0x84), spCC.de, arg0 + 0xA30);
        goto cend;
c62:
        *(f32 *)((char *)(arg0) + 0x288) = 0.0f;
        *(f32 *)((char *)(arg0) + 0x28C) = D_00000944;
        *(f32 *)((char *)(arg0) + 0x910) = (f32) *(f32 *)((char *)(arg0) + 0x588);
        *(f32 *)((char *)(arg0) + 0x290) = D_00000948;
        func_00000000(*(s32 *)((char *)((*(s32 *)((char *)&D_00000000 + 0))) + 0x84), spCC.de, arg0 + 0xA30);
cend:
        *(s32 *)((char *)(arg0) + 0x95C) = 0;
    } else {
        temp_v1_2 = *(s32 *)((char *)(arg0) + 0x95C);
        *(s32 *)((char *)(arg0) + 0xA4C) = 0;
        *(f32 *)((char *)(arg0) + 0x9D8) = (f32) 0;
        *(f32 *)((char *)(arg0) + 0x9E0) = (f32) 0;
        *(f32 *)((char *)(arg0) + 0x9E4) = (f32) 0;
        *(s32 *)((char *)(arg0) + 0x95C) = (s32) (temp_v1_2 + 1);
        *(f32 *)((char *)(arg0) + 0x9DC) = 1.0f;
        if (temp_v1_2 >= 0x97) {
            temp_a1_2 = *(s32 *)((char *)&func_00008A40 + 0x38);
            sp8C = temp_a1_2;
            temp_v0_3 = func_00000000(*(s32 *)((char *)&D_00000000 + 0), temp_a1_2, sp3C, 0);
            if (temp_v0_3 != 0) {
                sp7C = *(V3C234 *)((char *)(temp_v0_3) + 0x30);
                sp7C.y += 200.0f;
                func_00000000(arg0, &sp7C);
                *(s32 *)((char *)(arg0) + 0x95C) = 0;
                func_00000000((char *)func_00008A7C + 0);
                func_00000000((char *)func_00008A7C + 0x18, (char *)func_00008A7C + 0x14, (f64) sp7C.x, (f64) sp7C.y, (f64) sp7C.z);
            }
        }
    }
    *(f32 *)((char *)(arg0) + 0x968) = 0.0f;
    *(s32 *)((char *)(arg0) + 0x938) = 0;
    *(f32 *)((char *)(arg0) + 0x93C) = 0.0f;
    *(f32 *)((char *)(arg0) + 0x940) = 0.0f;
    *(f32 *)((char *)(arg0) + 0x944) = 0.0f;
    func_00000000((char *)func_00008A7C + 0x28);
    func_00000000(arg0);
    func_00000000((char *)func_00008A7C + 0x34);
    if (*(s32 *)((char *)(arg0) + 0x9A8) & 0x400) {
        func_00000000(arg0 + 0x808, 1);
        var_f2 = D_bu_ro_pool[0x94C / 4];
    } else {
        func_00000000(arg0 + 0x808, 0);
        var_f2 = 1.0f;
    }
    if (*(s32 *)((char *)&func_00000008 + 0x2C) != 2) {
        temp_f0 = *(f32 *)((char *)(arg0) + 0x91C);
        *(f32 *)((char *)(arg0) + 0x91C) = (f32) ((f64) temp_f0 + ((f64) (var_f2 - temp_f0) * D_bu_ro_dpool[0x950 / 8]));
        func_00000000(*(s32 *)((char *)(arg0) + 0x850), *(s32 *)((char *)(arg0) + 0x91C));
    }
    temp_v0_4 = *(s32 *)((char *)(arg0) + 0x938);
    if (temp_v0_4 != 0) {
        temp_f2_2 = (f32) temp_v0_4;
        var_v0_2 = arg0 + 0x93C;
        *(f32 *)((char *)(arg0) + 0x93C) = (f32) (*(f32 *)((char *)(arg0) + 0x93C) / temp_f2_2);
        *(f32 *)((char *)(arg0) + 0x940) = (f32) (*(f32 *)((char *)(arg0) + 0x940) / temp_f2_2);
        *(f32 *)((char *)(arg0) + 0x968) = (f32) (*(f32 *)((char *)(arg0) + 0x968) / temp_f2_2);
        temp_f12 = *(f32 *)((char *)(arg0) + 0x968);
        *(f32 *)((char *)(arg0) + 0x944) = (f32) (*(f32 *)((char *)(arg0) + 0x944) / temp_f2_2);
        if (temp_f12 < (f32) 0) {
            var_v0_2 = arg0 + 0x93C;
            sp68.x = *(f32 *)((char *)(arg0) + 0x93C) * temp_f12;
            sp68.y = *(f32 *)((char *)(var_v0_2) + 0x4) * temp_f12;
            sp68.z = *(f32 *)((char *)(var_v0_2) + 0x8) * temp_f12;
            spB4 = sp68;
            sp54 = spB4;
            *(f32 *)((char *)(arg0) + 0x920) = sp54.x;
            *(f32 *)((char *)(arg0) + 0x924) = sp54.y;
            *(f32 *)((char *)(arg0) + 0x928) = sp54.z;
        } else {
            *(f32 *)((char *)(arg0) + 0x920) = (f32) 0;
            *(f32 *)((char *)(arg0) + 0x924) = (f32) 0;
            *(f32 *)((char *)(arg0) + 0x928) = (f32) 0;
        }
        spB4 = *(V3C234 *)((char *)(var_v0_2) + 0x0);
        *(f32 *)((char *)(arg0) + 0x948) = spB4.x;
        *(f32 *)((char *)(arg0) + 0x94C) = spB4.y;
        *(f32 *)((char *)(arg0) + 0x950) = spB4.z;
        var_f2_2 = (*(f32 *)((char *)(arg0) + 0x33C) * *(f32 *)((char *)(arg0) + 0x93C)) + (*(f32 *)((char *)(arg0) + 0x340) * *(f32 *)((char *)(arg0) + 0x940)) + (*(f32 *)((char *)(arg0) + 0x344) * *(f32 *)((char *)(arg0) + 0x944));
        if (var_f2_2 > 0.0f) {
            var_f2_2 = 0.0f;
        }
        temp_f0_2 = -var_f2_2;
        sp40.x = *(f32 *)((char *)(var_v0_2) + 0x0) * temp_f0_2;
        sp40.y = *(f32 *)((char *)(var_v0_2) + 0x4) * temp_f0_2;
        sp40.z = *(f32 *)((char *)(var_v0_2) + 0x8) * temp_f0_2;
        spB4 = sp40;
        sp54 = spB4;
        *(f32 *)((char *)(arg0) + 0x92C) = sp54.x;
        *(f32 *)((char *)(arg0) + 0x930) = sp54.y;
        *(s32 *)((char *)(arg0) + 0x9D4) = 0;
        *(f32 *)((char *)(arg0) + 0x934) = sp54.z;
    } else {
        *(f32 *)((char *)(arg0) + 0x920) = (f32) 0;
        *(f32 *)((char *)(arg0) + 0x924) = (f32) 0;
        *(s32 *)((char *)(arg0) + 0x9D4) = (s32) (*(s32 *)((char *)(arg0) + 0x9D4) + 1);
        *(f32 *)((char *)(arg0) + 0x928) = (f32) 0;
        *(f32 *)((char *)(arg0) + 0x92C) = (f32) 0;
        *(f32 *)((char *)(arg0) + 0x930) = (f32) 0;
        *(f32 *)((char *)(arg0) + 0x934) = (f32) 0;
    }
    temp_f2_3 = *(f32 *)((char *)(arg0) + 0x92C);
    temp_f12_2 = *(f32 *)((char *)(arg0) + 0x930);
    temp_f14 = *(f32 *)((char *)(arg0) + 0x934);
    temp_f16 = *(f32 *)((char *)(arg0) + 0x910);
    *(f32 *)((char *)(arg0) + 0xA1C) = (f32) (*(f32 *)((char *)(arg0) + 0xA1C) + (((*(f32 *)((char *)(arg0) + 0x3BC) * temp_f2_3) + (*(f32 *)((char *)(arg0) + 0x3C0) * temp_f12_2) + (*(f32 *)((char *)(arg0) + 0x3C4) * temp_f14)) * temp_f16));
    *(f32 *)((char *)(arg0) + 0xA20) = (f32) (*(f32 *)((char *)(arg0) + 0xA20) + (((*(f32 *)((char *)(arg0) + 0x3B0) * temp_f2_3) + (*(f32 *)((char *)(arg0) + 0x3B4) * temp_f12_2) + (*(f32 *)((char *)(arg0) + 0x3B8) * temp_f14)) * temp_f16));
    *(f32 *)((char *)(arg0) + 0xA24) = (f32) (*(f32 *)((char *)(arg0) + 0xA24) - ((*(f32 *)((char *)(arg0) + 0x3C8) * temp_f2_3) + (*(f32 *)((char *)(arg0) + 0x3CC) * temp_f12_2) + (*(f32 *)((char *)(arg0) + 0x3D0) * temp_f14)));
}
#else
INCLUDE_ASM("asm/nonmatchings/bootup_uso", func_0000C234);
#endif

void func_0000CACC(char *a0) {
    *(int*)(a0 + 0xA14) = 0xF4240;
    *(int*)(a0 + 0xA58) = *(int*)(a0 + 0xA58) ^ 0x100;
}

// func_0000CAE8 — STRUCTURAL PASS (0x1F8 / 126 insns, no episode).
// Per-round / per-attempt state reset & re-arm on a large state struct
// (st = a0 = s0).
//
//   void func_0000CAE8(State *st) {
//     func_00000000(?, 1, tableA[st->0x8C4]);            // indexed trigger
//     int j = (s16)*(s16*)((char*)st + st->0x904*2 + 0x8C6);
//     func_00000000(st+0x3EC, 1, tableB[j]);             // 2nd idx trigger
//     if (st->0xA58 & 0x80) func_00000000(st, 0);        // cond reset
//     func_00000000(st+0x90C, 0x17C);
//     st->0x9D0 = F32(func_00000940+0x18);               // folded const
//     st->0xA78 = st->0x768;
//     switch (*(int*)(func_00000008+0x2C)) {              // mode/difficulty
//       case 2:        st->0xA14 = 0x5A;     break;       // 90
//       case 3: case 4:st->0xA14 = 0x4B;     break;       // 75
//       default:       st->0xA14 = 0xF4240;  break;       // 1000000
//     }
//     st->0x3DC = 1;
//     st->0x9E8 = st->0x9EC = st->0x9F0 = st->0x9F4 = 0.0f;  // clear quad
//     st->0xA54 = 0;
//     *(int*)D_0 &= ~0x8;  st->0xA0C = 1.0f;  *(int*)D_0b = 0;  // global rst
//     if (st->0x8BC & 0x20) { st->0xA58 = st->0x8BC; func_00000000(st); }
//     func_00000000(st, st+0x3A4);  func_00000000(st);
//     func_00000000(st->0x840);     func_00000000(st->0x804);   // sub-resets
//     if (!((st->0xA58 << 14) >= 0)) func_00000000(&func_00008A7C+0x40,
//                                                  st->0xC);
//     // snapshot: st->0xB4/B8/BC -> st->0x890/894/898 (prev pos),
//     //           st->0x318/31C/320 -> st->0x89C/8A0/8A4 (prev pos2)
//   }
//
// Struct-typing reference:
//   st: 0x8C4 / 0x904 indexed-trigger keys; 0x8C6[] s16 index array;
//     0xA58 status flags (bit 0x80 reset, bit-17 via <<14 test);
//     0xA14 round budget/limit (mode-set); 0x3DC armed flag;
//     0x9E8..0x9F4 cleared f32 quad; 0xA0C f32=1.0; 0x8BC config
//     (bit 0x20); 0xB4/B8/BC + 0x318/31C/320 pos Vec3s snapshotted to
//     0x890.. / 0x89C..; 0x768/0x9D0/0x840/0x804/0xC sub-objs.
//   Folded refs (literal-pool/placeholder fold family — func_00000940
//   +0x18 corroborates the contiguous strided table from func_0000B75C;
//   func_00000008+0x2C a mode global; func_00008A7C+0x40 a folded
//   struct; see
//   docs/N64_FORENSICS.md#bootup-uso-fp-literal-pool-folded-into-func-0000098C).
// Caps (DEFERRED): 126-insn reset w/ folded mode/const refs — byte
//   match blocked by deferred pool symbolization. Real-C STRUCTURAL
//   body below — per-round state reset / re-arm skeleton.
//   Name pre-checked: no extern reuse.
/* EXACT via an IDO 7.1 NCC (C++) donor: src/bootup_uso/bootup_uso_ncc_CAE8.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder (agent-c run 116). The
 * Vec3 snapshots are an inline V3::operator=(const V3 &) forwarding to a
 * by-value set(V3); see docs/IDO_CODEGEN.md#ncc-game-libs-vec3-62a2c. */
void func_0000CAE8(char *st) {
    volatile char *r = st;
}

// func_0000CCE0 — STRUCTURAL PASS (0x2C0 / 176 insns, no episode).
// HUD/menu layout builder: creates ~12 positioned widget elements into
// the state's UI container, gated by a flag, then finalizes them.
//
//   void func_0000CCE0(State *st, a1) {       // st -> s0; cont = st+0x39C
//     // repeated unit (x ~12), per element:
//     //   e = func_00000000(0, st, TYPE, X, P_at_sp10, Y_at_sp14);
//     //   func_00000000(cont, e);             // attach to st->0x39C
//     //   save e -> sp slot;
//     // batch 1 TYPE/coords: {2:(140,?)}, {8:(-140)}, {1:(350)},
//     //   {4:(-350)} with P = F32(func_00000940 + {0x1C,0x20,0x24,0x28});
//     // then func_00000000(st, savedE) x4  (configure pass);
//     if (st->0xA58 & 0x200) return;           // skip optional batch
//     // batch 2: TYPE {0,0x20,0x80,0x10,0x40} at screen coords
//     //   140/-140/350/-350/250/-250/-150/-400/400 (0x430C0000 etc.)
//     //   with P = F32(func_00000940 + 0x2C); attach + configure each.
//   }
//
// Struct-typing reference:
//   st(a0=s0): 0x39C = UI container/parent (s1, receives each element);
//     0xA58 status flags (bit 0x200 = suppress optional HUD batch).
//   func_00000000(0, st, type, xF32, paramF32, yF32) = widget factory;
//     func_00000000(cont, e) = attach; func_00000000(st, e) = configure.
//   Screen-coord consts: ±140 (0x430C0000), ±350 (0x43AF0000),
//     ±250 (0x437A0000), -150 (0xC3160000), ±400 (0x43C80000).
//   Folded param table: func_00000940 + {0x1C,0x20,0x24,0x28,0x2C}
//   (read-only f32) — extends the contiguous strided fold table
//   (now spans +0x4..+0x2C, ≥11 entries; strongly confirms the single
//   D_base[] characterization in
//   docs/N64_FORENSICS.md#bootup-uso-fp-literal-pool-folded-into-func-0000098C).
// 2026-07-30 (agent-g) 84.2->90.0: pool refs are matchable addend relocs
//   (premise corrected); anchor-split x4 (098C/08F4/08D4/08B4) kills the
//   base CSE (frame -88 exact), single-scope reversed decl list +
//   volatile int pad[1] lands all 9 save-slots + the 64(sp) gap exact;
//   g2 Y-coord decode fixed (+400 -> -400). RESIDUAL: direct-form
//   lui-at/lwc1 cap x5 (in-TU fn base emits addiu-form, +1 insn/site;
//   4 sites also carry the wrong-anchor reloc name) + func_cce0
//   float-prototype jal reloc names (NM-only; func_00000000 is K&R
//   in-TU, would double-promote floats) + cap-coupled t-reg numbering.
//   Name pre-checked: no extern reuse.
/* RUN 108 (agent-c) EXACT: the five pool params are D_bu_ro_pool at RoData +0x95C..+0x96C
 * (lui at / lwc1; the splat text shows them as func_00000940 + 0x1C..0x2C), and the first element is created
 * inside the attach call (`f(cont, e0 = cce0(...))`) so its spill store
 * schedules after the a0 copy. */
// PARTIAL-FORM corrected NM body for func_0000CCE0 (replaces lines 8516..8554)
extern char *func_cce0(int, char *, int, float, float, float);
void func_0000CCE0(char *st, int a1) {
    char *cont = st + 0x39C;
    /* Single decl list, slots assigned descending from frame top:
     * g4=80 g3=76 g2=72 g1=68 pad=64 g0=60 e3=56 e2=52 e1=48 e0=44 —
     * reproduces target's 64(sp) gap (odd-parity pad, volatile array
     * survives trimming). */
    char *g4, *g3, *g2, *g1;
    volatile int pad[1];
    char *g0, *e3, *e2, *e1, *e0;
    func_00000000(cont, e0 = (char *)func_cce0(0, st, 2, 140.0f, D_bu_ro_pool[0x95C / 4], -350.0f));
    e1 = (char *)func_cce0(0, st, 8, -140.0f, D_bu_ro_pool[0x960 / 4], -350.0f);
    func_00000000(cont, e1);
    e2 = (char *)func_cce0(0, st, 1, 140.0f, D_bu_ro_pool[0x964 / 4], 350.0f);
    func_00000000(cont, e2);
    e3 = (char *)func_cce0(0, st, 4, -140.0f, D_bu_ro_pool[0x968 / 4], 350.0f);
    func_00000000(cont, e3);
    func_00000000(st, e0);
    func_00000000(st, e1);
    func_00000000(st, e2);
    func_00000000(st, e3);
    if (*(int *)(st + 0xA58) & 0x200) return;
    g0 = (char *)func_cce0(0, st, 0x00, 0.0f, D_bu_ro_pool[0x96C / 4], 0.0f);
    func_00000000(cont, g0);
    func_00000000(st, g0);
    g1 = (char *)func_cce0(0, st, 0x20, 250.0f, -150.0f, -400.0f);
    func_00000000(cont, g1);
    g2 = (char *)func_cce0(0, st, 0x80, -250.0f, -150.0f, -400.0f);
    func_00000000(cont, g2);
    g3 = (char *)func_cce0(0, st, 0x10, 250.0f, -150.0f, 400.0f);
    func_00000000(cont, g3);
    g4 = (char *)func_cce0(0, st, 0x40, -250.0f, -150.0f, 400.0f);
    func_00000000(cont, g4);
    func_00000000(st, g1);
    func_00000000(st, g2);
    func_00000000(st, g3);
    func_00000000(st, g4);
}

// func_0000CFA0 — STRUCTURAL PASS (0x4A0 / 296 insns, 64.76% fuzzy, no episode).
// Per-frame active-object sync + draw-state / color update on a large
// state struct (st = a0 = s0).
//
// 2026-06-22 (agent-d): refined to byte-faithful structure (build 292/296
// insns, body logic matches target instruction-for-instruction). Fixed:
//   - reloc form: &func_0000023C+0x18 / &func_00000080+0x64 / &func_00000940
//     +0x30/+0x38 (was &D_00000000 — wrong reloc symbol).
//   - 112.0f/255.0f forced runtime div via shared `float div255=255.0f`
//     (held in $f0, reused for all 4 divisions) — defeats const-fold to .rodata.
//   - RGB halve = `(float)(unsigned)(unsigned char)(*(u8*)/2)` reproducing the
//     signed-/2 (bgez;+1;sra) + andi 0xff + unsigned->float (4f80) fixup chain.
// RESIDUAL CAP (regalloc/K&R, NOT GOT-blind — every global is reloc'd):
//   (1) First call passes f32 args (st->0xBC, 0.0f) that the target stores
//   single-precision (swc1 16(sp)/24(sp)); the in-TU K&R `func_00000000()`
//   default-promotes float->double (cvt.d.s + sdc1), inflating the frame to
//   -160 (target -136) and cascading the whole register allocation. The
//   `<name>f` float-prototype workaround is unavailable: bootup is LINK-DIRECT,
//   so a func_00000000f reloc breaks the ROM link AND mismatches the target's
//   R_MIPS_26 func_00000000 symbol (see docs IDO knr-float-call). (2) ctx =
//   *(func_0000023C+0x18) is read 3x; IDO -O2 CSEs the guard+call-arg reads
//   (no intervening barrier) while the target reloads each — no C knob forces
//   the reload. Net: correct logic, divergent regalloc — permuter/cc territory.
//
//   void func_0000CFA0(State *st) {
//     int *ctx = *(int**)(func_0000023C+0x18);          // global ctx ptr
//     if ((st->0xA58 & 0x2000) && ctx == st->0x8DC) return;   // unchanged
//     R *r = func_00000000(ctx, st->0xB4, st->0xB8, st->0xBC,
//                          0, out_at_sp84);              // query current
//     if (!r) return;
//     st->0xA40 = r;
//     int changed = (ctx->0x98 < out_at_sp84) ? 1
//                  : ((ctx ^ st->0x8DC) != 0);           // change detect
//     if (bit15(st->0xA58) && st->0x9A0==0x61 && st->0x904!=9
//         && st->0x938!=0) {
//       func_00000000(.,0xD);  st->0x8FC[0..2] = r[0..2];  // copy Vec3
//       set sp Vec3 = {1,1,1, F32(func_00000940+0x30)};
//       func_00000000(.); func_00000000(0x10);
//       dispatch st->0x824->0x28 (vtbl: ->0x1C fn, ->0x18 s16 base);
//       func_00000000(.,6);  st->0x8FC[0..2] = v0[0..2];
//     }
//     if (changed) { if (st->0x8B8 & 4) goto reset; }
//     st->0xA44 = 1;
//     if ((st->9A8 & 2) && (st->0xA58 & 0x1000) && st->0xA4C) {
//       func_00000000(st->0x850, st+0x9D8); func_00000000(st);
//       func_00000000(0x11); func_00000000(0, st+0x8E0);
//       func_00000000(.); func_00000000(st); func_00000000(st);
//       *(int*)(func_00000080+0x64) = 0;  goto tail;
//     }
//     if ((st->0x8B8 & 0xA) && st->0xA4C && !(st->9A8 & 0x400)) {
//       Vec3 d = func_00000000(st->0x850, &sp44);
//       float m = d.x*st->0x9D8 + d.y*st->0x9DC + d.z*st->0x9E0;
//       if ((double)m < DBL(func_00000940+0x38)) {        // folded f64
//         // RGB: st->0xA30/A31/A32 u8 -> halve(sra1) -> u32->float fixup
//         //   -> /255.0f (0x437F0000) -> st->0x9E8/0x9EC/0x9F0;
//         //   0x42E00000(112.0)/255 -> st->0x9F4;
//         func_00000000(st+0x9E8 ...);
//         if (st->0x8B8 & 2) { func_00000000(2);
//           dispatch st->0x814->0x28; dispatch st->0x820->0x28; }
//         if ((st->0x8B8 & 8) && st->0x938==0)
//           dispatch st->0x824->0x28;
//       }
//     }
//     *(int*)(func_00000080+0x64) = 0;
//   tail:
//     func_00000000((st->0x8B8 & 1) ? 0x11 : 0x14);
//     func_00000000(0, sp30); func_00000000(st);
//     func_00000000((*(int**)(func_0000023C+0x18))->0x108);
//   reset: func_00000000(. ); st->0xA44 = 0; ...
//   }
//
// Struct-typing reference:
//   st: 0x8DC cached-ctx ptr; 0xA58 status flags (0x2000 guard, 0x1000,
//     bit15); 0x8B8 draw flags (1,2,4,8,0xA); 0x9A8 sub-flags (2,0x400);
//     0xA40 query result; 0xA44 dirty; 0xA4C enable; 0x8FC Vec3 dest;
//     0x9D8/9DC/9E0 basis Vec3; 0x9E8/9EC/9F0/9F4 normalized color out;
//     0xA30/A31/A32 u8 RGB src; 0x814/0x820/0x824 sub-objs (->0x28
//     vtable, ->0x1C fn ptr, ->0x18 s16 base — the obj->0x28 dispatch
//     idiom); 0x850 a transform source; 0x9A0/0x904/0x938 state words.
//   Folded refs: func_0000023C+0x18 (global ctx ptr — writable fold);
//   func_00000940 +0x30 (f32) / +0x38 (f64) — extends the contiguous
//   strided fold table to +0x4..+0x38; func_00000080+0x64 (writable
//   global flag, cleared 2x); see
//   docs/N64_FORENSICS.md#bootup-uso-fp-literal-pool-folded-into-func-0000098C.
//   (Pool symbolization is RESOLVED: refs use real reloc symbols, not
//   placeholder offsets — see the cap note above for the true blocker.)
//   Real-C STRUCTURAL body below. Name pre-checked: no extern reuse.
/* EXACT via an IDO 7.1 NCC (C++) donor: src/bootup_uso/bootup_uso_ncc_CFA0.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder (agent-c run 116): real
 * virtual calls, V3 copies, ctx first defined as the call argument. */
void func_0000CFA0(char *st) {
    volatile char *r = st;
}

// func_0000D440 — STRUCTURAL PASS (0x4C0 / 304 insns, no episode).
// Per-frame UI/scene event-flag processor: walks status bits on the
// state struct, toggles a latch, fires descriptor-table actions and
// vtable sub-updates accordingly.
//
//   void func_0000D440(State *st) {            // st -> s0
//     func_00000000(&func_00008A7C+0x54);      // named action (table)
//     int fl = st->0xA58;
//     if (fl & 0x100) {
//       sub = st->0x800;
//       if (sub->0x18 & 0x20) st->0xA28 ^= 1;  // latch toggle
//       if (st->0xA28 && (sub->0x18 & 0x10)) return;   // early-out
//       if (sub->0x18 & 0x10) { st->0xA58 ^= 4; fl = st->0xA58; }
//     }
//     if (fl & 0x100) {
//       d = st->0x800;
//       if ((d->0x10 & 0x40) && (d->0x10 & 0x80)) {
//         sp58 = 9;
//         dispatch st->0x28 (vtbl: ->0x2C fn, ->0x28 s16 base, a0 = base+st);
//         fl = st->0xA58;
//       }
//     }
//     if (fl & 0x4) {
//       st->0x800->0x3C = 1;
//       func_00000000(&func_00008A7C+0x5C);
//       dispatch st->0x804->0x28 (->0x24 fn, ->0x20 base);
//       func_00000000(&func_00008A7C+0x64);
//       fl = st->0xA58;
//     }
//     if (fl & 0x80) { func_00000000(st+0x808 ...); ... }
//     // ... further fl-bit blocks (more func_00008A7C/func_00008AEC
//     //   descriptor actions + func_000000F0+0x48 + vtable dispatches);
//     func_00000000(&func_00008AEC+0x44);      // final action
//   }
//
// Struct-typing reference:
//   st: 0xA58 status/event flags (0x100 active, 0x4, 0x80 — toggled
//     via xori); 0xA28 bool latch (^=1); 0x800 sub-object
//     (->0x18 flags 0x10/0x20, ->0x10 flags 0x40/0x80, ->0x3C set 1);
//     0x804 sub-obj w/ ->0x28 vtable (->0x24 fn, ->0x20 s16 base);
//     0x28 own vtable (->0x2C fn, ->0x28 s16 base) — obj->0x28 dispatch
//     idiom; 0x808 a sub-record.
//   Folded action/desc tables (literal-pool/placeholder fold family):
//     func_00008A7C + {0x54,0x5C,0x64,0x6C} (8-stride),
//     func_00008AEC + {0x8,0x14,0x20,0x2C,0x38,0x44} (0xC-stride),
//     func_000000F0 + 0x48 (x3); see
//     docs/N64_FORENSICS.md#bootup-uso-fp-literal-pool-folded-into-func-0000098C.
// Caps (DEFERRED): 304-insn flag-walk dispatcher w/ folded tables +
//   vtable calls — byte-match blocked by deferred pool symbolization.
//   Real-C STRUCTURAL body below — UI/scene event-flag processor
//   skeleton. Name pre-checked: no extern reuse.
#ifdef NON_MATCHING
/* typed-float protos (0x0-aliases of func_00000000) so f32 args pass
 * single-precision (mfc1/swc1) instead of K&R double-promote (cvt.d.s+sdc1). */
extern void func_d440_f0call(int, int, float);
extern void func_d440_final(int, int, float, float, float, float, int, int, int);
extern float func_d440_frnd(void *);  /* float-return alias: result -> cvt.d.s */
extern float D_00000980;
/* Distinct folded-table action-record symbols (= func_00008A7C/AEC + N). Naming
 * each one distinctly defeats IDO's CSE of the shared %hi(func_00008A7C) base
 * into a spilled saved-reg — forces per-site lui+addiu rematerialization to
 * match the target's 21 luis (vs hoisted 12 + stack reloads). */
extern char D_00008AD0, D_00008AD8, D_00008AE0, D_00008AE8;
extern char D_00008AF4, D_00008B00, D_00008B0C, D_00008B18, D_00008B24, D_00008B30;
/* 3 distinct aliases of func_000000F0 so the repeated *(int*)(&F0+0x48) load is
 * rematerialized per call (target: 3x lui %hi + lw 0x48) not CSE'd to a spill. */
extern char D_F0a, D_F0b, D_F0c;
#endif
// func_0000D440 — STRUCTURAL PASS (0x4C0 / 304 insns, no episode).
// Per-frame UI/scene event-flag processor: walks status bits on the
// state struct, toggles a latch, fires descriptor-table actions and
// vtable sub-updates accordingly.
//
//   void func_0000D440(State *st) {            // st -> s0
//     func_00000000(&func_00008A7C+0x54);      // named action (table)
//     int fl = st->0xA58;
//     if (fl & 0x100) {
//       sub = st->0x800;
//       if (sub->0x18 & 0x20) st->0xA28 ^= 1;  // latch toggle
//       if (st->0xA28 && (sub->0x18 & 0x10)) return;   // early-out
//       if (sub->0x18 & 0x10) { st->0xA58 ^= 4; fl = st->0xA58; }
//     }
//     if (fl & 0x100) {
//       d = st->0x800;
//       if ((d->0x10 & 0x40) && (d->0x10 & 0x80)) {
//         sp58 = 9;
//         dispatch st->0x28 (vtbl: ->0x2C fn, ->0x28 s16 base, a0 = base+st);
//         fl = st->0xA58;
//       }
//     }
//     if (fl & 0x4) {
//       st->0x800->0x3C = 1;
//       func_00000000(&func_00008A7C+0x5C);
//       dispatch st->0x804->0x28 (->0x24 fn, ->0x20 base);
//       func_00000000(&func_00008A7C+0x64);
//       fl = st->0xA58;
//     }
//     if (fl & 0x80) { func_00000000(st+0x808 ...); ... }
//     // ... further fl-bit blocks (more func_00008A7C/func_00008AEC
//     //   descriptor actions + func_000000F0+0x48 + vtable dispatches);
//     func_00000000(&func_00008AEC+0x44);      // final action
//   }
//
// Struct-typing reference:
//   st: 0xA58 status/event flags (0x100 active, 0x4, 0x80 — toggled
//     via xori); 0xA28 bool latch (^=1); 0x800 sub-object
//     (->0x18 flags 0x10/0x20, ->0x10 flags 0x40/0x80, ->0x3C set 1);
//     0x804 sub-obj w/ ->0x28 vtable (->0x24 fn, ->0x20 s16 base);
//     0x28 own vtable (->0x2C fn, ->0x28 s16 base) — obj->0x28 dispatch
//     idiom; 0x808 a sub-record.
//   Folded action/desc tables (literal-pool/placeholder fold family):
//     func_00008A7C + {0x54,0x5C,0x64,0x6C} (8-stride),
//     func_00008AEC + {0x8,0x14,0x20,0x2C,0x38,0x44} (0xC-stride),
//     func_000000F0 + 0x48 (x3); see
//     docs/N64_FORENSICS.md#bootup-uso-fp-literal-pool-folded-into-func-0000098C.
// Caps (DEFERRED): 304-insn flag-walk dispatcher w/ folded tables +
//   vtable calls — byte-match blocked by deferred pool symbolization.
//   Real-C STRUCTURAL body below — UI/scene event-flag processor
//   skeleton. Name pre-checked: no extern reuse.
#ifdef NON_MATCHING
/* PASS 2026-07-10 (agent-g): 89.78 -> 94.40. Levers landed this pass:
 * (1) named-divisor locals `two=2.0f`/`four=4.0f` (INIT-AT-DECL form required;
 *     a separate `two=2.0f;` assignment const-propagates and REFOLDS to mul.s)
 *     -> both div.s sites now real div.s (0x348/2.0f and (f2-100)/4);
 * (2) six wobble records rewritten via ONE mutated pointer temp r4
 *     (lw v0; lwc1 0x64(v0); addiu v0,v0,0x64; swc1 0(v0) target shape;
 *     `*(f*)(p+0x64) +=` emits paired 0x64 disps, no addiu);
 * (3) volatile derefs bust duplicate-load CSE: guard (0x80)&(0x40) dual
 *     lw 0x10, and per-arm lw 0x18 in the A28 if/else (one lands in beql delay);
 * (4) per-arm `v0 = fl & 0x100` recompute duplicated into BOTH goto-block_9
 *     arms (target's andi sits in the if-arm's b delay slot; a shared merge
 *     label emitted one andi + extra b). Head region now locks rows 11-26.
 * NEGATIVE probes: full head de-goto restructure (early-return + else-only
 * recompute) 92.95->91.40 REVERTED (misread: target recomputes andi on both
 * paths); dot-product named partial-sum via f0 reuse 94.40->92.77 REVERTED.
 * REMAINING (94.40 residual): FP temp renumber cascade in the tf2 region
 * (f6/f7 vs f16/f17 double halves, div.s placement), frame -0xA0 vs -0x70
 * (named-local homes), int coloring (v0/v1 base swap, t-rotation offsets),
 * final dot-product add.s operand swap — allocator/scheduler class. */
void func_0000D440(char *st) {
    char *sub, *vt, *m, *a0_2;
    int fl, v0, t5;
    int sp58;
    float f0, f2;
    int a84;
    float two = 2.0f;   /* named divisors: keep /2.0f and /4.0f real div.s
                         * (IDO reciprocal-folds literal power-of-2 divisors
                         * to mul.s; init-AT-DECL form required — a separate
                         * `two = 2.0f;` assignment gets const-propagated and
                         * REFOLDS. See IDO_CODEGEN div-fold) */
    float four = 4.0f;
    func_00000000(&D_00008AD0);
    fl = *(int *)(st + 0xA58); if (1) {}
    v0 = fl & 0x100;
    /* volatile on the per-arm 0x18 reads: target loads 0x18(v0) SEPARATELY in
     * each branch arm (one lands in the beql delay); plain derefs get CSE'd
     * into one hoisted load + two andis. */
    if (v0 != 0) {
        if (*(int *)(*(char **)(st + 0x800) + 0x18) & 0x20) {
            *(int *)(st + 0xA28) ^= 1;
        }
        if (*(int *)(st + 0xA28) != 0) {
            if (*(volatile int *)(*(char **)(st + 0x800) + 0x18) & 0x10) {
                /* per-arm v0 recompute (duplicated statement, NOT a shared
                 * merge label): target's andi lands in this path's b delay. */
                fl = *(int *)(st + 0xA58);
                v0 = fl & 0x100;
                goto block_9;
            }
        } else {
            fl = *(int *)(st + 0xA58);
            t5 = fl ^ 4;
            if (*(volatile int *)(*(char **)(st + 0x800) + 0x18) & 0x10) {
                *(int *)(st + 0xA58) = t5;
                fl = t5;
            }
            v0 = fl & 0x100;
            goto block_9;
        }
    } else {
    block_9:
        if (v0 != 0) {
            sub = *(char **)(st + 0x800);
            /* volatile busts the duplicate-load CSE: target lw's 0x10(a0) TWICE
             * (one per bit test), plain int derefs merge to one lw. */
            if ((!!(*(volatile int *)(sub + 0x10) & 0x80)) & (!!(*(volatile int *)(sub + 0x10) & 0x40))) {
                sp58 = 9;
                vt = *(char **)(st + 0x28);
                (*(void (**)(char *, int *))(vt + 0x2C))((char *)(*(short *)(vt + 0x28) + (int)st), &sp58);
                fl = *(int *)(st + 0xA58);
            }
        }
        if (fl & 4) {
            *(int *)(*(char **)(st + 0x800) + 0x3C) = 1;
            func_00000000(&D_00008AD8);
            sub = *(char **)(st + 0x804);
            vt = *(char **)(sub + 0x28);
            (*(void (**)(char *))(vt + 0x24))((char *)(sub + (short)*(short *)(vt + 0x20)));
            func_00000000(&D_00008AE0);
            fl = *(int *)(st + 0xA58);
        }
        a0_2 = st + 0x808;
        if (fl & 0x80) {
            *(float *)(st + 0xA1C) = -*(float *)(st + 0xA1C);
        }
        func_00000000(a0_2, *(short *)(st + 0x9A4));
        m = *(char **)(st + 0x840);
        *(short *)(st + 0x9A4) = 0;
        vt = *(char **)(m + 0x28);
        (*(void (**)(char *))(vt + 0x24))((char *)(m + (short)*(short *)(vt + 0x20)));
        *(short *)(st + 0xA10) = 0;
        *(float *)(st + 0xA1C) = 0.0f;
        *(float *)(st + 0xA20) = 0.0f;
        *(float *)(st + 0xA24) = 0.0f;
        if (*(int *)(st + 0xA58) & 0x800) {
            func_00000000(&D_00008AE8);
            func_00000000(st);
            func_00000000(&D_00008AF4);
            if (*(int *)(st + 0xA58) & 0x40000) {
                f0 = *(float *)(st + 0x31C);
                f2 = (f0 < 0.0f) ? -f0 : f0;
                a84 = (int)((f2 - 100.0f) / four);
                *(int *)(st + 0xA84) = a84;
                if (a84 < 0) {
                    *(int *)(st + 0xA84) = 0;
                }
                func_00000000(a0_2, *(int *)(st + 0xA84));
                if (*(int *)(st + 0x8B8) & 1) {
                    *(int *)(*(char **)(st + 0x81C) + 0x94) = *(int *)(st + 0xA84);
                }
                if (*(int *)(st + 0x8B8) & 4) {
                    *(int *)(*(char **)(st + 0x818) + 0x94) = *(int *)(st + 0xA84);
                }
            }
        }
        func_00000000(&D_00008B00);
        *(float *)(st + 0xA38) = *(float *)(st + 0x3C8) * *(float *)(st + 0x318)
                               + *(float *)(st + 0x3CC) * *(float *)(st + 0x31C)
                               + *(float *)(st + 0x3D0) * *(float *)(st + 0x320);
        func_00000000(*(char **)(st + 0x840));
        if (*(int *)(st + 0xA5C) != 0) {
            /* six wobble records, each via ONE mutated pointer temp (r4):
             * lw v0; lwc1 0x64(v0); addiu v0,v0,0x64; add; swc1 0(v0) —
             * the named-base `+= at offset` form instead emits paired
             * lwc1/swc1 0x64(base) with no addiu (wrong shape). */
            char *r4;
            float fv;
            double rnd = (double)func_d440_frnd((void *)(((((int)*(float *)(st + 0xDC) ^ (int)*(float *)(st + 0xE4)) >> 5) ^ 0x12345678)));
            float tf2 = (float)((rnd - 0.5) * 30.0 * (double)((float)*(int *)(st + 0xA5C) / 100.0f));
            double tf12;
            r4 = *(char **)(st + 0x824);
            fv = *(float *)(r4 + 0x64);
            r4 += 0x64;
            *(float *)r4 = fv + tf2;
            tf12 = (double)tf2 * 0.5;
            r4 = *(char **)(st + 0x828);
            fv = *(float *)(r4 + 0x64);
            r4 += 0x64;
            *(float *)r4 = fv + tf2;
            r4 = *(char **)(st + 0x830);
            fv = *(float *)(r4 + 0x64);
            r4 += 0x64;
            *(float *)r4 = fv + tf2;
            r4 = *(char **)(st + 0x834);
            fv = *(float *)(r4 + 0x64);
            r4 += 0x64;
            *(float *)r4 = (float)((double)fv + tf12);
            r4 = *(char **)(st + 0x82C);
            fv = *(float *)(r4 + 0x64);
            r4 += 0x64;
            *(float *)r4 = (float)((double)fv + tf12);
            r4 = *(char **)(st + 0x838);
            fv = *(float *)(r4 + 0x64);
            r4 += 0x64;
            *(float *)r4 = (float)((double)fv - tf12);
        }
        func_00000000(&D_00008B0C);
        func_00000000(&D_00008B18);
        func_00000000(st);
        func_00000000(&D_00008B24);
        func_d440_f0call(*(int *)((char *)&D_F0a + 0x48), (int)st, *(float *)(st + 0x348) / two);
        func_00000000(*(int *)((char *)&D_F0b + 0x48), (int)st, *(int *)(st + 0xDC), *(int *)(st + 0xE4));
        func_00000000(*(int *)((char *)&D_F0c + 0x48), (int)st, *(int *)(st + 0xA14));
        t5 = *(int *)(st + 0x8DC);
        if (t5 != 0) {
            func_d440_final(a0_2, st + 0xDC, *(float *)(st + 0x348),
                          -*(float *)(st + 0x968), *(float *)(st + 0x970),
                          *(float *)(st + 0x9D0) + D_00000980,
                          (int)*(short *)(st + 0x9A2), t5, *(int *)(st + 0xA58));
        }
        func_00000000(&D_00008B30);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/bootup_uso", func_0000D440);
#endif

// func_0000D900 — STRUCTURAL PASS (0x4C4 / 305 insns, no episode).
// Event/message dispatcher: switch on the incoming message opcode and
// run the matching handler (FP threshold checks + table-driven actions
// + vtable sub-updates) on the state struct.
//
//   void func_0000D900(State *st, Msg *m) {     // st -> a3
//     switch (m->0x0) {                          // opcode
//       case 0x14:                               // .L0000D98C
//         if (st->0xA80) break;                  // busy
//         o = m->0x4;
//         if ((double)o->0xC < DBL(D_00000988)   // properly-symbolized
//             && o->0x28->0xC(u16) == 0x74)      //   double constant
//           func_00000000(st+0x808);
//         break;
//       case 0x9:   ... case 0x6: ...            // .L0000DBB8 / .L0000DBC8
//       case 0x3E8: case 0x3E9: case 0x3EA: case 0x3EB:
//       case 0x3EC: case 0x3ED: case 0x3EE: case 0x3EF:
//         // numbered sub-actions: set a small index (t-reg 1..5),
//         //   run func_00008B44 / func_00008AEC descriptor actions,
//         //   FP compares vs the folded pool constant;
//       default: st->0xA54 = 6; break;           // .L0000DDAC
//     }
//     // common tail .L0000DDB8: restore ra, return.
//   }
//
// Struct-typing reference:
//   m(a1): 0x0 opcode, 0x4 payload obj (->0xC f32, ->0x28 vtable
//     (->0xC u16 type tag, ==0x74 here)); st(a3): 0xA80 busy/cooldown,
//     0xA54 result/status (set 6 on default), 0x808 sub-record.
//   CORRECTION (2026-07-10, agent-f, CONFIRMED via USO reloc table): the old
//     "SPURIOUS code symbol func_0000098C swallows the pool" claim here is
//     WRONG. func_0000098C is a GENUINE function (0x99C carries a live
//     R_MIPS_26 jal reloc); D_00000988 is just func_00000940's tail nop. The
//     FP loads reference symIdx=1 = the RoData SECTION base (immediate = pool
//     offset), NOT func_0000098C. The real rodata constants: this fn's
//     `func_0000098C + 0x4` (@0x988) = f64 0.1, `+ 0xC`-form (@0x990) = f64 0.2
//     (rodata data @ ROM 0xD9FE44). `func_0000098C + N` is just the correct
//     placeholder-byte reproduction, NOT a pool bug to re-extract.
//     func_00008AEC/func_00008B44 refs are the same matchable addend-reloc
//     class. See docs/N64_FORENSICS.md#bootup-uso-fp-literal-pool-folded-into-func-0000098C.
// Caps (DEFERRED): 305-insn opcode dispatcher — byte-match blocked by full
//   FP-body reconstruction + FP coloring + the file-split needed to fold the
//   pool load (func_0000098C is defined in this same .c), NOT by any pool
//   symbolization. Real-C STRUCTURAL body below. Name pre-checked: no extern
//   reuse. D_00000988 reuses pre-symbolized double const at file scope.
#ifdef NON_MATCHING
/* PASS-2 2026-06-10 (big-swing): FULL m2c graft (305 insns, 9 COP1 +
 * 2 f64 = mild; prior body replaced -- preserved in git). */
/* PASS-3 2026-10-16 (agent-c, 57.11 -> 87.02): the dispatch is NOT a switch.
 * The target tests 20, 9, 6, 1000, 1001, 1003, 1002, 1004, 1005, 1006, 1007
 * in that (source) order with every body out of line and default last; the
 * switch built a 1000..1007 jump table. Written as an if-goto chain, the last
 * test inverted (`if (v != 0x3EF) goto def; goto c3ED;`, IDO_CODEGEN
 * #unsorted-dispatch-if-goto-plus-switch-54aec) and `if (1) {}` in the two
 * one-store bodies so IDO keeps them out of line. Also: the calls take ONE arg
 * where m2c added a1 = self; the 0x8B38.. literals are func_00008AEC /
 * func_00008B44 + K addresses (data aliases, lui/addiu %hi/%lo(sym+K)); the
 * 0.1/0.2 f64 are symbols (lui at + ldc1 %lo); the case-6 fields are f32;
 * sp48 -> sp54 -> sp6C and the 0xE0 copy are Vec3 struct copies (target
 * base-register lw/sw pairs), not word casts with an (f32) conversion.
 * PASS-4 2026-10-16 (agent-c, 87.02 -> 99.08; 305/305 words, 34 of them
 * differ, all in the clamp + scale block and one rodata addend):
 * - frame 0x90: locals are homed top-down in DECLARATION order, scalars too
 *   (sp8C, temp_v0_3, sp84, vA, v6C, vB, v54, v48, seven scalars, v20); vA/vB
 *   are unused Vec3 slots and temp_t1 was the same variable as temp_v0_2.
 * - the case-0x3ED call takes a 4-byte struct BY VALUE (sw a1,4(sp) arg-area
 *   copy); `sp8C.w = sp84->w` stores it direct (whole-struct copy goes through
 *   an addiu base).
 * - clamp as one ternary; the three products as named locals with the
 *   y-product written back into the scale variable and a copy for z (target
 *   f0/f2/f14 + mov.s f12 copy).
 * - both dot products: `z + (y + x)` (sum of x,y first, x loads first) into a
 *   named temp, then `old + (f64)dot * K`.
 * - case 6: payload read inline (no named pointer: the t-register ring
 *   matches again), `v20.x *= 4.0f` x3, stores B4, B8, BC, then the zeros.
 * Residual: the clamp colours s in f0 + 0.0 in f2 and puts the copy at the
 *   join; the target copies at entry (t = f0) and keeps 0.0 in f12. Ternary,
 *   if/else, copy-first, register, if (1) and comma spellings all give the
 *   same colouring. bu_d900_k990 needs a pin at 0x990 for an exact. */
extern f64 D_00000988, bu_d900_k990;  /* rodata f64 0.1 (pinned 0x988) / 0.2 (0x990; target spells it func_0000098C + 4) */
extern char bu_d900_aec[], bu_d900_b44[];  /* data aliases of func_00008AEC / func_00008B44 (target lui/addiu %hi/%lo(sym+K)) */
typedef struct { s32 w; } S4D900;  /* 4-byte struct passed by value (target sw a1,4(sp) arg-area copy) */
void func_0000D900(char *arg0, char *arg1) {
    S4D900 sp8C;
    s32 temp_v0_3;
    S4D900 *sp84;
    Vec3 vA;
    Vec3 v6C;
    Vec3 vB;
    Vec3 v54;
    Vec3 v48;
    f32 temp_f16;
    f32 temp_f2;
    f32 var_f2;
    s32 temp_v0;
    char *temp_v0_2;
    f32 px;
    f32 pz;
    Vec3 v20;

    temp_v0 = *(s32 *)((char *)(arg1) + 0x0);
    if (temp_v0 == 0x14) goto c14;
    if (temp_v0 == 9) goto c9;
    if (temp_v0 == 6) goto c6;
    if (temp_v0 == 0x3E8) goto c3E8;
    if (temp_v0 == 0x3E9) goto c3E9;
    if (temp_v0 == 0x3EB) goto c3EB;
    if (temp_v0 == 0x3EA) goto c3EA;
    if (temp_v0 == 0x3EC) goto c3EC;
    if (temp_v0 == 0x3ED) goto c3ED;
    if (temp_v0 == 0x3EE) goto c3ED;
    if (temp_v0 != 0x3EF) goto cdef;
    goto c3ED;
c14:
        if (*(s32 *)((char *)(arg0) + 0xA80) == 0) {
            temp_v0_2 = *(s32 *)((char *)(arg1) + 0x4);
            temp_f2 = *(f32 *)((char *)(temp_v0_2) + 0xC);
            if (((f64) temp_f2 < D_00000988) && (*(u16 *)((char *)(*(s32 *)((char *)(temp_v0_2) + 0x28)) + 0xC) == 0x74)) {
                func_00000000(arg0 + 0x808);
                return;
            }
            temp_f2 = (temp_f2 < 0.0f) ? 0.0f : ((temp_f2 > 50.0f) ? 50.0f : temp_f2);
            {
            px = *(f32 *)((char *)(temp_v0_2) + 0x0) * temp_f2;
            var_f2 = temp_f2;
            temp_f2 = *(f32 *)((char *)(temp_v0_2) + 0x4) * temp_f2;
            pz = *(f32 *)((char *)(temp_v0_2) + 0x8) * var_f2;
            v48.x = px; v48.y = temp_f2; v48.z = pz; }
            v54 = v48;
            v6C = v54;
            *(f32 *)((char *)(arg0) + 0xB4) = (f32) (*(f32 *)((char *)(arg0) + 0xB4) + v6C.x);
            *(f32 *)((char *)(arg0) + 0xB8) = (f32) (*(f32 *)((char *)(arg0) + 0xB8) + v6C.y);
            *(f32 *)((char *)(arg0) + 0xBC) = (f32) (*(f32 *)((char *)(arg0) + 0xBC) + v6C.z);
            *(f32 *)((char *)(arg0) + 0x318) = (f32) (*(f32 *)((char *)(arg0) + 0x318) + v6C.x);
            *(f32 *)((char *)(arg0) + 0x31C) = (f32) (*(f32 *)((char *)(arg0) + 0x31C) + v6C.y);
            *(f32 *)((char *)(arg0) + 0x320) = (f32) (*(f32 *)((char *)(arg0) + 0x320) + v6C.z);
            temp_f16 = (*(f32 *)((char *)(arg0) + 0x3C4) * v6C.z) + ((*(f32 *)((char *)(arg0) + 0x3C0) * v6C.y) + (*(f32 *)((char *)(arg0) + 0x3BC) * v6C.x));
            *(f32 *)((char *)(arg0) + 0xA1C) = (f32) ((f64) *(f32 *)((char *)(arg0) + 0xA1C) + (f64) temp_f16 * bu_d900_k990);
            temp_f16 = (*(f32 *)((char *)(arg0) + 0x3D0) * v6C.z) + ((*(f32 *)((char *)(arg0) + 0x3CC) * v6C.y) + (*(f32 *)((char *)(arg0) + 0x3C8) * v6C.x));
            *(u16 *)((char *)(arg0) + 0xA10) = (u16) (*(u16 *)((char *)(arg0) + 0xA10) | 0x1F0);
            *(f32 *)((char *)(arg0) + 0xA24) = (f32) ((f64) *(f32 *)((char *)(arg0) + 0xA24) + (f64) -temp_f16 * 0.5);
            *(u16 *)((char *)(arg0) + 0x9A4) = (u16) *(u16 *)((char *)(*(s32 *)((char *)(temp_v0_2) + 0x28)) + 0xC);
            *(s32 *)((char *)(arg0) + 0xA80) = 0xA;
            return;
        }
        return;
c9:
        func_00000000(arg0);
        return;
c6:
        v20 = *(Vec3 *)((char *)(*(s32 *)((char *)(arg1) + 0x4)) + 0xE0);
        v20.x *= 4.0f;
        v20.y *= 4.0f;
        v20.z *= 4.0f;
        *(f32 *)((char *)(arg0) + 0xB4) = (f32) (*(f32 *)((char *)(arg0) + 0xB4) + v20.x);
        *(f32 *)((char *)(arg0) + 0xB8) = (f32) (*(f32 *)((char *)(arg0) + 0xB8) + v20.y);
        *(f32 *)((char *)(arg0) + 0xBC) = (f32) (*(f32 *)((char *)(arg0) + 0xBC) + v20.z);
        *(f32 *)((char *)(arg0) + 0x320) = 0.0f;
        *(f32 *)((char *)(arg0) + 0x31C) = 0.0f;
        *(f32 *)((char *)(arg0) + 0x318) = 0.0f;
        return;
c3E8:
        *(s32 *)((char *)(arg0) + 0xA54) = 1;
        *(s32 *)((char *)(arg0) + 0xA58) = (s32) (*(s32 *)((char *)(arg0) + 0xA58) | 4);
        func_00000000(*(s32 *)((char *)(arg0) + 0x804));
        func_00000000(bu_d900_aec + 0x4C);
        return;
c3E9:
        *(s32 *)((char *)(arg0) + 0xA54) = 2;
        *(s32 *)((char *)(arg0) + 0xA58) = (s32) (*(s32 *)((char *)(arg0) + 0xA58) | 4);
        func_00000000(*(s32 *)((char *)(arg0) + 0x804));
        func_00000000(bu_d900_aec + 0x54);
        return;
c3EB:
        *(s32 *)((char *)(arg0) + 0xA54) = 3;
        *(s32 *)((char *)(arg0) + 0xA58) = (s32) (*(s32 *)((char *)(arg0) + 0xA58) | 4);
        func_00000000(bu_d900_b44 + 0x4);
        func_00000000(*(s32 *)((char *)(arg0) + 0x804));
        return;
c3EA:
        if (1) {}
        *(s32 *)((char *)(arg0) + 0xA54) = 4;
        return;
c3EC:
        if (1) {}
        *(s32 *)((char *)(arg0) + 0xA54) = 5;
        return;
c3ED:
        *(s32 *)((char *)(arg0) + 0xA54) = 6;
        func_00000000(bu_d900_b44 + 0x10);
        if (*(s32 *)((char *)(arg0) + 0x848) == 2) {
            sp84 = (S4D900 *)(bu_d900_b44 + 0x24);
            func_00000000(*(s32 *)((char *)(arg0) + 0x840));
        } else {
            sp84 = (S4D900 *)(bu_d900_b44 + 0x28);
            func_00000000(*(s32 *)((char *)(arg0) + 0x840));
        }
        sp8C.w = sp84->w;
        temp_v0_3 = func_00000000(*(s32 *)((char *)&D_00000000 + 0), sp8C, 0, arg0);
        if (temp_v0_3 != 0) {
            func_00000000(arg0, temp_v0_3 + 0x30);
            return;
        }
        func_00000000(bu_d900_b44 + 0x2C, sp84);
        return;
cdef:
        func_00000000(arg0);
}
#else
INCLUDE_ASM("asm/nonmatchings/bootup_uso", func_0000D900);
#endif

void func_0000DDC4(int a0) {
}

/* func_0000DDCC - verified structural decode (0x138, 78 insns).
 * NEAR-SIBLING of func_0000DF04 / func_0000E014 (same Vec3
 * fetch-and-store fan-out + s0 struct); variant: a leading state
 * switch on s0->0x8C4.
 * Struct-typing reference: same layout as func_0000DF04/E014 -
 * s0->0xA58 (2648) u32 flags bit 7 (0x80) selector; s0->0x8F4
 * (2292) / 0x8F8 (2296) / 0x8FC (2300) Vec3i dest pointers (3x int
 * @ +0/+4/+8). NEW: s0->0x8C4 (2244) s32 state - values 6/7 take
 * an early func_00000000(s0) delegate-and-return; other states run
 * the fan-out with id pair 0xE/0xF (flag set) or 0xA/0xB (clear)
 * plus final id 0xC. fetch = func_00000000(&D, id) -> Vec3i src.
 * 2026-05-30: now 98.08% (size-EXACT 78 insns). Residual is a pure register-
 * cascade cap: each Vec3i dest `d = *(int**)(s0+OFF)` lands in $v1 (because $v0
 * holds the just-returned func_00000000 result), while the target puts each `d`
 * in a fresh $t reg ($t8/$t1/$t4/$t7/$t0) and the copied values shift accordingly
 * ($t0/$t3/$t6/$t9/$t2 vs mine's $t8/$t9/...). Same $v0/$v1-region allocation cap
 * as gl_func_0004E384 etc. — using distinct C locals per `d` does not move IDO off
 * the $v1 reuse (the load is post-call so $v1 is the next free caller-saved reg).
 * INCLUDE_ASM remains build path. */
void func_0000DDCC(char *s0) {
    unsigned int st = *(unsigned int*)(s0 + 0x8C4);
    if (st == 7 || st == 6) {
        func_00000000(s0);
        return;
    }
    if (*(unsigned int*)(s0 + 0xA58) & 0x80) {
        **(Tri3i **)(s0 + 0x8F4) = *(Tri3i *)func_00000000(&D_00000000, 0xE);
        **(Tri3i **)(s0 + 0x8F8) = *(Tri3i *)func_00000000(&D_00000000, 0xF);
    } else {
        **(Tri3i **)(s0 + 0x8F4) = *(Tri3i *)func_00000000(&D_00000000, 0xA);
        **(Tri3i **)(s0 + 0x8F8) = *(Tri3i *)func_00000000(&D_00000000, 0xB);
    }
    **(Tri3i **)(s0 + 0x8FC) = *(Tri3i *)func_00000000(&D_00000000, 0xC);
}

/* func_0000DF04 - verified structural decode (0x110, 68 insns,
 * Vec3 fetch-and-store fan-out).
 * Struct-typing reference: s0->0xA58 (2648) u32 flags, bit 7 (0x80)
 * selects which source-id pair feeds the two slots; s0->0x8F4
 * (2292) / s0->0x8F8 (2296) / s0->0x8FC (2300) = pointers to
 * destination Vec3i (3x int at +0/+4/+8) buffers. fetch =
 * func_00000000(&D, id) returns a pointer to a Vec3i source record
 * for the given id; ids 0x11/0x12 (flag set) or 0x14/0x15 (flag
 * clear) fill 0x8F4/0x8F8, id 0x16 always fills 0x8FC.
 * Caps <80: 3-5 reloc calls + &D reloc + flag branch + per-slot
 * 3-word struct copies. INCLUDE_ASM remains build path. */
/* func_0000DF04 — EXACT 68/68 (Tri3i struct-assign cascade, same form as promoted E014/DDCC) */
void func_0000DF04(char *s0) {
    if (*(unsigned int*)(s0 + 0xA58) & 0x80) {
        **(Tri3i **)(s0 + 0x8F4) = *(Tri3i *)func_00000000(&D_00000000, 0x11);
        **(Tri3i **)(s0 + 0x8F8) = *(Tri3i *)func_00000000(&D_00000000, 0x12);
    } else {
        **(Tri3i **)(s0 + 0x8F4) = *(Tri3i *)func_00000000(&D_00000000, 0x14);
        **(Tri3i **)(s0 + 0x8F8) = *(Tri3i *)func_00000000(&D_00000000, 0x15);
    }
    **(Tri3i **)(s0 + 0x8FC) = *(Tri3i *)func_00000000(&D_00000000, 0x16);
}
/* VERIFIED in-tree 68/68 EXACT on clean rebuild (was 97.8%).
   Single lever: Tri3i struct-assign cascade (same form as promoted E014/DDCC siblings).
   First variant hit. Ready to promote (INCLUDE_ASM -> body) + land. */

/* func_0000E014 - verified structural decode (0x110, 68 insns).
 * NEAR-SIBLING of func_0000DF04 (same Vec3 fetch-and-store fan-out,
 * same s0 struct); variant: the first id of each pair comes from a
 * signed-half field on s0 instead of a constant.
 * Struct-typing reference: same layout as func_0000DF04 -
 * s0->0xA58 (2648) u32 flags bit 7 (0x80) selector; s0->0x8F4
 * (2292) / 0x8F8 (2296) / 0x8FC (2300) Vec3i dest pointers (3x int
 * @ +0/+4/+8). NEW fields here: s0->0x902 (2306) and s0->0x900
 * (2304) = s16 dynamic source-id selectors (the first fetch id is
 * read from the object, not hardcoded), with constant ids 8 / 5 /
 * 6 for the second-slot and final fetches.
 * Caps <80: 3-5 reloc calls + &D reloc + flag branch + lh
 * dynamic-id + per-slot 3-word struct copies. INCLUDE_ASM remains
 * build path. */
void func_0000E014(char *s0) {
    if (*(unsigned int*)(s0 + 0xA58) & 0x80) {
        **(Tri3i **)(s0 + 0x8F4) = *(Tri3i *)func_00000000(&D_00000000, *(short*)(s0 + 0x902));
        **(Tri3i **)(s0 + 0x8F8) = *(Tri3i *)func_00000000(&D_00000000, 8);
    } else {
        **(Tri3i **)(s0 + 0x8F4) = *(Tri3i *)func_00000000(&D_00000000, *(short*)(s0 + 0x900));
        **(Tri3i **)(s0 + 0x8F8) = *(Tri3i *)func_00000000(&D_00000000, 5);
    }
    **(Tri3i **)(s0 + 0x8FC) = *(Tri3i *)func_00000000(&D_00000000, 6);
}

/* func_0000E124 - verified structural decode (0x14C, 83 insns,
 * left/right mirror-flip toggle).
 * Struct-typing reference: s0->0xA58 (2648) u32 flag word - bit 7
 * (0x80) = facing/mirror state (toggled here), bit 3 (0x8) selects
 * the notify-callback arg form, bit 5 (0x20) gates a 180.0deg
 * re-orient. s0->0xA38 (2616) f32 cleared (a timer/accum). s0->0x978
 * (2424) f32 = an axis/spin value, sign-flipped on mirror. s0->0x844
 * (2116) = child A object, s0->0x864 (2148) = child B; both carry a
 * transform at +0x30 (child->0x30 f32 = x scale, negated to mirror;
 * child+0x30 = the Vec3 transform passed to func_00000000 reflect/
 * rotate helpers). Constants: -1.0/1.0 reflection vector, 0x43340000
 * = 180.0f rotation. This is the snowboarder/board left<->right
 * direction flip. Caps <80: FP-heavy neg.s/mul.s + many lui+mtc1
 * f32 consts + beql branch-likely + flag-bit branches + reloc.
 * INCLUDE_ASM remains build path. */
/* EXACT (agent-c run 115). Mirror-flip toggle: callees from the TextReloc
 * (func_0000DDCC / func_0000E014, func_072E58 x2, func_072F1C, func_072FB0
 * which takes (ptr, ptr, float) so 180.0f goes out in a2); the flag word is
 * a plain ^= RMW re-read per test; the r-call sits inside the null check and
 * one pointer variable p carries the negated component and both call args;
 * up[] sits at the frame top with five unused words above r[]. */
extern void func_00000000_e124_r(char *, float *, float);
void func_0000E124(char *s0) {
    float up[3];
    int unused[5];
    float r[3];
    float *p;

    *(unsigned int *)(s0 + 0xA58) ^= 0x80;
    *(float *)(s0 + 0xA38) = 0.0f;
    if (*(unsigned int *)(s0 + 0xA58) & 0x8) {
        func_00000000();
    } else {
        func_00000000(s0);
    }
    *(float *)(s0 + 0x978) = -*(float *)(s0 + 0x978);
    up[0] = 0.0f;
    up[1] = 1.0f;
    up[2] = 0.0f;
    if (*(unsigned int *)(s0 + 0xA58) & 0x80) {
        p = (float *)(*(char **)(s0 + 0x844) + 0x30);
        *p = *p * -1.0f;
        if (*(char **)(s0 + 0x864) != 0) {
            p = (float *)(*(char **)(s0 + 0x864) + 0x30);
            func_00000000(p);
            p = (float *)(*(char **)(s0 + 0x864) + 0x30);
            r[0] = -1.0f;
            r[2] = 1.0f;
            r[1] = 1.0f;
            func_00000000(p, r);
        }
    } else {
        p = (float *)(*(char **)(s0 + 0x844) + 0x30);
        *p = *p * -1.0f;
        if (*(char **)(s0 + 0x864) != 0) { p = (float *)(*(char **)(s0 + 0x864) + 0x30); func_00000000(p); }
    }
    if (*(unsigned int *)(s0 + 0xA58) & 0x20) {
        if (*(char **)(s0 + 0x864) != 0) func_00000000_e124_r(*(char **)(s0 + 0x864) + 0x30, up, 180.0f);
    }
}

/* func_0000E270: ratio wrapper. EXACT via the IDO 7.1 -O2 donor
 * bootup_uso_o2_E270.c (REPLACE_FUNC_BODY): the constant is the f32 literal pi
 * at bootup.uso RoData +0x998 (func_0000E270_rodata pin). The in-unit body is
 * the same C (keeps the .text size); the splice swaps in the donor body so the
 * .rodata reloc resolves to the RoData pool. Old "spill-slot pair swap" residue
 * was the named `ratio` local (one frame word too many). */
extern void aE270(void*, void*, float); /* float-proto alias; jal bakes RAW to 0 */
void func_0000E270(char *arg0, float arg1) {
    aE270(arg0 + 0xCC, arg0 + 0x3B0, 3.14159265f / arg1);
    aE270(arg0 + 0xF4, arg0 + 0x3B0, 3.14159265f / arg1);
}

/* func_0000E2D0 - verified structural decode (0x20C, 131 insns,
 * FP vector reflect/project math helper).
 *   void func_0000E2D0(Obj *a0, int a1, Vec3f *a2, Vec3f *a3) {
 *       // pass 1: input vector a2 vs axis a3
 *       Vec3f v = *a2;                            // sp+0x64
 *       f32 d = v.x*a3->x + v.y*a3->y + v.z*a3->z;// dot
 *       v = (Vec3f){ d*a3->x - v.x,
 *                    d*a3->y - v.y,
 *                    d*a3->z - v.z };             // d*n - v
 *       // pass 2: object's own vector a0->0x3BC vs same axis a3
 *       Vec3f w = *(Vec3f*)((char*)a0 + 0x3BC);   // sp+0x58
 *       d = w.x*a3->x + w.y*a3->y + w.z*a3->z;
 *       w = (Vec3f){ d*a3->x - w.x,
 *                    d*a3->y - w.y,
 *                    d*a3->z - w.z };
 *       // squared lengths of v / w, then a reloc (sqrt/normalize):
 *       lv = v.x*v.x + v.y*v.y + v.z*v.z;
 *       lw = w.x*w.x + w.y*w.y + w.z*w.z;
 *       func_00000000(...);                       // sqrtf/helper
 *       ... (uses func_0000098C datum) ...
 *   }
 * Struct-typing reference: a2 = input Vec3f (xyz @ +0/+4/+8), a3 =
 * axis/normal Vec3f, a0->0x3BC (956) = the object's stored Vec3f
 * (0x3BC/0x3C0/0x3C4). The per-vector transform is
 * `proj = (v . n); out = proj*n - v` (component-wise) - a plane
 * projection/reflection against axis a3 - applied to BOTH the
 * passed vector and the object's vector, then their squared
 * magnitudes feed a reloc helper (sqrtf / normalize, also reads
 * a func_0000098C+off datum). a1 = an unused-here index/flag.
 * NOTE (2026-07-10, agent-f, CONFIRMED via USO reloc table): the
 * `func_0000098C + 0x14` datum this fn loads (ldc1 @ pool offset 0x9A0) is
 * **f64 0.1**, read from the module's RoData literal pool (data @ ROM
 * 0xD9FE44) — a real rodata constant, NOT code / not a "folded pool" needing
 * re-extract. func_0000098C is genuine code; the `+ N` form just reproduces
 * the ROM placeholder bytes. See the RESOLVED block in
 * docs/N64_FORENSICS.md#bootup-uso-fp-literal-pool-folded-into-func-0000098C.
 * Caps <80: very FP-heavy (dozens of mul.s/add.s/sub.s, two
 * dot-products, two reflections, two |.|^2) + func_00000000 reloc
 * + spilled f-temps + the file-split needed to fold the pool load. Full body
 * INCLUDE_ASM-preserved (.s = source of truth). INCLUDE_ASM (no episode). */
#ifdef NON_MATCHING
#ifndef FW
#define FW(p, o) (*(int *)((char *)(p) + (o)))
#endif
/* PASS-2 2026-10-16 (agent-c, 34.24 -> 96.75): source rewrite. The two
 * reflected vectors are ONE `Vec3 v[2]` (v[0] at sp+0x58 goes to the calls, so
 * the whole array stays in memory: target swc1 then lwc1 of every component;
 * two separate Vec3 locals get promoted to registers). m2c's calls went
 * through fn-pointer casts (jalr) with invented float args: they are a
 * f32(f32) sqrt alias, normalize(&v[0], arg3) and (arg0, arg1, &v[0]); arg4 is
 * a stack f32 (1.0f + arg4), not an int. Sums written z + (y + x) (x,y first).
 * Residual: d and the arg3->x load colour f2/f0 swapped, the dot/scale tail
 * colours, frame 0x50 vs 0x70. */
/* Run 123 (agent-c, raw 55 -> 19 words): the f64 at RoData +0x9A0 is the literal
 * 0.1 (exact needs a donor, func_0000E2D0_rodata = 0x9A0 - 0); the frame is 112:
 * one unused word between s and dot and seven below dot (dot's home at sp+68);
 * dot and s are ONE variable (f0 across the call, spilled to its home); the
 * normalise call takes only &v[0] but arg3 stays live in a3 (spilled at 124),
 * reproduced by `if (arg3) {}`; `dot *= k` keeps dot as the first mul operand.
 * Left: the dot sum is emitted z-first (cfe canonical order; target (x+y)+z,
 * an NCC-style source order) and the 1.0f + arg4 load/mtc1 schedule + one nop;
 * NCC compiles of this body are much worse (frame 80, no pads). */
extern f64 bu_e2d0_k9a0;  /* rodata f64 (target func_0000098C + 0x14) */
void func_0000E2D0(char *arg0, s32 arg1, Vec3 *arg2, Vec3 *arg3, f32 arg4) {
    Vec3 v[2];
    f32 d;
    f32 lim;
    f32 s;
    int X;
    f32 dot;
    int pad[7];

    v[1].x = arg2->x;
    v[1].y = arg2->y;
    v[1].z = arg2->z;
    d = v[1].z * arg3->z + (v[1].y * arg3->y + v[1].x * arg3->x);
    v[1].x = d * arg3->x - v[1].x;
    v[1].y = d * arg3->y - v[1].y;
    v[1].z = d * arg3->z - v[1].z; if (1) {}
    v[0].x = *(f32 *)(arg0 + 0x3BC);
    v[0].y = *(f32 *)(arg0 + 0x3C0);
    v[0].z = *(f32 *)(arg0 + 0x3C4);
    d = v[0].z * arg3->z + (v[0].y * arg3->y + v[0].x * arg3->x);
    v[0].x = d * arg3->x - v[0].x;
    v[0].y = d * arg3->y - v[0].y;
    v[0].z = d * arg3->z - v[0].z;
    if (0.1 < (f64)func_00000000_4948_ff(v[0].z * v[0].z + (v[0].y * v[0].y + v[0].x * v[0].x))) {
        dot = v[1].x * v[0].x + v[1].y * v[0].y + v[0].z * v[1].z;
        func_00000000(&v[0]); if (arg3) {} if (arg4) {} /* run 131: arg4 kept live -> the 1.0f + arg4 load/mtc1/nop schedule, length 131 */
        lim = *(f32 *)(arg0 + 0x9FC);
        dot *= *(f32 *)(arg0 + 0x9F8);
        if (dot < -lim) {
            dot = -lim;
        }
        if (lim < dot) {
            dot = lim;
        }
        dot *= 1.0f + arg4;
        v[0].x *= dot;
        v[0].y *= dot;
        v[0].z *= dot;
        func_00000000(arg0, arg1, &v[0]);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/bootup_uso", func_0000E2D0);
#endif

/* func_0000E4DC - verified structural decode (0xAC, 43 insns).
 * Per-frame update/teardown over a composite object a0 (= s2). Uses
 * the documented obj->0x28 vtable-dispatch idiom (PATTERNS.md
 * #feedback-1080-obj-0x28-vtable-dispatch), 0x4C/0x48 offset variant.
 * Struct-typing reference: a0 composite object; a0+0x540 / a0+0x108
 * sub-regions passed to reloc sub-init/deinit; a0+0x8C8 s16 array of
 * `n` elements (n produced by func_000089C0, per-element processed by
 * func_000089FC); a0->0x840 (2112) child A ptr, a0->0x804 (2052)
 * child B ptr; child->0x28 (40) vtable ptr with fn @0x4C (76) and
 * s16 base-adjust @0x48 (72). Caps <80: 2x reloc + 2x vtable jalr +
 * blezl branch-likely loop with per-iter reload. INCLUDE_ASM remains
 * build path (no episode; tautology-trap rule).
 * 2026-06-21 (agent-b sweep): 7->5 non-reloc diffs. Hoisting the loop
 * counter init (`i = 0;` BEFORE `p = a0+0x8C8;`) and dropping the dead
 * `if(a0){}` / `n = n;` no-ops matches the target's counter-before-base
 * schedule (`move s0,zero` ahead of `addiu s1,s2,0x8C8`). RESIDUAL 5
 * diffs are ALL the frame-layout delta: target reserves -0x38, build
 * -0x40 (8 extra bytes of IDO spill-pad between the saved-reg block and
 * the `&n` slot at sp+0x34 vs sp+0x3C). Pure allocator frame-reservation
 * (the `&n` escapes to func_000089C0); not C-reachable. Stays NM. */
void func_0000E4DC(char *a0) {
  int n;
  short *p;
  int i;
  int *v;
  func_00000000(a0 + 0x540);
  func_000089C0(&n);
  i = 0;
  p = (short *) (a0 + 0x8C8);
  for (; i < n; i++)
  {
    func_000089FC(p);
    p += 1;
  }

 do { v = *((int **) (*((char **) (a0 + 0x840)) + 0x28)); (*((void (**)(char *)) (((char *) v) + 0x4C)))((char *) (((int) *((char **) (a0 + 0x840))) + ((int) ((short) (*((short *) (((char *) v) + 0x48))))))); } while (0);
  v = *((int **) (*((char **) (a0 + 0x804)) + 0x28));
  (*((void (**)(char *)) (((char *) v) + 0x4C)))((char *) (((int) *((char **) (a0 + 0x804))) + ((int) ((short) (*((short *) (((char *) v) + 0x48)))))));
  func_00000000(a0 + 0x108);
  if (v)
  {
  }
}

void func_0000E588(char *a0) {
    int scratch;
    func_000089C0(&scratch);
    func_00008A40((int*)(a0 + 0x10));
}

void func_0000E5B8(char *a0) {
    int scratch;
    func_000089C0(&scratch);
    func_00008A7C((int*)(a0 + 0x10));
}

float func_0000E5E8(char *a0) { return *(float*)(a0 + 0x198); }

void func_0000E5F0(char *a0) {
    func_00000000(a0 + 0x108);
}

void func_0000E610(char *a0) {
    func_00000000(a0 + 0x108);
}

void func_0000E630(char *a0) {
    int scratch;
    func_000089C0(&scratch);
    func_00008AEC((int*)(a0 + 0x10));
}

void func_0000E660(char *a0) {
    int scratch;
    func_000089C0(&scratch);
    func_000089C0((int*)(a0 + 0x10));
}

/* func_0000E690: byte-identical mirror of func_00000C10 / func_000046EC /
 * func_00007B08 — all byte-exact twins of donor eddproc_uso_func_000003BC.
 * BYTE-EXACT via the donor's clean C form (p2/head/p1 decl order + char*-cast
 * field access + p1[0x40/4] re-read). Was once force-matched via INSN_PATCH
 * (banned 2026-05-23); this is the genuine C match. */
void *func_0000E690(int *arg0) {
    int *p2;
    int *head;
    int *p1;
    p1 = (int*)func_00000000(0x40);
    if (p1 != 0) {
        func_00000000(p1);
        *(int*)((char*)p1 + 0x28) = (int)&D_00000000;
        *(int*)((char*)p1 + 0x3C) = 0;
    }
    p2 = p1;
    p1 = arg0;
    head = (int*)p1[0x40 / 4];
    if ((int*)p1[0x40 / 4] != 0) {
        func_00000000((char*)p2 + 0x10, head);
        if (*(int*)((char*)head + 0x14) != 0) {
            *(int*)((char*)head + 0x4) = 1;
        }
        *(int*)((char*)head + 0x14) = (int)p2;
    }
    return p2;
}

void func_0000E720(char *a0) {
    func_00000000(a0 + 0x2C);
}

/* func_0000E740 - verified structural decode (0xB4, 48 insns,
 * 2D strided->packed halfword blit / unswizzle).
 * Struct-typing reference: global root *(func_0000023C+4); root->0x28
 * (40) vtable ptr with fn @0x64 (100) + s16 base-adjust @0x60 (96)
 * returning a buffer base; src region at base+0x9140 laid out as
 * 0x20-byte cells with 4 u16 lanes at cell offsets 0/8/0x10/0x18;
 * dst = a0->0x1C (28) packed u16 output. Grid 32x(16*4)=2048 u16,
 * row stride +0xA80 in src. The obj-0x28 vtable-dispatch idiom
 * (0x64/0x60 variant). Caps <80: vtable jalr + func_0000023C+4
 * reloc data ref + nested strided copy loop (bne loop pair).
 * INCLUDE_ASM remains build path. */
/* EXACT via an IDO 7.1 NCC (C++) donor: src/bootup_uso/bootup_uso_ncc_E740.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder (agent-c run 116). */
void func_0000E740(char *a0, int a1) {
    volatile char *r = a0;
}

/* func_0000E800 - verified structural decode (0xBC, 47 insns,
 * config-parse + init routine).
 * Struct-typing reference: D_00008D34 / D_00008D48 / D_00008D5C =
 * three config-key data (string/descriptor) parsed by the reloc
 * helper into a (x,y,z) triple on the stack, then consumed in
 * (x,z,y) order. func_00000008+0x20 = global slot set to &D
 * (registration). Tail uses the engine-wide obj-0x28 vtable-dispatch
 * idiom, 0x5C/0x58 variant (root *(&D) ->0x28 -> {fn@0x5C,
 * s16@0x58}). Caps <80: 7x reloc + 5x &D reloc + func_00000008+0x20
 * cross-symbol data store + vtable jalr.
 * INCLUDE_ASM remains build path. */
extern char D_00008D34, D_00008D48, D_00008D5C;
/* EXACT via an IDO 7.1 NCC (C++) donor: src/bootup_uso/bootup_uso_ncc_E800.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder (agent-c run 116). */
void func_0000E800(void) {
    volatile int r = 0;
}

void func_0000E8BC(int *dst) {
    int buf[2];
    func_00000000(&D_00000000, buf, 4);
    *dst = buf[0];
}

void func_0000E8F8(float *dst) {
    float buf[2];
    func_00000000(&D_00000000, buf, 4);
    *dst = buf[0];
}

extern void func_0000E934();
void func_0000E934(Vec3 *dst) {
    int pad_top[1];
    Tri3i raw;
    int pad_mid[2];
    Tri3i tmp;
    int pad_bot[2];
    func_00000000(&D_00000000, &raw, 12);
    tmp = raw;
    dst->x = *(float*)&tmp.a;
    dst->y = *(float*)&tmp.b;
    dst->z = *(float*)&tmp.c;
}

void func_0000E9A4(Quad4 *dst) {
    Quad4 buf;
    func_00000000(&D_00000000, &buf, 16);
    *dst = buf;
}

/* func_0000E9FC: 12-insn 1-call wrapper. 2-diff NM cap (NOT matched — the top
 * comment previously claimed "matched via two levers", but the second was an
 * INSN_PATCH, REMOVED 2026-05-23 as match-faking). One genuine lever DOES apply:
 *  - `extern char D_func_00000008_data;` (DATA-decl alias of func_00000008)
 *    enables splat-fold on `lui at, %hi(func_00000008); sw t6, 0x20(at)`.
 *    Function-decl alias would force a separate %hi+%lo pair (14 insns).
 *    Recipe: docs/IDO_CODEGEN.md feedback-ido-extern-char-vs-extern-fn-folds-lo-offset.
 * Residual 2 diffs are the prologue `sw ra,0x14(sp)` ↔ `lui a0,%hi(D)`
 * scheduling swap: IDO -O2 emits sw-then-lui, target has lui-then-sw. Pure
 * instruction-scheduling cap, unreachable from C (the INSN_PATCH that used to
 * bridge it is banned). Identical cap on sibling func_00006204 in the same .o
 * (77.78%); both honest NM, default INCLUDE_ASM exact. */
extern char D_func_0000E9FC_arg1;
extern char D_func_0000E9FC_arg2;
extern char D_func_00000008_data;
#ifdef NON_MATCHING
/* 3→2 diffs: store offset is 0x20 (the prior comment claiming a 0x20->0x28
 * "correction" was BACKWARDS — the target stores at func_00000008+0x20
 * (`sw t6,32(at)`); 0x28 was a regression that added a 3rd diff). Residual 2
 * diffs are a prologue lui-$4 / sw-$31 SCHEDULING swap — target hoists the
 * call-arg lui before the ra-save; IDO ugen with our C emits ra-save first.
 * Tried local-arg variant; no change. Prologue instruction-scheduling cap. */
void func_0000E9FC(void) {
    func_00000000(&D_func_0000E9FC_arg1);
    *(int*)(&D_func_00000008_data + 0x20) = (int)&D_func_0000E9FC_arg2;
}
#else
INCLUDE_ASM("asm/nonmatchings/bootup_uso", func_0000E9FC);
#endif

extern char D_00008D70;
extern char D_00008D7C;
extern char D_00008D8C;
extern char D_00008D9C;
extern char D_00000000_a;
extern char D_00000000_b;
void func_0000EA2C(char *arg0) {
    char pad[4];
    int saved;

    (void)pad;
    *(int *)(arg0 + 0xC) = (int)&D_00008D70;
    func_00000000(arg0, &D_00008D7C);
    saved = func_00000000(0);
    *(int *)&D_00000000 = func_00000000(0, &D_00008D8C, 0x20, 0xC);
    func_00000000(saved);
    /* Distinct zero-address aliases keep IDO from caching the base in $s0. */
    func_00000000(&D_00000000_a, &D_00008D9C);
    func_00000000(&D_00000000_b, arg0);
    if (func_00000000(&D_00000000) != 0) {
        func_00000000(&D_00000000_a, 1, 0);
    } else {
        func_00000000(&D_00000000_b, 0, 0);
    }
}

extern int D_00000190;
void func_0000EAF8(int a0) {
    func_00000000(D_00000190, 3, 0);
}

void func_0000EB28(char *a0) {
    int scratch;
    func_0000E8BC(&scratch);
    func_0000E8F8((int*)(a0 + 0x10));
}

void func_0000EB58(char *a0) {
    int scratch;
    func_0000E8BC(&scratch);
    func_0000E934((int*)(a0 + 0x10));
}

void func_0000EB88(char *a0) {
    int scratch;
    func_0000E8BC(&scratch);
    func_0000E9A4((int*)(a0 + 0x10));
}

void func_0000EBB8(char *a0) {
    int scratch;
    func_0000E8BC(&scratch);
    func_0000E8BC((int*)(a0 + 0x10));
}

/* func_0000EBE8: 36-insn alloc-or-fail + linked-list-prepend wrapper.
 * EXACT 2026-07-15 (84.5% -> 36/36, ROM byte-exact with C as build path).
 * Three coupled levers cracked the old "0x20-frame $a2" residual:
 *  (1) `ret = p; p += 4;` — the target's `addiu a0,a0,0x10` is a DESTRUCTIVE
 *      in-place advance of p; the second def blocks copy-propagation so
 *      `ret = p` survives as `or v1,a0` and p's web colors $a0 (both defs
 *      feed arg0 uses). Passing `p` (post-advance) to the second call keeps
 *      the addiu in p's own register.
 *  (2) `if (caller_a0[0x10] != 0) { target = caller_a0[0x10]; ... }` —
 *      condition tests the raw expression, assignment CSE'd inside the
 *      branch: load lands in $v0, plain beqz (not beqzl-epilogue-steal),
 *      copy `or a1,v0` fills the delay slot.
 *  (3) decl order ret,target,p — spill slots assigned in REVERSE decl order
 *      (p->0x1C, target->0x20, ret->0x24).
 * Trailing 2 alignment nops (0xEC78/EC7C) restored via all-zero
 * SUFFIX_BYTES_FORCE in the Makefile (genuine pad, not instructions). */
int *func_0000EBE8(int *caller_a0) {
    int *ret;
    int *target;
    int *p;
    if ((p = (int*)func_00000000(0x40)) != 0) {
        func_00000000(p);
        p[0x28/4] = (int)&D_00000000;
        p[0x3C/4] = 0;
    }
    ret = p;
    p += 0x10/4;
    if (caller_a0[0x40/4] != 0) {
        target = (int*)caller_a0[0x40/4];
        func_00000000(p, target);
        if (target[0x14/4] != 0) {
            target[0x4/4] = 1;
        }
        target[0x14/4] = (int)ret;
    }
    return ret;
}

void func_0000EC80(int *dst) {
    int buf[2];
    func_00000000(&D_00000000, buf, 4);
    *dst = buf[0];
}

void func_0000ECBC(float *dst) {
    float buf[2];
    func_00000000(&D_00000000, buf, 4);
    *dst = buf[0];
}

extern void func_0000ECF8();
void func_0000ECF8(Vec3 *dst) {
    int pad_top[1];
    Tri3i raw;
    int pad_mid[2];
    Tri3i tmp;
    int pad_bot[2];
    func_00000000(&D_00000000, &raw, 12);
    tmp = raw;
    dst->x = *(float*)&tmp.a;
    dst->y = *(float*)&tmp.b;
    dst->z = *(float*)&tmp.c;
}

void func_0000ED68(Quad4 *dst) {
    Quad4 buf;
    func_00000000(&D_00000000, &buf, 16);
    *dst = buf;
}

void func_0000EDC0(int a0, int a1) {}

void func_0000EDCC(char *a0) {
    int scratch;
    func_0000EC80(&scratch);
    func_0000ECBC((int*)(a0 + 0x10));
}

void func_0000EDFC(char *a0) {
    int scratch;
    func_0000EC80(&scratch);
    func_0000ECF8((int*)(a0 + 0x10));
}

void func_0000EE2C(char *a0) {
    int scratch;
    func_0000EC80(&scratch);
    func_0000ED68((int*)(a0 + 0x10));
}

void func_0000EE5C(char *a0) {
    int scratch;
    func_0000EC80(&scratch);
    func_0000EC80((int*)(a0 + 0x10));
}

/* func_0000EE8C: alloc-cascade constructor, sibling of func_0000EBE8.
 * BYTE-EXACT + LANDED (2026-06-21). The body was byte-exact once `ret` was
 * declared FIRST (IDO assigns it the highest local slot 0x24, matching target);
 * the only residual was the target's single trailing 8-byte-alignment nop at
 * 0xEF1C (so func_0000EF20 sits at 0xEF20). Resolved by appending that one
 * all-zero (nop) word via `SUFFIX_BYTES_FORCE := func_0000EE8C=0x00000000` in
 * the Makefile — the SAME legitimate alignment-pad mechanism already used for
 * gui_func_0000161C / h2hproc fns. The earlier "cannot land" note assumed a
 * GLOBAL_ASM dummy (8-byte -O2 floor → +4 overshoot); SUFFIX_BYTES_FORCE appends
 * exactly one word, so func_0000EF20 lands at 0xEF20 and the ROM stays exact.
 * This works mid-segment (with a following symbol), not just at segment-end. */
void *func_0000EE8C(void *caller_a0) {
  void *ret;
  void *target;
  register void *p;
  p = (void *) func_00000000(0x40);
  if (p != 0)
  {
    func_00000000(p);
    *((int *) (((char *) p) + 0x28)) = (int) (&D_00000000);
    *((int *) (((char *) p) + 0x3C)) = 0;
  }
  ret = p;
  p = ((char *) p) + 0x10;
  target = *((void **) (((char *) caller_a0) + 0x40));
  if ((*((void **) (((char *) caller_a0) + 0x40))) != 0)
  {
    func_00000000(p, target);
    if ((*((int *) (((char *) target) + 0x14))) != 0)
    {
      *((int *) (((char *) target) + 0x4)) = 1;
    }
    *((int *) (((char *) target) + 0x14)) = (int) ret;
  }
  return ret;
}

void func_0000EF20(int *dst) {
    int buf[2];
    func_00000000(&D_00000000, buf, 4);
    *dst = buf[0];
}

void func_0000EF5C(float *dst) {
    float buf[2];
    func_00000000(&D_00000000, buf, 4);
    *dst = buf[0];
}

extern void func_0000EF98();
void func_0000EF98(Vec3 *dst) {
    int pad_top[1];
    Tri3i raw;
    int pad_mid[2];
    Tri3i tmp;
    int pad_bot[2];
    func_00000000(&D_00000000, &raw, 12);
    tmp = raw;
    dst->x = *(float*)&tmp.a;
    dst->y = *(float*)&tmp.b;
    dst->z = *(float*)&tmp.c;
}

void func_0000F008(Quad4 *dst) {
    Quad4 buf;
    func_00000000(&D_00000000, &buf, 16);
    *dst = buf;
}

void func_0000F060(char *a0) {
    int scratch;
    func_0000EF20(&scratch);
    func_0000EF5C((int*)(a0 + 0x10));
}

void func_0000F090(char *a0) {
    int scratch;
    func_0000EF20(&scratch);
    func_0000EF98((int*)(a0 + 0x10));
}

void func_0000F0C0(char *a0) {
    int scratch;
    func_0000EF20(&scratch);
    func_0000F008((int*)(a0 + 0x10));
}

void func_0000F0F0(char *a0) {
    int scratch;
    func_0000EF20(&scratch);
    func_0000EF20((int*)(a0 + 0x10));
}

void func_0000F120(int *dst) {
    int buf[2];
    func_00000000(&D_00000000, buf, 4);
    *dst = buf[0];
}

void func_0000F15C(Quad4 *dst) {
    Quad4 buf;
    func_00000000(&D_00000000, &buf, 16);
    *dst = buf;
}

/* func_0000F1B4: composite int+Quad4 reader (cross-USO template).
 * Body is 12 insns (0x30); FUNC st_size in expected is 60 (0x3C) because
 * 3 trailing alignment nops at 0xF1E4-0xF1EC are PART of the symbol's reach
 * (next function func_0000F1F0 is 16-byte aligned).
 *
 * SUFFIX_BYTES recipe (Makefile) appends 3 nops post-cc, growing the
 * symbol from 0x30 to 0x3C in place. Drops the prior NM wrap.
 * Verified 2026-05-05: build/.o byte-correct via byte_verify (st_size=0x3C
 * matches expected; trailing nops match). Per
 * feedback_suffix_bytes_unblocks_4byte_stolen_prologue.md (multi-insn
 * extension). */
void func_0000F1B4(char *a0) {
    int scratch;
    func_0000F120(&scratch);
    func_0000F15C((Quad4*)(a0 + 0x10));
}

/* func_0000F1F0, F23C, F288, F2EC moved to bootup_uso_o0_F1F0.c (-O0 file
 * split, see Makefile + tenshoe.ld). The 4 -O0 accessor templates promote
 * to byte-correct in the new file.
 * func_0000F390, func_0000F3D4, func_0000F404 moved to bootup_uso_o0_F390.c
 * (-O0 file split, see Makefile + tenshoe.ld).
 * func_0000F434..func_0000F6C4 moved to bootup_uso_F434.c (layout shim). */
