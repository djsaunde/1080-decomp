/* IDO 7.1 NCC (C++) donor for func_0000CAE8 (0x1F8 / 126 words) -- per-round
 * state reset / re-arm on the rider state object.
 *
 * bootup_uso is C++ too (agent-c run 116). The tell was the two Vec3
 * snapshots at the end: `addiu v0,sp,0x24` + three lw/sw into a stack temp,
 * then lwc1/swc1 per field into the destination. The cfe NM wrap was
 * byte-identical except that temp slot (sp+44 vs sp+36). Under edg:
 * - V3::operator=(const V3 &) forwards to an inline set(V3) taking its
 *   argument BY VALUE, which puts the temp lowest in the frame (sp+36; the
 *   62A2C V3c class, docs/IDO_CODEGEN.md#ncc-game-libs-vec3-62a2c);
 * - the named `mode` local and the two named destination pointers p/q each
 *   take a 4-byte edg frame slot even though they are register/folded
 *   (frame 64 -> 80, docs/IDO_CODEGEN.md#ncc-ctor-forwarding-inline-set-frame-b750).
 * Spliced into bootup_uso.c.o via REPLACE_FUNC_BODY. Every symbol is
 * extern "C"; the blank (in-place addend) refs go through the bu_cae8_*
 * aliases in undefined_syms_auto.txt (ta/tb/x/y = 0, mode = 0x34,
 * k958 = 0x958, d8abc = 0x8ABC). Callees are func_00000000 (USO TextReloc
 * jal 0). */
struct V3 { float x, y, z;
    void set(V3 v) { x = v.x; y = v.y; z = v.z; }
    V3 &operator=(const V3 &v) { set(v); return *this; } };
extern "C" {
void func_00000000(...);
extern int bu_cae8_ta[], bu_cae8_tb[];
extern int bu_cae8_x_ld, bu_cae8_x_st, bu_cae8_y;
extern int bu_cae8_mode;
extern float bu_cae8_k958;
extern char bu_cae8_d8abc[];
}
#define I(o) (*(int *)(st + (o)))
#define F(o) (*(float *)(st + (o)))
extern "C" void func_0000CAE8(char *st) {
    func_00000000(st, 1, bu_cae8_ta[I(0x8C4)]);
    func_00000000(st + 0x3EC, 1, bu_cae8_tb[((short *)(st + 0x8C6))[*(short *)(st + 0x904)]]);
    if (I(0xA58) & 0x80) func_00000000(st);
    func_00000000(st + 0x90C, (st + 0x17C) - st);
    F(0x9D0) = bu_cae8_k958;
    F(0xA78) = F(0x768);
    int mode = bu_cae8_mode;
    switch (mode) {
    case 2: I(0xA14) = 0x5A; break;
    case 3:
    case 4: I(0xA14) = 0x4B; break;
    default: I(0xA14) = 0xF4240; break;
    }
    F(0x9F4) = 0.0f;
    F(0x9F0) = 0.0f;
    F(0x9EC) = 0.0f;
    F(0x9E8) = 0.0f;
    I(0x3DC) = 1;
    I(0xA54) = 0;
    bu_cae8_x_st = bu_cae8_x_ld & ~8;
    F(0xA0C) = 1.0f;
    bu_cae8_y = 0;
    I(0xA58) = I(0x8BC);
    if (I(0x8BC) & 0x20) func_00000000(st);
    func_00000000(st, st + 0x3A4);
    func_00000000(st);
    func_00000000(*(char **)(st + 0x840));
    func_00000000(*(char **)(st + 0x804));
    if (I(0xA58) & 0x20000) func_00000000(bu_cae8_d8abc, I(0xC));
    V3 *p = (V3 *)(st + 0x890); V3 *q = (V3 *)(st + 0x89C);
    *p = *(V3 *)(st + 0xB4);
    *q = *(V3 *)(st + 0x318);
}
