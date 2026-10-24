# __osSetSR, game_libs copy -- libultra handwritten assembly (os/setsr.s).
# (libultra is statically linked into game_libs as well as the kernel;
# the code is byte-identical, only the splat symbol boundaries differ.)
# Reference: references/oot/src/libultra/os/setsr.s; kernel copy
# src/libultra/os/setsr.s.

glabel game_libs_func_0006C9D4
    mtc0    $a0, $12                   # C0_SR = value
    nop                                 # CP0 hazard
    jr      $ra
     nop
endlabel game_libs_func_0006C9D4
