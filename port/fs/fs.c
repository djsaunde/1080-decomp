/* port/fs: the ROM file system (port_fs.h; plan section 17.8). */
#include "port_fs.h"

#define memcpy __builtin_memcpy
#define memset __builtin_memset

extern void port_ultra_log(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

#define DEVICE_TABLE_ROM 0xAE60u          /* k_gFsDevices, link 0x80009E60 */
#define KERNEL_RUNTIME_TO_ROM 0x7FFFF400u /* runtime pointer (link + 0x400) -> ROM offset */
#define INFO_MAGIC 0x12345678u
#define MAX_DEVICES 16                    /* uso_init: up to 16 device entries */
#define MAX_ENTRIES 135                   /* gUsoModuleSlots[135] */
#define MAX_FILES 64                      /* open handles at once */

enum { REC_INFO = 0, REC_DATA = 6, REC_BSS = 8, REC_EOF = 10, REC_END = 11 };

typedef struct Device {
    char name[16];
    unsigned int romStart;
    unsigned int romEnd;
} Device;

static const unsigned char *sRom;
static unsigned int sRomSize;
static Device sDevices[MAX_DEVICES];
static int sNumDevices;
static PortFsEntry sEntries[MAX_ENTRIES];
static int sNumEntries;
static PortFsFile sFiles[MAX_FILES];
static int sErrno;

static unsigned int be32(unsigned int romAddr)
{
    const unsigned char *p;

    if (romAddr + 4 > sRomSize || romAddr + 4 < romAddr) {
        return 0;
    }
    p = sRom + romAddr;
    return ((unsigned int)p[0] << 24) | ((unsigned int)p[1] << 16) | ((unsigned int)p[2] << 8) | p[3];
}

static int str_eq(const char *a, const char *b)
{
    while (*a != 0 && *a == *b) {
        a++;
        b++;
    }
    return *a == *b;
}

/* uso_splitPath: dir = up to the first '/', file = the rest (no '/': dir = path, file = "") */
static void split_path(const char *path, char *dir, int dirSize, const char **file)
{
    int i = 0;

    while (path[i] != 0 && path[i] != '/') {
        if (i < dirSize - 1) {
            dir[i] = path[i];
        }
        i++;
    }
    dir[i < dirSize - 1 ? i : dirSize - 1] = 0;
    *file = path[i] == '/' ? path + i + 1 : path + i;
}

static int find_device(const char *name)
{
    int i;

    for (i = 0; i < sNumDevices; i++) {
        if (str_eq(sDevices[i].name, name)) {
            return i;
        }
    }
    return -1;
}

/* uso_scanRom over one device: every Info record names a file; skip its records up to End. */
static void scan_device(int dev)
{
    unsigned int base = sDevices[dev].romStart;
    unsigned int span = sDevices[dev].romEnd - base;
    unsigned int pos = 0;

    while (pos + 12 <= span) {
        unsigned int type = be32(base + pos);
        unsigned int size = be32(base + pos + 4);
        unsigned int payload = pos + 12;

        if (type == REC_EOF) {
            return;
        }
        if (type != REC_INFO || size < 0x2C || be32(base + payload) != INFO_MAGIC) {
            port_ultra_log("port/fs: %s: no Info record at device offset 0x%X", sDevices[dev].name, pos);
            return;
        }
        {
            int id = (int)be32(base + payload + 4);
            const unsigned char *nm = sRom + base + payload + 12;

            pos = payload + size; /* = the slot's romOffset: just past the Info record */
            if (id >= 0 && sNumEntries < MAX_ENTRIES) {
                PortFsEntry *e = &sEntries[sNumEntries++];
                int k;

                for (k = 0; k < (int)sizeof(e->name) - 1 && nm[k] != 0; k++) {
                    e->name[k] = (char)nm[k];
                }
                e->name[k] = 0;
                e->id = id;
                e->device = dev;
                e->offset = pos;
            }
        }
        /* uso_skip_to_end: records up to End (Bss has no payload) */
        for (;;) {
            unsigned int t, s;

            if (pos + 12 > span) {
                return;
            }
            t = be32(base + pos);
            s = be32(base + pos + 4);
            pos += 12;
            if (t != REC_BSS) {
                pos += s;
            }
            if (t == REC_END) {
                break;
            }
        }
    }
}

int port_fs_init(const unsigned char *rom, unsigned int romSize)
{
    unsigned int p;

    sRom = rom;
    sRomSize = romSize;
    sNumDevices = 0;
    sNumEntries = 0;
    sErrno = 0;
    memset(sFiles, 0, sizeof(sFiles));
    if (rom == 0 || romSize < DEVICE_TABLE_ROM + 0x14) {
        port_ultra_log("port/fs: no ROM image");
        return -1;
    }
    for (p = DEVICE_TABLE_ROM; sNumDevices < MAX_DEVICES; p += 0x14) {
        unsigned int namePtr = be32(p);
        unsigned int no, k;
        Device *d;

        if (namePtr == 0) {
            break;
        }
        no = namePtr - KERNEL_RUNTIME_TO_ROM;
        if (no >= romSize) {
            port_ultra_log("port/fs: bad device name pointer 0x%08X", namePtr);
            return -1;
        }
        d = &sDevices[sNumDevices++];
        for (k = 0; k < sizeof(d->name) - 1 && no + k < romSize && rom[no + k] != 0; k++) {
            d->name[k] = (char)rom[no + k];
        }
        d->name[k] = 0;
        d->romStart = be32(p + 4);
        d->romEnd = be32(p + 8);
        if (d->romEnd > romSize || d->romStart > d->romEnd) {
            port_ultra_log("port/fs: device %s range 0x%X-0x%X outside the ROM", d->name, d->romStart,
                           d->romEnd);
            return -1;
        }
    }
    if (sNumDevices == 0) {
        port_ultra_log("port/fs: empty device table");
        return -1;
    }
    for (p = 0; p < (unsigned int)sNumDevices; p++) {
        scan_device((int)p);
    }
    return sNumEntries;
}

int port_fs_device_count(void)
{
    return sNumDevices;
}

const char *port_fs_device_name(int i)
{
    return i >= 0 && i < sNumDevices ? sDevices[i].name : 0;
}

int port_fs_entry_count(void)
{
    return sNumEntries;
}

const PortFsEntry *port_fs_entry(int i)
{
    return i >= 0 && i < sNumEntries ? &sEntries[i] : 0;
}

/* uso_find_file: the device from the path, the offset from the directory entry of that name */
const PortFsEntry *port_fs_lookup(const char *path, int *device)
{
    char dir[32];
    const char *file;
    int dev, i;

    if (path == 0) {
        return 0;
    }
    split_path(path, dir, sizeof(dir), &file);
    dev = find_device(dir);
    if (dev < 0) {
        return 0;
    }
    for (i = 0; i < sNumEntries; i++) {
        if (str_eq(sEntries[i].name, file)) {
            if (device != 0) {
                *device = dev;
            }
            return &sEntries[i];
        }
    }
    return 0;
}

/* fs_read(buf, size, 1, fp) */
static int raw_read(PortFsFile *f, void *buf, int size)
{
    int total;

    if ((unsigned long)buf & 7) {
        return sErrno = PORT_FS_ERR_ALIGN;
    }
    total = size;
    if (total < 0) {
        return 0;
    }
    /* fs_read clamps size to limit - pos (an unsigned compare that wraps for a pos past the
     * limit); the port clamps without wrapping: past the limit there is nothing to read */
    if (f->pos >= f->limit) {
        total = 0;
    } else if (f->limit - f->pos < (unsigned int)total) {
        total = (int)(f->limit - f->pos);
    }
    if (total != 0) {
        unsigned int src = f->romStart + f->pos;
        unsigned int have = 0;

        if (src < sRomSize) {
            have = sRomSize - src < (unsigned int)total ? sRomSize - src : (unsigned int)total;
            memcpy(buf, sRom + src, have);
        }
        if (have < (unsigned int)total) {
            port_ultra_log("port/fs: read of 0x%X bytes at ROM 0x%X runs past the image", total, src);
            memset((char *)buf + have, 0, (unsigned int)total - have);
        }
    }
    f->pos += (unsigned int)total;
    return total;
}

/* a fresh file on the entry's device, positioned at the entry (fs_initFile + uso_find_file) */
static void file_at(PortFsFile *f, int dev, const PortFsEntry *e)
{
    f->device = dev;
    f->romStart = sDevices[dev].romStart;
    f->limit = sDevices[dev].romEnd; /* fs_initFile: file[7] = the entry's romEnd */
    f->pos = e->offset;
    f->window = 0;
}

/* uso_findDataSection */
int port_fs_info(const char *path, int *romAddr)
{
    PortFsFile f;
    const PortFsEntry *e;
    int dev = 0;

    e = port_fs_lookup(path, &dev);
    if (e == 0) {
        return PORT_FS_ERR_NOT_FOUND;
    }
    memset(&f, 0, sizeof(f));
    file_at(&f, dev, e);
    for (;;) {
        unsigned int hdr[4] __attribute__((aligned(8))); /* fs_read checks the alignment */
        int r = raw_read(&f, hdr, 12);
        unsigned int type, size;

        if (r <= 0) {
            return r == 0 ? PORT_FS_ERR_NO_DATA : r;
        }
        type = be32(f.romStart + f.pos - 12);
        size = be32(f.romStart + f.pos - 8);
        if (type == REC_DATA) {
            break;
        }
        /* The kernel walks on into the next file (and past the device's EndOfFile) when a file
         * has no Data section; the port stops at the device's end. */
        if (type == REC_EOF || f.pos + size > sDevices[dev].romEnd - sDevices[dev].romStart) {
            return PORT_FS_ERR_NO_DATA;
        }
        if (type == REC_BSS) {
            continue;
        }
        f.pos += size; /* fs_skip */
    }
    *romAddr = (int)(f.pos + f.romStart);
    return 0;
}

/* fs_openFile: the window is the payload of the file's first record */
PortFsFile *port_fs_open(const char *path)
{
    const PortFsEntry *e;
    PortFsFile *f = 0;
    unsigned int hdr[4] __attribute__((aligned(8)));
    int dev = 0, i;

    e = port_fs_lookup(path, &dev);
    if (e == 0) {
        sErrno = PORT_FS_ERR_NOT_FOUND; /* the kernel leaves errno alone here; the port reports it */
        return 0;
    }
    for (i = 0; i < MAX_FILES; i++) {
        if (!sFiles[i].inUse) {
            f = &sFiles[i];
            break;
        }
    }
    if (f == 0) {
        port_ultra_log("port/fs: more than %d files open (%s)", MAX_FILES, path);
        sErrno = PORT_FS_ERR_NO_HANDLE;
        return 0;
    }
    file_at(f, dev, e);
    if (raw_read(f, hdr, 12) < 0) {
        return 0;
    }
    f->window = f->pos;
    f->limit = be32(f->romStart + f->pos - 8) + f->pos;
    f->inUse = 1;
    return f;
}

/* fs_readChecked */
int port_fs_read(PortFsFile *f, void *buf, int size)
{
    int r;

    if (f == 0) {
        return sErrno = PORT_FS_ERR_NOT_FOUND;
    }
    r = raw_read(f, buf, size);
    return r < 0 ? sErrno : r;
}

/* fs_seek */
int port_fs_seek(PortFsFile *f, int whence, int offset)
{
    if (whence) {
        f->pos += (unsigned int)offset;
    } else {
        f->pos = (unsigned int)offset;
    }
    return (int)f->pos;
}

/* fs_tell */
int port_fs_tell(const PortFsFile *f)
{
    return (int)(f->pos + f->romStart);
}

/* fs_closeFile is a no-op on the N64 (the 0x28-byte struct stays in the heap it came from);
 * the port returns the handle to its pool. */
void port_fs_close(PortFsFile *f)
{
    if (f != 0) {
        f->inUse = 0;
    }
}

/* uso_getErrno */
int port_fs_errno(void)
{
    return sErrno;
}
