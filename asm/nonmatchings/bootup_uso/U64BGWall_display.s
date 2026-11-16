nonmatching U64BGWall_display, 0x1CC

glabel U64BGWall_display
    /* DD24B0 00001A44 3C0E0000 */  lui        $t6, %hi(snd_seqEndDbg_00000188 + 0x3C)
    /* DD24B4 00001A48 8DCE01C4 */  lw         $t6, %lo(snd_seqEndDbg_00000188 + 0x3C)($t6)
    /* DD24B8 00001A4C 27BDFFC8 */  addiu      $sp, $sp, -0x38
    /* DD24BC 00001A50 AFBF0024 */  sw         $ra, 0x24($sp)
    /* DD24C0 00001A54 31CF0002 */  andi       $t7, $t6, 0x2
    /* DD24C4 00001A58 15E00069 */  bnez       $t7, .L00001C00
    /* DD24C8 00001A5C AFA40038 */   sw        $a0, 0x38($sp)
    /* DD24CC 00001A60 3C090000 */  lui        $t1, (0x0 >> 16)
    /* DD24D0 00001A64 25290000 */  addiu      $t1, $t1, 0x0
    /* DD24D4 00001A68 8D380254 */  lw         $t8, 0x254($t1)
    /* DD24D8 00001A6C 8FAD0038 */  lw         $t5, 0x38($sp)
    /* DD24DC 00001A70 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DD24E0 00001A74 8F030158 */  lw         $v1, 0x158($t8)
    /* DD24E4 00001A78 8DAE002C */  lw         $t6, 0x2C($t5)
    /* DD24E8 00001A7C 44815000 */  mtc1       $at, $f10
    /* DD24EC 00001A80 8C68003C */  lw         $t0, 0x3C($v1)
    /* DD24F0 00001A84 448E2000 */  mtc1       $t6, $f4
    /* DD24F4 00001A88 8DB80034 */  lw         $t8, 0x34($t5)
    /* DD24F8 00001A8C 8D0B0004 */  lw         $t3, 0x4($t0)
    /* DD24FC 00001A90 8DAF0030 */  lw         $t7, 0x30($t5)
    /* DD2500 00001A94 8D190000 */  lw         $t9, 0x0($t0)
    /* DD2504 00001A98 000B6180 */  sll        $t4, $t3, 6
    /* DD2508 00001A9C 3C0B0000 */  lui        $t3, (0x0 >> 16)
    /* DD250C 00001AA0 8D6B0000 */  lw         $t3, 0x0($t3)
    /* DD2510 00001AA4 46802120 */  cvt.s.w    $f4, $f4
    /* DD2514 00001AA8 E7AA0010 */  swc1       $f10, 0x10($sp)
    /* DD2518 00001AAC 8D620070 */  lw         $v0, 0x70($t3)
    /* DD251C 00001AB0 C5320128 */  lwc1       $f18, 0x128($t1)
    /* DD2520 00001AB4 44984000 */  mtc1       $t8, $f8
    /* DD2524 00001AB8 C45000A0 */  lwc1       $f16, 0xA0($v0)
    /* DD2528 00001ABC 448F3000 */  mtc1       $t7, $f6
    /* DD252C 00001AC0 44052000 */  mfc1       $a1, $f4
    /* DD2530 00001AC4 46128102 */  mul.s      $f4, $f16, $f18
    /* DD2534 00001AC8 C5320130 */  lwc1       $f18, 0x130($t1)
    /* DD2538 00001ACC 032C2021 */  addu       $a0, $t9, $t4
    /* DD253C 00001AD0 46804220 */  cvt.s.w    $f8, $f8
    /* DD2540 00001AD4 E7A40014 */  swc1       $f4, 0x14($sp)
    /* DD2544 00001AD8 468031A0 */  cvt.s.w    $f6, $f6
    /* DD2548 00001ADC 44074000 */  mfc1       $a3, $f8
    /* DD254C 00001AE0 C528012C */  lwc1       $f8, 0x12C($t1)
    /* DD2550 00001AE4 44063000 */  mfc1       $a2, $f6
    /* DD2554 00001AE8 C44600A4 */  lwc1       $f6, 0xA4($v0)
    /* DD2558 00001AEC 46083282 */  mul.s      $f10, $f6, $f8
    /* DD255C 00001AF0 E7AA0018 */  swc1       $f10, 0x18($sp)
    /* DD2560 00001AF4 C45000A8 */  lwc1       $f16, 0xA8($v0)
    /* DD2564 00001AF8 AFA30034 */  sw         $v1, 0x34($sp)
    /* DD2568 00001AFC 46128102 */  mul.s      $f4, $f16, $f18
    /* DD256C 00001B00 0C000000 */  jal        func_00000000
    /* DD2570 00001B04 E7A4001C */   swc1      $f4, 0x1C($sp)
    /* DD2574 00001B08 8FA30034 */  lw         $v1, 0x34($sp)
    /* DD2578 00001B0C 3C0D0102 */  lui        $t5, (0x1020040 >> 16)
    /* DD257C 00001B10 35AD0040 */  ori        $t5, $t5, (0x1020040 & 0xFFFF)
    /* DD2580 00001B14 8C65000C */  lw         $a1, 0xC($v1)
    /* DD2584 00001B18 8CA60004 */  lw         $a2, 0x4($a1)
    /* DD2588 00001B1C 24D90001 */  addiu      $t9, $a2, 0x1
    /* DD258C 00001B20 ACB90004 */  sw         $t9, 0x4($a1)
    /* DD2590 00001B24 8C6C000C */  lw         $t4, 0xC($v1)
    /* DD2594 00001B28 000678C0 */  sll        $t7, $a2, 3
    /* DD2598 00001B2C 8D8E0000 */  lw         $t6, 0x0($t4)
    /* DD259C 00001B30 01CF5021 */  addu       $t2, $t6, $t7
    /* DD25A0 00001B34 AD4D0000 */  sw         $t5, 0x0($t2)
    /* DD25A4 00001B38 8C68003C */  lw         $t0, 0x3C($v1)
    /* DD25A8 00001B3C 8D0B0004 */  lw         $t3, 0x4($t0)
    /* DD25AC 00001B40 8D180000 */  lw         $t8, 0x0($t0)
    /* DD25B0 00001B44 AFAA0028 */  sw         $t2, 0x28($sp)
    /* DD25B4 00001B48 000BC980 */  sll        $t9, $t3, 6
    /* DD25B8 00001B4C 0C000000 */  jal        func_00000000
    /* DD25BC 00001B50 03192021 */   addu      $a0, $t8, $t9
    /* DD25C0 00001B54 8FAA0028 */  lw         $t2, 0x28($sp)
    /* DD25C4 00001B58 8FA30034 */  lw         $v1, 0x34($sp)
    /* DD25C8 00001B5C 3C190600 */  lui        $t9, (0x6000000 >> 16)
    /* DD25CC 00001B60 AD420004 */  sw         $v0, 0x4($t2)
    /* DD25D0 00001B64 8C65003C */  lw         $a1, 0x3C($v1)
    /* DD25D4 00001B68 8CA60004 */  lw         $a2, 0x4($a1)
    /* DD25D8 00001B6C 24CC0001 */  addiu      $t4, $a2, 0x1
    /* DD25DC 00001B70 ACAC0004 */  sw         $t4, 0x4($a1)
    /* DD25E0 00001B74 8C64000C */  lw         $a0, 0xC($v1)
    /* DD25E4 00001B78 8FAE0038 */  lw         $t6, 0x38($sp)
    /* DD25E8 00001B7C 8C870004 */  lw         $a3, 0x4($a0)
    /* DD25EC 00001B80 8DC90038 */  lw         $t1, 0x38($t6)
    /* DD25F0 00001B84 24EF0001 */  addiu      $t7, $a3, 0x1
    /* DD25F4 00001B88 AC8F0004 */  sw         $t7, 0x4($a0)
    /* DD25F8 00001B8C 8C6D000C */  lw         $t5, 0xC($v1)
    /* DD25FC 00001B90 0007C0C0 */  sll        $t8, $a3, 3
    /* DD2600 00001B94 8DAB0000 */  lw         $t3, 0x0($t5)
    /* DD2604 00001B98 01784021 */  addu       $t0, $t3, $t8
    /* DD2608 00001B9C AD190000 */  sw         $t9, 0x0($t0)
    /* DD260C 00001BA0 AD090004 */  sw         $t1, 0x4($t0)
    /* DD2610 00001BA4 8C65000C */  lw         $a1, 0xC($v1)
    /* DD2614 00001BA8 3C0BE700 */  lui        $t3, (0xE7000000 >> 16)
    /* DD2618 00001BAC 8CA60004 */  lw         $a2, 0x4($a1)
    /* DD261C 00001BB0 24CC0001 */  addiu      $t4, $a2, 0x1
    /* DD2620 00001BB4 ACAC0004 */  sw         $t4, 0x4($a1)
    /* DD2624 00001BB8 8C6E000C */  lw         $t6, 0xC($v1)
    /* DD2628 00001BBC 000668C0 */  sll        $t5, $a2, 3
    /* DD262C 00001BC0 8DCF0000 */  lw         $t7, 0x0($t6)
    /* DD2630 00001BC4 01ED1021 */  addu       $v0, $t7, $t5
    /* DD2634 00001BC8 AC4B0000 */  sw         $t3, 0x0($v0)
    /* DD2638 00001BCC AC400004 */  sw         $zero, 0x4($v0)
    /* DD263C 00001BD0 8C65000C */  lw         $a1, 0xC($v1)
    /* DD2640 00001BD4 3C0FBA00 */  lui        $t7, (0xBA000E02 >> 16)
    /* DD2644 00001BD8 35EF0E02 */  ori        $t7, $t7, (0xBA000E02 & 0xFFFF)
    /* DD2648 00001BDC 8CA60004 */  lw         $a2, 0x4($a1)
    /* DD264C 00001BE0 24D80001 */  addiu      $t8, $a2, 0x1
    /* DD2650 00001BE4 ACB80004 */  sw         $t8, 0x4($a1)
    /* DD2654 00001BE8 8C79000C */  lw         $t9, 0xC($v1)
    /* DD2658 00001BEC 000670C0 */  sll        $t6, $a2, 3
    /* DD265C 00001BF0 8F2C0000 */  lw         $t4, 0x0($t9)
    /* DD2660 00001BF4 018E2021 */  addu       $a0, $t4, $t6
    /* DD2664 00001BF8 AC8F0000 */  sw         $t7, 0x0($a0)
    /* DD2668 00001BFC AC800004 */  sw         $zero, 0x4($a0)
  .L00001C00:
    /* DD266C 00001C00 8FBF0024 */  lw         $ra, 0x24($sp)
    /* DD2670 00001C04 27BD0038 */  addiu      $sp, $sp, 0x38
    /* DD2674 00001C08 03E00008 */  jr         $ra
    /* DD2678 00001C0C 00000000 */   nop
endlabel U64BGWall_display
