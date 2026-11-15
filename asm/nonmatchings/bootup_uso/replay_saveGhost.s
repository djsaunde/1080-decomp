nonmatching replay_saveGhost, 0x100

glabel replay_saveGhost
    /* DE36D0 00012C64 27BDFFD0 */  addiu      $sp, $sp, -0x30
    /* DE36D4 00012C68 AFBF0024 */  sw         $ra, 0x24($sp)
    /* DE36D8 00012C6C AFA40030 */  sw         $a0, 0x30($sp)
    /* DE36DC 00012C70 AFA50034 */  sw         $a1, 0x34($sp)
    /* DE36E0 00012C74 00002025 */  or         $a0, $zero, $zero
    /* DE36E4 00012C78 8FAE0030 */  lw         $t6, 0x30($sp)
    /* DE36E8 00012C7C 8DC50148 */  lw         $a1, 0x148($t6)
    /* DE36EC 00012C80 24060003 */  addiu      $a2, $zero, 0x3
    /* DE36F0 00012C84 2407FFFF */  addiu      $a3, $zero, -0x1
    /* DE36F4 00012C88 0C000000 */  jal        func_00000000
    /* DE36F8 00012C8C 00000000 */   nop
    /* DE36FC 00012C90 8FAF0030 */  lw         $t7, 0x30($sp)
    /* DE3700 00012C94 ADE2014C */  sw         $v0, 0x14C($t7)
    /* DE3704 00012C98 8FB80030 */  lw         $t8, 0x30($sp)
    /* DE3708 00012C9C 8F040170 */  lw         $a0, 0x170($t8)
    /* DE370C 00012CA0 00042080 */  sll        $a0, $a0, 2
    /* DE3710 00012CA4 0C000000 */  jal        func_00000000
    /* DE3714 00012CA8 00000000 */   nop
    /* DE3718 00012CAC AFA2002C */  sw         $v0, 0x2C($sp)
    /* DE371C 00012CB0 8FA4002C */  lw         $a0, 0x2C($sp)
    /* DE3720 00012CB4 8FB90030 */  lw         $t9, 0x30($sp)
    /* DE3724 00012CB8 8F250168 */  lw         $a1, 0x168($t9)
    /* DE3728 00012CBC 8F260170 */  lw         $a2, 0x170($t9)
    /* DE372C 00012CC0 00063080 */  sll        $a2, $a2, 2
    /* DE3730 00012CC4 0C000000 */  jal        func_00000000
    /* DE3734 00012CC8 00000000 */   nop
    /* DE3738 00012CCC 8FA80030 */  lw         $t0, 0x30($sp)
    /* DE373C 00012CD0 8D09014C */  lw         $t1, 0x14C($t0)
    /* DE3740 00012CD4 8D240008 */  lw         $a0, 0x8($t1)
    /* DE3744 00012CD8 8FA50034 */  lw         $a1, 0x34($sp)
    /* DE3748 00012CDC 91060178 */  lbu        $a2, 0x178($t0)
    /* DE374C 00012CE0 91070179 */  lbu        $a3, 0x179($t0)
    /* DE3750 00012CE4 910A017A */  lbu        $t2, 0x17A($t0)
    /* DE3754 00012CE8 AFAA0010 */  sw         $t2, 0x10($sp)
    /* DE3758 00012CEC 910B017B */  lbu        $t3, 0x17B($t0)
    /* DE375C 00012CF0 AFAB0014 */  sw         $t3, 0x14($sp)
    /* DE3760 00012CF4 8FAC002C */  lw         $t4, 0x2C($sp)
    /* DE3764 00012CF8 AFAC0018 */  sw         $t4, 0x18($sp)
    /* DE3768 00012CFC 8D0D0170 */  lw         $t5, 0x170($t0)
    /* DE376C 00012D00 AFAD001C */  sw         $t5, 0x1C($sp)
    /* DE3770 00012D04 0C000000 */  jal        func_00000000
    /* DE3774 00012D08 00000000 */   nop
    /* DE3778 00012D0C 8FAE0030 */  lw         $t6, 0x30($sp)
    /* DE377C 00012D10 8DC40148 */  lw         $a0, 0x148($t6)
    /* DE3780 00012D14 8DCF014C */  lw         $t7, 0x14C($t6)
    /* DE3784 00012D18 8DE50008 */  lw         $a1, 0x8($t7)
    /* DE3788 00012D1C 0C000000 */  jal        func_00000000
    /* DE378C 00012D20 00000000 */   nop
    /* DE3790 00012D24 8FB80030 */  lw         $t8, 0x30($sp)
    /* DE3794 00012D28 8F04014C */  lw         $a0, 0x14C($t8)
    /* DE3798 00012D2C 24050003 */  addiu      $a1, $zero, 0x3
    /* DE379C 00012D30 0C000000 */  jal        func_00000000
    /* DE37A0 00012D34 00000000 */   nop
    /* DE37A4 00012D38 8FB90030 */  lw         $t9, 0x30($sp)
    /* DE37A8 00012D3C AF200184 */  sw         $zero, 0x184($t9)
    /* DE37AC 00012D40 2409FFFF */  addiu      $t1, $zero, -0x1
    /* DE37B0 00012D44 8FAA0030 */  lw         $t2, 0x30($sp)
    /* DE37B4 00012D48 AD49006C */  sw         $t1, 0x6C($t2)
    /* DE37B8 00012D4C 10000001 */  b          .L00012D54
    /* DE37BC 00012D50 00000000 */   nop
  .L00012D54:
    /* DE37C0 00012D54 8FBF0024 */  lw         $ra, 0x24($sp)
    /* DE37C4 00012D58 27BD0030 */  addiu      $sp, $sp, 0x30
    /* DE37C8 00012D5C 03E00008 */  jr         $ra
    /* DE37CC 00012D60 00000000 */   nop
endlabel replay_saveGhost
