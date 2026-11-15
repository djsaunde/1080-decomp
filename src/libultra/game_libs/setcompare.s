# __osSetCompare, game_libs copy -- libultra handwritten assembly
# (os/setcompare.s).
# (libultra is statically linked into game_libs as well as the kernel;
# the code is byte-identical, only the splat symbol boundaries differ.)
# Reference: references/oot/src/libultra/os/setcompare.s
# void __osSetCompare(u32 value)

glabel gl___osSetCompare
    mtc0    $a0, $11                   # C0_COMPARE = value
    jr      $ra
     nop
endlabel gl___osSetCompare
    nop                                 # object alignment padding
