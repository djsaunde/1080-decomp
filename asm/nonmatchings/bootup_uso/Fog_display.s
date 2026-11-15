nonmatching Fog_display, 0x1B0

glabel Fog_display
    /* DD8934 00007EC8 3C0E0000 */  lui        $t6, %hi(func_00000008 + 0x2C)
  alabel sStr_Sun_Flare_7ECC
    /* DD8938 00007ECC 8DCE0034 */  lw         $t6, %lo(func_00000008 + 0x2C)($t6)
    /* DD893C 00007ED0 27BDFFD0 */  addiu      $sp, $sp, -0x30
    /* DD8940 00007ED4 AFB00028 */  sw         $s0, 0x28($sp)
  alabel sStr_rom_flare5_bin_7ED8
    /* DD8944 00007ED8 24010008 */  addiu      $at, $zero, 0x8
    /* DD8948 00007EDC 00808025 */  or         $s0, $a0, $zero
    /* DD894C 00007EE0 11C10050 */  beq        $t6, $at, .L00008024
    /* DD8950 00007EE4 AFBF002C */   sw        $ra, 0x2C($sp)
  alabel sStr_sun_7EE8
    /* DD8954 00007EE8 3C030000 */  lui        $v1, (0x0 >> 16)
  alabel sStr_len_7EEC
    /* DD8958 00007EEC 24630000 */  addiu      $v1, $v1, 0x0
  alabel sStr_lt0_7EF0
    /* DD895C 00007EF0 8C6F0254 */  lw         $t7, 0x254($v1)
  alabel sStr_lt1_7EF4
    /* DD8960 00007EF4 C4620130 */  lwc1       $f2, 0x130($v1)
  alabel D_00007EF8
    /* DD8964 00007EF8 C4860038 */  lwc1       $f6, 0x38($a0)
  alabel sStr_CourseShape_7EFC
    /* DD8968 00007EFC 8DF80070 */  lw         $t8, 0x70($t7)
    /* DD896C 00007F00 C4900054 */  lwc1       $f16, 0x54($a0)
    /* DD8970 00007F04 44809000 */  mtc1       $zero, $f18
  alabel sStr_Map_7F08
    /* DD8974 00007F08 C70400A8 */  lwc1       $f4, 0xA8($t8)
  alabel sStr_MorphShape_7F0C
    /* DD8978 00007F0C 3C040000 */  lui        $a0, %hi(func_0000027C + 0x18)
    /* DD897C 00007F10 3C0B0000 */  lui        $t3, (0x0 >> 16)
    /* DD8980 00007F14 46022002 */  mul.s      $f0, $f4, $f2
  alabel sStr_morphpolyshape_7F18
    /* DD8984 00007F18 00001025 */  or         $v0, $zero, $zero
    /* DD8988 00007F1C 24840294 */  addiu      $a0, $a0, %lo(func_0000027C + 0x18)
    /* DD898C 00007F20 46023302 */  mul.s      $f12, $f6, $f2
    /* DD8990 00007F24 256B0000 */  addiu      $t3, $t3, 0x0
  alabel sStr_ParseCmds_7F28
    /* DD8994 00007F28 4600603C */  c.lt.s     $f12, $f0
    /* DD8998 00007F2C 460C8380 */  add.s      $f14, $f16, $f12
    /* DD899C 00007F30 4502000A */  bc1fl      .L00007F5C
  alabel sStr_Colours_7F34
    /* DD89A0 00007F34 4600703C */   c.lt.s    $f14, $f0
    /* DD89A4 00007F38 460E003C */  c.lt.s     $f0, $f14
  alabel sStr_Init_7F3C
    /* DD89A8 00007F3C 00000000 */  nop
    /* DD89AC 00007F40 45020006 */  bc1fl      .L00007F5C
  alabel sStr_MapLighter_7F44
    /* DD89B0 00007F44 4600703C */   c.lt.s    $f14, $f0
    /* DD89B4 00007F48 460C0201 */  sub.s      $f8, $f0, $f12
    /* DD89B8 00007F4C 24020001 */  addiu      $v0, $zero, 0x1
  alabel sStr_ItemMgr_7F50
    /* DD89BC 00007F50 1000000E */  b          .L00007F8C
    /* DD89C0 00007F54 46104483 */   div.s     $f18, $f8, $f16
  alabel sStr_st0_7F58
    /* DD89C4 00007F58 4600703C */  c.lt.s     $f14, $f0
  .L00007F5C:
    /* DD89C8 00007F5C 00000000 */  nop
    /* DD89CC 00007F60 4502000B */  bc1fl      .L00007F90
  alabel sStr_gol_7F64
    /* DD89D0 00007F64 8E19008C */   lw        $t9, 0x8C($s0)
  alabel sStr_no_gol_7F68
    /* DD89D4 00007F68 C60A006C */  lwc1       $f10, 0x6C($s0)
    /* DD89D8 00007F6C 3C013F80 */  lui        $at, (0x3F800000 >> 16)
  alabel sStr_Markers_7F70
    /* DD89DC 00007F70 460C5100 */  add.s      $f4, $f10, $f12
    /* DD89E0 00007F74 4604003C */  c.lt.s     $f0, $f4
  alabel sStr_MarkerMgr_7F78
    /* DD89E4 00007F78 00000000 */  nop
    /* DD89E8 00007F7C 45020004 */  bc1fl      .L00007F90
    /* DD89EC 00007F80 8E19008C */   lw        $t9, 0x8C($s0)
  alabel D_00007F84
    /* DD89F0 00007F84 44819000 */  mtc1       $at, $f18
    /* DD89F4 00007F88 24020001 */  addiu      $v0, $zero, 0x1
  .L00007F8C:
    /* DD89F8 00007F8C 8E19008C */  lw         $t9, 0x8C($s0)
  .L00007F90:
    /* DD89FC 00007F90 10590010 */  beq        $v0, $t9, .L00007FD4
    /* DD8A00 00007F94 00000000 */   nop
    /* DD8A04 00007F98 8C8A0000 */  lw         $t2, 0x0($a0)
    /* DD8A08 00007F9C 3C080000 */  lui        $t0, (0x0 >> 16)
  alabel D_00007FA0
    /* DD8A0C 00007FA0 25080000 */  addiu      $t0, $t0, 0x0
    /* DD8A10 00007FA4 AD0A0000 */  sw         $t2, 0x0($t0)
    /* DD8A14 00007FA8 8C890004 */  lw         $t1, 0x4($a0)
    /* DD8A18 00007FAC AD090004 */  sw         $t1, 0x4($t0)
  alabel D_00007FB0
    /* DD8A1C 00007FB0 8C8A0008 */  lw         $t2, 0x8($a0)
    /* DD8A20 00007FB4 AD0A0008 */  sw         $t2, 0x8($t0)
    /* DD8A24 00007FB8 8C89000C */  lw         $t1, 0xC($a0)
  alabel D_00007FBC
    /* DD8A28 00007FBC AD09000C */  sw         $t1, 0xC($t0)
    /* DD8A2C 00007FC0 8C8A0010 */  lw         $t2, 0x10($a0)
  alabel D_00007FC4
    /* DD8A30 00007FC4 AD0A0010 */  sw         $t2, 0x10($t0)
    /* DD8A34 00007FC8 8C890014 */  lw         $t1, 0x14($a0)
  alabel D_00007FCC
    /* DD8A38 00007FCC AD090014 */  sw         $t1, 0x14($t0)
  alabel D_00007FD0
    /* DD8A3C 00007FD0 AE02008C */  sw         $v0, 0x8C($s0)
  .L00007FD4:
    /* DD8A40 00007FD4 3C040000 */  lui        $a0, %hi(func_0000027C + 0x18)
    /* DD8A44 00007FD8 10400022 */  beqz       $v0, .L00008064
  alabel D_00007FDC
    /* DD8A48 00007FDC 24840294 */   addiu     $a0, $a0, %lo(func_0000027C + 0x18)
    /* DD8A4C 00007FE0 8D6D0000 */  lw         $t5, 0x0($t3)
  alabel D_00007FE4
    /* DD8A50 00007FE4 44069000 */  mfc1       $a2, $f18
    /* DD8A54 00007FE8 26050074 */  addiu      $a1, $s0, 0x74
  alabel D_00007FEC
    /* DD8A58 00007FEC AC8D0000 */  sw         $t5, 0x0($a0)
  alabel D_00007FF0
    /* DD8A5C 00007FF0 8D6C0004 */  lw         $t4, 0x4($t3)
  alabel D_00007FF4
    /* DD8A60 00007FF4 AC8C0004 */  sw         $t4, 0x4($a0)
    /* DD8A64 00007FF8 8D6D0008 */  lw         $t5, 0x8($t3)
    /* DD8A68 00007FFC AC8D0008 */  sw         $t5, 0x8($a0)
    /* DD8A6C 00008000 8D6C000C */  lw         $t4, 0xC($t3)
    /* DD8A70 00008004 AC8C000C */  sw         $t4, 0xC($a0)
    /* DD8A74 00008008 8D6D0010 */  lw         $t5, 0x10($t3)
    /* DD8A78 0000800C AC8D0010 */  sw         $t5, 0x10($a0)
    /* DD8A7C 00008010 8D6C0014 */  lw         $t4, 0x14($t3)
    /* DD8A80 00008014 0C000000 */  jal        func_00000000
  alabel D_00008018
    /* DD8A84 00008018 AC8C0014 */   sw        $t4, 0x14($a0)
    /* DD8A88 0000801C 10000012 */  b          .L00008068
    /* DD8A8C 00008020 8FBF002C */   lw        $ra, 0x2C($sp)
  .L00008024:
    /* DD8A90 00008024 C6060038 */  lwc1       $f6, 0x38($s0)
    /* DD8A94 00008028 44804000 */  mtc1       $zero, $f8
    /* DD8A98 0000802C 3C030000 */  lui        $v1, (0x0 >> 16)
    /* DD8A9C 00008030 8E060030 */  lw         $a2, 0x30($s0)
    /* DD8AA0 00008034 8E070034 */  lw         $a3, 0x34($s0)
    /* DD8AA4 00008038 24630000 */  addiu      $v1, $v1, 0x0
    /* DD8AA8 0000803C 8C640254 */  lw         $a0, 0x254($v1)
    /* DD8AAC 00008040 AFA00014 */  sw         $zero, 0x14($sp)
    /* DD8AB0 00008044 24050003 */  addiu      $a1, $zero, 0x3
    /* DD8AB4 00008048 E7A60010 */  swc1       $f6, 0x10($sp)
    /* DD8AB8 0000804C 0C000000 */  jal        func_00000000
    /* DD8ABC 00008050 E7A80018 */   swc1      $f8, 0x18($sp)
    /* DD8AC0 00008054 50400004 */  beql       $v0, $zero, .L00008068
    /* DD8AC4 00008058 8FBF002C */   lw        $ra, 0x2C($sp)
    /* DD8AC8 0000805C 0C000000 */  jal        func_00000000
  alabel D_00008060
    /* DD8ACC 00008060 02002025 */   or        $a0, $s0, $zero
  .L00008064:
    /* DD8AD0 00008064 8FBF002C */  lw         $ra, 0x2C($sp)
  .L00008068:
    /* DD8AD4 00008068 8FB00028 */  lw         $s0, 0x28($sp)
    /* DD8AD8 0000806C 27BD0030 */  addiu      $sp, $sp, 0x30
    /* DD8ADC 00008070 03E00008 */  jr         $ra
    /* DD8AE0 00008074 00000000 */   nop
endlabel Fog_display
