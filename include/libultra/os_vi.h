#ifndef LIBULTRA_OS_VI_H
#define LIBULTRA_OS_VI_H

/* libultra VI modes and the VI manager context (2.0I PR/os.h for OSViMode,
 * src/io/viint.h for __OSViContext; references/libreultra). Shared by the
 * game_libs osViSet* copies and the __osViInit / osViSetSpecialFeatures
 * donors (game_libs_ido53_70B04.c, game_libs_o1_71144.c; their private copies
 * were dropped in agent-a run 586). Opt-in like os_thread.h. Offsets and
 * sizes are asserted below (array size -1 => compile error). */

#include "libultra/os_message.h"

typedef struct {
    /* 0x00 */ u32 ctrl;
    /* 0x04 */ u32 width;
    /* 0x08 */ u32 burst;
    /* 0x0C */ u32 vSync;
    /* 0x10 */ u32 hSync;
    /* 0x14 */ u32 leap;
    /* 0x18 */ u32 hStart;
    /* 0x1C */ u32 xScale;
    /* 0x20 */ u32 vCurrent;
} OSViCommonRegs; /* size 0x24 */

typedef struct {
    /* 0x00 */ u32 origin;
    /* 0x04 */ u32 yScale;
    /* 0x08 */ u32 vStart;
    /* 0x0C */ u32 vBurst;
    /* 0x10 */ u32 vIntr;
} OSViFieldRegs; /* size 0x14 */

typedef struct {
    /* 0x00 */ u8 type;
    /* 0x04 */ OSViCommonRegs comRegs;
    /* 0x28 */ OSViFieldRegs fldRegs[2];
} OSViMode; /* size 0x50 */

typedef struct {
    /* 0x00 */ f32 factor;
    /* 0x04 */ u16 offset;
    /* 0x08 */ u32 scale;
} __OSViScale; /* size 0x0C */

/* __osViNext->state bits (viint.h) */
#define VI_STATE_XSCALE_UPDATED 0x02
#define VI_STATE_YSCALE_UPDATED 0x04
#define VI_SCALE_MASK 0xFFF

typedef struct {
    /* 0x00 */ u16 state;        /* VI_STATE_* */
    /* 0x02 */ u16 retraceCount;
    /* 0x04 */ void *framep;
    /* 0x08 */ OSViMode *modep;
    /* 0x0C */ u32 control;
    /* 0x10 */ OSMesgQueue *msgq;
    /* 0x14 */ OSMesg msg;
    /* 0x18 */ __OSViScale x;
    /* 0x24 */ __OSViScale y;
} __OSViContext; /* size 0x30 */

#ifndef __cplusplus /* edg (C++ donors) rejects the &((T *)0)->f constant form */
#define OS_VI_ASSERT(name, cond) typedef char os_vi_assert_##name[(cond) ? 1 : -1]
OS_VI_ASSERT(xScale, (int)&((OSViMode *)0)->comRegs.xScale == 0x20);
OS_VI_ASSERT(fldRegs, (int)&((OSViMode *)0)->fldRegs[1] == 0x3C);
OS_VI_ASSERT(mode_size, sizeof(OSViMode) == 0x50);
OS_VI_ASSERT(modep, (int)&((__OSViContext *)0)->modep == 0x08);
OS_VI_ASSERT(msgq, (int)&((__OSViContext *)0)->msgq == 0x10);
OS_VI_ASSERT(x_factor, (int)&((__OSViContext *)0)->x.factor == 0x18);
OS_VI_ASSERT(x_scale, (int)&((__OSViContext *)0)->x.scale == 0x20);
OS_VI_ASSERT(y_factor, (int)&((__OSViContext *)0)->y.factor == 0x24);
OS_VI_ASSERT(ctx_size, sizeof(__OSViContext) == 0x30);
#endif /* !__cplusplus */

#endif /* LIBULTRA_OS_VI_H */
