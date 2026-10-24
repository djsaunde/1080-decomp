# SP control routines -- handwritten assembly, ONE splat symbol holding three
# entry points (0x80008054 / 0x80008098 / 0x800080AC). Not compiled C: the
# second entry branches into the first entry's wait loop (no C function can
# share code with another entry), every address is a `li` expansion
# (lui + ori, then a 0 offset) and the poll loops have explicit nop delays.
#   SP_STATUS writes: 0x82 = set sig0 | clr broke; 0xC5 = set sig1 |
#   set intr_break | clr dma_busy... (see kernel_020_b.c); 0x125 = clr sig1 |
#   clr sstep | clr intr | clr halt. MI_INTR_MASK 2 = clear SP mask.

glabel func_80008054
    # entry 0: wait for SP DMA idle, set status 0x82, wait for halt|broke
    lui     $a0, 0xA404                 # a0 = SP_DMA_BUSY_REG (0xA4040018)
    ori     $a0, $a0, 0x18
.Lspctl_dmabusy:
    lw      $v0, 0x0($a0)
    bnez    $v0, .Lspctl_dmabusy
     nop
    lui     $a0, 0xA404                 # a0 = SP_STATUS_REG (0xA4040010)
    addiu   $a1, $zero, 0x82
    ori     $a0, $a0, 0x10
    sw      $a1, 0x0($a0)
.Lspctl_waithalt:
    lui     $a0, 0xA404                 # a0 = SP_STATUS_REG
    ori     $a0, $a0, 0x10
    lw      $v0, 0x0($a0)
    andi    $v0, $v0, 0x3               # HALT | BROKE
    beqz    $v0, .Lspctl_waithalt
     nop
    jr      $ra
     nop
    # entry 1 (0x80008098): set status 0xC5, then wait for halt|broke
    lui     $a0, 0xA404                 # a0 = SP_STATUS_REG
    ori     $a0, $a0, 0x10
    addiu   $a1, $zero, 0xC5
    b       .Lspctl_waithalt
     sw     $a1, 0x0($a0)
    # entry 2 (0x800080AC): mask the SP interrupt, set status 0x125
    lui     $a0, 0xA430                 # a0 = MI_INTR_MASK_REG (0xA430000C)
    ori     $a0, $a0, 0xC
    addiu   $a1, $zero, 0x2
    sw      $a1, 0x0($a0)
    lui     $a0, 0xA404                 # a0 = SP_STATUS_REG
    ori     $a0, $a0, 0x10
    addiu   $a1, $zero, 0x125
    jr      $ra
     sw     $a1, 0x0($a0)
endlabel func_80008054
