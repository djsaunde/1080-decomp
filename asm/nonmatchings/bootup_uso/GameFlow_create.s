nonmatching GameFlow_create, 0x360

glabel GameFlow_create
    /* DE16F8 00010C8C 27BDFF78 */  addiu      $sp, $sp, -0x88
    /* DE16FC 00010C90 AFBF0054 */  sw         $ra, 0x54($sp)
    /* DE1700 00010C94 AFA40088 */  sw         $a0, 0x88($sp)
    /* DE1704 00010C98 AFA5008C */  sw         $a1, 0x8C($sp)
    /* DE1708 00010C9C AFB10050 */  sw         $s1, 0x50($sp)
    /* DE170C 00010CA0 AFB0004C */  sw         $s0, 0x4C($sp)
    /* DE1710 00010CA4 F7BA0040 */  sdc1       $f26, 0x40($sp)
    /* DE1714 00010CA8 F7B80038 */  sdc1       $f24, 0x38($sp)
    /* DE1718 00010CAC F7B60030 */  sdc1       $f22, 0x30($sp)
    /* DE171C 00010CB0 F7B40028 */  sdc1       $f20, 0x28($sp)
    /* DE1720 00010CB4 8FB00088 */  lw         $s0, 0x88($sp)
    /* DE1724 00010CB8 3C110001 */  lui        $s1, %hi(sStr_GameFlow_C5D0)
    /* DE1728 00010CBC 2631C5D0 */  addiu      $s1, $s1, %lo(sStr_GameFlow_C5D0)
    /* DE172C 00010CC0 AE11000C */  sw         $s1, 0xC($s0)
    /* DE1730 00010CC4 8FAE008C */  lw         $t6, 0x8C($sp)
    /* DE1734 00010CC8 8FAF0088 */  lw         $t7, 0x88($sp)
    /* DE1738 00010CCC ADEE012C */  sw         $t6, 0x12C($t7)
    /* DE173C 00010CD0 00002025 */  or         $a0, $zero, $zero
    /* DE1740 00010CD4 0C000000 */  jal        func_00000000
    /* DE1744 00010CD8 00000000 */   nop
    /* DE1748 00010CDC AFA20068 */  sw         $v0, 0x68($sp)
    /* DE174C 00010CE0 27B0006C */  addiu      $s0, $sp, 0x6C
    /* DE1750 00010CE4 16000007 */  bnez       $s0, .L00010D04
    /* DE1754 00010CE8 00000000 */   nop
    /* DE1758 00010CEC 24040010 */  addiu      $a0, $zero, 0x10
    /* DE175C 00010CF0 0C000000 */  jal        func_00000000
    /* DE1760 00010CF4 00000000 */   nop
    /* DE1764 00010CF8 00408025 */  or         $s0, $v0, $zero
    /* DE1768 00010CFC 1200000B */  beqz       $s0, .L00010D2C
    /* DE176C 00010D00 00000000 */   nop
  .L00010D04:
    /* DE1770 00010D04 02008825 */  or         $s1, $s0, $zero
    /* DE1774 00010D08 4480A000 */  mtc1       $zero, $f20
    /* DE1778 00010D0C 4480B000 */  mtc1       $zero, $f22
    /* DE177C 00010D10 4480C000 */  mtc1       $zero, $f24
    /* DE1780 00010D14 3C010000 */  lui        $at, %hi(ColourItem_create_00000C10 + 0xC)
    /* DE1784 00010D18 C43A0C1C */  lwc1       $f26, %lo(ColourItem_create_00000C10 + 0xC)($at)
    /* DE1788 00010D1C E6340000 */  swc1       $f20, 0x0($s1)
    /* DE178C 00010D20 E6360004 */  swc1       $f22, 0x4($s1)
    /* DE1790 00010D24 E6380008 */  swc1       $f24, 0x8($s1)
    /* DE1794 00010D28 E63A000C */  swc1       $f26, 0xC($s1)
  .L00010D2C:
    /* DE1798 00010D2C 00002025 */  or         $a0, $zero, $zero
    /* DE179C 00010D30 3C050001 */  lui        $a1, %hi(sStr_Flow_C5DC)
    /* DE17A0 00010D34 24A5C5DC */  addiu      $a1, $a1, %lo(sStr_Flow_C5DC)
    /* DE17A4 00010D38 24060010 */  addiu      $a2, $zero, 0x10
    /* DE17A8 00010D3C 2407012C */  addiu      $a3, $zero, 0x12C
    /* DE17AC 00010D40 24180010 */  addiu      $t8, $zero, 0x10
    /* DE17B0 00010D44 AFB80010 */  sw         $t8, 0x10($sp)
    /* DE17B4 00010D48 24190014 */  addiu      $t9, $zero, 0x14
    /* DE17B8 00010D4C AFB90014 */  sw         $t9, 0x14($sp)
    /* DE17BC 00010D50 27A8006C */  addiu      $t0, $sp, 0x6C
    /* DE17C0 00010D54 8D0A0000 */  lw         $t2, 0x0($t0)
    /* DE17C4 00010D58 AFAA0018 */  sw         $t2, 0x18($sp)
    /* DE17C8 00010D5C 8D090004 */  lw         $t1, 0x4($t0)
    /* DE17CC 00010D60 AFA9001C */  sw         $t1, 0x1C($sp)
    /* DE17D0 00010D64 8D0A0008 */  lw         $t2, 0x8($t0)
    /* DE17D4 00010D68 AFAA0020 */  sw         $t2, 0x20($sp)
    /* DE17D8 00010D6C 8D09000C */  lw         $t1, 0xC($t0)
    /* DE17DC 00010D70 AFA90024 */  sw         $t1, 0x24($sp)
    /* DE17E0 00010D74 0C000000 */  jal        func_00000000
    /* DE17E4 00010D78 00000000 */   nop
    /* DE17E8 00010D7C 3C010000 */  lui        $at, %hi(stream_readQuad_000000F0)
    /* DE17EC 00010D80 AC2200F0 */  sw         $v0, %lo(stream_readQuad_000000F0)($at)
    /* DE17F0 00010D84 8FA40068 */  lw         $a0, 0x68($sp)
    /* DE17F4 00010D88 0C000000 */  jal        func_00000000
    /* DE17F8 00010D8C 00000000 */   nop
    /* DE17FC 00010D90 8FAB0088 */  lw         $t3, 0x88($sp)
    /* DE1800 00010D94 AD600130 */  sw         $zero, 0x130($t3)
    /* DE1804 00010D98 8FAC0088 */  lw         $t4, 0x88($sp)
    /* DE1808 00010D9C AD80002C */  sw         $zero, 0x2C($t4)
    /* DE180C 00010DA0 240D0009 */  addiu      $t5, $zero, 0x9
    /* DE1810 00010DA4 8FAE0088 */  lw         $t6, 0x88($sp)
    /* DE1814 00010DA8 ADCD0038 */  sw         $t5, 0x38($t6)
    /* DE1818 00010DAC 240FFFFF */  addiu      $t7, $zero, -0x1
    /* DE181C 00010DB0 8FB80088 */  lw         $t8, 0x88($sp)
    /* DE1820 00010DB4 AF0F006C */  sw         $t7, 0x6C($t8)
    /* DE1824 00010DB8 8FB90088 */  lw         $t9, 0x88($sp)
    /* DE1828 00010DBC AF200070 */  sw         $zero, 0x70($t9)
    /* DE182C 00010DC0 8FA80088 */  lw         $t0, 0x88($sp)
    /* DE1830 00010DC4 AD00013C */  sw         $zero, 0x13C($t0)
    /* DE1834 00010DC8 00002025 */  or         $a0, $zero, $zero
    /* DE1838 00010DCC 0C000000 */  jal        func_00000000
    /* DE183C 00010DD0 00000000 */   nop
    /* DE1840 00010DD4 AFA20084 */  sw         $v0, 0x84($sp)
    /* DE1844 00010DD8 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DE1848 00010DDC 24840000 */  addiu      $a0, $a0, 0x0
    /* DE184C 00010DE0 3C050001 */  lui        $a1, %hi(sStr_rom_gui_uso_C5E4)
    /* DE1850 00010DE4 24A5C5E4 */  addiu      $a1, $a1, %lo(sStr_rom_gui_uso_C5E4)
    /* DE1854 00010DE8 0C000000 */  jal        func_00000000
    /* DE1858 00010DEC 00000000 */   nop
    /* DE185C 00010DF0 00008025 */  or         $s0, $zero, $zero
    /* DE1860 00010DF4 16000007 */  bnez       $s0, .L00010E14
    /* DE1864 00010DF8 00000000 */   nop
    /* DE1868 00010DFC 24040080 */  addiu      $a0, $zero, 0x80
    /* DE186C 00010E00 0C000000 */  jal        func_00000000
    /* DE1870 00010E04 00000000 */   nop
    /* DE1874 00010E08 00408025 */  or         $s0, $v0, $zero
    /* DE1878 00010E0C 12000005 */  beqz       $s0, .L00010E24
    /* DE187C 00010E10 00000000 */   nop
  .L00010E14:
    /* DE1880 00010E14 02002025 */  or         $a0, $s0, $zero
    /* DE1884 00010E18 24050001 */  addiu      $a1, $zero, 0x1
    /* DE1888 00010E1C 0C000000 */  jal        func_00000000
    /* DE188C 00010E20 00000000 */   nop
  .L00010E24:
    /* DE1890 00010E24 8FA90088 */  lw         $t1, 0x88($sp)
    /* DE1894 00010E28 AD3000C0 */  sw         $s0, 0xC0($t1)
    /* DE1898 00010E2C 00008025 */  or         $s0, $zero, $zero
    /* DE189C 00010E30 16000007 */  bnez       $s0, .L00010E50
    /* DE18A0 00010E34 00000000 */   nop
    /* DE18A4 00010E38 24040080 */  addiu      $a0, $zero, 0x80
    /* DE18A8 00010E3C 0C000000 */  jal        func_00000000
    /* DE18AC 00010E40 00000000 */   nop
    /* DE18B0 00010E44 00408025 */  or         $s0, $v0, $zero
    /* DE18B4 00010E48 12000005 */  beqz       $s0, .L00010E60
    /* DE18B8 00010E4C 00000000 */   nop
  .L00010E50:
    /* DE18BC 00010E50 02002025 */  or         $a0, $s0, $zero
    /* DE18C0 00010E54 24050002 */  addiu      $a1, $zero, 0x2
    /* DE18C4 00010E58 0C000000 */  jal        func_00000000
    /* DE18C8 00010E5C 00000000 */   nop
  .L00010E60:
    /* DE18CC 00010E60 8FAA0088 */  lw         $t2, 0x88($sp)
    /* DE18D0 00010E64 AD5000C4 */  sw         $s0, 0xC4($t2)
    /* DE18D4 00010E68 8FA40088 */  lw         $a0, 0x88($sp)
    /* DE18D8 00010E6C 0C000000 */  jal        func_00000000
    /* DE18DC 00010E70 00000000 */   nop
    /* DE18E0 00010E74 8FAB0088 */  lw         $t3, 0x88($sp)
    /* DE18E4 00010E78 AD600084 */  sw         $zero, 0x84($t3)
    /* DE18E8 00010E7C 240C0001 */  addiu      $t4, $zero, 0x1
    /* DE18EC 00010E80 8FAD0088 */  lw         $t5, 0x88($sp)
    /* DE18F0 00010E84 ADAC0088 */  sw         $t4, 0x88($t5)
    /* DE18F4 00010E88 8FAE0088 */  lw         $t6, 0x88($sp)
    /* DE18F8 00010E8C ADC0018C */  sw         $zero, 0x18C($t6)
    /* DE18FC 00010E90 00002025 */  or         $a0, $zero, $zero
    /* DE1900 00010E94 0C000000 */  jal        func_00000000
    /* DE1904 00010E98 00000000 */   nop
    /* DE1908 00010E9C 8FAF0088 */  lw         $t7, 0x88($sp)
    /* DE190C 00010EA0 ADE20134 */  sw         $v0, 0x134($t7)
    /* DE1910 00010EA4 00002025 */  or         $a0, $zero, $zero
    /* DE1914 00010EA8 00002825 */  or         $a1, $zero, $zero
    /* DE1918 00010EAC 0C000000 */  jal        func_00000000
    /* DE191C 00010EB0 00000000 */   nop
    /* DE1920 00010EB4 8FB80088 */  lw         $t8, 0x88($sp)
    /* DE1924 00010EB8 AF020190 */  sw         $v0, 0x190($t8)
    /* DE1928 00010EBC 00002025 */  or         $a0, $zero, $zero
    /* DE192C 00010EC0 0C000000 */  jal        func_00000000
    /* DE1930 00010EC4 00000000 */   nop
    /* DE1934 00010EC8 8FB90088 */  lw         $t9, 0x88($sp)
    /* DE1938 00010ECC AF220148 */  sw         $v0, 0x148($t9)
    /* DE193C 00010ED0 8FA80088 */  lw         $t0, 0x88($sp)
    /* DE1940 00010ED4 8D040148 */  lw         $a0, 0x148($t0)
    /* DE1944 00010ED8 0C000000 */  jal        func_00000000
    /* DE1948 00010EDC 00000000 */   nop
    /* DE194C 00010EE0 8FA90088 */  lw         $t1, 0x88($sp)
    /* DE1950 00010EE4 AD220154 */  sw         $v0, 0x154($t1)
    /* DE1954 00010EE8 8FAA0088 */  lw         $t2, 0x88($sp)
    /* DE1958 00010EEC 8D4B0154 */  lw         $t3, 0x154($t2)
    /* DE195C 00010EF0 91640012 */  lbu        $a0, 0x12($t3)
    /* DE1960 00010EF4 0C000000 */  jal        func_00000000
    /* DE1964 00010EF8 00000000 */   nop
    /* DE1968 00010EFC 8FAC0088 */  lw         $t4, 0x88($sp)
    /* DE196C 00010F00 8D8D0154 */  lw         $t5, 0x154($t4)
    /* DE1970 00010F04 91A40013 */  lbu        $a0, 0x13($t5)
    /* DE1974 00010F08 0C000000 */  jal        func_00000000
    /* DE1978 00010F0C 00000000 */   nop
    /* DE197C 00010F10 8FAE0088 */  lw         $t6, 0x88($sp)
    /* DE1980 00010F14 8DCF0154 */  lw         $t7, 0x154($t6)
    /* DE1984 00010F18 91E40014 */  lbu        $a0, 0x14($t7)
    /* DE1988 00010F1C 0C000000 */  jal        func_00000000
    /* DE198C 00010F20 00000000 */   nop
    /* DE1990 00010F24 8FB80088 */  lw         $t8, 0x88($sp)
    /* DE1994 00010F28 8F190154 */  lw         $t9, 0x154($t8)
    /* DE1998 00010F2C 97240004 */  lhu        $a0, 0x4($t9)
    /* DE199C 00010F30 30840003 */  andi       $a0, $a0, 0x3
    /* DE19A0 00010F34 0C000000 */  jal        func_00000000
    /* DE19A4 00010F38 00000000 */   nop
    /* DE19A8 00010F3C 8FA80088 */  lw         $t0, 0x88($sp)
    /* DE19AC 00010F40 8D090154 */  lw         $t1, 0x154($t0)
    /* DE19B0 00010F44 952A0004 */  lhu        $t2, 0x4($t1)
    /* DE19B4 00010F48 314B0004 */  andi       $t3, $t2, 0x4
    /* DE19B8 00010F4C 396C0004 */  xori       $t4, $t3, 0x4
    /* DE19BC 00010F50 2D8C0001 */  sltiu      $t4, $t4, 0x1
    /* DE19C0 00010F54 3C010000 */  lui        $at, (0x0 >> 16)
    /* DE19C4 00010F58 AC2C0000 */  sw         $t4, 0x0($at)
    /* DE19C8 00010F5C 8FAD0088 */  lw         $t5, 0x88($sp)
    /* DE19CC 00010F60 8DAE0148 */  lw         $t6, 0x148($t5)
    /* DE19D0 00010F64 8DCF0000 */  lw         $t7, 0x0($t6)
    /* DE19D4 00010F68 ADAF0164 */  sw         $t7, 0x164($t5)
    /* DE19D8 00010F6C 8FB80088 */  lw         $t8, 0x88($sp)
    /* DE19DC 00010F70 8F040164 */  lw         $a0, 0x164($t8)
    /* DE19E0 00010F74 00042080 */  sll        $a0, $a0, 2
    /* DE19E4 00010F78 0C000000 */  jal        func_00000000
    /* DE19E8 00010F7C 00000000 */   nop
    /* DE19EC 00010F80 8FB90088 */  lw         $t9, 0x88($sp)
    /* DE19F0 00010F84 AF220160 */  sw         $v0, 0x160($t9)
    /* DE19F4 00010F88 00002025 */  or         $a0, $zero, $zero
    /* DE19F8 00010F8C 3C050001 */  lui        $a1, %hi(sStr_rom_flowimgs_bin_C5F0)
    /* DE19FC 00010F90 24A5C5F0 */  addiu      $a1, $a1, %lo(sStr_rom_flowimgs_bin_C5F0)
    /* DE1A00 00010F94 0C000000 */  jal        func_00000000
    /* DE1A04 00010F98 00000000 */   nop
    /* DE1A08 00010F9C 8FA80088 */  lw         $t0, 0x88($sp)
    /* DE1A0C 00010FA0 AD020150 */  sw         $v0, 0x150($t0)
    /* DE1A10 00010FA4 8FA40084 */  lw         $a0, 0x84($sp)
    /* DE1A14 00010FA8 0C000000 */  jal        func_00000000
    /* DE1A18 00010FAC 00000000 */   nop
    /* DE1A1C 00010FB0 8FA40088 */  lw         $a0, 0x88($sp)
    /* DE1A20 00010FB4 0C000000 */  jal        func_00000000
    /* DE1A24 00010FB8 00000000 */   nop
    /* DE1A28 00010FBC 10000001 */  b          .L00010FC4
    /* DE1A2C 00010FC0 00000000 */   nop
  .L00010FC4:
    /* DE1A30 00010FC4 D7B40028 */  ldc1       $f20, 0x28($sp)
    /* DE1A34 00010FC8 D7B60030 */  ldc1       $f22, 0x30($sp)
    /* DE1A38 00010FCC D7B80038 */  ldc1       $f24, 0x38($sp)
    /* DE1A3C 00010FD0 D7BA0040 */  ldc1       $f26, 0x40($sp)
    /* DE1A40 00010FD4 8FB0004C */  lw         $s0, 0x4C($sp)
    /* DE1A44 00010FD8 8FB10050 */  lw         $s1, 0x50($sp)
    /* DE1A48 00010FDC 8FBF0054 */  lw         $ra, 0x54($sp)
    /* DE1A4C 00010FE0 27BD0088 */  addiu      $sp, $sp, 0x88
    /* DE1A50 00010FE4 03E00008 */  jr         $ra
    /* DE1A54 00010FE8 00000000 */   nop
endlabel GameFlow_create
