/* IDO 7.1 NCC (C++) body for gl_func_0005062C (0xAC4 / 689 words) -- NON_MATCHING.
 *
 * Spliced into the NON_MATCHING post0b object only (REPLACE_FUNC_BODY on
 * build/non_matching/..., like the 65EE4 NM donor); the ROM keeps INCLUDE_ASM.
 * All 689 instructions are identical to the target once stack offsets are
 * masked; the only residual is ONE 4-byte edg pool slot: the first
 * statement's operator+ ctor temporary sits at sp+340 (target sp+336), i.e.
 * the target allocates two pool slots between the operator+ return slot
 * (sp+356) and that temporary where this source allocates one (4 words
 * differ: the three swc1 into the temp and its addiu base).
 *
 * Recursive triangle subdivider (agent-g run 114, from the run-113 draft):
 * three normal callbacks, an |n0+n1+n2| / 2 < scale*4 stop test, six edge
 * midpoints, then four recursive calls or six vertex + four triangle emits.
 * What each piece pins (docs/IDO_CODEGEN.md#ncc-5062c-subdivider-run114):
 * - V3(float, float, float) forwards to an inline set(float, float, float):
 *   gives the target FP colouring of every + / divide (x f12, y f0, z f2);
 *   assigning the members in the ctor body does not.
 * - the midpoint sums go through NAMED locals declared first (s1..s6, the
 *   top-of-frame slots 580..520, first declared highest) with the implicit
 *   operator=: (s1 = v0 + v1) / 2.0f. An unnamed const-ref temporary is laid
 *   out in the opposite order.
 * - the store into m01 is a by-value member returning V3 & (assign); the
 *   reference return adds the two pool slots below the divide temp.
 * - the abs is a macro, not an inline: an inline float fab() moves the
 *   result from f12 to f2 and adds two pool slots.
 * - float ax/ay/az are declared BEFORE the int handles (their three unused
 *   slots sit between n2 and h0, as in the target).
 * - no dead address escape is needed for the int params: with this shape
 *   uopt keeps them in their arg homes on its own.
 * Callees are blank USO imports (jal 0); the NM object is not linked. */
struct V3 {
    float x, y, z;
    V3() {}
    V3(float a, float b, float c) { set(a, b, c); }
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    void set(const V3 &v) { x = v.x; y = v.y; z = v.z; }
    V3 &assign(V3 v) { set(v); return *this; }
};
inline V3 operator+(const V3 &a, const V3 &b) { return V3(a.x + b.x, a.y + b.y, a.z + b.z); }
inline V3 operator/(const V3 &v, float s) { return V3(v.x / s, v.y / s, v.z / s); }
#define fab(f) ((f) < 0.0f ? -(f) : (f))
extern "C" {
void gl_func_00000000_5062c_nrm(V3 *, V3 *, V3 *);
int gl_func_00000000_5062c_vtx(int, int, V3 *, V3 *, int);
void gl_func_00000000_5062c_tri(int, int, int, int, int, int);
int gl_func_0005062C(int, int, int, int, int, int, int, V3, V3, V3, V3, V3, V3);
}
extern "C" int gl_func_0005062C(int scale, int a1, int level, int a3, int a4, int a5, int a6,
                                V3 v0, V3 v1, V3 v2, V3 t0, V3 t1, V3 t2) {
    V3 s1, s2, s3, s4, s5, s6;
    V3 m01, m12, m20, u01, u12, u20;
    V3 n0, n1, n2;
    float ax, ay, az;
    int h0, h1, h2, h3, h4, h5;

    gl_func_00000000_5062c_nrm(&v0, &v1, &n0);
    gl_func_00000000_5062c_nrm(&v1, &v2, &n1);
    gl_func_00000000_5062c_nrm(&v2, &v0, &n2);
    ax = fab(n0.x + n1.x + n2.x);
    ay = fab(n0.y + n1.y + n2.y);
    az = fab(n0.z + n1.z + n2.z);
    if ((ax + ay + az) / 2.0f < (float)(scale * 4)) {
        level = 1;
    }
    m01.assign((s1 = v0 + v1) / 2.0f);
    m12.assign((s2 = v1 + v2) / 2.0f);
    m20.assign((s3 = v2 + v0) / 2.0f);
    u01.assign((s4 = t0 + t1) / 2.0f);
    u12.assign((s5 = t1 + t2) / 2.0f);
    u20.assign((s6 = t2 + t0) / 2.0f);
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
