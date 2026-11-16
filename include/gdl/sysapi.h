#ifndef GDL_SYSAPI_H
#define GDL_SYSAPI_H

/* SysApi: the system table that gSysApi points at (gSysApi = bootup.uso Bss+0x416A0, Sym 1558,
 * a 4-byte POINTER; include/gdl/gl_data.h). boot_entry_00066C30 stores the pointer it is
 * started with; the table itself lives outside bootup.uso (loader / kernel side), so only the
 * slots bootup.uso reads or fills are known. docs/1080_GRAPHICS.md section 19 has the evidence.
 *
 * The loader fills the header words and the +0x40..+0x74 services; boot_main_00066850 installs
 * the game_libs callbacks (+0x34, +0x80..+0x9C) and app_initSystem two more (+0x8C / +0x90).
 * The fs_gdl* functions call the file services for kind-1 ("local", USO-loader) files.
 *
 * Parameters are typed the way the matched call sites pass them (names travel as int).
 * Plain C types only, so the NCC donors can include it. */

typedef struct SysApi {
    /* 0x00 */ int memStart;            /* first free KSEG0 address: sys_setRegion(memStart, memEnd);
                                           "Bootup Code size %dk" = (memStart + 0x80000000) >> 10 */
    /* 0x04 */ int memEnd;
    /* 0x08 */ int usoErrNum;           /* "USO error num %d, data=%x" (fatal) */
    /* 0x0C */ int usoWarnNum;          /* "USO warning num %d, data=%x, %x" */
    /* 0x10 */ int usoErrData;
    /* 0x14 */ int usoWarnData;
    /* 0x18 */ int unk18;
    /* 0x1C */ int usoWarnData2;
    /* 0x20 */ int unk20[5];
    /* 0x34 */ void *(*heapNew)();      /* boot_main: mem_heapNewThunk_00041258 */
    /* 0x38 */ int unk38[2];
    /* 0x40 */ int (*usoLoad)(int name);           /* < 0 = error (app_loadUso, boot_usoLoadFatal) */
    /* 0x44 */ int (*usoUnload)(int name);         /* app_unloadUso: "cant unload %s (%d)" */
    /* 0x48 */ int (*fileInfo)(int name, int out); /* fs_gdlFileInfo: "FileInfo '%s': %d" */
    /* 0x4C */ int (*fileOpen)(int name);          /* fs_gdlOpenFile kind 1: handle, 0 = error */
    /* 0x50 */ int (*fileRead)(int handle, int buf, int size); /* fs_gdlReadFile kind 1 */
    /* 0x54 */ int (*fileSeek)(int handle);        /* fs_gdlSeekFile kind 1 */
    /* 0x58 */ void (*fileClose)();                /* fs_gdlCloseFile kind 1: (handle, file) */
    /* 0x5C */ int (*fileError)(int handle);       /* fs_func_0003593C; the "OpenFile Error" code */
    /* 0x60 */ int (*fileRomAddr)(int handle);     /* fs_gdlReadFileAsync: the dma_readAsync source */
    /* 0x64 */ int unk64[3];
    /* 0x70 */ int (*symLookup)(int addr, int *name, int *offset); /* "(%s,0x%x)": dbg_printThreads
                                                                     (thread PC), dbg_faultThread */
    /* 0x74 */ void (*usoPreLoad)(int a0, int name); /* app_loadUso, before usoLoad(name) */
    /* 0x78 */ int unk78[2];
    /* 0x80 */ void *dmaRead;           /* boot_main: dma_readChunked_00066720 */
    /* 0x84 */ void *printfFn;          /* boot_main: dbg_printf */
    /* 0x88 */ int unk88;
    /* 0x8C */ void *sprintfFn;         /* app_initSystem: sys_sprintf_00061B30 */
    /* 0x90 */ void *consoleSetOrigin;  /* app_initSystem: ConsoleDev_setOrigin */
    /* 0x94 */ void *timeInFrames;      /* boot_main: sys_timeInFrames_00033444 */
    /* 0x98 */ void *tempAlloc;         /* boot_main: heap_tempAllocDefault_000333F4 */
    /* 0x9C */ void *tempFree;          /* boot_main: heap_tempFreeDefault_0003341C */
} SysApi;

/* gSysApi itself (a D_00000000 alias) and the per-site zero-valued aliases
 * (gSysApi_34458_obj, gSysApi_34548_obj, gSysApi_34A78_rec): read the pointer they hold. */
#define SYSAPI_AT(sym) (*(SysApi **)&(sym))
#define SYSAPI SYSAPI_AT(gSysApi)

#ifndef __cplusplus /* edg (NCC donors): &((T *)0)->f is not an integral constant expression */
typedef char SysApi_heapNew_check[(int)&((SysApi *)0)->heapNew == 0x34 ? 1 : -1];
typedef char SysApi_usoLoad_check[(int)&((SysApi *)0)->usoLoad == 0x40 ? 1 : -1];
typedef char SysApi_symLookup_check[(int)&((SysApi *)0)->symLookup == 0x70 ? 1 : -1];
typedef char SysApi_dmaRead_check[(int)&((SysApi *)0)->dmaRead == 0x80 ? 1 : -1];
typedef char SysApi_tempFree_check[(int)&((SysApi *)0)->tempFree == 0x9C ? 1 : -1];
#endif

#endif /* GDL_SYSAPI_H */
