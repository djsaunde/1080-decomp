# osWritebackDCache, game_libs copy -- libultra handwritten assembly
# (libultra is statically linked into game_libs as well as the kernel;
# the code is byte-identical, only the splat symbol boundaries differ.)
# Reference: references/oot/src/libultra/os/writebackdcache.s; kernel copy
# src/libultra/os/writebackdcache.s.
# splat's symbol starts with the zero word that pads the preceding object.

glabel game_libs_func_0006C400
    nop                                 # alignment padding (preceding object)
    blez    $a1, .Lgl_wbDCache_ret         # nbytes <= 0: nothing to do
     nop
    addiu   $t3, $zero, 0x2000          # DCACHE_SIZE
    sltu    $at, $a1, $t3
    beqz    $at, .Lgl_wbDCache_all         # nbytes >= cache size: write back all
     nop
    or      $t0, $a0, $zero             # t0 = start
    addu    $t1, $a0, $a1               # t1 = end
    sltu    $at, $t0, $t1
    beqz    $at, .Lgl_wbDCache_ret         # end wrapped around
     nop
    andi    $t2, $t0, 0xF               # DCACHE_LINEMASK
    addiu   $t1, $t1, -0x10
    subu    $t0, $t0, $t2               # align start to a line
.Lgl_wbDCache_hit:
    cache   0x19, 0x0($t0)              # Hit-Writeback
    sltu    $at, $t0, $t1
    bnez    $at, .Lgl_wbDCache_hit
     addiu  $t0, $t0, 0x10
.Lgl_wbDCache_ret:
    jr      $ra
     nop
.Lgl_wbDCache_all:
    lui     $t0, 0x8000                 # K0BASE
    addu    $t1, $t0, $t3
    addiu   $t1, $t1, -0x10
.Lgl_wbDCache_index:
    cache   0x01, 0x0($t0)              # Index-Writeback-Invalidate
    sltu    $at, $t0, $t1
    bnez    $at, .Lgl_wbDCache_index
     addiu  $t0, $t0, 0x10
    jr      $ra
     nop
endlabel game_libs_func_0006C400
