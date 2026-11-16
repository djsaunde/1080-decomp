/* MIPSpro 7.2.1 C++ (o32 edgcpfe, EDG 2.29, Mar 13 1998) EXACT donor for cam_updateView_00000B3C
 * (0xB3C..0x1644, 706 words), compiled by tools/ncc721.sh (edgcpfe -> IDO 7.1 uopt / ugen / as1, the NCC
 * flags) and spliced over a placeholder in both build trees (agent-h run 355).
 *
 * Why this front end: B3C's `switch (s->0x26C)` has 4 dense labels (0..3) and the target lowers it as a
 * 4-entry jump table (`sltiu at,v0,4; beqz; sll; lui; addu; lw; jr`, table at RoData +0x50). No IDO-era
 * front end does that: cfe tables only from 5 labels and the IDO 7.1 NCC edg (2.19) never emits xjp.
 * The period-correct MIPSpro 7.2 / 7.2.1 edgcpfe (EDG 2.29: table iff n >= 4 and size <= 2n) does, and
 * this body is byte-exact under it (docs/TOOLING_DECOMP.md "Toolchain forensics III", run 352).
 * Source = the cfe NM body (99.9986) with the phantom `case 4:` dropped, `ob[0] = tv` stored through
 * `*(char **)(char *)ob` (edg forwards a plain `ob[0]` / `*(char **)&ob[0]` store; the cast chain, like
 * the old `*(char **)((char *)ob + 0)`, is not forwarded: same bytes, agent-g run 580) and `x == 1`
 * instead of `1 == x` in the first && test.
 *
 * RoData layout: the target's `lw t,0x50(at)` is an in-place %lo (the game_uso Yay0 block is raw .text,
 * no reloc bake), so the table must sit at donor .rodata+0x50. The two layout-only functions below
 * re-create what precedes it in the unit's RoData: game_uso_func_00000000's 1/6 literal (+0x0) and
 * cam_pickResultView_00000940's three tables (+0x4 / +0x20 / +0x38). The splice copies only
 * cam_updateView_00000B3C; its renamed .rodata reloc (cam_updateView_00000B3C_rodata) is pinned to 0 in
 * undefined_syms_auto.txt, so lui 0 / lw 0x50 both come out of the in-place addends. */
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef double f64; typedef short s16; typedef unsigned char u8; typedef unsigned short u16;
/* agent-g run 485: field form over include/game/camera.h CourseCam / CamPath / CamKey / CamList and
 * include/game/snowboarder.h (included by relative path; their asserts are C-only). */
