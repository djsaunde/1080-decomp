# __osDisableInt, game_libs copy -- libultra handwritten assembly (first
# routine of the old-libultra os/interrupt.s).
# (libultra is statically linked into game_libs as well as the kernel;
# the code is byte-identical, only the splat symbol boundaries differ.)
# Reference: references/libreultra/src/os/interrupt.s; kernel copy
# src/libultra/os/disableint.s.
# splat's symbol starts with the zero word that pads the preceding object.

glabel game_libs_func_00070AC0
    nop                                 # alignment padding (preceding object)
    # u32 __osDisableInt(void)
    mfc0    $t0, $12                   # t0 = C0_SR
    addiu   $at, $zero, -2             # ~SR_IE
    and     $t1, $t0, $at
    mtc0    $t1, $12                   # C0_SR = t0 & ~SR_IE
    andi    $v0, $t0, 0x1              # return t0 & SR_IE
    nop                                 # CP0 hazard
    jr      $ra
     nop
endlabel game_libs_func_00070AC0
