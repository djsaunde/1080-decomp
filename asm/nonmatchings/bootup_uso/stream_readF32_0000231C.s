nonmatching stream_readF32_0000231C, 0x3C

glabel stream_readF32_0000231C
    /* DD2D88 0000231C 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD2D8C 00002320 AFA40020 */  sw         $a0, 0x20($sp)
    /* DD2D90 00002324 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD2D94 00002328 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD2D98 0000232C 24840000 */  addiu      $a0, $a0, 0x0
    /* DD2D9C 00002330 27A50018 */  addiu      $a1, $sp, 0x18
    /* DD2DA0 00002334 0C000000 */  jal        func_00000000
    /* DD2DA4 00002338 24060004 */   addiu     $a2, $zero, 0x4
    /* DD2DA8 0000233C C7A40018 */  lwc1       $f4, 0x18($sp)
    /* DD2DAC 00002340 8FAE0020 */  lw         $t6, 0x20($sp)
    /* DD2DB0 00002344 E5C40000 */  swc1       $f4, 0x0($t6)
    /* DD2DB4 00002348 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD2DB8 0000234C 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD2DBC 00002350 03E00008 */  jr         $ra
    /* DD2DC0 00002354 00000000 */   nop
endlabel stream_readF32_0000231C
