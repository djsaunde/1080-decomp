/* IDO 7.1 NCC (C++) EXACT donor for cam_create (0x328 / 202 words), REPLACE_FUNC_BODY-spliced over the
 * placeholder in bootup_uso.c on both build paths (agent-c run 271; was the _nm comparison donor).
 * agent-g run 164 (docs/IDO_CODEGEN.md kit CIV): the cfe body was C++ decoded as C.
 * `obj = new Obj2` (global inline operator new -> edg keeps the new-temp T and copies it
 * to obj at the join; Obj2 adds 4 bytes to the 0xB4 Obj, so the inlined Obj ctor's
 * this==0 fallback allocates 0xB4 while the new-expression allocates 0xB8), the Obj ctor
 * = out-of-line base B(&D_7460), vptr +0x28, member M at +0x2C with nested N (two
 * protocol tests), fin(this), pos.set(V3(0, 500, 0)); the 0-first-argument calls are
 * out-of-line ctors called by `new` (cfront protocol: ctor(0, args)); case 1 is
 * `new KB(obj, a3v)` (inline ctor over the out-of-line base B2, vptr = D_7120); the tail
 * is an inline P::add(n) twice plus the vec copies, in its own block (puts the
 * V3(0, 0, -1000) temp at the bottom like the target). The `if (1) {}` splits the
 * a3v live unit so obj takes v0 / a3v v1 as in the target.
 * 202/202 words; LEFT: edg frame layout (frame 128 vs 144). Run 165 (kit CV): V3's ctor
 * forwarding to an inline set() plus the Obj ctor's V3 temp moved into an inline pos() put
 * every home from the V3(0, 500, 0) temp down at the target offsets; still missing: a fresh
 * KB new-temp right below Obj-this (ours reuses T's slot) and 3 more words above the V3 temp.
 * Run 170 (kit CXV): four unused block-scoped volatile ints at the head of the inline Obj ctor
 * body (kit CXII item 2) give the target frame (144) and every home except T / KB new-temp
 * (target 0x74 / 0x68, ours 0x70 / 0x74) and the case-0 obj reloads (target obj copy, ours T).
 * -1500.0f = bootup RoData +0x530/+0x534.
 * agent-c run 270: the KB new-temp at the target's 0x68 (M-40). Two pool levers (kit CCCIV): an unused Obj(void *u)
 * parameter fed `this` from Obj2's mem-initialiser takes a pool entry at M-40 BEFORE the member M / N `this` temps
 * (they move to M-44 / M-48, one volatile pad dropped to keep frame 144), and case 1 goes through a two-statement
 * inline mk(o, a) whose parameter temps fill M-28 / M-36 first, so the new-temp lands on the u entry (M-40) instead
 * of edg's Obj2 new-temp T. (mk alone puts the new-temp on the M-this entry and that merged variable is spilled in
 * the M / N protocol; u alone changes nothing.) 99.98 -> 99.995. Left: the KB / B2 `o` argument reads obj's home
 * (sp+0x78) where the target reads T (sp+0x74): mk's `o` temp is T's entry, so T is redefined on that path.
 * agent-c run 271: EXACT -- case 1 through a folded-zero conditional whose DEAD arm holds the dummy pool temp (see
 * case 1), and `if (obj) {}` for the tail's `if (1) {}`. The two -1500.0f literals are the donor's .rodata +0 / +4,
 * renamed by the splice to cam_create_rodata (pinned to the RoData offset 0x530).
 * Symbols renamed after compile (names only, see Makefile). */
/* agent-g run 485: the classes carry their game names (include/game/camera.h, docs/1080_CAMERA.md section 2):
 *   MNode (the out-of-line libgdl matrix-node ctor, name arg) <- LookAtMNode (the "snowlookat" node, 0xB4)
 *   <- LookAtNode (+ chase, its chase component, 0xB8); MCamera <- RiderCam (mode 1); CourseCam (game.uso, modes 0 / 2);
 *   the chase components SnowChaseActorCam / SnowChaseActorLook / SnowChaseActor (game.uso; their last ctor argument
 *   is the parms file); Node::add = Node_addChild (child list +0x10, child->parent +0x14).
 * The renamed classes change only the mangled ctor names (renamed in the Makefile); the bytes are the run-271 match.
 * Bootup Data: 0x7460 "snowlookat", 0x746C "rom/cam_info.bin", 0x7480 "rom/cam_ca.bin", 0x7490 "rom/lookat_ca2.bin",
 * 0x74A4 / 0x74C4 "LookAtChase", 0x74B0 "rom/lookat_ca.bin". */
