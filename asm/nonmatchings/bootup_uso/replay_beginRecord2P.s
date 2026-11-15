nonmatching replay_beginRecord2P, 0x124

glabel replay_beginRecord2P
    /* DE340C 000129A0 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DE3410 000129A4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DE3414 000129A8 AFA40018 */  sw         $a0, 0x18($sp)
    /* DE3418 000129AC AFA5001C */  sw         $a1, 0x1C($sp)
    /* DE341C 000129B0 AFA60020 */  sw         $a2, 0x20($sp)
    /* DE3420 000129B4 8FAE001C */  lw         $t6, 0x1C($sp)
    /* DE3424 000129B8 11C00021 */  beqz       $t6, .L00012A40
    /* DE3428 000129BC 00000000 */   nop
    /* DE342C 000129C0 8FAF0018 */  lw         $t7, 0x18($sp)
    /* DE3430 000129C4 8DF80168 */  lw         $t8, 0x168($t7)
    /* DE3434 000129C8 17000004 */  bnez       $t8, .L000129DC
    /* DE3438 000129CC 00000000 */   nop
    /* DE343C 000129D0 8FB90018 */  lw         $t9, 0x18($sp)
    /* DE3440 000129D4 8F280160 */  lw         $t0, 0x160($t9)
    /* DE3444 000129D8 AF280168 */  sw         $t0, 0x168($t9)
  .L000129DC:
    /* DE3448 000129DC 8FA90018 */  lw         $t1, 0x18($sp)
    /* DE344C 000129E0 8D2A0064 */  lw         $t2, 0x64($t1)
    /* DE3450 000129E4 A12A0178 */  sb         $t2, 0x178($t1)
    /* DE3454 000129E8 8FAB0018 */  lw         $t3, 0x18($sp)
    /* DE3458 000129EC 8D6C004C */  lw         $t4, 0x4C($t3)
    /* DE345C 000129F0 A16C0179 */  sb         $t4, 0x179($t3)
    /* DE3460 000129F4 8FAD0018 */  lw         $t5, 0x18($sp)
    /* DE3464 000129F8 8DAE0050 */  lw         $t6, 0x50($t5)
    /* DE3468 000129FC A1AE017A */  sb         $t6, 0x17A($t5)
    /* DE346C 00012A00 8FAF0018 */  lw         $t7, 0x18($sp)
    /* DE3470 00012A04 8DF80054 */  lw         $t8, 0x54($t7)
    /* DE3474 00012A08 A1F8017B */  sb         $t8, 0x17B($t7)
    /* DE3478 00012A0C 3C08FFFB */  lui        $t0, (0xFFFBFFFF >> 16)
    /* DE347C 00012A10 3508FFFF */  ori        $t0, $t0, (0xFFFBFFFF & 0xFFFF)
    /* DE3480 00012A14 8FB9001C */  lw         $t9, 0x1C($sp)
    /* DE3484 00012A18 AF28006C */  sw         $t0, 0x6C($t9)
    /* DE3488 00012A1C 8FA4001C */  lw         $a0, 0x1C($sp)
    /* DE348C 00012A20 8FAA0018 */  lw         $t2, 0x18($sp)
    /* DE3490 00012A24 8D450168 */  lw         $a1, 0x168($t2)
    /* DE3494 00012A28 8D460164 */  lw         $a2, 0x164($t2)
    /* DE3498 00012A2C 25470170 */  addiu      $a3, $t2, 0x170
    /* DE349C 00012A30 0C000000 */  jal        func_00000000
    /* DE34A0 00012A34 00000000 */   nop
    /* DE34A4 00012A38 8FA90018 */  lw         $t1, 0x18($sp)
    /* DE34A8 00012A3C AD200184 */  sw         $zero, 0x184($t1)
  .L00012A40:
    /* DE34AC 00012A40 8FAC0020 */  lw         $t4, 0x20($sp)
    /* DE34B0 00012A44 11800019 */  beqz       $t4, .L00012AAC
    /* DE34B4 00012A48 00000000 */   nop
    /* DE34B8 00012A4C 8FAB0018 */  lw         $t3, 0x18($sp)
    /* DE34BC 00012A50 8D6E016C */  lw         $t6, 0x16C($t3)
    /* DE34C0 00012A54 15C00008 */  bnez       $t6, .L00012A78
    /* DE34C4 00012A58 00000000 */   nop
    /* DE34C8 00012A5C 8FAD0018 */  lw         $t5, 0x18($sp)
    /* DE34CC 00012A60 8DA40164 */  lw         $a0, 0x164($t5)
    /* DE34D0 00012A64 00042080 */  sll        $a0, $a0, 2
    /* DE34D4 00012A68 0C000000 */  jal        func_00000000
    /* DE34D8 00012A6C 00000000 */   nop
    /* DE34DC 00012A70 8FB80018 */  lw         $t8, 0x18($sp)
    /* DE34E0 00012A74 AF02016C */  sw         $v0, 0x16C($t8)
  .L00012A78:
    /* DE34E4 00012A78 3C0FFFFB */  lui        $t7, (0xFFFBFFFF >> 16)
    /* DE34E8 00012A7C 35EFFFFF */  ori        $t7, $t7, (0xFFFBFFFF & 0xFFFF)
    /* DE34EC 00012A80 8FA80020 */  lw         $t0, 0x20($sp)
    /* DE34F0 00012A84 AD0F006C */  sw         $t7, 0x6C($t0)
    /* DE34F4 00012A88 8FA40020 */  lw         $a0, 0x20($sp)
    /* DE34F8 00012A8C 8FB90018 */  lw         $t9, 0x18($sp)
    /* DE34FC 00012A90 8F25016C */  lw         $a1, 0x16C($t9)
    /* DE3500 00012A94 8F260164 */  lw         $a2, 0x164($t9)
    /* DE3504 00012A98 27270174 */  addiu      $a3, $t9, 0x174
    /* DE3508 00012A9C 0C000000 */  jal        func_00000000
    /* DE350C 00012AA0 00000000 */   nop
    /* DE3510 00012AA4 8FAA0018 */  lw         $t2, 0x18($sp)
    /* DE3514 00012AA8 AD400184 */  sw         $zero, 0x184($t2)
  .L00012AAC:
    /* DE3518 00012AAC 10000001 */  b          .L00012AB4
    /* DE351C 00012AB0 00000000 */   nop
  .L00012AB4:
    /* DE3520 00012AB4 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DE3524 00012AB8 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DE3528 00012ABC 03E00008 */  jr         $ra
    /* DE352C 00012AC0 00000000 */   nop
endlabel replay_beginRecord2P
