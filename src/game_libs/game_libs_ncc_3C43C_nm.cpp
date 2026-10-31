/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_0003C43C: the NM C body lifted under NCC (agent-c run 134 sweep, cfe 93.00).
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY.
 * agent-c run 177 (94.26 -> 96.05, 864 -> 868 bytes, target 872): the limit constant read into a block-scope
 * `k` then copied into lim (the target's f16 constant + `mov.s f12` copy, kept apart from the -600 constant), and
 * `0.0f + dist[best]` read into dd right after the f24 store (the target's early dist[best] load and add above the
 * h10 store). Residual: 24 extra frame bytes below the staging records (target frame 0xE0).
 * agent-c run 249 (98.26 -> 99.47, frame 0xE0 = target): the bottom of the frame holds only the ternary temp, the
 * uopt unroll home and two uopt temp words, so the named scalars are declared BETWEEN the staging records (tmp / tmp2
 * shrink to keep pos at the target offsets); dist before hitbuf, no zero-use on best, dd function-scope with
 * `dd = 0.0f; if (1) {} dd = dd + dist[best];` (99.47); then the position passed BY VALUE (`Vec3 *r`, `*r` as one
 * struct argument: the t8 / t7 / t8 alternating struct-copy words at sp+16..24 and the r home spill + reload): 99.91.
 * Residual: the acc zero constant is coloured f14 (target f0). */
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
extern "C" int gl_func_0003C43C(char *o, Vec3 *r, float s, GLHit3C43C *a3) {
    float dist[8];
    char *hitbuf[8];
    Vec3 acc;
    int n;
    int i;
    float dd;
    struct { Vec3 pos; int pad[4]; } tmp;
    int best;
    float lim;
    struct { Vec3 pos; int pad[5]; } tmp2;
    Vec3 *row;

    acc.x = 0.0f;
    acc.y = 0.0f;
    acc.z = 0.0f;
    n = gl_func_00000000(o, 8, dist, hitbuf, *r, 0);
    lim = 1000000.0f; /* agent-g run 217 (kit CCXIV): bootup.uso RoData 0x1AB8 = 0x49742400 is the literal 1e6, no k copy: 96.78 -> 98.24 */
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
            dd = 0.0f; if (1) {} dd = dd + dist[best];
            a3->h10 = *(unsigned short *)(*(char **)(*(char **)(o + 0x84) + 0x68) + *(short *)(hitbuf[best] + 4) * 8);
            a3->idx = *(short *)(hitbuf[best] + 4);
            tmp2.pos = acc;
            a3->pos.x = tmp2.pos.x;
            a3->pos.y = tmp2.pos.y;
            a3->pos.z = tmp2.pos.z;
            a3->dist = dd;
        } else {
            n = 0;
        }
    }
    return n;
}
