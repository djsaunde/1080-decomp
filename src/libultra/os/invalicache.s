# osInvalICache -- libultra handwritten assembly (os/invalicache.s).
# Reference: references/oot/src/libultra/os/invalicache.s,
#            references/libreultra/src/os/invalicache.s
# void osInvalICache(void *vaddr, s32 nbytes)
# splat symbol: func_80004F50.
#   cache 0x10 = CACH_PI | C_HINV (Hit-Invalidate), 0x00 = CACH_PI | C_IINV
#   ICACHE_SIZE 0x4000, ICACHE_LINESIZE 0x20

glabel func_80004F50
    blez    $a1, .LinvICache_ret        # nbytes <= 0: nothing to do
     nop
    addiu   $t3, $zero, 0x4000          # ICACHE_SIZE
    sltu    $at, $a1, $t3
    beqz    $at, .LinvICache_all        # nbytes >= cache size: invalidate all
     nop
    or      $t0, $a0, $zero             # t0 = start
    addu    $t1, $a0, $a1               # t1 = end
    sltu    $at, $t0, $t1
    beqz    $at, .LinvICache_ret        # end wrapped around
     nop
    andi    $t2, $t0, 0x1F              # ICACHE_LINEMASK
    addiu   $t1, $t1, -0x20
    subu    $t0, $t0, $t2               # align start to a line
.LinvICache_hit:
    cache   0x10, 0x0($t0)              # Hit-Invalidate
    sltu    $at, $t0, $t1
    bnez    $at, .LinvICache_hit
     addiu  $t0, $t0, 0x20
.LinvICache_ret:
    jr      $ra
     nop
.LinvICache_all:
    lui     $t0, 0x8000                 # K0BASE
    addu    $t1, $t0, $t3
    addiu   $t1, $t1, -0x20
.LinvICache_index:
    cache   0x00, 0x0($t0)              # Index-Invalidate
    sltu    $at, $t0, $t1
    bnez    $at, .LinvICache_index
     addiu  $t0, $t0, 0x20
    jr      $ra
     nop
endlabel func_80004F50
    nop                                 # object alignment padding
    nop
    nop
