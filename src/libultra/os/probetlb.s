# __osProbeTLB -- libultra handwritten assembly (os/probetlb.s).
# Reference: references/oot/src/libultra/os/probetlb.s,
#            references/libreultra/src/os/probetlb.s
# u32 __osProbeTLB(void *vaddr): physical address mapped for vaddr, or -1.
#
# Adapted for asm-processor GLOBAL_ASM: .set noreorder (explicit delay slots),
# no cpp, so CP0 registers and R4300.h masks are spelled numerically and
# macro-expanded immediates (and with a 32-bit mask, li) are written as the
# instruction pairs the assembler produces.
#   CP0: $0 = C0_INX, $2 = C0_ENTRYLO0, $3 = C0_ENTRYLO1, $5 = C0_PAGEMASK,
#        $10 = C0_ENTRYHI
#
# splat splits this routine into two symbols (__osException jal's the tail
# at func_80004DB8 directly). asm-processor sizes the first glabel of a
# GLOBAL_ASM block to the whole block, so each splat symbol is its own block:
# the routine continues in probetlb_2.s (labels resolve across blocks; all
# GLOBAL_ASM blocks of a unit are assembled as one file).

glabel __osProbeTLB
    # Set C0_ENTRYHI based on the supplied vaddr
    mfc0    $t0, $10                    # t0 = C0_ENTRYHI (saved)
    andi    $t1, $t0, 0xFF              # TLBHI_PIDMASK
    addiu   $at, $zero, -0x2000         # TLBHI_VPN2MASK (0xFFFFE000)
    and     $t2, $a0, $at
    or      $t1, $t1, $t2
    mtc0    $t1, $10
    nop
    nop
    nop
    # TLB probe: sets C0_INX, or TLBINX_PROBE if no entry matches
    tlbp
    nop
    nop
    mfc0    $t3, $0                     # C0_INX
    lui     $at, 0x8000                 # TLBINX_PROBE
    and     $t3, $t3, $at
    bnez    $t3, .LprobeTLB_fail        # no match
     nop

    # Read the matching entry (ENTRYHI/LO0/LO1/PAGEMASK)
    tlbr
    nop
    nop
    nop
    # page size = (page mask + 0x2000) >> 1
    mfc0    $t3, $5                     # C0_PAGEMASK
    addi    $t3, $t3, 0x2000
    srl     $t3, $t3, 1
    and     $t4, $t3, $a0               # odd or even page?
    bnez    $t4, .LprobeTLB_odd
     addi   $t3, $t3, -1                # page-offset bitmask
    mfc0    $v0, $2                     # C0_ENTRYLO0
    b       .LprobeTLB_check
     nop
.LprobeTLB_odd:
    mfc0    $v0, $3                     # C0_ENTRYLO1
.LprobeTLB_check:
    andi    $t5, $v0, 0x2               # TLBLO_V
    beqz    $t5, .LprobeTLB_fail        # entry not valid
     nop
    lui     $at, 0x3FFF                 # TLBLO_PFNMASK (0x3FFFFFC0)
    ori     $at, $at, 0xFFC0
    and     $v0, $v0, $at
    sll     $v0, $v0, 6                 # TLBLO_PFNSHIFT
endlabel __osProbeTLB
