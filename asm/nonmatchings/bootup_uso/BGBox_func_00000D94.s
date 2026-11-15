nonmatching BGBox_func_00000D94, 0xD4

glabel BGBox_func_00000D94
    /* DD1800 00000D94 3C060000 */  lui        $a2, (0x0 >> 16)
    /* DD1804 00000D98 24C60000 */  addiu      $a2, $a2, 0x0
    /* DD1808 00000D9C C4C402A0 */  lwc1       $f4, 0x2A0($a2)
    /* DD180C 00000DA0 3C010000 */  lui        $at, %hi(func_00000044 + 0x2C)
    /* DD1810 00000DA4 D4280070 */  ldc1       $f8, %lo(func_00000044 + 0x2C)($at)
    /* DD1814 00000DA8 460021A1 */  cvt.d.s    $f6, $f4
    /* DD1818 00000DAC 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD181C 00000DB0 4628303C */  c.lt.d     $f6, $f8
    /* DD1820 00000DB4 AFA40018 */  sw         $a0, 0x18($sp)
    /* DD1824 00000DB8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD1828 00000DBC 00C02025 */  or         $a0, $a2, $zero
    /* DD182C 00000DC0 45000015 */  bc1f       .L00000E18
    /* DD1830 00000DC4 3C050000 */   lui       $a1, %hi(func_0000027C + 0x18)
    /* DD1834 00000DC8 0C000000 */  jal        func_00000000
    /* DD1838 00000DCC 24A50294 */   addiu     $a1, $a1, %lo(func_0000027C + 0x18)
    /* DD183C 00000DD0 0C000000 */  jal        func_00000000
    /* DD1840 00000DD4 8FA40018 */   lw        $a0, 0x18($sp)
    /* DD1844 00000DD8 8FAE0018 */  lw         $t6, 0x18($sp)
    /* DD1848 00000DDC 3C060000 */  lui        $a2, (0x0 >> 16)
    /* DD184C 00000DE0 24C60000 */  addiu      $a2, $a2, 0x0
    /* DD1850 00000DE4 8DC3003C */  lw         $v1, 0x3C($t6)
    /* DD1854 00000DE8 8CC20254 */  lw         $v0, 0x254($a2)
    /* DD1858 00000DEC 10600005 */  beqz       $v1, .L00000E04
    /* DD185C 00000DF0 24420078 */   addiu     $v0, $v0, 0x78
    /* DD1860 00000DF4 8C4F0000 */  lw         $t7, 0x0($v0)
    /* DD1864 00000DF8 35F80004 */  ori        $t8, $t7, 0x4
    /* DD1868 00000DFC 10000016 */  b          .L00000E58
    /* DD186C 00000E00 AC580000 */   sw        $t8, 0x0($v0)
  .L00000E04:
    /* DD1870 00000E04 8C590000 */  lw         $t9, 0x0($v0)
    /* DD1874 00000E08 2401FFFB */  addiu      $at, $zero, -0x5
    /* DD1878 00000E0C 03214024 */  and        $t0, $t9, $at
    /* DD187C 00000E10 10000011 */  b          .L00000E58
    /* DD1880 00000E14 AC480000 */   sw        $t0, 0x0($v0)
  .L00000E18:
    /* DD1884 00000E18 8CC20254 */  lw         $v0, 0x254($a2)
    /* DD1888 00000E1C 3C0C0000 */  lui        $t4, %hi(func_0000027C + 0x18)
    /* DD188C 00000E20 258C0294 */  addiu      $t4, $t4, %lo(func_0000027C + 0x18)
    /* DD1890 00000E24 8C490078 */  lw         $t1, 0x78($v0)
    /* DD1894 00000E28 24420078 */  addiu      $v0, $v0, 0x78
    /* DD1898 00000E2C 352A0004 */  ori        $t2, $t1, 0x4
    /* DD189C 00000E30 AC4A0000 */  sw         $t2, 0x0($v0)
    /* DD18A0 00000E34 8D8E0000 */  lw         $t6, 0x0($t4)
    /* DD18A4 00000E38 8CCB0254 */  lw         $t3, 0x254($a2)
    /* DD18A8 00000E3C AD6E00DC */  sw         $t6, 0xDC($t3)
    /* DD18AC 00000E40 8D8D0004 */  lw         $t5, 0x4($t4)
    /* DD18B0 00000E44 AD6D00E0 */  sw         $t5, 0xE0($t3)
    /* DD18B4 00000E48 8D8E0008 */  lw         $t6, 0x8($t4)
    /* DD18B8 00000E4C AD6E00E4 */  sw         $t6, 0xE4($t3)
    /* DD18BC 00000E50 8D8D000C */  lw         $t5, 0xC($t4)
    /* DD18C0 00000E54 AD6D00E8 */  sw         $t5, 0xE8($t3)
  .L00000E58:
    /* DD18C4 00000E58 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD18C8 00000E5C 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD18CC 00000E60 03E00008 */  jr         $ra
    /* DD18D0 00000E64 00000000 */   nop
endlabel BGBox_func_00000D94
