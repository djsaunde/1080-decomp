nonmatching func_00007328, 0x1C0

glabel func_00007328
    /* DD7D94 00007328 27BDFFA8 */  addiu      $sp, $sp, -0x58
    /* DD7D98 0000732C AFB30020 */  sw         $s3, 0x20($sp)
    /* DD7D9C 00007330 AFB10018 */  sw         $s1, 0x18($sp)
  alabel D_00007334
    /* DD7DA0 00007334 AFB00014 */  sw         $s0, 0x14($sp)
    /* DD7DA4 00007338 00809825 */  or         $s3, $a0, $zero
    /* DD7DA8 0000733C AFBF0024 */  sw         $ra, 0x24($sp)
    /* DD7DAC 00007340 AFB2001C */  sw         $s2, 0x1C($sp)
  alabel D_00007344
    /* DD7DB0 00007344 27B00054 */  addiu      $s0, $sp, 0x54
    /* DD7DB4 00007348 27B10034 */  addiu      $s1, $sp, 0x34
    /* DD7DB8 0000734C AFA00054 */  sw         $zero, 0x54($sp)
  .L00007350:
    /* DD7DBC 00007350 0C0017BE */  jal        func_00005EF8
    /* DD7DC0 00007354 02002025 */   or        $a0, $s0, $zero
    /* DD7DC4 00007358 8FAE0054 */  lw         $t6, 0x54($sp)
    /* DD7DC8 0000735C 27A40050 */  addiu      $a0, $sp, 0x50
  alabel D_00007360
    /* DD7DCC 00007360 11C0003A */  beqz       $t6, .L0000744C
    /* DD7DD0 00007364 00000000 */   nop
    /* DD7DD4 00007368 0C00140B */  jal        func_0000502C
    /* DD7DD8 0000736C 00009025 */   or        $s2, $zero, $zero
    /* DD7DDC 00007370 8E6F002C */  lw         $t7, 0x2C($s3)
  alabel D_00007374
    /* DD7DE0 00007374 00001025 */  or         $v0, $zero, $zero
    /* DD7DE4 00007378 3C040000 */  lui        $a0, %hi(D_00007FA0)
    /* DD7DE8 0000737C AFAF0048 */  sw         $t7, 0x48($sp)
  alabel D_00007380
    /* DD7DEC 00007380 11E00005 */  beqz       $t7, .L00007398
    /* DD7DF0 00007384 AFAF0044 */   sw        $t7, 0x44($sp)
    /* DD7DF4 00007388 8DF90004 */  lw         $t9, 0x4($t7)
  alabel D_0000738C
    /* DD7DF8 0000738C AFB90048 */  sw         $t9, 0x48($sp)
  alabel D_00007390
    /* DD7DFC 00007390 10000001 */  b          .L00007398
  alabel D_00007394
    /* DD7E00 00007394 8DE20000 */   lw        $v0, 0x0($t7)
  .L00007398:
    /* DD7E04 00007398 10400016 */  beqz       $v0, .L000073F4
  alabel D_0000739C
    /* DD7E08 0000739C 24847FA0 */   addiu     $a0, $a0, %lo(D_00007FA0)
    /* DD7E0C 000073A0 8FA90044 */  lw         $t1, 0x44($sp)
  .L000073A4:
    /* DD7E10 000073A4 8E0B0000 */  lw         $t3, 0x0($s0)
    /* DD7E14 000073A8 8D220000 */  lw         $v0, 0x0($t1)
    /* DD7E18 000073AC AE2B0000 */  sw         $t3, 0x0($s1)
    /* DD7E1C 000073B0 8FAC0034 */  lw         $t4, 0x34($sp)
  alabel D_000073B4
    /* DD7E20 000073B4 8C4D0004 */  lw         $t5, 0x4($v0)
    /* DD7E24 000073B8 00001025 */  or         $v0, $zero, $zero
    /* DD7E28 000073BC 8FAF0048 */  lw         $t7, 0x48($sp)
    /* DD7E2C 000073C0 158D0004 */  bne        $t4, $t5, .L000073D4
    /* DD7E30 000073C4 00000000 */   nop
    /* DD7E34 000073C8 8FAE0044 */  lw         $t6, 0x44($sp)
  alabel D_000073CC
    /* DD7E38 000073CC 10000009 */  b          .L000073F4
    /* DD7E3C 000073D0 8DD20000 */   lw        $s2, 0x0($t6)
  .L000073D4:
    /* DD7E40 000073D4 11E00005 */  beqz       $t7, .L000073EC
    /* DD7E44 000073D8 AFAF0044 */   sw        $t7, 0x44($sp)
    /* DD7E48 000073DC 8DF80004 */  lw         $t8, 0x4($t7)
    /* DD7E4C 000073E0 AFB80048 */  sw         $t8, 0x48($sp)
    /* DD7E50 000073E4 10000001 */  b          .L000073EC
  alabel D_000073E8
    /* DD7E54 000073E8 8DE20000 */   lw        $v0, 0x0($t7)
  .L000073EC:
    /* DD7E58 000073EC 5440FFED */  bnel       $v0, $zero, .L000073A4
    /* DD7E5C 000073F0 8FA90044 */   lw        $t1, 0x44($sp)
  .L000073F4:
    /* DD7E60 000073F4 1240000C */  beqz       $s2, .L00007428
  alabel D_000073F8
    /* DD7E64 000073F8 26450004 */   addiu     $a1, $s2, 0x4
    /* DD7E68 000073FC 3C040000 */  lui        $a0, %hi(D_00007F90)
    /* DD7E6C 00007400 0C000000 */  jal        func_00000000
    /* DD7E70 00007404 24847F90 */   addiu     $a0, $a0, %lo(D_00007F90)
  alabel D_00007408
    /* DD7E74 00007408 8E430000 */  lw         $v1, 0x0($s2)
    /* DD7E78 0000740C 8C620028 */  lw         $v0, 0x28($v1)
    /* DD7E7C 00007410 8C59004C */  lw         $t9, 0x4C($v0)
    /* DD7E80 00007414 84480048 */  lh         $t0, 0x48($v0)
    /* DD7E84 00007418 0320F809 */  jalr       $t9
  alabel D_0000741C
    /* DD7E88 0000741C 01032021 */   addu      $a0, $t0, $v1
    /* DD7E8C 00007420 1000FFCB */  b          .L00007350
  alabel D_00007424
    /* DD7E90 00007424 AFA00054 */   sw        $zero, 0x54($sp)
  .L00007428:
    /* DD7E94 00007428 0C000000 */  jal        func_00000000
    /* DD7E98 0000742C 02002825 */   or        $a1, $s0, $zero
    /* DD7E9C 00007430 3C040000 */  lui        $a0, (0x0 >> 16)
  alabel D_00007434
    /* DD7EA0 00007434 24840000 */  addiu      $a0, $a0, 0x0
    /* DD7EA4 00007438 24050001 */  addiu      $a1, $zero, 0x1
    /* DD7EA8 0000743C 0C000000 */  jal        func_00000000
    /* DD7EAC 00007440 8FA60050 */   lw        $a2, 0x50($sp)
  alabel D_00007444
    /* DD7EB0 00007444 1000FFC2 */  b          .L00007350
    /* DD7EB4 00007448 AFA00054 */   sw        $zero, 0x54($sp)
  .L0000744C:
    /* DD7EB8 0000744C 0C00140B */  jal        func_0000502C
    /* DD7EBC 00007450 26640038 */   addiu     $a0, $s3, 0x38
  alabel D_00007454
    /* DD7EC0 00007454 8E620038 */  lw         $v0, 0x38($s3)
    /* DD7EC4 00007458 3C040000 */  lui        $a0, %hi(func_0000027C + 0x18)
    /* DD7EC8 0000745C 30490001 */  andi       $t1, $v0, 0x1
  alabel D_00007460
    /* DD7ECC 00007460 51200005 */  beql       $t1, $zero, .L00007478
    /* DD7ED0 00007464 304A0002 */   andi      $t2, $v0, 0x2
    /* DD7ED4 00007468 0C000000 */  jal        func_00000000
  alabel D_0000746C
    /* DD7ED8 0000746C 24840294 */   addiu     $a0, $a0, %lo(func_0000027C + 0x18)
    /* DD7EDC 00007470 8E620038 */  lw         $v0, 0x38($s3)
    /* DD7EE0 00007474 304A0002 */  andi       $t2, $v0, 0x2
  .L00007478:
    /* DD7EE4 00007478 11400005 */  beqz       $t2, .L00007490
    /* DD7EE8 0000747C 00000000 */   nop
  alabel D_00007480
    /* DD7EEC 00007480 0C0013FC */  jal        func_00004FF0
    /* DD7EF0 00007484 26640030 */   addiu     $a0, $s3, 0x30
    /* DD7EF4 00007488 0C0013FC */  jal        func_00004FF0
    /* DD7EF8 0000748C 26640034 */   addiu     $a0, $s3, 0x34
  .L00007490:
    /* DD7EFC 00007490 3C020000 */  lui        $v0, %hi(func_00000008 + 0x34)
    /* DD7F00 00007494 8C42003C */  lw         $v0, %lo(func_00000008 + 0x34)($v0)
    /* DD7F04 00007498 24010006 */  addiu      $at, $zero, 0x6
    /* DD7F08 0000749C 10410002 */  beq        $v0, $at, .L000074A8
    /* DD7F0C 000074A0 2401000B */   addiu     $at, $zero, 0xB
  alabel D_000074A4
    /* DD7F10 000074A4 14410009 */  bne        $v0, $at, .L000074CC
  .L000074A8:
    /* DD7F14 000074A8 3C013F80 */   lui       $at, (0x3F800000 >> 16)
    /* DD7F18 000074AC 44812000 */  mtc1       $at, $f4
  alabel D_000074B0
    /* DD7F1C 000074B0 3C010000 */  lui        $at, %hi(func_0000029C + 0x4)
    /* DD7F20 000074B4 240D0320 */  addiu      $t5, $zero, 0x320
    /* DD7F24 000074B8 E42402A0 */  swc1       $f4, %lo(func_0000029C + 0x4)($at)
    /* DD7F28 000074BC 8E6B0038 */  lw         $t3, 0x38($s3)
    /* DD7F2C 000074C0 AE6D0034 */  sw         $t5, 0x34($s3)
  alabel D_000074C4
    /* DD7F30 000074C4 356C0002 */  ori        $t4, $t3, 0x2
    /* DD7F34 000074C8 AE6C0038 */  sw         $t4, 0x38($s3)
  .L000074CC:
    /* DD7F38 000074CC 8FBF0024 */  lw         $ra, 0x24($sp)
  alabel D_000074D0
    /* DD7F3C 000074D0 8FB00014 */  lw         $s0, 0x14($sp)
    /* DD7F40 000074D4 8FB10018 */  lw         $s1, 0x18($sp)
    /* DD7F44 000074D8 8FB2001C */  lw         $s2, 0x1C($sp)
  alabel D_000074DC
    /* DD7F48 000074DC 8FB30020 */  lw         $s3, 0x20($sp)
    /* DD7F4C 000074E0 03E00008 */  jr         $ra
  alabel D_000074E4
    /* DD7F50 000074E4 27BD0058 */   addiu     $sp, $sp, 0x58
endlabel func_00007328
