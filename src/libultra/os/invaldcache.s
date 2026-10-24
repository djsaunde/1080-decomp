# osInvalDCache -- libultra handwritten assembly (os/invaldcache.s).
# Reference: references/oot/src/libultra/os/invaldcache.s,
#            references/libreultra/src/os/invaldcache.s
# void osInvalDCache(void *vaddr, s32 nbytes)
# splat symbol: func_80005000.
#   cache 0x15 = CACH_PD | C_HWBINV (Hit-Writeback-Invalidate),
#         0x11 = CACH_PD | C_HINV (Hit-Invalidate),
#         0x01 = CACH_PD | C_IWBINV (Index-Writeback-Invalidate)
#   DCACHE_SIZE 0x2000, DCACHE_LINESIZE 0x10

glabel func_80005000
    blez    $a1, .LinvDCache_ret        # nbytes <= 0: nothing to do
     nop
    addiu   $t3, $zero, 0x2000          # DCACHE_SIZE
    sltu    $at, $a1, $t3
    beqz    $at, .LinvDCache_all        # nbytes >= cache size: invalidate all
     nop
    or      $t0, $a0, $zero             # t0 = start
    addu    $t1, $a0, $a1               # t1 = end
    sltu    $at, $t0, $t1
    beqz    $at, .LinvDCache_ret        # end wrapped around
     nop
    andi    $t2, $t0, 0xF               # DCACHE_LINEMASK
    beqz    $t2, .LinvDCache_endchk     # start already line aligned
     addiu  $t1, $t1, -0x10
    subu    $t0, $t0, $t2
    cache   0x15, 0x0($t0)              # writeback+invalidate partial first line
    sltu    $at, $t0, $t1
    beqz    $at, .LinvDCache_ret        # that was all
     nop
    addiu   $t0, $t0, 0x10
.LinvDCache_endchk:
    andi    $t2, $t1, 0xF
    beqz    $t2, .LinvDCache_hit        # end already line aligned
     nop
    subu    $t1, $t1, $t2
    cache   0x15, 0x10($t1)             # writeback+invalidate partial last line
    sltu    $at, $t1, $t0
    bnez    $at, .LinvDCache_ret        # that was all
     nop
.LinvDCache_hit:
    cache   0x11, 0x0($t0)              # Hit-Invalidate the rest
    sltu    $at, $t0, $t1
    bnez    $at, .LinvDCache_hit
     addiu  $t0, $t0, 0x10
.LinvDCache_ret:
    jr      $ra
     nop
.LinvDCache_all:
    lui     $t0, 0x8000                 # K0BASE
    addu    $t1, $t0, $t3
    addiu   $t1, $t1, -0x10
.LinvDCache_index:
    cache   0x01, 0x0($t0)              # Index-Writeback-Invalidate
    sltu    $at, $t0, $t1
    bnez    $at, .LinvDCache_index
     addiu  $t0, $t0, 0x10
    jr      $ra
     nop
endlabel func_80005000
    nop                                 # object alignment padding
    nop
    nop
    nop
    nop
