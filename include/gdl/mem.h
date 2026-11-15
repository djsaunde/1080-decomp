#ifndef GDL_MEM_H
#define GDL_MEM_H

/* libgdl heap ("Mem::Alloc" / "Mem::Free", game_libs 0x5AFD4-0x5B90C in
 * src/game_libs/game_libs_post0b.c). A heap is two circular doubly linked
 * block lists: the free list (its sentinel is the heap head itself, so the
 * allocators take the heap as a MemBlock *) and the list of allocated blocks
 * at +0x10. Every block has a 16-byte MemBlock header in front of its payload:
 *   hdr   top byte = owner id (0xFF while free), low 24 bits = payload size
 *         in 16-byte units;
 *   magic MEM_MAGIC_ALLOC while allocated, MEM_MAGIC_FREE after a split/free
 *         (Mem_checkAlloc / Mem_checkFree report "Bad Mem::Alloc/Free").
 * Mem_allocLow walks the free list forward (first fit, split from the front),
 * Mem_allocHigh walks it backwards and carves from the end of the block. */

#define MEM_MAGIC_ALLOC 0x12345678
#define MEM_MAGIC_FREE  0x87654321

typedef struct MemBlock {
    /* 0x0 */ u32 hdr;
    /* 0x4 */ struct MemBlock *next;
    /* 0x8 */ struct MemBlock *prev;
    /* 0xC */ u32 magic;
} MemBlock;

typedef struct MemHeap {
    /* 0x00 */ MemBlock freeList;   /* sentinel of the free list */
    /* 0x10 */ MemBlock usedList;   /* sentinel of the allocated-block list */
    /* 0x20 */ s32 id;              /* stored in the top byte of each block's hdr */
    /* 0x24 */ s32 usedSize;        /* allocated 16-byte units */
} MemHeap;

typedef char MemBlock_size_check[sizeof(MemBlock) == 0x10 ? 1 : -1];
typedef char MemHeap_usedList_check[(int)&((MemHeap *)0)->usedList == 0x10 ? 1 : -1];
typedef char MemHeap_id_check[(int)&((MemHeap *)0)->id == 0x20 ? 1 : -1];
typedef char MemHeap_usedSize_check[(int)&((MemHeap *)0)->usedSize == 0x24 ? 1 : -1];

#endif /* GDL_MEM_H */
