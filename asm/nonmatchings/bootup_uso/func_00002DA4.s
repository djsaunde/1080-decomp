nonmatching func_00002DA4, 0x1EC

glabel func_00002DA4
    /* DD3810 00002DA4 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD3814 00002DA8 00803825 */  or         $a3, $a0, $zero
    /* DD3818 00002DAC AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD381C 00002DB0 AFA50024 */  sw         $a1, 0x24($sp)
    /* DD3820 00002DB4 3C040000 */  lui        $a0, %hi(D_000073EC)
    /* DD3824 00002DB8 248473EC */  addiu      $a0, $a0, %lo(D_000073EC)
    /* DD3828 00002DBC 00002825 */  or         $a1, $zero, $zero
    /* DD382C 00002DC0 AFA70020 */  sw         $a3, 0x20($sp)
    /* DD3830 00002DC4 0C000000 */  jal        func_00000000
    /* DD3834 00002DC8 AFA60028 */   sw        $a2, 0x28($sp)
    /* DD3838 00002DCC 0C000000 */  jal        func_00000000
    /* DD383C 00002DD0 00000000 */   nop
    /* DD3840 00002DD4 3C050000 */  lui        $a1, %hi(D_000073F8)
    /* DD3844 00002DD8 24A573F8 */  addiu      $a1, $a1, %lo(D_000073F8)
    /* DD3848 00002DDC 0C000000 */  jal        func_00000000
    /* DD384C 00002DE0 00002025 */   or        $a0, $zero, $zero
    /* DD3850 00002DE4 8FA70020 */  lw         $a3, 0x20($sp)
    /* DD3854 00002DE8 3C010000 */  lui        $at, (0x0 >> 16)
    /* DD3858 00002DEC AC220000 */  sw         $v0, 0x0($at)
    /* DD385C 00002DF0 8CE30084 */  lw         $v1, 0x84($a3)
    /* DD3860 00002DF4 3C050000 */  lui        $a1, (0x0 >> 16)
    /* DD3864 00002DF8 8CA50000 */  lw         $a1, 0x0($a1)
    /* DD3868 00002DFC 24640010 */  addiu      $a0, $v1, 0x10
    /* DD386C 00002E00 AFA30018 */  sw         $v1, 0x18($sp)
    /* DD3870 00002E04 0C000000 */  jal        func_00000000
    /* DD3874 00002E08 AFA5001C */   sw        $a1, 0x1C($sp)
    /* DD3878 00002E0C 8FA5001C */  lw         $a1, 0x1C($sp)
    /* DD387C 00002E10 8FA30018 */  lw         $v1, 0x18($sp)
    /* DD3880 00002E14 8FA60028 */  lw         $a2, 0x28($sp)
    /* DD3884 00002E18 8CAE0014 */  lw         $t6, 0x14($a1)
    /* DD3888 00002E1C 8FA70020 */  lw         $a3, 0x20($sp)
    /* DD388C 00002E20 240F0001 */  addiu      $t7, $zero, 0x1
    /* DD3890 00002E24 11C00002 */  beqz       $t6, .L00002E30
    /* DD3894 00002E28 3C040000 */   lui       $a0, (0x0 >> 16)
    /* DD3898 00002E2C ACAF0004 */  sw         $t7, 0x4($a1)
  .L00002E30:
    /* DD389C 00002E30 ACA30014 */  sw         $v1, 0x14($a1)
    /* DD38A0 00002E34 AFA70020 */  sw         $a3, 0x20($sp)
    /* DD38A4 00002E38 AFA60028 */  sw         $a2, 0x28($sp)
    /* DD38A8 00002E3C 0C000000 */  jal        func_00000000
    /* DD38AC 00002E40 8C840000 */   lw        $a0, 0x0($a0)
    /* DD38B0 00002E44 3C020000 */  lui        $v0, %hi(func_00000000 + 0x4)
    /* DD38B4 00002E48 8C420004 */  lw         $v0, %lo(func_00000000 + 0x4)($v0)
    /* DD38B8 00002E4C 3C010008 */  lui        $at, (0x82000 >> 16)
    /* DD38BC 00002E50 8FB80024 */  lw         $t8, 0x24($sp)
    /* DD38C0 00002E54 00411025 */  or         $v0, $v0, $at
    /* DD38C4 00002E58 34422000 */  ori        $v0, $v0, (0x82000 & 0xFFFF)
    /* DD38C8 00002E5C 3C010002 */  lui        $at, (0x20000 >> 16)
    /* DD38CC 00002E60 00411025 */  or         $v0, $v0, $at
    /* DD38D0 00002E64 3C010000 */  lui        $at, %hi(func_00000000 + 0x4)
    /* DD38D4 00002E68 AC220004 */  sw         $v0, %lo(func_00000000 + 0x4)($at)
    /* DD38D8 00002E6C 2719FFFF */  addiu      $t9, $t8, -0x1
    /* DD38DC 00002E70 2F21000C */  sltiu      $at, $t9, 0xC
    /* DD38E0 00002E74 8FA60028 */  lw         $a2, 0x28($sp)
    /* DD38E4 00002E78 1020003E */  beqz       $at, .L00002F74
    /* DD38E8 00002E7C 8FA70020 */   lw        $a3, 0x20($sp)
    /* DD38EC 00002E80 0019C880 */  sll        $t9, $t9, 2
    /* DD38F0 00002E84 3C010000 */  lui        $at, %hi(func_000003F8 + 0xFC)
    /* DD38F4 00002E88 00390821 */  addu       $at, $at, $t9
    /* DD38F8 00002E8C 8C3904F4 */  lw         $t9, %lo(func_000003F8 + 0xFC)($at)
    /* DD38FC 00002E90 03200008 */  jr         $t9
    /* DD3900 00002E94 00000000 */   nop
    /* DD3904 00002E98 00E02025 */  or         $a0, $a3, $zero
    /* DD3908 00002E9C 0C000000 */  jal        func_00000000
    /* DD390C 00002EA0 00C02825 */   or        $a1, $a2, $zero
    /* DD3910 00002EA4 10000037 */  b          .L00002F84
    /* DD3914 00002EA8 8FBF0014 */   lw        $ra, 0x14($sp)
    /* DD3918 00002EAC 00E02025 */  or         $a0, $a3, $zero
    /* DD391C 00002EB0 0C000000 */  jal        func_00000000
    /* DD3920 00002EB4 00C02825 */   or        $a1, $a2, $zero
    /* DD3924 00002EB8 10000032 */  b          .L00002F84
    /* DD3928 00002EBC 8FBF0014 */   lw        $ra, 0x14($sp)
    /* DD392C 00002EC0 00E02025 */  or         $a0, $a3, $zero
    /* DD3930 00002EC4 0C000000 */  jal        func_00000000
    /* DD3934 00002EC8 00C02825 */   or        $a1, $a2, $zero
    /* DD3938 00002ECC 1000002D */  b          .L00002F84
    /* DD393C 00002ED0 8FBF0014 */   lw        $ra, 0x14($sp)
    /* DD3940 00002ED4 00E02025 */  or         $a0, $a3, $zero
    /* DD3944 00002ED8 0C000000 */  jal        func_00000000
    /* DD3948 00002EDC 00C02825 */   or        $a1, $a2, $zero
    /* DD394C 00002EE0 10000028 */  b          .L00002F84
    /* DD3950 00002EE4 8FBF0014 */   lw        $ra, 0x14($sp)
    /* DD3954 00002EE8 00E02025 */  or         $a0, $a3, $zero
    /* DD3958 00002EEC 0C000000 */  jal        func_00000000
    /* DD395C 00002EF0 00C02825 */   or        $a1, $a2, $zero
    /* DD3960 00002EF4 10000023 */  b          .L00002F84
    /* DD3964 00002EF8 8FBF0014 */   lw        $ra, 0x14($sp)
    /* DD3968 00002EFC 00E02025 */  or         $a0, $a3, $zero
    /* DD396C 00002F00 0C000000 */  jal        func_00000000
    /* DD3970 00002F04 00C02825 */   or        $a1, $a2, $zero
    /* DD3974 00002F08 1000001E */  b          .L00002F84
    /* DD3978 00002F0C 8FBF0014 */   lw        $ra, 0x14($sp)
    /* DD397C 00002F10 00E02025 */  or         $a0, $a3, $zero
    /* DD3980 00002F14 0C000000 */  jal        func_00000000
    /* DD3984 00002F18 00002825 */   or        $a1, $zero, $zero
    /* DD3988 00002F1C 3C050000 */  lui        $a1, (0x0 >> 16)
    /* DD398C 00002F20 8CA50000 */  lw         $a1, 0x0($a1)
    /* DD3990 00002F24 8CA80018 */  lw         $t0, 0x18($a1)
    /* DD3994 00002F28 24A50018 */  addiu      $a1, $a1, 0x18
    /* DD3998 00002F2C 35090004 */  ori        $t1, $t0, 0x4
    /* DD399C 00002F30 10000013 */  b          .L00002F80
    /* DD39A0 00002F34 ACA90000 */   sw        $t1, 0x0($a1)
    /* DD39A4 00002F38 00E02025 */  or         $a0, $a3, $zero
    /* DD39A8 00002F3C 0C000000 */  jal        func_00000000
    /* DD39AC 00002F40 00C02825 */   or        $a1, $a2, $zero
    /* DD39B0 00002F44 1000000F */  b          .L00002F84
    /* DD39B4 00002F48 8FBF0014 */   lw        $ra, 0x14($sp)
    /* DD39B8 00002F4C 00E02025 */  or         $a0, $a3, $zero
    /* DD39BC 00002F50 0C000000 */  jal        func_00000000
    /* DD39C0 00002F54 00C02825 */   or        $a1, $a2, $zero
    /* DD39C4 00002F58 1000000A */  b          .L00002F84
    /* DD39C8 00002F5C 8FBF0014 */   lw        $ra, 0x14($sp)
    /* DD39CC 00002F60 00E02025 */  or         $a0, $a3, $zero
    /* DD39D0 00002F64 0C000000 */  jal        func_00000000
    /* DD39D4 00002F68 00C02825 */   or        $a1, $a2, $zero
    /* DD39D8 00002F6C 10000005 */  b          .L00002F84
    /* DD39DC 00002F70 8FBF0014 */   lw        $ra, 0x14($sp)
  .L00002F74:
    /* DD39E0 00002F74 3C040000 */  lui        $a0, %hi(D_00007408)
    /* DD39E4 00002F78 0C000000 */  jal        func_00000000
    /* DD39E8 00002F7C 24847408 */   addiu     $a0, $a0, %lo(D_00007408)
  .L00002F80:
    /* DD39EC 00002F80 8FBF0014 */  lw         $ra, 0x14($sp)
  .L00002F84:
    /* DD39F0 00002F84 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD39F4 00002F88 03E00008 */  jr         $ra
    /* DD39F8 00002F8C 00000000 */   nop
endlabel func_00002DA4
