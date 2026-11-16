/* port/sysapi.c: the kernel's SysApi table on the PC port (docs/1080_PC_PORT_PLAN.md 17.8).
 *
 * On the N64 the kernel boots bootup.uso through uso_callEntry(&gUsoCtx), and boot_entry stores
 * that pointer in gSysApi (bootup Bss 0x416A0): the SysApi table IS the kernel's loader context
 * gUsoCtx (include/uso.h UsoCtx; uso_init in kernel_000_1594.c fills it).  The port has no
 * kernel and starts at boot_main, so port_game_boot calls port_sysapi_install() first: it fills
 * a table with the same layout and stores its address in gSysApi.
 *
 * Slot by slot (SysApi = include/gdl/sysapi.h, UsoCtx = include/uso.h):
 *   +0x00 memStart   heapCur after bootup.uso loaded: here a host arena.
 *   +0x04 memEnd     UsoCtx.heapLeft: a SIZE, not an end (App_main: sys_getRegion(&a, &b);
 *                    Mem_init(heap, a, a + b)).  N64 value: 0x80400000 - 0x8001FFA0 (mainuso
 *                    heap start) - bootup.uso's allocations (0x13D8B8) = 0x2A27A8.  The port's
 *                    arena has that size: the game's main heap gets what it had on the N64 (the
 *                    N64 also loaded the mode USOs' Text / tables into it; the port does not).
 *   +0x08..+0x1C     USO error / warning words: 0 (no load errors).
 *   +0x34 heapNew    uso_alloc until boot_main installs mem_heapNewThunk; nothing calls it on
 *                    the port (the kernel loader was its only caller).
 *   +0x40 usoLoad    port/uso (port_uso_load); a data-only module port/uso does not carry is
 *                    accepted when port/fs has the file (logged once; see sysapi_usoLoad).
 *   +0x44 usoUnload  port/uso.
 *   +0x48 fileInfo   uso_findDataSection   -> port_fs_info (ROM address of the Data payload)
 *   +0x4C fileOpen   fs_openFile           -> port_fs_open
 *   +0x50 fileRead   fs_readChecked        -> port_fs_read
 *   +0x54 fileSeek   fs_seek(f, whence, o) -> port_fs_seek
 *   +0x58 fileClose  fs_closeFile          -> port_fs_close
 *   +0x5C fileError  uso_getErrno          -> port_fs_errno
 *   +0x60 fileRomAddr fs_tell              -> port_fs_tell (ROM address; dma_readAsync source)
 *   +0x70 symLookup  uso_addrToModule: the port has no module Text ranges, so every address is
 *                    reported as ("host", addr): nm of the port binary names it.
 *   +0x74 usoPreLoad uso_clearMemStats (the loader's statistics): a no-op.
 *   +0x80..+0x9C     boot_main / app_initSystem install the game's own hooks; the kernel's
 *                    initial values (fs_romRead, nullPrintf, ...) are never called on the port.
 *   the other UsoCtx hooks (init, callEntry, getModuleName, printVersion, printMemStats,
 *   printModules, printSlots) log when called: no game code is known to call them.
 *
 * File reads copy from the ROM image directly instead of going through +0x80 (the game's
 * dma_readChunked, which the kernel's fs_read used after boot_main): same bytes, no DMA queue
 * traffic. */
#include "gdl/sysapi.h"
#include "fs/port_fs.h"
#include "uso/port_uso.h"
#include "include/port_game.h"
#include "ultra/ultra_int.h"

#define SYSAPI_GSYSAPI_BSS 0x416A0     /* gSysApi: bootup Bss, Sym 1558 (include/gdl/gl_data.h) */
#define PORT_HEAP_SIZE 0x2A27A8        /* the N64's memEnd (a size; see above) */
#define USOCTX_SIZE 0xA4               /* UsoCtx (include/uso.h) */

static int sTable[USOCTX_SIZE / 4];
static char sArena[PORT_HEAP_SIZE] __attribute__((aligned(16)));
static unsigned char sDataModuleLoaded[256];
static int sInstalled;

#define API ((SysApi *)sTable)
#define SLOT(off) (sTable[(off) / 4])

/* ---- USO services ---- */

static int sysapi_usoLoad(int name)
{
    const char *path = (const char *)name;
    const PortFsEntry *e;
    int id = port_uso_load(path);

    if (id != -0x14) {
        return id;
    }
    /* Not a module port/uso carries.  On the N64 a data-only module (audio banks, course and
     * rider data) is a load like any other; the port reads such data through the ROM
     * (fileInfo / dma) or the blobs, so accept the load if the ROM has the file. */
    e = port_fs_lookup(path, 0);
    if (e == 0 || e->id < 0 || e->id >= (int)sizeof(sDataModuleLoaded)) {
        return -0x14;
    }
    if (sDataModuleLoaded[e->id]) {
        return -0x13;
    }
    if (sDataModuleLoaded[e->id] == 0) {
        port_ultra_log("SysApi usoLoad: %s (id %d) has no port module; load accepted, no data", path,
                       e->id);
    }
    sDataModuleLoaded[e->id] = 1;
    return e->id;
}

