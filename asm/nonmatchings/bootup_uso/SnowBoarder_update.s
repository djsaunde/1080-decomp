nonmatching SnowBoarder_update, 0x4C0

glabel SnowBoarder_update
    /* DDDEAC 0000D440 27BDFF90 */  addiu      $sp, $sp, -0x70
    /* DDDEB0 0000D444 AFB00030 */  sw         $s0, 0x30($sp)
    /* DDDEB4 0000D448 00808025 */  or         $s0, $a0, $zero
    /* DDDEB8 0000D44C AFBF0034 */  sw         $ra, 0x34($sp)
    /* DDDEBC 0000D450 3C040001 */  lui        $a0, %hi(stream_readVec3_00008A7C + 0x54)
    /* DDDEC0 0000D454 0C000000 */  jal        func_00000000
    /* DDDEC4 0000D458 24848AD0 */   addiu     $a0, $a0, %lo(stream_readVec3_00008A7C + 0x54)
    /* DDDEC8 0000D45C 8E030A58 */  lw         $v1, 0xA58($s0)
    /* DDDECC 0000D460 30620100 */  andi       $v0, $v1, 0x100
    /* DDDED0 0000D464 1040001C */  beqz       $v0, .L0000D4D8
    /* DDDED4 0000D468 00000000 */   nop
    /* DDDED8 0000D46C 8E020800 */  lw         $v0, 0x800($s0)
    /* DDDEDC 0000D470 8C4E0018 */  lw         $t6, 0x18($v0)
    /* DDDEE0 0000D474 31CF0020 */  andi       $t7, $t6, 0x20
    /* DDDEE4 0000D478 51E00006 */  beql       $t7, $zero, .L0000D494
    /* DDDEE8 0000D47C 8E080A28 */   lw        $t0, 0xA28($s0)
    /* DDDEEC 0000D480 8E180A28 */  lw         $t8, 0xA28($s0)
    /* DDDEF0 0000D484 8E020800 */  lw         $v0, 0x800($s0)
    /* DDDEF4 0000D488 3B190001 */  xori       $t9, $t8, 0x1
    /* DDDEF8 0000D48C AE190A28 */  sw         $t9, 0xA28($s0)
    /* DDDEFC 0000D490 8E080A28 */  lw         $t0, 0xA28($s0)
  .L0000D494:
    /* DDDF00 0000D494 51000009 */  beql       $t0, $zero, .L0000D4BC
    /* DDDF04 0000D498 8C4B0018 */   lw        $t3, 0x18($v0)
    /* DDDF08 0000D49C 8C490018 */  lw         $t1, 0x18($v0)
    /* DDDF0C 0000D4A0 312A0010 */  andi       $t2, $t1, 0x10
    /* DDDF10 0000D4A4 51400112 */  beql       $t2, $zero, .L0000D8F0
    /* DDDF14 0000D4A8 8FBF0034 */   lw        $ra, 0x34($sp)
    /* DDDF18 0000D4AC 8E030A58 */  lw         $v1, 0xA58($s0)
    /* DDDF1C 0000D4B0 10000009 */  b          .L0000D4D8
    /* DDDF20 0000D4B4 30620100 */   andi      $v0, $v1, 0x100
    /* DDDF24 0000D4B8 8C4B0018 */  lw         $t3, 0x18($v0)
  .L0000D4BC:
    /* DDDF28 0000D4BC 8E030A58 */  lw         $v1, 0xA58($s0)
    /* DDDF2C 0000D4C0 316C0010 */  andi       $t4, $t3, 0x10
    /* DDDF30 0000D4C4 11800003 */  beqz       $t4, .L0000D4D4
    /* DDDF34 0000D4C8 386D0004 */   xori      $t5, $v1, 0x4
    /* DDDF38 0000D4CC AE0D0A58 */  sw         $t5, 0xA58($s0)
    /* DDDF3C 0000D4D0 01A01825 */  or         $v1, $t5, $zero
  .L0000D4D4:
    /* DDDF40 0000D4D4 30620100 */  andi       $v0, $v1, 0x100
  .L0000D4D8:
    /* DDDF44 0000D4D8 50400015 */  beql       $v0, $zero, .L0000D530
    /* DDDF48 0000D4DC 306A0004 */   andi      $t2, $v1, 0x4
    /* DDDF4C 0000D4E0 8E040800 */  lw         $a0, 0x800($s0)
    /* DDDF50 0000D4E4 24080009 */  addiu      $t0, $zero, 0x9
    /* DDDF54 0000D4E8 8C820010 */  lw         $v0, 0x10($a0)
    /* DDDF58 0000D4EC 8C8E0010 */  lw         $t6, 0x10($a0)
    /* DDDF5C 0000D4F0 30420080 */  andi       $v0, $v0, 0x80
    /* DDDF60 0000D4F4 31CF0040 */  andi       $t7, $t6, 0x40
    /* DDDF64 0000D4F8 000FC02B */  sltu       $t8, $zero, $t7
    /* DDDF68 0000D4FC 0002102B */  sltu       $v0, $zero, $v0
    /* DDDF6C 0000D500 0302C824 */  and        $t9, $t8, $v0
    /* DDDF70 0000D504 5320000A */  beql       $t9, $zero, .L0000D530
    /* DDDF74 0000D508 306A0004 */   andi      $t2, $v1, 0x4
    /* DDDF78 0000D50C AFA80058 */  sw         $t0, 0x58($sp)
    /* DDDF7C 0000D510 8E020028 */  lw         $v0, 0x28($s0)
    /* DDDF80 0000D514 27A50058 */  addiu      $a1, $sp, 0x58
    /* DDDF84 0000D518 8C59002C */  lw         $t9, 0x2C($v0)
    /* DDDF88 0000D51C 84490028 */  lh         $t1, 0x28($v0)
    /* DDDF8C 0000D520 0320F809 */  jalr       $t9
    /* DDDF90 0000D524 01302021 */   addu      $a0, $t1, $s0
    /* DDDF94 0000D528 8E030A58 */  lw         $v1, 0xA58($s0)
    /* DDDF98 0000D52C 306A0004 */  andi       $t2, $v1, 0x4
  .L0000D530:
    /* DDDF9C 0000D530 51400012 */  beql       $t2, $zero, .L0000D57C
    /* DDDFA0 0000D534 306D0080 */   andi      $t5, $v1, 0x80
    /* DDDFA4 0000D538 8E020800 */  lw         $v0, 0x800($s0)
    /* DDDFA8 0000D53C 240B0001 */  addiu      $t3, $zero, 0x1
    /* DDDFAC 0000D540 3C040001 */  lui        $a0, %hi(stream_readVec3_00008A7C + 0x5C)
    /* DDDFB0 0000D544 24848AD8 */  addiu      $a0, $a0, %lo(stream_readVec3_00008A7C + 0x5C)
    /* DDDFB4 0000D548 0C000000 */  jal        func_00000000
    /* DDDFB8 0000D54C AC4B003C */   sw        $t3, 0x3C($v0)
    /* DDDFBC 0000D550 8E030804 */  lw         $v1, 0x804($s0)
    /* DDDFC0 0000D554 8C620028 */  lw         $v0, 0x28($v1)
    /* DDDFC4 0000D558 8C590024 */  lw         $t9, 0x24($v0)
    /* DDDFC8 0000D55C 844C0020 */  lh         $t4, 0x20($v0)
    /* DDDFCC 0000D560 0320F809 */  jalr       $t9
    /* DDDFD0 0000D564 01832021 */   addu      $a0, $t4, $v1
    /* DDDFD4 0000D568 3C040001 */  lui        $a0, %hi(stream_readVec3_00008A7C + 0x64)
    /* DDDFD8 0000D56C 0C000000 */  jal        func_00000000
    /* DDDFDC 0000D570 24848AE0 */   addiu     $a0, $a0, %lo(stream_readVec3_00008A7C + 0x64)
    /* DDDFE0 0000D574 8E030A58 */  lw         $v1, 0xA58($s0)
    /* DDDFE4 0000D578 306D0080 */  andi       $t5, $v1, 0x80
  .L0000D57C:
    /* DDDFE8 0000D57C 11A00004 */  beqz       $t5, .L0000D590
    /* DDDFEC 0000D580 26040808 */   addiu     $a0, $s0, 0x808
    /* DDDFF0 0000D584 C6040A1C */  lwc1       $f4, 0xA1C($s0)
    /* DDDFF4 0000D588 46002187 */  neg.s      $f6, $f4
    /* DDDFF8 0000D58C E6060A1C */  swc1       $f6, 0xA1C($s0)
  .L0000D590:
    /* DDDFFC 0000D590 860509A4 */  lh         $a1, 0x9A4($s0)
    /* DDE000 0000D594 0C000000 */  jal        func_00000000
    /* DDE004 0000D598 AFA40048 */   sw        $a0, 0x48($sp)
    /* DDE008 0000D59C 8E030840 */  lw         $v1, 0x840($s0)
    /* DDE00C 0000D5A0 A60009A4 */  sh         $zero, 0x9A4($s0)
    /* DDE010 0000D5A4 8C620028 */  lw         $v0, 0x28($v1)
    /* DDE014 0000D5A8 8C590024 */  lw         $t9, 0x24($v0)
    /* DDE018 0000D5AC 844E0020 */  lh         $t6, 0x20($v0)
    /* DDE01C 0000D5B0 0320F809 */  jalr       $t9
    /* DDE020 0000D5B4 01C32021 */   addu      $a0, $t6, $v1
    /* DDE024 0000D5B8 44800000 */  mtc1       $zero, $f0
    /* DDE028 0000D5BC 8E0F0A58 */  lw         $t7, 0xA58($s0)
    /* DDE02C 0000D5C0 A6000A10 */  sh         $zero, 0xA10($s0)
    /* DDE030 0000D5C4 E6000A1C */  swc1       $f0, 0xA1C($s0)
    /* DDE034 0000D5C8 31F80800 */  andi       $t8, $t7, 0x800
    /* DDE038 0000D5CC E6000A20 */  swc1       $f0, 0xA20($s0)
    /* DDE03C 0000D5D0 13000034 */  beqz       $t8, .L0000D6A4
    /* DDE040 0000D5D4 E6000A24 */   swc1      $f0, 0xA24($s0)
    /* DDE044 0000D5D8 3C040001 */  lui        $a0, %hi(stream_readVec3_00008A7C + 0x6C)
    /* DDE048 0000D5DC 0C000000 */  jal        func_00000000
    /* DDE04C 0000D5E0 24848AE8 */   addiu     $a0, $a0, %lo(stream_readVec3_00008A7C + 0x6C)
    /* DDE050 0000D5E4 0C000000 */  jal        func_00000000
    /* DDE054 0000D5E8 02002025 */   or        $a0, $s0, $zero
    /* DDE058 0000D5EC 3C040001 */  lui        $a0, %hi(stream_readQuad_00008AEC + 0x8)
    /* DDE05C 0000D5F0 0C000000 */  jal        func_00000000
    /* DDE060 0000D5F4 24848AF4 */   addiu     $a0, $a0, %lo(stream_readQuad_00008AEC + 0x8)
    /* DDE064 0000D5F8 8E080A58 */  lw         $t0, 0xA58($s0)
    /* DDE068 0000D5FC 3C0142C8 */  lui        $at, (0x42C80000 >> 16)
    /* DDE06C 0000D600 00084B40 */  sll        $t1, $t0, 13
    /* DDE070 0000D604 05210027 */  bgez       $t1, .L0000D6A4
    /* DDE074 0000D608 00000000 */   nop
    /* DDE078 0000D60C C600031C */  lwc1       $f0, 0x31C($s0)
    /* DDE07C 0000D610 44804000 */  mtc1       $zero, $f8
    /* DDE080 0000D614 44815000 */  mtc1       $at, $f10
    /* DDE084 0000D618 3C014080 */  lui        $at, (0x40800000 >> 16)
    /* DDE088 0000D61C 4608003C */  c.lt.s     $f0, $f8
    /* DDE08C 0000D620 00000000 */  nop
    /* DDE090 0000D624 45020004 */  bc1fl      .L0000D638
    /* DDE094 0000D628 46000086 */   mov.s     $f2, $f0
    /* DDE098 0000D62C 10000002 */  b          .L0000D638
    /* DDE09C 0000D630 46000087 */   neg.s     $f2, $f0
    /* DDE0A0 0000D634 46000086 */  mov.s      $f2, $f0
  .L0000D638:
    /* DDE0A4 0000D638 460A1401 */  sub.s      $f16, $f2, $f10
    /* DDE0A8 0000D63C 44819000 */  mtc1       $at, $f18
    /* DDE0AC 0000D640 00000000 */  nop
    /* DDE0B0 0000D644 46128103 */  div.s      $f4, $f16, $f18
    /* DDE0B4 0000D648 4600218D */  trunc.w.s  $f6, $f4
    /* DDE0B8 0000D64C 44023000 */  mfc1       $v0, $f6
    /* DDE0BC 0000D650 00000000 */  nop
    /* DDE0C0 0000D654 04410002 */  bgez       $v0, .L0000D660
    /* DDE0C4 0000D658 AE020A84 */   sw        $v0, 0xA84($s0)
    /* DDE0C8 0000D65C AE000A84 */  sw         $zero, 0xA84($s0)
  .L0000D660:
    /* DDE0CC 0000D660 8FA40048 */  lw         $a0, 0x48($sp)
    /* DDE0D0 0000D664 0C000000 */  jal        func_00000000
    /* DDE0D4 0000D668 8E050A84 */   lw        $a1, 0xA84($s0)
    /* DDE0D8 0000D66C 8E0208B8 */  lw         $v0, 0x8B8($s0)
    /* DDE0DC 0000D670 304B0001 */  andi       $t3, $v0, 0x1
    /* DDE0E0 0000D674 51600006 */  beql       $t3, $zero, .L0000D690
    /* DDE0E4 0000D678 304E0004 */   andi      $t6, $v0, 0x4
    /* DDE0E8 0000D67C 8E0C0A84 */  lw         $t4, 0xA84($s0)
    /* DDE0EC 0000D680 8E0D081C */  lw         $t5, 0x81C($s0)
    /* DDE0F0 0000D684 ADAC0094 */  sw         $t4, 0x94($t5)
    /* DDE0F4 0000D688 8E0208B8 */  lw         $v0, 0x8B8($s0)
    /* DDE0F8 0000D68C 304E0004 */  andi       $t6, $v0, 0x4
  .L0000D690:
    /* DDE0FC 0000D690 11C00004 */  beqz       $t6, .L0000D6A4
    /* DDE100 0000D694 00000000 */   nop
    /* DDE104 0000D698 8E190A84 */  lw         $t9, 0xA84($s0)
    /* DDE108 0000D69C 8E0F0818 */  lw         $t7, 0x818($s0)
    /* DDE10C 0000D6A0 ADF90094 */  sw         $t9, 0x94($t7)
  .L0000D6A4:
    /* DDE110 0000D6A4 3C040001 */  lui        $a0, %hi(stream_readQuad_00008AEC + 0x14)
    /* DDE114 0000D6A8 0C000000 */  jal        func_00000000
    /* DDE118 0000D6AC 24848B00 */   addiu     $a0, $a0, %lo(stream_readQuad_00008AEC + 0x14)
    /* DDE11C 0000D6B0 C60803C8 */  lwc1       $f8, 0x3C8($s0)
    /* DDE120 0000D6B4 C60A0318 */  lwc1       $f10, 0x318($s0)
    /* DDE124 0000D6B8 C61203CC */  lwc1       $f18, 0x3CC($s0)
    /* DDE128 0000D6BC C604031C */  lwc1       $f4, 0x31C($s0)
    /* DDE12C 0000D6C0 460A4402 */  mul.s      $f16, $f8, $f10
    /* DDE130 0000D6C4 C60A03D0 */  lwc1       $f10, 0x3D0($s0)
    /* DDE134 0000D6C8 8E040840 */  lw         $a0, 0x840($s0)
    /* DDE138 0000D6CC 46049182 */  mul.s      $f6, $f18, $f4
    /* DDE13C 0000D6D0 C6120320 */  lwc1       $f18, 0x320($s0)
    /* DDE140 0000D6D4 46125102 */  mul.s      $f4, $f10, $f18
    /* DDE144 0000D6D8 46068200 */  add.s      $f8, $f16, $f6
    /* DDE148 0000D6DC 46044400 */  add.s      $f16, $f8, $f4
    /* DDE14C 0000D6E0 0C000000 */  jal        func_00000000
    /* DDE150 0000D6E4 E6100A38 */   swc1      $f16, 0xA38($s0)
    /* DDE154 0000D6E8 8E180A5C */  lw         $t8, 0xA5C($s0)
    /* DDE158 0000D6EC 13000046 */  beqz       $t8, .L0000D808
    /* DDE15C 0000D6F0 00000000 */   nop
    /* DDE160 0000D6F4 C60600DC */  lwc1       $f6, 0xDC($s0)
    /* DDE164 0000D6F8 C61200E4 */  lwc1       $f18, 0xE4($s0)
    /* DDE168 0000D6FC 3C011234 */  lui        $at, (0x12345678 >> 16)
    /* DDE16C 0000D700 4600328D */  trunc.w.s  $f10, $f6
    /* DDE170 0000D704 34215678 */  ori        $at, $at, (0x12345678 & 0xFFFF)
    /* DDE174 0000D708 4600920D */  trunc.w.s  $f8, $f18
    /* DDE178 0000D70C 44095000 */  mfc1       $t1, $f10
    /* DDE17C 0000D710 440B4000 */  mfc1       $t3, $f8
    /* DDE180 0000D714 00000000 */  nop
    /* DDE184 0000D718 012B2026 */  xor        $a0, $t1, $t3
    /* DDE188 0000D71C 00042143 */  sra        $a0, $a0, 5
    /* DDE18C 0000D720 0C000000 */  jal        func_00000000
    /* DDE190 0000D724 00812026 */   xor       $a0, $a0, $at
    /* DDE194 0000D728 8E0C0A5C */  lw         $t4, 0xA5C($s0)
    /* DDE198 0000D72C 3C013FE0 */  lui        $at, (0x3FE00000 >> 16)
    /* DDE19C 0000D730 44817800 */  mtc1       $at, $f15
    /* DDE1A0 0000D734 448C9000 */  mtc1       $t4, $f18
    /* DDE1A4 0000D738 44807000 */  mtc1       $zero, $f14
    /* DDE1A8 0000D73C 46000121 */  cvt.d.s    $f4, $f0
    /* DDE1AC 0000D740 46809220 */  cvt.s.w    $f8, $f18
    /* DDE1B0 0000D744 3C01403E */  lui        $at, (0x403E0000 >> 16)
    /* DDE1B4 0000D748 44813800 */  mtc1       $at, $f7
    /* DDE1B8 0000D74C 44803000 */  mtc1       $zero, $f6
    /* DDE1BC 0000D750 3C0142C8 */  lui        $at, (0x42C80000 >> 16)
    /* DDE1C0 0000D754 462E2401 */  sub.d      $f16, $f4, $f14
    /* DDE1C4 0000D758 44812000 */  mtc1       $at, $f4
    /* DDE1C8 0000D75C 8E020824 */  lw         $v0, 0x824($s0)
    /* DDE1CC 0000D760 46268282 */  mul.d      $f10, $f16, $f6
    /* DDE1D0 0000D764 46044403 */  div.s      $f16, $f8, $f4
    /* DDE1D4 0000D768 C4480064 */  lwc1       $f8, 0x64($v0)
    /* DDE1D8 0000D76C 24420064 */  addiu      $v0, $v0, 0x64
    /* DDE1DC 0000D770 460081A1 */  cvt.d.s    $f6, $f16
    /* DDE1E0 0000D774 46265482 */  mul.d      $f18, $f10, $f6
    /* DDE1E4 0000D778 462090A0 */  cvt.s.d    $f2, $f18
    /* DDE1E8 0000D77C 46024100 */  add.s      $f4, $f8, $f2
    /* DDE1EC 0000D780 46001221 */  cvt.d.s    $f8, $f2
    /* DDE1F0 0000D784 E4440000 */  swc1       $f4, 0x0($v0)
    /* DDE1F4 0000D788 8E020828 */  lw         $v0, 0x828($s0)
    /* DDE1F8 0000D78C 462E4302 */  mul.d      $f12, $f8, $f14
    /* DDE1FC 0000D790 C4500064 */  lwc1       $f16, 0x64($v0)
    /* DDE200 0000D794 24420064 */  addiu      $v0, $v0, 0x64
    /* DDE204 0000D798 46028280 */  add.s      $f10, $f16, $f2
    /* DDE208 0000D79C E44A0000 */  swc1       $f10, 0x0($v0)
    /* DDE20C 0000D7A0 8E020830 */  lw         $v0, 0x830($s0)
    /* DDE210 0000D7A4 C4460064 */  lwc1       $f6, 0x64($v0)
    /* DDE214 0000D7A8 24420064 */  addiu      $v0, $v0, 0x64
    /* DDE218 0000D7AC 46023480 */  add.s      $f18, $f6, $f2
    /* DDE21C 0000D7B0 E4520000 */  swc1       $f18, 0x0($v0)
    /* DDE220 0000D7B4 8E020834 */  lw         $v0, 0x834($s0)
    /* DDE224 0000D7B8 C4440064 */  lwc1       $f4, 0x64($v0)
    /* DDE228 0000D7BC 24420064 */  addiu      $v0, $v0, 0x64
    /* DDE22C 0000D7C0 46002421 */  cvt.d.s    $f16, $f4
    /* DDE230 0000D7C4 462C8280 */  add.d      $f10, $f16, $f12
    /* DDE234 0000D7C8 462051A0 */  cvt.s.d    $f6, $f10
    /* DDE238 0000D7CC E4460000 */  swc1       $f6, 0x0($v0)
    /* DDE23C 0000D7D0 8E02082C */  lw         $v0, 0x82C($s0)
    /* DDE240 0000D7D4 C4520064 */  lwc1       $f18, 0x64($v0)
    /* DDE244 0000D7D8 24420064 */  addiu      $v0, $v0, 0x64
    /* DDE248 0000D7DC 46009221 */  cvt.d.s    $f8, $f18
    /* DDE24C 0000D7E0 462C4100 */  add.d      $f4, $f8, $f12
    /* DDE250 0000D7E4 46202420 */  cvt.s.d    $f16, $f4
    /* DDE254 0000D7E8 E4500000 */  swc1       $f16, 0x0($v0)
    /* DDE258 0000D7EC 8E020838 */  lw         $v0, 0x838($s0)
    /* DDE25C 0000D7F0 C44A0064 */  lwc1       $f10, 0x64($v0)
    /* DDE260 0000D7F4 24420064 */  addiu      $v0, $v0, 0x64
    /* DDE264 0000D7F8 460051A1 */  cvt.d.s    $f6, $f10
    /* DDE268 0000D7FC 462C3481 */  sub.d      $f18, $f6, $f12
    /* DDE26C 0000D800 46209220 */  cvt.s.d    $f8, $f18
    /* DDE270 0000D804 E4480000 */  swc1       $f8, 0x0($v0)
  .L0000D808:
    /* DDE274 0000D808 3C040001 */  lui        $a0, %hi(stream_readQuad_00008AEC + 0x20)
    /* DDE278 0000D80C 0C000000 */  jal        func_00000000
    /* DDE27C 0000D810 24848B0C */   addiu     $a0, $a0, %lo(stream_readQuad_00008AEC + 0x20)
    /* DDE280 0000D814 3C040001 */  lui        $a0, %hi(stream_readQuad_00008AEC + 0x2C)
    /* DDE284 0000D818 0C000000 */  jal        func_00000000
    /* DDE288 0000D81C 24848B18 */   addiu     $a0, $a0, %lo(stream_readQuad_00008AEC + 0x2C)
    /* DDE28C 0000D820 0C000000 */  jal        func_00000000
    /* DDE290 0000D824 02002025 */   or        $a0, $s0, $zero
    /* DDE294 0000D828 3C040001 */  lui        $a0, %hi(stream_readQuad_00008AEC + 0x38)
    /* DDE298 0000D82C 0C000000 */  jal        func_00000000
    /* DDE29C 0000D830 24848B24 */   addiu     $a0, $a0, %lo(stream_readQuad_00008AEC + 0x38)
    /* DDE2A0 0000D834 3C014000 */  lui        $at, (0x40000000 >> 16)
    /* DDE2A4 0000D838 44818000 */  mtc1       $at, $f16
    /* DDE2A8 0000D83C C6040348 */  lwc1       $f4, 0x348($s0)
    /* DDE2AC 0000D840 3C040000 */  lui        $a0, %hi(stream_readQuad_000000F0 + 0x48)
    /* DDE2B0 0000D844 8C840138 */  lw         $a0, %lo(stream_readQuad_000000F0 + 0x48)($a0)
    /* DDE2B4 0000D848 46102283 */  div.s      $f10, $f4, $f16
    /* DDE2B8 0000D84C 02002825 */  or         $a1, $s0, $zero
    /* DDE2BC 0000D850 44065000 */  mfc1       $a2, $f10
    /* DDE2C0 0000D854 0C000000 */  jal        func_00000000
    /* DDE2C4 0000D858 00000000 */   nop
    /* DDE2C8 0000D85C 3C040000 */  lui        $a0, %hi(stream_readQuad_000000F0 + 0x48)
    /* DDE2CC 0000D860 8C840138 */  lw         $a0, %lo(stream_readQuad_000000F0 + 0x48)($a0)
    /* DDE2D0 0000D864 02002825 */  or         $a1, $s0, $zero
    /* DDE2D4 0000D868 8E0600DC */  lw         $a2, 0xDC($s0)
    /* DDE2D8 0000D86C 0C000000 */  jal        func_00000000
    /* DDE2DC 0000D870 8E0700E4 */   lw        $a3, 0xE4($s0)
    /* DDE2E0 0000D874 3C040000 */  lui        $a0, %hi(stream_readQuad_000000F0 + 0x48)
    /* DDE2E4 0000D878 8C840138 */  lw         $a0, %lo(stream_readQuad_000000F0 + 0x48)($a0)
    /* DDE2E8 0000D87C 02002825 */  or         $a1, $s0, $zero
    /* DDE2EC 0000D880 0C000000 */  jal        func_00000000
    /* DDE2F0 0000D884 8E060A14 */   lw        $a2, 0xA14($s0)
    /* DDE2F4 0000D888 8E0308DC */  lw         $v1, 0x8DC($s0)
    /* DDE2F8 0000D88C 3C010000 */  lui        $at, %hi(D_00000980)
    /* DDE2FC 0000D890 10600013 */  beqz       $v1, .L0000D8E0
    /* DDE300 0000D894 00000000 */   nop
    /* DDE304 0000D898 C6080970 */  lwc1       $f8, 0x970($s0)
    /* DDE308 0000D89C C6060968 */  lwc1       $f6, 0x968($s0)
    /* DDE30C 0000D8A0 8E060348 */  lw         $a2, 0x348($s0)
    /* DDE310 0000D8A4 E7A80010 */  swc1       $f8, 0x10($sp)
    /* DDE314 0000D8A8 C60409D0 */  lwc1       $f4, 0x9D0($s0)
    /* DDE318 0000D8AC C4300980 */  lwc1       $f16, %lo(D_00000980)($at)
    /* DDE31C 0000D8B0 46003487 */  neg.s      $f18, $f6
    /* DDE320 0000D8B4 8FA40048 */  lw         $a0, 0x48($sp)
    /* DDE324 0000D8B8 46102280 */  add.s      $f10, $f4, $f16
    /* DDE328 0000D8BC 44079000 */  mfc1       $a3, $f18
    /* DDE32C 0000D8C0 260500DC */  addiu      $a1, $s0, 0xDC
    /* DDE330 0000D8C4 E7AA0014 */  swc1       $f10, 0x14($sp)
    /* DDE334 0000D8C8 860D09A2 */  lh         $t5, 0x9A2($s0)
    /* DDE338 0000D8CC AFA3001C */  sw         $v1, 0x1C($sp)
    /* DDE33C 0000D8D0 AFAD0018 */  sw         $t5, 0x18($sp)
    /* DDE340 0000D8D4 8E0E0A58 */  lw         $t6, 0xA58($s0)
    /* DDE344 0000D8D8 0C000000 */  jal        func_00000000
    /* DDE348 0000D8DC AFAE0020 */   sw        $t6, 0x20($sp)
  .L0000D8E0:
    /* DDE34C 0000D8E0 3C040001 */  lui        $a0, %hi(stream_readQuad_00008AEC + 0x44)
    /* DDE350 0000D8E4 0C000000 */  jal        func_00000000
    /* DDE354 0000D8E8 24848B30 */   addiu     $a0, $a0, %lo(stream_readQuad_00008AEC + 0x44)
    /* DDE358 0000D8EC 8FBF0034 */  lw         $ra, 0x34($sp)
  .L0000D8F0:
    /* DDE35C 0000D8F0 8FB00030 */  lw         $s0, 0x30($sp)
    /* DDE360 0000D8F4 27BD0070 */  addiu      $sp, $sp, 0x70
    /* DDE364 0000D8F8 03E00008 */  jr         $ra
    /* DDE368 0000D8FC 00000000 */   nop
endlabel SnowBoarder_update
