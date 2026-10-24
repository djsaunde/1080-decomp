# bcopy, game_libs copy -- libultra handwritten assembly (libc/bcopy.s,
# `.weakext bcopy, _bcopy`: bootup.uso exports this entry twice, syms 2378 and
# 2642). Reference: references/oot/src/libultra/libc/bcopy.s and
# references/libreultra/src/libc/bcopy.s. Written out as the IRIX assembler
# emitted it (macro expansions -- blt/bge/and-imm -- and the reorganizer's
# delay-slot fills, including the branch-likely target copies, are spelled as
# the real instructions; asm-processor assembles with noreorder). Not IDO
# output: trapping `add` (v0 = src + n, and the backwards-copy pointer
# advances) and several interior `jr ra` exits. Labels follow the reference
# source; the forward/backward copy blocks mirror each other.

glabel game_libs_func_00070324
    beqz    $a2, .Lbcopy_ret
     or      $a3, $a1, $zero
    beq     $a0, $a1, .Lbcopy_ret
     slt     $at, $a1, $a0
    bnezl   $at, .Lbcopy_034
     slti    $at, $a2, 16
    add     $v0, $a0, $a2
    slt     $at, $a1, $v0
    beqzl   $at, .Lbcopy_034
     slti    $at, $a2, 16
    b       .Lbcopy_gobackwards
     slti    $at, $a2, 16
    slti    $at, $a2, 16
.Lbcopy_034:
    bnez    $at, .Lbcopy_forwards_bytecopy
     nop
    andi    $v0, $a0, 0x3
    andi    $v1, $a1, 0x3
    beq     $v0, $v1, .Lbcopy_forwalignable
     nop
.Lbcopy_forwards_bytecopy:
    beqz    $a2, .Lbcopy_ret
     nop
    addu    $v1, $a0, $a2
.Lbcopy_058:
    lb      $v0, 0($a0)
    addiu   $a0, $a0, 1
    addiu   $a1, $a1, 1
    bne     $a0, $v1, .Lbcopy_058
     sb      $v0, -1($a1)
.Lbcopy_ret:
    jr      $ra
     or      $v0, $a3, $zero
.Lbcopy_forwalignable:
    beqz    $v0, .Lbcopy_forwards_32
     addiu   $at, $zero, 1
    beq     $v0, $at, .Lbcopy_forw_copy3
     addiu   $at, $zero, 2
    beql    $v0, $at, .Lbcopy_0A8
     lh      $v0, 0($a0)
    lb      $v0, 0($a0)
    addiu   $a0, $a0, 1
    addiu   $a1, $a1, 1
    addiu   $a2, $a2, -1
    b       .Lbcopy_forwards_32
     sb      $v0, -1($a1)
.Lbcopy_forw_copy2:
    lh      $v0, 0($a0)
.Lbcopy_0A8:
    addiu   $a0, $a0, 2
    addiu   $a1, $a1, 2
    addiu   $a2, $a2, -2
    b       .Lbcopy_forwards_32
     sh      $v0, -2($a1)
.Lbcopy_forw_copy3:
    lb      $v0, 0($a0)
    lh      $v1, 1($a0)
    addiu   $a0, $a0, 3
    addiu   $a1, $a1, 3
    addiu   $a2, $a2, -3
    sb      $v0, -3($a1)
    sh      $v1, -2($a1)
.Lbcopy_forwards_32:
    slti    $at, $a2, 32
    bnezl   $at, .Lbcopy_138
     slti    $at, $a2, 16
    lw      $v0, 0($a0)
    lw      $v1, 4($a0)
    lw      $t0, 8($a0)
    lw      $t1, 12($a0)
    lw      $t2, 16($a0)
    lw      $t3, 20($a0)
    lw      $t4, 24($a0)
    lw      $t5, 28($a0)
    addiu   $a0, $a0, 32
    addiu   $a1, $a1, 32
    addiu   $a2, $a2, -32
    sw      $v0, -32($a1)
    sw      $v1, -28($a1)
    sw      $t0, -24($a1)
    sw      $t1, -20($a1)
    sw      $t2, -16($a1)
    sw      $t3, -12($a1)
    sw      $t4, -8($a1)
    b       .Lbcopy_forwards_32
     sw      $t5, -4($a1)
.Lbcopy_forwards_16:
    slti    $at, $a2, 16
.Lbcopy_138:
    bnezl   $at, .Lbcopy_174
     slti    $at, $a2, 4
    lw      $v0, 0($a0)
    lw      $v1, 4($a0)
    lw      $t0, 8($a0)
    lw      $t1, 12($a0)
    addiu   $a0, $a0, 16
    addiu   $a1, $a1, 16
    addiu   $a2, $a2, -16
    sw      $v0, -16($a1)
    sw      $v1, -12($a1)
    sw      $t0, -8($a1)
    b       .Lbcopy_forwards_16
     sw      $t1, -4($a1)
