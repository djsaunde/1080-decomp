#include "common.h"
#include "game/game_uso_data.h"
#include "game/snowai.h"
#include "game/rideranim.h"
#include "game/camera.h"
#include "game/course.h"
#include "game/sun.h"
#include "game/input.h"
#include "gdl/item.h"
#include "gdl/gfxlib.h"
#include "uso_call.h"

/* typed views of an untyped object pointer (same base + offset access as the raw form) */
#define MN(p) ((CamMNode *)(p))              /* a libgdl matrix node: local[3] / world[3] = position */
#define CAMC(p) ((SnowChaseActorCam *)(p))
#define LOOK(p) ((SnowChaseActorLook *)(p))
#define AI(p) ((SnowAI *)(p))

/* P0 RESOLVED 2026-06-10: game_uso_block1 byte-verified vs
 * assets/game_uso_block_1.bin (exact length, only reloc-class lui/jal
 * field diffs). The +0x90 damage was: +4 at 0x3F4 (1-word pad sidecar,
 * now folded into camKey_init_000003F8.s head), +4 at 0xC0EC (1-word zero pad
 * between two matched C fns -- kept as sidecar, the Makefile recipe
 * drops the asserted-zero placeholder leftover at 0xC0F0), +0x80 at
 * 0xD5BC (func_0000D458.s over-extended into duplicated copies of
 * D5BC/D5DC/D5F8/D634 -- trimmed), an out-of-order snd_soundObjSetReverb
 * definition (moved), and +8 derivative tail alignment. Gate it with
 * `make verify-blocks` after any change to this unit. See
 * docs/MATCHING_WORKFLOW.md, asm-processor 1-word-pad defect. */
extern char *game_uso_alias();

/* File-top canonical decls (hoisted out of NM-wrap blocks
 * so DNM build doesn't see redecls — see
 * feedback_game_uso_dnm_typedef_inside_ifdef.md). */
extern int gl_func_00000000();
extern char D_00000000;
extern char import_80020110;
extern int import_8006EF48;
extern int D_game_11564_flag;
extern int D_game_11564_table;
typedef struct { float x, y, z; } Vec3;
typedef struct { int a, b, c; } Tri3i;
typedef struct { int a, b, c, d; } Quad4;
/* Pair2 is defined in game/rideranim.h */

#ifdef NON_MATCHING
/* Macro definitions for NM-wrap bodies. Auto-managed by /struct-name-tick.
 * Default build never sees these — wrap bodies aren't compiled.
 * GAME_D_E10: first int of the (0xE10,0xE14) global value pair passed
 * together (as args) to gl_func_00000000 across several wraps. game_uso D+0xE10.
 * GAME_D_F18/F1C: a second (0xF18,0xF1C) int pair passed together to
 * func_00000000 as `(a0, F18, F1C, 2, F18, F1C)` (the pair repeated) in the
 * func_00011258/112E0 sibling wraps. Positional. game_uso D+0xF18/0xF1C.
 */
#define GAME_D_E10 (*(int*)((char*)&D_00000000 + 0xE10))
#define GAME_D_E14 (*(int*)((char*)&D_00000000 + 0xE14))
#define GAME_D_F18 (*(int*)((char*)&D_00000000 + 0xF18))
#define GAME_D_F1C (*(int*)((char*)&D_00000000 + 0xF1C))
#endif

/* game_uso_func_00000000: uniform cubic B-spline basis (four weights for t
 * into out[0..3]). EXACT 2026-10-22 (agent-g run 120) via the IDO 7.1 NCC
 * donor src/game_uso/game_uso_ncc_0000.cpp (REPLACE_FUNC_BODY, both trees):
 * the 1/6 is a float literal there, so it is a reg-alloc-prep constant
 * candidate coloured after omt/t2/t3 -- the old cfe body's `D` local (read
 * through D_00000000) took $f0 first and the K&R `int ti` reinterpret was
 * only an ABI workaround. This placeholder exists to re-emit the unit's
 * .rodata item 0 (the 1/6 word, 0x3E2AAAAB) ahead of the C jumptables of
 * cam_pickResultView_00000940; its text and relocs are replaced by the donor's,
 * whose .rodata reloc becomes game_uso_func_00000000_rodata (= 0). */
void game_uso_func_00000000(out, t)
    float *out;
    float t;
{
    out[0] = 0.16666667f;
}

/* 4-element dot product. CRACKED 2026-06-11 to 100.0 via fully right-nested
 * grouping `a[3]*b[3] + (a[2]*b[2] + (a[1]*b[1] + b[0]*a[0]))` (exhaustive
 * 1920-variant enumeration; the "cap" below was a left-assoc-family-only
 * conclusion; the left-assoc forms all leave the final add.s operands swapped). */
float math_dot4_000000A0(float *a, float *b) {
    /* 2026-06-11 rules-sweep CRACK: right-nested grouping flips the final
     * add.s to target order (cfe ufad ichain = ucode shape, rule-1 cousin);
     * found via exhaustive 1920-variant enumeration (perm x mul-swap x
     * parenthesization). The left-assoc default was the only family probed
     * before. */
    return a[3]*b[3] + (a[2]*b[2] + (a[1]*b[1] + b[0]*a[0]));
}


/* Cubic B-spline weighted point evaluator (61 insns, FPU-only).
 * Input:  out (Vec3*),  ctrl (Vec3*[4]: 4 control point pointers),
 *         weights (float[4]: B0..B3 from game_uso_func_00000000)
 * Output: out[i] = sum over k=0..3 of (*ctrl[k])[i] * weights[k]
 *
 * Used with game_uso_func_00000000 (basis evaluator) to compute a point
 * on a uniform cubic B-spline curve in 3D — camera path / track / skater
 * limb interpolation.
 *
 * MATCHED via the FP-reduction commutative-spelling lever (same shape as the
 * sibling dot-product math_dot4_000000A0): each 4-term dot written
 * last-term-first with right-nesting and the innermost pair source-reversed
 * (`c3*w3 + (c2*w2 + (c1*w1 + w0*c0))`). This restores IDO's RTL eval so loads
 * and add.s all land in target order. */
void math_blendVec3x4_000000E0(Vec3 *out, Vec3 **ctrl, float *weights) {
    out->x = ctrl[3]->x * weights[3] + (ctrl[2]->x * weights[2]
           + (ctrl[1]->x * weights[1] + weights[0] * ctrl[0]->x));
    out->y = ctrl[3]->y * weights[3] + (ctrl[2]->y * weights[2]
           + (ctrl[1]->y * weights[1] + weights[0] * ctrl[0]->y));
    out->z = ctrl[3]->z * weights[3] + (ctrl[2]->z * weights[2]
           + (ctrl[1]->z * weights[1] + weights[0] * ctrl[0]->z));
}

static void stream_readVec3_000001D4(Vec3 *dst) {
    int pad_top[1];
    Tri3i raw;
    int pad_mid[2];
    Tri3i tmp;
    int pad_bot[2];
    CALL(fs_gdlReadFile)(&D_00000000, &raw, 12);
#ifdef PORT
    PORT_BE_SWAP32S(&raw, 12);
#endif
    tmp = raw;
    dst->x = *(float*)&tmp.a;
    dst->y = *(float*)&tmp.b;
    dst->z = *(float*)&tmp.c;
}

static void stream_readF32_00000244(float *dst) {
    float buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

static void stream_readS32_00000280(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

static void stream_readQuad_000002BC(Quad4 *dst) {
    Quad4 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 16);
#ifdef PORT
    PORT_BE_SWAP32S(&buf, 16);
#endif
    *dst = buf;
}

static void stream_read8_00000314(Pair2 *dst) {
    Pair2 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 8);
#ifdef PORT
    PORT_BE_SWAP32S(&buf, 8);
#endif
    *dst = buf;
}

/* Int-reader template, BYTE-EXACT 2026-05-31 (.text identical to expected/.o;
 * residual fuzzy<100 is the known uso_raw_word-vs-reloc artifact — expected/.o
 * is missing the LO16 on the &D addiu because game-uso-symbolize-s.py named the
 * %hi lui but not its paired %lo addiu, same class as eddproc_uso_func_00000000).
 *
 * NOT A REGALLOC CAP — the old "t6/t7/t8 vs t7/t9/t6, split-piece coloring,
 * permuter-only" note was WRONG. The target's post-call sequence
 *   addiu t7,sp,0x18 ; lw t9,0(t7) ; lw t6,0x20(sp) ; sw t9,0(t6)
 * is a single-WORD STRUCT COPY (`*dst = *(One1*)buf`), not a scalar read.
 * A scalar `*dst = buf[0]` emits the DIRECT-index form (`lw t6,0x18(sp)`, like
 * eddproc_uso_func_00000000). The struct-copy form makes IDO re-materialize the
 * source address into a register (t7) and walk it with offset-0 loads — which is
 * exactly the target. The `int buf[2]` (8 bytes) keeps the buffer at sp+0x18
 * (frame 0x20); copying only the first word out as a 1-int struct yields the
 * one-word t7/t9/t6 sequence. (Sibling 039C below is the same struct-copy form,
 * 4 words.) See docs/IDO_CODEGEN.md struct-copy-recompute-vs-scalar-index. */
typedef struct { int a; } One1;
static void stream_readWord_0000035C(One1 *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = *(One1*)buf;
}

static void stream_readQuad_0000039C(Quad4 *dst) {
    Quad4 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 16);
#ifdef PORT
    PORT_BE_SWAP32S(&buf, 16);
#endif
    *dst = buf;
}
/* Historical 003F8 name: true entry is 003F4, including the hoisted mtc1.
 * Keep the asm label before that instruction so baseline regeneration uses
 * the same complete function boundary as the compiled C. */

/* camKey_init: the CamKey defaults (include/game/camera.h): look (0, 0, 0), eye (0, 0, -1000),
 * up (0, 1, 0), fov 85, frames 15, rot (0, 0, 0).
 *
 * EXACT, 78 words. The old caller-supplied-f0 diagnosis was wrong: the
 * module exports 0x3F4 (Sym[56], call relocation at text+0x788), not 0x3F8.
 * IDO schedules mtc1 zero,f0 before the stack prologue; it is the first
 * instruction of this function. The .s formerly labelled it as an orphan,
 * so a pure-ASM baseline refresh spuriously changed 100% to 98.70%.
 * Boundary corrected 2026-09-09; no instruction bytes changed.
 * See docs/IDO_CODEGEN.md#camera-hoisted-zero-baseline-3f8. */
void camKey_init_000003F8(CamKey *a0) {
    Vec3 zero, fwd, up, zero2;
    Vec3 pad0, pad1;
    Vec3 tmp;
    int padbot[2];

    zero.x = 0.0f;
    zero.y = 0.0f;
    zero.z = 0.0f;
    *(Tri3i *)&tmp = *(Tri3i *)&zero;
    a0->look.x = tmp.x;
    a0->look.y = tmp.y;
    a0->look.z = tmp.z;
    fwd.y = 0.0f;
    fwd.x = 0.0f;
    fwd.z = -1000.0f;
    *(Tri3i *)&tmp = *(Tri3i *)&fwd;
    a0->eye.x = tmp.x;
    a0->eye.y = tmp.y;
    a0->eye.z = tmp.z;
    up.z = 0.0f;
    up.x = 0.0f;
    up.y = 1.0f;
    *(Tri3i *)&tmp = *(Tri3i *)&up;
    a0->up.x = tmp.x;
    a0->up.y = tmp.y;
    a0->up.z = tmp.z;
    a0->fov = 85.0f;
    a0->frames = 15;
    zero2.z = 0.0f;
    zero2.y = 0.0f;
    zero2.x = 0.0f;
    *(Tri3i *)&tmp = *(Tri3i *)&zero2;
    a0->rot.x = tmp.x;
    a0->rot.y = tmp.y;
    a0->rot.z = tmp.z;
}

/* paths whose eye / look mode is 2 are relative: eye += aim (the chase aim point), then look += eye */
void cam_applyRelative_0000052C(int unused, Vec3 *look, Vec3 *eye, CamPath *path, Vec3 *aim) {
    if (path->eyeMode == 2) {
        eye->x += aim->x;
        eye->y += aim->y;
        eye->z += aim->z;
    }
    if (path->lookMode == 2) {
        look->x += eye->x;
        look->y += eye->y;
        look->z += eye->z;
    }
}

void camKey_read_000005B8(CamKey *a0) {
    stream_readVec3_000001D4((Vec3 *)&a0->look);
    stream_readVec3_000001D4((Vec3 *)&a0->eye);
    stream_readVec3_000001D4((Vec3 *)&a0->up);
    stream_readF32_00000244(&a0->fov);
    stream_readS32_00000280(&a0->frames);
    stream_readVec3_000001D4((Vec3 *)&a0->rot);
}

void *camPath_new_00000608(void *a0) {
    if (a0 == 0) {
        a0 = (void*)CALL(mem_new)(0x7C);
    }
    return a0;
}

/* defaults: both bodies on their chase components, no splines, no keys */
void camPath_init_00000634(CamPath *a0) {
    a0->lookMode = 1;
    a0->lookSpline = 0;
    a0->unk08 = 0;
    a0->unk0C = 0;
    a0->eyeMode = 1;
    a0->eyeSpline = 0;
    a0->unk18 = 0;
    a0->unk1C = 0;
    a0->upSpline = 0;
    a0->unk24 = 0;
    a0->fovMode = 0;
    a0->unk2C = 0;
    a0->rotMode = 0;
    a0->numKeys = 0;
}

/* State-check: returns 1 if any of the three sub-states is active.
 * MATCHED 2026-06-10 (44/44): the "$v0/$v1 not C-flippable" cap fell to
 * the EARLY-PSEUDO trick -- `int r = 0; r = <expr>; return r;`. The
 * dead initializer creates r's pseudo FIRST, so the result colors $v0
 * and the loads take $v1 (the target's order); the bare return-expr
 * form creates the load pseudo first (30 diffs). Same family as the
 * web-order inversion (h2hproc E04). */
/* any channel on a spline (look / eye keyed (mode 2 / 3) with spline 1, up spline, fov / rot mode 2): such a
 * path ends one key early (cam_updateView_00000B3C) */
int camPath_hasSpline_00000674(CamPath *a0) {
    int r = 0;
    r = ((a0->lookMode == 2 || a0->lookMode == 3) && a0->lookSpline == 1)
        || ((a0->eyeMode == 2 || a0->eyeMode == 3) && a0->eyeSpline == 1)
        || a0->upSpline == 1
        || a0->fovMode == 2
        || a0->rotMode == 2;
    return r;
}

/* MATCHED 2026-06-20. The stale ~90%-cap comment was wrong: cnt was already
 * in $v0 and the loop scheduling already matched. The ONLY residual diff was
 * the trailing tail store address: target `addu v1,s2,t7` (a0 base first) vs
 * built `addu v1,t7,s2` (index first). Writing the tail as true array
 * indexing (`keys[cnt + 1] = keys[cnt]`, run 528: same bytes as the old
 * `((int*)a0)[cnt+15]` form) makes IDO canonicalize the commutative addu with
 * the base register first → 0 diffs. keys[i] in the loop strength-reduces to
 * the old walking word pointer. */
void camPath_read_00000724(char *a0) {
  int i;
  int new_var;
  int cnt;
  stream_readQuad_000002BC((Quad4 *) &((CamPath *) a0)->lookMode);
  new_var = 0x30;
  stream_readQuad_000002BC((Quad4 *) &((CamPath *) a0)->eyeMode);
  stream_read8_00000314((Pair2 *) &((CamPath *) a0)->upSpline);
  stream_read8_00000314((Pair2 *) &((CamPath *) a0)->fovMode);
  stream_readWord_0000035C((int *) (a0 + new_var)); /* rotMode */
  stream_readS32_00000280(&((CamPath *) a0)->numKeys);
  cnt = ((CamPath *) a0)->numKeys;
  i = 1;
  if (cnt > 0)
  {
    do
    {
      int tmp = XCALL(game_uso_func_055750, mem_new)(0x38);
      if (1)
      {
        ((CamPath *) a0)->keys[i] = (CamKey *) tmp;
        XCALL(game_uso_func_000003F4, camKey_init_000003F8)(tmp);
      }
      camKey_read_000005B8(((CamPath *) a0)->keys[i]);
      if (!a0)
      {
      }
      cnt = ((CamPath *) a0)->numKeys;
      i++;
    }
    while (i <= cnt);
  }
  ((CamPath *) a0)->keys[0] = ((CamPath *) a0)->keys[1];
  ((CamPath *) a0)->keys[cnt + 1] = ((CamPath *) a0)->keys[cnt];
}

void camList_init_000007E0(int *a0) {
    a0[9] = 0;
    *a0 = 0;
}

void camList_read_000007EC(int *arg0) {
    int i;
    int *p;

    stream_readS32_00000280(arg0 + 9);
    i = 0;
    p = arg0 + 1;
    while (i < arg0[9]) {
        stream_readS32_00000280(p);
        i++;
        p++;
    }
}

/* 8-arg constructor/initializer (byte-exact 2026-06-20).
 *   a0 = dst (alloc 0x27C if NULL); a1 -> a0->0x150; a2/a3/arg4 -> 047484 args;
 *   arg5 (float) -> 047484 5th arg; arg6 -> a0->0x274; arg7 -> flag + 04C774 3rd arg.
 * Crack: 04C774 takes arg7 as its 3rd argument (was passed only 2 args). Passing
 * arg7 to the call pins it to $a2, turning the prior `beql t7` (test-only reload)
 * into the target's plain `beq a2` (test value reused as call arg) — the prior
 * "allocator-weight $t7-vs-$a2 cap" was a missing call-argument. Store reorder
 * (0x24C before the if; 0x274 right after 0x150) lets IDO fill the delay/latency
 * slots in target order. */
extern char game_uso_D_807FE770;
extern int game_uso_func_055750();
extern void game_uso_func_047484(int *dst, int a, int b, int c, float f);
extern void game_uso_func_04C774(int *dst, int flag, int arg7);

int *cam_ctor_00000858(int *a0, int arg1, int arg2, int arg3, int arg4, float arg5, int arg6, int arg7) {
    if (a0 == 0) {
        a0 = (int *)XCALL(game_uso_func_055750, mem_new)(0x27C);
        if (a0 == 0) goto out;
    }
    XCALL(game_uso_func_047484, MCamera_ctor)(a0, arg2, arg3, arg4, arg5); /* MCamera ctor (target, x, y, z) */
    NODE(a0)->vptr = (char *)&game_uso_D_807FE770;
    ((CourseCam *)a0)->numLists = 0;
    ((CourseCam *)a0)->numPaths = 0;
    if (arg7 != 0) XCALL(game_uso_func_04C774, Node_Load)(a0, 1, arg7); /* load arg7 ("rom/cam_info.bin") */
    ((CourseCam *)a0)->rider = (struct SnowBoarder *)arg1;
    ((CourseCam *)a0)->pad = (struct RiderInput *)arg6;
    ((CourseCam *)a0)->chaseMode = 0;
    ((CourseCam *)a0)->savedChaseMode = 0;
    ((CourseCam *)a0)->freeRot = 0;
out:
    return a0;
}

void cam_reset_000008FC(int *a0) {
    int *v0 = (int*)((CourseCam *)a0)->cam.target;
    CALL(SnowChaseActorLook_reset_000028A8)(((LookAtNode *)v0)->chase, a0);
    CALL(SnowChaseActorCam_reset_00001DC4)(((CourseCam *)a0)->eyeChase, a0);
    ((CourseCam *)a0)->lastResult = 0;
    CALL(cam_pickResultView_00000940)(a0);
}

// cam_pickResultView_00000940 — three contiguous switches (jr @ 0x970/0x9DC/0xA58;
// sltiu 7/6/6). 2026-06-20: body fully resolved — base is the USO import
// &import_80020098 (NOT the &D_00000000 placeholder), the tail call is
// cam_setView_00000AEC(arg0, var_a1), the switch value is read inline
// (keeps it in $t7 not $v0), and case 6 written as `if(x!=7){4}else{5}` to
// hit the target's beq+b shape. EVERY .text word now matches EXCEPT the 3
// jumptable LO16 loads (off 0x968/0x9D4/0xA50): target = HI16-only relocs
// against external symbols game_uso_D_807FF924/940/958 with fixed offsets
// 0x4/0x20/0x38; IDO's C `switch` instead emits a self-local .rodata
// jumptable (HI16+LO16, offsets 0x0/0x1C/0x34). This is the documented
// jr-via-rodata jumptable cap (docs/MATCHING_WORKFLOW.md:613,6900): it can't
// byte-match without rodata-relayout infra placing the build's local jtbl at
// the external symbol addrs — a multi-tick splat/linker job, NOT a C lever.
// 104/107 words exact.
// 2026-10-16 (agent-g): EXACT, block-equal (no lo16 exception). The 3 words
// were never external-symbol relocs: the ROM %lo values are offsets into the
// unit's own RoData (past the section's 4-byte marker word), and the build's
// .rodata simply lacked the INCLUDE_ASM fns' literals. Those are now emitted
// as `.late_rodata` in their .s files (game_uso_func_00000000.s carries the
// +0x0 float 1/6), so this fn's jumptables land at +0x4/+0x20/+0x38 like the
// ROM. docs/MATCHING_WORKFLOW.md#game-uso-rodata-relayout-late-rodata.

extern char import_80020098;
void cam_setView_00000AEC(int *a0, int a1);

void cam_pickResultView_00000940(char *arg0) {
    s32 temp_v0;
    s32 var_a1;

    var_a1 = -1;
    switch (((CourseCam *)arg0)->rider->raceResult) { /* switch 1: the race result */
    case 0:                                         /* switch 1 */
        temp_v0 = INPUT_MGR(import_80020098).nextFlowMode;
        if ((temp_v0 == 4) || (temp_v0 == 6) || (temp_v0 == 7) || ((temp_v0 == 1) && (INPUT_MGR(import_80020098).procState == 0))) {
            var_a1 = 1;
        } else {
            switch (INPUT_MGR(import_80020098).course) {                      /* switch 2 */
            case 2:                                 /* switch 2 */
                var_a1 = 6;
                break;
            case 3:                                 /* switch 2 */
                var_a1 = 7;
                break;
            case 4:                                 /* switch 2 */
                var_a1 = 8;
                break;
            case 5:                                 /* switch 2 */
                var_a1 = 7;
                break;
            case 6:                                 /* switch 2 */
                var_a1 = 9;
                break;
            case 7:                                 /* switch 2 */
                var_a1 = 0xA;
                break;
            default:                                /* switch 2 */
                var_a1 = 0;
                break;
            }
        }
        break;
    case 1:                                         /* switch 1 */
        if (INPUT_MGR(import_80020098).winVariant != 0) {
            var_a1 = 0xB;
        } else {
            switch (INPUT_MGR(import_80020098).course) {                      /* switch 3 */
            case 2:                                 /* switch 3 */
                var_a1 = 0xD;
                break;
            case 3:                                 /* switch 3 */
                var_a1 = 2;
                break;
            case 4:                                 /* switch 3 */
                var_a1 = 0xD;
                break;
            case 5:                                 /* switch 3 */
                var_a1 = 2;
                break;
            case 6:                                 /* switch 3 */
                var_a1 = 2;
                break;
            case 7:                                 /* switch 3 */
                var_a1 = 0xE;
                break;
            default:                                /* switch 3 */
                var_a1 = 2;
                break;
            }
        }
        break;
    case 2:                                         /* switch 1 */
        var_a1 = 0xC;
        break;
    case 5:                                         /* switch 1 */
        var_a1 = 3;
        break;
    case 6:                                         /* switch 1 */
        if (INPUT_MGR(import_80020098).course != 7) {
            var_a1 = 4;
        } else {
            var_a1 = 5;
        }
        break;
    }
    if (var_a1 != -1) {
        cam_setView_00000AEC((int *)arg0, var_a1);
    }
}

void cam_setView_00000AEC(int *a0, int a1) {
    if (((CourseCam *)a0)->lists[0]->count > 0) {
        ((CourseCam *)a0)->view = a1;
    }
    ((CourseCam *)a0)->listPos = 0;
    ((CourseCam *)a0)->key = 1;
    ((CourseCam *)a0)->frame = 0;
}

/* Moved 2026-06-10 to its address-ordered position (was stranded after
 * func_00002FF8, emitting at block offset ~0x2FF4 instead of 0xB14 --
 * Yay0 block layout is definition-order). */
extern void game_uso_func_00000000();
void cam_update_00000B14(void *a0) {
    GCALL(cam_updateView_00000B3C)(a0);
    GCALL(MNode_update)(a0);
}

/* cam_updateView_00000B3C: spline path-follow update (706 words). EXACT 2026-11-12 (agent-h run 355)
 * through the MIPSpro 7.2.1 C++ front end donor src/game_uso/game_uso_mp72_B3C.cpp (tools/ncc721.sh =
 * period edgcpfe EDG 2.29 + the IDO 7.1 back end), REPLACE_FUNC_BODY-spliced in both trees. Its
 * `switch (s->0x26C)` has 4 dense labels lowered as a 4-entry jump table, which only EDG 2.29's switch
 * rule produces (docs/TOOLING_DECOMP.md "Toolchain forensics III"). The placeholder below only
 * re-emits the function's 16 RoData words (+0x50..+0x90: the 4-word table, then the mrk / mk1..mk9 /
 * mkl / mkr strings) at its place in the layout, as the INCLUDE_ASM .s did with .late_rodata; IDO merges
 * equal literals, so the stand-ins are distinct, and none has a zero low half (those load by lui).
 * Its text and relocs are dropped by the splice. The cfe body it came from (99.9986: the same logic,
 * plus a phantom `case 4:` that cfe needs for a table) was removed in agent-g run 506 (git e6043cf4d). */
void cam_updateView_00000B3C(char *arg0) {
    *(volatile float *)arg0 = 1.1f;
    *(volatile float *)arg0 = 1.2f;
    *(volatile float *)arg0 = 1.3f;
    *(volatile float *)arg0 = 1.4f;
    *(volatile float *)arg0 = 1.6f;
    *(volatile float *)arg0 = 1.7f;
    *(volatile float *)arg0 = 1.8f;
    *(volatile float *)arg0 = 1.9f;
    *(volatile float *)arg0 = 2.1f;
    *(volatile float *)arg0 = 2.2f;
    *(volatile float *)arg0 = 2.3f;
    *(volatile float *)arg0 = 2.4f;
    *(volatile float *)arg0 = 2.6f;
    *(volatile float *)arg0 = 2.7f;
    *(volatile float *)arg0 = 2.8f;
    *(volatile float *)arg0 = 2.9f;
}

/* (The cfe reference body of cam_updateView_00000B3C was removed in agent-g run 506; git e6043cf4d has it.) */

void cam_readParms_00001644(int *a0) {
    int i;
    int *p;
    int v;

    stream_readS32_00000280(&((CourseCam *)a0)->numPaths);
    i = 0;
    while (i < ((CourseCam *)a0)->numPaths) {
        v = CALL(camPath_new_00000608)(0);
        ((CourseCam *)a0)->paths[i] = (CamPath *)v;
        CALL(camPath_init_00000634)(v);
        CALL(camPath_read_00000724)(((CourseCam *)a0)->paths[i]);
        i++;
    }

    i = 0;
    stream_readS32_00000280(&((CourseCam *)a0)->numLists);
    if (((CourseCam *)a0)->numLists > 0) {
        /* p walks a0 one word per list, so ((CourseCam *)p)->lists[0] is lists[i]: indexing lists[i]
         * here makes IDO rebuild the pointer from i after the read call (an extra sll / addu) */
        p = a0;
        do {
            v = CALL(mem_new)(sizeof(CamList));
            ((CourseCam *)p)->lists[0] = (CamList *)v;
            CALL(camList_init_000007E0)(v);
            CALL(camList_read_000007EC)(((CourseCam *)p)->lists[0]);
            i++;
            p++;
        } while (i < ((CourseCam *)a0)->numLists);
    }
}

void cam_handleMsg_00001714(int a0, int *a1) {
    int v = *a1;
    if (v == 8 || v == 9) {
        CALL(cam_reset_000008FC)(a0);
    }
}

/* cam_command_0000174C: 16-case (a1==3..18) dispatch. MATCHED byte-exact
 * 2026-06-23 (agent-b). 16-case dispatch on a1 (cases 3..18); a1==3 has a
 * special body (extra `a0->0x268 = 1` after the call), cases 4..18 are a single
 * `cam_setView_00000AEC(a0, a1 + 11)` call. Implemented as an `if (a1 == N)
 * goto cN;` chain (switch emits a .rodata jump table that the linker discards in
 * this segment; if-else-if reorders the whole dispatch). The former 4-diff
 * residual was the LAST test (a1==18) folding into `bnel a1,at,end` (epilogue
 * `lw ra` pulled into the BL delay), where the target wants plain
 * `beq a1,at,c18 / nop / b end / lw ra(b-delay)`. FIX (docs/IDO_CODEGEN.md
 * "split-last-eq-test-to-suppress-bnel"): write the last test as an explicit
 * inverted skip + unconditional goto (`if (a1 != 18) goto end; goto c18;`) so
 * the optimizer can't fold the epilogue into a branch-likely. 0 diffs. */
void cam_command_0000174C(int *a0, int a1, int a2) {
    ((CourseCam *)a0)->freeRot = 0;
    if (a1 == 3)  goto c3;
    if (a1 == 4)  goto c4;
    if (a1 == 5)  goto c5;
    if (a1 == 6)  goto c6;
    if (a1 == 7)  goto c7;
    if (a1 == 8)  goto c8;
    if (a1 == 9)  goto c9;
    if (a1 == 10) goto c10;
    if (a1 == 11) goto c11;
    if (a1 == 12) goto c12;
    if (a1 == 13) goto c13;
    if (a1 == 14) goto c14;
    if (a1 == 15) goto c15;
    if (a1 == 16) goto c16;
    if (a1 == 17) goto c17;
    if (a1 != 18) goto end;
    goto c18;
c3:  cam_setView_00000AEC(a0, 1); ((CourseCam *)a0)->freeRot = 1; goto end;
c4:  cam_setView_00000AEC(a0, 15); goto end;
c5:  cam_setView_00000AEC(a0, 16); goto end;
c6:  cam_setView_00000AEC(a0, 17); goto end;
c7:  cam_setView_00000AEC(a0, 18); goto end;
c8:  cam_setView_00000AEC(a0, 19); goto end;
c9:  cam_setView_00000AEC(a0, 20); goto end;
c10: cam_setView_00000AEC(a0, 21); goto end;
c11: cam_setView_00000AEC(a0, 22); goto end;
c12: cam_setView_00000AEC(a0, 23); goto end;
c13: cam_setView_00000AEC(a0, 24); goto end;
c14: cam_setView_00000AEC(a0, 25); goto end;
c15: cam_setView_00000AEC(a0, 26); goto end;
c16: cam_setView_00000AEC(a0, 27); goto end;
c17: cam_setView_00000AEC(a0, 28); goto end;
c18: cam_setView_00000AEC(a0, 29);
end: ;
}

extern int game_uso_func_055750();
extern int game_uso_func_04A188();
extern char game_uso_D_807FE908, game_uso_D_807FE910, game_uso_D_807FE914,
            game_uso_D_807FE918, game_uso_D_807FE91C, game_uso_D_807FE920,
            game_uso_D_807FE924, game_uso_D_807FE928, game_uso_D_807FE92C,
            game_uso_D_807FE608, game_uso_D_807FE620;
/* SnowChaseActorCamParms_ctor (0x434): cfront's inlined ctor of the 0xD0-byte SnowChaseActorCamParms
 * (camera.h). `if (s0 || (s0 = new(0xD0)))` around everything; the Parms base part (name, list), whose own
 * alloc-or-given arm is dead; then per member parm `o = s0 + off; if (o || (o = new(size))) {...}` (the
 * member-address null test is never true; a failed alloc would skip only that member): Parms_addParm(o,
 * owner s0, tag word, 1), the parm-type vtable and the default. Member 0 (cof) is a CamVec3Parm (min / max /
 * default vectors passed by value: lo, big.v, hi = (0, 64, 0)); the other seven are CamParm floats
 * (default 0). Each tag word is a by-value 4-byte struct (per-stage temp t2..t8, copied into one shared
 * param prm).
 * EXACT 2026-10-21 (agent-g run 116) with the 3018 recipe (docs/IDO_CODEGEN.md#cfront-ctor-stage-struct-copy-3018):
 * per-stage struct copy `prm = T`, s0 as the parameter, `if (1) {}` between
 * the stage-1 vectors and int zeros in hi, and the pad map re-fit (no g8,
 * g0[3], one int after o1). */
typedef struct { int w; } T4_18FC;
void *SnowChaseActorCamParms_ctor(char *s0) {
    Vec3 hi;
    Vec3 lo;
    struct { T4_18FC idx; Vec3 v; } big;
    T4_18FC t2, t3, t4, t5, t6, t7, t8;
    int g0[3];
    char *o;
    int g1[5];
    char *o1;
    int g2;
    Vec3 pa;
    int g3;
    Vec3 pb;
    int g4;
    Vec3 pc;
    int g5;
    T4_18FC prm;
    int g6;
    Vec3 tmp;
    char *b;
    int g7;

    if (1) { T4_18FC *q = &t2; q = &t3; q = &t4; q = &t5; q = &t6; q = &t7; q = &t8; }
    if (s0 != 0 || (s0 = (char *)XCALL(game_uso_func_055750, mem_new)(0xD0)) != 0) {
        b = s0;
        if (b != 0 || (b = (char *)XCALL(game_uso_func_055750, mem_new)(8)) != 0) {
            ((SnowChaseActorCamParms *)b)->name = &game_uso_D_807FE908 + kStr_Parms_318;
            ((SnowChaseActorCamParms *)b)->unk04 = 0;
        }
        hi.x = 0; hi.y = 64.0f; hi.z = 0;
        if (1) {}
        lo.x = -1000.0f; lo.y = -1000.0f; lo.z = -1000.0f;
        if (1) {}
        big.idx.w = ((T4_18FC *)(&game_uso_D_807FE910 + kTag_cof_320))->w;
        if (1) {}
        big.v.x = 1000.0f; big.v.y = 1000.0f; big.v.z = 1000.0f;
        pa = hi;
        pb = lo;
        pc = big.v;
        prm.w = big.idx.w;
        o1 = s0 + 8;
        if (o1 != 0 || (o1 = (char *)XCALL(game_uso_func_055750, mem_new)(0x20)) != 0) {
            XCALL(game_uso_func_04A188, Parms_addParm)(o1, s0, prm, 1);
            ((CamVec3Parm *)o1)->vptr = &game_uso_D_807FE620 + 48;
            ((CamVec3Parm *)o1)->unk1C = 0;
            tmp = pa;
            ((CamVec3Parm *)o1)->value.x = tmp.x;
            ((CamVec3Parm *)o1)->value.y = tmp.y;
            ((CamVec3Parm *)o1)->value.z = tmp.z;
        }
/* NEW / ADDPARM: the two callees, passed per use so each jal has a source site
 * (name-placeholder-calls.py names macro arguments, not macro bodies) */
#define STAGE(OFF, T, SYM, TOFF, NEW, ADDPARM) \
        T.w = ((T4_18FC *)(&(SYM) + (TOFF)))->w; \
        prm = T; \
        o = s0 + (OFF); \
        if (o != 0 || (o = (char *)NEW(0x18)) != 0) { \
            ADDPARM(o, s0, prm, 1); \
            ((CamParm *)o)->vptr = &game_uso_D_807FE608 + 24; \
            ((CamParm *)o)->unk14 = 0; \
            ((CamParm *)o)->value = 0.0f; \
        }
        STAGE(40,  t2, game_uso_D_807FE914, 0x324, XCALL(game_uso_func_055750, mem_new), XCALL(game_uso_func_04A188, Parms_addParm))
        STAGE(64,  t3, game_uso_D_807FE918, 0x328, XCALL(game_uso_func_055750, mem_new), XCALL(game_uso_func_04A188, Parms_addParm))
        STAGE(88,  t4, game_uso_D_807FE91C, 0x32C, XCALL(game_uso_func_055750, mem_new), XCALL(game_uso_func_04A188, Parms_addParm))
        STAGE(112, t5, game_uso_D_807FE920, 0x330, XCALL(game_uso_func_055750, mem_new), XCALL(game_uso_func_04A188, Parms_addParm))
        STAGE(136, t6, game_uso_D_807FE924, 0x334, XCALL(game_uso_func_055750, mem_new), XCALL(game_uso_func_04A188, Parms_addParm))
        STAGE(160, t7, game_uso_D_807FE928, 0x338, XCALL(game_uso_func_055750, mem_new), XCALL(game_uso_func_04A188, Parms_addParm))
        STAGE(184, t8, game_uso_D_807FE92C, 0x33C, XCALL(game_uso_func_055750, mem_new), XCALL(game_uso_func_04A188, Parms_addParm))
#undef STAGE
    }
    return s0;
}


/* 4-arg constructor-or-init for 0x124-byte object. Lever: reuse `a0`
 * (drop the `register char *p = a0;` fresh local). With direct
 * param-mutation, IDO schedules `move s0, a0` BEFORE the sw ra/a1/a2
 * chain (matching target) instead of into the bne delay slot. */
SnowChaseActorCam *SnowChaseActorCam_ctor(SnowChaseActorCam *a0, int target, int source, int parmsFile) {
    if (a0 == 0) {
        a0 = (SnowChaseActorCam *)CALL(mem_new)(0x124);
        if (a0 == 0) goto end;
    }
    CALL(Node_ctor)(a0, &D_00000000 + kStr_SnowChaseActorCam_340); /* Node init, name "SnowChaseActorCam" */
    NODE(a0)->vptr = (char *)&D_00000000;
    CALL(SnowChaseActorCamParms_ctor)(&a0->parms); /* SnowChaseActorCamParms_ctor */
    if (parmsFile != 0) { /* load the parms from that file (bootup cam_create: "rom/cam_ca.bin" etc.) */
        CALL(Node_Load)(a0, 1, parmsFile);
    }
    a0->target = (CamMNode *)target;
    a0->source = (CamMNode *)source;
end:
    return a0;
}

void SnowChaseActorCam_reset_00001DC4(SnowChaseActorCam *a0) {
    a0->mode = 0;
    a0->eyeOffset.x = 0.0f;
    a0->eyeOffset.y = 0.0f;
    a0->eyeOffset.z = 0.0f;
}

/* SnowChaseActorCam_update: 0x5F8 (382 words). Two-pass entity homing.
 * key 0 does nothing; key 3 mirrors the source orientation into out+0x60
 * and out+0xA0. Other keys apply planar and vertical magnitude clamps,
 * emit each correction via 072EE8, and commit the frame-delta Vec3.
 *
 * 2026-09-12 (agent-h): 68.96 -> 90.43% NON_MATCHING. Reconstruct the
 * descending stack homes, a separately homed output pointer, self_v as
 * a float array, and scoped pointers for the scaling sources and shared
 * integer-copy hop. These reproduce the target's 384-byte frame, ra-only
 * save, a2 argument web and caller-home spills. The former claim that the
 * no-s-reg shape was unreachable is superseded by this compiled C.
 * Coefficients at +0xAC/+0x10C load after the first copy hop, as in target.
 * Named X/Z results and reuse of the short-path output pointer improve
 * floating-point and pointer lifetimes without changing the algorithm.
 *
 * Follow-up: 90.43 -> 96.65% NM, now 382/382 instruction words in length
 * and 259/382 raw words exact. Scope the X/Z position updates to retain
 * their reloads and the second a2-home store; stage planar Z before X.
 * Distinguish correction/coefficient/commit scalar lifetimes, reuse mag
 * and excess in the vertical pass, and read the saved output home directly
 * when capturing the reference vector. All five call offsets now agree.
 * Still not exact: FP registers/load order and early pointer registers
 * differ. Keep the ASM fallback; no training episode.
 * See docs/IDO_CODEGEN.md#homing-full-length-staging-1ddc.
 * Next pass: 96.65 -> 98.16% NM, 313/382 raw words. Read the source
 * pointer after capturing ref_v. In correction scales, leave X/Z inline
 * and name only Y; coefficient and commit stages use different shared
 * result slots. The remaining early pointer and FP differences are real,
 * not an exact match hidden by metadata. See
 * docs/IDO_CODEGEN.md#homing-result-slot-allocation-1ddc.
 * Next pass: 98.16 -> 98.94% NM, 341/382 raw words. Name correction
 * results X/Y/Z as fx/fz/mag (the old magnitude is dead after the factor
 * is formed), scope only the planar else Z store, and use fy for commit Y.
 * Sequence each coefficient load inside the X-result expression. Write
 * acc's X/Y/Z captures on one line to recover target copy-back ordering.
 * Frame, instruction count and all five calls still agree. Early output
 * pointers and shared FP values remain different; no exact episode.
 * See docs/IDO_CODEGEN.md#homing-correction-result-lifetimes-1ddc.
 * Next pass: 98.94 -> 99.46% NM, 343/382 raw words. Keep the short-path
 * output-home reload and first orientation store on one physical line.
 * This recovers target scheduling without changing the pointer or values;
 * 39 real pointer/FP-register differences remain. No exact episode.
 * See docs/IDO_CODEGEN.md#homing-output-reload-sameline-1ddc.
 * See docs/IDO_CODEGEN.md#homing-scoped-pointers-1ddc.
 * 2026-10-23 agent-g run 134: 36 -> 13 raw words. Key-3 arm stores the
 * output home FIRST and reads it back (outh[0] = a0[5]; out = outh[0];),
 * so the forwarded value is a ugen temp (t6) as in PR #10's 28C0; the
 * non-3 arm reads ref_v through a block-scoped copy of out_home[0], which
 * keeps the t-ring in step for the dcopy word copy. FP: the pass-1 else
 * excess and the later z products get their own variables, the final
 * commit Y uses fx. LEFT: the zero constant (target f14 both passes)
 * against fy (0x7C load, commit-1 Y). See docs kit XLVI. */
#ifdef NON_MATCHING
extern Vec3* cam_toTargetLocal_000023D4(Vec3 *out, char *a1);
extern float game_uso_func_070238(float);
extern void game_uso_func_071028(Vec3 *v);
extern void game_uso_func_072EE8(char *dst, Vec3 *v);

void SnowChaseActorCam_update(int *a0) {
    /* Descending stack homes; dead slots retain the target layout. */
    float m[3];
    char *outh[1];
    char *out;
    char *o2_unused; /* dead pointer home; out is reused on the short path */
    char *src;
    int key;
    Vec3 dcopy;
    Vec3 fin_c2;
    Vec3 ref_v;
    float self_v[3];
    char *out_home[1];
    Vec3 acc;
    float speed;
    Vec3 diff;
    float mag;
    float sel;
    float scratch[3];
    float excess, yd_unused, y_excess_unused;
    char *s_unused;
    Vec3 *delta;
    Vec3 st_p1t;
    Tri3i *hopA;
    char pad_d0[8];
    Vec3 hopB;
    char pad_bc[8];
    Vec3 hop_p1e;
    Vec3 st_p1e;
    char pad_a0[4];
    Vec3 st_p1e2;
    Vec3 *scaled_src;
    float fx, fy, fz; /* shared result slots; roles vary by scaling stage */
    char pad_80[4];
    Vec3 fin_e;
    char pad_6c[8];
    Vec3 st_c1;
    Vec3 st_p2t;
    Vec3 hop_p2e;
    Vec3 st_p2e;
    Vec3 st_p2e2;
    Vec3 st_c2;
    /* run 147 policy cleanup: run 135's entry `if (fz) {}` tested an
     * UNINITIALISED local (banned). Removed; 99.93 -> 99.10 accepted. Legal
     * stand-ins (if (a0), if (key), if (*a0), fz = 0 + test, if (1)) all
     * score the same 99.10. */
    key = CAMC(a0)->mode;

    if (key == 0) {
        return;
    }
    if (key == 3) {
        /* Store first, then read the home: the forwarded value stays a ugen temp (t6). */
        outh[0] = (char *)CAMC(a0)->body;
        out = outh[0];
        src = (char *)CAMC(a0)->source;
        m[0] = MN(src)->world[3][0];
        src += 0x70; /* &->world: +0x34 / +0x38 = world[3][1] / [2] */
        m[1] = *(float *)(src + 0x34);
        m[2] = *(float *)(src + 0x38);
        MN(out)->local[3][0] = m[0];
        MN(out)->local[3][1] = m[1];
        MN(out)->local[3][2] = m[2];
        /* Same source line preserves IDO's target reload scheduling. */
        out = outh[0]; MN(out)->world[3][0] = m[0];
        MN(out)->world[3][1] = m[1];
        MN(out)->world[3][2] = m[2];
        return;
    }

    {
        /* Keep the output pointer homed across both emit calls. */
        out_home[0] = (char *)CAMC(a0)->body;

        { char *mo = out_home[0]; /* named read of the home keeps the t-ring in step */
        ref_v.x = MN(mo)->world[3][0];
        ref_v.y = MN(mo)->world[3][1];
        ref_v.z = MN(mo)->world[3][2]; }

        src = (char *)CAMC(a0)->target;
        self_v[0] = MN(src)->world[3][0];
        src += 0x70; /* &->world: +0x34 / +0x38 = world[3][1] / [2] */
        self_v[1] = *(float *)(src + 0x34);
        self_v[2] = *(float *)(src + 0x38);

        delta = cam_toTargetLocal_000023D4((Vec3 *)scratch, (char *)a0);
        *(Tri3i *)&dcopy = *(Tri3i *)delta;
        /* Separate update scopes retain the target's post-store reloads. */
        if (1) { self_v[0] = self_v[0] + dcopy.x; }
        self_v[1] = self_v[1] + dcopy.y;
        if (1) { self_v[2] = self_v[2] + dcopy.z; }

        speed = CAMC(a0)->parms.txz.value;
        diff.y = (fz = (float)0); /* retain the target's zero-square term */
        mag = self_v[2] - ref_v.z; diff.z = mag;
        excess = self_v[0] - ref_v.x; diff.x = excess;

        mag = game_uso_func_070238(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
        game_uso_func_071028(&diff);

        /* These scoped pointer definitions split the cross-call webs. */
        if (1) { scaled_src = (Vec3 *)&diff; }
        if (1) { hopA = (Tri3i *)scratch; }
        /* run 140: the threshold in fz and the excess in fy (same-name web
         * reuse) colour them f2 / f0 as in the target (raw 10 -> 7). */
        fz = CAMC(a0)->parms.mxz.value;
        if (mag < fz) {
            /* run 145: z product in fy and the st_c1.y product in mag
             * (web reuse) colour f14 / f12 as in the target (raw 7 -> 5). */
            fy = mag - fz;
            st_p1t.x = (fx = scaled_src->x * fy); st_p1t.y = (fz = scaled_src->y * fy); st_p1t.z = (fy = scaled_src->z * fy);
            *hopA = *(Tri3i *)&st_p1t;
            *(Tri3i *)&hopB = *hopA;
            acc.x = hopB.x; acc.y = hopB.y; acc.z = hopB.z;
        } else {
            yd_unused = mag - speed; /* own variable: f0 like the then-arm excess */
            st_p1e.x = (fx = scaled_src->x * yd_unused); st_p1e.y = (fz = scaled_src->y * yd_unused); if (1) { st_p1e.z = (y_excess_unused = scaled_src->z * yd_unused); }
            *hopA = *(Tri3i *)&st_p1e;
            *(Tri3i *)&hop_p1e = *hopA;
            if (1) { scaled_src = &hop_p1e; }
            st_p1e2.x = (fz = (excess = CAMC(a0)->parms.cxz.value, scaled_src->x * excess)); st_p1e2.y = (fx = scaled_src->y * excess); st_p1e2.z = (fy = scaled_src->z * excess);
            *(Tri3i *)&hopB = *(Tri3i *)&st_p1e2;
            *(Tri3i *)&fin_e = *(Tri3i *)&hopB;
            acc.x = fin_e.x; acc.y = fin_e.y; acc.z = fin_e.z;
        }
        out = out_home[0];
        game_uso_func_072EE8((char *)MN(out)->local, &acc);

        if (1) { scaled_src = (Vec3 *)&diff; }
        if (1) { hopA = (Tri3i *)scratch; }
        st_c1.x = (excess = scaled_src->x * speed); st_c1.y = (mag = scaled_src->y * speed); st_c1.z = (fz = scaled_src->z * speed);
        *hopA = *(Tri3i *)&st_c1;
        *(Tri3i *)&hopB = *hopA;
        CAMC(a0)->eyeOffset.x = hopB.x;
        CAMC(a0)->eyeOffset.y = hopB.y;
        CAMC(a0)->eyeOffset.z = hopB.z;

        excess = (CAMC(a0)->mode == 2) ? CAMC(a0)->parms.ty2.value
                                  : CAMC(a0)->parms.toy.value;

        sel = excess;
        diff.x = 0.0f;
        diff.y = 1.0f;
        diff.z = 0.0f;
        mag = ref_v.y - self_v[1];
        if (1) { scaled_src = &diff; }
        if (mag < CAMC(a0)->parms.miy.value) {
            excess = CAMC(a0)->parms.miy.value - mag; if (1) {}
            st_p2t.x = (fx = scaled_src->x * excess); st_p2t.y = (fz = scaled_src->y * excess); st_p2t.z = (y_excess_unused = scaled_src->z * excess);
            *hopA = *(Tri3i *)&st_p2t;
            *(Tri3i *)&hopB = *hopA;
            acc.x = hopB.x; acc.y = hopB.y; acc.z = hopB.z;
        } else {
            excess = sel - mag;
            st_p2e.x = (fx = scaled_src->x * excess); st_p2e.y = (fz = scaled_src->y * excess); st_p2e.z = (y_excess_unused = scaled_src->z * excess);
            *hopA = *(Tri3i *)&st_p2e;
            *(Tri3i *)&hop_p2e = *hopA;
            if (1) { scaled_src = &hop_p2e; }
            st_p2e2.x = (fz = (excess = CAMC(a0)->parms.chy.value, scaled_src->x * excess)); st_p2e2.y = (fx = scaled_src->y * excess); st_p2e2.z = (fy = scaled_src->z * excess);
            *(Tri3i *)&hopB = *(Tri3i *)&st_p2e2;
            *(Tri3i *)&fin_e = *(Tri3i *)&hopB;
            acc.x = fin_e.x; acc.y = fin_e.y; acc.z = fin_e.z;
        }
        out = out_home[0];
        game_uso_func_072EE8((char *)MN(out)->local, &acc);

        if (1) { scaled_src = (Vec3 *)&diff; }
        if (1) { hopA = (Tri3i *)scratch; }
        st_c2.x = (excess = scaled_src->x * sel); st_c2.y = (fx = scaled_src->y * sel); st_c2.z = (fz = scaled_src->z * sel);
        *hopA = *(Tri3i *)&st_c2;
        *(Tri3i *)&fin_c2 = *hopA;
        CAMC(a0)->eyeOffset.x -= fin_c2.x;
        CAMC(a0)->eyeOffset.y -= fin_c2.y;
        CAMC(a0)->eyeOffset.z -= fin_c2.z;
    }
}
#else
/* EXACT via an IDO 7.1 NCC (C++) donor: src/game_uso/game_uso_ncc_1DDC.cpp (agent-g run 168 lift,
 * agent-c run 240 exact), REPLACE_FUNC_BODY-spliced over this placeholder and over the cfe body above
 * (kept as the C reference) in both build trees. */
void SnowChaseActorCam_update(int *a0) {
}
#endif

/* cam_toTargetLocal_000023D4: rotate a Vec3 by the upper-3x3 of a column-major
 * 4x4 matrix. Input Vec3 at a1+0x5C; matrix at (*(float**)(a1+0x38))+0x70
 * (column stride 0x10 = 4 floats, row stride 4). Double-buffers through a
 * stack Vec3 (struct copy in, struct copy out) so a0 may alias the input.
 * Returns a0.
 *
 *   out.x = M[0]*v.x + M[4]*v.y + M[8]*v.z   (M = matrix+0x70, in floats)
 *   out.y = M[1]*v.x + M[5]*v.y + M[9]*v.z
 *   out.z = M[2]*v.x + M[6]*v.y + M[10]*v.z
 *
 * MATCHED. Two operand-order spellings stacked: (1) outer add written
 * product-first (`M[8]*v.z + (...)`) flips the final reduction add so the
 * running-sum lands as `fs` (target order); (2) inner pair written
 * v.y-term-first (`M[4]*v.y + M[0]*v.x`) restores IDO's RTL evaluation so the
 * M[0]*v.x product/load is emitted first, matching the target's load schedule.
 * Both are commutative no-ops at the value level, byte-flips at the encoding. */
Vec3* cam_toTargetLocal_000023D4(Vec3 *a0, char *a1) { /* a1: SnowChaseActorCam */
    Vec3 v = *(Vec3*)&((SnowChaseActorCam *)a1)->parms.cof.value; /* the aim offset, rotated into the target's axes */
    float *mm = &((SnowChaseActorCam *)a1)->target->world[0][0];
    float X = mm[8]*v.z + (mm[4]*v.y + mm[0]*v.x);
    float Y = mm[9]*v.z + (mm[5]*v.y + mm[1]*v.x);
    float Z = mm[10]*v.z + (mm[6]*v.y + mm[2]*v.x);
    v.x = X; v.y = Y; v.z = Z;
    *a0 = v;
    return a0;
}

extern void game_uso_func_00000000();

void SnowChaseActorCam_loadParms(SnowChaseActorCam *a0) {
    GCALL(Parms_load_00035C6C)(&a0->parms);
}

/* SnowChaseActorCam_applyMode_000024BC: camera-homing update (0x258 / 150 words).
 * EXACT via REPLACE_FUNC_BODY donor splice: the real body is the IDO 7.1 NCC
 * (C++) donor src/game_uso/game_uso_ncc_24BC.cpp (150/150 words; out read
 * through an inline Ctx accessor, Out::setG / setP in a nested block, one
 * empty if after q += r). The cfe wrap capped at 72%.
 * docs/MATCHING_WORKFLOW.md#game-uso-ncc-cplusplus-donor-recipe. Body below
 * is a placeholder for the splice. */
void SnowChaseActorCam_applyMode_000024BC(int *a0, int *a1) {
    volatile int *ret = a0;
}

/* mode: see SnowChaseActorCam.mode; a2 = the rider's velocity (cam_setView passes rider + 0x318): applyMode snaps
 * the eye behind the direction of travel */
void SnowChaseActorCam_setMode_00002714(int *a0, unsigned int a1, int a2) {
    if (((SnowChaseActorCam *)a0)->mode == a1) return;
    ((SnowChaseActorCam *)a0)->mode = a1;
    SnowChaseActorCam_applyMode_000024BC(a0, (int *)a2);
}

/* SnowChaseActorLookParms_ctor (52 words): the 0x20-byte Parms object of SnowChaseActorLook, cfront's
 * inlined ctor chain. `this` = arg0 or new(0x20); the Parms base part (vtable &D+0x354, count 0; its own
 * alloc-or-given `new(8)` arm is dead but kept by IDO); then the one embedded float parm at this + 8
 * (a ParmNode: Parms_addParm(node, owner, tag word D+0x35C, 1), float-parm vtable &D+0x18, default 0.0f).
 * `arg0 == -8` is cfront's null test of the member address `this + 8`.
 * Levers that give the exact frame:
 *  (1) PARAM REUSE: drop var_a1, reassign arg0 itself -> IDO homes the spill
 *      at the PARAM HOME slot sp+0x38 (= frame+0), frame grows -0x30 -> -0x38.
 *  (2) DECL-ORDER slot map (locals decl-order TOP-DOWN from 0x34):
 *      init_arg@0x34, var_v1@0x30, pad_a@0x2C, pad_b@0x28, var_a0@0x24,
 *      pad_c@0x20, init_arg_copy(One1)@0x1C. Three volatile pads fill gaps.
 *  (3) TWO-HOMES-NO-RELOAD (docs RMW-forwarding mechanism): init_arg is a
 *      PLAIN int (NOT volatile), address-taken via the struct-cast re-read
 *      `init_arg_copy = *(One1*)(int*)&init_arg;` -> the folded reload keeps
 *      store 1 alive (sw t7,0x34) and forwards t7 into the second store
 *      (sw t7,0x1C in the bne delay). volatile here blocks forwarding (+1 lw);
 *      CSE-form (two loads of D+0x35C) promotes to a $v0 candidate (wrong reg).
 *      Single-use load also folds lw t7,860(t7) onto the address reg.
 *  (4) STRUCT-BY-VALUE ARG (One1): passing init_arg_copy (1-word struct) to
 *      K&R gl_func emits lw a2,0x1C + outgoing-arg home sw a2,8(sp) in the
 *      jal delay (int arg would drop the home store). */
void *SnowChaseActorLookParms_ctor(void *arg0) {
  int init_arg;
  void *var_v1;
  volatile int pad_a;
  volatile int pad_b;
  void *var_a0;
  volatile int pad_c;
  One1 init_arg_copy;
  if (arg0 == ((void *) 0))
  {
    arg0 = (void *) CALL(mem_new)(0x20);
    if (arg0 == ((void *) 0))
    {
      goto done;
    }
  }
  var_v1 = arg0;
  if (arg0 == ((void *) 0))
  {
    var_v1 = (void *) CALL(mem_new)(8);
    if (var_v1 == ((void *) 0))
    {
      goto after_template;
    }
  }
  ((SnowChaseActorLookParms *) var_v1)->name = ((char *) (&D_00000000)) + kStr_Parms_354; /* "Parms" */
  ((SnowChaseActorLookParms *) var_v1)->unk04 = 0;
  after_template:
  init_arg = *((int *) (((char *) (&D_00000000)) + kTag_ch0_35C));
  init_arg_copy = *((One1 *) (int *) &init_arg);
  var_a0 = ((char *) arg0) + 8;
  if (arg0 == ((void *) (-8)))
  {
    var_a0 = (void *) CALL(mem_new)(0x18);
    if (var_a0 == ((void *) 0))
    {
      goto done;
    }
  }
  CALL(Parms_addParm)(var_a0, arg0, init_arg_copy, 1);
  ((CamParm *) var_a0)->vptr = ((char *) (&D_00000000)) + 0x18; /* the float-parm vtable (CamParm, as SnowChaseActorCamParms_ctor) */
  ((CamParm *) var_a0)->value = 0.0f;
  ((CamParm *) var_a0)->unk14 = 0;
  done:
  return arg0;

}

/* Mirror of SnowChaseActorCam_ctor (alloc(0x64) variant; dispatch table
 * at &D_0+0x360). Lever: reuse a0 (drop the `register char *p = a0`
 * fresh local). */
SnowChaseActorLook *SnowChaseActorLook_ctor(SnowChaseActorLook *a0, int anchor, int cam, int parmsFile) {
    if (a0 == 0) {
        a0 = (SnowChaseActorLook *)CALL(mem_new)(0x64);
        if (a0 == 0) goto end;
    }
    CALL(Node_ctor)(a0, &D_00000000 + kStr_SnowChaseActorLook_360); /* Node init, name "SnowChaseActorLook" */
    NODE(a0)->vptr = (char *)&D_00000000;
    CALL(SnowChaseActorLookParms_ctor)(&a0->parms); /* SnowChaseActorLookParms_ctor */
    if (parmsFile != 0) { /* load the parms from that file (bootup cam_create: "rom/cam_ca.bin" etc.) */
        CALL(Node_Load)(a0, 1, parmsFile);
    }
    a0->anchor = (CamMNode *)anchor;
    a0->cam = (SnowChaseActorCam *)cam;
end:
    return a0;
}

void SnowChaseActorLook_reset_000028A8(SnowChaseActorLook *a0) {
    a0->mode = 0;
    a0->unk2C.x = 0.0f;
    a0->unk2C.y = 0.0f;
    a0->unk2C.z = 0.0f;
}

// SnowChaseActorLook_update — STRUCTURAL PASS (0x3E8 / 250 words,
// no episode). Raw-.word USO form (single function, game_uso main
// game-logic segment). Hand-decoded entry/shape; multi-run target.
//
// Per-object FP transform / world-matrix builder, gated by object
// state.
//
//   void SnowChaseActorLook_update(Obj *obj) {              // obj -> a2
//     sp.cache0 = obj->0x14;                              // sp+0xE4
//     sp.cache1 = obj->0x3C->0x38;                        // sp+0xE0
//     if (obj->0x40 == 0) return;
//     if (obj->0x40 != 1) { ...alt path... return; }
//     // copy orientation/scale Vec3 from obj->...->0xA0/0xA4/0xA8
//     //   into sp scratch (sp+0x8C/0xC4..), and position triple;
//     // func_00000000(obj->0x3C, …);  store result;
//     // assemble a transform: copy 3-word matrix-row blocks between
//     //   obj sub-records and sp, then matrix*vector FP chains
//     //   (mul.s/add.s) -> world Vec3/matrix in sp+0xB8..0xD8;
//     // write the composed transform back into obj fields.
//   }
//
// Struct-typing reference:
//   obj: 0x14 a cached word, 0x3C -> sub (->0x38 a field), 0x40
//     state (0 = skip, 1 = build transform, else alt), 0x154 -> a
//     transform source (->0xA0/0xA4/0xA8 orientation Vec3). sp+0x78/
//     0x8C/0xAC = matrix-row scratch; sp+0xB8..0xD8 = output world
//     Vec3/matrix. func_00000000 = USO placeholder dispatcher
//     (transform helper).
// Caps: raw-word USO + placeholder calls + 250-word FP transform —
//   not exact-matchable without proper USO mnemonic disasm;
//   structural (entry/state-gate/shape) partial pass only, no byte
//   body. Multi-run.
// Full body INCLUDE_ASM-preserved (.s = source of truth). INCLUDE_ASM (no episode; tautology-trap rule).
#ifdef NON_MATCHING
/* game_uso_func_070238/071028/072EE8 declared near SnowChaseActorCam_update. */
/* 2CC8/A0E8-family per-object world-transform builder. mode=a0->0x40 dispatch.
 * Both arms compute a position-delta Vec3 (relative to out->0xA0 origin),
 * route it through scratch Vec3 slots (IDO emits 3-word lw/sw block copies),
 * normalize its length (070238), normalize the vector in place (071028),
 * scale by a0->0x5C, emit transformed point into out->0x30.. (072EE8 ->
 * writes out->0x60..), then mirror out->0x60.. back into out->0xA0..
 *
 * mode 1: delta = (s->0xA0 + reloc(a0->0x3C)) - out->0xA0 (orientation vec
 *   plus a local-frame offset from cam_toTargetLocal_000023D4).
 * mode!=0,1: build a +/-1000 z-axis probe (sign from mode==3), rotate it by
 *   the s 3x3 matrix at s->0x70.., add s->0x38->0xA0 origin, delta vs out. */
void SnowChaseActorLook_update(char *a0) {
    /* Fresh-Vec3 / 1DDC-family kit: descending slot ladder, dead-gap pads,
     * two-hop diff->scratch->dc2 chain (mode 1), single-hop axis->axisc +
     * in-place diff over the ssv home (else arm). Ground truth expected/.o:
     * frame 0xE8, no s-regs (family a2 cap), matrix base = s (0x3C->0x38). */
    char *out;            /* s0-promoted (target: homed 0xE4; a2-family cap) */
    char *s;              /* s1/v1 (target: homed 0xE0) */
    volatile s32 pad0;    /* 0xDC gap */
    Vec3 dcopy;           /* 0xD0: int-copy of *delta */
    Vec3 refv1;           /* 0xC4: out orientation (mode 1) */
    float v2v[3];         /* 0xB8: s orientation + delta (array: per-use reload) */
    Vec3 dc2;             /* 0xAC: hop B / normalize+scale buffer */
    float mag;            /* 0xA8 */
    volatile s32 pad1[4]; /* 0x98..0xA4 gap */
    float scratch[3];     /* 0x8C: 23D4 out-arg + shared hop A */
    volatile s32 pad2[2]; /* 0x84..0x88 gap */
    Vec3 diff;            /* 0x78 */
    volatile s32 pad3[4]; /* 0x68..0x74 gap */
    Vec3 axis;            /* 0x5C: else-arm z probe */
    float refv2[3];       /* 0x50: out orientation (else; array: homed) */
    Vec3 ssv;             /* 0x44: (a0->0x38) orientation, reused as diff */
    Vec3 axisc;           /* 0x38: axis hop, overwritten by rotated result */
    float mag2;           /* 0x34 */
    int mode;
    Vec3 *delta;
    float f, dirv, ax, ay, az, zerof;

    out = (char *)LOOK(a0)->body;
    s = (char *)LOOK(a0)->cam->target;
    mode = LOOK(a0)->mode;
    if (mode == 0) {
        return;
    }
    if (mode == 1) {
        refv1.x = MN(out)->world[3][0];
        refv1.y = MN(out)->world[3][1];
        refv1.z = MN(out)->world[3][2];
        v2v[0] = MN(s)->world[3][0];
        v2v[1] = MN(s)->world[3][1];
        v2v[2] = MN(s)->world[3][2];
        delta = cam_toTargetLocal_000023D4((Vec3 *)scratch, (char *)LOOK(a0)->cam);
        *(Tri3i *)&dcopy = *(Tri3i *)delta;
        v2v[0] = v2v[0] + dcopy.x;
        v2v[1] = v2v[1] + dcopy.y;
        v2v[2] = v2v[2] + dcopy.z;
        diff.y = v2v[1] - refv1.y;
        diff.z = v2v[2] - refv1.z;
        diff.x = v2v[0] - refv1.x;
        *(Tri3i *)scratch = *(Tri3i *)&diff;
        *(Tri3i *)&dc2 = *(Tri3i *)scratch;
        mag = game_uso_func_070238(dc2.x * dc2.x + dc2.y * dc2.y + dc2.z * dc2.z);
        game_uso_func_071028(&dc2);
        f = mag * LOOK(a0)->parms.ch0.value;
        dc2.x = dc2.x * f;
        dc2.y = dc2.y * f;
        dc2.z = dc2.z * f;
        game_uso_func_072EE8((char *)MN(out)->local, &dc2);
        MN(out)->world[3][0] = MN(out)->local[3][0];
        MN(out)->world[3][1] = MN(out)->local[3][1];
        MN(out)->world[3][2] = MN(out)->local[3][2];
    } else {
        char *ss;

        refv2[0] = MN(out)->world[3][0];
        refv2[1] = MN(out)->world[3][1];
        refv2[2] = MN(out)->world[3][2];
        ss = (char *)LOOK(a0)->anchor;
        ssv.x = MN(ss)->world[3][0];
        ss += 0x70; /* &anchor->world: +0x34 / +0x38 = world[3][1] / [2] */
        ssv.y = *(float *)(ss + 0x34);
        ssv.z = *(float *)(ss + 0x38);
        if (LOOK(a0)->mode == 3) {
            dirv = -1000.0f;
        } else {
            dirv = 1000.0f;
        }
        zerof = 0.0f;
        axis.x = zerof;
        axis.y = zerof;
        axis.z = dirv;
        *(Tri3i *)&axisc = *(Tri3i *)&axis;
        /* 3x3 matrix rows at s+0x70.. applied to axisc; result overwrites
         * the axisc buffer itself. */
        ax = axisc.x;
        ay = axisc.y;
        az = axisc.z;
        axisc.x = MN(s)->world[0][0] * ax + MN(s)->world[1][0] * ay + MN(s)->world[2][0] * az;
        axisc.y = MN(s)->world[0][1] * ax + MN(s)->world[1][1] * ay + MN(s)->world[2][1] * az;
        axisc.z = MN(s)->world[0][2] * ax + MN(s)->world[1][2] * ay + MN(s)->world[2][2] * az;
        ssv.x = ssv.x + axisc.x;
        ssv.y = ssv.y + axisc.y;
        ssv.z = ssv.z + axisc.z;
        ssv.x = ssv.x - refv2[0];
        ssv.y = ssv.y - refv2[1];
        ssv.z = ssv.z - refv2[2];
        mag2 = game_uso_func_070238(ssv.x * ssv.x + ssv.y * ssv.y + ssv.z * ssv.z);
        game_uso_func_071028(&ssv);
        f = mag2 * LOOK(a0)->parms.ch0.value;
        ssv.x = ssv.x * f;
        ssv.y = ssv.y * f;
        ssv.z = ssv.z * f;
        game_uso_func_072EE8((char *)MN(out)->local, &ssv);
        MN(out)->world[3][0] = MN(out)->local[3][0];
        MN(out)->world[3][1] = MN(out)->local[3][1];
        MN(out)->world[3][2] = MN(out)->local[3][2];
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/game_uso/game_uso", SnowChaseActorLook_update);
#endif

void SnowChaseActorLook_loadParms(SnowChaseActorLook *a0) {
    GCALL(Parms_load_00035C6C)(&a0->parms);
}

/* SnowChaseActorLook_applyMode_00002CC8: camera position snap (0x300 / 192 words).
 * EXACT via REPLACE_FUNC_BODY donor splice: the real body is the IDO 7.1 NCC
 * (C++) donor src/game_uso/game_uso_ncc_2CC8.cpp (192/192 words, sibling of
 * 28C0 / BB8C: Ctx aggregate read through inline accessors, POD P3 return,
 * by-value V3::operator=). The cfe wrap capped at 190 raw words.
 * docs/MATCHING_WORKFLOW.md#game-uso-ncc-cplusplus-donor-recipe. Body below
 * is a placeholder for the splice. */
void SnowChaseActorLook_applyMode_00002CC8(char *a0, char *a1) {
    volatile char *ret = a0;
}

void SnowChaseActorLook_setMode_00002FC8(int *a0, int a1, int a2) {
    if (((SnowChaseActorLook *)a0)->mode == a1) return;
    ((SnowChaseActorLook *)a0)->mode = a1;
    SnowChaseActorLook_applyMode_00002CC8((char *)a0, a2);
}

void CameraBase_update_00002FF8(void) {
    GCALL(MNode_update)();
}

/* SnowChaseActorParms_ctor: EXACT 2026-10-21 (agent-g run 116), from 93.1 NM.
 * Three changes to the run-106 body: (1) each stage stores the template word
 * into its per-stage struct and copies the STRUCT (`T.w = G; prm = T;`) --
 * the `.w` copy put the load in v0 and the shared store first; (2) s0 is the
 * parameter (a `char *s0 = a0` copy added the top frame word) and the g8/g7
 * pads are gone (frame 0xE8); (3) the three stage-1 vectors are separate
 * basic blocks (`if (1) {}` between them) and the zero vector is spelled with
 * int zeros, so 0.0 is not one CSE candidate with the stage `f10 = 0.0f`
 * stores: every constant then lives in $f0 in turn, as in the target.
 * docs/IDO_CODEGEN.md#cfront-ctor-stage-struct-copy-3018.
 *
 * The 0x48C-byte inlined ctor of the 0xE8-byte SnowChaseActorParms (camera.h), the same shape as
 * SnowChaseActorCamParms_ctor: the Parms base part, the CamVec3Parm "off" (default (0, 0, 0)), then eight
 * CamParm floats (s0 + 0x28 .. 0xD0, stride 0x18). */
typedef struct { int w; } T4_3018;
extern int game_uso_func_055750();
extern int game_uso_func_04A188();
extern char game_uso_D_807FE608, game_uso_D_807FE620, game_uso_D_807FE964,
            game_uso_D_807FE96C, game_uso_D_807FE970, game_uso_D_807FE974,
            game_uso_D_807FE978, game_uso_D_807FE97C, game_uso_D_807FE980,
            game_uso_D_807FE984, game_uso_D_807FE988, game_uso_D_807FE98C;
void *SnowChaseActorParms_ctor(char *s0) {
    Vec3 hi;
    Vec3 lo;
    struct { T4_3018 idx; Vec3 v; } big;
    T4_3018 t2, t3, t4, t5, t6, t7, t8, t9;
    int g0[3];
    char *o;
    int g1[5];
    char *o1;
    int g2[2];
    Vec3 pa;
    int g3;
    Vec3 pb;
    int g4;
    Vec3 pc;
    int g5;
    T4_3018 prm;
    int g6;
    Vec3 tmp;
    char *b;

    if (1) { T4_3018 *q = &t2; q = &t3; q = &t4; q = &t5; q = &t6; q = &t7; q = &t8; q = &t9; }
    if (s0 != 0 || (s0 = (char *)XCALL(game_uso_func_055750, mem_new)(0xE8)) != 0) {
        b = s0;
        if (b != 0 || (b = (char *)XCALL(game_uso_func_055750, mem_new)(8)) != 0) {
            ((SnowChaseActorParms *)b)->name = &game_uso_D_807FE964 + kStr_Parms_374;
            ((SnowChaseActorParms *)b)->unk04 = 0;
        }
        hi.x = 0; hi.y = 0; hi.z = 0;
        if (1) {}
        lo.x = -1000.0f; lo.y = -1000.0f; lo.z = -1000.0f;
        if (1) {}
        big.idx.w = ((T4_3018 *)(&game_uso_D_807FE96C + kTag_off_37C))->w;
        if (1) {}
        big.v.x = 1000.0f; big.v.y = 1000.0f; big.v.z = 1000.0f;
        pa = hi;
        pb = lo;
        pc = big.v;
        prm.w = big.idx.w;
        o1 = s0 + 8;
        if (o1 != 0 || (o1 = (char *)XCALL(game_uso_func_055750, mem_new)(0x20)) != 0) {
            XCALL(game_uso_func_04A188, Parms_addParm)(o1, s0, prm, 1);
            ((CamVec3Parm *)o1)->vptr = &game_uso_D_807FE620 + 48;
            ((CamVec3Parm *)o1)->unk1C = 0;
            tmp = pa;
            ((CamVec3Parm *)o1)->value.x = tmp.x;
            ((CamVec3Parm *)o1)->value.y = tmp.y;
            ((CamVec3Parm *)o1)->value.z = tmp.z;
        }
/* NEW / ADDPARM: the two callees, passed per use (see SnowChaseActorCamParms_ctor) */
#define STAGE(OFF, T, SYM, TOFF, NEW, ADDPARM) \
T.w = ((T4_3018 *)(&(SYM) + (TOFF)))->w; prm = T; o = s0 + (OFF); \
        if (o != 0 || (o = (char *)NEW(0x18)) != 0) { \
            ADDPARM(o, s0, prm, 1); \
            ((CamParm *)o)->vptr = &game_uso_D_807FE608 + 24; \
            ((CamParm *)o)->unk14 = 0; \
            ((CamParm *)o)->value = 0.0f; \
        }
        STAGE(40,  t2, game_uso_D_807FE970, 0x380, XCALL(game_uso_func_055750, mem_new), XCALL(game_uso_func_04A188, Parms_addParm))
        STAGE(64,  t3, game_uso_D_807FE974, 0x384, XCALL(game_uso_func_055750, mem_new), XCALL(game_uso_func_04A188, Parms_addParm))
        STAGE(88,  t4, game_uso_D_807FE978, 0x388, XCALL(game_uso_func_055750, mem_new), XCALL(game_uso_func_04A188, Parms_addParm))
        STAGE(112, t5, game_uso_D_807FE97C, 0x38C, XCALL(game_uso_func_055750, mem_new), XCALL(game_uso_func_04A188, Parms_addParm))
        STAGE(136, t6, game_uso_D_807FE980, 0x390, XCALL(game_uso_func_055750, mem_new), XCALL(game_uso_func_04A188, Parms_addParm))
        STAGE(160, t7, game_uso_D_807FE984, 0x394, XCALL(game_uso_func_055750, mem_new), XCALL(game_uso_func_04A188, Parms_addParm))
        STAGE(184, t8, game_uso_D_807FE988, 0x398, XCALL(game_uso_func_055750, mem_new), XCALL(game_uso_func_04A188, Parms_addParm))
        STAGE(208, t9, game_uso_D_807FE98C, 0x39C, XCALL(game_uso_func_055750, mem_new), XCALL(game_uso_func_04A188, Parms_addParm))
#undef STAGE
    }
    return s0;
}


/* SnowChaseActor_ctor: 45-insn alloc-or-passthrough constructor, sibling of
 * cam_ctor_00000858 (same alloc-if-null + init + set-vtable + conditional
 * 04C774(self,1,a3) + field-init-with-floats idiom). MATCHED 2026-06-20 by
 * reconstruction from the resolved .s: the prior body used gl_func_00000000
 * placeholder callees + &D_00000000 bases (all wrong). Real callees: alloc
 * 055750(0x138), init 04C678(self, &D_807FE990+0x3A0), 00003018(self+0x50),
 * conditional 04C774(self,1,a3). Real vtable base &game_uso_D_807FE8A8. Twin's
 * `int *a0; if(!a0) a0=alloc; if(!a0) goto out;` form (reuse param as the
 * pointer, early goto on alloc-fail) gives the prologue `or s0,a0`. Final lever
 * (last 12 store-schedule diffs): write the `0x4C = a2` store IMMEDIATELY after
 * `0x48 = a1` (before 0x2C=0 and the six 0.0f swc1 stores) — this nudges IDO to
 * hoist the `lw a2,0x28(sp)` reload to the target's early slot (right after the
 * a1 store) instead of deferring it to just before its own store. C field-WRITE
 * order != asm field order but bytes are identical. 45/45 words. */
extern char game_uso_D_807FE990;
extern char game_uso_D_807FE8A8;
extern void game_uso_func_04C678(int *dst, void *tmpl);
extern void *SnowChaseActorParms_ctor();

SnowChaseActor *SnowChaseActor_ctor(SnowChaseActor *a0, int target, int lookBody, int parmsFile) {
    if (a0 == 0) {
        a0 = (SnowChaseActor *)XCALL(game_uso_func_055750, mem_new)(0x138);
        if (a0 == 0) goto out;
    }
    XCALL(game_uso_func_04C678, Node_ctor)((int *)a0, (char *)&game_uso_D_807FE990 + kStr_SnowChaseActor_3A0); /* Node init, name "SnowChaseActor" */
    NODE(a0)->vptr = (char *)&game_uso_D_807FE8A8;
    SnowChaseActorParms_ctor(&a0->parms);
    /* load the parms file */
    if (parmsFile != 0) XCALL(game_uso_func_04C774, Node_Load)((int *)a0, 1, parmsFile);
    a0->target = (CamMNode *)target;
    a0->lookBody = (CamMNode *)lookBody;
    a0->unk2C = 0;
    a0->unk30.z = 0.0f;
    a0->unk30.y = 0.0f;
    a0->unk30.x = 0.0f;
    a0->toGoal.z = 0.0f;
    a0->toGoal.y = 0.0f;
    a0->toGoal.x = 0.0f;
out:
    return a0;
}

/* SnowChaseActor_update: 252-insn (0x3F0) heavy state-builder. Frame -0x100,
 * saves s0/ra and double-FPU $f20/$f21 (sdc1 at 0x18). Sibling of
 * game_uso_func_00003A4C (also frame-0xB0+ data-init).
 *
 * Stage 1 @ 0x3558-0x3578 (prologue + a0->[0x14] spill at 0xFC):
 *   t8 = arg0->[0x14]
 *   s0 = arg0; (saved across calls)
 *   a0 = sp+0x8C (stack scratch buffer)
 *
 * Stage 2 @ 0x3578-0x35C0 (3 Vec3-copy operations):
 *   sp[0xF0..0xF8] = t8->Vec3@0xA0       ; arg0->[0x14]'s Vec3
 *   sp[0xE4..0xEC] = s0->[0x48]->Vec3@0xA0 ; arg0->[0x48]'s Vec3
 *   sp[0x8C..0x94] = s0->Tri3i@0x68      ; 3 ints from arg0+0x68
 *
 * Stage 3 @ 0x35C4-0x35E8 (re-copy as floats):
 *   sp[0xC0..0xC8] = sp[0x8C..0x94]      ; treats Tri3i as Vec3
 *
 * 2026-05-15 (agent-b, source-2 sibling of 0x3AC0) — FULL 252-insn algorithm
 * mapped from raw-word objdump. It is a camera/spring vector solve:
 *   p   = s0->[0x14];                 // pointer, spilled sp+0xFC
 *   va  = p->Vec3@0xA0;               // sp+0xF0
 *   m0  = s0->[0x48];                 // matrix/transform source
 *   vb  = { m0->[0xA0], (m0+0x70)->[0x34], (m0+0x70)->[0x38] };
 *   v   = (Vec3)s0->Tri3i@0x68;       // input vec (int bits reused as float)
 *   // 3x3 matrix-vector accumulate: out = vb + M*v, M at (m0+0x70)
 *   //   rows use (m0+0x70)+{0,4,8 / 20,24,36 / 32,40,...}
 *   acc = transform(v, M) + vb;       // -> sp+0xC0 + back to sp+0xE4
 *   d   = acc - va;                   // sp+0x68 (Vec3 sub)
 *   s0->[0x3C..0x44] = scratch Vec3 (write-back of sp+0x8C copy)
 *   len = normalize(&sp+0xD8);        // jal gl_func (sqrt/normalize)
 *   // build sub-frame at p->[0x48]: zero + one product term, 2 gl_func
 *   m1  = s0->[0x4C]; repeat gather+sub+normalize on m1; 2 gl_func
 *   t   = gl_func_result;             // sp+0xAC distance
 *   // 3-way distance clamp (c.lt.s + bc1fl/bc1f):
 *   if (s0->[0x88] < t)      scale = t - s0->[0xA0];
 *   else if (t < s0->[0xA0]) scale = t - s0->[0xA0];
 *   else                     scale = 0.0f;     // f20
 *   sp+0xB0 *= scale;
 *   k = (sp+0xBC - s0->[0xB8]) * s0->[0x118];
 *   sp+0xD8 = sp+0xD8*k + sp+0xB0; gl_func(...);
 * Frame -0x100 saving s0/ra + double $f20 (sdc1 0x18 / ldc1 epilogue). */
/* SnowChaseActor_update: 252-insn (0x3F0) camera/spring vector solve. Callees: game_uso_func_070238
 * (magnitude), game_uso_func_071028 (normalize a Vec3), game_uso_func_072EE8 (accumulate into a node
 * position). EXACT via REPLACE_FUNC_BODY donor splice of the IDO 7.1 NCC (C++) donor
 * src/game_uso/game_uso_ncc_3558.cpp (agent-c run 340, kit CDXXXII), on both build paths; the body
 * below is the splice placeholder. The cfe reference body and its NM history were removed in agent-g
 * run 506 (git e6043cf4d has them). */
void SnowChaseActor_update(char *s0) {
    /* placeholder; the body is spliced from game_uso_ncc_3558.cpp (no rodata / data) */
    s0[0] = 0;
}

void SnowChaseActor_loadParms(SnowChaseActor *a0) {
    GCALL(Parms_load_00035C6C)(&a0->parms);
}

void game_readFloatParm_00003968(char *dst) {
    int tmp;
    stream_readS32_00000280(&tmp);
    stream_readF32_00000244((float*)(dst + 0x10));
}

void game_readVec3Parm_00003998(char *dst) {
    int tmp;
    stream_readS32_00000280(&tmp);
    stream_readVec3_000001D4((Vec3*)(dst + 0x10));
}

void game_readQuadParm_000039C8(char *dst) {
    int tmp;
    stream_readS32_00000280(&tmp);
    stream_readQuad_0000039C((Quad4*)(dst + 0x10));
}

void game_readIntParm_000039F8(char *dst) {
    int tmp;
    stream_readS32_00000280(&tmp);
    stream_readS32_00000280((int*)(dst + 0x10));
}

/* game_ItemType_create_00003A28: item factory of an ItemType (arg0): a 0x40-byte ColourItem
 * (mem_new + ColourItem_ctor, vptr import_8006ED80, +0x3C = 0); when the type's unk40
 * node is set, it becomes the item's child (list_append on children + the parent link).
 *
 * 2026-06-21 CRACKED via masked-twin port from edd_ItemType_create_000003BC
 * (donor, byte-exact). The prior "89.22% NM ceiling / v0-v1 regalloc +
 * frame + jal-swap cap" was a structure+symbol issue: the old body used
 * placeholder gl_func/&D and the simplified obj/other locals. The donor's
 * p2/head/p1 split + p1-reuse-for-arg0 + decl-order (p2,head,p1) + condition
 * re-load forces the two-web spill (frame 0x28) and the a0/v1 coloring.
 * Do NOT simplify the variable reuse or the re-load. */
extern char import_8006ED80;
int *game_ItemType_create_00003A28(int *arg0) {
    int *p2;
    int *head;
    int *p1;
    p1 = (int*)XCALL(game_uso_func_055750, mem_new)(sizeof(ColourSubItem));
    if (p1 != 0) {
        XCALL(game_uso_func_051C28, ColourItem_ctor)(p1);
        NODE(p1)->vptr = (char *)&import_8006ED80;
        ((ColourSubItem *)p1)->unk3C = 0;
    }
    p2 = p1;
    p1 = arg0;
    head = (int*)ITEMTYPE(p1)->unk40;
    if ((int*)ITEMTYPE(p1)->unk40 != 0) {
        XCALL(game_uso_func_07ACE0, list_append)(&NODE(p2)->children, head);
        if (NODE(head)->parent != 0) {
            NODE(head)->unk04 = 1;
        }
        NODE(head)->parent = (Node *)p2;
    }
    return p2;
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_uso/game_uso/game_uso_func_00003A28_pad.s")

/* CourseMarker_registerTemplates: 261-insn (0x414) list builder, EXACT from the IDO
 * 7.1 NCC donor src/game_uso/game_uso_ncc_3AC0.cpp (REPLACE_FUNC_BODY).
 * Twelve `add(arg0, new Item(name_k), -1)` statements: name_k is read from
 * game_uso_D_807FEC70+0x680 .. game_uso_D_807FEC9C+0x6AC, Item is 0x58 bytes
 * (base ctor game_uso_func_051E64(this, name, 0, 0), vptr
 * &game_uso_D_807FEAA8 at +0x28), add = game_uso_func_052F70. cfe capped at
 * 253/261: the shared sp+36 staging slot is the inline Item ctor's by-value
 * parameter pool slot, which only edg produces; the four dead named locals
 * in the donor give the frame its 112 bytes. The placeholder returns pi so
 * the 3ED4 .rodata word the old .s carried (+0x90, 0x40490FDB) stays put.
 * docs/IDO_CODEGEN.md#ncc-3ac0-new-expr-param-pool. Body below is a
 * placeholder for the splice. */
float CourseMarker_registerTemplates(int *a0) {
    volatile int *ret = a0;
    return 3.14159274f; /* keeps math_signedAngleXZ_00003ED4's .rodata pi at +0x90 */
}

/* math_signedAngleXZ_00003ED4: 54-insn FPU geometry helper (yaw-between-vectors).
 * Copies a1's Vec3 to vb and normalizes it via game_uso_func_071028, copies a0's
 * Vec3 to va and normalizes it, then r = game_uso_func_07E330(dot_xz(va,vb)) /
 * D_00000000[0x90]; flips sign of the result by the cross-product test, zeroes
 * *a2 if non-NULL, returns the (possibly negated) result.
 * 2026-06-20 (agent-b): MATCHED byte-exact. Reconstructed to the real callees
 * the .s names (game_uso_func_071028 = the Vec3 normalize; game_uso_func_07E330
 * = the float-returning dot-product consumer). game_uso_func_07E330_ff is a
 * float-prototyped alias (addr 0) that suppresses the K&R float->double arg
 * promotion (no cvt.d.s). The prior 0.56% residual was NOT FP register coloring
 * (as earlier notes guessed) but a frame-layout diff: declaring va immediately
 * after vb (before new_var) packs the two Vec3 stack slots adjacent (va @sp+0x2C,
 * vb @sp+0x38), matching IDO's layout -> 0 diffs. Relocatable USO: intra-module
 * jal emits 0C000000 and the %hi/%lo data ref is base-0 +0x90 (&D_00000000+0x90),
 * relocs applied at load. The `unused` leading float local is load-bearing for
 * the frame size; pad[4] fills the tail slot. */
extern float game_uso_func_07E330_ff(float);  /* float-prototyped alias of game_uso_func_07E330 (addr 0): avoids K&R float->double arg promotion */
float math_signedAngleXZ_00003ED4(Vec3 *a0, Vec3 *a1, int *a2) {
  float unused;
  Vec3 vb;
  Vec3 va;
  char *new_var;
  char pad[4];
  float ret;
  float r;
  new_var = ((char *) (&D_00000000)) + 0x90;
  ret = r;
  (void) unused;
  (void) pad;
  vb = *a1;
  XCALL(game_uso_func_071028, math_vec3Normalize)(&vb);
  va = *a0;
  XCALL(game_uso_func_071028, math_vec3Normalize)(&va);
  r = XCALL(game_uso_func_07E330_ff, math_acosf)((va.x * vb.x) + (va.z * vb.z));
  r = r / (*((float *) new_var));
  ret = r;
  if ((va.z * vb.x) < (va.x * vb.z))
  {
    ret = -r;
  }
  if (a2 != ((void *) 0))
  {
    *a2 = 0;
  }
  return ret;
}

/* math_rotateXZPair_00003FAC: 53-insn (0xD4) FPU-heavy 2D-rotation-like
 * vector builder. Frame -0x20, single cross-USO call.
 *
 * Inputs (decoded from arg-spill + reload pattern):
 *   a0: float[3] output #1 (writes [0], [4]=0, [8])
 *   a1: float[3] output #2 (writes [0], [4]=0, [8])
 *   a2: float[3] input vector (reads [0] and [8] only — [4] unused)
 *   a3: float divisor (o32 mixed-mode ABI passes it in $a3, target mtc1s it)
 *   sp+0x30: float arg5 (numerator for scale = arg5/a3)
 *
 * Algorithm:
 *   orig_scale = arg5 / a3                 ; f0 before call
 *   ratio     = orig_scale * orig_scale    ; f8 = orig_scale^2
 *   inner_arg = 1.0 - ratio                ; f12 (passed to callee)
 *   new_scale = func_00000000(inner_arg, a3)
 *
 *   ; outputs use both new_scale (= post-call f0) and orig_scale (= f2 saved):
 *   a0[0] = new_scale * a2[0] + orig_scale * a2[2]
 *   a0[1] = 0
 *   a0[2] = -orig_scale * a2[0] + new_scale * a2[2]
 *   a1[0] = new_scale * a2[0] - orig_scale * a2[2]
 *   a1[1] = 0
 *   a1[2] = orig_scale * a2[0] + new_scale * a2[2]
 *
 * Looks like a rotation-matrix construction: a0 may be (cos*x + sin*z, 0,
 * sin^2*x + cos*z) and a1 may be the reflected/swapped variant. Inner
 * func is plausibly sqrt or sincos-like (returns derived scale).
 *
 *
 * 2026-05-20 refinement: corrected a3 to mixed-mode float, switched the
 * inner call to a typed zero-address alias returning float, and fixed a0[2]
 * to use -orig_scale instead of ratio. Fuzzy: 52.43% -> 98.23%.
 * 2026-05-29: MATCHED. The 5 residual diffs (3 commutative mul operand swaps +
 * the $f8<->$f10 neg_scale/coord allocation pair) all close by forcing each
 * scale factor into a register AT the multiply via an assignment-expression:
 * `coord * (tmp = scale)` makes IDO take the scale as the mul's fs (the default
 * for `scale * coord` after a fresh coord load is coord-as-fs), and for
 * neg_scale it also lands the target's $f8. The tmp vars (ns/os/nsd/neg) are
 * write-only operand-ordering levers. Found via decomp-permuter, then minimized. */
extern float game_uso_func_082880(float);
void math_rotateXZPair_00003FAC(float *a0, float *a1, float *a2, float a3, float arg4) {
    float orig_scale = arg4 / a3;
    float ns, os, nsd, neg;
    float ratio = orig_scale * orig_scale;
    float new_scale = XCALL(game_uso_func_082880, sqrtf)(1.0f - ratio);
    (void)ratio;
    a0[0] = (orig_scale * a2[2]) + (a2[0] * (ns = new_scale));
    a0[1] = 0.0f;
    a0[2] = (new_scale * a2[2]) + (a2[0] * (neg = -orig_scale));
    a1[0] = (new_scale * a2[0]) - (a2[2] * (os = orig_scale));
    a1[1] = 0.0f;
    a1[2] = (new_scale * a2[2]) + (a2[0] * (nsd = orig_scale));
}

static void stream_readS32_00004080(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

static void stream_readF32_000040BC(float *dst) {
    float buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

static void stream_readVec3_000040F8(Vec3 *dst) {
    int pad_top[1];
    Tri3i raw;
    int pad_mid[2];
    Tri3i tmp;
    int pad_bot[2];
    CALL(fs_gdlReadFile)(&D_00000000, &raw, 12);
#ifdef PORT
    PORT_BE_SWAP32S(&raw, 12);
#endif
    tmp = raw;
    dst->x = *(float*)&tmp.a;
    dst->y = *(float*)&tmp.b;
    dst->z = *(float*)&tmp.c;
}

static void stream_readQuad_00004168(Quad4 *dst) {
    Quad4 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 16);
#ifdef PORT
    PORT_BE_SWAP32S(&buf, 16);
#endif
    *dst = buf;
}

#ifdef NON_MATCHING
/* CourseMarkerTmpl_create: cascading subsystem-init constructor (frame -0x48).
 * Whole-body re-decode 2026-06-24 from expected/.o (size 0x218). alloc(0x8C)
 * + init(051C28) + set vtable slot 0x28; obj = p1+0x3C (realloc(0x50) only if
 * obj==0). Three dead sentinel-guarded sub-stages (obj == -8/-0x20/-0x38,
 * never live since obj = p1+0x3C from an opaque alloc — the family's
 * dead-stage-guard pattern): each loads template = *(D+0x6cX), computes
 * a0=obj+8/+0x20/+0x38, and on the (dead) equal arm alloc(0x18)+register
 * (04A188(obj2,obj,template,flag)) + template-init (incl. 1.0f swc1 in arm 1).
 * Tail: walk a0saved->0x40 node; if non-null link p1+0x10 via 07ACE0, set
 * node+4=1 when node+0x14!=0, then node+0x14 = p1. Returns p1 (v1).
 * 2026-10-17 agent-g (run 106) 61.3 -> 94.2: the "dead sentinel stages" are
 * cfront INLINED constructors on member sub-objects (`this = obj + 8;
 * if (this || (this = alloc(0x18))) { ctor(this, obj, tmpl, 1); ... }`, the
 * `obj == -8` compare is `obj + 8 == 0`), the 8-byte base sub-object is the
 * same shape with its result discarded, and a failed alloc skips only the
 * rest of the ctor -- the node tail always runs (with p1 possibly 0). The
 * template word is a BY-VALUE 4-byte struct (kit VI): per-stage temps t1..t3
 * copied through the int member into one inline parameter `prm`, passed by
 * value (`sw a2,8(sp)` in the jal delay), temps escaped with an if(1) block.
 * RESIDUAL: home-slot layout (target p1/obj/t1..t3 at 0x40..0x30, s/prm at
 * 0x20/0x1C; here 4 higher) and the template loads take v0 (target tN). */
extern char game_uso_D_807FEB68;
extern char game_uso_D_807FECA0;
extern char game_uso_D_807FECB4;
extern char game_uso_D_807FE9B8;
extern char game_uso_D_807FECB8;
extern char game_uso_D_807FEA00;
extern char game_uso_D_807FECBC;
extern int game_uso_func_055750();
extern int game_uso_func_051C28();
extern int game_uso_func_04A188();
extern int game_uso_func_07ACE0();
typedef struct { int w; } T4_41C0;
/* 2026-10-21 agent-g run 116: 94.2 -> 132/134 words with the 3018 recipe
 * (struct copy prm = tN) plus: the new result lives in a reused temp x that
 * later holds the node (both the alloc web and the node are memory-resident
 * at sp+64, so x is escaped), p1 = x copy in the tail (v1, spilled at 68),
 * one escape pointer for all four, and the condition re-loads a0->0x40 while
 * the argument is the earlier load (3A28's trick). Frame and every slot
 * match. LEFT: the target puts the node store `sw v0,64` in the 07ACE0 jal
 * delay slot and the p1 spill before it; here the order is swapped. */
int *CourseMarkerTmpl_create(int a0) {
    char *p1;
    char *x;
    char *obj;
    T4_41C0 t1;
    T4_41C0 t2;
    T4_41C0 t3;
    char *b;
    int pad[2];
    char *s;
    T4_41C0 prm;

    if (1) { void *q = &t1; q = &t2; q = &t3; q = &x; }
    if ((x = (char *)game_uso_func_055750(0x8C)) != 0) {
        game_uso_func_051C28(x);
        ((CourseMarker *)x)->vptr = &game_uso_D_807FEB68;
        obj = x + 0x3C;
        if (obj != 0 || (obj = (char *)game_uso_func_055750(0x50)) != 0) {
            b = obj;
            if (b != 0 || (b = (char *)game_uso_func_055750(8)) != 0) {
                *(char **)(b + 0) = &game_uso_D_807FECA0 + kStr_CourseMarker_Parms_6B0;
                *(int *)(b + 4) = 0;
            }
            t1.w = ((T4_41C0 *)(&game_uso_D_807FECB4 + kTag_wid_6C4))->w;
            prm = t1;
            s = obj + 8;
            if (s != 0 || (s = (char *)game_uso_func_055750(0x18)) != 0) {
                game_uso_func_04A188(s, obj, prm, 1);
                ((CourseParm *)s)->vptr = &game_uso_D_807FE9B8 + kVtbl_FloatParm_3C8;
                ((CourseParm *)s)->value2 = 0;
                ((CourseFParm *)s)->value = 1.0f;
            }
            t2.w = ((T4_41C0 *)(&game_uso_D_807FECB8 + kTag_ext_6C8))->w;
            prm = t2;
            s = obj + 0x20;
            if (s != 0 || (s = (char *)game_uso_func_055750(0x18)) != 0) {
                game_uso_func_04A188(s, obj, prm, 1);
                ((CourseParm *)s)->vptr = &game_uso_D_807FEA00 + kVtbl_IntParm_410;
                ((CourseParm *)s)->value = 0;
                ((CourseParm *)s)->value2 = 0;
            }
            t3.w = ((T4_41C0 *)(&game_uso_D_807FECBC + kTag_flg_6CC))->w;
            prm = t3;
            s = obj + 0x38;
            if (s != 0 || (s = (char *)game_uso_func_055750(0x18)) != 0) {
                game_uso_func_04A188(s, obj, prm, 0);
                ((CourseParm *)s)->vptr = &game_uso_D_807FEA00 + kVtbl_IntParm_410;
                ((CourseParm *)s)->value2 = 0;
                ((CourseParm *)s)->value = 0;
            }
        }
    }
    p1 = x;
    obj = *(char **)(a0 + 0x40);
    if (*(char **)(a0 + 0x40) != 0) {
        game_uso_func_07ACE0(p1 + 0x10, x = obj);
        if (NODE(x)->parent != 0) {
            NODE(x)->unk04 = 1;
        }
        NODE(x)->parent = (Node *)p1;
    }
    return (int *)p1;
}
#else
INCLUDE_ASM("asm/nonmatchings/game_uso/game_uso", CourseMarkerTmpl_create);
#endif

/* CourseMarker_findNearestZ_000043D8: nearest neighbours of a1's value among the owner's
 * children (closest above wins, else closest below). EXACT 2026-10-22
 * (agent-g run 120) via the IDO 7.1 NCC donor src/game_uso/game_uso_ncc_43D8.cpp
 * (REPLACE_FUNC_BODY, both trees): the {cur, nxt} stack iterator with
 * first()/next() is the game_libs 3DB38 class; the two seeds are the
 * unit's +/-FLT_MAX .rodata floats. The old notes here ("dual sp-slot cursor not
 * C-reproducible", "a1 promoted to $s0") were cfe artifacts. This placeholder
 * re-emits the unit's .rodata +0x94/+0x98 words (FLT_MAX, -FLT_MAX), which
 * the donor reads through D_43D8_fmin/fmax (= 0) + those offsets. */
void *CourseMarker_findNearestZ_000043D8(int **a0, int *a1, void *a2) {
    ((float *)a1)[0] = 3.4028235e38f;
    ((float *)a1)[1] = -3.4028235e38f;
    return 0;
}

void CourseMarker_readParms(char *a0) {
    GCALL(ColourItem_load)(a0);
    GCALL(Parms_load_00035C6C)(&((CourseMarker *)a0)->parms);
}

/* SnowAI_ctor (0x1234 bytes, 0xE8 frame): new(0x4E0) when a0 == 0, the base ctor, the 0x3E0 child at +0xE4,
 * then the unrolled build of the AI's parms (INIT_ITER in the body: each alloc'd, built from a template
 * and linked under the AI; the D_44F4_iter*_t / _p / _f externs below are unused leftovers), and last the
 * tunables at 0xA0..0xE0 and 0x4DC = 3 (field names in include/game/snowai.h). Exact (objdiff 100.0); the
 * matching levers (the by-value marshalling struct, 41 named spill structs, -SLOT sentinels, the
 * distinct D_44F4_st12_* externs) are explained at the top of the body. The pre-2026-06-11 decode
 * notes (prologue knobs, per-phase listings) were dropped in agent-g run 516; git history has them. */
extern char D_44F4_iterA_t, D_44F4_iterA_p, D_44F4_iterA_f, D_44F4_iterB_t, D_44F4_iterB_p, D_44F4_iterB_f, D_44F4_iterC_t, D_44F4_iterC_p;
extern char D_44F4_iterD_t, D_44F4_iterD_p, D_44F4_iterD_f, D_44F4_iterE_t, D_44F4_iterE_p, D_44F4_iterE_f, D_44F4_iterF_t, D_44F4_iterF_p;
extern char D_44F4_iterG_t, D_44F4_iterG_p, D_44F4_iterH_t, D_44F4_iterH_p, D_44F4_iterH_f, D_44F4_iterI_t, D_44F4_iterI_p, D_44F4_iterI_f;
extern char D_44F4_iterJ_t, D_44F4_iterJ_p, D_44F4_iterJ_f, D_44F4_iterK_t, D_44F4_iterK_p, D_44F4_iterK_f, D_44F4_iterL_t, D_44F4_iterL_p;
extern char D_44F4_iterL_f, D_44F4_iterM_t, D_44F4_iterM_p, D_44F4_iterM_f, D_44F4_iterN_t, D_44F4_iterN_p, D_44F4_iterN_f, D_44F4_iterO_t;
extern char D_44F4_iterO_p, D_44F4_iterO_f, D_44F4_iterP_t, D_44F4_iterP_p, D_44F4_iterP_f, D_44F4_iterQ_t, D_44F4_iterQ_p, D_44F4_iterQ_f;
extern char D_44F4_iterR_t, D_44F4_iterR_p, D_44F4_iterS_t, D_44F4_iterS_p, D_44F4_iterT_t, D_44F4_iterT_p, D_44F4_iterU_t, D_44F4_iterU_p;
extern char D_44F4_iterV_t, D_44F4_iterV_p, D_44F4_iterW_t, D_44F4_iterW_p, D_44F4_iterX_t, D_44F4_iterX_p, D_44F4_iterY_t, D_44F4_iterY_p;
extern char D_44F4_iterY_f, D_44F4_iterZ_t, D_44F4_iterZ_p, D_44F4_iterAA_t, D_44F4_iterAA_p, D_44F4_iterBB_t, D_44F4_iterBB_p, D_44F4_iterCC_t;
extern char D_44F4_iterCC_p, D_44F4_iterDD_t, D_44F4_iterDD_p, D_44F4_iterDD_f, D_44F4_iterEE_t, D_44F4_iterEE_p, D_44F4_iterEE_f, D_44F4_iterFF_t;
extern char D_44F4_iterFF_p, D_44F4_iterFF_f, D_44F4_iterGG_t, D_44F4_iterGG_p, D_44F4_iterHH_t, D_44F4_iterHH_p, D_44F4_iterII_t, D_44F4_iterII_p;
extern char D_44F4_iterJJ_t, D_44F4_iterJJ_p, D_44F4_iterKK_t, D_44F4_iterKK_p, D_44F4_iterLL_t, D_44F4_iterLL_p, D_44F4_iterMM_t, D_44F4_iterMM_p;
extern char D_44F4_iterNN_t, D_44F4_iterNN_p;
void *SnowAI_ctor(char *a0, int a1, int a2) {
    /* 2026-06-11 STRUCT-MARSHALLING REWRITE (agent-x):
     *  - The marshalling slot (sp+0x2C, held in $s2) is a 4-byte STRUCT
     *    passed BY VALUE as the init call's 3rd arg. Struct-by-value args
     *    are homed to the outgoing-arg area => `sw a2,8(sp)` in the jal
     *    delay slot (probe-verified vs 5th-dup-arg's `sw a2,0x10(sp)`).
     *    Struct locals are addressed via materialized ILDA pointers, which
     *    is (a) the per-iter `addiu tN,sp,slot; lw 0(tN)` indirect-load
     *    shape (struct copy `*s2 = slotK`) and (b) the $s2 promotion
     *    mechanism (high-refcount call-crossing &marsh web).
     *  - The per-iter tmpl spill slots (0xE0 down to 0x40) are 41 NAMED
     *    struct locals; decl order = slot map (FRAME-SLOT HOME ASSIGNMENT
     *    RULE). 48 decls span 0x28-0xE4; colored vars + volatile pads fill
     *    the dead slots (0xE4, 0x3C-0x30, 0x28).
     *  - Sentinel re-derive: target compare const = -SLOT for EVERY iter
     *    (old `SLOT - 0x100` table was an alignment misread). The real C is
     *    `s0 = s1 + SLOT; if (s0 == NULL) alloc;` (IDO folds to
     *    `bne s1, -SLOT`). Same for stage 3 (-0xE4 = -(0xE4)).
     *  - Control flow: iter alloc-failure SKIPS to the NEXT ITER (per-iter
     *    skip labels), alloc(0x3E0) failure goes to the FINALIZE phase,
     *    only alloc(0x4E0) failure returns directly. Return value is always
     *    the a0 home slot (sp+0xE8). */
    typedef struct { char *p; } S;
    volatile int pad_e4;  /* dead slot @0xE4 (colored var in original) */
    S slot_0;             /* @0xE0 — per-iter tmpl marshalling slots */
    S slot_A;
    S slot_B;
    S slot_C;
    S slot_D;
    S slot_E;
    S slot_F;
    S slot_G;
    S slot_H;
    S slot_I;
    S slot_J;
    S slot_K;
    S slot_L;
    S slot_M;
    S slot_N;
    S slot_O;
    S slot_P;
    S slot_Q;
    S slot_R;
    S slot_S;
    S slot_T;
    S slot_U;
    S slot_V;
    S slot_W;
    S slot_X;
    S slot_Y;
    S slot_Z;
    S slot_AA;
    S slot_BB;
    S slot_CC;
    S slot_DD;
    S slot_EE;
    S slot_FF;
    S slot_GG;
    S slot_HH;
    S slot_II;
    S slot_JJ;
    S slot_KK;
    S slot_LL;
    S slot_MM;
    S slot_NN;            /* @0x40 */
    char *v1;             /* colored — dead slot @0x3C */
    char *s1;             /* colored $s1 — dead slot @0x38 */
    char *s0;             /* colored $s0 — dead slot @0x34 */
    volatile int pad_30;  /* dead slot @0x30 */
    S marsh;              /* @0x2C — THE marshalling slot */
    S *s2 = &marsh;       /* colored $s2 — dead slot @0x28 */

    /* Stage 1: allocate main object if not provided */
    if (a0 == NULL) {
        a0 = (char*)CALL(mem_new)(0x4E0);
        if (a0 == NULL) goto epi;
    }

    /* Stage 2: the Node base (name at D[0x6D0]); then the SnowAI vtable at +0x28 */
    CALL(Node_ctor)(a0, (char*)&D_00000000 + kStr_Node_6D0);
    NODE(a0)->vptr = (char *)&D_00000000;

    /* Stage 3: sub-region; alloc failure still runs the finalize phase */
    s1 = a0 + 0xE4;
    if (s1 == NULL) {
        s1 = (char*)CALL(mem_new)(0x3E0);
        if (s1 == NULL) goto finalize;
    }

    /* Stage 4: template-head init; alloc(8) failure skips only the stores */
    v1 = s1;
    if (v1 == NULL) {
        v1 = (char*)CALL(mem_new)(8);
        if (v1 == NULL) goto skip_head;
    }
    ((SnowAIParms *)v1)->name = (char*)&D_00000000 + kStr_SnowAI_Parms_6D8;
    ((SnowAIParms *)v1)->unk04 = 0;
skip_head:

    {
        extern char D_44F4_iter0,
                    D_44F4_iterA, D_44F4_iterB, D_44F4_iterC, D_44F4_iterD,
                    D_44F4_iterE, D_44F4_iterF, D_44F4_iterG, D_44F4_iterH,
                    D_44F4_iterI, D_44F4_iterJ, D_44F4_iterK, D_44F4_iterL,
                    D_44F4_iterM, D_44F4_iterN, D_44F4_iterO, D_44F4_iterP,
                    D_44F4_iterQ, D_44F4_iterR, D_44F4_iterS, D_44F4_iterT,
                    D_44F4_iterU, D_44F4_iterV, D_44F4_iterW, D_44F4_iterX,
                    D_44F4_iterY, D_44F4_iterZ, D_44F4_iterAA, D_44F4_iterBB,
                    D_44F4_iterCC, D_44F4_iterDD, D_44F4_iterEE, D_44F4_iterFF,
                    D_44F4_iterGG, D_44F4_iterHH, D_44F4_iterII, D_44F4_iterJJ,
                    D_44F4_iterKK, D_44F4_iterLL, D_44F4_iterMM, D_44F4_iterNN;
        /* Per-iter unique externs (DB) defeat IDO's global CSE on the
         * &D base — target reloads lui per access. */
/* one SnowAIParm: s1 = the embedded SnowAIParms (this + 0xE4); IDX = its SNOWAI_PARM_* index, which also picks
 * its 4-char tag word (game_uso_data.h kSnowAIParmTags_6E8 + 4 * IDX, read by value); the game_libs parm ctor 0x35B1C(parm, owner, tag, 1), then the
 * float-parm vtable (Data 0x3C8) and the default value. The two calls are named by hand (name-placeholder-calls.py
 * does not see inside a #define): the ROM TextRelocs of SnowAI_ctor's 82 macro jals are 41 x (mem_new,
 * Parms_addParm), in that order (agent-g run 512) */
#define INIT_ITER(ID, IDX, FLOAT_EXPR, DB) \
        slot_##ID.p = *(char**)((char*)&DB + kSnowAIParmTags_6E8 + 4 * (IDX)); \
        *s2 = slot_##ID; \
        s0 = (char *)&((SnowAIParms *)s1)->parm[IDX]; \
        if (s0 == NULL) { \
            s0 = (char*)CALL(mem_new)(0x18); \
            if (s0 == NULL) goto skip_##ID; \
        } \
        CALL(Parms_addParm)(s0, s1, *s2, 1); \
        ((SnowAIParm *)s0)->vptr = (char*)&DB + kVtbl_FloatParm_3C8; \
        ((SnowAIParm *)s0)->unk14 = 0; \
        ((SnowAIParm *)s0)->value = (FLOAT_EXPR); \
skip_##ID:;

        INIT_ITER(0,  SNOWAI_PARM_DE + 0, *(float*)((char*)&D_44F4_iter0 + kRoF_m10000_9C), D_44F4_iter0)   /* iter 0 */
        INIT_ITER(A,  SNOWAI_PARM_DE + 1, *(float*)((char*)&D_44F4_iterA + kRoF_m10000_A0), D_44F4_iterA)   /* A */
        INIT_ITER(B,  SNOWAI_PARM_DE + 2, *(float*)((char*)&D_44F4_iterB + kRoF_m10000_A4), D_44F4_iterB)   /* B */
        INIT_ITER(C,  SNOWAI_PARM_BD + 0, -800.0f,                                D_44F4_iterC)   /* C (lui 0xC448) */
        INIT_ITER(D,  SNOWAI_PARM_BD + 1, *(float*)((char*)&D_44F4_iterD + kRoF_m3000_A8), D_44F4_iterD)   /* D */
        INIT_ITER(E,  SNOWAI_PARM_BD + 2, *(float*)((char*)&D_44F4_iterE + kRoF_m15000_AC), D_44F4_iterE)   /* E */
        INIT_ITER(F,  SNOWAI_PARM_BT + 0, -4000.0f,                               D_44F4_iterF)   /* F (lui 0xC57A) */
        INIT_ITER(G,  SNOWAI_PARM_BT + 1, -8000.0f,                               D_44F4_iterG)   /* G (lui 0xC5FA) */
        INIT_ITER(H,  SNOWAI_PARM_BT + 2, *(float*)((char*)&D_44F4_iterH + kRoF_m15000_B0), D_44F4_iterH)   /* H */
        INIT_ITER(I,  SNOWAI_PARM_BU + 0, *(float*)((char*)&D_44F4_iterI + kRoF_m10000_B4), D_44F4_iterI)   /* I */
        INIT_ITER(J,  SNOWAI_PARM_BU + 1, *(float*)((char*)&D_44F4_iterJ + kRoF_m10000_B8), D_44F4_iterJ)   /* J */
        INIT_ITER(K,  SNOWAI_PARM_BU + 2, *(float*)((char*)&D_44F4_iterK + kRoF_m10000_BC), D_44F4_iterK)   /* K */
        INIT_ITER(L,  SNOWAI_PARM_BJ + 0, *(float*)((char*)&D_44F4_iterL + kRoF_m10000_C0), D_44F4_iterL)   /* L */
        INIT_ITER(M,  SNOWAI_PARM_BJ + 1, *(float*)((char*)&D_44F4_iterM + kRoF_m10000_C4), D_44F4_iterM)   /* M */
        INIT_ITER(N,  SNOWAI_PARM_BJ + 2, *(float*)((char*)&D_44F4_iterN + kRoF_m10000_C8), D_44F4_iterN)   /* N */
        INIT_ITER(O,  SNOWAI_PARM_CF + 0, *(float*)((char*)&D_44F4_iterO + kRoF_m10000_CC), D_44F4_iterO)   /* O */
        INIT_ITER(P,  SNOWAI_PARM_CF + 1, *(float*)((char*)&D_44F4_iterP + kRoF_m10000_D0), D_44F4_iterP)   /* P */
        INIT_ITER(Q,  SNOWAI_PARM_CF + 2, *(float*)((char*)&D_44F4_iterQ + kRoF_m10000_D4), D_44F4_iterQ)   /* Q */
        INIT_ITER(R,  SNOWAI_PARM_DD0 + 0, 1200.0f,                                D_44F4_iterR)   /* R (lui 0x4496) */
        INIT_ITER(S,  SNOWAI_PARM_DD0 + 1, 1200.0f,                                D_44F4_iterS)   /* S */
        INIT_ITER(T,  SNOWAI_PARM_DD0 + 2, 1200.0f,                                D_44F4_iterT)   /* T */
        INIT_ITER(U,  SNOWAI_PARM_DD1 + 0, 60.0f,                                  D_44F4_iterU)   /* U (lui 0x4270) */
        INIT_ITER(V,  SNOWAI_PARM_DD1 + 1, 60.0f,                                  D_44F4_iterV)   /* V */
        INIT_ITER(W,  SNOWAI_PARM_DD1 + 2, 60.0f,                                  D_44F4_iterW)   /* W */
        INIT_ITER(X,  SNOWAI_PARM_DD2 + 0, 1.5f,                                   D_44F4_iterX)   /* X (lui 0x3FC0) */
        INIT_ITER(Y,  SNOWAI_PARM_DD2 + 1, *(float*)((char*)&D_44F4_iterY + kRoF_1p1_D8), D_44F4_iterY)   /* Y */
        INIT_ITER(Z,  SNOWAI_PARM_DD2 + 2, 1.5f,                                   D_44F4_iterZ)   /* Z */
        INIT_ITER(AA, SNOWAI_PARM_DP + 0, 2000.0f,                                D_44F4_iterAA)  /* AA (lui 0x44FA) */
        INIT_ITER(BB, SNOWAI_PARM_DP + 1, 2000.0f,                                D_44F4_iterBB)  /* BB */
        INIT_ITER(CC, SNOWAI_PARM_DP + 2, 2000.0f,                                D_44F4_iterCC)  /* CC */
        INIT_ITER(DD, SNOWAI_PARM_SE + 0, *(float*)((char*)&D_44F4_iterDD + kRoF_m6000_DC), D_44F4_iterDD) /* DD */
        INIT_ITER(EE, SNOWAI_PARM_SE + 1, *(float*)((char*)&D_44F4_iterEE + kRoF_m6000_E0), D_44F4_iterEE) /* EE */
        INIT_ITER(FF, SNOWAI_PARM_SE + 2, *(float*)((char*)&D_44F4_iterFF + kRoF_m6000_E4), D_44F4_iterFF) /* FF */
        INIT_ITER(GG, SNOWAI_PARM_MS + 0, 240.0f,                                 D_44F4_iterGG)  /* GG (lui 0x4370) */
        INIT_ITER(HH, SNOWAI_PARM_MS + 1, 240.0f,                                 D_44F4_iterHH)  /* HH */
        INIT_ITER(II, SNOWAI_PARM_MS + 2, 240.0f,                                 D_44F4_iterII)  /* II */
        INIT_ITER(JJ, SNOWAI_PARM_MS + 3, 240.0f,                                 D_44F4_iterJJ)  /* JJ */
        INIT_ITER(KK, SNOWAI_PARM_MS + 4, 240.0f,                                 D_44F4_iterKK)  /* KK */
        INIT_ITER(LL, SNOWAI_PARM_MS + 5, 240.0f,                                 D_44F4_iterLL)  /* LL */
        INIT_ITER(MM, SNOWAI_PARM_MS + 6, 240.0f,                                 D_44F4_iterMM)  /* MM */
        INIT_ITER(NN, SNOWAI_PARM_MS + 7, 240.0f,                                 D_44F4_iterNN)  /* NN (final iter) */
#undef INIT_ITER
    }
finalize:;

    /* Stage 5+ (deprecated doc — see Stages 8-11 for full per-iter
     * unrolled loop characterization; all 38 iters TBD as C body): write sub-region back-pointer; init sub-region from
     * template; loop-init sub-object table at a0+0x2C with stride 8.
     * Decoded ~25 insns at 0x4564-0x45D8:
     *   *(int*)(s1) = 0;            // s1->0 = NULL
     *   *(int*)(s1 + 4) = 0;        // s1->4 = NULL
     *   gl_func_00000000(s1, 8);    // small init call
     *   for (s0 = a0 + 0x2C; ...) {
     *     ptr = (a0+0xE0)->...;     // load template entry
     *     *(int*)s0 = ptr;
     *     gl_func_00000000(s0, 0x18);  // init sub-obj of size 0x18
     *     s0 += 8;
     *   }
     */
    /* Stage 5 (extended 2026-05-03, ~25 insns 0x45D8-0x4640):
     * After init-sub-obj jal, set 3 more fields per sub-obj from data
     * templates at &D + various offsets:
     *
     *   gl_func_00000000(s0, 0x18);
     *   *(int*)(s0 + 0xC)  = (int)((char*)&D_00000000 + 0x3C8);
     *   *(int*)(s0 + 0x14) = 0;
     *   *(float*)(s0 + 0x10) = *(float*)((char*)&D_00000000 + 0x9C);
     *   t8 = *(int*)((char*)&D_00000000 + 0x6EC);  // list head
     *   ... (continues with more loads + 2 more cross-USO calls
     *   at 0x4620 and 0x4640, decoding pending) */

    /* Stage 7 confirmation (2026-05-03 run): the 0x44F4-0x47F4 region is a
     * LOOP over sub-objects — Stages 4-6 are the per-iteration body, repeated
     * with rotating template offsets D[0x6EC], D[0x6F4], etc. (gap 8 bytes per
     * iter = next-template ptr). Estimated 16 iters total before the loop exits
     * to the outer constructor's link-setup phase at ~0x47F4.
     *
     * Stage 6 (extended 2026-05-03 run 13, ~16 insns 0x4630-0x4680):
     *   gl_func_00000000(s0, s1, *(int*)s2, 1);   // 4-arg sub-init call
     *   s0->[0xC] = (int)((char*)&D_00000000 + 0x3C8);  // template ptr (same as stage 5)
     *   s0->[0x14] = 0;
     *   s0->[0x10] = *(float*)((char*)&D_00000000 + 0xA0);  // float scalar
     *   if (s1 == (char*)-0x38) { alloc slot }     // unusual addr-as-imm cmp
     *
     * The 4-arg call here mirrors the structure of Stage 4's gl_func call
     * but with explicit (a0, a1, a2, a3) — different sub-init shape.
     *
     * Stage 7 (extended 2026-05-04, ~30 insns 0x4680-0x4734):
     * Confirms the loop is UNROLLED — each iteration:
     *   1. Compute s0 = s1 + N*0x50 (N=0,1,2,...; sub-obj slot in s1[])
     *   2. Load next template ptr from D[0x6F4 + N*4]
     *   3. alloc(0x18) → s0 (24-byte sub-obj)
     *   4. gl_func(s0, s1, *(int*)s2, 1)   ; 4-arg sub-init
     *   5. s0->[0xC] = (int)((char*)&D + 0x3C8)  ; template ptr (constant)
     *   6. s0->[0x14] = 0
     *   7. s0->[0x10] = D[0xA0..]    ; float scalar (varies per iter:
     *      D[0xA4]=val_iter1, -800.0f=iter2 via lui 0xC448, etc.)
     *
     * Stride: s0 advances by 0x18 (24 bytes) per iteration — the sub-obj
     * size, NOT 0x50 (Stage 7's earlier read was wrong; the 0x50 was a
     * one-off iter that didn't establish the stride). The s1 buffer is
     * 0x3E0 = 992 bytes total, fitting ~40 sub-obj slots if dense, but
     * the structure is sparser — only the slots at +0x20, +0x38, +0x50,
     * +0x68 are confirmed unrolled iters.
     *
     * Stage 8 (extended /decompile run, 2026-05-04, ~88 insns 0x4600-0x47C8):
     * 4 fully-characterized iterations of the unrolled-loop body. Each
     * iter is 22 insns / 0x58 bytes following this exact template:
     *
     *   1. lui+lw $tN, %hi/lo(D + tmpl_off_N)        ; template-N ptr
     *   2. addiu $tM, $sp, arg_slot_N                ; arg slot ptr (sp-relative)
     *   3. addiu $at, $zero, sentinel_N              ; sentinel constant
     *   4. sw $tN, arg_slot_N(sp)                    ; spill tmpl ptr to arg slot
     *   5. lw $tK, 0($tM)                            ; reload via arg slot
     *   6. addiu $s0, $s1, slot_off_N                ; s0 = s1 + slot offset N
     *   7. bne $s1, $at, +5                          ; sentinel check on s1
     *   8. sw $tK, 0($s2)                            ; (delay) init s2[0]
     *   9. jal gl_func_00000000                      ; alloc(0x18)
     *   10. addiu $a0, $zero, 0x18                   ; (delay)
     *   11. beqz $v0, epi                            ; bail on alloc fail
     *   12. or $s0, $v0, $zero                       ; (delay) s0 = alloc result
     *   13. lw $a2, 0($s2)                           ; reload tmpl ptr arg
     *   14. or $a0, $s0, $zero                       ; a0 = s0
     *   15. or $a1, $s1, $zero                       ; a1 = s1
     *   16. addiu $a3, $zero, 1                      ; a3 = 1
     *   17. jal gl_func_00000000                     ; 4-arg sub-init
     *   18. sw $a2, 0x8(sp)                          ; (delay) caller arg slot
     *   19. lui+addiu $tQ, %hi/lo(D + 0x3C8)         ; constant template ptr
     *   20. sw $tQ, 0xC(s0)                          ; s0->[0xC] = D+0x3C8
     *   21. sw $zero, 0x14(s0)                       ; s0->[0x14] = 0
     *   22. lui+lwc1+swc1 $fK, ... 0x10(s0)          ; s0->[0x10] = float scalar
     *
     * Per-iteration parameter table (4 iters: A B C D, with iter 0 being
     * the kick-off block at 0x45D8-0x4640 already documented in Stages 5-7):
     *
     *   iter | tmpl_off | arg_slot | sentinel | slot_off |  float_scalar
     *   -----+----------+----------+----------+----------+--------------
     *     A  |  0x6EC   |  0xDC    |  -0xE0   |  0x20    |  D + 0xA0
     *     B  |  0x6F0   |  0xD8    |  -0xC8   |  0x38    |  D + 0xA4
     *     C  |  0x6F4   |  0xD4    |  -0xB0   |  0x50    |  -800.0f literal
     *                                                       (lui 0xC448)
     *     D  |  0x6F8   |  0xD0    |  -0x98   |  0x68    |  D + 0xA8
     *
     * Patterns confirmed:
     *  - tmpl_off advances +4 per iter (D's table of template ptrs is dense)
     *  - arg_slot decreases by 4 per iter (stack spills grow downward
     *    as live ranges of earlier tmpl ptrs end)
     *  - sentinel = slot_off - 0x100 (sign-extended low half) — IDO
     *    rewrites `s1 + slot == magic_addr` as `s1 == magic_addr - slot`
     *    to fit the comparison constant in 16-bit imm
     *  - slot_off advances +0x18 (sub-obj size) per iter — sub-objs are
     *    laid out CONTIGUOUSLY in s1[]; offsets 0x20, 0x38, 0x50, 0x68
     *    are slot starts at indices 0, 1, 2, 3 within s1's sub-obj array
     *  - float scalar varies per iter: D+0xA0..0xA8 table OR literal
     *    constants (e.g. -800.0f at iter C). Suggests these are physics
     *    parameters (init pos? init velocity? gravity scale?) per sub-obj
     *
     * Stage 9 starts at 0x47C8 (5th unrolled iter, slot_off ~0x80,
     * tmpl_off 0x6FC, sentinel -0x80, arg_slot 0xCC). Pending decode.
     *
     * Cumulative ~170/1165 insns characterized (~15%). Loop has at least
     * 6+ more iters before exiting to parent constructor's link-setup
     * phase. The repeating 22-insn structure means Stage 9-12 should
     * each be a fast scan-and-document pass.
     *
     * Stage 9 correction (2026-05-04 run): the formula in Stage 8 should
     * read `sentinel == slot - 0x100` (sign-extended), where MAGIC is
     * `(char*)0xFFFFFF00`. Verified iters A-D: -0xE0=0x20-0x100,
     * -0xC8=0x38-0x100, -0xB0=0x50-0x100, -0x98=0x68-0x100. The C test
     * IDO rewrites is `(s1 + slot) == (char*)0xFFFFFF00`, regrouped as
     * `s1 == (char*)0xFFFFFF00 - slot` so the addiu imm fits 16-bit
     * signed.
     *
     * Stage 9 (~70 insns covering 0x47B0-0x48E0): iters E, F, G, H
     * observed:
     *
     *   iter | tmpl_off | arg_slot | sentinel-expected | slot | float_scalar
     *   -----+----------+----------+-------------------+------+--------------
     *     E  |  0x6FC   |  0xCC    |  -0x80            | 0x80 | literal
     *     F  |  0x700   |  0xC8    |  -0x68            | 0x98 | D + 0xAC
     *     G  |  0x704   |  0xC4    |  -0x50            | 0xB0 | literal -800
     *                                                          (lui 0xC57A)
     *     H  |  0x708   |  0xC0    |  -0x38            | 0xC8 | literal
     *                                                          (lui 0xC5FA)
     *
     * Pattern continues to hold: tmpl_off advances +4, arg_slot
     * decreases -4, slot advances +0x18 per iter. Float scalars
     * alternate between D-table loads (0xA0..0xAC) and literal
     * constants (mtc1 from lui-built immediate). The literal pattern
     * (lui 0xC57A → mtc1 → swc1) at iters G/H suggests these specific
     * sub-objs have hardcoded physics parameters not stored in the
     * D-template-table.
     *
     * Cumulative ~240/1165 insns characterized (~21%). Stage 10 starts
     * at 0x48E0 with iter I (predicted slot 0xE0, arg_slot 0xBC,
     * tmpl_off 0x70C, sentinel -0x20).
     *
     * Stage 10 (~280 insns covering 0x48E0-0x4DC0): iters I through T
     * verified by reading the tmpl_off lw imm and float-scalar lwc1/
     * mtc1 at each iter's footer. Pattern continues unbroken through
     * iter T at slot 0x1E8 (= s1 + 0x20 + 16 * 0x18). Per-iter:
     *
     *   iter | tmpl_off | float_scalar
     *   -----+----------+------------------
     *     I  |  0x70C   | (?)
     *     J  |  0x710   | D + 0xB4
     *     K  |  0x714   | D + 0xB8
     *     L  |  0x718   | D + 0xBC
     *     M  |  0x71C   | D + 0xC0
     *     N  |  0x720   | (?)
     *     O  |  0x724   | (?)
     *     P  |  0x728   | D + 0xCC
     *     Q  |  0x72C   | D + 0xD0
     *     R  |  0x730   | D + 0xD4
     *     S  |  0x734   | literal +1200.0f (lui 0x4496)
     *     T  |  0x738   | literal +1200.0f (lui 0x4496)
     *
     * The literal-pattern at iters S/T (lui 0x4496 → mtc1 from $at →
     * swc1) reuses the SAME constant +1200.0f for both. The earlier
     * literal iters (G/H lui 0xC57A/0xC5FA, C lui 0xC448) used
     * DIFFERENT constants per iter.
     *
     * arg_slot continues to decrease by 4: 0xCC, 0xC8, 0xC4, 0xC0, 0xBC,
     * 0xB8, 0xB4, 0xB0, 0xAC, 0xA8, 0xA4, 0xA0, 0x9C, 0x98, 0x94, 0x90,
     * 0x8C — runs out of frame at iter ~17, suggesting the loop body
     * exits or transitions before then. arg_slot 0x8C - 4 = 0x88 puts
     * us close to the s0/s1/s2 spill area (sp+0x18 region of the
     * 0xE8-byte frame), so the unroll likely ends around iter 17-20.
     *
     * Cumulative ~520/1165 insns characterized (~45%). Stage 11 starts
     * at 0x4DC0 with iter U+. The 0x4DC0+ region has another ~5-6 iters
     * before hitting the loop exit / link-setup phase at ~0x4F00.
     *
     * Stage 11 (~480 insns covering 0x4DC0-0x563C): iters U through Z+
     * — the unrolled loop continues to slot 0x3C8 (= 0x1E8 + 0x1E0,
     * which is +20 more iters from iter T). Total unrolled iter count
     * is ~32, NOT the previously-estimated 17-20. The arg_slot
     * decreases past the spill-slots boundary because the spill chain
     * reuses the same 0xE0-byte frame down to ~0x40 — fits up to 32
     * iters. Float scalars in this region are dominated by hardcoded
     * literal constants (0x4496=+1200, 0x4370=+240, 0x44FA=+2000,
     * 0x4140=+12, 0x43FA=+500, 0x4366=+230, 0x428C=+70, 0x41A0=+20,
     * 0x4170=+15, 0x4248=+50, 0x4000=+2.0, 0x3FC0=+1.5, 0x447A=+1000).
     * Some iters store the SAME literal to multiple sub-objs in a
     * row — likely setting batched physics constants per sub-obj group.
     *
     * Stage 12 (~80 insns covering 0x563C-0x5710): FINAL FIELD-INIT
     * phase after the unrolled loop exits. Pattern shifts from "alloc
     * + init sub-obj" to "store fixed values into a0's main struct
     * fields":
     *
     *   v0 = a0 (reload from sp+0xE8)
     *   t8 = a1 (reload from sp+0xEC)
     *   f18 = 0.0
     *   *(int*)(v0 + 0x30) = t8                ; a0->0x30 = a1
     *   t9 = a2 (reload from sp+0xF0)
     *   f0  = +500.0f literal (lui 0x43FA)
     *   *(float*)(v0 + 0xA8) = f18              ; a0->0xA8 = 0.0
     *   *(int*)(v0 + 0x2C) = t9                  ; a0->0x2C = a2
     *   f4 = D[0xE8] (float)
     *   f6 = +70.0f literal (lui 0x428C)
     *   f8 = +230.0f literal (lui 0x4366)
     *   *(float*)(v0 + 0xB0) = f0
     *   *(float*)(v0 + 0xAC) = f4
     *   *(float*)(v0 + 0xB4) = f6
     *   *(float*)(v0 + 0xB8) = f8
     *   fA = D[0xEC]
     *   fC = +12.0f literal (lui 0x4140)
     *   *(float*)(v0 + 0xBC) = fA
     *   ... (continues for 0xC0..0xE0 fields with mix of D-loads and
     *   literals: +2.0 lui 0x4000, +50.0 lui 0x4248, +20.0 lui 0x41A0,
     *   +15.0 lui 0x4170)
     *   *(int*)(v0 + 0x4DC) = const               ; final field
     *
     * Stage 13 (~8 insns 0x5710-0x5728): EPILOGUE.
     *   v0 = a0 (already loaded from sp+0xE8 above; carries through)
     *   $s0 = lw 0x18(sp)
     *   $s1 = lw 0x1C(sp)
     *   $s2 = lw 0x20(sp)
     *   jr  $ra
     *   addiu $sp, $sp, 0xE8
     *
     * Cumulative 1165/1165 insns characterized (~100%). The full
     * function structure is now mapped. C-body decomp work ahead:
     *
     *   - Prologue: ~28 insns (Stages 1-3) — partially written in body
     *   - Iter 0 init + sub-region setup: ~40 insns (Stage 4) — partial
     *   - Unrolled main loop: ~700 insns (Stages 5-11, 32 iters of
     *     ~22 insns each) — only Stages 5-7 (3 iters) currently in
     *     body; needs explicit unrolled writeout
     *   - Final field init: ~80 insns (Stage 12) — needs body
     *   - Epilogue: ~8 insns (Stage 13) — auto-emitted by IDO
     *
     * 2026-05-05 STATUS: macro INIT_ITER(SLOT, TMPL_OFF, FLOAT_EXPR)
     * implemented with all 41 iters expanded, plus Stage 12 finalize
     * phase as straight C stores. Achieved 61.61% fuzzy (from 2.46%
     * baseline this session). This appears to be the structural cap
     * for the macro-only approach.
     *
     * RESIDUAL 38pp gap analysis:
     *   - Mine: 921 insns. Expected: 1165 insns. Delta: 244 insns short.
     *   - Per-iter delta: ~6 insns (mine 21/iter, expected 25/iter).
     *   - Missing per-iter: (a) per-iter `lui $at, hi(D+0xN)` reload of
     *     D base for the float scalar (mine CSE's D base into $s3 once);
     *     (b) per-iter sp-relative arg-buffer spill pattern (mine uses
     *     local _t var that IDO keeps in $tN, no stack spill).
     *   - Frame size mismatch: mine -0x28, expected -0xE8 — directly
     *     caused by missing per-iter spill slots.
     *
     * Failed experiments (both regressed slightly):
     *   - `char *_arg_buf; char **_arg_ptr = &_arg_buf;` indirect
     *     pattern in macro: IDO optimized away the indirection; -1.6pp
     *   - `volatile char pad[200]; pad[0]=0;` to force frame size:
     *     IDO allocated 0x1B0 frame (TOO big), shifted everything;
     *     -0.6pp
     *
     * 2026-05-05 UPDATE: applied feedback_unique_extern_with_offset_cast_
     * breaks_cse.md recipe — declared 99 unique externs in
     * undefined_syms_auto.txt (D_44F4_iterX_{t,f,p}, all at 0x0),
     * replaced INIT_ITER with INIT_ITER_U (D-source iters) and
     * INIT_ITER_UC (const-float iters). Result: 61.61% → 63.33% (+1.72pp).
     *
     * Per-iter gain measured smaller than projected (~0.04pp/iter vs
     * predicted ~0.07pp/iter). Most of the residual gap is the per-iter
     * sp-relative arg-buffer spill pattern (separate cap class, would
     * need unique-pseudo or volatile-ptr-to-arg trick — see
     * feedback_volatile_ptr_to_arg_forces_caller_slot_spill.md).
     *
     * Remaining cap is structural: target uses extra stack slots
     * (sp+0xE0 per-iter scratch, sp+0xE8/0xEC/0xF0 for a0/a1/a2 spill
     * at entry) that mine doesn't emit. Each iter has a store-then-load
     * pattern at sp+0xE0 — `sw t1, 0xE0(sp); lw t4, 0(t2)` where
     * `t2 = sp+0xE0`. Looks like a varargs marshalling buffer or
     * deliberate CSE-defeating spill in original source.
     *
     * Tried 2026-05-05: `volatile char *_t = *(char**)(...)` in the
     * macro body — added 59 insns (3700 → 3936 bytes) but at wrong
     * positions; fuzzy unchanged at 63.33%.
     *
     * 2026-05-05 (later): THREE structural fixes that worked:
     *   1. `char *_t_buf[1];` local + macro `_t_buf[0] = *(char**)(...)`
     *      → IDO emits per-iter store-then-load through stack at sp+N,
     *      matching target's sp+0xE0 pattern. (+6.11pp)
     *   2. Changed s2 from `a0 + 0x2C` (struct member) to a stack-temp
     *      via `char *_s2_buf; char *s2 = (char*)&_s2_buf;`. Target
     *      had `addiu s2, sp, 0x2C` not `addiu s2, a0, 0x2C`. (+0.35pp)
     *   3. Added `char _pad[N];` to grow frame from 0x38 to 0xE8 (matching
     *      target). 2026-05-28: N is now 160 (was 168 → frame 0xF0/240, off by
     *      8 after later local churn); 160 gives `addiu sp,-232`. a0/a1/a2
     *      spill at sp+0xE8/0xEC/0xF0
     *      (matches target). +0.005pp.
     *
     * Result: 63.33% → 69.79%. Still ~30pp short.
     *
     * REMAINING CAP: target saves 3 $s-regs (s0/s1/s2/ra at sp+0x18/
     * 0x1C/0x20/0x24), mine saves only 2 (s0/s1/ra at sp+0x14/0x18/
     * 0x1C). My s2 is a stack-pointer not $s-promoted.
     *
     * 2026-05-05 (later): tried hoisting `s2` from inner block scope
     * to function scope (longer live range). Did NOT trigger $s2
     * promotion — frame size shrank to 0xE0 instead of growing,
     * fuzzy unchanged at 69.79%. The IDO global allocator picked
     * different priorities; my s2 still went to $tN.
     *
     * Target's s2 is `addiu s2, sp, 0x2C` set ONCE at 0x45A0 and
     * used across ~0x1180 bytes of function body (live until 0x5720
     * epilogue). To trigger $s2 promotion in IDO, need a value with
     * similarly long live range AND high ref count. The candidates
     * are limited — the marshalling-buffer base ptr is the obvious
     * one, but my macro structure already references it via s2.
     *
     * Promotion paths (multi-tick):
     *   - Move s2's `(char*)&_s2_buf` setup to LATER in the function
     *     so its live range starts after a few jal calls (could
     *     trick IDO into picking $s2 over $t). NOT YET TRIED — s2 is
     *     used in inner Stage 4 block at line 1811, can't easily move
     *     past first use without restructuring.
     *   - `register char *s2 = ...` keyword hint. TRIED 2026-05-05:
     *     IDO accepted the syntax (no error like the asm("$N") form),
     *     but did NOT change allocation — still goes to $tN. Confirmed
     *     via objdump: prologue still saves only s0/s1/ra, no s2.
     *     Conclusion: IDO's `register` hint is too weak to override
     *     the global allocator's priority calc here. RETEST 2026-06-01
     *     on the current 0xE8-frame body: same result. `register char *s2`
     *     rebuilt cleanly, but no-alias objdump still emitted only
     *     `sw ra,0x1c(sp)`, `sw s1,0x18(sp)`, `sw s0,0x14(sp)`; no
     *     `sw s2,0x20(sp)`. Do not retry without a separate live-range
     *     change.
     *   - Slot-shift via `_s2_buf[3]; s2 = &_s2_buf[2]`. TRIED 2026-06-01:
     *     with `_pad[160]`, frame stayed 0xE8 but IDO still used direct
     *     `sp+0x24` stores/loads for the marshalling slot (no `sp+0x2c`,
     *     no `$s2`). With `_pad[152]`, frame shrank to 0xE0 and moved
     *     caller-arg spills to sp+0xE0/0xE4/0xE8, a clear regression.
     *     Reverted; array indexing does not force the target stack slot
     *     or promotion.
     *   - SOURCE=5 retest 2026-06-01 on current branch:
     *     `(void)&a0; (void)&a1; (void)&a2;` with the volatile finalizer
     *     locals was ignored by IDO for this shape: match stayed 69.72618%,
     *     prologue still stored a1/a2 to local slots sp+0x34/sp+0x30, and
     *     still saved only s0/s1/ra. Removing volatile from a1_sp/a2_sp did
     *     force caller-shadow homes for a1/a2 at sp+0xEC/sp+0xF0, but regressed
     *     to 69.25751% and still emitted no s2 save. Hoisting `_s2_buf`/`s2`
     *     to function scope on top of that shrank the frame to 0xD8 and
     *     regressed to 69.248924%. Keep the volatile-local baseline until a
     *     real alias source for the long-lived sp+0x2C marshalling pointer is
     *     found.
     *   - decomp-permuter with PERM_RANDOMIZE around the macros
     *
     * 2026-05-05 (later): FIXED iter-G-NN macro bug. The macro at
     * line 1864 had `*(char**)s2 = _t_buf[0]` but never wrote `_t_buf[0]`
     * — so the load was reading uninitialized stack. Adding the missing
     * `_t_buf[0] = _t;` store before the read promoted iters G-NN's
     * codegen by ~32 SW instructions (one per iter). Net effect:
     * 62.05% → 69.48% (+7.43pp). 604 bytes / 151 insns deficit remain.
     *
     * 2026-05-05 (next run): Extended the marshalling-buffer pattern
     * to iters A-F (the FIRST INIT_ITER macro at line 1827) — also
     * needs `_t_buf[0] = _t; *(char**)s2 = _t_buf[0];` for the per-
     * iter store-then-load through sp+0xE0. +0.32pp (69.48 → 69.80).
     *
     * 2026-05-05 (negative): Tried the same fix on Stage 4's iter-0
     * setup (`tmpl0 = ...; *(char**)s2 = tmpl0;` at line 1811) — it
     * REGRESSED (-0.05pp). Iter-0 is special; target uses direct store
     * for iter 0 but marshalling for iters A onward. Don't extend the
     * pattern there.
     *
     * 2026-05-08: Stage 12 arg-finalizer nudge. Plain locals for a1/a2
     * did not change C emit, but volatile locals force late finalizer
     * stores through stack-resident values. Fresh objdiff after rebuilding
     * build/non_matching/src/game_uso/game_uso.c.o moved the C body from
     * 69.311584% to 69.577680%. */
    /*
     * 2026-06-01 SOURCE=5 improvement: split the init-call's third argument
     * into an explicit `char *arg2 = *(char**)s2;` local in iter0 and both
     * INIT_ITER macro bodies. This nudges IDO to materialize the template
     * reload before the call instead of keeping the `a3=1` setup as the call
     * delay-slot candidate. Direct objdiff improved 69.72618% -> 71.84635%;
     * refreshed report.json scores the function at 72.638626%. Still NM: the
     * long-lived sp+0x2C marshalling pointer is not promoted to $s2, so the
     * target's `lw a2,0(s2); ...; jal; sw a2,8(sp)` shape is only partly
     * recovered.
     *
     * 2026-06-01 SOURCE=5 follow-up: pass that same template pointer as a
     * duplicate fifth arg (`..., 1, arg2`) for iter0 and both INIT_ITER macro
     * bodies. This makes IDO emit the missing outgoing stack store in the call
     * delay slot and raises direct objdiff to 78.73047%. Still not exact:
     * built uses local `sp+0x2C` reloads and stores the fifth arg at `0x10(sp)`;
     * target keeps the marshalling pointer in `$s2` and stores `a2` at `0x8(sp)`.
     *
     * 2026-06-01 SOURCE=5 negative retests:
     *   - Replacing the duplicate fifth arg with a 4-arg call plus `(void)arg2`
     *     does NOT force the target caller-home store. IDO drops the delay-slot
     *     stack store, moves the scratch to a lower local slot, and regresses
     *     report score to 72.63262%.
     *   - Moving `_t_buf[1]` inside each unrolled macro expansion does create
     *     descending per-iteration stack slots, but it over-allocates the frame
     *     to 0x230 bytes and regresses to 79.56223%. Keep the shared `_t_buf`
     *     plus pad[152] baseline until the sp+0x2C pointer can be promoted
     *     without changing frame size.
     */
    /* Stage 12: LINKAGE/FINALIZE — store fixed values into a0's main
     * struct fields (offsets 0x2C, 0x30, 0xA0, 0xA8..0xE0, 0x4DC).
     * Mix of arg pass-through, zero-stores, D-table loads, and float
     * literals. ~57 insns.
     *
     * NEXT-PASS OBSERVATION (2026-05-05): this stage has 5 float loads
     * at &D + {0xE8, 0xEC, 0xF0, 0xF4, 0xF8}, and target asm at
     * 0x5658-0x56F4 DOES use per-access `lui $at, 0` reloads (5 distinct
     * base loads, not CSE'd). Tried unique-extern CSE-break with 5
     * unique externs (D_44F4_st12_E8/EC/F0/F4/F8) — confirmed via
     * objdump that build emits 5 separate `lui $at, 0` reloads as
     * intended, BUT report.json fuzzy stayed flat at 62.048927% (no
     * delta either way). Conclusion: the CSE-break is byte-equivalent
     * to the shared-&D form for this exact emit shape because IDO's
     * existing emit ALREADY uses 5 separate base loads at this point
     * in the function (likely because the live range of the cached
     * base is broken by intervening lui-for-fp-constant instructions:
     * 0x428C, 0x4366, 0x4140, 0x4248, 0x41A0, 0x4170 all clobber $at).
     * NOT a useful knob here — leave shared &D form. */
    {
        extern char D_44F4_st12_E8, D_44F4_st12_EC, D_44F4_st12_F0,
                    D_44F4_st12_F4, D_44F4_st12_F8;
        *(int*)&AI(a0)->rider = a1;
        AI(a0)->level = a2;
        AI(a0)->lookAhead = 0.0f;
        AI(a0)->turnGain = *(float*)((char*)&D_44F4_st12_E8 + kRoF_2p4_E8);
        AI(a0)->turnSpeedRef = 500.0f;
        AI(a0)->minSpeed = 70.0f;
        AI(a0)->crouchSteerRef = 230.0f;
        AI(a0)->stallYaw = *(float*)((char*)&D_44F4_st12_EC + kRoF_0p4_EC);
        AI(a0)->reachTime = 12.0f;
        AI(a0)->dParmsSteer = *(float*)((char*)&D_44F4_st12_F0 + kRoF_0p3_F0);
        AI(a0)->dParmsSteerScale = 2.0f;
        AI(a0)->aimRange = 50.0f;
        AI(a0)->rivalPathTime = 20.0f;
        AI(a0)->rivalPathDist = 500.0f;
        AI(a0)->weaveLeadTime = 15.0f;
        AI(a0)->weaveSideScale = *(float*)((char*)&D_44F4_st12_F4 + kRoF_0p8_F4);
        AI(a0)->weaveLead = *(float*)((char*)&D_44F4_st12_F8 + kRoF_m100000_F8);
        AI(a0)->leadLimit = 1000.0f;
        AI(a0)->unk4DC = 3;
    }
epi:
    return a0;
}


void SnowAI_reset(SnowAI *self) {
    self->unk34 = 0;
    self->unk44 = 0;
    self->unk48 = 0;
    self->unk4C = 0;
    self->unk50 = 0;
    self->unk54 = 0;
    self->unk58 = 0;
    self->targetNode = 0;
    self->aiState = 0;
    self->finishTimer = 0;
    self->modeFlags = 0;
    self->unk6C = 0;
    self->aimMode = 0;
    self->lastMarker = 0;
    self->sideBlock = 0;
    self->scaleTimer = 0;
    self->initPending = 1;
    self->unk38 = 0.0f;
    self->unk3C = 0.0f;
}

void SnowAI_dtor(int *a0, int a1) {
    if (a0 == 0) return;
    NODE(a0)->vptr = (char *)&D_00000000;
    if (a1 & 1) {
        GCALL(mem_delete)(KR_ARG0(a0));
    }
}

void SnowAI_readParms(char *a0) {
    GCALL(Parms_load_00035C6C)(a0 + 0xE4);
}

/* SnowAI_update: EXACT via REPLACE_FUNC_BODY donor splice: the real body is
 * the IDO 7.1 NCC (C++) donor src/game_uso/game_uso_ncc_57D8.cpp (81/81 words). The
 * cfe NM capped at 82.5: cfe turns the 8-way selector switch into a jumptable, edg
 * into the target's case-order compare chain (agent-g run 123). Placeholder body; the
 * 24 .late_rodata words the 57D8 .s carried (44F4's float pool, RoData +0x9C) moved to
 * the end of SnowAI_steer.s, the next INCLUDE_ASM block in text order. */
void SnowAI_update(char *a0) {
}

/* SnowAI_steer (0x1114, 1102 insns): the per-frame AI steering decision. EXACT via the IDO 7.1 cfe
 * donor src/game_uso/game_uso_591C.c, REPLACE_FUNC_BODY-spliced over the placeholder below on both
 * build paths (agent-c run 237); read the donor for the logic (docs/1080_PHYSICS.md section 8). The
 * superseded in-unit decode and its NM history (GAMEUSO_591C_OLD_DECODE) were removed in agent-g run 506
 * (git e6043cf4d has them). */
void SnowAI_steer(int *a0) {
    volatile int *ret = a0;
}

/* V3_6A30 is outside the NM block: the exact SnowAI_timeoutRun uses it. */
typedef struct { float x, y, z; } V3_6A30;

/* SnowAI_finishRun: per-frame steer update (0x2C0 / 176 words).
 * EXACT via REPLACE_FUNC_BODY donor splice: game_uso is C++, and the real
 * body lives in the IDO 7.1 NCC (C++) donor src/game_uso/game_uso_ncc_6A30.cpp
 * (176/176 words + all 7 relocs identical). The bnez-on-&pt + 055750(0xC)
 * block is edg's cfront-style constructor protocol for a derived Vec3 class,
 * and the copy chain is operator* returning by value; cfe capped at 93.2%.
 * docs/MATCHING_WORKFLOW.md#game-uso-ncc-cplusplus-donor-recipe. Body below
 * is a placeholder for the splice. */
void SnowAI_finishRun(int *a0) {
    volatile int *ret = a0;
}

/* SnowAI_timeoutRun: 102-insn (0x198) per-frame update. Copies
 * sub->{0xB4..0xBC} (sub = a0->0x30), adds sub->0x318 Vec3 scaled by
 * a0->0xA8, gates on sub->0x938 / 0xA54 and the a0->0x70 timer (dispatch
 * through RiderAnim_startTimeout_00011024(sub->0x840, flag) when sub->0x348 <= 30 or
 * timer == 60), then tail-calls SnowAI_applyStickInput(a0, flag); flag is 0x10
 * when a0->0x6C == 0.
 *
 * Exact-match notes (see the agent-g 2026-07-15 / 2026-10-15 passes in
 * docs/IDO_CODEGEN.md): register result floats rx/ry/rz + block-scope
 * register scale give the FP pools; de-named stage-1 sub; plain int
 * dispatch_arg at the a1 spill home; decl order + pad[8] = frame 0x88.
 * 2026-10-15 closed the last two cells: (1) the second sub load is an
 * int-typed read (a0[0x30/4]) instead of a volatile one - type keeps it out
 * of CSE, and without volatile uopt splits the a0 web at the 0x6C test like
 * the target; (2) the dead `addiu v0,v0,0x318` is `p += 0x318` placed BEFORE
 * the three reads, which read through p (folded back to 0x318(v0)..). */
void RiderAnim_startTimeout_00011024();
void SnowAI_applyStickInput();
void SnowAI_timeoutRun(int *a0) {
    V3_6A30 added_vec;   /* sp+0x7C */
    V3_6A30 base_vec;    /* sp+0x70 */
    int dispatch_arg;    /* sp+0x6C (a1 spill home) */
    register float rx;   /* dead home 0x68 */
    V3_6A30 tmp_vec;     /* sp+0x5C */
    register float ry, rz;      /* dead homes 0x58/0x54 */
    V3_6A30 scaled_vec;  /* sp+0x48 */
    int pad[8];          /* below the vec block */
    register char *p;    /* one web: stage-2 sub, tmp writer, tail sub */
    register int timer;

    (void)pad;
    /* stage-1 sub is DE-NAMED (inline deref -> ring temp t7); the named sub
     * web starts at the stage-2 reload. */
    base_vec = *(V3_6A30 *)&((SnowAI *)a0)->rider->dnode.vqs.trans;
    dispatch_arg = 0;    /* early: move a1,zero schedules into the stage-1 copy */

    /* the target reloads rider here (a second lw); no volatile needed (volatile moved the a0/a2 split) */
    p = (char *)((SnowAI *)a0)->rider;
    {
        register float scale;     /* block-scope: $f0, no home */
        scale = ((SnowAI *)a0)->lookAhead;
        p += 0x318;               /* rider->dnode.vel; bump FIRST, read through it: copyprop
                                     folds the reads to 0x318(v0).. but the
                                     bump survives as the dead addiu */
        rx = ((float *)p)[0] * scale;
        ry = ((float *)p)[1] * scale;
        rz = ((float *)p)[2] * scale;
        if (p) {}                 /* needed: without it 16 diff lines */
        scaled_vec.x = rx;
        scaled_vec.y = ry;
        scaled_vec.z = rz;
        p = (char*)&tmp_vec;      /* def3: addiu v0,sp,0x5C reuses the same reg */
        *(V3_6A30*)p = scaled_vec;
        added_vec = *(V3_6A30*)p;
    }

    base_vec.x = base_vec.x + added_vec.x;
    base_vec.y = base_vec.y + added_vec.y;
    base_vec.z = base_vec.z + added_vec.z;

    if (((SnowAI *)a0)->unk6C == 0) {
        dispatch_arg = 0x10;
    }
    p = (char *)((SnowAI *)a0)->rider;
    if (((SnowBoarder *)p)->contactCount != 0 && ((SnowBoarder *)p)->raceResult != 0) {
        timer = ((SnowAI *)a0)->finishTimer;
        if (timer < 60) {
            if (((SnowBoarder *)p)->dnode.speed <= 30.0f) {
                RiderAnim_startTimeout_00011024(((SnowBoarder *)p)->anim, dispatch_arg);
                AI(a0)->finishTimer = 61;
            } else {
                AI(a0)->finishTimer = timer + 1;
            }
        } else if (timer == 60) {
            RiderAnim_startTimeout_00011024(((SnowBoarder *)p)->anim, dispatch_arg);
            AI(a0)->finishTimer = 61;
        }
    }

    SnowAI_applyStickInput(a0, dispatch_arg);
}

void SnowAI_onWin(int *a0) {
    int r;
    CALL(SnowAI_enterFinish)(a0);
    r = CALL(SnowAI_isAtFinish)(a0);
    AI(a0)->atFinish =r;
    if (r == 0) return;
    AI(a0)->targetNode = CALL(SnowAI_pickFinishBranch)(a0);
}

void SnowAI_onLose(int *a0) {
    int v;
    if (AI(a0)->rider->controller != 2) {
        CALL(SnowAI_enterFinish)(KR_ARG0(a0));
        v = CALL(SnowAI_isAtFinish)(a0);
        AI(a0)->atFinish =v;
        if (v != 0) {
            AI(a0)->targetNode = CALL(SnowAI_pickFinishBranch)(a0);
        }
    }
}

void SnowAI_onTimeout(int *a0) {
    AI(a0)->aiState = 2;
    AI(a0)->finishTimer = 0;
}

/* SnowAI_isAtFinish - MATCHED 2026-07-08 (28/28 words incl. reloc fields).
 * Flag-test cascade over a chain pointer:
 * v = **(D_807FEB38+0x548); r = A374(a0, v BY-VALUE 1-int struct, a0->0x30+0xB4).
 * Levers: (1) a1 passed by-VALUE as 1-int struct (S1_6F38) -> IDO homes it to
 * the outgoing-arg slot sp+4 AND gives it a frame home (the A374
 * struct-by-value crack); (2) boolean cascade through a named chain var `b`
 * (colors $v1, return copy or v0,v1 duplicated into the bnezl/beqzl delay
 * slots; r stays live in $v0 - NOT a single &&/|| return expression);
 * (3) volatile char pad[0x18] below v lifts the frame to 0x40 with v@0x3C. */
extern char game_uso_D_807FEB38;
extern int SnowAI_findNearestMarker_0000A374();
typedef struct { int a; } S1_6F38;
int SnowAI_isAtFinish(int *a0) {
    S1_6F38 v;
    volatile char pad[0x18];
    int *r;
    int *q;
    int b;

    v.a = **(int **)((char *)&game_uso_D_807FEB38 + kMarkerTypeTags_548);
    r = (int *)SnowAI_findNearestMarker_0000A374(a0, v, AI(a0)->rider->dnode.vqs.trans);

    b = (r != 0);
    if (b) {
        b = ((((CourseMarker *)r)->parms.flg.value & MARKER_F_FINISH) != 0);
        if (!b) {
            q = (int *)((CourseMarker *)r)->link;
            b = (q != 0);
            if (b) {
                b = ((((CourseMarker *)q)->parms.flg.value & MARKER_F_FINISH) != 0);
            }
        }
    }
    return b;
}

/* SnowAI_pickFinishBranch: closer-of-two-lookups compare (0x1FC / 127 words).
 * EXACT via REPLACE_FUNC_BODY donor splice: the real body lives in the IDO
 * 7.1 NCC (C++) donor src/game_uso/game_uso_ncc_6FA8.cpp (Vec3 value class;
 * the d / e word-copy hops are edg return slots, and the frame is set by a
 * named reference in the first subtraction's inline). The cfe wrap capped at
 * 79.7%. docs/MATCHING_WORKFLOW.md#game-uso-ncc-cplusplus-donor-recipe and
 * docs/IDO_CODEGEN.md#ncc-temp-order-named-assign-6fa8. Body below is a
 * placeholder for the splice. */
int SnowAI_pickFinishBranch(int *a0) {
    volatile int *ret = a0;
    return 0;
}

void SnowAI_enterFinish(int *a0) {
    AI(a0)->aiState = 1;
    AI(a0)->finishTimer = 0;
    if (AI(a0)->scaleTimer > 0) {
        CALL(SnowAI_restoreRiderParms)(a0);
        AI(a0)->scaleTimer = 0;
    }
}

/* SnowAI_recoverStuck: speed-gated counter + state-3 steer stage (0x244 / 145 words).
 * EXACT via REPLACE_FUNC_BODY donor splice: game_uso is C++, and the real
 * body lives in the IDO 7.1 NCC (C++) donor src/game_uso/game_uso_ncc_71E0.cpp
 * (145/145 words + all 6 relocs identical); sibling of 6A30 (Vec3 value class,
 * derived-class constructor protocol for the alloc-or-stack block); cfe
 * capped at 83.6%. docs/MATCHING_WORKFLOW.md#game-uso-ncc-cplusplus-donor-recipe.
 * Body below is a placeholder for the splice. */
void SnowAI_recoverStuck(int *a0) {
    volatile int *ret = a0;
}

void SnowAI_enterRecover(void *a0) {
    AI(a0)->aiState = 3;
    CALL(SnowAI_startSegmentScan)(a0);
}

#ifdef NON_MATCHING
void SnowAI_scaleRiderParms(char *a0) {
    char *t;
    char *table;
    char *q;
    char *fc;

    t = a0;
    while (0) { t += 1; }
    table = (char *)AI(a0)->rider;
    t = table;
    if (1) { t += 0x758; } /* &rider->parms.max */
    AI(a0)->savedMax = ((SnowBParm *)t)->value;
    t = table;
    if (1) { t += 0x6F8; } /* &rider->parms.nts */
    AI(a0)->savedNts = ((SnowBParm *)t)->value;
    t = table;
    if (1) { t += 0x6E0; } /* &rider->parms.dts */
    AI(a0)->savedDts = ((SnowBParm *)t)->value;
    t = table;
    if (1) { t += 0x498; } /* &rider->child.parms.nbf */
    fc = a0;
    if (1) { fc += 0x35C; } /* &parms.parm[SNOWAI_PARM_DD2 + 2] (dd4) */
    AI(a0)->savedNbf = ((SnowBParm *)t)->value;
    ((SnowBoarder *)table)->parms.max.value = AI(a0)->parms.parm[SNOWAI_PARM_DD2].value * AI(a0)->savedMax;

    t = (char *)&AI(a0)->rider->parms.nts;
    ((SnowBParm *)t)->value = AI(a0)->parms.parm[SNOWAI_PARM_DD2 + 2].value * AI(a0)->savedNts;

    t = (char *)&AI(a0)->rider->parms.dts;
    ((SnowBParm *)t)->value = ((SnowAIParm *)fc)->value * AI(a0)->savedDts;

    table = (char *)AI(a0)->rider;
    q = table;
    if (1) { q += 0x4C8; } /* &rider->child.parms.dbf */
    t = table;
    if (1) { t += 0x498; } /* &rider->child.parms.nbf */
    ((SnowBParm *)t)->value = ((SnowBParm *)q)->value;
}

/* SnowAI_scaleRiderParms — 24/36 exact, LENGTH-EXACT, structure fully aligned
 * (was 73.5%; ~86% fuzzy now). NOT exact — genuine coloring cap. 2026-07-03
 * (agent-e). Levers that landed (see also docs/IDO_CODEGEN.md):
 *  (1) base materialization: `t = table; if (1) { t += K; }` — if(1){} blocks
 *      the copyprop merge; emits `addiu tN,table,K` + 0x10-offset accesses
 *      (plain `t = table + K` folds to full offsets).
 *  (2) `t = a0; while (0) { t += 1; }` at top = candidate-order lever
 *      (t colors v0 before table/v1).
 *  (3) fc's def embedded between t4's def and its use = live-range overlap
 *      with v0 => fc skips v0 AND its addiu schedules at target position.
 *  (4) unnamed sx + mul operands textually REVERSED (second operand's load
 *      emits first) => f18/f16 exact.
 * RESIDUAL (12 words): target colors blocks2/3-base=a1, q=a2, fc=a3 while
 * leaving v0/v1 free post-block1 — uopt's lowest-available-reuse picker can't
 * produce that from any of 12 probed candidate-order permutations. */
#else
INCLUDE_ASM("asm/nonmatchings/game_uso/game_uso", SnowAI_scaleRiderParms);
#endif

/* puts back the rider parm values saved by SnowAI_scaleRiderParms (0x4C8..0x4D4 of SnowAI) */
void SnowAI_restoreRiderParms(char *a0) {
    SnowBParm *t;
    t = &((SnowAI *)a0)->rider->parms.max;
    t->value = AI(a0)->savedMax;
    t = &((SnowAI *)a0)->rider->parms.nts;
    t->value = AI(a0)->savedNts;
    t = &((SnowAI *)a0)->rider->parms.dts;
    t->value = AI(a0)->savedDts;
    t = &((SnowAI *)a0)->rider->child.parms.nbf;
    t->value = AI(a0)->savedNbf;
}

/* rider max speed = saved max * dd3 */
void SnowAI_scaleRiderMax(char *a0) {
    /* savedMax is read through its address: as a plain member the two loads swap (lwc1 order) */
    SnowBParm *t = &((SnowAI *)a0)->rider->parms.max;
    t->value = ((SnowAI *)a0)->parms.parm[SNOWAI_PARM_DD2 + 1].value * *(float *)&((SnowAI *)a0)->savedMax;
}

#ifdef NON_MATCHING
/* 2026-10-23 agent-g run 128: REWRITTEN FROM SCRATCH (old body 611 raw words,
 * 41 now, same 344 words). The function is VOID: v0 is the event-bit word
 * (stored to out->0x40, the caller ignores the result) and v1 a local
 * 'moving' flag (dead before the final out reload reuses v1); with an int /
 * long long return uopt constrains the bits word away from v0. Dispatch on
 * the 0x6C mask: an if/else-if chain of event arms writing 0x6C = m | BIT,
 * one empty if after the 0x10 arm's if/else (joins it before the merge
 * reload); per-bit countdown trunk reusing m for the decremented 0x44 value
 * (m = t - 1 computed BEFORE the 0x100 range chain keeps bnez m instead of
 * t == 1); the 0x80 frame window is t < 8 || (t >= p + 8 && t < p + 16) ||
 * t >= p + p + 16 with lim a nested ternary; t reuses the dead arg. The
 * table base needs an empty if right after `tp = D + 0x638` (keeps the
 * lui/addiu with the addend). Variable reuse sets the a-register colours:
 * n (a2) is the 0x50 counter at entry, the 0x100-arm countdown value and
 * the 0x4DC period (`n = 0x4DC` in the window test and the frame body,
 * m = n + n), m (a1) the mask, the decremented counters and 2p (41 -> 18).
 * LEFT: the two table pointers and the 0x80 lim tail (t + 1 / 0x44 / lim)
 * take a2 / a1 swapped; every spelling that assigns m = t + 1 there shifts
 * the whole t-ring (217 words). */
extern char game_uso_D_807FEC28;
#ifndef FW
#define FW(p, o) (*(int *)((char *)(p) + (o)))
#endif
#define FF7(p, o) (*(float *)((char *)(p) + (o)))
#define O938(a) FW(FW(a, 0x30), 0x938)
#define TBL ((float *)((char *)&game_uso_D_807FEC28 + 0x638))
void SnowAI_applyStickInput(char *a0, int arg) {
    float zero = 0.0f;
    float fx = zero;
    float fy = zero;
    int ret = 0;
    int moving = 0;
    int m;
    int t;
    int n;
    float f, g;

    n = FW(a0, 0x50);
    if (n > 0) FW(a0, 0x50) = n - 1;
    if (FW(a0, 0x48) > 0) FW(a0, 0x48)--;
    m = FW(a0, 0x6C);
    if (m == 0) {
        if (arg & 0x10) {
            if (O938(a0) != 0) {
                fy = -1.0f;
                if (FF7(a0, 0x38) >= zero) FF7(a0, 0x3C) = 1.0f;
                else FF7(a0, 0x3C) = -1.0f;
                if (1) {}
            }
        } else if ((arg & 1) && FW(a0, 0x48) == 0) {
            FW(a0, 0x6C) = m | 1;
            FW(a0, 0x44) = 91;
        } else if (arg & 0x40) {
            FW(a0, 0x6C) = m | 0x40;
            FW(a0, 0x44) = 13;
        } else if ((arg & 4) && FW(a0, 0x50) == 0) {
            fy = 1.0f;
            moving = 1;
            FW(a0, 0x50) = 8;
        } else if (arg & 0x20) {
            FW(a0, 0x6C) = m | 0x20;
            FW(a0, 0x4C) = 38;
            f = FF7(FW(a0, 0x30), 0xB4);
            g = (f < zero) ? -f : f;
            FW(a0, 0x58) = (unsigned int)(int)g % 9;
            FW(a0, 0x44) = 60;
        } else if (arg & 0x80) {
            FW(a0, 0x6C) = m | 0x80;
            FW(a0, 0x4C) = 0;
            FW(a0, 0x44) = 2;
        } else if (arg & 8) {
            if (O938(a0) != 0) fy = -1.0f;
        } else if (arg & 2) {
            if (O938(a0) != 0) ret = 0x400;
        } else if (arg & 0x100) {
            FW(a0, 0x6C) = m | 0x100;
            FW(a0, 0x44) = 104;
        }
    }
    m = FW(a0, 0x6C);
    if (m & 1) {
        ret |= 0x500;
        m = FW(a0, 0x44) - 1;
        FW(a0, 0x44) = m;
        if (m == 0) FW(a0, 0x6C) &= ~0xA1;
    } else if (m & 0x40) {
        m = FW(a0, 0x44) - 1;
        FW(a0, 0x44) = m;
        if (m == 0) FW(a0, 0x6C) &= ~0x40;
        else ret |= 0x100;
    } else if (m & 0x20) {
        if (O938(a0) == 0) {
            arg = FW(a0, 0x4C);
            moving = 1;
            ret |= 0x200;
            if (arg >= 31) {
                { int i = FW(a0, 0x58); float *tp = (float *)((char *)&game_uso_D_807FEC28 + 0x638); if (1) {} fx = tp[i * 2]; fy = tp[i * 2 + 1]; }
            }
            FW(a0, 0x4C) = arg - 1;
        }
        if (FW(a0, 0x4C) != 0) {
            m = FW(a0, 0x44) - 1;
            FW(a0, 0x44) = m;
            if (m != 0) goto end;
        }
        FW(a0, 0x6C) &= ~0x20;
    } else if (m & 0x80) {
        int lim;
        arg = FW(a0, 0x4C);
        moving = 1;
        if (arg < 8 || (arg >= (n = FW(a0, 0x4DC)) + 8 && arg < n + 16) || arg >= n + n + 16) {
            n = FW(a0, 0x4DC);
            m = n + n;
            if (arg >= m + 16) {
                FW(a0, 0x58) = arg - m - 16;
                if (arg >= m + 21 && arg <= m + 23) ret |= 0x1600;
            } else if (arg >= n + 8) {
                FW(a0, 0x58) = arg - n - 8;
                if (arg >= n + 13 && arg <= n + 15) ret |= 0x1200;
            } else {
                FW(a0, 0x58) = arg;
                if (arg >= 5 && arg < 8) ret |= 0x1000;
            }
            { int i = FW(a0, 0x58); float *tp = (float *)((char *)&game_uso_D_807FEC28 + 0x638); if (1) {} fx = tp[i * 2]; fy = tp[i * 2 + 1]; }
        }
        FW(a0, 0x4C) = arg + 1;
        lim = (FW(a0, 0x44) == 0) ? 8 : (FW(a0, 0x44) == 1) ? FW(a0, 0x4DC) + 16 : (FW(a0, 0x4DC) + FW(a0, 0x4DC)) + 24;
        if (arg + 1 >= lim) FW(a0, 0x6C) &= ~0x80;
    } else if (m & 0x100) {
        n = FW(a0, 0x44);
        m = n - 1;
        if (n >= 93) {
            fy = 1.0f;
            moving = 1;
        } else if (n >= 73) {
            moving = 1;
        } else if (n >= 61) {
            ret |= 0x100;
        }
        FW(a0, 0x44) = m;
        if (m == 0) FW(a0, 0x6C) &= ~0x100;
    }
end:
    if (moving) {
        FF7(a0, 0x38) = zero;
    } else {
        if (FF7(a0, 0x3C) > 1.0) FF7(a0, 0x3C) = 1.0f;
        else if (FF7(a0, 0x3C) < -1.0) FF7(a0, 0x3C) = -1.0f;
        fx = FF7(a0, 0x38) = FF7(a0, 0x3C);
    }
    {
        char *o = (char *)FW(FW(a0, 0x30), 0x800);
        FF7(o, 0x44) = fx;
        FF7(o, 0x48) = fy;
        o = (char *)FW(FW(a0, 0x30), 0x800);
        FW(o, 0x40) = ret;
    }

}
#else
INCLUDE_ASM("asm/nonmatchings/game_uso/game_uso", SnowAI_applyStickInput);
#endif

/* SnowAI_rivalLeadZ (13 insns, 0x34): returns the float delta
 *   v0 = a0->0x30; v1 = v0->0x908;  v1 ? v1->0xBC - v0->0xBC : 0.0f
 * BOUNDARY MERGE 2026-09-05: the former "game_uso_func_00007ABC" (4 insns at
 * 0x7ABC: mtc1 zero,f2; nop; jr ra; mov.s f0,f2) was NOT a function and NOT a
 * cross-function tail-share cap. It is this function's own null-path block:
 * IDO's branch-likely idiom copies the target block's FIRST insn (mtc1 zero,f2)
 * into the beql delay slot and retargets to block+4, leaving the original mtc1
 * in place as dead code. The generate-uso-asm splitter saw the `jr ra` at 0x7AB4
 * and cut the null block off as a separate symbol. Merged back (asm .s + C).
 * Match keys: named single-return local (routes the result via $f2) AND the
 * non-null arm written FIRST (`if (v1 != 0) ... else 0.0f`) so IDO lays out
 * {non-null; jr ra; null; jr ra} with a forward beql. The old `if (v1 == 0)`
 * ordering emits bnezl-skip with the null epilogue inline (12 insns, the
 * "cap" every prior note described). */
float SnowAI_rivalLeadZ(int *a0) {
    SnowBoarder *v0 = AI(a0)->rider;
    SnowBoarder *v1 = v0->rival;
    float ret;
    if (v1 != 0) {
        ret = v1->dnode.vqs.trans[2] - v0->dnode.vqs.trans[2];
    } else {
        ret = 0.0f;
    }
    return ret;
}

/* SnowAI_updateMarker: 0x150 (84 insns). Function-table dispatcher.
 * a0[0x40] indexes a 12-entry struct-pointer table at &D + 0x548; the
 * first word of the selected struct goes to SnowAI_findNearestMarker_0000A374 as a
 * 1-word struct BY VALUE (that is what emits both `sw a1,0x34(sp)` and the
 * outgoing-home `sw a1,4(sp)` in the jal delay).  The returned record is
 * cached in a0[0x78] (with a changed/unchanged flag through arg4), its
 * +0x2C word goes out through a1, and both are mirrored into the shared
 * table at a0[0x30] + 0x954 / 0x958 before the 0x9B8 vector update.
 *
 * Exact-match notes (2026-10-15):
 *  - one pointer local `inner` is reused for THREE separate webs (the r+0x2C
 *    word, the 0x958 reload inside the second sub) - each web colours a1, so
 *    the r+0x2C value and the 0x958 reload land in a1 instead of the ugen
 *    temps t3 / t9;
 *  - the first 0x958 read goes into the dead param arg3, which keeps its
 *    a2 colour; with a1 and a2 taken, r is pushed to t0 and the tail temps
 *    renumber t1.. -> t2..;
 *  - `r` declared before `v` puts r's call-spill slot at 0x30 (not 0x2C). */
extern void game_uso_func_047B1C();
/* game_uso_func_071028 declared near SnowChaseActorCam_update. */
typedef struct { int a; } S1_7ACC;
int SnowAI_updateMarker(int *a0, int *a1, int arg3, int *arg4) {
    S1_7ACC av;
    int *r;
    int v;
    int *inner;

    *a1 = 0;
    v = AI(a0)->targetNode;
    if (v < 0 || v >= 12) {
        XCALL(game_uso_func_047B1C, sys_assertFail)((char*)&D_00000000 + kStr_0_markerType_markerType_11_78C,
                         (char*)&D_00000000 + kStr_snowai_cpp_7B0, 0x609);
    }
    av.a = *(int*)(*(int*)((char*)&D_00000000 + kMarkerTypeTags_548 + AI(a0)->targetNode * 4));
    r = (int*)SnowAI_findNearestMarker_0000A374(a0, av, arg3, arg4);
    if (r == 0) return 0;

    if (r != (int*)AI(a0)->lastMarker) {
        if (arg4 != 0) *arg4 = 1;
        AI(a0)->lastMarker = r;
    } else {
        if (arg4 != 0) *arg4 = 0;
    }

    inner = (int*)((CourseMarker *)r)->link;
    *a1 = (int)inner;
    AI(a0)->rider->marker = (CourseMarker *)r;
    AI(a0)->rider->markerLink = AI(a0)->rider->marker->link;

    arg3 = (int)AI(a0)->rider->markerLink;
    if (arg3 != 0) {
        AI(a0)->rider->linkDir.x =
            AI(a0)->rider->dnode.vqs.trans[0] - ((CourseMarker *)arg3)->pos[0];
        AI(a0)->rider->linkDir.y = 0.0f;
        inner = (int*)AI(a0)->rider->markerLink;
        AI(a0)->rider->linkDir.z =
            AI(a0)->rider->dnode.vqs.trans[2] - ((CourseMarker *)inner)->pos[2];
        XCALL(game_uso_func_071028, math_vec3Normalize)((Vec3 *)&AI(a0)->rider->linkDir);
    }

    return (int)r;
}

/* SnowAI_aimAlongSegment: EXACT via the IDO 7.1 cfe donor src/game_uso/game_uso_7C1C.c,
 * REPLACE_FUNC_BODY-spliced over the placeholder below on both build paths (agent-c run 231,
 * kits CCXL / CCXLI). The placeholder re-emits the unit's .rodata +0xFC word (3000.0f) the
 * donor reads through SnowAI_aimAlongSegment_rodata (= 0) + 0xFC. The superseded in-unit decode
 * (GAMEUSO_7C1C_OLD_DECODE) and the unbuilt game_uso_ncc_7C1C_nm.cpp were removed in agent-g run 506 (git e6043cf4d). */
int SnowAI_aimAlongSegment(char *arg0, char *arg1, char *arg2, char *arg3,
                           char *arg4, f32 *arg5) {
    *arg5 = 3000.0f;
    return 0;
}

/* SnowAI_aimWeave: segment-relative position update (709 words). EXACT 2026-11-14 (agent-h run 428)
 * through the MIPSpro 7.2.1 C++ front end donor src/game_uso/game_uso_mp72_8CD8.cpp (tools/ncc721.sh),
 * REPLACE_FUNC_BODY-spliced in both trees (the B3C recipe: its 4-label phase switch is a native 4-entry jump
 * table, which only EDG 2.29 emits). The placeholder below only re-emits the 8 RoData words the INCLUDE_ASM .s
 * carried as .late_rodata (+0x100..+0x120: the 4-word table, the A7D8 / AB98 floats and two pad words) at its
 * place in the layout; IDO merges equal literals, so the stand-ins are distinct, and none has a zero low half.
 * Its text and relocs are dropped by the splice. */
void *SnowAI_aimWeave(char *arg0) {
    *(volatile float *)arg0 = 1.1f;
    *(volatile float *)arg0 = 1.2f;
    *(volatile float *)arg0 = 1.3f;
    *(volatile float *)arg0 = 1.4f;
    *(volatile float *)arg0 = 1.6f;
    *(volatile float *)arg0 = 1.7f;
    *(volatile float *)arg0 = 1.8f;
    *(volatile float *)arg0 = 1.9f;
    return arg0;
}

/* (The cfe NM body of SnowAI_aimWeave was removed in agent-g run 506; git e6043cf4d has it.) */

/* SnowAI_isRivalInPath_000097EC (0x39C): EXACT via an IDO 7.1 NCC (C++) donor: src/game_uso/game_uso_ncc_97EC.cpp,
 * REPLACE_FUNC_BODY-spliced over the placeholder below in both build trees
 * (agent-g run 237). */
int SnowAI_isRivalInPath_000097EC(char *a0) {
    return (int)AI(a0)->rider;
}

extern void game_uso_func_047B1C();
extern char game_uso_D_807FEDB8;
/* SnowAI_isInSegmentWidth: XZ segment-side predicate, 344 target words.
 * 2026-09-12: exact C, all 344 words; 0x560 bytes, 0x1A8-byte frame.
 *
 * Project a2's position onto XZ, subtract a1's position, rotate/normalize
 * that delta, and form two scaled axes. Project a0's child-position
 * (a0->0x30 + 0xB4), subtract a1 again, then compare the signs of two
 * cross products formed from the +/- axis endpoints.
 *
 * Matching details:
 * - Keep the nine nested alloc guards. Cast the TEST (not p's definition)
 *   where p must be rematerialized after the dead allocation arm.
 * - Word-typed aggregate copies retain the shared EC staging buffer and
 *   copy-through temporaries; keep grouped copy statements on one line.
 * - Load both X/Z values before storing. Keep paired delta expressions
 *   on one line, and separate their source pointer from the child source.
 * - src_vec's home is 0x114; padding/declaration order preserves the
 *   remaining named stack slots. The four do/while(0) scopes shape pointer
 *   rematerialization and keep the endpoint scratch addresses in a1.
 * - The first scale's right-hand constant assignment and the second
 *   scale's compound multiply retain constant-first mul.s operands.
 * - Inline only the second final cross product, keeping cross1 named.
 *
 * See agent docs/IDO_CODEGEN.md#vector-staging-stack-map-9b88 and
 * #vector-endpoint-scopes-exact-9b88.
 */
int SnowAI_isInSegmentWidth(a0, a1, a2)
    int *a0;
    int *a1;
    int *a2;
{
    volatile int local_19C[3];
    float local_190[3];
    float local_184[3];
    volatile int local_178[3];
    volatile int local_16C[3];
    volatile int local_160[3];
    volatile int local_154[3];
    int   gap_150;
    float local_144[3];
    float local_138[3];
    float local_12C[3];
    float local_120[3];
    char pad_118[8];
    float *src_vec;
    char pad_F8[28];
    volatile int local_EC[3];
    char pad_E8[4];
    float local_DC[3];
    char pad_D0[12];
    int   local_C4[3];
    float local_B8[3];
    float scale0;
    float *delta_src;
    char pad_AC[4];
    volatile float local_A0[3];
    volatile int local_94[3];
    volatile float local_88[3];
    volatile int local_7C[3];
    char pad_78[4];
    volatile float local_6C[3];
    char pad_50[28];
    volatile int local_44[3];
    volatile float local_38[3];
    int *out;
    int *p;
    int * volatile *spill_a1 = &a1;
    float src_x, src_z;

    (void)gap_150;
    (void)pad_F8;
    (void)pad_E8;
    (void)pad_D0;
    (void)spill_a1;

    if (a2 == 0) {

        XCALL(game_uso_func_047B1C, sys_assertFail)(&game_uso_D_807FEDB8 + 0x7BC, &game_uso_D_807FEDB8 + 0x7C8, 0x623);
    }

    out = (int*)(unsigned)local_190;
    if (out == 0) {
        out = (int*)XCALL(game_uso_func_055750, mem_new)(0xC);
        if (out == 0) goto skip1;
    }
    {
        src_vec = 0; if (1) { src_vec = ((CourseMarker *)a2)->pos; }
        src_x = src_vec[0];
        src_z = src_vec[2];
        ((float*)out)[1] = 0.0f;
        ((float*)out)[2] = src_z;
        ((float*)out)[0] = src_x;
    }
skip1:;

    p = (int*)local_DC;
    out = p;
    if ((unsigned)out == 0) {
        out = (int*)XCALL(game_uso_func_055750, mem_new)(0xC);
        if (out == 0) goto skip2;
    }
    {
        delta_src = 0; if (1) { delta_src = ((CourseMarker *)a1)->pos; }
        src_x = local_190[0] - delta_src[0]; src_z = local_190[2] - delta_src[2];
        ((float*)out)[1] = 0.0f;
        ((float*)out)[2] = src_z;
        ((float*)out)[0] = src_x;

    }
skip2:;

    *(Tri3i *)local_EC = *(Tri3i *)p; *(Tri3i *)local_19C = *(Tri3i *)local_EC; *(Tri3i *)local_144 = *(Tri3i *)local_19C;

    if (1) { p = (int*)local_C4; } src_vec = (float*)p; if ((unsigned)src_vec == 0) {
        src_vec = (float*)XCALL(game_uso_func_055750, mem_new)(0xC);
        if (src_vec == 0) goto skip3;
    }
    {
        src_x = local_144[2];
        src_z = -local_144[0];
        ((float*)src_vec)[0] = src_x;
        ((float*)src_vec)[1] = 0.0f;
        ((float*)src_vec)[2] = src_z;
    }
skip3:;

    *(Tri3i *)local_EC = *(Tri3i *)p; *(Tri3i *)local_138 = *(Tri3i *)local_EC;
    XCALL(game_uso_func_071028, math_vec3Normalize)((Vec3 *)local_138);

    *(Tri3i *)local_12C = *(Tri3i *)local_138;
    scale0 = (((CourseMarker *)a1)->parms.wid.value) * (src_x = 250.0f) + 50.0f;
    local_12C[0] *= scale0;
    local_12C[1] *= scale0;
    local_12C[2] *= scale0;
    scale0 = 250.0f; scale0 *= (((CourseMarker *)a2)->parms.wid.value - ((CourseMarker *)a1)->parms.wid.value);
    local_138[0] *= scale0;
    local_138[1] *= scale0;
    local_138[2] *= scale0;

    src_vec = AI(a0)->rider->dnode.vqs.trans;
    out = (int*)(unsigned)local_184;
    if (out == 0) {
        out = (int*)XCALL(game_uso_func_055750, mem_new)(0xC);
        if (out == 0) goto skip4;
    }
    {
        src_x = src_vec[0];
        src_z = src_vec[2];
        ((float*)out)[0] = src_x;
        ((float*)out)[1] = 0.0f;
        ((float*)out)[2] = src_z;
    }
skip4:;

    p = (int*)local_B8; out = p;
    if ((unsigned)out == 0) {
        out = (int*)XCALL(game_uso_func_055750, mem_new)(0xC);
        if (out == 0) goto skip5;
    }
    {
        delta_src = 0; if (1) { delta_src = ((CourseMarker *)a1)->pos; }
        src_x = local_184[0] - delta_src[0]; src_z = local_184[2] - delta_src[2];
        ((float*)out)[0] = src_x;
        ((float*)out)[1] = 0.0f;
        ((float*)out)[2] = src_z;
    }
skip5:;

    *(Tri3i *)local_EC = *(Tri3i *)p;
    *(Tri3i *)local_120 = *(Tri3i *)local_EC;
    do { p = (int*)local_A0; } while (0); out = p; if ((unsigned)out == 0) {
        out = (int*)XCALL(game_uso_func_055750, mem_new)(0xC);
        if (out == 0) goto skip6;
    }
    {
        src_x = local_138[0] + local_144[0];
        src_z = local_138[2] + local_144[2];
        ((float*)out)[0] = src_x;
        ((float*)out)[1] = 0.0f;
        ((float*)out)[2] = src_z;
    }
skip6:;

    *(Tri3i *)local_EC = *(Tri3i *)p;
    *(Tri3i *)local_178 = *(Tri3i *)local_EC;
    do { p = (int*)local_88; } while (0); out = p;
    if ((unsigned)out == 0) {
        out = (int*)XCALL(game_uso_func_055750, mem_new)(0xC);
        if (out == 0) goto skip7;
    }
    {
        src_x = local_120[0] - local_12C[0];
        src_z = local_120[2] - local_12C[2];
        ((float*)out)[0] = src_x;
        ((float*)out)[1] = 0.0f;
        ((float*)out)[2] = src_z;
    }
skip7:;
    *(Tri3i *)local_94 = *(Tri3i *)p;
    *(Tri3i *)local_16C = *(Tri3i *)local_94;
    do { p = (int*)local_6C; } while (0); out = p;
    if ((unsigned)out == 0) {
        out = (int*)XCALL(game_uso_func_055750, mem_new)(0xC);
        if (out == 0) goto skip8;
    }
    {
        src_x = local_144[0] - local_138[0];
        src_z = local_144[2] - local_138[2];
        ((float*)out)[0] = src_x;
        ((float*)out)[1] = 0.0f;
        ((float*)out)[2] = src_z;
    }
skip8:;
    *(Tri3i *)local_7C = *(Tri3i *)p;
    *(Tri3i *)local_160 = *(Tri3i *)local_7C;
    do { p = (int*)local_38; } while (0); out = p; if ((unsigned)out == 0) {
        out = (int*)XCALL(game_uso_func_055750, mem_new)(0xC);
        if (out == 0) goto skip9;
    }
    {
        src_x = local_12C[0] + local_120[0];
        src_z = local_12C[2] + local_120[2];
        ((float*)out)[0] = src_x;
        ((float*)out)[1] = 0.0f;
        ((float*)out)[2] = src_z;
    }
skip9:;
    *(Tri3i *)local_44 = *(Tri3i *)p;
    *(Tri3i *)local_154 = *(Tri3i *)local_44;
    (void)local_19C;
    (void)local_EC;
    (void)local_C4;
    (void)scale0;
    (void)delta_src;

    {

        float cross1 = (*(float*)&local_160[2] * *(float*)&local_154[0]) - (*(float*)&local_154[2] * *(float*)&local_160[0]); return ((*(float*)&local_178[2] * *(float*)&local_16C[0]) - (*(float*)&local_16C[2] * *(float*)&local_178[0])) * cross1 < (float)0;
    }
}
/* SnowAI_isAboveSegmentFloor: projected-height predicate, 163 target words.
 * Build two XZ vectors relative to a1->position: a2's position and
 * a0's child position at +0xB4. Compare their projected height against
 * the child height above a1.y - 400. The three length calls are retained.
 *
 * MATCHED 2026-09-12: 163/163 raw words, 0x28C size/0xC8 frame.
 * Keep the shared word-copy buffer, the two guarded XZ fills, same-line
 * delta staging and named pointer homes. Both deltas use a1 + 0x30;
 * the assertion does not replace a2 with its source-line number.
 * fx/fy/fz are reused scalar scratch, not fixed vector components.
 * Final four-word fix: reuse fy for the X loads/stores in both XZ fills,
 * and assign the first length-call argument to fy. The actual float
 * argument constrains fy to f12; changing the fills alone instead gives
 * it f2 and cascades the remaining FP allocation. No extra call arguments
 * or instructions are needed. See docs/IDO_CODEGEN.md#projected-height-exact-a0e8.
 */
extern float game_uso_func_082880(float);
extern char game_uso_D_807FEDD0;
int SnowAI_isAboveSegmentFloor(char *a0, char *a1, char *a2) {
    volatile int local_BC[3];
    volatile int local_B0[3];
    char pad_AC[4];
    float local_A0[3];
    float local_94[3];
    float mag;
    float *p, *w; char pad_78[16];
    volatile int local_6C[3];
    char pad_68[4];
    float local_5C[3];
    char pad_3C[32];
    float local_30[3];
    float fx, fy, fz;
    float *q;
    if (a2 == 0) {
        XCALL(game_uso_func_047B1C, sys_assertFail)(&game_uso_D_807FEDD0 + kStr_enCm_0_7D4,
                           &game_uso_D_807FEDD0 + kStr_snowai_cpp_7E0, 1586);
    }
    w = 0; if (1) { w = (float*)(a2 + 0x30); }
    q = 0; if (1) { q = (float*)(a1 + 0x30); }
    fy = w[0] - q[0]; fx = w[1] - q[1]; fz = w[2] - q[2];
    local_5C[0] = fy;
    local_5C[2] = fz;
    local_5C[1] = fx;
    *(Tri3i*)local_6C = *(Tri3i*)local_5C;
    *(Tri3i*)local_BC = *(Tri3i*)local_6C;
    p = (float*)(unsigned)local_A0;
    if (p == 0) {
        p = (float*)XCALL(game_uso_func_055750, mem_new)(12);
        if (p == 0) goto skip1;
    }
    fy = *(float*)&local_BC[0];
    fz = *(float*)&local_BC[2];
    p[0] = fy;
    p[2] = fz;
    p[1] = 0.0f;
skip1:;
    w = (float*)AI(a0)->rider; if (1) { w = (float*)((char*)w + 0xB4); } /* rider->dnode.vqs.trans */
    fy = w[0] - q[0]; fx = w[1] - q[1]; fz = w[2] - q[2];
    local_30[0] = fy;
    local_30[1] = fx;
    local_30[2] = fz;
    *(Tri3i*)local_6C = *(Tri3i*)local_30;
    *(Tri3i*)local_B0 = *(Tri3i*)local_6C;
    p = (float*)(unsigned)local_94;
    if (p == 0) {
        p = (float*)XCALL(game_uso_func_055750, mem_new)(12);
        if (p == 0) goto skip2;
    }
    fy = *(float*)&local_B0[0];
    fz = *(float*)&local_B0[2];
    p[0] = fy;
    p[2] = fz;
    p[1] = 0.0f;
skip2:;
    mag = XCALL(game_uso_func_082880, sqrtf)(fy = local_A0[2]*local_A0[2] + local_A0[0]*local_A0[0]);
    if (0.0f < XCALL(game_uso_func_082880, sqrtf)(local_A0[2]*local_A0[2] + local_A0[0]*local_A0[0])) {
        fx = (local_A0[0]*local_94[0] + local_A0[2]*local_94[2]) /
                XCALL(game_uso_func_082880, sqrtf)(local_A0[2]*local_A0[2] + local_A0[0]*local_A0[0]);
    } else {
        fx = (float)0;
    }
    fz = ((CourseMarker *)a1)->pos[1];
    return mag * (AI(a0)->rider->dnode.vqs.trans[1] -(fz - 400.0f)) > (((CourseMarker *)a2)->pos[1] - fz)*fx;
}

/* MATCHED 2026-06-20 (stale-cap catch): the "2 pre-jal a1 spills" cap
 * (sw a1,0x1C(sp) incoming-home + sw a1,0x4(sp) in the jal delay slot) is the
 * struct-by-value-homes-arg lever (docs/IDO_CODEGEN). The first call is varargs
 * passing a1 by VALUE as a 1-int struct: IDO places it in $a1 AND homes it to
 * the outgoing-arg slot 0x4(sp), while the struct's addressability forces the
 * incoming home at 0x1C(sp). K&R and `(int,...)` prototypes both fail to emit
 * the home; only struct-by-value does. 20/20 words byte-exact, 0 non-reloc diffs. */
extern int game_uso_func_053104();
extern int import_8006EF48;
typedef struct { int a; } S1A374;
int SnowAI_findNearestMarker_0000A374(a0, a1, a2)
int a0, a1, a2;
{
    int r = XCALL(game_uso_func_053104, ItemMgr_findType_0003EA98)(*(int *)&import_8006EF48, *(S1A374*)&a1);
    if (r == 0) return 0;
    return (int)CourseMarker_findNearestZ_000043D8((int **)r, (int *)a2, 0);
}

/* SnowAI_pickFork - MATCHED 2026-07-08 (144/144 words + ROM byte-exact;
 * prior "sub-80 table-dispatch ceiling" note was FALSE). Picks the nearer of the
 * two table entries (a0->0x40 vs a0->0x7C) to the ref point ctx->0x908+0xB4 by
 * squared XZ distance and re-targets a0->0x40. Levers that cracked it:
 * (1) a0 param used DIRECTLY (no local copy) -> uopt homes it at the incoming
 *     slot (sp+0xB0) and shuttles via $a3 with flush/reload around every call
 *     (NO s-regs anywhere - do not introduce a `s0 = a0` copy);
 * (2) slot results in `int *slots[2]` (ARRAY, slots[1]=first/0x40, slots[0]=
 *     second/0x7C) - array elements resist uopt promotion -> homed at 0x88/0x84
 *     with `sw v0` in the bnez delay + fresh reload at each use (1-word-struct
 *     fields get region-cached in $t regs instead - wrong);
 * (3) only slot1 has the `->0x84 & 0x10 -> ->0x2C` re-point, via named temp w
 *     (colors $v0);
 * (4) alloc-fallback blocks: `if (1) { p = &buf; }` barrier keeps the
 *     `if (p==0) p = 055750(12); if (!p) goto skip;` null test un-folded
 *     (plain `p = &buf` folds the test + deletes the calls);
 * (5) per-block float values through named fx/fz (color $f0/$f2 candidates;
 *     zeros walk the fp temp FIFO f4/f6/f8), q = base+0x30 colors $v0;
 * (6) frame layout: decl order p,v1,v2,buf2,buf3,slots,buf1,ref,w,fx,fz,q +
 *     volatile char pad[0x4C] -> homes v1@A8,v2@A4,buf2@98,buf3@8C,slots@84/88,
 *     buf1@78,ref@74, p's dead home takes the 0xAC slack word, pad fills the
 *     0x18-0x63 dead region (frame 0xB0). a1 by-VALUE 1-int struct to A374
 *     (S1_A3C4) homes to sp+4 per the A374 crack. */
extern char game_uso_D_807FEB38;
extern char game_uso_D_807FEDF8;
extern char game_uso_D_807FEE20;
typedef struct { float x, y, z; } V3_A3C4;
typedef struct { int a; } S1_A3C4;
void SnowAI_pickFork(SnowAI *a0) {
    V3_A3C4 *p;
    S1_A3C4 v1, v2;
    V3_A3C4 buf2, buf3;
    int *slots[2];
    V3_A3C4 buf1;
    float *ref;
    int *w;
    float fx, fz;
    float *q;
    volatile char padA3C4[0x4C];

    a0->modeFlags &= ~1;
    if (a0->rider->rival == 0) {
        return;
    }
    v1.a = *(int *)(*(int **)((char *)&game_uso_D_807FEB38 + kMarkerTypeTags_548 + a0->targetNode * 4));
    slots[1] = (int *)SnowAI_findNearestMarker_0000A374(a0, v1,
                (int)a0->rider->rival->dnode.vqs.trans);
    if (slots[1] == 0) {
        XCALL(game_uso_func_047B1C, sys_assertFail)((char *)&game_uso_D_807FEDF8 + kStr_nearBoarderNorNearMrk_0_7EC,
                             (char *)&game_uso_D_807FEDF8 + kStr_snowai_cpp_808, 0x450);
    }
    if (((CourseMarker *)slots[1])->parms.flg.value & MARKER_F_REDIRECT) {
        w = (int *)((CourseMarker *)slots[1])->link;
        slots[1] = w;
    }
    v2.a = *(int *)(*(int **)((char *)&game_uso_D_807FEB38 + kMarkerTypeTags_548 + a0->altNode * 4));
    slots[0] = (int *)SnowAI_findNearestMarker_0000A374(a0, v2,
                (int)a0->rider->rival->dnode.vqs.trans);
    if (slots[0] == 0) {
        XCALL(game_uso_func_047B1C, sys_assertFail)((char *)&game_uso_D_807FEE20 + kStr_nearBoarderDivNearMrk_0_814,
                             (char *)&game_uso_D_807FEE20 + kStr_snowai_cpp_830, 0x45B);
    }
    ref = a0->rider->rival->dnode.vqs.trans;
    if (1) { p = &buf1; }
    if (p == 0) {
        p = (V3_A3C4 *)XCALL(game_uso_func_055750, mem_new)(12);
        if (p == 0) goto skip1;
    }
    fx = ref[0];
    fz = ref[2];
    p->x = fx;
    p->z = fz;
    p->y = 0.0f;
skip1:
    if (1) { p = &buf2; }
    if (p == 0) {
        p = (V3_A3C4 *)XCALL(game_uso_func_055750, mem_new)(12);
        if (p == 0) goto skip2;
    }
    q = ((CourseMarker *)slots[1])->pos;
    fz = q[2];
    fx = q[0];
    p->y = 0.0f;
    p->z = fz;
    p->x = fx;
skip2:
    if (1) { p = &buf3; }
    if (p == 0) {
        p = (V3_A3C4 *)XCALL(game_uso_func_055750, mem_new)(12);
        if (p == 0) goto skip3;
    }
    q = ((CourseMarker *)slots[0])->pos;
    fz = q[2];
    fx = q[0];
    p->y = 0.0f;
    p->z = fz;
    p->x = fx;
skip3:
    if ((buf1.x - buf3.x) * (buf1.x - buf3.x) + (buf1.z - buf3.z) * (buf1.z - buf3.z) <
        (buf1.x - buf2.x) * (buf1.x - buf2.x) + (buf1.z - buf2.z) * (buf1.z - buf2.z)) {
        a0->targetNode = a0->altNode;
    }
}

// SnowAI_scanNodes — STRUCTURAL PASS (0x1D4 / 117 words,
// no episode). Raw-.word USO form (single function, game_uso main
// game-logic; boundary already split by commit 66d6c531 — named fn
// itself was still undecoded). Hand-decoded.
//
// Per-object physics/state advance step.
//
//   void SnowAI_scanNodes(Obj *obj) {              // obj -> s0
//     World *w = obj->0x30;
//     // copy obj->0xB4/0xB8/0xBC transform into sp scratch;
//     // scale a Vec3 (w->0x318/0x31C/0x320) by w->0xA8, matrix*
//     //   vector compose -> sp+0x50/0x7C/0x84 (mul.s/add.s chains);
//     int r = func_00000000(w->0x548[obj->0x5C], …);      // query/sample
//     if (r) {
//       if (func_00000000(obj->0x2C, …) && …) {
//         // FP clamp: if (obj->0x84 ...) vs 250.0f (0x437A0000) and
//         //   obj->0x54 / obj->0x38 thresholds, write obj->0x60/
//         //   obj->0x40;
//       }
//     }
//     if (++obj->0x5C >= 0xA) obj->0x5C = 0;               // wrap counter
//     obj->0x68 &= ~2;                                      // clear flag
//   }
//
// Struct-typing reference:
//   obj: 0x30 -> World (w->0xA8 a scale, w->0x318/0x31C/0x320 a Vec3,
//     w->0x548 a per-index record array), 0x2C a sub-obj, 0x5C a
//     cyclic counter (++ mod 0xA), 0x60/0x40 outputs, 0x68 flag word
//     (bit 0x2 cleared each step), 0xB4/0xB8/0xBC a transform Vec3,
//     0x38/0x54/0x84 f32 state. const 250.0f. func_00000000 = USO
//     placeholder dispatcher (sample / query).
// Levers kept from the decode: the FP clamp is the compound `f0 += 250.0f*x`
//   (const-first operand order); scaled = w->0x318[i]*s goes through three
//   temp scalars before the stores; `w` is re-read inline (the target's two
//   `lw 48(a0)`).
/* SnowAI_scanNodes — EXACT 117/117 words; the interleaved call-spill slots
 * among the four Vec3 buffers come from the levers below.
 *
 * PREREQUISITE (outside this body): the SnowAI_findNearestMarker_0000A374 DEFINITION
 * (line ~8425) must be converted from ANSI `(int a0, int a1, int a2)` to K&R
 * `(a0, a1, a2) int a0, a1, a2;` so no prototype is in scope at this call —
 * otherwise cfe rejects the struct arg. A374's own 20/20 bytes verified
 * UNCHANGED by the conversion (int params are K&R/ANSI-identical in IDO).
 *
 * Levers (in addition to F948's re-base + decl-order + bottom-pad pair):
 *  (1) DECL-ORDER FRAME MAP (top-down from 0x94, frame -0x98): copy2@0x8C,
 *      a1arg@0x88, tbuf@0x7C, v0var-spill@0x78, pad_a[2]@0x70, copy1@0x64,
 *      pad_b[2]@0x5C, scaled@0x50, t@0x4C, pad_bot[5]+scalars fill 0x20-0x48.
 *      NOTE: scalar decls at the bottom (s,f0,q,mx,my,mz) DO take 6 slots here
 *      — pad_bot sized to compensate (5, not 11).
 *  (2) a1arg is a ONE-WORD STRUCT (One1) passed by value to K&R A374 ->
 *      emits BOTH missing words: home store sw a1,0x88(sp) (struct stores
 *      escape DSE) and outgoing-arg home sw a1,4(sp) in the jal delay.
 *      (Same mechanism as 2744's a2/sw a2,8(sp).)
 *  (3) rec-chain INLINED (no named rec/counter locals) -> D-base lui/addu/lw
 *      chain stays in temps t5/t6/t7 (named locals promote to $a3/$v1
 *      candidates — the old 90.01% residual).
 *  (4) t SHARED for field2c AND counter-increment -> both color to $a2
 *      (explains the three lw a2,0x5C(s0) copies: two branch-likely delay
 *      duplicates + join).
 *  (5) FP tail UN-NESTED: `if (f0<0) f0 += 250*x;` then a SEPARATE
 *      `if (0<=f0 && f0<obj->0x60)` -> uopt duplicates the c.le.s per path
 *      (bc1fl +6 shape). Nested form branches +0x12 (wrong).
 *  (6) &copy1 base in $v0 materialized LATE (addiu v0,sp,0x64 after q dies):
 *      q REASSIGNED to &copy1 (one-reg-per-variable web) AND the two struct
 *      copies wrapped in `if (1) { }` — the BB boundary blocks the copy-prop
 *      that otherwise folds q away and hoists a fresh $a3 temp to the
 *      prologue (the last 20-diff residual). Plain reassign without if(1)
 *      does NOT work.
 */
void SnowAI_scanNodes(SnowAI *obj) {
    Vec3 copy2;
    One1 a1arg;
    Vec3 tbuf;
    char *v0var;
    volatile int pad_a[2];
    Vec3 copy1;
    volatile int pad_b[2];
    Vec3 scaled;
    int t;
    volatile int pad_bot[5];
    float s;
    float f0;
    float *q;
    float mx, my, mz;

    tbuf = *(Vec3 *)&obj->rider->dnode.vqs.trans;
    s = obj->lookAhead;
    q = obj->rider->dnode.vel;
    mx = q[0] * s;
    my = q[1] * s;
    mz = q[2] * s;
    q = (float *)&copy1;
    scaled.x = mx;
    scaled.y = my;
    scaled.z = mz;
    if (1) {
        *(Vec3 *)q = scaled;
        copy2 = *(Vec3 *)q;
    }
    tbuf.x = tbuf.x + copy2.x;
    tbuf.y = tbuf.y + copy2.y;
    tbuf.z = tbuf.z + copy2.z;

    a1arg.a = **(int **)((char *)&D_00000000 + kMarkerTypeTags_548 + obj->scanIndex * 4);
    v0var = (char *)SnowAI_findNearestMarker_0000A374(obj, a1arg, &tbuf);
    if (v0var != 0) {
        /* RAW: byte-lock: int carrier t for the marker link (port-audit B); the matched lever, the callees take it as int / char * */
        t = (int)((CourseMarker *)v0var)->link;
        if (t != 0) {
            if (SnowAI_isInSegmentWidth(obj, v0var, t) != 0) {
                if (SnowAI_isAboveSegmentFloor((char *)obj, v0var, (char *)t) != 0) {
                    f0 = tbuf.z - ((CourseMarker *)v0var)->pos[2];
                    if (f0 < 0.0f) {
                        f0 += 250.0f * ((CourseMarker *)v0var)->parms.wid.value;
                    }
                    if (0.0f <= f0 && f0 < obj->scanBestDist) {
                        obj->scanBestDist = f0;
                        obj->targetNode = obj->scanIndex;
                    }
                }
            }
        }
    }
    t = obj->scanIndex + 1;
    obj->scanIndex = t;
    if (t >= 0xA) {
        obj->modeFlags &= ~2;
    }
}

void SnowAI_startNodeScan(SnowAI *a0) {
    a0->scanIndex = 0;
    a0->modeFlags |= 2;
    a0->scanBestDist = *(float *)((char *)&D_00000000 + kRoF_FLT_MAX_110);
}

/* SnowAI_scanSegments: nearest projected-segment distance update.
 * MATCHED: 232/232 target words. Predict the child position from
 * obj's scale and child direction, query the indexed segment, test its
 * projected height, then minimize squared XZ distance to the segment line.
 * Increment the scan index on every path; clear bit 2 after ten entries.
 *
 * 2026-09-12 matching levers:
 * - A374's second argument is One1 by value (not an extra vararg), giving
 *   both the node home at F0 and the outgoing argument home at sp+4.
 * - slots[0/1] preserve child/link and segment pointers at DC/E0; keep r1
 *   separate until the height call so the E0 store stays after both tests.
 *   cnt doubles as the link-pointer integer and the final scan counter.
 * - Shared c1 at CC stages all three integer-copy chains. q is reassigned
 *   to c1 for the opening copy; the scoped out definitions keep fixed
 *   endpoints separate from the possibly allocated p at b2/b5.
 * - Scalar homes occupy the padding above b1, preserving frame 0x100.
 * - Group X/Z subtraction assignments on one line; reverse the zero
 *   equality operands; name b3.z before the squared-length expression.
 * See docs/IDO_CODEGEN.md#projected-distance-exact-a7f8.
 */
void SnowAI_scanSegments(SnowAI *obj) {
    Vec3 c2;
    One1 node;
    Vec3 blockA;
    char *slots[2];
    char *r1;
    Vec3 c1;
    char pad_C4[8];
    Vec3 scaled;
    float f0, f2, f12, f14; Vec3 *out; char pad_8C[24];
    Vec3 b1, b4, b3, b6;
    char pad_54[8];
    Vec3 b2;
    char pad_34[20];
    Vec3 b5;
    float *q;
    Vec3 *p;
    int cnt;

    blockA = *(Vec3 *)&obj->rider->dnode.vqs.trans;
    f0 = obj->lookAhead;
    q = obj->rider->dnode.vel;
    f14 = q[0] * f0;
    f2 = q[1] * f0;
    f12 = q[2] * f0;
    q = (float *)&c1;
    scaled.x = f14;
    scaled.y = f2;
    scaled.z = f12;
    if (1) {
        *(Vec3 *)q = scaled;
        c2 = *(Vec3 *)q;
    }
    blockA.x = blockA.x + c2.x;
    blockA.y = blockA.y + c2.y;
    blockA.z = blockA.z + c2.z;
    node.a = **(int **)((char *)&D_00000000 + kMarkerTypeTags_548 + obj->scanIndex*4);
    r1 = (char *)SnowAI_findNearestMarker_0000A374(obj, node, &blockA);
    if (r1 != 0) {
        /* RAW: byte-lock: int carrier cnt for the marker link (port-audit B), stored to slots[0] as char *; the matched lever */
        cnt = (int)((CourseMarker *)r1)->link;
        slots[0] = (char *)cnt;
        if (cnt != 0) {
            slots[1] = r1;
            if (SnowAI_isAboveSegmentFloor((char *)obj, r1, (char *)cnt) != 0) {
                f0 = blockA.z - ((CourseMarker *)slots[1])->pos[2];
                if (f0 < 0.0f) f0 += 250.0f * ((CourseMarker *)slots[1])->parms.wid.value;
                if (f0 >= 0.0f) {
                    /* RAW: byte-lock: (unsigned) int-cast of &b1 (port-audit A); dropping it moves addiu sp,244 and the copy schedule (tried run 580) */
                    p = (Vec3 *)(unsigned)&b1;
                    if (p == 0) {
                        p = (Vec3 *)XCALL(game_uso_func_055750, mem_new)(12);
                        if (p == 0) goto skip1;
                    }
                    q = ((CourseMarker *)slots[0])->pos;
                    f0 = q[0];
                    f2 = q[2];
                    p->y = 0.0f;
                    p->z = f2;
                    p->x = f0;
                skip1:;
                    do { out = &b2; } while (0); p = out;
                    if (p == 0) {
                        p = (Vec3 *)XCALL(game_uso_func_055750, mem_new)(12);
                        if (p == 0) goto skip2;
                    }
                    q = ((CourseMarker *)slots[1])->pos;
                    f12 = b1.x - q[0]; f0 = b1.z - q[2];
                    p->y = 0.0f;
                    p->z = f0;
                    p->x = f12;
                skip2:;
                    *(Tri3i *)&c1 = *(Tri3i *)out;
                    *(Tri3i *)&b3 = *(Tri3i *)&c1;
                    /* RAW: byte-lock: (unsigned) int-cast of &b4 (port-audit A); dropping it changes the code (tried run 580) */
                    p = (Vec3 *)(unsigned)&b4;
                    if (p == 0) {
                        p = (Vec3 *)XCALL(game_uso_func_055750, mem_new)(12);
                        if (p == 0) goto skip4;
                    }
                    f0 = blockA.x;
                    f2 = blockA.z;
                    p->y = 0.0f;
                    p->x = f0;
                    p->z = f2;
                skip4:;
                    do { out = &b5; } while (0); p = out;
                    if (p == 0) {
                        p = (Vec3 *)XCALL(game_uso_func_055750, mem_new)(12);
                        if (p == 0) goto skip5;
                    }
                    q = ((CourseMarker *)slots[1])->pos;
                    f12 = b4.x - q[0]; f0 = b4.z - q[2];
                    p->y = 0.0f;
                    p->z = f0;
                    p->x = f12;
                skip5:;
                    *(Tri3i *)&c1 = *(Tri3i *)out;
                    *(Tri3i *)&b6 = *(Tri3i *)&c1;
                    f14 = b3.z; f12 = b3.x*b3.x + f14*f14;
                    if (0.0f == f12) {
                        f0 = 0.0f;
                    } else {
                        f2 = b3.z*b6.x - b3.x*b6.z;
                        f0 = f2*f2 / f12;
                    }
                    if (f0 < obj->scanBestDist) {
                        obj->scanBestDist = f0;
                        obj->targetNode = obj->scanIndex;
                    }
                }
            }
        }
    }
    cnt = obj->scanIndex + 1;
    obj->scanIndex = cnt;
    if (cnt >= 10) obj->modeFlags &= ~4;
}

void SnowAI_startSegmentScan(SnowAI *a0) {
    a0->scanIndex = 0;
    a0->modeFlags |= 4;
    a0->scanBestDist = *(float*)(&D_00000000 + kRoF_FLT_MAX_114);
}

void game_readFloatParm_0000ABB8(char *dst) {
    int tmp;
    stream_readS32_00004080(&tmp);
    stream_readF32_000040BC((float*)(dst + 0x10));
}

void game_readVec3Parm_0000ABE8(char *dst) {
    int tmp;
    stream_readS32_00004080(&tmp);
    stream_readVec3_000040F8((Vec3*)(dst + 0x10));
}

void game_readQuadParm_0000AC18(char *dst) {
    int tmp;
    stream_readS32_00004080(&tmp);
    stream_readQuad_00004168((Quad4*)(dst + 0x10));
}

void game_readIntParm_0000AC48(char *dst) {
    int tmp;
    stream_readS32_00004080(&tmp);
    stream_readS32_00004080((int*)(dst + 0x10));
}

/* game_ItemType_create_0000AC78: item factory of an ItemType (arg0): a 0x40-byte ColourItem
 * (mem_new + ColourItem_ctor, vptr import_8006ED80, +0x3C = 0); when the type's unk40
 * node is set, it becomes the item's child (list_append on children + the parent link).
 *
 * 2026-06-21 CRACKED via masked-twin port from edd_ItemType_create_000003BC
 * (donor, byte-exact). The prior "25-diff v1-vs-a2 regalloc cap" was a
 * structure issue, not a true cap: the donor's p2/head/p1 split + p1-reuse-
 * for-arg0 + decl-order (p2,head,p1) + condition re-load (`if((int*)p1[0x40/4]
 * !=0)`) forces IDO to spill the object as two live ranges (frame 0x28) and
 * color it a0(block1)/v1(block2) instead of a2(0x20). Do NOT simplify the
 * variable reuse or the re-load — they are load-bearing for the reg/slot
 * allocation. See docs/IDO_CODEGEN.md interleave-decl-spill-slot. */
extern char import_8006ED80;
int *game_ItemType_create_0000AC78(int *arg0) {
    int *p2;
    int *head;
    int *p1;
    p1 = (int*)XCALL(game_uso_func_055750, mem_new)(sizeof(ColourSubItem));
    if (p1 != 0) {
        XCALL(game_uso_func_051C28, ColourItem_ctor)(p1);
        NODE(p1)->vptr = (char *)&import_8006ED80;
        ((ColourSubItem *)p1)->unk3C = 0;
    }
    p2 = p1;
    p1 = arg0;
    head = (int*)ITEMTYPE(p1)->unk40;
    if ((int*)ITEMTYPE(p1)->unk40 != 0) {
        XCALL(game_uso_func_07ACE0, list_append)(&NODE(p2)->children, head);
        if (NODE(head)->parent != 0) {
            NODE(head)->unk04 = 1;
        }
        NODE(head)->parent = (Node *)p2;
    }
    return p2;
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_uso/game_uso/game_uso_func_0000AC78_pad.s")

static void stream_readF32_0000AD10(float *dst) {
    float buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

static void stream_readS32_0000AD4C(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

static void stream_readQuad_0000AD88(Quad4 *dst) {
    Quad4 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 16);
#ifdef PORT
    PORT_BE_SWAP32S(&buf, 16);
#endif
    *dst = buf;
}

static void stream_readS32_0000ADE0(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

/* Sun_ctor: primary + embedded-child constructor, 278 words.
 * Exact stock IDO 7.1 -O2: 0x458 bytes, 0xA0 frame. The final child is
 * at (arg0 + 0xEC) + 0x68, with that sub-object passed as its parent.
 * Sub-object/last-child allocation failure skips children, not the tail.
 * Shared parent/zero-node webs, One1 index arguments, aggregate copies and
 * scoped pointer definitions recover the target without instruction edits.
 * See docs/IDO_CODEGEN.md#embedded-child-constructor-exact-ae1c. */
extern void game_uso_func_04D3D0(void *self);
extern void game_uso_func_04D3C8(void *self, int a1);
extern char game_uso_D_807FF340, game_uso_D_807FF24C, game_uso_D_807FEE38,
            game_uso_D_807FEE30, game_uso_D_807FF234, game_uso_D_807FF344,
            game_uso_D_807FF34C, game_uso_D_807FF350, game_uso_D_807FF354,
            game_uso_D_807FF358, game_uso_D_807FEE50, game_uso_D_807FEE68,
            game_uso_D_807FF35C, game_uso_D_807FEE80, game_uso_D_807FF214;
extern char import_80086288, gPolyGeomMode_3EEC8_8005EE6C;
extern int import_000F5ADC();

#define SUN(p) ((Sun *)(p))
void *Sun_ctor(void *arg0) {
    /* Declaration order preserves the original 0xA0 stack frame. */
    char *s0;
    char *r;
    void *p;
    void *q;
    float v3[3];
    float z;
    char *unused7c;
    char *unused78;
    int *state;
    One1 idx0, idx1, idx2, idx3;
    float v4[4];
    One1 idx4;
    int flags;
    char *nfirst;
    One1 idx_arg;
    char *unused40;
    char *n;
    char *tbl1;
    char *tbl2;
    char *o36;
    Quad4 v4_copy;

    /* Primary object; all exits reload its argument home. */
    if (arg0 == NULL) {
        arg0 = (void *)XCALL(game_uso_func_055750, mem_new)(0x178);
        if (arg0 == NULL) goto end_ret;
    }

    s0 = arg0;
    if (s0 == NULL) {
        s0 = (void *)XCALL(game_uso_func_055750, mem_new)(0xB4);
        if (s0 == NULL) goto skip_sub;
    }
    XCALL(game_uso_func_04C678, Node_ctor)(s0, (char *)&game_uso_D_807FF340 + kTag_sun_D50);
    NODE(s0)->vptr = (char *)&import_80086288;

    p = (char *)s0 + 0x2C;
    if (s0 == (void *)-0x2C) {
        p = (void *)XCALL(game_uso_func_055750, mem_new)(4);
        if (p == NULL) goto after_node;
    }
    q = p;
    if (p == NULL) {
        q = (void *)XCALL(game_uso_func_055750, mem_new)(4);
        if (q == NULL) goto after_node;
    }
    *(int *)q = 0;
after_node:
    XCALL(game_uso_func_04D3D0, MNode_initMatrices)(s0);
    z = (float)0; v3[0] = z; v3[1] = z; v3[2] = z;
    XCALL(game_uso_func_072EE8, math_mtxAddTranslation)((char *)s0 + 0x30, (Vec3 *)v3);
skip_sub:

    NODE(arg0)->vptr = (char *)&game_uso_D_807FF24C;

    s0 = (char *)arg0 + 0xDC;
    if (arg0 == (void *)-0xDC) {
        s0 = (char *)XCALL(game_uso_func_055750, mem_new)(0x10);
        if (s0 == NULL) goto after_o16;
    }
    {
        r = s0;
        if (s0 == NULL) {
            r = (char *)XCALL(game_uso_func_055750, mem_new)(0x10);
            if (r == NULL) goto o16_set;
        }
        {
            p = r;
            if (r == NULL) {
                p = (char *)XCALL(game_uso_func_055750, mem_new)(4);
                if (p == NULL) goto o16_fill;
            }
            ((MemberCallback *)p)->vptr = (char *)&game_uso_D_807FEE38 + 0x848;
        o16_fill:
            ((MemberCallback *)r)->obj = arg0;
            ((MemberCallback *)r)->pmf = *(Pair2 *)((char *)&game_uso_D_807FEE30 + kPmf_Sun_measureVisibility);
        }
    o16_set:
        ((MemberCallback *)s0)->vptr = (char *)&game_uso_D_807FF234 + 0xC44;
    }
after_o16:

    /* The five children belong to this 140-byte sub-object (SunParms, Sun +0xEC), not arg0. */
    s0 = (char *)arg0 + 0xEC;
    if ((arg0 != (void *)-0xEC) ||
        (s0 = (char *)XCALL(game_uso_func_055750, mem_new)(140), s0 != NULL)) {
    {
        r = s0;
        if (s0 == NULL) {
            r = (char *)XCALL(game_uso_func_055750, mem_new)(8);
            if (r == NULL) goto after_o140hdr;
        }
        ((SunParms *)r)->name = (char *)&game_uso_D_807FF344 + kStr_Parms_D54; /* "Parms" */
        ((SunParms *)r)->head = 0;
    }
after_o140hdr:
    {
        idx0.a = *(int *)((char *)&game_uso_D_807FF34C + kTag_rot_D5C);
        /* One-word index records are passed by value to 04A188. */
        idx_arg = idx0;
        nfirst = s0 + 8;
        if (s0 == (char *)-8) {
            nfirst = (char *)XCALL(game_uso_func_055750, mem_new)(24);
            if (nfirst == NULL) goto node1;
        }
        XCALL(game_uso_func_04A188, Parms_addParm)(nfirst, s0, idx_arg, 1);
        ((SunParm *)nfirst)->vptr = (char *)&game_uso_D_807FEE50 + 0x860;
        ((SunParm *)nfirst)->unk14 = 0;
        ((SunParm *)nfirst)->value.f = 0.0f;
    }
node1:
    {
        idx1.a = *(int *)((char *)&game_uso_D_807FF350 + kTag_ele_D60);
        idx_arg = idx1;
        n = s0 + 32;
        if (s0 == (char *)-32) {
            n = (char *)XCALL(game_uso_func_055750, mem_new)(24);
            if (n == NULL) goto node2;
        }
        XCALL(game_uso_func_04A188, Parms_addParm)(n, s0, idx_arg, 1);
        { if (1) { tbl1 = (char *)&game_uso_D_807FEE50 + 0x860; } ((SunParm *)n)->vptr = tbl1; }
        ((SunParm *)n)->unk14 = 0;
        ((SunParm *)n)->value.f = 0.0f;
    }
node2:
    {
        idx2.a = *(int *)((char *)&game_uso_D_807FF354 + kTag_scl_D64);
        idx_arg = idx2;
        n = s0 + 56;
        if (s0 == (char *)-56) {
            n = (char *)XCALL(game_uso_func_055750, mem_new)(24);
            if (n == NULL) goto node3;
        }
        XCALL(game_uso_func_04A188, Parms_addParm)(n, s0, idx_arg, 1);
        { if (1) { tbl2 = (char *)&game_uso_D_807FEE50 + 0x860; } ((SunParm *)n)->vptr = tbl2; }
        ((SunParm *)n)->unk14 = 0;
        ((SunParm *)n)->value.f = 2.0f;
    }
node3:
    {
        idx3.a = *(int *)((char *)&game_uso_D_807FF358 + kTag_mat_D68);
        idx_arg = idx3;
        n = s0 + 80;
        if (s0 == (char *)-80) {
            n = (char *)XCALL(game_uso_func_055750, mem_new)(24);
            if (n == NULL) goto node4;
        }
        XCALL(game_uso_func_04A188, Parms_addParm)(n, s0, idx_arg, 0);
        ((SunParm *)n)->vptr = (char *)&game_uso_D_807FEE68 + 0x878;
        ((SunParm *)n)->unk14 = 0;
        ((SunParm *)n)->value.i = 0;
    }
node4:;
    {
        idx4.a = *(int *)((char *)&game_uso_D_807FF35C + kTag_col_D6C);
        v4[0] = 1.0f;
        v4[1] = 1.0f;
        v4[2] = 1.0f;
        v4[3] = 1.0f;
        /* Retain both integer-word copies around the child initializer. */
        v4_copy = *(Quad4 *)v4;
        idx_arg = idx4;
        if (1) { o36 = s0 + 0x68; }
        if ((s0 != (char *)-0x68) ||
            (o36 = (char *)XCALL(game_uso_func_055750, mem_new)(36), o36 != NULL)) {

        XCALL(game_uso_func_04A188, Parms_addParm)(o36, s0, idx_arg, 0);
        ((SunColorParm *)o36)->vptr = (char *)&game_uso_D_807FEE80 + 0x890;
        ((SunColorParm *)o36)->unk20 = 0;
        *(Quad4 *)((SunColorParm *)o36)->value = v4_copy;
        }
    }

    }
tail_init:
    /* Failed sub-object/last-child allocation still reaches this tail. */
    {
        if (1) { state = (int *)((char *)&gPolyGeomMode_3EEC8_8005EE6C + 4); } flags = *state;
        flags &= ~0x00080000;
        flags &= ~2;
        if (1) { state = (int *)((char *)&gPolyGeomMode_3EEC8_8005EE6C + 4); } *state = flags;
    }
    ((Sun *)arg0)->fade = 1.0f;
    XCALL(game_uso_func_04D3C8, Node_setUnk24)(arg0, 2);
    ((Sun *)arg0)->sprite = (Sprite *)import_000F5ADC(0, &game_uso_D_807FF214);
    ((Sun *)arg0)->visibility = 0.0f;
end_ret:
    return arg0;
}

/* Sun_display: easing/update, corrected 107-word boundary.
 * The former last word (mtc1 zero,f0 at B420) is the successor's entry,
 * after this function's return and delay slot; no inherited-FP-state cap.
 * Corrected C: continue only when threshold < progress (return on an
 * unordered comparison), and load the full word at +0x14C before storing its
 * low half to the child. The old short load selected the wrong half on
 * this big-endian target. Full-TU fuzzy 76.94 -> 88.77%; C is 432 bytes
 * versus target 428, frame 88 versus 64. Held context, compound scaling,
 * scoped temporaries and product ordering were tested; prologue scheduling,
 * stack homes, destination base and late pointer/FP registers still differ.
 * Retain NON_MATCHING until raw bytes agree.
 * See docs/MATCHING_WORKFLOW.md#b274-b420-zero-head-boundary and
 * docs/IDO_CODEGEN.md#easing-gate-and-halfword-b274. */
#ifdef NON_MATCHING
/* Sun_display — reconstructed from expected/.o (reloc-aware). s1 base =
 * gGfxLib_8005C108 cached in a saved register; four double thresholds live in distinct
 * rodata symbols game_uso_D_807FFA40/48/50/58 (each at +0x120/0x128/0x130/0x138);
 * destination vec = gGfxLib.modelView[3][0..2] (import_8005C1A8 is the gGfxLib token named for its
 * +0xA0 site). Callees: game_uso_func_063734,
 * import_000F8678, game_uso_func_04D700, game_uso_func_05B750. */
extern char gGfxLib_8005C108;
extern char import_8005C1A8;
extern char game_uso_D_807FFA40;
extern char game_uso_D_807FFA48;
extern char game_uso_D_807FFA50;
extern char game_uso_D_807FFA58;
extern int game_uso_func_063734();
extern int import_000F8678();
extern void game_uso_func_04D700();
extern void game_uso_func_05B750();

void Sun_display(char *s0) {
    char *s1 = (char *)(unsigned)&gGfxLib_8005C108;
    float *dst;
    float vec[3];
    float r;
    char *v1;
    int *vt;

    if ((double)GFXLIB(s1)->fog.colour[3] > *(double *)(&game_uso_D_807FFA40 + kRoD_0p98_120)) return;

    vec[0] = SUN(s0)->mnode.world[3][0];
    vec[1] = SUN(s0)->mnode.world[3][1];
    vec[2] = SUN(s0)->mnode.world[3][2];
    if (game_uso_func_063734(GFXLIB(s1)->curWindow, &vec[0], 1, 0) == 0) return;

    SUN(s0)->screenX = (int)vec[0];
    SUN(s0)->screenY = (int)vec[1];
    import_000F8678(s1, &SUN(s0)->visCallback);
    SUN(s0)->depthImage = GFXLIB(s1)->curWindow;
    SUN(s0)->fade = SUN(s0)->visibility;

    if (*(double *)(&game_uso_D_807FFA48 + kRoD_0p7_128) < (double)GFXLIB(s1)->fog.colour[3]) {
        SUN(s0)->fade *= (1.0 - (((double)GFXLIB(s1)->fog.colour[3] - *(double *)(&game_uso_D_807FFA48 + kRoD_0p7_128)) / *(double *)(&game_uso_D_807FFA50 + kRoD_0p3_130)));
    }

    if (!(*(double *)(&game_uso_D_807FFA58 + kRoD_0p1_138) < (double)SUN(s0)->fade)) return;

    game_uso_func_04D700(s0);

    dst = GFXLIB(&import_8005C1A8)->modelView[3]; /* the sprite's model-view translation */
    dst[0] = SUN(s0)->mnode.world[3][0];
    dst[1] = SUN(s0)->mnode.world[3][1];
    dst[2] = SUN(s0)->mnode.world[3][2];
    r = SUN(s0)->fade * SUN(s0)->parms.scl.value.f;
    game_uso_func_05B750(s1, SUN(s0)->parms.col.value);

    SUN(s0)->sprite->unkC0 = SUN(s0)->parms.mat.value.i;
    SUN(s0)->sprite->scaleX = r;
    SUN(s0)->sprite->scaleY = r;

    v1 = (char *)SUN(s0)->sprite;
    vt = *(int **)(v1 + 0x28);
    (*(void (**)(char *))((char *)vt + 0x1C))((char *)((int)(short)*(short *)((char *)vt + 0x18) + (int)v1));
}
#else
INCLUDE_ASM("asm/nonmatchings/game_uso/game_uso", Sun_display);
#endif

/* Exact, 30 words: initialize a vector, transform two component pairs,
 * store the results in the object, then run its update helper.
 * The two unused low array slots retain the target's 48-byte frame and
 * live vector at sp+0x24..0x2C. IDO emits mtc1 zero,f0 before the stack
 * adjustment; the old B424 boundary incorrectly assigned it to B274.
 * No inherited f0 is required. D_00000000+0x140 is the true constant
 * relocation, not a separate zero-based symbol.
 * See docs/MATCHING_WORKFLOW.md#b274-b420-zero-head-boundary. */
extern void game_uso_func_070338(float f, float *p1, float *p2);
extern void game_uso_func_04DA24(int *a0);
void Sun_update(int *a0) {
    float buf[5];
    buf[2] = 0.0f;
    buf[3] = 0.0f;
    buf[4] = *(float*)((char*)&D_00000000 + kRoF_16000000_140);      /* sp+0x2C = D[0x140] */
    /* float-first arg (-> $f12); a0 is saved across the calls, not passed */
    XCALL(game_uso_func_070338, math_rotate2D)(((Sun *)a0)->parms.ele.value.f, &buf[3], &buf[4]);
    XCALL(game_uso_func_070338, math_rotate2D)(((Sun *)a0)->parms.rot.value.f, &buf[2], &buf[4]);
    ((Sun *)a0)->mnode.local[3][0] = buf[2];
    ((Sun *)a0)->mnode.local[3][1] = buf[3];
    ((Sun *)a0)->mnode.local[3][2] = buf[4];
    XCALL(game_uso_func_04DA24, MNode_update)(a0);
}
void Sun_readParms(char *a0) {
    GCALL(Parms_load_00035C6C)(&((Sun *)a0)->parms);
}

/* B4B8: nine-neighbor sentinel count, then asymmetric smoothing (150 words).
 * obj+D0/D4 are x/y cell coordinates; obj+D8 points to grid metadata whose
 * B8/BC fields are width/height. The u16 cell buffer comes from the global
 * context's +148 child, at child+F0. Only 0 < index < width*height counts.
 * This is not bilinear interpolation: count the 0xFFFC cells, divide by 9,
 * immediately accept a lower fraction, otherwise approach it using the
 * double-precision coefficient. The difference is computed as float first.
 * Walk the signed-halfword offset pairs independently of the trip counter;
 * IDO peels one iteration and unrolls the remaining eight in groups of four.
 * 2026-09-12: 77.74 -> 86.27% NM, 576 -> 600 bytes, target 8-byte frame
 * and s0 save recovered. Only 14/150 raw words match: pointer/scalar
 * allocation, early loads, branch scheduling and FP registers still differ.
 * Preserve the ASM fallback; this is not an exact training episode.
 * Follow-up: 86.27 -> 86.87% NM by naming the unsigned-halfword cell
 * inside the bounds-checked block. The read must stay after both bounds.
 * Length/frame and 14/150 raw words are unchanged; no exact episode.
 * See docs/IDO_CODEGEN.md#sentinel-guarded-cell-b4b8.
 * See docs/IDO_CODEGEN.md#sentinel-neighbor-pointer-walk-b4b8. */
extern short game_uso_D_807FF2B4[];
#ifdef NON_MATCHING
/* Re-decoded 2026-06-24: prior body counted the 0xFFFC sentinel neighbours but
 * dropped the FP tail (result = count/9, clamp-or-EMA into obj->0xCC) and used
 * the wrong base symbol (&D_00000000 instead of import_8005C348). c comes from
 * *(import_8005C348 + 0x240); data = (c->0x148)->0xF0. Returns the count. */
extern char import_8005C348;
extern char game_uso_D_807FFA68;
#define GAME_D_807FFA68_148 (*(double *)(&game_uso_D_807FFA68 + 0x148))
int Sun_measureVisibility(char *obj) {
    char *c = *(char **)(&import_8005C348 + 0x240);
    char *grid = SUN(obj)->depthImage;
    unsigned short *data = *(unsigned short **)(*(char **)(c + 0x148) + 0xF0);
    short *tbl = game_uso_D_807FF2B4;
    int w = *(int *)(grid + 0xB8);
    int h = *(int *)(grid + 0xBC);
    int y = SUN(obj)->screenY;
    int x = SUN(obj)->screenX;
    int count = 0;
    int i;
    int idx;
    float result;
    float prev;
    for (i = 0; i < 9; i++, tbl += 2) {
        idx = tbl[1] + (x + (y + tbl[0]) * w);
        if (idx > 0 && idx < w * h) {
            unsigned short cell = data[idx];
            if (cell == 0xFFFC) count++;
        }
    }
    result = (float)count / 9.0f;
    prev = SUN(obj)->visibility;
    if (result < prev) {
        SUN(obj)->visibility = result;
    } else {
        SUN(obj)->visibility =
            (float)((double)prev + (double)(result - prev) * GAME_D_807FFA68_148);
    }
    return count;
}
#else
INCLUDE_ASM("asm/nonmatchings/game_uso/game_uso", Sun_measureVisibility);
#endif

void LensFlareElem_init_0000B710(char *a0) {
    float one = 1.0f;
    float zero = 0.0f;

    ((LensFlareElem *)a0)->color[3] = one;
    ((LensFlareElem *)a0)->color[2] = one;
    ((LensFlareElem *)a0)->color[1] = one;
    ((LensFlareElem *)a0)->color[0] = one;
    ((LensFlareElem *)a0)->pos = zero;
    ((LensFlareElem *)a0)->size = *(float*)(&D_00000000 + kRoF_0p01_150);
    ((LensFlareElem *)a0)->image = 0;
    ((LensFlareElem *)a0)->flags = 8;
    ((LensFlareElem *)a0)->minDot = zero;
}

/* LensFlareElem_draw_0000B750 (0x134 / 77 words): basis/scale update. f0 =
 * src->0x00 (+ k * 400 when flag 4; k = the stack float arg, 1.0 unless
 * flag 8); vec at import_8005C1A8+0xD0 = a2 + a3 * f0; obj->0xC0 (short) =
 * src->0x18; if src->0x14 (* k when flag 2) exceeds the double at
 * game_uso_D_807FFA78+0x158: obj->0xB0/0xB4 = it, 05B750(&gGfxLib_8005C108,
 * &q) with q = {src->0x04, 0x08, 0x0C, src->0x10 * k}, then obj's third
 * virtual (vptr at +0x28, jalr with this-adjust).
 * EXACT via REPLACE_FUNC_BODY donor splice: the real body is the IDO 7.1 NCC
 * (C++) donor src/game_uso/game_uso_ncc_B750.cpp (77/77 words + 7/7 relocs).
 * The cfe C capped at 72.8%: k is the parameter itself (re-loaded from its
 * home per path) and q is a class temporary whose constructor forwards to an
 * inline set(), which fixes the frame (88) and FP colouring. See
 * docs/IDO_CODEGEN.md#ncc-ctor-forwarding-inline-set-frame-b750 and
 * docs/MATCHING_WORKFLOW.md#game-uso-ncc-cplusplus-donor-recipe.
 * Body below is a placeholder for the splice. */
void LensFlareElem_draw_0000B750(char *a0, char *s0, float *a2, float *a3, float arg4) {
    volatile char *ret = a0;
}

void LensFlareElem_read_0000B884(char *dst) {
    stream_readF32_0000AD10(&((LensFlareElem *)dst)->pos);
    stream_readF32_0000AD10(&((LensFlareElem *)dst)->size);
    stream_readS32_0000AD4C(&((LensFlareElem *)dst)->image);
    stream_readQuad_0000AD88((Quad4 *)((LensFlareElem *)dst)->color);
    stream_readS32_0000AD4C(&((LensFlareElem *)dst)->flags);
    stream_readF32_0000AD10(&((LensFlareElem *)dst)->minDot);
}

/* LensFlare_create — EXACT 174/174 words (2026-07-07, agent-e/w30).
 * Alloc-cascade constructor, sibling of 0000AE1C/00003018/000044F4. Main obj
 * 0x278 (find-or-create), sub-obj 0xB4 via 04C678(&D_807FF360+0xD70) +
 * vtable &import_80086288 + zero-node @+0x2C + 04D3D0 + Vec3-zero 072EE8;
 * then s2 section: vtable &D_807FF2D8, +0x220=arg2, sub-list 0x50 @+0x224
 * with 3 24-byte nodes via 04A188 (3rd arg = 4-byte STRUCT BY VALUE ->
 * `sw a2,8(sp)` jal-delay home, docs/IDO_CODEGEN struct-by-value-marshalling);
 * finalize: +0xC=&D_807FF388+0xD98, +0xB4=import_000F5ADC(0,&D_807FF214),
 * 10-iter 0x24-stride 0000B710 loop, 04C774(self,1,arg1). Key levers:
 * ONE sp1 local for both sub-objects (wins $s1), a2v entry copy ($s0 pack),
 * n reused as stage node + loop cursor ($s0), slot_a/b/c + marsh struct
 * locals per marshalling recipe, D_B8D4_st2/st3 (=0) alias externs bust the
 * &D_807FEE50+0x860 PRE hoist, decl-order pads set frame 0x80. */
extern char game_uso_D_807FF360, game_uso_D_807FF2D8, game_uso_D_807FF368,
            game_uso_D_807FF37C, game_uso_D_807FF380, game_uso_D_807FF384,
            game_uso_D_807FF388, D_B8D4_st2, D_B8D4_st3;
extern char import_80086288;
extern char game_uso_D_807FEE50;
extern char game_uso_D_807FF214;
extern char game_uso_D_807FF360;
extern char game_uso_D_807FF368;
extern char game_uso_D_807FF388;
extern char game_uso_D_807FF2D8;
extern char D_B8D4_st2, D_B8D4_st3;
void *LensFlare_create(void *arg0, int arg1, int arg2) {
    typedef struct { char *p; } S0000B8D4;
    unsigned int i;        /* @0x7C (loop-counter spill home) */
    volatile int pad78;
    volatile int pad74;
    volatile int pad70;
    volatile int pad6C;
    Vec3 v3;               /* @0x60 */
    volatile int pad5C;
    volatile int pad58;
    volatile int pad54;
    volatile int pad50;
    S0000B8D4 slot_a;      /* @0x4C */
    S0000B8D4 slot_b;      /* @0x48 */
    S0000B8D4 slot_c;      /* @0x44 */
    char *sp1;             /* colored $s1 — dead @0x40 */
    void *p;               /* dead @0x3C */
    void *q;               /* dead @0x38 */
    S0000B8D4 marsh;       /* @0x34 */
    char *hdr;             /* dead @0x30 */
    char *n;               /* colored $s0 (stage node + loop cursor) */
    int a2v;

    a2v = arg2;
    if (arg0 == NULL) {
        arg0 = (void *)XCALL(game_uso_func_055750, mem_new)(0x278);
        if (arg0 == NULL) goto end_ret;
    }

    sp1 = arg0;
    if (sp1 == NULL) {
        sp1 = (char *)XCALL(game_uso_func_055750, mem_new)(0xB4);
        if (sp1 == NULL) goto skip_sub;
    }
    XCALL(game_uso_func_04C678, Node_ctor)((int *)sp1, (char *)&game_uso_D_807FF360 + kStr_MNode_D70);
    NODE(sp1)->vptr = (char *)&import_80086288;

    p = sp1 + 0x2C;
    if (sp1 == (char *)-0x2C) {
        p = (void *)XCALL(game_uso_func_055750, mem_new)(4);
        if (p == NULL) goto after_node;
    }
    q = p;
    if (p == NULL) {
        q = (void *)XCALL(game_uso_func_055750, mem_new)(4);
        if (q == NULL) goto after_node;
    }
    *(int *)q = 0;
after_node:
    XCALL(game_uso_func_04D3D0, MNode_initMatrices)(sp1);
    v3.x = 0.0f;
    v3.y = 0.0f;
    v3.z = 0.0f;
    XCALL(game_uso_func_072EE8, math_mtxAddTranslation)(sp1 + 0x30, &v3);
skip_sub:

    NODE(arg0)->vptr = (char *)&game_uso_D_807FF2D8;
    ((LensFlare *)arg0)->sun = (Sun *)a2v;
    sp1 = (char *)arg0 + 0x224;
    if (arg0 == (void *)-0x224) {
        sp1 = (char *)XCALL(game_uso_func_055750, mem_new)(0x50);
        if (sp1 == NULL) goto finalize;
    }
    hdr = sp1;
    if (sp1 == NULL) {
        hdr = (char *)XCALL(game_uso_func_055750, mem_new)(8);
        if (hdr == NULL) goto stage_a;
    }
    ((LensFlareParms *)hdr)->name = (char *)&game_uso_D_807FF368 + kStr_LensFlare_Parms_D78; /* "LensFlare::Parms" */
    ((LensFlareParms *)hdr)->head = 0;
stage_a:
    slot_a.p = *(char **)((char *)&game_uso_D_807FF37C + kTag_fin_D8C);
    marsh = slot_a;
    n = sp1 + 8;
    if (sp1 == (char *)-8) {
        n = (char *)XCALL(game_uso_func_055750, mem_new)(0x18);
        if (n == NULL) goto stage_b;
    }
    XCALL(game_uso_func_04A188, Parms_addParm)(n, sp1, marsh, 1);
    ((SunParm *)n)->vptr = (char *)&game_uso_D_807FEE50 + 0x860;
    ((SunParm *)n)->unk14 = 0;
    ((SunParm *)n)->value.f = *(float *)((char *)&game_uso_D_807FF380 + kRoF_0p8_160);
stage_b:
    slot_b.p = *(char **)((char *)&game_uso_D_807FF380 + kTag_dlv_D90);
    marsh = slot_b;
    n = sp1 + 0x20;
    if (sp1 == (char *)-0x20) {
        n = (char *)XCALL(game_uso_func_055750, mem_new)(0x18);
        if (n == NULL) goto stage_c;
    }
    XCALL(game_uso_func_04A188, Parms_addParm)(n, sp1, marsh, 1);
    ((SunParm *)n)->vptr = (char *)&D_B8D4_st2 + 0x860;
    ((SunParm *)n)->unk14 = 0;
    ((SunParm *)n)->value.f = *(float *)((char *)&game_uso_D_807FF384 + kRoF_0p6_164);
stage_c:
    slot_c.p = *(char **)((char *)&game_uso_D_807FF384 + kTag_dfm_D94);
    marsh = slot_c;
    n = sp1 + 0x38;
    if (sp1 == (char *)-0x38) {
        n = (char *)XCALL(game_uso_func_055750, mem_new)(0x18);
        if (n == NULL) goto finalize;
    }
    XCALL(game_uso_func_04A188, Parms_addParm)(n, sp1, marsh, 1);
    ((SunParm *)n)->vptr = (char *)&D_B8D4_st3 + 0x860;
    ((SunParm *)n)->unk14 = 0;
    ((SunParm *)n)->value.f = 0.0f;
finalize:
    NODE(arg0)->name = (const char *)((char *)&game_uso_D_807FF388 + kStr_LensFlare_D98);
    ((LensFlare *)arg0)->sprite = (Sprite *)import_000F5ADC(0, &game_uso_D_807FF214);
    ((LensFlare *)arg0)->numElems = LENSFLARE_MAX_ELEMS;
    i = 0;
    n = (char *)((LensFlare *)arg0)->elems;
    do {
        LensFlareElem_init_0000B710(n);
        n += 0x24;
        i++;
    } while (i < ((LensFlare *)arg0)->numElems);
    XCALL(game_uso_func_04C774, Node_Load)((int *)arg0, 1, arg1);
end_ret:
    return arg0;
}

/* LensFlare_display: EXACT via REPLACE_FUNC_BODY donor splice (agent-g run 264): the real body
 * is the IDO 7.1 NCC (C++) donor src/game_uso/game_uso_ncc_BB8C.cpp (252/252 words). Its three
 * doubles (RoData +0x168..0x180) are emitted as .late_rodata from Sun_measureVisibility.s.
 * Body below is a placeholder for the splice. */
void LensFlare_display(char *arg0) {
    volatile char *ret = arg0;
}
/* (The old cfe decode of LensFlare_display was removed in agent-g run 506; git e6043cf4d has it.) */

/* MATCHED 2026-05-31. The prior "no C-level lever to swap the loop-pre-header
 * schedule (li s2,0x168 / move s0,$0)" verdict was cracked by the FOR-LOOP
 * COMMA-INIT form: `for (i = 0, p = base + 0xB8; ...)` emits the bound
 * materialization before the counter init, matching the target — where the
 * do-while with separate `i = 0; p = ...;` statements emitted the reverse.
 * (The two are independent in the dataflow graph; the comma-init's evaluation
 * order pins IDO's schedule.) The remaining diffs were placeholder callees:
 * the real targets are game_uso_func_04A2D8 (setup) and LensFlareElem_read_0000B884
 * (per-element, 0x24 stride over 0xF entries from base+0xB8). Byte-exact. */
extern void game_uso_func_04A2D8();
void LensFlare_readParms(char *a0) {
    char *base = a0;
    int i;
    char *p;
    XCALL(game_uso_func_04A2D8, Parms_load_00035C6C)(&((LensFlare *)base)->parms);
    stream_readS32_0000ADE0((int *)&((LensFlare *)base)->numElems);
    for (i = 0, p = (char *)((LensFlare *)base)->elems; i != sizeof(((LensFlare *)base)->elems); p += 0x24, i += 0x24) {
        LensFlareElem_read_0000B884(p);
    }
}

/* MemberCallback_invoke_0000BFDC: 32-insn (0x80) sign-dispatched table-lookup wrapper
 * + indirect jalr. Reads two signed shorts from a0+0x8/0xA and a ptr from
 * a0+0x4; if a0->[0xA] < 0 takes a short path that reuses a0->[0xC] as the
 * function pointer; else dispatches through a 2-level pointer table where
 * a0->[0xA] indexes a (vtable_entry, fnptr) pair.
 *
 * Initial structural decode 2026-05-08; matching pass deferred — likely
 * 30-50% on first build due to the multi-way control flow + indirect jalr.
 * Open: identify the exact struct layout for a0 (probably a UI element or
 * dispatch handle).
 *
 * Asm @ 0xBFDC..0xC058 decoded structurally:
 *   short ka = a0->0xA  (signed half, dispatch key)
 *   short kb = a0->0x8  (signed half, secondary)
 *   int *base = (int*)a0->0x4  (table base ptr)
 *   if (ka < 0) {
 *       a1 = kb + base       (accum offset)
 *       fnptr = (void(*)())a0->0xC
 *       a0 = a1
 *       goto jalr
 *   } else {
 *       int v = a0->0xC
 *       if (v == 0) {
 *           if (a0->0x8 != 0) {
 *               // (offset value falls through)
 *           } else {
 *               v = 0x28
 *           }
 *       }
 *       short d = a0->0xA          (reload)
 *       int *table = *(int**)((char*)base + v)
 *       int *entry = (int*)((char*)table + (d << 3))
 *       short e = *(short*)entry
 *       fnptr = (void(*)())entry[1]
 *       a1 = e + (kb + base)
 *       a0 = a1
 *   }
 *   return fnptr(a0)
 *
 * 2026-05-17: C body emits 87% before post-cc. Remaining cap is local
 * branch-likely/register scheduling around the selector fallback; Makefile
 * INSN_PATCH + one suffix nop made the old unwrapped body byte-exact, but
 * instruction patching is banned now.
 *
 * 2026-06-01 sibling pass from BF7C exact: explicit `goto dispatch` for the
 * selector fallback still emitted `bnel` for both selector tests, and worsened
 * short-path register choices (fnptr moved from a2 to v1). Keep the nested
 * selector form unless a new branch-likely suppression lever appears. */
extern int gl_func_00000000();
/* MemberCallback_invoke_0000BFDC — EXACT 32/32 words (verified clean rebuild of
 * build/non_matching/src/game_uso/game_uso.c.o vs
 * asm/nonmatchings/game_uso/game_uso/MemberCallback_invoke_0000BFDC.s).
 * Cracked 2026-07-03 (agent-e). The old "bnezl branch-likely hard cap" was a
 * C-shape issue, not a cap. Three coupled levers:
 *  (1) &&-TERNARY for the selector: `idx = (sel==0 && pair[0]==0) ? 0x28 : sel;`
 *      -> uopt keeps sel(v0)/idx(a0) as separate webs (ternary result reg is a
 *      real phi home): as1 steals the arm's `move a0,v0` into the first bnez
 *      delay, retargets the second bnez to the merge with nop delay (plain
 *      branches, NO bnezl), and the 0x28 arm becomes `b merge; li a0,0x28`.
 *      Nested-if / goto / dead-kill / while(0) forms all coalesce -> bnezl.
 *  (2) comma-embedded arg init: `arg = (arg = *(char**)(a0+4), arg) + pair[0];`
 *      -> emission order lh 0xA / lh 0x8 / lw 4 (single-expr order) AND the lw
 *      lands in arg's home a1 (comma re-read avoids the v0 snapshot temp of
 *      `(arg = load) + pair[0]`) AND addu a1,t6,a1 operand order (textual-second
 *      operand becomes rs).
 *  (3) entry reuses the sel variable (multi-def) so entry colors v0. */
int MemberCallback_invoke_0000BFDC(char *a0) {
    short *pair = (short*)&((MemberCallback *)a0)->pmf; /* Cfront PMF: {delta, index, func / vtbl offset} */
    char *arg;
    int idx;
    int (*fnptr)(char *);

    arg = (arg = (char *)((MemberCallback *)a0)->obj, arg) + pair[0];
    if (pair[1] < 0) {
        fnptr = (int (*)(char *))((MemberCallback *)a0)->pmf.b;
    } else {
        int sel = ((Pair2 *)pair)->b; /* pmf.b, read through pair (a0 + 0xC moves the code) */
        idx = (sel == 0 && pair[0] == 0) ? 0x28 : sel;
        /* RAW: byte-lock: int-typed cfront PMF vtable dispatch (port-audit B): sel = vtbl + index * 8, the entry is {s16 delta, fn}; the int carrier is the matched form */
        sel = (int)((char*)(pair[1] << 3) + *(int*)(arg + idx));
        arg = *(short*)sel + arg;
        fnptr = *(int (**)(char *))((char*)sel + 4);
    }
    return fnptr(arg);
}

void game_readFloatParm_0000C05C(char *dst) {
    int tmp;
    stream_readS32_0000AD4C(&tmp);
    stream_readF32_0000AD10((float*)(dst + 0x10));
}

void game_readIntParm_0000C08C(char *dst) {
    int tmp;
    stream_readS32_0000AD4C(&tmp);
    stream_readS32_0000ADE0((int*)(dst + 0x10));
}

void game_readQuadParm_0000C0BC(char *dst) {
    int tmp;
    stream_readS32_0000AD4C(&tmp);
    stream_readQuad_0000AD88((Quad4*)(dst + 0x10));
}
/* 1-word zero alignment pad (truth 0xC0EC) between two MATCHED C fns: the
 * GLOBAL_ASM placeholder minimum is 8 bytes, so this emits one extra zero
 * word; the game_uso_block1 Makefile recipe drops the asserted-zero leftover
 * at 0xC0F0 (docs/MATCHING_WORKFLOW.md, asm-processor 1-word-pad defect). */
#pragma GLOBAL_ASM("asm/nonmatchings/game_uso/game_uso/game_uso_func_0000C0BC_pad.s")

static void TrickTable_readCount_0000C0F0(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

/* Struct assignment makes IDO emit the target 3-ints-per-iteration copy loop.
 * The Makefile patch strips relocations from the D_00000000 load and jal
 * words so objdiff sees the same raw .word baseline. */
static void TrickTable_readEntry_0000C12C(int *a0) {
    typedef struct {
        int v[16];
    } I16;
    I16 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 0x40);
    *(I16*)a0 = buf;
}

static void stream_readS32_0000C194(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

static void stream_readF32_0000C1D0(float *dst) {
    float buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

static void stream_readVec3_0000C20C(Vec3 *dst) {
    int pad_top[1];
    Tri3i raw;
    int pad_mid[2];
    Tri3i tmp;
    int pad_bot[2];
    CALL(fs_gdlReadFile)(&D_00000000, &raw, 12);
#ifdef PORT
    PORT_BE_SWAP32S(&raw, 12);
#endif
    tmp = raw;
    dst->x = *(float*)&tmp.a;
    dst->y = *(float*)&tmp.b;
    dst->z = *(float*)&tmp.c;
}

static void stream_read16_0000C27C(Quad4 *dst) {
    Quad4 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 16);
#ifdef PORT
    PORT_BE_SWAP32S(&buf, 16);
#endif
    *dst = buf;
}

/* TrickTable_ctor_0000C2D4: 28-insn alloc-init constructor with optional 3-arg sub-init. */
void *TrickTable_ctor_0000C2D4(void *a0, int a1) {
    if (a0 == 0) {
        a0 = (void*)CALL(mem_new)(0x2C);
        if (a0 == 0) goto end;
    }
    CALL(Node_ctor)(a0, (char*)&D_00000000 + 0x121C);
    NODE(a0)->vptr = (char *)&D_00000000;
    if (a1 != 0) {
        CALL(Node_Load)(a0, 1, a1);
    }
end:
    return a0;
}

/* C344-C3CC: 7 accessor fragments for a table at *(int**)&D_00000000, each
 * entry 0x40 bytes. Indexed by arg1 << 6. Templates:
 *   C344: return &entry[a1];               (entry pointer)
 *   C35C: return &entry[a1] + 0x10;        (entry + 0x10)
 *   C378: return entry[a1].field_0x2C;     (lw at +0x2C)
 *   C394: return entry[a1].field_0x34;
 *   C3B0: return entry[a1].field_0x38;
 *   C3CC: return entry[a1].field_0x30;
 * Unused $a0 is spilled to caller home slot — confirms IDO -O2 leaf-no-jal
 * variant of feedback-ido-unused-arg-save. */
int *TrickTable_getEntry_0000C344(int a0, int a1) {
    return (int*)((char*)(*(int**)&D_00000000) + (a1 << 6));
}

int *TrickTable_getEntry10_0000C35C(int a0, int a1) {
    return (int*)((char*)(*(int**)&D_00000000) + (a1 << 6) + 0x10);
}

int TrickTable_getInputMask_0000C378(int a0, int a1) {
    return *(int*)((char*)(*(int**)&D_00000000) + (a1 << 6) + 0x2C);
}

int TrickTable_getField34_0000C394(int a0, int a1) {
    return *(int*)((char*)(*(int**)&D_00000000) + (a1 << 6) + 0x34);
}

int TrickTable_getFlags_0000C3B0(int a0, int a1) {
    return *(int*)((char*)(*(int**)&D_00000000) + (a1 << 6) + 0x38);
}

int TrickTable_getPoints_0000C3CC(int a0, int a1) {
    return *(int*)((char*)(*(int**)&D_00000000) + (a1 << 6) + 0x30);
}

/* TrickTable_getCount_0000C3E8: EXACT 2026-09-05 (agent-g). The former "dead-arg-home
 * delay-slot CAP" was a SOURCE-LINE-LAYOUT artifact, not a compiler cap. IDO's
 * scheduler tie-breaks the independent arg-home store `sw a0,0(sp)` against the
 * return load by source line: with `return` on its OWN line the store keeps its
 * entry-line position and the load takes the jr delay (lui;sw;jr;lw); with
 * `return` on the SAME line as the function's `{` the store sinks into the jr
 * delay (lui;lw;jr;sw = target). Keep this definition on ONE line.
 * See docs/IDO_CODEGEN.md#same-line-brace-return-sinks-arg-home-c3e8. */
extern int game_uso_D_807FFBC4;
int TrickTable_getCount_0000C3E8(int a0) { return game_uso_D_807FFBC4; }

/* TrickTable_load_0000C3F8: 37-insn alloc-and-iter constructor. MATCHED 2026-06-20.
 *   func_C0F0(&D_807FFBC4);                   // setup, base held in $s3
 *   D_807FFBC0 = alloc(D_807FFBC4 << 6);      // ptr base held in $s2
 *   for (i = 0; (u32)i < (u32)D_807FFBC4; i++)
 *       func_C12C(D_807FFBC0 + i*0x40);
 *
 * Two DISTINCT globals (D_807FFBC4 = count in $s3, D_807FFBC0 = alloc'd-ptr in
 * $s2); reference them DIRECTLY (no held-pointer locals) so IDO promotes
 * &D_807FFBC4/&D_807FFBC0 into $s3/$s2 across the function. The C0F0/C12C calls
 * are intra-module (R_MIPS_26, resolve to the baked 0c00303c/0c00304b post-link).
 * LAND LEVER: hoist `i = 0;` ABOVE the `if (count)` so IDO fills the beqz delay
 * slot with `move s0,zero` instead of the alloc store `sw v0,0(s2)` — that
 * store/branch schedule swap was the last 2-word diff. */
extern int game_uso_D_807FFBC4;
extern int *game_uso_D_807FFBC0;
void TrickTable_load_0000C3F8(int *a0) {
    int i;
    int *p;

    TrickTable_readCount_0000C0F0(&game_uso_D_807FFBC4);
    p = (int*)XCALL(game_uso_func_055750, mem_new)(game_uso_D_807FFBC4 << 6);
    game_uso_D_807FFBC0 = p;
    i = 0;
    if (game_uso_D_807FFBC4 != 0) {
        do {
            TrickTable_readEntry_0000C12C((int*)((char*)game_uso_D_807FFBC0 + i * 0x40));
            i++;
        } while ((u32)i < (u32)game_uso_D_807FFBC4);
    }
}

/* 2026-06-24 (agent-e) BIG-SWING decode lever: add the per-stage s2-scratch
 * round-trip. GT (expected .o) shows EVERY one of the 32 sub-init stages does
 *   lw t,OFF(global); sw t,SLOT(sp); lw t,SLOT(sp); sw t,0(s2); ...; lw a2,0(s2)
 * i.e. the stage value is double-buffered: a descending per-stage spill slot,
 * THEN a single shared scratch word at s2=sp+0x2C (set once at entry, reused).
 * Prior body passed `val` directly (single spill) and omitted the s2 hop. Modeling
 * the shared word as a function-scope `volatile int vslot` written each stage and
 * reloaded for the 04A188 a2 arg reproduces the missing sw/lw 0(s2) pair x32.
 * Structure: alloc(0x444) main + import_0010D33C init + vtable@0x28, s1 sub-region
 * (p+0x13C or alloc 0x308), then 32 unrolled sub-inits at s1+8+N*0x18, each a
 * dead-sentinel-guarded alloc(0x18)/bump, registered via 04A188(obj,s1,vslot,1),
 * template ptr @0xC (F568+0xf78 float / F5B0+0xfc0 int), 0x14=0, 0x10=val/float. */
extern char game_uso_D_807FF568, game_uso_D_807FF5B0, game_uso_D_807FF74C, game_uso_D_807FF814, game_uso_D_807FF81C, game_uso_D_807FF820, game_uso_D_807FF824, game_uso_D_807FF828, game_uso_D_807FF82C, game_uso_D_807FF830, game_uso_D_807FF834, game_uso_D_807FF838, game_uso_D_807FF83C, game_uso_D_807FF840, game_uso_D_807FF844, game_uso_D_807FF848, game_uso_D_807FF84C, game_uso_D_807FF850, game_uso_D_807FF854, game_uso_D_807FF858, game_uso_D_807FF85C, game_uso_D_807FF860, game_uso_D_807FF864, game_uso_D_807FF868, game_uso_D_807FF86C, game_uso_D_807FF870, game_uso_D_807FF874, game_uso_D_807FF878, game_uso_D_807FF87C, game_uso_D_807FF880, game_uso_D_807FF884, game_uso_D_807FF888, game_uso_D_807FF88C, game_uso_D_807FF890, game_uso_D_807FF894, game_uso_D_807FF898;
extern int game_uso_func_055750();
extern int game_uso_func_04A188();
extern int import_0010D33C();
void RiderAnim_init_0000D458(s32);
extern char gu_c48c_tf0, gu_c48c_tf1, gu_c48c_tf2, gu_c48c_tf3, gu_c48c_tf4, gu_c48c_tf5, gu_c48c_tf6, gu_c48c_tf7, gu_c48c_tf8, gu_c48c_tf9, gu_c48c_tf10, gu_c48c_tf11, gu_c48c_tf12, gu_c48c_tf13, gu_c48c_tf14, gu_c48c_tf15, gu_c48c_ti0, gu_c48c_ti1, gu_c48c_ti2, gu_c48c_ti3, gu_c48c_ti4, gu_c48c_ti5, gu_c48c_ti6, gu_c48c_ti7, gu_c48c_ti8, gu_c48c_ti9, gu_c48c_ti10, gu_c48c_ti11, gu_c48c_ti12, gu_c48c_ti13, gu_c48c_ti14, gu_c48c_ti15;
/* RiderAnim_ctor_0000C48C: EXACT 2026-10-16 (agent-g), from 99.7 NM. Three levers:
 * (1) the 12 int-0x10 stages put `obj[0xC/4] = tmpl; obj[0x10/4] = K;` on ONE
 *     source line (as1 then emits the 0x10 store first, the target order);
 * (2) the per-stage 1-word scratch structs are function-scope u1..u32 declared
 *     BEFORE the shared `t` (s2 = &t lands at the bottom, sp+0x2C), with one
 *     pointer above them and `v1, obj` + a 3-word volatile pad between u32 and t
 *     (frame 0xC8, u1 at 0xC0);
 * (3) plain C now; no cfront/NCC donor needed. See docs/IDO_CODEGEN.md
 *     #same-line-store-pair-and-scratch-decl-order-c48c.
 * (4) the two float stages spell plain `0.7f` / `1.72f` literals (RoData
 *     +0x1EC/+0x1F0, the words the old .s carried as .late_rodata); the D204
 *     donor placeholder below emits D204's 28555.0f at +0x1F4. Extern-symbol
 *     spellings emitted no rodata and shifted every later %lo by 12 bytes. */
struct C48CArg { int v; };
/* each stage builds one RiderAnimParm in place: vptr = the float / int parm vtable, value, unk14 */
#define RAP(o) ((RiderAnimParm *)(o))
void *RiderAnim_ctor_0000C48C(char *p, int a1, int a2) {
    int *s1;
    struct C48CArg u1;
    struct C48CArg u2;
    struct C48CArg u3;
    struct C48CArg u4;
    struct C48CArg u5;
    struct C48CArg u6;
    struct C48CArg u7;
    struct C48CArg u8;
    struct C48CArg u9;
    struct C48CArg u10;
    struct C48CArg u11;
    struct C48CArg u12;
    struct C48CArg u13;
    struct C48CArg u14;
    struct C48CArg u15;
    struct C48CArg u16;
    struct C48CArg u17;
    struct C48CArg u18;
    struct C48CArg u19;
    struct C48CArg u20;
    struct C48CArg u21;
    struct C48CArg u22;
    struct C48CArg u23;
    struct C48CArg u24;
    struct C48CArg u25;
    struct C48CArg u26;
    struct C48CArg u27;
    struct C48CArg u28;
    struct C48CArg u29;
    struct C48CArg u30;
    struct C48CArg u31;
    struct C48CArg u32;
    int *v1;
    int *obj;
    volatile int pd[3];
    struct C48CArg t;
    if (p == 0) { p = (char *)XCALL(game_uso_func_055750, mem_new)(0x444); if (p == 0) goto end; }
    import_0010D33C(p, a1);
    NODE(p)->vptr = (char *)&game_uso_D_807FF74C;
    s1 = (int *)(p + 0x13C);
    if (p == (char *)-0x13C) { s1 = (int *)XCALL(game_uso_func_055750, mem_new)(0x308); if (s1 == 0) goto tail; }
    v1 = s1;
    if (s1 == 0) { v1 = (int *)XCALL(game_uso_func_055750, mem_new)(8); if (v1 == 0) goto stage0; }
    ((RiderAnimParms *)v1)->name = (char *)&game_uso_D_807FF814 + kStr_Parms_1224; /* "Parms" */
    ((RiderAnimParms *)v1)->unk04 = 0;
stage0:;
    {
        u1.v = *(int *)((char *)&game_uso_D_807FF81C + kTag_lnc_122C);
        t = u1;
        obj = (int *)((char *)s1 + 0x8);
        if (s1 != (int *)-0x8 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_tf0 + 0xf78);
        RAP(obj)->unk14 = 0;
        RAP(obj)->value.f =0.7f;
        }
    }
    {
        u2.v = *(int *)((char *)&game_uso_D_807FF820 + kTag_jct_1230);
        t = u2;
        obj = (int *)((char *)s1 + 0x20);
        if (s1 != (int *)-0x20 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_ti0 + 0xfc0); RAP(obj)->value.i =20;
        RAP(obj)->unk14 = 0;
        }
    }
    {
        u3.v = *(int *)((char *)&game_uso_D_807FF824 + kTag_jht_1234);
        t = u3;
        obj = (int *)((char *)s1 + 0x38);
        if (s1 != (int *)-0x38 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_ti1 + 0xfc0); RAP(obj)->value.i =60;
        RAP(obj)->unk14 = 0;
        }
    }
    {
        u4.v = *(int *)((char *)&game_uso_D_807FF828 + kTag_jot_1238);
        t = u4;
        obj = (int *)((char *)s1 + 0x50);
        if (s1 != (int *)-0x50 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_ti2 + 0xfc0); RAP(obj)->value.i =5;
        RAP(obj)->unk14 = 0;
        }
    }
    {
        u5.v = *(int *)((char *)&game_uso_D_807FF82C + kTag_jca_123C);
        t = u5;
        obj = (int *)((char *)s1 + 0x68);
        if (s1 != (int *)-0x68 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_tf1 + 0xf78);
        RAP(obj)->unk14 = 0;
        RAP(obj)->value.f =1.72f;
        }
    }
    {
        u6.v = *(int *)((char *)&game_uso_D_807FF830 + kTag_jlt_1240);
        t = u6;
        obj = (int *)((char *)s1 + 0x80);
        if (s1 != (int *)-0x80 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_ti3 + 0xfc0); RAP(obj)->value.i =20;
        RAP(obj)->unk14 = 0;
        }
    }
    {
        u7.v = *(int *)((char *)&game_uso_D_807FF834 + kTag_jli_1244);
        t = u7;
        obj = (int *)((char *)s1 + 0x98);
        if (s1 != (int *)-0x98 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_tf2 + 0xf78);
        RAP(obj)->unk14 = 0;
        RAP(obj)->value.f =3.0f;
        }
    }
    {
        u8.v = *(int *)((char *)&game_uso_D_807FF838 + kTag_rfc_1248);
        t = u8;
        obj = (int *)((char *)s1 + 0xb0);
        if (s1 != (int *)-0xb0 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_tf3 + 0xf78);
        RAP(obj)->unk14 = 0;
        RAP(obj)->value.f =7.0f;
        }
    }
    {
        u9.v = *(int *)((char *)&game_uso_D_807FF83C + kTag_tkt_124C);
        t = u9;
        obj = (int *)((char *)s1 + 0xc8);
        if (s1 != (int *)-0xc8 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_ti4 + 0xfc0); RAP(obj)->value.i =3;
        RAP(obj)->unk14 = 0;
        }
    }
    {
        u10.v = *(int *)((char *)&game_uso_D_807FF840 + kTag_b1f_1250);
        t = u10;
        obj = (int *)((char *)s1 + 0xe0);
        if (s1 != (int *)-0xe0 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_tf4 + 0xf78);
        RAP(obj)->unk14 = 0;
        RAP(obj)->value.f =0.0f;
        }
    }
    {
        u11.v = *(int *)((char *)&game_uso_D_807FF844 + kTag_b2f_1254);
        t = u11;
        obj = (int *)((char *)s1 + 0xf8);
        if (s1 != (int *)-0xf8 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_tf5 + 0xf78);
        RAP(obj)->unk14 = 0;
        RAP(obj)->value.f =4.0f;
        }
    }
    {
        u12.v = *(int *)((char *)&game_uso_D_807FF848 + kTag_xf1_1258);
        t = u12;
        obj = (int *)((char *)s1 + 0x110);
        if (s1 != (int *)-0x110 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_tf6 + 0xf78);
        RAP(obj)->unk14 = 0;
        RAP(obj)->value.f =50.0f;
        }
    }
    {
        u13.v = *(int *)((char *)&game_uso_D_807FF84C + kTag_f1f_125C);
        t = u13;
        obj = (int *)((char *)s1 + 0x128);
        if (s1 != (int *)-0x128 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_tf7 + 0xf78);
        RAP(obj)->unk14 = 0;
        RAP(obj)->value.f =25.0f;
        }
    }
    {
        u14.v = *(int *)((char *)&game_uso_D_807FF850 + kTag_f2f_1260);
        t = u14;
        obj = (int *)((char *)s1 + 0x140);
        if (s1 != (int *)-0x140 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_tf8 + 0xf78);
        RAP(obj)->unk14 = 0;
        RAP(obj)->value.f =40.0f;
        }
    }
    {
        u15.v = *(int *)((char *)&game_uso_D_807FF854 + kTag_f3f_1264);
        t = u15;
        obj = (int *)((char *)s1 + 0x158);
        if (s1 != (int *)-0x158 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_tf9 + 0xf78);
        RAP(obj)->unk14 = 0;
        RAP(obj)->value.f =100.0f;
        }
    }
    {
        u16.v = *(int *)((char *)&game_uso_D_807FF858 + kTag_f4f_1268);
        t = u16;
        obj = (int *)((char *)s1 + 0x170);
        if (s1 != (int *)-0x170 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_tf10 + 0xf78);
        RAP(obj)->unk14 = 0;
        RAP(obj)->value.f =120.0f;
        }
    }
    {
        u17.v = *(int *)((char *)&game_uso_D_807FF85C + kTag_l1f_126C);
        t = u17;
        obj = (int *)((char *)s1 + 0x188);
        if (s1 != (int *)-0x188 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_tf11 + 0xf78);
        RAP(obj)->unk14 = 0;
        RAP(obj)->value.f =80.0f;
        }
    }
    {
        u18.v = *(int *)((char *)&game_uso_D_807FF860 + kTag_l2f_1270);
        t = u18;
        obj = (int *)((char *)s1 + 0x1a0);
        if (s1 != (int *)-0x1a0 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_tf12 + 0xf78);
        RAP(obj)->unk14 = 0;
        RAP(obj)->value.f =100.0f;
        }
    }
    {
        u19.v = *(int *)((char *)&game_uso_D_807FF864 + kTag_l3f_1274);
        t = u19;
        obj = (int *)((char *)s1 + 0x1b8);
        if (s1 != (int *)-0x1b8 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_tf13 + 0xf78);
        RAP(obj)->unk14 = 0;
        RAP(obj)->value.f =150.0f;
        }
    }
    {
        u20.v = *(int *)((char *)&game_uso_D_807FF868 + kTag_r1f_1278);
        t = u20;
        obj = (int *)((char *)s1 + 0x1d0);
        if (s1 != (int *)-0x1d0 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_tf14 + 0xf78);
        RAP(obj)->unk14 = 0;
        RAP(obj)->value.f =100.0f;
        }
    }
    {
        u21.v = *(int *)((char *)&game_uso_D_807FF86C + kTag_r2f_127C);
        t = u21;
        obj = (int *)((char *)s1 + 0x1e8);
        if (s1 != (int *)-0x1e8 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_tf15 + 0xf78);
        RAP(obj)->unk14 = 0;
        RAP(obj)->value.f =150.0f;
        }
    }
    {
        u22.v = *(int *)((char *)&game_uso_D_807FF870 + kTag_dm1_1280);
        t = u22;
        obj = (int *)((char *)s1 + 0x200);
        if (s1 != (int *)-0x200 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_ti5 + 0xfc0);
        RAP(obj)->unk14 = 0;
        RAP(obj)->value.i =0;
        }
    }
    {
        u23.v = *(int *)((char *)&game_uso_D_807FF874 + kTag_dm2_1284);
        t = u23;
        obj = (int *)((char *)s1 + 0x218);
        if (s1 != (int *)-0x218 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_ti6 + 0xfc0);
        RAP(obj)->unk14 = 0;
        RAP(obj)->value.i =0;
        }
    }
    {
        u24.v = *(int *)((char *)&game_uso_D_807FF878 + kTag_dm3_1288);
        t = u24;
        obj = (int *)((char *)s1 + 0x230);
        if (s1 != (int *)-0x230 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_ti7 + 0xfc0);
        RAP(obj)->unk14 = 0;
        RAP(obj)->value.i =0;
        }
    }
    {
        u25.v = *(int *)((char *)&game_uso_D_807FF87C + kTag_dm4_128C);
        t = u25;
        obj = (int *)((char *)s1 + 0x248);
        if (s1 != (int *)-0x248 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_ti8 + 0xfc0);
        RAP(obj)->unk14 = 0;
        RAP(obj)->value.i =0;
        }
    }
    {
        u26.v = *(int *)((char *)&game_uso_D_807FF880 + kTag_st1_1290);
        t = u26;
        obj = (int *)((char *)s1 + 0x260);
        if (s1 != (int *)-0x260 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_ti9 + 0xfc0); RAP(obj)->value.i =1;
        RAP(obj)->unk14 = 0;
        }
    }
    {
        u27.v = *(int *)((char *)&game_uso_D_807FF884 + kTag_st2_1294);
        t = u27;
        obj = (int *)((char *)s1 + 0x278);
        if (s1 != (int *)-0x278 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_ti10 + 0xfc0); RAP(obj)->value.i =1;
        RAP(obj)->unk14 = 0;
        }
    }
    {
        u28.v = *(int *)((char *)&game_uso_D_807FF888 + kTag_st3_1298);
        t = u28;
        obj = (int *)((char *)s1 + 0x290);
        if (s1 != (int *)-0x290 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_ti11 + 0xfc0); RAP(obj)->value.i =1;
        RAP(obj)->unk14 = 0;
        }
    }
    {
        u29.v = *(int *)((char *)&game_uso_D_807FF88C + kTag_sl1_129C);
        t = u29;
        obj = (int *)((char *)s1 + 0x2a8);
        if (s1 != (int *)-0x2a8 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_ti12 + 0xfc0); RAP(obj)->value.i =1;
        RAP(obj)->unk14 = 0;
        }
    }
    {
        u30.v = *(int *)((char *)&game_uso_D_807FF890 + kTag_sl2_12A0);
        t = u30;
        obj = (int *)((char *)s1 + 0x2c0);
        if (s1 != (int *)-0x2c0 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_ti13 + 0xfc0); RAP(obj)->value.i =1;
        RAP(obj)->unk14 = 0;
        }
    }
    {
        u31.v = *(int *)((char *)&game_uso_D_807FF894 + kTag_slm_12A4);
        t = u31;
        obj = (int *)((char *)s1 + 0x2d8);
        if (s1 != (int *)-0x2d8 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_ti14 + 0xfc0); RAP(obj)->value.i =30;
        RAP(obj)->unk14 = 0;
        }
    }
    {
        u32.v = *(int *)((char *)&game_uso_D_807FF898 + kTag_stm_12A8);
        t = u32;
        obj = (int *)((char *)s1 + 0x2f0);
        if (s1 != (int *)-0x2f0 || (obj = (int *)XCALL(game_uso_func_055750, mem_new)(0x18)) != 0) {
        XCALL(game_uso_func_04A188, Parms_addParm)(obj, s1, t, 1);
        RAP(obj)->vptr = (void *)((char *)&gu_c48c_ti15 + 0xfc0); RAP(obj)->value.i =100;
        RAP(obj)->unk14 = 0;
        }
    }
tail:
    ((RiderAnim *)p)->rider = (SnowBoarder *)a2;
    RiderAnim_init_0000D458(p);
end:
    return p;
}

/* RiderAnim_update_0000D204 (0x214 / 133 words): per-frame update. Gate on
 * import->0x64: clamp self->0xB4->0xBC against D+0x1F4 (reset self->0x120)
 * and set/clear the child mode bits import->0x138->0xB4 & 3; trigger
 * 07C0C0 when the 0xB4 object's 0x938 flag is clear and its 0x9D0 < 700 /
 * 0x31C < -100; D9CC when self->0x114; then `if (self->pm) (self->*pm)();`
 * (pointer-to-member at +200); ECE8; 076E78 (AnimDataMgr_lookup) + latch 0x74 into 0x104; 0775F8.
 * EXACT via REPLACE_FUNC_BODY donor splice: game_uso is C++, and the real
 * body lives in the IDO 7.1 NCC (C++) donor src/game_uso/game_uso_ncc_D204.cpp
 * (133/133 words + all 13 relocs identical). The pointer-to-member compare
 * (2-word `at`/`addi` loop vs the null PMF at D_807FF3B8+0xDC8) and the
 * delta/index/vptr dispatch are edg-only codegen; cfe capped at 85.8%.
 * docs/MATCHING_WORKFLOW.md#game-uso-ncc-cplusplus-donor-recipe. The
 * 0xD210 alias label of the old .s is not a real entry (nothing references
 * it). Body below is a placeholder for the splice; its 28555.0f literal
 * reserves D204's RoData word +0x1F4 (the splice drops the placeholder's
 * own relocs, the .rodata word stays). */
void RiderAnim_update_0000D204(int *a0) {
    *(volatile float *)a0 = 28555.0f;
}

void RiderAnim_readParms_0000D418(char *a0) {
    CALL(Parms_load_00035C6C)(&RA(a0)->parms);
}

/* RiderAnim's onKey override (vtable Data 0x115C slot 10, +0x5C): playback passed the armed key (or the
 * blend-in ended), so disarm both events and enter the queued state (queuedState 0xC0 -> state 0xC8).
 * MATCH: copying the adjacent pair as a STRUCT (Pair2) emits lw/lw/sw/sw with the
 * target's t7/t6 register order; two separate s32 field-copies gave t6/t7 swapped
 * (was INSN_PATCH'd, banned 2026-05-23). Struct-copy vs separate-field-copy is the
 * lever for adjacent-field $t-renumber. Byte-exact. */
void RiderAnim_onKey_0000D438(char *a0) {
    RA(a0)->player.eventKey = ANIM_NO_KEY;
    RA(a0)->player.blendEvent = ANIM_NO_KEY;
    RA(a0)->state = RA(a0)->queuedState;
}

/* RiderAnim_init_0000D458: parent F1 (frame -0x48, 90 words/0x164) + 4 helper
 * functions it calls (D5BC/D5DC/D5F8/D634, defs below; D5BC/D5DC/D634 100%).
 * Boundary defect RESOLVED: this .s is truncated to 0x164 (F1 only, ends at the
 * 0xD5B8 nop) and expected/game_uso.c.o carries D458 at the F1 size — verified.
 * EXACT 2026-10-15 (agent-g, 98.61 -> 100; the 2026-06-23 "DECODE-ONLY CAP"
 * verdict was wrong on both counts):
 *   (1) the HI16-only D refs are `(char *)&game_uso_D_807FF3xx + 0xDB0/0xDB8/
 *       0xDC0` -- the offset bakes into the addiu exactly like FC34's pairs; the
 *       build's extra LO16 relocs are name/reloc-blind in the report and the raw
 *       .text bytes match;
 *   (2) `subu a1,s0,s0; addiu a1,a1,124` is a POINTER DIFFERENCE of two
 *       same-base addresses, `(char *)arg0 + 0x138 - ((char *)arg0 + 0xBC)` --
 *       IDO folds the +0x7C but not the base - base (an `x - x` or `x * 0`
 *       does fold to li);
 *   (3) frame 0x48 = `s32 pad[2]` declared above sp30 (one int is not enough:
 *       it shifts sp30 down instead).
 * Logic was decode-verified 2026-06-23: var_v0 nested-if shape (beq;li;b;move)
 * and the 16-byte &sp30[4] buffer (048020 size arg 0x10) match target.
 * DO NOT use split-fragments.py here: it DROPS the symbolic `lui %hi` line at
 * the D5DC boundary (size header says +1 word vs the file's word count),
 * corrupting the .s — see docs/MATCHING_WORKFLOW. */
#ifndef FW
#define FW(p, o) (*(int *)((char *)(p) + (o)))
#endif
extern int game_uso_func_07F6C0();
extern int game_uso_func_077574();
extern int game_uso_func_077C44();
extern int import_0010D420();
extern int game_uso_func_049280();
extern int game_uso_func_048020();
extern void RiderAnim_enterState_0000D5BC(char *, Pair2);
extern char import_800200CC;
extern char game_uso_D_807FF3A0, game_uso_D_807FF3A8, game_uso_D_807FF3B0;
void RiderAnim_init_0000D458(s32 arg0) {
    s32 pad[2];
    s32 sp30[4];
    s32 var_v0;

    game_uso_func_07F6C0((char *)arg0 + 0xBC, (char *)arg0 + 0x138 - ((char *)arg0 + 0xBC)); /* UNNAMED: mid-entry jal gl_bzero+0xC (game TextReloc T+0xD470 -> bootup:gl_bzero+0xC, uso-imports.py sites game: bzero(ptr, len), entry 12 bytes into the build function, no symbol of its own) */
    RA(arg0)->trickPhase = 2;
    RA(arg0)->animSet = RANIM_GROUP_REGULAR;
    RA(arg0)->rideContacts = 1;
    RA(arg0)->loadedAnim = -1;
    RA(arg0)->hitKind = 0;
    RA(arg0)->hitScale = 1.0f;
    XCALL(game_uso_func_077574, AnimPlayer_reset)(arg0);
    XCALL(game_uso_func_077C44, AnimPlayer_start)(arg0, RA(arg0)->animSet | RANIM_UP_DOWN, 0, 0, 1, 1);
    import_0010D420(arg0);
    if (INPUT_MGR(import_800200CC).nextFlowMode == 3) {
        RiderAnim_enterState_0000D5BC(arg0, *(Pair2*)((char*)&game_uso_D_807FF3A0 + kPmf_RiderAnim_stateStartIntro));
    } else {
        RiderAnim_enterState_0000D5BC(arg0, *(Pair2*)((char*)&game_uso_D_807FF3A8 + kPmf_RiderAnim_stateStartWait));
    }
    XCALL(game_uso_func_049280, sys_writebackDCacheAll)();
    XCALL(game_uso_func_048020, dma_read)(&sp30, 0xF10, 0x10);
#ifdef PORT /* the boot-code check reads raw big-endian ROM words (IPL3 at 0xF10): compare them as the N64 does */
    PORT_BE_SWAP32S(sp30, 0x10);
#endif
    if (sp30[0] != 0x04080040) {
        var_v0 = 0;
    } else if (sp30[1] != 0x02081040) {
        var_v0 = 0;
    } else {
        var_v0 = 1;
    }
    if (var_v0 == 0) {
        RiderAnim_enterState_0000D5BC(arg0, *(Pair2*)((char*)&game_uso_D_807FF3B0 + kPmf_RiderAnim_stateStopped));
    }
}

/* RiderAnim_enterState_0000D5BC: copies the varargs pair (a1,a2 via &a1) to the
 * adjacent a0->0xC8/0xCC.
 * MATCH: a STRUCT copy `*(Pair2*)(a0+0xC8) = *(Pair2*)&a1` homes the args, takes
 * &a1 into $t6, and emits the lw/sw/lw/sw via the pointer in the target's regs.
 * The volatile-int-pointer form left the &a1 pointer in $v0 (was a regalloc cap);
 * struct-copy is the lever (same as RiderAnim_onKey_0000D438). Byte-exact. */
void RiderAnim_enterState_0000D5BC(char *a0, Pair2 pair) {
    RA(a0)->state = pair;
}

/* Copy the adjacent global pair D+0xDC8/0xDCC to adjacent a0->0xC8/0xCC.
 * MATCH: struct-copy lever (*(Pair2*)(a0+0xC8) = *(Pair2*)(&D+0xDC8)) emits
 * lw/sw/lw/sw via the &D pointer in the target's $t order; separate field copies
 * gave the wrong $t order. Same lever as D438/D5BC (here the source is a global).
 * Byte-exact (in-tree verified). */
extern char game_uso_D_807FF3B8;
void RiderAnim_clearState_0000D5DC(char *a0) {
    RA(a0)->state = *(Pair2*)((char*)&game_uso_D_807FF3B8 + kPmfNull_DC8); /* the null PMF */
}

/* Varargs (&a1) pair-sink: if a3==-1 set a0->0x68=a3 else a0->0x64=a3; then copy
 * the a1/a2 pair to adjacent a0->0xC0/0xC4 via the struct-by-value `pair` arg
 * (homes a1,a2 to 4/8(sp)). Signature `(char*, Pair2, int)` gives the direct
 * jal from the in-TU callers (00011258/000112E0).
 *
 * MATCHED 2026-06-20 (stale-cap catch): the prior cap claimed the else-arm
 * direct sp-relative spill reload (`lw t7,0xC(sp); sw t7,0x64(a0)`) was
 * allocator-only / not C-reachable, because `*(volatile int*)&a3` materialized
 * the address (addiu t7,sp,0xC; lw 0(t7)) = +1 insn (16 vs 15). The fix is to
 * DROP the `volatile`: plain `*(int*)&a3` forces the a3 home (sw a3,0xC(sp) in
 * the bne delay) AND lets IDO fold the reload to a direct sp-offset load.
 * 15/15 words byte-exact, 0 diffs. */
void RiderAnim_queueState_0000D5F8(char *a0, Pair2 pair, int a3) {
    if (a3 == -1) {
        RA(a0)->player.blendEvent = a3;
    } else {
        RA(a0)->player.eventKey = *(int*)&a3;
    }
    RA(a0)->queuedState = pair;
}

/* 2-insn leaf: jr ra; sw a0, 0(sp) (a0 spilled to its caller-arg slot in the
 * delay slot, no prologue). Byte-exact: `void f(int a0){(void)a0;}` — the
 * (void)a0 cast makes IDO -O2 emit the arg-save `sw a0, 0(sp)` and the empty
 * leaf fills the jr delay slot with it. (Earlier "unmatchable" comment was
 * wrong — verified raw-diff=0, reloc-free.) */
void RiderAnim_display_0000D634(int a0) {
    (void)a0;
}

/* RiderAnim_endTrick_0000D63C: ramp-down/clamp dispatch. 99.17% objdiff / 36-of-42
 * raw words (2026-07-03 agent-e). See the NM-wrap comment below for the three
 * levers (value=call2-arg4, `value|0` -> caller-saved $a3, `idx*0+10` -> single
 * `li`) and the residual candidate-numbering temp-pool/spill-slot cap. */
extern int import_000B8698();
extern int game_uso_func_022784();
extern char import_800201D0;
extern char game_uso_D_807FF7B4;
extern int import_000B8698();
extern int game_uso_func_022784();
extern char import_800201D0;
extern char game_uso_D_807FF7B4;
/* RiderAnim_endTrick_0000D63C: ramp-down/clamp dispatch. EXACT 2026-07-15 (was
 * 99.17% "candidate-numbering temp-pool / spill-slot cap"). Final two levers:
 *   (4) value declared FIRST (before idx) -> value's spill home takes the
 *       HIGHER local slot 0x24 (decl-order home rank, first decl = highest).
 *   (5) `idx<<0<<1<<1` split-shift phantom for the table index: each phantom
 *       shift consumes one ugen temp-ring slot with zero emission, advancing
 *       the ring so the real sll lands in $t8 (not $t7) and the tail li in
 *       $t9 -- the documented +1 temp-pool phase shift. NOTE decl-order and
 *       ring phase INTERACT: value-first alone re-phased the ring back, so
 *       both levers must be applied together ((idx<<1<<1) sufficed with
 *       idx-first decls). Levers (1)-(3) from the 2026-07-03 pass retained:
 *       value=call2-arg4, `value | 0` -> caller-saved $a3, `(idx*0)+10`. */
void RiderAnim_endTrick_0000D63C(char *a0, int a1) {
    int value;
    int idx = RA(a0)->comboCount;

    value = 0;

    if (idx != 0 && a1 != 0) {
        if (idx > 0) {
            if (idx >= 10) {
                idx = (idx * 0) + 10;
                RA(a0)->comboCount = idx;
            }
            value = *(int*)((char*)&game_uso_D_807FF7B4 + (idx<<0<<1<<1));
            if (value != 0) {
                import_000B8698((int)INPUT_MGR(import_800201D0).dataMgr,
                                (int)RA(a0)->rider, value | 0, value);
            }
        }
    }
    RA(a0)->lastTrick = 0;
    RA(a0)->comboCount = 0;
    XCALL(game_uso_func_022784, RaceTracker_endTrick)((int)INPUT_MGR(import_800201D0).dataMgr,
                         (int)RA(a0)->rider, a1, value);
    RA(a0)->endTrickFlag = 1;
}

/* 26 insns. Toggle bit 0x40 in (a0->0xB4)[0xA58], call worker, test the
 * bit afterward, dispatch a0->0xFC to one of two flag values. Promoted
 * 2026-05-14 from 93.73% NM to byte-exact in C-only:
 *   (a) `int *p = base + 0xA58` named local forces target's
 *       `addiu v0, v0, 0xA58` (without it, IDO CSE-folds offset into
 *       the lw/sw pair using the same base reg).
 *   (b) Swap if/else arms + invert condition fixes both branch
 *       direction (bnel → beql) AND lui register allocation order
 *       (which value goes into t1 vs t2). */
extern int game_uso_func_00E124();
void RiderAnim_flipStance_0000D6E4(char *a0) {
    int *p = (int *)&RA(a0)->rider->flags;
    *p ^= 0x40;
    XCALL(game_uso_func_00E124, SnowBoarder_mirrorFlip_0000E124)(RA(a0)->rider, a0);
    if ((*(int *)&RA(a0)->rider->flags & 0x40) != 0) {
        RA(a0)->animSet = 0x00050000;
    } else {
        RA(a0)->animSet = 0x00010000;
    }
}

/* Conditional 3-call state update. BYTE-EXACT 2026-06-20.
 * The prior "v0/v1 regalloc swap + beql/beq" cap was MISDIAGNOSED: it came
 * from the `int result = 0; ...; result = 1; return result;` single-return
 * shape, which spans `result` to the tail and makes IDO pick $v1 + a
 * branch-likely (beqzl) + trailing `or v0,v1`. Rewriting as the POSITIVE
 * two-return form — `if (cond) { body; return 1; } return 0;` (no `result`
 * local) — preinits $v0=0, emits a plain `beqz t7,end; nop` fallthrough, and
 * a `b end; li v0,1` tail with $v0 throughout, matching the target exactly.
 * Dropping the `result` local also keeps `arg`'s spill at 0x24 (the extra
 * local had pushed it to 0x20). */
int RiderAnim_trickCrash_0000D74C(char *a0) {
    int arg;
    if (RA(a0)->rider->contactCount) {
        CALL(RiderAnim_setHitKind_0000EDCC)(a0, 1);
        CALL(RiderAnim_applyDamage_0000EDD4)(a0, 2, 0);
        arg = RA(a0)->animSet | RANIM_CRASH_SLIDE;
        if (RA(a0)->rider->health <= 0) {
            arg = 0x0006000A;
            RA(a0)->rider->slowdown = 0x64;
        }
        CALL(RiderAnim_hitFeedback_0001189C)(a0, 3, 0, arg);
        CALL(RiderAnim_playHitAnim_000115EC)(a0, arg);
        CALL(RiderAnim_endTrick_0000D63C)(a0, 0);
        return 1;
    }
    return 0;
}

/* Conditional fall-below-floor update. BYTE-EXACT. The E90 table pair is passed
 * BY VALUE as `*(Pair2*)((char*)&D_00000000 + 0xE90)`: the base+offset ADDEND form
 * (NOT a separate `&D_00000E90` symbol) emits `lui %hi; addiu base,0xE90; lw 0; lw 4`
 * with the 0xE90 baked into the addiu as a literal addend (matching the target's
 * HI16-only reloc) AND homes a1/a2 to sp+4/sp+8 via the struct-by-value spill —
 * same proven form as the byte-exact neighbor RiderAnim_checkBrake_0000D8A8 (uses +0xE70).
 * The old `&D_00000E90` form emitted a HI16+LO16 pair → addiu imm=0 (1-diff residual
 * after the banned INSN_PATCH that previously faked it was removed 2026-05-23).
 * See docs/MATCHING_WORKFLOW.md addend form. */
void RiderAnim_checkReverseBrake_0000D7F4(char *a0) {
    SnowBoarder *data = RA(a0)->rider;
    if (data->fwdSpeed < -20.0f) {
        if (data->contactCount != 0) {
            CALL(RiderAnim_applyDamage_0000EDD4)(a0, 1, 2);
            CALL(AnimPlayer_start)(a0, RA(a0)->animSet | RANIM_BUMP_R_BREAK, 0, 1, 1, 1);
            CALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateResumeRide), 1);
            CALL(RiderAnim_clearState_0000D5DC)(a0);
            RA(a0)->trickPhase = 0;
        }
    }
}

/* MATCHED 2026-05-28: struct-by-value (E70/E74 pair) + inlined B4-deref.
 * The named t6 local landed the B4-child in $v0; inlining the deref into
 * the condition shifts it to $t6 (target's contiguous t6/t7/t8 numbering).
 * See docs/IDO_CODEGEN.md#feedback-ido-struct-by-value-homes-arg-pair. */
void RiderAnim_checkBrake_0000D8A8(char *a0) {
    if (RA(a0)->rider->crouch != 0) {
        CALL(RiderAnim_enterState_0000D5BC)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateBrakeBlendIn));
    }
}

/* RiderAnim_checkJump_0000D8EC: 56-insn dispatch + 3-call wrapper.
 *
 * Mixed control flow:
 *   if (a0->[0xB4]->[0x938] == 0) {  ; v1->[0x938] gate
 *       fall through to L_BC tail
 *   } else if (*(int*)&D_00000000 == 0) {  ; ptr gate
 *       a0->[0xE4]_h = 0; gl_func(a0); v1 = a0->[0xB4];
 *   } else {  ; main path
 *       int a1 = 0;
 *       if (a0->[0x118] != 0) {
 *           if (v1->[0x800]->[0x18] & 0x100) a1 = 1;
 *       } else {
 *           if (v1->[0x800]->[0x10] & 0x100) a1 = 1;
 *       }
 *       if (a1) {
 *           gl_func(a0, *(D+0xEB8), *(D+0xEBC));  ; 3-arg varargs
 *           a0->[0xE4]_h = 0; a0->[0x118] = 0;
 *       }
 *   }
 *   L_BC: t6 = v1->[0x9D4]; if (t6 >= 11) gl_func(a0); */
void RiderAnim_checkJump_0000D8EC(int *a0) {
    int *v0;
    int a1;

    if (RA(a0)->rider->contactCount == 0) goto L_BC;
    if (*(int**)&XDATA(D_00000000, port_game_data, 0x1094) != 0) {
        a1 = 0;
        if (RA(a0)->jumpRepress != 0) {
            v0 = (int *)RA(a0)->rider->input;
            if ((((RiderInput *)v0)->pressed & 0x100) != 0) a1 = 1;
        } else {
            v0 = (int *)RA(a0)->rider->input;
            if ((((RiderInput *)v0)->held & 0x100) != 0) a1 = 1;
        }
        if (a1 != 0) {
            CALL(RiderAnim_enterState_0000D5BC)(a0, *(Pair2*)((char*)&XDATA(D_00000000, port_game_data, 0x0) + kPmf_RiderAnim_statePreJump));
            RA(a0)->jumpCharge = 0;
            RA(a0)->jumpRepress = 0;
        }
    } else {
        RA(a0)->jumpCharge = 0;
        CALL(RiderAnim_jumpTakeoff_0000F360)(a0);
    }
L_BC:
    if (RA(a0)->rider->airFrames >= 11) {
        CALL(RiderAnim_startHop_0001001C)(a0);
    }
}

/* RiderAnim_selectAnim_0000D9CC (0x830 / 524 words): per-frame landing/turn-state
 * classifier. EXACT 2026-10-22 (agent-g run 121) via the IDO 7.1 NCC donor
 * src/game_uso/game_uso_ncc_D9CC.cpp (REPLACE_FUNC_BODY, both trees). The old
 * "4-word as1 branch-likely cap" was a decode error that the branch-masking
 * diffs hid: the first gate is TWO nested ifs (`if (x > 30.0f) { if (mask) {`),
 * not one `&&`, and the early `goto` jumps past BOTH closing braces. The mask
 * test and the 0x22C test then branch to the inner if's end label, which is
 * immediately followed by the outer one, so as1 has no instruction to steal
 * (`beqz; nop`, `bc1f; nop`), while the first gate's target starts with the
 * 500.0f `lui` (`bc1fl` + duplicated lui). Earlier cfe history (kits and
 * negatives) is in git log for this file and docs/IDO_CODEGEN.md kit XXV.
 * This placeholder re-emits the three .rodata doubles the INCLUDE_ASM .s
 * carried at +0x1F8..+0x210 (0.8 and -0.4 for this function, 0.1 for
 * RiderAnim_checkRow_0000E1FC); the donor reads them through data symbols whose
 * addends hold the offsets (the game_uso block is extracted unlinked). */
void RiderAnim_selectAnim_0000D9CC(char *arg0) {
    *(volatile double *)arg0 = 0.8;
    *(volatile double *)arg0 = -0.4;
    *(volatile double *)arg0 = 0.1;
}

// RiderAnim_checkRow_0000E1FC — STRUCTURAL PASS (0xD4 / 53 words,
// no episode). Raw-.word USO form (single function, game_uso main
// game-logic; boundary already split by commit 085ef620 — named fn
// still undecoded). Hand-decoded.
//
// Threshold-gated event / effect trigger.
//
//   void RiderAnim_checkRow_0000E1FC(Obj *obj, A a1, A a2) {
//     Sub *s = obj->0xB4;
//     if (!(s->0x348 < s->0x10)) return;                  // timer/state
//     if (!(obj->0x800->0x18 & 0x2000)) return;            // flag gate
//     if (s->0x9CC == K) {                                 // table id
//       float m = fabsf(s->0x970);                          // magnitude
//       if ((double)m < DBL(D_00000E68 + 0x208)) {           // folded f64
//         func_00000000(a1->0x0, a2->0x0);                   // event A
//       } else {
//         func_00000000(a1->0x0, a2->0x4);                   // event B
//       }
//     }
//   }
//
// Struct-typing reference:
//   obj: 0xB4 -> s (s->0x10 a threshold, s->0x348 a current value/
//     timer, s->0x970 a magnitude f32, s->0x9CC a table id), 0x740
//     a sub-anchor, 0x800 -> ctx (->0x18 flags, bit 0x2000 gate).
//   D_00000E68 / D_00000E50 = USO static tables; D_00000E68 + 0x208
//     = a folded f64 threshold constant. a1/a2 = event arg records.
//   func_00000000 = USO placeholder dispatcher (event report /
//     state notify).
// Caps (DEFERRED): raw-word USO + placeholder calls + folded f64; USO
//   mnemonic disasm limitation prevents byte-match. Real-C STRUCTURAL
//   body below — threshold-gated event/effect trigger skeleton.
//   Byte-match deferred. Name pre-checked: no extern reuse.
/* RiderAnim_checkRow_0000E1FC: threshold-gated event trigger. EXACT 2026-07-15
 * (was 99.62 "folded f64 / USO raw-word" deferred cap). Final lever: DE-NAME
 * the double threshold — inline `*(double*)((char*)&D + 0x208)` directly in
 * the compare instead of a named `double thr` local. Nameless expression =
 * fresh FP pool temps numbered at the use point (const ldc1 -> $f10, cvt.d.s
 * -> $f16, matching target); the named form let uopt number the load early
 * and reuse dead low regs (ldc1 $f0 / cvt $f10, 3-word residual). Same
 * de-naming lever as bootup B49C (int side). */
extern void RiderAnim_enterState_0000D5BC(char *, Pair2);
extern char game_uso_D_807FF458;
extern char game_uso_D_807FF440;
void RiderAnim_checkRow_0000E1FC(char *obj) {
    SnowBoarder *s = RA(obj)->rider;
    char *sub = (char *)s;
    float raw, m;
    if(1){ sub += 0x740; } /* &s->parms.min */
    if (!(((SnowBParm *)sub)->value > s->dnode.speed)) return;
    sub = (char *)s->input;
    if (!(((RiderInput *)sub)->pressed & 0x2000)) return;
    if (s->useDParms != 0) goto callA;
    raw = s->edge;
    m = (raw < 0.0f) ? -raw : raw;
    if (*(double *)((char *)&XDATA(game_uso_D_807FF458, port_game_rodata, 0x0) + kRoD_0p1_208) < (double)m) {
    callA:
        RiderAnim_enterState_0000D5BC(obj, *(Pair2 *)((char *)&XDATA(game_uso_D_807FF458, port_game_data, 0x0) + kPmf_RiderAnim_stateRowIn));
    } else {
        RiderAnim_enterState_0000D5BC(obj, *(Pair2 *)((char *)&game_uso_D_807FF440 + kPmf_RiderAnim_stateRow));
    }
}

/* RiderAnim_holdJumpLift_0000E2D0 - per-frame advance of a timed sub-object on game
 * object a0. Global gate at &D+0; if active (a0->0xE4 frame counter >= 0) and
 * the sub-block (a0->0xB4->0x800->0x10) has flag bits 0x300, advance the
 * counter and accumulate a0->0x1E4 into parent->0x31C when not frozen
 * (parent->0x9A8 bit 0); otherwise reset the counter to -1.
 * Match keys: (s16)frame used inline (not reassigned) so the load stays live
 * across the beql, splitting the sign-extension (sll/sra) and coloring the
 * extended temp into v1; if/else (not early-return) emits the beql + dead
 * store-tail. Exact. */
extern char game_uso_D_807FF684;
void RiderAnim_holdJumpLift_0000E2D0(a0)
char *a0;
{
    int frame;
    char *block;
    float *acc;

    if (*(int*)&game_uso_D_807FF684 == 0 && (frame = RA(a0)->jumpCharge, frame >= 0)) {
        block = (char *)RA(a0)->rider->input;
        if (((RiderInput *)block)->held & 0x300) {
            RA(a0)->jumpCharge = (s16)frame + 1;
            if ((s16)frame < RA(a0)->parms.jlt.value.i) {
                SnowBoarder *parent = RA(a0)->rider;
                acc = &parent->dnode.vel[1];
                if ((parent->groundFlags & 1) == 0) {
                    *acc += RA(a0)->parms.jli.value.f;
                }
            }
        } else {
            RA(a0)->jumpCharge = -1;
        }
    }
}

/* RiderAnim_startQueuedTrick_0000E35C (0x208 / 130 words): slot/queue advance. If
 * self->0xF0 (current) != NULL: EE30(self, cur->0x30, 1); import_000B8518
 * index call (ceil-div-by-64 node offset, two forms on obj->0xA58 & 0x20);
 * counter (0x100)++, F4 = F0, F0 = 0, counter reset when F8 == F4, F8 = F4;
 * E564(self); cur->0x20 from ->0x28 (0xA58 & 0x40 and nonzero) else ->0x24;
 * 077C44 + D5F8 (pointer-to-member by value) on cur->0x38 & 2; D5BC;
 * return 1. Else return 0.
 * EXACT via REPLACE_FUNC_BODY donor splice: the real body is the IDO 7.1 NCC
 * (C++) donor src/game_uso/game_uso_ncc_E35C.cpp (130/130 words), built with
 * the sibling F6D4 recipe (no named prev local; member re-reads give the
 * expression-temp colouring and the two-copy join). The cfe wrap capped at
 * 95.3%; its "unpaired HI16" note was a reloc-form difference that objdiff
 * and the ROM do not see. See docs/IDO_CODEGEN.md#ncc-no-named-copy-local-f6d4.
 * Body below is a placeholder for the splice. */
extern int *game_uso_D_807FFBC0;
extern char game_uso_D_807FF4B0;
extern char game_uso_D_807FF4B8;
extern char game_uso_D_807FF4C0;
extern void RiderAnim_addTrickPoints_0000EE30(char *, int, int);
extern void RiderAnim_playSpinSound_0000E564(int *);
extern int import_000B8518();
extern char import_800201D0;
int RiderAnim_startQueuedTrick_0000E35C(char *arg0) {
    volatile char *ret = arg0;
}

void RiderAnim_playSpinSound_0000E564(int *a0) {
    int v0 = RA(a0)->curTrick->inputMask;
    if (v0 & 0xC000) {
        CALL(SndHandle_setScaleIdx31)(RA(a0)->rider->sound);
        v0 = RA(a0)->curTrick->inputMask;
    }
    if (v0 & 0x3000) {
        CALL(SndHandle_setScaleIdx15)(RA(a0)->rider->sound);
    }
}

// RiderAnim_readTrickInput_0000E5C8 — STRUCTURAL PASS (0x354 / 213 words,
// no episode). Raw-.word USO form (single function, game_uso main
// game-logic). Pure bitfield work (1 call, no FP). ~85.6% (insn-mnemonic).
//
// 2026-06-23 (agent-i): callee CONFIRMED = RiderAnim_readStickSpin_0000EAF4 (was the
//   generic `func_00000000` placeholder → wrong reloc symbol). Now calls
//   RiderAnim_readStickSpin_0000EAF4 directly so the NM-build emits the correct
//   `jal RiderAnim_readStickSpin_0000EAF4` R_MIPS_26 (matches target reloc). No score
//   change but a genuine correctness/reloc fix.
//   CAP CONFIRMED — function will NOT land in either dispatch form:
//   (a) SWITCH-LOWERING: cases 1-6 are contiguous so IDO emits this `switch`
//       as a JUMPTABLE (adds a .rodata jumptable + HI16/LO16 rodata relocs
//       the target does NOT have). The target lowers the SAME switch as a
//       linear compare cascade (`li at,N; beq v0,at,Hn` then forward bodies,
//       v0=lw 0xE8 loaded ONCE) with NO rodata. Rewriting as C `if/else if`
//       produces `li const; bne v0,const,NEXT` with INLINE bodies (not the
//       target's beq-to-forward-handler) and drops mnemonic-ratio 0.856->0.60
//       AND removes the rodata — but the inline-body shape never converges.
//       No clean IDO-C steers a contiguous-1-6 switch to the target's
//       beq-chain-with-trailing-bodies. Genuine switch-lowering cap.
//   (b) A58-call gate: target materializes `a1=(v0&0x20)!=0` via sltu and
//       `move a2,v1` (raw 0x20) before the call; clean `if(v0&0x20)` C emits
//       a direct beqz (no sltu) — eval-order/bool-materialization cap.
//   (c) count-global `lui a2,%hi(D_00000000-region)` is HOISTED to just after
//       the jal (e6c4) — a no-reloc lui0 codegen the search-loop C can't pin.
//   (d) several beqzl/bnezl likely-branch polarity + loop sltu/bnez choices.
//
// 2026-05-31 DECODE FINDING (do NOT "fix" the switch to a cascade — it
// REGRESSES). The 0x12C-gated dispatch on obj->0xE8 (cases 5,6,2,1,4,3)
// is emitted by the TARGET as an if-else CASCADE (li-const + `beq`,
// reloading `lw 232(a3)` fresh before each compare), NOT the jumptable
// our `switch` currently produces. BUT rewriting it as the matching
// `if (E8==5)... else if (E8==6)...` cascade recovers the 4 cascade insns
// yet DROPS fuzzy 78.2%->76.2%: the else-if caches E8 in one reg (vs the
// target's reload-per-compare), and that register choice churns the
// downstream allocation more than the structural gain is worth. The
// jumptable `switch` form is the HIGHER-SCORING decode; keep it until a
// reload-forcing cascade form (one that reproduces the per-compare
// `lw 232`) is found. The function is also ~17 insns short overall vs
// target (213 vs 196) — multi-run structural work remains beyond the
// dispatch (the e8&0x4 bit-clear block + the stride-64 search tail).
//
// State-flag remapper: tests bits in a source flag word and
// bit-permutes them into the object's internal flag field obj->0xE8.
//
//   void RiderAnim_readTrickInput_0000E5C8(Obj *obj, A a1) {
//     src = obj->0xB4 ? obj->0xB4->0x800->0x10 : obj->0x18;
//     // long unrolled chain, one test per source bit:
//     //   if (src & 0x200)  obj->0xE8 |= 0x02;
//     //   if (src & 0x1000) obj->0xE8 |= 0x...;            // (etc.)
//     //   if (src & 0x4000) obj->0xE8 |= 0x01;
//     //   if (src & 0x2000) obj->0xE8 |= 0x10;
//     //   if (src & 0x4000) obj->0xE8 |= 0x20;
//     //   if (src & 0x8000) obj->0xE8 |= 0x40;
//     //   … ~16 such (mask -> outbit) pairs, each a
//     //   lw 0xE8 / andi src,MASK / branch / ori OUTBIT / sw 0xE8;
//     // tail: also folds obj->0xB4->0xA58 bits into obj->0xE8.
//   }
//
// Struct-typing reference:
//   obj: 0xB4 -> sub (->0x800->0x10 a hardware/input flag word,
//     ->0xA58 more flags), 0x18 a fallback flag source, 0xE8 the
//     object's internal/derived flag field (the permuted target).
//   Pure bit-permutation: each source bit maps to a distinct
//     internal bit (input/state translation table, unrolled).
//   func_00000000 = USO placeholder dispatcher (1 trailing call).
// FULL RE-TRACE 2026-05-31 (11.79% -> 77.93%): the prior body was a WRONG
//   skeleton (tested obj->0xB4 not a0->0xF4; local dst not 0xE8-RMW; 5/7 table
//   entries). Now fully structured: condition+src (a0->0xF4 picks ->0x18/->0x10),
//   full 7-block mask->outbit table, A58-gated call, 0x12C-gated switch on 0xE8,
//   bit-clearing (0xE8&0x4 + a3->0xF4->0x2C bits 0x5000/0xa000/0xc000/0x100000/
//   0x200000), 0xEC counter, and the stride-64 search loop over the table at
//   *(&D_00000000 + 0x15D0) (count = *(int*)&D_00000000), tail 0xE8 &= 0x6.
//   Residual (~22%, len 196 vs 213): regalloc-renumber + the A58-call sltu-bool
//   materialization + the count-global no-reloc lui0 codegen. Full structure +
//   offsets: memory project_1080_e5c8_full_decode_211insn.
#ifdef NON_MATCHING
// RiderAnim_readTrickInput_0000E5C8 — reconstruction pass (agent-e, 2026-06-24).
// State-flag remapper: reads a source flag word (a0->0xF4 picks
// a0->0xB4->0x800->0x18 vs ->0x10), bit-permutes it into a3->0xE8,
// folds a3->0xB4->0xA58 bits via an ALWAYS-MADE call to
// RiderAnim_readStickSpin_0000EAF4 (args materialized via sltu(0,bit)), runs the
// 0x12C-gated dispatch on 0xE8, the 0xE8&4 bit-clear block (a3->0xF4->0x2C
// flags), the 0xEC counter, and the stride-64 search over the table at
// game_uso_D_807FFBC0 (count = *(int*)&D_00000000), tail 0xE8 &= 0x6.
//
// Fix vs prior 78.2% NM body: search table base is game_uso_D_807FFBC0
// (prior used &D_00000000+0x15D0 — wrong symbol). Switch stays the
// higher-scoring jumptable form (cascade regresses — documented cap), and
// the A58 call stays CONDITIONAL: the target's unconditional-call shape is
// semantically truer but churns regalloc and scores ~0.1pp WORSE in-tree,
// so the marginal table-base fix is the only net-positive lever here.
// 2026-10-17 agent-g (run 106) 76.9 -> 91.4: the "switch" is NOT on 0xE8 --
// the target's compare chain 0,5,6,2,1,4,3 runs on a3->0x12C itself (case 0
// = skip), unsorted, so it is the kit-VI nested-!= goto chain; ending EVERY
// goto body with `if (1) goto cdone;` makes uopt lay the bodies out in text
// order (with plain `goto cdone` they come out innermost-first). The EAF4
// call IS unconditional with arg1 = `g && (v0 & 0x40)`, the 0xB4 sub pointer
// is an unnamed temp, the 9A8 test is NEGATED (old body had the logic
// inverted), the count is re-read in the loop condition (gives the target's
// sltu bottom test instead of a collapsed bne), and the `&= 6` / `>= 21`
// tail sits INSIDE the 0x214 bound check with the `&= 6` unconditional.
// RESIDUAL: the table-base lui is hoisted to the entry here, 0xF4->0x2C
// re-reads after each 0xE8 store are CSE'd (target reloads 0xF4), and the
// found-store addu takes the shifted index first.
void RiderAnim_readTrickInput_0000E5C8(char *a0, int a1) {
    char *a3 = a0;
    unsigned int src;
    char *sub;
    unsigned int v0;

    /* src = a0->0xB4->0x800->(0x18 if a0->0xF4 else 0x10) */
    if (RA(a0)->curTrick != 0) {
        src = RA(a0)->rider->input->pressed;
    } else {
        src = RA(a0)->rider->input->held;
    }
    /* unrolled mask->outbit permutation into a3->0xE8 (read-modify-write) */
    if (src & 0x200)   RA(a3)->trickButtons |= 0x2;
    if (src & 0x1000)  RA(a3)->trickButtons |= 0x4;
    if (src & 0x400)   RA(a3)->trickButtons |= 0x1;
    if (src & 0x10000) RA(a3)->trickButtons |= 0x10;
    if (src & 0x4000)  RA(a3)->trickButtons |= 0x20;
    if (src & 0x2000)  RA(a3)->trickButtons |= 0x40;
    if (src & 0x8000)  RA(a3)->trickButtons |= 0x80;

    v0 = RA(a3)->rider->flags;
    {
        unsigned int g = v0 & 0x20;
        RiderAnim_readStickSpin_0000EAF4(a3, g && (v0 & 0x40), g);
    }

    {
        int k = RA(a3)->spinKind;
        if (k != 0) {
            if (k != 5) {
                if (k != 6) {
                    if (k != 2) {
                        if (k != 1) {
                            if (k != 4) {
                                if (k != 3) {
                                } else goto c3;
                            } else goto c4;
                        } else goto c1;
                    } else goto c2;
                } else goto c6;
            } else goto c5;
        }
        goto cdone;
c5:     RA(a3)->trickButtons |= 0x20000; if (1) goto cdone;
c6:     RA(a3)->trickButtons |= 0x10000; if (1) goto cdone;
c2:     RA(a3)->trickButtons |= 0x4000;  if (1) goto cdone;
c1:     RA(a3)->trickButtons |= 0x1000;  if (1) goto cdone;
c4:     RA(a3)->trickButtons |= 0x8000;  if (1) goto cdone;
c3:     RA(a3)->trickButtons |= 0x2000;
cdone:  ;
    }

    /* bit-clearing, gated by 0xE8 bit 0x4 and a3->0xF4->0x2C flags */
    {
        unsigned int e8 = RA(a3)->trickButtons;
        if (e8 & 0x4) {
            char *f4 = (char *)RA(a3)->curTrick; if (1) {}
            *(unsigned int *)(a3 + 0xE8) = e8 & 0x3F007;
            if (f4 != 0) {
                unsigned int m = ((TrickEntry *)f4)->inputMask;
                if (m & 0x5000) {
                    *(unsigned int *)(a3 + 0xE8) &= 0xFFFF5FFF;
                    m = RA(a3)->curTrick->inputMask;
                }
                if (m & 0xA000) {
                    *(unsigned int *)(a3 + 0xE8) &= 0xFFFFAFFF;
                    m = RA(a3)->curTrick->inputMask;
                }
                if (m & 0xC000) {
                    if (m & 0x100000) {
                        *(unsigned int *)(a3 + 0xE8) |= 0x200000;
                    } else if (!(m & 0x200000)) {
                        *(unsigned int *)(a3 + 0xE8) |= 0x100000;
                    }
                }
            }
        }
    }

    /* counter advance: 0xEC++ while 0xE8 nonzero, else reset */
    if (RA(a3)->trickButtons != 0) {
        *(int *)(a3 + 0xEC) += 1;
    } else {
        *(int *)(a3 + 0xEC) = 0;
    }

    /* stride-64 search over the table when 0xEC exceeds the 0x214 bound */
    {
        unsigned int t0 = RA(a3)->trickInputFrames;
        if (t0 > (unsigned int)RA(a3)->parms.tkt.value.i) {
            unsigned int v1;
            char *base;
            char *obj;
            char *e;
            for (v1 = 0, base = *(char **)&game_uso_D_807FFBC0, obj = (char *)RA(a3)->rider, e = base; v1 < *(unsigned int *)&D_00000000; v1++, e += 64) {
                if ((((TrickEntry *)e)->riderMask & ((SnowBoarder *)obj)->unk8C0) == 0) continue;
                if (((TrickEntry *)e)->inputMask != RA(a3)->trickButtons) continue;
                if ((((TrickEntry *)e)->flags & TRICK_FLAG_GROUND) && !(((SnowBoarder *)obj)->groundFlags & 0x1)) continue;
                if (((SnowBoarder *)obj)->contactCount != 0) continue;
                *(char **)(a3 + 0xF0) = base + v1 * 64;
                *(int *)(a3 + 0xE8) = 0;
                *(int *)(a3 + 0x12C) = 0;
                *(int *)(a3 + 0xEC) = 0;
                t0 = 0;
                break;
            }
            *(int *)(a3 + 0xE8) &= 0x6;
            if (t0 >= 21) {
                *(int *)(a3 + 0xEC) = 0;
                *(int *)(a3 + 0xE8) = 0;
            }
        }
    }
    (void)a1;
}
#else
INCLUDE_ASM("asm/nonmatchings/game_uso/game_uso", RiderAnim_readTrickInput_0000E5C8);
#endif

/* RiderAnim_checkGroundSpin_0000E91C — EE84-family branchy orchestrator, 118 insns.
 * EXACT (was NM 98.98%, 90.31% before that).
 *
 * Reads o=a0->0xB4, v0=o->0xA58, clears a0->0xB0, then
 * RiderAnim_readStickSpin_0000EAF4(a0, cond, v0&0x20) where cond=((v0&0x20)!=0) then
 * (if set) =(v0&0x40)!=0 — an if-without-else, NOT a ternary-with-:0 (the
 * :0 adds a spurious `move ai,zero`+`b`). When (*(a0->0xB4)->0x800)->0x10 &
 * 0x1000, dispatches on a0->0x12C. The dispatch is TESTS-FIRST in source
 * order 2,1,4,3 with bodies laid out likewise — a C `switch` SORTS the test
 * chain (1,2,3,4) and an if/else-if INTERLEAVES bodies, so neither matches;
 * it must be a goto chain (4 `if(sw==k) goto Lk`, last as `if(sw!=3) goto
 * done; goto L3` to get the beql-toward-L3 + b-default sense). Each arm ORs a
 * literal into a0->0xB0 (from a0->0xFC), homes a Pair2 from D_807FF430+0xE40
 * or D_807FF4E0+0xEF0 into the shared sp+0x38 slot, calls an import with
 * a0=a0->0xB4+0x808. Tail (a0->0xB0!=0): zero a0->0x12C, import_0010DB28,
 * RiderAnim_queueState_0000D5F8(a0, homed, 1), RiderAnim_clearState_0000D5DC(a0).
 *
 * 2026-10-15 (agent-g, 98.98 -> EXACT 118/118):
 *   (1) the A58 word is NOT a named local: inlined at all three uses (CSE'd
 *       to v0, mask v1), so the 0xB4 deref is the t6 expression temp;
 *   (2) the third EAF4 arg is `(unsigned)(word & 0x20)` -- the no-op cast makes
 *       it a distinct value from the `!= 0` test's CSE temp, which is what
 *       emits the target's `or a2,v1` copy in the jal delay (same idiom at the
 *       E5C8 EAF4 call site);
 *   (3) `cond` is REUSED for the 0x800 flag pointer and the 0x12C switch word:
 *       same-variable webs pull a1 (docs #same-name-web-reuse-plus-dead-if-param-332b4);
 *   (4) frame 0x48 with homed at sp+0x38: two unused ints, one above cond and
 *       one below homed (`int v0` removal had cost a word).
 */
extern int import_00096724();
extern int import_00096704();
extern int import_0010DB28();
extern char game_uso_D_807FF430;
extern char game_uso_D_807FF4E0;

void RiderAnim_checkGroundSpin_0000E91C(char *a0) {
    int pad;
    int cond;
    Pair2 homed;
    int pad2;

    RA(a0)->spinAnim = 0;
    /* rider->flags is u32; the (s32) keeps the target's signed AND web (an unsigned one is a different CSE
     * expression from the (unsigned) argument below and moves the andi / sltu). */
    cond = (((s32)RA(a0)->rider->flags & 0x20) != 0);
    if (cond) {
        cond = (((s32)RA(a0)->rider->flags & 0x40) != 0);
    }
    RiderAnim_readStickSpin_0000EAF4(a0, cond, (unsigned)((s32)RA(a0)->rider->flags & 0x20));

    cond = (int)RA(a0)->rider->input;
    if (((RiderInput *)cond)->held & 0x1000) {
        cond = RA(a0)->spinKind;
        if (cond == 2) goto L2;
        if (cond == 1) goto L1;
        if (cond == 4) goto L4;
        if (cond != 3) goto done_switch;
        goto L3;
    L2:
        RA(a0)->spinAnim = RA(a0)->animSet | RANIM_TO_FAKIE_360F;
        homed = *(Pair2 *)(&game_uso_D_807FF430 + kPmf_RiderAnim_stateRide);
        import_00096724(RA(a0)->rider->sound);
        goto done_switch;
    L1:
        RA(a0)->spinAnim = RA(a0)->animSet | RANIM_TO_FAKIE_180F;
        homed = *(Pair2 *)(&game_uso_D_807FF4E0 + kPmf_RiderAnim_stateFlipToRide);
        import_00096704(RA(a0)->rider->sound);
        goto done_switch;
    L4:
        RA(a0)->spinAnim = RA(a0)->animSet | RANIM_TO_FAKIE_360B;
        homed = *(Pair2 *)(&game_uso_D_807FF430 + kPmf_RiderAnim_stateRide);
        import_00096724(RA(a0)->rider->sound);
        goto done_switch;
    L3:
        RA(a0)->spinAnim = RA(a0)->animSet | RANIM_TO_FAKIE_180B;
        homed = *(Pair2 *)(&game_uso_D_807FF4E0 + kPmf_RiderAnim_stateFlipToRide);
        import_00096704(RA(a0)->rider->sound);
    done_switch:;
    }

    if (RA(a0)->spinAnim != 0) {
        RA(a0)->spinKind = 0;
        import_0010DB28(a0, RA(a0)->spinAnim, 0, 1, 1, 1);
        RiderAnim_queueState_0000D5F8(a0, homed, 1);
        RiderAnim_clearState_0000D5DC(a0);
    }
}

// RiderAnim_readStickSpin_0000EAF4 — STRUCTURAL PASS (0x1F8 / 126 words,
// no episode). Raw-.word USO form (single function, game_uso main
// game-logic). Pure integer/bitfield (no FP, no calls). Leaf-ish:
// no sp frame — spills args a1/a2 to the caller's shadow slots
// (sw a1,4(sp) / sw a2,8(sp)); this is a small function, NOT a
// fragment (it has its own jr ra at 0xECE0).
//
// Steering/lean input integrator with rate-clamping.
//
//   void RiderAnim_readStickSpin_0000EAF4(Obj *obj, int tgt, int a2) {
//     Sub *s = obj->0xB4;
//     if (s->0x800->0x7C(s16) == 0) { ... }
//     int cur = (s16)obj->0x124;
//     int d = tgt - cur;
//     // clamp the per-frame step: d = clamp(d, -7, +7) normally,
//     //   tightened to [-2,+2] / [-3,+3] when a flag selects slow
//     //   mode (slti vs 8 / -7 / 3 / -2 chains);
//     obj->0x124 = (s16)(cur + d);                          // step toward tgt
//     obj->0x128 = 0;  obj->0x12C = 0;                       // reset deriv
//     // if (s->0x800->0x18 & 0x4000) … recompute obj->0x12C from
//     //   flags (an alternate / boosted steering response);
//     // obj->0x12C = …;  return obj->0x12C-ish;
//   }
//
// Struct-typing reference:
//   obj: 0xB4 -> s (s->0x800->0x7C s16 enable, s->0x800->0x18 flags
//     bit 0x4000), 0x124 s16 current steering value (rate-limited
//     toward tgt), 0x128 s16 / 0x12C int steering derivative/state
//     (reset then recomputed). tgt/a2 = target + mode args. Clamp
//     bounds ±7 (fast) / ±2 / ±3 (slow). Snowboard steering/lean
//     input smoothing.
// FULLY RE-DECODED 2026-05-31 (20.9% -> 70.86%): complete logic-exact decode.
//   Returns int (=v1). Steering target = ctx->0x7C; d = (cur!=0)?tgt-cur:0;
//   REFLECTION clamp (d>=8 -> 7-d; d<-7 -> -7-d; keep only [-2,2] else 0);
//   obj->0x126 first-frame init (=1, zero 0x128/0x12C); when d==0 a ctx->0x18
//   flag dispatch sets d=-1/+1 (0x10000/0x4000) or obj->0x12C=5/6 (0x2000/0x8000);
//   obj->0x128 += d; obj->0x124 = tgt. Then: counter saturates at >=6 (sets v0);
//   if v0 (enable-zero OR saturated): clear 0x126, param-gated negations of 0x128
//   (a1 && 0xB4->0xA58&0x80; a2), then obj->0x128 -> obj->0x12C dispatch
//   (>=3:4, >0:3, <-2:2, <0:1, 0:0) returned as v1.
// 2026-06-21 (agent-e): DECODE TIGHTENED 70.86 -> 71.48 (476 -> 496 bytes,
//   reloc-set MATCH, now only 2 insns short of 504). Re-derived ctx fresh
//   inside the d==0 block (lw 0xB4(a0); lw 0x800; matches target eb84/eb88)
//   and re-read *(ctx+0x18) per flag-test instead of caching `fl`. RESIDUAL
//   (genuine cap): the target's d==0 flag dispatch and the tail param-gated
//   negations emit BRANCH-LIKELY (beql/bgezl) with a redundant reload
//   (lw 0x18(a2) / lw 8(sp)) annulled into each branch delay slot — an IDO
//   -O2 reorg-annul scheduling choice not steerable from C (per-test re-read
//   yields plain beq here, not beql). Same reorg-annul class as func_0000174C.
/* RIDER_V: obj->rider read through a volatile lvalue: the per-use reloads are the CSE/PRE breakers of
 * lever (4) below (the target reloads rider on every path). */
#define RIDER_V(o) (*(SnowBoarder * volatile *)&RA(o)->rider)
int RiderAnim_readStickSpin_0000EAF4(char *obj, int a1, int a2) {
    int flag;
    int result;
    int d;
    int cur;
    int tgt;

    result = 0;
    flag = 0;
    tgt = RIDER_V(obj)->input->stickDir;
    if (tgt != 0) {
        d = 0;
        cur = RA(obj)->lastStickDir;
        if (cur != 0) {
            d = tgt - cur;
        }
        if (d >= 8) d = 7 - d;
        if (d < -7) d = -7 - d;
        if (d >= 3 || d < -2) d = 0;
        if (RA(obj)->spinFrames == 0) {
            RA(obj)->spinFrames = 1;
            RA(obj)->spinSteps = 0;
            RA(obj)->spinKind = 0;
        }
        if (d == 0) {
            cur = (int)RIDER_V(obj)->input;
            if (((volatile RiderInput *)cur)->pressed & 0x10000) {
                d = -1;
            } else if (((volatile RiderInput *)cur)->pressed & 0x4000) {
                d = 1;
            } else if (((volatile RiderInput *)cur)->pressed & 0x2000) {
                RA(obj)->spinKind = 5;
            } else if (((volatile RiderInput *)cur)->pressed & 0x8000) {
                RA(obj)->spinKind = 6;
            }
        }
        RA(obj)->spinSteps = RA(obj)->spinSteps + d;
        tgt = RIDER_V(obj)->input->stickDir;
    } else {
        flag = 1;
    }
    d = RA(obj)->spinFrames;
    RA(obj)->lastStickDir = tgt;
    if (d != 0) {
        RA(obj)->spinFrames = (short)d + 1;
        if ((short)d >= 6) {
            flag = 1;
        }
    }
    if (flag != 0) {
        if (RA(obj)->spinFrames != 0) {
            RA(obj)->spinFrames = 0;
            if (a1 != 0) {
                if (RIDER_V(obj)->flags & 0x80) {
                    RA(obj)->spinSteps = -RA(obj)->spinSteps;
                }
            }
            if (a2 != 0) {
                RA(obj)->spinSteps = -RA(obj)->spinSteps;
            }
            flag = RA(obj)->spinSteps;
            if (flag >= 3) {
                result = 4;
            } else if (flag > 0) {
                result = 3;
            } else if (flag < -2) {
                result = 2;
            } else if (flag < 0) {
                result = 1;
            }
            RA(obj)->spinKind = result;
        }
    }
    return result;
}

/* RiderAnim_readStickSpin_0000EAF4 — 125/125 EXACT (verified clean rebuild of
 * build/non_matching/src/game_uso/game_uso.c.o word-by-word vs target .s;
 * leaf, no relocs). The .s's 126th word (0x1F4: lw v0,0x10C(a0)) is NOT part
 * of this function — it is the mis-split FIRST INSTRUCTION of
 * RiderAnim_updateSlowdown_0000ECEC (it loads the v0 that ECEC's first beqz tests;
 * EAF4 ends at jr ra + delay = 0x1F0). Landing needs the splat boundary fix:
 * move that word from RiderAnim_readStickSpin_0000EAF4.s to the head of
 * RiderAnim_updateSlowdown_0000ECEC.s (byte-neutral for the ROM).
 *
 * 2026-07-03 (agent-e). From 73.1%. Decisive levers:
 *  (1) LOGIC FIX: the 0x126 counter increments EVERY call when nonzero
 *      (sh in the plain-bnez delay); v0-flag only when (short)cnt >= 6. Old
 *      wrap gated the store on cnt>=6.
 *  (2) E2D0-style split sign-extension: `d = *(short*)(obj+0x126); ...
 *      *(obj+0x126) = (short)d + 1; if ((short)d >= 6)` — raw lh test +
 *      sll/sra re-extension.
 *  (3) SHARED RETURN: nested ifs (no duplicated `return` epilogues).
 *  (4) THE BIG ONE — volatile CSE/PRE breakers: all four obj->0xB4 loads as
 *      `*(char * volatile *)(obj + 0xB4)` and the flag-word reads as
 *      `*(volatile unsigned int *)(cur + 0x18)`. Without volatile, uopt PRE
 *      unifies the cross-path B4 loads into ONE long-lived candidate (t0) —
 *      dropping a reload AND phase-shifting the whole temp pool (t0,t6,t7,...
 *      vs target t6,t7,t8,...); the else-if flag reads CSE into one andi
 *      chain (-6 words). Volatile restores per-use loads incl. the
 *      beqzl-annulled delay-slot reload duplicates. (Named fresh per-level
 *      reload vars, dead kills, goto forms: all VN'd away — only volatile
 *      breaks availability.)
 *  (5) Variable economy mirrors target coloring: flag=v0, result=v1, d=a1
 *      (reused as cnt), cur=a2 (reused as the d==0 ctx pointer AND implicitly
 *      sval via flag reuse), tgt=a3. Init order `result=0; flag=0;` BEFORE
 *      the tgt chain (emission: move v1 then move v0).
 */

// RiderAnim_updateSlowdown_0000ECEC — EXACT 57/57 (2026-07-08 agent-e; 0xE4 incl.
// the boundary-fix head word lw v0,0x10C(a0) from commit 4d76f242).
// All 55 non-reloc words byte-equal; the 2 reloc words (lui $at %hi +
// lw t6 %lo jump-table) carry R_MIPS_HI16/LO16 .rodata — resolved at
// link (USO baked-reloc class; make verify is the landing gate).
// Jump-table dispatched field selector over idx = obj->0x10C (1..5).
//
// Decisive levers from 93.32% (9-diff v0/v1 + t1/t2 coloring swap):
//  (1) WRONG-SIGNATURE TELL: idx is NOT a param — it is loaded from
//      obj->0x10C (the mis-split head word). One-arg signature.
//  (2) COMMUTED SUM `*base = ab[0] + *base` flips the t1/t2 pair
//      (b reload -> t2, *base -> t1, matching target).
//  (3) v0/v1 WEB FLIP: define base FIRST from the full expression
//      (base = *(obj+0xB4)+0x960) and v SECOND from the CSE'd load —
//      pseudo-creation order flips coloring to v=v1/base=v0. The thr
//      compare must then read via v (*(int*)(v+0x960)), NOT *base,
//      or VN unifies thr with the else-arm *base loads (-2 words,
//      kills the beqzl annulled dup-load pair).
// Negative results: if(1) barrier around the pointer setup and
// base-first-with-*base-thr both collapse the dup loads (20 diffs);
// decl-order alone does nothing.
/* Case decode: case 3 b-source = 0x3F4; case 4 b-source = 0x40C. Common
 * tail: v = a0->0xB4; if (a < v->0x960 && v->0x938) { a0->0x10C = 0;
 * a0->0x110 = 1; } else { v->0x960 += b; a0->0x110 = 0; } */
void RiderAnim_updateSlowdown_0000ECEC(char *obj) {
    int ab[2];
    SnowBoarder *v;
    int *base;
    int idx = RA(obj)->hitKind;
    if (idx == 0) return;
    switch (idx - 1) {
        case 0: ab[1] = RA(obj)->parms.stm.value.i; ab[0] = RA(obj)->parms.st1.value.i; break;
        case 1: ab[1] = RA(obj)->parms.stm.value.i; ab[0] = RA(obj)->parms.st2.value.i; break;
        case 2: ab[1] = RA(obj)->parms.stm.value.i; ab[0] = RA(obj)->parms.st3.value.i; break;
        case 3: ab[1] = RA(obj)->parms.slm.value.i; ab[0] = RA(obj)->parms.sl1.value.i; break;
        case 4: ab[1] = RA(obj)->parms.slm.value.i; ab[0] = RA(obj)->parms.sl2.value.i; break;
    }
    /* base FIRST (full expr), v SECOND (CSE'd) — flips v0/v1 coloring
     * (2026-07-08); thr must read via v, not *base (VN unification). */
    base = &RA(obj)->rider->slowdown;
    v = RA(obj)->rider;
    if ((ab[1] < v->slowdown) && (v->contactCount != 0)) {
        RA(obj)->hitKind = 0;
        RA(obj)->slowCrash = 1;
    } else {
        *base = ab[0] + *base;
        RA(obj)->slowCrash = 0;
    }
}

void RiderAnim_setHitKind_0000EDCC(int *a0, int a1) {
    RA(a0)->hitKind = a1;
}

/* RiderAnim_applyDamage_0000EDD4: 23-insn 5-way switch + back-subtract.
 *
 * Structure:
 *   if (a1 < 5) v0 = pickByA1[a1] from {a0[0x34C], a0[0x364], a0[0x37C],
 *                                       a0[0x394], 0};
 *   else v0 = 0;
 *   p = a0->0xB4;
 *   p->0xA14 -= v0;
 *
 * Labels from the ROM table (port/tools/jumptable-audit.py, run 831): a1 = 0 is the
 * empty arm, 1..4 pick dm1..dm4 (the callers pass 1 and 2).
 *
 * Trigger jumptable emit: 5 explicit cases (case 0 is empty break, NOT
 * default) + `int v0 = 0;` pre-init. IDO -O2 then uses sltiu+jumptable
 * via D_base+0x224. WITHOUT case 4 explicit, IDO falls back to cascading
 * else-if chain (20% match). WITHOUT v0=0 pre-init, IDO emits redundant
 * `or v0, zero, zero` in the default arm before/after the jump (70.95%).
 *
 * The explicit char-pointer add keeps IDO from folding the final
 * `addiu v1, v1, 0xA14` into the store.
 *
 * a2 is an unused 3rd arg saved to caller arg-slot at sp+0x8 by IDO arg-
 * save (per docs/IDO_CODEGEN.md#feedback-ido-unused-arg-save). */
void RiderAnim_applyDamage_0000EDD4(int *a0, unsigned int a1, int a2) {
    int v0 = 0;
    int *p;
    (void)a2;
    switch (a1) {
        case 1: v0 = RA(a0)->parms.dm1.value.i; break;
        case 2: v0 = RA(a0)->parms.dm2.value.i; break;
        case 3: v0 = RA(a0)->parms.dm3.value.i; break;
        case 4: v0 = RA(a0)->parms.dm4.value.i; break;
        case 0: break;
    }
    p = &RA(a0)->rider->health;
    *p = *p - v0;
}

void RiderAnim_addTrickPoints_0000EE30(char *a0, int a1, int a2) {
    int *dptr = (int*)((char*)&D_00000000 + 0x138);
    CALL(RaceTracker_addTrickPoints)(*dptr, (int)RA(a0)->rider, a1 * a2);
}

void RiderAnim_stateStopped(void *a0) {
    RA(a0)->rider->slowdown = 100;
}

/* RiderAnim_stateSwitchStance: 39-insn init/reset.
 *   p = a0->0xB4; *(float*)(p+0xA74)=0; *(float*)(p+0xA70)=0;
 *   if (a0->0xB4->0x9CC != 0) a0->0xFC |= 0x24; else a0->0xFC |= 0x1D;
 *   X(a0, a0->0xFC, 0, 1, 1, 1);
 *   X(a0, D[0xE98], D[0xE9C], 1, ...); X(a0);
 * beql: ==0 case (|=0x1D) load in delay slot. USO: call->func_00000000,
 * data->&D_00000000+off. */
extern char game_uso_D_807FF488;
void RiderAnim_stateSwitchStance(int *a0) {
    int a1;
    RA(a0)->rider->unkA74 = 0.0f;
    RA(a0)->rider->airSpin = 0.0f;
    if (RA(a0)->rider->useDParms != 0) {
        a1 = RA(a0)->animSet | RANIM_DEEP_SWITCH;
    } else {
        a1 = RA(a0)->animSet | RANIM_SWITCH_STANCE;
    }
    XCALL(game_uso_func_077C44, AnimPlayer_start)(a0, a1, 0, 1, 1, 1);
    RiderAnim_queueState_0000D5F8((char *)a0, *(Pair2 *)((char *)&game_uso_D_807FF488 + kPmf_RiderAnim_stateSwitchStanceEnd), 1);
    RiderAnim_clearState_0000D5DC((char *)a0);
}

/* MATCHED 2026-05-28 via struct-by-value (E40/E44 pair homes a1,a2). See
 * docs/IDO_CODEGEN.md#feedback-ido-struct-by-value-homes-arg-pair. */
void RiderAnim_stateFlipToRide(int a0) {
    CALL(RiderAnim_flipStance_0000D6E4)(a0);
    CALL(RiderAnim_enterState_0000D5BC)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateRide));
    CALL(RiderAnim_clearRiderEdge_00011868)(a0);
    CALL(RiderAnim_stateRide)(a0);
}

/* RiderAnim_stateSwitchStanceEnd — verified decode (EE84-family branchy, reload-CSE cap).
 * void f(int *a0){
 *   int *s0 = a0;
 *   func_00000000(a0);                              // X1
 *   func_00000000(s0[0xB4/4], 0x3f800000);          // X2 (a1 = 1.0f bits, lui 0x3f80)
 *   func_00000000(s0);                              // X3
 *   if (((int*)s0[0xB4/4])[0x938/4] == 0) {
 *     func_00000000(s0);                            // X4
 *     func_00000000(s0, D[0xDF8], D[0xDFC]);        // X5
 *   } else {
 *     func_00000000(s0, 1);                         // X6
 *     if (((int*)s0[0xB4/4])[0x9CC/4] != 0) {
 *       func_00000000(s0, D[0xE60], D[0xE64]);      // X7
 *       func_00000000(s0);                          // X8
 *     } else {
 *       func_00000000(s0, D[0xE40], D[0xE44]);      // X9
 *       func_00000000(s0);                          // X10
 *     }
 *   }
 * }
 * Structure verified vs .s. MATCHED 2026-05-31 (74.65% -> 100%): the three
 * D-pair calls (DF8/DFC, E60/E64, E40/E44) pass the float-pair BY VALUE as
 * *(Pair2*) — the "reload-CSE / sub-80 cap" was the un-homed pair all along
 * (same lever as 10AC8/F284, docs/IDO_CODEGEN.md#feedback-ido-struct-by-value-homes-arg-pair). */
extern int gl_func_00000000();
void RiderAnim_stateSwitchStanceEnd(int *a0) {
    int *s0 = a0;
    CALL(RiderAnim_flipStance_0000D6E4)(a0);
    CALL(SnowBoarder_yawPiOver_0000E270)(RA(s0)->rider, 0x3F800000);   /* a1 = 1.0f bits */
    CALL(RiderAnim_clearRiderEdge_00011868)(s0);
    if (RA(s0)->rider->contactCount == 0) {
        CALL(RiderAnim_stateRide)(s0);
        CALL(RiderAnim_enterState_0000D5BC)(s0, *(Pair2 *)((char *)&D_00000000 + kPmf_RiderAnim_stateHopLand));
    } else {
        CALL(RiderAnim_endTrick_0000D63C)(s0, 1);
        if (RA(s0)->rider->useDParms != 0) {
            CALL(RiderAnim_enterState_0000D5BC)(s0, *(Pair2 *)((char *)&D_00000000 + kPmf_RiderAnim_stateRideDeep));
            CALL(RiderAnim_stateRideDeep)(s0);
        } else {
            CALL(RiderAnim_enterState_0000D5BC)(s0, *(Pair2 *)((char *)&D_00000000 + kPmf_RiderAnim_stateRide));
            CALL(RiderAnim_stateRide)(s0);
        }
    }
}

/* RiderAnim_statePreJump (0xDC / 55 words): flag-dispatch state handler.
 * self->0xF4 = 0; 11888(self); if the 0xB4 object's 0x800 child has
 * (!(flags & 0x100) || (flags & 0x200)) and the 0xB4 object's 0x938 is set,
 * F360(self) and return; else DB28(self, 0x10011, 0, 0, 0x100, self->0x16C),
 * D5F8(self, PMF at D_807FF4A0+0xEB0, -1), self->0xE6 = 0, ++self->0xE4,
 * 966AC(self->0xB4 + 0x808).
 * EXACT via REPLACE_FUNC_BODY donor splice: the real body is the IDO 7.1 NCC
 * (C++) donor src/game_uso/game_uso_ncc_F060.cpp (55/55 words + 7/7 relocs).
 * The cfe C capped at 99.64% because `v = self->0xB4` coloured v0 where the
 * target has v1; reading the 0x800 child through an edg-inlined member
 * (`self->0xB4->child()`) holds the inline return temp in v0 and pushes the
 * 0xB4 pointer to v1, which is what the original C++ did. See
 * docs/MATCHING_WORKFLOW.md#game-uso-ncc-cplusplus-donor-recipe.
 * Body below is a placeholder for the splice. */
extern int import_0010DB28();
extern int import_000966AC();
void RiderAnim_jumpTakeoff_0000F360(int *a0);
void RiderAnim_clearTricks_00011888(int *a0);
extern char game_uso_D_807FF4A0;
void RiderAnim_statePreJump(int *a0) {
    volatile int *ret = a0;
}

/* RiderAnim_statePreJumpHold (0x144 / 81 words): EE84-family state handler, same
 * flag block as F060. If the 0xB4 object's 0x800 child has
 * (!(flags & 0x100) || (flags & 0x200)) and the 0xB4 object's 0x938 is set,
 * F360(self); else pick DB78(self, 0x10028, 0x10029, f, 0, 1) when
 * |0xB4->0x970| exceeds the D_807FFB58+0x238 double (else DB28(self,
 * 0x10011, 0, 1, 6, 1)) and run 1001C(self) when 0xB4->0x938 is clear. Then
 * ++self->0xE6 and D5BC(self, PMF at D_807FF498+0xEA8) once the old value
 * passes self->0x184.
 * EXACT via REPLACE_FUNC_BODY donor splice: the real body is the IDO 7.1 NCC
 * (C++) donor src/game_uso/game_uso_ncc_F13C.cpp (81/81 words + 9/9 relocs).
 * The cfe C capped at 99.57% on the same v0-vs-v1 colouring as F060; the
 * inlined `self->0xB4->child()` member fixes both. The float 4th argument of
 * DB78 comes from the prototype (no alias needed in the donor). See
 * docs/IDO_CODEGEN.md#ncc-inline-member-child-v1-f060 and
 * docs/MATCHING_WORKFLOW.md#game-uso-ncc-cplusplus-donor-recipe.
 * Body below is a placeholder for the splice. */
extern void RiderAnim_startHop_0001001C(int *);
extern void RiderAnim_enterState_0000D5BC(char *, Pair2);
extern char game_uso_D_807FFB58;
extern char game_uso_D_807FF498;
void RiderAnim_statePreJumpHold(int *a0, int *a1) {
    /* placeholder; also emits the donor's double 0.06 (RoData 0x238) */
    *(volatile double *)a0 = 0.06;
}

/* RiderAnim_statePreJumpCancel: 55-insn EE84-family branchy, beql-selected mode+D-pair.
 * The two D-pair calls pass the float-pair BY VALUE as *(Pair2*) (struct-by-value
 * homes the arg pair, docs/IDO_CODEGEN.md#feedback-ido-struct-by-value-homes-arg-pair).
 * The final spill-slot residual (`m` wanted sp+44 in a 0x30 frame, build emitted
 * sp+40) was cracked by REMOVING the idiomatic `int *s0 = a0;` alias and using a0
 * directly: dropping the redundant local shifts the pseudo-numbering so `m`'s spill
 * bumps to the higher slot (docs/IDO_CODEGEN.md remove-idiomatic-local lever —
 * invalidates the "spill-slot placement not C-controllable" cap). BYTE-EXACT. */
extern int gl_func_00000000();
void RiderAnim_statePreJumpCancel(int *a0) {
    int m;
    CALL(SndHandle_stop)(RA(a0)->rider->sound);
    if (RA(a0)->rider->useDParms != 0) {
        m = (RANIM_GROUP_DEEP | RANIM_UP_DOWN);
        CALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2 *)((char *)&D_00000000 + kPmf_RiderAnim_stateRideDeep), -1);
    } else {
        m = RA(a0)->animSet | RANIM_UP_DOWN;
        CALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2 *)((char *)&D_00000000 + kPmf_RiderAnim_stateRide), -1);
    }
    RA(a0)->jumpRepress = 1;
    CALL(RiderAnim_clearRiderEdge_00011868)(a0);
    CALL(AnimPlayer_startIfChanged)(a0, m, 0, 0, 0x100, RA(a0)->parms.jot.value.i);
}

/* RiderAnim_jumpTakeoff_0000F360: 49-insn float-compare-gated state-mux update. EXACT.
 *
 * 2026-07-03 (game_uso W4, agent-e, 89.67% -> 47/49 = 95.9%): applied the W4
 * lever trio in-tree. Now byte-exact EVERYWHERE except 2 words (see RESIDUAL).
 *  (1) if(1)-wrapped pointer mutation (`b=(char*)p; if(1){b+=K;}`) materializes
 *      the per-block `addiu rd,v1,K` base regs (v1+0x770, +0x510, +0x788, +0x31C)
 *      + small (+0x10/+0x0) offset loads exactly (was the plain-fold 89.67% shape).
 *      This also flips the int/pointer coloring to target (int=$v0, p=$v1) and
 *      makes the beqzl-annulled dead `lwc1 f16` dup fall out naturally off b788.
 *  (2) `f0 *= *(b510+0x10)` (compound assign) gives the target's `mul.s f0,f0,f8`
 *      operand order (reusing f0); the split `f0 = f0 * ...` form emits f8,f0.
 *  Arms: if (p->0x9CC != 0) f0 = *(p+0x798)+f2; else f0 = *(p+0x780)+f2 (beqzl-taken).
 *  double const at game_uso_D_807FFB60 + 0x240 (ldc1 576(at)); the USO unit is
 *  HI16-only so the extra C-side LO16 reloc is byte-harmless (docs IDO_CODEGEN L23).
 *
 * 2026-07-15 (wave 3, agent-h, 47/49 -> 49/49 = objdiff-100): the old "FP
 * candidate-numbering coloring cap" (ldc1/cvt.d.s f6<->f18 temp-binding swap
 * at words f3d0/f3d4) RETRACTED via the cfe commutative-operand RANK lever
 * (docs/IDO_CODEGEN 199C entry): a TYPED extern-struct MEMBER load
 * (game_uso_D_807FFB60.v, typed directly; base-0 in undefined_syms_auto.txt,
 * struct pad puts .v at +0x240 so relocs/bytes are identical) restores
 * TEXTUAL operand order, so writing the double const FIRST
 * (`D.v * (double)f0`) creates its ldc1 pseudo before the cvt.d.s -> ldc1
 * f6 / cvt f18 as in target. Cast-deref spellings of D always ranked below
 * (double)f0 regardless of textual order (register-rooted deref reuse,
 * int-cast address form, named double all probed: 47/49 or worse; named
 * double recolors the whole fn f0->f2, 37/49).
 *
 * 2026-10-16 (agent-g): LANDED as compiled C. The old "not landable"
 * note (in-unit jal to 1001C would link to the real symbol) does not apply:
 * the Yay0 block is cut from the UNLINKED game_uso.c.o, so every in-unit
 * jal stays `jal 0` exactly like the ROM (verify-blocks clean, no block
 * diff in F360). The struct now types game_uso_D_807FFB60 itself (the only
 * user in this TU), so the _thr alias is no longer referenced.
 *
 * 2026-10-16 (agent-g): the const is now the plain literal 0.7 -- with the
 * unit's .rodata relaid out (INCLUDE_ASM literals emitted as .late_rodata)
 * IDO's own local double lands at RoData +0x240 exactly, and the literal
 * keeps the textual operand rank (ldc1 before cvt.d.s), so the extern struct
 * is no longer needed. docs/MATCHING_WORKFLOW.md#game-uso-rodata-relayout-late-rodata. */
extern void import_00096874(int);
extern void RiderAnim_startHop_0001001C(int *);
void RiderAnim_jumpTakeoff_0000F360(int *a0) {
    int v0 = RA(a0)->jumpCharge;
    int *p;
    char *b770, *b510, *b788, *b31C;
    float f2, f0;
    if (v0 < 0) {
        RA(a0)->jumpCharge = 0;
        v0 = RA(a0)->jumpCharge;
    }
    p = (int *)RA(a0)->rider;
    f2 = RA(a0)->parms.jca.value.f * (float)v0;
    b770 = (char*)p; if (1) { b770 += 0x770; } /* &rider->parms.jmp */
    b510 = (char*)p; if (1) { b510 += 0x510; } /* &rider->child.parms.flx */
    if (((SnowBoarder *)p)->useDParms != 0) {
        b788 = (char*)p; if (1) { b788 += 0x788; } /* &rider->parms.djp */
        f0 = ((SnowBParm *)b788)->value + f2;
    } else {
        f0 = ((SnowBParm *)b770)->value + f2;
    }
    if (((SnowBoarder *)p)->surface == 'b') {
        f0 = (float)(0.7 * (double)f0);
    }
    b31C = (char*)p; if (1) { b31C += 0x31C; } /* &rider->dnode.vel[1] */
    f0 *= ((SnowBParm *)b510)->value;
    *(float*)(b31C) = *(float*)(b31C) + f0;
    RA(a0)->queuedTrick = 0;
    import_00096874((int)RA(a0)->rider->sound);
    RiderAnim_startHop_0001001C(a0);
}

extern void RiderAnim_readTrickInput_0000E5C8(char *, int);
void RiderAnim_stateTrickAir(int *a0) {
    char *w;
    float *pa;
    float *pb;
    float *pc;

    RA(a0)->rider->unkA18 = 0;
    RA(a0)->trickPhase = 1;
    w = (char *)RA(a0)->rider;
    pa = (float*)(w + 0x170); /* &rider->dnode.parms.gvt */
    pb = (float*)(w + 0x698); /* &rider->parms.axr .. */
    pc = (float*)(w + 0x31C); /* &rider->dnode.vel[1] */
    if ((((SnowBoarder *)w)->groundFlags & 1) == 0) {
        pc[0] += -pa[4] * pb[4];
        RiderAnim_holdJumpLift_0000E2D0((char*)a0, pa, pb);
    }
    RiderAnim_readTrickInput_0000E5C8((char*)a0, 0);
}

/* PREREQUISITE (outside this body): RiderAnim_holdJumpLift_0000E2D0's definition
 * converted ANSI->K&R (`(a0) char *a0; {`) — verified byte-IDENTICAL vs
 * expected/src/game_uso/game_uso.c.o — and its ANSI extern prototype
 * (`extern void RiderAnim_holdJumpLift_0000E2D0(char *);` near func_00010128) REMOVED,
 * so the 3-arg call above is legal (extras unchecked, materialize a1/a2). */

/* RiderAnim_stateTrickEnd: 30-insn state-init. Matched via struct-by-value
 * arg-home: the D[0xDF0]/D[0xDF4] pair passed as `*(Pair2*)(&D+0xDF0)` homes
 * a1,a2 to sp+4/sp+8. */
void RiderAnim_stateTrickEnd(int *a0) {
    RA(a0)->trickPhase = 2;
    if (RA(a0)->curTrick->flags & TRICK_FLAG_HALF_TURN) {
        CALL(RiderAnim_flipStance_0000D6E4)(a0);
    }
    RA(a0)->rider->unkA18 = 1;
    RA(a0)->curTrick = 0;
    CALL(RiderAnim_enterState_0000D5BC)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateHopAir));
    CALL(RiderAnim_stateHopAir)(a0);
}

/* RiderAnim_stateTrickWindup: 37-insn state-init w/ flag-gated branch.
 * gl_func_0(a0); a0->0xB4->0xA68 = 1; check (a0->0xB4)->0x800->0x10 & 0x200:
 *   if set:  gl_func_0(a0); gl_func_0(a0, 1, 1);
 *   else:    gl_func_0(a0, D[0xED8], D[0xEDC]);
 * gl_func_0(a0). */
void RiderAnim_stateTrickWindup(int *a0) {
    RiderInput *p2;
    CALL(RiderAnim_stateTrickAir)(a0);
    /* inline `sub` (a0[0xB4], read twice) so it takes $t-regs, but NAME p2 so it
     * claims $v0 like the target (same lever as RiderAnim_stateHopStart). */
    RA(a0)->rider->unkA68 = 1;
    p2 = RA(a0)->rider->input;
    if (!(p2->held & 0x200)) {
        CALL(RiderAnim_enterState_0000D5BC)(a0,
            *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateTrickHold));
    } else {
        CALL(RiderAnim_clearTricks_00011888)(a0);
        CALL(RiderAnim_addTrickPoints_0000EE30)(a0, 1, 1);
    }
    CALL(RiderAnim_trickCrash_0000D74C)(a0);
}

extern int gl_func_00000000();
typedef struct { int a, b; } F5A8Pair;
extern char game_uso_D_807FF4D0;
extern char game_uso_D_807FF4D8;
int RiderAnim_trickCrash_0000D74C(char *a0);
void RiderAnim_stateTrickStart(char *a0);
/* RiderAnim_stateTrickRelease: 47-insn EE84-family two-path state orchestrator.
 * MATCHED 2026-06-21 (call-graph DFS + just-in-time family typing). The prior
 * NM body called the cross-USO placeholder gl_func_00000000 for every call and
 * carried a phantom `int a1` param. The resolved .s relocs name the real in-TU
 * callees: RiderAnim_trickCrash_0000D74C (predicate), RiderAnim_enterState_0000D5BC /
 * _0000D5F8 (pair-sinks), RiderAnim_clearState_0000D5DC (pair-copy),
 * import_0010DB28 (state logger), RiderAnim_stateTrickStart (sibling). The last
 * residual (the D5BC call not homing a1/a2 to sp+4/sp+8) was cracked by
 * converting RiderAnim_enterState_0000D5BC to its true Pair2-by-value prototype
 * `(char*, Pair2)` — the by-value pair triggers the K&R home spill that the
 * folded two-int form suppressed (same lever as the matched D5F8 callers
 * 00011258/000112E0). Byte-exact (0 SUSPECT / 0 reloc-type diffs). */
void RiderAnim_stateTrickRelease(int *a0) {
    int *s0 = a0;
    if (RA(s0)->queuedTrick != 0) {
        if (RiderAnim_trickCrash_0000D74C((char*)a0) == 0) {
            char *p = (char *)&game_uso_D_807FF4D0 + kPmf_RiderAnim_stateTrickStart;
            RiderAnim_enterState_0000D5BC((char*)s0, *(Pair2*)p);
            RiderAnim_stateTrickStart((char*)s0);
        }
    } else {
        import_0010DB28(s0, RA(s0)->curTrick->anim, 3, 1, 1, 1);
        RiderAnim_queueState_0000D5F8((char*)s0,
            *(Pair2 *)((char *)&game_uso_D_807FF4D8 + kPmf_RiderAnim_stateTrickEnd), 1);
        RiderAnim_clearState_0000D5DC((char*)s0);
    }
}

/* MATCHED 2026-05-28: struct-by-value (EC8/ECC pair homes a1,a2) + inlining the
 * a0->0xF4 deref (named local -> $v0; inline -> $t6 like target). The
 * "precall-arg-spill cap" was wrong. See
 * docs/IDO_CODEGEN.md#feedback-ido-struct-by-value-homes-arg-pair. */
void RiderAnim_stateTrickHold(char *a0) {
    CALL(AnimPlayer_startIfChanged)(a0, RA(a0)->curTrick->anim, 2, 3, 1, 1);
    CALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateTrickRelease), 3);
    CALL(RiderAnim_readTrickInput_0000E5C8)(a0, 0);
}

/* RiderAnim_stateTrickStart (0x210 / 132 words): node-process / queue-advance
 * handler, sibling of RiderAnim_startQueuedTrick_0000E35C. Optional D6E4 pre-flush
 * (cur->0x38 & 1); EE30(self, active->0x30, 1); import_000B8518 index call
 * with a ceil-div-by-64 node offset (two forms on obj->0xA58 & 0x20);
 * counter (0x100)++, F4 = F0, F0 = 0, counter reset when F8 == F4, F8 = F4;
 * E564(self); cur->0x20 = cur->0x28 (if 0xA58 & 0x40 and nonzero) else
 * cur->0x24; 077C44 + D5F8 (pointer-to-member by value) on cur->0x38 & 2;
 * then D5BC.
 * EXACT via REPLACE_FUNC_BODY donor splice: the real body is the IDO 7.1 NCC
 * (C++) donor src/game_uso/game_uso_ncc_F6D4.cpp (132/132 words). The cfe
 * wrap capped at 95.4%: edg puts the ceil-div addu base-first, and the
 * target has NO named prev local (the F0 -> F4 move goes through the members
 * and the F8 compare/store re-read F4; a named local colours v0 and renumbers
 * every later temp). See docs/MATCHING_WORKFLOW.md#game-uso-ncc-cplusplus-donor-recipe.
 * Body below is a placeholder for the splice. */
extern char game_uso_D_807FF4B0;
extern char game_uso_D_807FF4B8;
extern char game_uso_D_807FF4C0;
extern int import_000B8518();
void RiderAnim_stateTrickStart(char *a0) {
    volatile char *ret = a0;
}

/* MATCHED 2026-05-28 via struct-by-value (E50/E54 pair homes a1,a2). The
 * varargs-extern attempt failed because the spill is struct-by-value-driven,
 * not varargs. See docs/IDO_CODEGEN.md#feedback-ido-struct-by-value-homes-arg-pair. */
void RiderAnim_stateRowIn(int *a0) {
    CALL(AnimPlayer_startIfChanged)(a0, RA(a0)->animSet, 0, 0, 0x100, 5);
    CALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateRow), -1);
}

/* EXACT 2026-07-02 (67/67 words vs asm .s, self-verified via clean
 * build/non_matching rebuild). Was the "frame-spread / call-spill-interleave
 * coloring cap" NM at 98.15% — cap DISPROVEN by two levers:
 *  (1) FIRST-BLOCK RE-BASE (same lever as the final-add section): load the
 *      scaled triple through q = (float*)((char*)(int*)a0[0xB4/4] + 0x3C8)
 *      instead of a named b + per-field offsets. IDO folds the loads to
 *      0x3C8/0x3CC/0x3D0(v0) AND emits the (dead) addiu v0,v0,0x3C8 in the
 *      mul latency slot — the missing word — and the coloring cascade fixes
 *      itself: b/q coalesce into $v0 (was $v1), freeing $v1 for &vb (was $a1).
 *  (2) FRAME PUSH-UP FROM BELOW: target has vc flush against the frame top
 *      (sp+0x74..0x80, frame -0x80). Locals lay out in DECL ORDER top-down;
 *      scalar decls placed AFTER the aggregates get no slots; unreferenced
 *      volatile pads declared BEFORE the first Vec3 sink to the bottom (the
 *      old cap note is right about that) — but growing the BOTTOM pad
 *      (padbot[5]) pushes the whole Vec3 block up: 0x38/0x4C/0x64 ->
 *      0x48/0x5C/0x74 with frame exactly -0x80.
 * Earlier levers retained: grouped-mul eval order (mx/my/mz temp scalars,
 * docs/IDO_CODEGEN.md#feedback-ido-struct-copy-vs-field-copy-treg-order),
 * inter-decl pads padcb[3]/padba[2] for the 0x8/0xC inter-Vec3 gaps,
 * struct-assign copies (vb = a; vc = vb;) for the int lw/sw cascade. */
extern char game_uso_D_807FF448;
typedef struct { float x, y, z; } F948_Vec3;
void RiderAnim_stateRow(int *a0) {
    F948_Vec3 vc;
    volatile int padcb[3];
    F948_Vec3 vb;
    volatile int padba[2];
    F948_Vec3 a;
    float *q;
    float scale;
    float mx, my, mz;
    volatile int padbot[5];

    import_0010DB28(a0, RA(a0)->animSet, 0, 2, 1, 1);
    RiderAnim_queueState_0000D5F8((char*)a0, *(Pair2*)((char*)&game_uso_D_807FF448 + kPmf_RiderAnim_stateRowOut), 2);

    q = RA(a0)->rider->dnode.axes[2];
    scale = RA(a0)->parms.rfc.value.f;
    mx = q[0] * scale;
    my = q[1] * scale;
    mz = q[2] * scale;
    a.x = mx;
    a.y = my;
    a.z = mz;
    vb = a;
    vc = vb;

    q = RA(a0)->rider->dnode.vel;
    q[0] += vc.x;
    q[1] += vc.y;
    q[2] += vc.z;
}

/* MATCHED 2026-05-28: struct-by-value (E40/E44 pair). 26-insn 3-call
 * wrapper, family sibling of 10E2C/14FC/DC8 with one extra pass-through
 * call before the family-pattern body. See
 * docs/IDO_CODEGEN.md#feedback-ido-struct-by-value-homes-arg-pair. */
void RiderAnim_stateRowOut(int *a0) {
    GCALL(AnimPlayer_startIfChanged)(a0, RA(a0)->animSet, 2, 1, 1, 1);
    GCALL(RiderAnim_clearRiderEdge_00011868)(a0);
    GCALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateRide), 1);
}

/* MATCHED 2026-05-28: struct-by-value (E40/E44 pair). 18-insn 3-call
 * wrapper, sibling of RiderAnim_stateRowOut. See
 * docs/IDO_CODEGEN.md#feedback-ido-struct-by-value-homes-arg-pair. */
void RiderAnim_stateResumeRide(int a0) {
    CALL(RiderAnim_clearRiderEdge_00011868)(a0);
    CALL(RiderAnim_enterState_0000D5BC)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateRide));
    CALL(RiderAnim_stateRide)(a0);
}

/* RiderAnim_stateCrashRecover: 30-insn dispatcher with conditional gate.
 * Matched via struct-by-value arg-home: the D[0xE40]/D[0xE44] pair passed as
 * `*(Pair2*)(&D+0xE40)` homes a1,a2 to sp+4/sp+8. */
void RiderAnim_stateCrashRecover(int *a0) {
    CALL(RiderAnim_clearRiderEdge_00011868)(a0);
    if (RA(a0)->slowCrash != 0) {
        CALL(AnimPlayer_startIfChanged)(a0, RA(a0)->selectedAnim, 2, 1, 1, 1);
        CALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateRide), 1);
    }
}

/* RiderAnim_startCrashSlide_0000FB7C: 31-insn 3-call dispatcher. */
void RiderAnim_startCrashSlide_0000FB7C(int *a0) {
    int v = RA(a0)->animSet | RANIM_CRASH_SLIDE;
    RA(a0)->selectedAnim = v;
    CALL(AnimPlayer_startIfChanged)(a0, v, 0, 2, 1, 1);
    CALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateCrashRecover), 2);
    CALL(RiderAnim_clearRiderEdge_00011868)(a0);
    RA(a0)->trickPhase = 0;
}

void RiderAnim_blendTurn_0000FBF8(int *a0) {
    int v = RA(a0)->animSet;
    GCALL(AnimPlayer_setBlend)(a0, v | 2, v | 3,
        *(int*)&RA(a0)->rider->edge, 0, 1);
}

extern int gl_func_00000000();
extern char D_00000000;
/* RiderAnim_startOllie_0000FC34: EE84-family state setter (52 insns, 0xD0), EXACT.
 * The two pair-arg calls (D+0xE10, D+0xE20) pass `*(Pair2*)(&D+off)` BY VALUE
 * (clusters the base and homes a1/a2 to sp+4/sp+8). Other levers:
 *   (1) do-while(0) BB-break between field_31C's def and its load+store uses
 *       blocks the same-BB base+offset fold -> target's `addiu v0,v1,0x31C;
 *       lwc1/swc1 0(v0)` (docs #goto-mid-ptr-fold-block, 65060 kit).
 *   (2) de-named v + INLINED dconst deref inside the mul restores the target
 *       expression-temp FP ring f4/f6/f8/f10/f16/f18 (named v took f2, named
 *       dconst took f0 outside the ring).
 *   (3) `v*dconst + *field_31C` spelling (mul FIRST) emits target
 *       `add.d f4,f18,f10` (rs=late field-cvt, rt=mul) — commutative-operand
 *       reversal, same family as the |-reversal entry.
 * 2026-10-15 (agent-g, 97.96 -> EXACT): the "single-use" `addiu a3,v1,0x770`
 * was never single-use -- it is a 4th ARGUMENT word to the D5BC pair-copy
 * call (a3 set before the jal; call-shape-scan `nm=jal/012 tgt=jal/0123`).
 * Passing field_770 as a third C arg (after the 2-word Pair2) through the
 * prototyped zero-address alias game_uso_func_0000D5BC_a3 makes the base a
 * real multi-use value hoisted above the flag test, in a3.
 * 2026-10-16 (agent-g): dconst is now the literal 0.8 (IDO's local double at
 * RoData +0x248 after the rodata relayout; same bytes, honest source).
 * docs/MATCHING_WORKFLOW.md#game-uso-rodata-relayout-late-rodata. */
extern char game_uso_D_807FF400;
extern char game_uso_D_807FF410;
void RiderAnim_enterState_0000D5BC(char *, Pair2);
void RiderAnim_queueState_0000D5F8(char *a0, Pair2 pair, int a3);
void game_uso_func_0000D5BC_a3(char *, Pair2, char *);  /* alias of D5BC (pinned 0) with the a3 word the target passes */
extern int import_0010DB28();
void RiderAnim_startOllie_0000FC34(int *a0) {
    SnowBoarder *base = RA(a0)->rider;
    RiderInput *flag_loc = base->input;
    SnowBParm *field_770;
    field_770 = &base->parms.jmp;
    if ((flag_loc->held & 0x100) == 0) {
        float *field_31C;
        field_31C = &base->dnode.vel[1];
        do {
            *field_31C = (float)((double)field_770->value

                * 0.8
                + (double)*field_31C);
        } while (0);
        XCALL(game_uso_func_0000D5BC_a3, RiderAnim_enterState_0000D5BC)((char*)a0,
            *(Pair2*)((char*)&game_uso_D_807FF400 + kPmf_RiderAnim_stateRideBlendIn), field_770);
    }
    import_0010DB28(a0, (RANIM_GROUP_AIR | RANIM_OLLIE), 2, 2, ANIM_BLENDIN, 0xA);
    RiderAnim_queueState_0000D5F8((char*)a0,
        *(Pair2*)((char*)&game_uso_D_807FF410 + kPmf_RiderAnim_stateOllieTakeoff), -1);
}

extern char game_uso_D_807FF408;
void RiderAnim_clearState_0000D5DC(char *a0);
/* RiderAnim_stateOllieTakeoff: 50-insn sibling of RiderAnim_startOllie_0000FC34. When
 * bit 0x100 of (a0->0xB4)->0x800->0x10 is clear: log via import_0010DB28,
 * push the pair at game_uso_D_807FF408 + 0xE18 through
 * RiderAnim_queueState_0000D5F8 (trailing 3), RiderAnim_clearState_0000D5DC, then
 * sub->0x31C += sub->0x798 and sub->0x2FC..0x308 = {0, 0, 0, 1.0f}.
 *
 * Exact-match notes (2026-10-15, after the 2026-07-18 97.08 wrap): each base
 * pointer is `(float *)(*(char **)(s0 + 0xB4) + K)` with the load INLINE
 * (the 0xB4 load is CSE'd between the first two). That spelling is what keeps
 * `addiu a1,v1,0x788` + `lwc1 16(a1)` unfolded. Through a named base2/base3
 * local the 0x788 base folds to 0x798(v1). It also drops the do-while(0) BB
 * splits: the 1.0f store goes first by itself, and the second reload is
 * `lw v0; addiu v0,v0,0x2FC`. The zero stores are written high-to-low on
 * separate lines (the same-line order ties differently). */
void RiderAnim_stateOllieTakeoff(int *a0) {
    int *s0 = a0;
    /* base inlined -> expression temp t6 (dies at the flags load) */
    RiderInput *flags = RA(s0)->rider->input;

    if ((flags->held & 0x100) == 0) {
        float zero;
        SnowBParm *base_788;
        float *base_31C;
        float *base_2FC;
        import_0010DB28(s0, (RANIM_GROUP_AIR | RANIM_OLLIE), 2, 3, 1, 1);
        RiderAnim_queueState_0000D5F8((char*)s0,
            *(Pair2 *)((char *)&game_uso_D_807FF408 + kPmf_RiderAnim_stateOllieRise), 3);
        RiderAnim_clearState_0000D5DC((char*)s0);

        zero = 0.0f;
        base_788 = &RA(s0)->rider->parms.djp;
        base_31C = &RA(s0)->rider->dnode.vel[1];
        base_31C[0] = base_788->value + base_31C[0];
        base_2FC = RA(s0)->rider->dnode.angVel;
        base_2FC[3] = 1.0f;
        base_2FC[2] = zero;
        base_2FC[1] = zero;
        base_2FC[0] = zero;
    }
}

/* RiderAnim_stateOllieRise: 63-insn sibling of RiderAnim_stateOllieTakeoff / FC34.
 * Velocity-gated branch dispatcher. Outer gate fires only when
 * base->0x31C (signed float) is below -1.0f; inner branch dispatches
 * different call sequences based on whether base->0x9D0 is below 1000.0f.
 *
 * Outer-gate constants: lui $at, 0xBF80 → f6 = -1.0f; c.lt.s $f4, $f6 +
 * bc1fl skips the body if (base->0x31C) >= -1.0f.
 * Inner constants: lui $at, 0x447A → f10 = 1000.0f.
 *
 * BYTE-EXACT: the two &D+0xE28/0xE30 pair args passed as `*(Pair2*)` BY VALUE
 * produce both the cluster bases AND the `sw a1,4(sp); sw a2,8(sp)` caller-side
 * spills — cracking the precall-spill "cap" (see docs/PATTERNS.md). Replaces the
 * banned SUFFIX_BYTES + INSN_PATCH recipe. */
void RiderAnim_stateOllieRise(int *a0) {
    SnowBoarder *base = RA(a0)->rider;
    if (base->dnode.vel[1] < -1.0f) {
        if (base->groundDist < 1000.0f) {
            CALL(AnimPlayer_startIfChanged)(a0, (RANIM_GROUP_AIR | RANIM_OLLIE), 6, 6, ANIM_BLENDIN, 0x14);
            CALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateOllieLand), -1);
            CALL(RiderAnim_clearState_0000D5DC)(a0);
        } else {
            CALL(AnimPlayer_startIfChanged)(a0, (RANIM_GROUP_AIR | RANIM_OLLIE), 3, 4, 1, 1);
            CALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateOllieHang), 4);
        }
    }
}

/* MATCHED 2026-05-28 via struct-by-value (E38/E3C pair homes a1,a2). See
 * docs/IDO_CODEGEN.md#feedback-ido-struct-by-value-homes-arg-pair. */
void RiderAnim_stateOllieHang(char *a0) {
    CALL(AnimPlayer_startIfChanged)(a0, (RANIM_GROUP_AIR | RANIM_OLLIE), 4, 5, 2, 1);
    if (RA(a0)->rider->groundDist < 1000.0f) {
        CALL(RiderAnim_enterState_0000D5BC)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateOllieDrop));
    }
}

/* MATCHED 2026-05-28. 2-call gated dispatcher. The 2nd call's target home-stores
 * (sw a1,4(sp); sw a2,8(sp)) — long mislabeled a "precall-arg-spill cap" — are
 * produced by passing the D_E10/E14 pair AS A STRUCT BY VALUE:
 * `gl_func(a0, *(Pair2*)(&D+0xE10), 1)`. See
 * docs/IDO_CODEGEN.md#feedback-ido-struct-by-value-homes-arg-pair. */
void RiderAnim_stateOllieLand(char *a0) {
    if (RA(a0)->rider->contactCount != 0) {
        CALL(AnimPlayer_startIfChanged)(a0, (RANIM_GROUP_AIR | RANIM_OLLIE), 6, 1, 1, 1);
        CALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateRideBlendIn), 1);
    }
}

/* MATCHED 2026-05-28 via struct-by-value arg-pass (sibling of FF48): the 2nd
 * call homes the E28/E2C pair to a1/a2 by passing *(Pair2*)(&D+0xE28). See
 * docs/IDO_CODEGEN.md#feedback-ido-struct-by-value-homes-arg-pair. */
void RiderAnim_stateOllieDrop(int *a0) {
    CALL(AnimPlayer_startIfChanged)(a0, (RANIM_GROUP_AIR | RANIM_OLLIE), 5, 6, 1, 1);
    CALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateOllieLand), 6);
}

/* MATCHED 2026-05-28: struct-by-value (DD0/DD4 pair). The middle call's
 * 2-int load was the precall-arg-spill "cap" — passing the pair as
 * *(Pair2*) by value homes a1,a2 to sp+4/sp+8 exactly. See
 * docs/IDO_CODEGEN.md#feedback-ido-struct-by-value-homes-arg-pair. */
void RiderAnim_startHop_0001001C(int *a0) {
    RA(a0)->curTrick = 0;
    CALL(RiderAnim_clearTricks_00011888)(a0);
    CALL(RiderAnim_enterState_0000D5BC)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateHopStart));
    CALL(RiderAnim_stateHopStart)(a0);
}

/* RiderAnim_stateHopStart: 48-insn (0xC0) 6-call dispatcher (EXACT). The old NM-era wrap notes were
 * removed in agent-g run 506 (git e6043cf4d has them). */
extern int gl_func_00000000();
extern char D_00000000;
void RiderAnim_stateHopStart(int *a0) {
    /* Call 1: 6-arg w/ 2 stack args */
    CALL(AnimPlayer_startIfChanged)(a0,
                     RA(a0)->animSet | RANIM_HOP,
                     0, 2,
                     0x100, 10);
    /* Call 2: 4-arg with D[0xDD8/0xDDC] */
    CALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateHopRise), -1);
    /* Conditional Call 3 — inline `outer` (arg0[0xB4]) so it takes $t1 not $v0,
     * but NAME the middle pointer `mid` so it claims $v0 like the target. */
    {
        RiderInput *mid = RA(a0)->rider->input;
        if (mid->pressed & 0x400) {
            CALL(RiderAnim_enterState_0000D5BC)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateCushionIn));
        }
    }
    /* Call 4 */
    CALL(RiderAnim_holdJumpLift_0000E2D0)(a0);
    /* Call 5 */
    CALL(RiderAnim_readTrickInput_0000E5C8)(a0, 0);
    /* Call 6 */
    CALL(RiderAnim_startQueuedTrick_0000E35C)(a0);
}

/* RiderAnim_stateHopRise: 105-insn (0x1A4) complex dispatcher. LANDED 2026-06-22
 * (agent-e): byte-exact (ROM-identical, `make verify` clean; 0 non-reloc word
 * diffs vs expected). The "outer<->mid v0/v1 spilltemp-web coloring cap" was a
 * PLACEHOLDER-SYMBOL artifact, NOT a real coloring cap: the body had used
 * gl_func_00000000/D_00000000 placeholders, which color differently than the
 * REAL reloc callees. Rewriting with the real symbols (RiderAnim_enterState_0000D5BC,
 * import_0010DB28, RiderAnim_queueState_0000D5F8 typed varargs-Pair2-by-value,
 * D5DC/E5C8/E35C/E2D0/11750, and game_uso_D_807FF3D0/D8/E0) flipped the
 * coloring so `outer`(a0->0xB4 reload)=$v0 and `mid`(outer->0x800)=$v1 — exactly
 * the target — and the 6 v0/v1 word diffs vanished. (Same lesson as twin 11258:
 * placeholder symbols score under name-blind objdiff but are not byte-correct;
 * the real-symbol coloring is the match.) The only residual vs expected is 3
 * extra LO16 relocs (HI16+LO16 pair vs expected's HI16+baked-LO16) — a benign
 * raw-word-USO encoding artifact that resolves identically at the ROM-bin level.
 * Original decode notes preserved below.
 *
 * 105-insn complex dispatcher. Frame -0x28; saves s0 (= a0), ra. Multiple cross-USO calls dispatched
 * via float-compare and bit-flag tests. Sibling of 0x10068 (the same
 * "outer = a0->0xB4; if (outer->0x800->0x18 & 0x400) ..." pattern).
 *
 * Body shape (decoded 2026-05-08):
 *
 *   outer = a0->0xB4;
 *   outer->0xA18 = 1;          // unconditional flag set at function entry
 *   if (outer->0x800->0x18 & 0x400) {
 *       gl_func(a0, D[0xDE0], D[0xDE4]);   // call A — w/ shadow spills
 *   }
 *   if (outer->0x9D0 < 1000.0f) {           // float-cmp via c.lt.s + bc1fl
 *       if (outer->0x938 != 0) {
 *           gl_func(a0, D[0xDE8], D[0xDEC]);   // call B
 *       }
 *       inner = a0->0xF4;
 *       if (inner != 0) {
 *           if (inner->0x38 & 1) {
 *               gl_func(a0, (a0->0xFC|0x19), 0, 5, 5, 1, 1);  // call C-1
 *           } else {
 *               gl_func(a0, (a0->0xFC|0x19), 0, 5, 5, 0x100, 10);  // call C-2
 *           }
 *       } else {
 *           gl_func(a0, (a0->0xFC|0x19), 0, 5, 5, ?, ?);  // call C-3 (uninit
 *           // t3/t4 stack args — likely 0/0 default since regs not set in
 *           // this branch)
 *       }
 *       gl_func(a0, D[0xDF0], D[0xDF4], 3);   // call D (after the 3-way
 *                                                merge from C-1/C-2/C-3)
 *       gl_func(a0);                          // call E
 *       gl_func(a0, 0);                       // call F
 *       gl_func(a0);                          // call G — returns v0
 *       if (v0 == 0) {                        // bnel + delay-likely epilogue
 *           gl_func(a0);                       // call H
 *           gl_func(a0);                       // call I
 *       }
 *   }
 *   // (no else for the float-compare — bc1fl annulled the bc1fl arm)
 *
 * Shape notes from the decode: a0->0xB4 (the rider) is re-read at each use, not
 * cached; the 6-arg AnimPlayer calls are (a0, anim, a2, a3, sp16, sp20); the
 * inner == 0 path falls through to the &1 == 0 body, so the guard is
 * `(inner != 0) && (inner->0x38 & 1)`; the `>= 1000.0f` test has a real else.
 * The input word is named (`mid`) so the flag tests stay in v0/v1. */
extern char game_uso_D_807FF3D0;
extern char game_uso_D_807FF3D8;
extern char game_uso_D_807FF3E0;
extern void RiderAnim_queueState_0000D5F8(char *, Pair2, int);
extern void RiderAnim_clearState_0000D5DC(char *);
extern void RiderAnim_readTrickInput_0000E5C8(char *, int);
extern int RiderAnim_startQueuedTrick_0000E35C(char *);
extern void RiderAnim_holdJumpLift_0000E2D0();
extern void RiderAnim_checkBackwardSwitch_00011750(char *);
void RiderAnim_stateHopRise(int *a0) {
    TrickEntry *inner;
    int *mid;

    RA(a0)->rider->unkA18 = 1;
    mid = (int *)RA(a0)->rider->input;
    if ((((RiderInput *)mid)->pressed & 0x400) != 0) {
        RiderAnim_enterState_0000D5BC((char*)a0,
                         *(Pair2*)((char*)&game_uso_D_807FF3D0 + kPmf_RiderAnim_stateCushionIn));
    }
    if (RA(a0)->rider->groundDist < 1000.0f) {
        if (RA(a0)->rider->contactCount != 0) {
            RiderAnim_enterState_0000D5BC((char*)a0,
                             *(Pair2*)((char*)&game_uso_D_807FF3D8 + kPmf_RiderAnim_stateHopDrop));
        }
        inner = RA(a0)->curTrick;
        if ((inner != 0) && (inner->flags & TRICK_FLAG_HALF_TURN)) {
            import_0010DB28(a0,
                             RA(a0)->animSet | RANIM_HOP,
                             5, 5, 1, 1);
        } else {
            import_0010DB28(a0,
                             RA(a0)->animSet | RANIM_HOP,
                             5, 5, 0x100, 10);
        }
    } else {
        import_0010DB28(a0,
                         RA(a0)->animSet | RANIM_HOP,
                         2, 3, 1, 1);
        RiderAnim_queueState_0000D5F8((char*)a0,
                         *(Pair2*)((char*)&game_uso_D_807FF3E0 + kPmf_RiderAnim_stateHopAir),
                         3);
        RiderAnim_clearState_0000D5DC((char*)a0);
    }
    RiderAnim_readTrickInput_0000E5C8((char*)a0, 0);
    if (RiderAnim_startQueuedTrick_0000E35C((char*)a0) == 0) {
        RiderAnim_holdJumpLift_0000E2D0((char*)a0);
        RiderAnim_checkBackwardSwitch_00011750((char*)a0);
    }
}

/* RiderAnim_stateHopAir — EE84-family state (PATTERNS.md EE84-family template). Structure:
 *   func_00000000(a0);                                    // X1
 *   func_00000000(s0, s0->0xFC | 0x19, 3, 4, 2, 1);       // X2 (6 args; 2,1 @sp)
 *   v0 = (int*)s0->0xB4;
 *   if (*(float*)(v0+0x31C) < 0.0f && *(float*)(v0+0x9D0) < 1000.0f) {
 *       func_00000000(s0, D[0xDE8], D[0xDEC]);             // X3 (D-pair @sp+4/8)
 *       v0 = (int*)s0->0xB4;
 *   }
 *   if (((int*)v0->0x800)->0x18 & 0x400)
 *       func_00000000(s0, D[0xDE0], D[0xDE4]);             // X4
 *   func_00000000(s0, 0);                                  // X5
 *   if (func_00000000(s0) == 0) {                          // X6
 *       int *t = (int*)s0->0xB4;
 *       if (t->0x938 != 0) {
 *           func_00000000(s0, D[0xDF8], D[0xDFC]);          // X7
 *           func_00000000(s0);                              // X8
 *       }
 *       func_00000000(s0);                                  // X9
 *   }
 * Struct-typing: s0->0xFC flag word, s0->0xB4 object (float @0x31C, float
 * @0x9D0, ptr @0x800 whose +0x18 has bit 0x400, int @0x938). 78.57% -> 98.61%:
 * the three D-pair consts (0xDE0/DE4, 0xDE8/DEC, 0xDF8/DFC) pass BY VALUE as
 * *(Pair2*) — the "sp+4/sp+8 EE84 D-pair shape cap" was the un-homed pair
 * (same lever as EF70, docs/IDO_CODEGEN.md#feedback-ido-struct-by-value-homes-arg-pair).
 * 2026-07-15 (wave 3, agent-h): the old residual ("s0->0xB4 reload lands in
 * v1 vs v0, regalloc-renumber cascade" 8 words) RETRACTED via the FD0
 * dead-$v0-def exclusion lever (docs/IDO_CODEGEN): import_0010DB28's UNUSED
 * int K&R return left a dead $v0 def that excluded $v0 from the immediately
 * following v0-var web (colored v1, new_var took v0). Calling through the
 * void alias import_0010DB28_void (=0 in undefined_syms_auto.txt, identical
 * 0C000000 jal bytes) frees $v0 -> 79/79 objdiff-100. (26FC return-capture
 * probed first: inert, immediate-redef web split per the 1C90 caveat.)
 * 2026-10-16 (agent-g): UNWRAPPED EXACT. The "in-TU jal" hold-back does not
 * apply to Yay0-block units (block cut from the unlinked .o, jal stays 0);
 * see docs/MATCHING_WORKFLOW.md#yay0-block-unlinked-in-unit-jal-f360. */
extern char game_uso_D_807FF3D8;
extern char game_uso_D_807FF3D0;
extern char game_uso_D_807FF3E8;
void RiderAnim_stateHopLand(char *a0);
void RiderAnim_checkBackwardSwitch_00011750(char *obj);
extern void import_0010DB28_void();
void RiderAnim_stateHopAir(int *a0) {
  int *s0 = a0;
  SnowBoarder *v0;
  int new_var;
  RiderAnim_holdJumpLift_0000E2D0((char *)a0);
  import_0010DB28_void(s0, RA(s0)->animSet | RANIM_HOP, 3, 4, 2, 1);
  v0 = RA(s0)->rider;
  if ((v0->dnode.vel[1] < 0.0f) && (v0->groundDist < 1000.0f))
  {
    RiderAnim_enterState_0000D5BC((char *)s0, *((Pair2 *) (((char *) (&game_uso_D_807FF3D8)) + kPmf_RiderAnim_stateHopDrop)));
    v0 = RA(s0)->rider;
  }
  if (((RiderInput *) (new_var = (int) v0->input))->pressed & 0x400)
  {
    RiderAnim_enterState_0000D5BC((char *)s0, *((Pair2 *) (((char *) (&game_uso_D_807FF3D0)) + kPmf_RiderAnim_stateCushionIn)));
  }
  RiderAnim_readTrickInput_0000E5C8((char *)s0, 0);
  if (RiderAnim_startQueuedTrick_0000E35C((char *)s0) == 0)
  {
    ;
    if (RA(s0)->rider->contactCount != 0)
    {
      RiderAnim_enterState_0000D5BC((char *)s0, *((Pair2 *) (((char *) (&game_uso_D_807FF3E8)) + kPmf_RiderAnim_stateHopLand)));
      RiderAnim_stateHopLand((char *)s0);
    }
    RiderAnim_checkBackwardSwitch_00011750((char *)s0);
  }
}

/* Small state-dispatch helper. BYTE-EXACT (was force-promoted via banned
 * SUFFIX_BYTES + INSN_PATCH). Two legit levers: (1) pass the &D+0xDF8 pair as a
 * `*(Pair2*)` BY VALUE — the by-value struct both clusters the base (lui;addiu;
 * lw 0/4) and homes a1/a2 to sp+4/sp+8 (the late spills IDO omits for separate
 * int args); (2) inline the single-use spB4 deref (no named local) so it takes
 * a $t-reg instead of $v0. Same Pair2-by-value idiom as RiderAnim_stateCushionEnd. */
void RiderAnim_stateHopDrop(int *arg0) {
    CALL(AnimPlayer_startIfChanged)(arg0, RA(arg0)->animSet | RANIM_HOP, 4, 5, 1, 1);
    CALL(RiderAnim_readTrickInput_0000E5C8)(arg0, 0);
    if (CALL(RiderAnim_startQueuedTrick_0000E35C)(arg0) == 0) {
        if (RA(arg0)->rider->contactCount != 0) {

            CALL(RiderAnim_enterState_0000D5BC)(arg0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateHopLand));
            CALL(RiderAnim_stateHopLand)(arg0);
        }
    }
}

/* 50-insn sibling of RiderAnim_stateHopDrop/105DC family. Initializes
 * a0->0xB4->0xA18, conditionally runs the 0x938-gated path, then emits
 * the same t[0]/t[1] vararg-spill call shape used by nearby wrappers. */
void RiderAnim_stateHopLand(char *a0) {

    CALL(RiderAnim_clearRiderEdge_00011868)(a0);
    RA(a0)->rider->unkA18 = 1;
    CALL(RiderAnim_readTrickInput_0000E5C8)(a0, 0);

    if (CALL(RiderAnim_startQueuedTrick_0000E35C)(a0) == 0) {
        if (RA(a0)->rider->contactCount != 0) {
            CALL(RiderAnim_endTrick_0000D63C)(a0, 1);
            CALL(RiderAnim_clearRiderEdge_00011868)(a0);
            CALL(AnimPlayer_startIfChanged)(a0, RA(a0)->animSet | RANIM_HOP, 5, 1, 1, 1);
            CALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2 *)((char *)&D_00000000 + kPmf_RiderAnim_stateRideBlendIn), 1);
            CALL(RiderAnim_clearState_0000D5DC)(a0);
        }
    }
}

/* MATCHED via struct-by-value arg-home: the adjacent D[0xE08]/D[0xE0C] pair
 * passed as `*(Pair2*)(&D+0xE08)` homes a1,a2 to sp+4/sp+8. Has a final float
 * copy tail. (The "precall-arg-spill family / INSN_PATCH ineligible" cap was wrong.) */
void RiderAnim_stateCushionIn(char *a0) {
    CALL(AnimPlayer_startIfChanged)(a0, (RANIM_GROUP_REGULAR | RANIM_KNEE_CUSHION), 0, 0, ANIM_BLENDIN, 5);
    CALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateCushion), -1);
    RA(a0)->hitScale = RA(a0)->parms.lnc.value.f;
}

/* MATCHED 2026-05-28: struct-by-value (E00/E04 pair). 27-insn 3-call
 * sibling of 10E2C/11368/FA54 family. The "t8-base + varargs spills" cap
 * was the un-homed pair; symbol size is exactly 27 insns (the old
 * "trailing alt-entry stub" worry was a separate symbol, not this one).
 * See docs/IDO_CODEGEN.md#feedback-ido-struct-by-value-homes-arg-pair. */
void RiderAnim_stateCushion(int *a0) {
    GCALL(AnimPlayer_startIfChanged)(a0, (RANIM_GROUP_REGULAR | RANIM_KNEE_CUSHION), 0, 1, 1, 1);
    GCALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateCushionEnd), 1);
    GCALL(RiderAnim_clearRiderEdge_00011868)(a0);
}

/* RiderAnim_stateCushionEnd (0x4C): orphan-prologue MERGE of the 0x8-byte stub at
 * 0x10648 (`lui at,0x3F80; mtc1 at,$f4` = load 1.0f) + the 0x44 body formerly
 * mislabeled game_uso_func_00010650 (0x10650). The two are ONE routine: the stub
 * loads 1.0f into $f4, which IDO HOISTS above the prologue, and the body stores
 * it via `swc1 $f4,0x11C(a0)`. The earlier "caller-set float-in-$f4 cap" note was
 * wrong — $f4 is not a caller convention, it's this function's own 1.0f constant.
 * First call passes a 2-int struct BY VALUE (homes a1->sp+4, a2->sp+8); the named
 * base `&D+0xDF8` keeps the addiu split (lw 0(t6)/4(t6)). Byte-exact. */
void RiderAnim_stateCushionEnd(void *a0) {
    RA(a0)->hitScale = 1.0f;
    CALL(RiderAnim_enterState_0000D5BC)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateHopLand));
    CALL(RiderAnim_clearRiderEdge_00011868)(a0);
}

/* RiderAnim_stateRide: 0x1AC (107 insns). State-machine entry handler,
 * sibling of 00010840. EXACT via REPLACE_FUNC_BODY donor splice: the real
 * body is the IDO 7.1 NCC (C++) donor src/game_uso/game_uso_ncc_10694.cpp
 * (107/107 words identical). Two RiderAnim_enterState_0000D5BC(this, pm) calls pass
 * a pointer-to-member by value, the second guarded by the edg 2-word PMF
 * null compare (same block as D204). The old "mfc1-from-C cap" was a missing
 * float prototype for import_0010DB78. docs/MATCHING_WORKFLOW.md#game-uso-
 * ncc-cplusplus-donor-recipe. Body below is a placeholder for the splice. */
void RiderAnim_stateRide(int *a0) {
    /* placeholder; also emits the donor's double 0.06 (RoData 0x250) */
    *(volatile double *)a0 = 0.06;
}

/* RiderAnim_stateRideDeep (0x10C / 67 words): state-entry handler, sibling of
 * 10694. sub->0x3DC = 1, D8A8, D8EC; f0 = sub->0x970; |f0| > double at
 * D_807FFB78+0x258 ? DB78(self, 0x20002, 0x20003, f0, 0, 1)
 *                  : DB28(self, 0x20008, 0, 1, 6, 1);
 * E1FC; D5BC(self, PMF at D_807FF400+0xE10) when sub->0x9CC == 0; 11750.
 * EXACT via REPLACE_FUNC_BODY donor splice: the real body is the IDO 7.1 NCC
 * (C++) donor src/game_uso/game_uso_ncc_10840.cpp (67/67 words + 11/11
 * relocs). The old cfe wrap (87.67%) misread two things: `mfc1 a3,f0` is
 * DB78's float 4th parameter, and the "dead arg-prep" was the else-arm's
 * DB28 arguments. See
 * docs/MATCHING_WORKFLOW.md#game-uso-ncc-cplusplus-donor-recipe.
 * Body below is a placeholder for the splice. */
void RiderAnim_stateRideDeep(int *a0) {
    /* placeholder; also emits the donor's double 0.06 (RoData 0x258) */
    *(volatile double *)a0 = 0.06;
}

/* RiderAnim_stateBrake: 48-insn EE84-family (near-twin of 10A0C).
 * MATCHED 2026-07-08, word-diff 48/48 incl. reloc symbols (import_0010DB78,
 * game_uso_D_807FF400+0xE10, game_uso_D_807FF460+0xE70, D5BC/E91C/D8EC/D7F4):
 * (1) *(Pair2*) by-value D-pair calls (0xE10, 0xE70) home a1/a2 to sp+4/8;
 * (2) inline the first 0xB4 deref (named local colored it v0; inline -> t6)
 *     and the 0x9CC test (named t2 colored v1; inline -> $t2 temp + bnezl
 *     dup-load in the delay slot);
 * (3) the 0xFC state word must be INLINED TWICE (|4, |5) so CSE creates the
 *     v0 candidate AFTER the 0xB4 deref pseudo -- a named `int v` local is
 *     created before the s0=a0 copy point and bases its lw on $a0 not $s0
 *     (the last 47/48 residual). Param used directly, no explicit s0 local. */
extern int import_0010DB78();
extern void RiderAnim_enterState_0000D5BC(char *, Pair2);
extern void RiderAnim_checkGroundSpin_0000E91C(char *);
extern void RiderAnim_checkJump_0000D8EC(int *);
extern void RiderAnim_checkReverseBrake_0000D7F4(char *);
extern char game_uso_D_807FF400;
extern char game_uso_D_807FF460;
void RiderAnim_stateBrake(int *a0) {
    SnowBoarder *p;
    import_0010DB78(a0, RA(a0)->animSet | RANIM_BRAKE_B, RA(a0)->animSet | RANIM_BRAKE_F, *(int *)&RA(a0)->rider->edge, 0, 1);
    p = RA(a0)->rider;
    if (p->crouch == 0) {
        RiderAnim_enterState_0000D5BC((char *)a0, *(Pair2 *)((char *)&game_uso_D_807FF400 + kPmf_RiderAnim_stateRideBlendIn));
        p = RA(a0)->rider;
    }
    if (p->useDParms != 0) {
        RiderAnim_enterState_0000D5BC((char *)a0, *(Pair2 *)((char *)&game_uso_D_807FF460 + kPmf_RiderAnim_stateBrakeBlendIn));
    }
    RiderAnim_checkGroundSpin_0000E91C((char *)a0);
    RiderAnim_checkJump_0000D8EC(a0);
    RiderAnim_checkReverseBrake_0000D7F4((char *)a0);
}

/* RiderAnim_stateBrakeDeep (0xBC / 47 words): EE84-family, near-twin of
 * 0001094C (constant first-call args 0x20004/0x20005, PMF at
 * D_807FF438+0xE48, INVERTED 0x9CC test: D5BC(PMF at D_807FF460+0xE70) when
 * it is 0), then D8EC(self), D7F4(self).
 * EXACT via REPLACE_FUNC_BODY donor splice: the real body is the IDO 7.1 NCC
 * (C++) donor src/game_uso/game_uso_ncc_10A0C.cpp (47/47 words + 9/9 relocs).
 * The cfe C (93.4%) renumbered every temp; the same logic under NCC with the
 * PMFs passed by value matches. See
 * docs/MATCHING_WORKFLOW.md#game-uso-ncc-cplusplus-donor-recipe.
 * Body below is a placeholder for the splice. */
void RiderAnim_stateBrakeDeep(int *a0) {
    volatile int *ret = a0;
}

/* MATCHED via struct-by-value arg-home: the adjacent D[0xE40]/D[0xE44] pair
 * passed as `*(Pair2*)(&D+0xE40)` homes a1,a2 to sp+4/sp+8 (the target's
 * `sw a1,4(sp); sw a2,8(sp)`) — the "precall-arg-spill family" cap was wrong. */
void RiderAnim_stateRideBlendIn(char *a0) {
    int v0;
    CALL(RiderAnim_checkJump_0000D8EC)(a0);
    v0 = RA(a0)->animSet;
    CALL(AnimPlayer_setBlend)(a0, v0 | 2, v0 | 3, *(int *)&RA(a0)->rider->edge, 0x100, 10);
    CALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateRide), -1);
}

/* MATCHED 2026-05-28: struct-by-value (E60/E64 pair). 29-insn init +
 * 3-call orchestrator. The 3rd call's "tail reshape + varargs spills"
 * cap was the un-homed pair; *(Pair2*) by value reproduces the whole
 * tail. Inlined p->0x970 deref keeps the 2nd-call t6/t7/t8 regalloc.
 * See docs/IDO_CODEGEN.md#feedback-ido-struct-by-value-homes-arg-pair. */
void RiderAnim_stateRideDeepBlendIn(int *a0) {
    CALL(RiderAnim_checkJump_0000D8EC)(a0);
    CALL(AnimPlayer_setBlend)(a0, (RANIM_GROUP_DEEP | RANIM_TURN_B), (RANIM_GROUP_DEEP | RANIM_TURN_F), *(int *)&RA(a0)->rider->edge, ANIM_BLENDIN, 5);
    CALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateRideDeep), -1);
}

/* MATCHED 2026-05-28: struct-by-value arg-pass for the E10/E14 and E48/E4C
 * pairs (homes a1,a2), PLUS inlining the `sub = a0->0xB4` deref (named local
 * put it in $v1; inline keeps it in $t6 like target). See
 * docs/IDO_CODEGEN.md#feedback-ido-struct-by-value-homes-arg-pair. */
void RiderAnim_blendWeight_00010BAC(char *a0) {
    int state = RA(a0)->animSet;
    CALL(AnimPlayer_setBlend)(a0, state | 7, state | 6, *(int *)&RA(a0)->rider->localUp.x, 0, 1);

    if (RA(a0)->unkD8 == 0) {
        CALL(RiderAnim_enterState_0000D5BC)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateRideBlendIn));
    }

    if (RA(a0)->rider->useDParms != 0) {
        CALL(RiderAnim_enterState_0000D5BC)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateRideDeepBlendIn));
    }
}

/* BYTE-EXACT 2026-05-31 (stale-cap catch). Sibling of RiderAnim_blendWeight_00010BAC;
 * the struct-by-value arg-pass (*(Pair2*)(&D+0xE48/0xE10)) homes the pairs to
 * the outgoing arg slots, byte-exact — the 2026-05-28 struct-by-value sweep
 * landed 10BAC but missed this sibling, leaving it needlessly NM-wrapped.
 * .text identical to expected/.o. See docs/IDO_CODEGEN.md STALE-CAP CATCH. */
void RiderAnim_blendWeightDeep_00010C4C(char *a0) {
    CALL(AnimPlayer_setBlend)(a0, (RANIM_GROUP_DEEP | RANIM_WEIGHT_B), (RANIM_GROUP_DEEP | RANIM_WEIGHT_F),
                     *(int *)&RA(a0)->rider->localUp.x, 0, 1);

    if (RA(a0)->unkD8 == 0) {
        CALL(RiderAnim_enterState_0000D5BC)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateRideDeepBlendIn));
    }

    if (RA(a0)->rider->useDParms == 0) {
        CALL(RiderAnim_enterState_0000D5BC)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateRideBlendIn));
    }
}

void RiderAnim_stateBrakeBlendIn(char *a0) {
    SnowBoarder *sub;

    sub = RA(a0)->rider;
    if (sub->useDParms != 0) {
        CALL(AnimPlayer_setBlend)(a0, (RANIM_GROUP_DEEP | RANIM_BRAKE_B), (RANIM_GROUP_DEEP | RANIM_BRAKE_F), *(int *)&sub->edge, ANIM_BLENDIN, 10);
        CALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateBrakeDeep), -1);
    } else {
        int state;

        state = RA(a0)->animSet;
        CALL(AnimPlayer_setBlend)(a0, state | 4, state | 5, *(int *)&sub->edge, 0x100, 10);
        CALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateBrake), -1);
    }
    CALL(RiderAnim_checkGroundSpin_0000E91C)(a0);
}

/* MATCHED 2026-05-28: struct-by-value (E10/E14 pair). Family sibling of
 * RiderAnim_restartRide_00010E2C/000114FC. See
 * docs/IDO_CODEGEN.md#feedback-ido-struct-by-value-homes-arg-pair. */
void RiderAnim_startLanding_00010DC8(int a0) {
    GCALL(AnimPlayer_startIfChanged)(a0, RA(a0)->animSet | RANIM_LANDING, 0, 1, 1, 1);
    GCALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateRideBlendIn), 1);
}

/* MATCHED 2026-05-28: struct-by-value (E40/E44 pair). The prior "constant-
 * address-load-fold" cap framing was wrong — the 4-insn base+ofs load form is
 * the struct-by-value home-store, not a jumptable. See
 * docs/IDO_CODEGEN.md#feedback-ido-struct-by-value-homes-arg-pair. */
void RiderAnim_restartRide_00010E2C(int a0) {
    GCALL(AnimPlayer_startIfChanged)(a0, 0, 0, 1, 1, 1);
    GCALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateRide), 1);
}

void RiderAnim_stateGoalNormal(int a0) {
    GCALL(AnimPlayer_startIfChanged)(a0, (RANIM_GROUP_EVENT | RANIM_GOAL_NORMAL), 0, 1, 1, 1);
}

void RiderAnim_stateGoalWinOM(int a0) {
    GCALL(AnimPlayer_startIfChanged)(a0, (RANIM_GROUP_EVENT | RANIM_GOAL_WIN_OM), 0, 1, 1, 1);
}

void RiderAnim_stateGoalWin(int a0) {
    GCALL(AnimPlayer_startIfChanged)(a0, (RANIM_GROUP_EVENT | RANIM_GOAL_WIN), 0, 1, 1, 1);
}

void RiderAnim_stateGoalLose(int a0) {
    GCALL(AnimPlayer_startIfChanged)(a0, (RANIM_GROUP_EVENT | RANIM_GOAL_LOSE), 0, 1, 1, 1);
}

void RiderAnim_stateTimeUpLose(int a0) {
    GCALL(AnimPlayer_startIfChanged)(a0, (RANIM_GROUP_EVENT | RANIM_GOAL_LOSE), 0, 1, 1, 1);
}

/* MATCHED 2026-05-28: struct-by-value (F58/F5C pair). 27-insn 2-call
 * sibling of the 0x10E2C/11368/113C8 family. The "t0-base form + varargs
 * spills" cap was the un-homed pair — *(Pair2*) by value reproduces both.
 * See docs/IDO_CODEGEN.md#feedback-ido-struct-by-value-homes-arg-pair. */
void RiderAnim_stateStand(int *a0) {
    GCALL(AnimPlayer_startIfChanged)(a0, (RANIM_GROUP_EVENT | RANIM_STAND), 0, 1, 1, 1);
    RA(a0)->trickPhase = 0;
    RA(a0)->rider->slowdown = 100;
    GCALL(RiderAnim_enterState_0000D5BC)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateStand));
}

/* 32-insn / 0x80 dual-call orchestrator on a0->B4-child:
 *   a0->D0 = D[0xF48];
 *   a0->D4 = D[0xF4C];
 *   a0->B4->A58 &= ~4;          // clear bit 2 in B4-child's A58 field
 *   gl_func_00000000(a0, D[0xE10], D[0xE14]);  // first call
 *   gl_func_00000000(a0);                       // second call
 *   a0->B4->960 = 0x64;          // = 100
 *
 * Initial decode 2026-05-16; structural shape only. Two USO data
 * placeholders (D+0xF48/0xF4C and D+0xE10/0xE14) per
 * docs/PATTERNS.md uso-multi-placeholder-wrapper. The asm has a
 * double `lw a0, 0x18(sp)` at insns 24/25 (jal-delay + post-jal) —
 * scheduler quirk likely.
 *
 * 2026-05-28: the precall a1/a2 home-stores ARE reproducible via struct-by-value
 * (gl_func(a0, *(Pair2*)(&D+0xE10))) — applied, 65.84% -> 74.03%. Residual is
 * NOT the spill cap: (1) the p_B4->0xA58 RMW uses target's addiu-base+0(base)
 * write vs mine's offset write, and (2) a0 is reloaded TWICE around the 2nd jal
 * (jal-delay + after) which IDO won't emit from C here. 31 vs 32 insns. These
 * are scheduling/RMW-addressing residuals, not the (now-solved) home-store cap.
 * See docs/IDO_CODEGEN.md#feedback-ido-struct-by-value-homes-arg-pair. */
extern int gl_func_00000000();
extern char D_00000000;
void RiderAnim_startTimeout_00011024(int *a0) {
  int v;
  int *p_B4 = (int *) RA(a0)->rider;
  RA(a0)->pendingState = *((Pair2 *) (((char *) (&D_00000000)) + kPmf_RiderAnim_stateTimeUp)); /* stateTimeUp */
  ;
  p_B4 = (int *) &((SnowBoarder *) p_B4)->flags;
 goto dummy_label_965124; dummy_label_965124: ;
  *p_B4 = (*p_B4) & (~4);
  CALL(RiderAnim_enterState_0000D5BC)(a0, *((Pair2 *) (((char *) (&D_00000000)) + kPmf_RiderAnim_stateRideBlendIn)));
  CALL(RiderAnim_clearRiderEdge_00011868)(a0);
  RA(a0)->rider->slowdown = 0x64;
}


/* Sibling of RiderAnim_startTimeout_00011024 — same 32-insn dual-call orchestrator;
 * only the D-offsets differ (0xF20/0xF24 vs 0xF48/0xF4C). 2026-05-31: the
 * "precall-arg-spill + trailing-epilogue cap" was stale — struct-by-value
 * (*(Pair2*)) homes a1/a2 and the `p_B4->field_A58 &= ~4` RMW written as
 * load-via-offset + pointer-advance + store-via-0(p) (`v = p[A58]; p =
 * p+A58; *p = v & ~4;`) recovers the target's `addiu v0; sw 0(v0)` so the
 * body is now SIZE-EXACT + STRUCTURE-EXACT. Residual is a pure register
 * renumber (10 diffs): the RMW value lands in $v1 (named local) where the
 * target uses $t9, cascading the &D-E10 base $t0->$t1 and the tail $t3/$t4->
 * $t4/$t5. Both siblings share it. Needs the permuter or an allocno tweak
 * that births the loaded value in $t9; not a frame/structure problem. */
void RiderAnim_startFinish_000110A4(int *a0) {
  char *new_var;
  int *new_var2;
  int *p_B4 = (int *) RA(a0)->rider;
  int *new_var3;
  int v;
 do { } while (0);
  new_var = ((char *) (&D_00000000)) + kPmf_RiderAnim_stateFinish;
  new_var3 = a0;
  RA(a0)->pendingState = *((Pair2 *) new_var); /* stateFinish */
  ;
  new_var2 = (p_B4 = (int *) &((SnowBoarder *) p_B4)->flags);
  *p_B4 = (*new_var2) & (~4);
  CALL(RiderAnim_enterState_0000D5BC)(a0, *((Pair2 *) (((char *) (&D_00000000)) + kPmf_RiderAnim_stateRideBlendIn)));
  CALL(RiderAnim_clearRiderEdge_00011868)(a0);
  RA(new_var3)->rider->slowdown = 0x64;
}


/* BYTE-EXACT 2026-05-31. 2-call wrapper: gl_func(a0); gl_func(a0, D[0xF50],
 * D[0xF54], 1). The documented "82.18% pre-call-spill cap" was STALE — passing
 * the adjacent D[] pair AS A STRUCT BY VALUE (`*(Pair2*)(&D+0xF50)`) makes IDO
 * home a1/a2 to the outgoing arg slots (sw a1,4(sp); sw a2,8(sp)) AND keep the
 * single $t6 base, matching the target exactly (this is the struct-by-value
 * lever from docs/IDO_CODEGEN.md#feedback-ido-struct-by-value-homes-arg-pair,
 * which the stale comment's pre-2026-05-28 variants predate). .text 68/68 bytes
 * identical to expected/.o; residual fuzzy<100 is the usual uso reloc-presence. */
void RiderAnim_stateTimeUp(int *a0) {
    CALL(RiderAnim_stateStand)(a0);
    CALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateTimeUpLose), 1);
}

/* BYTE-EXACT 2026-05-31 (stale-cap catch): the struct-by-value body below was
 * already correct; the prior "~6% $v0-vs-$v1 &D-base residual" closed (game_uso
 * symbolization aligned the &D reloc) but the wrap was never removed. .text
 * 240/240 bytes identical to expected/.o.
 *
 * RiderAnim_stateFinish: 60-insn 4-way dispatcher, 0xF0 size, frame 0x18.
 *
 * Dispatch (asm 0x11168-0x11254):
 *
 *   gl_func_00000000(a0);                            // unconditional pre-call
 *   flag = a0->0xB4->0xA54;
 *   if (flag != 1) {
 *       gl_func_00000000(a0, D[0xF40+0], D[0xF40+4], 1);  // default
 *       return;
 *   }
 *   t8 = D[0x7C];
 *   if (t8 != 0) {
 *       gl_func_00000000(a0, D[0xF28+0], D[0xF28+4], 1);
 *       return;
 *   }
 *   t2 = D[0x64];
 *   if (t2 < 2) {
 *       gl_func_00000000(a0, D[0xF30+0], D[0xF30+4], 1);
 *   } else {
 *       gl_func_00000000(a0, D[0xF38+0], D[0xF38+4], 1);
 *   }
 *
 * The 4 inner calls share signature (a0, Pair2-by-value, 1): the target's
 * `sw a1,4(sp); sw a2,8(sp)` outgoing home-stores come from passing the
 * adjacent int pair AS A STRUCT BY VALUE (two separate int args do not home
 * them). See docs/IDO_CODEGEN.md struct-by-value-homes-arg-pair. */
/* PC port (plan 18.8): XDATA from the ROM TextRelocs: the +0x7C / +0x64 reads are gInputMgr fields
 * (HI16 +0x18 / LO16 +0x24 = Bss+0xF8), the Pair2 copies are game.uso Data section-base sites (+0xF28..
 * +0xF40 in the instructions); gcc folds both offsets into the token's addend. */
void RiderAnim_stateFinish(int *a0) {
    CALL(RiderAnim_stateStand)(a0);

    if (RA(a0)->rider->raceResult == 1) {
        if (*(int*)((char*)&XDATA(D_00000000, port_bootup_bss, 0xF8) + 0x7C) != 0) {
            /* struct-by-value pass homes a1,a2 to sp+4/sp+8 (the target's
             * `sw a1,4(sp); sw a2,8(sp)`) — NOT reachable from two int args.
             * This solved the prior "precall-arg-spill cap" (was 61%, now 94%). */
            CALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2*)((char*)&XDATA(D_00000000, port_game_data, 0x0) + kPmf_RiderAnim_stateGoalNormal), 1);
            return;
        }
        if (*(int*)((char*)&XDATA(D_00000000, port_bootup_bss, 0xF8) + 0x64) < 2) {
            CALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2*)((char*)&XDATA(D_00000000, port_game_data, 0x0) + kPmf_RiderAnim_stateGoalWinOM), 1);
        } else {
            CALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2*)((char*)&XDATA(D_00000000, port_game_data, 0x0) + kPmf_RiderAnim_stateGoalWin), 1);
        }
        return;
    }
    /* flag != 1: default path. Placed last so IDO emits BNEL-skip-to-here
     * with delay-load (matching asm 0x11198 bnel t7,at,+0x26). */
    CALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2*)((char*)&XDATA(D_00000000, port_game_data, 0x0) + kPmf_RiderAnim_stateGoalLose), 1);
}

/* RiderAnim_startEndingCpu_00011258: 34-insn (0x88) init+register-clear helper.
 * Structure:
 *   game_uso_func_077C44(a0, 0x70009, 0, 2, /stack: 1, 1/);     // call #1
 *   RiderAnim_queueState_0000D5F8(a0, *(D+0xF18), *(D+0xF1C), 2);      // call #2 (varargs pair home)
 *   RiderAnim_clearState_0000D5DC(a0);                                 // call #3
 *   p = a0->[0xB4]; p[0xA58] &= ~0x800;   // bit-clear (RSP/SP status mask)
 *
 * 2026-06-20: reconstructed with the REAL reloc callees (was placeholder
 * func_00000000/D_00000000, which score 100% under name-blind objdiff but are
 * NOT byte/episode-correct). RiderAnim_queueState_0000D5F8 is varargs (homes the pair
 * to 4(sp)/8(sp)); passing the D-pair BY VALUE (`Pair2`) reproduces the
 * `addiu base,base,0xF18; lw 0/4(base)` + stack-home shape with a DIRECT jal
 * (a fn-ptr cast would emit lui/addiu/jalr). The tail bit-clear emits the
 * target's `addiu base,base,0xA58; sw 0(base)` via the pointer-mutate lever
 * (`p = (char*)p + 0xA58; *p = ...`); `q = p + N` folds the offset back.
 *
 * Built is now EXACT SIZE (34 insns), all but 2 words byte-identical INCLUDING
 * relocs (was 1-insn-short + wrong-symbol placeholders). SOLE RESIDUAL = a
 * 2-register renumber in the tail RMW: target colors the loaded/masked values
 * into $t2/$t3, IDO colors them $v1/$t2 (one free-list slot lower). Confirmed
 * MATCH 2026-06-21 (byte-exact, ROM-identical). The "v1/t2 free-list coloring
 * cap" was NOT coloring-immune: the lever is the RMW-THROUGH-ADVANCED-POINTER
 * form. Instead of `raw = p[0xA58/4]; val = raw & ~M; p = p+0xA58; *p = val;`
 * (which colors the loaded value v1, masked t2), advance the pointer FIRST then
 * read+mask+write through it in a single statement:
 *   `p = (int*)((char*)p + 0xA58); *p = *p & ~0x800;`
 * This makes the load and the masked store share one address LR, and IDO colors
 * the value/result into t2/t3 (target). Found via decomp-permuter (it produced
 * the advance-first shape with a corrupted double-offset; fixing the offset to
 * the `*p = *p & ~M` RMW landed it). Callees/data resolve to absolute 0 (USO
 * runtime relocs) so the unwrapped C links and the ROM stays byte-identical. */
extern int func_00000000();
extern char game_uso_D_807FF508;
void RiderAnim_startEndingCpu_00011258(int *a0) {
    int *p;
    XCALL(game_uso_func_077C44, AnimPlayer_start)(a0, (RANIM_GROUP_EVENT | RANIM_ENDING_LOSE), 0, 2, 1, 1);
    RiderAnim_queueState_0000D5F8((char*)a0, *(Pair2*)((char*)&game_uso_D_807FF508 + kPmf_RiderAnim_stateEnding1), 2);
    RiderAnim_clearState_0000D5DC((char*)a0);
    p = (int *)&RA(a0)->rider->flags;
    *p = *p & ~0x800;
}

/* RiderAnim_startEnding_000112E0: 34-insn sibling of RiderAnim_startEndingCpu_00011258.
 * Byte-identical to 11258 except the first-call magic constant
 * 0x70009 -> 0x70008. MATCH 2026-06-21 via the same RMW-through-advanced-pointer
 * lever as the twin (see 11258 above). Byte-exact, ROM-identical. */
extern char game_uso_D_807FF508;
void RiderAnim_startEnding_000112E0(int *a0) {
    int *p;
    XCALL(game_uso_func_077C44, AnimPlayer_start)(a0, (RANIM_GROUP_EVENT | RANIM_ENDING_WIN), 0, 2, 1, 1);
    RiderAnim_queueState_0000D5F8((char*)a0, *(Pair2*)((char*)&game_uso_D_807FF508 + kPmf_RiderAnim_stateEnding1), 2);
    RiderAnim_clearState_0000D5DC((char*)a0);
    p = (int *)&RA(a0)->rider->flags;
    *p = *p & ~0x800;
}

/* MATCHED 2026-05-28: struct-by-value (F08/F0C pair). Family sibling #5 of
 * RiderAnim_restartRide_00010E2C. See
 * docs/IDO_CODEGEN.md#feedback-ido-struct-by-value-homes-arg-pair. */
void RiderAnim_stateEnding1(int *a0) {
    GCALL(AnimPlayer_startIfChanged)(a0, RA(a0)->player.anim, 2, 3, 1, 1);
    GCALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateEnding2), 3);
}

/* MATCHED 2026-05-28: struct-by-value (F10/F14 pair). Family sibling #6 of
 * RiderAnim_restartRide_00010E2C. See
 * docs/IDO_CODEGEN.md#feedback-ido-struct-by-value-homes-arg-pair. */
void RiderAnim_stateEnding2(int *a0) {
    GCALL(AnimPlayer_startIfChanged)(a0, RA(a0)->player.anim, 3, 4, 1, 1);
    GCALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateEnding3), 4);
}

void RiderAnim_stateEnding3(int *a0) {
    GCALL(AnimPlayer_startIfChanged)(a0, RA(a0)->player.anim, 4, 1, 1, 1);
}

/* RiderAnim_stateStartIntro: 39-insn — a1 = ((u)a0->0xB4->0x8C4 % 5)+10 | 0x70000;
 * then 3 USO calls with D[] arg-pairs (same family as RiderAnim_stateSwitchStance). */
void RiderAnim_stateStartIntro(int *a0) {
    int v = (int)(RA(a0)->rider->character % 5U);
    int a1 = (v + RANIM_START_INTRO) | RANIM_GROUP_EVENT;
    FCALL(AnimPlayer_start)(a0, a1, 0, 2, 1, 1);
    FCALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2 *)((char *)&D_00000000 + kPmf_RiderAnim_stateStartIntroEnd), 2);
    FCALL(RiderAnim_enterState_0000D5BC)(a0, *(Pair2 *)((char *)&D_00000000 + kPmf_RiderAnim_stateIdle));
}
/* MATCHED 2026-05-28: struct-by-value (DB8/DBC pair). Family sibling of
 * RiderAnim_restartRide_00010E2C. See
 * docs/IDO_CODEGEN.md#feedback-ido-struct-by-value-homes-arg-pair. */
void RiderAnim_stateStartIntroEnd(int *a0) {
    GCALL(AnimPlayer_startIfChanged)(a0, RA(a0)->player.anim, 2, 1, 1, 1);
    GCALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2*)((char*)&D_00000000 + kPmf_RiderAnim_stateStartWait), 1);
}

/* RiderAnim_stateStartWait (0x80): orphan-prologue MERGE of the 0x8 stub at 0x1155C
 * (lui t6,0; lw t6,0x78(t6) = load the &D+0x78 flag) + the 0x78 flag-dispatch body
 * formerly mislabeled game_uso_func_00011564. Sibling of RiderAnim_stateCushionEnd.
 * The global flag-load is IDO-hoisted above the prologue, landing at 0x1155C.
 * Single-entry: the merge is byte-exact, so it is one function (not dual-entry).
 * else-arm uses Pair2-by-value (&D+0xE40); a0->0xB4 is recomputed per-arm to match
 * the target register allocation. */
extern int gl_func_00000000();
void RiderAnim_stateStartWait(int *a0) {
    int val = 100;
    if (*(int*)((char*)&XDATA(D_00000000, port_bootup_bss, 0xF8) + 0x78) == 0) {
        RA(a0)->rider->slowdown = 0;
        CALL(RiderAnim_enterState_0000D5BC)(a0, *(Pair2*)((char*)&XDATA(D_00000000, port_game_data, 0x0) + kPmf_RiderAnim_stateRide));
    } else {
        RA(a0)->rider->slowdown = val;
        CALL(AnimPlayer_startIfChanged)(a0, RA(a0)->animSet | RANIM_UP_DOWN, 0, 1, 6, 1);
    }
}

void RiderAnim_stateIdle(void *a0) {
    RA(a0)->rider->slowdown = 100;
}

void RiderAnim_playHitAnim_000115EC(int *a0, int a1) {
    RA(a0)->selectedAnim = a1;
    GCALL(RiderAnim_startHitAnim_00011624)(a0);
    GCALL(RiderAnim_clearRiderEdge_00011868)(a0);
    RA(a0)->trickPhase = 0;
    GCALL(RiderAnim_clearState_0000D5DC)(a0);
}

/* RiderAnim_startHitAnim_00011624: 44-insn (EE84-family). MATCHED 2026-05-31
 * (70.6% -> 100%). Two levers: (1) the D-pair passes BY VALUE as *(Pair2*)
 * (homes a1,a2); (2) the call must be DUPLICATED inside each if-arm (loading
 * the pair directly from D in each branch) — selecting a pointer/value then
 * calling once builds a redundant local-copy + misses the bgtzl branch-likely.
 * (3) arms SWAPPED to `<= 0` first so IDO emits `bgtzl` (>0 case as branch
 * target, its D-pair load in the delay slot) matching the target polarity +
 * t0/t3 base-pointer order. docs/IDO_CODEGEN.md#feedback-ido-struct-by-value-homes-arg-pair */
void RiderAnim_startHitAnim_00011624(int *a0) {
    CALL(RiderAnim_clearRiderEdge_00011868)(a0);
    CALL(AnimPlayer_startIfChanged)(a0, RA(a0)->selectedAnim, 0, 1, 1, 1);
    if (RA(a0)->rider->health <= 0) {
        CALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2 *)((char *)&D_00000000 + kPmfNull_DC8), 1);
    } else {
        CALL(RiderAnim_queueState_0000D5F8)(a0, *(Pair2 *)((char *)&D_00000000 + kPmf_RiderAnim_stateRide), 1);
    }
    RA(a0)->trickPhase = 0;
}

/* RiderAnim_crashRecover_000116D4: 31-insn (0x7C) flag-gated double-call wrapper.
 * MATCHED 2026-05-31 (79.97% -> 100%): the "cross-USO varargs spills
 * (sw a1@4(sp), sw a2@8(sp))" that the old INSN_PATCH faked are exactly the
 * struct-by-value home stores — the D[0xE40]/E44 pair passes BY VALUE as
 * *(Pair2*), reproducing them HONESTLY. The "79.97% natural ceiling" was the
 * un-homed pair (same lever as EF70/F284).
 * docs/IDO_CODEGEN.md#feedback-ido-struct-by-value-homes-arg-pair */
void RiderAnim_crashRecover_000116D4(void *a0) {
    void *s0 = a0;
    CALL(RiderAnim_clearRiderEdge_00011868)(s0);
    if (RA(s0)->slowCrash != 0) {
        CALL(AnimPlayer_startIfChanged)(s0, RA(s0)->selectedAnim, 2, 1, 1, 1);
        CALL(RiderAnim_queueState_0000D5F8)(s0, *(Pair2 *)((char *)&D_00000000 + kPmf_RiderAnim_stateRide), 1);
    }
    RA(s0)->trickPhase = 0;
}

// RiderAnim_checkBackwardSwitch_00011750 — STRUCTURAL PASS (0x118 / 70 words,
// no episode). Raw-.word USO form (single function, game_uso main
// game-logic; boundary already split by commit d3e4ee03 — named fn
// still undecoded). Hand-decoded. LAST bare candidate in game_uso.c.
//
// Fall / wipeout detection + effect trigger.
//
//   void RiderAnim_checkBackwardSwitch_00011750(Obj *obj) {              // obj -> s0
//     Sub *s = obj->0xB4;
//     if (s->0xA38 < -30.0f) {                            // 0xC1F00000
//       obj->0x130 = 0;                                    // reset count
//     } else {
//       if ((s16)obj->0x130 + 1 >= 3) return;              // armed cap
//       obj->0x130++;                                       // s16 counter
//       obj->0x130 = 0;                                     // (re-clear)
//       if (*(int*)(. + 0x9CC) == 0x100) { … }
//       func_00000000(0x100, 5, obj->0xFC, 0x24);           // sound/event
//       e = D_00000EA0[0];  func_00000000(e, obj, -1,
//                                          D_00000EA0[1]);  // FX dispatch
//       func_00000000(0x100, …, obj->0xFC, …);              // 2nd event
//     }
//   }
//
// Struct-typing reference:
//   obj: 0xB4 -> s (s->0xA38 f32 downward velocity/impact metric;
//     < -30.0 = hard landing/crash), 0x130 s16 frame counter
//     (reset on hard hit, capped at 3 → arms the wipeout),
//     0xFC an id passed to the effect calls, 0x9CC a state word.
//   D_00000EA0 = USO static FX/event descriptor table; consts
//     -30.0f, 0x100 / 5 / 0x24 event op codes. func_00000000 = USO
//     placeholder dispatcher (sound / FX / event).
// Caps (DEFERRED): raw-word USO + placeholder cb's mean wrap can't
//   reach byte-match. Real-C STRUCTURAL body below — fall/wipeout
//   detection + effect trigger skeleton. Byte-match deferred. Name
//   pre-checked: no extern reuse. D_00000000 reuses file-scope
//   extern char (no redeclaration).
/* RiderAnim_checkBackwardSwitch_00011750: hard-landing/wipeout detector + FX trigger
 * (EE84 state-orchestrator family). MATCHED 2026-06-21 (call-graph DFS +
 * just-in-time family typing). The prior NM body called placeholder
 * gl_func_00000000 + loaded &D_00000000; the resolved .s relocs name the real
 * callees game_uso_func_077C44 (event/sound dispatch, R_MIPS_26 to an undefined
 * import-class symbol), RiderAnim_queueState_0000D5F8 (Pair2-by-value sink — homes
 * a1/a2 to sp+4/sp+8 via the prototype-flip lever), RiderAnim_clearState_0000D5DC
 * (pair-copy trailing call), and the real global game_uso_D_807FF490. Control
 * flow: the FP gate is the POSITIVE if (`if (s->0xA38 < -30.0f)` => the target
 * `c.lt.s; bc1fl` with the else-reset folded into the branch-likely delay slot);
 * the counter advance is UNCONDITIONAL (`obj->0x130 = cnt+1` always, before the
 * `cnt < 3` return — matches the bne-likely delay-slot store). FINAL lever to 0:
 * INLINE the `obj->0xB4` base derefs (drop the held `char *s` local) at BOTH the
 * FP read and the 0x9CC read — a held base local colored into $v0 and shifted
 * every later temp down one register (off-by-one renumber, 19 diffs); the inline
 * form lets IDO float the base into $t8 exactly like the target (19->0).
 * Byte-exact (0 non-reloc word diffs, reloc symbols agree). */
extern char game_uso_D_807FF490;
void RiderAnim_checkBackwardSwitch_00011750(char *obj) {
    short cnt;
    if (RA(obj)->rider->fwdSpeed < -30.0f) {   /* hard hit */
        cnt = RA(obj)->backHits;
        RA(obj)->backHits = cnt + 1;        /* always advance (delay slot) */
        if (cnt < 3) return;                      /* not yet armed */
        RA(obj)->backHits = 0;
        if (RA(obj)->rider->useDParms != 0) {
            XCALL(game_uso_func_077C44, AnimPlayer_start)(obj, RA(obj)->animSet | RANIM_DEEP_SWITCH, 0, 0, ANIM_BLENDIN, 5);
            RiderAnim_queueState_0000D5F8(obj, *(Pair2 *)((char *)&game_uso_D_807FF490 + kPmf_RiderAnim_stateSwitchStance), -1);
        } else {
            XCALL(game_uso_func_077C44, AnimPlayer_start)(obj, RA(obj)->animSet | RANIM_SWITCH_STANCE, 0, 0, ANIM_BLENDIN, 5);
            RiderAnim_queueState_0000D5F8(obj, *(Pair2 *)((char *)&game_uso_D_807FF490 + kPmf_RiderAnim_stateSwitchStance), -1);
        }
        RiderAnim_clearState_0000D5DC(obj);
    } else {
        RA(obj)->backHits = 0;              /* not a hard hit: reset */
    }
}

void RiderAnim_clearRiderEdge_00011868(int *a0) {
    RA(a0)->rider->edge = 0.0f;
    RA(a0)->rider->edgeSteer = 0.0f;
    RA(a0)->rider->edgeSlope = 0.0f;
}

void RiderAnim_clearTricks_00011888(int *a0) {
    RA(a0)->trickButtons = 0;
    RA(a0)->trickInputFrames = 0;
    RA(a0)->queuedTrick = 0;
    RA(a0)->spinKind = 0;
}

/* RiderAnim_hitFeedback_0001189C: hit kind a1 = 1..5 (a jump table at D+0x260): a pad
 * record on the rider's input (pad_set(rider->input, &D+off_k, 10), off_k =
 * {0x109C, 0x10A4, 0x10AC, 0x10C0, 0x10DC}) and a sound event on the rider's SndHandle
 * (rider + 0x808) with a2. Labels from the ROM table (port/tools/jumptable-audit.py, run 831):
 * 2 is the 0x10A4 arm, 3 the 0x10AC arm (the kRumble_hit2/3 names predate that). */
void RiderAnim_hitFeedback_0001189C(int *a0, int a1, int a2) {
    int *s0 = a0;
    int s1 = a2;
    switch (a1) {
    case 1:
        FCALL(pad_setRumble)((int)RA(s0)->rider->input, (char *)&D_00000000 + kRumble_hit1_109C, 10);
        FCALL(SndHandle_triggerEvent3_00000940)((int)RA(s0)->rider->sound, s1);
        break;
    case 3:
        FCALL(pad_setRumble)((int)RA(s0)->rider->input, (char *)&D_00000000 + kRumble_hit2_10AC, 10);
        FCALL(SndHandle_triggerEvent0_000008F4)((int)RA(s0)->rider->sound, s1);
        break;
    case 2:
        FCALL(pad_setRumble)((int)RA(s0)->rider->input, (char *)&D_00000000 + kRumble_hit3_10A4, 10);
        FCALL(SndHandle_triggerEvent0_000008F4)((int)RA(s0)->rider->sound, s1);
        break;
    case 4:
        FCALL(pad_setRumble)((int)RA(s0)->rider->input, (char *)&D_00000000 + kRumble_hit4_10C0, 10);
        FCALL(SndHandle_triggerEvent1_0000098C)((int)RA(s0)->rider->sound, s1);
        break;
    case 5:
        FCALL(pad_setRumble)((int)RA(s0)->rider->input, (char *)&D_00000000 + kRumble_hit5_10DC, 10);
        FCALL(SndHandle_triggerEvent2_000009D8)((int)RA(s0)->rider->sound, s1);
        break;
    }
}

void game_readFloatParm_000119D4(char *dst) {
    int tmp;
    stream_readS32_0000C194(&tmp);
    stream_readF32_0000C1D0((float*)(dst + 0x10));
}

void game_readVec3Parm_00011A04(char *dst) {
    int tmp;
    stream_readS32_0000C194(&tmp);
    stream_readVec3_0000C20C((Vec3*)(dst + 0x10));
}

void game_readQuadParm_00011A34(char *dst) {
    int tmp;
    stream_readS32_0000C194(&tmp);
    stream_read16_0000C27C((Quad4*)(dst + 0x10));
}

void game_readIntParm_00011A64(char *dst) {
    int tmp;
    stream_readS32_0000C194(&tmp);
    stream_readS32_0000C194((int*)(dst + 0x10));
}

/* game_ItemType_create_00011A94: item factory of an ItemType (arg0): a 0x40-byte ColourItem
 * (mem_new + ColourItem_ctor, vptr import_8006ED80, +0x3C = 0); when the type's unk40
 * node is set, it becomes the item's child (list_append on children + the parent link).
 *
 * 2026-06-21 CRACKED via masked-twin port from edd_ItemType_create_000003BC
 * (donor, byte-exact); identical to the 3A28/AC78 lands. The prior body
 * used placeholder gl_func/&D + the INSN_PATCH-era shape. The donor's
 * p2/head/p1 split + p1-reuse-for-arg0 + decl-order + condition re-load
 * forces the two-web spill and a0/v1 coloring. */
extern char import_8006ED80;
void *game_ItemType_create_00011A94(int *arg0) {
    int *p2;
    int *head;
    int *p1;
    p1 = (int*)XCALL(game_uso_func_055750, mem_new)(sizeof(ColourSubItem));
    if (p1 != 0) {
        XCALL(game_uso_func_051C28, ColourItem_ctor)(p1);
        NODE(p1)->vptr = (char *)&import_8006ED80;
        ((ColourSubItem *)p1)->unk3C = 0;
    }
    p2 = p1;
    p1 = arg0;
    head = (int*)ITEMTYPE(p1)->unk40;
    if ((int*)ITEMTYPE(p1)->unk40 != 0) {
        XCALL(game_uso_func_07ACE0, list_append)(&NODE(p2)->children, head);
        if (NODE(head)->parent != 0) {
            NODE(head)->unk04 = 1;
        }
        NODE(head)->parent = (Node *)p2;
    }
    return p2;
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_uso/game_uso/game_uso_func_00011A94_pad.s")

#ifdef PORT
/* PORT trace collector (plan 9.6.4, 17.7): the strong port_trace_collect that replaces the weak
 * default in port/trace.c. It lives in game.uso because the race objects it reads (SnowBoarder,
 * RiderAnim, the cameras) are game.uso's. Everything is reached from GameFlow (InputMgr, bootup
 * Bss 0xF8, through the port/uso blob):
 *   flow     GameFlow.procState (+0x40) / nextFlowMode (+0x34)
 *   rand     the sys_randf state word (bootup Data 0x21E04)
 *   rt       RcpMgr.retraceCount (virtual: 2 per frame)
 *   el       GameFlow.dataMgr->elapsed (RaceTracker / DataManager, ms)
 *   race     dataMgr->owner->raceState / flags (RaceProcess)
 *   e0       dataMgr->entries[0]: pct (100 at the goal), speed (the HUD km/h), finished, health
 *   r<i>     dataMgr->entries[i].rider (a SnowBoarder): dnode.vqs.trans, dnode.vqs.quat,
 *            dnode.speed, and the RiderAnim state method (anim->state.b, the PMF's function word;
 *            a host address on the port, a ROM address in an emulator trace: map both to names
 *            before diffing)
 *   cam      the eye (MCamera mnode.world[3]) and target (target->world[3], or eye + lookDir * 10
 *            with no target, as the lookAt does) of the camera in GameFlow.game's windows[0]
 *            (WorldRegistry, game/course.h).
 * GameFlow.dataMgr and .game are never cleared when a race ends, so the race groups are read only
 * while game.uso is loaded (port/uso registry); between races they would point at freed heap. */
#include "gdl/race.h"
#include "port_game.h"
#include "../../port/trace.h"
#include "../../port/uso/port_uso.h"

static int port_trace_raceLive(void) {
    static int sGameUsoId = -2;
    if (sGameUsoId == -2) {
        const PortUsoModule *m = port_uso_find("game.uso");
        sGameUsoId = m != NULL ? m->id : -1;
    }
    return sGameUsoId >= 0 && port_uso_isLoaded(sGameUsoId);
}

/* The title demo's dataMgr leaves owner (and other links) at -1 / small sentinels: read only
 * links that look like host pointers. */
#define PORT_TRACE_PTR(p) ((unsigned long)(p) >= 0x10000UL && (unsigned long)(p) < 0xFFFF0000UL)

void port_trace_collect(PortTraceFrame *f) {
    InputMgr *flow = (InputMgr *)PORT_GAMEFLOW;
    RaceTracker *dm;
    char *game;
    s32 i;

    f->flowState = flow->procState;
    f->flowNext = flow->nextFlowMode;
    f->flowMode = flow->flowMode;
    f->randState = *(u32 *)PORT_RANDSTATE;
    f->retraceCount = port_game_retrace_count();
    f->have |= PORT_TRACE_HAVE_FLOW | PORT_TRACE_HAVE_RAND | PORT_TRACE_HAVE_RETRACE;
    if (!port_trace_raceLive()) {
        return;
    }
    dm = flow->dataMgr;
    if (PORT_TRACE_PTR(dm)) {
        f->elapsed = dm->elapsed;
        f->have |= PORT_TRACE_HAVE_ELAPSED;
        if (PORT_TRACE_PTR(dm->owner)) {
            f->raceState = dm->owner->raceState;
            f->raceFlags = (u32)dm->owner->flags;
            f->have |= PORT_TRACE_HAVE_RACE;
        }
        if (PORT_TRACE_PTR(dm->entries) && dm->numEntries > 0) {
            f->entryPct = dm->entries[0].pct;
            f->entrySpeed = dm->entries[0].speed;
            f->entryFinished = dm->entries[0].finished;
            f->entryHealth = dm->entries[0].health;
            f->have |= PORT_TRACE_HAVE_ENTRY;
        }
        if (PORT_TRACE_PTR(dm->entries)) {
            for (i = 0; i < dm->numEntries && f->numRiders < PORT_TRACE_MAX_RIDERS; i++) {
                SnowBoarder *r = (SnowBoarder *)dm->entries[i].rider;
                PortTraceRider *o = &f->riders[f->numRiders];
                if (!PORT_TRACE_PTR(r)) {
                    continue;
                }
                o->pos[0] = r->dnode.vqs.trans[0];
                o->pos[1] = r->dnode.vqs.trans[1];
                o->pos[2] = r->dnode.vqs.trans[2];
                o->quat[0] = r->dnode.vqs.quat[0];
                o->quat[1] = r->dnode.vqs.quat[1];
                o->quat[2] = r->dnode.vqs.quat[2];
                o->quat[3] = r->dnode.vqs.quat[3];
                o->speed = r->dnode.speed;
                o->animState = PORT_TRACE_PTR(r->anim) ? r->anim->state.b : -1;
                f->numRiders++;
            }
            f->have |= PORT_TRACE_HAVE_RIDERS;
        }
    }
    game = flow->game;
    if (PORT_TRACE_PTR(game)) {
        CamWindow *w = (CamWindow *)WORLDREG(game)->windows[0];   /* the race camera window */
        MCamera *cam = PORT_TRACE_PTR(w) ? w->cam : NULL;
        if (PORT_TRACE_PTR(cam)) {
            for (i = 0; i < 3; i++) {
                f->camEye[i] = cam->mnode.world[3][i];
            }
            if (PORT_TRACE_PTR(cam->target)) {
                for (i = 0; i < 3; i++) {
                    f->camTarget[i] = cam->target->world[3][i];
                }
            } else {
                f->camTarget[0] = f->camEye[0] + cam->lookDir.x * 10.0f;
                f->camTarget[1] = f->camEye[1] + cam->lookDir.y * 10.0f;
                f->camTarget[2] = f->camEye[2] + cam->lookDir.z * 10.0f;
            }
            f->have |= PORT_TRACE_HAVE_CAMERA;
        }
    }
}
#endif
