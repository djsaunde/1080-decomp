/* IDO 7.1 NCC (C++) NON_MATCHING donor for game_uso_func_000097EC (0x39C / 231 words).
 *
 * XZ-plane proximity test: a/b = XZ projections of w->vel and w->other->vel;
 * e = XZ((a - b) * k), f = XZ(other->pos - pos); returns 1 when f projects
 * inside segment e (0 <= dot/|e|^2 < 1) and its perpendicular distance
 * |cross| / |e| is under s->lim.
 *
 * Every instruction matches the target except: (1) the e statement's two
 * edg temporaries -- the materialised (a - b) and the (a - b) * k result
 * bound to the XZ ctor -- are homed in the opposite order (build 204/192,
 * target 192/204; dp/f as named locals, the C chain 48 -> 140 -> 180 is
 * right); (2) the FP colouring follows from that (x in $f2 vs $f12).
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
#define ABS(x) ((x) < 0.0f ? -(x) : (x))
struct W { char pad0[0xB4]; Vec3 pos; char padC0[0x318 - 0xC0]; Vec3 vel; char pad324[0x908 - 0x324]; W *other; };
struct Self { char pad0[0x30]; W *w; char pad34[0xD0 - 0x34]; float k; float lim; };
extern "C" int game_uso_func_000097EC(Self *s) {
    int ret = 0;
    if (s->w->other != 0) {
        XZ a(s->w->vel);
        XZ b(s->w->other->vel);
        if (0.0f < a.x * a.x + a.z * a.z) {
            XZ e((a - b) * s->k);
            XZ f(s->w->other->pos - s->w->pos);
            float mag = game_uso_func_082880(e.x * e.x + e.z * e.z);
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
