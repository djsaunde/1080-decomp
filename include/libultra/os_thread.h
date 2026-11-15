#ifndef LIBULTRA_OS_THREAD_H
#define LIBULTRA_OS_THREAD_H

/* libultra OSThread layout (2.0I PR/os.h, references/libreultra), shared by
 * the kernel and game_libs copies. Opt-in header: several older TUs still
 * carry private OSThread typedefs, so do NOT add this to common.h until those
 * are migrated (duplicate typedef = cfe error). Offsets are asserted below
 * with negative-array tricks so a wrong field is a compile error, not a
 * silent byte change. */

#include "common.h"

typedef s32 OSPri;
typedef s32 OSId;
typedef union { struct { f32 f_odd; f32 f_even; } f; f64 d; } __OSfp;

typedef struct {
    u64 at, v0, v1, a0, a1, a2, a3;           /* 0x00 */
    u64 t0, t1, t2, t3, t4, t5, t6, t7;       /* 0x38 */
    u64 s0, s1, s2, s3, s4, s5, s6, s7;       /* 0x78 */
    u64 t8, t9, gp, sp, s8, ra;               /* 0xB8 */
    u64 lo, hi;                               /* 0xE8 */
    u32 sr, pc, cause, badvaddr, rcp;         /* 0xF8 */
    u32 fpcsr;                                /* 0x10C */
    __OSfp fp0, fp2, fp4, fp6, fp8, fp10, fp12, fp14;
    __OSfp fp16, fp18, fp20, fp22, fp24, fp26, fp28, fp30;
} __OSThreadContext; /* size 0x190 */

typedef struct OSThread_s {
    /* 0x00 */ struct OSThread_s *next;
    /* 0x04 */ OSPri priority;
    /* 0x08 */ struct OSThread_s **queue;
    /* 0x0C */ struct OSThread_s *tlnext;
    /* 0x10 */ u16 state;
    /* 0x12 */ u16 flags;
    /* 0x14 */ OSId id;
    /* 0x18 */ int fp;
    /* 0x20 */ __OSThreadContext context;
} OSThread; /* size 0x1B0 */

#define OS_STATE_STOPPED 1

/* compile-time offset checks (array size -1 => error) */
#define OS_THREAD_ASSERT_OFFSET(field, off) \
    typedef char os_thread_assert_##off[(int)&((OSThread *)0)->field == (off) ? 1 : -1]
OS_THREAD_ASSERT_OFFSET(context.a0, 0x38);
OS_THREAD_ASSERT_OFFSET(context.sp, 0xF0);
OS_THREAD_ASSERT_OFFSET(context.ra, 0x100);
OS_THREAD_ASSERT_OFFSET(context.sr, 0x118);
OS_THREAD_ASSERT_OFFSET(context.pc, 0x11C);
OS_THREAD_ASSERT_OFFSET(context.rcp, 0x128);
OS_THREAD_ASSERT_OFFSET(context.fpcsr, 0x12C);

#endif /* LIBULTRA_OS_THREAD_H */
