# __osDisableInt -- libultra handwritten assembly (first routine of the
# old-libultra os/interrupt.s; this game's copy predates the OoT version that
# also tracks __OSGlobalIntMask).
# Reference: references/libreultra/src/os/interrupt.s
# u32 __osDisableInt(void): clears SR_IE, returns the previous SR_IE bit.
# splat symbol: func_800062B0.

glabel func_800062B0
    mfc0    $t0, $12            # t0 = C0_SR
    addiu   $at, $zero, -2      # ~SR_IE
    and     $t1, $t0, $at
    mtc0    $t1, $12            # C0_SR = t0 & ~SR_IE
    andi    $v0, $t0, 0x1       # return t0 & SR_IE
    nop                         # CP0 hazard
    jr      $ra
     nop
endlabel func_800062B0
