/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00036694: the NM C body lifted under NCC + one parameter empty test from a fz sweep (agent-c run 134, cfe 82.41).
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
#undef NULL
#define NULL 0
typedef struct { f32 x, y, z; } V3_36694;
extern "C" {
extern char *p;
extern f32 gl_sqrtf_36694(f32);
extern f32 gl_sqrtf_36694(f32);
extern void gl_normalize_36694(V3_36694 *);
}
extern "C" V3_36694 *gl_func_00036694(V3_36694 *out, s32 unused, V3_36694 p, V3_36694 n) {
    V3_36694 vE4;
    V3_36694 vD8;
    f32 len2;
    V3_36694 vBC;
    V3_36694 vAC;
    V3_36694 v8C;
    V3_36694 v78;
    V3_36694 v5C;
    V3_36694 v48;
    V3_36694 v3C;
    V3_36694 v30;
    f32 px;
    f32 py;
    f32 pz;
    f32 d;
    f32 one;
    f32 m1;
    f32 len;
    V3_36694 *res;

    d = n.x * p.x + n.y * p.y + n.z * p.z;
    vAC.x = n.x * d;
    vAC.y = n.y * d;
    vAC.z = n.z * d;
    vBC = vAC;
    vE4 = vBC;
    v78.x = p.x - vE4.x;
    v78.y = p.y - vE4.y;
    v78.z = p.z - vE4.z;
    v8C = v78;
    v5C = v8C;
    px = v5C.x;
    py = v5C.y;
    pz = v5C.z;
    len2 = px * px + py * py + pz * pz;
    if (0.0f < len2) {
        if (pz < 0.0f) {
            m1 = (f32)-1;
            px = px * m1;
            py = py * m1;
            pz = pz * m1;
        }
        one = (f32)1;
        vD8.z = pz;
        vD8.x = px;
        vD8.y = py;
        v3C.x = vD8.x * one;
        v3C.y = vD8.y * one;
        v3C.z = vD8.z * one;
        vBC = v3C;
        v48 = vBC;
        len = gl_sqrtf_36694(len2);
        v30.x = v48.x / len;
        v30.y = v48.y / len;
        v30.z = v48.z / len;
        v8C = v30;
        res = &v8C;
    } else {
        if (n.z == 1.0f) {
            vD8.z = 0.0f;
            vD8.x = 1.0f;
            vD8.y = 0.0f;
        } else {
            vD8.z = 0.0f;
            vD8.x = -n.y;
            vD8.y = n.x;
            gl_normalize_36694(&vD8);
        }
        res = &vD8;
    }
    *out = *res; if (unused) {}
    return out;
}
