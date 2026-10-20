/* IDO 7.1 NCC (C++) donor for gl_func_00062A2C (0x344 / 209 words).
 *
 * game_libs_post0b holds C++ code too: this camera/transform blend step is
 * the first game_libs function matched under edg (agent-g run 113). The tells
 * were the per-Vec3 "by-value temp" copies (an `addiu vN,sp,K` base, three
 * lw/sw into the stack temp, then lwc1/swc1 per field into the destination)
 * and the virtual call with the s16 this-adjust through vtable +0x50/+0x54.
 * The cfe NM wrap capped at 210/209 words with FP colouring residuals.
 *
 * What the original source must have looked like (each item pins frame slots
 * or registers; see docs/IDO_CODEGEN.md#ncc-game-libs-vec3-62a2c):
 * - V3::operator=(V3 v) takes its argument BY VALUE (the stack temp) and
 *   forwards to an inline set(const V3 &) (two more inline locals per copy:
 *   the 32-byte spacing of the temps at sp+100/68/36);
 * - the third member of the record is a different class whose operator=
 *   takes a const reference and forwards to set(V3c) by value, so its temp
 *   is declared after the inline locals (sp+36, lowest in the frame);
 * - the record copies are plain `o->d = o->s;` assignments (the implicit
 *   Rec::operator= calls the member operators);
 * - the per-component lerp goes through an inline scalar
 *   flerp(float a, const float &b, float t), which keeps the components
 *   sequential (a in $f0, t in $f2, b loaded into the ugen ring);
 * - the second getter output is a 24-byte object (only the first Vec3 used).
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY
 * (docs/MATCHING_WORKFLOW.md#game-uso-ncc-cplusplus-donor-recipe). Every
 * symbol is extern "C"; both callees are blank USO imports (jal 0), wired to
 * zero-valued aliases in undefined_syms_auto.txt. */
inline float flerp(float a, const float &b, float t) { return (b - a) * t + a; }

struct V3 {
    float x, y, z;
    void set(const V3 &v) { x = v.x; y = v.y; z = v.z; }
    void operator=(V3 v) { set(v); }
    void lerp(const V3 &b, float t) { x = flerp(x, b.x, t); y = flerp(y, b.y, t); z = flerp(z, b.z, t); }
};

struct V3c {
    float x, y, z;
    void set(V3c v) { x = v.x; y = v.y; z = v.z; }
    V3c &operator=(const V3c &v) { set(v); return *this; }
};

struct Pose {
    V3 a;           /* 0x00 */
    V3 b;           /* 0x0C */
    V3c c;          /* 0x18 */
    float d;        /* 0x24 */
};

struct Pair { V3 v; V3 w; };

struct Listener {
    char pad[0x28];
    virtual void m0(); virtual void m1(); virtual void m2(); virtual void m3();
    virtual void m4(); virtual void m5(); virtual void m6(); virtual void m7();
    virtual void m8();
    virtual void notify(void *src);     /* vtable +0x50 / +0x54 */
};

struct Blend {
    char pad0[0x4C];
    int w4C;            /* 0x4C */
    int w50;            /* 0x50 */
    Listener *w54;      /* 0x54 */
    char pad58[0x28];
    Pose cur;           /* 0x80 */
    char padA8[0x28];
    Pose dst;           /* 0xD0 */
};

extern "C" {
void gl_func_00000000_62a2c_get(void *, int, void *);
void gl_func_00000000_62a2c_lerp(void *, void *, float, int);
}

extern "C" void gl_func_00062A2C(Blend *o, int from, int to, float t, int mode, float t2) {
    Pose x;
    float q[4];
    Pair p;

    o->w50 = 22;
    o->w4C = 7;
    gl_func_00000000_62a2c_get(o, from, &x);
    gl_func_00000000_62a2c_get(o, to, &p.v);
    gl_func_00000000_62a2c_lerp(&x.c, q, t, 0);
    x.a.lerp(p.v, t);
    if (mode != 0) {
        o->dst = o->cur;
        gl_func_00000000_62a2c_lerp(&o->dst.c, &x.c, t2, 0);
        o->dst.a.lerp(x.a, t2);
    } else {
        o->dst = x;
    }
    if (mode < 2) {
        o->cur = o->dst;
    }
    o->dst.b.x = 1.0f;
    o->w54->notify(o);
}
