nonmatching stream_readF32_0000ECBC, 0x3C

glabel stream_readF32_0000ECBC
    /* DDF728 0000ECBC 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDF72C 0000ECC0 AFA40020 */  sw         $a0, 0x20($sp)
    /* DDF730 0000ECC4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF734 0000ECC8 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDF738 0000ECCC 24840000 */  addiu      $a0, $a0, 0x0
    /* DDF73C 0000ECD0 27A50018 */  addiu      $a1, $sp, 0x18
    /* DDF740 0000ECD4 0C000000 */  jal        func_00000000
    /* DDF744 0000ECD8 24060004 */   addiu     $a2, $zero, 0x4
    /* DDF748 0000ECDC C7A40018 */  lwc1       $f4, 0x18($sp)
    /* DDF74C 0000ECE0 8FAE0020 */  lw         $t6, 0x20($sp)
    /* DDF750 0000ECE4 E5C40000 */  swc1       $f4, 0x0($t6)
    /* DDF754 0000ECE8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF758 0000ECEC 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDF75C 0000ECF0 03E00008 */  jr         $ra
    /* DDF760 0000ECF4 00000000 */   nop
endlabel stream_readF32_0000ECBC
