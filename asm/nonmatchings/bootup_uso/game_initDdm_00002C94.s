nonmatching game_initDdm_00002C94, 0x110

glabel game_initDdm_00002C94
    /* DD3700 00002C94 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DD3704 00002C98 3C010000 */  lui        $at, %hi(func_000003F8 + 0xF8)
    /* DD3708 00002C9C C42004F0 */  lwc1       $f0, %lo(func_000003F8 + 0xF8)($at)
    /* DD370C 00002CA0 AFB00018 */  sw         $s0, 0x18($sp)
    /* DD3710 00002CA4 3C100000 */  lui        $s0, %hi(func_000000F0 + 0x38)
    /* DD3714 00002CA8 AFB1001C */  sw         $s1, 0x1C($sp)
    /* DD3718 00002CAC 3C110000 */  lui        $s1, %hi(func_00000080 + 0x68)
    /* DD371C 00002CB0 26100128 */  addiu      $s0, $s0, %lo(func_000000F0 + 0x38)
    /* DD3720 00002CB4 263100E8 */  addiu      $s1, $s1, %lo(func_00000080 + 0x68)
    /* DD3724 00002CB8 AFBF0024 */  sw         $ra, 0x24($sp)
    /* DD3728 00002CBC AFA40028 */  sw         $a0, 0x28($sp)
    /* DD372C 00002CC0 AFB20020 */  sw         $s2, 0x20($sp)
    /* DD3730 00002CC4 AFA5002C */  sw         $a1, 0x2C($sp)
    /* DD3734 00002CC8 02202025 */  or         $a0, $s1, $zero
    /* DD3738 00002CCC E6000000 */  swc1       $f0, 0x0($s0)
    /* DD373C 00002CD0 E6000004 */  swc1       $f0, 0x4($s0)
    /* DD3740 00002CD4 0C000000 */  jal        func_00000000
    /* DD3744 00002CD8 E6000008 */   swc1      $f0, 0x8($s0)
    /* DD3748 00002CDC 02202025 */  or         $a0, $s1, $zero
    /* DD374C 00002CE0 0C000000 */  jal        func_00000000
    /* DD3750 00002CE4 02002825 */   or        $a1, $s0, $zero
    /* DD3754 00002CE8 3C0F0000 */  lui        $t7, (0x0 >> 16)
    /* DD3758 00002CEC 8DEF0000 */  lw         $t7, 0x0($t7)
    /* DD375C 00002CF0 240E0006 */  addiu      $t6, $zero, 0x6
    /* DD3760 00002CF4 3C010000 */  lui        $at, (0x0 >> 16)
    /* DD3764 00002CF8 AC2E0000 */  sw         $t6, 0x0($at)
    /* DD3768 00002CFC 2401FFF7 */  addiu      $at, $zero, -0x9
    /* DD376C 00002D00 3C020000 */  lui        $v0, %hi(func_00000000 + 0x4)
    /* DD3770 00002D04 01E1C024 */  and        $t8, $t7, $at
    /* DD3774 00002D08 8C420004 */  lw         $v0, %lo(func_00000000 + 0x4)($v0)
    /* DD3778 00002D0C 3C010000 */  lui        $at, (0x0 >> 16)
    /* DD377C 00002D10 AC380000 */  sw         $t8, 0x0($at)
    /* DD3780 00002D14 2401FFFD */  addiu      $at, $zero, -0x3
    /* DD3784 00002D18 00411024 */  and        $v0, $v0, $at
    /* DD3788 00002D1C 34422000 */  ori        $v0, $v0, 0x2000
    /* DD378C 00002D20 3C010008 */  lui        $at, (0x80000 >> 16)
    /* DD3790 00002D24 00411025 */  or         $v0, $v0, $at
    /* DD3794 00002D28 3C010000 */  lui        $at, %hi(func_00000000 + 0x4)
    /* DD3798 00002D2C 3C100000 */  lui        $s0, (0x0 >> 16)
    /* DD379C 00002D30 3C040000 */  lui        $a0, %hi(D_000073E8)
    /* DD37A0 00002D34 AC220004 */  sw         $v0, %lo(func_00000000 + 0x4)($at)
    /* DD37A4 00002D38 26100000 */  addiu      $s0, $s0, 0x0
    /* DD37A8 00002D3C 248473E8 */  addiu      $a0, $a0, %lo(D_000073E8)
    /* DD37AC 00002D40 0C000000 */  jal        func_00000000
    /* DD37B0 00002D44 00002825 */   or        $a1, $zero, $zero
    /* DD37B4 00002D48 3C190000 */  lui        $t9, (0x0 >> 16)
    /* DD37B8 00002D4C 8F390000 */  lw         $t9, 0x0($t9)
    /* DD37BC 00002D50 3C120000 */  lui        $s2, (0x0 >> 16)
    /* DD37C0 00002D54 26520000 */  addiu      $s2, $s2, 0x0
    /* DD37C4 00002D58 1320000A */  beqz       $t9, .L00002D84
    /* DD37C8 00002D5C 00000000 */   nop
    /* DD37CC 00002D60 8E110000 */  lw         $s1, 0x0($s0)
    /* DD37D0 00002D64 02402025 */  or         $a0, $s2, $zero
  .L00002D68:
    /* DD37D4 00002D68 02202825 */  or         $a1, $s1, $zero
    /* DD37D8 00002D6C 0C000000 */  jal        func_00000000
    /* DD37DC 00002D70 8E060004 */   lw        $a2, 0x4($s0)
    /* DD37E0 00002D74 8E110008 */  lw         $s1, 0x8($s0)
    /* DD37E4 00002D78 26100008 */  addiu      $s0, $s0, 0x8
    /* DD37E8 00002D7C 5620FFFA */  bnel       $s1, $zero, .L00002D68
    /* DD37EC 00002D80 02402025 */   or        $a0, $s2, $zero
  .L00002D84:
    /* DD37F0 00002D84 0C000000 */  jal        func_00000000
    /* DD37F4 00002D88 00000000 */   nop
    /* DD37F8 00002D8C 8FBF0024 */  lw         $ra, 0x24($sp)
    /* DD37FC 00002D90 8FB00018 */  lw         $s0, 0x18($sp)
    /* DD3800 00002D94 8FB1001C */  lw         $s1, 0x1C($sp)
    /* DD3804 00002D98 8FB20020 */  lw         $s2, 0x20($sp)
    /* DD3808 00002D9C 03E00008 */  jr         $ra
    /* DD380C 00002DA0 27BD0028 */   addiu     $sp, $sp, 0x28
endlabel game_initDdm_00002C94
