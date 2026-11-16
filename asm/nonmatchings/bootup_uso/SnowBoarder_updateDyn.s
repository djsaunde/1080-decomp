nonmatching SnowBoarder_updateDyn, 0x2A8

glabel SnowBoarder_updateDyn
    /* DDC9F8 0000BF8C 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDC9FC 0000BF90 AFBF001C */  sw         $ra, 0x1C($sp)
    /* DDCA00 0000BF94 AFB00018 */  sw         $s0, 0x18($sp)
    /* DDCA04 0000BF98 8C820800 */  lw         $v0, 0x800($a0)
    /* DDCA08 0000BF9C 3C01BFE8 */  lui        $at, (0xBFE80000 >> 16)
    /* DDCA0C 0000BFA0 44814800 */  mtc1       $at, $f9
    /* DDCA10 0000BFA4 C4440004 */  lwc1       $f4, 0x4($v0)
    /* DDCA14 0000BFA8 44804000 */  mtc1       $zero, $f8
    /* DDCA18 0000BFAC 00808025 */  or         $s0, $a0, $zero
    /* DDCA1C 0000BFB0 460021A1 */  cvt.d.s    $f6, $f4
    /* DDCA20 0000BFB4 00007025 */  or         $t6, $zero, $zero
    /* DDCA24 0000BFB8 4628303C */  c.lt.d     $f6, $f8
    /* DDCA28 0000BFBC 00000000 */  nop
    /* DDCA2C 0000BFC0 45020003 */  bc1fl      .L0000BFD0
    /* DDCA30 0000BFC4 AE0E0990 */   sw        $t6, 0x990($s0)
    /* DDCA34 0000BFC8 240E0001 */  addiu      $t6, $zero, 0x1
    /* DDCA38 0000BFCC AE0E0990 */  sw         $t6, 0x990($s0)
  .L0000BFD0:
    /* DDCA3C 0000BFD0 8C4F0010 */  lw         $t7, 0x10($v0)
    /* DDCA40 0000BFD4 C60A0490 */  lwc1       $f10, 0x490($s0)
    /* DDCA44 0000BFD8 31F80400 */  andi       $t8, $t7, 0x400
    /* DDCA48 0000BFDC 0018C82B */  sltu       $t9, $zero, $t8
    /* DDCA4C 0000BFE0 AE1909CC */  sw         $t9, 0x9CC($s0)
    /* DDCA50 0000BFE4 1320000A */  beqz       $t9, .L0000C010
    /* DDCA54 0000BFE8 E60A09F8 */   swc1      $f10, 0x9F8($s0)
    /* DDCA58 0000BFEC C61006F0 */  lwc1       $f16, 0x6F0($s0)
    /* DDCA5C 0000BFF0 C6120508 */  lwc1       $f18, 0x508($s0)
    /* DDCA60 0000BFF4 C60604F0 */  lwc1       $f6, 0x4F0($s0)
    /* DDCA64 0000BFF8 C60804D8 */  lwc1       $f8, 0x4D8($s0)
    /* DDCA68 0000BFFC 46128102 */  mul.s      $f4, $f16, $f18
    /* DDCA6C 0000C000 E60609FC */  swc1       $f6, 0x9FC($s0)
    /* DDCA70 0000C004 E6080360 */  swc1       $f8, 0x360($s0)
    /* DDCA74 0000C008 10000009 */  b          .L0000C030
    /* DDCA78 0000C00C E6040A00 */   swc1      $f4, 0xA00($s0)
  .L0000C010:
    /* DDCA7C 0000C010 C60A0708 */  lwc1       $f10, 0x708($s0)
    /* DDCA80 0000C014 C6100508 */  lwc1       $f16, 0x508($s0)
    /* DDCA84 0000C018 C60404C0 */  lwc1       $f4, 0x4C0($s0)
    /* DDCA88 0000C01C C60604A8 */  lwc1       $f6, 0x4A8($s0)
    /* DDCA8C 0000C020 46105482 */  mul.s      $f18, $f10, $f16
    /* DDCA90 0000C024 E60409FC */  swc1       $f4, 0x9FC($s0)
    /* DDCA94 0000C028 E6060360 */  swc1       $f6, 0x360($s0)
    /* DDCA98 0000C02C E6120A00 */  swc1       $f18, 0xA00($s0)
  .L0000C030:
    /* DDCA9C 0000C030 8E090990 */  lw         $t1, 0x990($s0)
    /* DDCAA0 0000C034 51200018 */  beql       $t1, $zero, .L0000C098
    /* DDCAA4 0000C038 860209A2 */   lh        $v0, 0x9A2($s0)
    /* DDCAA8 0000C03C 8E0A09CC */  lw         $t2, 0x9CC($s0)
    /* DDCAAC 0000C040 44807000 */  mtc1       $zero, $f14
    /* DDCAB0 0000C044 3C013F00 */  lui        $at, (0x3F000000 >> 16)
    /* DDCAB4 0000C048 51400005 */  beql       $t2, $zero, .L0000C060
    /* DDCAB8 0000C04C 44816000 */   mtc1      $at, $f12
    /* DDCABC 0000C050 3C010000 */  lui        $at, %hi(SndHandle_triggerEvent0_000008F4 + 0x20)
    /* DDCAC0 0000C054 10000003 */  b          .L0000C064
    /* DDCAC4 0000C058 C42C0914 */   lwc1      $f12, %lo(SndHandle_triggerEvent0_000008F4 + 0x20)($at)
    /* DDCAC8 0000C05C 44816000 */  mtc1       $at, $f12
  .L0000C060:
    /* DDCACC 0000C060 00000000 */  nop
  .L0000C064:
    /* DDCAD0 0000C064 C6000970 */  lwc1       $f0, 0x970($s0)
    /* DDCAD4 0000C068 460E003C */  c.lt.s     $f0, $f14
    /* DDCAD8 0000C06C 00000000 */  nop
    /* DDCADC 0000C070 45020004 */  bc1fl      .L0000C084
    /* DDCAE0 0000C074 46000086 */   mov.s     $f2, $f0
    /* DDCAE4 0000C078 10000002 */  b          .L0000C084
    /* DDCAE8 0000C07C 46000087 */   neg.s     $f2, $f0
    /* DDCAEC 0000C080 46000086 */  mov.s      $f2, $f0
  .L0000C084:
    /* DDCAF0 0000C084 460C1282 */  mul.s      $f10, $f2, $f12
    /* DDCAF4 0000C088 C6080360 */  lwc1       $f8, 0x360($s0)
    /* DDCAF8 0000C08C 460A4400 */  add.s      $f16, $f8, $f10
    /* DDCAFC 0000C090 E6100360 */  swc1       $f16, 0x360($s0)
    /* DDCB00 0000C094 860209A2 */  lh         $v0, 0x9A2($s0)
  .L0000C098:
    /* DDCB04 0000C098 44807000 */  mtc1       $zero, $f14
    /* DDCB08 0000C09C 24010063 */  addiu      $at, $zero, 0x63
    /* DDCB0C 0000C0A0 1041000F */  beq        $v0, $at, .L0000C0E0
    /* DDCB10 0000C0A4 24010061 */   addiu     $at, $zero, 0x61
    /* DDCB14 0000C0A8 10410020 */  beq        $v0, $at, .L0000C12C
    /* DDCB18 0000C0AC 24010062 */   addiu     $at, $zero, 0x62
    /* DDCB1C 0000C0B0 50410016 */  beql       $v0, $at, .L0000C10C
    /* DDCB20 0000C0B4 C6100360 */   lwc1      $f16, 0x360($s0)
    /* DDCB24 0000C0B8 C6120360 */  lwc1       $f18, 0x360($s0)
    /* DDCB28 0000C0BC 3C010000 */  lui        $at, %hi(SndHandle_triggerEvent0_000008F4 + 0x24)
    /* DDCB2C 0000C0C0 D4260918 */  ldc1       $f6, %lo(SndHandle_triggerEvent0_000008F4 + 0x24)($at)
    /* DDCB30 0000C0C4 46009121 */  cvt.d.s    $f4, $f18
    /* DDCB34 0000C0C8 E60E09FC */  swc1       $f14, 0x9FC($s0)
    /* DDCB38 0000C0CC 46262200 */  add.d      $f8, $f4, $f6
    /* DDCB3C 0000C0D0 E60E09F8 */  swc1       $f14, 0x9F8($s0)
    /* DDCB40 0000C0D4 462042A0 */  cvt.s.d    $f10, $f8
    /* DDCB44 0000C0D8 10000014 */  b          .L0000C12C
    /* DDCB48 0000C0DC E60A0360 */   swc1      $f10, 0x360($s0)
  .L0000C0E0:
    /* DDCB4C 0000C0E0 C6100360 */  lwc1       $f16, 0x360($s0)
    /* DDCB50 0000C0E4 C6120448 */  lwc1       $f18, 0x448($s0)
    /* DDCB54 0000C0E8 C60609FC */  lwc1       $f6, 0x9FC($s0)
    /* DDCB58 0000C0EC C6080430 */  lwc1       $f8, 0x430($s0)
    /* DDCB5C 0000C0F0 46128102 */  mul.s      $f4, $f16, $f18
    /* DDCB60 0000C0F4 00000000 */  nop
    /* DDCB64 0000C0F8 46083282 */  mul.s      $f10, $f6, $f8
    /* DDCB68 0000C0FC E6040360 */  swc1       $f4, 0x360($s0)
    /* DDCB6C 0000C100 1000000A */  b          .L0000C12C
    /* DDCB70 0000C104 E60A09FC */   swc1      $f10, 0x9FC($s0)
    /* DDCB74 0000C108 C6100360 */  lwc1       $f16, 0x360($s0)
  .L0000C10C:
    /* DDCB78 0000C10C C6120478 */  lwc1       $f18, 0x478($s0)
    /* DDCB7C 0000C110 C60609FC */  lwc1       $f6, 0x9FC($s0)
    /* DDCB80 0000C114 C6080460 */  lwc1       $f8, 0x460($s0)
    /* DDCB84 0000C118 46128102 */  mul.s      $f4, $f16, $f18
    /* DDCB88 0000C11C 00000000 */  nop
    /* DDCB8C 0000C120 46083282 */  mul.s      $f10, $f6, $f8
    /* DDCB90 0000C124 E6040360 */  swc1       $f4, 0x360($s0)
    /* DDCB94 0000C128 E60A09FC */  swc1       $f10, 0x9FC($s0)
  .L0000C12C:
    /* DDCB98 0000C12C C61009FC */  lwc1       $f16, 0x9FC($s0)
    /* DDCB9C 0000C130 C61205D0 */  lwc1       $f18, 0x5D0($s0)
    /* DDCBA0 0000C134 C6060360 */  lwc1       $f6, 0x360($s0)
    /* DDCBA4 0000C138 C60805E8 */  lwc1       $f8, 0x5E8($s0)
    /* DDCBA8 0000C13C 46128102 */  mul.s      $f4, $f16, $f18
    /* DDCBAC 0000C140 8E020960 */  lw         $v0, 0x960($s0)
    /* DDCBB0 0000C144 46083282 */  mul.s      $f10, $f6, $f8
    /* DDCBB4 0000C148 E60409FC */  swc1       $f4, 0x9FC($s0)
    /* DDCBB8 0000C14C 10400014 */  beqz       $v0, .L0000C1A0
    /* DDCBBC 0000C150 E60A0360 */   swc1      $f10, 0x360($s0)
    /* DDCBC0 0000C154 44828000 */  mtc1       $v0, $f16
    /* DDCBC4 0000C158 3C0142C8 */  lui        $at, (0x42C80000 >> 16)
    /* DDCBC8 0000C15C 44812000 */  mtc1       $at, $f4
    /* DDCBCC 0000C160 468084A0 */  cvt.s.w    $f18, $f16
    /* DDCBD0 0000C164 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DDCBD4 0000C168 44818000 */  mtc1       $at, $f16
    /* DDCBD8 0000C16C C6080360 */  lwc1       $f8, 0x360($s0)
    /* DDCBDC 0000C170 E60E09FC */  swc1       $f14, 0x9FC($s0)
    /* DDCBE0 0000C174 46049183 */  div.s      $f6, $f18, $f4
    /* DDCBE4 0000C178 C61206D8 */  lwc1       $f18, 0x6D8($s0)
    /* DDCBE8 0000C17C C6040318 */  lwc1       $f4, 0x318($s0)
    /* DDCBEC 0000C180 46128001 */  sub.s      $f0, $f16, $f18
    /* DDCBF0 0000C184 46064280 */  add.s      $f10, $f8, $f6
    /* DDCBF4 0000C188 46002202 */  mul.s      $f8, $f4, $f0
    /* DDCBF8 0000C18C C6060320 */  lwc1       $f6, 0x320($s0)
    /* DDCBFC 0000C190 E60A0360 */  swc1       $f10, 0x360($s0)
    /* DDCC00 0000C194 46003282 */  mul.s      $f10, $f6, $f0
    /* DDCC04 0000C198 E6080318 */  swc1       $f8, 0x318($s0)
    /* DDCC08 0000C19C E60A0320 */  swc1       $f10, 0x320($s0)
  .L0000C1A0:
    /* DDCC0C 0000C1A0 0C000000 */  jal        func_00000000
    /* DDCC10 0000C1A4 02002025 */   or        $a0, $s0, $zero
    /* DDCC14 0000C1A8 0C000000 */  jal        func_00000000
    /* DDCC18 0000C1AC 02002025 */   or        $a0, $s0, $zero
    /* DDCC1C 0000C1B0 3C040001 */  lui        $a0, %hi(stream_readF32_00008A40 + 0x20)
    /* DDCC20 0000C1B4 0C000000 */  jal        func_00000000
    /* DDCC24 0000C1B8 24848A60 */   addiu     $a0, $a0, %lo(stream_readF32_00008A40 + 0x20)
    /* DDCC28 0000C1BC 0C000000 */  jal        func_00000000
    /* DDCC2C 0000C1C0 02002025 */   or        $a0, $s0, $zero
    /* DDCC30 0000C1C4 3C040001 */  lui        $a0, %hi(stream_readF32_00008A40 + 0x2C)
    /* DDCC34 0000C1C8 0C000000 */  jal        func_00000000
    /* DDCC38 0000C1CC 24848A6C */   addiu     $a0, $a0, %lo(stream_readF32_00008A40 + 0x2C)
    /* DDCC3C 0000C1D0 0C000000 */  jal        func_00000000
    /* DDCC40 0000C1D4 02002025 */   or        $a0, $s0, $zero
    /* DDCC44 0000C1D8 8E0B0960 */  lw         $t3, 0x960($s0)
    /* DDCC48 0000C1DC 24010064 */  addiu      $at, $zero, 0x64
    /* DDCC4C 0000C1E0 1561000B */  bne        $t3, $at, .L0000C210
    /* DDCC50 0000C1E4 3C0141F0 */   lui       $at, (0x41F00000 >> 16)
    /* DDCC54 0000C1E8 44819000 */  mtc1       $at, $f18
    /* DDCC58 0000C1EC C6100348 */  lwc1       $f16, 0x348($s0)
    /* DDCC5C 0000C1F0 4612803C */  c.lt.s     $f16, $f18
    /* DDCC60 0000C1F4 00000000 */  nop
    /* DDCC64 0000C1F8 45000005 */  bc1f       .L0000C210
    /* DDCC68 0000C1FC 00000000 */   nop
    /* DDCC6C 0000C200 44800000 */  mtc1       $zero, $f0
    /* DDCC70 0000C204 00000000 */  nop
    /* DDCC74 0000C208 E6000318 */  swc1       $f0, 0x318($s0)
    /* DDCC78 0000C20C E6000320 */  swc1       $f0, 0x320($s0)
  .L0000C210:
    /* DDCC7C 0000C210 0C000000 */  jal        func_00000000
    /* DDCC80 0000C214 02002025 */   or        $a0, $s0, $zero
    /* DDCC84 0000C218 0C000000 */  jal        func_00000000
    /* DDCC88 0000C21C 02002025 */   or        $a0, $s0, $zero
    /* DDCC8C 0000C220 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DDCC90 0000C224 8FB00018 */  lw         $s0, 0x18($sp)
    /* DDCC94 0000C228 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDCC98 0000C22C 03E00008 */  jr         $ra
    /* DDCC9C 0000C230 00000000 */   nop
endlabel SnowBoarder_updateDyn
