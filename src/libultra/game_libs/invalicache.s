# osInvalICache, game_libs copy -- libultra handwritten assembly
# (libultra is statically linked into game_libs as well as the kernel;
# the code is byte-identical, only the splat symbol boundaries differ.)
# Reference: references/oot/src/libultra/os/invalicache.s; kernel copy
# src/libultra/os/invalicache.s.


glabel game_libs_func_0006F254
    blez    $a1, .Lgl_invICache_ret        # nbytes <= 0: nothing to do
     nop
    addiu   $t3, $zero, 0x4000          # ICACHE_SIZE
    sltu    $at, $a1, $t3
    beqz    $at, .Lgl_invICache_all        # nbytes >= cache size: invalidate all
     nop
    or      $t0, $a0, $zero             # t0 = start
    addu    $t1, $a0, $a1               # t1 = end
    sltu    $at, $t0, $t1
    beqz    $at, .Lgl_invICache_ret        # end wrapped around
     nop
    andi    $t2, $t0, 0x1F              # ICACHE_LINEMASK
    addiu   $t1, $t1, -0x20
    subu    $t0, $t0, $t2               # align start to a line
.Lgl_invICache_hit:
    cache   0x10, 0x0($t0)              # Hit-Invalidate
    sltu    $at, $t0, $t1
    bnez    $at, .Lgl_invICache_hit
     addiu  $t0, $t0, 0x20
.Lgl_invICache_ret:
    jr      $ra
     nop
.Lgl_invICache_all:
    lui     $t0, 0x8000                 # K0BASE
    addu    $t1, $t0, $t3
    addiu   $t1, $t1, -0x20
.Lgl_invICache_index:
    cache   0x00, 0x0($t0)              # Index-Invalidate
    sltu    $at, $t0, $t1
    bnez    $at, .Lgl_invICache_index
     addiu  $t0, $t0, 0x20
    jr      $ra
     nop
endlabel game_libs_func_0006F254
    nop                                 # object alignment padding
    nop
    nop
