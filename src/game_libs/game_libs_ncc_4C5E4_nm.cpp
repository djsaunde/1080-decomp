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
struct DL { int pad[3]; DLHead *head; char pad10[0x4C - 0x10]; VpHead *vph; };
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
    dl = ((DL **)((char *)sub + idx * 4))[0x198 / 4];
    gl_func_00034458(m15C, dl);
    char *s1;
    if (*(int *)(gl_d_4c5e4_flag + 0x1C4) & 2) {
        int f = ((m104 & 2) != 0) + 0 * (int)(mC0); /* agent-c run 210: folded counted use of the member mC0 (zero code) */
        s1 = (char *)sub->mF0;
        if (f) f = (m78 & 2) != 0;
        float buf[4];
        buf[0] = 0.0f; buf[1] = 0.0f; buf[2] = 0.0f; buf[3] = 0.0f;
        gl_func_00034458(dl, *(int *)gl_d_4c5e4_word, s1, 1, f, buf, 0, 0, 8, 8, 8, m1B0);
    } else {
        s1 = (char *)vt->fn((void *)(vt->off + (int)this)); /* offset first: the target's `addu a0,t7,s0` */
        Sub *sb = sub;
        void *pf = sb->mF0;
        int m4 = (m78 + 0 * mC0) & 4; /* run 205: m78 read at both uses (no named m); folded 0 * mC0 = ring phase */
        int f = (m104 & 2) != 0;
        if (f) f = (m78 & 2) != 0;
        int c0 = mC0, c4 = mC4, b8 = mB8, bc = mBC, sb8 = sb->mB8;
        gl_func_00034458(dl, s1, pf, m4, f, mDC, c0, c4, b8, bc, sb8, m1B0);
    }
    gl_func_00034458(dl);
    gl_func_00034458(m15C);
    gl_func_00034458(m168, dl);
    gl_func_00034458(gl_d_4c5e4_reg + 0x294); if (1) {} /* run 194 (lever-sweep bbsplit) */
    {
    s1 = (char *)dl;
    Sub2 *e = s14c[idx];
    int v = gl_func_00034458(e->m60) + 0 * (int)(idx); /* agent-c run 210: folded counted use of the member idx (zero code) */
    { Gfx *p = &((DL *)s1)->head->buf[((DL *)s1)->head->count++]; p->w0 = 0x06000000; p->w1 = v; }
    s1 = (char *)dl;
#define d ((DL *)s1)
    float fw = (float)x * 2.0f;
    float fx = (float)w;
    float fh = 2.0f * (float)y; /* agent-g run 231 (operand-commute sweep): 88.67 -> 88.69 */
    float fy = (float)h;
    Vp *vp = &d->vph->buf[d->vph->count++];
    vp->vscale[2] = 0x1FF; vp->vscale[3] = 0;
    vp->vtrans[2] = 0x1FF; vp->vtrans[3] = 0;
    vp->vtrans[0] = fx * 4.0f + fw;
    vp->vscale[0] = fw; /* agent-g run 224: [0] before [1] (sweep 88.65 -> 88.67; was the run-194 [1]-first order) */
    vp->vscale[1] = fh;
    vp->vtrans[1] = fy * 4.0f + fh;
    { Gfx *p = &d->head->buf[d->head->count++]; p->w0 = 0x03800010; p->w1 = gl_func_00034458(vp); }
    }
    gl_func_00034458(this);
    gl_func_00034458(gl_d_4c5e4_arg, this);
    *(int *)(gl_d_4c5e4_clr + 0x254) = 0;
}
