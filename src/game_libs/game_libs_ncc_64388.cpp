/* IDO 7.1 NCC (C++) donor for gl_func_00064388 (post1b, 128 words), EXACT.
 * agent-g run 148 (docs/IDO_CODEGEN.md kit LXXIV): the 5062C V3 class with a
 * by-value assign through set(v), a void operator/=, and a named `float k = s`
 * local in operator* reproduce the frame (176), every home and the FP
 * colouring; the references bound before the length test keep the target's
 * materialised addiu bases.
 * agent-g run 163 (kit CII): the last 2 words were the shared operator return
 * slot (sp+116, target sp+108). edg gives an inline function's multi-use
 * parameters temporaries from a per-type pool, freed at the end of the full
 * expression and reused later. operator/= forwarding to dv(*this, *this, s)
 * allocates two V3 pointer temporaries (d, v) above the slot during the
 * first statement; operator-'s a / b then reuse them instead of being
 * allocated below the return slot, so the slot drops to sp+108. */
struct P3 { float x, y, z; };
inline void dv(P3 &d, const P3 &v, float s) { d.x = v.x / s; d.y = v.y / s; d.z = v.z / s; }
struct V3 : P3 {
    V3() {}
    V3(float a, float b, float c) { set(a, b, c); }
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    void set(const V3 &v) { x = v.x; y = v.y; z = v.z; }
    void assign(V3 v) { set(v); }
    void operator/=(float s) { dv(*this, *this, s); }
};
extern "C" {
extern float gl_func_00000000_sq64388(float);
extern void gl_func_00000000_n64388(V3 *);
extern void gl_func_00000000_p64388(char *, V3 *, float);
extern char D_00000000;
}
inline V3 operator-(const V3 &a, const V3 &b) { return V3(a.x - b.x, a.y - b.y, a.z - b.z); }
inline V3 operator*(const V3 &a, float s) { float k = s; return V3(a.x * k, a.y * k, a.z * k); }
struct Obj {
    float len;
    V3 dir;
    V3 v10;
    V3 v1C;
    V3 v28;
};
extern "C" void gl_func_00064388(Obj *o, char *a1, V3 *a2, float a3) {
    V3 m;
    V3 &rv = o->v10;
    V3 g;
    float spd;
    V3 &pv = o->dir;
    V3 &qv = o->v28;
    if (!((double)o->len < *(double *)(&D_00000000 + 0x20C8))) {
        o->dir /= o->len;
        g.assign(pv - qv);
        spd = gl_func_00000000_sq64388(o->v1C.x * o->v1C.x + o->v1C.y * o->v1C.y + o->v1C.z * o->v1C.z)
              / *(float *)(&D_00000000 + 0x20D0) * a3;
        gl_func_00000000_n64388(&o->v1C);
        gl_func_00000000_p64388(a1, &o->v1C, spd);
        m = rv * 1.0f;
        a2->x = a2->x + m.x;
        a2->y = a2->y + m.y;
        a2->z = a2->z + m.z;
    }
}