#define FW(p, o) (*(int *)((char *)(p) + (o)))
#define FF(p, o) (*(float *)((char *)(p) + (o)))
/* SnowBoarder (include/game/snowboarder.h) fields: */
#define RIDER_CAM_SOURCE(r) FW(r, 0x874)    /* camSource: the body a mode-3 (fixed) eye parks at */
#define RIDER_START(r, i) FF(r, 0x3A4 + 4 * (i)) /* startPos */
/* CamMNode fields: */
#define MNODE_LOCAL_POS(n, i) FF(n, 0x60 + 4 * (i)) /* local[3]: the body's own translation */
#define MNODE_LOCAL(n) ((n) + 0x30)         /* local matrix (math_mtxAddTranslation target) */
#define CHASE_NAME(c) FW(c, 0xC)            /* Node name */
#define COURSECAM_EYE_CHASE(c) FW(c, 0x154) /* CourseCam.eyeChase */
#define LOOKAT_CHASE(l) FW(l, 0xB4)         /* LookAtNode.chase */
struct V3 { float x, y, z; void set(float a, float b, float c) { x = a; y = b; z = c; } V3(float a, float b, float c) { set(a, b, c); } };
extern "C" {
void *fn_31c0_alloc(int);
void  fn_31c0_fin(void *);
void  fn_31c0_vec(void *, const V3 &);   /* math_mtxAddTranslation(mtx, &delta) */
void  fn_31c0_link(void *, void *);      /* link(parent, child) */
void  fn_31c0_tail(void *, void *);      /* list append */
extern char sStr_snowlookat_7460[], sStr_rom_cam_info_bin_746C[], sStr_rom_cam_ca_bin_7480[], sStr_LookAtChase_74A4[], sStr_rom_lookat_ca_bin_74B0[], sStr_LookAtChase_74C4[], sStr_rom_lookat_ca2_bin_7490[];
}
inline void *operator new(unsigned n) { return fn_31c0_alloc(n); }
struct N { int v; N() { v = 0; } };
struct M { N n; M() {} };
struct MNode { char p[0x28]; MNode(void *name); virtual void g(); };
struct LookAtMNode : MNode { M m; char rest[0xB4 - 0x30]; LookAtMNode(void *u) : MNode(sStr_snowlookat_7460) { { volatile int q0, q1, q2; } fn_31c0_fin(this); pos(); } void pos() { fn_31c0_vec((char *)this + 0x30, V3(0.0f, 500.0f, 0.0f)); } virtual void g(); };
struct LookAtNode : LookAtMNode { int chase; LookAtNode() : LookAtMNode(this) { LookAtNode *p = this; } }; /* agent-c run 238: the named p is a home between the new-temp and the LookAtNode this temp (kit CCLV) */
struct MCamera { char p[0x28]; MCamera(LookAtMNode *target, float x, float y, float z); virtual void g(); };
struct RiderCam : MCamera { char q[0x14C - 0x2C]; float fov; float baseFov; char *rider; int mode;
    RiderCam(LookAtNode *o, char *a) : MCamera(o, 0.0f, 500.0f, -1500.0f) { mode = 0; rider = a; baseFov = fov; } virtual void g(); };
struct CourseCam { CourseCam(char *rider, LookAtNode *lookat, float x, float y, float z, int pad, char *infoFile); };
struct SnowChaseActorCam { SnowChaseActorCam(char *target, int source, char *parmsFile); };
struct SnowChaseActorLook { SnowChaseActorLook(char *anchor, char *cam, char *parmsFile); };
struct SnowChaseActor { SnowChaseActor(char *target, char *lookBody, char *parmsFile); };
struct Node { char p[0x10]; char list[4];
    void add(char *n) { fn_31c0_tail(list, n); if (FW(n, 0x14) != 0) FW(n, 0x4) = 1; FW(n, 0x14) = (int)this; } };
