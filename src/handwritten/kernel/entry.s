# Boot entry point -- handwritten assembly (the game's entry.s; same shape as
# the SDK boot stub used across N64 titles, cf. references/oot/src/makerom/entry.s
# and references/papermario/src/boot/entry_point.s). IPL3 jumps here.
# addi (trapping add) is assembler output IDO never emits for C.
#   D_8000ABC0 = start of .bss, 0x153E0 = .bss size (cleared 8 bytes a loop)
#   D_80000460 = runtime address of func_80000060 (boot main; the kernel
#   runs at +0x400), D_80012BC0 = boot stack top

glabel func_80000000
    lui     $t0, %hi(D_8000ABC0)        # t0 = bss start
    lui     $t1, 0x1                    # t1 = bss size (0x153E0)
    addiu   $t0, $t0, %lo(D_8000ABC0)
    ori     $t1, $t1, 0x53E0
.Lentry_clearbss:
    addi    $t1, $t1, -0x8
    sw      $zero, 0x0($t0)
    sw      $zero, 0x4($t0)
    bnez    $t1, .Lentry_clearbss
     addi   $t0, $t0, 0x8
    lui     $t2, %hi(D_80000460)        # t2 = boot routine
    lui     $sp, %hi(D_80012BC0)        # sp = boot stack top
    addiu   $t2, $t2, %lo(D_80000460)
    jr      $t2
     addiu  $sp, $sp, %lo(D_80012BC0)
    nop                                 # padding to 0x80000060
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
endlabel func_80000000
