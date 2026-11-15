nonmatching game_setupVsCpu, 0x1C4

glabel game_setupVsCpu
    /* DD47A8 00003D3C 27BDFFA8 */  addiu      $sp, $sp, -0x58
    /* DD47AC 00003D40 AFB10034 */  sw         $s1, 0x34($sp)
    /* DD47B0 00003D44 00808825 */  or         $s1, $a0, $zero
    /* DD47B4 00003D48 AFBF003C */  sw         $ra, 0x3C($sp)
    /* DD47B8 00003D4C AFB20038 */  sw         $s2, 0x38($sp)
    /* DD47BC 00003D50 AFB00030 */  sw         $s0, 0x30($sp)
    /* DD47C0 00003D54 AFA5005C */  sw         $a1, 0x5C($sp)
    /* DD47C4 00003D58 0C000000 */  jal        func_00000000
    /* DD47C8 00003D5C 24040080 */   addiu     $a0, $zero, 0x80
    /* DD47CC 00003D60 10400004 */  beqz       $v0, .L00003D74
    /* DD47D0 00003D64 00409025 */   or        $s2, $v0, $zero
    /* DD47D4 00003D68 00402025 */  or         $a0, $v0, $zero
    /* DD47D8 00003D6C 0C000000 */  jal        func_00000000
    /* DD47DC 00003D70 24050001 */   addiu     $a1, $zero, 0x1
  .L00003D74:
    /* DD47E0 00003D74 AFB2004C */  sw         $s2, 0x4C($sp)
    /* DD47E4 00003D78 0C000000 */  jal        func_00000000
    /* DD47E8 00003D7C 24040080 */   addiu     $a0, $zero, 0x80
    /* DD47EC 00003D80 10400004 */  beqz       $v0, .L00003D94
    /* DD47F0 00003D84 00409025 */   or        $s2, $v0, $zero
    /* DD47F4 00003D88 00402025 */  or         $a0, $v0, $zero
    /* DD47F8 00003D8C 0C000000 */  jal        func_00000000
    /* DD47FC 00003D90 00002825 */   or        $a1, $zero, $zero
  .L00003D94:
    /* DD4800 00003D94 3C100000 */  lui        $s0, (0x0 >> 16)
    /* DD4804 00003D98 26100000 */  addiu      $s0, $s0, 0x0
    /* DD4808 00003D9C 02002025 */  or         $a0, $s0, $zero
    /* DD480C 00003DA0 0C000000 */  jal        func_00000000
    /* DD4810 00003DA4 8FA5004C */   lw        $a1, 0x4C($sp)
    /* DD4814 00003DA8 02002025 */  or         $a0, $s0, $zero
    /* DD4818 00003DAC 0C000000 */  jal        func_00000000
    /* DD481C 00003DB0 02402825 */   or        $a1, $s2, $zero
    /* DD4820 00003DB4 02002025 */  or         $a0, $s0, $zero
    /* DD4824 00003DB8 8FA5004C */  lw         $a1, 0x4C($sp)
    /* DD4828 00003DBC 0C000000 */  jal        func_00000000
    /* DD482C 00003DC0 00003025 */   or        $a2, $zero, $zero
    /* DD4830 00003DC4 8FAE004C */  lw         $t6, 0x4C($sp)
    /* DD4834 00003DC8 3C0143FA */  lui        $at, (0x43FA0000 >> 16)
    /* DD4838 00003DCC 44813000 */  mtc1       $at, $f6
    /* DD483C 00003DD0 AFAE0010 */  sw         $t6, 0x10($sp)
    /* DD4840 00003DD4 8E2F0080 */  lw         $t7, 0x80($s1)
    /* DD4844 00003DD8 8E180050 */  lw         $t8, 0x50($s0)
    /* DD4848 00003DDC 3C010001 */  lui        $at, (0x10000 >> 16)
    /* DD484C 00003DE0 AFAF0014 */  sw         $t7, 0x14($sp)
    /* DD4850 00003DE4 8E230098 */  lw         $v1, 0x98($s1)
    /* DD4854 00003DE8 37190001 */  ori        $t9, $t8, 0x1
    /* DD4858 00003DEC 03214025 */  or         $t0, $t9, $at
    /* DD485C 00003DF0 C46400C4 */  lwc1       $f4, 0xC4($v1)
    /* DD4860 00003DF4 3C010004 */  lui        $at, (0x40000 >> 16)
    /* DD4864 00003DF8 01014825 */  or         $t1, $t0, $at
    /* DD4868 00003DFC 46062201 */  sub.s      $f8, $f4, $f6
    /* DD486C 00003E00 240A001B */  addiu      $t2, $zero, 0x1B
    /* DD4870 00003E04 02202025 */  or         $a0, $s1, $zero
    /* DD4874 00003E08 00002825 */  or         $a1, $zero, $zero
    /* DD4878 00003E0C E7A80018 */  swc1       $f8, 0x18($sp)
    /* DD487C 00003E10 C46A00CC */  lwc1       $f10, 0xCC($v1)
    /* DD4880 00003E14 AFAA0024 */  sw         $t2, 0x24($sp)
    /* DD4884 00003E18 AFA90020 */  sw         $t1, 0x20($sp)
    /* DD4888 00003E1C 8E06004C */  lw         $a2, 0x4C($s0)
    /* DD488C 00003E20 8E070054 */  lw         $a3, 0x54($s0)
    /* DD4890 00003E24 0C000000 */  jal        func_00000000
    /* DD4894 00003E28 E7AA001C */   swc1      $f10, 0x1C($sp)
    /* DD4898 00003E2C AFA20040 */  sw         $v0, 0x40($sp)
    /* DD489C 00003E30 AFB20010 */  sw         $s2, 0x10($sp)
    /* DD48A0 00003E34 8E2B0080 */  lw         $t3, 0x80($s1)
    /* DD48A4 00003E38 3C0143FA */  lui        $at, (0x43FA0000 >> 16)
    /* DD48A8 00003E3C 44819000 */  mtc1       $at, $f18
    /* DD48AC 00003E40 AFAB0014 */  sw         $t3, 0x14($sp)
    /* DD48B0 00003E44 8E230098 */  lw         $v1, 0x98($s1)
    /* DD48B4 00003E48 340C8004 */  ori        $t4, $zero, 0x8004
    /* DD48B8 00003E4C 240D001D */  addiu      $t5, $zero, 0x1D
    /* DD48BC 00003E50 C47000C4 */  lwc1       $f16, 0xC4($v1)
    /* DD48C0 00003E54 02202025 */  or         $a0, $s1, $zero
    /* DD48C4 00003E58 24050002 */  addiu      $a1, $zero, 0x2
    /* DD48C8 00003E5C 46128100 */  add.s      $f4, $f16, $f18
    /* DD48CC 00003E60 9206017D */  lbu        $a2, 0x17D($s0)
    /* DD48D0 00003E64 9207017F */  lbu        $a3, 0x17F($s0)
    /* DD48D4 00003E68 E7A40018 */  swc1       $f4, 0x18($sp)
    /* DD48D8 00003E6C C46600CC */  lwc1       $f6, 0xCC($v1)
    /* DD48DC 00003E70 AFAD0024 */  sw         $t5, 0x24($sp)
    /* DD48E0 00003E74 AFAC0020 */  sw         $t4, 0x20($sp)
    /* DD48E4 00003E78 0C000000 */  jal        func_00000000
    /* DD48E8 00003E7C E7A6001C */   swc1      $f6, 0x1C($sp)
    /* DD48EC 00003E80 8FB00040 */  lw         $s0, 0x40($sp)
    /* DD48F0 00003E84 3C0E0098 */  lui        $t6, (0x989680 >> 16)
    /* DD48F4 00003E88 35CE9680 */  ori        $t6, $t6, (0x989680 & 0xFFFF)
    /* DD48F8 00003E8C AC4E0A14 */  sw         $t6, 0xA14($v0)
    /* DD48FC 00003E90 AE020908 */  sw         $v0, 0x908($s0)
    /* DD4900 00003E94 AC500908 */  sw         $s0, 0x908($v0)
    /* DD4904 00003E98 8FAF004C */  lw         $t7, 0x4C($sp)
    /* DD4908 00003E9C 8E260080 */  lw         $a2, 0x80($s1)
    /* DD490C 00003EA0 00409025 */  or         $s2, $v0, $zero
    /* DD4910 00003EA4 02202025 */  or         $a0, $s1, $zero
    /* DD4914 00003EA8 24050002 */  addiu      $a1, $zero, 0x2
    /* DD4918 00003EAC 02003825 */  or         $a3, $s0, $zero
    /* DD491C 00003EB0 0C000000 */  jal        func_00000000
    /* DD4920 00003EB4 AFAF0010 */   sw        $t7, 0x10($sp)
    /* DD4924 00003EB8 8FB8005C */  lw         $t8, 0x5C($sp)
    /* DD4928 00003EBC 3C080000 */  lui        $t0, (0x0 >> 16)
    /* DD492C 00003EC0 25080000 */  addiu      $t0, $t0, 0x0
    /* DD4930 00003EC4 0018C8C0 */  sll        $t9, $t8, 3
    /* DD4934 00003EC8 0338C823 */  subu       $t9, $t9, $t8
    /* DD4938 00003ECC 0019C880 */  sll        $t9, $t9, 2
    /* DD493C 00003ED0 03282821 */  addu       $a1, $t9, $t0
    /* DD4940 00003ED4 02202025 */  or         $a0, $s1, $zero
    /* DD4944 00003ED8 0C000000 */  jal        func_00000000
    /* DD4948 00003EDC 00403025 */   or        $a2, $v0, $zero
    /* DD494C 00003EE0 AE0208DC */  sw         $v0, 0x8DC($s0)
    /* DD4950 00003EE4 AE4208DC */  sw         $v0, 0x8DC($s2)
    /* DD4954 00003EE8 8FBF003C */  lw         $ra, 0x3C($sp)
    /* DD4958 00003EEC 8FB20038 */  lw         $s2, 0x38($sp)
    /* DD495C 00003EF0 8FB10034 */  lw         $s1, 0x34($sp)
    /* DD4960 00003EF4 8FB00030 */  lw         $s0, 0x30($sp)
    /* DD4964 00003EF8 03E00008 */  jr         $ra
    /* DD4968 00003EFC 27BD0058 */   addiu     $sp, $sp, 0x58
endlabel game_setupVsCpu