/* agent-c run 271: a dummy whose parameter temp is allocated in the DEAD arm of a folded-zero conditional (case 1) */
inline char *dm(char *a) { if (0) { a = 0; } return a; }
extern "C" void *cam_create(int a0, int mode, Node *parent, char *a3v, int p4) {
    char *w;
    char *h;
    char *v;
    char *r2;
    char *r3;
    LookAtNode *obj;
    /* agent-c run 241 (kit CCLX): obj is a LookAtNode * (plain `obj = new LookAtNode`, no conversion temp); uses that need no
     * conversion (CourseCam / RiderCam take a LookAtNode *) are copy-propagated to edg's new-temp (the case-0 CourseCam argument reads sp+0x74
     * as in the target), while a DERIVED-TO-BASE conversion on a use blocks the propagation there (the case-0 link /
     * 0xB4 store spelled (LookAtMNode *)obj read obj's own home sp+0x78 as in the target). Replaces the run-237 folded a3v
     * read (which kept every use off the new-temp): 99.975 -> see report. */
    obj = new LookAtNode; /* the look-at point "snowlookat" */
    switch (mode) {
    case 0:
    case 2:
        /* the game.uso course camera (eye) with its eye chase component; the look-at gets a SnowChaseActorLook */
        w = (char *)new CourseCam(a3v, obj, 0.0f, 500.0f, -1500.0f, p4, sStr_rom_cam_info_bin_746C);
        h = w;
        r2 = (char *)new SnowChaseActorCam(a3v, RIDER_CAM_SOURCE(a3v), sStr_rom_cam_ca_bin_7480);
        fn_31c0_link(w, r2);
        COURSECAM_EYE_CHASE(w) = (int)r2;
        r3 = (char *)new SnowChaseActorLook(w, r2, sStr_rom_lookat_ca2_bin_7490);
        fn_31c0_link((LookAtMNode *)obj, r3);
        LOOKAT_CHASE((LookAtMNode *)obj) = (int)r3;
        CHASE_NAME(r3) = (int)sStr_LookAtChase_74A4;
        break;
    case 1:
        /* agent-c run 271 (EXACT): the RiderCam new-temp on the target's fresh pool entry WITHOUT any live write to T's entry.
         * `mode * 0` is a folded zero (no code; uopt deletes the dead arm), but edg allocates dm's parameter temp in the
         * dead arm first and keeps it for the whole full-expression, so the new-temp of the live arm lands past it (sp+0x68)
         * while T (sp+0x74) is never redefined on this path: the RiderCam / MCamera `o` argument copy-propagates obj -> T and reads
         * sp+0x74 like the target. The SnowChaseActor new shares the arm; link / store stay statements (link reads obj's home). */
        (mode * 0) ? (void)dm(a3v) : (void)(h = (char *)new RiderCam(obj, a3v), v = (char *)new SnowChaseActor(a3v, a3v, sStr_rom_lookat_ca_bin_74B0));
        fn_31c0_link(obj, v);
        CHASE_NAME(v) = (int)sStr_LookAtChase_74C4;
        break;
    }
    {
    /* both bodies go under parent and start at the rider's spawn point; the eye then moves 1000 back */
    parent->add(h);
    parent->add((char *)obj);
    MNODE_LOCAL_POS(obj, 0) = RIDER_START(a3v, 0);
    MNODE_LOCAL_POS(obj, 1) = RIDER_START(a3v, 1);
    MNODE_LOCAL_POS(obj, 2) = RIDER_START(a3v, 2);
    MNODE_LOCAL_POS(h, 0) = RIDER_START(a3v, 0);
    if (obj) {} /* run 271: code-free test instead of the run-237 if (1) {} (obj v0 / a3v v1 in the tail as in the target) */
    MNODE_LOCAL_POS(h, 1) = RIDER_START(a3v, 1);
    MNODE_LOCAL_POS(h, 2) = RIDER_START(a3v, 2);
    fn_31c0_vec(MNODE_LOCAL(h), V3(0.0f, 0.0f, -1000.0f));
    }
    return h;
}
