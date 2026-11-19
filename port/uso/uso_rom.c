/* port/uso: the USO data blobs filled from the user's ROM at run time (uso_rom.h; plan section 29). */
#include "uso_rom.h"

/* no libc headers: port/uso builds without a target libc */
#define memcpy __builtin_memcpy

#define DEVICE_TABLE_ROM 0xAE60u          /* k_gFsDevices, link 0x80009E60 */
#define KERNEL_RUNTIME_TO_ROM 0x7FFFF400u /* runtime pointer (link + 0x400) -> ROM offset */
#define INFO_MAGIC 0x12345678u
#define FLAG_YAY0 0x10000u
enum { REC_INFO = 0, REC_BSS = 8, REC_EOF = 10, REC_END = 11 };

static const unsigned char *sRom;
static unsigned int sRomSize;

static unsigned int be32(unsigned int a)
{
    const unsigned char *p = sRom + a;

    return ((unsigned int)p[0] << 24) | ((unsigned int)p[1] << 16) | ((unsigned int)p[2] << 8) | p[3];
}

static int in_rom(unsigned int a, unsigned int n)
{
    return a <= sRomSize && n <= sRomSize - a;
}

static int name_eq(const unsigned char *a, unsigned int max, const char *b)
{
    unsigned int i;

    for (i = 0; i < max; i++) {
        if (a[i] != (unsigned char)b[i]) {
            return 0;
        }
        if (b[i] == 0) {
            return 1;
        }
    }
    return 0;
}

/* the Yay0 block at ROM offset off into dst (want bytes); 0 on success (extract.py yay0_decode) */
static int yay0(unsigned int off, unsigned int end, char *dst, unsigned int want)
{
    unsigned int size, maskP, linkP, chunkP, o = 0, mask = 0;
    int bits = 0;

    if (!in_rom(off, 16) || be32(off) != 0x59617930u) { /* "Yay0" */
        return -1;
    }
    size = be32(off + 4);
    if (size != want) {
        return -1;
    }
    maskP = off + 16;
    linkP = off + be32(off + 8);
    chunkP = off + be32(off + 12);
    while (o < size) {
        if (bits == 0) {
            if (maskP + 4 > end) {
                return -1;
            }
            mask = be32(maskP);
            maskP += 4;
            bits = 32;
        }
        if (mask & 0x80000000u) {
            if (chunkP >= end) {
                return -1;
            }
            dst[o++] = (char)sRom[chunkP++];
        } else {
            unsigned int v, n, src;
            if (linkP + 2 > end) {
                return -1;
            }
            v = ((unsigned int)sRom[linkP] << 8) | sRom[linkP + 1];
            linkP += 2;
            n = v >> 12;
            if (n == 0) {
                if (chunkP >= end) {
                    return -1;
                }
                n = sRom[chunkP++] + 0x12u;
            } else {
                n += 2;
            }
            if ((v & 0xFFFu) + 1 > o || n > size - o) {
                return -1;
            }
            src = o - (v & 0xFFFu) - 1;
            while (n-- > 0) {    /* overlapping copies are byte-serial */
                dst[o++] = dst[src++];
            }
        }
        mask <<= 1;
        bits--;
    }
    return 0;
}

static void swap_runs(const PortBlobSection *s)
{
    int i;

    for (i = 0; i < s->nRuns; i++) {
        const PortBlobRun *r = &s->runs[i];
        unsigned int o;
        for (o = r->off; o + (unsigned int)r->w <= r->off + r->size; o += (unsigned int)r->w) {
            char *p = s->init + o;
            int a = 0, b = r->w - 1;
            while (a < b) {
                char t = p[a];
                p[a++] = p[b];
                p[b--] = t;
            }
        }
    }
}

int port_uso_blobs_load(const unsigned char *rom, unsigned int romSize, const char **why, const char **file)
{
    static unsigned char done[256];
    unsigned int d;
    int i, left = port_blob_nsections;

    sRom = rom;
    sRomSize = romSize;
    *file = 0;
    if (port_blob_nsections > (int)sizeof(done)) {
        *why = "too many blob sections";
        return -1;
    }
    for (i = 0; i < port_blob_nsections; i++) {
        done[i] = 0;
    }
    for (d = DEVICE_TABLE_ROM; in_rom(d, 0x14) && be32(d) != 0 && left > 0; d += 0x14) {
        unsigned int o = be32(d + 4), end = be32(d + 8);
        const unsigned char *cur = 0;     /* the current file's name (Info record) */
        if (end > romSize || o >= end) {
            *why = "bad device table (not the US 1.0 ROM?)";
            return -1;
        }
        while (o + 12 <= end) {
            unsigned int type = be32(o), size = be32(o + 4), flag = be32(o + 8), payload = o + 12;
            if (type == REC_EOF) {
                break;
            }
            if (type > 13) {
                *why = "bad file-system record (not the US 1.0 ROM?)";
                return -1;
            }
            if (type == REC_INFO) {
                if (size != 0x2C || !in_rom(payload, 0x2C) || be32(payload) != INFO_MAGIC) {
                    *why = "bad Info record (not the US 1.0 ROM?)";
                    return -1;
                }
                cur = rom + payload + 12;
                o = payload + size;
                continue;
            }
            if (cur != 0 && type != REC_BSS && type != REC_END) {
                for (i = 0; i < port_blob_nsections; i++) {
                    const PortBlobSection *s = &port_blob_sections[i];
                    if (done[i] || (unsigned int)s->type != type || !name_eq(cur, 0x20, s->file)) {
                        continue;
                    }
                    *file = s->file;
                    if (flag & FLAG_YAY0) {
                        if (yay0(payload, end, s->init, s->size) != 0) {
                            *why = "a section's size or Yay0 data does not match the build";
                            return -1;
                        }
                    } else {
                        if (size != s->size || !in_rom(payload, size)) {
                            *why = "a section's size does not match the build";
                            return -1;
                        }
                        memcpy(s->init, rom + payload, size);
                    }
                    swap_runs(s);
                    done[i] = 1;
                    left--;
                }
            }
            o = type == REC_BSS ? payload : payload + size;
            if (type == REC_END) {
                cur = 0;
            }
        }
    }
    for (i = 0; i < port_blob_nsections; i++) {
        if (!done[i]) {
            *file = port_blob_sections[i].file;
            *why = "a module section is missing from the ROM";
            return -1;
        }
    }
    *file = 0;
    return 0;
}
