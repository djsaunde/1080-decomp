/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_0003C43C: the NM C body lifted under NCC (agent-c run 134 sweep, cfe 93.00).
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
#undef NULL
#define NULL 0
typedef struct { float x, y, z; } Vec3;
#define ABS3(x) (((x) < 0.0f) ? -(x) : (x))
typedef struct {
    Vec3 pos;           
    float dist;         
    unsigned short h10; 
    short idx;          
    short count;        
    Vec3 vtx;           
    float f24;          
} GLHit3C43C;
extern "C" {
extern int D_00000000;
extern int gl_func_00000000(...);
extern float sqrtf(float);
extern int gl_proto_39E24(int, int, float, float, float, int, float);
extern Vec3 gl_36A48_cb(void *, Vec3, Vec3);
extern Vec3 pos;
extern char *o;
extern short count;
extern float dist;
extern unsigned short h10;
extern short idx;
extern Vec3 vtx;
extern float f24;
}
extern "C" int gl_func_0003C43C(char *o, int *r, float s, GLHit3C43C *a3) {
    float dist[8];
    char *hitbuf[8];
    Vec3 acc;
    GLHit3C43C tmp;
    GLHit3C43C tmp2;
    int n;
    int i;
    int best;
    float lim;
    Vec3 *row;

    acc.x = 0.0f;
    acc.y = 0.0f;
    acc.z = 0.0f;
    n = gl_func_00000000(o, 8, dist, hitbuf, r[0], r[1], r[2], 0);
    lim = *(float *)((char *)&D_00000000 + 0x1AB8);
    best = -1;
    a3->count = n;
    if (n > 0) {
        for (i = 0; i < n; i++) {
            if (ABS3(dist[i]) < lim && dist[i] > -600.0f && dist[i] > -s) {
                best = i;
                lim = ABS3(dist[i]);
            }
        }
        if (best >= 0) {
            row = (Vec3 *)(*(char **)(*(char **)(o + 0x84) + 0x54) + *(short *)(hitbuf[best] + 4) * 12);
            acc.x += row->x;
            acc.y += row->y;
            acc.z += row->z;
            tmp.pos.x = row->x;
            tmp.pos.y = row->y;
            tmp.pos.z = row->z; if (1) {}
            tmp2.pos = tmp.pos;
            a3->vtx.x = tmp2.pos.x;
            a3->vtx.y = tmp2.pos.y;
            a3->vtx.z = tmp2.pos.z;
            a3->f24 = *(float *)(hitbuf[best] + 8);
            a3->h10 = *(unsigned short *)(*(char **)(*(char **)(o + 0x84) + 0x68) + *(short *)(hitbuf[best] + 4) * 8);
            a3->idx = *(short *)(hitbuf[best] + 4);
            tmp2.pos = acc;
            a3->pos.x = tmp2.pos.x;
            a3->pos.y = tmp2.pos.y;
            a3->pos.z = tmp2.pos.z;
            a3->dist = 0.0f + dist[best];
        } else {
            n = 0;
        }
    }
    return n;
}
