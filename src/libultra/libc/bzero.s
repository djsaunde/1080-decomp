# bzero -- libultra handwritten assembly (libc/bzero.s).
# Reference: references/oot/src/libultra/libc/bzero.s,
#            references/libreultra/src/libc/bzero.s
# void bzero(void *dst, int len)
# splat symbol: func_80002CD0.
#
# Same algorithm as the reference: align the start with one unaligned swl,
# clear 32-byte blocks, then words, then trailing bytes.

glabel func_80002CD0
    slti    $at, $a1, 0xC               # short (< 12 bytes): bytes only
    bnez    $at, .Lbzero_bytes
     negu   $v1, $a0
    andi    $v1, $v1, 0x3               # bytes to word alignment
    beqz    $v1, .Lbzero_blkzero
     subu   $a1, $a1, $v1
    swl     $zero, 0x0($a0)             # clear the unaligned head
    addu    $a0, $a0, $v1
.Lbzero_blkzero:
    addiu   $at, $zero, -0x20           # len & ~31: 32-byte blocks
    and     $a3, $a1, $at
    beqz    $a3, .Lbzero_wordzero
     subu   $a1, $a1, $a3
    addu    $a3, $a3, $a0               # block end
.Lbzero_blkloop:
    addiu   $a0, $a0, 0x20
    sw      $zero, -0x20($a0)
    sw      $zero, -0x1C($a0)
    sw      $zero, -0x18($a0)
    sw      $zero, -0x14($a0)
    sw      $zero, -0x10($a0)
    sw      $zero, -0xC($a0)
    sw      $zero, -0x8($a0)
    bne     $a0, $a3, .Lbzero_blkloop
     sw     $zero, -0x4($a0)
.Lbzero_wordzero:
    addiu   $at, $zero, -0x4            # len & ~3: whole words
    and     $a3, $a1, $at
    beqz    $a3, .Lbzero_bytes
     subu   $a1, $a1, $a3
    addu    $a3, $a3, $a0
.Lbzero_wordloop:
    addiu   $a0, $a0, 0x4
    bne     $a0, $a3, .Lbzero_wordloop
     sw     $zero, -0x4($a0)
.Lbzero_bytes:
    blez    $a1, .Lbzero_done
     nop
    addu    $a1, $a1, $a0
.Lbzero_byteloop:
    addiu   $a0, $a0, 0x1
    bne     $a0, $a1, .Lbzero_byteloop
     sb     $zero, -0x1($a0)
.Lbzero_done:
    jr      $ra
     nop
endlabel func_80002CD0
    .space  0x44                        # object alignment padding (17 words)
