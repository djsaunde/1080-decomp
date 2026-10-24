# __osSetSR -- libultra handwritten assembly (os/setsr.s).
# Reference: references/oot/src/libultra/os/setsr.s,
#            references/libreultra/src/os/setsr.s
# void __osSetSR(u32 value): writes the CP0 Status register.
#
# splat symbol func_80002DB0 begins with 8 zero words of padding that precede
# __osSetSR (at 0x80002DD0, 16-byte aligned); they are written as nops inside
# the label to keep the symbol unchanged.

glabel func_80002DB0
    nop                         # alignment padding (8 words)
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    # void __osSetSR(u32 value)
    mtc0    $a0, $12            # C0_SR = value
    nop                         # CP0 hazard
    jr      $ra
     nop
endlabel func_80002DB0
