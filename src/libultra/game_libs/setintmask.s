# osSetIntMask, game_libs copy -- libultra handwritten assembly
# (os/setintmask.s).
# (libultra is statically linked into game_libs as well as the kernel;
# the code is byte-identical, only the splat symbol boundaries differ.)
# Reference: references/libreultra/src/os/setintmask.s; kernel copy is the
# second routine of src/handwritten/kernel/decode_yay0_setintmask.s.
# splat's symbol starts with the 2 zero words that pad the preceding object.
#
# USO module code: the two data addresses are load-time relocations, so they
# are referenced through symbols pinned to 0 (undefined_syms_auto.txt), which
# leaves the zero fields the ROM ships:
#   D_00000000_gim = __OSGlobalIntMask, D_00000000_rcpim = __osRcpImTable
#   0xA430000C = MI_INTR_MASK_REG (KSEG1)

glabel game_libs_func_0006A09C
    nop                                 # alignment padding (preceding object)
    nop
    # u32 osSetIntMask(u32 im)
    mfc0    $t4, $12                    # t4 = C0_SR
    andi    $v0, $t4, 0xFF01            # OS_IM_CPU
    lui     $t0, %hi(D_00000000_gim)    # &__OSGlobalIntMask
    addiu   $t0, $t0, %lo(D_00000000_gim)
    lw      $t3, 0x0($t0)
    addiu   $at, $zero, -0x1
    xor     $t0, $t3, $at
    andi    $t0, $t0, 0xFF00            # SR_IMASK
    or      $v0, $v0, $t0
    lui     $t2, 0xA430                 # MI_INTR_MASK_REG
    lw      $t2, 0xC($t2)
    beqz    $t2, .Lgl_setIntMask_rcp
     srl    $t1, $t3, 16
    addiu   $at, $zero, -0x1
    xor     $t1, $t1, $at
    andi    $t1, $t1, 0x3F              # MI_INTR_MASK
    or      $t2, $t2, $t1
.Lgl_setIntMask_rcp:
    sll     $t2, $t2, 16
    or      $v0, $v0, $t2               # v0 = previous mask
    lui     $at, 0x3F                   # MI_INTR_MASK << 16
    and     $t0, $a0, $at
    and     $t0, $t0, $t3
    srl     $t0, $t0, 15
    lui     $t2, %hi(D_00000000_rcpim)  # __osRcpImTable[t0]
    addu    $t2, $t2, $t0
    lhu     $t2, %lo(D_00000000_rcpim)($t2)
    lui     $at, 0xA430
    sw      $t2, 0xC($at)               # MI_INTR_MASK_REG = table value
    andi    $t0, $a0, 0xFF01            # OS_IM_CPU
    andi    $t1, $t3, 0xFF00            # SR_IMASK
    and     $t0, $t0, $t1
    lui     $at, 0xFFFF                 # ~SR_IMASK
    ori     $at, $at, 0xFF
    and     $t4, $t4, $at
    or      $t4, $t4, $t0
    mtc0    $t4, $12                    # C0_SR
    nop
    nop
    jr      $ra
     nop
endlabel game_libs_func_0006A09C
