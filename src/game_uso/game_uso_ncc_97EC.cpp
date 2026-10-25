/* IDO 7.1 NCC (C++) NON_MATCHING donor for game_uso_func_000097EC (0x39C / 231 words).
 *
 * XZ-plane proximity test: a/b = XZ projections of w->vel and w->other->vel;
 * e = XZ((a - b) * k), f = XZ(other->pos - pos); returns 1 when f projects
 * inside segment e (0 <= dot/|e|^2 < 1) and its perpendicular distance
 * |cross| / |e| is under s->lim.
 *
 * Run 163 (agent-g): the two squared-length sums (a.x*a.x + a.z*a.z and
 * e's) go through an inline dot(const Vec3 &, const Vec3 &): every FP
 * colour now matches (99.15 -> 99.97). Left (6 words): the two temporaries
 * bound to the XZ(const Vec3 &) ctor are homed in the opposite order --
 * e's at sp+180 / f's at sp+204, target 204 / 180. edg prepends each new
 * declaration-bound temporary to the block's list (creation order e-bound,
 * T1 = the materialised a - b, f-bound gives f-bound 204, T1 192, e-bound
 * 180 above e 168 / f 156); the target layout is the append order.
 * The m2 escape (`float *pm = &m2`) gives the target's spill/reload of
 * |e|^2 and the duplicate f.x / f.z loads. Run 146: cr, t and pm declared BEFORE m2
 * put m2's home at the target's sp+32 (was sp+44). Spliced into the NON_MATCHING
 * game_uso object only (docs/MATCHING_WORKFLOW.md#game-libs-ncc-nm-only-donor-run114).
 * Every symbol is extern "C". */
extern "C" void *game_uso_func_055750(unsigned);
extern "C" float game_uso_func_082880(float);
struct Vec3 {
    void *operator new(unsigned n) { return game_uso_func_055750(n); }
    float x, y, z;
    Vec3() {}
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    Vec3(float a, float b, float c) { set(a, b, c); }
    Vec3 operator-(const Vec3 &v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    Vec3 operator*(float s) const { return Vec3(x * s, y * s, z * s); }
};
struct XZ : Vec3 {
    XZ(const Vec3 &v) : Vec3(v.x, 0.0f, v.z) {}
    XZ(float a, float c) : Vec3(a, 0.0f, c) {}
    XZ operator-(const XZ &v) const { return XZ(x - v.x, z - v.z); }
};
inline float dot(const Vec3 &u, const Vec3 &v) { return u.x * v.x + u.z * v.z; }
#define ABS(x) ((x) < 0.0f ? -(x) : (x))
struct W { char pad0[0xB4]; Vec3 pos; char padC0[0x318 - 0xC0]; Vec3 vel; char pad324[0x908 - 0x324]; W *other; };
struct Self { char pad0[0x30]; W *w; char pad34[0xD0 - 0x34]; float k; float lim; };
extern "C" int game_uso_func_000097EC(Self *s) {
    int ret = 0;
    if (s->w->other != 0) {
        XZ a(s->w->vel);
        XZ b(s->w->other->vel);
        if (0.0f < dot(a, a)) {
            XZ e((a - b) * s->k);
            XZ f(s->w->other->pos - s->w->pos);
            float mag = game_uso_func_082880(dot(e, e));
            if (0.0f < mag) {
                float cr, t, *pm;
                float m2 = e.x * e.x + e.z * e.z; pm = &m2;
                cr = ABS(e.z * f.x - e.x * f.z);
                t = (e.x * f.x + e.z * f.z) / *pm;
                if (0.0f <= t && t < 1.0f && cr / mag < s->lim) ret = 1;
            }
        }
    }
    return ret;
}
