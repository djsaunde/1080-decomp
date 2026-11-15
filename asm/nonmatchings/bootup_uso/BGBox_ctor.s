nonmatching BGBox_ctor, 0xF4

glabel BGBox_ctor
    /* DD170C 00000CA0 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD1710 00000CA4 AFB00018 */  sw         $s0, 0x18($sp)
    /* DD1714 00000CA8 00808025 */  or         $s0, $a0, $zero
    /* DD1718 00000CAC AFBF001C */  sw         $ra, 0x1C($sp)
    /* DD171C 00000CB0 AFA50024 */  sw         $a1, 0x24($sp)
    /* DD1720 00000CB4 14800007 */  bnez       $a0, .L00000CD4
    /* DD1724 00000CB8 AFA60028 */   sw        $a2, 0x28($sp)
    /* DD1728 00000CBC 24040040 */  addiu      $a0, $zero, 0x40
    /* DD172C 00000CC0 0C000000 */  jal        func_00000000
    /* DD1730 00000CC4 AFA7002C */   sw        $a3, 0x2C($sp)
    /* DD1734 00000CC8 8FA7002C */  lw         $a3, 0x2C($sp)
    /* DD1738 00000CCC 1040002C */  beqz       $v0, .L00000D80
    /* DD173C 00000CD0 00408025 */   or        $s0, $v0, $zero
  .L00000CD4:
    /* DD1740 00000CD4 3C050000 */  lui        $a1, %hi(sStr_Node_66A8)
    /* DD1744 00000CD8 24A566A8 */  addiu      $a1, $a1, %lo(sStr_Node_66A8)
    /* DD1748 00000CDC 02002025 */  or         $a0, $s0, $zero
    /* DD174C 00000CE0 0C000000 */  jal        func_00000000
    /* DD1750 00000CE4 AFA7002C */   sw        $a3, 0x2C($sp)
    /* DD1754 00000CE8 3C0E0000 */  lui        $t6, (0x0 >> 16)
    /* DD1758 00000CEC 8FA7002C */  lw         $a3, 0x2C($sp)
    /* DD175C 00000CF0 25CE0000 */  addiu      $t6, $t6, 0x0
    /* DD1760 00000CF4 AE0E0028 */  sw         $t6, 0x28($s0)
    /* DD1764 00000CF8 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DD1768 00000CFC 44810000 */  mtc1       $at, $f0
    /* DD176C 00000D00 8FAF0024 */  lw         $t7, 0x24($sp)
    /* DD1770 00000D04 44802000 */  mtc1       $zero, $f4
    /* DD1774 00000D08 E6000034 */  swc1       $f0, 0x34($s0)
    /* DD1778 00000D0C AE0F000C */  sw         $t7, 0xC($s0)
    /* DD177C 00000D10 E6000030 */  swc1       $f0, 0x30($s0)
    /* DD1780 00000D14 E600002C */  swc1       $f0, 0x2C($s0)
    /* DD1784 00000D18 E6040038 */  swc1       $f4, 0x38($s0)
    /* DD1788 00000D1C 8FB80028 */  lw         $t8, 0x28($sp)
    /* DD178C 00000D20 3C010000 */  lui        $at, (0x0 >> 16)
    /* DD1790 00000D24 3C030000 */  lui        $v1, (0x0 >> 16)
    /* DD1794 00000D28 AE18003C */  sw         $t8, 0x3C($s0)
    /* DD1798 00000D2C AC270000 */  sw         $a3, 0x0($at)
    /* DD179C 00000D30 3C010000 */  lui        $at, %hi(func_00000000 + 0x4)
    /* DD17A0 00000D34 AC270004 */  sw         $a3, %lo(func_00000000 + 0x4)($at)
    /* DD17A4 00000D38 24630000 */  addiu      $v1, $v1, 0x0
    /* DD17A8 00000D3C 04E10003 */  bgez       $a3, .L00000D4C
    /* DD17AC 00000D40 00072043 */   sra       $a0, $a3, 1
    /* DD17B0 00000D44 24E10001 */  addiu      $at, $a3, 0x1
    /* DD17B4 00000D48 00012043 */  sra        $a0, $at, 1
  .L00000D4C:
    /* DD17B8 00000D4C 00042823 */  negu       $a1, $a0
    /* DD17BC 00000D50 A4640000 */  sh         $a0, 0x0($v1)
    /* DD17C0 00000D54 A4640002 */  sh         $a0, 0x2($v1)
    /* DD17C4 00000D58 A4640004 */  sh         $a0, 0x4($v1)
    /* DD17C8 00000D5C A4650010 */  sh         $a1, 0x10($v1)
    /* DD17CC 00000D60 A4640012 */  sh         $a0, 0x12($v1)
    /* DD17D0 00000D64 A4640014 */  sh         $a0, 0x14($v1)
    /* DD17D4 00000D68 A4650020 */  sh         $a1, 0x20($v1)
    /* DD17D8 00000D6C A4650022 */  sh         $a1, 0x22($v1)
    /* DD17DC 00000D70 A4640024 */  sh         $a0, 0x24($v1)
    /* DD17E0 00000D74 A4640030 */  sh         $a0, 0x30($v1)
    /* DD17E4 00000D78 A4650032 */  sh         $a1, 0x32($v1)
    /* DD17E8 00000D7C A4640034 */  sh         $a0, 0x34($v1)
  .L00000D80:
    /* DD17EC 00000D80 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DD17F0 00000D84 02001025 */  or         $v0, $s0, $zero
    /* DD17F4 00000D88 8FB00018 */  lw         $s0, 0x18($sp)
    /* DD17F8 00000D8C 03E00008 */  jr         $ra
    /* DD17FC 00000D90 27BD0020 */   addiu     $sp, $sp, 0x20
endlabel BGBox_ctor
