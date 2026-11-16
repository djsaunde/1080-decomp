#include "common.h"

extern s32 osRomBase;
extern s32 gPiDramAddrReg;
extern s32 gPiCartAddrReg;
extern s32 gPiRdLenReg;
extern s32 gPiWrLenReg;
extern s32 gPiStatusReg;
extern s32 func_80004B30(s32);
extern s32 func_800066B0(void);
extern void func_800066D0(s32);
extern s32 __osRdb_IP6_Data;
extern s32 __osRdb_IP6_Size;
extern s32 __osRdb_IP6_Ct;
extern s32 __osRdb_IP6_CurWrite;
extern s32 __osRdb_IP6_CurSend;

/* osInitRdb (ultralib os/initrdb.c) */
void osInitRdb(s32 addr, u32 size) {
    s32 saveMask;

    size >>= 2;
    if (addr & 3) {
        addr = (addr & 3) + 4;
        size--;
    }
    saveMask = func_800066B0();
    __osRdb_IP6_Data = addr;
    __osRdb_IP6_Size = size;
    __osRdb_IP6_Ct = 0;
    __osRdb_IP6_CurWrite = 0;
    __osRdb_IP6_CurSend = 0;
    func_800066D0(saveMask);
}
