#ifndef GDL_DMA_H
#define GDL_DMA_H

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

typedef char DmaSlot_size_check[sizeof(DmaSlot) == 0x44 ? 1 : -1];
typedef char DmaSlot_mq_check[(int)&((DmaSlot *)0)->mq == 0x18 ? 1 : -1];
typedef char DmaSlot_msg_check[(int)&((DmaSlot *)0)->msg == 0x30 ? 1 : -1];
typedef char DmaSlot_busy_check[(int)&((DmaSlot *)0)->busy == 0x3C ? 1 : -1];
typedef char DmaSlot_invalidate_check[(int)&((DmaSlot *)0)->invalidate == 0x40 ? 1 : -1];

#endif
