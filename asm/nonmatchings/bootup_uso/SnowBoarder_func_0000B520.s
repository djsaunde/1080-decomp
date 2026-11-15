nonmatching SnowBoarder_func_0000B520, 0x23C

glabel SnowBoarder_func_0000B520
    /* DDBF8C 0000B520 27BDFF80 */  addiu      $sp, $sp, -0x80
    /* DDBF90 0000B524 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDBF94 0000B528 8C8E0A58 */  lw         $t6, 0xA58($a0)
    /* DDBF98 0000B52C 44800000 */  mtc1       $zero, $f0
    /* DDBF9C 0000B530 00802825 */  or         $a1, $a0, $zero
    /* DDBFA0 0000B534 31CF0100 */  andi       $t7, $t6, 0x100
    /* DDBFA4 0000B538 11E00012 */  beqz       $t7, .L0000B584
    /* DDBFA8 0000B53C 46000086 */   mov.s     $f2, $f0
    /* DDBFAC 0000B540 8C820800 */  lw         $v0, 0x800($a0)
    /* DDBFB0 0000B544 3C014120 */  lui        $at, (0x41200000 >> 16)
    /* DDBFB4 0000B548 8C580010 */  lw         $t8, 0x10($v0)
    /* DDBFB8 0000B54C 33190004 */  andi       $t9, $t8, 0x4
    /* DDBFBC 0000B550 53200005 */  beql       $t9, $zero, .L0000B568
    /* DDBFC0 0000B554 8C480010 */   lw        $t0, 0x10($v0)
    /* DDBFC4 0000B558 44812000 */  mtc1       $at, $f4
    /* DDBFC8 0000B55C 00000000 */  nop
    /* DDBFCC 0000B560 46040080 */  add.s      $f2, $f0, $f4
    /* DDBFD0 0000B564 8C480010 */  lw         $t0, 0x10($v0)
  .L0000B568:
    /* DDBFD4 0000B568 3C014120 */  lui        $at, (0x41200000 >> 16)
    /* DDBFD8 0000B56C 31090008 */  andi       $t1, $t0, 0x8
    /* DDBFDC 0000B570 51200005 */  beql       $t1, $zero, .L0000B588
    /* DDBFE0 0000B574 24A203C8 */   addiu     $v0, $a1, 0x3C8
    /* DDBFE4 0000B578 44813000 */  mtc1       $at, $f6
    /* DDBFE8 0000B57C 00000000 */  nop
    /* DDBFEC 0000B580 46061081 */  sub.s      $f2, $f2, $f6
  .L0000B584:
    /* DDBFF0 0000B584 24A203C8 */  addiu      $v0, $a1, 0x3C8
  .L0000B588:
    /* DDBFF4 0000B588 C4480000 */  lwc1       $f8, 0x0($v0)
    /* DDBFF8 0000B58C C44A0004 */  lwc1       $f10, 0x4($v0)
    /* DDBFFC 0000B590 C4500008 */  lwc1       $f16, 0x8($v0)
    /* DDC000 0000B594 46024002 */  mul.s      $f0, $f8, $f2
    /* DDC004 0000B598 27AA0050 */  addiu      $t2, $sp, 0x50
    /* DDC008 0000B59C 27A2005C */  addiu      $v0, $sp, 0x5C
    /* DDC00C 0000B5A0 46025302 */  mul.s      $f12, $f10, $f2
    /* DDC010 0000B5A4 27AD0074 */  addiu      $t5, $sp, 0x74
    /* DDC014 0000B5A8 24A40318 */  addiu      $a0, $a1, 0x318
    /* DDC018 0000B5AC 46028382 */  mul.s      $f14, $f16, $f2
    /* DDC01C 0000B5B0 E7A00050 */  swc1       $f0, 0x50($sp)
    /* DDC020 0000B5B4 E7AC0054 */  swc1       $f12, 0x54($sp)
    /* DDC024 0000B5B8 E7AE0058 */  swc1       $f14, 0x58($sp)
    /* DDC028 0000B5BC 8D4C0000 */  lw         $t4, 0x0($t2)
    /* DDC02C 0000B5C0 AC4C0000 */  sw         $t4, 0x0($v0)
    /* DDC030 0000B5C4 8D4B0004 */  lw         $t3, 0x4($t2)
    /* DDC034 0000B5C8 8C4F0000 */  lw         $t7, 0x0($v0)
    /* DDC038 0000B5CC AC4B0004 */  sw         $t3, 0x4($v0)
    /* DDC03C 0000B5D0 8D4C0008 */  lw         $t4, 0x8($t2)
    /* DDC040 0000B5D4 AC4C0008 */  sw         $t4, 0x8($v0)
    /* DDC044 0000B5D8 ADAF0000 */  sw         $t7, 0x0($t5)
    /* DDC048 0000B5DC 8C4E0004 */  lw         $t6, 0x4($v0)
    /* DDC04C 0000B5E0 ADAE0004 */  sw         $t6, 0x4($t5)
    /* DDC050 0000B5E4 8C4F0008 */  lw         $t7, 0x8($v0)
    /* DDC054 0000B5E8 ADAF0008 */  sw         $t7, 0x8($t5)
    /* DDC058 0000B5EC C4B20318 */  lwc1       $f18, 0x318($a1)
    /* DDC05C 0000B5F0 C7A40074 */  lwc1       $f4, 0x74($sp)
    /* DDC060 0000B5F4 C4A8031C */  lwc1       $f8, 0x31C($a1)
    /* DDC064 0000B5F8 8CB80A58 */  lw         $t8, 0xA58($a1)
    /* DDC068 0000B5FC 46049180 */  add.s      $f6, $f18, $f4
    /* DDC06C 0000B600 C4B20320 */  lwc1       $f18, 0x320($a1)
    /* DDC070 0000B604 0018CB80 */  sll        $t9, $t8, 14
    /* DDC074 0000B608 E4A60318 */  swc1       $f6, 0x318($a1)
    /* DDC078 0000B60C C7AA0078 */  lwc1       $f10, 0x78($sp)
    /* DDC07C 0000B610 460A4400 */  add.s      $f16, $f8, $f10
    /* DDC080 0000B614 E4B0031C */  swc1       $f16, 0x31C($a1)
    /* DDC084 0000B618 C7A4007C */  lwc1       $f4, 0x7C($sp)
    /* DDC088 0000B61C 46049180 */  add.s      $f6, $f18, $f4
    /* DDC08C 0000B620 07210025 */  bgez       $t9, .L0000B6B8
    /* DDC090 0000B624 E4A60320 */   swc1      $f6, 0x320($a1)
    /* DDC094 0000B628 8CA20908 */  lw         $v0, 0x908($a1)
    /* DDC098 0000B62C C4A20768 */  lwc1       $f2, 0x768($a1)
    /* DDC09C 0000B630 50400017 */  beql       $v0, $zero, .L0000B690
    /* DDC0A0 0000B634 C4A00A78 */   lwc1      $f0, 0xA78($a1)
    /* DDC0A4 0000B638 C44800BC */  lwc1       $f8, 0xBC($v0)
    /* DDC0A8 0000B63C C4AA00BC */  lwc1       $f10, 0xBC($a1)
    /* DDC0AC 0000B640 3C010001 */  lui        $at, %hi(FogTmpl_create + 0x48)
    /* DDC0B0 0000B644 C42C816C */  lwc1       $f12, %lo(FogTmpl_create + 0x48)($at)
    /* DDC0B4 0000B648 460A4001 */  sub.s      $f0, $f8, $f10
    /* DDC0B8 0000B64C 3C010001 */  lui        $at, %hi(FogTmpl_create + 0x44)
    /* DDC0BC 0000B650 4600603C */  c.lt.s     $f12, $f0
    /* DDC0C0 0000B654 00000000 */  nop
    /* DDC0C4 0000B658 45020005 */  bc1fl      .L0000B670
    /* DDC0C8 0000B65C 46006487 */   neg.s     $f18, $f12
    /* DDC0CC 0000B660 C4308168 */  lwc1       $f16, %lo(FogTmpl_create + 0x44)($at)
    /* DDC0D0 0000B664 10000009 */  b          .L0000B68C
    /* DDC0D4 0000B668 46101080 */   add.s     $f2, $f2, $f16
    /* DDC0D8 0000B66C 46006487 */  neg.s      $f18, $f12
  .L0000B670:
    /* DDC0DC 0000B670 3C010001 */  lui        $at, %hi(FogTmpl_create + 0x44)
    /* DDC0E0 0000B674 4612003C */  c.lt.s     $f0, $f18
    /* DDC0E4 0000B678 00000000 */  nop
    /* DDC0E8 0000B67C 45020004 */  bc1fl      .L0000B690
    /* DDC0EC 0000B680 C4A00A78 */   lwc1      $f0, 0xA78($a1)
    /* DDC0F0 0000B684 C4248168 */  lwc1       $f4, %lo(FogTmpl_create + 0x44)($at)
    /* DDC0F4 0000B688 46041081 */  sub.s      $f2, $f2, $f4
  .L0000B68C:
    /* DDC0F8 0000B68C C4A00A78 */  lwc1       $f0, 0xA78($a1)
  .L0000B690:
    /* DDC0FC 0000B690 3C010000 */  lui        $at, %hi(func_000008B4 + 0xC)
    /* DDC100 0000B694 D42A08C0 */  ldc1       $f10, %lo(func_000008B4 + 0xC)($at)
    /* DDC104 0000B698 46001181 */  sub.s      $f6, $f2, $f0
    /* DDC108 0000B69C 460004A1 */  cvt.d.s    $f18, $f0
    /* DDC10C 0000B6A0 46003221 */  cvt.d.s    $f8, $f6
    /* DDC110 0000B6A4 462A4402 */  mul.d      $f16, $f8, $f10
    /* DDC114 0000B6A8 46309100 */  add.d      $f4, $f18, $f16
    /* DDC118 0000B6AC 462021A0 */  cvt.s.d    $f6, $f4
    /* DDC11C 0000B6B0 10000003 */  b          .L0000B6C0
    /* DDC120 0000B6B4 E4A60A78 */   swc1      $f6, 0xA78($a1)
  .L0000B6B8:
    /* DDC124 0000B6B8 C4A80768 */  lwc1       $f8, 0x768($a1)
    /* DDC128 0000B6BC E4A80A78 */  swc1       $f8, 0xA78($a1)
  .L0000B6C0:
    /* DDC12C 0000B6C0 C4AA0A78 */  lwc1       $f10, 0xA78($a1)
    /* DDC130 0000B6C4 C4B20348 */  lwc1       $f18, 0x348($a1)
    /* DDC134 0000B6C8 4612503C */  c.lt.s     $f10, $f18
    /* DDC138 0000B6CC 00000000 */  nop
    /* DDC13C 0000B6D0 45020011 */  bc1fl      .L0000B718
    /* DDC140 0000B6D4 8CA80938 */   lw        $t0, 0x938($a1)
    /* DDC144 0000B6D8 0C000000 */  jal        func_00000000
    /* DDC148 0000B6DC AFA50080 */   sw        $a1, 0x80($sp)
    /* DDC14C 0000B6E0 8FA50080 */  lw         $a1, 0x80($sp)
    /* DDC150 0000B6E4 C4A00A78 */  lwc1       $f0, 0xA78($a1)
    /* DDC154 0000B6E8 C4B00318 */  lwc1       $f16, 0x318($a1)
    /* DDC158 0000B6EC C4A6031C */  lwc1       $f6, 0x31C($a1)
    /* DDC15C 0000B6F0 C4AA0320 */  lwc1       $f10, 0x320($a1)
    /* DDC160 0000B6F4 46008102 */  mul.s      $f4, $f16, $f0
    /* DDC164 0000B6F8 00000000 */  nop
    /* DDC168 0000B6FC 46003202 */  mul.s      $f8, $f6, $f0
    /* DDC16C 0000B700 00000000 */  nop
    /* DDC170 0000B704 46005482 */  mul.s      $f18, $f10, $f0
    /* DDC174 0000B708 E4A40318 */  swc1       $f4, 0x318($a1)
    /* DDC178 0000B70C E4A8031C */  swc1       $f8, 0x31C($a1)
    /* DDC17C 0000B710 E4B20320 */  swc1       $f18, 0x320($a1)
    /* DDC180 0000B714 8CA80938 */  lw         $t0, 0x938($a1)
  .L0000B718:
    /* DDC184 0000B718 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DDC188 0000B71C 5500000C */  bnel       $t0, $zero, .L0000B750
    /* DDC18C 0000B720 8FBF0014 */   lw        $ra, 0x14($sp)
    /* DDC190 0000B724 44818000 */  mtc1       $at, $f16
    /* DDC194 0000B728 C4A406C0 */  lwc1       $f4, 0x6C0($a1)
    /* DDC198 0000B72C C4A60318 */  lwc1       $f6, 0x318($a1)
    /* DDC19C 0000B730 C4AA0320 */  lwc1       $f10, 0x320($a1)
    /* DDC1A0 0000B734 46048001 */  sub.s      $f0, $f16, $f4
    /* DDC1A4 0000B738 46003202 */  mul.s      $f8, $f6, $f0
    /* DDC1A8 0000B73C 00000000 */  nop
    /* DDC1AC 0000B740 46005482 */  mul.s      $f18, $f10, $f0
    /* DDC1B0 0000B744 E4A80318 */  swc1       $f8, 0x318($a1)
    /* DDC1B4 0000B748 E4B20320 */  swc1       $f18, 0x320($a1)
    /* DDC1B8 0000B74C 8FBF0014 */  lw         $ra, 0x14($sp)
  .L0000B750:
    /* DDC1BC 0000B750 27BD0080 */  addiu      $sp, $sp, 0x80
    /* DDC1C0 0000B754 03E00008 */  jr         $ra
    /* DDC1C4 0000B758 00000000 */   nop
endlabel SnowBoarder_func_0000B520
