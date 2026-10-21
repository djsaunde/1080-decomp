nonmatching func_00006808, 0x948

glabel func_00006808
    /* DD7274 00006808 27BDFF10 */  addiu      $sp, $sp, -0xF0
    /* DD7278 0000680C AFBF0044 */  sw         $ra, 0x44($sp)
    /* DD727C 00006810 AFB70040 */  sw         $s7, 0x40($sp)
    /* DD7280 00006814 AFB6003C */  sw         $s6, 0x3C($sp)
    /* DD7284 00006818 AFB50038 */  sw         $s5, 0x38($sp)
    /* DD7288 0000681C AFB40034 */  sw         $s4, 0x34($sp)
    /* DD728C 00006820 AFB30030 */  sw         $s3, 0x30($sp)
    /* DD7290 00006824 AFB2002C */  sw         $s2, 0x2C($sp)
    /* DD7294 00006828 AFB10028 */  sw         $s1, 0x28($sp)
    /* DD7298 0000682C AFB00024 */  sw         $s0, 0x24($sp)
    /* DD729C 00006830 AFA600F8 */  sw         $a2, 0xF8($sp)
    /* DD72A0 00006834 AFA000DC */  sw         $zero, 0xDC($sp)
    /* DD72A4 00006838 AFA000D4 */  sw         $zero, 0xD4($sp)
    /* DD72A8 0000683C 8CB10040 */  lw         $s1, 0x40($a1)
    /* DD72AC 00006840 3C190000 */  lui        $t9, %hi(func_00000000 + 0x4)
    /* DD72B0 00006844 8F390004 */  lw         $t9, %lo(func_00000000 + 0x4)($t9)
    /* DD72B4 00006848 44800000 */  mtc1       $zero, $f0
    /* DD72B8 0000684C 3C010002 */  lui        $at, (0x20000 >> 16)
    /* DD72BC 00006850 0080B825 */  or         $s7, $a0, $zero
    /* DD72C0 00006854 03214025 */  or         $t0, $t9, $at
    /* DD72C4 00006858 240E0028 */  addiu      $t6, $zero, 0x28
    /* DD72C8 0000685C 240F0014 */  addiu      $t7, $zero, 0x14
    /* DD72CC 00006860 24180001 */  addiu      $t8, $zero, 0x1
    /* DD72D0 00006864 3C010000 */  lui        $at, %hi(func_00000000 + 0x4)
    /* DD72D4 00006868 3C040000 */  lui        $a0, %hi(.L00007EB8)
    /* DD72D8 0000686C AFA000BC */  sw         $zero, 0xBC($sp)
    /* DD72DC 00006870 AFAE00B8 */  sw         $t6, 0xB8($sp)
    /* DD72E0 00006874 AFAF00B4 */  sw         $t7, 0xB4($sp)
    /* DD72E4 00006878 AFB800B0 */  sw         $t8, 0xB0($sp)
    /* DD72E8 0000687C AC280004 */  sw         $t0, %lo(func_00000000 + 0x4)($at)
    /* DD72EC 00006880 24847EB8 */  addiu      $a0, $a0, %lo(.L00007EB8)
    /* DD72F0 00006884 00002825 */  or         $a1, $zero, $zero
    /* DD72F4 00006888 E7A000C8 */  swc1       $f0, 0xC8($sp)
    /* DD72F8 0000688C E7A000C4 */  swc1       $f0, 0xC4($sp)
    /* DD72FC 00006890 0C000000 */  jal        func_00000000
    /* DD7300 00006894 E7A000C0 */   swc1      $f0, 0xC0($sp)
    /* DD7304 00006898 0C000000 */  jal        func_00000000
    /* DD7308 0000689C 00002025 */   or        $a0, $zero, $zero
    /* DD730C 000068A0 3C140000 */  lui        $s4, (0x0 >> 16)
    /* DD7310 000068A4 26940000 */  addiu      $s4, $s4, 0x0
    /* DD7314 000068A8 AE820000 */  sw         $v0, 0x0($s4)
    /* DD7318 000068AC 8EF30084 */  lw         $s3, 0x84($s7)
    /* DD731C 000068B0 00409025 */  or         $s2, $v0, $zero
    /* DD7320 000068B4 00402825 */  or         $a1, $v0, $zero
    /* DD7324 000068B8 0C000000 */  jal        func_00000000
    /* DD7328 000068BC 26640010 */   addiu     $a0, $s3, 0x10
    /* DD732C 000068C0 8E490014 */  lw         $t1, 0x14($s2)
    /* DD7330 000068C4 44800000 */  mtc1       $zero, $f0
    /* DD7334 000068C8 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DD7338 000068CC 11200003 */  beqz       $t1, .L000068DC
    /* DD733C 000068D0 00002025 */   or        $a0, $zero, $zero
    /* DD7340 000068D4 240A0001 */  addiu      $t2, $zero, 0x1
    /* DD7344 000068D8 AE4A0004 */  sw         $t2, 0x4($s2)
  .L000068DC:
    /* DD7348 000068DC 44812000 */  mtc1       $at, $f4
    /* DD734C 000068E0 AE530014 */  sw         $s3, 0x14($s2)
    /* DD7350 000068E4 3C050000 */  lui        $a1, %hi(D_00007EC4)
    /* DD7354 000068E8 E7A000A0 */  swc1       $f0, 0xA0($sp)
    /* DD7358 000068EC E7A000A4 */  swc1       $f0, 0xA4($sp)
    /* DD735C 000068F0 24A57EC4 */  addiu      $a1, $a1, %lo(D_00007EC4)
    /* DD7360 000068F4 2406FFFF */  addiu      $a2, $zero, -0x1
    /* DD7364 000068F8 0C000000 */  jal        func_00000000
    /* DD7368 000068FC E7A4009C */   swc1      $f4, 0x9C($sp)
    /* DD736C 00006900 3C010000 */  lui        $at, (0x0 >> 16)
    /* DD7370 00006904 3C150000 */  lui        $s5, (0x0 >> 16)
    /* DD7374 00006908 AC220000 */  sw         $v0, 0x0($at)
    /* DD7378 0000690C 0000B025 */  or         $s6, $zero, $zero
    /* DD737C 00006910 26B50000 */  addiu      $s5, $s5, 0x0
  .L00006914:
    /* DD7380 00006914 8E2B0000 */  lw         $t3, 0x0($s1)
    /* DD7384 00006918 02209025 */  or         $s2, $s1, $zero
    /* DD7388 0000691C 26310004 */  addiu      $s1, $s1, 0x4
    /* DD738C 00006920 000B6403 */  sra        $t4, $t3, 16
    /* DD7390 00006924 AFAC00A8 */  sw         $t4, 0xA8($sp)
    /* DD7394 00006928 8E420000 */  lw         $v0, 0x0($s2)
    /* DD7398 0000692C 3042FFFF */  andi       $v0, $v0, 0xFFFF
    /* DD739C 00006930 2C41000D */  sltiu      $at, $v0, 0xD
    /* DD73A0 00006934 102000CA */  beqz       $at, .L00006C60
    /* DD73A4 00006938 AFA20048 */   sw        $v0, 0x48($sp)
    /* DD73A8 0000693C 00026880 */  sll        $t5, $v0, 2
    /* DD73AC 00006940 3C010000 */  lui        $at, %hi(func_0000057C + 0x38)
    /* DD73B0 00006944 002D0821 */  addu       $at, $at, $t5
    /* DD73B4 00006948 8C2D05B4 */  lw         $t5, %lo(func_0000057C + 0x38)($at)
    /* DD73B8 0000694C 01A00008 */  jr         $t5
    /* DD73BC 00006950 00000000 */   nop
    /* DD73C0 00006954 3C040000 */  lui        $a0, %hi(D_00007ECC)
    /* DD73C4 00006958 24847ECC */  addiu      $a0, $a0, %lo(D_00007ECC)
    /* DD73C8 0000695C 0C000000 */  jal        func_00000000
    /* DD73CC 00006960 00002825 */   or        $a1, $zero, $zero
    /* DD73D0 00006964 0C000000 */  jal        func_00000000
    /* DD73D4 00006968 00002025 */   or        $a0, $zero, $zero
    /* DD73D8 0000696C 3C050000 */  lui        $a1, %hi(D_00007ED8)
    /* DD73DC 00006970 00408025 */  or         $s0, $v0, $zero
    /* DD73E0 00006974 24A57ED8 */  addiu      $a1, $a1, %lo(D_00007ED8)
    /* DD73E4 00006978 00002025 */  or         $a0, $zero, $zero
    /* DD73E8 0000697C 0C000000 */  jal        func_00000000
    /* DD73EC 00006980 00403025 */   or        $a2, $v0, $zero
    /* DD73F0 00006984 00409025 */  or         $s2, $v0, $zero
    /* DD73F4 00006988 02002025 */  or         $a0, $s0, $zero
    /* DD73F8 0000698C 0C000000 */  jal        func_00000000
    /* DD73FC 00006990 00402825 */   or        $a1, $v0, $zero
    /* DD7400 00006994 3C0E0000 */  lui        $t6, %hi(D_00007EE8)
    /* DD7404 00006998 8DC67EE8 */  lw         $a2, %lo(D_00007EE8)($t6)
    /* DD7408 0000699C 8E840000 */  lw         $a0, 0x0($s4)
    /* DD740C 000069A0 02002825 */  or         $a1, $s0, $zero
    /* DD7410 000069A4 AFA60078 */  sw         $a2, 0x78($sp)
    /* DD7414 000069A8 0C000000 */  jal        func_00000000
    /* DD7418 000069AC AFA60008 */   sw        $a2, 0x8($sp)
    /* DD741C 000069B0 3C180000 */  lui        $t8, %hi(D_00007EEC)
    /* DD7420 000069B4 8F067EEC */  lw         $a2, %lo(D_00007EEC)($t8)
    /* DD7424 000069B8 8E840000 */  lw         $a0, 0x0($s4)
    /* DD7428 000069BC 02402825 */  or         $a1, $s2, $zero
    /* DD742C 000069C0 AFA60074 */  sw         $a2, 0x74($sp)
    /* DD7430 000069C4 0C000000 */  jal        func_00000000
    /* DD7434 000069C8 AFA60008 */   sw        $a2, 0x8($sp)
    /* DD7438 000069CC 8EF3007C */  lw         $s3, 0x7C($s7)
    /* DD743C 000069D0 02002825 */  or         $a1, $s0, $zero
    /* DD7440 000069D4 0C000000 */  jal        func_00000000
    /* DD7444 000069D8 26640010 */   addiu     $a0, $s3, 0x10
    /* DD7448 000069DC 8E080014 */  lw         $t0, 0x14($s0)
    /* DD744C 000069E0 24090001 */  addiu      $t1, $zero, 0x1
    /* DD7450 000069E4 02002025 */  or         $a0, $s0, $zero
    /* DD7454 000069E8 51000003 */  beql       $t0, $zero, .L000069F8
    /* DD7458 000069EC AE130014 */   sw        $s3, 0x14($s0)
    /* DD745C 000069F0 AE090004 */  sw         $t1, 0x4($s0)
    /* DD7460 000069F4 AE130014 */  sw         $s3, 0x14($s0)
  .L000069F8:
    /* DD7464 000069F8 0C000000 */  jal        func_00000000
    /* DD7468 000069FC 8EE50094 */   lw        $a1, 0x94($s7)
    /* DD746C 00006A00 3C0A0000 */  lui        $t2, %hi(D_00007EF0)
    /* DD7470 00006A04 8D467EF0 */  lw         $a2, %lo(D_00007EF0)($t2)
    /* DD7474 00006A08 8EE50090 */  lw         $a1, 0x90($s7)
    /* DD7478 00006A0C 8E840000 */  lw         $a0, 0x0($s4)
    /* DD747C 00006A10 AFA60008 */  sw         $a2, 0x8($sp)
    /* DD7480 00006A14 0C000000 */  jal        func_00000000
    /* DD7484 00006A18 AFA60070 */   sw        $a2, 0x70($sp)
    /* DD7488 00006A1C 3C0C0000 */  lui        $t4, %hi(D_00007EF4)
    /* DD748C 00006A20 8D867EF4 */  lw         $a2, %lo(D_00007EF4)($t4)
    /* DD7490 00006A24 8EE50094 */  lw         $a1, 0x94($s7)
    /* DD7494 00006A28 8E840000 */  lw         $a0, 0x0($s4)
    /* DD7498 00006A2C AFA60008 */  sw         $a2, 0x8($sp)
    /* DD749C 00006A30 0C000000 */  jal        func_00000000
    /* DD74A0 00006A34 AFA6006C */   sw        $a2, 0x6C($sp)
    /* DD74A4 00006A38 0C000000 */  jal        func_00000000
    /* DD74A8 00006A3C 00000000 */   nop
    /* DD74AC 00006A40 10000088 */  b          .L00006C64
    /* DD74B0 00006A44 8FB900A8 */   lw        $t9, 0xA8($sp)
    /* DD74B4 00006A48 8E360004 */  lw         $s6, 0x4($s1)
    /* DD74B8 00006A4C 8E840000 */  lw         $a0, 0x0($s4)
    /* DD74BC 00006A50 24050001 */  addiu      $a1, $zero, 0x1
    /* DD74C0 00006A54 0C000000 */  jal        func_00000000
    /* DD74C4 00006A58 8E260000 */   lw        $a2, 0x0($s1)
    /* DD74C8 00006A5C 10000081 */  b          .L00006C64
    /* DD74CC 00006A60 8FB900A8 */   lw        $t9, 0xA8($sp)
    /* DD74D0 00006A64 8EAE0068 */  lw         $t6, 0x68($s5)
    /* DD74D4 00006A68 24050001 */  addiu      $a1, $zero, 0x1
    /* DD74D8 00006A6C 29C10002 */  slti       $at, $t6, 0x2
    /* DD74DC 00006A70 5020007C */  beql       $at, $zero, .L00006C64
    /* DD74E0 00006A74 8FB900A8 */   lw        $t9, 0xA8($sp)
    /* DD74E4 00006A78 8E360004 */  lw         $s6, 0x4($s1)
    /* DD74E8 00006A7C 8E840000 */  lw         $a0, 0x0($s4)
    /* DD74EC 00006A80 0C000000 */  jal        func_00000000
    /* DD74F0 00006A84 8E260000 */   lw        $a2, 0x0($s1)
    /* DD74F4 00006A88 10000076 */  b          .L00006C64
    /* DD74F8 00006A8C 8FB900A8 */   lw        $t9, 0xA8($sp)
    /* DD74FC 00006A90 8EAF0068 */  lw         $t7, 0x68($s5)
    /* DD7500 00006A94 24010002 */  addiu      $at, $zero, 0x2
    /* DD7504 00006A98 24050001 */  addiu      $a1, $zero, 0x1
    /* DD7508 00006A9C 55E10071 */  bnel       $t7, $at, .L00006C64
    /* DD750C 00006AA0 8FB900A8 */   lw        $t9, 0xA8($sp)
    /* DD7510 00006AA4 8E840000 */  lw         $a0, 0x0($s4)
    /* DD7514 00006AA8 0C000000 */  jal        func_00000000
    /* DD7518 00006AAC 8E260000 */   lw        $a2, 0x0($s1)
    /* DD751C 00006AB0 1000006B */  b          .L00006C60
    /* DD7520 00006AB4 8E360004 */   lw        $s6, 0x4($s1)
    /* DD7524 00006AB8 8EB80068 */  lw         $t8, 0x68($s5)
    /* DD7528 00006ABC 24010003 */  addiu      $at, $zero, 0x3
    /* DD752C 00006AC0 24050001 */  addiu      $a1, $zero, 0x1
    /* DD7530 00006AC4 57010067 */  bnel       $t8, $at, .L00006C64
    /* DD7534 00006AC8 8FB900A8 */   lw        $t9, 0xA8($sp)
    /* DD7538 00006ACC 8E840000 */  lw         $a0, 0x0($s4)
    /* DD753C 00006AD0 0C000000 */  jal        func_00000000
    /* DD7540 00006AD4 8E260000 */   lw        $a2, 0x0($s1)
    /* DD7544 00006AD8 10000061 */  b          .L00006C60
    /* DD7548 00006ADC 8E360004 */   lw        $s6, 0x4($s1)
    /* DD754C 00006AE0 24190001 */  addiu      $t9, $zero, 0x1
    /* DD7550 00006AE4 AFB900BC */  sw         $t9, 0xBC($sp)
    /* DD7554 00006AE8 8E280000 */  lw         $t0, 0x0($s1)
    /* DD7558 00006AEC AFA800B8 */  sw         $t0, 0xB8($sp)
    /* DD755C 00006AF0 8E290004 */  lw         $t1, 0x4($s1)
    /* DD7560 00006AF4 AFA900B4 */  sw         $t1, 0xB4($sp)
    /* DD7564 00006AF8 8E2A0008 */  lw         $t2, 0x8($s1)
    /* DD7568 00006AFC 10000058 */  b          .L00006C60
    /* DD756C 00006B00 AFAA00B0 */   sw        $t2, 0xB0($sp)
    /* DD7570 00006B04 8FAB00F8 */  lw         $t3, 0xF8($sp)
    /* DD7574 00006B08 24010006 */  addiu      $at, $zero, 0x6
    /* DD7578 00006B0C 3C040000 */  lui        $a0, %hi(D_00007EF8)
    /* DD757C 00006B10 11610053 */  beq        $t3, $at, .L00006C60
    /* DD7580 00006B14 24847EF8 */   addiu     $a0, $a0, %lo(D_00007EF8)
    /* DD7584 00006B18 0C000000 */  jal        func_00000000
    /* DD7588 00006B1C 00002825 */   or        $a1, $zero, $zero
    /* DD758C 00006B20 02A02025 */  or         $a0, $s5, $zero
    /* DD7590 00006B24 0C000000 */  jal        func_00000000
    /* DD7594 00006B28 8E250000 */   lw        $a1, 0x0($s1)
    /* DD7598 00006B2C 8E2C0014 */  lw         $t4, 0x14($s1)
    /* DD759C 00006B30 8E240004 */  lw         $a0, 0x4($s1)
    /* DD75A0 00006B34 8E250008 */  lw         $a1, 0x8($s1)
    /* DD75A4 00006B38 8E26000C */  lw         $a2, 0xC($s1)
    /* DD75A8 00006B3C 8E270010 */  lw         $a3, 0x10($s1)
    /* DD75AC 00006B40 AFAC0010 */  sw         $t4, 0x10($sp)
    /* DD75B0 00006B44 8E2D0018 */  lw         $t5, 0x18($s1)
    /* DD75B4 00006B48 AFAD0014 */  sw         $t5, 0x14($sp)
    /* DD75B8 00006B4C 8E2E001C */  lw         $t6, 0x1C($s1)
    /* DD75BC 00006B50 0C000000 */  jal        func_00000000
    /* DD75C0 00006B54 AFAE0018 */   sw        $t6, 0x18($sp)
    /* DD75C4 00006B58 8EF30084 */  lw         $s3, 0x84($s7)
    /* DD75C8 00006B5C 00408025 */  or         $s0, $v0, $zero
    /* DD75CC 00006B60 00409025 */  or         $s2, $v0, $zero
    /* DD75D0 00006B64 00402825 */  or         $a1, $v0, $zero
    /* DD75D4 00006B68 0C000000 */  jal        func_00000000
    /* DD75D8 00006B6C 26640010 */   addiu     $a0, $s3, 0x10
    /* DD75DC 00006B70 8E4F0014 */  lw         $t7, 0x14($s2)
    /* DD75E0 00006B74 24180001 */  addiu      $t8, $zero, 0x1
    /* DD75E4 00006B78 51E00003 */  beql       $t7, $zero, .L00006B88
    /* DD75E8 00006B7C AE530014 */   sw        $s3, 0x14($s2)
    /* DD75EC 00006B80 AE580004 */  sw         $t8, 0x4($s2)
    /* DD75F0 00006B84 AE530014 */  sw         $s3, 0x14($s2)
  .L00006B88:
    /* DD75F4 00006B88 8E990000 */  lw         $t9, 0x0($s4)
    /* DD75F8 00006B8C 0C000000 */  jal        func_00000000
    /* DD75FC 00006B90 AF30003C */   sw        $s0, 0x3C($t9)
    /* DD7600 00006B94 10000033 */  b          .L00006C64
    /* DD7604 00006B98 8FB900A8 */   lw        $t9, 0xA8($sp)
    /* DD7608 00006B9C 3C040000 */  lui        $a0, %hi(D_00007EFC)
    /* DD760C 00006BA0 24847EFC */  addiu      $a0, $a0, %lo(D_00007EFC)
    /* DD7610 00006BA4 0C000000 */  jal        func_00000000
    /* DD7614 00006BA8 00002825 */   or        $a1, $zero, $zero
    /* DD7618 00006BAC 02A02025 */  or         $a0, $s5, $zero
    /* DD761C 00006BB0 0C000000 */  jal        func_00000000
    /* DD7620 00006BB4 8E250000 */   lw        $a1, 0x0($s1)
    /* DD7624 00006BB8 8E280004 */  lw         $t0, 0x4($s1)
    /* DD7628 00006BBC 0C000000 */  jal        func_00000000
    /* DD762C 00006BC0 AFA800D0 */   sw        $t0, 0xD0($sp)
    /* DD7630 00006BC4 10000027 */  b          .L00006C64
    /* DD7634 00006BC8 8FB900A8 */   lw        $t9, 0xA8($sp)
    /* DD7638 00006BCC 8E290000 */  lw         $t1, 0x0($s1)
    /* DD763C 00006BD0 10000023 */  b          .L00006C60
    /* DD7640 00006BD4 AFA900DC */   sw        $t1, 0xDC($sp)
    /* DD7644 00006BD8 8E2A0000 */  lw         $t2, 0x0($s1)
    /* DD7648 00006BDC 10000020 */  b          .L00006C60
    /* DD764C 00006BE0 AFAA00D4 */   sw        $t2, 0xD4($sp)
    /* DD7650 00006BE4 8E2B0000 */  lw         $t3, 0x0($s1)
    /* DD7654 00006BE8 448B3000 */  mtc1       $t3, $f6
    /* DD7658 00006BEC 00000000 */  nop
    /* DD765C 00006BF0 46803220 */  cvt.s.w    $f8, $f6
    /* DD7660 00006BF4 E7A800C8 */  swc1       $f8, 0xC8($sp)
    /* DD7664 00006BF8 8E2C0004 */  lw         $t4, 0x4($s1)
    /* DD7668 00006BFC 448C5000 */  mtc1       $t4, $f10
    /* DD766C 00006C00 00000000 */  nop
    /* DD7670 00006C04 46805420 */  cvt.s.w    $f16, $f10
    /* DD7674 00006C08 E7B000C4 */  swc1       $f16, 0xC4($sp)
    /* DD7678 00006C0C 8E2D0008 */  lw         $t5, 0x8($s1)
    /* DD767C 00006C10 448D9000 */  mtc1       $t5, $f18
    /* DD7680 00006C14 00000000 */  nop
    /* DD7684 00006C18 46809120 */  cvt.s.w    $f4, $f18
    /* DD7688 00006C1C 10000010 */  b          .L00006C60
    /* DD768C 00006C20 E7A400C0 */   swc1      $f4, 0xC0($sp)
    /* DD7690 00006C24 8E2E0000 */  lw         $t6, 0x0($s1)
    /* DD7694 00006C28 448E3000 */  mtc1       $t6, $f6
    /* DD7698 00006C2C 00000000 */  nop
    /* DD769C 00006C30 46803220 */  cvt.s.w    $f8, $f6
    /* DD76A0 00006C34 E7A800A4 */  swc1       $f8, 0xA4($sp)
    /* DD76A4 00006C38 8E2F0004 */  lw         $t7, 0x4($s1)
    /* DD76A8 00006C3C 448F5000 */  mtc1       $t7, $f10
    /* DD76AC 00006C40 00000000 */  nop
    /* DD76B0 00006C44 46805420 */  cvt.s.w    $f16, $f10
    /* DD76B4 00006C48 E7B000A0 */  swc1       $f16, 0xA0($sp)
    /* DD76B8 00006C4C 8E380008 */  lw         $t8, 0x8($s1)
    /* DD76BC 00006C50 44989000 */  mtc1       $t8, $f18
    /* DD76C0 00006C54 00000000 */  nop
    /* DD76C4 00006C58 46809120 */  cvt.s.w    $f4, $f18
    /* DD76C8 00006C5C E7A4009C */  swc1       $f4, 0x9C($sp)
  .L00006C60:
    /* DD76CC 00006C60 8FB900A8 */  lw         $t9, 0xA8($sp)
  .L00006C64:
    /* DD76D0 00006C64 8FA90048 */  lw         $t1, 0x48($sp)
    /* DD76D4 00006C68 2401000C */  addiu      $at, $zero, 0xC
    /* DD76D8 00006C6C 00194080 */  sll        $t0, $t9, 2
    /* DD76DC 00006C70 1521FF28 */  bne        $t1, $at, .L00006914
    /* DD76E0 00006C74 02288821 */   addu      $s1, $s1, $t0
    /* DD76E4 00006C78 0C000000 */  jal        func_00000000
    /* DD76E8 00006C7C 00000000 */   nop
    /* DD76EC 00006C80 8EA3003C */  lw         $v1, 0x3C($s5)
    /* DD76F0 00006C84 24070006 */  addiu      $a3, $zero, 0x6
    /* DD76F4 00006C88 3C140000 */  lui        $s4, (0x0 >> 16)
    /* DD76F8 00006C8C 10E30003 */  beq        $a3, $v1, .L00006C9C
    /* DD76FC 00006C90 26940000 */   addiu     $s4, $s4, 0x0
    /* DD7700 00006C94 2401000B */  addiu      $at, $zero, 0xB
    /* DD7704 00006C98 14610004 */  bne        $v1, $at, .L00006CAC
  .L00006C9C:
    /* DD7708 00006C9C 240A0005 */   addiu     $t2, $zero, 0x5
    /* DD770C 00006CA0 240B0002 */  addiu      $t3, $zero, 0x2
    /* DD7710 00006CA4 AFAA00B4 */  sw         $t2, 0xB4($sp)
    /* DD7714 00006CA8 AFAB00B0 */  sw         $t3, 0xB0($sp)
  .L00006CAC:
    /* DD7718 00006CAC 3C150000 */  lui        $s5, (0x0 >> 16)
    /* DD771C 00006CB0 26B50000 */  addiu      $s5, $s5, 0x0
    /* DD7720 00006CB4 AE800000 */  sw         $zero, 0x0($s4)
    /* DD7724 00006CB8 AEA70000 */  sw         $a3, 0x0($s5)
    /* DD7728 00006CBC 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD772C 00006CC0 8C840000 */  lw         $a0, 0x0($a0)
    /* DD7730 00006CC4 8FA500F8 */  lw         $a1, 0xF8($sp)
    /* DD7734 00006CC8 0C000000 */  jal        func_00000000
    /* DD7738 00006CCC 02C03025 */   or        $a2, $s6, $zero
    /* DD773C 00006CD0 3C120000 */  lui        $s2, (0x0 >> 16)
    /* DD7740 00006CD4 8E520000 */  lw         $s2, 0x0($s2)
    /* DD7744 00006CD8 8EF30088 */  lw         $s3, 0x88($s7)
    /* DD7748 00006CDC 02402825 */  or         $a1, $s2, $zero
    /* DD774C 00006CE0 0C000000 */  jal        func_00000000
    /* DD7750 00006CE4 26640010 */   addiu     $a0, $s3, 0x10
    /* DD7754 00006CE8 8E4C0014 */  lw         $t4, 0x14($s2)
    /* DD7758 00006CEC 3C040000 */  lui        $a0, %hi(D_00007F08)
    /* DD775C 00006CF0 24847F08 */  addiu      $a0, $a0, %lo(D_00007F08)
    /* DD7760 00006CF4 11800003 */  beqz       $t4, .L00006D04
    /* DD7764 00006CF8 00002825 */   or        $a1, $zero, $zero
    /* DD7768 00006CFC 240D0001 */  addiu      $t5, $zero, 0x1
    /* DD776C 00006D00 AE4D0004 */  sw         $t5, 0x4($s2)
  .L00006D04:
    /* DD7770 00006D04 0C000000 */  jal        func_00000000
    /* DD7774 00006D08 AE530014 */   sw        $s3, 0x14($s2)
    /* DD7778 00006D0C 8FAE00BC */  lw         $t6, 0xBC($sp)
    /* DD777C 00006D10 24090006 */  addiu      $t1, $zero, 0x6
    /* DD7780 00006D14 3C020000 */  lui        $v0, (0x0 >> 16)
    /* DD7784 00006D18 11C00008 */  beqz       $t6, .L00006D3C
    /* DD7788 00006D1C 3C0A000A */   lui       $t2, (0xA8002 >> 16)
    /* DD778C 00006D20 3C0F0000 */  lui        $t7, %hi(func_00000188 + 0x3C)
    /* DD7790 00006D24 8DEF01C4 */  lw         $t7, %lo(func_00000188 + 0x3C)($t7)
    /* DD7794 00006D28 3C010000 */  lui        $at, %hi(func_00000188 + 0x3C)
    /* DD7798 00006D2C 2416FFF7 */  addiu      $s6, $zero, -0x9
    /* DD779C 00006D30 35F80008 */  ori        $t8, $t7, 0x8
    /* DD77A0 00006D34 10000007 */  b          .L00006D54
    /* DD77A4 00006D38 AC3801C4 */   sw        $t8, %lo(func_00000188 + 0x3C)($at)
  .L00006D3C:
    /* DD77A8 00006D3C 3C190000 */  lui        $t9, %hi(func_00000188 + 0x3C)
    /* DD77AC 00006D40 8F3901C4 */  lw         $t9, %lo(func_00000188 + 0x3C)($t9)
    /* DD77B0 00006D44 2416FFF7 */  addiu      $s6, $zero, -0x9
    /* DD77B4 00006D48 3C010000 */  lui        $at, %hi(func_00000188 + 0x3C)
    /* DD77B8 00006D4C 03364024 */  and        $t0, $t9, $s6
    /* DD77BC 00006D50 AC2801C4 */  sw         $t0, %lo(func_00000188 + 0x3C)($at)
  .L00006D54:
    /* DD77C0 00006D54 AEA90000 */  sw         $t1, 0x0($s5)
    /* DD77C4 00006D58 8C420000 */  lw         $v0, 0x0($v0)
    /* DD77C8 00006D5C 3C010000 */  lui        $at, (0x0 >> 16)
    /* DD77CC 00006D60 354A8002 */  ori        $t2, $t2, (0xA8002 & 0xFFFF)
    /* DD77D0 00006D64 00561024 */  and        $v0, $v0, $s6
    /* DD77D4 00006D68 34420100 */  ori        $v0, $v0, 0x100
    /* DD77D8 00006D6C AC220000 */  sw         $v0, 0x0($at)
    /* DD77DC 00006D70 3C020000 */  lui        $v0, (0x0 >> 16)
    /* DD77E0 00006D74 24420000 */  addiu      $v0, $v0, 0x0
    /* DD77E4 00006D78 3C040000 */  lui        $a0, %hi(D_00007F0C)
    /* DD77E8 00006D7C AC4A0004 */  sw         $t2, 0x4($v0)
    /* DD77EC 00006D80 AC400000 */  sw         $zero, 0x0($v0)
    /* DD77F0 00006D84 24847F0C */  addiu      $a0, $a0, %lo(D_00007F0C)
    /* DD77F4 00006D88 0C000000 */  jal        func_00000000
    /* DD77F8 00006D8C 00002825 */   or        $a1, $zero, $zero
    /* DD77FC 00006D90 3C050000 */  lui        $a1, %hi(D_00007F18)
    /* DD7800 00006D94 24A57F18 */  addiu      $a1, $a1, %lo(D_00007F18)
    /* DD7804 00006D98 00002025 */  or         $a0, $zero, $zero
    /* DD7808 00006D9C 00003025 */  or         $a2, $zero, $zero
    /* DD780C 00006DA0 0C000000 */  jal        func_00000000
    /* DD7810 00006DA4 00003825 */   or        $a3, $zero, $zero
    /* DD7814 00006DA8 0C000000 */  jal        func_00000000
    /* DD7818 00006DAC 00408825 */   or        $s1, $v0, $zero
    /* DD781C 00006DB0 3C040000 */  lui        $a0, %hi(D_00007F28)
    /* DD7820 00006DB4 24847F28 */  addiu      $a0, $a0, %lo(D_00007F28)
    /* DD7824 00006DB8 0C000000 */  jal        func_00000000
    /* DD7828 00006DBC 00002825 */   or        $a1, $zero, $zero
    /* DD782C 00006DC0 02202025 */  or         $a0, $s1, $zero
    /* DD7830 00006DC4 0C000000 */  jal        func_00000000
    /* DD7834 00006DC8 8FA500D0 */   lw        $a1, 0xD0($sp)
    /* DD7838 00006DCC 0C000000 */  jal        func_00000000
    /* DD783C 00006DD0 00000000 */   nop
    /* DD7840 00006DD4 3C040000 */  lui        $a0, %hi(D_00007F34)
    /* DD7844 00006DD8 24847F34 */  addiu      $a0, $a0, %lo(D_00007F34)
    /* DD7848 00006DDC 0C000000 */  jal        func_00000000
    /* DD784C 00006DE0 00002825 */   or        $a1, $zero, $zero
    /* DD7850 00006DE4 8E230040 */  lw         $v1, 0x40($s1)
    /* DD7854 00006DE8 0C000000 */  jal        func_00000000
    /* DD7858 00006DEC 00032080 */   sll       $a0, $v1, 2
    /* DD785C 00006DF0 0C000000 */  jal        func_00000000
    /* DD7860 00006DF4 AE220050 */   sw        $v0, 0x50($s1)
    /* DD7864 00006DF8 3C040000 */  lui        $a0, %hi(D_00007F3C)
    /* DD7868 00006DFC 24847F3C */  addiu      $a0, $a0, %lo(D_00007F3C)
    /* DD786C 00006E00 0C000000 */  jal        func_00000000
    /* DD7870 00006E04 00002825 */   or        $a1, $zero, $zero
    /* DD7874 00006E08 0C000000 */  jal        func_00000000
    /* DD7878 00006E0C 02202025 */   or        $a0, $s1, $zero
    /* DD787C 00006E10 0C000000 */  jal        func_00000000
    /* DD7880 00006E14 00000000 */   nop
    /* DD7884 00006E18 3C020000 */  lui        $v0, (0x0 >> 16)
    /* DD7888 00006E1C 8C420000 */  lw         $v0, 0x0($v0)
    /* DD788C 00006E20 2401FEFF */  addiu      $at, $zero, -0x101
    /* DD7890 00006E24 3C0D0000 */  lui        $t5, %hi(func_00000188 + 0x3C)
    /* DD7894 00006E28 00411024 */  and        $v0, $v0, $at
    /* DD7898 00006E2C 3C010000 */  lui        $at, (0x0 >> 16)
  alabel D_00006E30
    /* DD789C 00006E30 AC220000 */  sw         $v0, 0x0($at)
    /* DD78A0 00006E34 26220030 */  addiu      $v0, $s1, 0x30
  alabel D_00006E38
    /* DD78A4 00006E38 8C4B0000 */  lw         $t3, 0x0($v0)
    /* DD78A8 00006E3C 3C010000 */  lui        $at, %hi(func_00000188 + 0x3C)
    /* DD78AC 00006E40 356C0040 */  ori        $t4, $t3, 0x40
    /* DD78B0 00006E44 AC4C0000 */  sw         $t4, 0x0($v0)
  alabel D_00006E48
    /* DD78B4 00006E48 8DAD01C4 */  lw         $t5, %lo(func_00000188 + 0x3C)($t5)
    /* DD78B8 00006E4C 01B67024 */  and        $t6, $t5, $s6
    /* DD78BC 00006E50 0C000000 */  jal        func_00000000
  alabel D_00006E54
    /* DD78C0 00006E54 AC2E01C4 */   sw        $t6, %lo(func_00000188 + 0x3C)($at)
    /* DD78C4 00006E58 3C040000 */  lui        $a0, %hi(D_00007F44)
    /* DD78C8 00006E5C 24847F44 */  addiu      $a0, $a0, %lo(D_00007F44)
  alabel D_00006E60
    /* DD78CC 00006E60 0C000000 */  jal        func_00000000
    /* DD78D0 00006E64 00002825 */   or        $a1, $zero, $zero
    /* DD78D4 00006E68 00002025 */  or         $a0, $zero, $zero
    /* DD78D8 00006E6C 02202825 */  or         $a1, $s1, $zero
  alabel D_00006E70
    /* DD78DC 00006E70 8EE60090 */  lw         $a2, 0x90($s7)
    /* DD78E0 00006E74 0C000000 */  jal        func_00000000
    /* DD78E4 00006E78 8EE70094 */   lw        $a3, 0x94($s7)
    /* DD78E8 00006E7C 8EF30084 */  lw         $s3, 0x84($s7)
  alabel D_00006E80
    /* DD78EC 00006E80 00408025 */  or         $s0, $v0, $zero
    /* DD78F0 00006E84 00409025 */  or         $s2, $v0, $zero
    /* DD78F4 00006E88 00402825 */  or         $a1, $v0, $zero
  alabel D_00006E8C
    /* DD78F8 00006E8C 0C000000 */  jal        func_00000000
    /* DD78FC 00006E90 26640010 */   addiu     $a0, $s3, 0x10
    /* DD7900 00006E94 8E4F0014 */  lw         $t7, 0x14($s2)
  alabel D_00006E98
    /* DD7904 00006E98 02002025 */  or         $a0, $s0, $zero
    /* DD7908 00006E9C 11E00002 */  beqz       $t7, .L00006EA8
    /* DD790C 00006EA0 24180001 */   addiu     $t8, $zero, 0x1
    /* DD7910 00006EA4 AE580004 */  sw         $t8, 0x4($s2)
  .L00006EA8:
    /* DD7914 00006EA8 0C000000 */  jal        func_00000000
    /* DD7918 00006EAC AE530014 */   sw        $s3, 0x14($s2)
    /* DD791C 00006EB0 0C000000 */  jal        func_00000000
    /* DD7920 00006EB4 00000000 */   nop
  alabel D_00006EB8
    /* DD7924 00006EB8 8FA800B0 */  lw         $t0, 0xB0($sp)
    /* DD7928 00006EBC 24190001 */  addiu      $t9, $zero, 0x1
    /* DD792C 00006EC0 AEB90000 */  sw         $t9, 0x0($s5)
  alabel D_00006EC4
    /* DD7930 00006EC4 00002025 */  or         $a0, $zero, $zero
    /* DD7934 00006EC8 02202825 */  or         $a1, $s1, $zero
    /* DD7938 00006ECC 8FA600B8 */  lw         $a2, 0xB8($sp)
    /* DD793C 00006ED0 8FA700B4 */  lw         $a3, 0xB4($sp)
    /* DD7940 00006ED4 0C000000 */  jal        func_00000000
    /* DD7944 00006ED8 AFA80010 */   sw        $t0, 0x10($sp)
    /* DD7948 00006EDC 24430018 */  addiu      $v1, $v0, 0x18
    /* DD794C 00006EE0 8C690000 */  lw         $t1, 0x0($v1)
    /* DD7950 00006EE4 2401FFBF */  addiu      $at, $zero, -0x41
    /* DD7954 00006EE8 00408025 */  or         $s0, $v0, $zero
    /* DD7958 00006EEC 01215024 */  and        $t2, $t1, $at
    /* DD795C 00006EF0 AC6A0000 */  sw         $t2, 0x0($v1)
    /* DD7960 00006EF4 00002025 */  or         $a0, $zero, $zero
    /* DD7964 00006EF8 0C000000 */  jal        func_00000000
    /* DD7968 00006EFC 02202825 */   or        $a1, $s1, $zero
    /* DD796C 00006F00 3C010000 */  lui        $at, (0x0 >> 16)
    /* DD7970 00006F04 3C040000 */  lui        $a0, %hi(D_00007F50)
    /* DD7974 00006F08 AC220000 */  sw         $v0, 0x0($at)
    /* DD7978 00006F0C 24847F50 */  addiu      $a0, $a0, %lo(D_00007F50)
    /* DD797C 00006F10 0C000000 */  jal        func_00000000
    /* DD7980 00006F14 00002825 */   or        $a1, $zero, $zero
    /* DD7984 00006F18 8FAB00DC */  lw         $t3, 0xDC($sp)
    /* DD7988 00006F1C 11600005 */  beqz       $t3, .L00006F34
    /* DD798C 00006F20 3C040000 */   lui       $a0, (0x0 >> 16)
    /* DD7990 00006F24 8C840000 */  lw         $a0, 0x0($a0)
    /* DD7994 00006F28 24050001 */  addiu      $a1, $zero, 0x1
    /* DD7998 00006F2C 0C000000 */  jal        func_00000000
    /* DD799C 00006F30 01603025 */   or        $a2, $t3, $zero
  .L00006F34:
    /* DD79A0 00006F34 0C000000 */  jal        func_00000000
    /* DD79A4 00006F38 00000000 */   nop
    /* DD79A8 00006F3C C7A600C8 */  lwc1       $f6, 0xC8($sp)
    /* DD79AC 00006F40 3C0C0000 */  lui        $t4, %hi(D_00007F58)
    /* DD79B0 00006F44 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD79B4 00006F48 E60600C4 */  swc1       $f6, 0xC4($s0)
    /* DD79B8 00006F4C C7A800C4 */  lwc1       $f8, 0xC4($sp)
    /* DD79BC 00006F50 00003025 */  or         $a2, $zero, $zero
    /* DD79C0 00006F54 E60800C8 */  swc1       $f8, 0xC8($s0)
    /* DD79C4 00006F58 C7AA00C0 */  lwc1       $f10, 0xC0($sp)
    /* DD79C8 00006F5C E60A00CC */  swc1       $f10, 0xCC($s0)
    /* DD79CC 00006F60 C7B000A4 */  lwc1       $f16, 0xA4($sp)
    /* DD79D0 00006F64 E61000D0 */  swc1       $f16, 0xD0($s0)
    /* DD79D4 00006F68 C7B200A0 */  lwc1       $f18, 0xA0($sp)
    /* DD79D8 00006F6C E61200D4 */  swc1       $f18, 0xD4($s0)
    /* DD79DC 00006F70 C7A4009C */  lwc1       $f4, 0x9C($sp)
    /* DD79E0 00006F74 E60400D8 */  swc1       $f4, 0xD8($s0)
    /* DD79E4 00006F78 8D857F58 */  lw         $a1, %lo(D_00007F58)($t4)
    /* DD79E8 00006F7C 8C840000 */  lw         $a0, 0x0($a0)
    /* DD79EC 00006F80 AFA500E4 */  sw         $a1, 0xE4($sp)
    /* DD79F0 00006F84 0C000000 */  jal        func_00000000
    /* DD79F4 00006F88 AFA50004 */   sw        $a1, 0x4($sp)
    /* DD79F8 00006F8C 1040000F */  beqz       $v0, .L00006FCC
    /* DD79FC 00006F90 00000000 */   nop
    /* DD7A00 00006F94 8C580030 */  lw         $t8, 0x30($v0)
    /* DD7A04 00006F98 27AE004C */  addiu      $t6, $sp, 0x4C
    /* DD7A08 00006F9C ADD80000 */  sw         $t8, 0x0($t6)
    /* DD7A0C 00006FA0 8C4F0034 */  lw         $t7, 0x34($v0)
    /* DD7A10 00006FA4 ADCF0004 */  sw         $t7, 0x4($t6)
    /* DD7A14 00006FA8 8C580038 */  lw         $t8, 0x38($v0)
    /* DD7A18 00006FAC ADD80008 */  sw         $t8, 0x8($t6)
    /* DD7A1C 00006FB0 C7A6004C */  lwc1       $f6, 0x4C($sp)
    /* DD7A20 00006FB4 E60600C4 */  swc1       $f6, 0xC4($s0)
    /* DD7A24 00006FB8 C7A80050 */  lwc1       $f8, 0x50($sp)
    /* DD7A28 00006FBC E60800C8 */  swc1       $f8, 0xC8($s0)
    /* DD7A2C 00006FC0 C7AA0054 */  lwc1       $f10, 0x54($sp)
    /* DD7A30 00006FC4 10000004 */  b          .L00006FD8
    /* DD7A34 00006FC8 E60A00CC */   swc1      $f10, 0xCC($s0)
  .L00006FCC:
    /* DD7A38 00006FCC 3C040000 */  lui        $a0, %hi(D_00007F5C)
    /* DD7A3C 00006FD0 0C000000 */  jal        func_00000000
    /* DD7A40 00006FD4 24847F5C */   addiu     $a0, $a0, %lo(D_00007F5C)
  .L00006FD8:
    /* DD7A44 00006FD8 3C190000 */  lui        $t9, %hi(D_00007F64)
    /* DD7A48 00006FDC 8F257F64 */  lw         $a1, %lo(D_00007F64)($t9)
    /* DD7A4C 00006FE0 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD7A50 00006FE4 8C840000 */  lw         $a0, 0x0($a0)
    /* DD7A54 00006FE8 00003025 */  or         $a2, $zero, $zero
    /* DD7A58 00006FEC AFA500E0 */  sw         $a1, 0xE0($sp)
    /* DD7A5C 00006FF0 0C000000 */  jal        func_00000000
    /* DD7A60 00006FF4 AFA50004 */   sw        $a1, 0x4($sp)
    /* DD7A64 00006FF8 1040000F */  beqz       $v0, .L00007038
    /* DD7A68 00006FFC 3C040000 */   lui       $a0, %hi(D_00007F68)
    /* DD7A6C 00007000 8C4B0030 */  lw         $t3, 0x30($v0)
    /* DD7A70 00007004 27A9004C */  addiu      $t1, $sp, 0x4C
    /* DD7A74 00007008 AD2B0000 */  sw         $t3, 0x0($t1)
    /* DD7A78 0000700C 8C4A0034 */  lw         $t2, 0x34($v0)
    /* DD7A7C 00007010 AD2A0004 */  sw         $t2, 0x4($t1)
    /* DD7A80 00007014 8C4B0038 */  lw         $t3, 0x38($v0)
    /* DD7A84 00007018 AD2B0008 */  sw         $t3, 0x8($t1)
    /* DD7A88 0000701C C7B0004C */  lwc1       $f16, 0x4C($sp)
    /* DD7A8C 00007020 E61000D0 */  swc1       $f16, 0xD0($s0)
    /* DD7A90 00007024 C7B20050 */  lwc1       $f18, 0x50($sp)
    /* DD7A94 00007028 E61200D4 */  swc1       $f18, 0xD4($s0)
    /* DD7A98 0000702C C7A40054 */  lwc1       $f4, 0x54($sp)
    /* DD7A9C 00007030 10000003 */  b          .L00007040
    /* DD7AA0 00007034 E60400D8 */   swc1      $f4, 0xD8($s0)
  .L00007038:
    /* DD7AA4 00007038 0C000000 */  jal        func_00000000
    /* DD7AA8 0000703C 24847F68 */   addiu     $a0, $a0, %lo(D_00007F68)
  .L00007040:
    /* DD7AAC 00007040 8EF30084 */  lw         $s3, 0x84($s7)
    /* DD7AB0 00007044 02002825 */  or         $a1, $s0, $zero
    /* DD7AB4 00007048 0C000000 */  jal        func_00000000
    /* DD7AB8 0000704C 26640010 */   addiu     $a0, $s3, 0x10
    /* DD7ABC 00007050 8E0C0014 */  lw         $t4, 0x14($s0)
    /* DD7AC0 00007054 3C040000 */  lui        $a0, %hi(D_00007F70)
    /* DD7AC4 00007058 240E0006 */  addiu      $t6, $zero, 0x6
    /* DD7AC8 0000705C 11800003 */  beqz       $t4, .L0000706C
    /* DD7ACC 00007060 24847F70 */   addiu     $a0, $a0, %lo(D_00007F70)
    /* DD7AD0 00007064 240D0001 */  addiu      $t5, $zero, 0x1
    /* DD7AD4 00007068 AE0D0004 */  sw         $t5, 0x4($s0)
  .L0000706C:
    /* DD7AD8 0000706C AE130014 */  sw         $s3, 0x14($s0)
    /* DD7ADC 00007070 AEAE0000 */  sw         $t6, 0x0($s5)
    /* DD7AE0 00007074 0C000000 */  jal        func_00000000
    /* DD7AE4 00007078 00002825 */   or        $a1, $zero, $zero
    /* DD7AE8 0000707C 3C050000 */  lui        $a1, %hi(D_00007F78)
    /* DD7AEC 00007080 24A57F78 */  addiu      $a1, $a1, %lo(D_00007F78)
    /* DD7AF0 00007084 00002025 */  or         $a0, $zero, $zero
    /* DD7AF4 00007088 0C000000 */  jal        func_00000000
    /* DD7AF8 0000708C 2406FFFF */   addiu     $a2, $zero, -0x1
    /* DD7AFC 00007090 AE820000 */  sw         $v0, 0x0($s4)
    /* DD7B00 00007094 3C020000 */  lui        $v0, (0x0 >> 16)
    /* DD7B04 00007098 8C420000 */  lw         $v0, 0x0($v0)
    /* DD7B08 0000709C 3C010000 */  lui        $at, (0x0 >> 16)
    /* DD7B0C 000070A0 34420008 */  ori        $v0, $v0, 0x8
    /* DD7B10 000070A4 AC220000 */  sw         $v0, 0x0($at)
    /* DD7B14 000070A8 0C000000 */  jal        func_00000000
    /* DD7B18 000070AC 8E840000 */   lw        $a0, 0x0($s4)
    /* DD7B1C 000070B0 8FAF00D4 */  lw         $t7, 0xD4($sp)
    /* DD7B20 000070B4 24050001 */  addiu      $a1, $zero, 0x1
    /* DD7B24 000070B8 11E00003 */  beqz       $t7, .L000070C8
    /* DD7B28 000070BC 01E03025 */   or        $a2, $t7, $zero
    /* DD7B2C 000070C0 0C000000 */  jal        func_00000000
    /* DD7B30 000070C4 8E840000 */   lw        $a0, 0x0($s4)
  .L000070C8:
    /* DD7B34 000070C8 8FB800F8 */  lw         $t8, 0xF8($sp)
    /* DD7B38 000070CC 24010001 */  addiu      $at, $zero, 0x1
    /* DD7B3C 000070D0 1701000C */  bne        $t8, $at, .L00007104
    /* DD7B40 000070D4 00000000 */   nop
    /* DD7B44 000070D8 8E920000 */  lw         $s2, 0x0($s4)
    /* DD7B48 000070DC 8EF30080 */  lw         $s3, 0x80($s7)
    /* DD7B4C 000070E0 02402825 */  or         $a1, $s2, $zero
    /* DD7B50 000070E4 0C000000 */  jal        func_00000000
    /* DD7B54 000070E8 26640010 */   addiu     $a0, $s3, 0x10
    /* DD7B58 000070EC 8E590014 */  lw         $t9, 0x14($s2)
    /* DD7B5C 000070F0 24080001 */  addiu      $t0, $zero, 0x1
    /* DD7B60 000070F4 53200003 */  beql       $t9, $zero, .L00007104
    /* DD7B64 000070F8 AE530014 */   sw        $s3, 0x14($s2)
    /* DD7B68 000070FC AE480004 */  sw         $t0, 0x4($s2)
    /* DD7B6C 00007100 AE530014 */  sw         $s3, 0x14($s2)
  .L00007104:
    /* DD7B70 00007104 0C000000 */  jal        func_00000000
    /* DD7B74 00007108 00000000 */   nop
    /* DD7B78 0000710C 3C020000 */  lui        $v0, (0x0 >> 16)
    /* DD7B7C 00007110 8C420000 */  lw         $v0, 0x0($v0)
    /* DD7B80 00007114 3C010000 */  lui        $at, (0x0 >> 16)
    /* DD7B84 00007118 8FBF0044 */  lw         $ra, 0x44($sp)
    /* DD7B88 0000711C 00561024 */  and        $v0, $v0, $s6
  alabel D_00007120
    /* DD7B8C 00007120 AC220000 */  sw         $v0, 0x0($at)
    /* DD7B90 00007124 02001025 */  or         $v0, $s0, $zero
    /* DD7B94 00007128 8FB00024 */  lw         $s0, 0x24($sp)
    /* DD7B98 0000712C 8FB6003C */  lw         $s6, 0x3C($sp)
    /* DD7B9C 00007130 8FB10028 */  lw         $s1, 0x28($sp)
    /* DD7BA0 00007134 8FB2002C */  lw         $s2, 0x2C($sp)
    /* DD7BA4 00007138 8FB30030 */  lw         $s3, 0x30($sp)
    /* DD7BA8 0000713C 8FB40034 */  lw         $s4, 0x34($sp)
    /* DD7BAC 00007140 8FB50038 */  lw         $s5, 0x38($sp)
    /* DD7BB0 00007144 8FB70040 */  lw         $s7, 0x40($sp)
    /* DD7BB4 00007148 03E00008 */  jr         $ra
    /* DD7BB8 0000714C 27BD00F0 */   addiu     $sp, $sp, 0xF0
endlabel func_00006808
