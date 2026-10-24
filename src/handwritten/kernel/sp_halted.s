# SP halted/broken test -- handwritten assembly (RSP helper next to the
# three-entry SP control routine func_80008054, src/handwritten/kernel/
# sp_control.s). Returns 1 when SP_STATUS has neither HALT nor BROKE set.
# Not IDO output: a t0-first temp, `move v0,zero` + unfolded `ori v0,v0,1`
# (IDO -O2 folds the constant OR and starts its temp ring at t6), and an
# explicit nop in the bnez delay slot (see the probe battery in kernel_031b.c).

glabel func_80008030
    lui     $t0, %hi(D_A4040010)        # t0 = SP_STATUS_REG
    lw      $t0, %lo(D_A4040010)($t0)
    or      $v0, $zero, $zero           # v0 = 0
    andi    $t0, $t0, 0x3               # HALT | BROKE
    bnez    $t0, .Lsphalted_done
     nop
    ori     $v0, $v0, 0x1               # v0 = 1
.Lsphalted_done:
    jr      $ra
     nop
endlabel func_80008030
