#ifndef LIBULTRA_OS_PI_H
#define LIBULTRA_OS_PI_H

/* libultra PI device manager / I/O message types (2.0I PR/os.h + piint.h,
 * references/libreultra), shared by the kernel and game_libs copies. Opt-in
 * like os_message.h: a TU that still carries a private copy of any of these
 * typedefs must drop it before including this (duplicate typedef = cfe error).
 * Offsets and sizes are asserted below. */

#include "libultra/os_message.h"

typedef struct {
    /* 0x00 */ u32 errStatus;
    /* 0x04 */ void *dramAddr;
    /* 0x08 */ void *C2Addr;
    /* 0x0C */ u32 sectorSize;
    /* 0x10 */ u32 C1ErrNum;
    /* 0x14 */ u32 C1ErrSector[4];
} __OSBlockInfo; /* size 0x24 */

typedef struct {
    /* 0x00 */ u32 cmdType;
    /* 0x04 */ u16 transferMode;
    /* 0x06 */ u16 blockNum;
    /* 0x08 */ s32 sectorNum;
    /* 0x0C */ u32 devAddr;
    /* 0x10 */ u32 bmCtlShadow;
    /* 0x14 */ u32 seqCtlShadow;
    /* 0x18 */ __OSBlockInfo block[2];
} __OSTranxInfo; /* size 0x60 */

typedef struct OSPiHandle_s {
    /* 0x00 */ struct OSPiHandle_s *next;
    /* 0x04 */ u8 type;
    /* 0x05 */ u8 latency;
    /* 0x06 */ u8 pageSize;
    /* 0x07 */ u8 relDuration;
    /* 0x08 */ u8 pulse;
    /* 0x09 */ u8 domain;
    /* 0x0C */ u32 baseAddress;
    /* 0x10 */ u32 speed;
    /* 0x14 */ __OSTranxInfo transferInfo;
} OSPiHandle; /* size 0x74 */

typedef struct {
    /* 0x00 */ u16 type;
    /* 0x02 */ u8 pri;
    /* 0x03 */ u8 status;
    /* 0x04 */ OSMesgQueue *retQueue;
} OSIoMesgHdr; /* size 0x8 */

typedef struct {
    /* 0x00 */ OSIoMesgHdr hdr;
    /* 0x08 */ void *dramAddr;
    /* 0x0C */ u32 devAddr;
    /* 0x10 */ u32 size;
    /* 0x14 */ OSPiHandle *piHandle;
} OSIoMesg; /* size 0x18 */

typedef struct {
    /* 0x00 */ s32 active;
    /* 0x04 */ OSThread *thread;
    /* 0x08 */ OSMesgQueue *cmdQueue;
    /* 0x0C */ OSMesgQueue *evtQueue;
    /* 0x10 */ OSMesgQueue *acsQueue;
    /* 0x14 */ s32 (*dma)(s32, u32, void *, u32);
    /* 0x18 */ s32 (*edma)(OSPiHandle *, s32, u32, void *, u32);
} OSDevMgr; /* size 0x1C */

#define OS_READ 0
#define OS_WRITE 1
#define OS_MESG_PRI_NORMAL 0
#define OS_MESG_PRI_HIGH 1
#define OS_MESG_TYPE_LOOPBACK 10
#define OS_MESG_TYPE_DMAREAD 11
#define OS_MESG_TYPE_DMAWRITE 12
#define OS_MESG_TYPE_VRETRACE 13
#define OS_MESG_TYPE_COUNTER 14
#define OS_MESG_TYPE_EDMAREAD 15
#define OS_MESG_TYPE_EDMAWRITE 16

#define OS_PI_ASSERT(name, cond) typedef char os_pi_assert_##name[(cond) ? 1 : -1]
OS_PI_ASSERT(blk_size, sizeof(__OSBlockInfo) == 0x24);
OS_PI_ASSERT(tranx_block, (int)&((__OSTranxInfo *)0)->block == 0x18);
OS_PI_ASSERT(tranx_size, sizeof(__OSTranxInfo) == 0x60);
OS_PI_ASSERT(pih_base, (int)&((OSPiHandle *)0)->baseAddress == 0x0C);
OS_PI_ASSERT(pih_tranx, (int)&((OSPiHandle *)0)->transferInfo == 0x14);
OS_PI_ASSERT(pih_size, sizeof(OSPiHandle) == 0x74);
OS_PI_ASSERT(iomesg_hdr_size, sizeof(OSIoMesgHdr) == 0x8);
OS_PI_ASSERT(iomesg_pih, (int)&((OSIoMesg *)0)->piHandle == 0x14);
OS_PI_ASSERT(iomesg_size, sizeof(OSIoMesg) == 0x18);
OS_PI_ASSERT(devmgr_edma, (int)&((OSDevMgr *)0)->edma == 0x18);
OS_PI_ASSERT(devmgr_size, sizeof(OSDevMgr) == 0x1C);

#endif /* LIBULTRA_OS_PI_H */
