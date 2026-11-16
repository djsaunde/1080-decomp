nonmatching snd_calcEmitterDistPan_000003F8, 0x184

glabel snd_calcEmitterDistPan_000003F8
    /* DD0E64 000003F8 27BDFFB8 */  addiu      $sp, $sp, -0x48
    /* DD0E68 000003FC AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD0E6C 00000400 AFA60050 */  sw         $a2, 0x50($sp)
    /* DD0E70 00000404 8C980000 */  lw         $t8, 0x0($a0)
    /* DD0E74 00000408 27AE003C */  addiu      $t6, $sp, 0x3C
    /* DD0E78 0000040C ADD80000 */  sw         $t8, 0x0($t6)
    /* DD0E7C 00000410 8C8F0004 */  lw         $t7, 0x4($a0)
    /* DD0E80 00000414 ADCF0004 */  sw         $t7, 0x4($t6)
    /* DD0E84 00000418 8C980008 */  lw         $t8, 0x8($a0)
    /* DD0E88 0000041C ADD80008 */  sw         $t8, 0x8($t6)
    /* DD0E8C 00000420 8CF90070 */  lw         $t9, 0x70($a3)
    /* DD0E90 00000424 C7A4003C */  lwc1       $f4, 0x3C($sp)
    /* DD0E94 00000428 C7AA0040 */  lwc1       $f10, 0x40($sp)
    /* DD0E98 0000042C C72600A0 */  lwc1       $f6, 0xA0($t9)
    /* DD0E9C 00000430 46062201 */  sub.s      $f8, $f4, $f6
    /* DD0EA0 00000434 C7A40044 */  lwc1       $f4, 0x44($sp)
    /* DD0EA4 00000438 E7A8003C */  swc1       $f8, 0x3C($sp)
    /* DD0EA8 0000043C 8CE80070 */  lw         $t0, 0x70($a3)
    /* DD0EAC 00000440 C51000A4 */  lwc1       $f16, 0xA4($t0)
    /* DD0EB0 00000444 46105481 */  sub.s      $f18, $f10, $f16
    /* DD0EB4 00000448 46084402 */  mul.s      $f16, $f8, $f8
    /* DD0EB8 0000044C E7B20040 */  swc1       $f18, 0x40($sp)
    /* DD0EBC 00000450 8CE90070 */  lw         $t1, 0x70($a3)
    /* DD0EC0 00000454 C52600A8 */  lwc1       $f6, 0xA8($t1)
    /* DD0EC4 00000458 AFA70054 */  sw         $a3, 0x54($sp)
    /* DD0EC8 0000045C AFA5004C */  sw         $a1, 0x4C($sp)
    /* DD0ECC 00000460 46062281 */  sub.s      $f10, $f4, $f6
    /* DD0ED0 00000464 46129102 */  mul.s      $f4, $f18, $f18
    /* DD0ED4 00000468 E7AA0044 */  swc1       $f10, 0x44($sp)
    /* DD0ED8 0000046C 460A5202 */  mul.s      $f8, $f10, $f10
    /* DD0EDC 00000470 46048180 */  add.s      $f6, $f16, $f4
    /* DD0EE0 00000474 0C000000 */  jal        func_00000000
    /* DD0EE4 00000478 46083300 */   add.s     $f12, $f6, $f8
    /* DD0EE8 0000047C 3C013FF0 */  lui        $at, (0x3FF00000 >> 16)
    /* DD0EEC 00000480 44816800 */  mtc1       $at, $f13
    /* DD0EF0 00000484 44806000 */  mtc1       $zero, $f12
    /* DD0EF4 00000488 460004A1 */  cvt.d.s    $f18, $f0
    /* DD0EF8 0000048C 8FA5004C */  lw         $a1, 0x4C($sp)
    /* DD0EFC 00000490 46326403 */  div.d      $f16, $f12, $f18
    /* DD0F00 00000494 8FA70054 */  lw         $a3, 0x54($sp)
    /* DD0F04 00000498 E4A00000 */  swc1       $f0, 0x0($a1)
    /* DD0F08 0000049C C7A4003C */  lwc1       $f4, 0x3C($sp)
    /* DD0F0C 000004A0 C7A60040 */  lwc1       $f6, 0x40($sp)
    /* DD0F10 000004A4 C7B20044 */  lwc1       $f18, 0x44($sp)
    /* DD0F14 000004A8 3C01427C */  lui        $at, (0x427C0000 >> 16)
    /* DD0F18 000004AC 8FAF0050 */  lw         $t7, 0x50($sp)
    /* DD0F1C 000004B0 462080A0 */  cvt.s.d    $f2, $f16
    /* DD0F20 000004B4 46022282 */  mul.s      $f10, $f4, $f2
    /* DD0F24 000004B8 00000000 */  nop
    /* DD0F28 000004BC 46023202 */  mul.s      $f8, $f6, $f2
    /* DD0F2C 000004C0 00000000 */  nop
    /* DD0F30 000004C4 46029402 */  mul.s      $f16, $f18, $f2
    /* DD0F34 000004C8 E7AA003C */  swc1       $f10, 0x3C($sp)
    /* DD0F38 000004CC E7A80040 */  swc1       $f8, 0x40($sp)
    /* DD0F3C 000004D0 E7B00044 */  swc1       $f16, 0x44($sp)
    /* DD0F40 000004D4 8CEA0070 */  lw         $t2, 0x70($a3)
    /* DD0F44 000004D8 C54400B4 */  lwc1       $f4, 0xB4($t2)
    /* DD0F48 000004DC E7A40030 */  swc1       $f4, 0x30($sp)
    /* DD0F4C 000004E0 8CEB0070 */  lw         $t3, 0x70($a3)
    /* DD0F50 000004E4 C7A40030 */  lwc1       $f4, 0x30($sp)
    /* DD0F54 000004E8 C56600C4 */  lwc1       $f6, 0xC4($t3)
    /* DD0F58 000004EC E7A60034 */  swc1       $f6, 0x34($sp)
    /* DD0F5C 000004F0 8CEC0070 */  lw         $t4, 0x70($a3)
    /* DD0F60 000004F4 46045182 */  mul.s      $f6, $f10, $f4
    /* DD0F64 000004F8 C59200D4 */  lwc1       $f18, 0xD4($t4)
    /* DD0F68 000004FC E7B20038 */  swc1       $f18, 0x38($sp)
    /* DD0F6C 00000500 C7B20034 */  lwc1       $f18, 0x34($sp)
    /* DD0F70 00000504 46124282 */  mul.s      $f10, $f8, $f18
    /* DD0F74 00000508 C7A80038 */  lwc1       $f8, 0x38($sp)
    /* DD0F78 0000050C 46088482 */  mul.s      $f18, $f16, $f8
    /* DD0F7C 00000510 460A3100 */  add.s      $f4, $f6, $f10
    /* DD0F80 00000514 44815000 */  mtc1       $at, $f10
    /* DD0F84 00000518 3C010000 */  lui        $at, %hi(stream_readF32_00000044 + 0xC)
    /* DD0F88 0000051C 46122180 */  add.s      $f6, $f4, $f18
    /* DD0F8C 00000520 460A3402 */  mul.s      $f16, $f6, $f10
    /* DD0F90 00000524 4600820D */  trunc.w.s  $f8, $f16
    /* DD0F94 00000528 440E4000 */  mfc1       $t6, $f8
    /* DD0F98 0000052C 00000000 */  nop
    /* DD0F9C 00000530 ADEE0000 */  sw         $t6, 0x0($t7)
    /* DD0FA0 00000534 C4320050 */  lwc1       $f18, %lo(stream_readF32_00000044 + 0xC)($at)
    /* DD0FA4 00000538 C4A40000 */  lwc1       $f4, 0x0($a1)
    /* DD0FA8 0000053C 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DD0FAC 00000540 46122183 */  div.s      $f6, $f4, $f18
    /* DD0FB0 00000544 E4A60000 */  swc1       $f6, 0x0($a1)
    /* DD0FB4 00000548 C4AA0000 */  lwc1       $f10, 0x0($a1)
    /* DD0FB8 0000054C 46005421 */  cvt.d.s    $f16, $f10
    /* DD0FBC 00000550 4630603C */  c.lt.d     $f12, $f16
    /* DD0FC0 00000554 00000000 */  nop
    /* DD0FC4 00000558 45000004 */  bc1f       .L0000056C
    /* DD0FC8 0000055C 00000000 */   nop
    /* DD0FCC 00000560 44814000 */  mtc1       $at, $f8
    /* DD0FD0 00000564 00000000 */  nop
    /* DD0FD4 00000568 E4A80000 */  swc1       $f8, 0x0($a1)
  .L0000056C:
    /* DD0FD8 0000056C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD0FDC 00000570 27BD0048 */  addiu      $sp, $sp, 0x48
    /* DD0FE0 00000574 03E00008 */  jr         $ra
    /* DD0FE4 00000578 00000000 */   nop
endlabel snd_calcEmitterDistPan_000003F8
