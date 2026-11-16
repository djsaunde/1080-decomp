#ifndef GDL_GDLFILE_H
#define GDL_GDLFILE_H

/* GdlFile: the libgdl file object behind fs_gdlOpenFile / ReadFile / ReadFileAsync / SeekFile /
 * WriteFile / CloseFile / FileSize (game_libs post0b 0x35370-0x35990; agent-b run 542). Every
 * method dispatches on `kind` (the 1, 0, 2 compare chains):
 *   GDLFILE_REMOTE 0  the HostIO link to the development host (hostio_openFile / readFile /
 *                     seekFile / writeFile / closeFile / fileSize; "Remote OpenFile fail");
 *   GDLFILE_LOCAL  1  the ROM file system through SYSAPI (fileOpen / fileRead / fileSeek /
 *                     fileClose / fileRomAddr / fileError on `handle`); read-only ("Can't write
 *                     to Local"), the only kind that can read asynchronously;
 *   GDLFILE_SRAM   2  battery SRAM through the PI (osEPiStartDma, "SRAM read from" / "SRAM
 *                     write to"); the position is the counter word at gl 0x3B8E8 (0x08000000 =
 *                     the SRAM base), and open sets handle = 1.
 * Only kind and handle are known. */
typedef struct GdlFile {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 kind;            /* GDLFILE_* */
    /* 0x08 */ s32 unk08[6];
    /* 0x20 */ s32 handle;          /* the open result (fs_setHandle / fs_getHandle) */
} GdlFile;

#define GDLFILE_REMOTE 0
#define GDLFILE_LOCAL  1
#define GDLFILE_SRAM   2

#ifndef __cplusplus
typedef char GdlFile_handle_check[(int)&((GdlFile *)0)->handle == 0x20 ? 1 : -1];
#endif

#define GDLFILE(p) ((GdlFile *)(p))

#endif /* GDL_GDLFILE_H */
