nonmatching TimeGadget_ctor_00013D40, 0x2A0

glabel TimeGadget_ctor_00013D40
    /* DE47AC 00013D40 27BDFFB0 */  addiu      $sp, $sp, -0x50
    /* DE47B0 00013D44 AFB00018 */  sw         $s0, 0x18($sp)
    /* DE47B4 00013D48 00808025 */  or         $s0, $a0, $zero
    /* DE47B8 00013D4C 14800005 */  bnez       $a0, .L00013D64
    /* DE47BC 00013D50 AFBF001C */   sw        $ra, 0x1C($sp)
    /* DE47C0 00013D54 0C000000 */  jal        func_00000000
    /* DE47C4 00013D58 24040098 */   addiu     $a0, $zero, 0x98
    /* DE47C8 00013D5C 1040009B */  beqz       $v0, .L00013FCC
    /* DE47CC 00013D60 00408025 */   or        $s0, $v0, $zero
  .L00013D64:
    /* DE47D0 00013D64 16000005 */  bnez       $s0, .L00013D7C
    /* DE47D4 00013D68 02001825 */   or        $v1, $s0, $zero
    /* DE47D8 00013D6C 0C000000 */  jal        func_00000000
    /* DE47DC 00013D70 24040008 */   addiu     $a0, $zero, 0x8
    /* DE47E0 00013D74 10400005 */  beqz       $v0, .L00013D8C
    /* DE47E4 00013D78 00401825 */   or        $v1, $v0, $zero
  .L00013D7C:
    /* DE47E8 00013D7C 3C0E0001 */  lui        $t6, %hi(sStr_TimeGadget_Parms_CAAC)
    /* DE47EC 00013D80 25CECAAC */  addiu      $t6, $t6, %lo(sStr_TimeGadget_Parms_CAAC)
    /* DE47F0 00013D84 AC6E0000 */  sw         $t6, 0x0($v1)
    /* DE47F4 00013D88 AC600004 */  sw         $zero, 0x4($v1)
  .L00013D8C:
    /* DE47F8 00013D8C 3C0F0001 */  lui        $t7, %hi(sTag_xps_CAC0)
    /* DE47FC 00013D90 8DEFCAC0 */  lw         $t7, %lo(sTag_xps_CAC0)($t7)
    /* DE4800 00013D94 2401FFF8 */  addiu      $at, $zero, -0x8
    /* DE4804 00013D98 26040008 */  addiu      $a0, $s0, 0x8
    /* DE4808 00013D9C AFAF004C */  sw         $t7, 0x4C($sp)
    /* DE480C 00013DA0 16010005 */  bne        $s0, $at, .L00013DB8
    /* DE4810 00013DA4 AFAF0024 */   sw        $t7, 0x24($sp)
    /* DE4814 00013DA8 0C000000 */  jal        func_00000000
    /* DE4818 00013DAC 24040018 */   addiu     $a0, $zero, 0x18
    /* DE481C 00013DB0 1040000E */  beqz       $v0, .L00013DEC
    /* DE4820 00013DB4 00402025 */   or        $a0, $v0, $zero
  .L00013DB8:
    /* DE4824 00013DB8 8FA60024 */  lw         $a2, 0x24($sp)
    /* DE4828 00013DBC 02002825 */  or         $a1, $s0, $zero
    /* DE482C 00013DC0 24070001 */  addiu      $a3, $zero, 0x1
    /* DE4830 00013DC4 AFA40028 */  sw         $a0, 0x28($sp)
    /* DE4834 00013DC8 0C000000 */  jal        func_00000000
    /* DE4838 00013DCC AFA60008 */   sw        $a2, 0x8($sp)
    /* DE483C 00013DD0 8FA40028 */  lw         $a0, 0x28($sp)
    /* DE4840 00013DD4 3C080001 */  lui        $t0, %hi(D_0000C764)
    /* DE4844 00013DD8 2508C764 */  addiu      $t0, $t0, %lo(D_0000C764)
    /* DE4848 00013DDC 2409001E */  addiu      $t1, $zero, 0x1E
    /* DE484C 00013DE0 AC890010 */  sw         $t1, 0x10($a0)
    /* DE4850 00013DE4 AC88000C */  sw         $t0, 0xC($a0)
    /* DE4854 00013DE8 AC800014 */  sw         $zero, 0x14($a0)
  .L00013DEC:
    /* DE4858 00013DEC 3C0A0001 */  lui        $t2, %hi(sTag_yps_CAC4)
    /* DE485C 00013DF0 8D4ACAC4 */  lw         $t2, %lo(sTag_yps_CAC4)($t2)
    /* DE4860 00013DF4 2401FFE0 */  addiu      $at, $zero, -0x20
    /* DE4864 00013DF8 26040020 */  addiu      $a0, $s0, 0x20
    /* DE4868 00013DFC AFAA0048 */  sw         $t2, 0x48($sp)
    /* DE486C 00013E00 16010005 */  bne        $s0, $at, .L00013E18
    /* DE4870 00013E04 AFAA0024 */   sw        $t2, 0x24($sp)
    /* DE4874 00013E08 0C000000 */  jal        func_00000000
    /* DE4878 00013E0C 24040018 */   addiu     $a0, $zero, 0x18
    /* DE487C 00013E10 1040000E */  beqz       $v0, .L00013E4C
    /* DE4880 00013E14 00402025 */   or        $a0, $v0, $zero
  .L00013E18:
    /* DE4884 00013E18 8FA60024 */  lw         $a2, 0x24($sp)
    /* DE4888 00013E1C 02002825 */  or         $a1, $s0, $zero
    /* DE488C 00013E20 24070001 */  addiu      $a3, $zero, 0x1
    /* DE4890 00013E24 AFA40028 */  sw         $a0, 0x28($sp)
    /* DE4894 00013E28 0C000000 */  jal        func_00000000
    /* DE4898 00013E2C AFA60008 */   sw        $a2, 0x8($sp)
    /* DE489C 00013E30 8FA40028 */  lw         $a0, 0x28($sp)
    /* DE48A0 00013E34 3C0D0001 */  lui        $t5, %hi(D_0000C764)
    /* DE48A4 00013E38 25ADC764 */  addiu      $t5, $t5, %lo(D_0000C764)
    /* DE48A8 00013E3C 240E001E */  addiu      $t6, $zero, 0x1E
    /* DE48AC 00013E40 AC8E0010 */  sw         $t6, 0x10($a0)
    /* DE48B0 00013E44 AC8D000C */  sw         $t5, 0xC($a0)
    /* DE48B4 00013E48 AC800014 */  sw         $zero, 0x14($a0)
  .L00013E4C:
    /* DE48B8 00013E4C 3C0F0001 */  lui        $t7, %hi(sTag_lbx_CAC8)
    /* DE48BC 00013E50 8DEFCAC8 */  lw         $t7, %lo(sTag_lbx_CAC8)($t7)
    /* DE48C0 00013E54 2401FFC8 */  addiu      $at, $zero, -0x38
    /* DE48C4 00013E58 26040038 */  addiu      $a0, $s0, 0x38
    /* DE48C8 00013E5C AFAF0044 */  sw         $t7, 0x44($sp)
    /* DE48CC 00013E60 16010005 */  bne        $s0, $at, .L00013E78
    /* DE48D0 00013E64 AFAF0024 */   sw        $t7, 0x24($sp)
    /* DE48D4 00013E68 0C000000 */  jal        func_00000000
    /* DE48D8 00013E6C 24040018 */   addiu     $a0, $zero, 0x18
    /* DE48DC 00013E70 1040000E */  beqz       $v0, .L00013EAC
    /* DE48E0 00013E74 00402025 */   or        $a0, $v0, $zero
  .L00013E78:
    /* DE48E4 00013E78 8FA60024 */  lw         $a2, 0x24($sp)
    /* DE48E8 00013E7C 02002825 */  or         $a1, $s0, $zero
    /* DE48EC 00013E80 24070001 */  addiu      $a3, $zero, 0x1
    /* DE48F0 00013E84 AFA40028 */  sw         $a0, 0x28($sp)
    /* DE48F4 00013E88 0C000000 */  jal        func_00000000
    /* DE48F8 00013E8C AFA60008 */   sw        $a2, 0x8($sp)
    /* DE48FC 00013E90 8FA40028 */  lw         $a0, 0x28($sp)
    /* DE4900 00013E94 3C080001 */  lui        $t0, %hi(D_0000C764)
    /* DE4904 00013E98 2508C764 */  addiu      $t0, $t0, %lo(D_0000C764)
    /* DE4908 00013E9C 24090002 */  addiu      $t1, $zero, 0x2
    /* DE490C 00013EA0 AC890010 */  sw         $t1, 0x10($a0)
    /* DE4910 00013EA4 AC88000C */  sw         $t0, 0xC($a0)
    /* DE4914 00013EA8 AC800014 */  sw         $zero, 0x14($a0)
  .L00013EAC:
    /* DE4918 00013EAC 3C0A0001 */  lui        $t2, %hi(sTimeGadgetParmsTags_CACC)
    /* DE491C 00013EB0 8D4ACACC */  lw         $t2, %lo(sTimeGadgetParmsTags_CACC)($t2)
    /* DE4920 00013EB4 2401FFB0 */  addiu      $at, $zero, -0x50
    /* DE4924 00013EB8 26040050 */  addiu      $a0, $s0, 0x50
    /* DE4928 00013EBC AFAA0040 */  sw         $t2, 0x40($sp)
    /* DE492C 00013EC0 16010005 */  bne        $s0, $at, .L00013ED8
    /* DE4930 00013EC4 AFAA0024 */   sw        $t2, 0x24($sp)
    /* DE4934 00013EC8 0C000000 */  jal        func_00000000
    /* DE4938 00013ECC 24040018 */   addiu     $a0, $zero, 0x18
    /* DE493C 00013ED0 1040000E */  beqz       $v0, .L00013F0C
    /* DE4940 00013ED4 00402025 */   or        $a0, $v0, $zero
  .L00013ED8:
    /* DE4944 00013ED8 8FA60024 */  lw         $a2, 0x24($sp)
    /* DE4948 00013EDC 02002825 */  or         $a1, $s0, $zero
    /* DE494C 00013EE0 24070001 */  addiu      $a3, $zero, 0x1
    /* DE4950 00013EE4 AFA40028 */  sw         $a0, 0x28($sp)
    /* DE4954 00013EE8 0C000000 */  jal        func_00000000
    /* DE4958 00013EEC AFA60008 */   sw        $a2, 0x8($sp)
    /* DE495C 00013EF0 8FA40028 */  lw         $a0, 0x28($sp)
    /* DE4960 00013EF4 3C0D0001 */  lui        $t5, %hi(D_0000C764)
    /* DE4964 00013EF8 25ADC764 */  addiu      $t5, $t5, %lo(D_0000C764)
    /* DE4968 00013EFC 240EFFF4 */  addiu      $t6, $zero, -0xC
    /* DE496C 00013F00 AC8E0010 */  sw         $t6, 0x10($a0)
    /* DE4970 00013F04 AC8D000C */  sw         $t5, 0xC($a0)
    /* DE4974 00013F08 AC800014 */  sw         $zero, 0x14($a0)
  .L00013F0C:
    /* DE4978 00013F0C 3C0F0001 */  lui        $t7, %hi(sTimeGadgetParmsTags_CACC + 0x4)
    /* DE497C 00013F10 8DEFCAD0 */  lw         $t7, %lo(sTimeGadgetParmsTags_CACC + 0x4)($t7)
    /* DE4980 00013F14 2401FF98 */  addiu      $at, $zero, -0x68
    /* DE4984 00013F18 26040068 */  addiu      $a0, $s0, 0x68
    /* DE4988 00013F1C AFAF003C */  sw         $t7, 0x3C($sp)
    /* DE498C 00013F20 16010005 */  bne        $s0, $at, .L00013F38
    /* DE4990 00013F24 AFAF0024 */   sw        $t7, 0x24($sp)
    /* DE4994 00013F28 0C000000 */  jal        func_00000000
    /* DE4998 00013F2C 24040018 */   addiu     $a0, $zero, 0x18
    /* DE499C 00013F30 1040000E */  beqz       $v0, .L00013F6C
    /* DE49A0 00013F34 00402025 */   or        $a0, $v0, $zero
  .L00013F38:
    /* DE49A4 00013F38 8FA60024 */  lw         $a2, 0x24($sp)
    /* DE49A8 00013F3C 02002825 */  or         $a1, $s0, $zero
    /* DE49AC 00013F40 24070001 */  addiu      $a3, $zero, 0x1
    /* DE49B0 00013F44 AFA40028 */  sw         $a0, 0x28($sp)
    /* DE49B4 00013F48 0C000000 */  jal        func_00000000
    /* DE49B8 00013F4C AFA60008 */   sw        $a2, 0x8($sp)
    /* DE49BC 00013F50 8FA40028 */  lw         $a0, 0x28($sp)
    /* DE49C0 00013F54 3C080001 */  lui        $t0, %hi(D_0000C764)
    /* DE49C4 00013F58 2508C764 */  addiu      $t0, $t0, %lo(D_0000C764)
    /* DE49C8 00013F5C 24090002 */  addiu      $t1, $zero, 0x2
    /* DE49CC 00013F60 AC890010 */  sw         $t1, 0x10($a0)
    /* DE49D0 00013F64 AC88000C */  sw         $t0, 0xC($a0)
    /* DE49D4 00013F68 AC800014 */  sw         $zero, 0x14($a0)
  .L00013F6C:
    /* DE49D8 00013F6C 3C0A0001 */  lui        $t2, %hi(sTimeGadgetParmsTags_CACC + 0x8)
    /* DE49DC 00013F70 8D4ACAD4 */  lw         $t2, %lo(sTimeGadgetParmsTags_CACC + 0x8)($t2)
    /* DE49E0 00013F74 2401FF80 */  addiu      $at, $zero, -0x80
    /* DE49E4 00013F78 26040080 */  addiu      $a0, $s0, 0x80
    /* DE49E8 00013F7C AFAA0038 */  sw         $t2, 0x38($sp)
    /* DE49EC 00013F80 16010005 */  bne        $s0, $at, .L00013F98
    /* DE49F0 00013F84 AFAA0024 */   sw        $t2, 0x24($sp)
    /* DE49F4 00013F88 0C000000 */  jal        func_00000000
    /* DE49F8 00013F8C 24040018 */   addiu     $a0, $zero, 0x18
    /* DE49FC 00013F90 1040000E */  beqz       $v0, .L00013FCC
    /* DE4A00 00013F94 00402025 */   or        $a0, $v0, $zero
  .L00013F98:
    /* DE4A04 00013F98 8FA60024 */  lw         $a2, 0x24($sp)
    /* DE4A08 00013F9C 02002825 */  or         $a1, $s0, $zero
    /* DE4A0C 00013FA0 24070001 */  addiu      $a3, $zero, 0x1
    /* DE4A10 00013FA4 AFA40028 */  sw         $a0, 0x28($sp)
    /* DE4A14 00013FA8 0C000000 */  jal        func_00000000
    /* DE4A18 00013FAC AFA60008 */   sw        $a2, 0x8($sp)
    /* DE4A1C 00013FB0 8FA40028 */  lw         $a0, 0x28($sp)
    /* DE4A20 00013FB4 3C0D0001 */  lui        $t5, %hi(D_0000C764)
    /* DE4A24 00013FB8 25ADC764 */  addiu      $t5, $t5, %lo(D_0000C764)
    /* DE4A28 00013FBC 240EFFF4 */  addiu      $t6, $zero, -0xC
    /* DE4A2C 00013FC0 AC8E0010 */  sw         $t6, 0x10($a0)
    /* DE4A30 00013FC4 AC8D000C */  sw         $t5, 0xC($a0)
    /* DE4A34 00013FC8 AC800014 */  sw         $zero, 0x14($a0)
  .L00013FCC:
    /* DE4A38 00013FCC 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DE4A3C 00013FD0 02001025 */  or         $v0, $s0, $zero
    /* DE4A40 00013FD4 8FB00018 */  lw         $s0, 0x18($sp)
    /* DE4A44 00013FD8 03E00008 */  jr         $ra
    /* DE4A48 00013FDC 27BD0050 */   addiu     $sp, $sp, 0x50
endlabel TimeGadget_ctor_00013D40
