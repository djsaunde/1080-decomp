#include "common.h"

/* IDO 7.1 -O2 donor for gl_func_000240A0 (0x290 / 164 words) = OoT's
 * AudioLoad_AsyncLoadInner (load.c), the older form without
 * AudioLoad_GetRealTableIndex (id is used directly) and without the
 * MEDIUM_UNK branch. Spliced into game_libs_post.c.o via REPLACE_FUNC_BODY;
 * the splice renames the donor-local .rodata jumptable reloc (cachePolicy
 * switch, 5 entries, 3 and 4 shared) to gl_func_000240A0_rodata, pinned to the
 * USO table at +0xEB4. gAudioCtx load-status arrays: seq 0x2C70, font 0x2C40,
 * sample 0x2C10. Blank-jal callees are gl_func_00000000; the baked jals are
 * the pinned gl_ref_000381B0 (SearchCaches), gl_ref_00038204 (GetLoadTable),
 * gl_ref_0003959C (StartAsyncLoad). */
typedef struct { u32 romAddr; u32 size; s8 medium; s8 cachePolicy; s16 shortData1; s16 shortData2; s16 shortData3; } gl_240A0_TableEntry;
typedef struct { s16 numEntries; s16 unkMediumParam; u32 romAddr; char pad[8]; gl_240A0_TableEntry entries[1]; } gl_240A0_Table;
typedef struct {
    char pad0[0x2C10]; u8 sampleFontLoadStatus[0x30]; u8 fontLoadStatus[0x30]; u8 seqLoadStatus[0x80];
} gl_240A0_Ctx;
#define ALIGN16(val) (((val) + 0xF) & ~0xF)
extern int D_00000000;
#define gAudioCtx (*(gl_240A0_Ctx *)&D_00000000)
extern int gl_func_00000000();
extern void *gl_ref_000381B0();
extern gl_240A0_Table *gl_ref_00038204();
extern int gl_ref_0003959C();
#define AudioLoad_SearchCaches gl_ref_000381B0
#define AudioLoad_GetLoadTable gl_ref_00038204
#define AudioLoad_StartAsyncLoad gl_ref_0003959C
#define osSendMesg gl_func_00000000
#define AudioHeap_AllocPermanent (void *)gl_func_00000000
#define AudioHeap_AllocCached (void *)gl_func_00000000
#define AudioLoad_SetSeqLoadStatus gl_func_00000000
#define AudioLoad_SetFontLoadStatus gl_func_00000000
#define AudioLoad_SetSampleFontLoadStatus gl_func_00000000

void *gl_func_000240A0(s32 tableType, s32 id, s32 nChunks, s32 retData, void *retQueue) {
    u32 size;
    gl_240A0_Table *sampleBankTable;
    void *ramAddr;
    s32 medium;
    s8 cachePolicy;
    u32 devAddr;
    s32 status;

    switch (tableType) {
        case 0:
            if (gAudioCtx.seqLoadStatus[id] == 1) {
                return NULL;
            }
            break;
        case 1:
            if (gAudioCtx.fontLoadStatus[id] == 1) {
                return NULL;
            }
            break;
        case 2:
            if (gAudioCtx.sampleFontLoadStatus[id] == 1) {
                return NULL;
            }
            break;
    }

    ramAddr = AudioLoad_SearchCaches(tableType, id);
    if (ramAddr != NULL) {
        status = 2;
        osSendMesg(retQueue, retData << 0x18, 0);
    } else {
        sampleBankTable = AudioLoad_GetLoadTable(tableType);
        size = sampleBankTable->entries[id].size;
        size = ALIGN16(size);
        medium = sampleBankTable->entries[id].medium;
        cachePolicy = sampleBankTable->entries[id].cachePolicy;
        devAddr = sampleBankTable->entries[id].romAddr;
        status = 2;
        switch (cachePolicy) {
            case 0:
                ramAddr = AudioHeap_AllocPermanent(tableType, id, size);
                if (ramAddr == NULL) {
                    return ramAddr;
                }
                status = 5;
                break;
            case 1:
                ramAddr = AudioHeap_AllocCached(tableType, size, 1, id);
                if (ramAddr == NULL) {
                    return ramAddr;
                }
                break;
            case 2:
                ramAddr = AudioHeap_AllocCached(tableType, size, 0, id);
                if (ramAddr == NULL) {
                    return ramAddr;
                }
                break;
            case 3:
            case 4:
                ramAddr = AudioHeap_AllocCached(tableType, size, 2, id);
                if (ramAddr == NULL) {
                    return ramAddr;
                }
                break;
        }

        AudioLoad_StartAsyncLoad(devAddr, ramAddr, size, medium, nChunks, retQueue,
                                 (retData << 0x18) | (tableType << 0x10) | (id << 8) | status);
        status = 1;
    }

    switch (tableType) {
        case 0:
            AudioLoad_SetSeqLoadStatus(id, status);
            break;
        case 1:
            AudioLoad_SetFontLoadStatus(id, status);
            break;
        case 2:
            AudioLoad_SetSampleFontLoadStatus(id, status);
            break;
    }

    return ramAddr;
}
