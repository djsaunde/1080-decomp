/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00036F0C (0x3C8 / 242 words), agent-c run 177.
 * Real C++ `void FreeCam::update()` (debug free camera: pad buttons 0x40 / 0x80 / 0x400 build a move vector,
 * accelerating while any component exceeds the 0x19E8 threshold, rotated by the camera matrix; 0x1000 turns
 * yaw / pitch, pitch clamped to +-89, the boom vector (0, 0, dist) rotated by pitch then yaw, added to the target
 * position). Levers: `Cam *volatile c` (the target's memory home reloaded after every store; the 3x3 rotate as
 * an inline Cam::xform reads it once), V3 `operator+=` written `x = x + o.x` (target operand order),
 * `M &m = c->tgt->m` (the target's `addiu v1,v1,48` sub-object base), `Pad *p;` declared between c and v and
 * the move scalars between v and w (= the target's homes c 132 / v 116 / w 92), `pitch = pitch + -sy * 4`.
 * cfe body 91.26 -> 93.16 (972 B vs target 968, cfe 956). Residual: the target computes &w.y / &w.z into
 * a1 / a2 at entry and colours the move scalars f14 / f16 / f12 (ours one register up). Still NON_MATCHING.
 * agent-c run 179: the accel step without the `a` copy (`n = accel + 1; if (accel < 100) accel = n; k = accel / 10.0f`): 93.21 -> 93.22.
 */
typedef float f32; typedef double f64;
extern "C" {
void gl_func_00034458_36F0C(f32, f32 *, f32 *);
extern f64 D_000019E8;
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
void FreeCam::update() {
    Cam *volatile c = cam;
    Pad *p;
    V3 v; /* agent-g run 218: back after p (target homes c -4 / v -20 / w -44, needs the named h below) */
    f32 mx = 0.0f, my = 0.0f, mz;
    V3 w;
    p = ctl->pad;
    v.z = 0.0f; v.y = 0.0f; v.x = 0.0f;
    mz = 0.0f;
    f64 h = 100.0; /* agent-g run 218: a named double step = the target's 8 extra home bytes (frame 128 -> 136 = target): 94.42 -> 94.53 */
    if (p->btn & 0x40) mz += h;
    if (p->btn & 0x80) mz -= h;
    if (p->btn & 0x400) { mx = p->sx * 100.0f; my = p->sy * 100.0f; }
    f64 th = 0.8; /* agent-g run 217 (kit CCXIV): RoData 0x19E8 is the double literal 0.8 (named: frame 128 kept): 93.31 -> 94.43 */
    if (fabs_(mx) > th || fabs_(my) > th || fabs_(mz) > th) {
        if (accel < 100) { accel = accel + 1; } /* agent-c run 180: n de-named (lever-sweep) */
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
    pitch = pitch < -89.0f ? -89.0f : pitch > 89.0f ? 89.0f : pitch;
    w.y = 0.0f; w.x = 0.0f; w.z = dist;
    gl_func_00034458_36F0C(pitch, &w.y, &w.z);
    gl_func_00034458_36F0C(yaw, &w.x, &w.z);
    M &m = c->tgt->m;
    pos.x = m.t.x; pos.y = m.t.y; pos.z = m.t.z;
    pos += v;
    w += pos;
    c->pos.x = w.x; c->pos.y = w.y; c->pos.z = w.z;
    c->tgt->m.t.x = pos.x; c->tgt->m.t.y = pos.y; c->tgt->m.t.z = pos.z;
}
