#include "uso_call.h"
/* IDO 5.3 -O1 donor for __osViInit = libultra __osViInit
 * (vi.c verbatim). PROVEN EXACT 78/78 standalone at IDO 5.3 -O1 (donor
 * symbol 0x13C = 79 words incl. the jr-ra delay nop; the last 0x70C40
 * alignment word -> SUFFIX_BYTES_FORCE, _pad.s sidecar deleted).
 * The vi[2] context array is game_libs-internal data BAKED to 0x2E4C0
 * (lui 3 / addiu -0x1B40) -> gl_ref_0002E4C0. All other globals are
 * blanked USO reloc words: gl___osViCurr_vicurr/vinext (__osViCurr/Next),
 * D_00000000_tvtype (osTvType), D_00000000_pal/mpal/ntsc
 * (osViModePalLan1/MpalLan1/NtscLan1). Callees (blank jal 0x0C000000):
 * gl_func_00000000 = bzero, gl_func_00000001 = __osViSwapContext
 * (= __osViSwapContext, the next function).
 * Spliced into game_libs_post1c.c.o via REPLACE_FUNC_BODY. */
#include "common.h"
#include "libultra/os_vi.h" /* OSViMode / __OSViContext (offsets asserted there) */

extern __OSViContext gl_ref_0002E4C0[2];    /* vi[2] (baked 0x2E4C0) */
extern __OSViContext *gl___osViCurr_vicurr;    /* __osViCurr */
extern __OSViContext *gl___osViNext_vinext;    /* __osViNext */
extern s32 D_00000000_tvtype;               /* osTvType */
extern OSViMode D_00000000_pal;             /* osViModePalLan1 */
extern OSViMode D_00000000_mpal;            /* osViModeMpalLan1 */
extern OSViMode D_00000000_ntsc;            /* osViModeNtscLan1 */
extern void gl_func_00000000(void *, s32);  /* bzero */
extern void gl_func_00000001(void);         /* __osViSwapContext = __osViSwapContext */

#define vi gl_ref_0002E4C0
#define IO_READ(a) (*(volatile u32 *)((a) | 0xA0000000))
#define IO_WRITE(a, v) (*(volatile u32 *)(a) = (v))

void __osViInit(void) {
    gl_func_00000000(vi, sizeof(vi)); /* bzero */
    gl___osViCurr_vicurr = &vi[0];
    gl___osViNext_vinext = &vi[1];
    gl___osViNext_vinext->retraceCount = 1;
    gl___osViCurr_vicurr->retraceCount = 1;
    gl___osViNext_vinext->framep = (void *)0x80000000;
    gl___osViCurr_vicurr->framep = (void *)0x80000000;
    if (D_00000000_tvtype == 0) { /* OS_TV_PAL */
        gl___osViNext_vinext->modep = &D_00000000_pal;
    } else if (D_00000000_tvtype == 2) { /* OS_TV_MPAL */
        gl___osViNext_vinext->modep = &D_00000000_mpal;
    } else {
        gl___osViNext_vinext->modep = &D_00000000_ntsc;
    }
    gl___osViNext_vinext->state = 0x20; /* VI_STATE_BLACK */
    gl___osViNext_vinext->control = gl___osViNext_vinext->modep->comRegs.ctrl;
    while (IO_READ(0xA4400010) > 10) /* VI_CURRENT_REG */
        ;
    IO_WRITE(0xA4400000, 0); /* VI_CONTROL_REG */
    XCALL(gl_func_00000001, __osViSwapContext)();      /* __osViSwapContext */
}
