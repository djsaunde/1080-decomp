nonmatching game_setupDemoSplit, 0x258

glabel game_setupDemoSplit
    /* DD4E40 000043D4 27BDFFA0 */  addiu      $sp, $sp, -0x60
    /* DD4E44 000043D8 AFB0002C */  sw         $s0, 0x2C($sp)
    /* DD4E48 000043DC 00808025 */  or         $s0, $a0, $zero
    /* DD4E4C 000043E0 AFBF0034 */  sw         $ra, 0x34($sp)
    /* DD4E50 000043E4 AFB10030 */  sw         $s1, 0x30($sp)
    /* DD4E54 000043E8 AFA50064 */  sw         $a1, 0x64($sp)
    /* DD4E58 000043EC 0C000000 */  jal        func_00000000
    /* DD4E5C 000043F0 24040080 */   addiu     $a0, $zero, 0x80
    /* DD4E60 000043F4 10400004 */  beqz       $v0, .L00004408
    /* DD4E64 000043F8 00408825 */   or        $s1, $v0, $zero
    /* DD4E68 000043FC 00402025 */  or         $a0, $v0, $zero
    /* DD4E6C 00004400 0C000000 */  jal        func_00000000
    /* DD4E70 00004404 00002825 */   or        $a1, $zero, $zero
  .L00004408:
    /* DD4E74 00004408 AFB10058 */  sw         $s1, 0x58($sp)
    /* DD4E78 0000440C 0C000000 */  jal        func_00000000
    /* DD4E7C 00004410 24040080 */   addiu     $a0, $zero, 0x80
    /* DD4E80 00004414 10400004 */  beqz       $v0, .L00004428
    /* DD4E84 00004418 00408825 */   or        $s1, $v0, $zero
    /* DD4E88 0000441C 00402025 */  or         $a0, $v0, $zero
    /* DD4E8C 00004420 0C000000 */  jal        func_00000000
    /* DD4E90 00004424 00002825 */   or        $a1, $zero, $zero
  .L00004428:
    /* DD4E94 00004428 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD4E98 0000442C 24840000 */  addiu      $a0, $a0, 0x0
    /* DD4E9C 00004430 0C000000 */  jal        func_00000000
    /* DD4EA0 00004434 8FA50058 */   lw        $a1, 0x58($sp)
    /* DD4EA4 00004438 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD4EA8 0000443C 24840000 */  addiu      $a0, $a0, 0x0
    /* DD4EAC 00004440 0C000000 */  jal        func_00000000
    /* DD4EB0 00004444 02202825 */   or        $a1, $s1, $zero
    /* DD4EB4 00004448 0C000000 */  jal        func_00000000
    /* DD4EB8 0000444C 00000000 */   nop
    /* DD4EBC 00004450 3C010000 */  lui        $at, %hi(func_000003F8 + 0x154)
    /* DD4EC0 00004454 C424054C */  lwc1       $f4, %lo(func_000003F8 + 0x154)($at)
    /* DD4EC4 00004458 46040182 */  mul.s      $f6, $f0, $f4
    /* DD4EC8 0000445C 4600320D */  trunc.w.s  $f8, $f6
    /* DD4ECC 00004460 44064000 */  mfc1       $a2, $f8
    /* DD4ED0 00004464 0C000000 */  jal        func_00000000
    /* DD4ED4 00004468 AFA6004C */   sw        $a2, 0x4C($sp)
    /* DD4ED8 0000446C 8FA6004C */  lw         $a2, 0x4C($sp)
    /* DD4EDC 00004470 3C010000 */  lui        $at, %hi(func_000003F8 + 0x158)
    /* DD4EE0 00004474 C42A0550 */  lwc1       $f10, %lo(func_000003F8 + 0x158)($at)
    /* DD4EE4 00004478 44869000 */  mtc1       $a2, $f18
    /* DD4EE8 0000447C 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DD4EEC 00004480 460A0402 */  mul.s      $f16, $f0, $f10
    /* DD4EF0 00004484 44814000 */  mtc1       $at, $f8
    /* DD4EF4 00004488 24010003 */  addiu      $at, $zero, 0x3
    /* DD4EF8 0000448C 8FA90058 */  lw         $t1, 0x58($sp)
    /* DD4EFC 00004490 3C070000 */  lui        $a3, %hi(func_00000080 + 0x10)
    /* DD4F00 00004494 00064080 */  sll        $t0, $a2, 2
    /* DD4F04 00004498 46809120 */  cvt.s.w    $f4, $f18
    /* DD4F08 0000449C AFA90010 */  sw         $t1, 0x10($sp)
    /* DD4F0C 000044A0 00E83821 */  addu       $a3, $a3, $t0
    /* DD4F10 000044A4 340B8004 */  ori        $t3, $zero, 0x8004
    /* DD4F14 000044A8 240C0015 */  addiu      $t4, $zero, 0x15
    /* DD4F18 000044AC 8CE70090 */  lw         $a3, %lo(func_00000080 + 0x10)($a3)
    /* DD4F1C 000044B0 46048180 */  add.s      $f6, $f16, $f4
    /* DD4F20 000044B4 02002025 */  or         $a0, $s0, $zero
    /* DD4F24 000044B8 00002825 */  or         $a1, $zero, $zero
    /* DD4F28 000044BC 46083280 */  add.s      $f10, $f6, $f8
    /* DD4F2C 000044C0 4600548D */  trunc.w.s  $f18, $f10
    /* DD4F30 000044C4 44189000 */  mfc1       $t8, $f18
    /* DD4F34 000044C8 00000000 */  nop
    /* DD4F38 000044CC 0301001A */  div        $zero, $t8, $at
    /* DD4F3C 000044D0 0000C810 */  mfhi       $t9
    /* DD4F40 000044D4 AFB90048 */  sw         $t9, 0x48($sp)
    /* DD4F44 000044D8 8E0A0080 */  lw         $t2, 0x80($s0)
    /* DD4F48 000044DC 3C0143FA */  lui        $at, (0x43FA0000 >> 16)
    /* DD4F4C 000044E0 44812000 */  mtc1       $at, $f4
    /* DD4F50 000044E4 AFAA0014 */  sw         $t2, 0x14($sp)
    /* DD4F54 000044E8 8E030098 */  lw         $v1, 0x98($s0)
    /* DD4F58 000044EC C47000C4 */  lwc1       $f16, 0xC4($v1)
    /* DD4F5C 000044F0 46048181 */  sub.s      $f6, $f16, $f4
    /* DD4F60 000044F4 E7A60018 */  swc1       $f6, 0x18($sp)
    /* DD4F64 000044F8 C46800CC */  lwc1       $f8, 0xCC($v1)
    /* DD4F68 000044FC AFAC0024 */  sw         $t4, 0x24($sp)
    /* DD4F6C 00004500 AFAB0020 */  sw         $t3, 0x20($sp)
    /* DD4F70 00004504 0C000000 */  jal        func_00000000
    /* DD4F74 00004508 E7A8001C */   swc1      $f8, 0x1C($sp)
    /* DD4F78 0000450C AFA20050 */  sw         $v0, 0x50($sp)
    /* DD4F7C 00004510 AFB10010 */  sw         $s1, 0x10($sp)
    /* DD4F80 00004514 8E0E0080 */  lw         $t6, 0x80($s0)
    /* DD4F84 00004518 3C0143FA */  lui        $at, (0x43FA0000 >> 16)
    /* DD4F88 0000451C 44819000 */  mtc1       $at, $f18
    /* DD4F8C 00004520 AFAE0014 */  sw         $t6, 0x14($sp)
    /* DD4F90 00004524 8E030098 */  lw         $v1, 0x98($s0)
    /* DD4F94 00004528 8FA60048 */  lw         $a2, 0x48($sp)
    /* DD4F98 0000452C 3C070000 */  lui        $a3, %hi(func_00000080 + 0x10)
    /* DD4F9C 00004530 C46A00C4 */  lwc1       $f10, 0xC4($v1)
    /* DD4FA0 00004534 00066880 */  sll        $t5, $a2, 2
    /* DD4FA4 00004538 00ED3821 */  addu       $a3, $a3, $t5
    /* DD4FA8 0000453C 46125400 */  add.s      $f16, $f10, $f18
    /* DD4FAC 00004540 340F8024 */  ori        $t7, $zero, 0x8024
    /* DD4FB0 00004544 24180015 */  addiu      $t8, $zero, 0x15
    /* DD4FB4 00004548 8CE70090 */  lw         $a3, %lo(func_00000080 + 0x10)($a3)
    /* DD4FB8 0000454C E7B00018 */  swc1       $f16, 0x18($sp)
    /* DD4FBC 00004550 C46400CC */  lwc1       $f4, 0xCC($v1)
    /* DD4FC0 00004554 AFB80024 */  sw         $t8, 0x24($sp)
    /* DD4FC4 00004558 AFAF0020 */  sw         $t7, 0x20($sp)
    /* DD4FC8 0000455C 02002025 */  or         $a0, $s0, $zero
    /* DD4FCC 00004560 24050001 */  addiu      $a1, $zero, 0x1
    /* DD4FD0 00004564 0C000000 */  jal        func_00000000
    /* DD4FD4 00004568 E7A4001C */   swc1      $f4, 0x1C($sp)
    /* DD4FD8 0000456C 8FA70050 */  lw         $a3, 0x50($sp)
    /* DD4FDC 00004570 AFA20054 */  sw         $v0, 0x54($sp)
    /* DD4FE0 00004574 02002025 */  or         $a0, $s0, $zero
    /* DD4FE4 00004578 ACE20908 */  sw         $v0, 0x908($a3)
    /* DD4FE8 0000457C AC470908 */  sw         $a3, 0x908($v0)
    /* DD4FEC 00004580 8FB90058 */  lw         $t9, 0x58($sp)
    /* DD4FF0 00004584 8E060080 */  lw         $a2, 0x80($s0)
    /* DD4FF4 00004588 24050002 */  addiu      $a1, $zero, 0x2
    /* DD4FF8 0000458C 0C000000 */  jal        func_00000000
    /* DD4FFC 00004590 AFB90010 */   sw        $t9, 0x10($sp)
    /* DD5000 00004594 AFA2003C */  sw         $v0, 0x3C($sp)
    /* DD5004 00004598 8E060080 */  lw         $a2, 0x80($s0)
    /* DD5008 0000459C AFB10010 */  sw         $s1, 0x10($sp)
    /* DD500C 000045A0 02002025 */  or         $a0, $s0, $zero
    /* DD5010 000045A4 24050002 */  addiu      $a1, $zero, 0x2
    /* DD5014 000045A8 0C000000 */  jal        func_00000000
    /* DD5018 000045AC 8FA70054 */   lw        $a3, 0x54($sp)
    /* DD501C 000045B0 3C01425C */  lui        $at, (0x425C0000 >> 16)
    /* DD5020 000045B4 44810000 */  mtc1       $at, $f0
    /* DD5024 000045B8 8FA6003C */  lw         $a2, 0x3C($sp)
    /* DD5028 000045BC AFA20038 */  sw         $v0, 0x38($sp)
    /* DD502C 000045C0 3C0A0000 */  lui        $t2, (0x0 >> 16)
    /* DD5030 000045C4 E4C0014C */  swc1       $f0, 0x14C($a2)
    /* DD5034 000045C8 E440014C */  swc1       $f0, 0x14C($v0)
    /* DD5038 000045CC 8FA80064 */  lw         $t0, 0x64($sp)
    /* DD503C 000045D0 254A0000 */  addiu      $t2, $t2, 0x0
    /* DD5040 000045D4 02002025 */  or         $a0, $s0, $zero
    /* DD5044 000045D8 000848C0 */  sll        $t1, $t0, 3
    /* DD5048 000045DC 01284823 */  subu       $t1, $t1, $t0
    /* DD504C 000045E0 00094880 */  sll        $t1, $t1, 2
    /* DD5050 000045E4 012A8821 */  addu       $s1, $t1, $t2
    /* DD5054 000045E8 0C000000 */  jal        func_00000000
    /* DD5058 000045EC 02202825 */   or        $a1, $s1, $zero
    /* DD505C 000045F0 AFA20044 */  sw         $v0, 0x44($sp)
    /* DD5060 000045F4 02002025 */  or         $a0, $s0, $zero
    /* DD5064 000045F8 2625001C */  addiu      $a1, $s1, 0x1C
    /* DD5068 000045FC 0C000000 */  jal        func_00000000
    /* DD506C 00004600 8FA60038 */   lw        $a2, 0x38($sp)
    /* DD5070 00004604 8FAB0044 */  lw         $t3, 0x44($sp)
    /* DD5074 00004608 8FAC0050 */  lw         $t4, 0x50($sp)
    /* DD5078 0000460C AD8B08DC */  sw         $t3, 0x8DC($t4)
    /* DD507C 00004610 8FAD0054 */  lw         $t5, 0x54($sp)
    /* DD5080 00004614 ADA208DC */  sw         $v0, 0x8DC($t5)
    /* DD5084 00004618 8FBF0034 */  lw         $ra, 0x34($sp)
    /* DD5088 0000461C 8FB10030 */  lw         $s1, 0x30($sp)
    /* DD508C 00004620 8FB0002C */  lw         $s0, 0x2C($sp)
    /* DD5090 00004624 03E00008 */  jr         $ra
    /* DD5094 00004628 27BD0060 */   addiu     $sp, $sp, 0x60
endlabel game_setupDemoSplit
