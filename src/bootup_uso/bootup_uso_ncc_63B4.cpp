/* IDO 7.1 NCC (C++) donor for func_000063B4 (minimap marker projection, 0x380 / 224 words), EXACT.
 * agent-g run 184 (docs/IDO_CODEGEN.md kit CXXXIII). The cfe body sat at 83.21 with frame-struct tricks; the
 * target is C++:
 * - the matrix this-adjust tell `lwc1 180(v0); addiu v0,v0,180` (kit CXXXI) = an inline `M3::xf(V3 &)` called
 *   twice through `ctx->view->rot`; ctx is loaded once before the calls and reloaded from the global block
 *   after both (the second call's view load comes from the old ctx, after the first call's stores);
 * - A's three stores through an inline `set()` (x, y, z) and the A.z / B.z accumulates through an inline
 *   `addz()` member (the A.z reload into f14 in the first MIN arm, plus the mtc1-delay fill at the top);
 *   `inv` is not named (de-named, kit CXXIV);
 * - frame: A, B declared first (homes 132 / 120); `Q q` (POD) declared after ctx / r / fn and before t / ix /
 *   iy, so exactly three named scalars sit above it (the target's 12 unreferenced bytes) and q lands on
 *   sp+92; `if (1) {}` before q's stores.
 * Spliced into bootup_uso.c.o (both trees) via REPLACE_FUNC_BODY over an empty placeholder;
 * func_00000000_63b4_* are blank (0) aliases, bu_63b4_c57c = func_0000057C (target reloc func_0000057C + 0x34). */
typedef float f32;
extern "C" {
extern char D_00000000;
extern unsigned char bu_63b4_c57c[];
extern float func_00000000_63b4_ff(float);
extern void func_00000000_63b4_fpp(float, float *, float *);
extern void func_00000000_63b4_pq(void *, void *);
extern void func_00000000_63b4_p(void *);
}
struct V3 {
    f32 x, y, z;
    V3() {}
    void set(f32 a, f32 b, f32 c) { x = a; y = b; z = c; }
    void addz(f32 d) { z += d; }
};
struct Q { f32 x, y, z, w; };
struct M3 {
    f32 m[3][4];
    void xf(V3 &v) const {
        f32 x = m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z;
        f32 y = m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z;
        f32 z = m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z;
        v.x = x; v.y = y; v.z = z;
    }
};
struct View { char p[0xA8]; f32 scale; char pAC[0xB4 - 0xAC]; M3 rot; };
struct Ctx { char p[0x70]; View *view; char p74[0x10C - 0x74]; int px; int py; };
struct G { char p[0x130]; f32 k; char p134[0x254 - 0x134]; Ctx *ctx; };
struct Rec { char p[0xC]; int o; char p10[4]; f32 d; };
struct Rr { char p[0xA0]; Rec *r; };
struct Obj { char p[0xB4]; Rr *rr; int nb8; int nbc; int org; };
#define gG ((G *)&D_00000000)
extern "C" void func_000063B4(Obj *obj) {
    V3 A;
    V3 B;
    A.set(0.0f, 0.0f, (f32)(-obj->nb8) / func_00000000_63b4_ff((f32)obj->nbc / *(f32 *)(bu_63b4_c57c + 0x34)));
    func_00000000_63b4_fpp((f32)obj->nbc, &A.x, &A.z);
    B.y = 0.0f;
    B.x = -A.x;
    B.z = A.z;
    Ctx *ctx = gG->ctx;
    ctx->view->rot.xf(A);
    ctx->view->rot.xf(B);
    ctx = gG->ctx;
    Rec *r = obj->rr->r;
    f32 fn = (f32)(int)((ctx->view->scale * gG->k - (f32)r->o) / r->d);
    A.addz(fn);
    B.addz(fn);
    Q q;
    f32 t;
    int ix, iy;
    t = (A.z < fn) ? A.z : fn;
    ix = (int)t;
    t = ((f32)ix < B.z) ? (f32)ix : B.z;
    ix = (int)t;
    t = (fn < A.z) ? A.z : fn;
    iy = (int)t;
    t = (B.z < (f32)iy) ? (f32)iy : B.z;
    iy = (int)t;
    ix -= obj->org;
    iy += obj->org;
    if (ix < 0) ix = 0;
    if (ix >= 0x80) ix = 0x7F;
    if (iy < 0) iy = 0;
    if (iy >= 0x80) iy = 0x7F;
    ctx->px = ix;
    gG->ctx->py = iy;
    if (1) {}
    q.z = 0.0f;
    q.y = 0.0f;
    q.x = 0.0f;
    q.w = 1.0f;
    func_00000000_63b4_pq(gG, &q);
    func_00000000_63b4_p(obj);
}
