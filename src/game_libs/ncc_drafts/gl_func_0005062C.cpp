/* NCC (C++) DRAFT for gl_func_0005062C -- NOT built, NOT matching (agent-g run 113).
 *
 * Compile standalone with IDO 7.1 NCC (flags as in game_libs_ncc_62A2C.cpp).
 * Score vs expected/: 690/689 insns, objdiff 93.3%, frame 592 exact (the cfe
 * NM wrap scores ~67).
 *
 * Recursive triangle subdivider (C++): three normal callbacks, an
 * |n0+n1+n2| / 2 < scale*4 stop test, six edge midpoints
 * m = (a + b) / 2.0f with V3 operator+ / operator/ returning
 * V3(float, float, float) constructor temporaries and the by-value
 * operator= of game_libs_ncc_62A2C.cpp, then either four recursive calls
 * (level - 1) or six vertex emits + four triangle emits. The seven int
 * parameters live in their arg homes in the target (reloaded per call):
 * reproduced here with a dead address escape; the real reason is unknown.
 * Remaining: the six const-ref temporaries for operator/ are allocated in
 * ASCENDING address order of use here (sp+520..580), DESCENDING in the
 * target (580..520); fab() FP numbering (target sum f0 / abs f12 / zero
 * f14); a few sum-load schedules.
 */
struct V3 {
    float x, y, z;
    V3() {}
    V3(float a, float b, float c) { x = a; y = b; z = c; }
    void set(const V3 &v) { x = v.x; y = v.y; z = v.z; }
    void operator=(V3 v) { set(v); }
};
inline V3 operator+(const V3 &a, const V3 &b) { return V3(a.x + b.x, a.y + b.y, a.z + b.z); }
inline V3 operator/(const V3 &v, float s) { return V3(v.x / s, v.y / s, v.z / s); }
inline float fab(float f) { return f < 0.0f ? -f : f; }
extern "C" {
void gl_func_00000000_5062c_esc(int *, int *, int *, int *);
void gl_func_00000000_5062c_nrm(V3 *, V3 *, V3 *);
int gl_func_00000000_5062c_vtx(int, int, V3 *, V3 *, int);
void gl_func_00000000_5062c_tri(int, int, int, int, int, int);
int gl_func_0005062C(int, int, int, int, int, int, int, V3, V3, V3, V3, V3, V3);
}
extern "C" int gl_func_0005062C(int scale, int a1, int level, int a3, int a4, int a5, int a6,
                                V3 v0, V3 v1, V3 v2, V3 t0, V3 t1, V3 t2) {
    V3 m01, m12, m20, u01, u12, u20;
    V3 n0, n1, n2;
    int h0, h1, h2, h3, h4, h5;
    float ax, ay, az;

    if (0) { gl_func_00000000_5062c_esc(&scale, &a1, &level, &a3); }
    gl_func_00000000_5062c_nrm(&v0, &v1, &n0);
    gl_func_00000000_5062c_nrm(&v1, &v2, &n1);
    gl_func_00000000_5062c_nrm(&v2, &v0, &n2);
    ax = fab(n0.x + n1.x + n2.x);
    ay = fab(n0.y + n1.y + n2.y);
    az = fab(n0.z + n1.z + n2.z);
    if ((ax + ay + az) / 2.0f < (float)(scale * 4)) {
        level = 1;
    }
    m01 = (v0 + v1) / 2.0f;
    m12 = (v1 + v2) / 2.0f;
    m20 = (v2 + v0) / 2.0f;
    u01 = (t0 + t1) / 2.0f;
    u12 = (t1 + t2) / 2.0f;
    u20 = (t2 + t0) / 2.0f;
    if (--level != 0) {
        gl_func_0005062C(scale, a1, level, a3, a4, a5, a6, v0, m01, m20, t0, u01, u20);
        gl_func_0005062C(scale, a1, level, a3, a4, a5, a6, m01, v1, m12, u01, t1, u12);
        gl_func_0005062C(scale, a1, level, a3, a4, a5, a6, m20, m12, v2, u20, u12, t2);
        gl_func_0005062C(scale, a1, level, a3, a4, a5, a6, m01, m12, m20, u01, u12, u20);
    } else {
        h0 = gl_func_00000000_5062c_vtx(a1, a3, &v0, &t0, a4);
        h1 = gl_func_00000000_5062c_vtx(a1, a3, &v1, &t1, a4);
        h2 = gl_func_00000000_5062c_vtx(a1, a3, &v2, &t2, a4);
        h3 = gl_func_00000000_5062c_vtx(a1, a3, &m01, &u01, a4);
        h4 = gl_func_00000000_5062c_vtx(a1, a3, &m12, &u12, a4);
        h5 = gl_func_00000000_5062c_vtx(a1, a3, &m20, &u20, a4);
        gl_func_00000000_5062c_tri(a3, a5, h0, h3, h5, a6);
        gl_func_00000000_5062c_tri(a3, a5, h3, h1, h4, a6);
        gl_func_00000000_5062c_tri(a3, a5, h5, h4, h2, a6);
        gl_func_00000000_5062c_tri(a3, a5, h3, h4, h5, a6);
    }
    return 0;
}
