/* IDO 7.1 NCC (C++) donor for Mountain_display (0xB8 / 46 words) -- pushes the
 * object's vector and the root transform's scaled vector to a debug/draw
 * sink (stream_readVec3_00000080 + 0x20 is the sink's datum, passed to every call).
 *
 * C++ (agent-c run 116). Under cfe the body capped at 94.1: the two stack
 * vectors and the a2 reload of the homed arg cannot both come out. Under edg:
 * - V3(float, float, float) forwards to an inline set() (the 5062C rule):
 *   gives every FP register and the a0 home / a2 reloads;
 * - b1 is a function-scope local, b2 lives in an inner block: edg allocates
 *   block-scope locals when the block opens, i.e. AFTER the pool slots of
 *   b1's inline ctor, which puts b2 at sp+0x20 under 5 pool slots (target
 *   layout b1 0x40 / b2 0x20; function scope gives b2 0x34);
 * - b2's constructor and the call share ONE source line: on separate lines
 *   uopt schedules the x store before the a0 %lo (one swapped pair); IDO
 *   line boundaries limit the scheduler (PERM_FORCE_SAMELINE effect).
 * docs/IDO_CODEGEN.md#kit-xvi-agent-c-run-116. Spliced into bootup_uso.c.o
 * via REPLACE_FUNC_BODY; bu_485c_root / gGfxLib_485c_g are blank (0) aliases. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
#include "../../include/game/camera.h"
#include "../../include/game/course.h"
#include "../../include/uso_call.h"  /* NCC donors build without -I include */
struct V3 { float x, y, z;
    V3(float a, float b, float c) { set(a, b, c); }
    void set(float a, float b, float c) { x = a; y = b; z = c; } };
extern "C" {
void func_00000000(...);
extern char stream_readVec3_00000080[];
extern char gGfxLib_485c_g[];          /* gGfxLib (blank per-site alias) */
extern CamWindow *bu_485c_root;        /* bootup data sym161: the camera window (as 1A44's D_bu_205CC) */
}
#define STR (stream_readVec3_00000080 + 0x20)
/* gGfxLib.worldScale[i] (GfxLib 0x128, gfxlib.h assert) spelled base + offset: every member form
 * (.worldScale[i], *(float *)&->worldScale[i], a GfxLib * local, base + offsetof) folds the 0x128
 * into the lui / addiu pair or re-bases per load; the target keeps &gGfxLib in v1, 0x128.. in the lwc1s. */
#define GFX_WORLDSCALE(i) (*(float *)(gGfxLib_485c_g + 0x128 + 4 * (i)))
/* scale(scaleXZ, scaleY, scaleXZ), then the translation = camera eye * worldScale + (0, posY, 0). */
extern "C" void Mountain_display(Mountain *a0) {
    MCamera *cam;
    FCALL(math_mtx4Identity)(STR);
    V3 b1(a0->scaleXZ, a0->scaleY, a0->scaleXZ);
    FCALL(math_mtxScaleRows)(STR, &b1);
    cam = bu_485c_root->cam;
    {
        V3 b2(cam->mnode.world[3][0] * GFX_WORLDSCALE(0), cam->mnode.world[3][1] * GFX_WORLDSCALE(1) + a0->posY, cam->mnode.world[3][2] * GFX_WORLDSCALE(2)); FCALL(math_mtxAddTranslation)(STR, &b2);
    }
    FCALL(Node_display)(a0);
}
