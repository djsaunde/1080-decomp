#ifndef USO_H
#define USO_H

/* Kyoto USO module loader (kernel 0x800000B0-0x800023E0). See
 * docs/USO_FORMAT.md in the monorepo for the file format and loader flow.
 *
 * Section types index the kernel's section-name table (kernel_026.c):
 * the per-slot size / pointer arrays below are indexed by them. */
enum UsoSectionType {
    USO_SEC_INFO = 0,
    USO_SEC_SYM = 1,
    USO_SEC_TEXT_RELOC = 2,
    USO_SEC_DATA_RELOC = 3,
    USO_SEC_RODATA_RELOC = 4,
    USO_SEC_TEXT = 5,
    USO_SEC_DATA = 6,
    USO_SEC_RODATA = 7,
    USO_SEC_BSS = 8,
    USO_SEC_ENTRY_SYM = 9,
    USO_SEC_END_OF_FILE = 10,
    USO_SEC_END = 11,
    USO_SEC_BINARY_DATA = 12,
    USO_SEC_DISK_INFO = 13,
    USO_SEC_COUNT = 14
};

/* One of the 135 module slots gUsoModuleSlots[] (D_800130A0); a loaded
 * module's slot is also linked from gUsoModules[id] (D_80012D60). */
typedef struct UsoModuleSlot {
    /* 0x00 */ s32 secSize[USO_SEC_COUNT]; /* bytes (Sym: entry count) per section type */
    /* 0x38 */ void *secData[USO_SEC_COUNT]; /* loaded section, by type (0x3C = Sym, 0x4C = Text) */
    /* 0x70 */ s16 id;                     /* slot index, set by uso_load */
    /* 0x72 */ char name[0x22];            /* file name, filled by uso_scanRom */
    /* 0x94 */ s32 romOffset;              /* the module's file offset on its device */
    /* 0x98 */ u8 inUse;                   /* uso_allocSlot */
    /* 0x99 */ u8 unk99;                   /* set by uso_initSlot; cleared by uso_bindImports when an import is unresolved */
    /* 0x9A */ u8 pad9A[2];
} UsoModuleSlot; /* size 0x9C */

/* One Sym-section entry (secData[USO_SEC_SYM], secSize = the entry count; 12 bytes, USO_FORMAT.md):
 * uso_callEntry calls entry gUsoEntryIndex's value as the module entry point. */
typedef struct UsoSym {
    /* 0x0 */ s32 name;                    /* the symbol's name */
    /* 0x4 */ s32 value;                   /* its address once loaded */
    /* 0x8 */ s32 size;
} UsoSym; /* size 0xC */

/* compile-time layout checks (IDO cfe accepts negative-size typedefs as errors) */
typedef char UsoModuleSlot_size_check[sizeof(UsoModuleSlot) == 0x9C ? 1 : -1];
typedef char UsoSym_size_check[sizeof(UsoSym) == 0xC ? 1 : -1];
#ifndef __cplusplus /* edg (C++ donors): &((T *)0)->f is not an integral constant expression */
typedef char UsoModuleSlot_secData_check[(int)&((UsoModuleSlot *)0)->secData == 0x38 ? 1 : -1];
typedef char UsoModuleSlot_id_check[(int)&((UsoModuleSlot *)0)->id == 0x70 ? 1 : -1];
typedef char UsoModuleSlot_name_check[(int)&((UsoModuleSlot *)0)->name == 0x72 ? 1 : -1];
typedef char UsoModuleSlot_romOffset_check[(int)&((UsoModuleSlot *)0)->romOffset == 0x94 ? 1 : -1];
typedef char UsoModuleSlot_inUse_check[(int)&((UsoModuleSlot *)0)->inUse == 0x98 ? 1 : -1];
#endif

/* A device-table entry (uso_init's argument, fs_setDeviceTable; the ROM table is
 * k_gFsDevices in kernel_026.c, spelled there as KernelUsoEntry). */
typedef struct FsDevice {
    /* 0x00 */ const char *name;           /* "mainuso", ... (fs_openDevice matches it) */
    /* 0x04 */ u32 romStart;
    /* 0x08 */ u32 romEnd;
    /* 0x0C */ u32 heapStart;              /* RAM heap for the device's modules; 0 = none */
    /* 0x10 */ u32 heapEnd;                /* kernel_boot sets mainuso's to the RAM end */
} FsDevice; /* size 0x14 */

/* gFsDeviceHeaps[16] (D_80012C70): per-device heap range filled by uso_init,
 * returned by fs_openDevice (uso_load reads start / end). */
