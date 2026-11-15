nonmatching SndHandle_triggerEvent0_000008F4, 0x4C

glabel SndHandle_triggerEvent0_000008F4
    /* DD1360 000008F4 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD1364 000008F8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD1368 000008FC AFA50024 */  sw         $a1, 0x24($sp)
    /* DD136C 00000900 8C850008 */  lw         $a1, 0x8($a0)
    /* DD1370 00000904 0C000000 */  jal        func_00000000
    /* DD1374 00000908 AFA40020 */   sw        $a0, 0x20($sp)
    /* DD1378 0000090C 8FA40020 */  lw         $a0, 0x20($sp)
    /* DD137C 00000910 00402825 */  or         $a1, $v0, $zero
    /* DD1380 00000914 00003025 */  or         $a2, $zero, $zero
    /* DD1384 00000918 8C8E0000 */  lw         $t6, 0x0($a0)
    /* DD1388 0000091C 8FA70024 */  lw         $a3, 0x24($sp)
    /* DD138C 00000920 11C00003 */  beqz       $t6, .L00000930
    /* DD1390 00000924 AFAE0018 */   sw        $t6, 0x18($sp)
    /* DD1394 00000928 0C000000 */  jal        func_00000000
    /* DD1398 0000092C 01C02025 */   or        $a0, $t6, $zero
  .L00000930:
    /* DD139C 00000930 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD13A0 00000934 27BD0020 */  addiu      $sp, $sp, 0x20
  alabel D_00000938
    /* DD13A4 00000938 03E00008 */  jr         $ra
  alabel D_0000093C
    /* DD13A8 0000093C 00000000 */   nop
endlabel SndHandle_triggerEvent0_000008F4
