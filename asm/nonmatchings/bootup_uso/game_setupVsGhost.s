nonmatching game_setupVsGhost, 0x1A0

glabel game_setupVsGhost
    /* DD4444 000039D8 27BDFFB0 */  addiu      $sp, $sp, -0x50
    /* DD4448 000039DC AFB20038 */  sw         $s2, 0x38($sp)
    /* DD444C 000039E0 00809025 */  or         $s2, $a0, $zero
    /* DD4450 000039E4 AFBF003C */  sw         $ra, 0x3C($sp)
    /* DD4454 000039E8 AFB10034 */  sw         $s1, 0x34($sp)
    /* DD4458 000039EC AFB00030 */  sw         $s0, 0x30($sp)
    /* DD445C 000039F0 AFA50054 */  sw         $a1, 0x54($sp)
    /* DD4460 000039F4 0C000000 */  jal        func_00000000
    /* DD4464 000039F8 24040080 */   addiu     $a0, $zero, 0x80
    /* DD4468 000039FC 10400004 */  beqz       $v0, .L00003A10
    /* DD446C 00003A00 00408825 */   or        $s1, $v0, $zero
    /* DD4470 00003A04 00402025 */  or         $a0, $v0, $zero
    /* DD4474 00003A08 0C000000 */  jal        func_00000000
    /* DD4478 00003A0C 24050001 */   addiu     $a1, $zero, 0x1
  .L00003A10:
    /* DD447C 00003A10 AFB1004C */  sw         $s1, 0x4C($sp)
    /* DD4480 00003A14 0C000000 */  jal        func_00000000
    /* DD4484 00003A18 24040080 */   addiu     $a0, $zero, 0x80
    /* DD4488 00003A1C 10400004 */  beqz       $v0, .L00003A30
    /* DD448C 00003A20 00408825 */   or        $s1, $v0, $zero
    /* DD4490 00003A24 00402025 */  or         $a0, $v0, $zero
    /* DD4494 00003A28 0C000000 */  jal        func_00000000
    /* DD4498 00003A2C 00002825 */   or        $a1, $zero, $zero
  .L00003A30:
    /* DD449C 00003A30 3C100000 */  lui        $s0, (0x0 >> 16)
    /* DD44A0 00003A34 26100000 */  addiu      $s0, $s0, 0x0
    /* DD44A4 00003A38 02002025 */  or         $a0, $s0, $zero
    /* DD44A8 00003A3C 0C000000 */  jal        func_00000000
    /* DD44AC 00003A40 8FA5004C */   lw        $a1, 0x4C($sp)
    /* DD44B0 00003A44 02002025 */  or         $a0, $s0, $zero
    /* DD44B4 00003A48 0C000000 */  jal        func_00000000
    /* DD44B8 00003A4C 02202825 */   or        $a1, $s1, $zero
    /* DD44BC 00003A50 02002025 */  or         $a0, $s0, $zero
    /* DD44C0 00003A54 8FA5004C */  lw         $a1, 0x4C($sp)
    /* DD44C4 00003A58 0C000000 */  jal        func_00000000
    /* DD44C8 00003A5C 02203025 */   or        $a2, $s1, $zero
    /* DD44CC 00003A60 8FAE004C */  lw         $t6, 0x4C($sp)
    /* DD44D0 00003A64 8E180050 */  lw         $t8, 0x50($s0)
    /* DD44D4 00003A68 3C010001 */  lui        $at, (0x10000 >> 16)
    /* DD44D8 00003A6C AFAE0010 */  sw         $t6, 0x10($sp)
    /* DD44DC 00003A70 8E4F0080 */  lw         $t7, 0x80($s2)
    /* DD44E0 00003A74 37190001 */  ori        $t9, $t8, 0x1
    /* DD44E4 00003A78 03214025 */  or         $t0, $t9, $at
    /* DD44E8 00003A7C AFAF0014 */  sw         $t7, 0x14($sp)
    /* DD44EC 00003A80 8E430098 */  lw         $v1, 0x98($s2)
    /* DD44F0 00003A84 3C010004 */  lui        $at, (0x40000 >> 16)
    /* DD44F4 00003A88 01014825 */  or         $t1, $t0, $at
    /* DD44F8 00003A8C C46400C4 */  lwc1       $f4, 0xC4($v1)
    /* DD44FC 00003A90 240A001B */  addiu      $t2, $zero, 0x1B
    /* DD4500 00003A94 02402025 */  or         $a0, $s2, $zero
    /* DD4504 00003A98 E7A40018 */  swc1       $f4, 0x18($sp)
    /* DD4508 00003A9C C46600CC */  lwc1       $f6, 0xCC($v1)
    /* DD450C 00003AA0 AFAA0024 */  sw         $t2, 0x24($sp)
    /* DD4510 00003AA4 AFA90020 */  sw         $t1, 0x20($sp)
    /* DD4514 00003AA8 00002825 */  or         $a1, $zero, $zero
    /* DD4518 00003AAC 8E06004C */  lw         $a2, 0x4C($s0)
    /* DD451C 00003AB0 8E070054 */  lw         $a3, 0x54($s0)
    /* DD4520 00003AB4 0C000000 */  jal        func_00000000
    /* DD4524 00003AB8 E7A6001C */   swc1      $f6, 0x1C($sp)
    /* DD4528 00003ABC AFA20044 */  sw         $v0, 0x44($sp)
    /* DD452C 00003AC0 AFB10010 */  sw         $s1, 0x10($sp)
    /* DD4530 00003AC4 8E4B0080 */  lw         $t3, 0x80($s2)
    /* DD4534 00003AC8 920D0182 */  lbu        $t5, 0x182($s0)
    /* DD4538 00003ACC 240F0014 */  addiu      $t7, $zero, 0x14
    /* DD453C 00003AD0 AFAB0014 */  sw         $t3, 0x14($sp)
    /* DD4540 00003AD4 8E430098 */  lw         $v1, 0x98($s2)
    /* DD4544 00003AD8 35AE8008 */  ori        $t6, $t5, 0x8008
    /* DD4548 00003ADC 02402025 */  or         $a0, $s2, $zero
    /* DD454C 00003AE0 C46800C4 */  lwc1       $f8, 0xC4($v1)
    /* DD4550 00003AE4 00002825 */  or         $a1, $zero, $zero
    /* DD4554 00003AE8 92060181 */  lbu        $a2, 0x181($s0)
    /* DD4558 00003AEC E7A80018 */  swc1       $f8, 0x18($sp)
    /* DD455C 00003AF0 C46A00CC */  lwc1       $f10, 0xCC($v1)
    /* DD4560 00003AF4 AFAF0024 */  sw         $t7, 0x24($sp)
    /* DD4564 00003AF8 AFAE0020 */  sw         $t6, 0x20($sp)
    /* DD4568 00003AFC 92070183 */  lbu        $a3, 0x183($s0)
    /* DD456C 00003B00 0C000000 */  jal        func_00000000
    /* DD4570 00003B04 E7AA001C */   swc1      $f10, 0x1C($sp)
    /* DD4574 00003B08 8FB10044 */  lw         $s1, 0x44($sp)
    /* DD4578 00003B0C AC40038C */  sw         $zero, 0x38C($v0)
    /* DD457C 00003B10 8FB8004C */  lw         $t8, 0x4C($sp)
    /* DD4580 00003B14 8E460080 */  lw         $a2, 0x80($s2)
    /* DD4584 00003B18 00408025 */  or         $s0, $v0, $zero
    /* DD4588 00003B1C 02402025 */  or         $a0, $s2, $zero
    /* DD458C 00003B20 24050002 */  addiu      $a1, $zero, 0x2
    /* DD4590 00003B24 02203825 */  or         $a3, $s1, $zero
    /* DD4594 00003B28 0C000000 */  jal        func_00000000
    /* DD4598 00003B2C AFB80010 */   sw        $t8, 0x10($sp)
    /* DD459C 00003B30 8FB90054 */  lw         $t9, 0x54($sp)
    /* DD45A0 00003B34 3C090000 */  lui        $t1, (0x0 >> 16)
    /* DD45A4 00003B38 25290000 */  addiu      $t1, $t1, 0x0
    /* DD45A8 00003B3C 001940C0 */  sll        $t0, $t9, 3
    /* DD45AC 00003B40 01194023 */  subu       $t0, $t0, $t9
    /* DD45B0 00003B44 00084080 */  sll        $t0, $t0, 2
    /* DD45B4 00003B48 01092821 */  addu       $a1, $t0, $t1
    /* DD45B8 00003B4C 02402025 */  or         $a0, $s2, $zero
    /* DD45BC 00003B50 0C000000 */  jal        func_00000000
    /* DD45C0 00003B54 00403025 */   or        $a2, $v0, $zero
    /* DD45C4 00003B58 AE2208DC */  sw         $v0, 0x8DC($s1)
    /* DD45C8 00003B5C AE0208DC */  sw         $v0, 0x8DC($s0)
    /* DD45CC 00003B60 8FBF003C */  lw         $ra, 0x3C($sp)
    /* DD45D0 00003B64 8FB20038 */  lw         $s2, 0x38($sp)
    /* DD45D4 00003B68 8FB10034 */  lw         $s1, 0x34($sp)
    /* DD45D8 00003B6C 8FB00030 */  lw         $s0, 0x30($sp)
    /* DD45DC 00003B70 03E00008 */  jr         $ra
    /* DD45E0 00003B74 27BD0050 */   addiu     $sp, $sp, 0x50
endlabel game_setupVsGhost
