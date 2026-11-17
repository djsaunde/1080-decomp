/* port/fs: the ROM file system of the PC port (docs/1080_PC_PORT_PLAN.md section 17.8,
 * docs/USO_FORMAT.md sections 2 and 9).
 *
 * It replaces the kernel's file layer (kernel_000.c: uso_scanRom, uso_find_file, fs_openFile,
 * fs_read, fs_seek, fs_tell, uso_findDataSection, uso_getErrno) with the same semantics, over
 * the in-memory big-endian ROM image the PI shim already holds (port/ultra, PortUltraConfig.rom).
 * The ROM image is the source rather than the extracted tree (port/tools/extract): fileRomAddr
 * hands the game a ROM address that it then DMAs from through the PI shim, so the image has to
 * be there anyway, and positions are device offsets exactly as on the cartridge.
 *
 * Model (the kernel's):
 *   - Devices come from the kernel's device table in the ROM (k_gFsDevices, ROM 0xAE60):
 *     "mainuso" (bootup.uso) and "rom" (every other .uso and the .bin files).
 *   - The directory (gUsoModuleSlots) maps a FILE NAME to the device offset just past its Info
 *     record, for every file of every device with a module id >= 0.  The lookup is by name only;
 *     the device named in the path only supplies the base (romStart), as in uso_find_file.
 *   - A path is "device/file", split at the first '/'.  Unknown device or name: -0x11.
 *   - An open file is a window on the first record after the Info record: the 12-byte record
 *     header is read, then [pos, pos + size) is the payload.  For a .bin file that record is its
 *     BinaryDataSection.
 *   - Reads are fread-like (fs_read): size is clamped to the window (the kernel's 8-alignment
 *     check on buf, errno -0x1C, is dropped: plan 18.9), bytes are copied raw (big-endian, Yay0 left compressed: the readers decode).
 *   - Seek: whence 0 sets the DEVICE offset (as fs_seek does), else adds.  Tell = romStart + pos,
 *     a ROM address (fileRomAddr; the dma_readAsync source).
 *   - Info: the ROM address of the file's Data section payload (uso_findDataSection: walk the
 *     records from the file's first one, Bss has no payload, stop at Data; 0 bytes left: -1000).
 *
 * Plain C, no libc headers (builds without a 32-bit libc). */
#ifndef PORT_FS_H
#define PORT_FS_H

#define PORT_FS_ERR_NOT_FOUND (-0x11) /* uso_find_file / uso_load: unknown device or name */
#define PORT_FS_ERR_ALIGN (-0x1C)     /* the kernel's fs_read: buffer not 8-aligned (not produced on the port) */
#define PORT_FS_ERR_NO_DATA (-1000)   /* uso_findDataSection: ran out of records */
#define PORT_FS_ERR_NO_HANDLE (-18)   /* no free handle (the kernel's alloc failure code) */

/* An open file (the kernel's 0x28-byte file struct, reduced to what its services read). */
typedef struct PortFsFile {
    int inUse;
    int device;            /* index in the device table */
    unsigned int romStart; /* the device's ROM base */
    unsigned int pos;      /* device offset (file[1]) */
    unsigned int limit;    /* end of the window, device offset (file[7]) */
    unsigned int window;   /* start of the window (file[8]) */
} PortFsFile;

typedef struct PortFsEntry {
    char name[0x22];       /* UsoModuleSlot.name (0x72..0x93) */
    int id;                /* module id (the slot index on the N64) */
    int device;            /* device the file lives on */
    unsigned int offset;   /* device offset just past the Info record (UsoModuleSlot.romOffset) */
} PortFsEntry;

/* Parse the device table and build the directory from a big-endian .z64 image (the image must
 * stay alive).  Returns the number of files, or < 0 if the image does not look like 1080. */
int port_fs_init(const unsigned char *rom, unsigned int romSize);

int port_fs_device_count(void);
const char *port_fs_device_name(int i);
int port_fs_entry_count(void);
const PortFsEntry *port_fs_entry(int i);
const PortFsEntry *port_fs_lookup(const char *path, int *device); /* "dev/file"; NULL if none */

/* the kernel services (SysApi +0x48 .. +0x60) */
int port_fs_info(const char *path, int *romAddr);                  /* 0 or < 0 */
PortFsFile *port_fs_open(const char *path);                        /* NULL on error (see errno) */
int port_fs_read(PortFsFile *f, void *buf, int size);              /* bytes, or errno (< 0) */
int port_fs_seek(PortFsFile *f, int whence, int offset);           /* new device offset */
int port_fs_tell(const PortFsFile *f);                             /* ROM address */
void port_fs_close(PortFsFile *f);
int port_fs_errno(void);

#endif
