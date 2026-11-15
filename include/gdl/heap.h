#ifndef GDL_HEAP_H
#define GDL_HEAP_H

/* libgdl heap (allocator at game_libs 0x5B070, free 0x5B3E0; heap_new 0x41148
 * is the "New(): Cant allocate %dk" front end that mem_new / mem_newAligned
 * call). Every allocation is preceded by a 16-byte header; sizes are kept in
 * 16-byte units. The allocator fills a new payload with 0xFF, stamps
 * magic = 0x12345678 and adds the size to heap->used.
 *
 * The allocator bodies (0x5B070..) still use their private Blk5B070 /
 * Heap5B070 typedefs, which have this same layout. */
typedef struct HeapBlock {
    /* 0x00 */ u32 hdr;                 /* heap id << 24 | size in 16-byte units */
    /* 0x04 */ struct HeapBlock *next;
    /* 0x08 */ struct HeapBlock *prev;
    /* 0x0C */ u32 magic;               /* 0x12345678 while allocated */
} HeapBlock;

#define HEAP_BLOCK_MAGIC 0x12345678
#define HEAP_BLOCK_ID(b) ((b)->hdr >> 24)

typedef struct Heap {
    /* 0x00 */ u32 unk00;
    /* 0x04 */ HeapBlock *next;         /* free-list head */
    /* 0x08 */ char unk08[8];
    /* 0x10 */ char list[0x10];         /* registration list (0x6F620 call) */
    /* 0x20 */ s32 id;                  /* goes into each block's hdr top byte */
    /* 0x24 */ s32 used;                /* bytes in use */
} Heap;

typedef char HeapBlock_size_check[sizeof(HeapBlock) == 0x10 ? 1 : -1];
typedef char Heap_id_check[(int)&((Heap *)0)->id == 0x20 ? 1 : -1];
typedef char Heap_used_check[(int)&((Heap *)0)->used == 0x24 ? 1 : -1];

#endif
