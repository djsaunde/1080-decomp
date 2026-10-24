# bzero, game_libs copy -- libultra handwritten assembly
# (libultra is statically linked into game_libs as well as the kernel;
# the code is byte-identical, only the splat symbol boundaries differ.)
# Reference: references/oot/src/libultra/libc/bzero.s; kernel copy
# src/libultra/libc/bzero.s.
# splat's symbol starts with the 3 zero words that pad the preceding object.

glabel game_libs_func_0006B048
    nop                                 # alignment padding (preceding object)
    nop
    nop
    slti    $at, $a1, 0xC               # short (< 12 bytes): bytes only
    bnez    $at, .Lgl_bzero_bytes
     negu   $v1, $a0
    andi    $v1, $v1, 0x3               # bytes to word alignment
    beqz    $v1, .Lgl_bzero_blkzero
     subu   $a1, $a1, $v1
    swl     $zero, 0x0($a0)             # clear the unaligned head
    addu    $a0, $a0, $v1
.Lgl_bzero_blkzero:
    addiu   $at, $zero, -0x20           # len & ~31: 32-byte blocks
    and     $a3, $a1, $at
    beqz    $a3, .Lgl_bzero_wordzero
     subu   $a1, $a1, $a3
    addu    $a3, $a3, $a0               # block end
.Lgl_bzero_blkloop:
    addiu   $a0, $a0, 0x20
    sw      $zero, -0x20($a0)
    sw      $zero, -0x1C($a0)
    sw      $zero, -0x18($a0)
    sw      $zero, -0x14($a0)
    sw      $zero, -0x10($a0)
    sw      $zero, -0xC($a0)
    sw      $zero, -0x8($a0)
    bne     $a0, $a3, .Lgl_bzero_blkloop
     sw     $zero, -0x4($a0)
.Lgl_bzero_wordzero:
    addiu   $at, $zero, -0x4            # len & ~3: whole words
    and     $a3, $a1, $at
    beqz    $a3, .Lgl_bzero_bytes
     subu   $a1, $a1, $a3
    addu    $a3, $a3, $a0
.Lgl_bzero_wordloop:
    addiu   $a0, $a0, 0x4
    bne     $a0, $a3, .Lgl_bzero_wordloop
     sw     $zero, -0x4($a0)
.Lgl_bzero_bytes:
    blez    $a1, .Lgl_bzero_done
     nop
    addu    $a1, $a1, $a0
.Lgl_bzero_byteloop:
    addiu   $a0, $a0, 0x1
    bne     $a0, $a1, .Lgl_bzero_byteloop
     sb     $zero, -0x1($a0)
.Lgl_bzero_done:
    jr      $ra
     nop
endlabel game_libs_func_0006B048
    nop                                 # object alignment padding
