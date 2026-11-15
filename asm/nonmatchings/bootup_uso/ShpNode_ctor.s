nonmatching ShpNode_ctor, 0x90

glabel ShpNode_ctor
    /* DD9984 00008F18 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD9988 00008F1C AFB00018 */  sw         $s0, 0x18($sp)
    /* DD998C 00008F20 00808025 */  or         $s0, $a0, $zero
    /* DD9990 00008F24 14800005 */  bnez       $a0, .L00008F3C
    /* DD9994 00008F28 AFBF001C */   sw        $ra, 0x1C($sp)
    /* DD9998 00008F2C 0C000000 */  jal        func_00000000
    /* DD999C 00008F30 240403D8 */   addiu     $a0, $zero, 0x3D8
    /* DD99A0 00008F34 10400017 */  beqz       $v0, .L00008F94
    /* DD99A4 00008F38 00408025 */   or        $s0, $v0, $zero
  .L00008F3C:
    /* DD99A8 00008F3C 3C050001 */  lui        $a1, %hi(sStr_QNode_8708)
    /* DD99AC 00008F40 24A58708 */  addiu      $a1, $a1, %lo(sStr_QNode_8708)
    /* DD99B0 00008F44 0C000000 */  jal        func_00000000
    /* DD99B4 00008F48 02002025 */   or        $a0, $s0, $zero
    /* DD99B8 00008F4C 3C0E0000 */  lui        $t6, (0x0 >> 16)
    /* DD99BC 00008F50 25CE0000 */  addiu      $t6, $t6, 0x0
    /* DD99C0 00008F54 3C050001 */  lui        $a1, %hi(sStr_shp_8710)
    /* DD99C4 00008F58 3C060000 */  lui        $a2, (0x0 >> 16)
    /* DD99C8 00008F5C AE0E0028 */  sw         $t6, 0x28($s0)
    /* DD99CC 00008F60 24C60000 */  addiu      $a2, $a2, 0x0
    /* DD99D0 00008F64 24A58710 */  addiu      $a1, $a1, %lo(sStr_shp_8710)
    /* DD99D4 00008F68 00002025 */  or         $a0, $zero, $zero
    /* DD99D8 00008F6C 0C000000 */  jal        func_00000000
    /* DD99DC 00008F70 00003825 */   or        $a3, $zero, $zero
    /* DD99E0 00008F74 02002025 */  or         $a0, $s0, $zero
    /* DD99E4 00008F78 0C000000 */  jal        func_00000000
    /* DD99E8 00008F7C 00402825 */   or        $a1, $v0, $zero
    /* DD99EC 00008F80 3C014248 */  lui        $at, (0x42480000 >> 16)
    /* DD99F0 00008F84 44812000 */  mtc1       $at, $f4
    /* DD99F4 00008F88 240F0064 */  addiu      $t7, $zero, 0x64
    /* DD99F8 00008F8C AE0F03D4 */  sw         $t7, 0x3D4($s0)
    /* DD99FC 00008F90 E6040260 */  swc1       $f4, 0x260($s0)
  .L00008F94:
    /* DD9A00 00008F94 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DD9A04 00008F98 02001025 */  or         $v0, $s0, $zero
    /* DD9A08 00008F9C 8FB00018 */  lw         $s0, 0x18($sp)
    /* DD9A0C 00008FA0 03E00008 */  jr         $ra
    /* DD9A10 00008FA4 27BD0020 */   addiu     $sp, $sp, 0x20
endlabel ShpNode_ctor
