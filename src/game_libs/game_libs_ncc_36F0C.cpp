/* IDO 7.1 NCC (C++) donor for gl_func_00036F0C (0x3C8 / 242 words): EXACT (agent-g run 219).
 * Real C++ `void FreeCam::update()` (debug free camera: pad buttons 0x40 / 0x80 / 0x400 build a move vector,
 * accelerating while any component exceeds the 0.8 threshold (RoData 0x19E8), rotated by the camera matrix;
 * 0x1000 turns yaw / pitch, pitch clamped to +-89, the boom vector (0, 0, dist) rotated by pitch then yaw, added
 * to the target position). Lift agent-c run 177; NM history runs 179-218 (agent-c / agent-g).
 * Levers: c address-taken through a folded `&c == 0` test (memory-resident, reloaded after every aliasing store);
 * V3 `operator+=` written `x = x + o.x`; decl order c / p / v / move scalars / w (= target homes); the pitch clamp
 * as an if / return inline; dist read into a local before the w stores; the tail stores through inline setters
 * whose Cam * parameter is kept (`if (0) { n = 0; }`, the 37348 idiom) = v0 parameter webs.
 * agent-g run 219 (99.73 -> 100): the y / z / target setters take the value by const reference (a by-value float
 * parameter is an f0 variable; the reference reads come out as ugen temps f6 / f10 / f8 / f4 / f6 like the
 * target), and the x store is `n->pos.x = w.x` through `Cam *n = gc0(c)` taken BEFORE `w += pos`: the
 * accessor's kept parameter is a pooled edg temp (v0, no interference with the setter webs) and w.x's add result
 * is forwarded straight into the store (f4). That also gives the target's 8 extra frame bytes, so the run-218
 * named `f64 h = 100.0` step is gone. Calls go through the 0-pinned gl_func_00000000 alias (the ROM has jal 0).
 */
typedef float f32; typedef double f64;
extern "C" {
void gl_func_00000000(f32, f32 *, f32 *);
}
struct V3 { f32 x, y, z;
    void set(const V3 &o) { x = o.x; y = o.y; z = o.z; }
    void operator+=(const V3 &o) { x = x + o.x; y = y + o.y; z = z + o.z; }
};
struct Pad { f32 sx, sy; char pad8[8]; int btn; };
struct Ctl { char pad0[0xB4]; Pad *pad; };
struct M { char pad0[0x30]; V3 t; };
struct Node { char pad0[0x30]; M m; };
struct Cam { char pad0[0x60]; V3 pos; char pad6c[0xB4 - 0x6C]; f32 m[3][4]; char padE4[0xF4 - 0xE4]; Node *tgt;
    void xform(V3 &o, f32 x, f32 y, f32 z) {
        o.x = m[0][0] * x + m[0][1] * y + m[0][2] * z;
        o.y = m[1][0] * x + m[1][1] * y + m[1][2] * z;
        o.z = m[2][0] * x + m[2][1] * y + m[2][2] * z;
    }
};
struct FreeCam {
    char pad0[0x14]; Cam *cam; char pad18[0x2C - 0x18]; f32 dist; f32 pitch; f32 yaw; char pad38[4]; int accel;
    V3 pos; Ctl *ctl;
    void update();
};
static inline f32 fabs_(f32 x) { return x < 0.0f ? -x : x; }
static inline f32 clamp_(f32 x, f32 lo, f32 hi) { if (x < lo) return lo; if (x > hi) return hi; return x; }
static inline void spy(Cam *n, const f32 &v) { if (0) { n = 0; } n->pos.y = v; }
static inline void spz(Cam *n, const f32 &v) { if (0) { n = 0; } n->pos.z = v; }
static inline void tgx(Cam *n, const f32 &v) { if (0) { n = 0; } n->tgt->m.t.x = v; }
static inline void tgy(Cam *n, const f32 &v) { if (0) { n = 0; } n->tgt->m.t.y = v; }
static inline void tgz(Cam *n, const f32 &v) { if (0) { n = 0; } n->tgt->m.t.z = v; }
static inline Cam *gc0(Cam *n) { if (0) { n = 0; } return n; }
static inline M &tm(Cam *n) { if (0) { n = 0; } return n->tgt->m; }
void FreeCam::update() {
    Cam *c = cam;
    if (&c == 0) return;
    Pad *p;
    V3 v;
    f32 mx = 0.0f, my = 0.0f, mz;
    V3 w;
    p = ctl->pad;
    v.x = 0.0f; v.y = 0.0f; v.z = 0.0f;
    mz = 0.0f;
    if (p->btn & 0x40) mz += 100.0;
    if (p->btn & 0x80) mz -= 100.0;
    if (p->btn & 0x400) { mx = p->sx * 100.0f; my = p->sy * 100.0f; }
    f64 th = 0.8;
    if (fabs_(mx) > th || fabs_(my) > th || fabs_(mz) > th) {
        if (accel < 100) { accel = accel + 1; }
        f32 k = accel / 10.0f;
        mx += mx * k; my += my * k; mz += mz * k;
    } else {
        accel = 0;
    }
    c->xform(v, mx, my, -mz);
    if (p->btn & 0x1000) {
        yaw = yaw + p->sx * 4.0f;
        pitch = pitch + -p->sy * 4.0f;
    }
    pitch = clamp_(pitch, -89.0f, 89.0f);
    f32 d = dist;
    w.x = 0.0f; w.y = 0.0f; w.z = d;
    gl_func_00000000(pitch, &w.y, &w.z);
    gl_func_00000000(yaw, &w.x, &w.z);
    M &m = tm(c);
    pos.x = m.t.x; pos.y = m.t.y; pos.z = m.t.z;
    pos += v;
    Cam *n = gc0(c);
    w += pos;
    n->pos.x = w.x; spy(c, w.y); spz(c, w.z);
    tgx(c, pos.x); tgy(c, pos.y); tgz(c, pos.z);
}
