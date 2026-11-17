/* IDO 7.1 NCC (C++) NON_MATCHING donor for snoweffects.uso 0x47D4 (agent-h run 694), spliced over the empty
 * NON_MATCHING placeholder in snoweffects_uso.c on the non_matching build only (the matching build keeps
 * INCLUDE_ASM). The trail emitter update: u4C = +-cam+0x970 (flag 0x80); p / q zeroed; call 0x4374; hits /
 * u80; on = 0x46BC(t); alpha from u160..u168; 0x4714(t, on, prev, alpha); then for each i < stride the point
 * w = lerp(P, Q, i / (stride - 1)) (+ the normalized cam+0x318 * u154 and - u158 for i == 1 / 2 in mode 98),
 * written to points[writeOfs + i] and vtx[writeOfs + i]. Left: FP colouring (the target keeps w.x / w.y / w.z
 * in uopt colours, alpha in f22 and 5.0f in f20 across the 0x46BC call) and the 184-byte frame. */
extern "C" int gl_func_00000000(...);
extern "C" char D_00000000[];
struct SnowBV3 {
    float x, y, z;
    SnowBV3() {}
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    SnowBV3 &operator=(SnowBV3 o) { x = o.x; y = o.y; z = o.z; return *this; }
};
struct SnowVEnt { short d; short pad; void (*fn)(void *); };
struct SnowECam {
    char pad[0x318];
    SnowBV3 v318;
    char pad324[0x348 - 0x324];
    float speed;          /* 348 */
    char pad34C[0x970 - 0x34C];
    float f970;
    char pad974[0x9A2 - 0x974];
    short mode;           /* 9A2 */
    char pad9A4[0xA30 - 0x9A4];
    unsigned char col[4]; /* A30 */
    char padA34[0xA58 - 0xA34];
    int flags;            /* A58 */
};
struct SnowEPoint { SnowBV3 pos; SnowBV3 vel; float w18; float w1C; float w20; float w24; int u28; };
struct SnowESlot { int state; int u4; int u8; };
struct SnowEVtx { short ob[3]; short flag; short tc[2]; unsigned char cn[4]; };
struct SnowEm {
    char pad00[0x28];
    SnowVEnt *vptr;     /* 28 */
    SnowECam *cam;      /* 2C */
    char pad30[0x34 - 0x30];
    float u34, u38;
    float len, u40;     /* 3C 40 */
    char pad44[0x4C - 0x44];
    float u4C;
    char pad50[0x5C - 0x50];
    int writeOfs;       /* 5C */
    int read;           /* 60 */
    int write;          /* 64 */
    int size;           /* 68 */
    int stride;         /* 6C */
    int count;          /* 70 */
    int on;             /* 74 */
    int prev;           /* 78 */
    int prevMode;       /* 7C */
    int u80;            /* 80 */
    char pad84[0x8C - 0x84];
    SnowEVtx *vtx;      /* 8C */
    SnowEPoint *points; /* 90 */
    SnowESlot *slots;   /* 94 */
    char pad98[0xA8 - 0x98];
    int uA8;
    char padAC[0x120 - 0xAC];
    SnowBV3 p;          /* 120 */
    SnowBV3 q;          /* 12C */
    char pad138[0x144 - 0x138];
    int hits;           /* 144 */
    char pad148[0x154 - 0x148];
    float u154, u158;
    char pad15C[0x160 - 0x15C];
    float u160, u164, u168, u16C;
};
extern "C" int gl_func_00000000_snow_46bc(SnowEm *);  /* 0x46BC, typed blank jal */
extern "C" void gl_func_00000000_snow_4714(SnowEm *, int, int, float);
extern "C" void snoweffects_uso_func_000047D4(SnowEm *t) {
    SnowECam *cam;
    float a, alpha, f, ab, drop;
    float lim;
    int i;
    SnowBV3 P, Q, dv, v;
    cam = t->cam;
    if (cam->flags & 0x80) {
        t->u4C = -cam->f970;
    } else {
        t->u4C = cam->f970;
    }
    t->p.x = 0.0f; t->p.y = 0.0f; t->p.z = 0.0f;
    t->q.x = 0.0f; t->q.y = 0.0f; t->q.z = 0.0f;
    t->len = t->u34;
    t->u40 = t->u38;
    gl_func_00000000(t);  /* 0x4374 */
    if (t->hits <= 0) {
        t->u80 = 1;
    } else {
        t->u80 = 0;
    }
    cam = t->cam;
    if ((cam->mode != 98 && cam->mode != 97) || cam->speed < 5.0f) {
        t->hits = 0;
    }
    t->on = gl_func_00000000_snow_46bc(t);
    cam = t->cam;
    if (cam->speed < t->u160) {
        a = t->u164 / (t->u160 - 5.0f) * (cam->speed - 5.0f);
    } else {
        a = t->u164;
    }
    if (t->u4C < 0.0f) {
        ab = -t->u4C;
    } else {
        ab = t->u4C;
    }
    alpha = ab * t->u168 * (255.0f - a) + a;
    if (cam->mode == 98) {
        t->slots[t->write].u4 = 1;
    } else {
        t->slots[t->write].u4 = 0;
    }
    P = t->p;
    Q = t->q;
    gl_func_00000000_snow_4714(t, t->on, t->prev, alpha);
    if (t->slots[t->write].state == 2) {
        alpha = 0.0f;
    }
    if (t->stride == 4 && (cam = t->cam)->mode == 98) {
        lim = t->u158;
        dv = cam->v318;
        gl_func_00000000(&dv);  /* math_vec3Normalize */
        { float s = t->u154; dv.x *= s; dv.y *= s; dv.z *= s; }
    } else {
        lim = 0.0f;
        dv.x = 0.0f; dv.y = 0.0f; dv.z = 0.0f;
    }
    if (t->on != 0) {
        for (i = 0; i < t->stride; i++) {
            SnowBV3 w = P;
            f = (float)i / (float)(t->stride - 1);
            w.x = (Q.x - w.x) * f + w.x;
            w.y = (Q.y - w.y) * f + w.y;
            w.z = (Q.z - w.z) * f + w.z;
            if (i == 1 || i == 2) {
                w.x += dv.x; w.y += dv.y; w.z += dv.z;
                w.y -= lim;
            }
            t->points[t->writeOfs + i].pos = w;
            t->points[t->writeOfs + i].w18 = 0.0f;
            t->points[t->writeOfs + i].vel.set(0.0f, 0.0f, 0.0f);
            t->points[t->writeOfs + i].u28 = 0;
            t->points[t->writeOfs + i].w1C = alpha;
            t->points[t->writeOfs + i].w20 = t->points[t->writeOfs + i].w1C / ((float)t->size / t->u16C);
            t->vtx[t->writeOfs + i].tc[0] = (int)(((float)t->uA8 - 1.0f) * ((float)i / (float)(t->stride - 1))) << 6;
            t->vtx[t->writeOfs + i].tc[1] = ((t->writeOfs & t->stride) >> 1) * 31U << 6;
            t->vtx[t->writeOfs + i].cn[0] = t->cam->col[0];
            t->vtx[t->writeOfs + i].cn[1] = t->cam->col[1];
            t->vtx[t->writeOfs + i].cn[2] = t->cam->col[2];
        }
    }
    t->vptr[16].fn((void *)((int)t->vptr[16].d + (int)t));
    t->prev = t->on;
    t->prevMode = t->cam->mode;
}
