# osWritebackDCache -- libultra handwritten assembly (os/writebackdcache.s).
# Reference: references/oot/src/libultra/os/writebackdcache.s,
#            references/libreultra/src/os/writebackdcache.s
# void osWritebackDCache(void *vaddr, s32 nbytes)
# splat symbol: func_80002DF0.
#   cache 0x19 = CACH_PD | C_HWB (Hit-Writeback), 0x01 = CACH_PD | C_IWBINV
#   DCACHE_SIZE 0x2000, DCACHE_LINESIZE 0x10

glabel func_80002DF0
    blez    $a1, .LwbDCache_ret         # nbytes <= 0: nothing to do
     nop
    addiu   $t3, $zero, 0x2000          # DCACHE_SIZE
    sltu    $at, $a1, $t3
    beqz    $at, .LwbDCache_all         # nbytes >= cache size: write back all
     nop
    or      $t0, $a0, $zero             # t0 = start
    addu    $t1, $a0, $a1               # t1 = end
    sltu    $at, $t0, $t1
    beqz    $at, .LwbDCache_ret         # end wrapped around
     nop
    andi    $t2, $t0, 0xF               # DCACHE_LINEMASK
    addiu   $t1, $t1, -0x10
    subu    $t0, $t0, $t2               # align start to a line
.LwbDCache_hit:
    cache   0x19, 0x0($t0)              # Hit-Writeback
    sltu    $at, $t0, $t1
    bnez    $at, .LwbDCache_hit
     addiu  $t0, $t0, 0x10
.LwbDCache_ret:
    jr      $ra
     nop
.LwbDCache_all:
    lui     $t0, 0x8000                 # K0BASE
    addu    $t1, $t0, $t3
    addiu   $t1, $t1, -0x10
.LwbDCache_index:
    cache   0x01, 0x0($t0)              # Index-Writeback-Invalidate
    sltu    $at, $t0, $t1
    bnez    $at, .LwbDCache_index
     addiu  $t0, $t0, 0x10
    jr      $ra
     nop
endlabel func_80002DF0
    nop                                 # object alignment padding
    nop
    nop
