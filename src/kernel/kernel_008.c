#include "common.h"

typedef struct {
    u8 pad[0x74];
} OSPiHandle;

extern OSPiHandle CartRomHandle;
extern void* __osPiTable;
extern u8 CartRomHandle_type;
extern u8 CartRomHandle_latency;
extern u8 CartRomHandle_pageSize;
extern u8 CartRomHandle_relDuration;
extern u8 CartRomHandle_pulse;
extern u8 CartRomHandle_domain;
extern volatile u32 CartRomHandle_baseAddress;
extern u32 CartRomHandle_speed;
extern s32 func_80002DB0(s32, u32*);
extern void func_800030D0(void*, s32);
extern s32 func_800066B0(void);
extern void func_800066D0(s32);

/* osCartRomInit = libultra osCartRomInit (cartrominit.c).
 * 2026-07-10 CRACKED EXACT 60/60: the documented "one extra lui" cap (the
 * pageSize/relDuration sb pair sharing one $at lui) falls to the TU-DEFINED
 * shared-$at lever — the -O1 donor kernel_008_o1_58C0.c defines a 2-byte
 * struct at CartRomHandle_pageSize so that pair batches under one lui; all other fields
 * stay extern scalars (statement-granular own-lui stores). CartRomHandle is
 * typed `extern void *` in the donor so the `next = __osPiTable` store emits
 * the direct lui-$at global-store form. Donor bytes replace the body below
 * in the .o via REPLACE_FUNC_BODY (see Makefile); this -O2 host body is the
 * semantic reference.
 * (Historical: 2026-06-10 relayout retracted an earlier FALSE match that
 * emitted 0xE0 vs ROM's 0xF4.) */
OSPiHandle* osCartRomInit(void) {
    u32 domain;
    u32 saveMask;

    domain = 0;
    if (CartRomHandle_baseAddress == 0xB0000000) {
        return &CartRomHandle;
    }

    CartRomHandle_type = 0;
    CartRomHandle_baseAddress = 0xB0000000;
    func_80002DB0(0, &domain);
    CartRomHandle_latency = domain & 0xFF;
    CartRomHandle_pulse = (domain >> 8) & 0xFF;
    CartRomHandle_pageSize = (domain >> 0x10) & 0xF;
    CartRomHandle_relDuration = (domain >> 0x14) & 0xF;
    CartRomHandle_domain = 0;
    CartRomHandle_speed = 0;
    func_800030D0((u8*)&CartRomHandle + 0x14, 0x60);
    saveMask = func_800066B0();
    *(void**)&CartRomHandle = __osPiTable;
    __osPiTable = &CartRomHandle;
    func_800066D0(saveMask);
    return &CartRomHandle;
}
/* (donor bytes replace the body above in the .o; see Makefile) */
