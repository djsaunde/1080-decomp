# __osRestoreInt, game_libs copy -- libultra handwritten assembly (second
# routine of the old-libultra os/interrupt.s).
# (libultra is statically linked into game_libs as well as the kernel;
# the code is byte-identical, only the splat symbol boundaries differ.)
# Reference: references/libreultra/src/os/interrupt.s; kernel copy
# src/libultra/os/restoreint.s.
# splat's symbol ends after the jr; the delay-slot nop and the object padding
# follow in the host unit's next block.

glabel game_libs_func_00070AE4
    mfc0    $t0, $12                   # t0 = C0_SR
    or      $t0, $t0, $a0
    mtc0    $t0, $12                   # C0_SR = t0 | im
    nop                                 # CP0 hazard
    nop
    jr      $ra                         # (delay slot: next block's nop)
endlabel game_libs_func_00070AE4
