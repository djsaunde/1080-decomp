/* IDO 7.1 NCC (C++) EXACT donors for heli.uso (agent-g run 658), REPLACE_FUNC_BODY-spliced over
 * the empty placeholders in heli_uso.c on both build paths (Makefile HELI_NCC_DONOR).
 * heli.uso is C++ (edg temps, `mul.s` by 2.0 in 0xB08); these are the functions whose frame and
 * constant webs only edg's lowering reproduces. */

/* the RoData base (float constants), pinned at 0 in undefined_syms_auto.txt */
extern "C" char heli_rodata_00000000;

struct HeliV3 {
    float x, y, z;
    HeliV3 &operator=(HeliV3 v) { x = v.x; y = v.y; z = v.z; return *this; }
};

/* Heli (0x174 QNode): the flight state from +0x114 */
struct Heli {
    char qnode[0x114];
    HeliV3 target;              /* 0x114: the point flown towards */
    HeliV3 off;                 /* 0x120: the camera-style offset (0, 400, 2000) */
    HeliV3 look;                /* 0x12C */
    HeliV3 head;                /* 0x138: the heading */
    float roll, pitch, yaw;     /* 0x144 / 0x148 / 0x14C */
    float maxSpeed;             /* 0x150 */
    float speed;                /* 0x154 */
    float accel;                /* 0x158 */
    float nearDist;             /* 0x15C */
    float speed2;               /* 0x160 */
    float slerp;                /* 0x164: heading slerp factor (RoData+0x74) */
    int marker;                 /* 0x168 */
    int mode;                   /* 0x16C */
    int mode2;                  /* 0x170 */
};

/* Heli_resetFlight: Heli flight-state reset. The +0x120 zero is its own constant web
 * (mtc1 zero f8, stored last): an int local converted at the store; edg folds `(float)0` into
 * the shared 0.0f. The block-scope int and the dead two-word scratch at the end are nested
 * blocks, so their homes sit below the look-copy temp (sp+12, frame 0x28). The off block stays on
 * one source line: split over four lines, the 2000 load schedules after the zero (as1 line tie). */
extern "C" void Heli_resetFlight(Heli *h) {
    h->head.z = 1.0f;
    h->roll = h->pitch = h->yaw = 0.0f;
    h->head.x = 0.0f;
    h->head.y = 0.0f;
    h->speed = 0.0f;
    h->accel = 5.0f;
    h->maxSpeed = 200.0f;
    h->nearDist = 250.0f;
    h->slerp = *(float *)(&heli_rodata_00000000 + 0x74); /* RAW: data-table RoData+0x74 float constant */
    h->target.x = 0.0f;
    h->speed2 = 0.0f;
    h->target.z = 0.0f;
    h->target.y = 0.0f;
    h->look = h->target;
    { int z = 0; h->off.x = z; h->off.y = 400.0f; h->off.z = 2000.0f; } /* one line: load-bearing */
    h->mode = 7;
    h->marker = 0;
    h->mode2 = 0;
    {
        int scratch[2];
        scratch[0] = 0;
    }
}
