nonmatching SnowBoarder_func_0000B75C, 0x830

glabel SnowBoarder_func_0000B75C
    /* DDC1C8 0000B75C 44800000 */  mtc1       $zero, $f0
    /* DDC1CC 0000B760 27BDFF88 */  addiu      $sp, $sp, -0x78
    /* DDC1D0 0000B764 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DDC1D4 0000B768 44812000 */  mtc1       $at, $f4
    /* DDC1D8 0000B76C AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDC1DC 0000B770 E7A0006C */  swc1       $f0, 0x6C($sp)
    /* DDC1E0 0000B774 E7A00074 */  swc1       $f0, 0x74($sp)
    /* DDC1E4 0000B778 E7A40070 */  swc1       $f4, 0x70($sp)
    /* DDC1E8 0000B77C 8C8E0800 */  lw         $t6, 0x800($a0)
    /* DDC1EC 0000B780 3C010000 */  lui        $at, %hi(SndHandle_setScaleIdx15_000008B4 + 0x14)
    /* DDC1F0 0000B784 D42808C8 */  ldc1       $f8, %lo(SndHandle_setScaleIdx15_000008B4 + 0x14)($at)
    /* DDC1F4 0000B788 C5C60000 */  lwc1       $f6, 0x0($t6)
    /* DDC1F8 0000B78C 3C010000 */  lui        $at, %hi(SndHandle_setScaleIdx15_000008B4 + 0x1C)
    /* DDC1FC 0000B790 D42408D0 */  ldc1       $f4, %lo(SndHandle_setScaleIdx15_000008B4 + 0x1C)($at)
    /* DDC200 0000B794 46003021 */  cvt.d.s    $f0, $f6
    /* DDC204 0000B798 24010064 */  addiu      $at, $zero, 0x64
    /* DDC208 0000B79C 46280282 */  mul.d      $f10, $f0, $f8
    /* DDC20C 0000B7A0 00803825 */  or         $a3, $a0, $zero
    /* DDC210 0000B7A4 46240182 */  mul.d      $f6, $f0, $f4
    /* DDC214 0000B7A8 46205320 */  cvt.s.d    $f12, $f10
    /* DDC218 0000B7AC 46203220 */  cvt.s.d    $f8, $f6
    /* DDC21C 0000B7B0 E7A80060 */  swc1       $f8, 0x60($sp)
    /* DDC220 0000B7B4 8C8F0960 */  lw         $t7, 0x960($a0)
    /* DDC224 0000B7B8 55E10005 */  bnel       $t7, $at, .L0000B7D0
    /* DDC228 0000B7BC C4E00974 */   lwc1      $f0, 0x974($a3)
    /* DDC22C 0000B7C0 44806000 */  mtc1       $zero, $f12
    /* DDC230 0000B7C4 00000000 */  nop
    /* DDC234 0000B7C8 E7AC0060 */  swc1       $f12, 0x60($sp)
    /* DDC238 0000B7CC C4E00974 */  lwc1       $f0, 0x974($a3)
  .L0000B7D0:
    /* DDC23C 0000B7D0 C4EA0720 */  lwc1       $f10, 0x720($a3)
    /* DDC240 0000B7D4 C4E20A00 */  lwc1       $f2, 0xA00($a3)
    /* DDC244 0000B7D8 8CE20990 */  lw         $v0, 0x990($a3)
    /* DDC248 0000B7DC 460A0102 */  mul.s      $f4, $f0, $f10
    /* DDC24C 0000B7E0 44807000 */  mtc1       $zero, $f14
    /* DDC250 0000B7E4 46022182 */  mul.s      $f6, $f4, $f2
    /* DDC254 0000B7E8 46060201 */  sub.s      $f8, $f0, $f6
    /* DDC258 0000B7EC 46026102 */  mul.s      $f4, $f12, $f2
    /* DDC25C 0000B7F0 E4E80974 */  swc1       $f8, 0x974($a3)
    /* DDC260 0000B7F4 C4EA0974 */  lwc1       $f10, 0x974($a3)
    /* DDC264 0000B7F8 46045180 */  add.s      $f6, $f10, $f4
    /* DDC268 0000B7FC 10400007 */  beqz       $v0, .L0000B81C
    /* DDC26C 0000B800 E4E60974 */   swc1      $f6, 0x974($a3)
    /* DDC270 0000B804 E7AE005C */  swc1       $f14, 0x5C($sp)
    /* DDC274 0000B808 C4E803C4 */  lwc1       $f8, 0x3C4($a3)
    /* DDC278 0000B80C C4F203C0 */  lwc1       $f18, 0x3C0($a3)
    /* DDC27C 0000B810 C4F003BC */  lwc1       $f16, 0x3BC($a3)
    /* DDC280 0000B814 1000000F */  b          .L0000B854
    /* DDC284 0000B818 E7A8001C */   swc1      $f8, 0x1C($sp)
  .L0000B81C:
    /* DDC288 0000B81C C4F003BC */  lwc1       $f16, 0x3BC($a3)
    /* DDC28C 0000B820 C7AA006C */  lwc1       $f10, 0x6C($sp)
    /* DDC290 0000B824 C4F203C0 */  lwc1       $f18, 0x3C0($a3)
    /* DDC294 0000B828 C7A60070 */  lwc1       $f6, 0x70($sp)
    /* DDC298 0000B82C 460A8102 */  mul.s      $f4, $f16, $f10
    /* DDC29C 0000B830 C4E003C4 */  lwc1       $f0, 0x3C4($a3)
    /* DDC2A0 0000B834 46069202 */  mul.s      $f8, $f18, $f6
    /* DDC2A4 0000B838 C7A60074 */  lwc1       $f6, 0x74($sp)
    /* DDC2A8 0000B83C E7A0001C */  swc1       $f0, 0x1C($sp)
    /* DDC2AC 0000B840 46082280 */  add.s      $f10, $f4, $f8
    /* DDC2B0 0000B844 46060102 */  mul.s      $f4, $f0, $f6
    /* DDC2B4 0000B848 46045200 */  add.s      $f8, $f10, $f4
    /* DDC2B8 0000B84C 46004187 */  neg.s      $f6, $f8
    /* DDC2BC 0000B850 E7A6005C */  swc1       $f6, 0x5C($sp)
  .L0000B854:
    /* DDC2C0 0000B854 C4E00978 */  lwc1       $f0, 0x978($a3)
    /* DDC2C4 0000B858 C7AA005C */  lwc1       $f10, 0x5C($sp)
    /* DDC2C8 0000B85C 3C010000 */  lui        $at, %hi(SndHandle_setScaleIdx31_000008D4 + 0x4)
    /* DDC2CC 0000B860 D42608D8 */  ldc1       $f6, %lo(SndHandle_setScaleIdx31_000008D4 + 0x4)($at)
    /* DDC2D0 0000B864 46005101 */  sub.s      $f4, $f10, $f0
    /* DDC2D4 0000B868 C4E2030C */  lwc1       $f2, 0x30C($a3)
    /* DDC2D8 0000B86C C4EC0310 */  lwc1       $f12, 0x310($a3)
    /* DDC2DC 0000B870 C4EE0314 */  lwc1       $f14, 0x314($a3)
    /* DDC2E0 0000B874 46002221 */  cvt.d.s    $f8, $f4
    /* DDC2E4 0000B878 46000121 */  cvt.d.s    $f4, $f0
    /* DDC2E8 0000B87C 46264282 */  mul.d      $f10, $f8, $f6
    /* DDC2EC 0000B880 3C013FE0 */  lui        $at, (0x3FE00000 >> 16)
    /* DDC2F0 0000B884 462A2200 */  add.d      $f8, $f4, $f10
    /* DDC2F4 0000B888 C4E403C8 */  lwc1       $f4, 0x3C8($a3)
    /* DDC2F8 0000B88C 462041A0 */  cvt.s.d    $f6, $f8
    /* DDC2FC 0000B890 46022282 */  mul.s      $f10, $f4, $f2
    /* DDC300 0000B894 C4E803CC */  lwc1       $f8, 0x3CC($a3)
    /* DDC304 0000B898 E4E60978 */  swc1       $f6, 0x978($a3)
    /* DDC308 0000B89C 460C4182 */  mul.s      $f6, $f8, $f12
    /* DDC30C 0000B8A0 C4E803D0 */  lwc1       $f8, 0x3D0($a3)
    /* DDC310 0000B8A4 46065100 */  add.s      $f4, $f10, $f6
    /* DDC314 0000B8A8 460E4282 */  mul.s      $f10, $f8, $f14
    /* DDC318 0000B8AC 460A2180 */  add.s      $f6, $f4, $f10
    /* DDC31C 0000B8B0 46028202 */  mul.s      $f8, $f16, $f2
    /* DDC320 0000B8B4 00000000 */  nop
    /* DDC324 0000B8B8 460C9102 */  mul.s      $f4, $f18, $f12
    /* DDC328 0000B8BC E7A60058 */  swc1       $f6, 0x58($sp)
    /* DDC32C 0000B8C0 C7A6001C */  lwc1       $f6, 0x1C($sp)
    /* DDC330 0000B8C4 46044280 */  add.s      $f10, $f8, $f4
    /* DDC334 0000B8C8 460E3202 */  mul.s      $f8, $f6, $f14
    /* DDC338 0000B8CC C7A60060 */  lwc1       $f6, 0x60($sp)
    /* DDC33C 0000B8D0 46085100 */  add.s      $f4, $f10, $f8
    /* DDC340 0000B8D4 10400007 */  beqz       $v0, .L0000B8F4
    /* DDC344 0000B8D8 E7A40054 */   swc1      $f4, 0x54($sp)
    /* DDC348 0000B8DC 44814800 */  mtc1       $at, $f9
    /* DDC34C 0000B8E0 44804000 */  mtc1       $zero, $f8
    /* DDC350 0000B8E4 460032A1 */  cvt.d.s    $f10, $f6
    /* DDC354 0000B8E8 46285102 */  mul.d      $f4, $f10, $f8
    /* DDC358 0000B8EC 462021A0 */  cvt.s.d    $f6, $f4
    /* DDC35C 0000B8F0 E7A60060 */  swc1       $f6, 0x60($sp)
  .L0000B8F4:
    /* DDC360 0000B8F4 C4EA0974 */  lwc1       $f10, 0x974($a3)
    /* DDC364 0000B8F8 C4E80978 */  lwc1       $f8, 0x978($a3)
    /* DDC368 0000B8FC 8CF80A58 */  lw         $t8, 0xA58($a3)
    /* DDC36C 0000B900 3C01BF80 */  lui        $at, (0xBF800000 >> 16)
    /* DDC370 0000B904 46085100 */  add.s      $f4, $f10, $f8
    /* DDC374 0000B908 44816000 */  mtc1       $at, $f12
    /* DDC378 0000B90C 33190080 */  andi       $t9, $t8, 0x80
    /* DDC37C 0000B910 13200004 */  beqz       $t9, .L0000B924
    /* DDC380 0000B914 E4E40970 */   swc1      $f4, 0x970($a3)
    /* DDC384 0000B918 C4E60970 */  lwc1       $f6, 0x970($a3)
    /* DDC388 0000B91C 46003287 */  neg.s      $f10, $f6
    /* DDC38C 0000B920 E4EA0970 */  swc1       $f10, 0x970($a3)
  .L0000B924:
    /* DDC390 0000B924 C4E00970 */  lwc1       $f0, 0x970($a3)
    /* DDC394 0000B928 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DDC398 0000B92C 460C003C */  c.lt.s     $f0, $f12
    /* DDC39C 0000B930 00000000 */  nop
    /* DDC3A0 0000B934 45020004 */  bc1fl      .L0000B948
    /* DDC3A4 0000B938 44814000 */   mtc1      $at, $f8
    /* DDC3A8 0000B93C 1000000B */  b          .L0000B96C
    /* DDC3AC 0000B940 46006086 */   mov.s     $f2, $f12
    /* DDC3B0 0000B944 44814000 */  mtc1       $at, $f8
  .L0000B948:
    /* DDC3B4 0000B948 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DDC3B8 0000B94C 4600403C */  c.lt.s     $f8, $f0
    /* DDC3BC 0000B950 00000000 */  nop
    /* DDC3C0 0000B954 45020005 */  bc1fl      .L0000B96C
    /* DDC3C4 0000B958 46000086 */   mov.s     $f2, $f0
    /* DDC3C8 0000B95C 44811000 */  mtc1       $at, $f2
    /* DDC3CC 0000B960 10000003 */  b          .L0000B970
    /* DDC3D0 0000B964 8CE809A8 */   lw        $t0, 0x9A8($a3)
    /* DDC3D4 0000B968 46000086 */  mov.s      $f2, $f0
  .L0000B96C:
    /* DDC3D8 0000B96C 8CE809A8 */  lw         $t0, 0x9A8($a3)
  .L0000B970:
    /* DDC3DC 0000B970 E4E20970 */  swc1       $f2, 0x970($a3)
    /* DDC3E0 0000B974 3C0141A0 */  lui        $at, (0x41A00000 >> 16)
    /* DDC3E4 0000B978 31090001 */  andi       $t1, $t0, 0x1
    /* DDC3E8 0000B97C 55200005 */  bnel       $t1, $zero, .L0000B994
    /* DDC3EC 0000B980 C4E603C0 */   lwc1      $f6, 0x3C0($a3)
    /* DDC3F0 0000B984 8CEA0A50 */  lw         $t2, 0xA50($a3)
    /* DDC3F4 0000B988 5540000A */  bnel       $t2, $zero, .L0000B9B4
    /* DDC3F8 0000B98C C4E009D8 */   lwc1      $f0, 0x9D8($a3)
    /* DDC3FC 0000B990 C4E603C0 */  lwc1       $f6, 0x3C0($a3)
  .L0000B994:
    /* DDC400 0000B994 C4E403CC */  lwc1       $f4, 0x3CC($a3)
    /* DDC404 0000B998 C4E803B4 */  lwc1       $f8, 0x3B4($a3)
    /* DDC408 0000B99C 46003287 */  neg.s      $f10, $f6
    /* DDC40C 0000B9A0 E4E40A04 */  swc1       $f4, 0xA04($a3)
    /* DDC410 0000B9A4 E4EA0A08 */  swc1       $f10, 0xA08($a3)
    /* DDC414 0000B9A8 10000020 */  b          .L0000BA2C
    /* DDC418 0000B9AC E4E80A0C */   swc1      $f8, 0xA0C($a3)
    /* DDC41C 0000B9B0 C4E009D8 */  lwc1       $f0, 0x9D8($a3)
  .L0000B9B4:
    /* DDC420 0000B9B4 C4E403C8 */  lwc1       $f4, 0x3C8($a3)
    /* DDC424 0000B9B8 C4E209DC */  lwc1       $f2, 0x9DC($a3)
    /* DDC428 0000B9BC C4EA03CC */  lwc1       $f10, 0x3CC($a3)
    /* DDC42C 0000B9C0 46002182 */  mul.s      $f6, $f4, $f0
    /* DDC430 0000B9C4 C4EC09E0 */  lwc1       $f12, 0x9E0($a3)
    /* DDC434 0000B9C8 46025202 */  mul.s      $f8, $f10, $f2
    /* DDC438 0000B9CC C4EA03D0 */  lwc1       $f10, 0x3D0($a3)
    /* DDC43C 0000B9D0 46083100 */  add.s      $f4, $f6, $f8
    /* DDC440 0000B9D4 460C5182 */  mul.s      $f6, $f10, $f12
    /* DDC444 0000B9D8 C4EA03BC */  lwc1       $f10, 0x3BC($a3)
    /* DDC448 0000B9DC 46062200 */  add.s      $f8, $f4, $f6
    /* DDC44C 0000B9E0 46005102 */  mul.s      $f4, $f10, $f0
    /* DDC450 0000B9E4 C4E603C0 */  lwc1       $f6, 0x3C0($a3)
    /* DDC454 0000B9E8 E4E80A04 */  swc1       $f8, 0xA04($a3)
    /* DDC458 0000B9EC 46023202 */  mul.s      $f8, $f6, $f2
    /* DDC45C 0000B9F0 C4E603C4 */  lwc1       $f6, 0x3C4($a3)
    /* DDC460 0000B9F4 46082280 */  add.s      $f10, $f4, $f8
    /* DDC464 0000B9F8 460C3102 */  mul.s      $f4, $f6, $f12
    /* DDC468 0000B9FC 46045200 */  add.s      $f8, $f10, $f4
    /* DDC46C 0000BA00 C4EA03B0 */  lwc1       $f10, 0x3B0($a3)
    /* DDC470 0000BA04 46004187 */  neg.s      $f6, $f8
    /* DDC474 0000BA08 46005102 */  mul.s      $f4, $f10, $f0
    /* DDC478 0000BA0C C4E803B4 */  lwc1       $f8, 0x3B4($a3)
    /* DDC47C 0000BA10 E4E60A08 */  swc1       $f6, 0xA08($a3)
    /* DDC480 0000BA14 46024182 */  mul.s      $f6, $f8, $f2
    /* DDC484 0000BA18 C4E803B8 */  lwc1       $f8, 0x3B8($a3)
    /* DDC488 0000BA1C 46062280 */  add.s      $f10, $f4, $f6
    /* DDC48C 0000BA20 460C4102 */  mul.s      $f4, $f8, $f12
    /* DDC490 0000BA24 46045180 */  add.s      $f6, $f10, $f4
    /* DDC494 0000BA28 E4E60A0C */  swc1       $f6, 0xA0C($a3)
  .L0000BA2C:
    /* DDC498 0000BA2C 8CEB0938 */  lw         $t3, 0x938($a3)
    /* DDC49C 0000BA30 5560009E */  bnel       $t3, $zero, .L0000BCAC
    /* DDC4A0 0000BA34 44806000 */   mtc1      $zero, $f12
    /* DDC4A4 0000BA38 44807000 */  mtc1       $zero, $f14
    /* DDC4A8 0000BA3C 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DDC4AC 0000BA40 44814000 */  mtc1       $at, $f8
    /* DDC4B0 0000BA44 8CE20800 */  lw         $v0, 0x800($a3)
    /* DDC4B4 0000BA48 ACE00A5C */  sw         $zero, 0xA5C($a3)
    /* DDC4B8 0000BA4C E4EE02FC */  swc1       $f14, 0x2FC($a3)
    /* DDC4BC 0000BA50 E4EE0300 */  swc1       $f14, 0x300($a3)
    /* DDC4C0 0000BA54 E4EE0304 */  swc1       $f14, 0x304($a3)
    /* DDC4C4 0000BA58 E4E80308 */  swc1       $f8, 0x308($a3)
    /* DDC4C8 0000BA5C 8C4C0010 */  lw         $t4, 0x10($v0)
    /* DDC4CC 0000BA60 318D0200 */  andi       $t5, $t4, 0x200
    /* DDC4D0 0000BA64 51A00028 */  beql       $t5, $zero, .L0000BB08
    /* DDC4D4 0000BA68 8CE809A8 */   lw        $t0, 0x9A8($a3)
    /* DDC4D8 0000BA6C 8CEE09A8 */  lw         $t6, 0x9A8($a3)
    /* DDC4DC 0000BA70 31CF0001 */  andi       $t7, $t6, 0x1
    /* DDC4E0 0000BA74 51E00024 */  beql       $t7, $zero, .L0000BB08
    /* DDC4E4 0000BA78 8CE809A8 */   lw        $t0, 0x9A8($a3)
    /* DDC4E8 0000BA7C 84E20A6C */  lh         $v0, 0xA6C($a3)
    /* DDC4EC 0000BA80 24580001 */  addiu      $t8, $v0, 0x1
    /* DDC4F0 0000BA84 2841000B */  slti       $at, $v0, 0xB
    /* DDC4F4 0000BA88 1420006F */  bnez       $at, .L0000BC48
    /* DDC4F8 0000BA8C A4F80A6C */   sh        $t8, 0xA6C($a3)
    /* DDC4FC 0000BA90 8CF90800 */  lw         $t9, 0x800($a3)
    /* DDC500 0000BA94 3C010000 */  lui        $at, %hi(SndHandle_setScaleIdx31_000008D4 + 0xC)
    /* DDC504 0000BA98 C42C08E0 */  lwc1       $f12, %lo(SndHandle_setScaleIdx31_000008D4 + 0xC)($at)
    /* DDC508 0000BA9C C72A0000 */  lwc1       $f10, 0x0($t9)
    /* DDC50C 0000BAA0 3C010000 */  lui        $at, %hi(SndHandle_setScaleIdx31_000008D4 + 0x14)
    /* DDC510 0000BAA4 D42608E8 */  ldc1       $f6, %lo(SndHandle_setScaleIdx31_000008D4 + 0x14)($at)
    /* DDC514 0000BAA8 46005121 */  cvt.d.s    $f4, $f10
    /* DDC518 0000BAAC C4EA0A70 */  lwc1       $f10, 0xA70($a3)
    /* DDC51C 0000BAB0 46262202 */  mul.d      $f8, $f4, $f6
    /* DDC520 0000BAB4 3C010000 */  lui        $at, %hi(SndHandle_setScaleIdx31_000008D4 + 0x1C)
    /* DDC524 0000BAB8 46005121 */  cvt.d.s    $f4, $f10
    /* DDC528 0000BABC 46282180 */  add.d      $f6, $f4, $f8
    /* DDC52C 0000BAC0 46203020 */  cvt.s.d    $f0, $f6
    /* DDC530 0000BAC4 460C003C */  c.lt.s     $f0, $f12
    /* DDC534 0000BAC8 00000000 */  nop
    /* DDC538 0000BACC 45000003 */  bc1f       .L0000BADC
    /* DDC53C 0000BAD0 00000000 */   nop
    /* DDC540 0000BAD4 10000009 */  b          .L0000BAFC
    /* DDC544 0000BAD8 46006086 */   mov.s     $f2, $f12
  .L0000BADC:
    /* DDC548 0000BADC C42C08F0 */  lwc1       $f12, %lo(SndHandle_setScaleIdx31_000008D4 + 0x1C)($at)
    /* DDC54C 0000BAE0 4600603C */  c.lt.s     $f12, $f0
    /* DDC550 0000BAE4 00000000 */  nop
    /* DDC554 0000BAE8 45020004 */  bc1fl      .L0000BAFC
    /* DDC558 0000BAEC 46000086 */   mov.s     $f2, $f0
    /* DDC55C 0000BAF0 10000002 */  b          .L0000BAFC
    /* DDC560 0000BAF4 46006086 */   mov.s     $f2, $f12
    /* DDC564 0000BAF8 46000086 */  mov.s      $f2, $f0
  .L0000BAFC:
    /* DDC568 0000BAFC 10000052 */  b          .L0000BC48
    /* DDC56C 0000BB00 E4E20A70 */   swc1      $f2, 0xA70($a3)
    /* DDC570 0000BB04 8CE809A8 */  lw         $t0, 0x9A8($a3)
  .L0000BB08:
    /* DDC574 0000BB08 A4E00A6C */  sh         $zero, 0xA6C($a3)
    /* DDC578 0000BB0C C7A4006C */  lwc1       $f4, 0x6C($sp)
    /* DDC57C 0000BB10 31090001 */  andi       $t1, $t0, 0x1
    /* DDC580 0000BB14 5120000F */  beql       $t1, $zero, .L0000BB54
    /* DDC584 0000BB18 C4EA03BC */   lwc1      $f10, 0x3BC($a3)
    /* DDC588 0000BB1C 8CEA0800 */  lw         $t2, 0x800($a3)
    /* DDC58C 0000BB20 C4E80678 */  lwc1       $f8, 0x678($a3)
    /* DDC590 0000BB24 24E400CC */  addiu      $a0, $a3, 0xCC
    /* DDC594 0000BB28 C54A0000 */  lwc1       $f10, 0x0($t2)
    /* DDC598 0000BB2C AFA70078 */  sw         $a3, 0x78($sp)
    /* DDC59C 0000BB30 24E503B0 */  addiu      $a1, $a3, 0x3B0
    /* DDC5A0 0000BB34 46005107 */  neg.s      $f4, $f10
    /* DDC5A4 0000BB38 46082182 */  mul.s      $f6, $f4, $f8
    /* DDC5A8 0000BB3C 44063000 */  mfc1       $a2, $f6
    /* DDC5AC 0000BB40 0C000000 */  jal        func_00000000
    /* DDC5B0 0000BB44 00000000 */   nop
    /* DDC5B4 0000BB48 1000003D */  b          .L0000BC40
    /* DDC5B8 0000BB4C 8FA70078 */   lw        $a3, 0x78($sp)
    /* DDC5BC 0000BB50 C4EA03BC */  lwc1       $f10, 0x3BC($a3)
  .L0000BB54:
    /* DDC5C0 0000BB54 C4E603C0 */  lwc1       $f6, 0x3C0($a3)
    /* DDC5C4 0000BB58 3C010000 */  lui        $at, %hi(SndHandle_triggerEvent0_000008F4 + 0x4)
    /* DDC5C8 0000BB5C 46045202 */  mul.s      $f8, $f10, $f4
    /* DDC5CC 0000BB60 C7AA0070 */  lwc1       $f10, 0x70($sp)
    /* DDC5D0 0000BB64 24E400CC */  addiu      $a0, $a3, 0xCC
    /* DDC5D4 0000BB68 24E503C8 */  addiu      $a1, $a3, 0x3C8
    /* DDC5D8 0000BB6C 460A3102 */  mul.s      $f4, $f6, $f10
    /* DDC5DC 0000BB70 C4EA03C4 */  lwc1       $f10, 0x3C4($a3)
    /* DDC5E0 0000BB74 AFA70078 */  sw         $a3, 0x78($sp)
    /* DDC5E4 0000BB78 AFA50028 */  sw         $a1, 0x28($sp)
    /* DDC5E8 0000BB7C AFA40024 */  sw         $a0, 0x24($sp)
    /* DDC5EC 0000BB80 46044180 */  add.s      $f6, $f8, $f4
    /* DDC5F0 0000BB84 C7A80074 */  lwc1       $f8, 0x74($sp)
    /* DDC5F4 0000BB88 46085102 */  mul.s      $f4, $f10, $f8
    /* DDC5F8 0000BB8C D42808F8 */  ldc1       $f8, %lo(SndHandle_triggerEvent0_000008F4 + 0x4)($at)
    /* DDC5FC 0000BB90 46043000 */  add.s      $f0, $f6, $f4
    /* DDC600 0000BB94 460002A1 */  cvt.d.s    $f10, $f0
    /* DDC604 0000BB98 46285182 */  mul.d      $f6, $f10, $f8
    /* DDC608 0000BB9C 46203120 */  cvt.s.d    $f4, $f6
    /* DDC60C 0000BBA0 44062000 */  mfc1       $a2, $f4
    /* DDC610 0000BBA4 0C000000 */  jal        func_00000000
    /* DDC614 0000BBA8 00000000 */   nop
    /* DDC618 0000BBAC 8FA70078 */  lw         $a3, 0x78($sp)
    /* DDC61C 0000BBB0 8FA40024 */  lw         $a0, 0x24($sp)
    /* DDC620 0000BBB4 8FA50028 */  lw         $a1, 0x28($sp)
    /* DDC624 0000BBB8 8CEB0A18 */  lw         $t3, 0xA18($a3)
    /* DDC628 0000BBBC 51600021 */  beql       $t3, $zero, .L0000BC44
    /* DDC62C 0000BBC0 44807000 */   mtc1      $zero, $f14
    /* DDC630 0000BBC4 8CEC0800 */  lw         $t4, 0x800($a3)
    /* DDC634 0000BBC8 C4E80690 */  lwc1       $f8, 0x690($a3)
    /* DDC638 0000BBCC C58A0000 */  lwc1       $f10, 0x0($t4)
    /* DDC63C 0000BBD0 AFA70078 */  sw         $a3, 0x78($sp)
    /* DDC640 0000BBD4 AFA40024 */  sw         $a0, 0x24($sp)
    /* DDC644 0000BBD8 46085182 */  mul.s      $f6, $f10, $f8
    /* DDC648 0000BBDC 44063000 */  mfc1       $a2, $f6
    /* DDC64C 0000BBE0 0C000000 */  jal        func_00000000
    /* DDC650 0000BBE4 00000000 */   nop
    /* DDC654 0000BBE8 8FA70078 */  lw         $a3, 0x78($sp)
    /* DDC658 0000BBEC 8FA40024 */  lw         $a0, 0x24($sp)
    /* DDC65C 0000BBF0 8CED0800 */  lw         $t5, 0x800($a3)
    /* DDC660 0000BBF4 C4EA0660 */  lwc1       $f10, 0x660($a3)
    /* DDC664 0000BBF8 24E503BC */  addiu      $a1, $a3, 0x3BC
    /* DDC668 0000BBFC C5A40004 */  lwc1       $f4, 0x4($t5)
    /* DDC66C 0000BC00 460A2202 */  mul.s      $f8, $f4, $f10
    /* DDC670 0000BC04 44064000 */  mfc1       $a2, $f8
    /* DDC674 0000BC08 0C000000 */  jal        func_00000000
    /* DDC678 0000BC0C 00000000 */   nop
    /* DDC67C 0000BC10 8FA70078 */  lw         $a3, 0x78($sp)
    /* DDC680 0000BC14 8FA40024 */  lw         $a0, 0x24($sp)
    /* DDC684 0000BC18 8CEE0800 */  lw         $t6, 0x800($a3)
    /* DDC688 0000BC1C C4EA0678 */  lwc1       $f10, 0x678($a3)
    /* DDC68C 0000BC20 24E503B0 */  addiu      $a1, $a3, 0x3B0
    /* DDC690 0000BC24 C5C60000 */  lwc1       $f6, 0x0($t6)
    /* DDC694 0000BC28 46003107 */  neg.s      $f4, $f6
    /* DDC698 0000BC2C 460A2202 */  mul.s      $f8, $f4, $f10
    /* DDC69C 0000BC30 44064000 */  mfc1       $a2, $f8
    /* DDC6A0 0000BC34 0C000000 */  jal        func_00000000
    /* DDC6A4 0000BC38 00000000 */   nop
    /* DDC6A8 0000BC3C 8FA70078 */  lw         $a3, 0x78($sp)
  .L0000BC40:
    /* DDC6AC 0000BC40 44807000 */  mtc1       $zero, $f14
  .L0000BC44:
    /* DDC6B0 0000BC44 00000000 */  nop
  .L0000BC48:
    /* DDC6B4 0000BC48 8CE209A8 */  lw         $v0, 0x9A8($a3)
    /* DDC6B8 0000BC4C 30420001 */  andi       $v0, $v0, 0x1
    /* DDC6BC 0000BC50 104000AC */  beqz       $v0, .L0000BF04
    /* DDC6C0 0000BC54 00000000 */   nop
    /* DDC6C4 0000BC58 C4E603B4 */  lwc1       $f6, 0x3B4($a3)
    /* DDC6C8 0000BC5C 3C010000 */  lui        $at, %hi(SndHandle_triggerEvent0_000008F4 + 0xC)
    /* DDC6CC 0000BC60 D42A0900 */  ldc1       $f10, %lo(SndHandle_triggerEvent0_000008F4 + 0xC)($at)
    /* DDC6D0 0000BC64 46003121 */  cvt.d.s    $f4, $f6
    /* DDC6D4 0000BC68 24E503B0 */  addiu      $a1, $a3, 0x3B0
    /* DDC6D8 0000BC6C 462A203C */  c.lt.d     $f4, $f10
    /* DDC6DC 0000BC70 24E400CC */  addiu      $a0, $a3, 0xCC
    /* DDC6E0 0000BC74 45020003 */  bc1fl      .L0000BC84
    /* DDC6E4 0000BC78 C4E80A70 */   lwc1      $f8, 0xA70($a3)
    /* DDC6E8 0000BC7C E4EE0318 */  swc1       $f14, 0x318($a3)
    /* DDC6EC 0000BC80 C4E80A70 */  lwc1       $f8, 0xA70($a3)
  .L0000BC84:
    /* DDC6F0 0000BC84 AFA70078 */  sw         $a3, 0x78($sp)
    /* DDC6F4 0000BC88 46004187 */  neg.s      $f6, $f8
    /* DDC6F8 0000BC8C 44063000 */  mfc1       $a2, $f6
    /* DDC6FC 0000BC90 0C000000 */  jal        func_00000000
    /* DDC700 0000BC94 00000000 */   nop
    /* DDC704 0000BC98 8FA70078 */  lw         $a3, 0x78($sp)
    /* DDC708 0000BC9C 8CE209A8 */  lw         $v0, 0x9A8($a3)
    /* DDC70C 0000BCA0 10000098 */  b          .L0000BF04
    /* DDC710 0000BCA4 30420001 */   andi      $v0, $v0, 0x1
    /* DDC714 0000BCA8 44806000 */  mtc1       $zero, $f12
  .L0000BCAC:
    /* DDC718 0000BCAC C4E20970 */  lwc1       $f2, 0x970($a3)
    /* DDC71C 0000BCB0 44812000 */  mtc1       $at, $f4
    /* DDC720 0000BCB4 3C0141F0 */  lui        $at, (0x41F00000 >> 16)
    /* DDC724 0000BCB8 460C103C */  c.lt.s     $f2, $f12
    /* DDC728 0000BCBC 44815000 */  mtc1       $at, $f10
    /* DDC72C 0000BCC0 A4E00A6C */  sh         $zero, 0xA6C($a3)
    /* DDC730 0000BCC4 E4EC0A74 */  swc1       $f12, 0xA74($a3)
    /* DDC734 0000BCC8 45000003 */  bc1f       .L0000BCD8
    /* DDC738 0000BCCC E4EC0A70 */   swc1      $f12, 0xA70($a3)
    /* DDC73C 0000BCD0 10000002 */  b          .L0000BCDC
    /* DDC740 0000BCD4 46001007 */   neg.s     $f0, $f2
  .L0000BCD8:
    /* DDC744 0000BCD8 46001006 */  mov.s      $f0, $f2
  .L0000BCDC:
    /* DDC748 0000BCDC 46005202 */  mul.s      $f8, $f10, $f0
    /* DDC74C 0000BCE0 8CF909A8 */  lw         $t9, 0x9A8($a3)
    /* DDC750 0000BCE4 24E40808 */  addiu      $a0, $a3, 0x808
    /* DDC754 0000BCE8 33280001 */  andi       $t0, $t9, 0x1
    /* DDC758 0000BCEC 46082180 */  add.s      $f6, $f4, $f8
    /* DDC75C 0000BCF0 4600328D */  trunc.w.s  $f10, $f6
    /* DDC760 0000BCF4 44185000 */  mfc1       $t8, $f10
    /* DDC764 0000BCF8 11000028 */  beqz       $t0, .L0000BD9C
    /* DDC768 0000BCFC ACF80A5C */   sw        $t8, 0xA5C($a3)
    /* DDC76C 0000BD00 8CE500B4 */  lw         $a1, 0xB4($a3)
    /* DDC770 0000BD04 0C000000 */  jal        func_00000000
    /* DDC774 0000BD08 AFA70078 */   sw        $a3, 0x78($sp)
    /* DDC778 0000BD0C 8FA70078 */  lw         $a3, 0x78($sp)
    /* DDC77C 0000BD10 8CE90960 */  lw         $t1, 0x960($a3)
    /* DDC780 0000BD14 55200013 */  bnel       $t1, $zero, .L0000BD64
    /* DDC784 0000BD18 C4E803B4 */   lwc1      $f8, 0x3B4($a3)
    /* DDC788 0000BD1C C4E203C8 */  lwc1       $f2, 0x3C8($a3)
    /* DDC78C 0000BD20 44802000 */  mtc1       $zero, $f4
    /* DDC790 0000BD24 00000000 */  nop
    /* DDC794 0000BD28 4604103C */  c.lt.s     $f2, $f4
    /* DDC798 0000BD2C 00000000 */  nop
    /* DDC79C 0000BD30 45020004 */  bc1fl      .L0000BD44
    /* DDC7A0 0000BD34 46001006 */   mov.s     $f0, $f2
    /* DDC7A4 0000BD38 10000002 */  b          .L0000BD44
    /* DDC7A8 0000BD3C 46001007 */   neg.s     $f0, $f2
    /* DDC7AC 0000BD40 46001006 */  mov.s      $f0, $f2
  .L0000BD44:
    /* DDC7B0 0000BD44 C4E20318 */  lwc1       $f2, 0x318($a3)
    /* DDC7B4 0000BD48 C4E80558 */  lwc1       $f8, 0x558($a3)
    /* DDC7B8 0000BD4C 46081182 */  mul.s      $f6, $f2, $f8
    /* DDC7BC 0000BD50 00000000 */  nop
    /* DDC7C0 0000BD54 46003282 */  mul.s      $f10, $f6, $f0
    /* DDC7C4 0000BD58 460A1100 */  add.s      $f4, $f2, $f10
    /* DDC7C8 0000BD5C E4E40318 */  swc1       $f4, 0x318($a3)
    /* DDC7CC 0000BD60 C4E803B4 */  lwc1       $f8, 0x3B4($a3)
  .L0000BD64:
    /* DDC7D0 0000BD64 3C010000 */  lui        $at, %hi(SndHandle_triggerEvent0_000008F4 + 0x14)
    /* DDC7D4 0000BD68 D42A0908 */  ldc1       $f10, %lo(SndHandle_triggerEvent0_000008F4 + 0x14)($at)
    /* DDC7D8 0000BD6C 460041A1 */  cvt.d.s    $f6, $f8
    /* DDC7DC 0000BD70 3C014000 */  lui        $at, (0x40000000 >> 16)
    /* DDC7E0 0000BD74 462A303C */  c.lt.d     $f6, $f10
    /* DDC7E4 0000BD78 C7A40060 */  lwc1       $f4, 0x60($sp)
    /* DDC7E8 0000BD7C 45000005 */  bc1f       .L0000BD94
    /* DDC7EC 0000BD80 00000000 */   nop
    /* DDC7F0 0000BD84 44814000 */  mtc1       $at, $f8
    /* DDC7F4 0000BD88 00000000 */  nop
    /* DDC7F8 0000BD8C 46082182 */  mul.s      $f6, $f4, $f8
    /* DDC7FC 0000BD90 E7A60060 */  swc1       $f6, 0x60($sp)
  .L0000BD94:
    /* DDC800 0000BD94 10000054 */  b          .L0000BEE8
    /* DDC804 0000BD98 24E400CC */   addiu     $a0, $a3, 0xCC
  .L0000BD9C:
    /* DDC808 0000BD9C C7AA0058 */  lwc1       $f10, 0x58($sp)
    /* DDC80C 0000BDA0 C4E40648 */  lwc1       $f4, 0x648($a3)
    /* DDC810 0000BDA4 C7A80054 */  lwc1       $f8, 0x54($sp)
    /* DDC814 0000BDA8 4604503C */  c.lt.s     $f10, $f4
    /* DDC818 0000BDAC 00000000 */  nop
    /* DDC81C 0000BDB0 4502001F */  bc1fl      .L0000BE30
    /* DDC820 0000BDB4 C4E20A08 */   lwc1      $f2, 0xA08($a3)
    /* DDC824 0000BDB8 4608603C */  c.lt.s     $f12, $f8
    /* DDC828 0000BDBC C7A60060 */  lwc1       $f6, 0x60($sp)
    /* DDC82C 0000BDC0 C7AA0054 */  lwc1       $f10, 0x54($sp)
    /* DDC830 0000BDC4 45020006 */  bc1fl      .L0000BDE0
    /* DDC834 0000BDC8 460C503C */   c.lt.s    $f10, $f12
    /* DDC838 0000BDCC 460C303C */  c.lt.s     $f6, $f12
    /* DDC83C 0000BDD0 00000000 */  nop
    /* DDC840 0000BDD4 4503000A */  bc1tl      .L0000BE00
    /* DDC844 0000BDD8 8CEA0A5C */   lw        $t2, 0xA5C($a3)
    /* DDC848 0000BDDC 460C503C */  c.lt.s     $f10, $f12
  .L0000BDE0:
    /* DDC84C 0000BDE0 C7A40060 */  lwc1       $f4, 0x60($sp)
    /* DDC850 0000BDE4 45020012 */  bc1fl      .L0000BE30
    /* DDC854 0000BDE8 C4E20A08 */   lwc1      $f2, 0xA08($a3)
    /* DDC858 0000BDEC 4604603C */  c.lt.s     $f12, $f4
    /* DDC85C 0000BDF0 00000000 */  nop
    /* DDC860 0000BDF4 4502000E */  bc1fl      .L0000BE30
    /* DDC864 0000BDF8 C4E20A08 */   lwc1      $f2, 0xA08($a3)
    /* DDC868 0000BDFC 8CEA0A5C */  lw         $t2, 0xA5C($a3)
  .L0000BE00:
    /* DDC86C 0000BE00 8CE40800 */  lw         $a0, 0x800($a3)
    /* DDC870 0000BE04 3C050001 */  lui        $a1, %hi(FogTmpl_create + 0x4C)
    /* DDC874 0000BE08 254B0014 */  addiu      $t3, $t2, 0x14
    /* DDC878 0000BE0C ACEB0A5C */  sw         $t3, 0xA5C($a3)
    /* DDC87C 0000BE10 AFA70078 */  sw         $a3, 0x78($sp)
    /* DDC880 0000BE14 E7AC0060 */  swc1       $f12, 0x60($sp)
    /* DDC884 0000BE18 24A58170 */  addiu      $a1, $a1, %lo(FogTmpl_create + 0x4C)
    /* DDC888 0000BE1C 0C000000 */  jal        func_00000000
    /* DDC88C 0000BE20 24060001 */   addiu     $a2, $zero, 0x1
    /* DDC890 0000BE24 44806000 */  mtc1       $zero, $f12
    /* DDC894 0000BE28 8FA70078 */  lw         $a3, 0x78($sp)
    /* DDC898 0000BE2C C4E20A08 */  lwc1       $f2, 0xA08($a3)
  .L0000BE30:
    /* DDC89C 0000BE30 460C103C */  c.lt.s     $f2, $f12
    /* DDC8A0 0000BE34 00000000 */  nop
    /* DDC8A4 0000BE38 45020004 */  bc1fl      .L0000BE4C
    /* DDC8A8 0000BE3C 46001006 */   mov.s     $f0, $f2
    /* DDC8AC 0000BE40 10000002 */  b          .L0000BE4C
    /* DDC8B0 0000BE44 46001007 */   neg.s     $f0, $f2
    /* DDC8B4 0000BE48 46001006 */  mov.s      $f0, $f2
  .L0000BE4C:
    /* DDC8B8 0000BE4C C4E80618 */  lwc1       $f8, 0x618($a3)
    /* DDC8BC 0000BE50 4600403C */  c.lt.s     $f8, $f0
    /* DDC8C0 0000BE54 00000000 */  nop
    /* DDC8C4 0000BE58 4502000C */  bc1fl      .L0000BE8C
    /* DDC8C8 0000BE5C C4E20A04 */   lwc1      $f2, 0xA04($a3)
    /* DDC8CC 0000BE60 C4E60600 */  lwc1       $f6, 0x600($a3)
    /* DDC8D0 0000BE64 AFA70078 */  sw         $a3, 0x78($sp)
    /* DDC8D4 0000BE68 24E400CC */  addiu      $a0, $a3, 0xCC
    /* DDC8D8 0000BE6C 46061282 */  mul.s      $f10, $f2, $f6
    /* DDC8DC 0000BE70 24E503C8 */  addiu      $a1, $a3, 0x3C8
    /* DDC8E0 0000BE74 44065000 */  mfc1       $a2, $f10
    /* DDC8E4 0000BE78 0C000000 */  jal        func_00000000
    /* DDC8E8 0000BE7C 00000000 */   nop
    /* DDC8EC 0000BE80 44806000 */  mtc1       $zero, $f12
    /* DDC8F0 0000BE84 8FA70078 */  lw         $a3, 0x78($sp)
    /* DDC8F4 0000BE88 C4E20A04 */  lwc1       $f2, 0xA04($a3)
  .L0000BE8C:
    /* DDC8F8 0000BE8C 24E400CC */  addiu      $a0, $a3, 0xCC
    /* DDC8FC 0000BE90 460C103C */  c.lt.s     $f2, $f12
    /* DDC900 0000BE94 00000000 */  nop
    /* DDC904 0000BE98 45020004 */  bc1fl      .L0000BEAC
    /* DDC908 0000BE9C 46001006 */   mov.s     $f0, $f2
    /* DDC90C 0000BEA0 10000002 */  b          .L0000BEAC
    /* DDC910 0000BEA4 46001007 */   neg.s     $f0, $f2
    /* DDC914 0000BEA8 46001006 */  mov.s      $f0, $f2
  .L0000BEAC:
    /* DDC918 0000BEAC C4E40630 */  lwc1       $f4, 0x630($a3)
    /* DDC91C 0000BEB0 4600203C */  c.lt.s     $f4, $f0
    /* DDC920 0000BEB4 00000000 */  nop
    /* DDC924 0000BEB8 4502000C */  bc1fl      .L0000BEEC
    /* DDC928 0000BEBC 24E503B0 */   addiu     $a1, $a3, 0x3B0
    /* DDC92C 0000BEC0 C4E80600 */  lwc1       $f8, 0x600($a3)
    /* DDC930 0000BEC4 AFA70078 */  sw         $a3, 0x78($sp)
    /* DDC934 0000BEC8 AFA40024 */  sw         $a0, 0x24($sp)
    /* DDC938 0000BECC 46081182 */  mul.s      $f6, $f2, $f8
    /* DDC93C 0000BED0 24E503BC */  addiu      $a1, $a3, 0x3BC
    /* DDC940 0000BED4 44063000 */  mfc1       $a2, $f6
    /* DDC944 0000BED8 0C000000 */  jal        func_00000000
    /* DDC948 0000BEDC 00000000 */   nop
    /* DDC94C 0000BEE0 8FA40024 */  lw         $a0, 0x24($sp)
    /* DDC950 0000BEE4 8FA70078 */  lw         $a3, 0x78($sp)
  .L0000BEE8:
    /* DDC954 0000BEE8 24E503B0 */  addiu      $a1, $a3, 0x3B0
  .L0000BEEC:
    /* DDC958 0000BEEC 8FA60060 */  lw         $a2, 0x60($sp)
    /* DDC95C 0000BEF0 0C000000 */  jal        func_00000000
    /* DDC960 0000BEF4 AFA70078 */   sw        $a3, 0x78($sp)
    /* DDC964 0000BEF8 8FA70078 */  lw         $a3, 0x78($sp)
    /* DDC968 0000BEFC 8CE209A8 */  lw         $v0, 0x9A8($a3)
    /* DDC96C 0000BF00 30420001 */  andi       $v0, $v0, 0x1
  .L0000BF04:
    /* DDC970 0000BF04 10400003 */  beqz       $v0, .L0000BF14
    /* DDC974 0000BF08 3C010000 */   lui       $at, %hi(SndHandle_triggerEvent0_000008F4 + 0x1C)
    /* DDC978 0000BF0C 10000002 */  b          .L0000BF18
    /* DDC97C 0000BF10 C4200910 */   lwc1      $f0, %lo(SndHandle_triggerEvent0_000008F4 + 0x1C)($at)
  .L0000BF14:
    /* DDC980 0000BF14 C4E007F8 */  lwc1       $f0, 0x7F8($a3)
  .L0000BF18:
    /* DDC984 0000BF18 C4EA02FC */  lwc1       $f10, 0x2FC($a3)
    /* DDC988 0000BF1C C4E80300 */  lwc1       $f8, 0x300($a3)
    /* DDC98C 0000BF20 3C013FF0 */  lui        $at, (0x3FF00000 >> 16)
    /* DDC990 0000BF24 46005102 */  mul.s      $f4, $f10, $f0
    /* DDC994 0000BF28 C4EA0304 */  lwc1       $f10, 0x304($a3)
    /* DDC998 0000BF2C 44814800 */  mtc1       $at, $f9
    /* DDC99C 0000BF30 46004182 */  mul.s      $f6, $f8, $f0
    /* DDC9A0 0000BF34 44804000 */  mtc1       $zero, $f8
    /* DDC9A4 0000BF38 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DDC9A8 0000BF3C C4E20308 */  lwc1       $f2, 0x308($a3)
    /* DDC9AC 0000BF40 E4E402FC */  swc1       $f4, 0x2FC($a3)
    /* DDC9B0 0000BF44 46005102 */  mul.s      $f4, $f10, $f0
    /* DDC9B4 0000BF48 E4E60300 */  swc1       $f6, 0x300($a3)
    /* DDC9B8 0000BF4C 460001A1 */  cvt.d.s    $f6, $f0
    /* DDC9BC 0000BF50 46264281 */  sub.d      $f10, $f8, $f6
    /* DDC9C0 0000BF54 E4E40304 */  swc1       $f4, 0x304($a3)
    /* DDC9C4 0000BF58 44812000 */  mtc1       $at, $f4
    /* DDC9C8 0000BF5C 00000000 */  nop
    /* DDC9CC 0000BF60 46022201 */  sub.s      $f8, $f4, $f2
    /* DDC9D0 0000BF64 460041A1 */  cvt.d.s    $f6, $f8
    /* DDC9D4 0000BF68 46001221 */  cvt.d.s    $f8, $f2
    /* DDC9D8 0000BF6C 462A3102 */  mul.d      $f4, $f6, $f10
    /* DDC9DC 0000BF70 46244180 */  add.d      $f6, $f8, $f4
    /* DDC9E0 0000BF74 462032A0 */  cvt.s.d    $f10, $f6
    /* DDC9E4 0000BF78 E4EA0308 */  swc1       $f10, 0x308($a3)
    /* DDC9E8 0000BF7C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDC9EC 0000BF80 27BD0078 */  addiu      $sp, $sp, 0x78
    /* DDC9F0 0000BF84 03E00008 */  jr         $ra
    /* DDC9F4 0000BF88 00000000 */   nop
endlabel SnowBoarder_func_0000B75C
