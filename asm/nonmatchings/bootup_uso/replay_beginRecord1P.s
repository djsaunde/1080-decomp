nonmatching replay_beginRecord1P, 0xB8

glabel replay_beginRecord1P
    /* DE3530 00012AC4 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DE3534 00012AC8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DE3538 00012ACC AFA40018 */  sw         $a0, 0x18($sp)
    /* DE353C 00012AD0 AFA5001C */  sw         $a1, 0x1C($sp)
    /* DE3540 00012AD4 AFA60020 */  sw         $a2, 0x20($sp)
    /* DE3544 00012AD8 8FAE001C */  lw         $t6, 0x1C($sp)
    /* DE3548 00012ADC 11C00021 */  beqz       $t6, .L00012B64
    /* DE354C 00012AE0 00000000 */   nop
    /* DE3550 00012AE4 8FAF0018 */  lw         $t7, 0x18($sp)
    /* DE3554 00012AE8 8DF80168 */  lw         $t8, 0x168($t7)
    /* DE3558 00012AEC 17000004 */  bnez       $t8, .L00012B00
    /* DE355C 00012AF0 00000000 */   nop
    /* DE3560 00012AF4 8FB90018 */  lw         $t9, 0x18($sp)
    /* DE3564 00012AF8 8F280160 */  lw         $t0, 0x160($t9)
    /* DE3568 00012AFC AF280168 */  sw         $t0, 0x168($t9)
  .L00012B00:
    /* DE356C 00012B00 8FA90018 */  lw         $t1, 0x18($sp)
    /* DE3570 00012B04 8D2A0064 */  lw         $t2, 0x64($t1)
    /* DE3574 00012B08 A12A0178 */  sb         $t2, 0x178($t1)
    /* DE3578 00012B0C 8FAB0018 */  lw         $t3, 0x18($sp)
    /* DE357C 00012B10 8D6C004C */  lw         $t4, 0x4C($t3)
    /* DE3580 00012B14 A16C0179 */  sb         $t4, 0x179($t3)
    /* DE3584 00012B18 8FAD0018 */  lw         $t5, 0x18($sp)
    /* DE3588 00012B1C 8DAE0050 */  lw         $t6, 0x50($t5)
    /* DE358C 00012B20 A1AE017A */  sb         $t6, 0x17A($t5)
    /* DE3590 00012B24 8FAF0018 */  lw         $t7, 0x18($sp)
    /* DE3594 00012B28 8DF80054 */  lw         $t8, 0x54($t7)
    /* DE3598 00012B2C A1F8017B */  sb         $t8, 0x17B($t7)
    /* DE359C 00012B30 3C08FFFB */  lui        $t0, (0xFFFBFFFF >> 16)
    /* DE35A0 00012B34 3508FFFF */  ori        $t0, $t0, (0xFFFBFFFF & 0xFFFF)
    /* DE35A4 00012B38 8FB9001C */  lw         $t9, 0x1C($sp)
    /* DE35A8 00012B3C AF28006C */  sw         $t0, 0x6C($t9)
    /* DE35AC 00012B40 8FA4001C */  lw         $a0, 0x1C($sp)
    /* DE35B0 00012B44 8FAA0018 */  lw         $t2, 0x18($sp)
    /* DE35B4 00012B48 8D450168 */  lw         $a1, 0x168($t2)
    /* DE35B8 00012B4C 8D460164 */  lw         $a2, 0x164($t2)
    /* DE35BC 00012B50 25470170 */  addiu      $a3, $t2, 0x170
    /* DE35C0 00012B54 0C000000 */  jal        func_00000000
    /* DE35C4 00012B58 00000000 */   nop
    /* DE35C8 00012B5C 8FA90018 */  lw         $t1, 0x18($sp)
    /* DE35CC 00012B60 AD200184 */  sw         $zero, 0x184($t1)
  .L00012B64:
    /* DE35D0 00012B64 10000001 */  b          .L00012B6C
    /* DE35D4 00012B68 00000000 */   nop
  .L00012B6C:
    /* DE35D8 00012B6C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DE35DC 00012B70 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DE35E0 00012B74 03E00008 */  jr         $ra
    /* DE35E4 00012B78 00000000 */   nop
endlabel replay_beginRecord1P
