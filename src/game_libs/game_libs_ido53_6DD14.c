/* IDO 5.3 -O1 donor for gl___osDevMgrMain = libultra __osDevMgrMain
 * (devmgr.c verbatim, 2.0I struct layout). The PI device-manager thread
 * loop: recv on dm->cmdQueue; 64DD LEO block/track transfers get the
 * special path (sectorNum=-1, dramAddr rewind for non-sector mode,
 * LEO_BM_CTL start write, evtQueue completion loop with LEO_ERROR_29
 * reset recovery + PI_STATUS_REG clear), everything else dispatches on
 * mb->hdr.type: DMAREAD/DMAWRITE via dm->dma, EDMAREAD/EDMAWRITE via
 * dm->edma, LOOPBACK echoes. Sibling of the already-landed LEO family
 * donors (6DA74 osCreatePiManager, 6F088 osLeoDiskInit, 6DC0C
 * osCartRomInit). No data relocs: LEO_BM_CTL/LEO_STATUS (0x0500051x)
 * and PI_STATUS_REG (0xA4600010) are absolute literals; the only relocs
 * are the seven blank jals. Spliced into game_libs_post1b2c.c.o via
 * REPLACE_FUNC_BODY (donor relocs imported, resolve to 0 via
 * undefined_syms_auto). */
#include "libultra/os_pi.h"
#include "uso_call.h"

extern s32 gl_func_00000000_recv(void *, OSMesg *, s32);   /* osRecvMesg */
extern s32 gl_func_00000000_send(void *, OSMesg, s32);     /* osSendMesg */
extern s32 gl_func_00000000_ewio(OSPiHandle *, u32, u32);  /* osEPiRawWriteIo */
extern s32 gl_func_00000000_erio(OSPiHandle *, u32, u32 *);/* osEPiRawReadIo */
extern void gl_func_00000000_rgim(u32);                    /* __rmonWriteWordTo */
extern void gl_func_00000000_sgim(u32);                    /* __osSetGlobalIntMask */
extern void gl_func_00000000_yield(void);                  /* osYieldThread */

void gl___osDevMgrMain(void *args)
{
    OSIoMesg *mb;
    OSMesg em;
    OSMesg dummy;
    s32 ret;
    OSDevMgr *dm;
    s32 messageSend;

    messageSend = 0;
    mb = 0;
    ret = 0;
    dm = (OSDevMgr *)args;
    while (1) {
        XCALL(gl_func_00000000_recv, gl_osRecvMesg)(dm->cmdQueue, (OSMesg *)&mb, 1);
        if (mb->piHandle != 0 &&
            mb->piHandle->type == 2 &&
            (mb->piHandle->transferInfo.cmdType == 0 ||
             mb->piHandle->transferInfo.cmdType == 1)) {
            __OSBlockInfo *blockInfo;
            __OSTranxInfo *info;
            info = &mb->piHandle->transferInfo;
            blockInfo = &info->block[info->blockNum];
            info->sectorNum = -1;
            if (info->transferMode != 3) {
                blockInfo->dramAddr = (void *)((u32)blockInfo->dramAddr - blockInfo->sectorSize);
            }
            if (info->transferMode == 2 && mb->piHandle->transferInfo.cmdType == 0)
                messageSend = 1;
            else
                messageSend = 0;
            XCALL(gl_func_00000000_recv, gl_osRecvMesg)(dm->acsQueue, &dummy, 1);
            XCALL(gl_func_00000000_rgim, gl___osResetGlobalIntMask)(0x00100401);
            XCALL(gl_func_00000000_ewio, gl_osEPiRawWriteIo)(mb->piHandle, 0x05000510, info->bmCtlShadow | 0x80000000);
            while (1) {
                XCALL(gl_func_00000000_recv, gl_osRecvMesg)(dm->evtQueue, &em, 1);
                info = &mb->piHandle->transferInfo;
                blockInfo = &info->block[info->blockNum];
                if (blockInfo->errStatus == 29) {
                    u32 stat;
                    XCALL(gl_func_00000000_ewio, gl_osEPiRawWriteIo)(mb->piHandle, 0x05000510, info->bmCtlShadow | 0x10000000);
                    XCALL(gl_func_00000000_ewio, gl_osEPiRawWriteIo)(mb->piHandle, 0x05000510, info->bmCtlShadow);
                    XCALL(gl_func_00000000_erio, gl_osEPiRawReadIo)(mb->piHandle, 0x05000508, &stat);
                    if (stat & 0x02000000) {
                        XCALL(gl_func_00000000_ewio, gl_osEPiRawWriteIo)(mb->piHandle, 0x05000510, info->bmCtlShadow | 0x01000000);
                    }
                    blockInfo->errStatus = 4;
                    *(volatile u32 *)0xA4600010 = 2;
                    XCALL(gl_func_00000000_sgim, gl___osSetGlobalIntMask)(0x00100C01);
                }
                XCALL(gl_func_00000000_send, gl_osSendMesg)(mb->hdr.retQueue, mb, 0);
                if (messageSend != 1)
                    break;
                if (mb->piHandle->transferInfo.block[0].errStatus != 0)
                    break;
                messageSend = 0;
            }
            XCALL(gl_func_00000000_send, gl_osSendMesg)(dm->acsQueue, 0, 0);
            if (mb->piHandle->transferInfo.blockNum == 1)
                XCALL(gl_func_00000000_yield, gl_osYieldThread)();
        } else {
            switch (mb->hdr.type) {
            case 11: /* OS_MESG_TYPE_DMAREAD */
                XCALL(gl_func_00000000_recv, gl_osRecvMesg)(dm->acsQueue, &dummy, 1);
                ret = dm->dma(0, mb->devAddr, mb->dramAddr, mb->size);
                break;
            case 12: /* OS_MESG_TYPE_DMAWRITE */
                XCALL(gl_func_00000000_recv, gl_osRecvMesg)(dm->acsQueue, &dummy, 1);
                ret = dm->dma(1, mb->devAddr, mb->dramAddr, mb->size);
                break;
            case 15: /* OS_MESG_TYPE_EDMAREAD */
                XCALL(gl_func_00000000_recv, gl_osRecvMesg)(dm->acsQueue, &dummy, 1);
                ret = dm->edma(mb->piHandle, 0, mb->devAddr, mb->dramAddr, mb->size);
                break;
            case 16: /* OS_MESG_TYPE_EDMAWRITE */
                XCALL(gl_func_00000000_recv, gl_osRecvMesg)(dm->acsQueue, &dummy, 1);
                ret = dm->edma(mb->piHandle, 1, mb->devAddr, mb->dramAddr, mb->size);
                break;
            case 10: /* OS_MESG_TYPE_LOOPBACK */
                XCALL(gl_func_00000000_send, gl_osSendMesg)(mb->hdr.retQueue, mb, 0);
                ret = -1;
                break;
            case 13: /* OS_MESG_TYPE_VRETRACE / COUNTER (13 / 14): the table spans 10..16 (sltiu 7), these two -> default (libultra 2.0 numbering, as the kernel copy; the ROM table at kernel 0x8000A770) */
            case 14:
            default:
                ret = -1;
                break;
            }
            if (ret == 0) {
                XCALL(gl_func_00000000_recv, gl_osRecvMesg)(dm->evtQueue, &em, 1);
                XCALL(gl_func_00000000_send, gl_osSendMesg)(mb->hdr.retQueue, mb, 0);
                XCALL(gl_func_00000000_send, gl_osSendMesg)(dm->acsQueue, 0, 0);
            }
        }
    }
}
