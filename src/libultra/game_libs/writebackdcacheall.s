# osWritebackDCacheAll, game_libs copy -- libultra handwritten assembly
# (os/writebackdcacheall.s).
# (libultra is statically linked into game_libs as well as the kernel;
# the code is byte-identical, only the splat symbol boundaries differ.)
# Reference: references/oot/src/libultra/os/writebackdcacheall.s; kernel copy
# src/libultra/os/writebackdcacheall.s.
# splat's symbol starts with the zero word that pads the preceding object.
#   cache op 0x01 = CACH_PD | C_IWBINV; DCACHE_SIZE 0x2000, line 0x10.

glabel game_libs_func_00074860
    nop                                 # alignment padding (preceding object)
    # void osWritebackDCacheAll(void)
    lui     $t0, 0x8000                 # t0 = K0BASE
    addiu   $t2, $zero, 0x2000          # DCACHE_SIZE
    addu    $t1, $t0, $t2
    addiu   $t1, $t1, -0x10             # last line
.Lgl_wbDCacheAll_loop:
    cache   0x01, 0x0($t0)              # Index-Writeback-Invalidate
    sltu    $at, $t0, $t1
    bnez    $at, .Lgl_wbDCacheAll_loop
     addiu  $t0, $t0, 0x10              # DCACHE_LINESIZE
    jr      $ra
     nop
endlabel game_libs_func_00074860
