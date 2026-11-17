/* SnowAI_steer: EXACT donor, IDO 7.1 cfe -O2 C (agent-c run 237, from the
 * in-unit NM body: agent-e / agent-g / agent-c runs 182-236, 99.977 -> 100),
 * REPLACE_FUNC_BODY-spliced over the placeholder in src/game_uso/game_uso.c on both
 * build paths (GAMEUSO_591C_DONOR). The superseded in-unit body (with its decode
 * history) was removed from game_uso.c in agent-g run 506 (git e6043cf4d has it).
 *
 * The last two cells (docs/IDO_CODEGEN.md kit for agent-c run 237):
 *  - `v0 * 1 == 0x10000`: uopt input swaps ==/!= when the left operand is not an
 *    isvar, so the multiply-by-one gives the target's const-first `bne v1, v0`.
 *  - every nested float selector's inner join goes through `scale` (dead after the
 *    transform dispatch; `fsel = c3 ? A : (scale = c2 ? B : C, scale)`): scale's
 *    single LR is coloured f14 (it interferes with x f0 / z f2 / fsel f12 at the
 *    head), so the inner value is f14 at all ten sites while the 0x204 selector can
 *    be fsel (f12) and the dead `if (z) {}` keeps z above fsel in priority. With
 *    cfe's own join temp the temp took f2 once z no longer reached a join block. */
