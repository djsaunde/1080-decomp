nonmatching SndHandle_triggerEvent1_0000098C, 0x4C

glabel SndHandle_triggerEvent1_0000098C
    /* DD13F8 0000098C 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD13FC 00000990 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD1400 00000994 AFA50024 */  sw         $a1, 0x24($sp)
    /* DD1404 00000998 8C850008 */  lw         $a1, 0x8($a0)
    /* DD1408 0000099C 0C000000 */  jal        func_00000000
    /* DD140C 000009A0 AFA40020 */   sw        $a0, 0x20($sp)
    /* DD1410 000009A4 8FA40020 */  lw         $a0, 0x20($sp)
    /* DD1414 000009A8 00402825 */  or         $a1, $v0, $zero
    /* DD1418 000009AC 24060001 */  addiu      $a2, $zero, 0x1
    /* DD141C 000009B0 8C8E0000 */  lw         $t6, 0x0($a0)
    /* DD1420 000009B4 8FA70024 */  lw         $a3, 0x24($sp)
    /* DD1424 000009B8 11C00003 */  beqz       $t6, .L000009C8
    /* DD1428 000009BC AFAE0018 */   sw        $t6, 0x18($sp)
    /* DD142C 000009C0 0C000000 */  jal        func_00000000
    /* DD1430 000009C4 01C02025 */   or        $a0, $t6, $zero
  .L000009C8:
    /* DD1434 000009C8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD1438 000009CC 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD143C 000009D0 03E00008 */  jr         $ra
    /* DD1440 000009D4 00000000 */   nop
endlabel SndHandle_triggerEvent1_0000098C
