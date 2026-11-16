#include "../../include/uso_call.h"  /* NCC donors build without -I include */
/* IDO 7.1 NCC (C++) donor for balloon_uso_func_000001A8 (Balloon display, 0x114), spliced into
 * balloon_uso.c.o via REPLACE_FUNC_BODY (agent-b run 627). It is SNode_display (game_libs 39E24,
 * game_libs_ncc_39E24.cpp) with the bob added: cull through cam_isSphereInView (kind 4), advance
 * the phase by the speed, s = fsin(phase), load the node matrix, scale the camera vector by the
 * GfxLib world scale through the inline Cam::mul on the `Cam &c` reference, then add amp * s (and
 * the SNode height on y). The reference / inline member are the 39E24 levers: a cfe decode folds
 * the GfxLib+0xA0 base into the offsets (lui v1; lwc1 0xD0(v1)) instead of the target's
 * lui/addiu v0,+0xA0; lwc1 0x30(v0). EXACT. */
typedef int s32; typedef unsigned int u32; typedef short s16; typedef unsigned short u16; typedef unsigned char u8; typedef float f32;
#include "../../include/gdl/mnode.h" /* SNode: pos, height */
extern "C" {
int gl_proto_bal1A8(int, int, float, float, float, int, float); /* cam_isSphereInView */
float gl_fsin_bal1A8(float);                                     /* fsin */
void gl_func_00000000_bal1A8v(...);
int gl_func_00000000(...);
}
struct V3 { float x, y, z; };
struct G;
struct Cam { char pad[0x30]; V3 v; void mul(G &g); };
struct G { char pad[0xA0]; Cam cam; char padDC[0x128 - 0xDC]; V3 scale; char pad134[0x254 - 0x134]; int root; };
inline void Cam::mul(G &g) { v.x *= g.scale.x; v.y *= g.scale.y; v.z *= g.scale.z; }
/* Balloon (0x9C): SNode + the hostio "x/y/z" bob amplitude, the bob speed and its phase */
struct Balloon { SNode snode; V3 amp; float speed; float phase; };
extern "C" G gGfxLib_bal1A8_g;
extern "C" void balloon_uso_func_000001A8(Balloon *a0) {
    float s;
    if (XCALL(gl_proto_bal1A8, cam_isSphereInView)(gGfxLib_bal1A8_g.root, 4, a0->snode.pos[0], a0->snode.pos[1],
                                                   a0->snode.pos[2], 0, 0.0f) != 0) {
        a0->phase += a0->speed;
        s = XCALL(gl_fsin_bal1A8, fsin)(a0->phase);
        XCALL(gl_func_00000000_bal1A8v, gfx_loadMatrix_00047DD8)(&gGfxLib_bal1A8_g, (char *)a0 + 0x2C);
        {
            Cam &c = gGfxLib_bal1A8_g.cam;
            c.mul(gGfxLib_bal1A8_g);
            c.v.x += a0->amp.x * s;
            c.v.y += a0->amp.y * s + a0->snode.height;
            c.v.z += a0->amp.z * s;
        }
        CALL(Node_display)(a0);
    }
}
