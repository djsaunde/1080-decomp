nonmatching SnowTmpl_create, 0x1C4

glabel SnowTmpl_create
    /* DD8F0C 000084A0 27BDFFB0 */  addiu      $sp, $sp, -0x50
    /* DD8F10 000084A4 3C050000 */  lui        $a1, (0x0 >> 16)
    /* DD8F14 000084A8 8CA50000 */  lw         $a1, 0x0($a1)
    /* DD8F18 000084AC AFBF001C */  sw         $ra, 0x1C($sp)
    /* DD8F1C 000084B0 AFB00018 */  sw         $s0, 0x18($sp)
    /* DD8F20 000084B4 AFA40050 */  sw         $a0, 0x50($sp)
    /* DD8F24 000084B8 8CAE0040 */  lw         $t6, 0x40($a1)
    /* DD8F28 000084BC 15C00003 */  bnez       $t6, .L000084CC
    /* DD8F2C 000084C0 00000000 */   nop
    /* DD8F30 000084C4 0C000000 */  jal        func_00000000
    /* DD8F34 000084C8 00A02025 */   or        $a0, $a1, $zero
  .L000084CC:
    /* DD8F38 000084CC 0C000000 */  jal        func_00000000
    /* DD8F3C 000084D0 24040074 */   addiu     $a0, $zero, 0x74
    /* DD8F40 000084D4 1040004E */  beqz       $v0, .L00008610
    /* DD8F44 000084D8 00408025 */   or        $s0, $v0, $zero
    /* DD8F48 000084DC 0C000000 */  jal        func_00000000
    /* DD8F4C 000084E0 00402025 */   or        $a0, $v0, $zero
    /* DD8F50 000084E4 3C0F0000 */  lui        $t7, %hi(D_00007968)
    /* DD8F54 000084E8 25EF7968 */  addiu      $t7, $t7, %lo(D_00007968)
    /* DD8F58 000084EC 2401FFC4 */  addiu      $at, $zero, -0x3C
    /* DD8F5C 000084F0 AE0F0028 */  sw         $t7, 0x28($s0)
    /* DD8F60 000084F4 16010005 */  bne        $s0, $at, .L0000850C
    /* DD8F64 000084F8 2605003C */   addiu     $a1, $s0, 0x3C
    /* DD8F68 000084FC 0C000000 */  jal        func_00000000
    /* DD8F6C 00008500 24040038 */   addiu     $a0, $zero, 0x38
    /* DD8F70 00008504 10400042 */  beqz       $v0, .L00008610
    /* DD8F74 00008508 00402825 */   or        $a1, $v0, $zero
  .L0000850C:
    /* DD8F78 0000850C 14A00007 */  bnez       $a1, .L0000852C
    /* DD8F7C 00008510 00A01825 */   or        $v1, $a1, $zero
    /* DD8F80 00008514 24040008 */  addiu      $a0, $zero, 0x8
    /* DD8F84 00008518 0C000000 */  jal        func_00000000
    /* DD8F88 0000851C AFA50044 */   sw        $a1, 0x44($sp)
    /* DD8F8C 00008520 8FA50044 */  lw         $a1, 0x44($sp)
    /* DD8F90 00008524 10400005 */  beqz       $v0, .L0000853C
    /* DD8F94 00008528 00401825 */   or        $v1, $v0, $zero
  .L0000852C:
    /* DD8F98 0000852C 3C180000 */  lui        $t8, %hi(D_00007FE4)
    /* DD8F9C 00008530 27187FE4 */  addiu      $t8, $t8, %lo(D_00007FE4)
    /* DD8FA0 00008534 AC780000 */  sw         $t8, 0x0($v1)
    /* DD8FA4 00008538 AC600004 */  sw         $zero, 0x4($v1)
  .L0000853C:
    /* DD8FA8 0000853C 3C190000 */  lui        $t9, %hi(D_00007FEC)
    /* DD8FAC 00008540 8F397FEC */  lw         $t9, %lo(D_00007FEC)($t9)
    /* DD8FB0 00008544 2401FFF8 */  addiu      $at, $zero, -0x8
    /* DD8FB4 00008548 24A40008 */  addiu      $a0, $a1, 0x8
    /* DD8FB8 0000854C AFB90040 */  sw         $t9, 0x40($sp)
    /* DD8FBC 00008550 14A10007 */  bne        $a1, $at, .L00008570
    /* DD8FC0 00008554 AFB90024 */   sw        $t9, 0x24($sp)
    /* DD8FC4 00008558 24040018 */  addiu      $a0, $zero, 0x18
    /* DD8FC8 0000855C 0C000000 */  jal        func_00000000
    /* DD8FCC 00008560 AFA50044 */   sw        $a1, 0x44($sp)
    /* DD8FD0 00008564 8FA50044 */  lw         $a1, 0x44($sp)
    /* DD8FD4 00008568 10400010 */  beqz       $v0, .L000085AC
    /* DD8FD8 0000856C 00402025 */   or        $a0, $v0, $zero
  .L00008570:
    /* DD8FDC 00008570 8FA60024 */  lw         $a2, 0x24($sp)
    /* DD8FE0 00008574 24070001 */  addiu      $a3, $zero, 0x1
    /* DD8FE4 00008578 AFA4002C */  sw         $a0, 0x2C($sp)
    /* DD8FE8 0000857C AFA50044 */  sw         $a1, 0x44($sp)
    /* DD8FEC 00008580 0C000000 */  jal        func_00000000
    /* DD8FF0 00008584 AFA60008 */   sw        $a2, 0x8($sp)
    /* DD8FF4 00008588 8FA4002C */  lw         $a0, 0x2C($sp)
    /* DD8FF8 0000858C 3C014396 */  lui        $at, (0x43960000 >> 16)
    /* DD8FFC 00008590 44812000 */  mtc1       $at, $f4
    /* DD9000 00008594 8FA50044 */  lw         $a1, 0x44($sp)
    /* DD9004 00008598 3C0A0000 */  lui        $t2, %hi(D_00007540)
    /* DD9008 0000859C 254A7540 */  addiu      $t2, $t2, %lo(D_00007540)
    /* DD900C 000085A0 AC8A000C */  sw         $t2, 0xC($a0)
    /* DD9010 000085A4 AC800014 */  sw         $zero, 0x14($a0)
    /* DD9014 000085A8 E4840010 */  swc1       $f4, 0x10($a0)
  .L000085AC:
    /* DD9018 000085AC 3C0B0000 */  lui        $t3, %hi(D_00007FF0)
    /* DD901C 000085B0 8D6B7FF0 */  lw         $t3, %lo(D_00007FF0)($t3)
    /* DD9020 000085B4 2401FFE0 */  addiu      $at, $zero, -0x20
    /* DD9024 000085B8 24A40020 */  addiu      $a0, $a1, 0x20
    /* DD9028 000085BC AFAB003C */  sw         $t3, 0x3C($sp)
    /* DD902C 000085C0 14A10007 */  bne        $a1, $at, .L000085E0
    /* DD9030 000085C4 AFAB0024 */   sw        $t3, 0x24($sp)
    /* DD9034 000085C8 24040018 */  addiu      $a0, $zero, 0x18
    /* DD9038 000085CC 0C000000 */  jal        func_00000000
    /* DD903C 000085D0 AFA50044 */   sw        $a1, 0x44($sp)
    /* DD9040 000085D4 8FA50044 */  lw         $a1, 0x44($sp)
    /* DD9044 000085D8 1040000D */  beqz       $v0, .L00008610
    /* DD9048 000085DC 00402025 */   or        $a0, $v0, $zero
  .L000085E0:
    /* DD904C 000085E0 8FA60024 */  lw         $a2, 0x24($sp)
    /* DD9050 000085E4 00003825 */  or         $a3, $zero, $zero
    /* DD9054 000085E8 AFA4002C */  sw         $a0, 0x2C($sp)
    /* DD9058 000085EC 0C000000 */  jal        func_00000000
    /* DD905C 000085F0 AFA60008 */   sw        $a2, 0x8($sp)
    /* DD9060 000085F4 8FA4002C */  lw         $a0, 0x2C($sp)
    /* DD9064 000085F8 3C0E0000 */  lui        $t6, %hi(func_00007620 + 0x70)
    /* DD9068 000085FC 25CE7690 */  addiu      $t6, $t6, %lo(func_00007620 + 0x70)
    /* DD906C 00008600 240F0001 */  addiu      $t7, $zero, 0x1
    /* DD9070 00008604 AC8F0010 */  sw         $t7, 0x10($a0)
    /* DD9074 00008608 AC8E000C */  sw         $t6, 0xC($a0)
    /* DD9078 0000860C AC800014 */  sw         $zero, 0x14($a0)
  .L00008610:
    /* DD907C 00008610 8FB80050 */  lw         $t8, 0x50($sp)
    /* DD9080 00008614 02001825 */  or         $v1, $s0, $zero
    /* DD9084 00008618 24640010 */  addiu      $a0, $v1, 0x10
    /* DD9088 0000861C 8F020040 */  lw         $v0, 0x40($t8)
    /* DD908C 00008620 1040000B */  beqz       $v0, .L00008650
    /* DD9090 00008624 00408025 */   or        $s0, $v0, $zero
    /* DD9094 00008628 00402825 */  or         $a1, $v0, $zero
    /* DD9098 0000862C 0C000000 */  jal        func_00000000
    /* DD909C 00008630 AFA3004C */   sw        $v1, 0x4C($sp)
    /* DD90A0 00008634 8E190014 */  lw         $t9, 0x14($s0)
    /* DD90A4 00008638 8FA3004C */  lw         $v1, 0x4C($sp)
    /* DD90A8 0000863C 24080001 */  addiu      $t0, $zero, 0x1
    /* DD90AC 00008640 53200003 */  beql       $t9, $zero, .L00008650
    /* DD90B0 00008644 AE030014 */   sw        $v1, 0x14($s0)
    /* DD90B4 00008648 AE080004 */  sw         $t0, 0x4($s0)
    /* DD90B8 0000864C AE030014 */  sw         $v1, 0x14($s0)
  .L00008650:
    /* DD90BC 00008650 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DD90C0 00008654 8FB00018 */  lw         $s0, 0x18($sp)
    /* DD90C4 00008658 27BD0050 */  addiu      $sp, $sp, 0x50
    /* DD90C8 0000865C 03E00008 */  jr         $ra
    /* DD90CC 00008660 00601025 */   or        $v0, $v1, $zero
endlabel SnowTmpl_create
