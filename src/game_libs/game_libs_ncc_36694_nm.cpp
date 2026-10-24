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
    { f32 h_x0 = n.x * d; f32 h_y0 = n.y * d; f32 h_z0 = n.z * d;
    vAC.x = h_x0; vAC.z = h_z0; vAC.y = h_y0; }
    vBC = vAC;
    vE4 = vBC;
    { f32 h_x1 = p.x - vE4.x; f32 h_y1 = p.y - vE4.y; f32 h_z1 = p.z - vE4.z;
    v78.x = h_x1; v78.z = h_z1; v78.y = h_y1; }
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
        { f32 h_x2 = vD8.x * one; f32 h_y2 = vD8.y * one; f32 h_z2 = vD8.z * one;
        v3C.y = h_y2; v3C.z = h_z2; v3C.x = h_x2; }
        vBC = v3C;
        v48 = vBC;
        len = gl_sqrtf_36694(len2);
        { f32 h_x3 = v48.x / len; f32 h_y3 = v48.y / len; f32 h_z3 = v48.z / len;
        v30.x = h_x3; v30.y = h_y3; v30.z = h_z3; }
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
