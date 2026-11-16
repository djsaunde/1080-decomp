nonmatching FallingSnow_create, 0x138

glabel FallingSnow_create
    /* DD7F54 000074E8 3C0E0000 */  lui        $t6, %hi(stream_readS32_00000008 + 0x2C)
  alabel sStr_Player2_74EC
    /* DD7F58 000074EC 8DCE0034 */  lw         $t6, %lo(stream_readS32_00000008 + 0x2C)($t6)
    /* DD7F5C 000074F0 27BDFFD8 */  addiu      $sp, $sp, -0x28
  alabel sStr_Mountain_74F4
    /* DD7F60 000074F4 24010008 */  addiu      $at, $zero, 0x8
    /* DD7F64 000074F8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD7F68 000074FC 11C10044 */  beq        $t6, $at, .L00007610
  alabel sStr_Pos_Y_7500
    /* DD7F6C 00007500 AFA40028 */   sw        $a0, 0x28($sp)
    /* DD7F70 00007504 3C040000 */  lui        $a0, %hi(sStr_FallingSnow_7FB0)
  alabel sStr_Scale_Y_7508
    /* DD7F74 00007508 24847FB0 */  addiu      $a0, $a0, %lo(sStr_FallingSnow_7FB0)
    /* DD7F78 0000750C 0C000000 */  jal        func_00000000
  alabel sStr_Scale_XZ_7510
    /* DD7F7C 00007510 00002825 */   or        $a1, $zero, $zero
    /* DD7F80 00007514 0C000000 */  jal        func_00000000
    /* DD7F84 00007518 24040134 */   addiu     $a0, $zero, 0x134
    /* DD7F88 0000751C 10400016 */  beqz       $v0, .L00007578
  alabel D_00007520
    /* DD7F8C 00007520 00403025 */   or        $a2, $v0, $zero
    /* DD7F90 00007524 14400007 */  bnez       $v0, .L00007544
  alabel D_00007528
    /* DD7F94 00007528 00402025 */   or        $a0, $v0, $zero
    /* DD7F98 0000752C 24040048 */  addiu      $a0, $zero, 0x48
    /* DD7F9C 00007530 0C000000 */  jal        func_00000000
    /* DD7FA0 00007534 AFA20020 */   sw        $v0, 0x20($sp)
    /* DD7FA4 00007538 8FA60020 */  lw         $a2, 0x20($sp)
    /* DD7FA8 0000753C 1040000B */  beqz       $v0, .L0000756C
  alabel D_00007540
    /* DD7FAC 00007540 00402025 */   or        $a0, $v0, $zero
  .L00007544:
    /* DD7FB0 00007544 3C050000 */  lui        $a1, %hi(sStr_Node_7FBC)
    /* DD7FB4 00007548 24A57FBC */  addiu      $a1, $a1, %lo(sStr_Node_7FBC)
    /* DD7FB8 0000754C AFA4001C */  sw         $a0, 0x1C($sp)
    /* DD7FBC 00007550 0C000000 */  jal        func_00000000
    /* DD7FC0 00007554 AFA60020 */   sw        $a2, 0x20($sp)
    /* DD7FC4 00007558 8FA4001C */  lw         $a0, 0x1C($sp)
    /* DD7FC8 0000755C 3C0F0000 */  lui        $t7, (0x0 >> 16)
    /* DD7FCC 00007560 25EF0000 */  addiu      $t7, $t7, 0x0
    /* DD7FD0 00007564 8FA60020 */  lw         $a2, 0x20($sp)
    /* DD7FD4 00007568 AC8F0028 */  sw         $t7, 0x28($a0)
  .L0000756C:
    /* DD7FD8 0000756C 3C180000 */  lui        $t8, (0x0 >> 16)
    /* DD7FDC 00007570 27180000 */  addiu      $t8, $t8, 0x0
    /* DD7FE0 00007574 ACD80028 */  sw         $t8, 0x28($a2)
  .L00007578:
    /* DD7FE4 00007578 8FB90028 */  lw         $t9, 0x28($sp)
    /* DD7FE8 0000757C 3C080000 */  lui        $t0, %hi(stream_readQuad_000000F0 + 0x44)
    /* DD7FEC 00007580 00C02825 */  or         $a1, $a2, $zero
    /* DD7FF0 00007584 AF260040 */  sw         $a2, 0x40($t9)
    /* DD7FF4 00007588 8D080134 */  lw         $t0, %lo(stream_readQuad_000000F0 + 0x44)($t0)
    /* DD7FF8 0000758C 8D070088 */  lw         $a3, 0x88($t0)
    /* DD7FFC 00007590 AFA60020 */  sw         $a2, 0x20($sp)
    /* DD8000 00007594 24E40010 */  addiu      $a0, $a3, 0x10
    /* DD8004 00007598 0C000000 */  jal        func_00000000
    /* DD8008 0000759C AFA70018 */   sw        $a3, 0x18($sp)
    /* DD800C 000075A0 8FA60020 */  lw         $a2, 0x20($sp)
    /* DD8010 000075A4 8FA70018 */  lw         $a3, 0x18($sp)
    /* DD8014 000075A8 240A0001 */  addiu      $t2, $zero, 0x1
    /* DD8018 000075AC 8CC90014 */  lw         $t1, 0x14($a2)
    /* DD801C 000075B0 51200003 */  beql       $t1, $zero, .L000075C0
    /* DD8020 000075B4 ACC70014 */   sw        $a3, 0x14($a2)
    /* DD8024 000075B8 ACCA0004 */  sw         $t2, 0x4($a2)
    /* DD8028 000075BC ACC70014 */  sw         $a3, 0x14($a2)
  .L000075C0:
    /* DD802C 000075C0 8FAB0028 */  lw         $t3, 0x28($sp)
    /* DD8030 000075C4 24050096 */  addiu      $a1, $zero, 0x96
    /* DD8034 000075C8 8D630040 */  lw         $v1, 0x40($t3)
    /* DD8038 000075CC 8C620028 */  lw         $v0, 0x28($v1)
    /* DD803C 000075D0 8C59005C */  lw         $t9, 0x5C($v0)
    /* DD8040 000075D4 844C0058 */  lh         $t4, 0x58($v0)
    /* DD8044 000075D8 0320F809 */  jalr       $t9
    /* DD8048 000075DC 01832021 */   addu      $a0, $t4, $v1
    /* DD804C 000075E0 8FAD0028 */  lw         $t5, 0x28($sp)
    /* DD8050 000075E4 240E0001 */  addiu      $t6, $zero, 0x1
    /* DD8054 000075E8 44802000 */  mtc1       $zero, $f4
    /* DD8058 000075EC 8DA20040 */  lw         $v0, 0x40($t5)
    /* DD805C 000075F0 AC4E0048 */  sw         $t6, 0x48($v0)
    /* DD8060 000075F4 8FAF0028 */  lw         $t7, 0x28($sp)
    /* DD8064 000075F8 8DE20040 */  lw         $v0, 0x40($t7)
    /* DD8068 000075FC E444004C */  swc1       $f4, 0x4C($v0)
  alabel ColourItem_vtbl_75A0_type_7600
    /* DD806C 00007600 8FB80028 */  lw         $t8, 0x28($sp)
    /* DD8070 00007604 8F080040 */  lw         $t0, 0x40($t8)
    /* DD8074 00007608 0C000000 */  jal        func_00000000
    /* DD8078 0000760C AD000048 */   sw        $zero, 0x48($t0)
  .L00007610:
    /* DD807C 00007610 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD8080 00007614 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DD8084 00007618 03E00008 */  jr         $ra
    /* DD8088 0000761C 00000000 */   nop
endlabel FallingSnow_create
