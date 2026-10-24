# __osGetSR -- libultra handwritten assembly (os/getsr.s).
# Reference: references/oot/src/libultra/os/getsr.s,
#            references/libreultra/src/os/getsr.s
# u32 __osGetSR(void): returns the CP0 Status register.
# splat symbol: func_80002DE0.

glabel func_80002DE0
    mfc0    $v0, $12            # v0 = C0_SR
    jr      $ra
     nop
endlabel func_80002DE0
    nop                         # object alignment padding
