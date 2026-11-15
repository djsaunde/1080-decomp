nonmatching SnowBoarder_procColl, 0x898

glabel SnowBoarder_procColl
    /* DDCCA0 0000C234 27BDFF08 */  addiu      $sp, $sp, -0xF8
    /* DDCCA4 0000C238 AFBF002C */  sw         $ra, 0x2C($sp)
    /* DDCCA8 0000C23C AFB00028 */  sw         $s0, 0x28($sp)
    /* DDCCAC 0000C240 8C820A80 */  lw         $v0, 0xA80($a0)
    /* DDCCB0 0000C244 00808025 */  or         $s0, $a0, $zero
    /* DDCCB4 0000C248 10400002 */  beqz       $v0, .L0000C254
    /* DDCCB8 0000C24C 244EFFFF */   addiu     $t6, $v0, -0x1
    /* DDCCBC 0000C250 AC8E0A80 */  sw         $t6, 0xA80($a0)
  .L0000C254:
    /* DDCCC0 0000C254 8E1800B4 */  lw         $t8, 0xB4($s0)
    /* DDCCC4 0000C258 27A200B4 */  addiu      $v0, $sp, 0xB4
    /* DDCCC8 0000C25C 44808000 */  mtc1       $zero, $f16
    /* DDCCCC 0000C260 AC580000 */  sw         $t8, 0x0($v0)
    /* DDCCD0 0000C264 8E0F00B8 */  lw         $t7, 0xB8($s0)
    /* DDCCD4 0000C268 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DDCCD8 0000C26C 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDCCDC 0000C270 AC4F0004 */  sw         $t7, 0x4($v0)
    /* DDCCE0 0000C274 8E1800BC */  lw         $t8, 0xBC($s0)
    /* DDCCE4 0000C278 260500DC */  addiu      $a1, $s0, 0xDC
    /* DDCCE8 0000C27C 3C06457A */  lui        $a2, (0x457A0000 >> 16)
    /* DDCCEC 0000C280 AC580008 */  sw         $t8, 0x8($v0)
    /* DDCCF0 0000C284 C7A400B4 */  lwc1       $f4, 0xB4($sp)
    /* DDCCF4 0000C288 8E080318 */  lw         $t0, 0x318($s0)
    /* DDCCF8 0000C28C 27A700CC */  addiu      $a3, $sp, 0xCC
    /* DDCCFC 0000C290 E6040890 */  swc1       $f4, 0x890($s0)
    /* DDCD00 0000C294 C7A600B8 */  lwc1       $f6, 0xB8($sp)
    /* DDCD04 0000C298 E6060894 */  swc1       $f6, 0x894($s0)
    /* DDCD08 0000C29C C7A800BC */  lwc1       $f8, 0xBC($sp)
    /* DDCD0C 0000C2A0 44813000 */  mtc1       $at, $f6
    /* DDCD10 0000C2A4 3C010000 */  lui        $at, %hi(func_000008F4 + 0x2C)
    /* DDCD14 0000C2A8 E6080898 */  swc1       $f8, 0x898($s0)
    /* DDCD18 0000C2AC AC480000 */  sw         $t0, 0x0($v0)
    /* DDCD1C 0000C2B0 8E19031C */  lw         $t9, 0x31C($s0)
    /* DDCD20 0000C2B4 AC590004 */  sw         $t9, 0x4($v0)
    /* DDCD24 0000C2B8 8E080320 */  lw         $t0, 0x320($s0)
    /* DDCD28 0000C2BC AC480008 */  sw         $t0, 0x8($v0)
    /* DDCD2C 0000C2C0 C7AA00B4 */  lwc1       $f10, 0xB4($sp)
    /* DDCD30 0000C2C4 E60A089C */  swc1       $f10, 0x89C($s0)
    /* DDCD34 0000C2C8 C7B200B8 */  lwc1       $f18, 0xB8($sp)
    /* DDCD38 0000C2CC E61208A0 */  swc1       $f18, 0x8A0($s0)
    /* DDCD3C 0000C2D0 C7A400BC */  lwc1       $f4, 0xBC($sp)
    /* DDCD40 0000C2D4 E60408A4 */  swc1       $f4, 0x8A4($s0)
    /* DDCD44 0000C2D8 C4280920 */  lwc1       $f8, %lo(func_000008F4 + 0x2C)($at)
    /* DDCD48 0000C2DC E7B000F0 */  swc1       $f16, 0xF0($sp)
    /* DDCD4C 0000C2E0 E7B000EC */  swc1       $f16, 0xEC($sp)
    /* DDCD50 0000C2E4 E7B000E4 */  swc1       $f16, 0xE4($sp)
    /* DDCD54 0000C2E8 E7A600E8 */  swc1       $f6, 0xE8($sp)
    /* DDCD58 0000C2EC A60009A2 */  sh         $zero, 0x9A2($s0)
    /* DDCD5C 0000C2F0 E60809D0 */  swc1       $f8, 0x9D0($s0)
    /* DDCD60 0000C2F4 AFA5003C */  sw         $a1, 0x3C($sp)
    /* DDCD64 0000C2F8 0C000000 */  jal        func_00000000
    /* DDCD68 0000C2FC 8C840000 */   lw        $a0, 0x0($a0)
    /* DDCD6C 0000C300 44808000 */  mtc1       $zero, $f16
    /* DDCD70 0000C304 104000D4 */  beqz       $v0, .L0000C658
    /* DDCD74 0000C308 3C013F80 */   lui       $at, (0x3F800000 >> 16)
    /* DDCD78 0000C30C 3C090000 */  lui        $t1, (0x0 >> 16)
    /* DDCD7C 0000C310 8D290000 */  lw         $t1, 0x0($t1)
    /* DDCD80 0000C314 87A500DC */  lh         $a1, 0xDC($sp)
    /* DDCD84 0000C318 0C000000 */  jal        func_00000000
    /* DDCD88 0000C31C 8D240084 */   lw        $a0, 0x84($t1)
    /* DDCD8C 0000C320 A60209A0 */  sh         $v0, 0x9A0($s0)
    /* DDCD90 0000C324 3C010000 */  lui        $at, %hi(func_000008F4 + 0x30)
    /* DDCD94 0000C328 C4320924 */  lwc1       $f18, %lo(func_000008F4 + 0x30)($at)
    /* DDCD98 0000C32C C7AA00D8 */  lwc1       $f10, 0xD8($sp)
    /* DDCD9C 0000C330 3C0B0000 */  lui        $t3, (0x0 >> 16)
    /* DDCDA0 0000C334 4612503C */  c.lt.s     $f10, $f18
    /* DDCDA4 0000C338 00000000 */  nop
    /* DDCDA8 0000C33C 45020005 */  bc1fl      .L0000C354
    /* DDCDAC 0000C340 A60009A2 */   sh        $zero, 0x9A2($s0)
    /* DDCDB0 0000C344 860A09A0 */  lh         $t2, 0x9A0($s0)
    /* DDCDB4 0000C348 10000002 */  b          .L0000C354
    /* DDCDB8 0000C34C A60A09A2 */   sh        $t2, 0x9A2($s0)
    /* DDCDBC 0000C350 A60009A2 */  sh         $zero, 0x9A2($s0)
  .L0000C354:
    /* DDCDC0 0000C354 8D6B0000 */  lw         $t3, 0x0($t3)
    /* DDCDC4 0000C358 87A500DC */  lh         $a1, 0xDC($sp)
    /* DDCDC8 0000C35C 0C000000 */  jal        func_00000000
    /* DDCDCC 0000C360 8D640084 */   lw        $a0, 0x84($t3)
    /* DDCDD0 0000C364 AE0209A8 */  sw         $v0, 0x9A8($s0)
    /* DDCDD4 0000C368 C7A400D8 */  lwc1       $f4, 0xD8($sp)
    /* DDCDD8 0000C36C C60600DC */  lwc1       $f6, 0xDC($s0)
    /* DDCDDC 0000C370 3C010000 */  lui        $at, %hi(func_000008F4 + 0x34)
    /* DDCDE0 0000C374 E60409D0 */  swc1       $f4, 0x9D0($s0)
    /* DDCDE4 0000C378 C7A800E4 */  lwc1       $f8, 0xE4($sp)
    /* DDCDE8 0000C37C C4240928 */  lwc1       $f4, %lo(func_000008F4 + 0x34)($at)
    /* DDCDEC 0000C380 C61200E0 */  lwc1       $f18, 0xE0($s0)
    /* DDCDF0 0000C384 46083282 */  mul.s      $f10, $f6, $f8
    /* DDCDF4 0000C388 C7A800E8 */  lwc1       $f8, 0xE8($sp)
    /* DDCDF8 0000C38C 46049180 */  add.s      $f6, $f18, $f4
    /* DDCDFC 0000C390 3C010000 */  lui        $at, %hi(func_000000F0 + 0x38)
    /* DDCE00 0000C394 44808000 */  mtc1       $zero, $f16
    /* DDCE04 0000C398 240D0001 */  addiu      $t5, $zero, 0x1
    /* DDCE08 0000C39C 46083482 */  mul.s      $f18, $f6, $f8
    /* DDCE0C 0000C3A0 C7A800EC */  lwc1       $f8, 0xEC($sp)
    /* DDCE10 0000C3A4 C60600E4 */  lwc1       $f6, 0xE4($s0)
    /* DDCE14 0000C3A8 27AF00E4 */  addiu      $t7, $sp, 0xE4
    /* DDCE18 0000C3AC 46125100 */  add.s      $f4, $f10, $f18
    /* DDCE1C 0000C3B0 46083282 */  mul.s      $f10, $f6, $f8
    /* DDCE20 0000C3B4 C4260128 */  lwc1       $f6, %lo(func_000000F0 + 0x38)($at)
    /* DDCE24 0000C3B8 3C014080 */  lui        $at, (0x40800000 >> 16)
    /* DDCE28 0000C3BC C7A800F0 */  lwc1       $f8, 0xF0($sp)
    /* DDCE2C 0000C3C0 460A2480 */  add.s      $f18, $f4, $f10
    /* DDCE30 0000C3C4 44812000 */  mtc1       $at, $f4
    /* DDCE34 0000C3C8 46123002 */  mul.s      $f0, $f6, $f18
    /* DDCE38 0000C3CC 46040280 */  add.s      $f10, $f0, $f4
    /* DDCE3C 0000C3D0 460A403C */  c.lt.s     $f8, $f10
    /* DDCE40 0000C3D4 00000000 */  nop
    /* DDCE44 0000C3D8 45030006 */  bc1tl      .L0000C3F4
    /* DDCE48 0000C3DC AE0D0A4C */   sw        $t5, 0xA4C($s0)
    /* DDCE4C 0000C3E0 8E0C0938 */  lw         $t4, 0x938($s0)
    /* DDCE50 0000C3E4 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DDCE54 0000C3E8 5180001E */  beql       $t4, $zero, .L0000C464
    /* DDCE58 0000C3EC 44812000 */   mtc1      $at, $f4
    /* DDCE5C 0000C3F0 AE0D0A4C */  sw         $t5, 0xA4C($s0)
  .L0000C3F4:
    /* DDCE60 0000C3F4 8DF90000 */  lw         $t9, 0x0($t7)
    /* DDCE64 0000C3F8 27AE00B4 */  addiu      $t6, $sp, 0xB4
    /* DDCE68 0000C3FC 3C010000 */  lui        $at, %hi(.L00000930)
    /* DDCE6C 0000C400 ADD90000 */  sw         $t9, 0x0($t6)
    /* DDCE70 0000C404 8DF80004 */  lw         $t8, 0x4($t7)
    /* DDCE74 0000C408 00004025 */  or         $t0, $zero, $zero
    /* DDCE78 0000C40C ADD80004 */  sw         $t8, 0x4($t6)
    /* DDCE7C 0000C410 8DF90008 */  lw         $t9, 0x8($t7)
    /* DDCE80 0000C414 ADD90008 */  sw         $t9, 0x8($t6)
    /* DDCE84 0000C418 C7A600B4 */  lwc1       $f6, 0xB4($sp)
    /* DDCE88 0000C41C E60609D8 */  swc1       $f6, 0x9D8($s0)
    /* DDCE8C 0000C420 C7B200B8 */  lwc1       $f18, 0xB8($sp)
    /* DDCE90 0000C424 E61209DC */  swc1       $f18, 0x9DC($s0)
    /* DDCE94 0000C428 C7A400BC */  lwc1       $f4, 0xBC($sp)
    /* DDCE98 0000C42C C60609DC */  lwc1       $f6, 0x9DC($s0)
    /* DDCE9C 0000C430 E60409E0 */  swc1       $f4, 0x9E0($s0)
    /* DDCEA0 0000C434 C7A800F0 */  lwc1       $f8, 0xF0($sp)
    /* DDCEA4 0000C438 460034A1 */  cvt.d.s    $f18, $f6
    /* DDCEA8 0000C43C E60809E4 */  swc1       $f8, 0x9E4($s0)
    /* DDCEAC 0000C440 D42A0930 */  ldc1       $f10, %lo(.L00000930)($at)
    /* DDCEB0 0000C444 4632503C */  c.lt.d     $f10, $f18
    /* DDCEB4 0000C448 00000000 */  nop
    /* DDCEB8 0000C44C 45000002 */  bc1f       .L0000C458
    /* DDCEBC 0000C450 00000000 */   nop
    /* DDCEC0 0000C454 24080001 */  addiu      $t0, $zero, 0x1
  .L0000C458:
    /* DDCEC4 0000C458 10000008 */  b          .L0000C47C
    /* DDCEC8 0000C45C AE080A50 */   sw        $t0, 0xA50($s0)
    /* DDCECC 0000C460 44812000 */  mtc1       $at, $f4
  .L0000C464:
    /* DDCED0 0000C464 AE000A4C */  sw         $zero, 0xA4C($s0)
    /* DDCED4 0000C468 E61009D8 */  swc1       $f16, 0x9D8($s0)
    /* DDCED8 0000C46C E61009E0 */  swc1       $f16, 0x9E0($s0)
    /* DDCEDC 0000C470 E61009E4 */  swc1       $f16, 0x9E4($s0)
    /* DDCEE0 0000C474 AE000A50 */  sw         $zero, 0xA50($s0)
    /* DDCEE4 0000C478 E60409DC */  swc1       $f4, 0x9DC($s0)
  .L0000C47C:
    /* DDCEE8 0000C47C C60809D0 */  lwc1       $f8, 0x9D0($s0)
    /* DDCEEC 0000C480 C7A600E8 */  lwc1       $f6, 0xE8($sp)
    /* DDCEF0 0000C484 4610403C */  c.lt.s     $f8, $f16
    /* DDCEF4 0000C488 00000000 */  nop
    /* DDCEF8 0000C48C 45020009 */  bc1fl      .L0000C4B4
    /* DDCEFC 0000C490 8E090938 */   lw        $t1, 0x938($s0)
    /* DDCF00 0000C494 4606803C */  c.lt.s     $f16, $f6
    /* DDCF04 0000C498 3C014120 */  lui        $at, (0x41200000 >> 16)
    /* DDCF08 0000C49C 45020005 */  bc1fl      .L0000C4B4
    /* DDCF0C 0000C4A0 8E090938 */   lw        $t1, 0x938($s0)
    /* DDCF10 0000C4A4 44815000 */  mtc1       $at, $f10
    /* DDCF14 0000C4A8 00000000 */  nop
    /* DDCF18 0000C4AC E60A031C */  swc1       $f10, 0x31C($s0)
    /* DDCF1C 0000C4B0 8E090938 */  lw         $t1, 0x938($s0)
  .L0000C4B4:
    /* DDCF20 0000C4B4 3C014120 */  lui        $at, (0x41200000 >> 16)
    /* DDCF24 0000C4B8 5120002F */  beql       $t1, $zero, .L0000C578
    /* DDCF28 0000C4BC 860209A0 */   lh        $v0, 0x9A0($s0)
    /* DDCF2C 0000C4C0 44819000 */  mtc1       $at, $f18
    /* DDCF30 0000C4C4 C6040348 */  lwc1       $f4, 0x348($s0)
    /* DDCF34 0000C4C8 4604903C */  c.lt.s     $f18, $f4
    /* DDCF38 0000C4CC 00000000 */  nop
    /* DDCF3C 0000C4D0 45020029 */  bc1fl      .L0000C578
    /* DDCF40 0000C4D4 860209A0 */   lh        $v0, 0x9A0($s0)
    /* DDCF44 0000C4D8 860309A0 */  lh         $v1, 0x9A0($s0)
    /* DDCF48 0000C4DC 24040064 */  addiu      $a0, $zero, 0x64
    /* DDCF4C 0000C4E0 00001025 */  or         $v0, $zero, $zero
    /* DDCF50 0000C4E4 14830004 */  bne        $a0, $v1, .L0000C4F8
    /* DDCF54 0000C4E8 24010063 */   addiu     $at, $zero, 0x63
  alabel D_0000C4EC
    /* DDCF58 0000C4EC AE040A5C */  sw         $a0, 0xA5C($s0)
    /* DDCF5C 0000C4F0 10000005 */  b          .L0000C508
    /* DDCF60 0000C4F4 24020002 */   addiu     $v0, $zero, 0x2
  .L0000C4F8:
    /* DDCF64 0000C4F8 14610003 */  bne        $v1, $at, .L0000C508
    /* DDCF68 0000C4FC 240A0046 */   addiu     $t2, $zero, 0x46
    /* DDCF6C 0000C500 AE0A0A5C */  sw         $t2, 0xA5C($s0)
    /* DDCF70 0000C504 24020001 */  addiu      $v0, $zero, 0x1
  .L0000C508:
    /* DDCF74 0000C508 1040001A */  beqz       $v0, .L0000C574
    /* DDCF78 0000C50C 3C014040 */   lui       $at, (0x40400000 >> 16)
    /* DDCF7C 0000C510 C6020970 */  lwc1       $f2, 0x970($s0)
    /* DDCF80 0000C514 44824000 */  mtc1       $v0, $f8
    /* DDCF84 0000C518 4610103C */  c.lt.s     $f2, $f16
    /* DDCF88 0000C51C 00000000 */  nop
    /* DDCF8C 0000C520 45020004 */  bc1fl      .L0000C534
    /* DDCF90 0000C524 46001006 */   mov.s     $f0, $f2
    /* DDCF94 0000C528 10000002 */  b          .L0000C534
    /* DDCF98 0000C52C 46001007 */   neg.s     $f0, $f2
    /* DDCF9C 0000C530 46001006 */  mov.s      $f0, $f2
  .L0000C534:
    /* DDCFA0 0000C534 44815000 */  mtc1       $at, $f10
    /* DDCFA4 0000C538 468041A0 */  cvt.s.w    $f6, $f8
    /* DDCFA8 0000C53C 3C050001 */  lui        $a1, %hi(D_000081C0)
    /* DDCFAC 0000C540 460A0482 */  mul.s      $f18, $f0, $f10
    /* DDCFB0 0000C544 8E040800 */  lw         $a0, 0x800($s0)
    /* DDCFB4 0000C548 24060002 */  addiu      $a2, $zero, 0x2
    /* DDCFB8 0000C54C 46123100 */  add.s      $f4, $f6, $f18
  alabel D_0000C550
    /* DDCFBC 0000C550 4600220D */  trunc.w.s  $f8, $f4
    /* DDCFC0 0000C554 44024000 */  mfc1       $v0, $f8
    /* DDCFC4 0000C558 00000000 */  nop
    /* DDCFC8 0000C55C 00026080 */  sll        $t4, $v0, 2
    /* DDCFCC 0000C560 00AC2821 */  addu       $a1, $a1, $t4
    /* DDCFD0 0000C564 0C000000 */  jal        func_00000000
    /* DDCFD4 0000C568 8CA581C0 */   lw        $a1, %lo(D_000081C0)($a1)
    /* DDCFD8 0000C56C 44808000 */  mtc1       $zero, $f16
    /* DDCFDC 0000C570 00000000 */  nop
  .L0000C574:
    /* DDCFE0 0000C574 860209A0 */  lh         $v0, 0x9A0($s0)
  .L0000C578:
    /* DDCFE4 0000C578 24010061 */  addiu      $at, $zero, 0x61
  alabel D_0000C57C
    /* DDCFE8 0000C57C 3C0D0000 */  lui        $t5, (0x0 >> 16)
    /* DDCFEC 0000C580 10410014 */  beq        $v0, $at, .L0000C5D4
    /* DDCFF0 0000C584 26060A30 */   addiu     $a2, $s0, 0xA30
    /* DDCFF4 0000C588 24010062 */  addiu      $at, $zero, 0x62
  alabel D_0000C58C
    /* DDCFF8 0000C58C 10410020 */  beq        $v0, $at, .L0000C610
    /* DDCFFC 0000C590 3C0E0000 */   lui       $t6, (0x0 >> 16)
  alabel D_0000C594
    /* DDD000 0000C594 3C010000 */  lui        $at, %hi(D_00000938)
    /* DDD004 0000C598 C42A0938 */  lwc1       $f10, %lo(D_00000938)($at)
    /* DDD008 0000C59C 3C010000 */  lui        $at, %hi(D_0000093C)
    /* DDD00C 0000C5A0 C60405A0 */  lwc1       $f4, 0x5A0($s0)
  alabel D_0000C5A4
    /* DDD010 0000C5A4 E60A0288 */  swc1       $f10, 0x288($s0)
    /* DDD014 0000C5A8 C426093C */  lwc1       $f6, %lo(D_0000093C)($at)
  alabel D_0000C5AC
    /* DDD018 0000C5AC 3C010000 */  lui        $at, %hi(func_00000940)
    /* DDD01C 0000C5B0 E606028C */  swc1       $f6, 0x28C($s0)
    /* DDD020 0000C5B4 C4320940 */  lwc1       $f18, %lo(func_00000940)($at)
    /* DDD024 0000C5B8 A2000A33 */  sb         $zero, 0xA33($s0)
  alabel D_0000C5BC
    /* DDD028 0000C5BC A2000A32 */  sb         $zero, 0xA32($s0)
    /* DDD02C 0000C5C0 A2000A31 */  sb         $zero, 0xA31($s0)
  alabel D_0000C5C4
    /* DDD030 0000C5C4 A2000A30 */  sb         $zero, 0xA30($s0)
    /* DDD034 0000C5C8 E6040910 */  swc1       $f4, 0x910($s0)
    /* DDD038 0000C5CC 10000020 */  b          .L0000C650
  alabel D_0000C5D0
    /* DDD03C 0000C5D0 E6120290 */   swc1      $f18, 0x290($s0)
  .L0000C5D4:
    /* DDD040 0000C5D4 C6080120 */  lwc1       $f8, 0x120($s0)
    /* DDD044 0000C5D8 C60A0138 */  lwc1       $f10, 0x138($s0)
  alabel D_0000C5DC
    /* DDD048 0000C5DC C60601B0 */  lwc1       $f6, 0x1B0($s0)
    /* DDD04C 0000C5E0 C6120570 */  lwc1       $f18, 0x570($s0)
  alabel D_0000C5E4
    /* DDD050 0000C5E4 E6080288 */  swc1       $f8, 0x288($s0)
    /* DDD054 0000C5E8 E60A028C */  swc1       $f10, 0x28C($s0)
    /* DDD058 0000C5EC E6060290 */  swc1       $f6, 0x290($s0)
  alabel D_0000C5F0
    /* DDD05C 0000C5F0 E6120910 */  swc1       $f18, 0x910($s0)
    /* DDD060 0000C5F4 8DAD0000 */  lw         $t5, 0x0($t5)
    /* DDD064 0000C5F8 87A500DE */  lh         $a1, 0xDE($sp)
    /* DDD068 0000C5FC 0C000000 */  jal        func_00000000
    /* DDD06C 0000C600 8DA40084 */   lw        $a0, 0x84($t5)
  alabel D_0000C604
    /* DDD070 0000C604 44808000 */  mtc1       $zero, $f16
    /* DDD074 0000C608 10000011 */  b          .L0000C650
  alabel D_0000C60C
    /* DDD078 0000C60C 00000000 */   nop
  .L0000C610:
    /* DDD07C 0000C610 E6100288 */  swc1       $f16, 0x288($s0)
    /* DDD080 0000C614 3C010000 */  lui        $at, %hi(func_00000940 + 0x4)
    /* DDD084 0000C618 C4240944 */  lwc1       $f4, %lo(func_00000940 + 0x4)($at)
    /* DDD088 0000C61C 3C010000 */  lui        $at, %hi(func_00000940 + 0x8)
    /* DDD08C 0000C620 C60A0588 */  lwc1       $f10, 0x588($s0)
  alabel D_0000C624
    /* DDD090 0000C624 E604028C */  swc1       $f4, 0x28C($s0)
    /* DDD094 0000C628 C4280948 */  lwc1       $f8, %lo(func_00000940 + 0x8)($at)
    /* DDD098 0000C62C E60A0910 */  swc1       $f10, 0x910($s0)
    /* DDD09C 0000C630 26060A30 */  addiu      $a2, $s0, 0xA30
    /* DDD0A0 0000C634 E6080290 */  swc1       $f8, 0x290($s0)
    /* DDD0A4 0000C638 8DCE0000 */  lw         $t6, 0x0($t6)
    /* DDD0A8 0000C63C 87A500DE */  lh         $a1, 0xDE($sp)
    /* DDD0AC 0000C640 0C000000 */  jal        func_00000000
    /* DDD0B0 0000C644 8DC40084 */   lw        $a0, 0x84($t6)
    /* DDD0B4 0000C648 44808000 */  mtc1       $zero, $f16
    /* DDD0B8 0000C64C 00000000 */  nop
  .L0000C650:
    /* DDD0BC 0000C650 1000003B */  b          .L0000C740
    /* DDD0C0 0000C654 AE00095C */   sw        $zero, 0x95C($s0)
  .L0000C658:
    /* DDD0C4 0000C658 8E03095C */  lw         $v1, 0x95C($s0)
  alabel D_0000C65C
    /* DDD0C8 0000C65C 44813000 */  mtc1       $at, $f6
    /* DDD0CC 0000C660 AE000A4C */  sw         $zero, 0xA4C($s0)
    /* DDD0D0 0000C664 28610097 */  slti       $at, $v1, 0x97
    /* DDD0D4 0000C668 246F0001 */  addiu      $t7, $v1, 0x1
    /* DDD0D8 0000C66C E61009D8 */  swc1       $f16, 0x9D8($s0)
    /* DDD0DC 0000C670 E61009E0 */  swc1       $f16, 0x9E0($s0)
    /* DDD0E0 0000C674 E61009E4 */  swc1       $f16, 0x9E4($s0)
    /* DDD0E4 0000C678 AE0F095C */  sw         $t7, 0x95C($s0)
    /* DDD0E8 0000C67C 14200030 */  bnez       $at, .L0000C740
    /* DDD0EC 0000C680 E60609DC */   swc1      $f6, 0x9DC($s0)
    /* DDD0F0 0000C684 3C180001 */  lui        $t8, %hi(func_00008A40 + 0x38)
    /* DDD0F4 0000C688 8F058A78 */  lw         $a1, %lo(func_00008A40 + 0x38)($t8)
    /* DDD0F8 0000C68C 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDD0FC 0000C690 8C840000 */  lw         $a0, 0x0($a0)
  alabel D_0000C694
    /* DDD100 0000C694 8FA6003C */  lw         $a2, 0x3C($sp)
    /* DDD104 0000C698 00003825 */  or         $a3, $zero, $zero
  alabel D_0000C69C
    /* DDD108 0000C69C AFA5008C */  sw         $a1, 0x8C($sp)
    /* DDD10C 0000C6A0 0C000000 */  jal        func_00000000
    /* DDD110 0000C6A4 AFA50004 */   sw        $a1, 0x4($sp)
    /* DDD114 0000C6A8 44808000 */  mtc1       $zero, $f16
    /* DDD118 0000C6AC 50400025 */  beql       $v0, $zero, .L0000C744
    /* DDD11C 0000C6B0 44800000 */   mtc1      $zero, $f0
    /* DDD120 0000C6B4 8C490030 */  lw         $t1, 0x30($v0)
    /* DDD124 0000C6B8 27A5007C */  addiu      $a1, $sp, 0x7C
    /* DDD128 0000C6BC 3C014348 */  lui        $at, (0x43480000 >> 16)
    /* DDD12C 0000C6C0 ACA90000 */  sw         $t1, 0x0($a1)
    /* DDD130 0000C6C4 8C480034 */  lw         $t0, 0x34($v0)
    /* DDD134 0000C6C8 44812000 */  mtc1       $at, $f4
    /* DDD138 0000C6CC 02002025 */  or         $a0, $s0, $zero
    /* DDD13C 0000C6D0 ACA80004 */  sw         $t0, 0x4($a1)
    /* DDD140 0000C6D4 8C490038 */  lw         $t1, 0x38($v0)
    /* DDD144 0000C6D8 ACA90008 */  sw         $t1, 0x8($a1)
    /* DDD148 0000C6DC C7B20080 */  lwc1       $f18, 0x80($sp)
    /* DDD14C 0000C6E0 46049200 */  add.s      $f8, $f18, $f4
    /* DDD150 0000C6E4 0C000000 */  jal        func_00000000
    /* DDD154 0000C6E8 E7A80080 */   swc1      $f8, 0x80($sp)
    /* DDD158 0000C6EC 3C040001 */  lui        $a0, %hi(func_00008A7C)
    /* DDD15C 0000C6F0 AE00095C */  sw         $zero, 0x95C($s0)
    /* DDD160 0000C6F4 0C000000 */  jal        func_00000000
    /* DDD164 0000C6F8 24848A7C */   addiu     $a0, $a0, %lo(func_00008A7C)
    /* DDD168 0000C6FC C7AA007C */  lwc1       $f10, 0x7C($sp)
    /* DDD16C 0000C700 C7B20080 */  lwc1       $f18, 0x80($sp)
    /* DDD170 0000C704 C7A80084 */  lwc1       $f8, 0x84($sp)
    /* DDD174 0000C708 460051A1 */  cvt.d.s    $f6, $f10
    /* DDD178 0000C70C 46009121 */  cvt.d.s    $f4, $f18
    /* DDD17C 0000C710 460042A1 */  cvt.d.s    $f10, $f8
    /* DDD180 0000C714 44073000 */  mfc1       $a3, $f6
    /* DDD184 0000C718 44063800 */  mfc1       $a2, $f7
    /* DDD188 0000C71C 3C040001 */  lui        $a0, %hi(func_00008A7C + 0x18)
    /* DDD18C 0000C720 3C050001 */  lui        $a1, %hi(func_00008A7C + 0x14)
    /* DDD190 0000C724 24A58A90 */  addiu      $a1, $a1, %lo(func_00008A7C + 0x14)
    /* DDD194 0000C728 24848A94 */  addiu      $a0, $a0, %lo(func_00008A7C + 0x18)
    /* DDD198 0000C72C F7AA0018 */  sdc1       $f10, 0x18($sp)
  alabel D_0000C730
    /* DDD19C 0000C730 0C000000 */  jal        func_00000000
    /* DDD1A0 0000C734 F7A40010 */   sdc1      $f4, 0x10($sp)
  alabel D_0000C738
    /* DDD1A4 0000C738 44808000 */  mtc1       $zero, $f16
    /* DDD1A8 0000C73C 00000000 */  nop
  .L0000C740:
    /* DDD1AC 0000C740 44800000 */  mtc1       $zero, $f0
  .L0000C744:
    /* DDD1B0 0000C744 3C040001 */  lui        $a0, %hi(func_00008A7C + 0x28)
    /* DDD1B4 0000C748 E6100968 */  swc1       $f16, 0x968($s0)
    /* DDD1B8 0000C74C AE000938 */  sw         $zero, 0x938($s0)
    /* DDD1BC 0000C750 24848AA4 */  addiu      $a0, $a0, %lo(func_00008A7C + 0x28)
    /* DDD1C0 0000C754 E600093C */  swc1       $f0, 0x93C($s0)
    /* DDD1C4 0000C758 E6000940 */  swc1       $f0, 0x940($s0)
    /* DDD1C8 0000C75C 0C000000 */  jal        func_00000000
    /* DDD1CC 0000C760 E6000944 */   swc1      $f0, 0x944($s0)
  alabel D_0000C764
    /* DDD1D0 0000C764 0C000000 */  jal        func_00000000
    /* DDD1D4 0000C768 02002025 */   or        $a0, $s0, $zero
    /* DDD1D8 0000C76C 3C040001 */  lui        $a0, %hi(func_00008A7C + 0x34)
    /* DDD1DC 0000C770 0C000000 */  jal        func_00000000
    /* DDD1E0 0000C774 24848AB0 */   addiu     $a0, $a0, %lo(func_00008A7C + 0x34)
    /* DDD1E4 0000C778 8E0A09A8 */  lw         $t2, 0x9A8($s0)
    /* DDD1E8 0000C77C 24050001 */  addiu      $a1, $zero, 0x1
    /* DDD1EC 0000C780 26040808 */  addiu      $a0, $s0, 0x808
    /* DDD1F0 0000C784 314B0400 */  andi       $t3, $t2, 0x400
    /* DDD1F4 0000C788 11600007 */  beqz       $t3, .L0000C7A8
    /* DDD1F8 0000C78C 00000000 */   nop
    /* DDD1FC 0000C790 0C000000 */  jal        func_00000000
    /* DDD200 0000C794 26040808 */   addiu     $a0, $s0, 0x808
    /* DDD204 0000C798 44808000 */  mtc1       $zero, $f16
    /* DDD208 0000C79C 3C010000 */  lui        $at, %hi(func_00000940 + 0xC)
    /* DDD20C 0000C7A0 10000007 */  b          .L0000C7C0
    /* DDD210 0000C7A4 C422094C */   lwc1      $f2, %lo(func_00000940 + 0xC)($at)
  .L0000C7A8:
    /* DDD214 0000C7A8 0C000000 */  jal        func_00000000
    /* DDD218 0000C7AC 00002825 */   or        $a1, $zero, $zero
    /* DDD21C 0000C7B0 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DDD220 0000C7B4 44808000 */  mtc1       $zero, $f16
    /* DDD224 0000C7B8 44811000 */  mtc1       $at, $f2
    /* DDD228 0000C7BC 00000000 */  nop
  .L0000C7C0:
    /* DDD22C 0000C7C0 3C0C0000 */  lui        $t4, %hi(func_00000008 + 0x2C)
    /* DDD230 0000C7C4 8D8C0034 */  lw         $t4, %lo(func_00000008 + 0x2C)($t4)
    /* DDD234 0000C7C8 24010002 */  addiu      $at, $zero, 0x2
    /* DDD238 0000C7CC 51810011 */  beql       $t4, $at, .L0000C814
    /* DDD23C 0000C7D0 8E020938 */   lw        $v0, 0x938($s0)
    /* DDD240 0000C7D4 C600091C */  lwc1       $f0, 0x91C($s0)
    /* DDD244 0000C7D8 3C010000 */  lui        $at, %hi(func_00000940 + 0x10)
    /* DDD248 0000C7DC D4240950 */  ldc1       $f4, %lo(func_00000940 + 0x10)($at)
    /* DDD24C 0000C7E0 46001181 */  sub.s      $f6, $f2, $f0
    /* DDD250 0000C7E4 8E040850 */  lw         $a0, 0x850($s0)
    /* DDD254 0000C7E8 460002A1 */  cvt.d.s    $f10, $f0
    /* DDD258 0000C7EC 460034A1 */  cvt.d.s    $f18, $f6
    /* DDD25C 0000C7F0 46249202 */  mul.d      $f8, $f18, $f4
    /* DDD260 0000C7F4 46285180 */  add.d      $f6, $f10, $f8
    /* DDD264 0000C7F8 462034A0 */  cvt.s.d    $f18, $f6
    /* DDD268 0000C7FC E612091C */  swc1       $f18, 0x91C($s0)
    /* DDD26C 0000C800 0C000000 */  jal        func_00000000
    /* DDD270 0000C804 8E05091C */   lw        $a1, 0x91C($s0)
    /* DDD274 0000C808 44808000 */  mtc1       $zero, $f16
    /* DDD278 0000C80C 00000000 */  nop
    /* DDD27C 0000C810 8E020938 */  lw         $v0, 0x938($s0)
  .L0000C814:
    /* DDD280 0000C814 50400079 */  beql       $v0, $zero, .L0000C9FC
    /* DDD284 0000C818 8E0C09D4 */   lw        $t4, 0x9D4($s0)
    /* DDD288 0000C81C 44822000 */  mtc1       $v0, $f4
    /* DDD28C 0000C820 C60A093C */  lwc1       $f10, 0x93C($s0)
    /* DDD290 0000C824 C6060940 */  lwc1       $f6, 0x940($s0)
    /* DDD294 0000C828 468020A0 */  cvt.s.w    $f2, $f4
    /* DDD298 0000C82C C6040944 */  lwc1       $f4, 0x944($s0)
    /* DDD29C 0000C830 2602093C */  addiu      $v0, $s0, 0x93C
    /* DDD2A0 0000C834 46025203 */  div.s      $f8, $f10, $f2
    /* DDD2A4 0000C838 46023483 */  div.s      $f18, $f6, $f2
  alabel D_0000C83C
    /* DDD2A8 0000C83C E608093C */  swc1       $f8, 0x93C($s0)
    /* DDD2AC 0000C840 C6080968 */  lwc1       $f8, 0x968($s0)
    /* DDD2B0 0000C844 46024183 */  div.s      $f6, $f8, $f2
    /* DDD2B4 0000C848 E6120940 */  swc1       $f18, 0x940($s0)
    /* DDD2B8 0000C84C 46022283 */  div.s      $f10, $f4, $f2
    /* DDD2BC 0000C850 E6060968 */  swc1       $f6, 0x968($s0)
    /* DDD2C0 0000C854 C60C0968 */  lwc1       $f12, 0x968($s0)
    /* DDD2C4 0000C858 4610603C */  c.lt.s     $f12, $f16
    /* DDD2C8 0000C85C 00000000 */  nop
    /* DDD2CC 0000C860 45000023 */  bc1f       .L0000C8F0
    /* DDD2D0 0000C864 E60A0944 */   swc1      $f10, 0x944($s0)
    /* DDD2D4 0000C868 2602093C */  addiu      $v0, $s0, 0x93C
    /* DDD2D8 0000C86C C4520000 */  lwc1       $f18, 0x0($v0)
    /* DDD2DC 0000C870 C4440004 */  lwc1       $f4, 0x4($v0)
    /* DDD2E0 0000C874 46006006 */  mov.s      $f0, $f12
    /* DDD2E4 0000C878 460C9082 */  mul.s      $f2, $f18, $f12
    /* DDD2E8 0000C87C C44A0008 */  lwc1       $f10, 0x8($v0)
    /* DDD2EC 0000C880 27AE0068 */  addiu      $t6, $sp, 0x68
    /* DDD2F0 0000C884 460C2302 */  mul.s      $f12, $f4, $f12
    /* DDD2F4 0000C888 27AD00B4 */  addiu      $t5, $sp, 0xB4
    /* DDD2F8 0000C88C 27A800B4 */  addiu      $t0, $sp, 0xB4
    /* DDD2FC 0000C890 46005382 */  mul.s      $f14, $f10, $f0
    /* DDD300 0000C894 E7A20068 */  swc1       $f2, 0x68($sp)
    /* DDD304 0000C898 27B90054 */  addiu      $t9, $sp, 0x54
    /* DDD308 0000C89C E7AC006C */  swc1       $f12, 0x6C($sp)
    /* DDD30C 0000C8A0 E7AE0070 */  swc1       $f14, 0x70($sp)
    /* DDD310 0000C8A4 8DD80000 */  lw         $t8, 0x0($t6)
    /* DDD314 0000C8A8 ADB80000 */  sw         $t8, 0x0($t5)
    /* DDD318 0000C8AC 8DCF0004 */  lw         $t7, 0x4($t6)
    /* DDD31C 0000C8B0 ADAF0004 */  sw         $t7, 0x4($t5)
    /* DDD320 0000C8B4 8DD80008 */  lw         $t8, 0x8($t6)
    /* DDD324 0000C8B8 ADB80008 */  sw         $t8, 0x8($t5)
    /* DDD328 0000C8BC 8D0A0000 */  lw         $t2, 0x0($t0)
    /* DDD32C 0000C8C0 AF2A0000 */  sw         $t2, 0x0($t9)
    /* DDD330 0000C8C4 8D090004 */  lw         $t1, 0x4($t0)
    /* DDD334 0000C8C8 AF290004 */  sw         $t1, 0x4($t9)
    /* DDD338 0000C8CC 8D0A0008 */  lw         $t2, 0x8($t0)
    /* DDD33C 0000C8D0 AF2A0008 */  sw         $t2, 0x8($t9)
    /* DDD340 0000C8D4 C7A80054 */  lwc1       $f8, 0x54($sp)
    /* DDD344 0000C8D8 E6080920 */  swc1       $f8, 0x920($s0)
    /* DDD348 0000C8DC C7A60058 */  lwc1       $f6, 0x58($sp)
    /* DDD34C 0000C8E0 E6060924 */  swc1       $f6, 0x924($s0)
    /* DDD350 0000C8E4 C7B2005C */  lwc1       $f18, 0x5C($sp)
    /* DDD354 0000C8E8 10000004 */  b          .L0000C8FC
    /* DDD358 0000C8EC E6120928 */   swc1      $f18, 0x928($s0)
  .L0000C8F0:
    /* DDD35C 0000C8F0 E6100920 */  swc1       $f16, 0x920($s0)
    /* DDD360 0000C8F4 E6100924 */  swc1       $f16, 0x924($s0)
    /* DDD364 0000C8F8 E6100928 */  swc1       $f16, 0x928($s0)
  .L0000C8FC:
    /* DDD368 0000C8FC 8C4D0000 */  lw         $t5, 0x0($v0)
    /* DDD36C 0000C900 27AB00B4 */  addiu      $t3, $sp, 0xB4
    /* DDD370 0000C904 AD6D0000 */  sw         $t5, 0x0($t3)
    /* DDD374 0000C908 8C4C0004 */  lw         $t4, 0x4($v0)
    /* DDD378 0000C90C AD6C0004 */  sw         $t4, 0x4($t3)
    /* DDD37C 0000C910 8C4D0008 */  lw         $t5, 0x8($v0)
    /* DDD380 0000C914 AD6D0008 */  sw         $t5, 0x8($t3)
    /* DDD384 0000C918 C7A400B4 */  lwc1       $f4, 0xB4($sp)
    /* DDD388 0000C91C C612093C */  lwc1       $f18, 0x93C($s0)
    /* DDD38C 0000C920 C606033C */  lwc1       $f6, 0x33C($s0)
    /* DDD390 0000C924 E6040948 */  swc1       $f4, 0x948($s0)
    /* DDD394 0000C928 C7AA00B8 */  lwc1       $f10, 0xB8($sp)
    /* DDD398 0000C92C 46123102 */  mul.s      $f4, $f6, $f18
    /* DDD39C 0000C930 E60A094C */  swc1       $f10, 0x94C($s0)
    /* DDD3A0 0000C934 C7A800BC */  lwc1       $f8, 0xBC($sp)
    /* DDD3A4 0000C938 C60A0340 */  lwc1       $f10, 0x340($s0)
    /* DDD3A8 0000C93C E6080950 */  swc1       $f8, 0x950($s0)
    /* DDD3AC 0000C940 C6080940 */  lwc1       $f8, 0x940($s0)
    /* DDD3B0 0000C944 46085182 */  mul.s      $f6, $f10, $f8
    /* DDD3B4 0000C948 C6080944 */  lwc1       $f8, 0x944($s0)
    /* DDD3B8 0000C94C C60A0344 */  lwc1       $f10, 0x344($s0)
    /* DDD3BC 0000C950 46062480 */  add.s      $f18, $f4, $f6
    /* DDD3C0 0000C954 46085102 */  mul.s      $f4, $f10, $f8
    /* DDD3C4 0000C958 46049080 */  add.s      $f2, $f18, $f4
    /* DDD3C8 0000C95C 4602803C */  c.lt.s     $f16, $f2
    /* DDD3CC 0000C960 00000000 */  nop
    /* DDD3D0 0000C964 45020003 */  bc1fl      .L0000C974
    /* DDD3D4 0000C968 C4460000 */   lwc1      $f6, 0x0($v0)
    /* DDD3D8 0000C96C 46008086 */  mov.s      $f2, $f16
    /* DDD3DC 0000C970 C4460000 */  lwc1       $f6, 0x0($v0)
  .L0000C974:
    /* DDD3E0 0000C974 46001007 */  neg.s      $f0, $f2
    /* DDD3E4 0000C978 C44A0004 */  lwc1       $f10, 0x4($v0)
    /* DDD3E8 0000C97C 46003082 */  mul.s      $f2, $f6, $f0
    /* DDD3EC 0000C980 C4480008 */  lwc1       $f8, 0x8($v0)
    /* DDD3F0 0000C984 27AF0040 */  addiu      $t7, $sp, 0x40
    /* DDD3F4 0000C988 46005302 */  mul.s      $f12, $f10, $f0
    /* DDD3F8 0000C98C 27AE00B4 */  addiu      $t6, $sp, 0xB4
    /* DDD3FC 0000C990 27A900B4 */  addiu      $t1, $sp, 0xB4
    /* DDD400 0000C994 46004382 */  mul.s      $f14, $f8, $f0
    /* DDD404 0000C998 E7A20040 */  swc1       $f2, 0x40($sp)
    /* DDD408 0000C99C 27A80054 */  addiu      $t0, $sp, 0x54
    /* DDD40C 0000C9A0 E7AC0044 */  swc1       $f12, 0x44($sp)
    /* DDD410 0000C9A4 E7AE0048 */  swc1       $f14, 0x48($sp)
    /* DDD414 0000C9A8 8DF90000 */  lw         $t9, 0x0($t7)
    /* DDD418 0000C9AC ADD90000 */  sw         $t9, 0x0($t6)
    /* DDD41C 0000C9B0 8DF80004 */  lw         $t8, 0x4($t7)
    /* DDD420 0000C9B4 ADD80004 */  sw         $t8, 0x4($t6)
    /* DDD424 0000C9B8 8DF90008 */  lw         $t9, 0x8($t7)
    /* DDD428 0000C9BC ADD90008 */  sw         $t9, 0x8($t6)
    /* DDD42C 0000C9C0 8D2B0000 */  lw         $t3, 0x0($t1)
    /* DDD430 0000C9C4 AD0B0000 */  sw         $t3, 0x0($t0)
    /* DDD434 0000C9C8 8D2A0004 */  lw         $t2, 0x4($t1)
    /* DDD438 0000C9CC AD0A0004 */  sw         $t2, 0x4($t0)
    /* DDD43C 0000C9D0 8D2B0008 */  lw         $t3, 0x8($t1)
    /* DDD440 0000C9D4 AD0B0008 */  sw         $t3, 0x8($t0)
    /* DDD444 0000C9D8 C7B20054 */  lwc1       $f18, 0x54($sp)
    /* DDD448 0000C9DC E612092C */  swc1       $f18, 0x92C($s0)
    /* DDD44C 0000C9E0 C7A40058 */  lwc1       $f4, 0x58($sp)
    /* DDD450 0000C9E4 E6040930 */  swc1       $f4, 0x930($s0)
    /* DDD454 0000C9E8 C7A6005C */  lwc1       $f6, 0x5C($sp)
    /* DDD458 0000C9EC AE0009D4 */  sw         $zero, 0x9D4($s0)
    /* DDD45C 0000C9F0 1000000A */  b          .L0000CA1C
    /* DDD460 0000C9F4 E6060934 */   swc1      $f6, 0x934($s0)
    /* DDD464 0000C9F8 8E0C09D4 */  lw         $t4, 0x9D4($s0)
  .L0000C9FC:
    /* DDD468 0000C9FC E6100920 */  swc1       $f16, 0x920($s0)
    /* DDD46C 0000CA00 E6100924 */  swc1       $f16, 0x924($s0)
    /* DDD470 0000CA04 258D0001 */  addiu      $t5, $t4, 0x1
    /* DDD474 0000CA08 AE0D09D4 */  sw         $t5, 0x9D4($s0)
    /* DDD478 0000CA0C E6100928 */  swc1       $f16, 0x928($s0)
    /* DDD47C 0000CA10 E610092C */  swc1       $f16, 0x92C($s0)
    /* DDD480 0000CA14 E6100930 */  swc1       $f16, 0x930($s0)
    /* DDD484 0000CA18 E6100934 */  swc1       $f16, 0x934($s0)
  .L0000CA1C:
    /* DDD488 0000CA1C C602092C */  lwc1       $f2, 0x92C($s0)
    /* DDD48C 0000CA20 C60A03BC */  lwc1       $f10, 0x3BC($s0)
    /* DDD490 0000CA24 C60C0930 */  lwc1       $f12, 0x930($s0)
    /* DDD494 0000CA28 C61203C0 */  lwc1       $f18, 0x3C0($s0)
    /* DDD498 0000CA2C 46025202 */  mul.s      $f8, $f10, $f2
    /* DDD49C 0000CA30 C60A03C4 */  lwc1       $f10, 0x3C4($s0)
    /* DDD4A0 0000CA34 C60E0934 */  lwc1       $f14, 0x934($s0)
    /* DDD4A4 0000CA38 460C9102 */  mul.s      $f4, $f18, $f12
    /* DDD4A8 0000CA3C C6100910 */  lwc1       $f16, 0x910($s0)
    /* DDD4AC 0000CA40 460E5482 */  mul.s      $f18, $f10, $f14
    /* DDD4B0 0000CA44 46044180 */  add.s      $f6, $f8, $f4
    /* DDD4B4 0000CA48 C6080A1C */  lwc1       $f8, 0xA1C($s0)
    /* DDD4B8 0000CA4C 46123000 */  add.s      $f0, $f6, $f18
    /* DDD4BC 0000CA50 C60603B0 */  lwc1       $f6, 0x3B0($s0)
    /* DDD4C0 0000CA54 46100102 */  mul.s      $f4, $f0, $f16
    /* DDD4C4 0000CA58 46044280 */  add.s      $f10, $f8, $f4
    /* DDD4C8 0000CA5C 46023482 */  mul.s      $f18, $f6, $f2
    /* DDD4CC 0000CA60 C60803B4 */  lwc1       $f8, 0x3B4($s0)
    /* DDD4D0 0000CA64 C60603B8 */  lwc1       $f6, 0x3B8($s0)
    /* DDD4D4 0000CA68 E60A0A1C */  swc1       $f10, 0xA1C($s0)
    /* DDD4D8 0000CA6C 460C4102 */  mul.s      $f4, $f8, $f12
    /* DDD4DC 0000CA70 46049280 */  add.s      $f10, $f18, $f4
    /* DDD4E0 0000CA74 460E3202 */  mul.s      $f8, $f6, $f14
    /* DDD4E4 0000CA78 C6120A20 */  lwc1       $f18, 0xA20($s0)
    /* DDD4E8 0000CA7C 46085000 */  add.s      $f0, $f10, $f8
    /* DDD4EC 0000CA80 C60A03C8 */  lwc1       $f10, 0x3C8($s0)
    /* DDD4F0 0000CA84 46100102 */  mul.s      $f4, $f0, $f16
    /* DDD4F4 0000CA88 46049180 */  add.s      $f6, $f18, $f4
    /* DDD4F8 0000CA8C 46025202 */  mul.s      $f8, $f10, $f2
    /* DDD4FC 0000CA90 C61203CC */  lwc1       $f18, 0x3CC($s0)
    /* DDD500 0000CA94 C60A03D0 */  lwc1       $f10, 0x3D0($s0)
    /* DDD504 0000CA98 E6060A20 */  swc1       $f6, 0xA20($s0)
    /* DDD508 0000CA9C 460C9102 */  mul.s      $f4, $f18, $f12
    /* DDD50C 0000CAA0 46044180 */  add.s      $f6, $f8, $f4
    /* DDD510 0000CAA4 460E5482 */  mul.s      $f18, $f10, $f14
    /* DDD514 0000CAA8 C6080A24 */  lwc1       $f8, 0xA24($s0)
  alabel D_0000CAAC
    /* DDD518 0000CAAC 46123000 */  add.s      $f0, $f6, $f18
    /* DDD51C 0000CAB0 46004101 */  sub.s      $f4, $f8, $f0
    /* DDD520 0000CAB4 E6040A24 */  swc1       $f4, 0xA24($s0)
    /* DDD524 0000CAB8 8FBF002C */  lw         $ra, 0x2C($sp)
    /* DDD528 0000CABC 8FB00028 */  lw         $s0, 0x28($sp)
  alabel D_0000CAC0
    /* DDD52C 0000CAC0 27BD00F8 */  addiu      $sp, $sp, 0xF8
  alabel D_0000CAC4
    /* DDD530 0000CAC4 03E00008 */  jr         $ra
  alabel D_0000CAC8
    /* DDD534 0000CAC8 00000000 */   nop
endlabel SnowBoarder_procColl
