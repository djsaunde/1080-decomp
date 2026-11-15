nonmatching stream_readQuad_0000F008, 0x58

glabel stream_readQuad_0000F008
    /* DDFA74 0000F008 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DDFA78 0000F00C AFA40028 */  sw         $a0, 0x28($sp)
    /* DDFA7C 0000F010 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDFA80 0000F014 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDFA84 0000F018 24840000 */  addiu      $a0, $a0, 0x0
    /* DDFA88 0000F01C 27A50018 */  addiu      $a1, $sp, 0x18
    /* DDFA8C 0000F020 0C000000 */  jal        func_00000000
    /* DDFA90 0000F024 24060010 */   addiu     $a2, $zero, 0x10
    /* DDFA94 0000F028 27AF0018 */  addiu      $t7, $sp, 0x18
    /* DDFA98 0000F02C 8DF90000 */  lw         $t9, 0x0($t7)
    /* DDFA9C 0000F030 8FAE0028 */  lw         $t6, 0x28($sp)
    /* DDFAA0 0000F034 ADD90000 */  sw         $t9, 0x0($t6)
    /* DDFAA4 0000F038 8DF80004 */  lw         $t8, 0x4($t7)
    /* DDFAA8 0000F03C ADD80004 */  sw         $t8, 0x4($t6)
    /* DDFAAC 0000F040 8DF90008 */  lw         $t9, 0x8($t7)
    /* DDFAB0 0000F044 ADD90008 */  sw         $t9, 0x8($t6)
    /* DDFAB4 0000F048 8DF8000C */  lw         $t8, 0xC($t7)
    /* DDFAB8 0000F04C ADD8000C */  sw         $t8, 0xC($t6)
    /* DDFABC 0000F050 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDFAC0 0000F054 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DDFAC4 0000F058 03E00008 */  jr         $ra
    /* DDFAC8 0000F05C 00000000 */   nop
endlabel stream_readQuad_0000F008
