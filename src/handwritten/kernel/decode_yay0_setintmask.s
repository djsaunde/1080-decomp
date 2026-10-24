# Yay0 decoder + osSetIntMask -- handwritten assembly.
# splat symbol func_800023E0 spans two original routines:
#   0x800023E0  Nintendo's Yay0 decompressor (decode.s; the same routine is
#               references/papermario/src/os/decode_yay0.s -- addi/add/sub
#               forms IDO never emits)
#   0x80002488  osSetIntMask (libultra os/setintmask.s;
#               reference references/libreultra/src/os/setintmask.s)
# The second routine keeps a local label so the symbol table is unchanged.
#
# void decode(u8 *src /*Yay0 header*/, u8 *dst)
#   header: +4 decompressed size, +8 link (copy-group) table offset,
#           +0xC chunk (literal byte) table offset, +0x10 layout bit words

glabel func_800023E0
    lw      $a3, 0x8($a0)               # a3 = link table offset
    lw      $t9, 0xC($a0)               # t9 = chunk table offset
    lw      $t8, 0x4($a0)               # t8 = decompressed size
    add     $a3, $a3, $a0               # a3 = link table
    add     $t9, $t9, $a0               # t9 = chunk table
    or      $a2, $zero, $zero           # a2 = layout bits left
    addi    $a0, $a0, 0x10              # a0 = layout words
    add     $t8, $t8, $a1               # t8 = dst end
.Lyay0_loop:
    bnez    $a2, .Lyay0_havebits
     nop
    lw      $t0, 0x0($a0)               # next 32 layout bits
    addiu   $a2, $zero, 0x20
    addi    $a0, $a0, 0x4
.Lyay0_havebits:
    slt     $t1, $t0, $zero             # top bit set: literal byte
    beqz    $t1, .Lyay0_copy
     nop
    lb      $t2, 0x0($t9)
    addi    $t9, $t9, 0x1
    addi    $a1, $a1, 0x1
    b       .Lyay0_next
     sb     $t2, -0x1($a1)
.Lyay0_copy:
    lhu     $t2, 0x0($a3)               # link: count:4 | distance-1:12
    addi    $a3, $a3, 0x2
    srl     $t3, $t2, 12
    andi    $t2, $t2, 0xFFF
    beqz    $t3, .Lyay0_longcount       # count 0: length byte from chunks
     sub    $t1, $a1, $t2               # t1 = copy source + 1
    addi    $t3, $t3, 0x2               # length = count + 2
.Lyay0_copyloop:
    lb      $t2, -0x1($t1)
    addi    $t3, $t3, -0x1
    addi    $t1, $t1, 0x1
    addi    $a1, $a1, 0x1
    bnez    $t3, .Lyay0_copyloop
     sb     $t2, -0x1($a1)
.Lyay0_next:
    sll     $t0, $t0, 1
    bne     $a1, $t8, .Lyay0_loop
     addi   $a2, $a2, -0x1
    jr      $ra
     nop
.Lyay0_longcount:
    lbu     $t3, 0x0($t9)               # length = chunk byte + 0x12
    addi    $t9, $t9, 0x1
    b       .Lyay0_copyloop
     addi   $t3, $t3, 0x12
    nop                                 # object alignment padding

# u32 osSetIntMask(u32 im) -- libultra os/setintmask.s
#   D_8000A3E0 = __OSGlobalIntMask, D_8000A680 = __osRcpImTable,
#   D_A430000C = MI_INTR_MASK_REG (KSEG1)
.LosSetIntMask:
    mfc0    $t4, $12                    # t4 = C0_SR
    andi    $v0, $t4, 0xFF01            # OS_IM_CPU
    lui     $t0, %hi(D_8000A3E0)        # &__OSGlobalIntMask
    addiu   $t0, $t0, %lo(D_8000A3E0)
    lw      $t3, 0x0($t0)
    addiu   $at, $zero, -0x1
    xor     $t0, $t3, $at
    andi    $t0, $t0, 0xFF00            # SR_IMASK
    or      $v0, $v0, $t0
    lui     $t2, %hi(D_A430000C)        # MI_INTR_MASK_REG
    lw      $t2, %lo(D_A430000C)($t2)
    beqz    $t2, .LosSetIntMask_rcp
     srl    $t1, $t3, 16
    addiu   $at, $zero, -0x1
    xor     $t1, $t1, $at
    andi    $t1, $t1, 0x3F              # MI_INTR_MASK
    or      $t2, $t2, $t1
.LosSetIntMask_rcp:
    sll     $t2, $t2, 16
    or      $v0, $v0, $t2               # v0 = previous mask
    lui     $at, 0x3F                   # MI_INTR_MASK << 16
    and     $t0, $a0, $at
    and     $t0, $t0, $t3
    srl     $t0, $t0, 15
    lui     $t2, %hi(D_8000A680)        # __osRcpImTable[t0]
    addu    $t2, $t2, $t0
    lhu     $t2, %lo(D_8000A680)($t2)
    lui     $at, %hi(D_A430000C)
    sw      $t2, %lo(D_A430000C)($at)   # MI_INTR_MASK_REG = table value
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
endlabel func_800023E0
