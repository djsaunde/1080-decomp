/* IDO 7.1 NCC (C++) NM-only comparison donor for func_000063B4 (minimap marker projection, 224 words).
 * agent-g run 184 (kit CXXXIII): rewritten as C++ -- the target's `lwc1 180(v0); addiu v0,v0,180` is the matrix
 * this-adjust tell (kit CXXXI): an inline `M3::xf(V3 &)` called twice through `ctx->view->rot`, with ctx loaded
 * once and reloaded from the global block after both calls (the second call's view load comes from the old ctx).
 * Frame: A, B declared first (homes 132 / 120), `Q q` declared at its use AFTER the xf calls -- xf's three
 * register-coloured temporaries then sit above q (the target's 12 unreferenced bytes at sp+108) and q lands on
 * sp+92: length = target (896), standalone 83.05 -> 92.89. LEFT: FP colours (f8/f6 vs f6/f4 in the xf spill
 * shuffle) and the A.z reload into f14 in the first MIN arm.
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
typedef float f32;
extern "C" {
extern char D_00000000;
extern unsigned char bu_63b4_c57c[];
extern float func_00000000_63b4_ff(float);
extern void func_00000000_63b4_fpp(float, float *, float *);
extern void func_00000000_63b4_pq(void *, void *);
extern void func_00000000_63b4_p(void *);
}
struct V3 { f32 x, y, z; };
struct Q { f32 x, y, z, w; Q() {} Q(f32 a, f32 b, f32 c, f32 d) { z = c; y = b; x = a; w = d; } };
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
    f32 inv = func_00000000_63b4_ff((f32)obj->nbc / *(f32 *)(bu_63b4_c57c + 0x34));
    A.y = 0.0f;
    A.x = 0.0f;
    A.z = (f32)(-obj->nb8) / inv;
    func_00000000_63b4_fpp((f32)obj->nbc, &A.x, &A.z);
    B.y = 0.0f;
    B.z = A.z;
    B.x = -A.x;
    Ctx *ctx = gG->ctx;
    ctx->view->rot.xf(A);
    ctx->view->rot.xf(B);
    ctx = gG->ctx;
    Rec *r = obj->rr->r;
    f32 fn = (f32)(int)((ctx->view->scale * gG->k - (f32)r->o) / r->d);
    A.z += fn;
    B.z += fn;
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
    Q q;
    q.z = 0.0f;
    q.y = 0.0f;
    q.x = 0.0f;
    q.w = 1.0f;
    func_00000000_63b4_pq(gG, &q);
    func_00000000_63b4_p(obj);
}
