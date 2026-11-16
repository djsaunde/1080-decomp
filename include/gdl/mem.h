#ifndef GDL_MEM_H
#define GDL_MEM_H

#include "node.h" /* same directory: the NCC donors include this header by relative path */

/* libgdl heap ("Mem::Alloc" / "Mem::Free", game_libs 0x5AFD4-0x5B90C in
 * src/game_libs/game_libs_post0b.c). A heap is two circular doubly linked
 * block lists: the free list (its sentinel is the heap head itself, so the
 * allocators take the heap as a MemBlock *) and the list of allocated blocks
 * at +0x10. Every block has a 16-byte MemBlock header in front of its payload:
 *   hdr   top byte = owner id (0xFF while free), low 24 bits = payload size
 *         in 16-byte units;
 *   magic MEM_MAGIC_ALLOC while allocated, MEM_MAGIC_FREE after a split/free
 *         (Mem_checkAlloc / Mem_checkFree report "Bad Mem::Alloc/Free").
 * heap_new (0x41148, "New(): Cant allocate %dk") is the front end that
 * mem_new / mem_newAligned call; its debug print reads the owner id from the
 * header in front of the returned payload.
 * Mem_allocLow walks the free list forward (first fit, split from the front),
 * Mem_allocHigh walks it backwards and carves from the end of the block. */

#define MEM_MAGIC_ALLOC 0x12345678
#define MEM_MAGIC_FREE  0x87654321

/* owner id of a block (the heap id; 0xFF while free) */
#define MEM_BLOCK_OWNER(b) ((b)->hdr >> 24)

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
    /* 0x28 */ u32 size;            /* heap size in bytes: heap_printStats "%dk/%dk used/size" (gMainHeap is 44 bytes) */
} MemHeap;

typedef char MemBlock_size_check[sizeof(MemBlock) == 0x10 ? 1 : -1];
#ifndef __cplusplus /* edg (NCC donors): &((T *)0)->f is not an integral constant expression */
typedef char MemHeap_usedList_check[(int)&((MemHeap *)0)->usedList == 0x10 ? 1 : -1];
typedef char MemHeap_id_check[(int)&((MemHeap *)0)->id == 0x20 ? 1 : -1];
typedef char MemHeap_usedSize_check[(int)&((MemHeap *)0)->usedSize == 0x24 ? 1 : -1];
#endif

/* Memory accounting (game_libs 0x5FE14-0x60370, MemTracker_* / memtrack_*).
 * A MemTracker (0x44) is one named section ("Total", "BootUp", ...) of a tree:
 * memtrack_begin(name) finds or appends the child of the open tracker, makes it
 * the open one (saving the previous in prevOpen) and records a start mark from
 * the heap; the end call folds (endMark - startMark) into bytes
 * (MemTrackerMgr_hostioParms does the same for the root). MemTracker_printTree /
 * MemTracker_printEntry print bytes / 1024 as KB. Trackers are 68-byte records
 * of the manager's pool (MemTracker_listAppend; ctor alloc(80 * 68)). */
typedef struct MemTracker {
    /* 0x00 */ s32 startMark;       /* heap mark at begin */
    /* 0x04 */ s32 endMark;
    /* 0x08 */ u32 bytes;           /* accumulated endMark - startMark */
    /* 0x0C */ f32 kb;              /* bytes / 1024, MemTracker_printTree */
    /* 0x10 */ s32 heapId;          /* ctor: blank call; MemTracker_walk_00060318 zeroes bytes for a matching id */
    /* 0x14 */ struct MemTracker *sibling; /* next child of the parent */
    /* 0x18 */ struct MemTracker *children;
    /* 0x1C */ struct MemTracker *parent;
    /* 0x20 */ struct MemTracker *prevOpen; /* open tracker before memtrack_begin */
    /* 0x24 */ char name[0x20];
} MemTracker;

/* MemTrackerMgr (0x48, MemTrackerMgr_ctor 0x60370, sMemTrackerMgrName): a Node
 * holding the tracker pool, the open tracker and the root. */
typedef struct MemTrackerPool {
    /* 0x00 */ MemTracker *items;   /* alloc(5440) = 80 records */
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s32 capacity;        /* 80 */
    /* 0x0C */ s32 count;
    /* 0x10 */ s32 unk10;
} MemTrackerPool;

typedef struct MemTrackerMgr {
    /* 0x00 */ Node node;
    /* 0x2C */ MemTrackerPool pool;
    /* 0x40 */ MemTracker *open;    /* innermost open section (MemTrackerMgr_clearList zeroes both) */
    /* 0x44 */ MemTracker *root;
} MemTrackerMgr;

typedef char MemTracker_size_check[sizeof(MemTracker) == 0x44 ? 1 : -1];
#ifndef __cplusplus /* edg (NCC donors): &((T *)0)->f is not an integral constant expression */
typedef char MemTracker_name_check[(int)&((MemTracker *)0)->name == 0x24 ? 1 : -1];
#endif
typedef char MemTrackerMgr_size_check[sizeof(MemTrackerMgr) == 0x48 ? 1 : -1];
#ifndef __cplusplus /* edg (NCC donors): &((T *)0)->f is not an integral constant expression */
typedef char MemTrackerMgr_root_check[(int)&((MemTrackerMgr *)0)->root == 0x44 ? 1 : -1];
#endif

#define MEMTRACKER(p) ((MemTracker *)(p))
#define MEMTRACKERMGR(p) ((MemTrackerMgr *)(p))

/* DynArr (0xC; DynArr_ctor_00066514 allocs it when this == 0): a fixed block of 8-byte
 * slots handed out in order. DynArr_push_0006656C returns &items[count++] while
 * count < capacity - 1, else 0 (full). */
typedef struct DynArr {
    /* 0x0 */ char *items;          /* alloc(capacity * 8) */
    /* 0x4 */ s32 count;
    /* 0x8 */ s32 capacity;
} DynArr;

/* DynArrPool: list_allocNode_000665B4 takes 8-byte slots from the current DynArr and
 * starts a new one (DynArr_ctor(0, chunkSize)) when it is full or there is none.
 * Full chunks are dropped, not freed. */
typedef struct DynArrPool {
    /* 0x0 */ s32 chunkSize;        /* slots per DynArr */
    /* 0x4 */ DynArr *cur;
    /* 0x8 */ s32 nChunks;          /* DynArrs created */
} DynArrPool;

/* GdlList: a singly linked list of 8-byte cells {data, next}, the slots list_allocNode_000665B4
 * takes from a DynArrPool; list_append(&list->head, data) appends, list_removeKey_00066620 unlinks
 * by data. The head is a 4-byte Cfront class whose inline ctor zeroes it: DNode_ctor builds two in
 * place (`this + 0x39C != 0 || (p = new(4))`, then `p->head = 0`), DNode.attached (+0x39C, walked by
 * DNode_updateAttached as {Friction *, next} cells) and DNode.unk3A0. */
typedef struct GdlListCell {
    /* 0x0 */ char *data;
    /* 0x4 */ struct GdlListCell *next;
} GdlListCell;

typedef struct GdlList {
    /* 0x0 */ GdlListCell *head;
} GdlList;

typedef char DynArr_size_check[sizeof(DynArr) == 0xC ? 1 : -1];

#define DYNARR(p) ((DynArr *)(p))
#define DYNARRPOOL(p) ((DynArrPool *)(p))

#endif /* GDL_MEM_H */
