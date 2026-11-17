#include "common.h"
#include "gdl/gl_data.h"
#include "gdl/gfxlib.h"
#include "gdl/window.h"
#include "game/snowboarder.h"
#include "game/camera.h"
#include "game/course.h"
#include "audio.h"
#include "game/input.h"
#include "gdl/node.h"
#include "gdl/item.h"
#include "gdl/light.h"
#include "gdl/polyshape.h"
#include "gdl/material.h"
#include "game/envobj.h"
#include "uso_call.h"
/* bootup.uso RoData section base (USO symIdx 1): every FP literal load is
 * `lui at,0; lwc1 OFF(at)` with OFF = the RoData pool offset, so a DATA
 * symbol pinned at 0 indexed by OFF gives the lui-at/lwc1 shape (the old
 * `(char *)&SndHandle_triggerEvent3_00000940 + OFF` cast materialized the function address). */
extern float D_bu_ro_pool[];
extern double D_bu_ro_dpool[];  /* same base, f64 view (8-aligned pool offsets) */
extern f32 kRo_SnowBoarder_ctor_868, kRo_SnowBoarder_ctor_86C, kRo_SnowBoarder_ctor_870, kRo_SnowBoarder_ctor_874, kRo_SnowBoarder_ctor_878, kRo_SnowBoarder_ctor_87C, kRo_SnowBoarder_ctor_880, kRo_SnowBoarder_ctor_884, kRo_SnowBoarder_ctor_888, kRo_SnowBoarder_ctor_88C, kRo_SnowBoarder_ctor_890, kRo_SnowBoarder_ctor_894, kRo_SnowBoarder_ctor_898, kRo_SnowBoarder_ctor_89C, kRo_SnowBoarder_ctor_8A0, kRo_SnowBoarder_ctor_8A4, kRo_SnowBoarder_ctor_8A8, kRo_SnowBoarder_ctor_8AC, kRo_SnowBoarder_ctor_8B0, kRo_SnowBoarder_ctor_8B4, kRo_SnowBoarder_procColl_938, kRo_SnowBoarder_procColl_93C;

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


void stream_readS32_00000008(int *dst) {
    int buf[2];
    FCALL(fs_gdlReadFile)(&gSysLib, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

void stream_readF32_00000044(float *dst) {
    float buf[2];
    FCALL(fs_gdlReadFile)(&gSysLib, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

typedef struct { int a, b, c; } Tri3i;
typedef struct { float x, y, z; } Vec3;
void stream_readVec3_00000080(Vec3 *dst) {
    int pad_top[1];
    Tri3i raw;
    int pad_mid[2];
    Tri3i tmp;
    int pad_bot[2];
    FCALL(fs_gdlReadFile)(&gSysLib, &raw, 12);
#ifdef PORT
    PORT_BE_SWAP32S(&raw, 12);
#endif
    tmp = raw;
    dst->x = *(float*)&tmp.a;
    dst->y = *(float*)&tmp.b;
    dst->z = *(float*)&tmp.c;
}

typedef struct { int a, b, c, d; } Quad4;
void stream_readQuad_000000F0(Quad4 *dst) {
    Quad4 buf;
    FCALL(fs_gdlReadFile)(&gSysLib, &buf, 16);
#ifdef PORT
    PORT_BE_SWAP32S(&buf, 16);
#endif
    *dst = buf;
}

extern char sStr_Poop_start_6538;
void snd_seqEventDbg_00000148(int a0, int a1, int a2) {
    FCALL(dbg_printf)(&sStr_Poop_start_6538);
    FCALL(snd_HandleSeqEvent_30AF4)(a0, a1, a2);
}

extern char sStr_Poop_end_6544;
void snd_seqEndDbg_00000188(int a0, int a1, int a2) {
    FCALL(dbg_printf)(&sStr_Poop_end_6544);
    FCALL(snd_HandleSeqEnd)(a0, a1, a2);
}

void snd_setLevelRampTarget(int a0) {
    if (a0 != 0) {
        FCALL(snd_SetLevelRampTarget)(2);
    } else {
        FCALL(snd_SetLevelRampTarget)(0);
    }
}

void snd_setSoundOutputMode_000001FC(void) {
    FCALL(snd_SetSoundOutputMode)();
}

void snd_setPlayer1VolumeLevel(void) {
    FCALL(snd_SetPlayer1VolumeLevel)();
}

void snd_setPlayer0VolumeLevel_0000023C(void) {
    FCALL(snd_SetPlayer0VolumeLevel)();
}

void snd_setPlayer2VolumeLevel(void) {
    FCALL(snd_SetPlayer2VolumeLevel)();
}

void snd_startPlayer2Seq_0000027C(void) {
    FCALL(snd_StartPlayer2Seq)();
}

void snd_muteAll_0000029C(void) {
    FCALL(snd_MuteAll)();
}

void snd_unmuteAll(void) {
    FCALL(snd_UnmuteAll)();
}

void snd_postEvent(void) {
    FCALL(snd_PostEvent)();
}

void snd_sendEvent(void) {
    FCALL(snd_dispatchEvent)();
}

void snd_updateChannel10Emitter(int a0, int a1, int a2) {
    int local1, local2;
    FCALL(snd_calcEmitterDistPan_000003F8)(a1, &local1, &local2, a2);
    FCALL(snd_UpdateChannel10Emitter)(a0, local1, (signed char)local2);
}

extern char sStr_Poop_Added_sound_heap_6550;
void snd_addSoundHeap(void) {
    FCALL(snd_AddSoundHeap)();
    FCALL(dbg_printf)(&sStr_Poop_Added_sound_heap_6550);
}

extern char sStr_Poop_Removed_sound_heap_6568;
void snd_removeSoundHeap(void) {
    FCALL(snd_RemoveSoundHeap)();
    FCALL(dbg_printf)(&sStr_Poop_Removed_sound_heap_6568);
}

void snd_setPlayer1Channel15IO(void) {
    FCALL(snd_SetPlayer1Channel15IO_2D910)();
}

void snd_setPlayer3FreqScaleFromStick(void) {
    FCALL(snd_SetPlayer3FreqScaleFromStick)();
}

// snd_calcEmitterDistPan_000003F8(Vec3 *world, float *outDist, int *outProj, Obj *obj):
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
//   *outDist = *outDist / K;                      // K = stream_readF32_00000044+0xC literal
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
 * the f32 at stream_readF32_00000044+0xC (D_00000050). */
typedef struct { f32 x, y, z; } V3_3F8;
extern f32 func_00000000_3F8_f(f32);
/* UNNAMED: data-base D_00000050 = 0x50 is RoData +0x50 here (snd_calcEmitterDistPan_000003F8 distance scale) but Bss sym272 +0x50 in game_libs app_createGame_00008FFC; a pure rename would merge them, it needs a per-site split. */
extern f32 D_00000050;
void snd_calcEmitterDistPan_000003F8(V3_3F8 *pos, f32 *dist, s32 *proj, char *obj) {
    V3_3F8 d;
    f32 n[3];
    f32 q0, q1, q2, q3;
    f32 inv;
    f32 t;

    d = *pos;
    d.x = d.x - ((CamWindow *)obj)->cam->mnode.world[3][0];
    d.y = d.y - ((CamWindow *)obj)->cam->mnode.world[3][1];
    d.z = d.z - ((CamWindow *)obj)->cam->mnode.world[3][2];
    *dist = XCALL(func_00000000_3F8_f, math_vec3Length_0005BBCC)(d.x * d.x + d.y * d.y + d.z * d.z);
    inv = 1.0 / *dist;
    d.x *= inv;
    d.y *= inv;
    d.z *= inv;
    n[0] = ((CamWindow *)obj)->cam->view[0][0];
    n[1] = ((CamWindow *)obj)->cam->view[1][0];
    n[2] = ((CamWindow *)obj)->cam->view[2][0];
    *proj = (s32)((d.z * n[2] + (d.y * n[1] + n[0] * (t = d.x))) * 63.0f);
    *dist = *dist / D_00000050;
    if (*dist > 1.0) {
        *dist = 1.0f;
    }
}

/* snd_soundObjCreate_0000057C: alloc obj = func(0x64) into arg0->0x0; switch(arg1) selects
 * 0/1/2; init obj->0x22=(s8)arg2, obj->0x10=0.0f, arg0->0x8=0; then call obj's
 * handler with the selector. Byte-exact (incl. the switch branch-likely + dead
 * store); func_00000000 = cross-segment placeholder (jal 0). Key: `sel=0` must
 * be set AFTER the alloc call so it stays in $a2 (before-the-call spills it). */
void snd_soundObjCreate_0000057C(int *arg0, int arg1, int arg2) {
    int sel;
    arg0[0] = (int)FCALL(mem_new)(0x64);
    sel = 0;
    switch (arg1) {
    case 0: break;
    case 1: sel = 1; break;
    case 2: sel = 2; break;
    }
    /* arg0[0] is a SoundObj (audio.h, 0x64: snd_SoundObjInit's object) */
    ((SoundObj *)arg0[0])->surface = (char)arg2;
    ((SoundObj *)arg0[0])->unk10 = 0.0f;
    arg0[2] = 0;
    FCALL(snd_SoundObjInit)(arg0[0], sel);
}

/* snd_updatePlayer - corrected full decode (0x24C, 147 insns, FP param
 * normalize / clamp / scale + 5 func_00000000 dispatch calls).
 * Signature recovered from arg-home slots: a0=int* (struct-ptr ptr),
 * then 5 f32 (arg1..arg5) + 3 s32 (arg6..arg8). o = arg0[0]; bails if 0.
 *   - o->0xC  = clamp01( (f32)((f64)arg5 / *(f64*)(&stream_readF32_00000044+0x14)) )
 *   - o->0x4  = (f32)((f64)fabsf(arg4) + *(f64*)(&stream_readF32_00000044+0x1C))
 *   - o->0x18 = (s8)(s32)(arg4 * 63.0f)            // 0x427C0000
 *   - o->0x1E = (arg8 & 0x80)   != 0
 *   - switch(arg6){0,'a','b','c','d'} -> o->0x15 / o->0x14 tags
 *   - o->0x0  = arg2 / 4.0f ;  o->0x8 = arg3 / 4.0f
 *   - o->0x20 = 1 ;  o->0x1C = (arg8 & 0x8000) != 0
 *   - if(o->0x1C) { func_00000000((int)arg1bits,&sp28,&sp24,arg7);
 *                   func_00000000(arg0[0],*(int*)&sp28,(s8)sp24); }
 *   - func_00000000(&sStr_NA_UpdPl_6584); func_00000000(arg0[0],0);
 *     func_00000000();
 * The &stream_readF32_00000044+off ldc1 doubles are f64 consts embedded in
 * stream_readF32_00000044's literal pool (same idiom as CamDirector_update line ~1794,
 * which emits the matching R_MIPS_HI16/LO16 stream_readF32_00000044 relocs).
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
extern char sStr_NA_UpdPl_6584;
/* Run 109 (agent-c, 93.18 -> 99.75): per-address pool doubles; arg5 is
 * reassigned with the scaled value (target keeps it in f2); ONE float local
 * carries both the clamp and the fabs results (f0); `/ (f32)4` keeps the
 * div.s (a 4.0f literal folds to * 0.25). Residual: frame 0x38 vs 0x30
 * (sp28/sp24 sit 8 bytes higher; pads only grow it). */
extern f64 D_bu_ro_d058, D_bu_ro_d060;  /* RoData pool doubles at 0x58 / 0x60 (stream_readF32_00000044 + 0x14 / 0x1C); one symbol each, the target re-luis per load */
void snd_updatePlayer(SndHandle *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    SoundObj *o;
    f32 sp28;
    int sp24;
    f32 var_f0;

    o = arg0->obj;
    if (o != 0) {
        arg5 = (f32) ((f64) arg5 / D_bu_ro_d058);
        if (arg5 < 0.0f) {
            var_f0 = 0.0f;
        } else if (1.0f < arg5) {
            var_f0 = 1.0f;
        } else {
            var_f0 = arg5;
        }
        o->unk0C = var_f0;
        if (arg4 < 0.0f) {
            var_f0 = -arg4;
        } else {
            var_f0 = arg4;
        }
        arg0->obj->unk04 = (f32) ((f64) var_f0 + D_bu_ro_d060);
        arg0->obj->unk18 = (s8) (s32) (arg4 * 63.0f);
        arg0->obj->unk1E = (s8) ((arg8 & 0x80) != 0);
        switch (arg6) {
        case 0x0:
            arg0->obj->unk15 = 1;
            arg0->obj->unk14 = 0;
            break;
        case 0x61:
            arg0->obj->unk15 = 1;
            arg0->obj->unk14 = 1;
            break;
        case 0x62:
            arg0->obj->unk15 = 3;
            arg0->obj->unk14 = 1;
            break;
        case 0x63:
            arg0->obj->unk15 = 2;
            arg0->obj->unk14 = 1;
            break;
        case 0x64:
            arg0->obj->unk15 = 2;
            arg0->obj->unk14 = 2;
            break;
        }
        arg0->obj->speed = arg2 / (f32)4;
        arg0->obj->unk08 = arg3 / (f32)4;
        arg0->obj->fired = 1;
        arg0->obj->flags1C = (u8) ((arg8 & 0x8000) != 0);
        if (arg0->obj->flags1C != 0) {
            func_00000000(*(int*)&arg1, &sp28, &sp24, arg7);
            func_00000000(arg0->obj, *(int*)&sp28, (s8) sp24);
        }
        func_00000000(&sStr_NA_UpdPl_6584);
        func_00000000(arg0->obj, 0);
        func_00000000();
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/bootup_uso", snd_updatePlayer);
#endif

void SndHandle_start(SndHandle *h) {
    SoundObj *o = h->obj;
    if (o != 0) {
        FCALL(snd_SoundObjStart)(o);
    }
}

void SndHandle_stop(SndHandle *h) {
    SoundObj *o = h->obj;
    if (o != 0) {
        FCALL(snd_SoundObjStop)(o);
    }
}

void SndHandle_setScaleIdx15(SndHandle *h) {
    FCALL(snd_SoundObjSetScaleIdx15)(h->obj);
}

void SndHandle_setScaleIdx31(SndHandle *h) {
    FCALL(snd_SoundObjSetScaleIdx31)(h->obj);
}

void SndHandle_triggerEvent0_000008F4(SndHandle *h, int a1_orig) {
    int ret = FCALL(SndHandle_contactEventType)(h, h->contactSurface);
    SoundObj *node = h->obj;
    if (node != 0) {
        FCALL(snd_SoundObjTriggerEvent)(node, ret, 0, a1_orig);
    }
}

void SndHandle_triggerEvent3_00000940(SndHandle *h, int a1_orig) {
    int ret = FCALL(SndHandle_contactEventType)(h, h->contactSurface);
    SoundObj *node = h->obj;
    if (node != 0) {
        FCALL(snd_SoundObjTriggerEvent)(node, ret, 3, a1_orig);
    }
}

void SndHandle_triggerEvent1_0000098C(SndHandle *h, int a1_orig) {
    int ret = FCALL(SndHandle_contactEventType)(h, h->contactSurface);
    SoundObj *node = h->obj;
    if (node != 0) {
        FCALL(snd_SoundObjTriggerEvent)(node, ret, 1, a1_orig);
    }
}

void SndHandle_triggerEvent2_000009D8(SndHandle *h, int a1_orig) {
    int ret = FCALL(SndHandle_contactEventType)(h, h->contactSurface);
    SoundObj *node = h->obj;
    if (node != 0) {
        FCALL(snd_SoundObjTriggerEvent)(node, ret, 2, a1_orig);
    }
}

void SndHandle_disable(SndHandle *h) {
    SoundObj *o = h->obj;
    if (o != 0) {
        FCALL(snd_SoundObjDisable)(o);
    }
}

void SndHandle_setUnk1F_00000A50(SndHandle *h, int a1) {
    SoundObj *v0 = h->obj;
    if (v0 != 0) {
        v0->unk1F = (char)a1;
    }
}

void SndHandle_setFlag1A(SndHandle *h) {
    SoundObj *o = h->obj;
    if (o != 0) {
        FCALL(snd_SoundObjSetFlag1A)(o);
    }
}

void SndHandle_setContactSurface(SndHandle *h, int a1) {
    h->contactSurface = a1;
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
/* SndHandle_contactEventType — best NM body (28/30 words, 97.83%). CAP CONFIRMED 2026-07-02
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
int SndHandle_contactEventType(int a0, int a1) {
    volatile int r = 0;
    return r;
}

void snd_soundObjSetReverb(int **a0, int a1) {
    int *v0 = *a0;
    if (v0 != 0) {
        ((char*)v0)[0x1D] = (char)a1;
    }
}

/* UNNAMED: data-base D_00000068 = 0x68 is RoData +0x68 in bootup snd_soundObjSetPan_00000B2C but InputMgr +0x68 (import mod0) in the arcproc / timproc b1 / b3 openSession helpers; a pure rename would merge them, it needs a per-site split. */
extern float D_00000068;
void snd_soundObjSetPan_00000B2C(void **a0, float a1) {
    a1 = a1 / D_00000068;
    if (*a0 != 0) {
        ((SoundObj *)*a0)->unk10 = a1;

    }
}

void boot_readFloatParm_00000B50(char *a0) {
    int scratch;
    stream_readS32_00000008(&scratch);
    stream_readF32_00000044((float*)(a0 + 0x10));
}

void boot_readVec3Parm_00000B80(char *a0) {
    int scratch;
    stream_readS32_00000008(&scratch);
    stream_readVec3_00000080(a0 + 0x10);
}

void boot_readQuadParm_00000BB0(char *a0) {
    int scratch;
    stream_readS32_00000008(&scratch);
    stream_readQuad_000000F0(a0 + 0x10);
}

void boot_readIntParm_00000BE0(char *a0) {
    int scratch;
    stream_readS32_00000008(&scratch);
    stream_readS32_00000008((int*)(a0 + 0x10));
}

/* ColourItem_create_00000C10: mirror of boot_ItemType_create_000046EC / boot_ItemType_create_00007B08 / boot_ItemType_create_0000E690.
 * BYTE-EXACT twin of donor edd_ItemType_create_000003BC, via the donor's clean C
 * form (p2/head/p1 decl order + char*-cast field access + p1[0x40/4] re-read).
 * Was once force-matched via 22-word INSN_PATCH (banned 2026-05-23, recipe
 * deprecated); this is the genuine C match. */
extern int func_00000000();
void *ColourItem_create_00000C10(int *arg0) {
    int *p2;
    int *head;
    int *p1;
    p1 = (int*)FCALL(mem_new)(0x40);
    if (p1 != 0) {
        FCALL(ColourItem_ctor)(p1);
        NODE(p1)->vptr = (char *)&ColourItem_vtbl_75A0;
        ((ColourSubItem *)p1)->unk3C = 0;
    }
    p2 = p1;
    p1 = arg0;
    head = (int*)p1[0x40 / 4];
    if ((int*)p1[0x40 / 4] != 0) {
        FCALL(list_append)((char*)p2 + 0x10, head);
        if (NODE(head)->parent != 0) {
            NODE(head)->unk04 = 1;
        }
        NODE(head)->parent = (Node *)p2;
    }
    return p2;
}

/* BGBox_ctor - verified structural decode (0xF4, 61 insns,
 * get-or-create constructor + box-corner table generator).
 *   void *BGBox_ctor(void *a0, int a1, int a2, int a3) {
 *       o = a0;
 *       if (o == 0) {
 *           o = alloc(0x40);                      // func_00000000
 *           if (o == 0) return 0;
 *           init(o, &sStr_Node_66A8);                 // func_00000000
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
 * slots (&D and func_00000000+4). sStr_Node_66A8 = init datum.
 * 2026-06-20 reconstruction 61->79%: (1) control flow rewritten to
 * `if (a0 || (o=alloc())) { body }` + shared-epilogue `return o`
 * (was a separate `return 0` path); (2) added the missing
 * `*(func_00000000+4)=a3` store; (3) `a3/2` (IDO signed-div emit)
 * puts the half-extent in $a0 matching target (manual >>1 ternary used
 * $v0). RESIDUAL caps: descriptor-symbol reg, prologue move/sw-a2
 * scheduling, and the 2 single-int global stores want $at-fused form
 * but IDO holds D_00000000 in $v1 (shared with the 12-entry box table).
 * Stays INCLUDE_ASM (no episode). */
/* BGBox_ctor: EXACT via an IDO 7.1 NCC (C++) donor,
 * src/bootup_uso/bootup_uso_ncc_CA0.cpp, REPLACE_FUNC_BODY-spliced over this
 * placeholder (agent-g run 150). It is the real constructor D::D(a1, a2, a3):
 * edg's this==0 -> new(0x40) protocol, out-of-line base ctor B(&sStr_Node_66A8)
 * (import), D's vptr at +0x28 (the stores now schedule as in the target), and
 * the last vertex corner written one store per source line (as1 reorders only
 * inside a .loc line group; the cfe body colored h/-h v0/a0, 97.2). */
void BGBox_ctor(int a0, int a1, int a2, int a3) {
    volatile int r = 0;
}

/* BGBox_display - verified structural decode (0xD4, 53 insns,
 * FP-threshold-gated state update).
 * Struct-typing reference: g = &D_00000000 global; g->0x2A0 (672) f32
 * value compared (promoted to f64) against the f64 constant at
 * stream_readF32_00000044+0x2C; g->0x254 (596) ptr to a state object whose
 * +0x78 (120) is a flags word (bit 2 / mask 0x4 = an enable, set or
 * cleared by a0->0x3C in the below-threshold path, always set in the
 * at/above path) and +0xDC..+0xE8 (220..232) a 4x int param block
 * filled from snd_startPlayer2Seq_0000027C+0x18; a0->0x3C (60) selector.
 * Caps <80: FP cvt.d.s + c.lt.d double-promote + ldc1 f64 const +
 * bc1f branch + 2x reloc + 2x cross-symbol data ref.
 * INCLUDE_ASM remains build path. */
extern void stream_readF32_00000044();  /* used as data-symbol base */
extern void snd_startPlayer2Seq_0000027C();  /* used as data-symbol base */
extern int sStr_cam_7324;
/* EXACT (agent-c run 115). Globals block import_0005C108 (alias gGfxLib_d94_g),
 * threshold = RoData f64 at +0x70 (stream_readF32_00000044 + 0x2C), record at 0x294
 * (snd_startPlayer2Seq_0000027C + 0x18, alias gGfxLib_d94_src); callees func_05B750(g, rec) and
 * func_04CCA8(a0). The a0->0x3C test goes through a named int (target v1),
 * the flag word through a pointer, and the 16-byte record is a struct copy. */
/* GfxLib +0x294 is the scene FogParms (FogParms_emit(&gGfxLib_00000294), FogParms_lerp in Fog_display;
 * gfxlib.h still calls it unk294[6]); colour[3] is the fog alpha. BGBox draws the box while the fog
 * is (nearly) transparent and otherwise clears the window with the fog colour (Window.flags bit 2). */
#define FOGPARMS(p) ((FogParms *)(p))
typedef struct { int w[4]; } S16_D94;
extern char gGfxLib_d94_g[], gGfxLib_d94_src[];
void BGBox_display(char *a0) {
    int *p;
    int on;
    if ((double)GFXLIB(gGfxLib_d94_g)->fog.colour[3] < D_bu_ro_dpool[0x70 / 8]) {
        FCALL(gfx_setPrimColour)(gGfxLib_d94_g, gGfxLib_d94_src);
        FCALL(Node_display)(a0);
        p = &GFXLIB(gGfxLib_d94_g)->curWindow->win.flags;
        on = ((BGBox *)a0)->clearWindow;
        if (on != 0) {
            *p |= 4;
        } else {
            *p &= ~4;
        }
    } else {
        p = &GFXLIB(gGfxLib_d94_g)->curWindow->win.flags;
        *p |= 4;
        *(S16_D94 *)GFXLIB(gGfxLib_d94_g)->curWindow->win.clearColour = *(S16_D94 *)gGfxLib_d94_src;
    }
}

// U64BGWall_ctor — 0xBDC / 759 insns (exact since agent-h run 406). Procedural tiled quad-mesh + index-buffer builder/constructor
// (terrain/water-surface grid for the bootup/title scene).
//
//   void *U64BGWall_ctor(void *self, Cfg *a1, void *owner2, void *owner3,
//                       arg5)  // arg5 @ sp+0x1E8; a1->s0 owner2->s1 owner3->s2
//     if (!self) { self = func_00000000(0x4C); if (!self) return 0; }  // alloc
//     func_00000000(self, &sStr_Node_66B0);          // register / base-init
//     self->0x28 = 0;
//     self->0x0C = &sStr_U64BGWall_66B8;                   // vtable/template
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
//   (agent-g run 555: the "owner" words are not pointers -- 0x2C / 0x30 / 0x34 = rotX / rotY / rotZ in
//    degrees, read by U64BGWall_display; 0x38 = the display list built here. See envobj.h U64BGWall.)
//   self: 0x0C vtable(&sStr_U64BGWall_66B8), 0x28 fnptr=0, 0x2C/0x30 rotX/rotY,
//         0x34 rotZ, 0x3C/0x40/0x44/0x48 f32=1.0 (scale/uv defaults), size 0x4C.
//   owner2(s1)->0x0C = a {data,count} index-list handle.
//   sStr_Node_66B0 base-init descriptor; sStr_U64BGWall_66B8 instance vtable.
//   func_00000000 + 0x4 = a placeholder/folded data ref (same JAL-target-0
//   + folded-pool family as the SndHandle_triggerEvent1_0000098C/stream_readF32_00000044 literal-pool bug,
//   docs/N64_FORENSICS.md#bootup-uso-fp-literal-pool-folded-into-func-0000098C).
// EXACT (agent-h run 406, 759 words): promoted in place, INCLUDE_ASM dropped.
//   Procedural tiled quad-mesh + index-buffer builder. Name pre-checked:
//   no extern reuse.
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
 * identities: dims = import_8001FFA0 {w, h}, mgr = *(gGfxLib_8005C108 +
 * 0x18), vtx quad = bootup data sym163.
 * agent-g run 169 (87.04 -> 88.82): `k = 0` set right before phase 2 (its only
 * user) instead of before phase 1: the shorter k live range outranks buf, which
 * is now spilled to 0x1CC and reloaded like the target. */
extern char sStr_Node_66B0, sStr_U64BGWall_66B8;
extern char D_e68_vtx;  /* bootup.uso data sym163 (D_0065AC): 4-vertex quad */
typedef struct { u32 w0, w1; } GwE68;
typedef struct { GwE68 *buf; s32 count; } DlE68;
/* MgrE68 / DlE68 mirror DLContext.gfx.cur / DLBuf (gdl/dlctx.h) with a typed Gfx base: E68_MGR is
 * GfxLib.staticDL (+0x214, "MainStaticDL"), so the wall's index list goes into the static display list. */
typedef struct { char pad[0xC]; DlE68 *dl; } MgrE68;
typedef struct { s16 x, y, z; u16 flag; s16 s, t; u8 cn[4]; } VtxE68;
extern s32 import_8001FFA0[];   /* screen/grid dims {w, h}: w reads */
extern s32 import_8001FFA0_h[]; /* same import, h reads ([1] = +4) */
extern int gGfxLib_8005C108[];
extern int gGfxLib_8005C108_e68[]; /* agent-h run 406: the same import, pinned at the target's link value (USO reloc snd_setSoundOutputMode_000001FC: +0x18 = 0x214) */
#define E68_MGR (*(MgrE68 **)((char *)gGfxLib_8005C108_e68 + 0x18))
#define E68_NEXT(m) (_n = (m)->dl->count++, &(m)->dl->buf[_n])
/* agent-g run 172: two slots use the explicit increment (objdiff 88.98 -> 89.21, greedy per-site sweep;
 * which increments feed the shared _n changes its colouring, cf. 1A44). */
#define E68_NEXTB(m) (_n = (m)->dl->count, (m)->dl->count = _n + 1, &(m)->dl->buf[_n])
/* agent-h run 397 (kit CDLXXXII): the phase-3 G_DL loop counts in its own variable c3, and the last four phase-3
 * appends (F0, E7, the per-row VTX, END) count through it too -- c3's live range then shares blocks with the
 * phase-3 dl (v1), so the loop count avoids v1 and the loop colours like the target (99.308 -> 99.322). uls loses
 * its phase-3 uses, so the phase-2 fold carries 3 uls terms (was 1) to keep uls in t4. */
#define E68_NEXTC(m) (c3 = (m)->dl->count, (m)->dl->count = c3 + 1, &(m)->dl->buf[c3])
/* agent-g run 254: the dl pointer read as a 4-byte int (same size, same value) is a distinct uopt expression, so the
 * phase-3 G_DL loop's dl loads form their own live range and colour v0 like the target (98.41 -> 98.61) */
#define E68_DL_I(m) ((DlE68 *)*(s32 *)&(m)->dl)
#define E68_NEXTBI(m) (_n = E68_DL_I(m)->count, E68_DL_I(m)->count = _n + 1, &E68_DL_I(m)->buf[_n])
#define E68_DL_U(m) ((DlE68 *)*(u32 *)&(m)->dl) /* agent-h run 403: same value as E68_DL_I, a separate uopt expression */
#define E68_SHL(v, s, w) ((u32)(((u32)(v) & ((0x01 << (w)) - 1)) << (s)))
#define E68_W(pkt, a, b) { GwE68 *_g = (GwE68 *)(pkt); _g->w0 = (a); _g->w1 = (b); }
#define E68_TILEGEN(pkt, c, tile, uls, ult, lrs, lrt) \
    E68_W(pkt, E68_SHL(c, 24, 8) | E68_SHL(uls, 12, 12) | E68_SHL(ult, 0, 12), \
          E68_SHL(tile, 24, 3) | E68_SHL(lrs, 12, 12) | E68_SHL(lrt, 0, 12))
#define E68_SETTILE(pkt, fmt, siz, line, tmem, tile, pal, cmt, maskt, shiftt, cms, masks, shifts) \
    E68_W(pkt, E68_SHL(0xF5, 24, 8) | E68_SHL(fmt, 21, 3) | E68_SHL(siz, 19, 2) | E68_SHL(line, 9, 9) | E68_SHL(tmem, 0, 9), \
          E68_SHL(tile, 24, 3) | E68_SHL(pal, 20, 4) | E68_SHL(cmt, 18, 2) | E68_SHL(maskt, 14, 4) | \
          E68_SHL(shiftt, 10, 4) | E68_SHL(cms, 8, 2) | E68_SHL(masks, 4, 4) | E68_SHL(shifts, 0, 4))
/* agent-h run 399: E68_W with two folded (zero-code) uses of the packet pointer -- on the first LOADTEXTILE
 * packet they raise its priority over the CULL packet temp, so it takes a0 and the CULL packet a2 like the
 * target (99.671 -> 99.709; the three phase-2 spill temps move up one slot, 76/80/84 -> 80/84/88) */
#define E68_WF2(pkt, a, b) { GwE68 *_g = (GwE68 *)(pkt); _g->w0 = (a) + 0 * ((int)_g + (int)_g); _g->w1 = (b); }
#define E68_LOADTEXTILE(pkt, timg, fmt, siz, width, height, uls, ult, lrs, lrt, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ \
    E68_WF2(pkt, E68_SHL(0xFD, 24, 8) | E68_SHL(fmt, 21, 3) | E68_SHL(siz, 19, 2) | E68_SHL((width) - 1, 0, 12), (u32)(timg)); \
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
#define E68_TRI1S(pkt, a, b, c) E68_W(pkt, E68_SHL(0xBF, 24, 8), E68_SHL(a, 16, 8) | E68_SHL(b, 8, 8) | E68_SHL(c, 0, 8)) /* agent-h run 403: E68_TRI1 with the operands already scaled by 2 */
#define E68_END(pkt) E68_W(pkt, E68_SHL(0xB8, 24, 8), 0)
#define E68_M10(p) (int)(p) + (int)(p) + (int)(p) + (int)(p) + (int)(p) + (int)(p) + (int)(p) + (int)(p) + (int)(p) + (int)(p) /* agent-h run 389 (kit CDLXXV): flat, unparenthesised -- each term is one counted use of p */
#define E68_M80(p) E68_M10(p) + E68_M10(p) + E68_M10(p) + E68_M10(p) + E68_M10(p) + E68_M10(p) + E68_M10(p) + E68_M10(p)
extern void func_00000000_e68v(); /* run 156: discarded bzero-like call (void: frees v0) */
void *U64BGWall_ctor(char *self, char *cfg, s32 arg2, s32 arg3, s32 arg4) {
    u32 timg;
    u32 tlut;
    VtxE68 *buf;
    s32 row;
    s32 k2;
    s32 col, x, y;
    u32 vidx; /* agent-h run 403: unsigned again on the j-after-IVs body -- the three phase-2 spill temps move to the target's 76 / 80 / 84 (99.907 -> 99.914; vidx is a non-negative vertex index, so the semantics are unchanged); run 172: unsigned (88.98/89.21 -> 90.53; changes the vidx*2 web colouring); agent-g run 291: signed again on the CULL-on-vs/ve body (98.642 -> 98.650; the values are small and non-negative, so the semantics are unchanged) */
    s32 ult;
    GwE68 *arr[32];
    s32 k, uls;
    s32 i;
    GwE68 *head;
    /* agent-h run 399: the run-397 phase-3 count c3 dropped (see the G_DL loop); padq back to 3 words */
    s32 j, n;
    MgrE68 *m;
    MgrE68 *m2; /* agent-c run 353 (kit CDXLIII): phase 3's own MGR variable (the target's phase-2 m is v0, phase-3 m s1: two LRs) */
    VtxE68 *vp;
    s32 _n;
    int padq[3]; /* agent-h run 399: 3 words without c3; agent-h run 397: back to 2 words with c3 in the all-NEXTB phase 2 (the spill temps sit at the target's 76 / 80 / 84 again); agent-g run 293: no longer volatile (byte-identical); run 253: 3 words (the block-scope vs / ve take no home); run 138: frame = target; agent-g run 224: declared LAST, the four words sit under the named homes (ult at frame-40, i/head at -184 like the target); agent-c run 353: 2 words with m2 (frame still 0x1D8) */

    if (&self) {} /* agent-c run 353 (kit CDXLIII): self address-taken = homed at 472(sp) with a reload per use, as in the target -- byte-identical on the one-m body, and with m2 it keeps self / buf out of the s-registers */
    if ((self != 0) || (self = (char *)FCALL(mem_new)(0x4C)) != 0) {
        FCALL(Node_ctor)(self, &sStr_Node_66B0);
        NODE(self)->vptr = (char *)(&U64BGWall_vtbl);
        NODE(self)->name = (const char *)(&sStr_U64BGWall_66B8);
        timg = ((BGWallTex *)cfg)->timg;
        tlut = ((BGWallTex *)cfg)->tlut;
        ((U64BGWall *)self)->rotX = arg2;
        ((U64BGWall *)self)->rotY = arg3;
        ((U64BGWall *)self)->rotZ = arg4;
        ((U64BGWall *)self)->scale[3] = 1.0f;
        ((U64BGWall *)self)->scale[2] = 1.0f;
        ((U64BGWall *)self)->scale[1] = 1.0f;
        ((U64BGWall *)self)->scale[0] = 1.0f;
        buf = (VtxE68 *)FCALL(mem_newAligned)(0x800, 16);
        if (arg2 != 0x123456) {}
        XCALL(func_00000000_e68v, str_fill)(buf, 0x800, 0);
        vidx = 0;
        y = import_8001FFA0_h[1] / 2 - 16;
        for (row = 0; row < import_8001FFA0_h[1] / 32; row++) {
            x = import_8001FFA0[0] / 2 - 32;
            /* agent-c run 233: the column counter is n (reused, assigned again before phase 3): col's web
             * then takes the target's s3 (96.42 -> 96.54); col stays declared for its home */
            for (n = 0; n < import_8001FFA0[0] / 64; n++) {
                /* run 192: per-vertex store order x, y, z, s, t (vertex 2: t before s) -- greedy statement-order grid, 92.78 -> 93.36 standalone */
                buf[vidx + 0].x = x + 32;
                buf[vidx + 0].y = y + 16;
                buf[vidx + 0].z = import_8001FFA0[0] / 2;
                buf[vidx + 0].s = 0;
                buf[vidx + 0].t = 0;
                buf[vidx + 1].x = x - 32;
                buf[vidx + 1].y = y + 16; /* run 233: the old if (1) {} here dropped (96.56 -> 96.72) */
                buf[vidx + 1].z = import_8001FFA0[0] / 2;
        if (vidx) {}
        if (arg2 != 0x123456) {}
                buf[vidx + 1].s = 0xFC0;
                buf[vidx + 1].t = 0;
                buf[vidx + 2].x = x - 32;
                buf[vidx + 2].y = y - 16;
                buf[vidx + 2].z = import_8001FFA0[0] / 2;
                buf[vidx + 2].s = 0xFC0; /* run 233: s before t again (96.54 -> 96.56) */
                buf[vidx + 2].t = 0x7C0;
                buf[vidx + 3].x = x + 32;
                buf[vidx + 3].y = y - 16;
                buf[vidx + 3].z = import_8001FFA0[0] / 2;
        if (arg3 != 0x123456) {}
                buf[vidx + 3].s = 0;
                buf[vidx + 3].t = 0x7C0;
                x -= 64;
                vidx += 4;
            }
        if (arg3) {}
            y -= 32;
        }
        k = 0;
        m = E68_MGR + 0 * arg3; /* agent-c run 209: folded counted use (kit CXCIV lever b); agent-g run 221: of arg3, not *cfg -- cfg's LR then ends at its two loads and takes s0 (arg2 s1) like the target */
        ult = 0; if (m) {} /* agent-g run 291: code-free test, see the `if (j) {}` below */
        for (i = 0; i < import_8001FFA0_h[1] / 64; i++) {
            vidx = 0;
            for (k2 = 0; k2 != 2; k2++) {
                uls = 0;
                for (j = 0; j < import_8001FFA0[0] / 64; j++) {
                    s32 vs, ve;
                    vs = vidx * 2; if (uls) {} /* agent-h run 403: ve is assigned inside the CULL packet below; the test on uls, not j (see `ult += 32` below); agent-g run 291: code-free test (random lever search, with the `if (m) {}` after `ult = 0`: 98.630 -> 98.642); agent-h run 393: vs assigned first -- the two dead `or` copies come out in the target's order (vs copy, then ve copy), 99.302 -> 99.308 */
                    /* agent-g run 291: the CULL packet on the named vs / ve (the gSPCullDisplayList operands
                     * pre-scaled) instead of recomputing (vidx) * 2 / (vidx + 3) * 2: two fewer coloured items in
                     * the packet BBs, so gen_outparcode's per-BB store temps fit the spilltemps pool and the frame
                     * bottom loses its two emit-time words -- the spills sit at the target's 76 / 80 / 84 */
                    /* agent-h run 403: ve assigned in the CULL w1 operand, after the arr[k] store in evaluation order --
                     * uopt numbers the arr address IV before the (vidx + 3) * 2 IV, so in their adjsave tie (620, with
                     * the second TRI1 on the pre-scaled ve) arr is coloured first and takes ra, the ve IV s0, like the
                     * target; 4 more folded vs uses below put the dead vs / ve copies in a3 / t0 (99.914 -> 99.980) */
                    E68_W(arr[k++] = E68_NEXTB(m), E68_SHL(0xBE, 24, 8) | E68_SHL(vs, 0, 16), E68_SHL(ve = (vidx + 3) * 2, 0, 16));
                    /* agent-g run 253: plain width read again (the old *(s32 *)(int)&w stand-in dropped); the scaled CULL operand
                     * copies vs / ve, kept live by empty tests AFTER the IV increments, give the target's dead `or` copies (96.83 -> 97.80) */
                    /* agent-h run 397: every phase-2 append on the explicit increment (E68_NEXTB): the NEXT post-increment
                     * temp was an extra live range (a0) -- the target's count shares a1 with _n. Without the old
                     * `if (1) {}` after LOADTEXTILE the body is 759 words again (that split cost the target's 84(sp)
                     * spill), then the fold counts below re-tuned: 99.361 -> 99.592 */
                    E68_LOADTEXTILE(E68_NEXTB(m), timg, 2, 1, import_8001FFA0[0], 0, uls, ult, uls + 63, ult + 31, 0, 0, 0, 6, 5, 0, 0);
                    E68_TRI1(E68_NEXTB(m), vidx, vidx + 1, vidx + 2); if (timg) {} /* agent-c run 353: with the `if (uls) {}` below (climb, 98.709 -> 98.742; either alone byte-identical) */
                    /* agent-g run 254: an empty test of m between the two TRI1 packets (lever sweep over every statement slot) lines
                     * the body up word-for-word up to registers at the target's 3036 B (97.80 -> 98.05; with the if (1) {} above) */
                    /* agent-c run 353: the run-254 `if (m) {}` and the `if (uls) {}` here dropped once the phase-3 loop counts through uls (98.748 -> 98.781) */
                    E68_TRI1S(E68_NEXTB(m), vidx * 2, (vidx + 2) * 2, ve); /* agent-h run 403: pre-scaled operands, the third through ve (= (vidx + 3) * 2): one fewer use of the ve IV (4100 -> 3100) */
                    E68_END(E68_NEXTB(m));
                    uls += 64;
                    vidx += 4;
                    if ((vs) + 0 * ((int)(vs) + (int)(vs) + E68_M80(m) + E68_M10(m) + E68_M10(m) + E68_M10(m) + E68_M10(m) + (int)(uls) + (int)(uls) + (int)(vs) + (int)(vs) + (int)(vs) + (int)(vs) + (int)(vs) + (int)(vs))) {} if ((ve) + 0 * ((int)(ve) + (int)(ve) + (int)(ve) + (int)(ve) + (int)(ve) + (int)(ve))) {} /* agent-h run 397: on the all-NEXTB body 120 m / 2 uls / 1 j / 5 vs folds and 6 folded ve uses (grids); */ /* agent-g run 254: folded counted use of vs (zero-use sweep, 98.17 -> 98.21); agent-h run 389 (kit CDLXXV): + 80 folded uses of m at this depth raise m's uopt priority (save / blocks) above the count load and dl, so m is coloured first and takes v0 like the target; with the first TRI1 back on NEXT and the four param / constant tests in phase 1 / 3 (random climb) 98.781 -> 98.860; then 20 folded uls and 3 folded j uses here (greedy per-variable fold sweep) plus `if (arg2) {}` / `if (arg3) {}` in phase 1 (climb): 98.860 -> 99.137; two more folded vs uses (random fold combos): 99.183; the vertex-1 `if (arg2) {}` as a constant test: 99.216; `if (vidx) {}` in front of it: 99.262; agent-h run 393: without the ult fold, a fold-count grid (uls 0..25 x j 0..5) gives 1 uls + 5 j (uls falls to t4 like the target) and the vertex-2 `if (arg2) {}` is inert: 99.302 */
                    if (1) {} /* agent-g run 254 (lever sweep, 98.05 -> 98.17: the vs / ve IVs take the target's s1 / s2) */
                }
                /* agent-h run 403: j's in-loop folded use and `if (j) {}` dropped and one folded use here, after the
                 * loop: j appears in one more block at depth 2 (save 3200 over 6 blocks = 533, below the 620 of the
                 * +2 / +4 IVs and arr), so j is coloured after them and takes s3, the IVs s1 / s2 (99.867 -> 99.907) */
                ult += 32 + 0 * ((int)j); /* agent-h run 393 (kit CDLXXIX): the run-291 folded ult use dropped -- ult's lower
                            * priority lets the target's earlier LRs take a1 / t0 and ult falls to t1 (99.262 -> 99.295) */
            }
        }
        /* agent-c run 353: phase 3 through m2 (explicit-increment appends, head on the int view; random
         * climb on the two-variable body, 97.80 -> 98.709 with the padq[2] / &self changes above) */
        /* agent-h run 403: a folded use of the phase-2 m keeps m live into the phase-3 head, so the MGR value
         * takes t0 in both phases (ult falls to t1) like the target; the G_DL loop then reads dl through the u32
         * view (E68_DL_U), a distinct uopt expression from the head's s32 view, so the head dl takes v1 and the
         * loop dl v0 as in the target (99.709 -> 99.867) */
        /* agent-h run 406: EXACT. The head packet in its own block-local _h, written to head through a u32 view
         * (`*(u32 *)&head = (u32)_h`, same value): copy propagation no longer merges _h into the long head LR,
         * so _h is a one-block LR coloured early -- with one folded use (`_h[0 * (int)_h]`, adjsave 3 -> 4) it
         * wins its tie with the head dl and takes v0, the dl v1 and MGR t0 like the target; the run-403 m fold
         * (which kept m in v0 through this block) is dropped (99.980 -> 100) */
        m2 = E68_MGR;
        { GwE68 *_h = E68_NEXTBI(m2);
          _h[0 * ((int)_h)].w0 = E68_SHL(0x04, 24, 8) | E68_SHL(0, 16, 8) | E68_SHL(((4) << 10) | (sizeof(VtxE68) * (4) - 1), 0, 16);
          *(u32 *)&head = (u32)_h;
          head->w1 = (u32)(FCALL(gl_osVirtualToPhysical)(&D_e68_vtx)); }
        E68_W(E68_NEXTB(m2), 0xBE000000, 6);
        E68_W(E68_NEXTB(m2), 0xE7000000, 0); if (1) {} /* run 182 (lever-sweep bbsplit); agent-c run 288: the run-210 folded h use dropped (inert, byte-identical) */
        E68_W(E68_NEXTB(m2), 0xBA001402, 0);
        E68_W(E68_NEXTB(m2), 0xBB000001, 0x80008000);
        E68_W(E68_NEXTB(m2), 0xB6000000, 0x33205); if (1) {} /* run 182 (lever-sweep bbsplit) */
        E68_W(E68_NEXTB(m2), 0xB900031D, 0x0C084000);
        E68_W(E68_NEXTB(m2), 0xFC357E6A, 0x11FCF67B);
        E68_W(E68_NEXTB(m2), 0xBA000E02, 0x8000); if (m2) {}
        E68_W(E68_NEXTB(m2), 0xFD100000, tlut);
        E68_W(E68_NEXTB(m2), 0xE8000000, 0);
        E68_W(E68_NEXTB(m2), 0xF5000100, 0x07000000); if (1) {} /* agent-h run 397: phase-3 bbsplit (99.322 -> 99.361) */
        E68_W(E68_NEXTB(m2), 0xE6000000, 0);
        E68_W(E68_NEXTB(m2), 0xF0000000, 0x073FC000); if (1) {}
        E68_W(E68_NEXTB(m2), 0xE7000000, 0);
        vp = buf;
        n = import_8001FFA0[0] / 32;
        if (arg4 != 0x654321) {}
        /* agent-c run 233: the row pointer is &vp[i * (n * 4)] (uopt strength-reduces it: the target's
         * in-loop `sll n,2; sll 4; addu` step, never hoisted), and the h re-read goes through the
         * address the target holds in s8 -- 94.59 -> 95.10 standalone; spelled (int)&h[1] the +4 folds
         * into the lui/addiu pair (sym+4) like the target -- 95.17 -> 95.31 */
        for (i = 0; i < *(s32 *)(int)&import_8001FFA0_h[1] / 64; i++) {
            E68_VTX(E68_NEXTB(m2), FCALL(gl_osVirtualToPhysical)(&vp[i * (n * 4)]), n * 4, 0); if (1) {} /* run 233 (lever-sweep bbsplit) */
            for (col = 0; col < n; col++) { /* agent-g run 250: phase-3 counter in col, not j (uopt colours per variable) */
                /* run 192: the target loads arr[] BEFORE the append (named before the packet) -- 90.82 -> 92.78 standalone */
                /* agent-g run 254: explicit-increment append here (per-site NEXT / NEXTB greedy, 98.21 -> 98.41) */
                /* agent-c run 353: the count in the dead phase-2 uls (written first) instead of _n -- the arr[] IV then
                 * takes the target's a1 (98.742 -> 98.748; j scores the same) */
                /* agent-h run 399: the four outer phase-3 appends back on _n (a1 like the target) and the G_DL count
                 * in the dead tlut: its live range spans phases 1-3, so its priority falls below the arr[] IV and it
                 * takes the target's a2 (arr a1) -- 99.592 -> 99.671 (a fresh local takes v1; arg4 scores the same) */
                { GwE68 *dl = arr[i * n + col]; E68_DMA1P((tlut = E68_DL_U(m2)->count, E68_DL_U(m2)->count = tlut + 1, &E68_DL_U(m2)->buf[tlut]), 0x06, dl, 0, 0); }
            }
            if (1) {} /* run 182 (lever-sweep bbsplit) */
        }
        E68_END(E68_NEXTB(m2));
        ((U64BGWall *)self)->displayList = (s32)head;
    }
    return self;
}

/* U64BGWall_display - verified structural decode (0x1CC, 115 insns,
 * FP transform/draw + DL list-append). Same family as
 * Mountain_display / Tv1_display (global root vector scaled by the
 * Dc->0x128/0x12C/0x130 triple, reloc draw helper, command-list
 * append).
 *   void U64BGWall_display(Obj *a0) {
 *       if (*(int*)(snd_seqEndDbg_00000188 + 0x3C) & 0x2) return;  // gate
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
 * Struct-typing reference: global gate *(snd_seqEndDbg_00000188+0x3C) bit 1
 * (0x2) = suppress/already-emitted. *(&D_a)->0x254 (596) -> state
 * st; st->0x158 (344) -> node n; n->0x3C (60) -> a renderable
 * entry e (e->0x0 base ptr, e->0x4 << 6 = byte offset); n->0xC
 * (12) -> a command list {0x0 buffer, 0x4 u32 count}, 8-byte
 * entries, first word = 0x01020040 marker. root = *(&D_root),
 * root->0x70 (112) -> transform obj, ->0xA0/0xA4/0xA8 (160/164/
 * 168) f32 Vec3 scaled per-component by Dc->0x128/0x12C/0x130
 * (296/300/304) (the Mountain_display scale triple). a0->0x2C/0x30/
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
extern void snd_seqEndDbg_00000188();  /* used as data-symbol base */
/* U64BGWall_display: EXACT via an IDO 7.1 NCC (C++) donor, src/bootup_uso/bootup_uso_ncc_1A44.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder (agent-g run 172). Decode notes kept: */
/* PASS-3 2026-10-20 (agent-c run 114): fresh decode against the target
 * words and the bootup.uso reloc table. It is a gSPMatrix + gSPDisplayList
 * draw: gate = gGfxLib_8005C108[0x3C] bit 1; gfx ctx = G->0x254->0x158; the
 * matrix stack ctx->0x3C = {Mtx *buf, count} (64-byte Mtx). The 7-float
 * builder is a typed-float proto (floats go in a1..a3 + stack slots). w1
 * of the G_MTX word = osVirtualToPhysical of the same &buf[count], then
 * count++. DL appends use the comma-temp reload macro (see E68). The
 * camera pointer must stay inline (a named local costs a frame slot and
 * re-colors the a0 reloads: 81.7 -> 91.4 when inlined). */
void U64BGWall_display(char *a0) {
    volatile int r = 0;
}

void stream_readS32_00001C10(int *dst) {
    int buf[2];
    FCALL(fs_gdlReadFile)(&gSysLib, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

void stream_readF32_00001C4C(float *dst) {
    float buf[2];
    FCALL(fs_gdlReadFile)(&gSysLib, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

extern void stream_readVec3_00001C88();
void stream_readVec3_00001C88(Vec3 *dst) {
    int pad_top[1];
    Tri3i raw;
    int pad_mid[2];
    Tri3i tmp;
    int pad_bot[2];
    FCALL(fs_gdlReadFile)(&gSysLib, &raw, 12);
#ifdef PORT
    PORT_BE_SWAP32S(&raw, 12);
#endif
    tmp = raw;
    dst->x = *(float*)&tmp.a;
    dst->y = *(float*)&tmp.b;
    dst->z = *(float*)&tmp.c;
}

void stream_readS32_00001CF8(int *dst) {
    int buf[2];
    FCALL(fs_gdlReadFile)(&gSysLib, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

void stream_readQuad_00001D34(Quad4 *dst) {
    Quad4 buf;
    FCALL(fs_gdlReadFile)(&gSysLib, &buf, 16);
#ifdef PORT
    PORT_BE_SWAP32S(&buf, 16);
#endif
    *dst = buf;
}

void *WorldCourse_ctorEmpty_00001D8C(void *a0) {
    if (a0 == 0) {
        a0 = (void*)FCALL(mem_new)(0x48);
    }
    return a0;
}

void *WorldCourse_ctor_00001DB8(void *a0, int a1, int a2) {
    if (a0 == 0) {
        a0 = (void*)FCALL(mem_new)(0x48);
        if (a0 == 0) goto end;
    }
    FCALL(str_copyEnd)(a0, a1);
    ((WorldCourse *)a0)->desc = (s32 *)a2;
end:
    return a0;
}

/* WorldRegistry_create_00001E08 - verified structural decode (0x170, 92 insns,
 * get-or-create constructor + bulk property registration).
 * (reg = func_00000000(obj, &descriptor, 0).) Struct-typing
 * reference: o = 0x118-byte object. o->0x28 (40) descriptor ptr
 * (&D), o->0xC (12) type/vtable ptr (&sStr_World_Registry_6E38), o->0x70 (112) =
 * 0, o->0x78 (120) = 0, o->0x74 (116) = &o[0x2C] (an embedded
 * sub-struct self-pointer; o+0x2C is init'd by the second reloc
 * call). sStr_One_Make_6E48..sStr_Test_Grid_6EC4 (and the .L00006EA8 entry) = ~10
 * property/field descriptor data blobs registered into o via the
 * reg helper (a property-table builder; arg3 = 0 default).
 * EXACT 2026-10-16 agent-c (run 97): arg3 is NOT 0 but a per-site
 * base-0 address (lui/addiu 0 each; one shared name would CSE into an
 * s-reg) -> D_1E08_a..j, o->0x28 = D_1E08_d, all pinned 0; the param is
 * reused as `o` (prologue or s0 / bnez / sw ra delay, as in the exact
 * 8F18 sibling) and an unused `pad` local gives frame 0x30 + the 0x24
 * spill slot for `sub`; .L00006EA8 -> sStr_Black_Diamond_6EA8 pinned to 0x6EA8. */
extern char sStr_Node_6E30, sStr_World_Registry_6E38;
extern char sStr_One_Make_6E48, sStr_HalfPipe_6E54, sStr_Slalom_Course_6E60, sStr_Stunt_Course_6E70, sStr_Long_Course_6E80;
extern char sStr_New_Course_6E8C, sStr_Night_Course_6E98, sStr_Black_Diamond_6EA8, sStr_Practice_6EB8, sStr_Test_Grid_6EC4;
/* per-site base-0 placeholders (target lui 0 / addiu 0 each, no CSE) */
extern char D_1E08_d, D_1E08_a, D_1E08_b, D_1E08_c, D_1E08_e, D_1E08_f;
extern char D_1E08_g, D_1E08_h, D_1E08_i, D_1E08_j;
void *WorldRegistry_create_00001E08(char *o) {
    char *pad;
    char *sub;
    if (o == 0) {
        o = (char*)FCALL(mem_new)(0x118);
        if (o == 0) goto end;
    }
    FCALL(Node_ctor)(o, &sStr_Node_6E30);
    ((WorldRegistry *)o)->vptr = &XDATA(D_1E08_d, port_bootup_data, 0x6DD0);
    sub = (char *)&((WorldRegistry *)o)->head;
    FCALL(WorldCourse_ctorEmpty_00001D8C)(sub);
    NODE(o)->name = (const char *)&sStr_World_Registry_6E38; /* Node name "World Registry" */
    ((WorldRegistry *)o)->head.next = 0;
    ((WorldRegistry *)o)->numCourses = 0;
    ((WorldRegistry *)o)->tail = (WorldCourse *)sub;
    FCALL(WorldRegistry_addCourse_00001F78)(o, &sStr_One_Make_6E48, &D_1E08_a);
    FCALL(WorldRegistry_addCourse_00001F78)(o, &sStr_HalfPipe_6E54, &D_1E08_b);
    FCALL(WorldRegistry_addCourse_00001F78)(o, &sStr_Slalom_Course_6E60, &D_1E08_c);
    FCALL(WorldRegistry_addCourse_00001F78)(o, &sStr_Stunt_Course_6E70, &XDATA(D_1E08_d, port_bootup_data, 0x6A10));
    FCALL(WorldRegistry_addCourse_00001F78)(o, &sStr_Long_Course_6E80, &D_1E08_e);
    FCALL(WorldRegistry_addCourse_00001F78)(o, &sStr_New_Course_6E8C, &D_1E08_f);
    FCALL(WorldRegistry_addCourse_00001F78)(o, &sStr_Night_Course_6E98, &D_1E08_g);
    FCALL(WorldRegistry_addCourse_00001F78)(o, &sStr_Black_Diamond_6EA8, &D_1E08_h);
    FCALL(WorldRegistry_addCourse_00001F78)(o, &sStr_Practice_6EB8, &D_1E08_i);
    FCALL(WorldRegistry_addCourse_00001F78)(o, &sStr_Test_Grid_6EC4, &D_1E08_j);
end:
    return o;
}

// Linked-list insert-head: alloc node via func_00000000(0), splice after
// obj->0x74 (head), advance head, bump obj->0x78 count. Body byte-matches at
// 98%; the sole diff is IDO -O2 saving obj across the jal in $a1 while the
// target uses $a3 — documented-unflippable from C, see
// docs/IDO_CODEGEN.md#feedback-ido-arg-save-reg-pick (this func is that
// entry's canonical example). Permuter-only; no episode (tautology-trap rule).
void WorldRegistry_addCourse_00001F78(WorldRegistry *obj, int d1, int d2) {
    WorldCourse *n;
    n = (WorldCourse *)FCALL(WorldCourse_ctor_00001DB8)(0, d1, d2, obj); /* WorldCourse_ctor_00001DB8(0, name, desc) */
    n->next = obj->tail->next;
    obj->tail->next = n;
    obj->tail = n;
    obj->numCourses += 1;
}

void *WorldRegistry_getCourse_00001FC8(WorldRegistry *a0, int a1) {
    WorldCourse *p = a0->head.next;
    int i;
    for (i = 0; i < a1; i++) {
        p = p->next;
    }
    return p;
}

void *WorldRegistry_getCourse_00002014(WorldRegistry *a0, int a1) {
    WorldCourse *p = a0->head.next;
    int i;
    for (i = 0; i < a1; i++) {
        p = p->next;
    }
    return p;
}

void game_setParams_00002060(char *a0, int a1, int a2, int a3, int a4, int a5) {
    WORLDREG(a0)->fgRoot = a1;
    WORLDREG(a0)->bgRoot = a2;
    WORLDREG(a0)->egRoot = a4;
    WORLDREG(a0)->ogRoot = a5;
    WORLDREG(a0)->mainGame = a3;
}

void game_clearWindows_00002080(int *a0) { WORLDREG(a0)->numWindows = 0; }

/* append to count+entries list at 0x104 (count) + 0x108 (4-byte entries).
 * MATCHED 2026-06-21: the long-standing `addu`-before-`sw count` cap was an
 * as1 SCHEDULER TIE keyed on per-statement debug line numbers. Collapsing the
 * count-store and the entry-store onto a SINGLE source line lets IDO's as1
 * scheduler interleave them by data-readiness (it hoists `addu t8,a0,t7` so
 * t8 is ready for the delay-slot `sw a1,0x108(t8)`), producing the target's
 * addu-before-sw order. Same-line lever (cf. game_createBGBox). Sibling
 * game_addPlayer_000020AC cracked identically. */
void game_addWindow_00002088(char *a0, int a1) {
    int idx = WORLDREG(a0)->numWindows;
    WORLDREG(a0)->numWindows = idx + 1; WORLDREG(a0)->windows[idx] = a1;
}

void game_clearPlayers_000020A4(int *a0) { WORLDREG(a0)->numPlayers = 0; }

/* append-pair to a count+entries list at offset 0xC0 (count) + 0xC4 (pairs
 * of 8). MATCHED 2026-06-21: same as1 SCHEDULER TIE as game_addWindow_00002088 — the
 * `sw t9,0xC0(a0)`-before-`addu t1,a0,t0` cap was a per-statement-line
 * tie-break. Collapsing the count-store and entry-store onto one source line
 * lets as1 hoist the addu (t1 needed for the delay-slot store), matching the
 * target's addu-before-sw order. */
void game_addPlayer_000020AC(int *a0, int a1, int a2) {
    int idx;
    WORLDREG(a0)->players[WORLDREG(a0)->numPlayers].arg = a2;
    idx = WORLDREG(a0)->numPlayers;
    WORLDREG(a0)->numPlayers = idx + 1; WORLDREG(a0)->players[idx].rider = (char *)a1;
}

void game_forEachPlayer_000020D8(char *a0, int a1) {
    int i;
    char *p;
    if (WORLDREG(a0)->numPlayers > 0) {
        i = 0;
        p = a0;
        do {
            FCALL(RaceTracker_addEntry)(a1, WORLDREG(p)->players[0].rider, WORLDREG(p)->players[0].arg); /* p steps one player */
            i++;
            p += 8;
        } while (i < WORLDREG(a0)->numPlayers);
    }
}

void game_stub_0000214C(int a0) {
}

void boot_readFloatParm_00002154(char *a0) {
    int scratch;
    stream_readS32_00001C10(&scratch);
    stream_readF32_00001C4C((float*)(a0 + 0x10));
}

void boot_readVec3Parm_00002184(char *a0) {
    int scratch;
    stream_readS32_00001C10(&scratch);
    stream_readVec3_00001C88(a0 + 0x10);
}

void boot_readIntParm_000021B4(char *a0) {
    int scratch;
    stream_readS32_00001C10(&scratch);
    stream_readS32_00001CF8((int*)(a0 + 0x10));
}

void boot_readQuadParm_000021E4(char *a0) {
    int scratch;
    stream_readS32_00001C10(&scratch);
    stream_readQuad_00001D34((Quad4*)(a0 + 0x10));
}

void boot_readIntParm_00002214(char *a0) {
    int scratch;
    stream_readS32_00001C10(&scratch);
    stream_readS32_00001C10((int*)(a0 + 0x10));
}

/* boot_ItemType_create_00002244: byte-identical twin of boot_ItemType_create_0000EBE8 (alloc-or-fail +
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
int *boot_ItemType_create_00002244(int *caller_a0) {
    int *ret;
    int *target;
    int *p;
    if ((p = (int*)FCALL(mem_new)(0x40)) != 0) {
        FCALL(ColourItem_ctor)(p);
        NODE(p)->vptr = (char *)&ColourItem_vtbl_75A0;
        p[0x3C/4] = 0;
    }
    ret = p;
    p += 0x10/4;
    if (caller_a0[0x40/4] != 0) {
        target = (int*)caller_a0[0x40/4];
        FCALL(list_append)(p, target);
        if (target[0x14/4] != 0) {
            target[0x4/4] = 1;
        }
        target[0x14/4] = (int)ret;
    }
    return ret;
}

#pragma GLOBAL_ASM("asm/nonmatchings/bootup_uso/bootup_orphan_000022D4.s")

void stream_readS32_000022E0(int *dst) {
    int buf[2];
    FCALL(fs_gdlReadFile)(&gSysLib, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

void stream_readF32_0000231C(float *dst) {
    float buf[2];
    FCALL(fs_gdlReadFile)(&gSysLib, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

extern void stream_readVec3_00002358();
void stream_readVec3_00002358(Vec3 *dst) {
    int pad_top[1];
    Tri3i raw;
    int pad_mid[2];
    Tri3i tmp;
    int pad_bot[2];
    FCALL(fs_gdlReadFile)(&gSysLib, &raw, 12);
#ifdef PORT
    PORT_BE_SWAP32S(&raw, 12);
#endif
    tmp = raw;
    dst->x = *(float*)&tmp.a;
    dst->y = *(float*)&tmp.b;
    dst->z = *(float*)&tmp.c;
}

void stream_readQuad_000023C8(Quad4 *dst) {
    Quad4 buf;
    FCALL(fs_gdlReadFile)(&gSysLib, &buf, 16);
#ifdef PORT
    PORT_BE_SWAP32S(&buf, 16);
#endif
    *dst = buf;
}

/* MATCHED 2026-06-21: the s4/s5-store schedule swap is C-reachable after all.
 * The target emits the s4 store (sp-loaded) then the s5 store BEFORE the four
 * zero/const stores; the prior C ordered s5 LAST (after the zeros), and IDO's
 * list scheduler then deferred the s4 store past the cheap zero-stores. Moving
 * the `a0->0x34 = s5;` statement up to be adjacent to `a0->0x30 = s4;` pins
 * both sp-loaded stores together ahead of the zero/const block, matching the
 * target schedule exactly (0 non-reloc diffs vs expected; reloc symbol set =
 * func_00000000 x2 + sStr_Node_731C HI/LO; the a0+0x28 store's `&D_00000000`
 * materialises as `lui 0; addiu 0` post-link, byte-identical to the target's
 * reloc-stripped `lui (0x0>>16); addiu 0x0`). */
extern char sStr_Node_731C;
/* include/game/camera.h CamDirector (agent-g run 475 field form) */
CamDirector *CamDirector_ctor(CamDirector *a0, int pad, int rider, int window, int cam0, int cam1) {
    if (a0 == 0) {
        a0 = (CamDirector *)FCALL(mem_new)(0x50);
        if (a0 == 0) goto end;
    }
    FCALL(Node_ctor)(a0, &sStr_Node_731C); /* Node init, "<Node>" */
    NODE(a0)->vptr = (char *)&CamDirector_vtbl; /* vptr (Data 0x6FD8) */
    a0->pad = (struct RiderInput *)pad;
    a0->rider = (SnowBoarder *)rider;
    a0->window = (CamWindow *)window;
    a0->cams[0] = (MCamera *)cam0;
    a0->cams[1] = (MCamera *)cam1;
    a0->activeCam = 0;
    a0->lastCamNode = 0;
    a0->pollTimer = 10000;
    a0->manual = 0;
end:
    return a0;
}

// CamDirector_update (vtable update slot): picks the race camera each frame (docs/1080_CAMERA.md section 7).
//   pad pressed 0x40 toggles manual mode. Entering it makes cams[0] active (command 3) at fov 85; while in it,
//   held 8 / 4 move fov by +-2 within 45..120 and the stick adds 2x to the active camera's pitch / yaw.
//   (gInputMgr_00000064 == 1, a global debug mode, also passes the pad to a hook first.)
//   Auto mode: every 60 frames find the course "cam" node nearest rider->pos; on a new node, kind 3..0x12
//   goes to cams[0] (the game.uso CourseCam), any other kind to cams[1] (RiderCam), via the camera's
//   command slot (kind, node), after making that camera active.
// Match history:
// NM body now structurally faithful: gate read folds via FW(&stream_readF32_00000044,0x20)
//   HI16/LO16 reloc; 0x68 delegate base is s16 (lh, FH macro); cam->0x14C activate
//   is a FLOAT store (FF, was wrongly FW=int 85); sStr_cam_7324 is the poll-table
//   global (extern int); 2.0f hoisted to `two` local so cfg->0x4/0x0 emit mul.s
//   (not strength-reduced x+x); cam field updates via &cam->field pointer temps.
// Reg-agnostic similarity ~82% (166/175 insns). RESIDUAL CAP = register coloring +
//   frame-size: target saves TWO callee regs (s0,s1) for arg0+held-temp across the
//   4 dispatcher calls; IDO here colors arg0 into a single saved reg (frame 0x40 vs
//   target 0x50). Permuter 3205->2810 plateau (only 0x14C-hoist nudge, no crack) —
//   classic coloring/two-saved-reg barrier. Genuine type/value/CSE bugs all fixed.
// agent-g run 276: exact (the C below compiles to the target bytes).
/* 2026-10-16 agent-c rewrite: gate read through gInputMgr_00000064 (pinned 0x64 ==
 * stream_readF32_00000044+0x20, folds into the lw like the target); the two cfg->0x10
 * reads stay separate because the second is read as u32 (one type CSEs them);
 * sStr_cam_7324 is passed as a 4-byte struct by value (`tb.w = D; f(.., tb, ..)`
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
extern int gInputMgr_00000064;
extern int D_24B8_tbl;
typedef struct { int w; } W24B8;
typedef void (*VF_24B8)();
/* agent-g run 155: the discarded placeholder calls go through a void alias (kit IX dead-v0 lever): 94.87 -> 95.21.
 * agent-g run 189 (kit CXLII): the target's second saved register s0 also carries the first call's
 * `(cfg->0x10 & 0x400) != 0` argument (`lw s0,16(v0); andi s0,s0,0x400; sltu a0,zero,s0`), so that
 * value goes through the reused s0 variable: arg0 moves to s1 and the multi-role var to s0 as in the
 * target (length = target); the t0 latch and k share one int variable. 95.21 -> see report. */
extern void func_00000000_24b8v();
/* agent-g run 275: the latch (k) and the p->0x3C value (k2) are separate variables: the target spills the
 * latch to its own home at 76 and k2 at 44. Decl order k / v0 / v1 / p / pad / tb / r / s0 / k2 puts every
 * home on the target (76 / 64 / 56 / 44; the run-272 pad2 is no longer needed). The folded arg0[0x4C]
 * uses in the toggle test and `k = 1` are one zero-code CSE candidate across the head. */
/* agent-g run 485: on include/game/camera.h CamDirector / MCamera / RiderInput fields. The odd-looking zero-code
 * uses (`if (CD_BYTE(0x4C)) {}`, `0 * (int)..`, `if (1) {}`) and the one multi-role `s0` are the register-colouring
 * levers of the exact match (runs 155..276): keep them. CD_BYTE reads a byte of the director (a dead use only). */
#define CD_BYTE(o) (((char *)self)[o])
#define CD_CAM(p) ((MCamera *)(p))
#define CD_PAD(p) ((RiderInput *)(p))
/* a Node's command slot (vtable slot 12: s16 this-delta at 0x68, fn at 0x6C); RiderCam's is RiderCam_command_00004914 */
#define CD_VPTR(o) FW(o, 0x28) /* Node vptr (include/gdl/node.h) */
#define CD_SLOT_DELTA(vt) FH(vt, 0x68)
#define CD_SLOT_FN(vt) ((VF_24B8)FW(vt, 0x6C))
void CamDirector_update(CamDirector *self) {
    int k;                  /* set = manual mode was toggled this frame */
    char *v0;
    char *v1;
    char *p;                /* the course "cam" node nearest the rider */
    volatile int pad;
    W24B8 tb;
    int r;
    char *s0;
    int k2;                 /* that node's kind */

    k = 0; if (CD_BYTE(0x4C)) {} /* agent-g run 275: with the folded use in the gate block, one zero-code manual candidate across the head takes v1, so the latch colours t0 like the target */
    if (gInputMgr_00000064 == 1) {
        v0 = (char *)self->pad + 0 * (int)(CD_BYTE(0x4C));
        r = 0;
        s0 = (char *)((CD_PAD(v0)->held & 0x400) != 0); XCALL(func_00000000_24b8v, snd_setPlayer1Channel15IO)((int)s0, (*(u32 *)&CD_PAD(v0)->held & 0x1000) != 0, CD_PAD(v0)->rawX, CD_PAD(v0)->rawY); /* agent-c run 227: one line = the target's a1-then-a0 sltu order (as1 line tie, kit CCXXXIV) */
        k = r;
    }
    s0 = (char *)self->pad;
    if (CD_PAD(s0)->pressed & 0x40) {
        k = 1;
        self->manual = (self->manual != 0) == 0;
    }
    if (self->manual != 0) {
        /* manual mode: fov on held 8 / 4 (45..120), pitch / yaw by the stick */
        f32 two = 2.0f;
        if (k != 0) {
            /* entering: make the path camera active and reset it */
            XCALL(func_00000000_24b8v, CamDirector_setActiveCam)(self, 0);
            v0 = (char *)self->cams[0];
            s0 = (char *)CD_VPTR(v0);
            CD_SLOT_FN(s0)(CD_SLOT_DELTA(s0) + v0, 3, 0);
            self->window->cam->fov = 85.0f;
        }
        s0 = (char *)self->pad; if (self) {}
        if (CD_PAD(s0)->held & 8) { /* agent-g run 275: agent-c's run-210 folded use dropped here and in the 0x134 copy; the 0x14C copy counts s0 instead (the head candidate re-phases the temp ring) */
            v1 = (char *)self->window->cam; if (1) {} /* agent-g run 275: empty blocks in the cam web (1 / 2 / 1) lower its priority below the cfg CSE */
            if (CD_CAM(v1)->fov < 120.0f) { if (CD_BYTE(0x40)) {} /* agent-g run 275: zero-code rider-byte candidate over the cam-only blocks (three sites) takes v0 there, so cam colours v1 and the cfg CSE v0 like the target */
                s0 = (char *)&CD_CAM(v1)->fov + 0 * (int)(*s0); if (1) {} /* folded counted use of s0 (= pad, set above; zero code) */
                *(f32 *)s0 = *(f32 *)s0 + two;
            }
        }
        s0 = (char *)self->pad;
        if (CD_PAD(s0)->held & 4) {
            v1 = (char *)self->window->cam; if (1) {} if (1) {}
            if (CD_CAM(v1)->fov > 45.0f) { if (CD_BYTE(0x40)) {}
                s0 = (char *)&CD_CAM(v1)->fov + 0 * (int)(*s0) + 0 * (int)(CD_BYTE(0x40)); if (1) {} /* agent-g run 275: folded counted use of s0 (= pad, set above): the copy keeps s0 (target addiu s0,v1,332), not a v0 CSE temp */
                *(f32 *)s0 = *(f32 *)s0 - two;
            }
        }
        v1 = (char *)self->window->cam; if (1) {}
        s0 = (char *)&CD_CAM(v1)->pitch + 0 * (int)(*s0); if (1) {} /* folded use of s0 (= pad): the copy stays in s0 (target addiu s0,v1,308) */
        *(f32 *)s0 = self->pad->stickY * two + *(f32 *)s0;
        s0 = (char *)self->window->cam;
        s0 = (char *)&CD_CAM(s0)->yaw;
        *(f32 *)s0 = self->pad->stickX * two + *(f32 *)s0;
        return;
    }
    /* auto mode: every 60 frames find the course "cam" node nearest the rider */
    if (k != 0) {
        self->pollTimer = 10000; /* just left manual mode: search at once */
    }
    s0 = (char *)self->pollTimer;
    self->pollTimer = (int)s0 + 1;
    if ((int)s0 >= 0x3C) {
        tb.w = sStr_cam_7324;
        p = (char *)FCALL(ItemMgr_queryType)(D_24B8_tbl, tb, self->rider->dnode.vqs.trans, 0);
        if (p != 0 && p != self->lastCamNode) {
            k2 = CAMITEM(p)->kind;
            if ((u32)k2 >= 3 && (u32)k2 < 0x13) {
                /* kinds 3..0x12: the game.uso path camera (CourseCam views) */
                XCALL(func_00000000_24b8v, CamDirector_setActiveCam)(self, 0);
                v0 = (char *)self->cams[0];
                s0 = (char *)CD_VPTR(v0);
                CD_SLOT_FN(s0)(CD_SLOT_DELTA(s0) + v0, k2, p);
            } else {
                /* other kinds: RiderCam */
                XCALL(func_00000000_24b8v, CamDirector_setActiveCam)(self, 1);
                v0 = (char *)self->cams[1];
                s0 = (char *)CD_VPTR(v0);
                CD_SLOT_FN(s0)(CD_SLOT_DELTA(s0) + v0, k2, p);
            }
            /* agent-g run 276: the 0x44 store written first (no test on p): the tail reload of p is then
             * an uncoloured split piece read into the ugen temp t6 like the target (the scheduler still
             * issues the 0x48 store first). 99.91 -> 100. */
            self->lastCamNode = p;
            self->pollTimer = 0;
        }
    }
}

void CamDirector_setActiveCam(CamDirector *a0, int a1) {
    if (NODE(a0)->doFlags & 8) { /* DoUpdate */
        f32 *p;
        a0->activeCam = a1;
        a0->window->cam = a0->cams[a1];
        p = &a0->window->cam->pitch;
        p[2] = 0.0f; /* unk13C */
        p[1] = 0.0f; /* yaw */
        p[0] = 0.0f; /* pitch */
    }
}

void CamDirector_onMessage(char *a0, int *a1) {
    int msg_type = *a1;
    int *flags = (int*)(a0 + 0x18);
    if (msg_type == 0x3ED) {
        *flags &= ~0x8;
    }
}

// game_init — RECONSTRUCTED 95.21% fuzzy (0x4AC / 299 insns, no episode).
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
//   void *game_init(Scene *scene, A a1, B a2, C a3) {  // args spilled
//                                                          // sp+0x50/54/58/5C
//     v0 = func_00000000(&D_00000000);  scene->0x3C = a2;  // (sp+0x38)
//     func_00000000(&EnvSettings_load, 0);                    // register descs:
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
//   snd_setSoundOutputMode_000001FC + 0x10 (lw, 3x) = folded int ref — same JAL-target-0 /
//   folded-symbol family as the SndHandle_triggerEvent1_0000098C literal-pool bug
//   (docs/N64_FORENSICS.md#bootup-uso-fp-literal-pool-folded-into-func-0000098C);
//   here it's the integer denominator of the ratio fixups.
// Caps (DEFERRED): 299-insn dispatcher-heavy ctor; raw-word USO +
//   placeholder cb's. Real-C STRUCTURAL body below — bootup/title
//   scene constructor skeleton. Byte-match deferred. Name pre-checked:
//   no extern reuse. D_00000000 reuses file-scope extern char.
#ifdef NON_MATCHING
void *game_init(char *scene, int a1, int a2, int a3) {
    char *s4C, *s48, *s44, *s40, *s3C, *p, *g;
    int sv, m;
    f32 fd;
    sv = (int)func_00000000(&D_00000000);
    ((InputMgr *)scene)->gameMode = a2;
    func_00000000((char *)&D_00000000 + 0x7328, 0);
    func_00000000(&D_00000000, (char *)&D_00000000 + 0x7334);
    func_00000000((int)((InputMgr *)scene)->game, a2);
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
    if (NODE(g)->parent) NODE(g)->unk04 = 1;
    NODE(g)->parent = (Node *)s4C;
    func_00000000();
    func_00000000((int)((InputMgr *)scene)->game, s44, s4C, s3C, s48, s40);
    func_00000000((int)((InputMgr *)scene)->game);
    func_00000000((int)((InputMgr *)scene)->game);
    func_00000000((int)((InputMgr *)scene)->game);
    m = (int)func_00000000(&D_00000000);
    fd = (f32)(u32)*(int *)((char *)&D_00000000 + 0x20C);
    func_00000000((char *)&D_00000000 + 0x739C,
                  ((f32)(u32)m - (f32)(u32)sv) / fd);
    sv = (int)func_00000000(&D_00000000);
    func_00000000((int)((InputMgr *)scene)->game, func_00000000((int)((InputMgr *)scene)->game, a1), a2);
    g = ((InputMgr *)scene)->game;
    m = (int)func_00000000(&D_00000000);
    WORLDREG(g)->map = (MapProp *)m;
    fd = (f32)(u32)*(int *)((char *)&D_00000000 + 0x20C);
    func_00000000((char *)&D_00000000 + 0x73B4,
                  ((f32)(u32)m - (f32)(u32)sv) / fd);
    sv = (int)func_00000000(&D_00000000);
    func_00000000((int)((InputMgr *)scene)->game, a2, a3);
    m = (int)func_00000000(&D_00000000);
    fd = (f32)(u32)*(int *)((char *)&D_00000000 + 0x20C);
    func_00000000((char *)&D_00000000 + 0x73CC,
                  ((f32)(u32)m - (f32)(u32)sv) / fd);
    p = s48 + 0x10;
    func_00000000(p, s44);
    if (NODE(s44)->parent) NODE(s44)->unk04 = 1;
    NODE(s44)->parent = (Node *)s48;
    func_00000000(p, s40);
    if (NODE(s40)->parent) NODE(s40)->unk04 = 1;
    NODE(s40)->parent = (Node *)s48;
    p = s3C + 0x10;
    func_00000000(p, s4C);
    if (NODE(s4C)->parent) NODE(s4C)->unk04 = 1;
    NODE(s4C)->parent = (Node *)s3C;
    func_00000000(p, s48);
    if (NODE(s48)->parent) NODE(s48)->unk04 = 1;
    NODE(s48)->parent = (Node *)s3C;
    return s3C;
}
#else
INCLUDE_ASM("asm/nonmatchings/bootup_uso", game_init);
#endif

/* game_initDdm_00002C94 - verified structural decode (0x110, 68 insns,
 * subsystem init: scratch-vec setup + global flag config + table
 * walk).
 * Struct-typing reference: snd_calcEmitterDistPan_000003F8+0xF8 = f32 default value;
 * stream_readQuad_000000F0+0x38 = a global scratch Vec3 (set to (k,k,k));
 * stream_readVec3_00000080+0x68 = a paired context object (reloc setup args).
 * Global flag words: &D_mode = 6, &D_flags bit3 (0x8) cleared,
 * config word at func_00000000+4 = (old & ~0x2) | 0x2000 | 0x80000
 * (clears bit1, sets the 0x2000 and 0x80000 feature bits). &D_tbl =
 * a {int a; int b;} stride-8 table (next-probe reads p[0] after the
 * advance) walked until a zero terminator, each (a,b) passed to the
 * reloc helper with a fixed &D_ctx. sStr_ddm_73E8 = init datum.
 * Caps <80: ~6 reloc calls + cross-symbol data refs (snd_calcEmitterDistPan_000003F8/
 * 000000F0/00000080/00000000+off) + multi-global flag RMW + table-
 * walk branch-likely. INCLUDE_ASM remains build path. */
extern char sStr_ddm_73E8;
extern void snd_calcEmitterDistPan_000003F8();  /* used as data-symbol base */
/* EXACT (agent-c run 115). Identities/offsets from the TextReloc and the
 * asm addends: scale = RoData pool +0x4F0, dst = 0x128 / src = 0xE8 (both
 * import_0005C108 fields: GfxLib.worldScale and the 4x4 matrix at GfxLib +0xE8, set to
 * diag(worldScale) here; gdl/gfxlib.h still has unk0E8[16]), mode D_01EBDC, flags import_0005EE64 (separate
 * load/store aliases), cfg word import_0005EE6C + 4 through a named int v
 * (target keeps it in v0), table/gate D_00719C (two aliases so the gate is
 * its own load), ctx import_0005C108; callees func_072E58, func_072F1C,
 * func_074710(&sStr_ddm_73E8, 0), func_059354 per entry, func_074840. */
extern f32 bu_2c94_k4f0;
extern f32 gGfxLib_2c94_dst[];
extern char gGfxLib_2c94_src[], gGfxLib_2c94_ctx[], sStr_ddm_73E8;
extern int bu_2c94_mode, gPolyFlags_3EEC4_2c94_x_ld, gPolyFlags_3EEC4_2c94_x_st, gPolyGeomMode_3EEC8_2c94_y_ld[], gPolyGeomMode_3EEC8_2c94_y_st[];
extern int bu_2c94_tbl[], bu_2c94_gate;
void game_initDdm_00002C94(void *a0, void *a1) {
    f32 k;
    int *p;
    int e;
    int v;

    k = bu_2c94_k4f0;
    gGfxLib_2c94_dst[0] = k;
    gGfxLib_2c94_dst[1] = k;
    gGfxLib_2c94_dst[2] = k;
    FCALL(math_mtx4Identity)(gGfxLib_2c94_src);
    FCALL(math_mtxScaleRows)(gGfxLib_2c94_src, gGfxLib_2c94_dst);
    bu_2c94_mode = 6;
    gPolyFlags_3EEC4_2c94_x_st = gPolyFlags_3EEC4_2c94_x_ld & ~8;
    v = gPolyGeomMode_3EEC8_2c94_y_ld[1];
    v &= ~2;
    v |= 0x2000;
    v |= 0x80000;
    gPolyGeomMode_3EEC8_2c94_y_st[1] = v;
    p = bu_2c94_tbl;
    FCALL(memtrack_begin)(&sStr_ddm_73E8, 0);
    if (bu_2c94_gate != 0) {
        do {
            FCALL(gfx_addDdm)(gGfxLib_2c94_ctx, p[0], p[1]);
            p += 2;
        } while (p[0] != 0);
    }
    FCALL(memtrack_end)();
}

/* game_setupMode: bootup_uso subsystem init + 12-way command dispatch
 * (0x1EC / 123 words). init(&sStr_ImgFileMgr_73EC, 0); create -> global; attach the
 * new object under arg0->0x84 (+0x10 list, 0x14 back-link, 0x4 linked flag);
 * config word |= 0x82000 | 0x20000; then switch (arg1) 1..12 -> per-mode
 * handler(arg0, arg2) (case 1: handler(arg0, 0) + obj->0x18 |= 4; cases
 * 7/8 and out-of-range: func(&sStr_unknown_GAME_MODE_7408)).
 * Exact C (identical to the IDO 7.1 -O2 donor unit bootup_uso_o2_2DA4.c,
 * 123/123 words; levers documented there). WIRED via REPLACE_FUNC_BODY donor
 * splice: the in-unit compile's .rodata jumptable is discarded by the link, so
 * the splice swaps in the donor body with its .rodata reloc renamed
 * game_setupMode_rodata (pinned to the bootup_uso RoData table at +0x4F4).
 * The in-unit body keeps the .text size exact: a smaller placeholder shifts
 * the section-relative %lo(.L...) addends of later INCLUDE_ASM functions
 * (replace-function-body.py does not rebase those). */
extern void func_00000000_2DA4_v();
extern char sStr_ImgFileMgr_73EC, sStr_rom_tricks_bin_73F8, sStr_unknown_GAME_MODE_7408;
extern int D_2DA4_obj, D_2DA4_obj_rd;
extern int gPolyGeomMode_3EEC8_2DA4_cfg[]; /* gPolyGeomMode_3EEC8 read alias: mode words [0], [1] */
void game_setupMode(char *arg0, s32 arg1, s32 arg2) {
    char *a1;
    char *v1;
    int v;
    XCALL(func_00000000_2DA4_v, memtrack_begin)(&sStr_ImgFileMgr_73EC, 0);
    XCALL(func_00000000_2DA4_v, memtrack_end)();
    D_2DA4_obj = FCALL(TrickTable_ctor_0000C2D4)(0, &sStr_rom_tricks_bin_73F8);
    v1 = (char *)WORLDREG(arg0)->bgRoot;
    a1 = (char *)D_2DA4_obj_rd;
    XCALL(func_00000000_2DA4_v, list_append)(v1 + 0x10, a1);
    if (NODE(a1)->parent != 0) {
        NODE(a1)->unk04 = 1;
    }
    NODE(a1)->parent = (Node *)v1;
    XCALL(func_00000000_2DA4_v, AnimDataMgr_releaseFrames)(D_2DA4_obj);
    v = gPolyGeomMode_3EEC8_2DA4_cfg[1]; /* [1]: the geometry mode word */
    v = v | 0x80000 | 0x2000 | 0x20000;
    ((int *)&gPolyGeomMode_3EEC8)[1] = v;
    switch (arg1) {
    case 2: XCALL(func_00000000_2DA4_v, game_setupSolo)(arg0, arg2); break;
    case 4: XCALL(func_00000000_2DA4_v, game_setupReplay)(arg0, arg2); break;
    case 12: XCALL(func_00000000_2DA4_v, game_setupReplay)(arg0, arg2); break;
    case 3: XCALL(func_00000000_2DA4_v, game_setupVsGhost)(arg0, arg2); break;
    case 5: XCALL(func_00000000_2DA4_v, game_setupVsCpu)(arg0, arg2); break;
    case 6: XCALL(func_00000000_2DA4_v, game_setupVs2P)(arg0, arg2); break;
    case 1:
        XCALL(func_00000000_2DA4_v, game_setupEmpty)(arg0, 0);
        a1 = (char *)D_2DA4_obj; if (1) { a1 += 0x18; } *(int *)a1 = *(int *)a1 | 4;
        break;
    case 9: XCALL(func_00000000_2DA4_v, game_setupDemoSolo)(arg0, arg2); break;
    case 10: XCALL(func_00000000_2DA4_v, game_setupDemoRace)(arg0, arg2); break;
    case 11: XCALL(func_00000000_2DA4_v, game_setupDemoSplit)(arg0, arg2); break;
    default:
        XCALL(func_00000000_2DA4_v, sys_fatal)(&sStr_unknown_GAME_MODE_7408);
        break;
    }
}

/* game_createLights - verified structural decode (0x228, 138 insns,
 * multi-widget builder + per-child init). Builder+register family
 * (cf. WorldRegistry_create_00001E08 / MapProp_hostioParms).
 *   void game_createLights(St *s1) {
 *       func_00000000(&sStr_Lights_741C, 0);            // pre-init
 *       c = func_00000000(0, &sStr_Ambient_Light_7424);        // build child 0
 *       s1->0x90 = c;
 *       f32 k = *(f32*)(snd_calcEmitterDistPan_000003F8 + 0x12C);
 *       c->0x124 = c->0x128 = c->0x12C = k;       // colour rgb
 *       c->0x130 = 1.0f;                          // alpha
 *       c->0xD0  = 1;                             // type tag
 *       c->0xC4 |= 0x40;  c->0xC4 |= 0x80;        // flag bits
 *       c = func_00000000(0, &sStr_Infinite_Light_7434);        // build child 1
 *       s1->0x94 = c;
 *       c->0x124..0x130 = 1.0f;                   // white
 *       c->0xD0  = 2;
 *       c->0xC4 |= 0x40; ...
 *       ... (repeats for sStr_Shadow_Light_7444 / sStr_lighttarget_7454 children) ...
 *   }
 * Struct-typing reference: s1->0x90 (144) / s1->0x94 (148) / ... =
 * slots holding the built child widgets (sStr_Ambient_Light_7424/7434/7444/
 * 7454 = each child's key/descriptor data). Per child: ->0x124
 * (292) / 0x128 (296) / 0x12C (300) f32 = RGB colour (child 0 uses
 * the global snd_calcEmitterDistPan_000003F8+0x12C value; later children use 1.0),
 * ->0x130 (304) f32 = 1.0 alpha, ->0xD0 (208) s32 = type tag
 * (1,2,...), ->0xC4 (196) u32 flags (bits 0x40 then 0x80 OR'd in).
 * sStr_Lights_741C = pre-init datum. EXACT (see the comment on the body). */
/* EXACT 2026-10-16 (agent-c, 75.4 -> 100). The old "held float-pool base"
 * cap was the shared snd_calcEmitterDistPan_000003F8 base: three scalar f32 globals pinned at
 * the real offsets (kRo_game_createLights_524/528/52C = snd_calcEmitterDistPan_000003F8+0x12C/0x130/0x134)
 * each get their own `lui at; lwc1`. Other levers:
 *  - the three registration calls take a base-0 object (gGfxLib_2F90_reg, one
 *    symbol: the target holds it in s0), not a literal 0; m->0x28 is the
 *    base-0 address LightTarget_vtbl_2F90_m28, and the s1->0x90 link call passes p90 as a1;
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
extern char sStr_Lights_741C, sStr_Ambient_Light_7424, sStr_Infinite_Light_7434, sStr_Shadow_Light_7444, sStr_lighttarget_7454;
extern float kRo_game_createLights_524, kRo_game_createLights_528, kRo_game_createLights_52C;
extern char gGfxLib_2F90_reg, LightTarget_vtbl_2F90_m28;
extern void func_00000000_2F90_v();
extern void func_00000000_2F90_f(char *, char *, float, float, float);
void game_createLights(char *s1) {
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
    XCALL(func_00000000_2F90_v, memtrack_begin)(&sStr_Lights_741C, 0);
    c = (char *)FCALL(BaseLight_ctor)(0, &sStr_Ambient_Light_7424);
    {
        float k = kRo_game_createLights_524;
        WORLDREG(s1)->light0 = (s32)c;
        ((BaseLight *)c)->parms.col[2] = k;
        ((BaseLight *)c)->parms.col[1] = k;
        ((BaseLight *)c)->parms.col[0] = k;
        ((BaseLight *)c)->parms.col[3] = 1.0f;
    }
    p90 = (char *)WORLDREG(s1)->light0; ((BaseLight *)p90)->mask = 1;
    p90 = (char *)WORLDREG(s1)->light0; if (1) { p90 = (char *)&((BaseLight *)p90)->flags; } *(int *)p90 |= 0x40;
    p90 = (char *)WORLDREG(s1)->light0; if (1) { p90 = (char *)&((BaseLight *)p90)->flags; } *(int *)p90 |= 0x80;
    c = (char *)FCALL(BaseLight_ctor)(0, &sStr_Infinite_Light_7434);
    WORLDREG(s1)->light1 = (s32)c;
    ((BaseLight *)c)->parms.col[3] = 1.0f;
    ((BaseLight *)c)->parms.col[2] = 1.0f;
    ((BaseLight *)c)->parms.col[1] = 1.0f;
    ((BaseLight *)c)->parms.col[0] = 1.0f;
    p90 = (char *)WORLDREG(s1)->light1; ((BaseLight *)p90)->mask = 2;
    p90 = (char *)WORLDREG(s1)->light1; if (1) { p90 = (char *)&((BaseLight *)p90)->flags; } *(int *)p90 |= 0x40;
    o = (char *)FCALL(BaseLight_ctor)(0, &sStr_Shadow_Light_7444);
    vec[0] = 0.0f;
    vec[2] = 0.0f;
    vec[1] = kRo_game_createLights_528;
    XCALL(func_00000000_2F90_v, math_mtxAddTranslation)(o + 0x70, vec);
    c = o; if (1) { c = (char *)&((BaseLight *)c)->flags; } *(int *)c |= 0x40;
    XCALL(func_00000000_2F90_v, gfx_setShadowLight)(&gGfxLib_2F90_reg, WORLDREG(s1)->light1);
    XCALL(func_00000000_2F90_v, gfx_listAppend_0004E214)(&gGfxLib_2F90_reg, WORLDREG(s1)->light0);
    XCALL(func_00000000_2F90_v, gfx_listAppend_0004E214)(&gGfxLib_2F90_reg, WORLDREG(s1)->light1);
    p90 = (char *)WORLDREG(s1)->light0;
    o = (char *)WORLDREG(s1)->bgRoot;
    XCALL(func_00000000_2F90_v, list_append)(o + 0x10, p90);
    if (NODE(p90)->parent != 0) {
        NODE(p90)->unk04 = 1;
    }
    NODE(p90)->parent = (Node *)o;
    o = (char *)FCALL(mem_new)(0x16C);
    if (o != 0) {
        XCALL(func_00000000_2F90_f, QNode_ctor)(o, &sStr_lighttarget_7454, 0.0f, 0.0f, 0.0f);
        k2 = kRo_game_createLights_52C;
        NODE(o)->vptr = (char *)&LightTarget_vtbl_2F90_m28; ((LightTarget *)o)->mask = 0xFFFF;
        ((LightTarget *)o)->unk108[0] = k2;
        ((LightTarget *)o)->unk108[1] = k2;
        ((LightTarget *)o)->unk108[2] = k2;
        ((LightTarget *)o)->scale = 1.0f;
    }
    n2 = WORLDREG(s1)->bgRoot;
    XCALL(func_00000000_2F90_v, list_append)(n2 + 0x10, o);
    if (NODE(o)->parent != 0) {
        NODE(o)->unk04 = 1;
    }
    NODE(o)->parent = (Node *)n2;
    XCALL(func_00000000_2F90_v, memtrack_end)();
}

void game_stub_000031B8(int a0) {
}

// cam_create(a0, mode, parent, rider, pad): builds the race camera (docs/1080_CAMERA.md section 2; agent-g run 485).
//   A "snowlookat" look-at node (0xB8: MNode + chase component at +0xB4). Modes 0 / 2: the game.uso CourseCam
//   (eye; target = the look-at node; "rom/cam_info.bin"), a SnowChaseActorCam under it (rider, rider->camSource,
//   "rom/cam_ca.bin"; stored at CourseCam.eyeChase) and a SnowChaseActorLook under the look-at node (anchor = the
//   CourseCam, cam = that SnowChaseActorCam, "rom/lookat_ca2.bin"; named "LookAtChase"). Mode 1: a RiderCam
//   (MCamera(lookat, 0, 500, -1500)) and a SnowChaseActor under the look-at node (rider, rider,
//   "rom/lookat_ca.bin"). Both bodies are added under parent, start at rider->startPos, and the eye is then
//   moved by (0, 0, -1000). Returns the camera (eye).
// Match history (the bytes come from the NCC donor):
//   fuzzy 27.5% -> 42.0% (2026-07) as cfe C; regalloc / literal-pool caps.
// 2026-10-24 agent-g run 164: C++ port src/bootup_uso/bootup_uso_ncc_*31C0*.cpp.
// 2026-11-04 agent-c run 271: EXACT via the IDO 7.1 NCC (C++) donor
// src/bootup_uso/bootup_uso_ncc_31C0.cpp, REPLACE_FUNC_BODY-spliced over the
// placeholder below on both build paths (the old cfe decode is in git history:
// a cfront-style ctor, mode switch 0/2 vs 1, attach + Vec3-copy tail).
typedef struct { int w; } SC4_; /* run 131 struct-copy lever (31C0); also used by a later NM body */
void *cam_create(int a0, int mode, char *parent, char *a3v, int p4) {
    volatile int r = 0;
    return 0;
}

/* game_create3DWindow - verified structural decode (0x150, 84 insns,
 * tagged-arg-block object builder + wire). Cross-symbol data refs:
 * stream_readS32_00000008+0x28 (global Vec3 slot), stream_readS32_00000008+0x24 (global
 * pointer slot), snd_calcEmitterDistPan_000003F8+0x140 (source Vec3). The C body uses
 * &func_NNNNNNNN + offset casts to recreate the data references.
 * Struct-typing reference: snd_calcEmitterDistPan_000003F8+0x140 = a global source
 * Vec3 (3 f32) copied (with a 0.0 4th lane) into the global slot
 * stream_readS32_00000008+0x28. a1 = an int[] params source: indices 0,1,2,
 * 3,4,5 and 6 (a1[6] OR'd with 0x2) feed the builder arg block,
 * interleaved with id tags 0x64/0x6D/0x6E/0x6F/0x70/0x71/0x73/0x74
 * (the func_00000000 builder is a tagged-vararg constructor; tag
 * 0x64 head, 0x6D terminator-ish). Result r: r->0x70 (112) = a2,
 * and r is published to the global stream_readS32_00000008+0x24. a0->0x84
 * (132) / a0->0x80 (128) = optional-callback gates; final
 * func_00000000(a0, r) attaches r to a0. sStr_3DWindow_74D0 = builder datum.
 * 2026-06-20 correctness fixes (67->68.6%): removed a spurious
 * `*(+0x24)=r` store (not in target); `r->0x70=a2` placed correctly
 * (2nd-call delay slot); the two optional-callback calls take TWO args
 * `func(r, a0->0x84)` / `func(r, a0->0x80)` (the 2nd arg was missing).
 * 2026-10-16 EXACT (lui-count sweep, docs/IDO_CODEGEN.md#lui-count-
 * placeholder-sweep-44de4: target 6 luis vs 3): the three source floats are
 * distinct scalar globals pinned at 0x538/0x53C/0x540 (one shared
 * &snd_calcEmitterDistPan_000003F8 base was CSE'd into one held register), and the Vec4 slot is
 * the zero-offset D_00000030 (== stream_readS32_00000008+0x28), so its address is held
 * in v0 with 0/4/8/C displacements as in the target. Each optional callback
 * reads its gate once (`if ((p = a0->0x84) != 0) f(r, p)`), and the r->0x70
 * store precedes the publish call (it lands in that jal's delay slot). */
extern char sStr_3DWindow_74D0;
extern f32 kRo_game_create3DWindow_538, kRo_game_create3DWindow_53C, kRo_game_create3DWindow_540;
/* UNNAMED: data-base D_00000030 = 0x30 is one pinned value for two Bss objects (sym324 +0x30 in game_create3DWindow / game_createProcessWindow, sym326 +0x30 in Process_exit); a pure rename would merge them, it needs a per-site split. */
extern float D_00000030[4];
extern char gSnowBoard_0000002C;
extern void stream_readS32_00000008();  /* used as data-symbol base */
extern void snd_calcEmitterDistPan_000003F8();  /* used as data-symbol base */
void *game_create3DWindow(char *a0, int *a1, int a2) {
    float *g;
    char *r;
    int p;
    g = D_00000030;
    g[0] = kRo_game_create3DWindow_538;
    g[1] = kRo_game_create3DWindow_53C;
    g[2] = kRo_game_create3DWindow_540;
    g[3] = 0.0f;
    r = (char *)FCALL(ScreenWindow_ctor)(0, 0x64, &sStr_3DWindow_74D0, 0x6D,
                              2, 0x6E, a1[0], a1[1], 0x6F, a1[2], a1[3],
                              0x70, a1[6] | 2, 0x71, 1, 0x73, a1[4],
                              0x74, a1[5], 0);
    ((ScreenWindow *)r)->win.camera = (char *)a2; /* the window's MCamera (a2 = cam_create's result) */
    FCALL(list_append)(&gSnowBoard_0000002C, r);
    if ((p = WORLDREG(a0)->bgRoot) != 0) FCALL(node_linkBack_0004E8D4)(r, p);
    if ((p = WORLDREG(a0)->fgRoot) != 0) FCALL(node_linkBack_0004E920)(r, p);
    FCALL(game_addWindow_00002088)(a0, r);
    return r;
}

/* game_createPlayers - verified structural decode (0xFC, 63 insns,
 * object factory + link + notify).
 *   void *game_createPlayers(void *a0, int a1, int a2, int a3,
 *                       int e0, f32 e1, f32 e2, int flags,
 *                       int e4, void *parent) {   // e* = stack args
 *       reloc_init(&sStr_Players_74DC);
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
 * stack args (lwc1/swc1 through sp+0x14/0x18). sStr_Players_74DC = init
 * datum. Caps <80: 5x func_00000000 reloc calls + f32 stack-arg
 * passing + beql branch-likely link guard + flags-bit branch.
 * Full body INCLUDE_ASM-preserved (.s = source of truth).
 * INCLUDE_ASM (no episode; tautology-trap rule). */
#ifdef NON_MATCHING
/* game_createPlayers: 5-call orchestrator. All 10 args home-spilled at entry; calls
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
extern char sStr_Players_74DC;
/* typed-float proto (0x0-alias): args 6,7 are single floats on the stack
 * (target swc1 0x14/0x18); K&R func_00000000 double-promotes them. */
extern void *func_3638_node(int, void *, int, int, int, float, float, int, int);
/* void-returning alias for the init call: kills the dead $v0 def so flags colors $v0 (38C0 recipe) */
extern void func_00000000_3638_v(void *);
void *game_createPlayers(int *arg0, void *arg1, int arg2, int arg3, int arg4, char *arg5,
                    float arg6, float arg7, int arg8, int arg9) {
    void *node;
    char *p;
    func_00000000_3638_v(&sStr_Players_74DC);
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
INCLUDE_ASM("asm/nonmatchings/bootup_uso", game_createPlayers);
#endif

/* game_setupDemoSolo - verified structural decode (0x18C, 99 insns,
 * multi-stage builder chain). LARGER SIBLING of game_setupSolo
 * (same alloc-0x80 + tagged-arg func_00000000 builder family,
 * idx*0x1C descriptor table, link via ->0x14/->0x4, r->0x8DC =
 * result).
 * Struct-typing reference (same family as game_setupSolo): s1->0x80
 * (128) handle passed through; s1->0x98 (152) -> cfg with f32
 * 0xC4 (196)/0xCC (204) builder params; s1->0x84 (132) = parent
 * for the r5 list-link (r5->0x14 (20) back-link, r5->0x4 (4) =1
 * when already-linked). r1->0x8DC (2268) = slot receiving the
 * idx-table build result r4. &D + a1*0x1C = a 28-byte descriptor
 * table indexed by a1; stream_readVec3_00000080+0x14 = a static datum.
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
 *  (2) arg4 of the r1 builder is a VALUE load *(stream_readVec3_00000080+0x14)
 *      (target lui/lw), not the address — gInputMgr_3734_v alias;
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
 * addend 0 vs 0x14) links to the ROM's 0x94 because gInputMgr_3734_v is pinned to
 * stream_readVec3_00000080+0x14 = 0x94; every other reloc resolves to 0. */
/* typed-float proto (0x0-alias): args 7,8 are single floats on the stack
 * (target swc1 0x18/0x1C); K&R func_00000000 double-promotes them. */
extern char *func_3734_r1(void *, int, int, int, void *, int, float, float, int, int);
extern int gInputMgr_3734_v;  /* VALUE load at stream_readVec3_00000080+0x14 (target lui/lw, not lui/addiu) */
/* void-returning alias for the reg call: kills the dead $v0 def so cfg colors $v0 (38C0 recipe) */
extern void func_00000000_3734_v(void *, void *);
void game_setupDemoSolo(char *s1, int a1) {
    char *s0;
    char *r1, *r2, *r3, *r4;
    volatile int pad1, pad2, pad3, pad4;  /* 4 ghost home words 0x3C-0x48 (phantom-slot lever) */
    char *p;
    s0 = (char*)FCALL(mem_new)(0x80);
    if (s0 != 0) {
        FCALL(pad_open)(s0, 0);   /* alloc-fail SKIPS ONLY THIS (target beqz +4, 38C0 recipe) */
    }
    XCALL(func_00000000_3734_v, input_addRacePad)(&XDATA(D_00000000, port_bootup_bss, 0xF8), s0);
    r1 = func_3734_r1(s1, 2, 1, gInputMgr_3734_v,
                      s0, WORLDREG(s1)->fgRoot,
                      WORLD_MAP(s1)->start[0],
                      WORLD_MAP(s1)->start[2],
                      0x58005, 0x1B);
    r2 = (char*)FCALL(cam_create)(s1, 0, WORLDREG(s1)->fgRoot, r1, s0);
    r3 = (char*)FCALL(cam_create)(s1, 1, WORLDREG(s1)->fgRoot, r1, s0);
    r4 = (char*)FCALL(game_create3DWindow)(s1, (char*)&XDATA(D_00000000, port_bootup_data, 0x7290) + a1 * 0x1C, r2);
    s0 = (char*)FCALL(mem_new)(0x80);   /* merged $s0 web: o -> sub -> r5 */
    if (s0 != 0) {
        FCALL(pad_open)(s0, 1);
    }
    s0 = (char*)FCALL(CamDirector_ctor)(0, s0, r1, r4, r2, r3);
    p = (char *)WORLDREG(s1)->bgRoot;
    FCALL(list_append)(p + 0x10, s0);
    if (NODE(s0)->parent != 0) NODE(s0)->unk04 = 1;
    NODE(s0)->parent = (Node *)p;
    SNOWB(r1)->camera = r4;
}

/* game_setupSolo - verified structural decode (0x118, 70 insns,
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
 * the K&R double-promote. Sibling of game_setupDemoSolo (double-promote-scan vein).
 * 2026-07-15 (80.1->91.3, agent-g): TWO DECODE ERRORS + one lever:
 *  (1) alloc-fail does NOT return — target beq v0,zero,+4 skips ONLY the
 *      func(o,1) init call and continues the whole builder chain;
 *  (2) the r2 builder call has FIVE args: func(s1, 2, s1->0x80, r, o) —
 *      arg3 = r (or a3,v0 in the jal delay), o goes to the stack slot;
 *      matches sibling 39D8's decoded b3=builder(s2,2,s2->0x80,r1,o1);
 *  (3) array-decay retype `extern int gInputMgr_38c0_z[]` + indexed loads shares the
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
 * (gInputMgr_38c0_z pinned at the end of undefined_syms_auto.txt). */
/* typed-float proto (0x0-alias): args 7,8 are single floats on the stack
 * (target swc1 0x18); K&R func_00000000 double-promotes them. */
extern char *func_38c0_r(void *, int, int, int, void *, int, float, float, int, int);
extern int gInputMgr_38c0_z[];   /* D_z global — array-typed so the base is held in ONE reg */
#define SOLO_MGR ((InputMgr *)gInputMgr_38c0_z)
/* void-returning alias for reg_b: kills the dead $v0 def so Dz/cfg color v1/v0 */
extern void func_00000000_38C0_v(void *, void *, int);
void game_setupSolo(char *s1, int a1) {
    char *o = (char*)FCALL(mem_new)(0x80);
    char *r;
    char *r2;
    int *row;
    char *res;
    if (o != 0) {
        FCALL(pad_open)(o, 1);   /* alloc-fail SKIPS ONLY THIS (target beq +4) */
    }
    FCALL(input_addRacePad)(&XDATA(D_00000000, port_bootup_bss, 0xF8), o);          /* reg_a — D_x placeholder */
    XCALL(func_00000000_38C0_v, replay_beginRecordWithGhost)(&XDATA(D_00000000, port_bootup_bss, 0xF8), o, 0); /* reg_b — D_y placeholder (void alias) */
    r = func_38c0_r(s1, 0, SOLO_MGR->playerRider, SOLO_MGR->playerBoard,
                    o, WORLDREG(s1)->fgRoot,
                    WORLD_MAP(s1)->start[0],
                    WORLD_MAP(s1)->start[2],
                    (SOLO_MGR->playerFlags | 1) | 0x10000 | 0x40000, 0x1B);
    r2 = (char*)FCALL(cam_create)(s1, 2, WORLDREG(s1)->fgRoot, r, o);
    row = (int*)((char*)&XDATA(D_00000000, port_bootup_data, 0x7290) + a1 * 0x1C);  /* D_w placeholder */
    res = (char*)FCALL(game_create3DWindow)(s1, row, r2);
    SNOWB(r)->camera = res;
}

/* game_setupVsGhost: the player rider from InputMgr playerRider / playerFlags /
 * playerBoard (tag 0x1B), the ghost rider from ghostRider / ghostFlags / ghostBoard
 * (flags | 0x8008, tag 0x14), one camera window shared by both (+0x8DC).
 * game_setupVsGhost - verified structural decode (0x1A0, 104 insns,
 * multi-stage builder chain). MEMBER of the game_setupSolo /
 * game_setupDemoSolo builder-chain family (same alloc-0x80 + tagged-arg
 * func_00000000 builder pattern; flags = global|1|0x10000|0x40000,
 * tag 0x1B; s2 source, cfg = s2->0x98).
 * Struct-typing reference (same family as game_setupSolo /
 * game_setupDemoSolo): s2->0x80 (128) pass-through handle; s2->0x98
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
extern unsigned char gInputMgr_39d8_g[];  /* D_g placeholder alias — array-typed so base lives in $s0 */
#define VG_MGR ((InputMgr *)gInputMgr_39d8_g)
void game_setupVsGhost(char *s2, int a1) {
    char *o1;
    char *o2;
    char *r1, *r2, *b3, *f1;
    o2 = (char*)FCALL(mem_new)(0x80);  /* o2 is the MERGED $s1 web: o1-carrier, then o2 */
    if (o2 != 0) {
        FCALL(pad_open)(o2, 1);   /* alloc-fail SKIPS ONLY THIS (target beqz +4, 38C0 recipe) */
    }
    if (0) { func_00000000(&o1); }  /* address-escape: o1 memory-homed at 0x4C */ /* UNNAMED: no-reloc dead if (0) call kept for the frame layout: it emits no jal */
    o1 = o2;  /* carrier ($s1) -> home store at the branch join */
    o2 = (char*)FCALL(mem_new)(0x80);
    if (o2 != 0) {
        FCALL(pad_open)(o2, 0);
    }
    FCALL(input_addRacePad)(gInputMgr_39d8_g, o1);
    FCALL(input_addRacePad)(gInputMgr_39d8_g, o2);
    FCALL(replay_beginRecordWithGhost)(gInputMgr_39d8_g, o1, o2);
    r1 = func_39d8_r(s2, 0, VG_MGR->playerRider, VG_MGR->playerBoard,
                     o1, WORLDREG(s2)->fgRoot,
                     WORLD_MAP(s2)->start[0],
                     WORLD_MAP(s2)->start[2],
                     (VG_MGR->playerFlags | 1) | 0x10000 | 0x40000, 0x1B);
    r2 = func_39d8_r(s2, 0, VG_MGR->ghostRider, VG_MGR->ghostBoard,
                     o2, WORLDREG(s2)->fgRoot,
                     WORLD_MAP(s2)->start[0],
                     WORLD_MAP(s2)->start[2],
                     (VG_MGR->ghostFlags | 0x8) | 0x8000, 0x14);
    SNOWB(r2)->dnode.unk38C = 0;
    b3 = (char*)FCALL(cam_create)(s2, 2, WORLDREG(s2)->fgRoot, r1, o1);
    f1 = (char*)FCALL(game_create3DWindow)(s2, (char*)&D_00000000 + a1 * 0x1C, b3);
    SNOWB(r1)->camera = f1;
    SNOWB(r2)->camera = f1;
}

/* game_setupReplay - verified structural decode (0x1C4, 113 insns,
 * multi-stage builder chain). MEMBER of the game_setupSolo /
 * game_setupDemoSolo / game_setupVsGhost builder-chain family (same
 * alloc-0x80 + tagged-arg func_00000000 builder pattern).
 * Struct-typing reference (same family as game_setupSolo /
 * game_setupDemoSolo / game_setupVsGhost): s1->0x80 (128) pass-through
 * handle; s1->0x98 (152) -> cfg with f32 0xC4 (196) / 0xCC (204)
 * builder params. CFG = the InputMgr (bootup Bss 0xF8, RP_MGR): its
 * ghostRider / ghostFlags / ghostBoard bytes (0x181..0x183, written by titproc's
 * vs-ghost states) build the replayed rider (flags | 0x8001|0x10000|0x40000). o = the 0x80-byte builder object; a1 indexes the
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
 *      typed gInputMgr_CFG_3b78 base is CSE'd into v1 by itself) -> r1/b2/b3/f1 spill
 *      homes 0x58..0x4C exact;
 *  (3) four unused volatile pads between f1 and v1_84 = the ghost words that
 *      put v1_84's home at 0x38;
 *  (4) NEW: the flag constant spelled `| 1 | 0x8000` (not `| 0x8001`): the
 *      folded intermediate burns one t-ring register, so the lbu/ori/or chain
 *      numbers t8/t9/t0/t1 like the target instead of t7.. (every later
 *      temp was one ring slot early). See docs/IDO_CODEGEN.md
 *      #split-or-constant-ring-burn-3b78. */
extern char *func_3b78_r(char *, int, int, int, char *, int, float, float, int, int);
extern unsigned char gInputMgr_CFG_3b78[];
#define RP_MGR ((InputMgr *)gInputMgr_CFG_3b78)
extern void func_00000000_3B78_v(void *, void *, int);
void game_setupReplay(char *s1, int a1) {
    char *o;
    char *r1, *b2, *b3, *f1;
    volatile int pad1, pad2, pad3, pad4;  /* ghost home words (phantom-slot lever) */
    int v1_84;
    o = (char*)FCALL(mem_new)(0x80);
    if (o != 0) {
        FCALL(pad_open)(o, 0);
    }
    FCALL(input_addRacePad)(gInputMgr_CFG_3b78, o);
    XCALL(func_00000000_3B78_v, replay_beginPlayback)(gInputMgr_CFG_3b78, o, 0);
    r1 = func_3b78_r(s1, 0,
                     RP_MGR->ghostRider,
                     RP_MGR->ghostBoard,
                     o, WORLDREG(s1)->fgRoot,
                     WORLD_MAP(s1)->start[0], WORLD_MAP(s1)->start[2],
                     (RP_MGR->ghostFlags | 1 | 0x8000) | 0x10000 | 0x40000, 0x1B);
    b2 = (char*)FCALL(cam_create)(s1, 0, WORLDREG(s1)->fgRoot, r1, o);
    b3 = (char*)FCALL(cam_create)(s1, 1, WORLDREG(s1)->fgRoot, r1, o);
    f1 = (char*)FCALL(game_create3DWindow)(s1, (char*)&D_00000000 + a1 * 0x1C, b2);
    ((MCamera *)b2)->fov = 85.0f;
    o = (char*)FCALL(mem_new)(0x80);
    if (o != 0) {
        FCALL(pad_open)(o, 1);
    }
    o = (char*)FCALL(CamDirector_ctor)(0, o, r1, f1, b2, b3);
    v1_84 = WORLDREG(s1)->bgRoot;
    FCALL(list_append)(v1_84 + 0x10, o);
    if (NODE(o)->parent != 0) {
        NODE(o)->unk04 = 1;
    }
    NODE(o)->parent = (Node *)v1_84;
    SNOWB(r1)->camera = f1;
}

/* game_setupVsCpu - verified structural decode (0x1C4, 113 insns,
 * multi-stage builder chain). MEMBER of the game_setupSolo /
 * game_setupDemoSolo / game_setupVsGhost / game_setupReplay builder-chain
 * family (same alloc-0x80 x2 + register-&D + tagged-arg
 * func_00000000 builder pattern; variant params).
 * Struct-typing reference (same family as game_setupSolo et al.):
 * s1->0x80 (128) pass-through handle; s1->0x98 (152) -> cfg with
 * f32 0xC4 (196) / 0xCC (204) builder params; o1/o2 = 0x80-byte
 * builder objects cross-wired through the tagged func_00000000
 * builder (saved at sp+0x4C across the alloc pair); a1 indexes
 * the 0x1C-stride descriptor table per the family pattern. */
/* EXACT (agent-c run 115). Identities from the bootup.uso TextReloc: alloc =
 * func_055750, ctor = func_07BA00, list adds = func_011CA4 / func_0128AC on
 * import_000200FC (= &D_00000000, whose +0x4C/0x50/0x54/0x17D/0x17F fields
 * are read through the same held base), spawn = game_createPlayers (x2), link =
 * cam_create, attach = game_create3DWindow with &D_007294 + a1 * 0x1C (zero
 * alias bu_3d3c_d7294). The two new-expression results go through ONE temp t (target
 * s2) with o1 copied out to the frame; the camera reads are inline per arg
 * (a cfg local hoists the load); decl order puts o1 at sp+76 and r at sp+64
 * (seven declared locals, cfg unused but it holds the 0x58 frame). */
extern char *func_00000000_3d3cm(char *, int, int, int, char *, int, float, float, int, int);
extern char bu_3d3c_d7294[];
void game_setupVsCpu(char *s1, int a1) {
    char *o2, *o3, *o1, *o4, *t, *r, *cfg;
    t = (char*)FCALL(mem_new)(0x80);
    if (t != 0) FCALL(pad_open)(t, 1);
    o1 = t;
    t = (char*)FCALL(mem_new)(0x80);
    if (t != 0) FCALL(pad_open)(t, 0);
    o2 = t;
    FCALL(input_addRacePad)(&gInputMgr, o1);
    FCALL(input_addRacePad)(&gInputMgr, o2);
    FCALL(replay_beginRecordWithGhost)(&gInputMgr, o1, 0);
    r = XCALL(func_00000000_3d3cm, game_createPlayers)(s1, 0, *(int*)((char*)&D_00000000 + 0x4C), *(int*)((char*)&D_00000000 + 0x54),
                             o1, WORLDREG(s1)->fgRoot, WORLD_MAP(s1)->start[0] - 500.0f, WORLD_MAP(s1)->start[2],
                             *(int*)((char*)&D_00000000 + 0x50) | 1 | 0x10000 | 0x40000, 0x1B);
    o3 = XCALL(func_00000000_3d3cm, game_createPlayers)(s1, 2, *(unsigned char*)((char*)&D_00000000 + 0x17D),
                              *(unsigned char*)((char*)&D_00000000 + 0x17F), o2, WORLDREG(s1)->fgRoot,
                              WORLD_MAP(s1)->start[0] + 500.0f, WORLD_MAP(s1)->start[2], 0x8004, 0x1D);
    SNOWB(o3)->health = 0x989680;
    SNOWB(r)->rival = SNOWB(o3);
    SNOWB(o3)->rival = SNOWB(r);
    o4 = (char*)FCALL(game_create3DWindow)(s1, bu_3d3c_d7294 + a1 * 0x1C, FCALL(cam_create)(s1, 2, WORLDREG(s1)->fgRoot, r, o1));
    SNOWB(r)->camera = o4;
    if (1) { }
    if (1) { }
    SNOWB(o3)->camera = o4;
}

/* game_setupVs2P - verified structural decode (0x21C, 135 insns,
 * multi-stage builder chain). MEMBER of the game_setupSolo /
 * game_setupDemoSolo / game_setupVsGhost / game_setupReplay / game_setupVsCpu
 * builder-chain family (same alloc-0x80 x2 + register-&D +
 * tagged-arg func_00000000 builder pattern; variant params).
 * Struct-typing reference (same family as game_setupSolo et al.):
 * s1->0x80 (128) pass-through handle; s1->0x98 (152) -> cfg with
 * f32 0xC4 (196) builder param, here biased by -500.0f
 * (0x43FA0000); D_g (a global) ->0x4C (76)/0x50 (80)/0x54 (84)
 * global build params; tag 0x15. o1/o2 = 0x80-byte builder
 * objects (init types 1 and 2) cross-wired via the tagged builder;
 * a1 indexes the 0x1C-stride descriptor table per the family.
 */
/* EXACT (agent-c run 115). Identities from the bootup.uso TextReloc (same
 * chain as game_setupVsCpu): alloc func_055750, ctor func_07BA00, list adds
 * func_011CA4 x2 + func_0129A0 on import_000200FC, spawn game_createPlayers x2,
 * link cam_create x2, attach game_create3DWindow x2 on &D_007294 + a1 * 0x1C
 * (zero alias), r->0xC = sStr_Player1_74E4 / sStr_Player2_74EC (baked). No early returns:
 * the two new-expression results share temp t and are copied into an object
 * ARRAY o[2] (slots 104/108, re-read from memory for the link calls); b3/f1
 * are their own variables (sharing t's register drops the param to s2); the
 * if (1) { } after the copies lowers t below r1 (s3 vs s2). */
extern char sStr_Player1_74E4[], sStr_Player2_74EC[], bu_3f00_d7294[];
extern char *func_00000000_3d3cm(char *, int, int, int, char *, int, float, float, int, int);
void game_setupVs2P(char *s1, int a1) {
    char *o[2], *t, *r1, *r2, *x, *y, *b4, *f2, *row;
    t = (char*)FCALL(mem_new)(0x80);
    if (t != 0) FCALL(pad_open)(t, 1);
    o[0] = t;
    t = (char*)FCALL(mem_new)(0x80);
    if (t != 0) FCALL(pad_open)(t, 2);
    o[1] = t;
    if (1) { }
    FCALL(input_addRacePad)(&gInputMgr, o[0]);
    FCALL(input_addRacePad)(&gInputMgr, t);
    FCALL(replay_beginRecord2P)(&gInputMgr, o[0], t);
    r1 = XCALL(func_00000000_3d3cm, game_createPlayers)(s1, 0, *(int*)((char*)&D_00000000 + 0x4C), *(int*)((char*)&D_00000000 + 0x54),
                             o[0], WORLDREG(s1)->fgRoot, WORLD_MAP(s1)->start[0] - 500.0f, WORLD_MAP(s1)->start[2],
                             *(int*)((char*)&D_00000000 + 0x50), 0x15);
    r2 = XCALL(func_00000000_3d3cm, game_createPlayers)(s1, 1, *(int*)((char*)&D_00000000 + 0x58), *(int*)((char*)&D_00000000 + 0x60),
                             t, WORLDREG(s1)->fgRoot, WORLD_MAP(s1)->start[0] + 500.0f, WORLD_MAP(s1)->start[2],
                             *(int*)((char*)&D_00000000 + 0x5C), 0x15);
    NODE(r1)->name = (const char *)sStr_Player1_74E4;
    NODE(r2)->name = (const char *)sStr_Player2_74EC;
    SNOWB(r1)->rival = SNOWB(r2);
    SNOWB(r2)->rival = SNOWB(r1);
    x = (char*)FCALL(cam_create)(s1, 2, WORLDREG(s1)->fgRoot, r1, o[0]);
    b4 = (char*)FCALL(cam_create)(s1, 2, WORLDREG(s1)->fgRoot, r2, o[1]);
    ((MCamera *)x)->fov = 55.0f;
    ((MCamera *)b4)->fov = 55.0f;
    row = bu_3f00_d7294 + a1 * 0x1C;
    y = (char*)FCALL(game_create3DWindow)(s1, row, x);
    f2 = (char*)FCALL(game_create3DWindow)(s1, row + 0x1C, b4);
    SNOWB(r1)->camera = y;
    SNOWB(r2)->camera = f2;
    ((ScreenWindow *)f2)->unk1B8 = (struct Window *)y; /* player 2's window points at player 1's */
}

/* game_setupDemoRace - verified structural decode (0x2B8, 174 insns,
 * randomized multi-stage builder chain). MEMBER of the
 * game_setupSolo / game_setupDemoSolo / game_setupDemoSplit builder-chain
 * family (alloc-0x80 x2 + register-&D + tagged-arg func_00000000
 * builder; rand-scaled params, same shape as game_setupDemoSplit).
 * Struct-typing reference (same family as game_setupSolo /
 * game_setupDemoSplit): o1/o2 = 0x80-byte builder objects (sp+0x68);
 * rand01() = func_00000000() 0..1 f32; snd_calcEmitterDistPan_000003F8+0x14C /
 * +0x150 = f32 scale consts (one -> int table index via trunc,
 * one -> float param); stream_readVec3_00000080+0x10 = descriptor table
 * indexed by idx*4; tag 0x58005; flag 0x3. Per-call random
 * descriptor selection (same as game_setupDemoSplit).
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
 *  (d) descriptor table offset is stream_readVec3_00000080+0x10 (not +0x90).
 *  LEVERS: o1/o2/o3/big = ONE serial temp `p` (s0, target's or-s0 beq delays);
 *  homed locals f1/b4/b3/idx2/idx1/r1/r2/o1 = local ARRAY A[8] (IDO never
 *  promotes array elements — struct fields DO get scalarized/promoted, plain
 *  locals stole s-regs); frame gap filled with volatile padA[1] above / padB[2]
 *  below A (single volatile int gets trimmed; arrays don't); per-region
 *  `q = A[5]` temps reproduce block-granular r1 reloads + a3 reuse; E68-style
 *  reloc-identity split (&snd_soundObjCreate_0000057C-0x34 / &stream_readF32_00000044+0x4C) busts the
 *  in-TU pool/table base CSE across the 2 uses (frees s1 -> param, kills s2).
 *  RESIDUAL (~12%, hard cap, BF8C class): expected pool/table loads are
 *  DIRECT-form `lui at,%hi(sym+K); lwc1 %lo` — in-TU-defined-function base
 *  means our C spells pointer arith -> addiu-form (+1 insn per use) and the
 *  2nd-use anchors have wrong reloc name/lo16 (semantically same address);
 *  downstream f10/f18 ring swap + mfc1 t8-vs-a2 idx-temp coloring couple to
 *  that. Struct-cast ((P*)func)->field does NOT fold either. */
/* EXACT (agent-c run 115). The game_setupDemoSplit shape (reloc identities,
 * per-site pinned aliases for the RoData scales +0x544/+0x548 and the 0x90
 * descriptor table, zero alias for the second list add, spawn results in
 * r[2], block temps for the cross link) plus the tail: a third new-expression
 * (ctor flag 1) passed to func_002420(0, t, r[0], f, b3, b4), linked into
 * s1->0x84 + 0x10 by func_07ACE0. */
extern char bu_411c_d7294[], gInputMgr_411c_d00_b;
extern int gInputMgr_411c_t90[];
extern f32 kRo_game_setupDemoRace_544, kRo_game_setupDemoRace_548;
extern f32 func_00000000_411c_f(void);
extern char *func_00000000_3d3cm(char *, int, int, int, char *, int, float, float, int, int);
void game_setupDemoRace(char *s1, int a1) {
    char *t, *o1, *r[2];
    int idx1, idx2;
    char *b3, *b4, *f;
    int unused[4];  /* frame words between f (sp+76) and the p84 home (sp+56) */
    char *p84;
    t = (char*)FCALL(mem_new)(0x80);
    if (t != 0) FCALL(pad_open)(t, 0);
    o1 = t;
    t = (char*)FCALL(mem_new)(0x80);
    if (t != 0) FCALL(pad_open)(t, 0);
    FCALL(input_addRacePad)(&gInputMgr, o1);
    FCALL(input_addRacePad)(&gInputMgr_411c_d00_b, t);
    idx1 = (int)(XCALL(func_00000000_411c_f, sys_randf)() * kRo_game_setupDemoRace_544);
    idx2 = (int)(XCALL(func_00000000_411c_f, sys_randf)() * kRo_game_setupDemoRace_548 + (float)idx1 + 1.0f) % 3;
    r[0] = XCALL(func_00000000_3d3cm, game_createPlayers)(s1, 0, idx1, gInputMgr_411c_t90[idx1], o1, WORLDREG(s1)->fgRoot,
                             WORLD_MAP(s1)->start[0] - 500.0f, WORLD_MAP(s1)->start[2], 0x58005, 0x1B);
    r[1] = XCALL(func_00000000_3d3cm, game_createPlayers)(s1, 2, idx2, gInputMgr_411c_t90[idx2], t, WORLDREG(s1)->fgRoot,
                             WORLD_MAP(s1)->start[0] + 500.0f, WORLD_MAP(s1)->start[2], 0x48024, 0x1B);
    { char *a = r[0], *b = r[1]; SNOWB(a)->rival = SNOWB(b); SNOWB(b)->rival = SNOWB(a);
    b3 = (char*)FCALL(cam_create)(s1, 0, WORLDREG(s1)->fgRoot, a, o1); }
    b4 = (char*)FCALL(cam_create)(s1, 1, WORLDREG(s1)->fgRoot, r[0], o1);
    f = (char*)FCALL(game_create3DWindow)(s1, bu_411c_d7294 + a1 * 0x1C, b3);
    t = (char*)FCALL(mem_new)(0x80);
    if (t != 0) FCALL(pad_open)(t, 1);
    t = (char*)FCALL(CamDirector_ctor)(0, t, r[0], f, b3, b4);
    p84 = (char *)WORLDREG(s1)->bgRoot;
    FCALL(list_append)(p84 + 0x10, t);
    if (NODE(t)->parent != 0) NODE(t)->unk04 = 1;
    NODE(t)->parent = (Node *)p84;
    SNOWB(r[0])->camera = f;
    SNOWB(r[1])->camera = f;
}

/* game_setupDemoSplit - verified structural decode (0x258, 150 insns,
 * multi-stage builder chain w/ randomized params). MEMBER of the
 * game_setupSolo / game_setupDemoSolo / game_setupVsGhost / game_setupReplay
 * builder-chain family (alloc-0x80 x2 + register-&D + tagged-arg
 * func_00000000 builder).
 * Struct-typing reference (same family as game_setupSolo et al.):
 * o1/o2 = 0x80-byte builder objects (saved sp+0x58); rand01() =
 * func_00000000() returning a 0..1 f32; snd_calcEmitterDistPan_000003F8+0x154 /
 * +0x158 = f32 scale constants (one yields an int table index via
 * trunc, one a float param); stream_readVec3_00000080+0x10 = a descriptor
 * table indexed by idx*4; tags 0x8004 / 0x15 + flag bits drive the
 * builder. Randomized variant of the family.
 * (Historical notes below; see the EXACT comment for the final shape.)
 *
 *
 * 2026-06-02 FULL CHAIN DECODED 27.6->55.5% (+27.9pp): two rng() calls give
 * idx1=(int)(r*F3F8->0x154) and idx2=(int)(rr*F3F8->0x158 + idx1 + 1.0f) % 3;
 * row1/row2 = *(stream_readVec3_00000080 + idxN*4 + 0x90) (descriptor table); r1/r2 =
 * builder(s0, 0/1, idxN, rowN, oN, s0->0x80, cfg->0xC4 -/+ 500.0f, cfg->0xCC,
 * 0x8004/0x8024, 0x15); r1<->r2 0x908 cross-link; b3/b4=builder(s0,2,s0->0x80,
 * rN,oN); bN->0x14C=55.0f; rowF=&D+a1*0x1C; f1/f2=builder(s0,rowF[+0x1C],bN);
 * rN->0x8DC=fN. All field stores byte-match; residual ~45% = regalloc. */
/* EXACT (agent-c run 115). Same reloc-identified chain as game_setupVsCpu /
 * game_setupVs2P (alloc, ctor, list add x2, RNG func_075EE4 x2, spawn
 * game_createPlayers x2, link cam_create x2, attach game_create3DWindow x2). The
 * RoData scales (pool +0x54C / +0x550) and the descriptor table (0x90) are
 * per-site data aliases pinned to their real offsets, and the second list
 * add gets its own zero alias of import_000200FC, so nothing is held in an
 * s-reg. The two spawn results are an ARRAY r[2] (frame 80/84); the cross
 * link and the first link call go through block temps a/b (a = a3). */
extern char bu_43d4_d7294[], gInputMgr_43d4_d00_b;
extern int gInputMgr_43d4_t90[];
extern f32 kRo_game_setupDemoSplit_54C, kRo_game_setupDemoSplit_550;
extern f32 func_00000000_43d4_f(void);
extern char *func_00000000_3d3cm(char *, int, int, int, char *, int, float, float, int, int);
void game_setupDemoSplit(char *s0, int a1) {
    char *t, *o1, *r[2];
    int idx1, idx2;
    char *f1, *row, *b3, *b4;
    t = (char*)FCALL(mem_new)(0x80);
    if (t != 0) FCALL(pad_open)(t, 0);
    o1 = t;
    t = (char*)FCALL(mem_new)(0x80);
    if (t != 0) FCALL(pad_open)(t, 0);
    FCALL(input_addRacePad)(&gInputMgr, o1);
    FCALL(input_addRacePad)(&gInputMgr_43d4_d00_b, t);
    idx1 = (int)(XCALL(func_00000000_43d4_f, sys_randf)() * kRo_game_setupDemoSplit_54C);
    idx2 = (int)(XCALL(func_00000000_43d4_f, sys_randf)() * kRo_game_setupDemoSplit_550 + (float)idx1 + 1.0f) % 3;
    r[0] = XCALL(func_00000000_3d3cm, game_createPlayers)(s0, 0, idx1, gInputMgr_43d4_t90[idx1], o1, WORLDREG(s0)->fgRoot,
                             WORLD_MAP(s0)->start[0] - 500.0f, WORLD_MAP(s0)->start[2], 0x8004, 0x15);
    r[1] = XCALL(func_00000000_3d3cm, game_createPlayers)(s0, 1, idx2, gInputMgr_43d4_t90[idx2], t, WORLDREG(s0)->fgRoot,
                             WORLD_MAP(s0)->start[0] + 500.0f, WORLD_MAP(s0)->start[2], 0x8024, 0x15);
    { char *a = r[0], *b = r[1]; SNOWB(a)->rival = SNOWB(b); SNOWB(b)->rival = SNOWB(a);
    b3 = (char*)FCALL(cam_create)(s0, 2, WORLDREG(s0)->fgRoot, a, o1); }
    b4 = (char*)FCALL(cam_create)(s0, 2, WORLDREG(s0)->fgRoot, r[1], t);
    ((MCamera *)b3)->fov = 55.0f;
    ((MCamera *)b4)->fov = 55.0f;
    row = bu_43d4_d7294 + a1 * 0x1C;
    f1 = (char*)FCALL(game_create3DWindow)(s0, row, b3);
    b4 = (char*)FCALL(game_create3DWindow)(s0, row + 0x1C, b4);
    SNOWB(r[0])->camera = f1;
    SNOWB(r[1])->camera = b4;
}

void boot_readFloatParm_0000462C(char *a0) {
    int scratch;
    stream_readS32_000022E0(&scratch);
    stream_readF32_0000231C((int*)(a0 + 0x10));
}

void boot_readVec3Parm_0000465C(char *a0) {
    int scratch;
    stream_readS32_000022E0(&scratch);
    stream_readVec3_00002358((int*)(a0 + 0x10));
}

void boot_readQuadParm_0000468C(char *a0) {
    int scratch;
    stream_readS32_000022E0(&scratch);
    stream_readQuad_000023C8((int*)(a0 + 0x10));
}

void boot_readIntParm_000046BC(char *a0) {
    int scratch;
    stream_readS32_000022E0(&scratch);
    stream_readS32_000022E0((int*)(a0 + 0x10));
}

extern char D_00000000;

/* boot_ItemType_create_000046EC: 36-insn (0x90) entry-list constructor. BYTE-EXACT, ported
 * from its masked-hash twin donor edd_ItemType_create_000003BC (byte-identical
 * body, 0 non-reloc word diffs). The donor's clean C form (p2/head/p1 decl
 * order + char*-cast field access + p1[0x40/4] re-read) reproduces the target's
 * $a0/$v1 regalloc + 0x28 frame that the prior node[]/head[] array-index NM
 * body (89.31% ceiling) couldn't. Was once force-matched via 22-entry
 * INSN_PATCH (banned 2026-05-23); this is the genuine C match. */
void *boot_ItemType_create_000046EC(int *arg0) {
    int *p2;
    int *head;
    int *p1;
    p1 = (int*)FCALL(mem_new)(0x40);
    if (p1 != 0) {
        FCALL(ColourItem_ctor)(p1);
        NODE(p1)->vptr = (char *)&ColourItem_vtbl_75A0;
        ((ColourSubItem *)p1)->unk3C = 0;
    }
    p2 = p1;
    p1 = arg0;
    head = (int*)p1[0x40 / 4];
    if ((int*)p1[0x40 / 4] != 0) {
        FCALL(list_append)((char*)p2 + 0x10, head);
        if (NODE(head)->parent != 0) {
            NODE(head)->unk04 = 1;
        }
        NODE(head)->parent = (Node *)p2;
    }
    return p2;
}

/* Mountain_hostioParms - verified structural decode (0xE0, 56 insns,
 * slider/parameter registration). SIBLING of MapProp_hostioParms (same
 * registration-builder shape, here with float range/step params).
 * (reg = func_00000000; a3 = min/step as f32 bits, 5th/6th args =
 * max + flag on the stack at sp+0x10 / sp+0x14.)
 * Constants decoded: 0xC4FA0000 = -2000.0f, 0x44FA0000 = 2000.0f,
 * 0x3C23D70A = 0.01f, 0x3F800000 = 1.0f.
 * Struct-typing reference: sStr_Mountain_74F4/7500/7508/7510 = four
 * slider/param key data; bound object fields a0->0x2C (44) [range
 * +-2000.0, step 0], a0->0x34 (52) and a0->0x30 (48) [step 0.01,
 * max 1.0] - i.e. one wide unbounded-ish axis plus two fine 0..1
 * normalized params (entry 0 binds no field). Caps <80: 6x reloc +
 * 4x &D reloc + f32-const-as-(a3/stack)-arg passing.
 * INCLUDE_ASM remains build path. */
extern char sStr_Mountain_74F4, sStr_Pos_Y_7500, sStr_Scale_Y_7508, sStr_Scale_XZ_7510;
/* MATCHED 2026-06-02: typed-float proto (0x0-alias) makes args 4,5 pass as
 * single floats (a3 reg + swc1 0x10 stack) instead of K&R double-promote. */
extern void func_477c_reg(void *, void *, void *, float, float, int);
void Mountain_hostioParms(char *a0) {
    FCALL(hostio_beginGroup)(&gHostio, &sStr_Mountain_74F4, 0);
    func_477c_reg(&gHostio, &sStr_Pos_Y_7500, &((Mountain *)a0)->posY, -2000.0f,  2000.0f, 0);  /* "Pos.Y" */
    func_477c_reg(&gHostio, &sStr_Scale_Y_7508, &((Mountain *)a0)->scaleY,    0.01f,    1.0f, 0); /* "Scale.Y" */
    func_477c_reg(&gHostio, &sStr_Scale_XZ_7510, &((Mountain *)a0)->scaleXZ,    0.01f,    1.0f, 0); /* "Scale.XZ" */
    FCALL(hostio_endGroup)(&gHostio);
    FCALL(Node_hostioParms)(a0);
}

/* Mountain_display - verified structural decode (0xB8, 46 insns,
 * FP transform/draw helper).
 * Struct-typing reference: a0 = object; a0->0x2C/0x30/0x34 (44/48/52)
 * f32 source vector components; global root *(&D_a) ->0x70 (112) ptr
 * to a transform/state obj whose ->0xA0/0xA4/0xA8 (160/164/168) is an
 * f32 Vec3 scaled per-component by Dc->0x128/0x12C/0x130 (296/300/304)
 * f32 scale constants; stream_readVec3_00000080+0x20 = static string/param datum
 * passed to the reloc draw/print helper. Caps <80: FP-heavy mul.s/
 * add.s chains + 4x func_00000000 reloc calls + 2x &D reloc +
 * stream_readVec3_00000080+0x20 cross-symbol data ref. INCLUDE_ASM remains build
 * path. */
extern void stream_readVec3_00000080();  /* used as data-symbol base */
/* EXACT via an IDO 7.1 NCC (C++) donor: src/bootup_uso/bootup_uso_ncc_485C.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder (agent-c run 116): V3 ctor
 * forwarding to set(), b2 in an inner block (allocated after b1's pool slots). */
void Mountain_display(char *a0) {
    volatile char *r = a0;
}

void RiderCam_command_00004914(char *a0, int a1, char *a2) {
    ((RiderCam *)a0)->cam.mnode.local[3][0] = CAMITEM(a2)->pos[0];
    ((RiderCam *)a0)->cam.mnode.local[3][1] = CAMITEM(a2)->pos[1];
    ((RiderCam *)a0)->cam.mnode.local[3][2] = CAMITEM(a2)->pos[2];
    ((RiderCam *)a0)->mode = a1;
    ((RiderCam *)a0)->cam.mnode.local[3][1] += 100.0f;
}

/* RiderCam_update_00004948: camera transition tick keyed by obj->0x158 (snap / distance-table
 * blend / view-angle blend). EXACT via an IDO 7.1 NCC (C++) donor:
 * src/bootup_uso/bootup_uso_ncc_4948.cpp, REPLACE_FUNC_BODY-spliced over this
 * placeholder (agent-c run 125); the trailing all-zero pad word is a
 * SUFFIX_BYTES_FORCE word. */
extern float func_00000000_4948_ff(float); /* also used by a later NM body */
void RiderCam_update_00004948(char *obj) {
    volatile int r = 0;
}

void stream_readS32_00004FF0(int *dst) {
    int buf[2];
    FCALL(fs_gdlReadFile)(&gSysLib, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

void stream_readS32_0000502C(int *dst) {
    int buf[2];
    FCALL(fs_gdlReadFile)(&gSysLib, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

/* BYTE-EXACT 2026-05-31. 2-call wrapper: func(&D_7D94); func(0, a0). The
 * documented "3rd-arg-copy-vs-dead-home cap" was wrong — it never tried
 * struct-by-value. The target homes a1 to its outgoing slot (`sw a1,4(sp)`,
 * dead) which a single-int STRUCT-BY-VALUE arg (`func(0, *(struct OneI*)&a0)`)
 * reproduces exactly (same lever as the game_uso pair-homing, applied to one
 * int). Sibling item_createCfr identical. See docs/IDO_CODEGEN.md
 * #feedback-ido-struct-by-value-homes-arg-pair. */
extern char gStrHeliUso_7D94;
struct OneI { int x; };
void item_createHel(int a0) {
    FCALL(game_loadUsoOnce)(&gStrHeliUso_7D94);
    FCALL(HeliItemType_ctor)(0, *(struct OneI*)&a0); /* jal -> heli.uso Text+0x1D4 = heli.uso HeliItemType_ctor (TextReloc import, proven by name-placeholder-calls) */
}

/* 33-insn / 0x84 init+alloc+populate.
 *   func_00000000(&sStr_rom_heli_uso_7DA4);       ; init call
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
extern char sStr_rom_heli_uso_7DA4;
/* item_createHem — EXACT (33/33 post-link) 2026-07-03 agent-e. CRACKED the
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
 * .o-level residual = 1 reloc-class word [5]: addiu a0,a0,%lo(sStr_rom_heli_uso_7DA4);
 * sStr_rom_heli_uso_7DA4=0x7DA4 in undefined_syms_auto.txt -> links to 24847DA4 = target.
 * All jals zero-target K&R func_00000000. No new wiring needed.
 */
typedef struct { int v; } S50A0;
int item_createHem(int a0) {
  float buf[5];
  int *p;
  char pad[16];
  volatile int saved_a0;
  (void) pad;
  FCALL(game_loadUsoOnce)(&sStr_rom_heli_uso_7DA4);
  buf[1] = (0, 0.0f);
  buf[2] = 0.0f;
  buf[3] = 0.0f;
  buf[4] = 0.0f;
  saved_a0 = a0;
  p = (int *) FCALL(mem_new)(0x58);
  if (p != 0)
  {
    FCALL(ColourItemType_ctor)(p, *(S50A0*)&saved_a0, *((int *) (&XDATA(D_00000000, port_bootup_data, 0x7B48))), &buf[1]);
    NODE(p)->vptr = (char *)&XDATA(D_00000000, port_heli_data, 0x78);
  }
  return (int) p;
}

/* item_createBal - verified structural decode (0xB0, 44 insns,
 * alloc-cascade constructor with defensive-dead-check).
 * Struct-typing reference: object = 0x4C bytes; obj->0x28 (40) and
 * obj->0x48 (72) descriptor/vtable ptrs (&D, runtime-patched); arg
 * passed through (spilled sp+0x2C / sp+0x1C / sp+0x4) into the dead
 * arm's sub2. sStr_rom_balloon_uso_7DB4 = named init datum (label/string). Caps <80:
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
extern char sStr_rom_balloon_uso_7DB4;
/* item_createBal - alloc-cascade get-or-create constructor.
 * Reconstructed missing logic: the three descriptor stores
 * (obj->0x28 = A; obj->0x28 = B; obj->0x48 = C) target DISTINCT
 * registers (t9/t0/t1) -> three DISTINCT externs to defeat the
 * &D_00000000 address-CSE that collapses them onto one base reg.
 * The 0x48 second-alloc arm is provably-dead (kept under -g) and
 * is written as a get-or-create fall-through so IDO keeps it. */
extern char sStr_rom_balloon_uso_7DB4;
extern char D_00000000_a;
extern char D_00000000_b;
extern char D_00000000_c;
/* 2026-09-04 (agent-f, pass 2): frame 0x38 slot layout = three `volatile int`
 * pads interleaved in decl order (pad,obj,b1,pad,o2,pad,b2 -> homes
 * 0x30/0x2C/0x24/0x1C); the beqz-on-v0 + delay-slot `sw a1,4(sp)` schedule
 * comes from folding the get-or-create into one condition
 * (`o2 != 0 || (o2 = alloc(0x48)) != 0`); tail addiu order = temp-first `t = &c`;
 * obj-reload placement = call+store on ONE source line. Standalone -O2 44/44 exact. */
/* The USO item types item_createBal / Ba2 / Chk (alloc 0x4C) and item_createGtr / Gtl (alloc 0x50) build
 * (agent-h run 567): an ItemType (0x48; its vptr gets D_00000000_b) plus one more vtable pointer
 * (D_00000000_c), at 0x48, or at 0x4C after a side word (Gtr 0, Gtl 1). The extra pointer after
 * the subclass's own data is the Cfront layout of a second base class's vptr. */
typedef struct BootItemType {
    /* 0x00 */ ItemType type;
    /* 0x48 */ char *vptr2;          /* D_00000000_c */
} BootItemType;

typedef struct BootGateItemType {
    /* 0x00 */ ItemType type;
    /* 0x48 */ s32 side;             /* item_createGtr 0, item_createGtl 1 */
    /* 0x4C */ char *vptr2;          /* D_00000000_c */
} BootGateItemType;

typedef char BootItemType_size_check[sizeof(BootItemType) == 0x4C ? 1 : -1];
typedef char BootGateItemType_size_check[sizeof(BootGateItemType) == 0x50 ? 1 : -1];

typedef struct { int v; } Box5124;
void *item_createBal(Box5124 arg) {
    volatile int p0;
    char *obj;
    Box5124 b1;
    volatile int p1;
    char *o2;
    volatile int p2;
    Box5124 b2;
    FCALL(game_loadUsoOnce)(&sStr_rom_balloon_uso_7DB4);
    b1 = arg;
    obj = (char *)FCALL(mem_new)(0x4C);
    if (obj != 0) {
        b2 = b1;
        o2 = obj;
        if (o2 != 0 || (o2 = (char *)FCALL(mem_new)(0x48)) != 0) {
            FCALL(ItemType_ctor)(o2, b2, 0); NODE(o2)->vptr = (char *)(&D_00000000_a); /* same-line: keeps obj reload after the addiu */
        }
        /* temp-first `t = &c` -> addiu(c) before addiu(b); whole tail on ONE line (line-number scheduling hint) */
        { char *t = &D_00000000_c; NODE(obj)->vptr = (char *)(&D_00000000_b); ((BootItemType *)obj)->vptr2 = t; }
    }
    return obj;
}

/* item_createBa2 - verified structural decode (0xB0, 44 insns).
 * BYTE-STRUCTURALLY IDENTICAL SIBLING of item_createBal (same
 * alloc-cascade constructor with defensive-dead-check; the ONLY
 * difference is the init datum sStr_rom_balloon_uso_7DC4 here vs sStr_rom_balloon_uso_7DB4 there).
 * Struct-typing reference: identical to item_createBal - object 0x4C
 * bytes, obj->0x28 (40) / obj->0x48 (72) descriptor/vtable ptrs (&D
 * runtime-patched). Caps <80: alloc-cascade + defensive-dead-check +
 * 3-4 reloc calls + 3x &D-store reloc. MATCHED 2026-09-04 (100, ROM-exact; kit in docs/IDO_CODEGEN.md#alloc-cascade-ctor-family-kit-5124).
 * NM body captures live behavior only — dead-arm artifacts not
 * recreated (same convention as item_createBal). */
extern char sStr_rom_balloon_uso_7DC4;
/* item_createBa2 - alloc-cascade get-or-create constructor.
 * Reconstructed missing logic (sibling of item_createBal, init datum
 * sStr_rom_balloon_uso_7DC4): the three descriptor stores (obj->0x28 = A; obj->0x28
 * = B; obj->0x48 = C) target DISTINCT registers (t9/t0/t1) -> three
 * DISTINCT externs to defeat the &D_00000000 address-CSE that
 * collapses them onto one base reg. The 0x48 second-alloc arm is
 * provably-dead (kept under -g) and is written as a get-or-create
 * fall-through so IDO keeps it. */
extern char sStr_rom_balloon_uso_7DC4;
extern char D_00000000_a;
extern char D_00000000_b;
extern char D_00000000_c;
/* 2026-09-04: struct-by-value shape ported from item_createBal (see its note). */
/* 2026-09-04 (agent-f, pass 2): frame 0x38 slot layout = three `volatile int`
 * pads interleaved in decl order (pad,obj,b1,pad,o2,pad,b2 -> homes
 * 0x30/0x2C/0x24/0x1C); the beqz-on-v0 + delay-slot `sw a1,4(sp)` schedule
 * comes from folding the get-or-create into one condition
 * (`o2 != 0 || (o2 = alloc(0x48)) != 0`); tail addiu order = temp-first `t = &c`;
 * obj-reload placement = call+store on ONE source line. Standalone -O2 44/44 exact. */
typedef struct { int v; } Box51D4;
void *item_createBa2(Box51D4 arg) {
    volatile int p0;
    char *obj;
    Box51D4 b1;
    volatile int p1;
    char *o2;
    volatile int p2;
    Box51D4 b2;
    FCALL(game_loadUsoOnce)(&sStr_rom_balloon_uso_7DC4);
    b1 = arg;
    obj = (char *)FCALL(mem_new)(0x4C);
    if (obj != 0) {
        b2 = b1;
        o2 = obj;
        if (o2 != 0 || (o2 = (char *)FCALL(mem_new)(0x48)) != 0) {
            FCALL(ItemType_ctor)(o2, b2, 0); NODE(o2)->vptr = (char *)(&D_00000000_a); /* same-line: keeps obj reload after the addiu */
        }
        /* temp-first `t = &c` -> addiu(c) before addiu(b); whole tail on ONE line (line-number scheduling hint) */
        { char *t = &D_00000000_c; NODE(obj)->vptr = (char *)(&D_00000000_b); ((BootItemType *)obj)->vptr2 = t; }
    }
    return obj;
}

/* item_createChk - verified structural decode (0xB0, 44 insns).
 * THIRD IDENTICAL SIBLING of the item_createBal / item_createBa2
 * alloc-cascade-ctor family (instruction-for-instruction identical
 * except the init datum sStr_rom_checkpoint_uso_7DD4 and local label addresses).
 * Struct-typing reference: identical to item_createBal — object 0x4C
 * bytes, obj->0x28 (40) / obj->0x48 (72) descriptor ptrs (&D runtime-
 * patched). sStr_rom_checkpoint_uso_7DD4 = named init datum (family: sStr_rom_balloon_uso_7DB4/DC4/DD4
 * at +0x10 stride). Caps <80: alloc-cascade + defensive-dead-check +
 * 3-4 reloc calls + 3x &D-store reloc. MATCHED 2026-09-04 (100, ROM-exact; kit in docs/IDO_CODEGEN.md#alloc-cascade-ctor-family-kit-5124).
 * NM body captures live behavior only — dead-arm artifacts not
 * recreated. */
extern char sStr_rom_checkpoint_uso_7DD4;
/* item_createChk - alloc-cascade get-or-create constructor.
 * THIRD IDENTICAL SIBLING of item_createBal / item_createBa2
 * (instruction-for-instruction identical except the init datum
 * sStr_rom_checkpoint_uso_7DD4 and local label addresses).
 * Reconstructed missing logic: the three descriptor stores
 * (obj->0x28 = A; obj->0x28 = B; obj->0x48 = C) target DISTINCT
 * registers (t9/t0/t1) -> three DISTINCT externs to defeat the
 * &D_00000000 address-CSE that collapses them onto one base reg.
 * The 0x48 second-alloc arm is provably-dead (kept under -g) and
 * is written as a get-or-create fall-through so IDO keeps it. */
extern char sStr_rom_checkpoint_uso_7DD4;
/* 2026-09-04 (agent-f, pass 2): frame 0x38 slot layout = three `volatile int`
 * pads interleaved in decl order (pad,obj,b1,pad,o2,pad,b2 -> homes
 * 0x30/0x2C/0x24/0x1C); the beqz-on-v0 + delay-slot `sw a1,4(sp)` schedule
 * comes from folding the get-or-create into one condition
 * (`o2 != 0 || (o2 = alloc(0x48)) != 0`); tail addiu order = temp-first `t = &c`;
 * obj-reload placement = call+store on ONE source line. Standalone -O2 44/44 exact. */
typedef struct { int v; } Box5284;
void *item_createChk(Box5284 arg) {
    volatile int p0;
    char *obj;
    Box5284 b1;
    volatile int p1;
    char *o2;
    volatile int p2;
    Box5284 b2;
    FCALL(game_loadUsoOnce)(&sStr_rom_checkpoint_uso_7DD4);
    b1 = arg;
    obj = (char *)FCALL(mem_new)(0x4C);
    if (obj != 0) {
        b2 = b1;
        o2 = obj;
        if (o2 != 0 || (o2 = (char *)FCALL(mem_new)(0x48)) != 0) {
            FCALL(ItemType_ctor)(o2, b2, 0); NODE(o2)->vptr = (char *)(&D_00000000_a); /* same-line: keeps obj reload after the addiu */
        }
        /* temp-first `t = &c` -> addiu(c) before addiu(b); whole tail on ONE line (line-number scheduling hint) */
        { char *t = &D_00000000_c; NODE(obj)->vptr = (char *)(&D_00000000_b); ((BootItemType *)obj)->vptr2 = t; }
    }
    return obj;
}

/* item_createGtr - verified structural decode (0xB4, 45 insns).
 * NEAR-SIBLING of the item_createBal/51D4/5284 alloc-cascade-ctor +
 * defensive-dead-check family. Variant: object size 0x50 (vs 0x4C),
 * init datum sStr_rom_checkpoint_uso_7DE8, one extra insn (0xB4 vs 0xB0).
 * Struct-typing reference: object = 0x50 bytes (this variant);
 * obj->0x28 (40) / obj->0x48 (72) descriptor ptrs (&D runtime-
 * patched). Caps <80: alloc-cascade + defensive-dead-check + 3-4
 * reloc + 3x &D-store reloc. MATCHED 2026-09-04 (100, ROM-exact; kit in docs/IDO_CODEGEN.md#alloc-cascade-ctor-family-kit-5124). NM body
 * captures live behavior only — dead-arm artifacts not recreated. */
extern char sStr_rom_checkpoint_uso_7DE8;
/* item_createGtr - alloc-cascade get-or-create constructor (0x50 variant).
 * Reconstructed to the item_createBal family shape: object 0x50 bytes,
 * init datum sStr_rom_checkpoint_uso_7DE8. Four descriptor stores target DISTINCT
 * registers (t9 -> obj->0x28; t0 -> obj->0x28; t1 -> obj->0x4C; zero ->
 * obj->0x48) so the &D_00000000 address-CSE does not collapse them onto
 * one base register -> distinct lui/addiu per store. The 0x48 second-
 * alloc arm is provably dead (kept under -g) and written as a get-or-
 * create fall-through so IDO retains it. */
extern char sStr_rom_checkpoint_uso_7DE8;
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
void *item_createGtr(Box5334 arg) {
    volatile int p0;
    char *obj;
    Box5334 b1;
    volatile int p1;
    char *o2;
    volatile int p2;
    Box5334 b2;
    FCALL(game_loadUsoOnce)(&sStr_rom_checkpoint_uso_7DE8);
    b1 = arg;
    obj = (char *)FCALL(mem_new)(0x50);
    if (obj != 0) {
        b2 = b1;
        o2 = obj;
        if (o2 != 0 || (o2 = (char *)FCALL(mem_new)(0x48)) != 0) {
            FCALL(ItemType_ctor)(o2, b2, 0); NODE(o2)->vptr = (char *)(&D_00000000_a); /* same-line: keeps obj reload after the addiu */
        }
        /* temp-first `t = &c` -> addiu(c) before addiu(b); whole tail on ONE line (line-number scheduling hint) */
        { char *t = &D_00000000_c; NODE(obj)->vptr = (char *)(&D_00000000_b); ((BootGateItemType *)obj)->vptr2 = t; ((BootGateItemType *)obj)->side = 0; }
    }
    return obj;
}

/* item_createGtl - verified structural decode (0xB8, 46 insns).
 * NEAR-SIBLING of the item_createBal/51D4/5284/5334 alloc-cascade-ctor
 * family. Variant: object 0x50 (like item_createGtr), init datum
 * sStr_rom_checkpoint_uso_7DFC, 0xB8 (one extra descriptor store vs 5334's 0xB4; uses
 * t2 reg where 5124 uses t1).
 * Struct-typing reference: object = 0x50 bytes; obj->0x28 (40) /
 * obj->0x48 (72) descriptor ptrs (&D runtime-patched); sStr_rom_checkpoint_uso_7DFC =
 * named init datum (family data run sStr_rom_balloon_uso_7DB4/DC4/DD4/DE8/DFC).
 * Caps <80: alloc-cascade + defensive-dead-check + 3-4 reloc calls +
 * &D-store reloc. MATCHED 2026-09-04 (100, ROM-exact; kit in docs/IDO_CODEGEN.md#alloc-cascade-ctor-family-kit-5124). NM body captures
 * live behavior only — dead-arm artifacts not recreated. */
extern char sStr_rom_checkpoint_uso_7DFC;
/* item_createGtl - alloc-cascade get-or-create constructor.
 * Reconstructed missing logic: the three descriptor stores
 * (obj->0x28 = A in the live arm; obj->0x28 = B; obj->0x4C = C in
 * the tail) target DISTINCT registers (t9/t0/t2) -> three DISTINCT
 * externs to defeat the &D_00000000 address-CSE that collapses them
 * onto one base reg. The obj->0x48 store is the immediate 1. The
 * 0x48 second-alloc arm is provably-dead (kept under -g) and is
 * written as a get-or-create fall-through so IDO keeps it. */
extern char sStr_rom_checkpoint_uso_7DFC;
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
void *item_createGtl(Box53E8 arg) {
    volatile int p0;
    char *obj;
    Box53E8 b1;
    volatile int p1;
    char *o2;
    volatile int p2;
    Box53E8 b2;
    FCALL(game_loadUsoOnce)(&sStr_rom_checkpoint_uso_7DFC);
    b1 = arg;
    obj = (char *)FCALL(mem_new)(0x50);
    if (obj != 0) {
        b2 = b1;
        o2 = obj;
        if (o2 != 0 || (o2 = (char *)FCALL(mem_new)(0x48)) != 0) {
            FCALL(ItemType_ctor)(o2, b2, 0); NODE(o2)->vptr = (char *)(&D_00000000_a); /* same-line: keeps obj reload after the addiu */
        }
        /* temp-first `t = &c` -> addiu(c) before addiu(b); store order b,1,c = temp numbering t0/t1/t2; whole tail on ONE line (multi-line tail perturbs the li/addiu schedule) */
        { char *t = &D_00000000_c; NODE(obj)->vptr = (char *)(&D_00000000_b); ((BootGateItemType *)obj)->side = 1; ((BootGateItemType *)obj)->vptr2 = t; }
    }
    return obj;
}

/* item_createCfr: 14-insn 2-call wrapper. Sibling of item_createHel (same
 * struct-by-value one-int recipe). Passing `*(struct OneI*)&a0` homes the
 * value to the outgoing a1 slot and reproduces the target's dead
 * `sw a1, 0x4(sp)` without instruction patching. */
extern char sStr_rom_fire_uso_7E10;
void item_createCfr(int a0) {
    FCALL(game_loadUsoOnce)(&sStr_rom_fire_uso_7E10);
    FCALL(FireTmpl_ctor)(0, *(struct OneI*)&a0); /* jal -> fire.uso Text+0x0 = fire.uso FireTmpl_ctor (TextReloc import, proven by name-placeholder-calls) */
}

extern char FogTmpl_vtbl_78D8;

/* 25-insn constructor wrapper: alloc 0x58 bytes, init via runtime-patched
 * callee, set field 0x28 to &FogTmpl_vtbl_78D8.  BYTE-EXACT (2026-06-20).
 * Byte-identical sibling of item_createSnow — same three IDO levers stacked
 * (struct-by-value PARAM homes a0; struct-by-value ARG homes outgoing a1 in
 * the jal delay slot; volatile int pad grows the frame 0x20 -> 0x28). The
 * only difference from 553C is the field-0x28 data symbol (FogTmpl_vtbl_78D8 here
 * vs SnowTmpl_vtbl_79C8 there). D_54D8_init_value/_arg are base-0 externs. */
extern char D_54D8_init_value;
extern char D_54D8_init_arg;
int *item_createFog(struct OneI arg0) {
    volatile int pad;
    int *p;
    int saved_a0 = arg0.x;
    p = (int*)FCALL(mem_new)(0x58);
    if (p == 0) goto end;
    FCALL(ColourItemType_ctor)(p, *(struct OneI*)&saved_a0, *(int*)&D_54D8_init_value, &D_54D8_init_arg);
    NODE(p)->vptr = (char *)&FogTmpl_vtbl_78D8;
end:
    return p;
}

/* 25-insn constructor wrapper: alloc 0x58 bytes, init via runtime-patched
 * callee, set field 0x28 to &SnowTmpl_vtbl_79C8.  BYTE-EXACT (2026-06-20).
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
extern char SnowTmpl_vtbl_79C8;
extern char D_553C_init_value;
extern char D_553C_init_arg;
int *item_createSnow(struct OneI arg0) {
    volatile int pad;
    int *p;
    int saved_a0 = arg0.x;
    p = (int*)FCALL(mem_new)(0x58);
    if (p == 0) goto end;
    FCALL(ColourItemType_ctor)(p, *(struct OneI*)&saved_a0, *(int*)&D_553C_init_value, &D_553C_init_arg);
    NODE(p)->vptr = (char *)&SnowTmpl_vtbl_79C8;
end:
    return p;
}

/* item_registerTemplates: EXACT via the IDO 7.1 NCC donor src/bootup_uso/bootup_uso_ncc_55A0.cpp, REPLACE_FUNC_BODY-spliced
 * over the placeholder below on both build paths (agent-g run 241). The superseded in-unit decode is kept
 * for reference under BOOTUP_55A0_OLD_DECODE (never defined). */
void item_registerTemplates(char *b) {
    volatile char *r = b;
}

void stream_readS32_00005EF8(int *dst) {
    int buf[2];
    FCALL(fs_gdlReadFile)(&gSysLib, buf, 4);
    *dst = buf[0];
}

/* game_createBGBox - SIX-element list builder. 7 params (a0 unused + a1,a2,a3
 * in regs + a4,a5,a6 on the caller stack at sp+80/84/88). s1 = a container
 * object built from sStr_BGBox_7E6C; list = s1+0x10 is its child attach point.
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
 * byte-exact (the remaining word is a R_MIPS_LO16 reloc to sStr_BGBox_7E6C, equal
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
 * words byte-exact (the lone reloc word to sStr_BGBox_7E6C resolves equal).
 * Frame 64 with list at sp+0x28. */
extern char sStr_BGBox_7E6C;
char *game_createBGBox(int a0, int a1, int a2, int a3, int a4, int a5, int a6) {
  char *s1 = (char *) FCALL(BGBox_ctor)(0, &sStr_BGBox_7E6C, 0, a0);
  char *list;
  char *r;
  volatile int pad[2];
 r = (char *) FCALL(U64BGWall_ctor)(0, a1, 0, 0, 0); list = s1 + 0x10; FCALL(list_append)(list, r); if ((*((int *) (r + 0x14))) != 0) {
    *((int *) (r + 0x4)) = 1;
  }
  *((void **) (r + 0x14)) = s1;
  r = (char *) FCALL(U64BGWall_ctor)(0, a2, 0, 90, 0);
  FCALL(list_append)(list, r);
  if ((*((int *) (r + 0x14))) != 0)
  {
    *((int *) (r + 0x4)) = 1;
  }
  *((void **) (r + 0x14)) = s1;
  r = (char *) FCALL(U64BGWall_ctor)(0, a3, 0, 180, 0);
  FCALL(list_append)(list, r);
  if ((*((int *) (r + 0x14))) != 0)
  {
    *((int *) (r + 0x4)) = 1;
  }
  *((void **) (r + 0x14)) = s1;
  r = (char *) FCALL(U64BGWall_ctor)(0, a4, 0, 270, 0);
  FCALL(list_append)(list, r);
  if ((*((int *) (r + 0x14))) != 0)
  {
    *((int *) (r + 0x4)) = 1;
  }
  *((void **) (r + 0x14)) = s1;
  r = (char *) FCALL(U64BGWall_ctor)(0, a5, -90, 0, 0);
  FCALL(list_append)(list, r);
  if ((*((int *) (r + 0x14))) != 0)
  {
    *((int *) (r + 0x4)) = 1;
  }
  *((void **) (r + 0x14)) = s1;
  if (a6 != 0)
  {
    r = (char *) FCALL(U64BGWall_ctor)(0, a6, 90, 0, 0);
    FCALL(list_append)(list, r);
    if ((*((int *) (r + 0x14))) != 0)
    {
      *((int *) (r + 0x4)) = 1;
      *((int *) (r + 0x4)) = 1;
    }
    *((void **) (r + 0x14)) = s1;
  }
  return s1;
}

void stream_readF32_00006100(float *dst) {
    float buf[2];
    FCALL(fs_gdlReadFile)(&gSysLib, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

void stream_readQuad_0000613C(Quad4 *dst) {
    Quad4 buf;
    FCALL(fs_gdlReadFile)(&gSysLib, &buf, 16);
#ifdef PORT
    PORT_BE_SWAP32S(&buf, 16);
#endif
    *dst = buf;
}

extern void stream_readVec3_00006194();
void stream_readVec3_00006194(Vec3 *dst) {
    int pad_top[1];
    Tri3i raw;
    int pad_mid[2];
    Tri3i tmp;
    int pad_bot[2];
    FCALL(fs_gdlReadFile)(&gSysLib, &raw, 12);
#ifdef PORT
    PORT_BE_SWAP32S(&raw, 12);
#endif
    tmp = raw;
    dst->x = *(float*)&tmp.a;
    dst->y = *(float*)&tmp.b;
    dst->z = *(float*)&tmp.c;
}

/* boot_staticInit_00006204: EXACT via an IDO 7.1 NCC (C++) donor,
 * src/bootup_uso/bootup_uso_ncc_6204.cpp, REPLACE_FUNC_BODY-spliced over this
 * placeholder (agent-c run 349). It is not a hand-written function but the
 * edg static initializer (__sti__) for a global object with an out-of-line
 * constructor: the __sti__ prologue carries the translation unit's LAST line
 * and the ctor call the object definition's earlier line, so `lui a0` is
 * scheduled above `sw ra` (no C function can give the call an earlier line
 * than its own `{`). */
void boot_staticInit_00006204(void) {
    volatile int r = 0;
}

void ColourItem_readParms_00006228(char *a0) {
    stream_readS32_0000502C((int*)(a0 + 0x3C));
    FCALL(ColourItem_load)(a0);
}

/* MapProp_ctor - verified structural decode (0x160, 88 insns,
 * get-or-create constructor + sub-object + Vec3 init).
 * Struct-typing reference: o = 0xDC-byte object. o->0x28 (40)
 * descriptor ptr (&D), o->0xC (12) type/vtable ptr (&sStr_Course_7E7C),
 * o->0xB4 (180) = a1 (a parent/owner handle; o->0xB4->0xA0 (160)
 * gated for an extra reloc registration), o->0xB8 (184)=a3, o->0xBC
 * (188)=a2, o->0xC0 (192)=a4. s = 0xB4-byte sub-object: s->0x28 (40)
 * descriptor (&D), s+0x30 (48) a Vec3f zero-initialized via the
 * reloc helper; plus a defensive 4-byte child alloc (*(int*)c = 0).
 * sStr_MNode_7E74/7E7C/7E84/7E98 = init datums.
 * Caps <80: get-or-create + ~8 reloc + &D descriptors + defensive-
 * dead alloc guards + FP Vec3 + bnel branch-likely. INCLUDE_ASM
 * remains build path.
 * 2026-05-31: applied the alloc-or-passthrough cascade dead-arm form to the
 * child alloc (c = s+0x2C; if(s==-44) c=alloc(4); if(!c) c=alloc(4); if(c)
 * *c=0) -> 64.9->67.3%. Remaining residual is the sub-object saved-reg(s1)-
 * vs-stack-spill(76sp) choice + the ~8 &D %hi/%lo reloc scheduling. */
extern char sStr_MNode_7E74, sStr_Course_7E7C, sStr_shp_pGrid_NULL_7E84, sStr_mapprop_cpp_7E98;
/* run 111 (agent-c): cfront ctor shape -- if (this || (this = new(0xDC))), the inlined base ctor
 * if (s || (s = new(0xB4))) and the doubly-inlined member ctor for the +0x2C cell; the param
 * itself is the object (no copy local); pads restore the 0x50 frame / home offsets; block-count
 * lever (IDO_CODEGEN#hold-vs-remat-firstusecost-run111). EXACT: z[1]/z[2] stored in the call's
 * argument (comma expression) puts the s+0x30 setup between the z[0] and z[1] stores as in the target. */
void *MapProp_ctor(char *a0, int a1, int a2, int a3, int a4) {
    char *s;
    volatile int pad[3];
    float z[3];
    volatile int pad2[1];
    if (a0 != 0 || (a0 = (char*)FCALL(mem_new)(0xDC)) != 0) {
    s = a0;
    if (s != 0 || (s = (char*)FCALL(mem_new)(0xB4)) != 0) {
        FCALL(Node_ctor)(s, &sStr_MNode_7E74);
        NODE(s)->vptr = (char *)(&XDATA(MNode_vtbl, port_bootup_data, 0x1EAA8));
        {
            int *c = (int*)(s + 0x2C);
            if (c != 0 || (c = (int*)FCALL(mem_new)(4)) != 0) {
                int *c2 = c;
                if (c2 != 0 || (c2 = (int*)FCALL(mem_new)(4)) != 0) {
                    *c2 = 0;
                }
            }
        }
        FCALL(MNode_initMatrices)(s);
        z[0] = 0.0f;
        FCALL(math_mtxAddTranslation)(s + 0x30, (z[1] = 0.0f, z[2] = 0.0f, z));
    }
    NODE(a0)->vptr = (char *)(&XDATA(MapProp_vtbl, port_bootup_data, 0x7C20));
    NODE(a0)->name = (const char *)&sStr_Course_7E7C;
    ((MapProp *)a0)->shape = a1;
    FCALL(MNode_addChild)(a0, a1);
    /* shp->pGrid != NULL: the course shape is a BasePolyShape (MapProp.shape), its +0xA0 is the
     * parse-cmd-35 XZ cell grid (polyshape.h collGrid), the "pGrid" of the assert string */
    if (((BasePolyShape *)((MapProp *)a0)->shape)->collGrid == 0)
        FCALL(sys_assertFail)(&sStr_shp_pGrid_NULL_7E84, &sStr_mapprop_cpp_7E98, 0x3E4);
    ((MapProp *)a0)->end = a3;
    ((MapProp *)a0)->angle = a2;
    ((MapProp *)a0)->add = a4;
    }
    if (1) { }
    if (1) { }
    if (1) { }
    if (1) { }
    return a0;
}

// MapProp_display — 0x380 / 224 insns. Minimap marker projection. 23.4 -> 76.0
// (agent-f 2026-09-04, hand reconstruction from the target .s; old body was
// a 78-insn stub that dropped the whole 3x3 transform + clamp cascade).
// Shape recovered: obj homed at 0x90(sp) (no s-regs); K = folded const
// snd_soundObjCreate_0000057C+0x34 read via DATA ALIAS bu_63b4_c57c (fn-typed base never
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
/* EXACT via an IDO 7.1 NCC (C++) donor: src/bootup_uso/bootup_uso_ncc_63B4.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder (agent-g run 184: an inline M3::xf(V3 &) called
 * through ctx->view->rot, plus set() / addz() members on the two frame vectors). The cfe decode
 * (83.21) is in git history. */
void MapProp_display(char *obj) {
    volatile char *r = obj;
}

/* MapProp_hostioParms - verified structural decode (0xD4, 53 insns,
 * 4-entry registration/builder).
 * (reg = func_00000000; the 5th/6th args are passed on the stack at
 * sp+0x10 / sp+0x14, the documented IDO stack-arg slots.)
 * Struct-typing reference: sStr_Map_7EA4/EA8/EAC/EB0 = four registration
 * key data (label/descriptor); the bound targets are object fields
 * a0->0xB8 (184), a0->0xC0 (192), a0->0xBC (188) respectively (entry
 * 0 binds no target). Per-entry trailing params: (a3=0, 0x7F, 1) for
 * EA8/EAC and (a3=0, 0x59, 0) for EB0 - look like {range/limit, flag}
 * pairs. Caps <80: 6x reloc calls + 4x &D reloc + 5th/6th stack-arg
 * passing (sp+0x10/0x14). INCLUDE_ASM remains build path. */
extern char sStr_Map_7EA4, sStr_end_7EA8, sStr_add_7EAC, sStr_angle_7EB0;
void MapProp_hostioParms(char *a0) {
    FCALL(hostio_beginGroup)(&gHostio, &sStr_Map_7EA4, 0);
    FCALL(hostio_addInt)(&gHostio, &sStr_end_7EA8, &((MapProp *)a0)->end, 0, 0x7F, 1);   /* "end" */
    FCALL(hostio_addInt)(&gHostio, &sStr_add_7EAC, &((MapProp *)a0)->add, 0, 0x7F, 1);   /* "add" */
    FCALL(hostio_addInt)(&gHostio, &sStr_angle_7EB0, &((MapProp *)a0)->angle, 0, 0x59, 0); /* "angle" */
    FCALL(hostio_endGroup)(&gHostio);
    FCALL(MNode_hostioParms)(a0);
}

// game_createMap — STRUCTURAL PASS (0x948 / 594 insns, no episode).
// Large scene/screen constructor, bracketed by a global mode-flag
// set/clear; builds a widget tree from the obj's sub-object table via
// ~65 func_00000000 dispatcher calls.
//
//   void *game_createMap(Scene *obj, Ctx *a1, void *a2) {  // obj->s7
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
//     //   .L00007F5C / snd_seqEndDbg_00000188+0x3C (6x); ~65 dispatcher calls
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
//     snd_seqEndDbg_00000188 + 0x3C = a folded table (read 6x);
//     snd_soundObjCreate_0000057C + 0x38 = a folded f32 const (sibling of the
//                            snd_soundObjCreate_0000057C+0x34 ref in MapProp_display).
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
//   snd_seqEndDbg_00000188+0x3C table, snd_soundObjCreate_0000057C+0x38 jumptable) kept as
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
//   60.28: +0x1C4 RMWs -> true-identity snd_seqEndDbg_00000188+0x3C form 58.82;
//   + mode-word RMW -> func_00000000+0x4 form 57.93; + gc alias for the
//   s5-class 0x68/0x3C reads 56.56), despite each moving insn count
//   toward target (545->553 of 594) and matching the target's per-site
//   %hi/%lo shapes (target: no held base for the snd_seqEndDbg_00000188+0x3C
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
//   8001FFC0 (root slot, s4 pre-loop), C = gInputMgr_800200FC (s5), B =
//   D_7770 (current obj), post-loop s4/s5 = D_776C / D_1EBDC, mode word
//   gPolyGeomMode_3EEC8_8005EE6C, flags import_8005EE64 / gGfxLib_8005C108+0x3C. Every
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
// PASS-8 2026-10-29 (agent-g run 223): EXACT. The C now lives in the IDO 7.1 -O2 donor
//   src/bootup_uso/bootup_uso_o2_6808.c, REPLACE_FUNC_BODY-spliced over the placeholder
//   below in both trees (jumptable reloc -> game_createMap_rodata = 0x5B4). The 4-byte frame
//   residual was a cfe temporary: the `call() + 0 * x` fold and the struct-valued comma args
//   each allocate a temp word under the lowest home; with them gone plus one dead named home
//   (`int gE = 0;` after gD) every offset matches, and `va + 0 * (t70.w = X)` restores the
//   target's arg-slot-store-before-jal order. The old in-unit NM body is in git history.
void *game_createMap(char *obj, char *arg1, s32 arg2) {
    volatile char *r = obj;
    return 0;
}

/* EnvSettings_ctor - get-or-create constructor (0xB4, 45 insns).
 * EXACT 2026-07-15 (84.0% -> 45/45, ROM byte-exact, C is build path).
 * Object = 0x44 bytes: ->0x18 u32 flags (bit 3 cleared), ->0x28
 * descriptor ptr (&D runtime-patched), ->0x2C sub-ptr (inline &o+0x2C,
 * defensive alloc(4) arm when o == -0x2C), ->0x30 = 4, ->0x34 = 0xB00,
 * ->0x38 = 1, ->0x3C/0x40 = 0; sStr_EnvSettings_7F84 = init datum.
 * Levers: (1) destructive a0 reuse (no local copy) -> spill goes to the
 * a0 HOME slot 0x18(sp), frame 0x18 not 0x20; (2) the flag RMW uses the
 * if(1)-pointer-mutation idiom `q = a0; if (1) { q += 0x18; } *(int*)q
 * &= ~8;` to materialize `addiu v0,a2,0x18` + 0(v0) accesses (plain
 * `*(int*)(a0+0x18) &= ~8` folds into 0x18(a2) addressing, -1 insn);
 * (3) alloc(4) arm written `if ((v1 = r) == 0) goto after_v1;` so the
 * or v1,v0 copy fills the beqz delay slot (reload lw a2 hoists above
 * the branch). The if(1) braces and shared lines are LOAD-BEARING. */
extern char sStr_EnvSettings_7F84;
void *EnvSettings_ctor(char *a0) {
    if (a0 == 0) {
        a0 = (char*)FCALL(mem_new)(0x44);
        if (a0 == 0) goto end;
    }
    FCALL(Node_ctor)(a0, &sStr_EnvSettings_7F84);
    NODE(a0)->vptr = (char *)(&EnvSettings_vtbl);
    {
        int *v1 = (int *)(a0 + 0x2C);
        if (v1 == 0) {
            int *r = (int *)FCALL(mem_new)(4);
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
    ENVSET(a0)->fallingSnow = 0;
    ENVSET(a0)->bgBox = 0;
    ENVSET(a0)->clipNear = 4;
    ENVSET(a0)->clipFar = 0xB00;
    ENVSET(a0)->flags = 1;
end:
    return a0;
}

/* map_addEntry_00007204: 33-insn alloc/link helper. NATURAL CEILING: 90.15% NM.
 * Built emits 0x28 frame / 31 insns; target needs 0x30 frame / 33 insns
 * with defensive a2 dead-spill at jal-delay + a1<->a2 regalloc swap +
 * base-pointer form for a2 reload. The historical SUFFIX_BYTES_FORCE
 * (+8 bytes, 2 nops) + 31-entry INSN_PATCH promotion was REMOVED
 * 2026-05-23 as match-faking. Last of the 3-function bootup_uso regalloc
 * cluster (7C74/7B08/7204) per project_1080_bootup_regalloc_cluster.md. */
/* map_addEntry_00007204: allocate a {word, M4} record (cfront new + inline ctors with
 * the member this==0 protocol) and add it to the list at a0 + 0x2C.
 * EXACT via an IDO 7.1 NCC (C++) donor: src/bootup_uso/bootup_uso_ncc_7204.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder (agent-c run 123). The add is
 * an inline list member forwarding to the import, which keeps the new-expression
 * result a separate web from the call argument (target a2, copied to a1). */
void map_addEntry_00007204(int a0, int a1, int a2) {
    volatile int r = 0;
}

/* EnvSettings_display: 40-insn (0xA0) two-conditional state-init helper.
 *
 * Structure:
 *   self = D[0x254];   // global state pointer at &D_00000000 + 0x254
 *   if (a0->0x38 & 2) {
 *       self->0xAC = (float)(int)a0->0x30;   // cvt.s.w via int→float regs
 *       self->0xB0 = (float)(int)a0->0x34;
 *   }
 *   if (a0->0x3C == 0) {
 *       self->0x78 |= 4;
 *       copy 4 words from (snd_startPlayer2Seq_0000027C + 0x18..0x24) to self->0xDC..0xE8;
 *   }
 *
 * The `snd_startPlayer2Seq_0000027C + 0x18` is the splat-fold-into-nearest-func pattern
 * (per docs/MATCHING_WORKFLOW.md): a real D_<addr> rodata symbol was folded
 * into the preceding function. */
/* EXACT (agent-c run 121): the record is a 16-byte struct copy from a pinned alias (gGfxLib_7288_src = snd_startPlayer2Seq_0000027C + 0x18), and the flags RMW goes through the advanced pointer q (*q |= 4). */
/* 2026-07-31 (agent-g newest-lever sweep): 94.6 -> 95.4. Levers landed:
 * de-named self (per-use *(base+0x254) deref -> t-ring reload temps t9/t1
 * exact, kills the old named-self $v0 web) + named per-region base local +
 * named single-load `d` for the 4-store tail (one lw like target) + q
 * pointer-bump RMW (lw 120(v0)/addiu v0,v0,120/ori/sw 0(v0) shape exact).
 * Body is now insn-for-insn isomorphic to target (40 words, all opcodes
 * + offsets aligned); RESIDUAL is a pure register-name rotation web:
 * base colors $a2 vs target $v1, d $a1 vs t5, RMW temps $a0 vs t3/t4,
 * data temps t3-t6 vs t8/t7 ping-pong, plus the folded-rodata src-base
 * addend form — target addiu carries +0x18 (25ce0018 = %lo(snd_startPlayer2Seq_0000027C)
 * +24: the ORIGINAL had its own D_00000294 rodata symbol, splat folded it
 * into snd_startPlayer2Seq_0000027C) where C can only produce %lo+0 with the 0x18 folded
 * into the load offsets (24..36); if(1) pointer-mutation does NOT bake
 * addends on FUNCTION symbols (probed — emits a separate addiu+24, unlike
 * the data-symbol C14/2BE4 cases). Coloring probes: if(1) BB-split flips
 * base a2->v1 and makes region 1 + region-2 head BYTE-EXACT but costs +2
 * insns in the tail (split addiu + d remat, net fuzzy 91.3 — rejected);
 * def-order swap src<->RMW = 95.275; register kw inert. a-reg-vs-t-ring
 * named-web free-pool tie class. NM. */
typedef struct { int w[4]; } Q7288;
extern Q7288 gGfxLib_7288_src; /* = snd_startPlayer2Seq_0000027C + 0x18 (asm %lo addend), pinned at 0x294 */
void EnvSettings_display(int *a0) {
    if (ENVSET(a0)->flags & 2) {
        GfxLib *base = gGfx;
        base->curWindow->win.near = (float)(int)ENVSET(a0)->clipNear;
        base->curWindow->win.far = (float)(int)ENVSET(a0)->clipFar;
    }
    if (ENVSET(a0)->bgBox == 0) { /* no BG box: clear the screen */
        GfxLib *b2 = gGfx;
        int *q;
        q = &b2->curWindow->win.flags;
        *q |= 4;
        *(Q7288 *)b2->curWindow->win.clearColour = gGfxLib_7288_src;
    }
}

/* EnvSettings_load - FULL structural decode (0x1C0, 112 insns, node-list
 * search + match-dispatch loop + tail state-set). fuzzy 24 -> 69 (2026-07,
 * agent-f). Logic + all reloc callees are correct; the FP-literal-pool
 * premise does NOT apply here (the single float 1.0f is emitted inline via
 * lui 0x3F800000 / mtc1, no pool).
 *
 * Structure (verified vs .s):
 *   while (stream_readS32_00005EF8(&flag), flag != 0) {
 *       stream_readS32_0000502C(&outv);
 *       node = s3->0x2C; cval = node ? node->0x4 : node;
 *       for (; node && node->0x0; node = cval, cval = node->0x4)
 *           if (flag == node->0x0->0x4) { match = node->0x0; break; }
 *       if (match) { vt = match->0x0->0x28;                 // vtable
 *                    (*(vt->0x4C))((char*)(match->0x0) + (short)vt->0x48); }
 *       else { func_00000000(&sFmt_Env_Skip_7FA0,&flag);
 *              func_00000000(&D,1,outv); }
 *   }
 *   stream_readS32_0000502C(&s3->0x38); ...flag-1 -> reg-desc; flag-2 -> free 0x30/0x34;
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
 * (stream_readS32_00005EF8/keybuf likely wrote/compared a small stack STRUCT, not a
 * scalar). All three folded-symbol refs (snd_startPlayer2Seq_0000027C+0x18,
 * stream_readS32_00000008+0x34, snd_muteAll_0000029C+0x4) and the jal-0 callees ARE
 * reloc-correct here. INCLUDE_ASM remains build path. */
extern char sFmt_Env_Load_7F90;
extern char sFmt_Env_Skip_7FA0;
/* stream_readS32_00005EF8 / stream_readS32_0000502C already file-scope-declared. */
extern void snd_startPlayer2Seq_0000027C();   /* K&R data-symbol base (0x27C) */
extern void snd_muteAll_0000029C();   /* K&R data-symbol base (0x29C) */
extern void stream_readS32_00000008();   /* K&R data-symbol base (0x08)  */
/* EXACT via an IDO 7.1 NCC (C++) donor: src/bootup_uso/bootup_uso_ncc_7328.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder (agent-c run 117). Handler list walk through the C++ {cur, nxt} iterator; see the .cpp header. */
void EnvSettings_load(char *s3) {
}

/* FallingSnow_create - verified structural decode (0x138, 78 insns,
 * guarded constructor + list-link + vtable + finalize).
 * Struct-typing reference: global gate *(stream_readS32_00000008+0x2C) == 8
 * skips construction (already-done / disabled state). r = 0x134-byte
 * object, sub = 0x48-byte aux; both get a &D descriptor at ->0x28
 * (r's is the obj-0x28 vtable, 0x5C/0x58 variant: fn@0x5C (92),
 * s16@0x58 (88), dispatched with arg 0x96). a0->0x40 (64) = the
 * owning slot for r. r linked into the global list whose head is
 * (*(stream_readQuad_000000F0+0x44))->0x88 via r->0x14 (back-link) / r->0x4
 * (=1 when already-linked). r->0x48 (72) s32 toggled 1 then 0,
 * r->0x4C (76) f32 = 0.0 (init state/flags). sStr_FallingSnow_7FB0 / sStr_Node_7FBC
 * = init datums. Caps <80: global gate + ~6 reloc (alloc/init/
 * finalize) + &D descriptors + list-link beql + obj-0x28 vtable
 * jalr + FP. INCLUDE_ASM remains build path. */
extern char sStr_FallingSnow_7FB0, sStr_Node_7FBC;
extern void stream_readQuad_000000F0();  /* used as data-symbol base */
/* 2026-10-22 (agent-c run 120) EXACT: the final call takes NO argument (the target only stores p->0x48 = 0 in its delay slot; a0 is whatever the previous call left), and the child's init call and its +0x28 store share one source line (as1 line tie). */
/* run 111 (agent-c): cfront ctor shape (r = new(0x134); if (r) { sub = r; if (sub || (sub = new(0x48))) ... }),
 * no early returns, data symbols for the two %lo-folded globals, a0 escaped so every a0->0x40 is re-read,
 * v reused for the tail stores, block-count lever (IDO_CODEGEN#hold-vs-remat-firstusecost-run111). */
extern char gInputMgr_74e8_g, gInputMgr_74e8_h;
/* Both aliases relocate to bootup Bss+0xF8 (the InputMgr / GameFlow, uso-imports.py sites bootup):
 * _g is pinned at +8 and _h at +0xF0, so _g + 0x2C = nextFlowMode and _h + 0x44 = game. a0 is
 * gEnvSettings (Bss+0x20, SnowTmpl_create). */
#define FLOW_74E8_G GAMEFLOW((char *)&gInputMgr_74e8_g - 8)
#define FLOW_74E8_H GAMEFLOW((char *)&gInputMgr_74e8_h - 0xF0)
void FallingSnow_create(char *a0) {
    int *v;
    char *r;
    char *sub;
    char *head;
    if (FLOW_74E8_G->nextFlowMode == 8) return; /* 8 = EDITORS */
    FCALL(memtrack_begin)(&sStr_FallingSnow_7FB0, 0);
    r = (char*)FCALL(mem_new)(0x134);
    if (r != 0) {
        sub = r;
        if (sub != 0 || (sub = (char*)FCALL(mem_new)(0x48)) != 0) {
            FCALL(Node_ctor)(sub, &sStr_Node_7FBC); NODE(sub)->vptr = (char *)(&XDATA(D_00000000, port_snoweffects_data, 0x60));
        }
        NODE(r)->vptr = (char *)(&XDATA(D_00000000, port_snoweffects_data, 0xD0));
    }
    ENVSET(a0)->fallingSnow = (FallingSnow *)r;
    head = (char *)WORLDREG(FLOW_74E8_H->game)->egRoot;
    if (1) { }
    if (1) { }
    if (1) { }
    if (1) { }
    if (1) { }
    if (1) { }
    if (1) { }
    if (1) { }
    if (1) { char **pa = &a0; }
    FCALL(list_append)(head + 0x10, r);
    if (NODE(r)->parent != 0) NODE(r)->unk04 = 1;
    NODE(r)->parent = (Node *)head;
    v = (int *)NODE(ENVSET(a0)->fallingSnow)->vptr;
    ((void (*)(char*, int))VT_SLOT(v, 10).fn)(
        (char *)ENVSET(a0)->fallingSnow + VT_SLOT(v, 10).delta,
        0x96);
    v = (int *)ENVSET(a0)->fallingSnow; ((FallingSnow *)v)->enabled = 1;
    v = (int *)ENVSET(a0)->fallingSnow; ((FallingSnow *)v)->unk4C = 0.0f;
    ENVSET(a0)->fallingSnow->enabled = 0;
    FCALL(memtrack_end)();
}

/* EnvSettings_setSnow - verified structural decode (0xD4, 53 insns,
 * enable/disable toggle on a substate object).
 * Struct-typing reference: a0->0x40 (64) = substate object (NULL =
 * no-op). substate->0x18 (24) flags word: bit 2 (0x4) and bit 3
 * (0x8) = the enable pair (set together on enable, cleared on
 * disable); substate->0x48 (72) = bound handle (= a1 on enable);
 * substate->0x4C (76) f32 = kRo_EnvSettings_setSnow_5E8 on enable / 0.0 on disable;
 * substate->0xBC (188) = a gate that forces the enable path even
 * when a1==0. kRo_EnvSettings_setSnow_5E8 = f32 constant (default value). [Caps note below RETRACTED 2026-09-04 — matched, see body.] Caps <80:
 * beql/bnel branch-likely + mtc1/swc1 FP + kRo_EnvSettings_setSnow_5E8 f32 reloc +
 * reloc notify call. INCLUDE_ASM remains build path. */
extern float kRo_EnvSettings_setSnow_5E8;
/* 2026-09-04 MATCHED 53/53 (agent-f): the old body merged `a1 != 0 ||
 * st->0xBC != 0` into one enable test; the target gates only the two
 * flag-CLEARs on 0xBC and always writes 0.0f + handle on the a1==0 path.
 * Levers: substate ptr reloaded per access (no cached local); each flag RMW
 * through an explicit `int *f = st + 0x18; *f op= k` (materializes the
 * addiu + lw/sw 0(v0) pairs, direct `*(int*)(st+0x18) |= k` folds the
 * displacement); enable-path swc1 base = the same `f` var re-pointed at st
 * with the 0x4C folded into the store (colors v0). */
#define ST_7620 ((char *)ENVSET(a0)->fallingSnow)
void EnvSettings_setSnow(char *a0, int a1) {
    int *f;
    if (ST_7620 == 0) return;
    FCALL(snd_setLevelRampTarget)(a1);
    if (a1 == 0) {
        if (((FallingSnow *)ST_7620)->unkBC == 0) {
            f = (int*)(ST_7620 + 0x18); *f &= ~0x8;
            f = (int*)(ST_7620 + 0x18); *f &= ~0x4;
        }
        ((FallingSnow *)ST_7620)->unk4C = 0.0f;
    } else {
        f = (int*)(ST_7620 + 0x18); *f |= 0x8;
        f = (int*)(ST_7620 + 0x18); *f |= 0x4;
        f = (int*)ST_7620; ((FallingSnow *)f)->unk4C = kRo_EnvSettings_setSnow_5E8;
    }
    ((FallingSnow *)ST_7620)->enabled = a1;
}
#undef ST_7620

/* Tv1_display - verified structural decode (0xDC, 55 insns).
 * Same FP transform/draw family as Mountain_display (shared globals:
 * root *(snd_setPlayer0VolumeLevel_0000023C+0x18), scratch vector stream_readVec3_00000080+0x20,
 * scale consts &D+0x128/0x12C/0x130, a global emit counter).
 * Struct-typing reference: s0 = drawable/emitter object. s0->0x5C
 * (92) / 0x60 (96) s32 params, s0->0x64 (100) f32 (stack arg),
 * s0->0x70 (112) f32 * s0->0x80 (128) f32 = a derived f32 stack
 * arg, s0->0x6C (108) f32 y-offset, s0+0x2C = a sub-struct pointer
 * arg. root = *(snd_setPlayer0VolumeLevel_0000023C+0x18) global context; stream_readVec3_00000080+
 * 0x20 = shared f32 scratch vector {0x30,0x34,0x38} scaled per-frame
 * by the &D+0x128/0x12C/0x130 scale triple (same as Mountain_display).
 * reloc_emit returns nonzero to proceed with the transform+draw.
 * Caps <80: FP mul.s/add.s chains + 3x reloc + cross-symbol data
 * refs (snd_setPlayer0VolumeLevel_0000023C+0x18, stream_readVec3_00000080+0x20) + f32 stack-arg.
 *
 * 2026-06-02 (69.1->75.2%): the reloc_emit call's args 5,7 are single floats
 * (s0->0x64, s0->0x70*s0->0x80; target swc1 0x10/0x18) — typed-float proto
 * func_76f4_q avoids K&R double-promote. Residual ~25%: target FOLDS the root
 * load (lui%hi/lw%lo of snd_setPlayer0VolumeLevel_0000023C+0x18) but the function-symbol arithmetic
 * here materializes it (extra addiu) + the 0x70/0x80 mul-operand load order.
 * Double-promote-scan vein. INCLUDE_ASM stays. */
extern void snd_setPlayer0VolumeLevel_0000023C();  /* used as data-symbol base */
/* EXACT via an IDO 7.1 NCC (C++) donor: src/bootup_uso/bootup_uso_ncc_76F4.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder (agent-g run 180, the gl 39E24 shape:
 * the camera vector is a member of the global block, scaled through an inline
 * Cam::mul(G &) called on a local reference). The cfe decode (99.82) is in git history. */
void Tv1_display(char *s0) {
    volatile char *r = s0;
}

/* Tv1Tmpl_blitFrame_000077D0 - verified structural decode (0x278, 158 insns,
 * glyph/tile blit with mode dispatch). Font/HUD text renderer.
 *   void Tv1Tmpl_blitFrame_000077D0(St *a0, u16 *dst, int mode) {
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
void Tv1Tmpl_blitFrame_000077D0(char *arg0, u16 *dst, s32 arg2) {
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

    tbl = ((Tv1Tmpl *)arg0)->frameImage; /* arg0 is the Tv1Tmpl (Tv1Tmpl_stepPicture_00007D04 passes s0) */
    if (tbl == NULL) {
        return;
    }
    mode = ((Tv1Tmpl *)arg0)->scroll;
    c = mode;
    if (arg2 != 0) {
        c = mode + 0x60;
    }
    mode = ((Tv1Tmpl *)arg0)->state;
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

void boot_readFloatParm_00007A48(char *a0) {
    int scratch;
    stream_readS32_00004FF0(&scratch);
    stream_readF32_00006100((int*)(a0 + 0x10));
}

void boot_readQuadParm_00007A78(char *a0) {
    int scratch;
    stream_readS32_00004FF0(&scratch);
    stream_readQuad_0000613C((int*)(a0 + 0x10));
}

void boot_readIntParm_00007AA8(char *a0) {
    int scratch;
    stream_readS32_00004FF0(&scratch);
    stream_readS32_00004FF0((int*)(a0 + 0x10));
}

void boot_readVec3Parm_00007AD8(char *a0) {
    int scratch;
    stream_readS32_00004FF0(&scratch);
    stream_readVec3_00006194((int*)(a0 + 0x10));
}

/* boot_ItemType_create_00007B08: 36-insn (0x90) alloc/init/link constructor. BYTE-EXACT,
 * ported from its masked-hash twin donor edd_ItemType_create_000003BC — the two
 * bodies are byte-identical (0 non-reloc word diffs). The donor's clean C
 * form (int* param + p2/head/p1 decl order + p1[0x40/4] int-array indexing)
 * is what produces the target's frame (0x28) and $a0/$v1 register allocation,
 * which the prior permuter-mangled char* NM body (93.72% ceiling) could not
 * reach. Was once force-matched via 25-entry INSN_PATCH (banned 2026-05-23);
 * this is the genuine C match. */
void *boot_ItemType_create_00007B08(int *arg0) {
    int *p2;
    int *head;
    int *p1;
    p1 = (int*)FCALL(mem_new)(0x40);
    if (p1 != 0) {
        FCALL(ColourItem_ctor)(p1);
        NODE(p1)->vptr = (char *)&ColourItem_vtbl_75A0;
        ((ColourSubItem *)p1)->unk3C = 0;
    }
    p2 = p1;
    p1 = arg0;
    head = (int*)p1[0x40 / 4];
    if ((int*)p1[0x40 / 4] != 0) {
        FCALL(list_append)((char*)p2 + 0x10, head);
        if (NODE(head)->parent != 0) {
            NODE(head)->unk04 = 1;
        }
        NODE(head)->parent = (Node *)p2;
    }
    return p2;
}

void boot_readIntParm_00007B98(char *a0) {
    int scratch;
    stream_readS32_00004FF0(&scratch);
    stream_readS32_0000502C((int*)(a0 + 0x10));
}

void ColourItem_readParms_00007BC8(char *a0) {
    stream_readS32_00004FF0((int*)(a0 + 0x3C));
    FCALL(ColourItem_load)(a0);
}

/* MemberCallback_invoke_00007BF4 - EXACT (run 113 agent-c): twin port of MemberCallback_invoke
 * (same vtable-dispatch thunk). Keys from the twin: plain if/else idx select
 * (no goto / branch-likely), loaded-value-LAST entry add, and the arg load +
 * accumulate joined on one line (hoists the lh above the lw). */
/* The callback object (sun.h's MemberCallback, whose pmf is a plain Pair2 there): a cfront pointer
 * to member function. index < 0: u.fn is the function; otherwise u.vptrOff is the offset of the
 * vtable pointer in the object (0 with delta 0 means the Node vptr at +0x28) and index selects the
 * GdlVtEnt {this-delta, fn} slot. */
typedef struct CbPmf_7BF4 {
    /* 0x0 */ s16 delta;            /* this adjustment */
    /* 0x2 */ s16 index;            /* vtable slot, < 0 for a non-virtual member */
    /* 0x4 */ union { int (*fn)(char *); int vptrOff; } u;
} CbPmf_7BF4;
typedef struct MemberCb_7BF4 {
    /* 0x0 */ void *vptr;
    /* 0x4 */ char *obj;
    /* 0x8 */ CbPmf_7BF4 pmf;
} MemberCb_7BF4;
int MemberCallback_invoke_00007BF4(char *a0) {
    char *arg;
    CbPmf_7BF4 *v1;
    char *entry;
    int (*fnptr)(char *);
    int idx;
    v1 = &((MemberCb_7BF4 *)a0)->pmf;
    arg = ((MemberCb_7BF4 *)a0)->obj; arg = ((MemberCb_7BF4 *)a0)->pmf.delta + arg;
    if (((MemberCb_7BF4 *)a0)->pmf.index < 0) {
        fnptr = ((MemberCb_7BF4 *)a0)->pmf.u.fn;
    } else {
        int selector = v1->u.vptrOff;
        if (selector == 0 && v1->delta == 0) idx = 0x28; else idx = selector;
        entry = (v1->index << 3) + *(char**)(arg + idx);
        arg = ((GdlVtEnt *)entry)->delta + arg;
        fnptr = (int (*)(char *))((GdlVtEnt *)entry)->fn;
    }
    return fnptr(arg);
}

extern float kRo_Tv1Tmpl_create_5EC;
/* Tv1Tmpl_create: 36-insn alloc/init/link helper. EXACT 2026-10-16 (was NM spill-slot "cap", fix at the end).
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
 * dead-if ref boost), and a live named `float f = kRo_Tv1Tmpl_create_5EC` (which also
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
void *Tv1Tmpl_create(char *a0) {
    char pad[4];
    int *link;
    char *ret;

    ret = (char*)FCALL(mem_new)(0x88);
    if (ret != 0) {
        FCALL(SNode_ctor)(ret);
        NODE(ret)->vptr = (char *)(&Tv1_vtbl);
    }

    TV1(ret)->radius = kRo_Tv1Tmpl_create_5EC;
    link = (int *)((Tv1Tmpl *)a0)->model;
    FCALL(list_append)(ret + 0x10, link);
    if (link[5] != 0) {
        link[1] = 1;
    }
    link[5] = (int)ret;

    return ret;
}

/* Tv1Tmpl_stepPicture_00007D04 - verified structural decode (0x14C, 83 insns,
 * phased animation/timer state machine).
 * Struct-typing reference: s0->0x5C (92) s32 = animation state /
 * next-state (0/1/2 phases); s0->0x60 (96) s32 = per-phase frame
 * counter (thresholds 0x1F / 0x3D / 0x5B per state); s0->0x58 (88)
 * s32 = a timer/position adjusted on phase transition (-0xA for
 * state1->next, =0x18 reset on state2 wrap); s0->0x64 (100) =
 * stored result of the obj-0x28 vtable dispatch (0x64/0x60
 * variant, root = *(snd_setPlayer0VolumeLevel_0000023C+0x18)); s0->0x40 (64) -> obj
 * whose ->0x84 (132) is a message/anim list (entries -> ->0x1C
 * payload) driven by the two func_00000000 calls (idx 0 and 1,
 * arg = next state a3). State 2 wrap randomizes the next state
 * via rand01()*3.0. Caps <80: obj-0x28 vtable jalr + switch
 * (beq/beql chain) + FP rand*3.0 trunc + snd_setPlayer0VolumeLevel_0000023C+0x18
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
extern char *gGfxLib_7D04_root[];
/* the alias is GfxLib + 0x23C (undefined_syms_auto.txt), so [6] is GfxLib.curWindow (+0x254): the
 * TV asks the window being drawn for its vtable slot 11 result (Fader_buildTexture makes the same call
 * on GfxLib.mainWindow and indexes the result as a u16 RGBA5551 image: the frame buffer). */
#define TV1_WIN (gGfxLib_7D04_root[6])
extern float func_7D04_randf();
void Tv1Tmpl_stepPicture_00007D04(char *s0) {
    int a3 = ((Tv1Tmpl *)s0)->state;
    int k;
    ((Tv1Tmpl *)s0)->frameImage =
        ((u16 *(*)(char *))VT_SLOT(NODE(TV1_WIN)->vptr, 11).fn)(TV1_WIN + VT_SLOT(NODE(TV1_WIN)->vptr, 11).delta);
    k = ((Tv1Tmpl *)s0)->state;
    if (k != 1) {
        if (k != 0) {
            if (k != 2) {
            } else goto c2;
        } else goto c0;
    } else goto c1;
    goto end;
c1:
    k = ((Tv1Tmpl *)s0)->timer;
    ((Tv1Tmpl *)s0)->timer = k + 1;
    if (k >= 0x1F) a3 = 2;
    goto end;
c0:
    k = ((Tv1Tmpl *)s0)->timer;
    ((Tv1Tmpl *)s0)->timer = k + 1;
    if (k >= 0x3D) a3 = 2;
    ((Tv1Tmpl *)s0)->scroll -= 0xA;
    goto end;
c2:
    k = ((Tv1Tmpl *)s0)->timer;
    ((Tv1Tmpl *)s0)->timer = k + 1;
    if (k >= 0x5B) {
        a3 = (int)(func_7D04_randf() * 3.0f);
        ((Tv1Tmpl *)s0)->timer = 0;
    }
    ((Tv1Tmpl *)s0)->scroll = 0x18;
end:
    { int *l; /* k is reused for the model (one variable keeps it in v1; a Tv1Model * local swaps v0 / v1) */
    k = *(int *)&((Tv1Tmpl *)s0)->model; l = (int *)((Tv1Model *)k)->screens;
    /* The two "screens" are BaseMats (Tv1Model +0x84 is the shape's material table, BasePolyShape.mats):
     * the frame image is blitted into each screen material's texels (+0x1C, gdl/material.h) */
    FCALL(Tv1Tmpl_blitFrame_000077D0)(s0, *(int *)&((BaseMat *)*l)->texels, 0, a3); if (1) {}
    k = *(int *)&((Tv1Tmpl *)s0)->model; l = (int *)((Tv1Model *)k)->screens + 1;
    FCALL(Tv1Tmpl_blitFrame_000077D0)(s0, *(int *)&((BaseMat *)*l)->texels, 1, a3); }
    ((Tv1Tmpl *)s0)->state = a3;
}

void Tv1Tmpl_update(int *a0) {
    ((Tv1Tmpl *)a0)->displayCount = 0;
    FCALL(Node_update)(a0);
}

extern int D_func_00007E70_a;
extern int D_func_00007E70_b;
void Tv1Tmpl_display(char *a0) {
    D_func_00007E70_a = 0;
    FCALL(Node_display)(a0);
    if (D_func_00007E70_b != 0) {
        int counter = ((Tv1Tmpl *)a0)->displayCount + 1;
        ((Tv1Tmpl *)a0)->displayCount = counter;
        if (counter == 1) {
            FCALL(Tv1Tmpl_stepPicture_00007D04)();
        }
    }
}

/* Fog_display - verified structural decode (0x1B0, 108 insns,
 * FP 1D collision / threshold-crossing test).
 * Struct-typing reference: s0->0x38 (56) f32 = position, s0->0x54
 * (84) f32 = velocity, s0->0x6C (108) f32 = a derived value,
 * s0->0x8C (140) s32 = a latched collision/zone state (compared
 * to the new hit flag - mismatch triggers the transition + the
 * snd_startPlayer2Seq_0000027C+0x18 transform-block copy to a global). g =
 * *(&D_g); g->0x130 (304) f32 = world scale, g->0x254 (596) ->
 * 0x70 (112) -> 0xA8 (168) f32 = the boundary/limit position
 * (scaled). The (limit - p) / vel is the time-of-impact fraction.
 * Global gate *(stream_readS32_00000008+0x2C) == 8 disables the test.
 * Likely a course/wall boundary crossing detector.
 * Caps <80: FP-heavy mul.s/c.lt.s/add.s/sub.s/div.s + bc1fl
 * branch-likely chain + &D + snd_startPlayer2Seq_0000027C+0x18 cross-symbol block
 * copy. INCLUDE_ASM remains build path. */
/* EXACT (agent-c run 115). Identities from the bootup.uso TextReloc: the
 * mode word is import_000200FC + 0x2C (= D_00000000), the globals block is
 * import_0005C108 (zero alias gGfxLib_7ec8_g: +0x130 scale, +0x254 game object,
 * +0x18 a 24-byte record), the backup record is import_0001FFC8 (bu_7ec8_h);
 * (the asm spells the mode word stream_readS32_00000008 + 0x2C = 0x34, alias
 * gInputMgr_7ec8_m, and the record snd_startPlayer2Seq_0000027C + 0x18 = 0x294, alias
 * gGfxLib_7ec8_src: the baked words carry those offsets); callees func_0586D8 (char *, char *, float: frac goes out by mfc1 a2),
 * func_063D70 (four ints then float/int/float on the stack) and func_051D88.
 * The record copies are 24-byte struct assignments; the scale is re-read
 * inline (a scale local colours f2/f0 the wrong way round) and written
 * scale * field for the target mul.s operand order. */
typedef struct { int a, b, c, d, e, f; } S24_7EC8;
extern char gGfxLib_7ec8_g[], bu_7ec8_h[], gGfxLib_7ec8_src[];
extern int gInputMgr_7ec8_m;
extern void func_00000000_7ec8_c(char *, char *, float);
extern int func_00000000_7ec8_t(int, int, int, int, float, int, float);
void Fog_display(char *s0) {
    float scale, limit, p, end, frac;
    int hit;
    if (gInputMgr_7ec8_m != 8) {
        frac = 0.0f;
        limit = GFXLIB(gGfxLib_7ec8_g)->worldScale[2] * ((MCamera *)GFXLIB(gGfxLib_7ec8_g)->curWindow->win.camera)->mnode.world[3][2];
        hit = 0;
        p = GFXLIB(gGfxLib_7ec8_g)->worldScale[2] * FOGITEM(s0)->pos[2];
        end = FOGITEM(s0)->fin.value + p;
        if (p < limit && limit < end) {
            hit = 1;
            frac = (limit - p) / FOGITEM(s0)->fin.value;
        } else if (end < limit && limit < FOGITEM(s0)->rng.value + p) {
            frac = 1.0f;
            hit = 1;
        }
        if (hit != FOGITEM(s0)->inside) {
            *(S24_7EC8 *)bu_7ec8_h = *(S24_7EC8 *)(gGfxLib_7ec8_src);
            FOGITEM(s0)->inside = hit;
        }
        if (hit != 0) {
            *(S24_7EC8 *)(gGfxLib_7ec8_src) = *(S24_7EC8 *)bu_7ec8_h;
            XCALL(func_00000000_7ec8_c, FogParms_lerp_0004406C)(gGfxLib_7ec8_src, FOGITEM(s0)->fog, frac);
        }
    } else {
        if (XCALL(func_00000000_7ec8_t, cam_isSphereInView)(*(int *)&GFXLIB(gGfxLib_7ec8_g)->curWindow, 3, *(int *)&FOGITEM(s0)->pos[0],
                          *(int *)&FOGITEM(s0)->pos[1], FOGITEM(s0)->pos[2], 0, 0.0f) != 0) {
            FCALL(ColourItem_display)(s0);
        }
    }
}

void Fog_hostioParms(char *a0) {
    FCALL(Parms_hostio)(a0 + 0x3C, 0);
    FCALL(FogParms_hostioParms)(a0 + 0x74);
    FCALL(Node_hostioParms)(a0);
}

void Fog_saveData(char *a0) {
    FCALL(Parms_save)((int*)(a0 + 0x3C));
    FCALL(gfx_saveQuadIntInt_00043FFC)((int*)(a0 + 0x74));
    FCALL(Node_saveData)(a0);
}

void Fog_readParms(char *a0) {
    FCALL(Parms_load_00035C6C)((int*)(a0 + 0x3C));
    FCALL(gfx_loadQuadIntInt_00044034)((int*)(a0 + 0x74));
    FCALL(ColourItem_load)(a0);
}

/* FogTmpl_create - verified structural decode (0x1D4, 117 insns).
 * NEAR-SIBLING of FloatPosParms_ctor / SnowTmpl_create (bootup_uso
 * get-or-create constructor + child sub-objects family); variant
 * params: object 0x90, init datum Fog_vtbl_7878, child descriptor
 * sStr_Parms_7FC4, child type FloatParm_vtbl_7540, child->0x10 = 200.0f
 * (0x43480000), and FP/&D constants from sTag_fin_7FCC/0x7FD0.
 *   void *FogTmpl_create(void *a0) {
 *       o = a0 ? a0 : alloc(0x90); if (!o) return 0;
 *       init(o, &Fog_vtbl_7878);
 *       o->0x28 = &D_desc;
 *       s = alloc(0x38);
 *       c = alloc(8);                              // defensive arm
 *       if (c) { c->0x0 = &sStr_Parms_7FC4; c->0x4 = 0; }
 *       v = *(int*)&sTag_fin_7FCC;
 *       if (s != (void*)-8) {
 *           c2 = alloc(0x18);
 *           if (c2) {
 *               init2(c2, s, v, 1);                // func_00000000
 *               c2->0xC  = &FloatParm_vtbl_7540;            // type/vtable
 *               c2->0x14 = 0;
 *               c2->0x10 = 200.0f;                 // 0x43480000
 *           }
 *       }
 *       ... (further child from sTag_rng_7FD0, same family shape)
 *   }
 * Struct-typing reference (same as FloatPosParms_ctor): o = 0x90-byte
 * object, o->0x28 (40) descriptor (&Fog_vtbl_7878). s = 0x38-byte
 * sub; c = 8-byte aux (c->0x0 = &sStr_Parms_7FC4 descriptor, c->0x4=0);
 * c2 = 0x18-byte child: c2->0xC (12) type ptr (&FloatParm_vtbl_7540),
 * c2->0x10 (16) f32 = 200.0 / 300.0, c2->0x14 (20) = 0. The u !=
 * -0x3C / -0x8 / -0x20 / -0x74 tests are the `c = u+k; if (c != 0
 * || alloc)` get-or-create guards folded by IDO. sTag_fin_7FCC /
 * sTag_rng_7FD0 = global init values feeding the children.
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
extern char Fog_vtbl_7878;
extern char sStr_Parms_7FC4;
extern int sTag_fin_7FCC;
extern int sTag_rng_7FD0;
extern char FloatParm_vtbl_7540;
char *FogTmpl_create(char *arg0) {
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
        NODE(o)->vptr = (char *)(&Fog_vtbl_7878);
        u = o + 0x3C;
        if (u != 0 || (u = (char *)func_00000000(0x38)) != 0) {
            c = u;
            if (c != 0 || (c = (char *)func_00000000(8)) != 0) {
                *(char **)(c + 0) = &sStr_Parms_7FC4;
                *(int *)(c + 4) = 0;
            }
            p.v = sTag_fin_7FCC; A[4] = p.v;
            c = u + 8;
            if (c != 0 || (c = (char *)func_00000000(0x18)) != 0) {
                func_00000000(w = c, u, p, 1);
                c = w;
                *(char **)(c + 0xC) = &FloatParm_vtbl_7540;
                *(int *)(c + 0x14) = 0;
                *(float *)(c + 0x10) = 200.0f;
            }
            p.v = sTag_rng_7FD0; A[3] = p.v;
            c = u + 0x20;
            if (c != 0 || (c = (char *)func_00000000(0x18)) != 0) {
                func_00000000(w = c, u, p, 1);
                c = w;
                *(char **)(c + 0xC) = &FloatParm_vtbl_7540;
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
        if (NODE(o)->parent != 0) {
            NODE(o)->unk04 = 1;
        }
        NODE(o)->parent = (Node *)v;
    }
    return v;
}
#else
INCLUDE_ASM("asm/nonmatchings/bootup_uso", FogTmpl_create);
#endif

/* Snower_display — EXACT (54/54 post-link) 2026-07-03 agent-e. CRACKED the
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
void Snower_display(int *a0) {
    float f0 = gGfx->worldScale[2];
    float f2 = ((MCamera *)gGfx->curWindow->win.camera)->mnode.world[3][2] * f0;
    float f12 = SNOWITEM(a0)->pos[2] * f0;

    if (f12 < f2) {
        if (SNOWITEM(a0)->rng.value + f12 > f2) {
            XCALL(func_00000000_082F8_v, EnvSettings_setSnow)(*(int*)&D_00000000_a, SNOWITEM(a0)->sta.value);
        }
    }

    /* GameFlow (Bss+0xF8 Sym 272) +0x34 through the value-8 alias: flow mode 8 = EDITORS */
    if (GAMEFLOW((char *)&D_func_00000008_data - 8)->nextFlowMode == 8) {
        if (XCALL(func_00000000_082F8, cam_isSphereInView)(
            *(int *)&gGfx->curWindow,
            3,
            *(int *)&SNOWITEM(a0)->pos[0],
            *(int *)&SNOWITEM(a0)->pos[1],
            SNOWITEM(a0)->pos[2],
            0,
            0.0f) != 0) {
            FCALL(ColourItem_display)(a0);
        }
    }
}

/* Snower_hostioParms: 30-insn (0x78) 5-call init/dispatch wrapper. Callee
 * func_00000000 is the K&R cross-USO placeholder.
 *
 * Decoded: single int* arg, makes 5 sequential calls, no return:
 *   call(&D, &sStr_Snower_7FD4, 0)
 *   call(&D, &sStr_Snow_7FDC, (int)a0+0x6C, 1)
 *   call((int)a0+0x3C, 0)
 *   call(&D)
 *   call(a0)
 *
 * Pattern: $a0 saved at +0x18(sp) (= caller's a0 slot above this 0x18-byte
 * frame) at function entry, reloaded at every call that needs it (calls
 * 2/3/5). All delay slots are filled with addiu/move/lw arg setup. */
extern char sStr_Snower_7FD4;
extern char sStr_Snow_7FDC;
void Snower_hostioParms(int *a0) {
    FCALL(hostio_beginGroup)(&gHostio, &sStr_Snower_7FD4, 0);
    FCALL(hostio_addFlag)(&gHostio, &sStr_Snow_7FDC, (int)a0 + 0x6C, 1);
    FCALL(Parms_hostio)((int)a0 + 0x3C, 0);
    FCALL(hostio_endGroup)(&gHostio);
    FCALL(Node_hostioParms)(a0);
}
/* Status 2026-05-18: build-path tested. 30/30 insns, build/.o vs
 * expected/.o differ at EXACTLY ONE word (w5, 0x14): the call-1
 * arg2. Target's .s uses %hi/%lo of LOCAL label `.L00007FD4`
 * (assembled-in intra-segment → expected/.o bakes addiu a1,a1,0x7FC4,
 * NO reloc). This NM uses extern `&sStr_Snower_7FD4` (sStr_Snower_7FD4=0x7FD4 in
 * undefined_syms) → emits a reloc with placeholder 0 (build/.o
 * 24A50000). The 0x10 delta (0x7FD4 vs 0x7FC4) = lui/addiu %hi-carry
 * of a local-label pair, NOT a flat symbol. w12 (&sStr_Snow_7FDC) is
 * CORRECT (matches .o-vs-.o). Fix path: reference the local
 * `.L00007FD4` segment data symbol at the value that bakes 0x7FC4 in
 * %lo (account for %hi carry), not the flat extern. Reloc word →
 * INSN_PATCH-unsafe. One-word symbol-form cap; rest byte-exact. */

void Snower_saveData(char *a0) {
    FCALL(Parms_save)((int*)(a0 + 0x3C));
    FCALL(Node_saveData)(a0);
}

void Snower_readParms(char *a0) {
    FCALL(Parms_load_00035C6C)((int*)(a0 + 0x3C));
    FCALL(ColourItem_load)(a0);
}

/* SnowTmpl_create - verified structural decode (0x1C4, 113 insns,
 * guarded singleton constructor). Member of the bootup_uso
 * alloc-cascade-ctor + defensive-dead-check family (cf.
 * item_createBal / FloatPosParms_ctor / MapProp_ctor).
 *   void SnowTmpl_create(void *a0) {
 *       g = *(St**)&D_root;
 *       if (g->0x40 != 0) return;                 // already built
 *       func_00000000(g);                          // pre-init
 *       o = alloc(0x74); if (!o) return;
 *       init(o, &Snower_vtbl_7968);
 *       o->0x28 = &D_desc;
 *       s = alloc(0x38); if (!s) return;
 *       c = alloc(8);                              // defensive arm
 *       if (c) { c->0x0 = &sStr_Parms_7FE4; c->0x4 = 0; }
 *       v = *(int*)&sTag_rng_7FEC;
 *       if (s != (void*)-8) {                      // defensive guard
 *           c2 = alloc(0x18);
 *           if (c2) init2(c2, s, v, 1);            // func_00000000
 *       }
 *       ... (chain continues: more sub-allocs, FP 300.0f
 *           (0x43960000) param, &sTag_sta_7FF0/FloatParm_vtbl_7540 datums,
 *           EnvSettings_setSnow helper - same shape as the family).
 *   }
 * Struct-typing reference: D_root global -> g, g->0x40 (64) =
 * built-once latch (nonzero = skip). o = 0x74-byte object,
 * o->0x28 (40) descriptor (&Snower_vtbl_7968 init datum). s = 0x38-byte
 * sub-object; c = 8-byte aux (c->0x0 = &sStr_Parms_7FE4 descriptor,
 * c->0x4 = 0); c2 = 0x18-byte child init'd from the global
 * sTag_rng_7FEC value. The `u != -0x3C / -0x8 / -0x20` tests are the
 * `c = u+k; if (c != 0 || alloc)` get-or-create guards folded by
 * IDO. FP const 0x43960000 = 300.0f; block2 kind a3=0 and child2
 * gets {0x10=1 (int), 0xC=&EnvSettings_setSnow+0x70, 0x14=0} instead of
 * the float form.
 * 2026-07-30 agent-g: 13D40 recipe applied (62.5 -> 93.3, 113/113
 * structure, ALL homed slots exact via int A[6] + q2 dead decl
 * (9-word odd parity pads 0x20, p 0x24, q2 0x28, w 0x2C, A base
 * 0x30); v=o tail-reuse lever as 8124. Residual = 13D40
 * allocator-web cap (p.v webs coalesce to v0, t-cycle renumber,
 * u-spill/frame +0x10 reserved words, sw-a0 window x2, tail
 * lw-v0/move-s0 elide) + `(char *)EnvSettings_setSnow + 0x70` emits
 * separate addiu 0x70 (addend does not fold into %lo since the
 * symbol is a real fn in this TU, unlike the extern-array
 * sTimeGadgetParmsTags_CACC form in 1438C) + head g = *(char**)&D_00000000
 * carries HI/LO relocs where target's lui/lw pair is baked
 * reloc-free (baked-USO-symbol cap; bytes identical). Sub-100 ->
 * NM wrap per policy. */
#ifdef NON_MATCHING
typedef struct { int v; } Val84A0;
extern char Snower_vtbl_7968;
extern char sStr_Parms_7FE4;
extern int sTag_rng_7FEC;
extern int sTag_sta_7FF0;
extern char FloatParm_vtbl_7540;
char *SnowTmpl_create(char *arg0) {
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
        NODE(o)->vptr = (char *)(&Snower_vtbl_7968);
        u = o + 0x3C;
        if (u != 0 || (u = (char *)func_00000000(0x38)) != 0) {
            s = u;
            if (s != 0 || (s = (char *)func_00000000(8)) != 0) {
                *(char **)(s + 0) = &sStr_Parms_7FE4;
                *(int *)(s + 4) = 0;
            }
            p.v = sTag_rng_7FEC; A[4] = p.v;
            c = u + 8;
            if (c != 0 || (c = (char *)func_00000000(0x18)) != 0) {
                func_00000000(w = c, u, p, 1);
                c = w;
                ((CourseFParm *)c)->vptr = &FloatParm_vtbl_7540;
                ((CourseFParm *)c)->value2 = 0;
                ((CourseFParm *)c)->value = 300.0f;
            }
            p.v = sTag_sta_7FF0; A[3] = p.v;
            c = u + 0x20;
            if (c != 0 || (c = (char *)func_00000000(0x18)) != 0) {
                func_00000000(w = c, u, p, 0);
                c = w;
                ((CourseParm *)c)->value = 1;
                ((CourseParm *)c)->vptr = (char *)EnvSettings_setSnow + 0x70;
                ((CourseParm *)c)->value2 = 0;
            }
        }
    }
    v = o;
    o = *(char **)(arg0 + 0x40);
    if (o != 0) {
        func_00000000(v + 0x10, o);
        if (NODE(o)->parent != 0) {
            NODE(o)->unk04 = 1;
        }
        NODE(o)->parent = (Node *)v;
    }
    return v;
}
#else
INCLUDE_ASM("asm/nonmatchings/bootup_uso", SnowTmpl_create);
#endif

void Tree_handleMsg(char *a0, int *a1) {
    FCALL(SNode_handleMsg)(a0);
    if (*a1 == 7 || *a1 == 19) {
        TREE(a0)->coll.obj.pos[0] = TREE(a0)->snode.pos[0];
        TREE(a0)->coll.obj.pos[1] = TREE(a0)->snode.pos[1];
        TREE(a0)->coll.obj.pos[2] = TREE(a0)->snode.pos[2];
    }
}

/* TreeTmpl_display: 33-insn (0x84) two-init + linked-list dispatcher.
 *
 * Matching notes: reusing arg0 as the list iterator makes IDO keep it in
 * $s0 across the two init calls; the separate `node` null-test produces the
 * target's `lw v0` + branch-delay `or s0,v0` loop entry. The direct function
 * pointer call keeps the callback in $t9. */
extern int func_00000000();
extern char gGfxLib_00000294;

void TreeTmpl_display(char *arg0) {
    char *node;
    char *obj;

    FCALL(gfx_setPrimColour)(&gGfxLib, TREETMPL(arg0)->colour);
    FCALL(FogParms_emit_00043F1C)(TREETMPL(arg0)->fog);

    node = (char *)TREETMPL(arg0)->items;
    if (node != 0) {
        arg0 = node;
        do {
            if ((NODE(arg0)->doFlags & 4) != 0) {
                obj = (char *)NODE(arg0)->vptr;
                ((int (*)(char *))VT_SLOT(obj, 2).fn)(arg0 + VT_SLOT(obj, 2).delta);
            }
            arg0 = (char *)((SNode *)arg0)->next;
        } while (arg0 != 0);
    }
    FCALL(FogParms_emit_00043F1C)(&gGfxLib_00000294);  /* restore the scene fog: GfxLib +0x294 (snd_startPlayer2Seq_0000027C+0x18) */
}

extern char gHostio_func_00008744_arg1;
extern char gHostio_func_00008744_arg2;
void TreeTmpl_hostioParms(char *a0) {
    int arg = (int)NODE(a0)->name;
    FCALL(hostio_beginGroup)(&gHostio_func_00008744_arg1, arg, 0);
    FCALL(gfx_declColourParm)(TREETMPL(a0)->colour, 0);
    FCALL(FogParms_hostioParms)(TREETMPL(a0)->fog);
    FCALL(hostio_endGroup)(&gHostio_func_00008744_arg2);
    FCALL(Node_hostioParms)(a0);
}

extern char sStr_tree_7FF4;
extern float kRo_TreeTmpl_create_5F0;

/* TreeTmpl_create: allocator/init sibling of TreeTmpl_display. EXACT 2026-10-16
 * (agent-c). The old caps were all C-reachable:
 *  - the three bare `lui r,0` bases at the top are three DISTINCT base-0
 *    globals (gPolyFlags_3EEC4_87A4_rd read, gPolyFlags_3EEC4_87A4_wr OR-store, gPolyGeomMode_3EEC8_87A4_cfg +4/+0 stores),
 *    pinned to 0. An empty `if (1) {}` after the OR-store makes IDO hoist the
 *    rd/wr luis above the bnez and put the cfg lui in its delay slot (without
 *    it the cfg lui/addiu pair is hoisted instead).
 *  - the single-float 5th arg (swc1, no cvt.d.s) comes from a prototyped
 *    alias of the blank, func_00000000_087A4_f(char *, char *, int, float,
 *    float); 150.0f rides in a3 as bits. Unused-result calls go through the
 *    void alias func_00000000_087A4.
 *  - &boot_readFloatParm_00007A48 + 0x10 through the data alias D_087A4_vtable (pinned to
 *    0x7A48) folds the addend into %lo (a fn-typed symbol emits a separate
 *    addiu).
 *  - frame 0x50 with kind at sp+0x44: no named `desc` local (each extra named
 *    pointer took a ghost home) plus `int pad[5]` declared after kind.
 *  - the three trailing nops are file-end alignment pad (all-zero
 *    SUFFIX_BYTES_FORCE). */
extern int gPolyFlags_3EEC4_87A4_rd, gPolyFlags_3EEC4_87A4_wr, D_87A4_tail;
extern char gPolyGeomMode_3EEC8_87A4_cfg, D_87A4_vt;
extern char D_087A4_vtable[];
extern void func_00000000_087A4();
extern void func_00000000_087A4_f(char *, char *, int, float, float);
void *TreeTmpl_create(char *arg0) {
    char *s1;
    char *s0;
    int kind; int pad[5];

    if (TREETMPL(arg0)->shape == 0) {
        gPolyFlags_3EEC4_87A4_wr = gPolyFlags_3EEC4_87A4_rd | 8;
        if (1) {}
        /* gPolyGeomMode_3EEC8's two mode words (gl_data.h): [1] = the geometry-mode bits */
        ((int *)&gPolyGeomMode_3EEC8_87A4_cfg)[1] = 0xAA002;
        ((int *)&gPolyGeomMode_3EEC8_87A4_cfg)[0] = 0;
        TREETMPL(arg0)->shape = FCALL(PolyShape_ctor)(0, &sStr_tree_7FF4, TREETMPL(arg0)->shapeData, 0);
    }
    s1 = (char *)FCALL(mem_new)(0xC8);
    if (s1 != 0) {
        XCALL(func_00000000_087A4, SNode_ctor)(s1);
        NODE(s1)->vptr = (char *)(D_087A4_vtable + 0x10);
        s0 = (char *)&TREE(s1)->coll;
        if (s0 == 0) {
            s0 = (char *)FCALL(mem_new)(0x40);
            if (s0 == 0) {
                goto skip;
            }
        }
        XCALL(func_00000000_087A4, coll_boxCtor_0003CAA0)(s0);
        COLLOBJ(s0)->vptr = (char *)&D_87A4_vt;
    }
skip:
    TREE(s1)->snode.scale = 10.0f;
    kind = 0x13;
    s0 = s1;
    ((void (*)(char *, int *))VT_SLOT(NODE(s1)->vptr, 4).fn)(s1 + VT_SLOT(NODE(s1)->vptr, 4).delta, &kind);
    s1 = (char *)TREETMPL(arg0)->shape;
    XCALL(func_00000000_087A4, list_append)(&NODE(s0)->children, s1);
    if (NODE(s1)->parent != 0) {
        NODE(s1)->unk04 = 1;
    }
    NODE(s1)->parent = (Node *)s0;
    XCALL(func_00000000_087A4_f, CollObj_initCylinder)((char *)&TREE(s0)->coll, s0, 0x74, 150.0f, kRo_TreeTmpl_create_5F0);
    TREE(s0)->coll.obj.flags |= 1;
    XCALL(func_00000000_087A4, CollObjMgr_addStatic)(D_87A4_tail, &TREE(s0)->coll);
    return s0;
}

extern char *gInputMgr_00000130;
int game_isUsoLoaded(int a0) {
    FlowUso *p = (FlowUso *)gInputMgr_00000130; /* GameFlow.usoList */
    while (p != 0) {
        if (FCALL(str_compare_00067B04)(p->handle, a0) == 0) {
            return 1;
        }
        p = p->next;
    }
    return 0;
}

int game_loadUsoOnce(int arg0) {
    if (FCALL(game_isUsoLoaded)(arg0) == 0) {
        FCALL(game_loadUso)(&gInputMgr, arg0);
        return 1;
    }
    return 0;
}

void stream_readS32_000089C0(int *dst) {
    int buf[2];
    FCALL(fs_gdlReadFile)(&gSysLib, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

void stream_read16_000089FC(short *dst) {
    short buf[4];
    FCALL(fs_gdlReadFile)(&gSysLib, buf, 2);
#ifdef PORT
    PORT_BE_SWAP16S(buf, 2);
#endif
    *dst = buf[0];
}

void boot_emptyStub_00008A38(int a0) {
}

void stream_readF32_00008A40(float *dst) {
    float buf[2];
    FCALL(fs_gdlReadFile)(&gSysLib, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

extern void stream_readVec3_00008A7C();
void stream_readVec3_00008A7C(Vec3 *dst) {
    int pad_top[1];
    Tri3i raw;
    int pad_mid[2];
    Tri3i tmp;
    int pad_bot[2];
    FCALL(fs_gdlReadFile)(&gSysLib, &raw, 12);
#ifdef PORT
    PORT_BE_SWAP32S(&raw, 12);
#endif
    tmp = raw;
    dst->x = *(float*)&tmp.a;
    dst->y = *(float*)&tmp.b;
    dst->z = *(float*)&tmp.c;
}

void stream_readQuad_00008AEC(Quad4 *dst) {
    Quad4 buf;
    FCALL(fs_gdlReadFile)(&gSysLib, &buf, 16);
#ifdef PORT
    PORT_BE_SWAP32S(&buf, 16);
#endif
    *dst = buf;
}

/* SnowBCollider_ctor: 36-insn (0x90) alloc-or-given init. BYTE-EXACT (0 non-reloc
 * diffs, 36/36 insns). SnowBoarder_handleMsg (still INCLUDE_ASM) takes the address of
 * two interior points of this function (8B44+0x28, 8B44+0x2C); its .s now
 * references them as `SnowBCollider_ctor + 0x28 / + 0x2C` (identical relocation
 * bytes) instead of the now-removed local labels `.L00008B6C` / `D_00008B70`.
 *
 * Two levers that took the prior 87.2% NM to byte-exact:
 *  - the (u16)a2 store must read the saved int slot's low halfword by MEMORY
 *    (`((unsigned short*)&a2)[1]` -> lhu 0x2A(sp)), not reload+truncate (lw).
 *  - the short store (0x174) must be emitted BEFORE the float stores in C so
 *    as1 schedules the 1.0f/10.0f lui/mtc1/swc1 materialization to match.
 * typed-float proto func_8b44_init keeps the 5th arg a single swc1. */
extern void func_8b44_init(char *, int, int, int, float);
char* SnowBCollider_ctor(char *a0, int a1, int a2, int a3, int a4, float f_arg) {
    if (a0 == 0) {
        a0 = (char*)FCALL(mem_new)(0x178);
        if (a0 == 0) goto end;
    }
    func_8b44_init(a0, a1, a3, a4, f_arg);
    NODE(a0)->vptr = (char *)&SnowBCollider_vtbl;
    SNOWBCOL(a0)->rider = (struct SnowBoarder *)a1;
    SNOWBCOL(a0)->contactFlags = ((unsigned short*)&a2)[1];
    SNOWBCOL(a0)->unk150 = 1.0f;
    SNOWBCOL(a0)->unk154 = 10.0f;
end:
    return a0;
}

// SnowBCollider_onContact (0x344 / 209 insns): summary of the donor body.
// Collision impulse / force-application onto a physics body, gated by
// the body's flag-class (body->0x174) and the impulse magnitude.
//
//   void SnowBCollider_onContact(Body *b, float amt, Vec3 *dir, x a3) {
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
/* SnowBCollider_onContact: EXACT via the IDO 7.1 NCC donor src/bootup_uso/bootup_uso_ncc_8BD4.cpp
 * (the real body, on SnowBCollider fields), REPLACE_FUNC_BODY-spliced over this placeholder on both
 * build paths (agent-c run 238). The superseded in-unit decode was deleted in agent-h run 505 (git
 * history has it). */
void SnowBCollider_onContact(char *b) {
    volatile char *r = b;
}

/* ShpNode_ctor: 36-insn (0x90) optional-alloc + multi-call init. */
extern char sStr_QNode_8708;
extern char sStr_shp_8710;
extern char ShpNode_vtbl_8F18_vtable;
extern char D_8F18_call_arg;
char *ShpNode_ctor(char *self) {
    int handle;
    if (self == 0) {
        self = (char*)FCALL(mem_new)(0x3D8);
        if (self == 0) goto end;
    }
    FCALL(DNode_ctor)(self, &sStr_QNode_8708);
    NODE(self)->vptr = (char *)(&ShpNode_vtbl_8F18_vtable);
    handle = FCALL(PolyShape_ctor)(0, &sStr_shp_8710, &D_8F18_call_arg, 0);
    FCALL(MNode_addChild)(self, handle);
    SHPNODE(self)->life = 100;
    SHPNODE(self)->dnode.parms.rad.value = 50.0f;
end:
    return self;
}

/* ShpNode_update: 73-insn (0x124) "scatter-bitcast-int-to-float-vec3-fields"
 * + counter decrement + global-table update. Sibling of recently-matched
 * ShpNode_ctor (alloc + multi-call init).
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
 * &sStr_Too_many_objects_for_List_8714 address into the pre-branch delay region, forcing `count`
 * off $a0 onto $v1 (coloring-renumber), plus the tbl-spill slot lands at
 * 0x1C instead of 0x40. Pure scheduler-tie + coloring; not C-fixable.
 * Run 129 (agent-c) 15 -> 13 raw: tbl declared at function scope between
 * `volatile int pad_t[1]` and `pad_hi[2]` -- its home IS the spill slot, now
 * sp+0x40 as the target. LEFT: count v1 (target a0) lets the &D_8714 lui
 * hoist above the compare; while (0) / trailing if (count) {} are flat.
 * Run 131 (agent-c) EXACT: the pre-realloc compare reads tbl[3] directly and
 * count is assigned only AFTER the if (one load, not a count copy reloaded in
 * the then-arm) -> count colours a0 and the &D_8714 lui stays in the arm. */
extern char sStr_Too_many_objects_for_List_8714;
typedef struct { int a, b, c; } Vec3i_8FA8;
void ShpNode_update(int *self) {
    volatile int pad_t[1];
    int *tbl;
    volatile int pad_hi[2];
    Vec3i_8FA8 scratch;
    volatile int pad_lo[2];
    scratch = *(Vec3i_8FA8 *)SHPNODE(self)->dnode.vqs.trans;
    SHPNODE(self)->dnode.cvqs.trans[0] = *(float*)&scratch.a;
    SHPNODE(self)->dnode.cvqs.trans[1] = *(float*)&scratch.b;
    SHPNODE(self)->dnode.cvqs.trans[2] = *(float*)&scratch.c;
    scratch = *(Vec3i_8FA8 *)SHPNODE(self)->dnode.vqs.scale;
    SHPNODE(self)->dnode.cvqs.scale[0] = *(float*)&scratch.a;
    SHPNODE(self)->dnode.cvqs.scale[1] = *(float*)&scratch.b;
    SHPNODE(self)->dnode.cvqs.scale[2] = *(float*)&scratch.c;
    scratch = *(Vec3i_8FA8 *)SHPNODE(self)->dnode.vqs.quat;
    SHPNODE(self)->dnode.cvqs.quat[0] = *(float*)&scratch.a;
    SHPNODE(self)->dnode.cvqs.quat[1] = *(float*)&scratch.b;
    SHPNODE(self)->dnode.cvqs.quat[2] = *(float*)&scratch.c;
    SHPNODE(self)->dnode.cvqs.quat[3] = SHPNODE(self)->dnode.vqs.quat[3];
    FCALL(DNode_update)(self);
    {
        int counter = SHPNODE(self)->life;
        SHPNODE(self)->life = counter - 1;
        if (counter <= 0) {
            int count;
            tbl = *(int**)&D_00000000;
            if (tbl[3] >= tbl[2]) {
                FCALL(sys_fatal)(&sStr_Too_many_objects_for_List_8714, self);
            }
            count = tbl[3];
            tbl[3] = count + 1;
            *(int*)(tbl[0] + count * 4) = (int)self;
        }
    }
}

/* SnowBoarder_ctor: EXACT via the IDO 7.1 cfe donor src/bootup_uso/bootup_uso_90CC.c, REPLACE_FUNC_BODY-spliced
 * over the placeholder below on both build paths (agent-c run 227). The superseded in-unit decode is kept
 * for reference under BOOTUP_90CC_OLD_DECODE (never defined). */
void *SnowBoarder_ctor(char *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5, f32 arg6, s32 arg7, s32 arg8) {
    volatile char *r = arg0;
    return 0;
}

// SnowBoarder_loadBoardModel_0000B1B4 — STRUCTURAL PASS (0x2E8 / 186 insns, no episode).
// Sound/resource-bank lazy loader + global-table registrar + play
// dispatcher.
//
//   void SnowBoarder_loadBoardModel_0000B1B4(int cat, int idx, a2, a3) {
//     if (idx >= 0xA) idx = 9;               // clamp [0,9]
//     if (cat >= 9)   cat = 8;               // clamp [0,8]
//     if (cat == 7 || cat == 6) {            // banked categories
//       if (*(int*)(snd_seqEventDbg_00000148) == 0) {   // not-yet-loaded guard
//         func_00000000(&stream_read16_000089FC+0x2C);              // a name/path
//         R *r = func_00000000(0x90);  if (!r) goto reg;    // alloc (retry)
//         S *s = func_00000000(0x60);
//         s->0x5C = &D_0;                                   // vtable
//         // optional sub-allocs keyed by sentinel addrs (-0x34/-0x58):
//         //   alloc 8 -> {0,0} ; alloc 4 -> {0}
//         func_00000000(s);  s2->0x5C = &D_0;
//         r->0x24 = 0x110;  r->0x5C = &Snower_hostioParms+0x70;  // folded vtbl
//         void *buf = func_00000000(0x1000);
//         r->0x28 = 0x1100;  r->0x22 = (s16)0x20;  r->0x20 = (s16)0x40;
//         r->0x38 = 0x402C4B;  r->0x1C = buf;  r->0x34 = 0;  r->0x18 = 0;
//       reg:
//         if (*(int*)(snd_seqEventDbg_00000148+0x14) >= 5) func_00000000(r);  // evict
//         int n = *(int*)(snd_seqEventDbg_00000148+0x14);
//         *(int*)(snd_seqEventDbg_00000148+0x14) = n + 1;
//         ((void**)snd_seqEventDbg_00000148)[n] = r;                   // register
//       }
//     }
//     func_00000000(&stream_readF32_00008A40+0x8, 0);
//     func_00000000(&stream_readF32_00008A40+0x14, 0);
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
//     0x34 flag, 0x38=0x402C4B id, 0x5C vtable(&Snower_hostioParms+0x70).
//   snd_seqEventDbg_00000148 = WRITABLE global resource table: [0]=loaded-root,
//     [0x14]=entry count, [n] = registered R ptrs. tableA/tableB =
//     other global index->entry arrays.
//   Folded refs (multi-class fold family — writable globals + tables +
//   vtables + name data; see
//   docs/N64_FORENSICS.md#bootup-uso-fp-literal-pool-folded-into-func-0000098C):
//     snd_seqEventDbg_00000148 (writable table), stream_read16_000089FC+0x2C /
//     stream_readF32_00008A40+0x8/+0x14 (name/string data), Snower_hostioParms+0x70
//     (vtable).
// Caps (DEFERRED): 186-insn lazy-init + table-register + multi-dispatch.
//   2026-05-31 partial completion 55.9->67.1%: added the two sentinel-keyed
//   sub-allocs (alloc(8)->s+0x34={0,0} guarded by s==-52; alloc(4)->s+0x58={0}
//   guarded by s==-88, dead-arm passthrough form) and fixed the clamps to
//   UNSIGNED (sltiu: (unsigned)idx>=0xA, (unsigned)cat>=9).
//   2026-06-23 decode pass (agent-t) 161->177 insns (target 186), sim
//   35.7->37.5%: (1) CORRECTNESS FIX — replaced all D_00000000+off named-table
//   refs with the REAL symbols the .s relocs name: snd_seqEventDbg_00000148 (writable
//   table+flag+count), stream_read16_000089FC+0x2C (name data), Snower_hostioParms+0x70
//   (vtable), stream_readF32_00008A40+0x8/+0x14 (string data). Build reloc set now
//   matches target's named relocs. (2) Reconstructed the nested ||-allocator-
//   fallback cascade (m2c shape): r=alloc(0x90); r||(r=alloc) ; s=r||(s=alloc
//   0x60) ; the s+0x34 / s+0x58 sentinel sub-allocs. (3) 5th stack arg a4 +
//   a2/a4 home copies. RESIDUAL (37.5->100), genuine IDO -O2 caps:
//   * CSE-COLLAPSE: target re-materializes &snd_seqEventDbg_00000148 at 5 sites (5x HI16);
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
 * Reg::add() on the 5-slot table at snd_seqEventDbg_00000148 doing `tbl[n++] = b`. */
void SnowBoarder_loadBoardModel_0000B1B4(unsigned int cat, unsigned int idx, int a2, int a3, int a4) {
    volatile unsigned int r = cat;
}

/* SnowBoarder_createBoardShape_0000B49C: 33-insn (0x84) clamped-index dispatcher. EXACT 2026-07-15.
 *
 *   j = clamp(idx, 9); v = table1[j];
 *   if (gl_func(v, j) == 0) gl_func(&table1, v);
 *   gl_func(0, (char*)stream_readF32_00008A40 + 0x18, table2[j], 0);   // dispatch
 *
 * The dispatched function ptr is `stream_readF32_00008A40 + 0x18` — a code address
 * inside an existing function (alt-entry); sStr_board_8A58 folds the +0x18.
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
extern char sStr_board_8A58;
extern int D_00000000_b49c[];  /* base-0 alias: 3rd table's separate lui (no CSE with 1st table's addr temp) */
void SnowBoarder_createBoardShape_0000B49C(int idx, int j) {
    j = idx;
    if (j >= 10) j = 9;
    if (CALL(game_isUsoLoaded)(((int*)&D_00000000)[j], j) == 0) {
        CALL(game_loadUso)(&D_00000000, ((int*)&D_00000000)[j]);
    }
    CALL(PolyShape_ctor)(0, &sStr_board_8A58, D_00000000_b49c[j], 0);  /* stream_readF32_00008A40+0x18 */
}

/* SnowBoarder_limitSpeed - verified structural decode (0x23C, 143 insns,
 * FP directional-nudge / input-driven position update).
 *   void SnowBoarder_limitSpeed(St *a1) {
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
 * undefined_syms_auto.txt; target FogTmpl_create + K / SndHandle_setScaleIdx15 + 0xC).
 * PASS-2 2026-10-16 (agent-c, 36.90 -> 99.34): m2c had the Vec3 copies as
 * int structs with (f32)/(s32) casts (4 cvt + 4 trunc too many) and a K&R call
 * carrying stale f12/f14. 143/143 words; 19 differ: the 0.0 constant / s
 * swap f0<->f2, the v5C copy base v1 (target v0), and the four pool addends
 * (k48/k44/kc need pins for an exact). Same shape as SnowBoarder_handleMsg: named
 * products, v50 -> v5C -> v74 struct copies, decl-order frame (an unused
 * Vec3 between v74 and v5C, pad[3] below v50). Also: q = arg0 + 0x3C8 assigned
 * before the if (target re-forms it on both join edges, addiu v0,a1,968); the
 * second flag test re-reads p->0x10 (volatile read; plain CSEs the lw); the
 * 0x48 limit read inline at both uses; the call is (arg0 + 0x318, arg0). */
extern f32 bu_b520_k48, bu_b520_k44;  /* rodata f32 (target FogTmpl_create + 0x48 / + 0x44) */
extern f64 bu_b520_kc;               /* rodata f64 (target SndHandle_setScaleIdx15 + 0xC) */
void SnowBoarder_limitSpeed(SnowBoarder *sb) {
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

    q = (char *)sb->dnode.axes[2]; /* dir */
    t = 0.0f;
    if (sb->flags & SNOWB_FLAG_NUDGE) {
        p = (char *)sb->input;
        if (((RiderInput *)p)->held & 4) {
            t += 10.0f;
        }
        if (*(volatile int *)&((RiderInput *)p)->held & 8) {
            t -= 10.0f;
        }
    }
    px = ((f32 *)q)[0] * t;
    py = ((f32 *)q)[1] * t;
    pz = ((f32 *)q)[2] * t;
    if (1) {}
    v50.x = px; v50.y = py; v50.z = pz;
    v5C = v50;
    v74 = v5C;
    sb->dnode.vel[0] = sb->dnode.vel[0] + v74.x;
    sb->dnode.vel[1] = sb->dnode.vel[1] + v74.y;
    sb->dnode.vel[2] = sb->dnode.vel[2] + v74.z;
    if (((s32)sb->flags << 14) < 0) {  /* SNOWB_FLAG_CATCHUP */
        p = (char *)sb->rival;
        t = sb->parms.max.value;
        if (p != NULL) {
            d = ((SnowBoarder *)p)->dnode.vqs.trans[2] - sb->dnode.vqs.trans[2];
            if (bu_b520_k48 < d) {
                t += bu_b520_k44;
            } else if (d < -bu_b520_k48) {
                t -= bu_b520_k44;
            }
        }
        k = sb->maxSpeed;
        sb->maxSpeed = (f64)k + (f64)(t - k) * bu_b520_kc;
    } else {
        sb->maxSpeed = sb->parms.max.value;
    }
    if (sb->maxSpeed < sb->dnode.speed) {
        FCALL(math_vec3Normalize)(sb->dnode.vel, sb);  /* normalize */
        k = sb->maxSpeed;
        sb->dnode.vel[0] = sb->dnode.vel[0] * k;
        sb->dnode.vel[1] = sb->dnode.vel[1] * k;
        sb->dnode.vel[2] = sb->dnode.vel[2] * k;
    }
    if (sb->contactCount == 0) {
        k = 1.0f - sb->parms.wfr.value;
        sb->dnode.vel[0] = sb->dnode.vel[0] * k;
        sb->dnode.vel[2] = sb->dnode.vel[2] * k;
    }
}

// SnowBoarder_updateOrient — STRUCTURAL PASS (0x830 / 524 insns, no episode).
// agent-g run 242: the NM comparison object now uses the NCC port bootup_uso_ncc_B75C_nm.cpp (edg-shaped,
// 99.97, 524 words, frame 120); the cfe body below is kept as the reference lift.
// Per-frame camera/transform interpolation update over a large scene-
// state struct (compute-heavy: f64 lerp/curve math, only 11 dispatcher
// calls; ~70 accesses to the state struct via a3 = a0).
//
//   void SnowBoarder_updateOrient(State *st) {           // st kept in a3
//     // local default Vec @ sp+0x6C..0x74 = {0.0, 1.0f, 0.0}
//     float t = *(float*)st->0x800;            // a normalized phase/time
//     double k0 = DBL(SndHandle_setScaleIdx15+0x14), k1 = DBL(SndHandle_setScaleIdx15+0x1C);
//     // curve eval: scale t (as double) by the folded f64 coefficient
//     //   bank, producing eased blend factors;
//     if (st->0x960 != 0x64) { ... uses st->0x974 ... }   // state guard
//     // main body: many double mul/add/sub chains driven by the
//     //   SndHandle_setScaleIdx15 / SndHandle_setScaleIdx31 / SndHandle_triggerEvent0_000008F4 coefficient
//     //   table (each symbol = 4 f64 at +0x4/+0xC/+0x14/+0x1C) plus
//     //   FogTmpl_create+0x4C — interpolates a transform/orientation
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
//   not scattered singletons: SndHandle_setScaleIdx15 / SndHandle_setScaleIdx31 /
//   SndHandle_triggerEvent0_000008F4 each carry 4 f64 at +0x4/+0xC/+0x14/+0x1C, plus
//   FogTmpl_create+0x4C; see
//   docs/N64_FORENSICS.md#bootup-uso-fp-literal-pool-folded-into-func-0000098C).
// Caps (DEFERRED): 524-insn f64 interpolation w/ folded coefficient
//   table — byte-match blocked by deferred pool symbolization.
//   Real-C STRUCTURAL body below — per-frame camera/transform
//   interpolation skeleton. Name pre-checked: no extern reuse.
/* SnowBoarder_updateOrient: EXACT via the IDO 7.1 NCC donor src/bootup_uso/bootup_uso_ncc_B75C.cpp, REPLACE_FUNC_BODY-spliced
 * over the placeholder below on both build paths (agent-c run 258: the 0x990 load read in place at both tests +
 * the dot3 value held in var_f0 = the target's uopt spilltemps slot order). The superseded cfe lift is kept for
 * reference under BOOTUP_B75C_OLD_DECODE (never defined). */
void SnowBoarder_updateOrient(char *arg0) {
    volatile char *r = arg0;
}

/* SnowBoarder_updateDyn - verified structural decode (0x2A8, 170 insns,
 * large FP per-frame parameter selector/update).
 *   void SnowBoarder_updateDyn(St *s0) {
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
 *           ... uses SndHandle_triggerEvent0_000008F4+0x20 / stream_readF32_00008A40 ...
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
 * resolved reloc symbols (SndHandle_triggerEvent0_000008F4+0x20 f32 / +0x24 f64 const-pool;
 * stream_readF32_00008A40+0x20 / +0x2C dispatch-target addresses passed to the
 * allocator func_00000000) in place of the old &D_00000000 placeholders.
 * Build emits EXACTLY 186 instructions = target's 186 (same opcode
 * multiset, all relocs correct: 2x SndHandle_triggerEvent0_000008F4 HI/LO, 2x stream_readF32_00008A40
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
 *   (3) RESOLVED 2026-07-30 (agent-g): the stream_readF32_00008A40+0x2C tail ref is
 *       anchor-split to &boot_emptyStub_00008A38+0x34 (same absolute 0x8A6C, distinct
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
extern f32 D_BF8C_pool8F4[]; /* data views of the SndHandle_triggerEvent0_000008F4 const pool (NM only; pins = SndHandle_triggerEvent0_000008F4 / SndHandle_triggerEvent0_000008F4 + 0x24 if promoted) */
extern f64 D_BF8C_pool8F4_24;
extern char D_BF8C_8A40[]; /* data view of stream_readF32_00008A40 (dispatch-target addresses; pin = stream_readF32_00008A40 if promoted) */
void SnowBoarder_updateDyn(char *arg0) {
    /* agent-g run 470: field form (snowboarder.h). The statement order, the nested-!= goto chain and the
     * volatile turnRate parm reads are the run-108 comparison levers; keep them. */
    SnowBoarder *sb = (SnowBoarder *)arg0;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 var_f12;
    f32 var_f2;
    s16 temp_v0_2;
    s32 temp_v0_3;
    char *temp_v0;

    var_f12 = 0.0f;
    temp_v0 = (char *)sb->input;
    sb->crouch = (f64) (*(f32 *)((char *)temp_v0 + 0x4)) < -0.75; /* input stick y */
    if ((sb->useDParms = (*(s32 *)((char *)temp_v0 + 0x10) & 0x400) != 0, sb->edgeFriction = sb->child.parms.efr.value, sb->useDParms) != 0) {
        sb->edgeGrip = sb->child.parms.del.value;
        sb->dnode.friction = sb->child.parms.dbf.value;
        sb->turnRate = (f32) ((*(volatile f32 *)&sb->child.parms.res.value) * sb->parms.dts.value);
    } else {
        sb->edgeGrip = sb->child.parms.nel.value;
        sb->dnode.friction = sb->child.parms.nbf.value;
        sb->turnRate = (f32) ((*(volatile f32 *)&sb->child.parms.res.value) * sb->parms.nts.value);
    }
    if (sb->crouch != 0) {
        if (sb->useDParms != 0) {
            var_f12 = D_BF8C_pool8F4[8];
        } else {
            var_f12 = 0.5f;
        }
        temp_f0 = sb->edge;
        if (temp_f0 < 0.0f) {
            var_f2 = -temp_f0;
        } else {
            var_f2 = temp_f0;
        }
        sb->dnode.friction = (f32) (sb->dnode.friction + (var_f2 * var_f12));
    }
    temp_v0_2 = sb->surface;
    if (temp_v0_2 != 0x63) {
        if (temp_v0_2 != 0x61) {
            if (temp_v0_2 != 0x62) {
        /* off the ground (surface 0): extra friction, no edge terms */
        sb->dnode.friction = (f32) (D_BF8C_pool8F4_24 + (f64) sb->dnode.friction);
        sb->edgeGrip = 0.0f;
        sb->edgeFriction = 0.0f;
            } else goto L62;
        } else goto Lend;
    } else goto L63;
    goto Lend;
L63: /* surface 'c' */
        sb->dnode.friction = (f32) (sb->child.parms.ifr.value * sb->dnode.friction);
        sb->edgeGrip = (f32) (sb->child.parms.ied.value * sb->edgeGrip);
    goto Lend;
L62: /* surface 'b' */
        sb->dnode.friction = (f32) (sb->child.parms.dfr.value * sb->dnode.friction);
        sb->edgeGrip = (f32) (sb->child.parms.ded.value * sb->edgeGrip);
Lend:
    temp_v0_3 = sb->slowdown;
    sb->edgeGrip = (f32) (sb->parms.edf.value * sb->edgeGrip);
    sb->dnode.friction = (f32) (sb->parms.bdf.value * sb->dnode.friction);
    if (temp_v0_3 != 0) {
        sb->edgeGrip = 0.0f;
        sb->dnode.friction = (f32) (sb->dnode.friction + ((f32) temp_v0_3 / 100.0f));
        temp_f0_2 = 1.0f - sb->parms.sfr.value;
        sb->dnode.vel[0] = (f32) (sb->dnode.vel[0] * temp_f0_2);
        sb->dnode.vel[2] = (f32) (sb->dnode.vel[2] * temp_f0_2);
    }
    func_00000000(sb);                  /* SnowBoarder_limitSpeed */
    func_00000000(sb);                  /* SnowBoarder_updateOrient */
    func_00000000(D_BF8C_8A40 + 0x20);  /* perf section start "SnowBCol" */
    func_00000000(sb);                  /* SnowBoarder_procColl */
    /* anchor-split (E68/CCE0 lever): same absolute addr 0x8A6C as
     * &stream_readF32_00008A40+0x2C but distinct in-TU anchor kills the base
     * CSE-collapse (frame bloat cascade); reloc-name diff at this site
     * is the accepted fuzzy residual. */
    func_00000000(D_BF8C_8A40 + 0x2C);  /* perf section end */
    func_00000000(sb);                  /* DNode_applyAccums_00065448 */
    if ((sb->slowdown == 0x64) && (sb->dnode.speed < 30.0f)) {
        sb->dnode.vel[0] = 0.0f;
        sb->dnode.vel[2] = 0.0f;
    }
    func_00000000(sb);                  /* DNode_integrate_00064DEC */
    func_00000000(sb);                  /* DNode_resetAccums_00065148 */
}
#else
INCLUDE_ASM("asm/nonmatchings/bootup_uso", SnowBoarder_updateDyn);
#endif

// SnowBoarder_procColl — STRUCTURAL PASS (0x898 / 550 insns, no episode).
// Per-frame entity physics/state update over a large state struct
// (st = a0 = s0; ~152 field accesses, 16 dispatcher sub-calls).
//
//   void SnowBoarder_procColl(State *st) {
//     if (st->0xA80) st->0xA80--;              // cooldown/timer decrement
//     Vec3 p = { st->0xB4, st->0xB8, st->0xBC };   // -> sp+0xB4 local
//     st->0x890 = p.x;  st->0x894 = p.y;       // snapshot prev position
//     // const 0x457A0000 = 4000.0f (a range/limit); 1.0f baseline;
//     // a1 = &st->0xDC (a sub-record), a3 = sp+0xCC scratch.
//     // main body: physics/animation integration driven by folded
//     //   constant tables —
//     //     SndHandle_triggerEvent3_00000940 + {0x4,0x8,0xC,0x10}  (4-word table),
//     //     stream_readVec3_00008A7C + {0x14,0x18,0x28,0x34} (a folded struct),
//     //     stream_readF32_00008A40 + 0x38, SndHandle_triggerEvent0_000008F4 + {0x2C,0x30,0x34}
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
//   strided table at SndHandle_triggerEvent3_00000940 +0x4/0x8/0xC/0x10; see
//   docs/N64_FORENSICS.md#bootup-uso-fp-literal-pool-folded-into-func-0000098C):
//     SndHandle_triggerEvent3_00000940 (4-word table), stream_readVec3_00008A7C (folded struct),
//     stream_readF32_00008A40+0x38, SndHandle_triggerEvent0_000008F4+{0x2C,0x30,0x34} (f-pool).
// Caps (DEFERRED): 550-insn folded-table-driven physics integrator —
//   byte-match blocked by deferred pool symbolization. Real-C
//   STRUCTURAL body below — per-frame entity physics/state update
//   skeleton. Name pre-checked: no extern reuse.
/* SnowBoarder_procColl: EXACT via the IDO 7.1 NCC donor src/bootup_uso/bootup_uso_ncc_C234.cpp, REPLACE_FUNC_BODY-spliced
 * over the placeholder below on both build paths (agent-g run 236). The superseded in-unit decode is kept
 * for reference under BOOTUP_C234_OLD_DECODE (never defined). */
void SnowBoarder_procColl(char *arg0) {
    volatile char *r = arg0;
}

void SnowBoarder_toggleNudge(char *a0) {
    SNOWB(a0)->health = 0xF4240;
    SNOWB(a0)->flags = (int)SNOWB(a0)->flags ^ 0x100;
}

// SnowBoarder_reset — STRUCTURAL PASS (0x1F8 / 126 insns, no episode).
// Per-round / per-attempt state reset & re-arm on a large state struct
// (st = a0 = s0).
//
//   void SnowBoarder_reset(State *st) {
//     func_00000000(?, 1, tableA[st->0x8C4]);            // indexed trigger
//     int j = (s16)*(s16*)((char*)st + st->0x904*2 + 0x8C6);
//     func_00000000(st+0x3EC, 1, tableB[j]);             // 2nd idx trigger
//     if (st->0xA58 & 0x80) func_00000000(st, 0);        // cond reset
//     func_00000000(st+0x90C, 0x17C);
//     st->0x9D0 = F32(SndHandle_triggerEvent3_00000940+0x18);               // folded const
//     st->0xA78 = st->0x768;
//     switch (*(int*)(stream_readS32_00000008+0x2C)) {              // mode/difficulty
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
//     if (!((st->0xA58 << 14) >= 0)) func_00000000(&stream_readVec3_00008A7C+0x40,
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
//   Folded refs (literal-pool/placeholder fold family — SndHandle_triggerEvent3_00000940
//   +0x18 corroborates the contiguous strided table from SnowBoarder_updateOrient;
//   stream_readS32_00000008+0x2C a mode global; stream_readVec3_00008A7C+0x40 a folded
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
void SnowBoarder_reset(char *st) {
    volatile char *r = st;
}

// SnowBoarder_createColliders_0000CCE0 — STRUCTURAL PASS (0x2C0 / 176 insns, no episode).
// HUD/menu layout builder: creates ~12 positioned widget elements into
// the state's UI container, gated by a flag, then finalizes them.
//
//   void SnowBoarder_createColliders_0000CCE0(State *st, a1) {       // st -> s0; cont = st+0x39C
//     // repeated unit (x ~12), per element:
//     //   e = func_00000000(0, st, TYPE, X, P_at_sp10, Y_at_sp14);
//     //   func_00000000(cont, e);             // attach to st->0x39C
//     //   save e -> sp slot;
//     // batch 1 TYPE/coords: {2:(140,?)}, {8:(-140)}, {1:(350)},
//     //   {4:(-350)} with P = F32(SndHandle_triggerEvent3_00000940 + {0x1C,0x20,0x24,0x28});
//     // then func_00000000(st, savedE) x4  (configure pass);
//     if (st->0xA58 & 0x200) return;           // skip optional batch
//     // batch 2: TYPE {0,0x20,0x80,0x10,0x40} at screen coords
//     //   140/-140/350/-350/250/-250/-150/-400/400 (0x430C0000 etc.)
//     //   with P = F32(SndHandle_triggerEvent3_00000940 + 0x2C); attach + configure each.
//   }
//
// Struct-typing reference:
//   st(a0=s0): 0x39C = UI container/parent (s1, receives each element);
//     0xA58 status flags (bit 0x200 = suppress optional HUD batch).
//   func_00000000(0, st, type, xF32, paramF32, yF32) = widget factory;
//     func_00000000(cont, e) = attach; func_00000000(st, e) = configure.
//   Screen-coord consts: ±140 (0x430C0000), ±350 (0x43AF0000),
//     ±250 (0x437A0000), -150 (0xC3160000), ±400 (0x43C80000).
//   Folded param table: SndHandle_triggerEvent3_00000940 + {0x1C,0x20,0x24,0x28,0x2C}
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
 * (lui at / lwc1; the splat text shows them as SndHandle_triggerEvent3_00000940 + 0x1C..0x2C), and the first element is created
 * inside the attach call (`f(cont, e0 = cce0(...))`) so its spill store
 * schedules after the a0 copy. */
// PARTIAL-FORM corrected NM body for SnowBoarder_createColliders_0000CCE0 (replaces lines 8516..8554)
extern char *func_cce0(int, char *, int, float, float, float);
void SnowBoarder_createColliders_0000CCE0(char *st, int a1) {
    char *cont = st + 0x39C;
    /* Single decl list, slots assigned descending from frame top:
     * g4=80 g3=76 g2=72 g1=68 pad=64 g0=60 e3=56 e2=52 e1=48 e0=44 —
     * reproduces target's 64(sp) gap (odd-parity pad, volatile array
     * survives trimming). */
    char *g4, *g3, *g2, *g1;
    volatile int pad[1];
    char *g0, *e3, *e2, *e1, *e0;
    FCALL(list_append)(cont, e0 = (char *)func_cce0(0, st, 2, 140.0f, D_bu_ro_pool[0x95C / 4], -350.0f));
    e1 = (char *)func_cce0(0, st, 8, -140.0f, D_bu_ro_pool[0x960 / 4], -350.0f);
    FCALL(list_append)(cont, e1);
    e2 = (char *)func_cce0(0, st, 1, 140.0f, D_bu_ro_pool[0x964 / 4], 350.0f);
    FCALL(list_append)(cont, e2);
    e3 = (char *)func_cce0(0, st, 4, -140.0f, D_bu_ro_pool[0x968 / 4], 350.0f);
    FCALL(list_append)(cont, e3);
    FCALL(MNode_addChild)(st, e0);
    FCALL(MNode_addChild)(st, e1);
    FCALL(MNode_addChild)(st, e2);
    FCALL(MNode_addChild)(st, e3);
    if ((int)SNOWB(st)->flags & 0x200) return;
    g0 = (char *)func_cce0(0, st, 0x00, 0.0f, D_bu_ro_pool[0x96C / 4], 0.0f);
    FCALL(list_append)(cont, g0);
    FCALL(MNode_addChild)(st, g0);
    g1 = (char *)func_cce0(0, st, 0x20, 250.0f, -150.0f, -400.0f);
    FCALL(list_append)(cont, g1);
    g2 = (char *)func_cce0(0, st, 0x80, -250.0f, -150.0f, -400.0f);
    FCALL(list_append)(cont, g2);
    g3 = (char *)func_cce0(0, st, 0x10, 250.0f, -150.0f, 400.0f);
    FCALL(list_append)(cont, g3);
    g4 = (char *)func_cce0(0, st, 0x40, -250.0f, -150.0f, 400.0f);
    FCALL(list_append)(cont, g4);
    FCALL(MNode_addChild)(st, g1);
    FCALL(MNode_addChild)(st, g2);
    FCALL(MNode_addChild)(st, g3);
    FCALL(MNode_addChild)(st, g4);
}

// SnowBoarder_display — STRUCTURAL PASS (0x4A0 / 296 insns, 64.76% fuzzy, no episode).
// Per-frame active-object sync + draw-state / color update on a large
// state struct (st = a0 = s0).
//
// 2026-06-22 (agent-d): refined to byte-faithful structure (build 292/296
// insns, body logic matches target instruction-for-instruction). Fixed:
//   - reloc form: &snd_setPlayer0VolumeLevel_0000023C+0x18 / &stream_readVec3_00000080+0x64 / &SndHandle_triggerEvent3_00000940
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
//   *(snd_setPlayer0VolumeLevel_0000023C+0x18) is read 3x; IDO -O2 CSEs the guard+call-arg reads
//   (no intervening barrier) while the target reloads each — no C knob forces
//   the reload. Net: correct logic, divergent regalloc — permuter/cc territory.
//
//   void SnowBoarder_display(State *st) {
//     int *ctx = *(int**)(snd_setPlayer0VolumeLevel_0000023C+0x18);          // global ctx ptr
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
//       set sp Vec3 = {1,1,1, F32(SndHandle_triggerEvent3_00000940+0x30)};
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
//       *(int*)(stream_readVec3_00000080+0x64) = 0;  goto tail;
//     }
//     if ((st->0x8B8 & 0xA) && st->0xA4C && !(st->9A8 & 0x400)) {
//       Vec3 d = func_00000000(st->0x850, &sp44);
//       float m = d.x*st->0x9D8 + d.y*st->0x9DC + d.z*st->0x9E0;
//       if ((double)m < DBL(SndHandle_triggerEvent3_00000940+0x38)) {        // folded f64
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
//     *(int*)(stream_readVec3_00000080+0x64) = 0;
//   tail:
//     func_00000000((st->0x8B8 & 1) ? 0x11 : 0x14);
//     func_00000000(0, sp30); func_00000000(st);
//     func_00000000((*(int**)(snd_setPlayer0VolumeLevel_0000023C+0x18))->0x108);
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
//   Folded refs: snd_setPlayer0VolumeLevel_0000023C+0x18 (global ctx ptr — writable fold);
//   SndHandle_triggerEvent3_00000940 +0x30 (f32) / +0x38 (f64) — extends the contiguous
//   strided fold table to +0x4..+0x38; stream_readVec3_00000080+0x64 (writable
//   global flag, cleared 2x); see
//   docs/N64_FORENSICS.md#bootup-uso-fp-literal-pool-folded-into-func-0000098C.
//   (Pool symbolization is RESOLVED: refs use real reloc symbols, not
//   placeholder offsets — see the cap note above for the true blocker.)
//   Real-C STRUCTURAL body below. Name pre-checked: no extern reuse.
/* EXACT via an IDO 7.1 NCC (C++) donor: src/bootup_uso/bootup_uso_ncc_CFA0.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder (agent-c run 116): real
 * virtual calls, V3 copies, ctx first defined as the call argument. */
void SnowBoarder_display(char *st) {
    volatile char *r = st;
}

// SnowBoarder_update — STRUCTURAL PASS (0x4C0 / 304 insns, no episode).
// Per-frame UI/scene event-flag processor: walks status bits on the
// state struct, toggles a latch, fires descriptor-table actions and
// vtable sub-updates accordingly.
//
//   void SnowBoarder_update(State *st) {            // st -> s0
//     func_00000000(&stream_readVec3_00008A7C+0x54);      // named action (table)
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
//       func_00000000(&stream_readVec3_00008A7C+0x5C);
//       dispatch st->0x804->0x28 (->0x24 fn, ->0x20 base);
//       func_00000000(&stream_readVec3_00008A7C+0x64);
//       fl = st->0xA58;
//     }
//     if (fl & 0x80) { func_00000000(st+0x808 ...); ... }
//     // ... further fl-bit blocks (more stream_readVec3_00008A7C/stream_readQuad_00008AEC
//     //   descriptor actions + stream_readQuad_000000F0+0x48 + vtable dispatches);
//     func_00000000(&stream_readQuad_00008AEC+0x44);      // final action
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
//     stream_readVec3_00008A7C + {0x54,0x5C,0x64,0x6C} (8-stride),
//     stream_readQuad_00008AEC + {0x8,0x14,0x20,0x2C,0x38,0x44} (0xC-stride),
//     stream_readQuad_000000F0 + 0x48 (x3); see
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
extern float kRo_SnowBoarder_update_980;
/* Distinct folded-table action-record symbols (= stream_readVec3_00008A7C/AEC + N). Naming
 * each one distinctly defeats IDO's CSE of the shared %hi(stream_readVec3_00008A7C) base
 * into a spilled saved-reg — forces per-site lui+addiu rematerialization to
 * match the target's 21 luis (vs hoisted 12 + stack reloads). */
extern char D_00008AD0, D_00008AD8, D_00008AE0, D_00008AE8;
extern char D_00008AF4, D_00008B00, D_00008B0C, D_00008B18, D_00008B24, D_00008B30;
/* 3 distinct aliases of stream_readQuad_000000F0 so the repeated *(int*)(&F0+0x48) load is
 * rematerialized per call (target: 3x lui %hi + lw 0x48) not CSE'd to a spill. */
extern char D_F0a, D_F0b, D_F0c;
#endif
// SnowBoarder_update — STRUCTURAL PASS (0x4C0 / 304 insns, no episode).
// Per-frame UI/scene event-flag processor: walks status bits on the
// state struct, toggles a latch, fires descriptor-table actions and
// vtable sub-updates accordingly.
//
//   void SnowBoarder_update(State *st) {            // st -> s0
//     func_00000000(&stream_readVec3_00008A7C+0x54);      // named action (table)
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
//       func_00000000(&stream_readVec3_00008A7C+0x5C);
//       dispatch st->0x804->0x28 (->0x24 fn, ->0x20 base);
//       func_00000000(&stream_readVec3_00008A7C+0x64);
//       fl = st->0xA58;
//     }
//     if (fl & 0x80) { func_00000000(st+0x808 ...); ... }
//     // ... further fl-bit blocks (more stream_readVec3_00008A7C/stream_readQuad_00008AEC
//     //   descriptor actions + stream_readQuad_000000F0+0x48 + vtable dispatches);
//     func_00000000(&stream_readQuad_00008AEC+0x44);      // final action
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
//     stream_readVec3_00008A7C + {0x54,0x5C,0x64,0x6C} (8-stride),
//     stream_readQuad_00008AEC + {0x8,0x14,0x20,0x2C,0x38,0x44} (0xC-stride),
//     stream_readQuad_000000F0 + 0x48 (x3); see
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
void SnowBoarder_update(char *st) {
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
    fl = (int)SNOWB(st)->flags; if (1) {}
    v0 = fl & 0x100;
    /* volatile on the per-arm 0x18 reads: target loads 0x18(v0) SEPARATELY in
     * each branch arm (one lands in the beql delay); plain derefs get CSE'd
     * into one hoisted load + two andis. */
    if (v0 != 0) {
        if (SNOWB(st)->input->pressed & 0x20) {
            SNOWB(st)->unkA28 ^= 1;
        }
        if (SNOWB(st)->unkA28 != 0) {
            if (((volatile RiderInput *)SNOWB(st)->input)->pressed & 0x10) {
                /* per-arm v0 recompute (duplicated statement, NOT a shared
                 * merge label): target's andi lands in this path's b delay. */
                fl = (int)SNOWB(st)->flags;
                v0 = fl & 0x100;
                goto block_9;
            }
        } else {
            fl = (int)SNOWB(st)->flags;
            t5 = fl ^ 4;
            if (((volatile RiderInput *)SNOWB(st)->input)->pressed & 0x10) {
                SNOWB(st)->flags = t5;
                fl = t5;
            }
            v0 = fl & 0x100;
            goto block_9;
        }
    } else {
    block_9:
        if (v0 != 0) {
            sub = (char *)SNOWB(st)->input;
            /* volatile busts the duplicate-load CSE: target lw's 0x10(a0) TWICE
             * (one per bit test), plain int derefs merge to one lw. */
            if ((!!(((volatile RiderInput *)sub)->held & 0x80)) & (!!(((volatile RiderInput *)sub)->held & 0x40))) {
                sp58 = 9;
                vt = (char *)NODE(st)->vptr;
                ((void (*)(char *, int *))VT_SLOT(vt, 4).fn)((char *)(VT_SLOT(vt, 4).delta + (int)st), &sp58);
                fl = (int)SNOWB(st)->flags;
            }
        }
        if (fl & 4) {
            *(int *)((char *)SNOWB(st)->input + 0x3C) = 1;
            func_00000000(&D_00008AD8);
            sub = (char *)SNOWB(st)->ai;
            vt = (char *)NODE(sub)->vptr;
            ((void (*)(char *))VT_SLOT(vt, 3).fn)((char *)(sub + VT_SLOT(vt, 3).delta));
            func_00000000(&D_00008AE0);
            fl = (int)SNOWB(st)->flags;
        }
        a0_2 = st + 0x808;
        if (fl & 0x80) {
            SNOWB(st)->bumpSide = -SNOWB(st)->bumpSide;
        }
        func_00000000(a0_2, SNOWB(st)->contactSurface);
        m = (char *)SNOWB(st)->anim;
        SNOWB(st)->contactSurface = 0;
        vt = (char *)NODE(m)->vptr;
        ((void (*)(char *))VT_SLOT(vt, 3).fn)((char *)(m + VT_SLOT(vt, 3).delta));
        *(short *)(st + 0xA10) = 0;
        SNOWB(st)->bumpSide = 0.0f;
        SNOWB(st)->bumpUp = 0.0f;
        SNOWB(st)->bumpFwd = 0.0f;
        if ((int)SNOWB(st)->flags & 0x800) {
            func_00000000(&D_00008AE8);
            func_00000000(st);
            func_00000000(&D_00008AF4);
            if ((int)SNOWB(st)->flags & 0x40000) {
                f0 = SNOWB(st)->dnode.vel[1];
                f2 = (f0 < 0.0f) ? -f0 : f0;
                a84 = (int)((f2 - 100.0f) / four);
                SNOWB(st)->landImpact = a84;
                if (a84 < 0) {
                    SNOWB(st)->landImpact = 0;
                }
                func_00000000(a0_2, SNOWB(st)->landImpact);
                if (SNOWB(st)->unk8B8 & 1) {
                    *(int *)(SNOWB(st)->unk81C + 0x94) = SNOWB(st)->landImpact;
                }
                if (SNOWB(st)->unk8B8 & 4) {
                    *(int *)(SNOWB(st)->unk818 + 0x94) = SNOWB(st)->landImpact;
                }
            }
        }
        func_00000000(&D_00008B00);
        SNOWB(st)->fwdSpeed = SNOWB(st)->dnode.axes[2][0] * SNOWB(st)->dnode.vel[0]
                               + SNOWB(st)->dnode.axes[2][1] * SNOWB(st)->dnode.vel[1]
                               + SNOWB(st)->dnode.axes[2][2] * SNOWB(st)->dnode.vel[2];
        func_00000000((char *)SNOWB(st)->anim);
        if (SNOWB(st)->shake != 0) {
            /* six wobble records, each via ONE mutated pointer temp (r4):
             * lw v0; lwc1 0x64(v0); addiu v0,v0,0x64; add; swc1 0(v0) —
             * the named-base `+= at offset` form instead emits paired
             * lwc1/swc1 0x64(base) with no addiu (wrong shape). */
            char *r4;
            float fv;
            double rnd = (double)func_d440_frnd((void *)(((((int)SNOWB(st)->dnode.cvqs.trans[0] ^ (int)SNOWB(st)->dnode.cvqs.trans[2]) >> 5) ^ 0x12345678)));
            float tf2 = (float)((rnd - 0.5) * 30.0 * (double)((float)SNOWB(st)->shake / 100.0f));
            double tf12;
            r4 = SNOWB(st)->bodyNodes[1];
            fv = *(float *)(r4 + 0x64);
            r4 += 0x64;
            *(float *)r4 = fv + tf2;
            tf12 = (double)tf2 * 0.5;
            r4 = SNOWB(st)->bodyNodes[2];
            fv = *(float *)(r4 + 0x64);
            r4 += 0x64;
            *(float *)r4 = fv + tf2;
            r4 = SNOWB(st)->bodyNodes[4];
            fv = *(float *)(r4 + 0x64);
            r4 += 0x64;
            *(float *)r4 = fv + tf2;
            r4 = SNOWB(st)->bodyNodes[5];
            fv = *(float *)(r4 + 0x64);
            r4 += 0x64;
            *(float *)r4 = (float)((double)fv + tf12);
            r4 = SNOWB(st)->bodyNodes[3];
            fv = *(float *)(r4 + 0x64);
            r4 += 0x64;
            *(float *)r4 = (float)((double)fv + tf12);
            r4 = SNOWB(st)->bodyNodes[6];
            fv = *(float *)(r4 + 0x64);
            r4 += 0x64;
            *(float *)r4 = (float)((double)fv - tf12);
        }
        func_00000000(&D_00008B0C);
        func_00000000(&D_00008B18);
        func_00000000(st);
        func_00000000(&D_00008B24);
        func_d440_f0call(*(int *)((char *)&D_F0a + 0x48), (int)st, SNOWB(st)->dnode.speed / two);
        func_00000000(*(int *)((char *)&D_F0b + 0x48), (int)st, *(int *)(st + 0xDC), *(int *)(st + 0xE4));
        func_00000000(*(int *)((char *)&D_F0c + 0x48), (int)st, SNOWB(st)->health);
        t5 = (int)SNOWB(st)->camera;
        if (t5 != 0) {
            func_d440_final(a0_2, st + 0xDC, SNOWB(st)->dnode.speed,
                          -SNOWB(st)->contactDepth, SNOWB(st)->edge,
                          SNOWB(st)->groundDist + kRo_SnowBoarder_update_980,
                          (int)SNOWB(st)->surface, t5, (int)SNOWB(st)->flags);
        }
        func_00000000(&D_00008B30);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/bootup_uso", SnowBoarder_update);
#endif

// SnowBoarder_handleMsg — STRUCTURAL PASS (0x4C4 / 305 insns, no episode).
// Event/message dispatcher: switch on the incoming message opcode and
// run the matching handler (FP threshold checks + table-driven actions
// + vtable sub-updates) on the state struct.
//
//   void SnowBoarder_handleMsg(State *st, Msg *m) {     // st -> a3
//     switch (m->0x0) {                          // opcode
//       case 0x14:                               // .L0000D98C
//         if (st->0xA80) break;                  // busy
//         o = m->0x4;
//         if ((double)o->0xC < DBL(kRo_SnowBoarder_handleMsg_988)   // properly-symbolized
//             && o->0x28->0xC(u16) == 0x74)      //   double constant
//           func_00000000(st+0x808);
//         break;
//       case 0x9:   ... case 0x6: ...            // .L0000DBB8 / .L0000DBC8
//       case 0x3E8: case 0x3E9: case 0x3EA: case 0x3EB:
//       case 0x3EC: case 0x3ED: case 0x3EE: case 0x3EF:
//         // numbered sub-actions: set a small index (t-reg 1..5),
//         //   run SnowBCollider_ctor / stream_readQuad_00008AEC descriptor actions,
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
//     "SPURIOUS code symbol SndHandle_triggerEvent1_0000098C swallows the pool" claim here is
//     WRONG. SndHandle_triggerEvent1_0000098C is a GENUINE function (0x99C carries a live
//     R_MIPS_26 jal reloc); kRo_SnowBoarder_handleMsg_988 is just SndHandle_triggerEvent3_00000940's tail nop. The
//     FP loads reference symIdx=1 = the RoData SECTION base (immediate = pool
//     offset), NOT SndHandle_triggerEvent1_0000098C. The real rodata constants: this fn's
//     `SndHandle_triggerEvent1_0000098C + 0x4` (@0x988) = f64 0.1, `+ 0xC`-form (@0x990) = f64 0.2
//     (rodata data @ ROM 0xD9FE44). `SndHandle_triggerEvent1_0000098C + N` is just the correct
//     placeholder-byte reproduction, NOT a pool bug to re-extract.
//     stream_readQuad_00008AEC/SnowBCollider_ctor refs are the same matchable addend-reloc
//     class. See docs/N64_FORENSICS.md#bootup-uso-fp-literal-pool-folded-into-func-0000098C.
// Caps (DEFERRED): 305-insn opcode dispatcher — byte-match blocked by full
//   FP-body reconstruction + FP coloring + the file-split needed to fold the
//   pool load (SndHandle_triggerEvent1_0000098C is defined in this same .c), NOT by any pool
//   symbolization. Real-C STRUCTURAL body below. Name pre-checked: no extern
//   reuse. kRo_SnowBoarder_handleMsg_988 reuses pre-symbolized double const at file scope.
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
 * where m2c added a1 = self; the 0x8B38.. literals are stream_readQuad_00008AEC /
 * SnowBCollider_ctor + K addresses (data aliases, lui/addiu %hi/%lo(sym+K)); the
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
extern f64 kRo_SnowBoarder_handleMsg_988, bu_d900_k990;  /* rodata f64 0.1 (pinned 0x988) / 0.2 (0x990; target spells it SndHandle_triggerEvent1_0000098C + 4) */
extern char bu_d900_aec[], bu_d900_b44[];  /* data aliases of stream_readQuad_00008AEC / SnowBCollider_ctor (target lui/addiu %hi/%lo(sym+K)) */
typedef struct { s32 w; } S4D900;  /* 4-byte struct passed by value (target sw a1,4(sp) arg-area copy) */
void SnowBoarder_handleMsg(char *arg0, char *arg1) {
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
            if (((f64) temp_f2 < kRo_SnowBoarder_handleMsg_988) && (*(u16 *)((char *)(*(s32 *)((char *)(temp_v0_2) + 0x28)) + 0xC) == 0x74)) {
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
INCLUDE_ASM("asm/nonmatchings/bootup_uso", SnowBoarder_handleMsg);
#endif

void SnowBoarder_stub_0000DDC4(int a0) {
}

/* SnowBoarder_setDefaultDdms - verified structural decode (0x138, 78 insns).
 * NEAR-SIBLING of SnowBoarder_setReflectionDdms / SnowBoarder_setRiderDdms (same Vec3
 * fetch-and-store fan-out + s0 struct); variant: a leading state
 * switch on s0->0x8C4.
 * Struct-typing reference: same layout as SnowBoarder_setReflectionDdms/E014 -
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
void SnowBoarder_setDefaultDdms(SnowBoarder *sb) {
    unsigned int st = sb->character;
    if (st == 7 || st == 6) {
        FCALL(SnowBoarder_setRiderDdms)(sb);
        return;
    }
    if (sb->flags & SNOWB_FLAG_MIRROR) {
        *sb->unk8F4[0] = *(SnowBRec3 *)FCALL(gfx_getDdm)(&gGfxLib, 0xE);
        *sb->unk8F4[1] = *(SnowBRec3 *)FCALL(gfx_getDdm)(&gGfxLib, 0xF);
    } else {
        *sb->unk8F4[0] = *(SnowBRec3 *)FCALL(gfx_getDdm)(&gGfxLib, 0xA);
        *sb->unk8F4[1] = *(SnowBRec3 *)FCALL(gfx_getDdm)(&gGfxLib, 0xB);
    }
    *sb->unk8F4[2] = *(SnowBRec3 *)FCALL(gfx_getDdm)(&gGfxLib, 0xC);
}

/* SnowBoarder_setReflectionDdms - verified structural decode (0x110, 68 insns,
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
/* SnowBoarder_setReflectionDdms — EXACT 68/68 (Tri3i struct-assign cascade, same form as promoted E014/DDCC) */
void SnowBoarder_setReflectionDdms(SnowBoarder *sb) {
    if (sb->flags & SNOWB_FLAG_MIRROR) {
        *sb->unk8F4[0] = *(SnowBRec3 *)FCALL(gfx_getDdm)(&gGfxLib, 0x11);
        *sb->unk8F4[1] = *(SnowBRec3 *)FCALL(gfx_getDdm)(&gGfxLib, 0x12);
    } else {
        *sb->unk8F4[0] = *(SnowBRec3 *)FCALL(gfx_getDdm)(&gGfxLib, 0x14);
        *sb->unk8F4[1] = *(SnowBRec3 *)FCALL(gfx_getDdm)(&gGfxLib, 0x15);
    }
    *sb->unk8F4[2] = *(SnowBRec3 *)FCALL(gfx_getDdm)(&gGfxLib, 0x16);
}
/* VERIFIED in-tree 68/68 EXACT on clean rebuild (was 97.8%).
   Single lever: Tri3i struct-assign cascade (same form as promoted E014/DDCC siblings).
   First variant hit. Ready to promote (INCLUDE_ASM -> body) + land. */

/* SnowBoarder_setRiderDdms - verified structural decode (0x110, 68 insns).
 * NEAR-SIBLING of SnowBoarder_setReflectionDdms (same Vec3 fetch-and-store fan-out,
 * same s0 struct); variant: the first id of each pair comes from a
 * signed-half field on s0 instead of a constant.
 * Struct-typing reference: same layout as SnowBoarder_setReflectionDdms -
 * s0->0xA58 (2648) u32 flags bit 7 (0x80) selector; s0->0x8F4
 * (2292) / 0x8F8 (2296) / 0x8FC (2300) Vec3i dest pointers (3x int
 * @ +0/+4/+8). NEW fields here: s0->0x902 (2306) and s0->0x900
 * (2304) = s16 dynamic source-id selectors (the first fetch id is
 * read from the object, not hardcoded), with constant ids 8 / 5 /
 * 6 for the second-slot and final fetches.
 * Caps <80: 3-5 reloc calls + &D reloc + flag branch + lh
 * dynamic-id + per-slot 3-word struct copies. INCLUDE_ASM remains
 * build path. */
void SnowBoarder_setRiderDdms(SnowBoarder *sb) {
    if (sb->flags & SNOWB_FLAG_MIRROR) {
        *sb->unk8F4[0] = *(SnowBRec3 *)FCALL(gfx_getDdm)(&gGfxLib, sb->unk902);
        *sb->unk8F4[1] = *(SnowBRec3 *)FCALL(gfx_getDdm)(&gGfxLib, 8);
    } else {
        *sb->unk8F4[0] = *(SnowBRec3 *)FCALL(gfx_getDdm)(&gGfxLib, sb->unk900);
        *sb->unk8F4[1] = *(SnowBRec3 *)FCALL(gfx_getDdm)(&gGfxLib, 5);
    }
    *sb->unk8F4[2] = *(SnowBRec3 *)FCALL(gfx_getDdm)(&gGfxLib, 6);
}

/* SnowBoarder_mirrorFlip_0000E124 - verified structural decode (0x14C, 83 insns,
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
 * (SnowBoarder_setDefaultDdms / SnowBoarder_setRiderDdms, func_072E58 x2, func_072F1C, func_072FB0
 * which takes (ptr, ptr, float) so 180.0f goes out in a2); the flag word is
 * a plain ^= RMW re-read per test; the r-call sits inside the null check and
 * one pointer variable p carries the negated component and both call args;
 * up[] sits at the frame top with five unused words above r[]. */
extern void func_00000000_e124_r(char *, float *, float);
void SnowBoarder_mirrorFlip_0000E124(SnowBoarder *sb) {
    float up[3];
    int unused[5];
    float r[3];
    float *p;

    sb->flags ^= SNOWB_FLAG_MIRROR;
    sb->fwdSpeed = 0.0f;
    if (sb->flags & SNOWB_FLAG_08) {
        FCALL(SnowBoarder_setDefaultDdms)();
    } else {
        FCALL(SnowBoarder_setRiderDdms)(sb);
    }
    sb->edgeSlope = -sb->edgeSlope;
    up[0] = 0.0f;
    up[1] = 1.0f;
    up[2] = 0.0f;
    if (sb->flags & SNOWB_FLAG_MIRROR) {
        p = (float *)(sb->top + 0x30);
        *p = *p * -1.0f;
        if (sb->boardNode != 0) {
            p = (float *)(sb->boardNode + 0x30);
            FCALL(math_mtx4Identity)(p);
            p = (float *)(sb->boardNode + 0x30);
            r[0] = -1.0f;
            r[2] = 1.0f;
            r[1] = 1.0f;
            FCALL(math_mtxScaleRows)(p, r);
        }
    } else {
        p = (float *)(sb->top + 0x30);
        *p = *p * -1.0f;
        if (sb->boardNode != 0) { p = (float *)(sb->boardNode + 0x30); FCALL(math_mtx4Identity)(p); }
    }
    if (sb->flags & SNOWB_FLAG_20) {
        if (sb->boardNode != 0) XCALL(func_00000000_e124_r, math_mtxRotateAxis)(sb->boardNode + 0x30, up, 180.0f);
    }
}

/* SnowBoarder_yawPiOver_0000E270: ratio wrapper. EXACT via the IDO 7.1 -O2 donor
 * bootup_uso_o2_E270.c (REPLACE_FUNC_BODY): the constant is the f32 literal pi
 * at bootup.uso RoData +0x998 (SnowBoarder_yawPiOver_0000E270_rodata pin). The in-unit body is
 * the same C (keeps the .text size); the splice swaps in the donor body so the
 * .rodata reloc resolves to the RoData pool. Old "spill-slot pair swap" residue
 * was the named `ratio` local (one frame word too many). */
extern void aE270(void*, void*, float); /* float-proto alias; jal bakes RAW to 0 */
void SnowBoarder_yawPiOver_0000E270(char *arg0, float arg1) {
    aE270(arg0 + 0xCC, arg0 + 0x3B0, 3.14159265f / arg1);
    aE270(arg0 + 0xF4, arg0 + 0x3B0, 3.14159265f / arg1);
}

/* SnowBCollider_edgeGrip_0000E2D0 - verified structural decode (0x20C, 131 insns,
 * FP vector reflect/project math helper).
 *   void SnowBCollider_edgeGrip_0000E2D0(Obj *a0, int a1, Vec3f *a2, Vec3f *a3) {
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
 *       ... (uses SndHandle_triggerEvent1_0000098C datum) ...
 *   }
 * Struct-typing reference: a2 = input Vec3f (xyz @ +0/+4/+8), a3 =
 * axis/normal Vec3f, a0->0x3BC (956) = the object's stored Vec3f
 * (0x3BC/0x3C0/0x3C4). The per-vector transform is
 * `proj = (v . n); out = proj*n - v` (component-wise) - a plane
 * projection/reflection against axis a3 - applied to BOTH the
 * passed vector and the object's vector, then their squared
 * magnitudes feed a reloc helper (sqrtf / normalize, also reads
 * a SndHandle_triggerEvent1_0000098C+off datum). a1 = an unused-here index/flag.
 * NOTE (2026-07-10, agent-f, CONFIRMED via USO reloc table): the
 * `SndHandle_triggerEvent1_0000098C + 0x14` datum this fn loads (ldc1 @ pool offset 0x9A0) is
 * **f64 0.1**, read from the module's RoData literal pool (data @ ROM
 * 0xD9FE44) — a real rodata constant, NOT code / not a "folded pool" needing
 * re-extract. SndHandle_triggerEvent1_0000098C is genuine code; the `+ N` form just reproduces
 * the ROM placeholder bytes. See the RESOLVED block in
 * docs/N64_FORENSICS.md#bootup-uso-fp-literal-pool-folded-into-func-0000098C.
 * Caps <80: very FP-heavy (dozens of mul.s/add.s/sub.s, two
 * dot-products, two reflections, two |.|^2) + func_00000000 reloc
 * + spilled f-temps + the file-split needed to fold the pool load. Full body
 * INCLUDE_ASM-preserved (.s = source of truth). INCLUDE_ASM (no episode).
 * EXACT (agent-h run 408) via the IDO 7.1 -O2 donor bootup_uso_o2_E2D0.c
 * (REPLACE_FUNC_BODY, SnowBCollider_edgeGrip_0000E2D0_rodata = 0x9A0 for the 0.1 literal). The
 * in-unit body is the same C (keeps the .text size). The post-call FP ring:
 * the two code-free tests after the 9F8 line hand ugen a shared v[]
 * sub-expression that it evaluates into ring temps which as1 deletes as dead,
 * so the FIFO rotates into the target's 1.0 f8 / arg4 f16 / sum f18 order;
 * comparing against lim / dot keeps lim f2 / -lim f12; arg4 is not kept. */
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
 * 0.1 (exact needs a donor, SnowBCollider_edgeGrip_0000E2D0_rodata = 0x9A0 - 0); the frame is 112:
 * one unused word between s and dot and seven below dot (dot's home at sp+68);
 * dot and s are ONE variable (f0 across the call, spilled to its home); the
 * normalise call takes only &v[0] but arg3 stays live in a3 (spilled at 124),
 * reproduced by `if (arg3) {}`; `dot *= k` keeps dot as the first mul operand.
 * Run 153 (agent-g, 99.39 -> 99.54): the dot sum is `z + (y + x)` with the
 * x product written v[0].x * v[1].x (cfe swaps the operands of each product
 * and sum per expression shape; sweep of all 40 order/shape combos). Left: the
 * 1.0f + arg4 colouring (target arg4 f16 / 1.0 ring f8, ours f14 / f6) and the
 * ring offset it causes in the final scale block.
 * NCC compiles of this body are much worse (frame 80, no pads). */
extern f64 bu_e2d0_k9a0;  /* rodata f64 (target SndHandle_triggerEvent1_0000098C + 0x14) */
void SnowBCollider_edgeGrip_0000E2D0(char *arg0, s32 arg1, Vec3 *arg2, Vec3 *arg3, f32 arg4) {
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
    v[0].x = SNOWB(arg0)->dnode.axes[1][0]; /* side axis */
    v[0].y = SNOWB(arg0)->dnode.axes[1][1];
    v[0].z = SNOWB(arg0)->dnode.axes[1][2];
    d = v[0].z * arg3->z + (v[0].y * arg3->y + v[0].x * arg3->x);
    v[0].x = d * arg3->x - v[0].x;
    v[0].y = d * arg3->y - v[0].y;
    v[0].z = d * arg3->z - v[0].z;
    if (0.1 < (f64)XCALL(func_00000000_4948_ff, math_vec3Length_0005BBCC)(v[0].z * v[0].z + (v[0].y * v[0].y + v[0].x * v[0].x))) {
        dot = v[1].z * v[0].z + (v[1].y * v[0].y + v[0].x * v[1].x);
        FCALL(math_vec3Normalize)(&v[0]); if (arg3) {}
        lim = SNOWB(arg0)->edgeGrip;
        dot *= SNOWB(arg0)->edgeFriction;
        if ((((v[1].z + v[1].y) + (v[0].z * v[0].x)) - (v[0].y * v[1].x)) != lim) {} if ((((v[1].z + v[1].y) + (v[0].z * v[0].x)) - (v[0].y * v[1].x)) != dot) {}
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
        FCALL(DNode_addImpulseAt_00065250)(arg0, arg1, &v[0]);
    }
}

/* SnowBoarder_readParms - verified structural decode (0xAC, 43 insns).
 * Per-frame update/teardown over a composite object a0 (= s2). Uses
 * the documented obj->0x28 vtable-dispatch idiom (PATTERNS.md
 * #feedback-1080-obj-0x28-vtable-dispatch), 0x4C/0x48 offset variant.
 * Struct-typing reference: a0 composite object; a0+0x540 / a0+0x108
 * sub-regions passed to reloc sub-init/deinit; a0+0x8C8 s16 array of
 * `n` elements (n produced by stream_readS32_000089C0, per-element processed by
 * stream_read16_000089FC); a0->0x840 (2112) child A ptr, a0->0x804 (2052)
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
 * (the `&n` escapes to stream_readS32_000089C0); not C-reachable. Stays NM. */
void SnowBoarder_readParms(char *a0) {
  int n;
  short *p;
  int i;
  int *v;
  FCALL(Parms_load_00035C6C)(a0 + 0x540);
  stream_readS32_000089C0(&n);
  i = 0;
  p = (short *) (a0 + 0x8C8);
  for (; i < n; i++)
  {
    stream_read16_000089FC(p);
    p += 1;
  }

 do { v = *((int **) (*((char **) (a0 + 0x840)) + 0x28)); (*((void (**)(char *)) (((char *) v) + 0x4C)))((char *) (((int) *((char **) (a0 + 0x840))) + ((int) ((short) (*((short *) (((char *) v) + 0x48))))))); } while (0);
  v = *((int **) (*((char **) (a0 + 0x804)) + 0x28));
  (*((void (**)(char *)) (((char *) v) + 0x4C)))((char *) (((int) *((char **) (a0 + 0x804))) + ((int) ((short) (*((short *) (((char *) v) + 0x48)))))));
  FCALL(Parms_load_00035C6C)(a0 + 0x108);
  if (v)
  {
  }
}

void boot_readFloatParm_0000E588(char *a0) {
    int scratch;
    stream_readS32_000089C0(&scratch);
    stream_readF32_00008A40((int*)(a0 + 0x10));
}

void boot_readVec3Parm_0000E5B8(char *a0) {
    int scratch;
    stream_readS32_000089C0(&scratch);
    stream_readVec3_00008A7C((int*)(a0 + 0x10));
}

float ShpNode_getSuc(char *a0) { return SHPNODE(a0)->dnode.parms.suc.value; } /* the "suc" DNode parm (+0x198) */

void ShpNode_saveData(char *a0) {
    FCALL(Parms_save)(a0 + 0x108);
}

void ShpNode_readParms(char *a0) {
    FCALL(Parms_load_00035C6C)(a0 + 0x108);
}

void boot_readQuadParm_0000E630(char *a0) {
    int scratch;
    stream_readS32_000089C0(&scratch);
    stream_readQuad_00008AEC((int*)(a0 + 0x10));
}

void boot_readIntParm_0000E660(char *a0) {
    int scratch;
    stream_readS32_000089C0(&scratch);
    stream_readS32_000089C0((int*)(a0 + 0x10));
}

/* boot_ItemType_create_0000E690: byte-identical mirror of ColourItem_create_00000C10 / boot_ItemType_create_000046EC /
 * boot_ItemType_create_00007B08 — all byte-exact twins of donor edd_ItemType_create_000003BC.
 * BYTE-EXACT via the donor's clean C form (p2/head/p1 decl order + char*-cast
 * field access + p1[0x40/4] re-read). Was once force-matched via INSN_PATCH
 * (banned 2026-05-23); this is the genuine C match. */
void *boot_ItemType_create_0000E690(int *arg0) {
    int *p2;
    int *head;
    int *p1;
    p1 = (int*)FCALL(mem_new)(0x40);
    if (p1 != 0) {
        FCALL(ColourItem_ctor)(p1);
        NODE(p1)->vptr = (char *)&ColourItem_vtbl_75A0;
        ((ColourSubItem *)p1)->unk3C = 0;
    }
    p2 = p1;
    p1 = arg0;
    head = (int*)p1[0x40 / 4];
    if ((int*)p1[0x40 / 4] != 0) {
        FCALL(list_append)((char*)p2 + 0x10, head);
        if (NODE(head)->parent != 0) {
            NODE(head)->unk04 = 1;
        }
        NODE(head)->parent = (Node *)p2;
    }
    return p2;
}

void SnowBChild_readParms_0000E720(char *a0) {
    FCALL(Parms_load_00035C6C)(a0 + 0x2C);
}

/* ScreenThumbMat_grab_0000E740 - verified structural decode (0xB4, 48 insns,
 * 2D strided->packed halfword blit / unswizzle).
 * Struct-typing reference: global root *(snd_setPlayer0VolumeLevel_0000023C+4); root->0x28
 * (40) vtable ptr with fn @0x64 (100) + s16 base-adjust @0x60 (96)
 * returning a buffer base; src region at base+0x9140 laid out as
 * 0x20-byte cells with 4 u16 lanes at cell offsets 0/8/0x10/0x18;
 * dst = a0->0x1C (28) packed u16 output. Grid 32x(16*4)=2048 u16,
 * row stride +0xA80 in src. The obj-0x28 vtable-dispatch idiom
 * (0x64/0x60 variant). Caps <80: vtable jalr + snd_setPlayer0VolumeLevel_0000023C+4
 * reloc data ref + nested strided copy loop (bne loop pair).
 * INCLUDE_ASM remains build path. */
/* EXACT via an IDO 7.1 NCC (C++) donor: src/bootup_uso/bootup_uso_ncc_E740.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder (agent-c run 116). */
void ScreenThumbMat_grab_0000E740(char *a0, int a1) {
    volatile char *r = a0;
}

/* SnowBoard_ctor - verified structural decode (0xBC, 47 insns,
 * config-parse + init routine).
 * Struct-typing reference: sStr_rom_audio_seq_uso_8D34 / sStr_rom_audio_wave_uso_8D48 / sStr_rom_audio_bank_uso_8D5C =
 * three config-key data (string/descriptor) parsed by the reloc
 * helper into a (x,y,z) triple on the stack, then consumed in
 * (x,z,y) order. stream_readS32_00000008+0x20 = global slot set to &D
 * (registration). Tail uses the engine-wide obj-0x28 vtable-dispatch
 * idiom, 0x5C/0x58 variant (root *(&D) ->0x28 -> {fn@0x5C,
 * s16@0x58}). Caps <80: 7x reloc + 5x &D reloc + stream_readS32_00000008+0x20
 * cross-symbol data store + vtable jalr.
 * INCLUDE_ASM remains build path. */
extern char sStr_rom_audio_seq_uso_8D34, sStr_rom_audio_wave_uso_8D48, sStr_rom_audio_bank_uso_8D5C;
/* EXACT via an IDO 7.1 NCC (C++) donor: src/bootup_uso/bootup_uso_ncc_E800.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder (agent-c run 116). */
void SnowBoard_ctor(void) {
    volatile int r = 0;
}

void stream_readS32_0000E8BC(int *dst) {
    int buf[2];
    FCALL(fs_gdlReadFile)(&gSysLib, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

void stream_readF32_0000E8F8(float *dst) {
    float buf[2];
    FCALL(fs_gdlReadFile)(&gSysLib, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

extern void stream_readVec3_0000E934();
void stream_readVec3_0000E934(Vec3 *dst) {
    int pad_top[1];
    Tri3i raw;
    int pad_mid[2];
    Tri3i tmp;
    int pad_bot[2];
    FCALL(fs_gdlReadFile)(&gSysLib, &raw, 12);
#ifdef PORT
    PORT_BE_SWAP32S(&raw, 12);
#endif
    tmp = raw;
    dst->x = *(float*)&tmp.a;
    dst->y = *(float*)&tmp.b;
    dst->z = *(float*)&tmp.c;
}

void stream_readQuad_0000E9A4(Quad4 *dst) {
    Quad4 buf;
    FCALL(fs_gdlReadFile)(&gSysLib, &buf, 16);
#ifdef PORT
    PORT_BE_SWAP32S(&buf, 16);
#endif
    *dst = buf;
}

/* SnowBoard_staticCtor_0000E9FC: EXACT via an IDO 7.1 NCC (C++) donor,
 * src/bootup_uso/bootup_uso_ncc_E9FC.cpp, REPLACE_FUNC_BODY-spliced over this
 * placeholder (agent-c run 349). It is the edg static initializer (__sti__)
 * for a global object of a class whose inline ctor calls the out-of-line base
 * ctor and then stores the vtable address into the vptr at +0x28 (the ROM
 * word is `sw t6, 0x28(at)` with the object at 0); same end-of-TU line
 * mechanism as boot_staticInit_00006204 (`lui a0` above `sw ra`). */
void SnowBoard_staticCtor_0000E9FC(void) {
    volatile int r = 0;
}

extern char sStr_SnowBoard_8D70;
extern char sStr_rom_version_bin_8D7C;
extern char sStr_rom_anims_bin_8D8C;
extern char sStr_Node_8D9C;
extern char D_00000000_a;
extern char D_00000000_b;
void SnowBoard_InitInstance(char *arg0) {
    char pad[4];
    int saved;

    (void)pad;
    NODE(arg0)->name = (const char *)&sStr_SnowBoard_8D70;
    FCALL(fs_readRecord_00041F90)(arg0, &sStr_rom_version_bin_8D7C);
    saved = FCALL(app_setHeapMode)(0);
    *(int *)&XDATA(D_00000000, port_bootup_bss, 0xD8) = FCALL(AnimDataMgr_ctor)(0, &sStr_rom_anims_bin_8D8C, 0x20, 0xC);
    FCALL(app_setHeapMode)(saved);
    /* Distinct zero-address aliases keep IDO from caching the base in $s0. */
    FCALL(Node_ctor)(&D_00000000_a, &sStr_Node_8D9C);
    FCALL(GameFlow_create)(&D_00000000_b, arg0);
    if (FCALL(sys_getResetType)(&XDATA(D_00000000, port_bootup_bss, 0x3C740)) != 0) {
        FCALL(GameFlow_startMode)(&D_00000000_a, 1, 0);
    } else {
        FCALL(GameFlow_startMode)(&D_00000000_b, 0, 0);
    }
}

extern int gInputMgr_00000190;
void SnowBoard_onPreNmi(int a0) {
    FCALL(Fader_start)(gInputMgr_00000190, 3, 0);
}

void boot_readFloatParm_0000EB28(char *a0) {
    int scratch;
    stream_readS32_0000E8BC(&scratch);
    stream_readF32_0000E8F8((int*)(a0 + 0x10));
}

void boot_readVec3Parm_0000EB58(char *a0) {
    int scratch;
    stream_readS32_0000E8BC(&scratch);
    stream_readVec3_0000E934((int*)(a0 + 0x10));
}

void boot_readQuadParm_0000EB88(char *a0) {
    int scratch;
    stream_readS32_0000E8BC(&scratch);
    stream_readQuad_0000E9A4((int*)(a0 + 0x10));
}

void boot_readIntParm_0000EBB8(char *a0) {
    int scratch;
    stream_readS32_0000E8BC(&scratch);
    stream_readS32_0000E8BC((int*)(a0 + 0x10));
}

/* boot_ItemType_create_0000EBE8: 36-insn alloc-or-fail + linked-list-prepend wrapper.
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
int *boot_ItemType_create_0000EBE8(int *caller_a0) {
    int *ret;
    int *target;
    int *p;
    if ((p = (int*)FCALL(mem_new)(0x40)) != 0) {
        FCALL(ColourItem_ctor)(p);
        NODE(p)->vptr = (char *)&ColourItem_vtbl_75A0;
        p[0x3C/4] = 0;
    }
    ret = p;
    p += 0x10/4;
    if (caller_a0[0x40/4] != 0) {
        target = (int*)caller_a0[0x40/4];
        FCALL(list_append)(p, target);
        if (target[0x14/4] != 0) {
            target[0x4/4] = 1;
        }
        target[0x14/4] = (int)ret;
    }
    return ret;
}

void stream_readS32_0000EC80(int *dst) {
    int buf[2];
    FCALL(fs_gdlReadFile)(&gSysLib, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

void stream_readF32_0000ECBC(float *dst) {
    float buf[2];
    FCALL(fs_gdlReadFile)(&gSysLib, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

extern void stream_readVec3_0000ECF8();
void stream_readVec3_0000ECF8(Vec3 *dst) {
    int pad_top[1];
    Tri3i raw;
    int pad_mid[2];
    Tri3i tmp;
    int pad_bot[2];
    FCALL(fs_gdlReadFile)(&gSysLib, &raw, 12);
#ifdef PORT
    PORT_BE_SWAP32S(&raw, 12);
#endif
    tmp = raw;
    dst->x = *(float*)&tmp.a;
    dst->y = *(float*)&tmp.b;
    dst->z = *(float*)&tmp.c;
}

void stream_readQuad_0000ED68(Quad4 *dst) {
    Quad4 buf;
    FCALL(fs_gdlReadFile)(&gSysLib, &buf, 16);
#ifdef PORT
    PORT_BE_SWAP32S(&buf, 16);
#endif
    *dst = buf;
}

void game_setupEmpty(int a0, int a1) {}

void boot_readFloatParm_0000EDCC(char *a0) {
    int scratch;
    stream_readS32_0000EC80(&scratch);
    stream_readF32_0000ECBC((int*)(a0 + 0x10));
}

void boot_readVec3Parm_0000EDFC(char *a0) {
    int scratch;
    stream_readS32_0000EC80(&scratch);
    stream_readVec3_0000ECF8((int*)(a0 + 0x10));
}

void boot_readQuadParm_0000EE2C(char *a0) {
    int scratch;
    stream_readS32_0000EC80(&scratch);
    stream_readQuad_0000ED68((int*)(a0 + 0x10));
}

void boot_readIntParm_0000EE5C(char *a0) {
    int scratch;
    stream_readS32_0000EC80(&scratch);
    stream_readS32_0000EC80((int*)(a0 + 0x10));
}

/* boot_ItemType_create_0000EE8C: alloc-cascade constructor, sibling of boot_ItemType_create_0000EBE8.
 * BYTE-EXACT + LANDED (2026-06-21). The body was byte-exact once `ret` was
 * declared FIRST (IDO assigns it the highest local slot 0x24, matching target);
 * the only residual was the target's single trailing 8-byte-alignment nop at
 * 0xEF1C (so stream_readS32_0000EF20 sits at 0xEF20). Resolved by appending that one
 * all-zero (nop) word via `SUFFIX_BYTES_FORCE := boot_ItemType_create_0000EE8C=0x00000000` in
 * the Makefile — the SAME legitimate alignment-pad mechanism already used for
 * gui_fontAdjustAdvance / h2hproc fns. The earlier "cannot land" note assumed a
 * GLOBAL_ASM dummy (8-byte -O2 floor → +4 overshoot); SUFFIX_BYTES_FORCE appends
 * exactly one word, so stream_readS32_0000EF20 lands at 0xEF20 and the ROM stays exact.
 * This works mid-segment (with a following symbol), not just at segment-end. */
void *boot_ItemType_create_0000EE8C(void *caller_a0) {
  void *ret;
  void *target;
  register void *p;
  p = (void *) FCALL(mem_new)(0x40);
  if (p != 0)
  {
    FCALL(ColourItem_ctor)(p);
    NODE(p)->vptr = (char *)&ColourItem_vtbl_75A0;
    *((int *) (((char *) p) + 0x3C)) = 0;
  }
  ret = p;
  p = ((char *) p) + 0x10;
  target = *((void **) (((char *) caller_a0) + 0x40));
  if ((*((void **) (((char *) caller_a0) + 0x40))) != 0)
  {
    FCALL(list_append)(p, target);
    if ((*((int *) (((char *) target) + 0x14))) != 0)
    {
      *((int *) (((char *) target) + 0x4)) = 1;
    }
    *((int *) (((char *) target) + 0x14)) = (int) ret;
  }
  return ret;
}

void stream_readS32_0000EF20(int *dst) {
    int buf[2];
    FCALL(fs_gdlReadFile)(&gSysLib, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

void stream_readF32_0000EF5C(float *dst) {
    float buf[2];
    FCALL(fs_gdlReadFile)(&gSysLib, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

extern void stream_readVec3_0000EF98();
void stream_readVec3_0000EF98(Vec3 *dst) {
    int pad_top[1];
    Tri3i raw;
    int pad_mid[2];
    Tri3i tmp;
    int pad_bot[2];
    FCALL(fs_gdlReadFile)(&gSysLib, &raw, 12);
#ifdef PORT
    PORT_BE_SWAP32S(&raw, 12);
#endif
    tmp = raw;
    dst->x = *(float*)&tmp.a;
    dst->y = *(float*)&tmp.b;
    dst->z = *(float*)&tmp.c;
}

void stream_readQuad_0000F008(Quad4 *dst) {
    Quad4 buf;
    FCALL(fs_gdlReadFile)(&gSysLib, &buf, 16);
#ifdef PORT
    PORT_BE_SWAP32S(&buf, 16);
#endif
    *dst = buf;
}

void boot_readFloatParm_0000F060(char *a0) {
    int scratch;
    stream_readS32_0000EF20(&scratch);
    stream_readF32_0000EF5C((int*)(a0 + 0x10));
}

void boot_readVec3Parm_0000F090(char *a0) {
    int scratch;
    stream_readS32_0000EF20(&scratch);
    stream_readVec3_0000EF98((int*)(a0 + 0x10));
}

void boot_readQuadParm_0000F0C0(char *a0) {
    int scratch;
    stream_readS32_0000EF20(&scratch);
    stream_readQuad_0000F008((int*)(a0 + 0x10));
}

void boot_readIntParm_0000F0F0(char *a0) {
    int scratch;
    stream_readS32_0000EF20(&scratch);
    stream_readS32_0000EF20((int*)(a0 + 0x10));
}

void stream_readS32_0000F120(int *dst) {
    int buf[2];
    FCALL(fs_gdlReadFile)(&gSysLib, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

void stream_readQuad_0000F15C(Quad4 *dst) {
    Quad4 buf;
    FCALL(fs_gdlReadFile)(&gSysLib, &buf, 16);
#ifdef PORT
    PORT_BE_SWAP32S(&buf, 16);
#endif
    *dst = buf;
}

/* boot_readQuadParm_0000F1B4: composite int+Quad4 reader (cross-USO template).
 * Body is 12 insns (0x30); FUNC st_size in expected is 60 (0x3C) because
 * 3 trailing alignment nops at 0xF1E4-0xF1EC are PART of the symbol's reach
 * (next function stream_readS32_0000F1F0 is 16-byte aligned).
 *
 * SUFFIX_BYTES recipe (Makefile) appends 3 nops post-cc, growing the
 * symbol from 0x30 to 0x3C in place. Drops the prior NM wrap.
 * Verified 2026-05-05: build/.o byte-correct via byte_verify (st_size=0x3C
 * matches expected; trailing nops match). Per
 * feedback_suffix_bytes_unblocks_4byte_stolen_prologue.md (multi-insn
 * extension). */
void boot_readQuadParm_0000F1B4(char *a0) {
    int scratch;
    stream_readS32_0000F120(&scratch);
    stream_readQuad_0000F15C((Quad4*)(a0 + 0x10));
}

/* stream_readS32_0000F1F0, F23C, F288, F2EC moved to bootup_uso_o0_F1F0.c (-O0 file
 * split, see Makefile + tenshoe.ld). The 4 -O0 accessor templates promote
 * to byte-correct in the new file.
 * input_ctor_0000F390, SnowBoard_restart, SnowBoard_softReset moved to bootup_uso_o0_F390.c
 * (-O0 file split, see Makefile + tenshoe.ld).
 * Process_start..Process_update moved to bootup_uso_F434.c (layout shim). */