.Lbcopy_forwards_4:
    slti    $at, $a2, 4
.Lbcopy_174:
    bnez    $at, .Lbcopy_forwards_bytecopy
     nop
    lw      $v0, 0($a0)
    addiu   $a0, $a0, 4
    addiu   $a1, $a1, 4
    addiu   $a2, $a2, -4
    b       .Lbcopy_forwards_4
     sw      $v0, -4($a1)
    slti    $at, $a2, 16
.Lbcopy_gobackwards:
    add     $a0, $a0, $a2
    bnez    $at, .Lbcopy_backwards_bytecopy
     add     $a1, $a1, $a2
    andi    $v0, $a0, 0x3
    andi    $v1, $a1, 0x3
    beq     $v0, $v1, .Lbcopy_backalignable
     nop
.Lbcopy_backwards_bytecopy:
    beqz    $a2, .Lbcopy_ret
     nop
    addiu   $a0, $a0, -1
    addiu   $a1, $a1, -1
    subu    $v1, $a0, $a2
.Lbcopy_1C8:
    lb      $v0, 0($a0)
    addiu   $a0, $a0, -1
    addiu   $a1, $a1, -1
    bne     $a0, $v1, .Lbcopy_1C8
     sb      $v0, 1($a1)
    jr      $ra
     or      $v0, $a3, $zero
.Lbcopy_backalignable:
    beqz    $v0, .Lbcopy_backwards_32
     addiu   $at, $zero, 3
    beq     $v0, $at, .Lbcopy_back_copy3
     addiu   $at, $zero, 2
    beql    $v0, $at, .Lbcopy_218
     lh      $v0, -2($a0)
    lb      $v0, -1($a0)
    addiu   $a0, $a0, -1
    addiu   $a1, $a1, -1
    addiu   $a2, $a2, -1
    b       .Lbcopy_backwards_32
     sb      $v0, 0($a1)
.Lbcopy_back_copy2:
    lh      $v0, -2($a0)
.Lbcopy_218:
    addiu   $a0, $a0, -2
    addiu   $a1, $a1, -2
    addiu   $a2, $a2, -2
    b       .Lbcopy_backwards_32
     sh      $v0, 0($a1)
.Lbcopy_back_copy3:
    lb      $v0, -1($a0)
    lh      $v1, -3($a0)
    addiu   $a0, $a0, -3
    addiu   $a1, $a1, -3
    addiu   $a2, $a2, -3
    sb      $v0, 2($a1)
    sh      $v1, 0($a1)
.Lbcopy_backwards_32:
    slti    $at, $a2, 32
    bnezl   $at, .Lbcopy_2A8
     slti    $at, $a2, 16
    lw      $v0, -4($a0)
    lw      $v1, -8($a0)
    lw      $t0, -12($a0)
    lw      $t1, -16($a0)
    lw      $t2, -20($a0)
    lw      $t3, -24($a0)
    lw      $t4, -28($a0)
    lw      $t5, -32($a0)
    addiu   $a0, $a0, -32
    addiu   $a1, $a1, -32
    addiu   $a2, $a2, -32
    sw      $v0, 28($a1)
    sw      $v1, 24($a1)
    sw      $t0, 20($a1)
    sw      $t1, 16($a1)
    sw      $t2, 12($a1)
    sw      $t3, 8($a1)
    sw      $t4, 4($a1)
    b       .Lbcopy_backwards_32
     sw      $t5, 0($a1)
.Lbcopy_backwards_16:
    slti    $at, $a2, 16
.Lbcopy_2A8:
    bnezl   $at, .Lbcopy_2E4
     slti    $at, $a2, 4
    lw      $v0, -4($a0)
    lw      $v1, -8($a0)
    lw      $t0, -12($a0)
    lw      $t1, -16($a0)
    addiu   $a0, $a0, -16
    addiu   $a1, $a1, -16
    addiu   $a2, $a2, -16
    sw      $v0, 12($a1)
    sw      $v1, 8($a1)
    sw      $t0, 4($a1)
    b       .Lbcopy_backwards_16
     sw      $t1, 0($a1)
.Lbcopy_backwards_4:
    slti    $at, $a2, 4
.Lbcopy_2E4:
    bnez    $at, .Lbcopy_backwards_bytecopy
     nop
    lw      $v0, -4($a0)
    addiu   $a0, $a0, -4
    addiu   $a1, $a1, -4
    addiu   $a2, $a2, -4
    b       .Lbcopy_backwards_4
     sw      $v0, 0($a1)
endlabel game_libs_func_00070324
