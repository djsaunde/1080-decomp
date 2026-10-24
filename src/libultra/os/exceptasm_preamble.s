# __osExceptionPreamble (first half) -- libultra handwritten assembly.
# Copied to the CPU exception vectors; loads the handler address into k0.
# splat splits it: the jr k0 / nop pair opens func_80002E78
# (exceptasm_rdb.s). D_80003344 is the handler's runtime address
# (.Lexc_save_scratch; the kernel image runs 0x400 above its link address).

glabel func_80002E70
    lui     $k0, %hi(D_80003344)
    addiu   $k0, $k0, %lo(D_80003344)
endlabel func_80002E70

