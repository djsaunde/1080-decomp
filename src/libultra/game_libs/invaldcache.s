# osInvalDCache, game_libs copy -- libultra handwritten assembly
# (libultra is statically linked into game_libs as well as the kernel;
# the code is byte-identical, only the splat symbol boundaries differ.)
# Reference: references/oot/src/libultra/os/invaldcache.s; kernel copy
# src/libultra/os/invaldcache.s.


glabel game_libs_func_0006F2D4
    blez    $a1, .Lgl_invDCache_ret        # nbytes <= 0: nothing to do
     nop
    addiu   $t3, $zero, 0x2000          # DCACHE_SIZE
    sltu    $at, $a1, $t3
    beqz    $at, .Lgl_invDCache_all        # nbytes >= cache size: invalidate all
     nop
    or      $t0, $a0, $zero             # t0 = start
    addu    $t1, $a0, $a1               # t1 = end
    sltu    $at, $t0, $t1
    beqz    $at, .Lgl_invDCache_ret        # end wrapped around
     nop
    andi    $t2, $t0, 0xF               # DCACHE_LINEMASK
    beqz    $t2, .Lgl_invDCache_endchk     # start already line aligned
     addiu  $t1, $t1, -0x10
    subu    $t0, $t0, $t2
    cache   0x15, 0x0($t0)              # writeback+invalidate partial first line
    sltu    $at, $t0, $t1
    beqz    $at, .Lgl_invDCache_ret        # that was all
     nop
    addiu   $t0, $t0, 0x10
.Lgl_invDCache_endchk:
    andi    $t2, $t1, 0xF
    beqz    $t2, .Lgl_invDCache_hit        # end already line aligned
     nop
    subu    $t1, $t1, $t2
    cache   0x15, 0x10($t1)             # writeback+invalidate partial last line
    sltu    $at, $t1, $t0
    bnez    $at, .Lgl_invDCache_ret        # that was all
     nop
.Lgl_invDCache_hit:
    cache   0x11, 0x0($t0)              # Hit-Invalidate the rest
    sltu    $at, $t0, $t1
    bnez    $at, .Lgl_invDCache_hit
     addiu  $t0, $t0, 0x10
.Lgl_invDCache_ret:
    jr      $ra
     nop
.Lgl_invDCache_all:
    lui     $t0, 0x8000                 # K0BASE
    addu    $t1, $t0, $t3
    addiu   $t1, $t1, -0x10
.Lgl_invDCache_index:
    cache   0x01, 0x0($t0)              # Index-Writeback-Invalidate
    sltu    $at, $t0, $t1
    bnez    $at, .Lgl_invDCache_index
     addiu  $t0, $t0, 0x10
    jr      $ra
     nop
endlabel game_libs_func_0006F2D4
