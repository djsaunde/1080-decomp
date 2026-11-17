#ifndef PORT_GFX_DLUMP_H
#define PORT_GFX_DLUMP_H

/* port/gfx: DLMP, a self-contained dump of one F3DEX (GBI 1) graphics task
 * (PC port plan section 19, M2).
 *
 * The 32-bit game hands each M_GFXTASK to port_gfx_task (port/gfx/port_gfx.c).
 * With PORT_GFX_DUMP set, the task's display list is walked the way the RSP
 * walks it (G_DL push / branch, G_ENDDL, segments), and every byte range a
 * command references is copied out: the lists themselves, vertices, matrices,
 * G_MOVEMEM blocks (viewport, lights, lookat) and texture / TLUT bytes from
 * LOADBLOCK / LOADTILE / LOADTLUT. A separate process (port/gfx/replay, 64-bit)
 * can then rebuild the task in its own memory and render it, with no game code.
 *
 * File layout (all integers in the HOST byte order of the writer; the
 * byteOrderMark tells a reader which; on x86 it is little-endian):
 *
 *     DlumpHeader                       64 bytes
 *     { DlumpChunk; u8 data[size]; pad to 4 } * chunkCount
 *     DlumpChunk { kind = DLUMP_END }   terminator
 *
 * A chunk is a copy of game memory at [addr, addr + size), tagged with what
 * referenced it. Chunks may overlap (a matrix block read as two MTX ranges,
 * a list entered half-way); the copies agree, because the whole dump is taken
 * at one instant. A reader maps every chunk at its addr into a sparse memory
 * image and interprets the root list from there.
 *
 * The bytes are copied raw: the display-list words, Vtx, Mtx, Vp and Light
 * are in host order exactly as the game wrote them (the port's Gfx is two
 * host-order u32s), and texels stay in the N64 big-endian image order that
 * Fast3D's importers expect.
 *
 * Addresses: G_DL / G_VTX / ... operands are the game's 32-bit host pointers.
 * Segments: F3DEX resolves w1 as segTable[(w1 >> 24) & 15] + (w1 & 0xFFFFFF).
 * 1080 only ever sets segment 0 = 0 (plan 11.1), and a host pointer's top byte
 * is not a segment number, so the dumper (and a replayer) use: if the segment
 * named by the top byte was set to a NONZERO base by G_MOVEWORD SEGMENT in this
 * task, apply it; otherwise the operand is a plain pointer. The dumped words
 * are never rewritten. */

#include <stddef.h>

typedef unsigned int dlump_u32;
typedef unsigned char dlump_u8;

#define DLUMP_MAGIC "1080DLMP"
#define DLUMP_VERSION 1
#define DLUMP_BYTE_ORDER_MARK 0x01020304u

/* header flags */
#define DLUMP_FLAG_TRUNCATED   0x1u /* the command or byte limit stopped the walk */
#define DLUMP_FLAG_SEGMENTS    0x2u /* a nonzero segment base was applied */

typedef struct DlumpHeader {
    char magic[8];           /* "1080DLMP" */
    dlump_u32 version;       /* DLUMP_VERSION */
    dlump_u32 headerSize;    /* sizeof(DlumpHeader) = 64 */
    dlump_u32 byteOrderMark; /* 0x01020304 in the writer's order */
    dlump_u32 ptrSize;       /* sizeof(void *) of the writer (4 for the game) */
    dlump_u32 seq;           /* gfx task number since boot */
    dlump_u32 rootAddr;      /* task data_ptr: the root display list */
    dlump_u32 rootSize;      /* task data_size (informational; the walk ends at G_ENDDL) */
    dlump_u32 chunkCount;    /* chunks before the DLUMP_END terminator */
    dlump_u32 cmdCount;      /* commands interpreted (re-walks of a shared list count again) */
    dlump_u32 errorCount;    /* DLUMP_ERROR chunks */
    dlump_u32 flags;         /* DLUMP_FLAG_* */
    dlump_u32 dataBytes;     /* sum of chunk sizes */
    dlump_u32 reserved[2];
} DlumpHeader;

typedef struct DlumpChunk {
    dlump_u32 kind; /* DlumpKind */
    dlump_u32 addr; /* game address of data[0] (DLUMP_ERROR: the command's address) */
    dlump_u32 size; /* bytes of data that follow (then padding to 4) */
    dlump_u32 aux;  /* per kind, below */
} DlumpChunk;

