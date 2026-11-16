/* IDO 7.1 cfe -O2 donor for uso_init (0x220 / 136 words): USO loader
 * init (see the placeholder in kernel_000.c). Kept out of C_FILES (Makefile).
 *
 * uso_init(devices, ramSize) installs the USO loader hook table in gUsoCtx
 * (UsoCtx, uso.h; cb held in a3 via the int-cast base), clears the 135
 * gUsoModuleSlots names and the gUsoModules words, then gives each of up to 16
 * FsDevice entries its FsDeviceHeap in gFsDeviceHeaps, failing with
 * gUsoCtx.error = -0x17 when a heap end passes 0x80000000 + ramSize.
 *
 * Run 536 (agent-a): the hook stores keep the int base: CTX(f) is
 * *(s32 *)(cb + offsetof(UsoCtx, f)). A UsoCtx * base (cb->f) colours cb t3/v0
 * instead of a3. The hook values are kernel RUNTIME addresses (link + 0x400),
 * spelled with the pinned func_8000XXXX / D_8000XXXX / label names; the comment
 * on each store names the function.
 *
 * agent-c run 229: the slot-clear loop is an INDEX loop on i (`for (i = 0;
 * i < 135; i++) { ... }` on ONE line). Its i web starts before the
 * fs_initFile + 0x30 address CSE dies, so that address colours a2 (a1 is
 * i's) like the target; the pointer walk (p / q) left it in a1. The one-line
 * body keeps the target's sltu / sw / sb / bnez / addiu order (as1 line ties).
 * The rest is agent-g's / agent-c's run 163-228 NM body. */
typedef signed int s32;
typedef unsigned int u32;
typedef short s16;
typedef unsigned char u8;
#include "uso.h"
extern UsoCtx gUsoCtx;
extern s32 gUsoModules[];
extern char gUsoModuleSlots;
extern FsDeviceHeap gFsDeviceHeaps[];
extern void *gUsoModuleSlotsPtr;
/* The kernel runs at link address + 0x400: the calls and the hook values are
 * runtime addresses, pinned in undefined_syms_auto.txt under these names. */
extern void func_80001584(FsDevice *, u32);   /* uso_clearMemStats */
extern void func_80000874(FsDevice *);        /* fs_setDeviceTable */
extern void func_800018A8(void);              /* uso_scanRom */
extern char func_80001994[], func_80000DEC[], D_80000CF0[], func_80001088[];
extern char D_80001258[], D_8000128C[], D_80000BD4[], D_800016BC[];
extern char D_80001814[], D_80001894[], D_80001870[], D_80001748[];
/* the target's .text-relative (0x80000C5C) and .L800007A8 local-label references, pinned in undefined_syms_auto.txt */
extern char func_80001594_L80000C5C[], func_80001594_L800007A8[];
extern char D_800016AC[], D_80001860[], D_8000189C[];
extern char func_800014A8[], func_800010E8[], func_80000480[];

/* a gUsoCtx field by name, addressed as the int cb + its offset (see the header comment) */
#define CTX(f) (*(s32 *)(cb + (s32)&((UsoCtx *)0)->f))

void uso_init(FsDevice *devices, u32 ramSize) {
    s32 cb;
    s32 i;
    FsDeviceHeap *e;
    u32 lim;

    func_80001584(devices, ramSize);
    gUsoModuleSlotsPtr = &gUsoModuleSlots;
    func_80000874(devices);
    cb = (s32)&gUsoCtx;
    CTX(init) = (s32)func_80001994;                        /* uso_init */
    CTX(load) = (s32)func_80000DEC;                        /* uso_load */
    CTX(error) = 0;
    CTX(unload) = (s32)D_80000CF0;                         /* uso_unload */
    CTX(callEntry) = (s32)func_80001088;                   /* uso_callEntry */
    CTX(getModuleName) = (s32)(func_800010E8 + 0x44);      /* uso_getModuleName */
    CTX(print) = (s32)(func_800014A8 + 0x24);              /* uso_nullPrintf */
    CTX(print88) = (s32)(func_800014A8 + 0x24);            /* uso_nullPrintf */
    CTX(alloc) = (s32)(func_80000480 + 0x30);              /* uso_alloc */
    CTX(printVersion) = (s32)D_80001258;                   /* uso_printVersion */
    CTX(alloc30) = (s32)(func_80000480 + 0x30);            /* uso_alloc */
    CTX(addrToModule) = (s32)D_8000128C;                   /* uso_addrToModule */
    CTX(clearMemStats) = (s32)func_80001584;               /* uso_clearMemStats */
    CTX(printMemStats) = (s32)(func_800014A8 + 0x40);      /* uso_printMemStats */
    CTX(printModules) = (s32)D_80000BD4;                   /* uso_printModules */
    CTX(unk38) = 0;
    CTX(openFile) = (s32)D_800016BC;                       /* fs_openFile */
    CTX(readFile) = (s32)D_80001814;                       /* fs_readChecked */
    CTX(closeFile) = (s32)D_80001894;                      /* fs_closeFile */
    CTX(seekFile) = (s32)D_80001870;                       /* fs_seek */
    CTX(findDataSection) = (s32)D_80001748;                /* uso_findDataSection */
    CTX(printSlots) = (s32)func_80001594_L80000C5C;        /* uso_printSlots */
    CTX(print8C) = (s32)(func_800014A8 + 0x24);            /* uso_nullPrintf */
    CTX(nop90) = (s32)(func_800014A8 + 0x18);              /* uso_nop_800010C0 */
    CTX(romRead) = (s32)func_80001594_L800007A8;           /* fs_romRead */
    CTX(getErrno) = (s32)D_800016AC;                       /* uso_getErrno */
    CTX(tellFile) = (s32)D_80001860;                       /* fs_tell */
    CTX(zeroF94) = (s32)D_8000189C;                        /* uso_returnZeroF_8000149C */
    for (i = 0; i < 135; i++) { gUsoModules[i] = 0; ((UsoModuleSlot *)&gUsoModuleSlots)[i].name[0] = 0; }
    for (i = 0; i < 16; i++) {
        if (devices[i].name == 0) break;
        e = gFsDeviceHeaps + i;
        e->start = devices[i].heapStart; if (1) {}
        if (e->start != 0) {
            e->end = devices[i].heapEnd;
            e->cur = e->start;
            if (ramSize + 0x80000000 < (u32)e->end) {
                lim = e->end;
                CTX(error) = -0x17;
                CTX(errorAddr) = lim;
                break;
            }
        }
    }
    func_800018A8();
}