#include "common.h"
#include "game/snowai.h"
#include "game/course.h"
#include "game/input.h"
#include "uso_call.h"
extern char import_80020110;
extern int import_8006EF48;
typedef struct { float x, y, z; } Vec3;
typedef struct { int a, b, c; } Tri3i;
extern int game_uso_func_055750();
float math_signedAngleXZ_00003ED4(Vec3 *a0, Vec3 *a1, int *a2);
void SnowAI_pickFork();
void SnowAI_scanNodes(SnowAI *);
void SnowAI_scanSegments(SnowAI *);
int SnowAI_updateMarker();
float SnowAI_rivalLeadZ(SnowAI *);
int SnowAI_aimAlongSegment();
#ifdef PORT /* the definition (game_uso_7C1C.c) returns its Vec3 by value: gcc's callee pops the hidden pointer (ret $4) */
extern Vec3 port_591c_aimAlongSegment(void *, void *, void *, void *, void *) __asm__("SnowAI_aimAlongSegment");
#endif
void *SnowAI_aimWeave();
void SnowAI_restoreRiderParms(SnowAI *);
void SnowAI_scaleRiderMax(SnowAI *);
void SnowAI_scaleRiderParms(SnowAI *);
void SnowAI_enterRecover(void *);
int SnowAI_isInSegmentWidth();
void SnowAI_startNodeScan(SnowAI *);
int SnowAI_isAboveSegmentFloor(SnowAI *, char *, char *);
void SnowAI_startSegmentScan(void *);
void SnowAI_enterFinish(SnowAI *);
int SnowAI_pickFinishBranch(SnowAI *);
void SnowAI_applyStickInput();
void SnowAI_steer(int *a0) {
    Vec3 copy2;
    Vec3 staged_axis;
    int out_flags;
    char *helper_ptr;
    char *hit_parent;
    char *hit_obj;
    int state_flag;
    int resolved_state;
    float metric_a;
    Vec3 transform_out;
    SnowAI *self;
    int out_w;
    int hit_out;
    Vec3 scratch_xz;
    char *sub; /* the rider (SnowBoarder *). Its two `dnode.speed * reachTime` products are written member-first:
                * with the member second the FP registers move (agent-g run 523) */
    float yaw_metric;
    float accel_metric;
    int v0;
    Vec3 stage;
    int vsel;
    int t0;
    Vec3 scaled_axis;
    float *src;
    int *child_flags;
    float fsel;
    float f2v;
    float dist_sq;
    float dot;
    float f12v;
    float yaw_raw;
    float neg;
    float *accel_params;
    Vec3 res_a;
    Vec3 res_b;
    Vec3 res_c;
    Vec3 pos_a;
    Vec3 pos_c;
    Vec3 delta_a;
    Vec3 delta_b;
    Vec3 *p;
    float x;
    float *hsrc;
    Vec3 diff_a;
    float z;
    float *axis_src;
    float scale;
    int bits;
    Vec3 *scratch_dst;
    Vec3 diff_b;
    Vec3 *delta_a_dst;
    Vec3 *delta_b_dst;
    Vec3 pos_e;
    Vec3 pos_g;
    Vec3 delta_e;
    Vec3 delta_g;
    Vec3 *delta_e_dst;
    Vec3 diff_e;
    Vec3 diff_g;
    Vec3 *delta_g_dst;
    self = (SnowAI *)a0;
    if (INPUT_MGR(import_80020110).unk78 != 0) return;
    if (import_8006EF48 == 0) return;
    v0 = self->modeFlags;
    if (v0 & 1) {
        SnowAI_pickFork(KR_ARG0(a0));
        return;
    }
    if (v0 & 2) {
        SnowAI_scanNodes(self);
        return;
    }
    if (v0 & 4) {
        SnowAI_scanSegments(self);
        return;
    }
    *(Tri3i *)&staged_axis = *(Tri3i *)&self->rider->dnode.vqs.trans;
    {
        axis_src = self->rider->dnode.vel;
        fsel = self->lookAhead;
        scale = *axis_src * fsel;
        z = axis_src[1] * fsel; if (z) {}
        x = axis_src[2] * fsel;
        scaled_axis.z = x; if (1) {}
        scaled_axis.y = z;
        scaled_axis.x = scale;
    }
    *(Tri3i *)&stage = *(Tri3i *)&scaled_axis;
    *(Tri3i *)&copy2 = *(Tri3i *)&stage;
    out_flags = 0;
    staged_axis.x += copy2.x;
    staged_axis.y += copy2.y;
    staged_axis.z += copy2.z;
    helper_ptr = (char*)SnowAI_updateMarker(self, &hit_parent, &staged_axis, &hit_obj);
    if (helper_ptr == 0) return;
    state_flag = ((CourseMarker *)helper_ptr)->parms.flg.value;
    if (hit_obj != 0) {
        resolved_state = state_flag;
    } else {
        resolved_state = 0;
    }
    metric_a = SnowAI_rivalLeadZ(self);
    t0 = 0;
    switch (v0 = self->aimMode) {
    case 0:
        if (metric_a <= self->weaveLead) {
            if (((v0 = state_flag) == 0) && (hit_parent != 0)
                    && (((CourseMarker *)hit_parent)->parms.flg.value == 0)) {
                if (self->minSpeed <= self->rider->dnode.speed) {
                    self->aimMode = 1;
                    t0 = 1;
                }
            }
        }
        break;
    case 1:
        v0 = self->level;
        fsel = v0 == 3 ? self->parms.parm[SNOWAI_PARM_SE + 2].value : (scale = v0 == 2 ? self->parms.parm[SNOWAI_PARM_SE + 1].value : self->parms.parm[SNOWAI_PARM_SE].value, scale);
        if (!(metric_a <= fsel) || state_flag != 0 || hit_parent == 0
                || ((CourseMarker *)hit_parent)->parms.flg.value != 0
                || !(self->minSpeed
                     <= self->rider->dnode.speed)) {
            self->aimMode = 0;
        }
        break;
    }
    p = 0;
    if (1) { p = &scratch_xz; }
    switch (v0 = self->aimMode) {
    case 0:
#ifdef PORT /* called as an out-pointer function, the 4-byte drift corrupts this frame (plan 18.17) */
        stage = port_591c_aimAlongSegment(self, helper_ptr, hit_parent, &staged_axis, &out_w);
        v0 = (int)&stage;
#else
        v0 = SnowAI_aimAlongSegment(&stage, self, helper_ptr, hit_parent,
            &staged_axis, &out_w);
#endif
        *(Tri3i *)&res_a = *(Tri3i *)v0;
        *(Tri3i *)&res_c = *(Tri3i *)&res_a;
        x = res_c.x; scale = res_c.y; z = res_c.z; transform_out.x = x; transform_out.y = scale; transform_out.z = z;
        break;
    case 1:
        v0 = (int)SnowAI_aimWeave(&stage, self, helper_ptr, hit_parent, t0);
        *(Tri3i *)&res_b = *(Tri3i *)v0;
        *(Tri3i *)&res_c = *(Tri3i *)&res_b;
        if (1) {}
        x = res_c.x; scale = res_c.y; z = res_c.z; transform_out.x = x; transform_out.y = scale; transform_out.z = z;
        break;
    }
    {
        scratch_dst = p;
        src = (float *)((int)(char *)self->rider + 968);
        if (scratch_dst != 0 || (scratch_dst = (Vec3*)XCALL(game_uso_func_055750, mem_new)(0xC + 0 * (int)src)) != 0) {
            fsel = src[0 * ((int)src | 1)];
            z = *(float *)((int)src + 8 + 0 * (int)src);
            do { } while (0);
            scratch_dst->x = fsel;
            scratch_dst->z = z;
            scratch_dst->y = 0.0f;
        }
    }
    yaw_raw = math_signedAngleXZ_00003ED4(p, &transform_out, &hit_out);
    fsel = yaw_raw < 0.0f ? -yaw_raw : yaw_raw;
    yaw_metric = fsel;
    neg = -yaw_raw;
    sub = (char *)self->rider;
    {
        axis_src = (float *)&((SnowAI *)a0)->rider->parms.nts;
        self->unk3C =
            (neg * (1.0f + (((SnowBoarder *)sub)->dnode.speed / self->turnSpeedRef))
             * self->turnGain) / *(float *)&((SnowBParm *)axis_src)->value;
    }
    f2v = self->unk3C;
    fsel = f2v < 0.0f ? -f2v : f2v;
    accel_metric = fsel;
    v0 = self->aimMode;
    switch (v0) {
    case 0:
    bits = self->unk6C;
    if (bits & 1) {
        if ((resolved_state & 1) && !(resolved_state & 2)) {
            self->unk6C = bits & ~1;
            self->unk48 = 0x14;
            if (resolved_state & 4) {
                out_flags = 0x20;
            } else if (resolved_state & 0x800) {
                out_flags = 0x80;
            }
            goto tail_common;
        }
        if (hit_parent == 0 || !(((CourseMarker *)hit_parent)->parms.flg.value & 2))
            goto tail_common;
        p = 0;
        if (1) { p = &pos_a; }
        src = ((CourseMarker *)helper_ptr)->pos;
        if (p != 0 || (p = (Vec3*)XCALL(game_uso_func_055750, mem_new)(0xC)) != 0) {
            fsel = *src;
            z = src[2];
            p->y = 0.0f;
            p->z = z;
            p->x = fsel;
        }
        p = 0;
        if (1) { p = &diff_a; }
        hsrc = ((CourseMarker *)hit_parent)->pos;
        {
            delta_a_dst = p;
            if (delta_a_dst != 0 || (delta_a_dst = (Vec3*)XCALL(game_uso_func_055750, mem_new)(0xC)) != 0) {
                x = pos_a.x - *hsrc;
                z = pos_a.z - hsrc[2];
                delta_a_dst->y = 0.0f;
                delta_a_dst->x = x;
                delta_a_dst->z = z;
            }
        }
        stage = *p;
        delta_a = stage;
        p = 0;
        if (1) { p = &pos_c; }
        child_flags = (int *)self->rider->dnode.vqs.trans;
        if (p != 0 || (p = (Vec3*)XCALL(game_uso_func_055750, mem_new)(0xC)) != 0) {
            fsel = *(float *)child_flags;
            z = ((float *)child_flags)[2];
            p->y = 0.0f;
            p->x = fsel;
            p->z = z;
        }
        p = 0;
        if (1) { p = &diff_b; }
        accel_params = (float *)((int)hit_parent + 48 + 0 * (int)child_flags);
        {
            child_flags = (int *)p;
            if (child_flags != 0 || (child_flags = (int *)XCALL(game_uso_func_055750, mem_new)(0xC)) != 0) {
                x = pos_c.x - *accel_params;
                z = pos_c.z - *(float *)((int)accel_params + 8 + 0 * (int)accel_params);
                ((Vec3 *)child_flags)->y = 0.0f;
                ((Vec3 *)child_flags)->x = x;
                ((Vec3 *)child_flags)->z = z;
            }
        }
        stage = *(Vec3 *)((int)p + 0 * (int)accel_params);
        delta_b = stage;
        sub = (char *)((SnowAI *)a0)->rider;
        f2v = ((SnowBoarder *)sub)->dnode.speed * self->reachTime;
        dist_sq = (delta_a.x * delta_a.x) + (delta_a.z * delta_a.z);
        if (0.0f == dist_sq) {
        f12v = 0.0f;
    } else {
        dot = (delta_a.x * delta_b.x) + (delta_a.z * delta_b.z);
        f12v = (dot * dot) / dist_sq;
    }
        if (f12v <= f2v * f2v) {
            self->unk6C = self->unk6C & ~1;
            self->unk48 = 0x14;
        }
        goto tail_common;
    }
    if (bits != 0) goto tail_common;
    if (hit_parent == 0 || !((v0 = ((CourseMarker *)hit_parent)->parms.flg.value) & 1)) goto yaw_region;
    if (v0 & 0x804) {
        if (((SnowBoarder *)sub)->rival != 0) {
            if (!(metric_a <= -2000.0f)) goto yaw_region;
        }
    }
    p = 0;
    if (1) { p = &pos_e; }
    src = ((CourseMarker *)helper_ptr)->pos;
    if (p != 0 || (p = (Vec3*)XCALL(game_uso_func_055750, mem_new)(0xC)) != 0) {
        fsel = *src;
        z = src[2];
        p->y = 0.0f;
        p->z = z;
        p->x = fsel;
    }
    p = 0;
    if (1) { p = &diff_e; }
    hsrc = ((CourseMarker *)hit_parent)->pos;
    {
        delta_e_dst = p;
        if (delta_e_dst != 0 || (delta_e_dst = (Vec3*)XCALL(game_uso_func_055750, mem_new)(0xC)) != 0) {
            x = pos_e.x - *hsrc;
            z = pos_e.z - hsrc[2];
            delta_e_dst->y = 0.0f;
            delta_e_dst->x = x;
            delta_e_dst->z = z;
        }
    }
    stage = *p;
    delta_e = stage;
    p = 0;
    if (1) { p = &pos_g; }
    child_flags = (int *)self->rider->dnode.vqs.trans;
    if (p != 0 || (p = (Vec3*)XCALL(game_uso_func_055750, mem_new)(0xC)) != 0) {
        fsel = *(float *)child_flags;
        z = ((float *)child_flags)[2];
        p->y = 0.0f;
        p->x = fsel;
        p->z = z;
    }
    p = 0;
    if (1) { p = &diff_g; }
    accel_params = (float *)((int)hit_parent + 48 + 0 * (int)child_flags);
    {
        child_flags = (int *)p;
        if (child_flags != 0 || (child_flags = (int *)XCALL(game_uso_func_055750, mem_new)(0xC)) != 0) {
            x = pos_g.x - *accel_params;
            z = pos_g.z - *(float *)((int)accel_params + 8 + 0 * (int)accel_params);
            ((Vec3 *)child_flags)->y = 0.0f;
            ((Vec3 *)child_flags)->x = x;
            ((Vec3 *)child_flags)->z = z;
        }
    }
    stage = *(Vec3 *)((int)p + 0 * (int)accel_params);
    delta_g = stage;
    sub = (char *)self->rider;
    f2v = ((SnowBoarder *)sub)->dnode.speed * (self->reachTime + 12.0f);
    dist_sq = (delta_e.x * delta_e.x) + (delta_e.z * delta_e.z);
    if (0.0f == dist_sq) {
        f12v = 0.0f;
    } else {
        dot = (delta_e.x * delta_g.x) + (delta_e.z * delta_g.z);
        f12v = (dot * dot) / dist_sq;
    }
    if (f12v <= f2v * f2v) {
        out_flags = 1;
    }
yaw_region:
    sub = (char *)self->rider;
    dot = ((SnowBoarder *)sub)->dnode.speed;
    if (self->stallYaw <= yaw_metric) {
        if (dot < 50.0f) {
            out_flags |= 0x40;
        }
    }
    if (dot < self->minSpeed) {
        if (hit_out == 0) {
            if ((double)yaw_metric < 0.125) {
                if (INPUT_MGR(import_80020110).unk78 == 0) {
                    out_flags |= 4;
                }
            }
        }
    }
    if (1.0f < (dot / self->crouchSteerRef) * accel_metric) {
        out_flags |= 8;
    }
    if (state_flag & 8) {
        if (self->scaleTimer > 0) {
            SnowAI_restoreRiderParms(self);
            self->scaleTimer = 0;
        }
        goto tail_common;
    }
    v0 = self->scaleTimer;
    if (v0 * 1 == 0x10000) {
        v0 = self->level;
        fsel = v0 == 3 ? self->parms.parm[SNOWAI_PARM_DD0 + 2].value : (scale = v0 == 2 ? self->parms.parm[SNOWAI_PARM_DD0 + 1].value : self->parms.parm[SNOWAI_PARM_DD0].value, scale);
        if (metric_a < fsel) {
            SnowAI_scaleRiderMax(self);
            v0 = self->level;
            fsel = v0 == 3 ? self->parms.parm[SNOWAI_PARM_DD1 + 2].value : (scale = v0 == 2 ? self->parms.parm[SNOWAI_PARM_DD1 + 1].value : self->parms.parm[SNOWAI_PARM_DD1].value, scale);
            self->scaleTimer = (int)fsel;
        }
        goto tail_common;
    } else if (v0 > 0) {
        self->scaleTimer = v0 - 1;
        if (self->scaleTimer == 0) {
            SnowAI_restoreRiderParms(self);
        }
    } else {
        v0 = self->level;
        if (v0 != 0) {
            fsel = v0 == 3 ? self->parms.parm[SNOWAI_PARM_DP + 2].value : (scale = v0 == 2 ? self->parms.parm[SNOWAI_PARM_DP + 1].value : self->parms.parm[SNOWAI_PARM_DP].value, scale);
            if (fsel < metric_a) {
                self->scaleTimer = 0x10000;
                SnowAI_scaleRiderParms(self);
            }
        }
    }
    v0 = self->level;
    fsel = v0 == 3 ? self->parms.parm[SNOWAI_PARM_DE + 2].value : (scale = v0 == 2 ? self->parms.parm[SNOWAI_PARM_DE + 1].value : self->parms.parm[SNOWAI_PARM_DE].value, scale);
    sub = (char *)self->rider;
    if (fsel < metric_a) {
        if (((SnowBoarder *)sub)->useDParms != 0) {
            f2v = self->dParmsSteerScale;
        } else {
            f2v = 1.0f;
        }
        if (((accel_metric <= f2v * self->dParmsSteer) + 0 * (int)(*a0))) {
            out_flags |= 2;
            goto tail_common;
        }
    }
    if (resolved_state & 0x800) {
        if ((((SnowBoarder *)sub)->rival == 0) || (metric_a <= -2000.0f)) {
            out_flags |= 0x80;
        }
    }
tail_common:
        /* self->level, read as unsigned through its address: the plain member (even cast) moves the code by 10 words */
        v0 = *(unsigned int *)&self->level;
        break;
    case 1:
    default:
        v0 = *(unsigned int *)&self->level;
        break;
    }
    fsel = v0 == 1 ? self->parms.parm[SNOWAI_PARM_CF].value : (scale = v0 == 2 ? self->parms.parm[SNOWAI_PARM_CF + 1].value : self->parms.parm[SNOWAI_PARM_CF + 2].value, scale);
    sub = (char *)self->rider;
    {
        child_flags = (int *)&((SnowBoarder *)sub)->flags;
        if ((metric_a < fsel) && (((int)metric_a & 3) == 0)) {
            *child_flags |= 0x4000;
        } else {
            *child_flags &= ~0x4000;
        }
    }
    if (self->rider->dnode.speed < 30.0f) {
        { int cnt; cnt = self->unk54 + 1;
        self->unk54 = cnt;
        if (child_flags) {}
        if (cnt >= 75) {
            self->unk54 = 0;
            SnowAI_enterRecover(self);
        } }
    } else {
        self->unk54 = 0;
    }
    if (state_flag & 0x20) {
        if (SnowAI_isInSegmentWidth(self, helper_ptr, hit_parent) == 0) {
            SnowAI_startNodeScan(self);
            goto commit_flags;
        }
    }
    if (state_flag & 0x40) {
        if (SnowAI_isAboveSegmentFloor(self, helper_ptr, hit_parent) == 0) {
            SnowAI_startSegmentScan(self);
            goto commit_flags;
        }
    }
    if (resolved_state & 0x400) {
        SnowAI_enterFinish(self);
        self->targetNode = SnowAI_pickFinishBranch(self);
        goto commit_flags;
    }
    if (resolved_state & 0x10) {
        self->altNode = ((CourseMarker *)helper_ptr)->parms.ext.value;
        if (resolved_state & 0x200) {
            v0 = self->level;
            fsel = v0 == 3 ? self->parms.parm[SNOWAI_PARM_BJ + 2].value : (scale = v0 == 2 ? self->parms.parm[SNOWAI_PARM_BJ + 1].value : self->parms.parm[SNOWAI_PARM_BJ].value, scale);
            if (!(fsel <= metric_a)) {
                v0 = (int)self->rider->rival;
                if (v0 == 0) goto commit_flags;
                yaw_raw = ((SnowBoarder *)v0)->dnode.vqs.trans[2];
                f2v = yaw_raw < 0.0f ? -yaw_raw : yaw_raw;
                if (((int)f2v % 5) != 0) goto commit_flags;
            }
            self->targetNode = self->altNode;
        } else if (resolved_state & 0x100) {
            v0 = self->level;
            if (v0 != 0) {
                fsel = v0 == 3 ? self->parms.parm[SNOWAI_PARM_BT + 2].value : (scale = v0 == 2 ? self->parms.parm[SNOWAI_PARM_BT + 1].value : self->parms.parm[SNOWAI_PARM_BT].value, scale);
                if (!(metric_a <= fsel)) goto commit_flags;
            }
            self->targetNode = self->altNode;
        } else if (resolved_state & 0x80) {
            v0 = self->level;
            fsel = v0 == 3 ? self->parms.parm[SNOWAI_PARM_BD + 2].value : (scale = v0 == 2 ? self->parms.parm[SNOWAI_PARM_BD + 1].value : self->parms.parm[SNOWAI_PARM_BD].value, scale);
            if (!(metric_a <= fsel)) goto commit_flags;
            self->targetNode = self->altNode;
        } else {
            v0 = self->level;
            fsel = v0 == 3 ? self->parms.parm[SNOWAI_PARM_BU + 2].value : (scale = v0 == 2 ? self->parms.parm[SNOWAI_PARM_BU + 1].value : self->parms.parm[SNOWAI_PARM_BU].value, scale);
            if (fsel <= metric_a) {
                self->targetNode = self->altNode;
            } else if (0.0f < metric_a) {
                self->modeFlags |= 1;
            } else {
                v0 = (int)self->rider->rival;
                if (v0 == 0) goto commit_flags;
                yaw_raw = ((SnowBoarder *)v0)->dnode.vqs.trans[2];
                fsel = yaw_raw < 0.0f ? -yaw_raw : yaw_raw;
                if (((int)fsel % 5) != 0) goto commit_flags;
                self->targetNode = self->altNode;
            }
        }
    } else if (hit_parent != 0 && ((CourseMarker *)hit_parent)->link == 0) {
        if (self->targetNode != 0) {
            self->targetNode = ((CourseMarker *)hit_parent)->parms.ext.value;
        }
    } else if (hit_parent == 0) {
        if (self->targetNode != 0) {
            self->targetNode = ((CourseMarker *)helper_ptr)->parms.ext.value;
        } else {
            out_flags |= 0x10;
        }
    }
commit_flags:
    SnowAI_applyStickInput(self, out_flags);
}
