# __osGetSR -- libultra handwritten assembly (os/getsr.s).
# Reference: references/oot/src/libultra/os/getsr.s,
#            references/libreultra/src/os/getsr.s
# u32 __osGetSR(void): returns the CP0 Status register.
# splat symbol: __osGetSR.

glabel __osGetSR
    mfc0    $v0, $12            # v0 = C0_SR
    jr      $ra
     nop
endlabel __osGetSR
    nop                         # object alignment padding
