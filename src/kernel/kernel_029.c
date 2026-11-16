#include "common.h"
#include "libultra/rmon.h"

extern void func_800073F8(void*, s32, s32);

/* __rmonGetRegionCount (rmonmem.c): rmon dispatchTable[12]. Typed on rmon.h (agent-a run 531;
 * the private RmonMsg / RmonHdr were KKObjectRequest / KKNumberEvent). */
s32 __rmonGetRegionCount(KKObjectRequest* msg) {
    KKObjectRequest* p = msg;
    KKNumberEvent hdr;
    hdr.header.code = p->header.code;
    hdr.header.error = TV_ERROR_NO_ERROR;
    hdr.object = p->object;
    if (p->header.method == RMON_RSP) {
        hdr.number = 2;
    } else {
        hdr.number = 5;
    }
    func_800073F8(&hdr, sizeof(hdr), KK_TYPE_REPLY);
    return 0;
}

/* __rmonGetRegions (rmonmem.c): rmon dispatchTable[13]. The reply in __rmonUtilityBuffer is a
 * KKRegionEvent: regions[0] SP DMEM, [1] SP IMEM, and for the CPU also [2] the RDRAM image
 * 0x88200000 (0x6130 bytes), [3] 4 (0x200000 bytes), [4] 0x04002000 (0x800000), [5] 0x88206130
 * (0x9000). flags = 1 read | 2 write | 4 execute. */
extern s32 __rmonUtilityBuffer;

s32 __rmonGetRegions(KKObjectRequest* msg) {
    KKObjectRequest* p = msg;
    KKRegionEvent* buf = (KKRegionEvent*)&__rmonUtilityBuffer;
    s32 count;

    if (p->header.method == RMON_RSP) {
        count = 2;
    } else {
        count = 6;
    }
    buf->header.length = count * 16 + 0x24;
    buf->header.code = p->header.code;
    buf->header.error = TV_ERROR_NO_ERROR;
    buf->object = p->object;
    buf->number = count;
    buf->regions[1].vaddr = 0x04001000;
    buf->regions[1].size = 0x1000;
    buf->regions[1].flags = 7;
    buf->regions[1].paddr = 0x04001000;
    buf->regions[0].vaddr = 0x04000000;
    buf->regions[0].size = 0x1000;
    buf->regions[0].flags = 3;
    buf->regions[0].paddr = 0x04000000;
    if (count >= 3) {
        buf->regions[2].vaddr = 0x88200000;
        buf->regions[2].size = 0x6130;
        buf->regions[2].flags = 5;
        buf->regions[2].paddr = 0;
        buf->regions[3].vaddr = 4;
        buf->regions[3].size = 0x200000;
        buf->regions[3].flags = 3;
        buf->regions[3].paddr = 0;
        buf->regions[4].vaddr = 0x04002000;
        buf->regions[4].size = 0x800000;
        buf->regions[4].flags = 3;
        buf->regions[4].paddr = 0;
        buf->regions[5].vaddr = 0x88206130;
        buf->regions[5].size = 0x9000;
        buf->regions[5].flags = 3;
        buf->regions[5].paddr = 0;
    }
    func_800073F8(buf, buf->header.length, KK_TYPE_REPLY);
    return 0;
}
