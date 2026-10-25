/* IDO 7.1 NCC (C++) donor for timproc_uso_b5_func_0000AC20 (0x534 / 333 words): the timing-screen
 * camera marker update (offset of the object from a fixed point, normalised; when it faces the view
 * axis, the orientation quaternion is rebuilt from three axis-angle rotations, a 0727A4 matrix and a
 * 0714D4 scale, then turned 90 degrees about two of its own axes and spun by the f10C angle).
 * Spliced into timproc_uso_b5.c.o (both trees) via REPLACE_FUNC_BODY. agent-g run 166 (kit CVII):
 * - the reference point is a file-local x / y pair plus a z (as B624's head): the donor's .bss relocs
 *   are renamed timproc_uso_b5_func_0000AC20_bss by the splice and pinned to 0 (the USO bakes the
 *   offsets 0 / 4 / 8); as1 shares the lui of the local x / y pair as in the target, an extern symbol
 *   does not. rel()'s second parameter and the `if (0)` keep the a / g parameter temporaries (the
 *   target's v0 = &o->pos and its frame home);
 * - every o->q.rot() call goes through edg's pooled `this` temporary, which uopt leaves in memory
 *   (the target's sw / lw a0,44(sp));
 * - the 1.0f + f11C * f120 sum is written out (a named product took a uopt colour);
 * - operand order of the axis rows as edg evaluates them, 2.0f as a literal;
 * - the scalar declarations and the nested angle blocks give the target's frame homes. */
typedef float f32; typedef int s32; typedef double f64;
extern "C" {
extern char timproc_uso_b5_D_807FF358, import_80087DA8, D_timb5_AC20_w0, D_timb5_AC20_w1, D_timb5_AC20_w2, D_timb5_AC20_w3;
extern char timproc_uso_b5_D_807FFBB0;
void *timproc_uso_b5_func_071028(...);
void timproc_uso_b5_func_071AE0(void *quat, void *axis, f32 angle);
void timproc_uso_b5_func_0727A4(void *src, void *dst);
void import_0010806C(void *src, void *dst);
void timproc_uso_b5_func_0714D4(void *quat, void *vec, f32 scale, s32 flag);
void timproc_uso_b5_func_06970C(void *obj, f32 val);
void timproc_uso_b5_func_054CAC(void *obj);
}
static float sXY[2]; static float sZ;
struct V3 { f32 x, y, z; V3() {} V3(f32 a, f32 b, f32 c) { x = a; y = b; z = c; } void operator=(V3 o) { x = o.x; y = o.y; z = o.z; } };
inline f32 dot(const V3 &a, const V3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline V3 rel(const V3 &a, f32 *g) { f32 &x = g[0]; f32 &y = g[1]; if (0) { x = a.x; } return V3(a.x - x, a.y - y, a.z - sZ); }
struct Quat {
    f32 x, y, z, w;
    void rot(V3 *axis, f32 ang) { timproc_uso_b5_func_071AE0(this, axis, ang); }
};
struct Obj {
    char pad0[0xDC]; V3 pos; f32 sE8, sEC, sF0; Quat q; char pad104[0x108 - 0x104]; void *m108;
    f32 f10C, f110, f114, f118, f11C, f120, f124; s32 i128, i12C, i130;
};
extern "C" void timproc_uso_b5_func_0000AC20(Obj *o) {
    V3 d = rel(o->pos, &sXY[0]);
    timproc_uso_b5_func_071028(&d);
    if ((f64)dot(*(V3 *)&timproc_uso_b5_D_807FFBB0, d) < *(f64 *)((char *)&timproc_uso_b5_D_807FF358 + 0x338)) {
        f32 wrap;
        f32 sc;
        s32 m7C[13];
        s32 m6C[4];
        f32 t120;
        V3 ax;
        o->q.x = 0.0f; o->q.y = 0.0f; o->q.z = 0.0f; o->q.w = 1.0f;
        { f32 ang = o->f114; V3 a(1.0f, 0.0f, 0.0f); o->q.rot(&a, ang); }
        { f32 ang = o->f110; { V3 a(0.0f, 0.0f, 1.0f); o->q.rot(&a, ang); } ang = o->f118; { V3 a(0.0f, 1.0f, 0.0f); o->q.rot(&a, ang); } }
        timproc_uso_b5_func_0727A4(*(char **)((char *)(*(char **)&import_80087DA8) + 0x70) + 0xB4, m7C);
        import_0010806C(m7C, m6C);
        {
            t120 = o->f120;
            sc = 1.0f;
            o->sE8 = sc + o->f11C * t120; o->sEC = sc + o->f11C * t120; o->sF0 = sc + o->f11C * t120;
            if (o->i12C != 0) sc = t120;
            timproc_uso_b5_func_0714D4(&o->q, m6C, sc, 0);
        }
        ax.x = (o->q.x * o->q.z * 2.0f) + (o->q.w * o->q.y * 2.0f);
        ax.y = (o->q.y * o->q.z * 2.0f) - (o->q.w * o->q.x * 2.0f);
        ax.z = (1.0f - (o->q.x * o->q.x * 2.0f)) - (o->q.y * o->q.y * 2.0f);
        o->q.rot(&ax, 1.5707964f);
        ax.x = (1.0f - (o->q.y * o->q.y * 2.0f)) - (o->q.z * o->q.z * 2.0f);
        ax.y = (o->q.x * o->q.y * 2.0f) + (o->q.w * o->q.z * 2.0f);
        ax.z = (o->q.x * o->q.z * 2.0f) - (o->q.w * o->q.y * 2.0f);
        o->q.rot(&ax, 1.5707964f);
        if (o->i12C != 0 && o->i130 != 0) {
            wrap = *(f32 *)((char *)&D_timb5_AC20_w0 + 0x340);
            o->f10C = o->f10C + *(f32 *)((char *)&D_timb5_AC20_w1 + 0x344);
            if (wrap <= o->f10C) o->f10C = o->f10C - wrap;
            ax.x = (1.0f - (o->q.y * o->q.y * 2.0f)) - (o->q.z * o->q.z * 2.0f);
            ax.y = (o->q.x * o->q.y * 2.0f) + (o->q.w * o->q.z * 2.0f);
            ax.z = (o->q.x * o->q.z * 2.0f) - (o->q.w * o->q.y * 2.0f);
            o->q.rot(&ax, o->f10C);
        } else if (o->f10C > 0.0f) {
            wrap = *(f32 *)((char *)&D_timb5_AC20_w3 + 0x348);
            o->f10C = o->f10C + *(f32 *)((char *)&D_timb5_AC20_w2 + 0x34C);
            if (wrap <= o->f10C) o->f10C = 0.0f;
            ax.x = (1.0f - (o->q.y * o->q.y * 2.0f)) - (o->q.z * o->q.z * 2.0f);
            ax.y = (o->q.x * o->q.y * 2.0f) + (o->q.w * o->q.z * 2.0f);
            ax.z = (o->q.x * o->q.z * 2.0f) - (o->q.w * o->q.y * 2.0f);
            o->q.rot(&ax, o->f10C);
        }
        timproc_uso_b5_func_06970C(o->m108, o->f124);
        timproc_uso_b5_func_054CAC(o);
        o->i128 = 1;
    }
}
