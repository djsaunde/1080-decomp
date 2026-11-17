/* IDO 7.1 NCC (C++) donor for ScreenThumbMat_grab_0000E740 (0xB4 / 45 words) -- copies a
 * 64x32 u16 tile out of the root object's buffer (every 4th sample) into
 * self->pix.
 *
 * C++ (agent-c run 116): the buffer comes from a virtual getter on the root
 * object at snd_setPlayer0VolumeLevel_0000023C+4 (vptr +0x28, slot 11: lh +0x60 / lw +0x64). With
 * the source pointer stepped per element (`*dst++ = *src; src += 4;`, then
 * `src += 0x540` per row) IDO unrolls the inner loop x4 into the target's
 * 0/8/16/24 offsets with one `addiu v1,v1,32`. The cfe NM indexed src[4*k]
 * by hand and scored 26.4. Spliced into bootup_uso.c.o via REPLACE_FUNC_BODY;
 * the root reference is snd_setPlayer0VolumeLevel_0000023C itself (same reloc as the target). */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef float f32; typedef double f64;
#include "../../include/gdl/gfxlib.h"  /* GfxLib.mainWindow (NCC donors build without -I include) */
extern "C" {
extern char snd_setPlayer0VolumeLevel_0000023C[];
}
/* snd_setPlayer0VolumeLevel_0000023C + 4 is GfxLib +0x240 = mainWindow (uso-data-sites: Bss+0x3C168 sym154, K = 0x240; agent-g
 * run 580), so the root object is the main ScreenWindow and buf() its frame buffer. */
#ifdef PORT /* plan 17.8: on the port the code-symbol token is a function in .text; uso-data-sites.py gives the data: &gGfxLib */
#include "port_game.h"
#define GFX_E740 GFXLIB(PORT_GFXLIB)
#else
#define GFX_E740 GFXLIB(snd_setPlayer0VolumeLevel_0000023C - 0x23C)
#endif
#ifdef PORT /* cfront layout: explicit vptr, virtuals as slot calls (port_cfront.h, plan 17.8) */
#include "port_cfront.h"
struct VO { char pad[0x28];
    char *vptr; /* cfront vptr */
    CF_V0(void, v0, 0) CF_V0(void, v1, 1) CF_V0(void, v2, 2) CF_V0(void, v3, 3) CF_V0(void, v4, 4)
    CF_V0(void, v5, 5) CF_V0(void, v6, 6) CF_V0(void, v7, 7) CF_V0(void, v8, 8) CF_V0(void, v9, 9)
    CF_V0(void, v10, 10) CF_V0(char *, buf, 11)
};
#else
struct VO { char pad[0x28];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4();
    virtual void v5(); virtual void v6(); virtual void v7(); virtual void v8(); virtual void v9();
    virtual void v10(); virtual char *buf(); };
#endif
struct Obj { char pad[0x1C]; unsigned short *pix; };
#ifdef PORT
#include "port_fb.h" /* the framebuffer model (plan 19.10) */
#endif
extern "C" void ScreenThumbMat_grab_0000E740(Obj *self, int a1) {
    /* the 256x160 region at pixel (32, 58) of the 320-wide RGBA5551 frame buffer, sampled every
     * 4th pixel and every 5th row (byte offset 0x9140 = (58 * 320 + 32) * 2) */
    unsigned short *src = (unsigned short *)((VO *)GFX_E740->mainWindow)->buf() + (58 * 320 + 32);
    unsigned short *dst = self->pix;
#ifdef PORT /* plan 19.10: the frame is read back into this image after the next task; the texels are rewritten */
    port_fb_read_color(src - (58 * 320 + 32));
    port_fb_texture_written(self->pix, 64 * 32 * 2);
#endif
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            *dst++ = *src;
            src += 4;
        }
        src += 5 * 320 - 64 * 4; /* 0x540: on to the next sampled row */
    }
}
