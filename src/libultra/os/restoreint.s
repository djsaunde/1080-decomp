# __osRestoreInt -- libultra handwritten assembly (second routine of the
# old-libultra os/interrupt.s).
# Reference: references/libreultra/src/os/interrupt.s
# void __osRestoreInt(u32 im): ORs the mask returned by __osDisableInt back
# into the Status register.
# splat symbol: func_800062D0.

glabel func_800062D0
    mfc0    $t0, $12            # t0 = C0_SR
    or      $t0, $t0, $a0
    mtc0    $t0, $12            # C0_SR = t0 | im
    nop                         # CP0 hazard
    nop
    jr      $ra
     nop
endlabel func_800062D0
    nop                         # object alignment padding
