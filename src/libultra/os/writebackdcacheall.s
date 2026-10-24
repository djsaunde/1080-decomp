# osWritebackDCacheAll -- libultra handwritten assembly (os/writebackdcacheall.s).
# Reference: references/oot/src/libultra/os/writebackdcacheall.s,
#            references/libreultra/src/os/writebackdcacheall.s
# void osWritebackDCacheAll(void): Index-Writeback-Invalidate every D-cache line.
# splat symbol: func_80009CE0.
#   cache op 0x01 = CACH_PD | C_IWBINV; DCACHE_SIZE 0x2000, line 0x10.

glabel func_80009CE0
    lui     $t0, 0x8000                 # t0 = K0BASE
    addiu   $t2, $zero, 0x2000          # DCACHE_SIZE
    addu    $t1, $t0, $t2
    addiu   $t1, $t1, -0x10             # last line
.LwbDCacheAll_loop:
    cache   0x01, 0x0($t0)              # Index-Writeback-Invalidate
    sltu    $at, $t0, $t1
    bnez    $at, .LwbDCacheAll_loop
     addiu  $t0, $t0, 0x10              # DCACHE_LINESIZE
    jr      $ra
     nop
endlabel func_80009CE0
    nop                                 # object alignment padding
    nop