typedef struct FsDeviceHeap {
    /* 0x00 */ s32 start;
    /* 0x04 */ s32 end;
    /* 0x08 */ s32 cur;                    /* = start at uso_init */
} FsDeviceHeap; /* size 0xC */

/* A loader hook. uso_init stores the kernel's RUNTIME addresses (link address
 * + 0x400), spelled as the pinned func_8000XXXX / D_8000XXXX / label symbols. */
typedef s32 (*UsoHook)();

/* gUsoCtx (D_80012BC0): the loader context. uso_init fills the hook table,
 * uso_callEntry passes &gUsoCtx to a module's entry, uso_getContext returns it.
 * Hook roles from the runtime address each one gets (USO_FORMAT.md section 10). */
typedef struct UsoCtx {
    /* 0x00 */ s32 heapCur;                /* uso_load */
    /* 0x04 */ s32 heapLeft;               /* uso_load */
    /* 0x08 */ s32 error;                  /* uso_init: 0, or -0x17 when a device heap passes the RAM end */
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 errorAddr;              /* uso_init: the heap end that failed */
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 missCount;              /* unresolved imports (uso_bindImports); uso_load clears it */
    /* 0x1C */ s32 unk1C[2];
    /* 0x24 */ s32 entryResult;            /* gUsoEntryResult: uso_callEntry's result */
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s16 unk2C;                  /* uso_load clears it */
    /* 0x2E */ s16 pad2E;
    /* 0x30 */ UsoHook alloc30;            /* uso_alloc */
    /* 0x34 */ UsoHook alloc;              /* gUsoAllocHook: uso_alloc (uso_loadSections, fs_openFile) */
    /* 0x38 */ s32 unk38;
    /* 0x3C */ UsoHook init;               /* uso_init */
    /* 0x40 */ UsoHook load;               /* uso_load */
    /* 0x44 */ UsoHook unload;             /* uso_unload */
    /* 0x48 */ UsoHook findDataSection;    /* uso_findDataSection */
    /* 0x4C */ UsoHook openFile;           /* fs_openFile */
    /* 0x50 */ UsoHook readFile;           /* fs_readChecked */
    /* 0x54 */ UsoHook seekFile;           /* fs_seek */
    /* 0x58 */ UsoHook closeFile;          /* fs_closeFile */
    /* 0x5C */ UsoHook getErrno;           /* uso_getErrno */
    /* 0x60 */ UsoHook tellFile;           /* fs_tell */
    /* 0x64 */ UsoHook callEntry;          /* uso_callEntry */
    /* 0x68 */ UsoHook getModuleName;      /* uso_getModuleName */
    /* 0x6C */ UsoHook printVersion;       /* uso_printVersion */
    /* 0x70 */ UsoHook addrToModule;       /* uso_addrToModule */
    /* 0x74 */ UsoHook clearMemStats;      /* uso_clearMemStats */
    /* 0x78 */ UsoHook printMemStats;      /* uso_printMemStats */
    /* 0x7C */ UsoHook printModules;       /* uso_printModules */
    /* 0x80 */ UsoHook romRead;            /* gUsoReadHook: fs_romRead (PI DMA) */
    /* 0x84 */ UsoHook print;              /* gUsoPrintfHook: uso_nullPrintf */
    /* 0x88 */ UsoHook print88;            /* uso_nullPrintf */
    /* 0x8C */ UsoHook print8C;            /* uso_nullPrintf */
    /* 0x90 */ UsoHook nop90;              /* uso_nop_800010C0 */
    /* 0x94 */ UsoHook zeroF94;            /* uso_returnZeroF_8000149C */
    /* 0x98 */ UsoHook alloc98;            /* not set by uso_init (uso_loadSections calls it) */
    /* 0x9C */ UsoHook final9C;            /* not set by uso_init (uso_loadSections calls it) */
    /* 0xA0 */ UsoHook printSlots;         /* uso_printSlots */
} UsoCtx; /* size 0xA4 (at least) */

typedef char FsDevice_size_check[sizeof(FsDevice) == 0x14 ? 1 : -1];
typedef char FsDeviceHeap_size_check[sizeof(FsDeviceHeap) == 0xC ? 1 : -1];
#ifndef __cplusplus /* edg (C++ donors): &((T *)0)->f is not an integral constant expression */
typedef char UsoCtx_print_check[(int)&((UsoCtx *)0)->print == 0x84 ? 1 : -1];
typedef char UsoCtx_printSlots_check[(int)&((UsoCtx *)0)->printSlots == 0xA0 ? 1 : -1];
#endif

#endif
