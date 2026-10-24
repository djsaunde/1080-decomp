# __osGetCause -- libultra handwritten assembly (os/getcause.s).
# Reference: references/oot/src/libultra/os/getcause.s,
#            references/libreultra/src/os/getcause.s
# u32 __osGetCause(void): returns the CP0 Cause register.
# splat symbol: func_80009AA0.

glabel func_80009AA0
    mfc0    $v0, $13            # v0 = C0_CAUSE
    jr      $ra
     nop
endlabel func_80009AA0
    nop                         # object alignment padding
