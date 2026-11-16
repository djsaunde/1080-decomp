/* IDO 7.1 NCC (C++) donor for SnowBoarder_reset (0x1F8 / 126 words) -- per-round
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
 * jal 0).
 * agent-g run 558: the HANDICAP printf arg is the Node name (NODE(st)->name, gdl/node.h); the board index is
 * RIDER(st)->board. Same bytes. */
struct V3 { float x, y, z;
    void set(V3 v) { x = v.x; y = v.y; z = v.z; }
    V3 &operator=(const V3 &v) { set(v); return *this; } };
extern "C" {
void func_00000000(...);
extern int gPlayerBinPaths_cae8_ta[], gBoardBinPaths_cae8_tb[];
extern int gPolyFlags_3EEC4_cae8_x_ld, gPolyFlags_3EEC4_cae8_x_st, bu_cae8_y;
extern int gInputMgr_cae8_mode;
extern float bu_cae8_k958;
extern char sFmt_HANDICAP_ON_8ABC[];
}
typedef float f32; typedef int s32; typedef short s16; typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;
#include "../../include/game/snowboarder.h"
#include "../../include/gdl/node.h" /* NODE(p)->name (the DNode mirror has no Node fields) */
#define RIDER(st) ((SnowBoarder *)(st))
extern "C" void SnowBoarder_reset(char *st) {
    func_00000000(st, 1, gPlayerBinPaths_cae8_ta[RIDER(st)->character]);
    func_00000000(st + 0x3EC, 1, gBoardBinPaths_cae8_tb[((short *)(st + 0x8C6))[RIDER(st)->board]]);
    if (RIDER(st)->flags & SNOWB_FLAG_MIRROR) func_00000000(st);
    func_00000000(st + 0x90C, (st + 0x17C) - st);
    RIDER(st)->groundDist = bu_cae8_k958;
    RIDER(st)->maxSpeed = RIDER(st)->parms.max.value;
    int mode = gInputMgr_cae8_mode;
    switch (mode) {
    case 2: RIDER(st)->health = 0x5A; break;
    case 3:
    case 4: RIDER(st)->health = 0x4B; break;
    default: RIDER(st)->health = 0xF4240; break;
    }
    RIDER(st)->shade[3] = 0.0f;
    RIDER(st)->shade[2] = 0.0f;
    RIDER(st)->shade[1] = 0.0f;
    RIDER(st)->shade[0] = 0.0f;
    RIDER(st)->riding = 1;
    RIDER(st)->raceResult = 0;
    gPolyFlags_3EEC4_cae8_x_st = gPolyFlags_3EEC4_cae8_x_ld & ~8;
    RIDER(st)->localUp.z = 1.0f;
    bu_cae8_y = 0;
    RIDER(st)->flags = RIDER(st)->initFlags;
    if (RIDER(st)->initFlags & SNOWB_FLAG_20) func_00000000(st);
    func_00000000(st, RIDER(st)->dnode.unk3A4); /* reset to the spawn point (startPos) */
    func_00000000(st);
    func_00000000(RIDER(st)->anim);
    func_00000000(RIDER(st)->ai);
    if (RIDER(st)->flags & SNOWB_FLAG_CATCHUP) func_00000000(sFmt_HANDICAP_ON_8ABC, NODE(st)->name);
    V3 *p = (V3 *)&RIDER(st)->prevPos; V3 *q = (V3 *)&RIDER(st)->prevVel;
    *p = *(V3 *)RIDER(st)->dnode.vqs.trans; /* prevPos = pos */
    *q = *(V3 *)RIDER(st)->dnode.vel;       /* prevVel = vel */
}
