/* port/uso: the USO data blobs filled from the user's ROM at run time (plan section 29).
 *
 * A build with PORT_RUNTIME_BLOBS (the Windows build; optional elsewhere) holds no cartridge bytes:
 * port/tools/gen-uso-blobs.py --runtime emits every blob and its pristine _init copy as zeroed arrays,
 * and port_blob_sections[] says where each _init copy comes from. port_uso_blobs_load() walks the ROM's
 * file system (the kernel device table, as port/tools/extract/extract.py and port/fs do), decodes the
 * Yay0 sections, applies port/gen/blob_types.tsv's typed swaps (port/tools/blob_swap.py) and fills them,
 * before the boot loads bootup.uso. The result is byte for byte the .incbin of the default build. */
#ifndef PORT_USO_ROM_H
#define PORT_USO_ROM_H

typedef struct PortBlobRun {
    unsigned int off;    /* byte offset in the section */
    unsigned int size;   /* bytes, a multiple of w */
    int w;               /* element size: 2, 4 or 8, reversed per element */
} PortBlobRun;

typedef struct PortBlobSection {
    const char *file;            /* "bootup.uso" */
    int type;                    /* USO record type: 6 Data, 7 RoData */
    unsigned int size;
    char *init;                  /* port_<mod>_<sec>_init */
    const PortBlobRun *runs;
    int nRuns;
} PortBlobSection;

extern const PortBlobSection port_blob_sections[];
extern const int port_blob_nsections;

/* fill every _init copy from rom (a big-endian .z64 image). 0 on success; else -1 and *why (and *file,
 * the section's file, or 0) say what did not match */
int port_uso_blobs_load(const unsigned char *rom, unsigned int romSize, const char **why, const char **file);

#endif
