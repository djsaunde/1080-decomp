/* IDO 7.1 NCC (C++) donor for Tv1_display (0xDC / 55 words) -- the bootup twin of SNode_display
 * (docs/IDO_CODEGEN.md kit CXXVIII / CXXIX): query the root (G+0x18) through a 7-arg prototyped call; on
 * success hand the global block and s0+0x2C to a callee, scale the camera vector by the block's scale
 * triple (+0x128) through an inline member Cam::mul(G &) called on a local reference `Cam &c = g.cam`, add
 * s0->0x6C to its y, call the second helper on s0 and bump the emit counter.
 * - the camera is a MEMBER of the same block (block+0xA0, vector at +0x30 of it; root at +0x254 -- the ROM
 *   immediates, the expected object's relocation addends read 0x20 / 0x18): `this` (lui v0) is
 *   materialised before the re-materialised block base (lui a2) and each scale load stays after the
 *   previous vector store (may-alias through the reference) -- the two things the cfe body (99.82, a
 *   function-typed vector alias) could not get together;
 * - the stack-argument product is written 0x80 * 0x70 (NCC load order; cfe wanted 0x70 * 0x80).
 * Spliced into bootup_uso.c.o (both trees) via REPLACE_FUNC_BODY over an empty placeholder;
 * gGfxLib_76f4_g / bu_76f4_cnt / func_00000000_76f4_q / func_00000000_76f4_v are blank (0) aliases.
 * agent-g run 555: `self` on envobj.h Tv1 (pos / height / scale / radius / mtx); same bytes. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef float f32; typedef double f64;
#include "../../include/game/course.h"
#include "../../include/game/envobj.h" /* Tv1 (the SNode-layout instance) */
#include "../../include/uso_call.h"  /* NCC donors build without -I include */
extern "C" {
int func_00000000_76f4_q(void *, int, int, int, float, int, float);
void func_00000000_76f4_v(...);
extern int bu_76f4_cnt;
}
struct V3 { float x, y, z; };
struct G;
struct Cam { char pad[0x30]; V3 v; void mul(G &g); };
struct G { char pad[0xA0]; Cam cam; char padDC[0x128 - 0xDC]; V3 scale; char pad134[0x254 - 0x134]; void *root; };
inline void Cam::mul(G &g) { v.x *= g.scale.x; v.y *= g.scale.y; v.z *= g.scale.z; }
extern "C" G gGfxLib_76f4_g;
extern "C" void Tv1_display(Tv1 *self) {
    if (XCALL(func_00000000_76f4_q, cam_isSphereInView)(gGfxLib_76f4_g.root, 3, *(int *)&self->pos[0], *(int *)&self->pos[1],
                    self->pos[2], 0, self->radius * self->scale) != 0) {
        XCALL(func_00000000_76f4_v, gfx_loadMatrix_00047DD8)(&gGfxLib_76f4_g, self->mtx);
        {
            Cam &c = gGfxLib_76f4_g.cam;
            c.mul(gGfxLib_76f4_g);
            c.v.y += self->height;
        }
        XCALL(func_00000000_76f4_v, Node_display)(self);
        bu_76f4_cnt += 1;
    }
}
