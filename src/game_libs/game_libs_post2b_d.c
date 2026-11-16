#include "common.h"
#include "libultra/os_timer.h"
extern int D_00000000;
typedef struct { int a, b, c, d; } Quad4;
typedef struct { float x, y, z; } Vec3;

/* gl_osEPiRawWriteIo = libultra osEPiRawWriteIo (epirawwrite.c verbatim,
 * EPI_SYNC/UPDATE_REG piint.h macros): PI busy-wait, per-domain BSD
 * timing-reg update vs __osCurrentHandle[domain], then
 * IO_WRITE(baseAddress | devAddr, data). Sibling of osEPiRawReadIo =
 * gl_osEPiRawReadIo. BOUNDARY FIX: the 2-word orphan
 * game_libs_func_00073694 (lui/lw PI_STATUS_REG) was this function's
 * hoisted first status read scheduled before the addiu-sp prologue by
 * IDO 5.3; its INCLUDE_ASM was removed and the spliced symbol covers
 * 0x73694..0x73824. (The _Genld NM bodies below still call the old
 * game_libs_func_00073694 name from their blank-jal decode guesses --
 * extern-only, NON_MATCHING path, revisit when those are decoded.)
 * WIRED 2026-07-10 via REPLACE_FUNC_BODY donor splice: real C lives in
 * the IDO 5.3 -O1 donor unit game_libs_ido53_7369C.c (100/100). Body
 * below is a placeholder for the splice. */
int gl_osEPiRawWriteIo(char *pihandle, unsigned int devAddr, unsigned int data) {
    volatile int ret = 0;
    if (pihandle == 0) {
        ret = -1;
    }
    return ret;
}

/* gl_osSetTimer: 53-insn 8-word record init + u64-return call +
 * conditional u64-arg call (frame 0x20, saves ra).
 * LANDED 2026-07-09 via REPLACE_FUNC_BODY donor splice: the target is plain
 * IDO 7.1 -O1 with a (int*, u64, u64, int, int) signature — the u64 params
 * explain the "a2/a3 3-way tree" and the missing sw a1 home. Real C lives
 * in the -O1 donor unit game_libs_o1_73824.c (53/53), spliced over this
 * -O2 stand-in. Body below is a placeholder for the splice (its bytes are
 * replaced by the donor); it spells the donor's libultra osSetTimer. */
