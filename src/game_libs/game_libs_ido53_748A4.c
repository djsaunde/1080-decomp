#include "uso_call.h"
/* game_libs ido53 carve: contramread.c pair, verbatim at IDO 5.3 -O1.
 * __osContRamRead = __osContRamRead (135/135) and __osPackRamReadData =
 * __osPackRamReadData (79/79), both first try -- mirror of the
 * contramwrite.c carve. RamRead calls PackRamReadData via the BUILD-TIME-
 * PATCHED hardcoded jal gl_ref_0008912C (= USO 0x74AC0 + reloc base
 * 0x1466C, cross-confirming the identification); other callees are
 * runtime-patch placeholders. The pair's 8-byte trailing pad comes from a
 * genuine all-zero SUFFIX. Literals: CONT_CMD_READ_MEMPACK=2, TX=3, RX=33,
 * NOP=255, END=254, EXE=1, CONTRFAIL=4, NOPACK=1. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef int s32;

typedef struct {
    int ramarray[15];
    int pifstatus;
} GlPifRam;

typedef struct {
    u8 dummy; u8 txsize; u8 rxsize; u8 cmd;
    u16 address;
    u8 data[32];
    u8 datacrc;
} __OSContRamReadFormat;

extern int gl_func_00000000();
extern int gl_ref_0008912C();
extern GlPifRam gl___osPfsPifRam_748A4_pifram;
extern u8 gl___osContLastCmd_748A4_lastcmd;

s32 __osContRamRead(void *mq, int channel, u16 address, u8 *buffer)
{
    s32 ret;
    int i;
    u8 *ptr;
    __OSContRamReadFormat ramreadformat;
    int retry;
    ret = 0;
    ptr = (u8 *)&gl___osPfsPifRam_748A4_pifram;
    retry = 2;
    CALL(gl___osSiGetAccess)();
    gl___osContLastCmd_748A4_lastcmd = 2;
    gl_ref_0008912C(channel, address);
    ret = CALL(gl___osSiRawStartDma)(1, &gl___osPfsPifRam_748A4_pifram);
    CALL(gl_osRecvMesg)(mq, 0, 1);
    do
    {
        ret = CALL(gl___osSiRawStartDma)(0, &gl___osPfsPifRam_748A4_pifram);
        CALL(gl_osRecvMesg)(mq, 0, 1);
        ptr = (u8 *)&gl___osPfsPifRam_748A4_pifram;
        if (channel != 0)
        {
            for (i = 0; i < channel; i++)
            {
                ptr++;
            }
        }
        ramreadformat = *(__OSContRamReadFormat *)ptr;
        ret = ((ramreadformat.rxsize & 0xC0) >> 4);
        if (ret == 0)
        {
            u8 c;
            c = CALL(__osContDataCrc)((u8*)&ramreadformat.data);
            if (c != ramreadformat.datacrc)
            {
                ret = CALL(__osPfsGetStatus)(mq, channel);
                if (ret != 0)
                {
                    CALL(gl___osSiRelAccess)();
                    return ret;
                }
                ret = 4;
            }
            else
            {
                for (i = 0; i < 32; i++)
                {
                    *buffer++ = ramreadformat.data[i];
                }
            }
        }
        else
        {
            ret = 1;
        }
        if (ret != 4)
            break;
    } while (retry-- >= 0);
    CALL(gl___osSiRelAccess)();
    return ret;
}

void __osPackRamReadData(int channel, u16 address)
{
    u8 *ptr;
    __OSContRamReadFormat ramreadformat;
    int i;

    ptr = (u8 *)gl___osPfsPifRam_748A4_pifram.ramarray;
    gl___osPfsPifRam_748A4_pifram.pifstatus = 1;
    ramreadformat.dummy = 255;
    ramreadformat.txsize = 3;
    ramreadformat.rxsize = 33;
    ramreadformat.cmd = 2;
    ramreadformat.address = (address << 0x5) | CALL(__osContAddressCrc)(address);
    ramreadformat.datacrc = 255;
    for (i = 0; i < 32; i++)
    {
        ramreadformat.data[i] = 255;
    }
    if (channel != 0)
    {
        for (i = 0; i < channel; i++)
        {
            *ptr++ = 0;
        }
    }
    *(__OSContRamReadFormat *)ptr = ramreadformat;
    ptr += sizeof(__OSContRamReadFormat);
    ptr[0] = 254;
}
