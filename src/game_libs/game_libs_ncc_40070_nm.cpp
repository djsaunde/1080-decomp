/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00040070 (0x234 / 141 words), agent-c run 157.
 * A C++ constructor Obj::Obj(void *a1, float x, float y, float z) : Base(a1): cfront `this == 0 -> new(0x108)`,
 * inline Base ctor (`new(0xB4)` fallback, external BB ctor -- BB itself declares a virtual, which is what puts
 * the vptr at 0x28 -- a W4 member at 0x2C with the 4E584 nested protocol, `gl(this)`, a zeroed float[3] passed
 * to gl(m30, z)), Obj's vptr, a W4f member at 0x104 whose element is a FLOAT zero (that use keeps the FP zero
 * constant live across the member protocol, so uopt rematerialises `mtc1 zero` after every call there like the
 * target; with an int element the zero is materialised once at the join, two words short), and the Vec3 copies
 * as `set(V3 v)` with a BY-VALUE parameter (the target's int-word copy into a stack temp, then float stores).
 * cfe 95.03 -> 99.28, length = target. Run 158: the 0xCC..0xD8 block is a Q4 (V3 + w, the rotation) and the
 * 0xF4 copy is `f4.set(rq)` through `Q4::set(const Q4 &)` forwarding to the by-value V3 set (the target's
 * ft0 for the w load), 99.28 -> 99.35; the zeroed float[3] lives in an inline `zero()` (z homes at the
 * target's M-20..M-28). Residual: frame 0x50 vs 0x70 (four more homes between z and the V3 temp, four below)
 * and the 0x104 element store (`sw zero` in the target, swc1 here; an int element loses the FP-zero remats:
 * uopt then splits the zero constant's live range, kit CVI). Still NON_MATCHING: build/non_matching/ only. */
typedef int s32;
extern "C" {
void *gl_func_00034458(...);
}
inline void *operator new(unsigned n) { return gl_func_00034458(n); }
struct M4 { int a; M4() { a = 0; } };
struct W4 { M4 m; W4() {} };
struct M4f { float a; M4f() { a = 0.0f; } };
struct W4f { M4f m; W4f() {} };
struct V3 { float x, y, z; void set(V3 v) { x = v.x; y = v.y; z = v.z; } };
struct Q4 { V3 v; float w; void set(const Q4 &q) { v.set(q.v); w = q.w; } };
struct BB { int pad0[2]; int flags; int pad[7]; BB(void *a1); virtual void f(); };
struct Base : BB {
    W4 m2c;
    char m30[0xB4 - 0x30];
    Base(void *a1) : BB(a1) {
        gl_func_00034458(this);
        zero();
    }
    void zero() { float z[3]; z[0] = 0.0f; z[1] = 0.0f; z[2] = 0.0f; gl_func_00034458(m30, z); }
    virtual void f();
};
struct Obj : Base {
    V3 b4, c0;
    Q4 rq;
    V3 dc, e8;
    Q4 f4;
    W4f m104;
    Obj(void *a1, float x, float y, float zz);
    virtual void f();
};
Obj::Obj(void *a1, float x, float y, float zz) : Base(a1) {
    rq.v.x = 0.0f;
    flags |= 0x20000;
    rq.v.y = 0.0f;
    rq.v.z = 0.0f;
    rq.w = 1.0f;
    c0.x = 1.0f;
    c0.y = 1.0f;
    c0.z = 1.0f;
    b4.x = x;
    b4.y = y;
    b4.z = zz;
    dc.set(b4);
    e8.set(c0);
    f4.set(rq);
    gl_func_00034458(&dc, (char *)this + 0x70);
}
