/* IDO 7.1 NCC (C++) donor for SnowBoard_ctor (0xBC / 47 words) -- reads three
 * config ints by key, applies them, then installs a handler pointer at
 * func_00000008+0x20 and calls the root object's virtual slot 10
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
extern "C" {
void func_00000000(...);
extern char D_00000000;
extern char gSysLib_e800_g1, gSysLib_e800_g2, gSysLib_e800_g3, gSnowBoard_e800_g4, SnowBoard_vtbl_e800_v;
extern char sStr_rom_audio_seq_uso_8D34, sStr_rom_audio_wave_uso_8D48, sStr_rom_audio_bank_uso_8D5C;
extern char func_00000008[];
}
struct VO { char pad[0x28];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4();
    virtual void v5(); virtual void v6(); virtual void v7(); virtual void v8(); virtual void v9();
    virtual void f(); };
/* The handler store is the app object's vptr: func_00000008 + 0x20 relocates against gSnowBoard (bootup
 * Bss 0x58, sym326, the same object as the D_00000000 root) at +0x28, the cfront vptr slot of a Node
 * subclass (uso-data-sites.py sites SnowBoard_ctor; agent-g run 580). */
struct AppVptrE800 { char node[0x28]; void *vptr; };
#define SNOWBOARD_E800 ((AppVptrE800 *)(func_00000008 - 8))
extern "C" void SnowBoard_ctor(void) {
    VO *root;
    int z, x, y;
    func_00000000();
    z = 0; x = 0; y = 0;
    func_00000000(&gSysLib_e800_g1, &sStr_rom_audio_seq_uso_8D34, &x);
    func_00000000(&gSysLib_e800_g2, &sStr_rom_audio_wave_uso_8D48, &y);
    func_00000000(&gSysLib_e800_g3, &sStr_rom_audio_bank_uso_8D5C, &z);
    func_00000000(x, z, y);
    func_00000000(&gSnowBoard_e800_g4); *(void **)(func_00000008 + 0x20) = &SnowBoard_vtbl_e800_v; /* RAW: donor-mirror: NCC donor vptr store through a data alias (func_00000008 + 0x20) */
    root = (VO *)&D_00000000;
    root->f();
}
