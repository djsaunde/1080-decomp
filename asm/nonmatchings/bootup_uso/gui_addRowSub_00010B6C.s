nonmatching gui_addRowSub_00010B6C, 0x120

glabel gui_addRowSub_00010B6C
    /* DE15D8 00010B6C 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DE15DC 00010B70 AFBF001C */  sw         $ra, 0x1C($sp)
    /* DE15E0 00010B74 AFA40028 */  sw         $a0, 0x28($sp)
    /* DE15E4 00010B78 AFA5002C */  sw         $a1, 0x2C($sp)
    /* DE15E8 00010B7C AFA60030 */  sw         $a2, 0x30($sp)
    /* DE15EC 00010B80 AFA70034 */  sw         $a3, 0x34($sp)
    /* DE15F0 00010B84 AFB00018 */  sw         $s0, 0x18($sp)
    /* DE15F4 00010B88 240E0001 */  addiu      $t6, $zero, 0x1
    /* DE15F8 00010B8C 8FAF0028 */  lw         $t7, 0x28($sp)
    /* DE15FC 00010B90 8DF80078 */  lw         $t8, 0x78($t7)
    /* DE1600 00010B94 0018C880 */  sll        $t9, $t8, 2
    /* DE1604 00010B98 0338C821 */  addu       $t9, $t9, $t8
    /* DE1608 00010B9C 0019C8C0 */  sll        $t9, $t9, 3
    /* DE160C 00010BA0 01F94021 */  addu       $t0, $t7, $t9
    /* DE1610 00010BA4 AD0E0088 */  sw         $t6, 0x88($t0)
    /* DE1614 00010BA8 8FA90030 */  lw         $t1, 0x30($sp)
    /* DE1618 00010BAC 2401FFFF */  addiu      $at, $zero, -0x1
    /* DE161C 00010BB0 1121000B */  beq        $t1, $at, .L00010BE0
    /* DE1620 00010BB4 00000000 */   nop
    /* DE1624 00010BB8 8FAA0030 */  lw         $t2, 0x30($sp)
    /* DE1628 00010BBC 8FAB0028 */  lw         $t3, 0x28($sp)
    /* DE162C 00010BC0 8D6C0078 */  lw         $t4, 0x78($t3)
    /* DE1630 00010BC4 000C6880 */  sll        $t5, $t4, 2
    /* DE1634 00010BC8 01AC6821 */  addu       $t5, $t5, $t4
    /* DE1638 00010BCC 000D68C0 */  sll        $t5, $t5, 3
    /* DE163C 00010BD0 016DC021 */  addu       $t8, $t3, $t5
    /* DE1640 00010BD4 AF0A008C */  sw         $t2, 0x8C($t8)
    /* DE1644 00010BD8 10000008 */  b          .L00010BFC
    /* DE1648 00010BDC 00000000 */   nop
  .L00010BE0:
    /* DE164C 00010BE0 8FAF0028 */  lw         $t7, 0x28($sp)
    /* DE1650 00010BE4 8DF90078 */  lw         $t9, 0x78($t7)
    /* DE1654 00010BE8 00197080 */  sll        $t6, $t9, 2
    /* DE1658 00010BEC 01D97021 */  addu       $t6, $t6, $t9
    /* DE165C 00010BF0 000E70C0 */  sll        $t6, $t6, 3
    /* DE1660 00010BF4 01EE4021 */  addu       $t0, $t7, $t6
    /* DE1664 00010BF8 AD19008C */  sw         $t9, 0x8C($t0)
  .L00010BFC:
    /* DE1668 00010BFC 8FA90028 */  lw         $t1, 0x28($sp)
    /* DE166C 00010C00 8D2C0078 */  lw         $t4, 0x78($t1)
    /* DE1670 00010C04 000C5880 */  sll        $t3, $t4, 2
    /* DE1674 00010C08 016C5821 */  addu       $t3, $t3, $t4
    /* DE1678 00010C0C 000B58C0 */  sll        $t3, $t3, 3
    /* DE167C 00010C10 012B6821 */  addu       $t5, $t1, $t3
    /* DE1680 00010C14 ADA00084 */  sw         $zero, 0x84($t5)
    /* DE1684 00010C18 8FAA0028 */  lw         $t2, 0x28($sp)
    /* DE1688 00010C1C 8D580078 */  lw         $t8, 0x78($t2)
    /* DE168C 00010C20 00187880 */  sll        $t7, $t8, 2
    /* DE1690 00010C24 01F87821 */  addu       $t7, $t7, $t8
    /* DE1694 00010C28 000F78C0 */  sll        $t7, $t7, 3
    /* DE1698 00010C2C 014F2021 */  addu       $a0, $t2, $t7
    /* DE169C 00010C30 24840094 */  addiu      $a0, $a0, 0x94
    /* DE16A0 00010C34 8FA5002C */  lw         $a1, 0x2C($sp)
    /* DE16A4 00010C38 0C000000 */  jal        func_00000000
    /* DE16A8 00010C3C 00000000 */   nop
    /* DE16AC 00010C40 8FAE0034 */  lw         $t6, 0x34($sp)
    /* DE16B0 00010C44 8FB90028 */  lw         $t9, 0x28($sp)
    /* DE16B4 00010C48 8F280078 */  lw         $t0, 0x78($t9)
    /* DE16B8 00010C4C 00086080 */  sll        $t4, $t0, 2
    /* DE16BC 00010C50 01886021 */  addu       $t4, $t4, $t0
    /* DE16C0 00010C54 000C60C0 */  sll        $t4, $t4, 3
    /* DE16C4 00010C58 032C4821 */  addu       $t1, $t9, $t4
    /* DE16C8 00010C5C AD2E0090 */  sw         $t6, 0x90($t1)
    /* DE16CC 00010C60 8FB00028 */  lw         $s0, 0x28($sp)
    /* DE16D0 00010C64 8E0B0078 */  lw         $t3, 0x78($s0)
    /* DE16D4 00010C68 256D0001 */  addiu      $t5, $t3, 0x1
    /* DE16D8 00010C6C AE0D0078 */  sw         $t5, 0x78($s0)
    /* DE16DC 00010C70 10000001 */  b          .L00010C78
    /* DE16E0 00010C74 00000000 */   nop
  .L00010C78:
    /* DE16E4 00010C78 8FB00018 */  lw         $s0, 0x18($sp)
    /* DE16E8 00010C7C 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DE16EC 00010C80 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DE16F0 00010C84 03E00008 */  jr         $ra
    /* DE16F4 00010C88 00000000 */   nop
endlabel gui_addRowSub_00010B6C
