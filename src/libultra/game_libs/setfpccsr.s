# __osSetFpcCsr, game_libs copy -- libultra handwritten assembly
# (os/setfpccsr.s; libultra is statically linked into game_libs as well as the
# kernel, byte-identical code). Kernel copy: src/libultra/os/setfpccsr.s.
# Reference: references/oot/src/libultra/os/setfpccsr.s
#
# splat's symbol gl___osSetFpcCsr starts one word early: its first word
# is the zero alignment padding that ends the preceding (C) object, so the
# padding nop is written inside the label to keep the symbol unchanged.

glabel gl___osSetFpcCsr
    nop                         # alignment padding (end of preceding object)
    # u32 __osSetFpcCsr(u32 value)
    cfc1    $v0, $31            # v0 = C1_FPCSR (old value)
    ctc1    $a0, $31            # C1_FPCSR = value
    jr      $ra
     nop
endlabel gl___osSetFpcCsr
