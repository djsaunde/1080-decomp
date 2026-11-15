/* IDO 7.1 -O2 donor for SnowBCollider_edgeGrip_0000E2D0 -- plane reflect / scale helper (0x20C / 131 words).
 *
 * WIRED via REPLACE_FUNC_BODY donor splice into bootup_uso.c.o (the E270 recipe):
 * the f64 literal 0.1 lives in bootup.uso RoData at +0x9A0 (target reloc
 * SndHandle_triggerEvent1_0000098C + 0x14); the donor-local .rodata reloc is renamed
 * SnowBCollider_edgeGrip_0000E2D0_rodata and pinned to 0x9A0.
 *
 * The post-call FP temp ring (1.0 f8 / edgeSq f16 / sum f18, then f4 f10 f16 /
 * f6 f8 f18 in the scale block) comes from the two code-free tests after the
 * 9F8 line (agent-h run 408): uopt deletes the tests only after ugen has been
 * handed the shared sub-expression, so ugen loads and combines the v[] terms
 * into ring temps that as1 then deletes as dead. Those zero-code pop / free
 * pairs rotate the FIFO into the target's order. Comparing against lim and
 * dot (not constants) also leaves lim in f2 and -lim in f12. edgeSq is NOT kept
 * live, so it is a ring temp as in the target. */
#include "common.h"
#include "game/snowboarder.h"
#include "uso_call.h"

typedef struct { float x, y, z; } Vec3;
extern float func_00000000_4948_ff(float);
void func_00000000();

/* The edge-grip response (agent-g run 475, on snowboarder.h fields). Called by SnowBCollider_onContact for a board
 * probe: point = the probe's world position, pointVel = its velocity, normal = the contact normal, edgeSq = edge^2.
 * v[1] = -(pointVel's in-plane part), v[0] = -(the side axis's in-plane part). When the in-plane side axis is long
 * enough (> 0.1), it is normalized and the sideways slip dot(v[1], v[0]) * edgeFriction, clamped to +-edgeGrip and
 * scaled by 1 + edgeSq, is applied along it as an impulse at the probe (DNode_addImpulseAt): the edge bites. */
void SnowBCollider_edgeGrip_0000E2D0(SnowBoarder *rider, s32 point, Vec3 *pointVel, Vec3 *normal, f32 edgeSq) {
    Vec3 v[2];
    f32 d;
    f32 lim;
    f32 s;
    int X;
    f32 dot;
    int pad[7];

    v[1].x = pointVel->x;
    v[1].y = pointVel->y;
    v[1].z = pointVel->z;
    d = v[1].z * normal->z + (v[1].y * normal->y + v[1].x * normal->x);
    v[1].x = d * normal->x - v[1].x;
    v[1].y = d * normal->y - v[1].y;
    v[1].z = d * normal->z - v[1].z; if (1) {}
    v[0].x = rider->side.x;
    v[0].y = rider->side.y;
    v[0].z = rider->side.z;
    d = v[0].z * normal->z + (v[0].y * normal->y + v[0].x * normal->x);
    v[0].x = d * normal->x - v[0].x;
    v[0].y = d * normal->y - v[0].y;
    v[0].z = d * normal->z - v[0].z;
    if (0.1 < (f64)XCALL(func_00000000_4948_ff, math_vec3Length_0005BBCC)( /* math_vec3Length_0005BBCC */v[0].z * v[0].z + (v[0].y * v[0].y + v[0].x * v[0].x))) {
        dot = v[1].z * v[0].z + (v[1].y * v[0].y + v[0].x * v[1].x);
        FCALL(math_vec3Normalize)(&v[0]); if (normal) {} /* math_vec3Normalize */
        lim = rider->edgeGrip;
        dot *= rider->edgeFriction;
        if ((((v[1].z + v[1].y) + (v[0].z * v[0].x)) - (v[0].y * v[1].x)) != lim) {} if ((((v[1].z + v[1].y) + (v[0].z * v[0].x)) - (v[0].y * v[1].x)) != dot) {}
        if (dot < -lim) {
            dot = -lim;
        }
        if (lim < dot) {
            dot = lim;
        }
        dot *= 1.0f + edgeSq;
        v[0].x *= dot;
        v[0].y *= dot;
        v[0].z *= dot;
        FCALL(DNode_addImpulseAt_00065250)(rider, point, &v[0]); /* DNode_addImpulseAt_00065250 */
    }
}
