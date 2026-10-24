# __osProbeTLB, game_libs copy -- libultra handwritten assembly
# (libultra is statically linked into game_libs as well as the kernel;
# the code is byte-identical, only the splat symbol boundaries differ.)
# Reference: references/oot/src/libultra/os/probetlb.s; kernel copy
# src/libultra/os/probetlb.s.
# Here the whole routine is one splat symbol (the kernel copy is split in two).
# Its 2 trailing pad words open game_libs_func_0006D94C (getcount.s).

glabel game_libs_func_0006D894
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
    bnez    $t3, .Lgl_probeTLB_fail        # no match
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
    bnez    $t4, .Lgl_probeTLB_odd
     addi   $t3, $t3, -1                # page-offset bitmask
    mfc0    $v0, $2                     # C0_ENTRYLO0
    b       .Lgl_probeTLB_check
     nop
.Lgl_probeTLB_odd:
    mfc0    $v0, $3                     # C0_ENTRYLO1
.Lgl_probeTLB_check:
    andi    $t5, $v0, 0x2               # TLBLO_V
    beqz    $t5, .Lgl_probeTLB_fail        # entry not valid
     nop
    lui     $at, 0x3FFF                 # TLBLO_PFNMASK (0x3FFFFFC0)
    ori     $at, $at, 0xFFC0
    and     $v0, $v0, $at
    sll     $v0, $v0, 6                 # TLBLO_PFNSHIFT
    and     $t5, $a0, $t3               # page offset of vaddr
    add     $v0, $v0, $t5               # paddr = pfn + offset
    b       .Lgl_probeTLB_done
     nop
.Lgl_probeTLB_fail:
    addiu   $v0, $zero, -1              # no mapping: return -1
.Lgl_probeTLB_done:
    mtc0    $t0, $10                    # restore C0_ENTRYHI
    jr      $ra
     nop
endlabel game_libs_func_0006D894
