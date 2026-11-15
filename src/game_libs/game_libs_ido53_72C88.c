#include "uso_call.h"
/* game_libs ido53 carve unit: corrupted_init, the first -Olimit-fallback
 * match (IDO 5.3, -O1 -Olimit 1 -- uopt skipped so the emit is -O0-shaped
 * with homed args and per-use reloads, but the -O1 pipeline still fills
 * delay slots). 1080-custom osPfsChecker-family helper: builds a 256-byte
 * cross-bank inode link matrix. Match-critical shapes: reverse-declaration-
 * order locals (i lowest at 0x12C ... ret highest? no: i declared FIRST ->
 * 0x12C), &&-combined skip conditions (keeps unit.bank/bank live into the
 * marking block), |= (single address computation), (map + idx)[0x101]
 * (addu operand order), and ++bank inside the while condition (u8
 * truncation andi without a reload). See docs/IDO_CODEGEN -Olimit entry. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef int s32;

typedef struct {
    int status; void *queue; int channel;
    u8 id[32]; u8 label[32];
    int version; int dir_size; int inode_table; int minode_table;
    int dir_table; int inode_start_page; u8 banks; u8 activebank;
} OSPfs;

typedef union {
    struct { u8 bank; u8 page; } inode_t;
    u16 ipage;
} __OSInodeUnit;

typedef struct {
    __OSInodeUnit inode_page[128];
} __OSInode;

extern int gl_func_00000000();

s32 corrupted_init(OSPfs *pfs, u8 *map) {
    s32 i;
    s32 idx;
    s32 startpage;
    u8 bank;
    __OSInodeUnit unit;
    __OSInode inode;
    s32 ret;

    for (i = 0; i < 256; i++) {
        map[i + 0x101] = 0;
    }
    map[0x100] = 0xFF;
    bank = 0;
    if (pfs->banks > 0) {
        do {
            if (bank > 0) {
                startpage = 1;
            } else {
                startpage = pfs->inode_start_page;
            }
            ret = CALL(__osPfsRWInode)(pfs, &inode, 0, bank);
            if (ret != 0) {
                if (ret != 3) {
                    return ret;
                }
            }
            for (i = startpage; i < 128; i++) {
                unit = inode.inode_page[i];
                if (unit.ipage >= pfs->inode_start_page && unit.inode_t.bank != bank) {
                    idx = unit.inode_t.page / 4 + (unit.inode_t.bank % 8) * 32;
                    (map + idx)[0x101] |= 1 << (bank % 8);
                }
            }
        } while (++bank < pfs->banks);
    }
    return 0;
}

/* corrupted = `corrupted` (io/pfschecker.c static helper) -- the
 * pfschecker.c trio is COMPLETE: osPfsChecker (725C4, -O1) /
 * corrupted_init (72C88, above) / corrupted (here). Verbatim libreultra
 * source, 122/122 first try; byte-identical under BOTH -O1 and the
 * -Olimit fallback, so it lives happily in this -Olimit unit. Literals:
 * PFS_ERR_INCONSISTENT=3, PFS_ERR_NEW_PACK=2 (this build). Its 4-byte
 * trailing pad sidecar stays at the head of post2b_c. */
typedef struct {
    __OSInode inode; u8 bank; u8 map[256];
} __OSInodeCache;

s32 corrupted(OSPfs *pfs, __OSInodeUnit fpage, __OSInodeCache *cache)
{
    int j;
    int n;
    int hit;
    u8 bank;
    int offset;
    s32 ret;

    hit = 0;
    ret = 0;
    n = (fpage.inode_t.page / 4) + (fpage.inode_t.bank % 8) * 32;
    for (bank = 0; bank < pfs->banks; bank++)
    {
        if (bank > 0)
            offset = 1;
        else
            offset = pfs->inode_start_page;
        if (bank == fpage.inode_t.bank || cache->map[n] & (1 << (bank % 8)))
        {
            if (bank != cache->bank)
            {
                ret = CALL(__osPfsRWInode)(pfs, &cache->inode, 0, bank);
                if (ret != 0 && ret != 3)
                    return ret;
                cache->bank = bank;
            }

            for (j = offset; hit < 2 && (j < 128); j++)
            {
                if (cache->inode.inode_page[j].ipage == fpage.ipage)
                    hit++;
            }
            if (1 < hit)
                return 2;
        }
    }
    return hit;
}

/* gl_osAiGetLength: AI_LEN_REG-class volatile reader (0xA4500004),
 * 3 insns + the trailing pad nop that places gl_osPiWriteIo at 0x73034.
 * Moved here from post2b_c so the pad comes from cc's natural function
 * padding (the GLOBAL_ASM pad sidecar 16-aligns and breaks placement when
 * the fn is unit-first). */
int gl_osAiGetLength(void) {
    return *(volatile int *)0xA4500004;
}
