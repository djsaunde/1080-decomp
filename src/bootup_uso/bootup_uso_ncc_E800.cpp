/* IDO 7.1 NCC (C++) donor for SnowBoard_ctor (0xBC / 47 words) -- reads three
 * config ints by key, applies them, then installs a handler pointer at
 * stream_readS32_00000008+0x20 and calls the root object's virtual slot 10
 * (vptr +0x28: lh +0x58 / lw +0x5C).
 *
 * C++ (agent-c run 116). Needed beyond the cfe logic:
 * - the root object is &D_00000000 itself and the call is a real virtual call;
 * - each blank first argument is its own symbol (gSysLib_e800_g1..g4, SnowBoard_vtbl_e800_v,
 *   all 0): one shared symbol is CSE'd into an s-reg;
 * - a named `root` local declared first gives frame 0x30 (z/x/y homes
 *   40/36/32, every named edg local takes a slot);
 * - the last config call and the handler store share one source line; on two
 *   lines as1 schedules the handler's addiu after `lui v1`.
 * Spliced into bootup_uso.c.o via REPLACE_FUNC_BODY. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef float f32; typedef double f64;
#include "../../include/gdl/node.h" /* Node.vptr (+0x28) */
#include "../../include/uso_call.h"  /* NCC donors build without -I include */
extern "C" {
void func_00000000(...);
extern char D_00000000;
extern char gSysLib_e800_g1, gSysLib_e800_g2, gSysLib_e800_g3, gSnowBoard_e800_g4, SnowBoard_vtbl_e800_v;
extern char sStr_rom_audio_seq_uso_8D34, sStr_rom_audio_wave_uso_8D48, sStr_rom_audio_bank_uso_8D5C;
extern char stream_readS32_00000008[];
}
#ifdef PORT
#include "port_cfront.h" /* cfront vptr at +0x28, slot 10 (plan 17.8) */
#include "port_game.h"
struct VO { char pad[0x28]; char *vptr; CF_V0(void, f, 10) };
#else
struct VO { char pad[0x28];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4();
    virtual void v5(); virtual void v6(); virtual void v7(); virtual void v8(); virtual void v9();
    virtual void f(); };
#endif
/* The handler store is the app object's vptr: stream_readS32_00000008 + 0x20 relocates against gSnowBoard (bootup
 * Bss 0x58, sym326, the same object as the D_00000000 root) at +0x28, the cfront vptr slot of a Node
 * subclass (uso-data-sites.py sites SnowBoard_ctor; agent-g run 580). The symbol and addend stay the
 * target's: SNOWBOARD_E800 is stream_readS32_00000008 - 8, so &->vptr is stream_readS32_00000008 + 0x20. */
struct AppVptrE800 { char node[0x28]; void *vptr; };
#ifdef PORT /* gSnowBoard is bootup Bss 0x58; the vptr is port_bootup_bss + 0x80 */
#define SNOWBOARD_E800 ((AppVptrE800 *)(port_bootup_bss + 0x58))
/* The root is the same object. Spelled through the same symbol, gcc orders the vptr load after
 * the store (bound only at the asm level, D_00000000 looked like a separate object to gcc). */
#define D_00000000 port_bootup_bss[0x58]
#else
#define SNOWBOARD_E800 ((AppVptrE800 *)(stream_readS32_00000008 - 8))
#endif
extern "C" void SnowBoard_ctor(void) {
    VO *root;
    int z, x, y;
    FCALL(boot_stub_00066C58)();
    z = 0; x = 0; y = 0;
    FCALL(fs_gdlFileInfo)(&gSysLib_e800_g1, &sStr_rom_audio_seq_uso_8D34, &x);
    FCALL(fs_gdlFileInfo)(&gSysLib_e800_g2, &sStr_rom_audio_wave_uso_8D48, &y);
    FCALL(fs_gdlFileInfo)(&gSysLib_e800_g3, &sStr_rom_audio_bank_uso_8D5C, &z);
    FCALL(snd_Init)(x, z, y);
    FCALL(App_ctor)(&gSnowBoard_e800_g4); SNOWBOARD_E800->vptr = &SnowBoard_vtbl_e800_v; /* install the app's vtable */
    root = (VO *)&D_00000000;
    root->f();
}
