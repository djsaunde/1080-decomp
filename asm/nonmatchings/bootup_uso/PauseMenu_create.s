nonmatching PauseMenu_create, 0x208

glabel PauseMenu_create
    /* DE0954 0000FEE8 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DE0958 0000FEEC AFBF001C */  sw         $ra, 0x1C($sp)
    /* DE095C 0000FEF0 AFA40028 */  sw         $a0, 0x28($sp)
    /* DE0960 0000FEF4 AFB10018 */  sw         $s1, 0x18($sp)
    /* DE0964 0000FEF8 AFB00014 */  sw         $s0, 0x14($sp)
    /* DE0968 0000FEFC 8FAE0028 */  lw         $t6, 0x28($sp)
    /* DE096C 0000FF00 15C00008 */  bnez       $t6, .L0000FF24
    /* DE0970 0000FF04 00000000 */   nop
    /* DE0974 0000FF08 24040304 */  addiu      $a0, $zero, 0x304
    /* DE0978 0000FF0C 0C000000 */  jal        func_00000000
    /* DE097C 0000FF10 00000000 */   nop
    /* DE0980 0000FF14 AFA20028 */  sw         $v0, 0x28($sp)
    /* DE0984 0000FF18 8FAF0028 */  lw         $t7, 0x28($sp)
    /* DE0988 0000FF1C 11E0006B */  beqz       $t7, .L000100CC
    /* DE098C 0000FF20 00000000 */   nop
  .L0000FF24:
    /* DE0990 0000FF24 8FB00028 */  lw         $s0, 0x28($sp)
    /* DE0994 0000FF28 16000007 */  bnez       $s0, .L0000FF48
    /* DE0998 0000FF2C 00000000 */   nop
    /* DE099C 0000FF30 2404002C */  addiu      $a0, $zero, 0x2C
    /* DE09A0 0000FF34 0C000000 */  jal        func_00000000
    /* DE09A4 0000FF38 00000000 */   nop
    /* DE09A8 0000FF3C 00408025 */  or         $s0, $v0, $zero
    /* DE09AC 0000FF40 12000009 */  beqz       $s0, .L0000FF68
    /* DE09B0 0000FF44 00000000 */   nop
  .L0000FF48:
    /* DE09B4 0000FF48 02002025 */  or         $a0, $s0, $zero
    /* DE09B8 0000FF4C 3C050001 */  lui        $a1, %hi(D_0000C5BC)
    /* DE09BC 0000FF50 24A5C5BC */  addiu      $a1, $a1, %lo(D_0000C5BC)
    /* DE09C0 0000FF54 0C000000 */  jal        func_00000000
    /* DE09C4 0000FF58 00000000 */   nop
    /* DE09C8 0000FF5C 3C180000 */  lui        $t8, (0x0 >> 16)
    /* DE09CC 0000FF60 27180000 */  addiu      $t8, $t8, 0x0
    /* DE09D0 0000FF64 AE180028 */  sw         $t8, 0x28($s0)
  .L0000FF68:
    /* DE09D4 0000FF68 3C190000 */  lui        $t9, (0x0 >> 16)
    /* DE09D8 0000FF6C 27390000 */  addiu      $t9, $t9, 0x0
    /* DE09DC 0000FF70 8FA80028 */  lw         $t0, 0x28($sp)
    /* DE09E0 0000FF74 AD190028 */  sw         $t9, 0x28($t0)
    /* DE09E4 0000FF78 8FB00028 */  lw         $s0, 0x28($sp)
    /* DE09E8 0000FF7C 3C110001 */  lui        $s1, %hi(D_0000C5C4)
    /* DE09EC 0000FF80 2631C5C4 */  addiu      $s1, $s1, %lo(D_0000C5C4)
    /* DE09F0 0000FF84 AE11000C */  sw         $s1, 0xC($s0)
    /* DE09F4 0000FF88 8FB00028 */  lw         $s0, 0x28($sp)
    /* DE09F8 0000FF8C 2610004C */  addiu      $s0, $s0, 0x4C
    /* DE09FC 0000FF90 24090077 */  addiu      $t1, $zero, 0x77
    /* DE0A00 0000FF94 44892000 */  mtc1       $t1, $f4
    /* DE0A04 0000FF98 00000000 */  nop
    /* DE0A08 0000FF9C 468021A0 */  cvt.s.w    $f6, $f4
    /* DE0A0C 0000FFA0 3C01437F */  lui        $at, (0x437F0000 >> 16)
    /* DE0A10 0000FFA4 44814000 */  mtc1       $at, $f8
    /* DE0A14 0000FFA8 00000000 */  nop
    /* DE0A18 0000FFAC 46083283 */  div.s      $f10, $f6, $f8
    /* DE0A1C 0000FFB0 E60A0000 */  swc1       $f10, 0x0($s0)
    /* DE0A20 0000FFB4 240A0075 */  addiu      $t2, $zero, 0x75
    /* DE0A24 0000FFB8 448A8000 */  mtc1       $t2, $f16
    /* DE0A28 0000FFBC 00000000 */  nop
    /* DE0A2C 0000FFC0 468084A0 */  cvt.s.w    $f18, $f16
    /* DE0A30 0000FFC4 3C01437F */  lui        $at, (0x437F0000 >> 16)
    /* DE0A34 0000FFC8 44812000 */  mtc1       $at, $f4
    /* DE0A38 0000FFCC 00000000 */  nop
    /* DE0A3C 0000FFD0 46049183 */  div.s      $f6, $f18, $f4
    /* DE0A40 0000FFD4 E6060004 */  swc1       $f6, 0x4($s0)
    /* DE0A44 0000FFD8 240B00E5 */  addiu      $t3, $zero, 0xE5
    /* DE0A48 0000FFDC 448B4000 */  mtc1       $t3, $f8
    /* DE0A4C 0000FFE0 00000000 */  nop
    /* DE0A50 0000FFE4 468042A0 */  cvt.s.w    $f10, $f8
    /* DE0A54 0000FFE8 3C01437F */  lui        $at, (0x437F0000 >> 16)
    /* DE0A58 0000FFEC 44818000 */  mtc1       $at, $f16
    /* DE0A5C 0000FFF0 00000000 */  nop
    /* DE0A60 0000FFF4 46105483 */  div.s      $f18, $f10, $f16
    /* DE0A64 0000FFF8 E6120008 */  swc1       $f18, 0x8($s0)
    /* DE0A68 0000FFFC 240C00FF */  addiu      $t4, $zero, 0xFF
    /* DE0A6C 00010000 448C2000 */  mtc1       $t4, $f4
    /* DE0A70 00010004 00000000 */  nop
    /* DE0A74 00010008 468021A0 */  cvt.s.w    $f6, $f4
    /* DE0A78 0001000C 3C01437F */  lui        $at, (0x437F0000 >> 16)
    /* DE0A7C 00010010 44814000 */  mtc1       $at, $f8
    /* DE0A80 00010014 00000000 */  nop
    /* DE0A84 00010018 46083283 */  div.s      $f10, $f6, $f8
    /* DE0A88 0001001C E60A000C */  swc1       $f10, 0xC($s0)
    /* DE0A8C 00010020 8FB00028 */  lw         $s0, 0x28($sp)
    /* DE0A90 00010024 2610005C */  addiu      $s0, $s0, 0x5C
    /* DE0A94 00010028 240D00C0 */  addiu      $t5, $zero, 0xC0
    /* DE0A98 0001002C 448D8000 */  mtc1       $t5, $f16
    /* DE0A9C 00010030 00000000 */  nop
    /* DE0AA0 00010034 468084A0 */  cvt.s.w    $f18, $f16
    /* DE0AA4 00010038 3C01437F */  lui        $at, (0x437F0000 >> 16)
    /* DE0AA8 0001003C 44812000 */  mtc1       $at, $f4
    /* DE0AAC 00010040 00000000 */  nop
    /* DE0AB0 00010044 46049183 */  div.s      $f6, $f18, $f4
    /* DE0AB4 00010048 E6060000 */  swc1       $f6, 0x0($s0)
    /* DE0AB8 0001004C 240E00C2 */  addiu      $t6, $zero, 0xC2
    /* DE0ABC 00010050 448E4000 */  mtc1       $t6, $f8
    /* DE0AC0 00010054 00000000 */  nop
    /* DE0AC4 00010058 468042A0 */  cvt.s.w    $f10, $f8
    /* DE0AC8 0001005C 3C01437F */  lui        $at, (0x437F0000 >> 16)
    /* DE0ACC 00010060 44818000 */  mtc1       $at, $f16
    /* DE0AD0 00010064 00000000 */  nop
    /* DE0AD4 00010068 46105483 */  div.s      $f18, $f10, $f16
    /* DE0AD8 0001006C E6120004 */  swc1       $f18, 0x4($s0)
    /* DE0ADC 00010070 240F00E5 */  addiu      $t7, $zero, 0xE5
    /* DE0AE0 00010074 448F2000 */  mtc1       $t7, $f4
    /* DE0AE4 00010078 00000000 */  nop
    /* DE0AE8 0001007C 468021A0 */  cvt.s.w    $f6, $f4
    /* DE0AEC 00010080 3C01437F */  lui        $at, (0x437F0000 >> 16)
    /* DE0AF0 00010084 44814000 */  mtc1       $at, $f8
    /* DE0AF4 00010088 00000000 */  nop
    /* DE0AF8 0001008C 46083283 */  div.s      $f10, $f6, $f8
    /* DE0AFC 00010090 E60A0008 */  swc1       $f10, 0x8($s0)
    /* DE0B00 00010094 241800FF */  addiu      $t8, $zero, 0xFF
    /* DE0B04 00010098 44988000 */  mtc1       $t8, $f16
    /* DE0B08 0001009C 00000000 */  nop
    /* DE0B0C 000100A0 468084A0 */  cvt.s.w    $f18, $f16
    /* DE0B10 000100A4 3C01437F */  lui        $at, (0x437F0000 >> 16)
    /* DE0B14 000100A8 44812000 */  mtc1       $at, $f4
    /* DE0B18 000100AC 00000000 */  nop
    /* DE0B1C 000100B0 46049183 */  div.s      $f6, $f18, $f4
    /* DE0B20 000100B4 E606000C */  swc1       $f6, 0xC($s0)
    /* DE0B24 000100B8 8FB90028 */  lw         $t9, 0x28($sp)
    /* DE0B28 000100BC AF200080 */  sw         $zero, 0x80($t9)
    /* DE0B2C 000100C0 8FA40028 */  lw         $a0, 0x28($sp)
    /* DE0B30 000100C4 0C000000 */  jal        func_00000000
    /* DE0B34 000100C8 00000000 */   nop
  .L000100CC:
    /* DE0B38 000100CC 8FA20028 */  lw         $v0, 0x28($sp)
    /* DE0B3C 000100D0 10000001 */  b          .L000100D8
    /* DE0B40 000100D4 00000000 */   nop
  .L000100D8:
    /* DE0B44 000100D8 8FB00014 */  lw         $s0, 0x14($sp)
    /* DE0B48 000100DC 8FB10018 */  lw         $s1, 0x18($sp)
    /* DE0B4C 000100E0 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DE0B50 000100E4 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DE0B54 000100E8 03E00008 */  jr         $ra
    /* DE0B58 000100EC 00000000 */   nop
endlabel PauseMenu_create
