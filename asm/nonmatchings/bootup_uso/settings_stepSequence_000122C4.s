nonmatching settings_stepSequence_000122C4, 0x12C

glabel settings_stepSequence_000122C4
    /* DE2D30 000122C4 27BDFFD0 */  addiu      $sp, $sp, -0x30
    /* DE2D34 000122C8 AFBF001C */  sw         $ra, 0x1C($sp)
    /* DE2D38 000122CC AFA40030 */  sw         $a0, 0x30($sp)
    /* DE2D3C 000122D0 AFA50034 */  sw         $a1, 0x34($sp)
    /* DE2D40 000122D4 AFB00018 */  sw         $s0, 0x18($sp)
    /* DE2D44 000122D8 27AE0024 */  addiu      $t6, $sp, 0x24
    /* DE2D48 000122DC 3C0F0001 */  lui        $t7, %hi(D_0000C69C)
    /* DE2D4C 000122E0 25EFC69C */  addiu      $t7, $t7, %lo(D_0000C69C)
    /* DE2D50 000122E4 8DF90000 */  lw         $t9, 0x0($t7)
    /* DE2D54 000122E8 ADD90000 */  sw         $t9, 0x0($t6)
    /* DE2D58 000122EC 8DF80004 */  lw         $t8, 0x4($t7)
    /* DE2D5C 000122F0 ADD80004 */  sw         $t8, 0x4($t6)
    /* DE2D60 000122F4 8DF90008 */  lw         $t9, 0x8($t7)
    /* DE2D64 000122F8 ADD90008 */  sw         $t9, 0x8($t6)
    /* DE2D68 000122FC 8FA80030 */  lw         $t0, 0x30($sp)
    /* DE2D6C 00012300 8D090154 */  lw         $t1, 0x154($t0)
    /* DE2D70 00012304 952A0002 */  lhu        $t2, 0x2($t1)
    /* DE2D74 00012308 29410004 */  slti       $at, $t2, 0x4
    /* DE2D78 0001230C 10200012 */  beqz       $at, .L00012358
    /* DE2D7C 00012310 00000000 */   nop
    /* DE2D80 00012314 8FAB0034 */  lw         $t3, 0x34($sp)
    /* DE2D84 00012318 256C0001 */  addiu      $t4, $t3, 0x1
    /* DE2D88 0001231C 8FAD0030 */  lw         $t5, 0x30($sp)
    /* DE2D8C 00012320 8DAE0154 */  lw         $t6, 0x154($t5)
    /* DE2D90 00012324 95CF0002 */  lhu        $t7, 0x2($t6)
    /* DE2D94 00012328 158F0009 */  bne        $t4, $t7, .L00012350
    /* DE2D98 0001232C 00000000 */   nop
    /* DE2D9C 00012330 8FB80030 */  lw         $t8, 0x30($sp)
    /* DE2DA0 00012334 8F100154 */  lw         $s0, 0x154($t8)
    /* DE2DA4 00012338 96190002 */  lhu        $t9, 0x2($s0)
    /* DE2DA8 0001233C 27280001 */  addiu      $t0, $t9, 0x1
    /* DE2DAC 00012340 A6080002 */  sh         $t0, 0x2($s0)
    /* DE2DB0 00012344 8FA40030 */  lw         $a0, 0x30($sp)
    /* DE2DB4 00012348 0C000000 */  jal        func_00000000
    /* DE2DB8 0001234C 00000000 */   nop
  .L00012350:
    /* DE2DBC 00012350 10000020 */  b          .L000123D4
    /* DE2DC0 00012354 00000000 */   nop
  .L00012358:
    /* DE2DC4 00012358 8FA90034 */  lw         $t1, 0x34($sp)
    /* DE2DC8 0001235C 24010002 */  addiu      $at, $zero, 0x2
    /* DE2DCC 00012360 1521001C */  bne        $t1, $at, .L000123D4
    /* DE2DD0 00012364 00000000 */   nop
    /* DE2DD4 00012368 8FAA0030 */  lw         $t2, 0x30($sp)
    /* DE2DD8 0001236C 8D4B0154 */  lw         $t3, 0x154($t2)
    /* DE2DDC 00012370 956D0002 */  lhu        $t5, 0x2($t3)
    /* DE2DE0 00012374 29A10004 */  slti       $at, $t5, 0x4
    /* DE2DE4 00012378 14200016 */  bnez       $at, .L000123D4
    /* DE2DE8 0001237C 00000000 */   nop
    /* DE2DEC 00012380 29A10005 */  slti       $at, $t5, 0x5
    /* DE2DF0 00012384 10200013 */  beqz       $at, .L000123D4
    /* DE2DF4 00012388 00000000 */   nop
    /* DE2DF8 0001238C 8FAE0030 */  lw         $t6, 0x30($sp)
    /* DE2DFC 00012390 8DCC004C */  lw         $t4, 0x4C($t6)
    /* DE2E00 00012394 258F0001 */  addiu      $t7, $t4, 0x1
    /* DE2E04 00012398 8DD80154 */  lw         $t8, 0x154($t6)
    /* DE2E08 0001239C 97190002 */  lhu        $t9, 0x2($t8)
    /* DE2E0C 000123A0 00194080 */  sll        $t0, $t9, 2
    /* DE2E10 000123A4 03A84821 */  addu       $t1, $sp, $t0
    /* DE2E14 000123A8 8D290014 */  lw         $t1, 0x14($t1)
    /* DE2E18 000123AC 15E90009 */  bne        $t7, $t1, .L000123D4
    /* DE2E1C 000123B0 00000000 */   nop
    /* DE2E20 000123B4 8FAA0030 */  lw         $t2, 0x30($sp)
    /* DE2E24 000123B8 8D500154 */  lw         $s0, 0x154($t2)
    /* DE2E28 000123BC 960B0002 */  lhu        $t3, 0x2($s0)
    /* DE2E2C 000123C0 256D0001 */  addiu      $t5, $t3, 0x1
    /* DE2E30 000123C4 A60D0002 */  sh         $t5, 0x2($s0)
    /* DE2E34 000123C8 8FA40030 */  lw         $a0, 0x30($sp)
    /* DE2E38 000123CC 0C000000 */  jal        func_00000000
    /* DE2E3C 000123D0 00000000 */   nop
  .L000123D4:
    /* DE2E40 000123D4 10000001 */  b          .L000123DC
    /* DE2E44 000123D8 00000000 */   nop
  .L000123DC:
    /* DE2E48 000123DC 8FB00018 */  lw         $s0, 0x18($sp)
    /* DE2E4C 000123E0 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DE2E50 000123E4 27BD0030 */  addiu      $sp, $sp, 0x30
    /* DE2E54 000123E8 03E00008 */  jr         $ra
    /* DE2E58 000123EC 00000000 */   nop
endlabel settings_stepSequence_000122C4