#include "../../include/game/snowboarder.h"
#include "../../include/game/camera.h"
#include "../../include/game/input.h"
extern "C" {
typedef struct { float x, y, z; } Vec3;
typedef struct { int a, b, c; } Tri3i;
typedef struct { int w[16]; } B3CBlk40;

float math_dot4_000000A0(float *a, float *b);
void math_blendVec3x4_000000E0(Vec3 *out, Vec3 **ctrl, float *weights);
void cam_applyRelative_0000052C(int unused, Vec3 *a1, Vec3 *a2, int *a3, Vec3 *p4);
int camPath_hasSpline_00000674(int *a0);
extern char import_80020098;
void cam_setView_00000AEC(int *a0, int a1);
void cam_pickResultView_00000940(char *arg0);
Vec3 *cam_toTargetLocal_000023D4(Vec3 *out, char *a1);
/* 2714's second parameter is unsigned (agent-g run 239, kit CCLVII): the literal-1 arguments are then
 * materialised by `addiu a1,zero,1`, not copied from the `tmp = 1` LR. */
void SnowChaseActorCam_setMode_00002714(int *a0, unsigned int a1, int a2);
void SnowChaseActorLook_setMode_00002FC8(int *a0, int a1, int a2);
/* basis evaluator; prototyped so the float t travels in $a1 (blank jal 0 = the unit's func_00000000) */
void game_uso_func_00000000(f32 *out, f32 t);

/* layout only: game_uso RoData +0x0 (the 1/6 literal) */
void game_uso_func_00000000_lay(float *out, float t) { out[0] = 0.16666667f; }

/* layout only: the cfe-exact cam_pickResultView_00000940 body (identical under this front end); its three
 * tables are RoData +0x4 / +0x20 / +0x38 */
void game_uso_func_00000940_lay(char *arg0) {
    s32 temp_v0;
    s32 var_a1;

    var_a1 = -1;
    switch (((CourseCam *)arg0)->rider->raceResult) {
    case 0:
        temp_v0 = INPUT_MGR(import_80020098).nextFlowMode;
        if ((temp_v0 == 4) || (temp_v0 == 6) || (temp_v0 == 7) || ((temp_v0 == 1) && (INPUT_MGR(import_80020098).procState == 0))) {
            var_a1 = 1;
        } else {
            switch (INPUT_MGR(import_80020098).course) {
            case 2: var_a1 = 6; break;
            case 3: var_a1 = 7; break;
            case 4: var_a1 = 8; break;
            case 5: var_a1 = 7; break;
            case 6: var_a1 = 9; break;
            case 7: var_a1 = 0xA; break;
            default: var_a1 = 0; break;
            }
        }
        break;
    case 1:
        if (INPUT_MGR(import_80020098).winVariant != 0) {
            var_a1 = 0xB;
        } else {
            switch (INPUT_MGR(import_80020098).course) {
            case 2: var_a1 = 0xD; break;
            case 3: var_a1 = 2; break;
            case 4: var_a1 = 0xD; break;
            case 5: var_a1 = 2; break;
            case 6: var_a1 = 2; break;
            case 7: var_a1 = 0xE; break;
            default: var_a1 = 2; break;
            }
        }
        break;
    case 2:
        var_a1 = 0xC;
        break;
    case 5:
        var_a1 = 3;
        break;
    case 6:
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

/* Spline path-follow update (docs/1080_CAMERA.md section 2a): play key `key` of path `path` (= lists[view]
 * ->paths[listPos]), frame `frame` of key->frames. Each channel (eye, look, up, fov, rot) is either the current
 * key's value or a uniform cubic B-spline over keys[key - 1 .. key + 2], weighted by the basis at
 * t = frame / frames (game_uso_func_00000000(state, t)). A path whose eyeMode / lookMode is 1 leaves that body
 * to its chase component; on such a path the pad cycles chaseMode (the 4-label switch). The odd statement shapes
 * (`tmp = 1`, `if (1)`, `tv += ..`, Tri3i copies) are the run-355 match levers. */
void cam_updateView_00000B3C(CourseCam *arg0) {
    Vec3 vD;          /* 0x144 */
    Vec3 vC;          /* 0x138 */
    char *ob[1];      /* 0x134: the look-at node (cam.target), a one-word stack object (memory-resident) */
    CamList *tbl;     /* 0x130: lists[view] */
    CamPath *seg;     /* 0x12C: paths[path] */
    CamKey *kf;       /* 0x128: the current key */
    Vec3 pos2;        /* 0x11C: look-at point */
    Vec3 pos1;        /* 0x110: eye */
    Vec3 up;          /* 0x104 */
    f32 state[4];     /* 0xF4: the four basis weights */
    s32 nsub;         /* 0xF0: next frame */
    s32 ncur;         /* 0xEC: next key */
    s32 nseg;         /* 0xE8: next listPos */
    CourseCam *s;     /* 0xE4 */
    Vec3 zv;          /* 0xD8 */
    char *tv;
    s32 cur;
    s32 node;
    s32 selv;
    s32 st;
    char *bv;         /* 0xC0 */
    Vec3 stage;       /* 0xB4 */
    s32 tmp;
    f32 f1;
    f32 fy;           /* 0xA8 */
    Vec3 *c1[4];      /* 0x98 */
    Vec3 *c2[4];      /* 0x88 */
    Vec3 fresh;       /* 0x7C */
    f32 fz;           /* 0x78 */
    Vec3 *c3[4];      /* 0x68 */
    f32 w[4];         /* 0x58 */
    Vec3 *c4[4];      /* 0x48 */
    Vec3 v3C;         /* 0x3C */
    Vec3 v30;         /* 0x30 */

    s = arg0;
    tv = (char *)s->cam.target; *(char **)(char *)ob = tv;
    /* a new race result picks a result view */
    tmp = s->rider->raceResult;
    if (tmp != s->lastResult) {
        s->lastResult = tmp;
        cam_pickResultView_00000940((char *)s);
    }
    tmp = 1; /* tmp also holds the constant 1 (target `bne t0,v0` operand order) */
    tbl = s->lists[s->view];
    cur = s->key;
    node = tbl->paths[s->listPos];
    s->path = node;
    seg = 0;
    if (1) { seg = s->paths[node]; }
    kf = seg->keys[cur];
    if (tmp == cur) {
        if (s->frame == 0) {
            /* the first frame of a path: rotation source and the chase components' modes */
            if (*(char **)&s->freeRot != 0) {
                s->cam.rotEnable = tmp;
                zv.z = 0.0f;
                zv.y = 0.0f;
                zv.x = 0.0f;
                *(Tri3i *)&stage = *(Tri3i *)&zv;
                s->cam.pitch = stage.x;
                s->cam.yaw = stage.y;
                s->cam.unk13C = stage.z;
            } else {
                s->cam.rotEnable = (seg->rotMode == 0) == 0;
            }
            if (tmp == seg->eyeMode) {
                SnowChaseActorCam_setMode_00002714((int *)s->eyeChase, 1, (int)s->rider->dnode.vel);
            } else {
                SnowChaseActorCam_setMode_00002714((int *)s->eyeChase, 0, (int)s->rider->dnode.vel);
            }
            if (seg->lookMode == 1) {
                SnowChaseActorLook_setMode_00002FC8((int *)((LookAtNode *)ob[0])->chase, 1, (int)s->rider->dnode.vel);
            } else {
                SnowChaseActorLook_setMode_00002FC8((int *)((LookAtNode *)ob[0])->chase, 0, (int)s->rider->dnode.vel);
            }
            s->savedChaseMode = 0;
            s->chaseMode = 0;
            tv = (char *)&s->rider->flags; *(s32 *)tv = *(s32 *)tv & ~0x2000;
        }
    }
    if ((seg->lookMode == 1) && (seg->eyeMode == 1)) {
        /* a chase path: pad held 0x80 = look back (mode 3), pressed 0x40 cycles 0..2 */
        tmp = 0; /* the advance flag shares tmp */
        st = s->chaseMode;
        if (st == 3) {
            tv = (char *)s->pad;
            if (!(((RiderInput *)tv)->held & 0x80)) {
                tmp = 1;
                s->chaseMode = s->savedChaseMode;
            }
        } else {
            tv = (char *)s->pad;
            if (((RiderInput *)tv)->held & 0x80) {
                s->savedChaseMode = st;
                s->chaseMode = 3;
                tmp = 1;
            } else if (((RiderInput *)tv)->pressed & 0x40) {
                tmp = st + 1;
                s->chaseMode = tmp;
                if (tmp >= 3) {
                    s->chaseMode = 0;
                }
                tmp = 1;
            }
        }
        if (tmp != 0) {
            /* 4 dense labels: the native 4-entry table (RoData +0x50). eye / look component modes */
            switch (s->chaseMode) {
            case 0: /* chase at height toy, look at the aim point */
                SnowChaseActorCam_setMode_00002714((int *)s->eyeChase, 1, (int)s->rider->dnode.vel);
                SnowChaseActorLook_setMode_00002FC8((int *)((LookAtNode *)ob[0])->chase, 1, (int)s->rider->dnode.vel);
                tv = (char *)&s->rider->flags; *(s32 *)tv = *(s32 *)tv & ~0x2000;
                break;
            case 1: /* chase at height ty2 */
                SnowChaseActorCam_setMode_00002714((int *)s->eyeChase, 2, (int)s->rider->dnode.vel);
                SnowChaseActorLook_setMode_00002FC8((int *)((LookAtNode *)ob[0])->chase, 1, (int)s->rider->dnode.vel);
                tv = (char *)&s->rider->flags; *(s32 *)tv = *(s32 *)tv & ~0x2000;
                break;
            case 2: /* fixed eye, looking 1000 ahead */
                SnowChaseActorCam_setMode_00002714((int *)s->eyeChase, 3, (int)s->rider->dnode.vel);
                SnowChaseActorLook_setMode_00002FC8((int *)((LookAtNode *)ob[0])->chase, 2, (int)s->rider->dnode.vel);
                tv = (char *)&s->rider->flags; *(s32 *)tv = *(s32 *)tv | 0x2000;
                break;
            case 3: /* fixed eye, looking 1000 behind */
                SnowChaseActorCam_setMode_00002714((int *)s->eyeChase, 3, (int)s->rider->dnode.vel);
                SnowChaseActorLook_setMode_00002FC8((int *)((LookAtNode *)ob[0])->chase, 3, (int)s->rider->dnode.vel);
                tv = (char *)&s->rider->flags; *(s32 *)tv = *(s32 *)tv | 0x2000;
                break;
            }
        }
    }
    f1 = 1.0;
    game_uso_func_00000000(state, (f1 * (f32) s->frame) / (f32) kf->frames);
    /* the eye */
    ncur = seg->eyeMode;
    switch (ncur) {
    case 1: /* the chase component moved it: keep the world position */
        pos1.x = s->cam.mnode.world[3][0];
        pos1.y = s->cam.mnode.world[3][1];
        pos1.z = s->cam.mnode.world[3][2];
        break;
    case 2:
    case 3:
        if (seg->eyeSpline == 0) {
            *(Tri3i *)&stage = *(Tri3i *)&kf->eye;
            pos1.x = stage.x; pos1.y = stage.y; pos1.z = stage.z;
        } else {
            c1[0] = (Vec3 *)&seg->keys[s->key - 1]->eye;
            c1[1] = (Vec3 *)&seg->keys[s->key]->eye;
            c1[2] = (Vec3 *)&seg->keys[s->key + 1]->eye;
            c1[3] = (Vec3 *)&seg->keys[s->key + 2]->eye;
            math_blendVec3x4_000000E0(&pos1, c1, state);
        }
        break;
    }
    /* the look-at point */
    ncur = seg->lookMode;
    switch (ncur) {
    case 1:
        bv = (char *)s->cam.target;
        pos2.x = ((CamMNode *)bv)->world[3][0];
        bv += 0x70; /* world */
        pos2.y = ((f32 *)bv)[13];
        pos2.z = ((f32 *)bv)[14];
        break;
    case 2:
    case 3:
        if (seg->lookSpline == 0) {
            *(Tri3i *)&stage = *(Tri3i *)&kf->look;
            pos2.x = stage.x; pos2.y = stage.y; pos2.z = stage.z;
        } else {
            c2[0] = (Vec3 *)seg->keys[s->key - 1];
            c2[1] = (Vec3 *)seg->keys[s->key];
            c2[2] = (Vec3 *)seg->keys[s->key + 1];
            c2[3] = (Vec3 *)seg->keys[s->key + 2];
            math_blendVec3x4_000000E0(&pos2, c2, state);
        }
        break;
    }
    /* up: (0, 1, 0) on a chase path */
    if ((seg->lookMode == 1) && (seg->eyeMode == 1)) {
        fresh.x = 0.0f; fresh.y = 1.0f; fresh.z = 0.0f;
        *(Tri3i *)&stage = *(Tri3i *)&fresh;
        up.x = stage.x; up.y = stage.y; up.z = stage.z;
    } else if (seg->upSpline == 0) {
        *(Tri3i *)&stage = *(Tri3i *)&kf->up;
        up.x = stage.x; up.y = stage.y; up.z = stage.z;
    } else {
        c3[0] = (Vec3 *)&seg->keys[s->key - 1]->up;
        c3[1] = (Vec3 *)&seg->keys[s->key]->up;
        c3[2] = (Vec3 *)&seg->keys[s->key + 1]->up;
        c3[3] = (Vec3 *)&seg->keys[s->key + 2]->up;
        math_blendVec3x4_000000E0(&up, c3, state);
    }
    /* fov */
    selv = seg->fovMode;
    if (selv == 1) {
        s->cam.fov = kf->fov;
    } else if (selv == 2) {
        w[0] = seg->keys[s->key - 1]->fov;
        w[1] = seg->keys[s->key]->fov;
        w[2] = seg->keys[s->key + 1]->fov;
        w[3] = seg->keys[s->key + 2]->fov;
        s->cam.fov = math_dot4_000000A0(w, state);
    }
    /* pitch / yaw / unk13C */
    if (s->freeRot == 0) {
        selv = seg->rotMode;
        if (selv == 1) {
            *(Tri3i *)&stage = *(Tri3i *)&kf->rot;
            s->cam.pitch = stage.x;
            s->cam.yaw = stage.y;
            s->cam.unk13C = stage.z;
        } else if (selv == 2) {
            c4[0] = (Vec3 *)&seg->keys[s->key - 1]->rot;
            c4[1] = (Vec3 *)&seg->keys[s->key]->rot;
            c4[2] = (Vec3 *)&seg->keys[s->key + 1]->rot;
            c4[3] = (Vec3 *)&seg->keys[s->key + 2]->rot;
            math_blendVec3x4_000000E0((Vec3 *)&s->cam.pitch, c4, state);
        }
    }
    /* the chase aim point (rider position + rotated cof) for relative paths */
    *(Tri3i *)&vC = *(Tri3i *)cam_toTargetLocal_000023D4(&stage, (char *)s->eyeChase);
    tv = (char *)s->rider->dnode.vqs.trans;
    { f1 = ((f32 *)tv)[0] + vC.x; fy = ((f32 *)tv)[1] + vC.y; fz = ((f32 *)tv)[2] + vC.z; v30.z = fz; v30.y = fy; v30.x = f1; }
    if (arg0) {}
    *(Tri3i *)&v3C = *(Tri3i *)&v30;
    *(Tri3i *)&vD = *(Tri3i *)&v3C;
    cam_applyRelative_0000052C((int)kf, &pos2, &pos1, (int *)seg, &vD);
    /* write the look-at / eye bodies that the path drives (local[3], world = local) */
    if (seg->lookMode != 1) {
        tv = (char *)s->cam.target;
        ((CamMNode *)tv)->local[3][0] = pos2.x;
        tv += 0x30; /* local */
        ((f32 *)tv)[13] = pos2.y;
        ((f32 *)tv)[14] = pos2.z;
        tv = (char *)s->cam.target;
        *(B3CBlk40 *)&((CamMNode *)tv)->world = *(B3CBlk40 *)&((CamMNode *)tv)->local;
    }
    if (seg->eyeMode != 1) {
        s->cam.mnode.local[3][0] = pos1.x;
        s->cam.mnode.local[3][1] = pos1.y;
        s->cam.mnode.local[3][2] = pos1.z;
        *(B3CBlk40 *)&s->cam.mnode.world = *(B3CBlk40 *)&s->cam.mnode.local;
    }
    *(Tri3i *)&stage = *(Tri3i *)&up;
    s->cam.up.x = stage.x;
    s->cam.up.y = stage.y;
    s->cam.up.z = stage.z;
    /* advance: next frame; past key->frames the next key; past numKeys (or at it, for a relative path) the
     * next path of the list (key 1) -- unless the list is done */
    nsub = s->frame; nsub += 1;
    ncur = s->key;
    nseg = s->listPos;
    if (!(nsub < kf->frames)) {
        tmp = seg->numKeys;
        ncur += 1;
        nsub = 0;
        if (tmp < ncur) goto adv; else if (ncur == tmp) { if (camPath_hasSpline_00000674((int *)seg)) { adv: ncur = 1; nseg += 1; } }
    }
    if (nseg < tbl->count) {
        s->frame = nsub;
        s->listPos = nseg;
        s->key = ncur;
    }
}

}
