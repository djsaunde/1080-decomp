nonmatching Fader_ctor, 0x24C

glabel Fader_ctor
    /* DE386C 00012E00 27BDFFC0 */  addiu      $sp, $sp, -0x40
    /* DE3870 00012E04 AFB00018 */  sw         $s0, 0x18($sp)
    /* DE3874 00012E08 00808025 */  or         $s0, $a0, $zero
    /* DE3878 00012E0C AFBF001C */  sw         $ra, 0x1C($sp)
    /* DE387C 00012E10 14800005 */  bnez       $a0, .L00012E28
    /* DE3880 00012E14 AFA50044 */   sw        $a1, 0x44($sp)
    /* DE3884 00012E18 0C000000 */  jal        func_00000000
    /* DE3888 00012E1C 24040078 */   addiu     $a0, $zero, 0x78
    /* DE388C 00012E20 10400085 */  beqz       $v0, .L00013038
    /* DE3890 00012E24 00408025 */   or        $s0, $v0, $zero
  .L00012E28:
    /* DE3894 00012E28 16000005 */  bnez       $s0, .L00012E40
    /* DE3898 00012E2C 02002025 */   or        $a0, $s0, $zero
    /* DE389C 00012E30 0C000000 */  jal        func_00000000
    /* DE38A0 00012E34 2404002C */   addiu     $a0, $zero, 0x2C
    /* DE38A4 00012E38 10400009 */  beqz       $v0, .L00012E60
    /* DE38A8 00012E3C 00402025 */   or        $a0, $v0, $zero
  .L00012E40:
    /* DE38AC 00012E40 3C050001 */  lui        $a1, %hi(D_0000C730)
    /* DE38B0 00012E44 24A5C730 */  addiu      $a1, $a1, %lo(D_0000C730)
    /* DE38B4 00012E48 0C000000 */  jal        func_00000000
    /* DE38B8 00012E4C AFA40020 */   sw        $a0, 0x20($sp)
    /* DE38BC 00012E50 8FA40020 */  lw         $a0, 0x20($sp)
    /* DE38C0 00012E54 3C0E0000 */  lui        $t6, (0x0 >> 16)
    /* DE38C4 00012E58 25CE0000 */  addiu      $t6, $t6, 0x0
    /* DE38C8 00012E5C AC8E0028 */  sw         $t6, 0x28($a0)
  .L00012E60:
    /* DE38CC 00012E60 3C0F0000 */  lui        $t7, (0x0 >> 16)
    /* DE38D0 00012E64 3C180001 */  lui        $t8, %hi(D_0000C738)
    /* DE38D4 00012E68 25EF0000 */  addiu      $t7, $t7, 0x0
    /* DE38D8 00012E6C 2718C738 */  addiu      $t8, $t8, %lo(D_0000C738)
    /* DE38DC 00012E70 AE0F0028 */  sw         $t7, 0x28($s0)
    /* DE38E0 00012E74 AE18000C */  sw         $t8, 0xC($s0)
    /* DE38E4 00012E78 8FB90044 */  lw         $t9, 0x44($sp)
    /* DE38E8 00012E7C 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DE38EC 00012E80 44812000 */  mtc1       $at, $f4
    /* DE38F0 00012E84 24090020 */  addiu      $t1, $zero, 0x20
    /* DE38F4 00012E88 240B0400 */  addiu      $t3, $zero, 0x400
    /* DE38F8 00012E8C AE09005C */  sw         $t1, 0x5C($s0)
    /* DE38FC 00012E90 AE090060 */  sw         $t1, 0x60($s0)
    /* DE3900 00012E94 AE0B0064 */  sw         $t3, 0x64($s0)
    /* DE3904 00012E98 00002025 */  or         $a0, $zero, $zero
    /* DE3908 00012E9C AE19002C */  sw         $t9, 0x2C($s0)
    /* DE390C 00012EA0 0C000000 */  jal        func_00000000
    /* DE3910 00012EA4 E6040048 */   swc1      $f4, 0x48($s0)
    /* DE3914 00012EA8 AFA2003C */  sw         $v0, 0x3C($sp)
    /* DE3918 00012EAC 24040800 */  addiu      $a0, $zero, 0x800
    /* DE391C 00012EB0 0C000000 */  jal        func_00000000
    /* DE3920 00012EB4 24050010 */   addiu     $a1, $zero, 0x10
    /* DE3924 00012EB8 AE02004C */  sw         $v0, 0x4C($s0)
    /* DE3928 00012EBC 24040400 */  addiu      $a0, $zero, 0x400
    /* DE392C 00012EC0 0C000000 */  jal        func_00000000
    /* DE3930 00012EC4 24050010 */   addiu     $a1, $zero, 0x10
    /* DE3934 00012EC8 AE020050 */  sw         $v0, 0x50($s0)
    /* DE3938 00012ECC 0C000000 */  jal        func_00000000
    /* DE393C 00012ED0 24041000 */   addiu     $a0, $zero, 0x1000
    /* DE3940 00012ED4 AE020058 */  sw         $v0, 0x58($s0)
    /* DE3944 00012ED8 0C000000 */  jal        func_00000000
    /* DE3948 00012EDC 24041000 */   addiu     $a0, $zero, 0x1000
    /* DE394C 00012EE0 AE020054 */  sw         $v0, 0x54($s0)
    /* DE3950 00012EE4 0C000000 */  jal        func_00000000
    /* DE3954 00012EE8 8FA4003C */   lw        $a0, 0x3C($sp)
    /* DE3958 00012EEC 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DE395C 00012EF0 44811000 */  mtc1       $at, $f2
    /* DE3960 00012EF4 3C0140F0 */  lui        $at, (0x40F00000 >> 16)
    /* DE3964 00012EF8 44813000 */  mtc1       $at, $f6
    /* DE3968 00012EFC 3C014000 */  lui        $at, (0x40000000 >> 16)
    /* DE396C 00012F00 44814000 */  mtc1       $at, $f8
    /* DE3970 00012F04 3C014120 */  lui        $at, (0x41200000 >> 16)
    /* DE3974 00012F08 44815000 */  mtc1       $at, $f10
    /* DE3978 00012F0C 3C014000 */  lui        $at, (0x40000000 >> 16)
    /* DE397C 00012F10 44818000 */  mtc1       $at, $f16
    /* DE3980 00012F14 3C014120 */  lui        $at, (0x41200000 >> 16)
    /* DE3984 00012F18 E602003C */  swc1       $f2, 0x3C($s0)
    /* DE3988 00012F1C E6020038 */  swc1       $f2, 0x38($s0)
    /* DE398C 00012F20 E6020034 */  swc1       $f2, 0x34($s0)
    /* DE3990 00012F24 E6020030 */  swc1       $f2, 0x30($s0)
    /* DE3994 00012F28 44811000 */  mtc1       $at, $f2
    /* DE3998 00012F2C 24090020 */  addiu      $t1, $zero, 0x20
    /* DE399C 00012F30 00005025 */  or         $t2, $zero, $zero
    /* DE39A0 00012F34 00002825 */  or         $a1, $zero, $zero
    /* DE39A4 00012F38 24080140 */  addiu      $t0, $zero, 0x140
    /* DE39A8 00012F3C 46083303 */  div.s      $f12, $f6, $f8
    /* DE39AC 00012F40 46105003 */  div.s      $f0, $f10, $f16
    /* DE39B0 00012F44 448A9000 */  mtc1       $t2, $f18
  .L00012F48:
    /* DE39B4 00012F48 3C0140F0 */  lui        $at, (0x40F00000 >> 16)
    /* DE39B8 00012F4C 44813000 */  mtc1       $at, $f6
    /* DE39BC 00012F50 46809120 */  cvt.s.w    $f4, $f18
    /* DE39C0 00012F54 00003025 */  or         $a2, $zero, $zero
    /* DE39C4 00012F58 254A0001 */  addiu      $t2, $t2, 0x1
    /* DE39C8 00012F5C 46062202 */  mul.s      $f8, $f4, $f6
    /* DE39CC 00012F60 460C4280 */  add.s      $f10, $f8, $f12
    /* DE39D0 00012F64 4600540D */  trunc.w.s  $f16, $f10
    /* DE39D4 00012F68 44078000 */  mfc1       $a3, $f16
    /* DE39D8 00012F6C 00000000 */  nop
  .L00012F70:
    /* DE39DC 00012F70 44869000 */  mtc1       $a2, $f18
    /* DE39E0 00012F74 24CB0001 */  addiu      $t3, $a2, 0x1
    /* DE39E4 00012F78 448B8000 */  mtc1       $t3, $f16
    /* DE39E8 00012F7C 46809120 */  cvt.s.w    $f4, $f18
    /* DE39EC 00012F80 00E02025 */  or         $a0, $a3, $zero
    /* DE39F0 00012F84 468084A0 */  cvt.s.w    $f18, $f16
    /* DE39F4 00012F88 46022182 */  mul.s      $f6, $f4, $f2
    /* DE39F8 00012F8C 46003200 */  add.s      $f8, $f6, $f0
    /* DE39FC 00012F90 4600428D */  trunc.w.s  $f10, $f8
    /* DE3A00 00012F94 44035000 */  mfc1       $v1, $f10
    /* DE3A04 00012F98 00000000 */  nop
    /* DE3A08 00012F9C 28610140 */  slti       $at, $v1, 0x140
    /* DE3A0C 00012FA0 54200003 */  bnel       $at, $zero, .L00012FB0
    /* DE3A10 00012FA4 28E100F0 */   slti      $at, $a3, 0xF0
    /* DE3A14 00012FA8 2403013F */  addiu      $v1, $zero, 0x13F
    /* DE3A18 00012FAC 28E100F0 */  slti       $at, $a3, 0xF0
  .L00012FB0:
    /* DE3A1C 00012FB0 46029102 */  mul.s      $f4, $f18, $f2
    /* DE3A20 00012FB4 14200002 */  bnez       $at, .L00012FC0
    /* DE3A24 00012FB8 00000000 */   nop
    /* DE3A28 00012FBC 240400EF */  addiu      $a0, $zero, 0xEF
  .L00012FC0:
    /* DE3A2C 00012FC0 00880019 */  multu      $a0, $t0
    /* DE3A30 00012FC4 46002180 */  add.s      $f6, $f4, $f0
    /* DE3A34 00012FC8 8E180054 */  lw         $t8, 0x54($s0)
    /* DE3A38 00012FCC 00E02025 */  or         $a0, $a3, $zero
    /* DE3A3C 00012FD0 4600320D */  trunc.w.s  $f8, $f6
    /* DE3A40 00012FD4 0305C821 */  addu       $t9, $t8, $a1
    /* DE3A44 00012FD8 24A50004 */  addiu      $a1, $a1, 0x4
    /* DE3A48 00012FDC 00007012 */  mflo       $t6
    /* DE3A4C 00012FE0 01C37821 */  addu       $t7, $t6, $v1
    /* DE3A50 00012FE4 44034000 */  mfc1       $v1, $f8
    /* DE3A54 00012FE8 AF2F0000 */  sw         $t7, 0x0($t9)
    /* DE3A58 00012FEC 28610140 */  slti       $at, $v1, 0x140
    /* DE3A5C 00012FF0 54200003 */  bnel       $at, $zero, .L00013000
    /* DE3A60 00012FF4 28E100F0 */   slti      $at, $a3, 0xF0
    /* DE3A64 00012FF8 2403013F */  addiu      $v1, $zero, 0x13F
    /* DE3A68 00012FFC 28E100F0 */  slti       $at, $a3, 0xF0
  .L00013000:
    /* DE3A6C 00013000 14200002 */  bnez       $at, .L0001300C
    /* DE3A70 00013004 00000000 */   nop
    /* DE3A74 00013008 240400EF */  addiu      $a0, $zero, 0xEF
  .L0001300C:
    /* DE3A78 0001300C 00880019 */  multu      $a0, $t0
    /* DE3A7C 00013010 8E180054 */  lw         $t8, 0x54($s0)
    /* DE3A80 00013014 24C60002 */  addiu      $a2, $a2, 0x2
    /* DE3A84 00013018 03057821 */  addu       $t7, $t8, $a1
    /* DE3A88 0001301C 24A50004 */  addiu      $a1, $a1, 0x4
    /* DE3A8C 00013020 00006812 */  mflo       $t5
    /* DE3A90 00013024 01A37021 */  addu       $t6, $t5, $v1
    /* DE3A94 00013028 14C9FFD1 */  bne        $a2, $t1, .L00012F70
    /* DE3A98 0001302C ADEE0000 */   sw        $t6, 0x0($t7)
    /* DE3A9C 00013030 5549FFC5 */  bnel       $t2, $t1, .L00012F48
    /* DE3AA0 00013034 448A9000 */   mtc1      $t2, $f18
  .L00013038:
    /* DE3AA4 00013038 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DE3AA8 0001303C 02001025 */  or         $v0, $s0, $zero
    /* DE3AAC 00013040 8FB00018 */  lw         $s0, 0x18($sp)
    /* DE3AB0 00013044 03E00008 */  jr         $ra
    /* DE3AB4 00013048 27BD0040 */   addiu     $sp, $sp, 0x40
endlabel Fader_ctor
