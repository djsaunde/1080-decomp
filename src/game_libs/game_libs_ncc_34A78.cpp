#include "../../include/uso_call.h"  /* NCC donors build without -I include */
#include "../../include/gdl/sysapi.h"
/* IDO 7.1 NCC (C++) donor for app_initSystem (0xEC / 59 words): four init
 * callbacks, back-link two root pointers into the record, then (once) install
 * the KSEG1 config block at module data 0x3B8EC.. and register it.
 * The 0x3B8F6 / 0x3B8F7 byte pair is ONE file-local object: as1 stores it under a
 * single shared `lui at` (`li t4,13; li t5,2; sb t5,F7(at); sb t4,F6(at)`) only
 * for the same static symbol, written through two references in one block (plain
 * sF6[0] / sF6[1] stores CSE the base into v0). Every other access keeps its own
 * extern alias (own `lui at` per access). The splice renames the donor's .bss reloc
 * to app_initSystem_bss, pinned to 0x3B8F6 in undefined_syms_auto.txt.
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY (agent-c run 136). */
extern "C" {
extern int gl_func_00000000(...);
extern int gSysApi_34A78_rec, D_34A78_b, D_34A78_c;
extern signed char gl_ref_0003B8F4, gl_ref_0003B8F5, gl_ref_0003B8F8, gl_ref_0003B8F9;
extern int gl_ref_0003B8EC, gl_ref_0003B8F0, gl_ref_0003B8FC, gl_ref_0003B8FC_st, gl_ref_0003B900, gl_ref_0003B904;
}
static signed char sF6[2];
extern "C" void app_initSystem(int a0) {
    CALL(console_createSystemWin)();
    CALL(dbg_startFaultThread)();
    CALL(pad_initGlobal)(a0);
    CALL(sys_pollDevices_00034B64)(a0);
    SYSAPI_AT(gSysApi_34A78_rec)->sprintfFn = &D_34A78_b;        /* sys_sprintf_00061B30 */
    SYSAPI_AT(gSysApi_34A78_rec)->consoleSetOrigin = &D_34A78_c; /* ConsoleDev_setOrigin */
    if (gl_ref_0003B8FC != (int)0xA8000000) {
        gl_ref_0003B8F4 = 3;
        gl_ref_0003B8FC_st = (int)0xA8000000;
        gl_ref_0003B8F5 = 5;
        gl_ref_0003B8F8 = 12;
        { signed char &a = sF6[0]; signed char &b = sF6[1]; a = 13; b = 2; }
        gl_ref_0003B8F9 = 1;
        /* UNNAMED: mid-entry gl_bzero+0xC (libultra bzero entry, 3 insns into the build symbol) */
        gl_ref_0003B900 = 0; gl_func_00000000(&gl_ref_0003B904, 96);
        CALL(gl_osEPiLinkHandle)(&gl_ref_0003B8F0);
    }
    gl_ref_0003B8EC = (int)&gl_ref_0003B8F0;
}
