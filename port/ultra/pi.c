/* port/ultra: PI (cartridge bus): ROM DMA from the in-memory .z64 image and
 * the SRAM save.
 *
 * Address decoding follows libultra: osPiStartDma / osPiReadIo address the
 * cartridge as osRomBase | devAddr; osEPi* use handle->baseAddress | devAddr.
 * The physical address (& 0x1FFFFFFF) then selects the device:
 *   0x10000000..0x1FBFFFFF  cartridge ROM (domain 1): read-only
 *   0x08000000..0x0FFFFFFF  SRAM (domain 2): 1080's save, PORT_SRAM_SIZE bytes.
 *                           app_initSystem builds this handle (baseAddress
 *                           0xA8000000); fs_gdlReadFile / fs_gdlWriteFile
 *                           (GdlFile kind 2) DMA through it.
 *   0x05000000..0x06FFFFFF  64DD (osDriveRomInit): absent, reads return zeros
 * Data is copied as-is: the ROM is big-endian and so is everything DMA'd from
 * it; byte swapping happens in the readers (plan 4.4). The SRAM image (and so the
 * save file) is the N64's, big-endian: the game's Backup_read / Backup_write convert
 * each record at that boundary (port/include/port_backup.h). osPiReadIo returns the
 * big-endian word as a host integer, as the N64 CPU would see it.
 *
 * Every DMA completes immediately and posts its OSIoMesg to the return queue
 * (OS_MESG_NOBLOCK, as the PI manager does), so a following blocking
 * osRecvMesg never waits. */

#include "ultra_int.h"

#define ROM_PHYS 0x10000000u
#define ROM_PHYS_END 0x1FC00000u
#define SRAM_PHYS 0x08000000u
#define SRAM_PHYS_END 0x10000000u

static u8 sSram[PORT_SRAM_SIZE];
static OSPiHandle sCartHandle;
static OSPiHandle sDriveHandle;
static OSPiHandle *sHandleList;
static OSMesgQueue *sCmdQueue;
static u8 sWarned64DD;
static u8 sWarnedRomRange;

static u32 be32(const u8 *p) {
    return ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | p[3];
}

static void put_be32(u8 *p, u32 v) {
    p[0] = (u8)(v >> 24);
    p[1] = (u8)(v >> 16);
    p[2] = (u8)(v >> 8);
    p[3] = (u8)v;
}

/* Returns the device byte pointer for [phys, phys+size), or NULL. writable: SRAM only. */
static u8 *device_ptr(u32 phys, u32 size, int *writable) {
    *writable = 0;
    if (phys >= ROM_PHYS && phys < ROM_PHYS_END) {
        u32 off = phys - ROM_PHYS;
        if (port_ultra_cfg.rom != NULL && off <= port_ultra_cfg.romSize &&
            size <= port_ultra_cfg.romSize - off) {
            return (u8 *)port_ultra_cfg.rom + off;
        }
        if (!sWarnedRomRange) {
            sWarnedRomRange = 1;
            port_ultra_log("ultra: PI read past the ROM image (0x%08X + 0x%X, image 0x%X); zeros",
                           phys, size, port_ultra_cfg.romSize);
        }
        return NULL;
    }
    if (phys >= SRAM_PHYS && phys < SRAM_PHYS_END) {
        u32 off = phys - SRAM_PHYS;
        if (off <= PORT_SRAM_SIZE && size <= PORT_SRAM_SIZE - off) {
            u8 *base = sSram;
            *writable = 1;
#ifdef PORT64
            /* x32 (plan 21): gcc folds sSram + (phys - SRAM_PHYS) into the immediate sSram - 0x8000000,
             * which an ELF64 R_X86_64_32 cannot hold; keep the base in a register */
            __asm__("" : "+r"(base));
#endif
            return base + off;
        }
        port_ultra_log("ultra: SRAM access out of range (0x%08X + 0x%X)", phys, size);
        return NULL;
    }
    if (!sWarned64DD) {
        sWarned64DD = 1;
        port_ultra_log("ultra: PI access to unmapped 0x%08X (64DD / unknown); zeros", phys);
    }
    return NULL;
}

static int sPiLog = -1;

