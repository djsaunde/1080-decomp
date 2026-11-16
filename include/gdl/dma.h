#ifndef GDL_DMA_H
#define GDL_DMA_H

#include "gdl/gl_data.h"

/* One libgdl PI DMA slot (game_libs 0x337AC-0x33B6C, src/game_libs/game_libs_post.c).
 * The table is 10 slots of 0x44 bytes at the start of game_libs' data base
 * (&D_00000000 + 0). Slot 0 is the synchronous slot (dma_read / dma_write
 * spin until it is idle); dma_allocSlot hands out 1..9 for the async calls
 * ("Out of DMA slots" when all are busy).
 *
 * dma_startRead / dma_startWrite: check alignment ("read not aligned"),
 * write back the cache, mark the slot busy and call
 *   osPiStartDma(&slot->ioMesg, 0, dir, devAddr, dram, len, &slot->mq).
 * dma_wait: when busy, osRecvMesg(&slot->mq, .., OS_MESG_BLOCK), then
 * dma_invalCache(dram, len) for reads, then busy = 0.
 * dma_init: osCreateMesgQueue(&slot->mq, &slot->msg, 1) for every slot.
 *
 * Field widths are the ones the matched bodies use (s32 for the words). */
typedef struct DmaSlot {
    /* 0x00 */ char ioMesg[0x18];  /* OSIoMesg */
    /* 0x18 */ char mq[0x18];      /* OSMesgQueue */
    /* 0x30 */ s32 msg;            /* the queue's 1-entry OSMesg buffer */
    /* 0x34 */ s32 dram;           /* RDRAM address of the transfer */
    /* 0x38 */ s32 len;            /* byte count */
    /* 0x3C */ s32 busy;           /* set by dma_start*, cleared by dma_wait */
    /* 0x40 */ s32 invalidate;     /* 1 for reads: dma_wait invalidates dram..+len */
} DmaSlot;

#define DMA_NUM_SLOTS 10

/* The slot table: bootup.uso Bss object at Bss+0x3A240 (Sym 1504, tag
 * 0x008277D2). Every dma_* TextReloc names that Sym; the instructions carry
 * only the field offsets (include/gdl/gl_data.h). */
#define gDmaSlots ((DmaSlot *)&D_00000000)

/* dma_* error strings, Data-section-base sites (Sym 2): */
#define sDmaReadNotAligned  GL_DATA(0x1E2C0) /* "read not aligned %d %x" */
#define sDmaBadReadLen      GL_DATA(0x1E2D8) /* "bad read len %d\n" */
#define sDmaWriteNotAligned GL_DATA(0x1E2EC) /* "write not aligned %d, %x" */
#define sDmaBadWriteLen     GL_DATA(0x1E308) /* "bad write len %d\n" */
#define sDmaOutOfSlots      GL_DATA(0x1E31C) /* "Out of DMA slots" */

/* One buffered PI stream (0x40): gFsStream0 / 1 / 2 (Bss+0x3F030 / 0x3F070 / 0x3F0B0, Syms 1520 /
 * 1542 / 1544), set up by fs_piXfer_00065EE4 (game_libs_post1b.c, its local view Save65EE4) at the
 * PI header addresses 0xB1FF0000 / 0xB1FF2710 / 0xB1FFC350. The stream has a 0x10-byte header at `hdr`
 * on the device side: +0 write cursor, +4 read cursor, +8 a handshake word, +0xC the committed
 * write end. Handshake values: a writer that reaches `end` posts 0x12340001 and spins until it
 * reads 0x12340002, rewinds to `data`, posts 0x12340003 and spins for 0x12340002 again; the
 * reader answers 0x12340001 with 0x12340002, rewinds, waits for 0x12340003 and posts 0x12340002;
 * fs_piXfer_0006612C waits for 0x12340004 at setup. fs_bufWrite_334E8 / fs_bufWrite_33BE4 append through stream 0 / 1,
 * fs_bufRead_33EB8 consumes stream 2 (game_libs_post.c, agent-c run 535). */
typedef struct FsStream {
    /* 0x00 */ u8 stage[8];   /* bounce buffer for an unaligned head (the byte copy loops) */
    /* 0x08 */ s32 wrPos;     /* write cursor (device address; header +0) */
    /* 0x0C */ s32 rdPos;     /* read cursor (header +4) */
    /* 0x10 */ s32 handshake; /* last handshake word fs_bufRead read (header +8) */
    /* 0x14 */ s32 committed; /* write end published to header +0xC at segment end */
    /* 0x18 */ char pad18[0x24 - 0x18];
    /* 0x24 */ s32 data;      /* first data address (hdr + 0x10); the cursors rewind here */
    /* 0x28 */ s32 hdr;       /* PI address of the header (fs_piXfer's base) */
    /* 0x2C */ s32 end;       /* data + size */
    /* 0x30 */ s32 size;
    /* 0x34 */ s32 unk34;     /* cleared by fs_piXfer_00065EE4 */
    /* 0x38 */ char pad38[0x40 - 0x38];
} FsStream; /* size = 0x40 */

typedef char DmaSlot_size_check[sizeof(DmaSlot) == 0x44 ? 1 : -1];
typedef char DmaSlot_mq_check[(int)&((DmaSlot *)0)->mq == 0x18 ? 1 : -1];
typedef char DmaSlot_msg_check[(int)&((DmaSlot *)0)->msg == 0x30 ? 1 : -1];
typedef char DmaSlot_busy_check[(int)&((DmaSlot *)0)->busy == 0x3C ? 1 : -1];
typedef char DmaSlot_invalidate_check[(int)&((DmaSlot *)0)->invalidate == 0x40 ? 1 : -1];

typedef char FsStream_size_check[sizeof(FsStream) == 0x40 ? 1 : -1];
typedef char FsStream_handshake_check[(int)&((FsStream *)0)->handshake == 0x10 ? 1 : -1];
typedef char FsStream_data_check[(int)&((FsStream *)0)->data == 0x24 ? 1 : -1];
typedef char FsStream_end_check[(int)&((FsStream *)0)->end == 0x2C ? 1 : -1];
typedef char FsStream_unk34_check[(int)&((FsStream *)0)->unk34 == 0x34 ? 1 : -1];

#endif
