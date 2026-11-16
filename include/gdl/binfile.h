#ifndef GDL_BINFILE_H
#define GDL_BINFILE_H

#include "gdl/node.h"

/*
 * libgdl packed-file managers (game_libs post1b 0x682F8-0x68BF4; agent-b run 515).
 *
 * A BinFileMgr is a Node that owns one ROM file of groups of sub-files. Its vfn8
 * (BinFileMgr_vfn8_00068524) opens the file (fs_getHandle), reads the group count and
 * runs BinFileMgr_loadFile_000683D4 on each 0x10-byte group: read the file count,
 * allocate the BinFile * array, and per file create it through the manager's newFile
 * slot (vtable +0x5C, this-adjust +0x58), back-link it and run BinFile_load_00068350.
 * BinFileMgr_get_00068730(mgr, key) returns groups[key >> 16].files[key & 0xFFFF]
 * after BinFileMgr_check_000685C0 ("BinFile Group out of range" / "File out of range" /
 * "File empty"). ImageFileMgr derives from it (newFile = ImageFileMgr_newFile_000688F8,
 * 0x30-byte ImageFile).
 *
 * Cfront layout: the BinFile vptr is the LAST member (+0x1C), the derived ImageFile
 * appends after it.
 */

/* one sub-file (0x20; BinFileMgr_newFile_0006877C, vtable BinFile_vtbl) */
typedef struct BinFile {
    /* 0x00 */ s32 flags;           /* bit 0: no file data (set by BinFile_load when the
                                     * load context has no stream); ImageFile_vfn1 /
                                     * ImageFile_free skip such files */
    /* 0x04 */ s32 fileOffset;      /* stream position at load (fs_gdlSeekFile(.., 1, 0)) */
    /* 0x08 */ char *data;          /* ImageFile: the pixels, read on demand (ImageFile_vfn1) */
    /* 0x0C */ struct BinFileMgr *mgr; /* owner (BinFileMgr_loadFile, BinFile_set) */
    /* 0x10 */ s32 size;            /* read by BinFile_load; 0 = "BinFile File empty" */
    /* 0x14 */ char unk14[0x1C - 0x14]; /* BinFile_init copies "<empty>" here (str_copyEnd) */
    /* 0x1C */ char *vptr;          /* Cfront vtable pointer */
} BinFile;

/* an image sub-file (0x30; ImageFileMgr_newFile_000688F8, vtable ImageFile_vtbl).
 * ImageFile_load_00068990 reads tagged ints: 1 width / height, 2 format, 4 dataSize
 * (then the data is read now, or its position kept in dataOffset), -1 end. */
typedef struct ImageFile {
    /* 0x00 */ BinFile base;
    /* 0x20 */ s16 width;
    /* 0x22 */ s16 height;
    /* 0x24 */ s32 format;          /* tag 2 */
    /* 0x28 */ s32 dataOffset;      /* file position of the data (read later by vfn1) */
    /* 0x2C */ s32 dataSize;
} ImageFile;

/* one group of sub-files (0x10) */
typedef struct BinFileGroup {
    /* 0x0 */ s32 unk0;             /* set by a call on (group, mgr) before loadFile */
    /* 0x4 */ BinFile **files;
    /* 0x8 */ s32 nFiles;
    /* 0xC */ struct BinFileMgr *mgr;
} BinFileGroup;

/* the manager (0x38; BinFileMgr_ctor_000684AC, ImageFileMgr_ctor) */
typedef struct BinFileMgr {
    /* 0x00 */ Node node;           /* vptr +0x28: BinFileMgr_vtbl / ImageFileMgr_vtbl */
    /* 0x2C */ s32 fileHandle;      /* fs_getHandle (vfn8); ImageFile_vfn1 reads through it */
    /* 0x30 */ BinFileGroup *groups;
    /* 0x34 */ s32 nGroups;
} BinFileMgr;

typedef char BinFile_size_check[sizeof(BinFile) == 0x20 ? 1 : -1];
typedef char ImageFile_size_check[sizeof(ImageFile) == 0x30 ? 1 : -1];
typedef char BinFileGroup_size_check[sizeof(BinFileGroup) == 0x10 ? 1 : -1];
typedef char BinFileMgr_size_check[sizeof(BinFileMgr) == 0x38 ? 1 : -1];

#define BINFILE(p) ((BinFile *)(p))
#define IMAGEFILE(p) ((ImageFile *)(p))
#define BINFILEMGR(p) ((BinFileMgr *)(p))

#endif /* GDL_BINFILE_H */
