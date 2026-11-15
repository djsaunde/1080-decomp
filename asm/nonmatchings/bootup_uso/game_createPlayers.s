nonmatching game_createPlayers, 0xFC

glabel game_createPlayers
    /* DD40A4 00003638 27BDFFC8 */  addiu      $sp, $sp, -0x38
    /* DD40A8 0000363C AFA40038 */  sw         $a0, 0x38($sp)
    /* DD40AC 00003640 AFBF002C */  sw         $ra, 0x2C($sp)
    /* DD40B0 00003644 3C040000 */  lui        $a0, %hi(sStr_Players_74DC)
    /* DD40B4 00003648 AFA5003C */  sw         $a1, 0x3C($sp)
    /* DD40B8 0000364C 248474DC */  addiu      $a0, $a0, %lo(sStr_Players_74DC)
    /* DD40BC 00003650 AFA60040 */  sw         $a2, 0x40($sp)
    /* DD40C0 00003654 0C000000 */  jal        func_00000000
    /* DD40C4 00003658 AFA70044 */   sw        $a3, 0x44($sp)
    /* DD40C8 0000365C 8FA20058 */  lw         $v0, 0x58($sp)
    /* DD40CC 00003660 8FA60040 */  lw         $a2, 0x40($sp)
    /* DD40D0 00003664 8FA70044 */  lw         $a3, 0x44($sp)
    /* DD40D4 00003668 8FAE0048 */  lw         $t6, 0x48($sp)
    /* DD40D8 0000366C C7A40050 */  lwc1       $f4, 0x50($sp)
    /* DD40DC 00003670 C7A60054 */  lwc1       $f6, 0x54($sp)
    /* DD40E0 00003674 8FAF005C */  lw         $t7, 0x5C($sp)
    /* DD40E4 00003678 34420002 */  ori        $v0, $v0, 0x2
    /* DD40E8 0000367C AFA2001C */  sw         $v0, 0x1C($sp)
    /* DD40EC 00003680 AFA20058 */  sw         $v0, 0x58($sp)
    /* DD40F0 00003684 00002025 */  or         $a0, $zero, $zero
    /* DD40F4 00003688 8FA5003C */  lw         $a1, 0x3C($sp)
    /* DD40F8 0000368C 24C60001 */  addiu      $a2, $a2, 0x1
    /* DD40FC 00003690 24E70001 */  addiu      $a3, $a3, 0x1
    /* DD4100 00003694 AFAE0010 */  sw         $t6, 0x10($sp)
    /* DD4104 00003698 E7A40014 */  swc1       $f4, 0x14($sp)
    /* DD4108 0000369C E7A60018 */  swc1       $f6, 0x18($sp)
    /* DD410C 000036A0 0C000000 */  jal        func_00000000
    /* DD4110 000036A4 AFAF0020 */   sw        $t7, 0x20($sp)
    /* DD4114 000036A8 8FA4004C */  lw         $a0, 0x4C($sp)
    /* DD4118 000036AC AFA20034 */  sw         $v0, 0x34($sp)
    /* DD411C 000036B0 00402825 */  or         $a1, $v0, $zero
    /* DD4120 000036B4 AFA20030 */  sw         $v0, 0x30($sp)
    /* DD4124 000036B8 0C000000 */  jal        func_00000000
    /* DD4128 000036BC 24840010 */   addiu     $a0, $a0, 0x10
    /* DD412C 000036C0 8FA30030 */  lw         $v1, 0x30($sp)
    /* DD4130 000036C4 24190001 */  addiu      $t9, $zero, 0x1
    /* DD4134 000036C8 24060001 */  addiu      $a2, $zero, 0x1
    /* DD4138 000036CC 8C780014 */  lw         $t8, 0x14($v1)
    /* DD413C 000036D0 53000003 */  beql       $t8, $zero, .L000036E0
    /* DD4140 000036D4 8FA8004C */   lw        $t0, 0x4C($sp)
    /* DD4144 000036D8 AC790004 */  sw         $t9, 0x4($v1)
    /* DD4148 000036DC 8FA8004C */  lw         $t0, 0x4C($sp)
  .L000036E0:
    /* DD414C 000036E0 AC680014 */  sw         $t0, 0x14($v1)
    /* DD4150 000036E4 8FA90058 */  lw         $t1, 0x58($sp)
    /* DD4154 000036E8 8FA50034 */  lw         $a1, 0x34($sp)
    /* DD4158 000036EC 8FA40038 */  lw         $a0, 0x38($sp)
    /* DD415C 000036F0 312A000C */  andi       $t2, $t1, 0xC
    /* DD4160 000036F4 11400006 */  beqz       $t2, .L00003710
    /* DD4164 000036F8 00000000 */   nop
    /* DD4168 000036FC 8FA40038 */  lw         $a0, 0x38($sp)
    /* DD416C 00003700 0C000000 */  jal        func_00000000
    /* DD4170 00003704 00003025 */   or        $a2, $zero, $zero
    /* DD4174 00003708 10000003 */  b          .L00003718
    /* DD4178 0000370C 00000000 */   nop
  .L00003710:
    /* DD417C 00003710 0C000000 */  jal        func_00000000
    /* DD4180 00003714 8FA50034 */   lw        $a1, 0x34($sp)
  .L00003718:
    /* DD4184 00003718 0C000000 */  jal        func_00000000
    /* DD4188 0000371C 00000000 */   nop
    /* DD418C 00003720 8FBF002C */  lw         $ra, 0x2C($sp)
    /* DD4190 00003724 8FA20034 */  lw         $v0, 0x34($sp)
    /* DD4194 00003728 27BD0038 */  addiu      $sp, $sp, 0x38
    /* DD4198 0000372C 03E00008 */  jr         $ra
    /* DD419C 00003730 00000000 */   nop
endlabel game_createPlayers
