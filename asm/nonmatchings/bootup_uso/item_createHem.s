nonmatching item_createHem, 0x84

glabel item_createHem
    /* DD5B0C 000050A0 27BDFFB8 */  addiu      $sp, $sp, -0x48
    /* DD5B10 000050A4 AFA40048 */  sw         $a0, 0x48($sp)
    /* DD5B14 000050A8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD5B18 000050AC 3C040000 */  lui        $a0, %hi(D_00007DA4)
    /* DD5B1C 000050B0 0C000000 */  jal        func_00000000
    /* DD5B20 000050B4 24847DA4 */   addiu     $a0, $a0, %lo(D_00007DA4)
    /* DD5B24 000050B8 44800000 */  mtc1       $zero, $f0
    /* DD5B28 000050BC 8FAE0048 */  lw         $t6, 0x48($sp)
    /* DD5B2C 000050C0 24040058 */  addiu      $a0, $zero, 0x58
    /* DD5B30 000050C4 E7A00038 */  swc1       $f0, 0x38($sp)
    /* DD5B34 000050C8 E7A0003C */  swc1       $f0, 0x3C($sp)
    /* DD5B38 000050CC E7A00040 */  swc1       $f0, 0x40($sp)
    /* DD5B3C 000050D0 E7A00044 */  swc1       $f0, 0x44($sp)
    /* DD5B40 000050D4 0C000000 */  jal        func_00000000
    /* DD5B44 000050D8 AFAE001C */   sw        $t6, 0x1C($sp)
    /* DD5B48 000050DC 1040000C */  beqz       $v0, .L00005110
    /* DD5B4C 000050E0 00402025 */   or        $a0, $v0, $zero
    /* DD5B50 000050E4 8FA5001C */  lw         $a1, 0x1C($sp)
    /* DD5B54 000050E8 3C060000 */  lui        $a2, (0x0 >> 16)
    /* DD5B58 000050EC 8CC60000 */  lw         $a2, 0x0($a2)
    /* DD5B5C 000050F0 27A70038 */  addiu      $a3, $sp, 0x38
    /* DD5B60 000050F4 AFA20030 */  sw         $v0, 0x30($sp)
    /* DD5B64 000050F8 0C000000 */  jal        func_00000000
    /* DD5B68 000050FC AFA50004 */   sw        $a1, 0x4($sp)
    /* DD5B6C 00005100 8FA40030 */  lw         $a0, 0x30($sp)
    /* DD5B70 00005104 3C180000 */  lui        $t8, (0x0 >> 16)
    /* DD5B74 00005108 27180000 */  addiu      $t8, $t8, 0x0
    /* DD5B78 0000510C AC980028 */  sw         $t8, 0x28($a0)
  .L00005110:
    /* DD5B7C 00005110 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD5B80 00005114 27BD0048 */  addiu      $sp, $sp, 0x48
    /* DD5B84 00005118 00801025 */  or         $v0, $a0, $zero
    /* DD5B88 0000511C 03E00008 */  jr         $ra
    /* DD5B8C 00005120 00000000 */   nop
endlabel item_createHem
