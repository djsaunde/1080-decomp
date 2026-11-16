nonmatching Snower_display, 0xD8

glabel Snower_display
    /* DD8D64 000082F8 3C020000 */  lui        $v0, (0x0 >> 16)
    /* DD8D68 000082FC 24420000 */  addiu      $v0, $v0, 0x0
    /* DD8D6C 00008300 8C4E0254 */  lw         $t6, 0x254($v0)
    /* DD8D70 00008304 27BDFFD0 */  addiu      $sp, $sp, -0x30
    /* DD8D74 00008308 AFBF002C */  sw         $ra, 0x2C($sp)
    /* DD8D78 0000830C AFB00028 */  sw         $s0, 0x28($sp)
    /* DD8D7C 00008310 8DCF0070 */  lw         $t7, 0x70($t6)
    /* DD8D80 00008314 C4400130 */  lwc1       $f0, 0x130($v0)
    /* DD8D84 00008318 C4860038 */  lwc1       $f6, 0x38($a0)
    /* DD8D88 0000831C C5E400A8 */  lwc1       $f4, 0xA8($t7)
    /* DD8D8C 00008320 00808025 */  or         $s0, $a0, $zero
    /* DD8D90 00008324 46002082 */  mul.s      $f2, $f4, $f0
    /* DD8D94 00008328 00000000 */  nop
    /* DD8D98 0000832C 46003302 */  mul.s      $f12, $f6, $f0
    /* DD8D9C 00008330 4602603C */  c.lt.s     $f12, $f2
    /* DD8DA0 00008334 00000000 */  nop
    /* DD8DA4 00008338 4500000D */  bc1f       .L00008370
    /* DD8DA8 0000833C 00000000 */   nop
    /* DD8DAC 00008340 C4880054 */  lwc1       $f8, 0x54($a0)
    /* DD8DB0 00008344 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD8DB4 00008348 460C4280 */  add.s      $f10, $f8, $f12
    /* DD8DB8 0000834C 460A103C */  c.lt.s     $f2, $f10
    /* DD8DBC 00008350 00000000 */  nop
    /* DD8DC0 00008354 45000006 */  bc1f       .L00008370
    /* DD8DC4 00008358 00000000 */   nop
    /* DD8DC8 0000835C 8C840000 */  lw         $a0, 0x0($a0)
    /* DD8DCC 00008360 0C000000 */  jal        func_00000000
    /* DD8DD0 00008364 8E05006C */   lw        $a1, 0x6C($s0)
    /* DD8DD4 00008368 3C020000 */  lui        $v0, (0x0 >> 16)
    /* DD8DD8 0000836C 24420000 */  addiu      $v0, $v0, 0x0
  .L00008370:
    /* DD8DDC 00008370 3C180000 */  lui        $t8, %hi(stream_readS32_00000008 + 0x2C)
    /* DD8DE0 00008374 8F180034 */  lw         $t8, %lo(stream_readS32_00000008 + 0x2C)($t8)
    /* DD8DE4 00008378 24010008 */  addiu      $at, $zero, 0x8
    /* DD8DE8 0000837C 24050003 */  addiu      $a1, $zero, 0x3
    /* DD8DEC 00008380 5701000F */  bnel       $t8, $at, .L000083C0
    /* DD8DF0 00008384 8FBF002C */   lw        $ra, 0x2C($sp)
    /* DD8DF4 00008388 C6100038 */  lwc1       $f16, 0x38($s0)
    /* DD8DF8 0000838C 44809000 */  mtc1       $zero, $f18
    /* DD8DFC 00008390 8E060030 */  lw         $a2, 0x30($s0)
    /* DD8E00 00008394 8E070034 */  lw         $a3, 0x34($s0)
    /* DD8E04 00008398 AFA00014 */  sw         $zero, 0x14($sp)
    /* DD8E08 0000839C 8C440254 */  lw         $a0, 0x254($v0)
    /* DD8E0C 000083A0 E7B00010 */  swc1       $f16, 0x10($sp)
    /* DD8E10 000083A4 0C000000 */  jal        func_00000000
    /* DD8E14 000083A8 E7B20018 */   swc1      $f18, 0x18($sp)
    /* DD8E18 000083AC 50400004 */  beql       $v0, $zero, .L000083C0
    /* DD8E1C 000083B0 8FBF002C */   lw        $ra, 0x2C($sp)
    /* DD8E20 000083B4 0C000000 */  jal        func_00000000
    /* DD8E24 000083B8 02002025 */   or        $a0, $s0, $zero
    /* DD8E28 000083BC 8FBF002C */  lw         $ra, 0x2C($sp)
  .L000083C0:
    /* DD8E2C 000083C0 8FB00028 */  lw         $s0, 0x28($sp)
    /* DD8E30 000083C4 27BD0030 */  addiu      $sp, $sp, 0x30
    /* DD8E34 000083C8 03E00008 */  jr         $ra
    /* DD8E38 000083CC 00000000 */   nop
endlabel Snower_display
