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

/* compile-time layout checks (IDO cfe accepts negative-size typedefs as errors) */
typedef char UsoModuleSlot_size_check[sizeof(UsoModuleSlot) == 0x9C ? 1 : -1];
typedef char UsoModuleSlot_secData_check[(int)&((UsoModuleSlot *)0)->secData == 0x38 ? 1 : -1];
typedef char UsoModuleSlot_id_check[(int)&((UsoModuleSlot *)0)->id == 0x70 ? 1 : -1];
typedef char UsoModuleSlot_name_check[(int)&((UsoModuleSlot *)0)->name == 0x72 ? 1 : -1];
typedef char UsoModuleSlot_romOffset_check[(int)&((UsoModuleSlot *)0)->romOffset == 0x94 ? 1 : -1];
typedef char UsoModuleSlot_inUse_check[(int)&((UsoModuleSlot *)0)->inUse == 0x98 ? 1 : -1];

#endif
