/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_0004C5E4 (0x344 / 209 words), agent-g run 191.
 * The cfe NM body (79.69, 212 words) ported to C++: `void Obj::run()` (renamed with objcopy). Registers this
 * object in the module global, picks its DL from the sub-object's per-index table, issues one of two draw calls
 * (flag 0x1C4 & 2: a zero colour quad; else through the cfront virtual at vtbl+0x60/+0x64), then appends a
 * G_DL packet and a viewport (Vp: scale = w*2, trans = x*4 + w*2) with its G_MOVEMEM packet.
 * Shape levers (standalone reloc-masked compares): the base-0 alias symbols per D access (no &D CSE across the
 * first call), ONE multi-role pointer s1 (draw arg in both arms, then the DL after them: the target's callee-saved
 * s1), the else arm's five stack args as named block locals (uopt t0..t4 like the target, which also takes the
 * ugen ring back to t6..t9 everywhere else), the four viewport floats named before the Vp append, and the tail in
 * its own block (the zero quad buf then sits at the target's frame top).
 * Residual: frame 168 vs 208 (block homes / spill slots 64 / 68 vs 72 / 80), the second 2.0f literal that the
 * target re-materialises (832 vs 836 bytes), the else arm's sub / m78 / F0 order.
 * agent-c run 205: the else arm reads m78 at both uses (no named m): the arm is then word-for-word the target up to
 * registers (`lw a0,120(s0)`, `andi a3,a0,4` in the beqz delay) but the ugen ring shifts by 3 from the first flag
 * test on (78.1); a folded `+ 0 * mC0` in the m4 expression re-phases it (86.87 -> 88.12 local, length unchanged).
 * Left: the 0x148 load colours t4 (target t5) because the mC0 load is CSE-hoisted into t5.
 * Still NON_MATCHING: build/non_matching/ only. */
extern "C" {
int gl_func_00034458(...);
extern char D_00000000[];
extern char gl_d_4c5e4_sh[], gl_d_4c5e4_flag[], gl_d_4c5e4_word[], gl_d_4c5e4_reg[], gl_d_4c5e4_arg[], gl_d_4c5e4_clr[];
}
#define S(o) (D_00000000 + (o))
#define SI(o) (*(int *)S(o))
struct Gfx { unsigned w0, w1; };
struct DLHead { Gfx *buf; int count; };
struct Vp { short vscale[4]; short vtrans[4]; };
struct VpHead { Vp *buf; int count; };
struct DL {
    int pad[3]; DLHead *head; char pad10[0x4C - 0x10]; VpHead *vph;
    /* agent-c run 260: inline appends for the G_DL packet and the Vp (own edg temporaries per expansion; the second
     * G_MOVEMEM packet pointer then colours v1 like the target) */
    Gfx *gnext() { return &head->buf[head->count++]; }
    Vp *vnext() { return &vph->buf[vph->count++]; }
};
struct Sub { char pad[0xB8]; int mB8; char padbc[0xF0 - 0xBC]; void *mF0; char padf4[0x198 - 0xF4]; DL *dl198[1]; };
struct Sub2 { char pad[0x60]; int m60; };
struct VT { char pad[0x60]; short off; short pad62; void *(*fn)(void *); };
struct Obj {
    char pad0[0x28];
    VT *vt;
    char pad2c[0x78 - 0x2C];
    int m78;
    char pad7c[0xAC - 0x7C];
    int mAC, mB0;
    int mB4;
    int mB8, mBC, mC0, mC4;
    int x, y, w, h;      /* C8 CC D0 D4 */
    char padd8[0xDC - 0xD8];
    char mDC[0x104 - 0xDC];
    int m104;
    char pad108[0x144 - 0x108];
    int idx;            /* 144 */
    Sub *sub;           /* 148 */
    Sub2 *s14c[3];      /* 14C */
    DL *dl;             /* 158 */
    char m15C[0x168 - 0x15C];
    char m168[0x180 - 0x168];
    int a180[0x1B0 / 4 - 0x180 / 4];
    int m1B0;
    int m1B4;
    char pad1b8[0x1C0 - 0x1B8];
    int m1C0;
    void run();
};
void Obj::run() {
    char *g = S(0);
    *(Obj **)(g + 0x254) = this;
    idx = *(int *)(g + 0x204);
    m1C0 = a180[idx];
    *(short *)(gl_d_4c5e4_sh + 0x208) = gl_func_00034458(g, mAC, mB0);
    m1B4 = 0;
    g = m15C; /* agent-g run 260 (kit CCXCIII spilltemps): the m15C address first mentioned before the sub load, so its item takes the first temp (spill 68 -> 72 = target); held in the dead g (a fresh name costs a home) */
    dl = ((DL **)((char *)sub + idx * 4))[0x198 / 4];
    gl_func_00034458(g, dl);
    char *s1;
    if (*(int *)(gl_d_4c5e4_flag + 0x1C4) & 2) {
        int f = ((m104 & 2) != 0) + 0 * (int)(mC0); /* agent-c run 210: folded counted use of the member mC0 (zero code) */
        s1 = (char *)sub->mF0;
        if (f) f = (m78 & 2) != 0;
        float buf[4]; int one = 1; /* agent-c run 232: home-keeping argument locals -> frame 208 / buf 180 = target (88.69 -> 88.72); run 260: e3 dropped (the inline appends' homes take its word); run 261: za / e1 / e2 dropped for the named hh / nn / vh / vn homes */
        buf[0] = 0.0f; buf[1] = 0.0f; buf[2] = 0.0f; buf[3] = 0.0f;
        gl_func_00034458(dl, *(int *)gl_d_4c5e4_word, s1, one, f, buf, 0, 0, 8, 8, 8, m1B0);
    } else {
        s1 = (char *)vt->fn((void *)(vt->off + (int)this)); /* offset first: the target's `addu a0,t7,s0` */
        Sub *sb = sub;
        void *pf = sb->mF0;
        int m4 = m78 & 4; /* run 205: m78 read at both uses (no named m); run 246: the folded 0 * mC0 dropped (c0 loads in the arm, v0, as the target) */
        int f = (m104 & 2) != 0;
        if (f) f = (m78 & 2) != 0;
        int c0 = mC0, c4 = mC4, b8 = mB8, bc = mBC, sb8 = sb->mB8 + 0 * (int)(sub); /* run 246: folded counted use of sub: sb = t5 (target) and the ring phase */
        gl_func_00034458(dl, s1, pf, m4, f, mDC, c0, c4, b8, bc, sb8, m1B0);
    }
    gl_func_00034458(dl);
    gl_func_00034458(m15C);
    gl_func_00034458(m168, dl);
    gl_func_00034458(gl_d_4c5e4_reg + 0x294); if (1) {} /* run 194 (lever-sweep bbsplit) */
    {
    /* agent-c run 261 (kit CCXCIX): the G_DL / G_MOVEMEM appends share ONE named head (hh) and ONE named count
     * (nn) -- the target's a2 / a1 in both -- and the Vp append has its own named vh / vn (v1 / t0); folded uses
     * of nn and vh raise their uopt priorities above hh's, so hh is coloured after v1 / a0 / a1 are taken
     * (99.59 -> 99.95) */
    s1 = (char *)dl; DLHead *hh; int nn;
    Sub2 *e = s14c[idx];
    int v = gl_func_00034458(e->m60) + 0 * (int)(idx); /* agent-c run 210: folded counted use of the member idx (zero code) */
    { hh = ((DL *)s1)->head; nn = hh->count++; Gfx *p = &((DL *)s1)->head->buf[nn]; p->w0 = 0x06000000; p->w1 = v + 0 * nn; }
    s1 = (char *)dl;
#define d ((DL *)s1)
    /* agent-c run 260: the four int -> float conversions as named locals BEFORE the Vp append, the scale products
     * after it: the append's count store blocks copy propagation, so cw / ch / cy / cx are uopt-coloured LRs
     * (target f12 / f14 / f16 / f0) and the 2.0f constant loses its colour -> re-loaded per use like the target
     * (832 -> 836 bytes, 89.59 -> 99.48) */
    float cw = (float)w;
    float ch = (float)h;
    float cx = (float)x;
    float cy = (float)y;
    VpHead *vh = d->vph; int vn = vh->count; vh->count = vn + 1; Vp *vp = &d->vph->buf[vn + 0 * (int)vh];
    vp->vscale[2] = 0x1FF; vp->vscale[3] = 0;
    vp->vtrans[2] = 0x1FF; vp->vtrans[3] = 0;
    vp->vscale[0] = cx * 2.0f;
    if (1) {}
    vp->vscale[1] = cy * 2.0f;
    vp->vtrans[0] = cw * 4.0f + cx * 2.0f;
    vp->vtrans[1] = ch * 4.0f + cy * 2.0f;
    { hh = d->head; nn = hh->count++; Gfx *p = &d->head->buf[nn]; unsigned c3 = 0x03800010; p->w0 = c3; p->w1 = gl_func_00034458(vp); } /* agent-c run 232: used c3 below p -> p home sp+0x50 = target (88.72 -> 88.73) */
    }
    gl_func_00034458(this);
    gl_func_00034458(gl_d_4c5e4_arg, this);
    *(int *)(gl_d_4c5e4_clr + 0x254) = 0;
}