typedef enum DlumpKind {
    DLUMP_END = 0,
    DLUMP_TASK = 1,    /* the OSTask_t (0x40 bytes); aux = task type */
    DLUMP_DL = 2,      /* display-list words; aux = call depth at first visit */
    DLUMP_VTX = 3,     /* G_VTX source; aux = vertex count */
    DLUMP_MTX = 4,     /* G_MTX source (0x40); aux = G_MTX params byte */
    DLUMP_MOVEMEM = 5, /* G_MOVEMEM source; aux = index byte (0x80 VIEWPORT, 0x82 / 0x84 LOOKAT, 0x86.. lights) */
    DLUMP_TEXTURE = 6, /* LOADBLOCK / LOADTILE source; aux = the G_SETTIMG w0 in force (fmt, siz, width - 1) */
    DLUMP_TLUT = 7,    /* LOADTLUT source; aux = the G_SETTIMG w0 in force */
    DLUMP_IMAGE = 8,   /* G_SETCIMG / G_SETZIMG target, no data (size 0); aux = the command's w0 */
    DLUMP_ERROR = 9,   /* data = NUL-terminated text; addr = the command address */
    /* The framebuffer model (../include/port_fb.h, plan 19.10): what the game's CPU did with the
     * colour / depth images around this task. Each applies only if the task's last G_SETCIMG
     * (FB_READ, FB_WRITE) or G_SETZIMG (FB_DEPTH) names the image: low 24 bits equal to addr's. */
    DLUMP_FB_READ = 10,  /* the CPU reads the colour image at addr (no data); after the task, read the frame back
                            into it: 320x240 RGBA5551, big-endian; aux = width (320) */
    DLUMP_FB_DEPTH = 11, /* the CPU reads these depth pixels of the Z image at addr: data = u32 pixel indices
                            (y * width + x); after the task, store each one's N64 Z (0xFFFC = far), big-endian;
                            aux = width */
    DLUMP_FB_WRITE = 12, /* the CPU wrote a rectangle of the colour image at addr: data = u16 x, y, w, h, then
                            w * h RGBA5551 pixels as in memory (big-endian), row by row; after the task,
                            composite it over the frame, opaque; aux = width */
    DLUMP_KIND_COUNT
} DlumpKind;

/* Limits. F3DEX 1.23 has a 10-deep G_DL stack; deeper is reported as an error. */
#define DLUMP_DL_STACK 10
#define DLUMP_MAX_COMMANDS (1u << 22)
#define DLUMP_MAX_RANGE (8u << 20) /* one capture */

/* A growable output buffer. */
typedef struct DlumpBuf {
    dlump_u8 *data;
    size_t size;
    size_t cap;
    int oom;
} DlumpBuf;

typedef struct DlumpOptions {
    /* Game address -> pointer: (void *)(arenaBase + addr). 0 on the 32-bit game; a
     * test or a 64-bit host with a 4 GB arena sets it (plan 11.5 item 7). */
    size_t arenaBase;
    /* Optional: nonzero if [p, p + n) may be read. NULL = only reject addresses
     * below 0x1000. port_gfx.c passes a mincore()-based check on Linux. */
    int (*readable)(const void *p, size_t n);
    /* Optional: chunks appended after the walked ones (the DLUMP_FB_* records), written as
     * given. data is copied (size bytes; NULL when size is 0). */
    const struct DlumpExtra *extra;
    size_t nExtra;
} DlumpOptions;

typedef struct DlumpExtra {
    dlump_u32 kind, addr, size, aux;
    const void *data;
} DlumpExtra;

/* Dump the task whose OSTask_t is at taskAddr (copied as DLUMP_TASK if nonzero)
 * and whose root list is rootAddr. Appends the whole file image to out.
 * Returns the number of DLUMP_ERROR chunks, or -1 when out ran out of memory. */
int dlump_task(DlumpBuf *out, const DlumpOptions *opt, dlump_u32 seq, dlump_u32 taskAddr,
               dlump_u32 taskType, dlump_u32 rootAddr, dlump_u32 rootSize);

void dlump_buf_free(DlumpBuf *b);

/* A pointer-free content hash of the task's display list (dlump.c; soak.py's screen detector). */
dlump_u32 dlump_screen_hash(const DlumpOptions *opt, dlump_u32 root);

#endif
