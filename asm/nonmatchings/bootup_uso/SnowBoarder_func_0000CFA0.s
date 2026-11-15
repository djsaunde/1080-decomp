nonmatching SnowBoarder_func_0000CFA0, 0x4A0

glabel SnowBoarder_func_0000CFA0
    /* DDDA0C 0000CFA0 27BDFF78 */  addiu      $sp, $sp, -0x88
    /* DDDA10 0000CFA4 AFBF002C */  sw         $ra, 0x2C($sp)
    /* DDDA14 0000CFA8 AFB00028 */  sw         $s0, 0x28($sp)
    /* DDDA18 0000CFAC 8C8E0A58 */  lw         $t6, 0xA58($a0)
    /* DDDA1C 0000CFB0 00808025 */  or         $s0, $a0, $zero
    /* DDDA20 0000CFB4 3C180000 */  lui        $t8, %hi(func_0000023C + 0x18)
    /* DDDA24 0000CFB8 31CF2000 */  andi       $t7, $t6, 0x2000
    /* DDDA28 0000CFBC 11E00005 */  beqz       $t7, .L0000CFD4
    /* DDDA2C 0000CFC0 24050002 */   addiu     $a1, $zero, 0x2
    /* DDDA30 0000CFC4 8F180254 */  lw         $t8, %lo(func_0000023C + 0x18)($t8)
    /* DDDA34 0000CFC8 8C9908DC */  lw         $t9, 0x8DC($a0)
    /* DDDA38 0000CFCC 53190118 */  beql       $t8, $t9, .L0000D430
    /* DDDA3C 0000CFD0 8FBF002C */   lw        $ra, 0x2C($sp)
  .L0000CFD4:
    /* DDDA40 0000CFD4 C60400BC */  lwc1       $f4, 0xBC($s0)
    /* DDDA44 0000CFD8 44803000 */  mtc1       $zero, $f6
    /* DDDA48 0000CFDC 8E0600B4 */  lw         $a2, 0xB4($s0)
    /* DDDA4C 0000CFE0 8E0700B8 */  lw         $a3, 0xB8($s0)
    /* DDDA50 0000CFE4 3C040000 */  lui        $a0, %hi(func_0000023C + 0x18)
    /* DDDA54 0000CFE8 27A80084 */  addiu      $t0, $sp, 0x84
    /* DDDA58 0000CFEC AFA80014 */  sw         $t0, 0x14($sp)
    /* DDDA5C 0000CFF0 8C840254 */  lw         $a0, %lo(func_0000023C + 0x18)($a0)
    /* DDDA60 0000CFF4 E7A40010 */  swc1       $f4, 0x10($sp)
    /* DDDA64 0000CFF8 0C000000 */  jal        func_00000000
    /* DDDA68 0000CFFC E7A60018 */   swc1      $f6, 0x18($sp)
    /* DDDA6C 0000D000 1040010A */  beqz       $v0, .L0000D42C
    /* DDDA70 0000D004 AE020A40 */   sw        $v0, 0xA40($s0)
    /* DDDA74 0000D008 3C040000 */  lui        $a0, %hi(func_0000023C + 0x18)
    /* DDDA78 0000D00C 8C840254 */  lw         $a0, %lo(func_0000023C + 0x18)($a0)
    /* DDDA7C 0000D010 C7AA0084 */  lwc1       $f10, 0x84($sp)
    /* DDDA80 0000D014 00001025 */  or         $v0, $zero, $zero
    /* DDDA84 0000D018 C4880098 */  lwc1       $f8, 0x98($a0)
    /* DDDA88 0000D01C 460A403C */  c.lt.s     $f8, $f10
    /* DDDA8C 0000D020 00000000 */  nop
    /* DDDA90 0000D024 45000002 */  bc1f       .L0000D030
    /* DDDA94 0000D028 00000000 */   nop
    /* DDDA98 0000D02C 24020001 */  addiu      $v0, $zero, 0x1
  .L0000D030:
    /* DDDA9C 0000D030 54400005 */  bnel       $v0, $zero, .L0000D048
    /* DDDAA0 0000D034 AFA20080 */   sw        $v0, 0x80($sp)
    /* DDDAA4 0000D038 8E0908DC */  lw         $t1, 0x8DC($s0)
    /* DDDAA8 0000D03C 00891026 */  xor        $v0, $a0, $t1
    /* DDDAAC 0000D040 0002102B */  sltu       $v0, $zero, $v0
    /* DDDAB0 0000D044 AFA20080 */  sw         $v0, 0x80($sp)
  .L0000D048:
    /* DDDAB4 0000D048 8E0A0A58 */  lw         $t2, 0xA58($s0)
    /* DDDAB8 0000D04C 000A5BC0 */  sll        $t3, $t2, 15
    /* DDDABC 0000D050 05630039 */  bgezl      $t3, .L0000D138
    /* DDDAC0 0000D054 8FAC0080 */   lw        $t4, 0x80($sp)
    /* DDDAC4 0000D058 860C09A0 */  lh         $t4, 0x9A0($s0)
    /* DDDAC8 0000D05C 24010061 */  addiu      $at, $zero, 0x61
    /* DDDACC 0000D060 55810035 */  bnel       $t4, $at, .L0000D138
    /* DDDAD0 0000D064 8FAC0080 */   lw        $t4, 0x80($sp)
    /* DDDAD4 0000D068 860D0904 */  lh         $t5, 0x904($s0)
    /* DDDAD8 0000D06C 24010009 */  addiu      $at, $zero, 0x9
    /* DDDADC 0000D070 51A10031 */  beql       $t5, $at, .L0000D138
    /* DDDAE0 0000D074 8FAC0080 */   lw        $t4, 0x80($sp)
    /* DDDAE4 0000D078 8E0E0938 */  lw         $t6, 0x938($s0)
    /* DDDAE8 0000D07C 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDDAEC 0000D080 24840000 */  addiu      $a0, $a0, 0x0
    /* DDDAF0 0000D084 51C0002C */  beql       $t6, $zero, .L0000D138
    /* DDDAF4 0000D088 8FAC0080 */   lw        $t4, 0x80($sp)
    /* DDDAF8 0000D08C 0C000000 */  jal        func_00000000
    /* DDDAFC 0000D090 2405000D */   addiu     $a1, $zero, 0xD
    /* DDDB00 0000D094 8E0F08FC */  lw         $t7, 0x8FC($s0)
    /* DDDB04 0000D098 8C590000 */  lw         $t9, 0x0($v0)
    /* DDDB08 0000D09C 24040010 */  addiu      $a0, $zero, 0x10
    /* DDDB0C 0000D0A0 ADF90000 */  sw         $t9, 0x0($t7)
    /* DDDB10 0000D0A4 8C580004 */  lw         $t8, 0x4($v0)
    /* DDDB14 0000D0A8 ADF80004 */  sw         $t8, 0x4($t7)
    /* DDDB18 0000D0AC 8C590008 */  lw         $t9, 0x8($v0)
    /* DDDB1C 0000D0B0 0C000000 */  jal        func_00000000
    /* DDDB20 0000D0B4 ADF90008 */   sw        $t9, 0x8($t7)
    /* DDDB24 0000D0B8 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DDDB28 0000D0BC 44810000 */  mtc1       $at, $f0
    /* DDDB2C 0000D0C0 3C010000 */  lui        $at, %hi(SndHandle_triggerEvent3_00000940 + 0x30)
    /* DDDB30 0000D0C4 C4300970 */  lwc1       $f16, %lo(SndHandle_triggerEvent3_00000940 + 0x30)($at)
    /* DDDB34 0000D0C8 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDDB38 0000D0CC 24840000 */  addiu      $a0, $a0, 0x0
    /* DDDB3C 0000D0D0 27A50064 */  addiu      $a1, $sp, 0x64
    /* DDDB40 0000D0D4 E7A00064 */  swc1       $f0, 0x64($sp)
    /* DDDB44 0000D0D8 E7A00068 */  swc1       $f0, 0x68($sp)
    /* DDDB48 0000D0DC E7A0006C */  swc1       $f0, 0x6C($sp)
    /* DDDB4C 0000D0E0 0C000000 */  jal        func_00000000
    /* DDDB50 0000D0E4 E7B00070 */   swc1      $f16, 0x70($sp)
    /* DDDB54 0000D0E8 0C000000 */  jal        func_00000000
    /* DDDB58 0000D0EC 24040010 */   addiu     $a0, $zero, 0x10
    /* DDDB5C 0000D0F0 8E030824 */  lw         $v1, 0x824($s0)
    /* DDDB60 0000D0F4 8C620028 */  lw         $v0, 0x28($v1)
    /* DDDB64 0000D0F8 8C59001C */  lw         $t9, 0x1C($v0)
    /* DDDB68 0000D0FC 84480018 */  lh         $t0, 0x18($v0)
    /* DDDB6C 0000D100 0320F809 */  jalr       $t9
    /* DDDB70 0000D104 01032021 */   addu      $a0, $t0, $v1
    /* DDDB74 0000D108 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDDB78 0000D10C 24840000 */  addiu      $a0, $a0, 0x0
    /* DDDB7C 0000D110 0C000000 */  jal        func_00000000
    /* DDDB80 0000D114 24050006 */   addiu     $a1, $zero, 0x6
    /* DDDB84 0000D118 8E0908FC */  lw         $t1, 0x8FC($s0)
    /* DDDB88 0000D11C 8C4B0000 */  lw         $t3, 0x0($v0)
    /* DDDB8C 0000D120 AD2B0000 */  sw         $t3, 0x0($t1)
    /* DDDB90 0000D124 8C4A0004 */  lw         $t2, 0x4($v0)
    /* DDDB94 0000D128 AD2A0004 */  sw         $t2, 0x4($t1)
    /* DDDB98 0000D12C 8C4B0008 */  lw         $t3, 0x8($v0)
    /* DDDB9C 0000D130 AD2B0008 */  sw         $t3, 0x8($t1)
    /* DDDBA0 0000D134 8FAC0080 */  lw         $t4, 0x80($sp)
  .L0000D138:
    /* DDDBA4 0000D138 51800007 */  beql       $t4, $zero, .L0000D158
    /* DDDBA8 0000D13C 8E0309A8 */   lw        $v1, 0x9A8($s0)
    /* DDDBAC 0000D140 8E0D08B8 */  lw         $t5, 0x8B8($s0)
    /* DDDBB0 0000D144 24040014 */  addiu      $a0, $zero, 0x14
    /* DDDBB4 0000D148 31AE0004 */  andi       $t6, $t5, 0x4
    /* DDDBB8 0000D14C 15C000A9 */  bnez       $t6, .L0000D3F4
    /* DDDBBC 0000D150 00000000 */   nop
    /* DDDBC0 0000D154 8E0309A8 */  lw         $v1, 0x9A8($s0)
  .L0000D158:
    /* DDDBC4 0000D158 240F0001 */  addiu      $t7, $zero, 0x1
    /* DDDBC8 0000D15C AE0F0A44 */  sw         $t7, 0xA44($s0)
    /* DDDBCC 0000D160 30780002 */  andi       $t8, $v1, 0x2
    /* DDDBD0 0000D164 5300001D */  beql       $t8, $zero, .L0000D1DC
    /* DDDBD4 0000D168 8E0208B8 */   lw        $v0, 0x8B8($s0)
    /* DDDBD8 0000D16C 8E080A58 */  lw         $t0, 0xA58($s0)
    /* DDDBDC 0000D170 31191000 */  andi       $t9, $t0, 0x1000
    /* DDDBE0 0000D174 53200019 */  beql       $t9, $zero, .L0000D1DC
    /* DDDBE4 0000D178 8E0208B8 */   lw        $v0, 0x8B8($s0)
    /* DDDBE8 0000D17C 8E090A4C */  lw         $t1, 0xA4C($s0)
    /* DDDBEC 0000D180 260509D8 */  addiu      $a1, $s0, 0x9D8
    /* DDDBF0 0000D184 51200015 */  beql       $t1, $zero, .L0000D1DC
    /* DDDBF4 0000D188 8E0208B8 */   lw        $v0, 0x8B8($s0)
    /* DDDBF8 0000D18C 0C000000 */  jal        func_00000000
    /* DDDBFC 0000D190 8E040850 */   lw        $a0, 0x850($s0)
    /* DDDC00 0000D194 0C000000 */  jal        func_00000000
    /* DDDC04 0000D198 02002025 */   or        $a0, $s0, $zero
    /* DDDC08 0000D19C 0C000000 */  jal        func_00000000
    /* DDDC0C 0000D1A0 24040011 */   addiu     $a0, $zero, 0x11
    /* DDDC10 0000D1A4 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDDC14 0000D1A8 260508E0 */  addiu      $a1, $s0, 0x8E0
    /* DDDC18 0000D1AC AFA50030 */  sw         $a1, 0x30($sp)
    /* DDDC1C 0000D1B0 0C000000 */  jal        func_00000000
    /* DDDC20 0000D1B4 24840000 */   addiu     $a0, $a0, 0x0
    /* DDDC24 0000D1B8 0C000000 */  jal        func_00000000
    /* DDDC28 0000D1BC 02002025 */   or        $a0, $s0, $zero
    /* DDDC2C 0000D1C0 0C000000 */  jal        func_00000000
    /* DDDC30 0000D1C4 02002025 */   or        $a0, $s0, $zero
    /* DDDC34 0000D1C8 3C010000 */  lui        $at, %hi(func_00000080 + 0x64)
    /* DDDC38 0000D1CC AC2000E4 */  sw         $zero, %lo(func_00000080 + 0x64)($at)
    /* DDDC3C 0000D1D0 1000007D */  b          .L0000D3C8
    /* DDDC40 0000D1D4 8E0208B8 */   lw        $v0, 0x8B8($s0)
    /* DDDC44 0000D1D8 8E0208B8 */  lw         $v0, 0x8B8($s0)
  .L0000D1DC:
    /* DDDC48 0000D1DC 304A000A */  andi       $t2, $v0, 0xA
    /* DDDC4C 0000D1E0 51400078 */  beql       $t2, $zero, .L0000D3C4
    /* DDDC50 0000D1E4 260A08E0 */   addiu     $t2, $s0, 0x8E0
    /* DDDC54 0000D1E8 8E0B0A4C */  lw         $t3, 0xA4C($s0)
    /* DDDC58 0000D1EC 306C0400 */  andi       $t4, $v1, 0x400
    /* DDDC5C 0000D1F0 51600074 */  beql       $t3, $zero, .L0000D3C4
    /* DDDC60 0000D1F4 260A08E0 */   addiu     $t2, $s0, 0x8E0
    /* DDDC64 0000D1F8 15800071 */  bnez       $t4, .L0000D3C0
    /* DDDC68 0000D1FC 260509D8 */   addiu     $a1, $s0, 0x9D8
    /* DDDC6C 0000D200 8E040850 */  lw         $a0, 0x850($s0)
    /* DDDC70 0000D204 0C000000 */  jal        func_00000000
    /* DDDC74 0000D208 27A60044 */   addiu     $a2, $sp, 0x44
    /* DDDC78 0000D20C C7B20044 */  lwc1       $f18, 0x44($sp)
    /* DDDC7C 0000D210 C60409D8 */  lwc1       $f4, 0x9D8($s0)
    /* DDDC80 0000D214 C7A80048 */  lwc1       $f8, 0x48($sp)
    /* DDDC84 0000D218 C60A09DC */  lwc1       $f10, 0x9DC($s0)
    /* DDDC88 0000D21C 46049182 */  mul.s      $f6, $f18, $f4
    /* DDDC8C 0000D220 C7A4004C */  lwc1       $f4, 0x4C($sp)
    /* DDDC90 0000D224 3C010000 */  lui        $at, %hi(SndHandle_triggerEvent3_00000940 + 0x38)
    /* DDDC94 0000D228 460A4402 */  mul.s      $f16, $f8, $f10
    /* DDDC98 0000D22C C60809E0 */  lwc1       $f8, 0x9E0($s0)
    /* DDDC9C 0000D230 46082282 */  mul.s      $f10, $f4, $f8
    /* DDDCA0 0000D234 D4240978 */  ldc1       $f4, %lo(SndHandle_triggerEvent3_00000940 + 0x38)($at)
    /* DDDCA4 0000D238 3C01437F */  lui        $at, (0x437F0000 >> 16)
    /* DDDCA8 0000D23C 46103480 */  add.s      $f18, $f6, $f16
    /* DDDCAC 0000D240 460A9180 */  add.s      $f6, $f18, $f10
    /* DDDCB0 0000D244 46003421 */  cvt.d.s    $f16, $f6
    /* DDDCB4 0000D248 4624803C */  c.lt.d     $f16, $f4
    /* DDDCB8 0000D24C 00000000 */  nop
    /* DDDCBC 0000D250 45000058 */  bc1f       .L0000D3B4
    /* DDDCC0 0000D254 00000000 */   nop
    /* DDDCC4 0000D258 92020A30 */  lbu        $v0, 0xA30($s0)
    /* DDDCC8 0000D25C 44810000 */  mtc1       $at, $f0
    /* DDDCCC 0000D260 92030A31 */  lbu        $v1, 0xA31($s0)
    /* DDDCD0 0000D264 04410002 */  bgez       $v0, .L0000D270
    /* DDDCD4 0000D268 00400821 */   addu      $at, $v0, $zero
    /* DDDCD8 0000D26C 24410001 */  addiu      $at, $v0, 0x1
  .L0000D270:
    /* DDDCDC 0000D270 00011043 */  sra        $v0, $at, 1
    /* DDDCE0 0000D274 92060A32 */  lbu        $a2, 0xA32($s0)
    /* DDDCE4 0000D278 304200FF */  andi       $v0, $v0, 0xFF
    /* DDDCE8 0000D27C 04610002 */  bgez       $v1, .L0000D288
    /* DDDCEC 0000D280 00600821 */   addu      $at, $v1, $zero
    /* DDDCF0 0000D284 24610001 */  addiu      $at, $v1, 0x1
  .L0000D288:
    /* DDDCF4 0000D288 00011843 */  sra        $v1, $at, 1
    /* DDDCF8 0000D28C 44824000 */  mtc1       $v0, $f8
    /* DDDCFC 0000D290 306300FF */  andi       $v1, $v1, 0xFF
    /* DDDD00 0000D294 04C10002 */  bgez       $a2, .L0000D2A0
    /* DDDD04 0000D298 00C00821 */   addu      $at, $a2, $zero
    /* DDDD08 0000D29C 24C10001 */  addiu      $at, $a2, 0x1
  .L0000D2A0:
    /* DDDD0C 0000D2A0 00013043 */  sra        $a2, $at, 1
    /* DDDD10 0000D2A4 30C600FF */  andi       $a2, $a2, 0xFF
    /* DDDD14 0000D2A8 04410005 */  bgez       $v0, .L0000D2C0
    /* DDDD18 0000D2AC 468044A0 */   cvt.s.w   $f18, $f8
    /* DDDD1C 0000D2B0 3C014F80 */  lui        $at, (0x4F800000 >> 16)
    /* DDDD20 0000D2B4 44815000 */  mtc1       $at, $f10
    /* DDDD24 0000D2B8 00000000 */  nop
    /* DDDD28 0000D2BC 460A9480 */  add.s      $f18, $f18, $f10
  .L0000D2C0:
    /* DDDD2C 0000D2C0 46009183 */  div.s      $f6, $f18, $f0
    /* DDDD30 0000D2C4 44838000 */  mtc1       $v1, $f16
    /* DDDD34 0000D2C8 3C014F80 */  lui        $at, (0x4F800000 >> 16)
    /* DDDD38 0000D2CC 46808120 */  cvt.s.w    $f4, $f16
    /* DDDD3C 0000D2D0 04610004 */  bgez       $v1, .L0000D2E4
    /* DDDD40 0000D2D4 E60609E8 */   swc1      $f6, 0x9E8($s0)
    /* DDDD44 0000D2D8 44814000 */  mtc1       $at, $f8
    /* DDDD48 0000D2DC 00000000 */  nop
    /* DDDD4C 0000D2E0 46082100 */  add.s      $f4, $f4, $f8
  .L0000D2E4:
    /* DDDD50 0000D2E4 46002283 */  div.s      $f10, $f4, $f0
    /* DDDD54 0000D2E8 44869000 */  mtc1       $a2, $f18
    /* DDDD58 0000D2EC 3C014F80 */  lui        $at, (0x4F800000 >> 16)
    /* DDDD5C 0000D2F0 468091A0 */  cvt.s.w    $f6, $f18
    /* DDDD60 0000D2F4 04C10004 */  bgez       $a2, .L0000D308
    /* DDDD64 0000D2F8 E60A09EC */   swc1      $f10, 0x9EC($s0)
    /* DDDD68 0000D2FC 44818000 */  mtc1       $at, $f16
    /* DDDD6C 0000D300 00000000 */  nop
    /* DDDD70 0000D304 46103180 */  add.s      $f6, $f6, $f16
  .L0000D308:
    /* DDDD74 0000D308 3C0142E0 */  lui        $at, (0x42E00000 >> 16)
    /* DDDD78 0000D30C 44812000 */  mtc1       $at, $f4
    /* DDDD7C 0000D310 46003203 */  div.s      $f8, $f6, $f0
    /* DDDD80 0000D314 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDDD84 0000D318 24840000 */  addiu      $a0, $a0, 0x0
    /* DDDD88 0000D31C 260509E8 */  addiu      $a1, $s0, 0x9E8
    /* DDDD8C 0000D320 46002283 */  div.s      $f10, $f4, $f0
    /* DDDD90 0000D324 E60809F0 */  swc1       $f8, 0x9F0($s0)
    /* DDDD94 0000D328 0C000000 */  jal        func_00000000
    /* DDDD98 0000D32C E60A09F4 */   swc1      $f10, 0x9F4($s0)
    /* DDDD9C 0000D330 8E0208B8 */  lw         $v0, 0x8B8($s0)
    /* DDDDA0 0000D334 304D0002 */  andi       $t5, $v0, 0x2
    /* DDDDA4 0000D338 51A00011 */  beql       $t5, $zero, .L0000D380
    /* DDDDA8 0000D33C 30580008 */   andi      $t8, $v0, 0x8
    /* DDDDAC 0000D340 0C000000 */  jal        func_00000000
    /* DDDDB0 0000D344 24040002 */   addiu     $a0, $zero, 0x2
    /* DDDDB4 0000D348 8E030814 */  lw         $v1, 0x814($s0)
    /* DDDDB8 0000D34C 8C620028 */  lw         $v0, 0x28($v1)
    /* DDDDBC 0000D350 8C59001C */  lw         $t9, 0x1C($v0)
    /* DDDDC0 0000D354 844E0018 */  lh         $t6, 0x18($v0)
    /* DDDDC4 0000D358 0320F809 */  jalr       $t9
    /* DDDDC8 0000D35C 01C32021 */   addu      $a0, $t6, $v1
    /* DDDDCC 0000D360 8E030820 */  lw         $v1, 0x820($s0)
    /* DDDDD0 0000D364 8C620028 */  lw         $v0, 0x28($v1)
    /* DDDDD4 0000D368 8C59001C */  lw         $t9, 0x1C($v0)
    /* DDDDD8 0000D36C 844F0018 */  lh         $t7, 0x18($v0)
    /* DDDDDC 0000D370 0320F809 */  jalr       $t9
    /* DDDDE0 0000D374 01E32021 */   addu      $a0, $t7, $v1
    /* DDDDE4 0000D378 8E0208B8 */  lw         $v0, 0x8B8($s0)
    /* DDDDE8 0000D37C 30580008 */  andi       $t8, $v0, 0x8
  .L0000D380:
    /* DDDDEC 0000D380 1300000C */  beqz       $t8, .L0000D3B4
    /* DDDDF0 0000D384 00000000 */   nop
    /* DDDDF4 0000D388 8E080938 */  lw         $t0, 0x938($s0)
    /* DDDDF8 0000D38C 15000009 */  bnez       $t0, .L0000D3B4
    /* DDDDFC 0000D390 00000000 */   nop
    /* DDDE00 0000D394 0C000000 */  jal        func_00000000
    /* DDDE04 0000D398 24040008 */   addiu     $a0, $zero, 0x8
    /* DDDE08 0000D39C 8E030824 */  lw         $v1, 0x824($s0)
    /* DDDE0C 0000D3A0 8C620028 */  lw         $v0, 0x28($v1)
    /* DDDE10 0000D3A4 8C59001C */  lw         $t9, 0x1C($v0)
    /* DDDE14 0000D3A8 84490018 */  lh         $t1, 0x18($v0)
    /* DDDE18 0000D3AC 0320F809 */  jalr       $t9
    /* DDDE1C 0000D3B0 01232021 */   addu      $a0, $t1, $v1
  .L0000D3B4:
    /* DDDE20 0000D3B4 3C010000 */  lui        $at, %hi(func_00000080 + 0x64)
    /* DDDE24 0000D3B8 AC2000E4 */  sw         $zero, %lo(func_00000080 + 0x64)($at)
    /* DDDE28 0000D3BC 8E0208B8 */  lw         $v0, 0x8B8($s0)
  .L0000D3C0:
    /* DDDE2C 0000D3C0 260A08E0 */  addiu      $t2, $s0, 0x8E0
  .L0000D3C4:
    /* DDDE30 0000D3C4 AFAA0030 */  sw         $t2, 0x30($sp)
  .L0000D3C8:
    /* DDDE34 0000D3C8 304B0001 */  andi       $t3, $v0, 0x1
    /* DDDE38 0000D3CC 11600005 */  beqz       $t3, .L0000D3E4
    /* DDDE3C 0000D3D0 00000000 */   nop
    /* DDDE40 0000D3D4 0C000000 */  jal        func_00000000
    /* DDDE44 0000D3D8 24040011 */   addiu     $a0, $zero, 0x11
    /* DDDE48 0000D3DC 10000009 */  b          .L0000D404
    /* DDDE4C 0000D3E0 00000000 */   nop
  .L0000D3E4:
    /* DDDE50 0000D3E4 0C000000 */  jal        func_00000000
    /* DDDE54 0000D3E8 24040014 */   addiu     $a0, $zero, 0x14
    /* DDDE58 0000D3EC 10000005 */  b          .L0000D404
    /* DDDE5C 0000D3F0 00000000 */   nop
  .L0000D3F4:
    /* DDDE60 0000D3F4 0C000000 */  jal        func_00000000
    /* DDDE64 0000D3F8 AE000A44 */   sw        $zero, 0xA44($s0)
    /* DDDE68 0000D3FC 260C08E0 */  addiu      $t4, $s0, 0x8E0
    /* DDDE6C 0000D400 AFAC0030 */  sw         $t4, 0x30($sp)
  .L0000D404:
    /* DDDE70 0000D404 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDDE74 0000D408 24840000 */  addiu      $a0, $a0, 0x0
    /* DDDE78 0000D40C 0C000000 */  jal        func_00000000
    /* DDDE7C 0000D410 8FA50030 */   lw        $a1, 0x30($sp)
    /* DDDE80 0000D414 0C000000 */  jal        func_00000000
    /* DDDE84 0000D418 02002025 */   or        $a0, $s0, $zero
    /* DDDE88 0000D41C 3C0D0000 */  lui        $t5, %hi(func_0000023C + 0x18)
    /* DDDE8C 0000D420 8DAD0254 */  lw         $t5, %lo(func_0000023C + 0x18)($t5)
    /* DDDE90 0000D424 0C000000 */  jal        func_00000000
    /* DDDE94 0000D428 8DA40108 */   lw        $a0, 0x108($t5)
  .L0000D42C:
    /* DDDE98 0000D42C 8FBF002C */  lw         $ra, 0x2C($sp)
  .L0000D430:
    /* DDDE9C 0000D430 8FB00028 */  lw         $s0, 0x28($sp)
    /* DDDEA0 0000D434 27BD0088 */  addiu      $sp, $sp, 0x88
    /* DDDEA4 0000D438 03E00008 */  jr         $ra
    /* DDDEA8 0000D43C 00000000 */   nop
endlabel SnowBoarder_func_0000CFA0