extern int gl_func_00000000();
extern int D_00000000;
int gl_osSetTimer(OSTimer *t, OSTime countdown, OSTime interval, OSMesgQueue *mq, OSMesg msg) {
    OSTime r;

    t->next = 0;
    /* (t->prev = 0 is the donor's; left out here so the stand-in keeps the old length,
     * which the non_matching object's spliced .text size depends on) */
    t->interval = interval;
    if (countdown != 0) {
        t->value = countdown;
    } else {
        t->value = interval;
    }
    t->mq = mq;
    t->msg = msg;
    r = ((OSTime (*)(OSTimer *))gl_func_00000000)(t);
    if ((*(OSTimer **)&D_00000000)->next == t) {
        ((void (*)(OSTime))gl_func_00000000)(r);
    }
    return 0;
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_libs/game_libs/gl_func_00073824_pad.s")

/* DRIFT ROOT CAUSE (2026-06-10): ROM has a bare jr-ra/nop stub at
 * [0x73E6C..0x73E74) that splat's 0x568 size for THIS symbol drops --
 * the source of the whole [0x743C4..0x748A0] -4 content drift. The
 * naive fix (append 2 words, size 0x570) overshoots (+8 downstream) due
 * to hidden inter-block alignment in the unit; needs an alignment-aware
 * relayout of [0x73904..0x748A4) in one focused pass. See
 * docs/MATCHING_WORKFLOW "drift-region root cause FOUND". */
/* gl_Genld = _Genld (Plauger libc xldtob.c verbatim) -- the
 * printf float-formatter. LANDED 2026-08-22 via REPLACE_FUNC_BODY donor
 * splice: real C lives in the IDO 5.3 -O3 donor unit
 * game_libs_ido53_73904.c (346/346 raw words spliced; the donor TU also
 * reproduces the [0x73E6C,0x73E74) jr-ra tail word-exact, which stays
 * the separate gl_xldtob_stub_00073E6C def below.
 * The only non-zero reloc imm is the "0" string %lo,
 * .rodata base pinned 0x2540). The old "caller-set s-reg argument cap"
 * was the -O3 WHOLE-TU custom linkage: _Genld is static in xldtob.c and
 * IDO -O3 interprocedural regalloc passes its five args in $s0-$s4
 * (px, p, xexp, nsig, code) -- no per-function compile can emit that;
 * the -O3 TU donor does. Body below is a placeholder for the splice. */
void gl_Genld(void *px, char *p, int xexp, int nsig, int code) {
    volatile int genld_spliced = 0;
    if (code != 0) {
        genld_spliced = nsig + xexp;
    }
}

void gl_xldtob_stub_00073E6C(void) {}

/* gl_Ldtob = _Ldtob (Plauger libc xldtob.c verbatim) -- the
 * printf float-converter, sibling of _Genld/73904. LANDED 2026-08-22
 * from the same IDO 5.3 -O3 donor unit game_libs_ido53_73904.c
 * (340/340 raw words): at -O3 the static _Ldunscale is integrated into
 * _Ldtob (the old 74.0% "_Ldunscale-family classifier" redecode was
 * exactly that inlined classifier head), and the final call is the
 * s-reg-linkage jal to _Genld (target bakes jal 0x87F70 = the USO's
 * original-link _Genld address; donor's local .text reloc renamed
 * gl_Ldtob_text by the splice, pinned 0x87F70). Blank jals:
 * memcpy x1 + ldiv (= ldiv identity, "a divmod helper" as
 * the old wrap guessed). .rodata pins: pows[] table @0x2540, "Inf"
 * 0x2588 / "NaN" 0x258C / 1.0e8 @0x2598 (base 0x2540).
 * Body below is a placeholder for the splice. */
void gl_Ldtob(void *px, int code) {
    volatile int ldtob_spliced = 0;
    if (code != 0) {
        ldtob_spliced = code;
    }
}
/* 743C0 pad word merged into gl_Ldtob.s tail (1-word GLOBAL_ASM
 * blocks emit 2 words — asm-processor 8-byte placeholder minimum — which
 * shifted links 743C4..748A0 +4; the trailing 747F4_pad then got clipped
 * by TRUNCATE_TEXT 0x1210). */

/* lldiv = libc lldiv (ldiv.c verbatim, long-long sibling of
 * ldiv = ldiv; IDO 5.3 -O2 donor: game_libs_ido53_743C4.c).
 * LANDED 2026-08-22 via REPLACE_FUNC_BODY donor splice, 64/64 words
 * exact. The old "timestamp/range helper" re-derivation (54.8%) is
 * retired: the two chained jal-0 "64-bit helpers" are the compiler's
 * __ll_div / __ll_mul intrinsics (blank USO relocs, pins = 0), and the
 * manual subu/sltu/subu + sign-fix tail is verbatim Plauger
 * `ret.rem = num - denom*ret.quot; if (quot<0 && rem>0) {...}`.
 * Corroboration: the xlitob.c donor (_Litob) ships a blank lldiv jal
 * whose USO load-time reloc targets this address. Body below is a
 * placeholder for the splice. */
void lldiv(int m) {
    volatile int lldiv_spliced = 0;
    if (m != 0) {
        lldiv_spliced = m;
    }
}


/* game_libs_func_000744C4 (0x8 orphan, no jr ra: div/mflo) was the HOISTED
 * HEAD of ldiv = ldiv — absorbed into the donor splice below
 * (true entry 0x744C4, spliced symbol covers 0x744C4..0x74548). */


/* ldiv = libc ldiv (xldiv.c verbatim): quot = numer/denom
 * (the div/mflo hoisted above the prologue = the absorbed 0x8 orphan),
 * rem = numer - denom*quot, then the C-rounding fixup
 * (quot<0 && rem>0 => quot++, rem-=denom), returned as an o32 sret
 * struct {quot, rem} through $a0. Needs IDO 5.3 -O2 (7.1 -O2 picks
 * $t9/$t8 for the sret copy instead of the target's $at/$t0), so the
 * real C lives in the donor unit game_libs_ido53_744C4.c (33/33 exact,
 * zero relocs). Body below is a placeholder for the REPLACE_FUNC_BODY
 * splice (its bytes are replaced by the donor). */
void ldiv(void) {
    volatile int i;
    for (i = 0; i < 15; i++) {}
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_libs/game_libs/gl_func_000744CC_pad.s")

/* _Litob = _Litob (Plauger libc xlitob.c verbatim) -- the
 * printf integer-formatter, completing the printf-machinery vein
 * (73904=_Genld, 73E74=_Ldtob, 744CC=ldiv). LANDED 2026-08-22 via
 * REPLACE_FUNC_BODY donor splice: real C in the IDO 5.3 -O2 donor unit
 * game_libs_ido53_74554.c (166/166 raw words; 7.1 -O2 diverges 77
 * words). ldigs/udigs .data base pinned 0x2E620
 * (_Litob_data); blank jals __ull_rem/__ull_div (objcopy
 * -redefined; USO load-time relocs point them at the game_libs copies
 * 6C740/6C77C), lldiv, memcpy. Body below is a placeholder for the
 * splice. */
void _Litob(void *args, int type) {
    volatile int litob_spliced = 0;
    if (type != 0) {
        litob_spliced = type;
    }
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_libs/game_libs/gl_func_00074554_pad.s")

/* gl___osSiRawWriteIo = libultra __osPiRawWriteIo. LANDED 2026-06-21 as a
 * byte-identical TWIN-PORT of matched kernel __osSiRawWriteIo (kernel_046) -- the
 * write twin of the landed __osPiRawReadIo at gl___osSiRawReadIo: PI-busy probe
 * (no-arg call), then uncached KSEG1 write of data to (devAddr | 0xA0000000).
 * The prior wrap guessed a 2-arg acquire call + a 4-stub bundle; it is a single
 * 19-insn fn. Real C lives in the donor unit game_libs_ido53_747F4.c (IDO 5.3
 * -O1), spliced via REPLACE_FUNC_BODY. Busy probe -> gl_func_00000000. */
int gl___osSiRawWriteIo(int devAddr, int data) {
    if (gl_func_00000000() != 0) {
        return -1;
    }
    *(volatile int *)(0xA0000000 | devAddr) = data;
    return 0;
}


/* gl___osSpSetStatus = libultra __osSpSetStatus (sp/spsetstat.c
 * verbatim): `*(volatile u32*)SP_STATUS_REG = data`. BOUNDARY FIX
 * 2026-09-09 (TWENTY-SIXTH mis-split case): the old "game_libs_func_00074840"
 * symbol carried a LEADING nop that was documented as a non-reproducible
 * "hardware-access hazard idiom" CAP. bootup.uso's Sym export table says
 * otherwise: section 0x88EB0 (= splat 0x74844, the `lui`) is exported
 * (sym 2521, three R_MIPS_26 refs) and 0x88EAC (the nop) is not. Every
 * libultra .o in this tail is 16-byte aligned in the module text, so the
 * nop is the inter-object pad after __osPiRawWriteIo (747F4, 0x4C) --
 * restored as an all-zero SUFFIX_BYTES_FORCE on gl___osSiRawWriteIo (a 1-word
 * GLOBAL_ASM sidecar would emit 8 bytes, docs #one-word-pad-legal-shapes).
 * 3/3 words at 7.1 -O2. */
void gl___osSpSetStatus(unsigned int data) {
    *(volatile unsigned int *)0xA4040010 = data;
}

/* gl___osSpGetStatus = libultra __osSpGetStatus (sp/spgetstat.c
 * verbatim): `return *(volatile u32*)SP_STATUS_REG`. Same boundary fix as
 * 74844: section 0x88EC0 (= splat 0x74854) is the export (sym 2630, one
 * R_MIPS_26 ref); the nop at 0x74850 is __osSpSetStatus's inter-object
 * pad (SUFFIX_BYTES_FORCE on 74844). The nop at 0x74860 heading the next
 * INCLUDE block is likewise this .o's pad (0x74864 = section 0x88ED0 is the
 * exported osWritebackDCacheAll twin, handwritten). 3/3 words. */
unsigned int gl___osSpGetStatus(void) {
    return *(volatile unsigned int *)0xA4040010;
}

/* gl_osWritebackDCacheAll = osWritebackDCacheAll (game_libs libultra copy): real source in src/libultra/game_libs/writebackdcacheall.s. See docs/MATCHING_WORKFLOW.md#handwritten-assembly-sources. */
GLOBAL_ASM("src/libultra/game_libs/writebackdcacheall.s")

/* 7488C decomposed 2026-06-10 (drift region realigned by the relayout,
 * unblocking the deferred match): 2 inter-fn pad words (orphan block)
 * + the standard collapsed int-reader at 0x74894 (lui/jr/lw with hi/lo
 * relocs; ROM stores the unrelocated zeros, D_00000000 at VRAM 0
 * resolves identically). */
#pragma GLOBAL_ASM("asm/nonmatchings/game_libs/game_libs/_pad_pre_74894.s")
int gl___osGetCurrFaultedThread(void) {
    return D_00000000;
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_libs/game_libs/gl_func_000747F4_pad.s")