static s32 transfer(u32 addr, void *dram, u32 size, s32 direction) {
    u32 phys = addr & 0x1FFFFFFF;
    int writable;
    u8 *dev = device_ptr(phys, size, &writable);

    port_ultra_activity++;
    if (sPiLog < 0) {
        sPiLog = getenv("PORT_PI_LOG") != NULL; /* debugging: one line per PI transfer */
    }
    if (sPiLog) {
        port_ultra_log("PI %c 0x%08X 0x%X retrace %u", direction == OS_READ ? 'r' : 'w', (unsigned)addr, (unsigned)size,
                       (unsigned)port_ultra_st.retraces);
    }
    if (direction == OS_READ) {
        port_ultra_st.piBytes += size;
        if (dev != NULL) {
            memcpy(dram, dev, size);
        } else {
            memset(dram, 0, size);
        }
        return 0;
    }
    if (dev == NULL || !writable) {
        port_ultra_log("ultra: PI write to read-only 0x%08X (0x%X bytes) dropped", phys, size);
        return -1;
    }
    memcpy(dev, dram, size);
    port_ultra_st.sramWrites++;
    port_sram_flush();
    return 0;
}

static void complete(OSIoMesg *mb, OSMesgQueue *mq) {
    if (mq != NULL) {
        osSendMesg(mq, (OSMesg)mb, OS_MESG_NOBLOCK);
    }
}

void osCreatePiManager(OSPri pri, OSMesgQueue *cmdQ, OSMesg *cmdBuf, s32 cmdMsgCnt) {
    (void)pri;
    osCreateMesgQueue(cmdQ, cmdBuf, cmdMsgCnt);
    sCmdQueue = cmdQ;
}

OSMesgQueue *osPiGetCmdQueue(void) {
    return sCmdQueue;
}

s32 osPiStartDma(OSIoMesg *mb, s32 pri, s32 direction, u32 devAddr, void *dramAddr, u32 size,
                 OSMesgQueue *mq) {
    s32 ret;

    mb->hdr.type = direction == OS_READ ? OS_MESG_TYPE_DMAREAD : OS_MESG_TYPE_DMAWRITE;
    mb->hdr.pri = (u8)pri;
    mb->hdr.retQueue = mq;
    mb->dramAddr = dramAddr;
    mb->devAddr = devAddr;
    mb->size = size;
    mb->piHandle = NULL;
    ret = transfer(osRomBase | devAddr, dramAddr, size, direction);
    complete(mb, mq);
    return ret;
}

s32 osEPiStartDma(OSPiHandle *h, OSIoMesg *mb, s32 direction) {
    s32 ret;

    mb->hdr.type = direction == OS_READ ? OS_MESG_TYPE_EDMAREAD : OS_MESG_TYPE_EDMAWRITE;
    mb->piHandle = h;
    ret = transfer(h->baseAddress | mb->devAddr, mb->dramAddr, mb->size, direction);
    complete(mb, mb->hdr.retQueue);
    return ret;
}

s32 osPiRawStartDma(s32 direction, u32 devAddr, void *dramAddr, u32 size) {
    return transfer(osRomBase | devAddr, dramAddr, size, direction);
}

s32 osEPiRawStartDma(OSPiHandle *h, s32 direction, u32 devAddr, void *dramAddr, u32 size) {
    return transfer(h->baseAddress | devAddr, dramAddr, size, direction);
}

static s32 read_word(u32 addr, u32 *data) {
    u32 phys = addr & 0x1FFFFFFF;
    int writable;
    u8 *dev = device_ptr(phys, 4, &writable);

    *data = dev != NULL ? be32(dev) : 0;
    return 0;
}

static s32 write_word(u32 addr, u32 data) {
    u32 phys = addr & 0x1FFFFFFF;
    int writable;
    u8 *dev = device_ptr(phys, 4, &writable);

    if (dev == NULL || !writable) {
        port_ultra_log("ultra: PI word write to read-only 0x%08X dropped", phys);
        return -1;
    }
    put_be32(dev, data);
    port_ultra_st.sramWrites++;
    port_sram_flush();
    return 0;
}

s32 osPiReadIo(u32 devAddr, u32 *data) {
    return read_word(osRomBase | devAddr, data);
}

s32 osPiRawReadIo(u32 devAddr, u32 *data) {
    return read_word(osRomBase | devAddr, data);
}

s32 osPiWriteIo(u32 devAddr, u32 data) {
    return write_word(osRomBase | devAddr, data);
}

