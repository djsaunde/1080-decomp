nonmatching EnvSettings_func_00007288, 0xA0

glabel EnvSettings_func_00007288
    /* DD7CF4 00007288 8C8E0038 */  lw         $t6, 0x38($a0)
    /* DD7CF8 0000728C 31CF0002 */  andi       $t7, $t6, 0x2
    /* DD7CFC 00007290 51E0000F */  beql       $t7, $zero, .L000072D0
    /* DD7D00 00007294 8C8A003C */   lw        $t2, 0x3C($a0)
    /* DD7D04 00007298 8C980030 */  lw         $t8, 0x30($a0)
    /* DD7D08 0000729C 3C030000 */  lui        $v1, (0x0 >> 16)
    /* DD7D0C 000072A0 24630000 */  addiu      $v1, $v1, 0x0
    /* DD7D10 000072A4 44982000 */  mtc1       $t8, $f4
    /* DD7D14 000072A8 8C790254 */  lw         $t9, 0x254($v1)
    /* DD7D18 000072AC 468021A0 */  cvt.s.w    $f6, $f4
    /* DD7D1C 000072B0 E72600AC */  swc1       $f6, 0xAC($t9)
    /* DD7D20 000072B4 8C880034 */  lw         $t0, 0x34($a0)
    /* DD7D24 000072B8 8C690254 */  lw         $t1, 0x254($v1)
    /* DD7D28 000072BC 44884000 */  mtc1       $t0, $f8
    /* DD7D2C 000072C0 00000000 */  nop
    /* DD7D30 000072C4 468042A0 */  cvt.s.w    $f10, $f8
    /* DD7D34 000072C8 E52A00B0 */  swc1       $f10, 0xB0($t1)
    /* DD7D38 000072CC 8C8A003C */  lw         $t2, 0x3C($a0)
  .L000072D0:
    /* DD7D3C 000072D0 3C030000 */  lui        $v1, (0x0 >> 16)
    /* DD7D40 000072D4 24630000 */  addiu      $v1, $v1, 0x0
    /* DD7D44 000072D8 15400011 */  bnez       $t2, .L00007320
    /* DD7D48 000072DC 00000000 */   nop
    /* DD7D4C 000072E0 8C620254 */  lw         $v0, 0x254($v1)
    /* DD7D50 000072E4 3C0E0000 */  lui        $t6, %hi(func_0000027C + 0x18)
    /* DD7D54 000072E8 25CE0294 */  addiu      $t6, $t6, %lo(func_0000027C + 0x18)
    /* DD7D58 000072EC 8C4B0078 */  lw         $t3, 0x78($v0)
    /* DD7D5C 000072F0 24420078 */  addiu      $v0, $v0, 0x78
    /* DD7D60 000072F4 356C0004 */  ori        $t4, $t3, 0x4
    /* DD7D64 000072F8 AC4C0000 */  sw         $t4, 0x0($v0)
    /* DD7D68 000072FC 8DD80000 */  lw         $t8, 0x0($t6)
    /* DD7D6C 00007300 8C6D0254 */  lw         $t5, 0x254($v1)
    /* DD7D70 00007304 ADB800DC */  sw         $t8, 0xDC($t5)
    /* DD7D74 00007308 8DCF0004 */  lw         $t7, 0x4($t6)
    /* DD7D78 0000730C ADAF00E0 */  sw         $t7, 0xE0($t5)
    /* DD7D7C 00007310 8DD80008 */  lw         $t8, 0x8($t6)
    /* DD7D80 00007314 ADB800E4 */  sw         $t8, 0xE4($t5)
    /* DD7D84 00007318 8DCF000C */  lw         $t7, 0xC($t6)
  alabel D_0000731C
    /* DD7D88 0000731C ADAF00E8 */  sw         $t7, 0xE8($t5)
  .L00007320:
    /* DD7D8C 00007320 03E00008 */  jr         $ra
  alabel D_00007324
    /* DD7D90 00007324 00000000 */   nop
endlabel EnvSettings_func_00007288
