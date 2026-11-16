/* IDO 7.1 NCC (C++) donor for SnowBoarder_display (0x4A0 / 296 words) -- rider
 * ground-contact / lighting refresh: asks the context (func_cfa0) for the
 * surface under the rider, flags a change, re-tints from the surface colour
 * and fires the sub-object hooks.
 *
 * C++ (agent-c run 116). The cfe NM capped at 99.03 (54 words off: v-reg
 * swaps around the dispatches and the context temp). A direct NCC port of
 * the same logic was already closer (43); then:
 * - the sub-object hooks are real virtual calls (vptr at +0x28, slot 2:
 *   lh +0x18 / lw +0x1C), `o->f()` instead of a hand-written dispatch;
 * - the two 12-byte copies into st->0x8FC are V3 struct assignments;
 * - CTX is compared `st->0x8DC == CTX` (edg operand order);
 * - the context pointer is one named local whose first def is the call
 *   argument (`func_cfa0(ctx = CTX, ...)`), which colours the post-call
 *   reload a0 like the target;
 * - frame 0x88: every referenced named local takes an edg slot even when it
 *   is register-only; dst / o / vt are the original's (dead here) names and
 *   the dead `= 0` stores keep their three slots.
 * docs/IDO_CODEGEN.md#kit-xvi-agent-c-run-116. Spliced into bootup_uso.c.o
 * via REPLACE_FUNC_BODY; the bu_cfa0_* aliases are pinned in
 * undefined_syms_auto.txt (ctx 0x254 = snd_setPlayer0VolumeLevel_0000023C+0x18, flag 0xE4,
 * k970 0x970, k978 0x978, the per-site blank args a..f and the two callees 0).
 * agent-g run 543: field form over include/game/snowboarder.h. Same bytes; the two position words passed
 * as int arguments read as `*(int *)&st->dnode.vqs.trans[i]`. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
#include "../../include/game/snowboarder.h"
#include "../../include/gdl/window.h" /* Window: drawDistFov / displayMask on the current window */
#include "../../include/uso_call.h"  /* NCC donors build without -I include */
extern "C" {
char *bu_cfa0_probe(char *, int, int, int, float, float *, float);
extern char gGfxLib_cfa0_a, gGfxLib_cfa0_b, gGfxLib_cfa0_c, gGfxLib_cfa0_d, gGfxLib_cfa0_e, gGfxLib_cfa0_f;
extern char *gGfxLib_cfa0_ctx;
extern int gGfxLib_cfa0_flag;
extern float kRo_SnowBoarder_display_970;
extern double kRo_SnowBoarder_display_978;
int func_00000000(...);
void bu_cfa0_v(...);
}
#define CTX gGfxLib_cfa0_ctx
/* CTX = GfxLib.curWindow (gfx +0x254, the alias value 0x254 = snd_setPlayer0VolumeLevel_0000023C + 0x18; GfxLib +0x240 is
 * mainWindow, the Fader_buildTexture root at snd_setPlayer0VolumeLevel_0000023C + 4): a ScreenWindow, whose Window base
 * holds drawDistFov[0] at +0x98 (the class-0 draw distance at this fov, the float compared with the
 * probe distance) and displayMask at +0x108 (passed on, as Window_display passes it to
 * Node_setDisplayMask). agent-g run 580. */