s32 osEPiReadIo(OSPiHandle *h, u32 devAddr, u32 *data) {
    return read_word(h->baseAddress | devAddr, data);
}

s32 osEPiWriteIo(OSPiHandle *h, u32 devAddr, u32 data) {
    return write_word(h->baseAddress | devAddr, data);
}

s32 osPiGetStatus(void) {
    return 0; /* never busy */
}

/* Handle values from libultra osCartRomInit / osDriveRomInit (2.0I). */
OSPiHandle *osCartRomInit(void) {
    sCartHandle.type = 0; /* DEVICE_TYPE_CART */
    sCartHandle.baseAddress = 0xB0000000;
    sCartHandle.latency = 0x40;
    sCartHandle.pulse = 0x12;
    sCartHandle.pageSize = 0x07;
    sCartHandle.relDuration = 0x03;
    sCartHandle.domain = 0;
    sCartHandle.speed = 0;
    osEPiLinkHandle(&sCartHandle);
    return &sCartHandle;
}

OSPiHandle *osDriveRomInit(void) {
    sDriveHandle.type = 1; /* DEVICE_TYPE_BULK */
    sDriveHandle.baseAddress = 0xA6000000;
    sDriveHandle.latency = 0x40;
    sDriveHandle.pulse = 0x07;
    sDriveHandle.pageSize = 0x07;
    sDriveHandle.relDuration = 0x02;
    sDriveHandle.domain = 0;
    sDriveHandle.speed = 0;
    osEPiLinkHandle(&sDriveHandle);
    return &sDriveHandle;
}

s32 osEPiLinkHandle(OSPiHandle *h) {
    OSPiHandle *p;

    for (p = sHandleList; p != NULL; p = p->next) {
        if (p == h) {
            return 0;
        }
    }
    h->next = sHandleList;
    sHandleList = h;
    return 0;
}

u8 *port_sram_image(void) {
    return sSram;
}

int port_sram_flush(void) {
    if (port_ultra_cfg.sramPath == NULL) {
        return 0;
    }
    if (host_file_write_all(port_ultra_cfg.sramPath, sSram, sizeof(sSram)) != 0) {
        port_ultra_log("ultra: could not write the save file %s", port_ultra_cfg.sramPath);
        return -1;
    }
    return 0;
}

/* Load the save file. A missing file is a blank (zeroed) SRAM: the game's
 * Backup checksums reject it and it formats a new save, as on a new cartridge. */
void port_pi_reset(void) {
    void *data;
    u32 size;

    memset(sSram, 0, sizeof(sSram));
    sHandleList = NULL;
    sCmdQueue = NULL;
    sWarned64DD = 0;
    sWarnedRomRange = 0;
    if (port_ultra_cfg.sramPath != NULL &&
        host_file_read_all(port_ultra_cfg.sramPath, &data, &size) == 0) {
        if (size != sizeof(sSram)) {
            port_ultra_log("ultra: save file %s is %u bytes, expected %u; using what fits",
                           port_ultra_cfg.sramPath, (unsigned)size, (unsigned)sizeof(sSram));
        }
        memcpy(sSram, data, size < sizeof(sSram) ? size : sizeof(sSram));
        host_free(data);
    }
}

PORT_ALIAS(osPiStartDma, gl_osPiStartDma);
PORT_ALIAS(osEPiStartDma, gl_osEPiStartDma);
PORT_ALIAS(osPiRawStartDma, gl_osPiRawStartDma);
PORT_ALIAS(osEPiRawStartDma, gl_osEPiRawStartDma);
PORT_ALIAS(osPiReadIo, gl_osPiReadIo);
PORT_ALIAS(osPiRawReadIo, gl_osPiRawReadIo);
PORT_ALIAS(osPiWriteIo, gl_osPiWriteIo);
PORT_ALIAS(osEPiReadIo, gl_osEPiRawReadIo);
PORT_ALIAS(osEPiWriteIo, gl_osEPiRawWriteIo);
PORT_ALIAS(osCartRomInit, gl_osCartRomInit);
PORT_ALIAS(osDriveRomInit, gl_osDriveRomInit);
PORT_ALIAS(osEPiLinkHandle, gl_osEPiLinkHandle);
PORT_ALIAS(osCreatePiManager, gl_osCreatePiManager);
PORT_ALIAS(osPiGetCmdQueue, gl_osPiGetCmdQueue);
