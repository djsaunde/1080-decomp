/* IDO 7.1 NCC (C++) donor for anim_blendPose (0x344 / 209 words).
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

/* Local C++ views of include/gdl/anim.h's AnimPose / Animator with its field names
 * (agent-b run 489); the member classes (V3 by value, V3c by reference) carry the match. */
struct AnimPose {
    V3 trans;       /* 0x00 */
    V3 scale;       /* 0x0C */
    V3c quat;       /* 0x18 x y z */
    float quatW;    /* 0x24 */
};

struct Pair { V3 v; V3 w; };

struct Listener {   /* the animated node */
    char pad[0x28];
    virtual void m0(); virtual void m1(); virtual void m2(); virtual void m3();
    virtual void m4(); virtual void m5(); virtual void m6(); virtual void m7();
    virtual void m8();
    virtual void notify(void *src);     /* vtable +0x50 / +0x54 (slot 9) */
};

struct Animator {
    char pad0[0x4C];
    int unk4C;          /* 0x4C */
    int unk50;          /* 0x50 */
    Listener *target;   /* 0x54 */
    char pad58[0x28];
    AnimPose cur;       /* 0x80 */
    char padA8[0x28];
    AnimPose dst;       /* 0xD0 */
};

extern "C" {
void gl_func_00000000_62a2c_get(void *, int, void *);           /* anim_unpackKey(animator, record, out) */
void gl_func_00000000_62a2c_lerp(void *, void *, float, int);   /* math_quatSlerp */
}

/* key0 / key1: the AnimNodeKey records of frame0 / frame1 for this node; frac: time - frame0;
 * blendLeft != 0: blend dst from cur toward the keyed pose by blendWeight (docs/1080_GRAPHICS.md 11) */
extern "C" void anim_blendPose(Animator *o, int key0, int key1, float frac, int blendLeft, float blendWeight) {
    AnimPose x;
    float q[4];
    Pair p;

    o->unk50 = 22;
    o->unk4C = 7;
    gl_func_00000000_62a2c_get(o, key0, &x);
    gl_func_00000000_62a2c_get(o, key1, &p.v);
    gl_func_00000000_62a2c_lerp(&x.quat, q, frac, 0);
    x.trans.lerp(p.v, frac);
    if (blendLeft != 0) {
        o->dst = o->cur;
        gl_func_00000000_62a2c_lerp(&o->dst.quat, &x.quat, blendWeight, 0);
        o->dst.trans.lerp(x.trans, blendWeight);
    } else {
        o->dst = x;
    }
    if (blendLeft < 2) {
        o->cur = o->dst;
    }
    o->dst.scale.x = 1.0f;
    o->target->notify(o);
}