#define CTX_WIN(c) ((Window *)(c))
#ifdef PORT /* cfront layout: explicit vptr, virtuals as slot calls (port_cfront.h, plan 17.8) */
#include "port_cfront.h"
struct VO { char pad[0x28];
    char *vptr; /* cfront vptr */
    CF_V0(void, v0, 0) CF_V0(void, v1, 1) CF_V0(void, f, 2)
};
#else
struct VO { char pad[0x28]; virtual void v0(); virtual void v1(); virtual void f(); };
#endif
struct V3 { float x, y, z; };
#define DISPATCH_CFA0(node) { ((VO *)(node))->f(); }
extern "C" void SnowBoarder_display(SnowBoarder *st) {
    float out;
    volatile int changed;
    char *r;
    char *ctx;
    char *p;
    float c[4];
    char *dst;
    int flg;
    char *o;
    char *vt;
    char *t;
    float dp[3];

    dst = 0; o = 0; vt = 0;
    if ((st->flags & 0x2000) && st->camera == CTX) return;
    r = bu_cfa0_probe(ctx = CTX, 2, *(int *)&st->dnode.vqs.trans[0], *(int *)&st->dnode.vqs.trans[1],
                  st->dnode.vqs.trans[2], &out, 0.0f);
    st->unkA40 = r;
    if (r == 0) return;
    ctx = CTX;
    changed = (CTX_WIN(ctx)->drawDistFov[0] < out) || (ctx != st->camera);
    if ((st->flags & 0x10000) && st->groundSurface == 0x61 &&
        st->board != 9 && st->contactCount != 0) {
        p = (char *)FCALL(gfx_getDdm)(&gGfxLib_cfa0_a, 0xD);
        *(V3 *)st->unk8F4[2] = *(V3 *)p;
        bu_cfa0_v(0x10);
        c[0] = 1.0f; c[1] = 1.0f; c[2] = 1.0f;
        c[3] = kRo_SnowBoarder_display_970;
        bu_cfa0_v(&gGfxLib_cfa0_b, c);
        bu_cfa0_v(0x10);
        DISPATCH_CFA0(st->bodyNodes[1]);
        p = (char *)FCALL(gfx_getDdm)(&gGfxLib_cfa0_c, 6);
        *(V3 *)st->unk8F4[2] = *(V3 *)p;
    }
    if (!changed || !(st->unk8B8 & 4)) {
        flg = st->groundFlags;
        st->unkA44 = 1;
        if ((flg & 2) && (st->flags & 0x1000) && st->onGround) {
            bu_cfa0_v(st->light, &st->groundNormal);
            bu_cfa0_v(st);
            bu_cfa0_v(0x11);
            t = (char *)st->tint;
            bu_cfa0_v(&gGfxLib_cfa0_d, t);
            bu_cfa0_v(st);
            bu_cfa0_v(st);
            gGfxLib_cfa0_flag = 0;
        } else {
          if ((st->unk8B8 & 0xA) && st->onGround && !(flg & 0x400)) {
            bu_cfa0_v(st->light, &st->groundNormal, dp);
            if ((double)(dp[0] * st->groundNormal.x + dp[1] * st->groundNormal.y +
                         dp[2] * st->groundNormal.z) < kRo_SnowBoarder_display_978) {
                float div255 = 255.0f;
                unsigned char cr = st->groundColor[0] / 2;
                unsigned char cg = st->groundColor[1] / 2;
                unsigned char cb = st->groundColor[2] / 2;
                st->shade[0] = (float)(unsigned int)cr / div255;
                st->shade[1] = (float)(unsigned int)cg / div255;
                st->shade[2] = (float)(unsigned int)cb / div255;
                st->shade[3] = 112.0f / div255;
                bu_cfa0_v(&gGfxLib_cfa0_e, st->shade);
                if (st->unk8B8 & 2) {
                    bu_cfa0_v(2);
                    DISPATCH_CFA0(st->unk814);
                    DISPATCH_CFA0(st->bodyNodes[0]);
                }
                if ((st->unk8B8 & 8) && st->contactCount == 0) {
                    bu_cfa0_v(8);
                    DISPATCH_CFA0(st->bodyNodes[1]);
                }
            }
            gGfxLib_cfa0_flag = 0;
          }
          t = (char *)st->tint;
        }
        if (st->unk8B8 & 1) {
            bu_cfa0_v(0x11);
        } else {
            bu_cfa0_v(0x14);
        }
    } else {
        st->unkA44 = 0;
        bu_cfa0_v(0x14);
        t = (char *)st->tint;
    }
    bu_cfa0_v(&gGfxLib_cfa0_f, t);
    bu_cfa0_v(st);
    bu_cfa0_v(CTX_WIN(CTX)->displayMask);
}