static int sysapi_usoUnload(int name)
{
    const char *path = (const char *)name;
    const PortFsEntry *e;

    if (port_uso_find(path) != 0) {
        return port_uso_unload(path);
    }
    e = port_fs_lookup(path, 0);
    if (e != 0 && e->id >= 0 && e->id < (int)sizeof(sDataModuleLoaded) && sDataModuleLoaded[e->id]) {
        sDataModuleLoaded[e->id] = 0;
        return 0;
    }
    return -1;
}

static void sysapi_usoPreLoad(int a0, int name)
{
    (void)a0;
    (void)name;
}

static int sysapi_symLookup(int addr, int *name, int *offset)
{
    static const char host[] = "host";

    *name = (int)host;
    *offset = addr;
    return 0;
}

/* ---- file services (the kind-1 "local" files of fs_gdl*) ---- */

static int sysapi_fileInfo(int name, int out)
{
    return port_fs_info((const char *)name, (int *)out);
}

static int sysapi_fileOpen(int name)
{
    return (int)port_fs_open((const char *)name);
}

static int sysapi_fileRead(int handle, int buf, int size)
{
    return port_fs_read((PortFsFile *)handle, (void *)buf, size);
}

/* fs_seek(file, whence, offset).  The game's call site passes only the handle on the N64 (a1 /
 * a2 ride through from fs_gdlSeekFile's own arguments); under PORT fs_gdlSeekFile passes them. */
static int sysapi_fileSeek(int handle, int whence, int offset)
{
    return port_fs_seek((PortFsFile *)handle, whence, offset);
}

static void sysapi_fileClose(int handle, int gdlFile)
{
    (void)gdlFile;
    port_fs_close((PortFsFile *)handle);
}

static int sysapi_fileError(int handle)
{
    (void)handle;
    return port_fs_errno();
}

static int sysapi_fileRomAddr(int handle)
{
    return port_fs_tell((PortFsFile *)handle);
}

/* ---- kernel hooks no game code is known to call: log ---- */

static void *sysapi_heapNew(int size, int align)
{
    port_ultra_log("SysApi heapNew(0x%X, %d) before boot_main installed the game's", size, align);
    return 0;
}

#define UNUSED_HOOK(off, what)                                         \
    static int sysapi_unused_##off(void)                               \
    {                                                                  \
        port_ultra_log("SysApi +0x%X (%s) called: not implemented", off, what); \
        return 0;                                                      \
    }
UNUSED_HOOK(0x3C, "uso_init")
UNUSED_HOOK(0x64, "uso_callEntry")
UNUSED_HOOK(0x68, "uso_getModuleName")
UNUSED_HOOK(0x6C, "uso_printVersion")
UNUSED_HOOK(0x78, "uso_printMemStats")
UNUSED_HOOK(0x7C, "uso_printModules")
UNUSED_HOOK(0x80, "fs_romRead")
UNUSED_HOOK(0xA0, "uso_printSlots")

static void sysapi_nullPrintf(int fmt, ...)
{
    (void)fmt;
}

static void sysapi_nop(int a0, int a1)
{
    (void)a0;
    (void)a1;
}

static float sysapi_zeroF(void)
{
    return 0.0f;
}

/* ---- install ---- */

void port_sysapi_install(void)
{
    int n;

    if (!sInstalled) {
        n = port_fs_init(port_ultra_cfg.rom, port_ultra_cfg.romSize);
        if (n <= 0) {
            port_ultra_log("port_sysapi_install: no ROM file system (%d); file services will fail", n);
        }
        sInstalled = 1;
    }
    __builtin_memset(sTable, 0, sizeof(sTable));
    API->memStart = (int)sArena;
    API->memEnd = PORT_HEAP_SIZE;
    API->heapNew = (void *(*)())sysapi_heapNew;
    SLOT(0x30) = (int)sysapi_heapNew;  /* alloc30 */
    SLOT(0x3C) = (int)sysapi_unused_0x3C;
    API->usoLoad = sysapi_usoLoad;
    API->usoUnload = sysapi_usoUnload;
    API->fileInfo = sysapi_fileInfo;
    API->fileOpen = sysapi_fileOpen;
    API->fileRead = sysapi_fileRead;
    API->fileSeek = (int (*)(int))(void (*)(void))sysapi_fileSeek; /* 3 arguments: see sysapi_fileSeek */
    API->fileClose = (void (*)())sysapi_fileClose;
    API->fileError = sysapi_fileError;
    API->fileRomAddr = sysapi_fileRomAddr;
    SLOT(0x64) = (int)sysapi_unused_0x64;
    SLOT(0x68) = (int)sysapi_unused_0x68;
    SLOT(0x6C) = (int)sysapi_unused_0x6C;
    API->symLookup = sysapi_symLookup;
    API->usoPreLoad = sysapi_usoPreLoad;
    SLOT(0x78) = (int)sysapi_unused_0x78;
    SLOT(0x7C) = (int)sysapi_unused_0x7C;
    API->dmaRead = (void *)sysapi_unused_0x80;
    API->printfFn = (void *)sysapi_nullPrintf;
    SLOT(0x88) = (int)sysapi_nullPrintf;
    API->sprintfFn = (void *)sysapi_nullPrintf;
    API->consoleSetOrigin = (void *)sysapi_nop;
    API->timeInFrames = (void *)sysapi_zeroF;
    SLOT(0xA0) = (int)sysapi_unused_0xA0;
    *(SysApi **)(port_bootup_bss + SYSAPI_GSYSAPI_BSS) = API;
}

SysApi *port_sysapi(void)
{
    return API;
}
