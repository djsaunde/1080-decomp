/* IDO 7.1 -O2 donor for AudioLoad_SyncLoad (0x1F4 / 125 words) -- resource
 * acquire (OoT AudioLoad_SyncLoad): begin (baked jal 0x381B0) short-circuits, else fetch the table
 * entry (baked jal 0x38204), allocate by mode (5-entry switch), fill it,
 * then notify per a0. Word-exact standalone apart from relocations.
 *
 * WIRED via REPLACE_FUNC_BODY donor splice into game_libs_post.c.o (the
 * 29CCC / 343F4 recipe): the C switch puts its 5-entry jumptable in the
 * donor's local .rodata; the splice renames that reloc to
 * AudioLoad_SyncLoad_rodata, pinned in undefined_syms_auto.txt to the table
 * the USO ships in game_libs rodata at +0xEA0.
 *
 * Levers (agent-c run 114 decode + run 122 fix): the two leading callees
 * are the baked in-unit jals 0x381B0 / 0x38204 (gl_ref_ pins; the run-114
 * NM called the in-file 23B44 / 23B98, which the raw diff exposed);
 * `case 4: break;` keeps the 5-entry table; the entry is OoT's
 * AudioTableEntry (`&tbl->entries[a1]` gives addu tbl-first, as the old
 * `(char *)((int)tbl + a1 * 0x10)` did; agent-c run 539); the size is computed into one local twice
 * (the target keeps the dead first store); `if (mode == 0) k = 5; else
 * k = 2;` gives the b-around-b tail. */
#include "common.h"
#include "uso_call.h"
#include "audio.h" /* AudioTable / AudioTableEntry */

extern int gl_func_00000000();
extern int gl_ref_000381B0();
extern int gl_ref_00038204();

int AudioLoad_SyncLoad(int a0, int a1, int *a2) {
    int size;
    AudioTable *tbl;
    AudioTableEntry *e;
    int kind;
    int mode;
    int src;
    int r;
    int k;

    r = gl_ref_000381B0(a0, a1, a2);
    if (r != 0) {
        *a2 = 0;
        k = 2;
    } else {
        tbl = (AudioTable *)gl_ref_00038204(a0); /* AudioLoad_GetLoadTable */
        e = &tbl->entries[a1];
        size = e->size;
        size = (size + 0xF) & ~0xF;
        kind = e->medium;
        mode = e->cachePolicy;
        src = e->romAddr;
        switch (mode) {
        case 0:
            r = CALL(AudioHeap_AllocPermanent)(a0, a1, size);
            if (r == 0) return r;
            break;
        case 1:
            r = CALL(AudioHeap_AllocCached)(a0, size, 1, a1);
            if (r == 0) return r;
            break;
        case 2:
            r = CALL(AudioHeap_AllocCached)(a0, size, 0, a1);
            if (r == 0) return r;
            break;
        case 3:
        case 4:
            r = CALL(AudioHeap_AllocCached)(a0, size, 2, a1);
            if (r == 0) return r;
            break;
        }
        *a2 = 1;
        if (kind == 1) {
            CALL(AudioLoad_SyncDmaUnkMedium)(src, r, size, tbl->header.unkMediumParam);
        } else {
            CALL(AudioLoad_SyncDma)(src, r, size, kind);
        }
        if (mode == 0) {
            k = 5;
        } else {
            k = 2;
        }
    }
    switch (a0) {
    case 0:
        CALL(AudioLoad_SetSeqLoadStatus)(a1, k);
        break;
    case 1:
        CALL(AudioLoad_SetFontLoadStatus)(a1, k);
        break;
    case 2:
        CALL(AudioLoad_SetSampleFontLoadStatus)(a1, k);
        break;
    }
    return r;
}
