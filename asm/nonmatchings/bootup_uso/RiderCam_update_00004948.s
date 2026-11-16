nonmatching RiderCam_update_00004948, 0x6A4

glabel RiderCam_update_00004948
    /* DD53B4 00004948 27BDFEE0 */  addiu      $sp, $sp, -0x120
    /* DD53B8 0000494C AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD53BC 00004950 8C820158 */  lw         $v0, 0x158($a0)
    /* DD53C0 00004954 24010002 */  addiu      $at, $zero, 0x2
    /* DD53C4 00004958 00803025 */  or         $a2, $a0, $zero
    /* DD53C8 0000495C 5441002A */  bnel       $v0, $at, .L00004A08
    /* DD53CC 00004960 2C410013 */   sltiu     $at, $v0, 0x13
    /* DD53D0 00004964 8C820154 */  lw         $v0, 0x154($a0)
    /* DD53D4 00004968 3C010000 */  lui        $at, (0x0 >> 16)
    /* DD53D8 0000496C C4200000 */  lwc1       $f0, 0x0($at)
    /* DD53DC 00004970 C4440318 */  lwc1       $f4, 0x318($v0)
    /* DD53E0 00004974 C446031C */  lwc1       $f6, 0x31C($v0)
    /* DD53E4 00004978 C4480320 */  lwc1       $f8, 0x320($v0)
    /* DD53E8 0000497C 46002382 */  mul.s      $f14, $f4, $f0
    /* DD53EC 00004980 27AE00F4 */  addiu      $t6, $sp, 0xF4
    /* DD53F0 00004984 24420318 */  addiu      $v0, $v0, 0x318
    /* DD53F4 00004988 46003082 */  mul.s      $f2, $f6, $f0
    /* DD53F8 0000498C 27A20104 */  addiu      $v0, $sp, 0x104
    /* DD53FC 00004990 27A50114 */  addiu      $a1, $sp, 0x114
    /* DD5400 00004994 46004302 */  mul.s      $f12, $f8, $f0
    /* DD5404 00004998 E7AE00F4 */  swc1       $f14, 0xF4($sp)
    /* DD5408 0000499C 3C010000 */  lui        $at, (0x0 >> 16)
    /* DD540C 000049A0 24840030 */  addiu      $a0, $a0, 0x30
    /* DD5410 000049A4 E7A200F8 */  swc1       $f2, 0xF8($sp)
    /* DD5414 000049A8 E7AC00FC */  swc1       $f12, 0xFC($sp)
    /* DD5418 000049AC 8DD80000 */  lw         $t8, 0x0($t6)
    /* DD541C 000049B0 AC580000 */  sw         $t8, 0x0($v0)
    /* DD5420 000049B4 8DCF0004 */  lw         $t7, 0x4($t6)
    /* DD5424 000049B8 8C480000 */  lw         $t0, 0x0($v0)
    /* DD5428 000049BC AC4F0004 */  sw         $t7, 0x4($v0)
    /* DD542C 000049C0 8DD80008 */  lw         $t8, 0x8($t6)
    /* DD5430 000049C4 ACAF0004 */  sw         $t7, 0x4($a1)
    /* DD5434 000049C8 ACA80000 */  sw         $t0, 0x0($a1)
    /* DD5438 000049CC AC580008 */  sw         $t8, 0x8($v0)
    /* DD543C 000049D0 ACB80008 */  sw         $t8, 0x8($a1)
    /* DD5440 000049D4 C7AA0118 */  lwc1       $f10, 0x118($sp)
    /* DD5444 000049D8 C4320000 */  lwc1       $f18, 0x0($at)
    /* DD5448 000049DC AFA60120 */  sw         $a2, 0x120($sp)
    /* DD544C 000049E0 AFA40028 */  sw         $a0, 0x28($sp)
    /* DD5450 000049E4 46125102 */  mul.s      $f4, $f10, $f18
    /* DD5454 000049E8 0C000000 */  jal        func_00000000
    /* DD5458 000049EC E7A40118 */   swc1      $f4, 0x118($sp)
    /* DD545C 000049F0 8FA60120 */  lw         $a2, 0x120($sp)
    /* DD5460 000049F4 8FA40028 */  lw         $a0, 0x28($sp)
    /* DD5464 000049F8 C4C60150 */  lwc1       $f6, 0x150($a2)
    /* DD5468 000049FC 10000173 */  b          .L00004FCC
    /* DD546C 00004A00 E4C6014C */   swc1      $f6, 0x14C($a2)
    /* DD5470 00004A04 2C410013 */  sltiu      $at, $v0, 0x13
  .L00004A08:
    /* DD5474 00004A08 14200002 */  bnez       $at, .L00004A14
    /* DD5478 00004A0C 2C410018 */   sltiu     $at, $v0, 0x18
    /* DD547C 00004A10 14200005 */  bnez       $at, .L00004A28
  .L00004A14:
    /* DD5480 00004A14 2C410018 */   sltiu     $at, $v0, 0x18
    /* DD5484 00004A18 1420009F */  bnez       $at, .L00004C98
    /* DD5488 00004A1C 2C41001D */   sltiu     $at, $v0, 0x1D
    /* DD548C 00004A20 5020009E */  beql       $at, $zero, .L00004C9C
    /* DD5490 00004A24 2C41001D */   sltiu     $at, $v0, 0x1D
  .L00004A28:
    /* DD5494 00004A28 8CC200F4 */  lw         $v0, 0xF4($a2)
    /* DD5498 00004A2C 27A900A8 */  addiu      $t1, $sp, 0xA8
    /* DD549C 00004A30 27AC00D4 */  addiu      $t4, $sp, 0xD4
    /* DD54A0 00004A34 C44800A0 */  lwc1       $f8, 0xA0($v0)
    /* DD54A4 00004A38 24420070 */  addiu      $v0, $v0, 0x70
    /* DD54A8 00004A3C E7A800C8 */  swc1       $f8, 0xC8($sp)
    /* DD54AC 00004A40 C44A0034 */  lwc1       $f10, 0x34($v0)
    /* DD54B0 00004A44 E7AA00CC */  swc1       $f10, 0xCC($sp)
    /* DD54B4 00004A48 C4520038 */  lwc1       $f18, 0x38($v0)
    /* DD54B8 00004A4C C7AA00C8 */  lwc1       $f10, 0xC8($sp)
    /* DD54BC 00004A50 27A20104 */  addiu      $v0, $sp, 0x104
    /* DD54C0 00004A54 E7B200D0 */  swc1       $f18, 0xD0($sp)
    /* DD54C4 00004A58 C4C400A0 */  lwc1       $f4, 0xA0($a2)
    /* DD54C8 00004A5C E7A400BC */  swc1       $f4, 0xBC($sp)
    /* DD54CC 00004A60 C4C600A4 */  lwc1       $f6, 0xA4($a2)
    /* DD54D0 00004A64 C7B200BC */  lwc1       $f18, 0xBC($sp)
    /* DD54D4 00004A68 C7A400CC */  lwc1       $f4, 0xCC($sp)
    /* DD54D8 00004A6C E7A600C0 */  swc1       $f6, 0xC0($sp)
    /* DD54DC 00004A70 C4C800A8 */  lwc1       $f8, 0xA8($a2)
    /* DD54E0 00004A74 46125381 */  sub.s      $f14, $f10, $f18
    /* DD54E4 00004A78 C7A600C0 */  lwc1       $f6, 0xC0($sp)
    /* DD54E8 00004A7C E7A800C4 */  swc1       $f8, 0xC4($sp)
    /* DD54EC 00004A80 C7AA00C4 */  lwc1       $f10, 0xC4($sp)
    /* DD54F0 00004A84 C7A800D0 */  lwc1       $f8, 0xD0($sp)
    /* DD54F4 00004A88 46062081 */  sub.s      $f2, $f4, $f6
    /* DD54F8 00004A8C E7AE00A8 */  swc1       $f14, 0xA8($sp)
    /* DD54FC 00004A90 460A4301 */  sub.s      $f12, $f8, $f10
    /* DD5500 00004A94 E7A200AC */  swc1       $f2, 0xAC($sp)
    /* DD5504 00004A98 E7AC00B0 */  swc1       $f12, 0xB0($sp)
    /* DD5508 00004A9C 8D2B0000 */  lw         $t3, 0x0($t1)
    /* DD550C 00004AA0 AC4B0000 */  sw         $t3, 0x0($v0)
    /* DD5510 00004AA4 8D2A0004 */  lw         $t2, 0x4($t1)
    /* DD5514 00004AA8 8C4E0000 */  lw         $t6, 0x0($v0)
    /* DD5518 00004AAC AC4A0004 */  sw         $t2, 0x4($v0)
    /* DD551C 00004AB0 8D2B0008 */  lw         $t3, 0x8($t1)
    /* DD5520 00004AB4 AC4B0008 */  sw         $t3, 0x8($v0)
    /* DD5524 00004AB8 AD8E0000 */  sw         $t6, 0x0($t4)
    /* DD5528 00004ABC 8C4D0004 */  lw         $t5, 0x4($v0)
    /* DD552C 00004AC0 AD8D0004 */  sw         $t5, 0x4($t4)
    /* DD5530 00004AC4 8C4E0008 */  lw         $t6, 0x8($v0)
    /* DD5534 00004AC8 AD8E0008 */  sw         $t6, 0x8($t4)
    /* DD5538 00004ACC C7B200D4 */  lwc1       $f18, 0xD4($sp)
    /* DD553C 00004AD0 C7A600D8 */  lwc1       $f6, 0xD8($sp)
    /* DD5540 00004AD4 AFA60120 */  sw         $a2, 0x120($sp)
    /* DD5544 00004AD8 46129102 */  mul.s      $f4, $f18, $f18
    /* DD5548 00004ADC C7B200DC */  lwc1       $f18, 0xDC($sp)
    /* DD554C 00004AE0 46063202 */  mul.s      $f8, $f6, $f6
    /* DD5550 00004AE4 46082280 */  add.s      $f10, $f4, $f8
    /* DD5554 00004AE8 46129182 */  mul.s      $f6, $f18, $f18
    /* DD5558 00004AEC 0C000000 */  jal        func_00000000
    /* DD555C 00004AF0 46065300 */   add.s     $f12, $f10, $f6
    /* DD5560 00004AF4 8FA60120 */  lw         $a2, 0x120($sp)
    /* DD5564 00004AF8 24010013 */  addiu      $at, $zero, 0x13
    /* DD5568 00004AFC 3C180000 */  lui        $t8, (0x0 >> 16)
    /* DD556C 00004B00 8CC20158 */  lw         $v0, 0x158($a2)
    /* DD5570 00004B04 00001825 */  or         $v1, $zero, $zero
    /* DD5574 00004B08 27180000 */  addiu      $t8, $t8, 0x0
    /* DD5578 00004B0C 1041001C */  beq        $v0, $at, .L00004B80
    /* DD557C 00004B10 24010018 */   addiu     $at, $zero, 0x18
    /* DD5580 00004B14 1041001A */  beq        $v0, $at, .L00004B80
    /* DD5584 00004B18 24010014 */   addiu     $at, $zero, 0x14
    /* DD5588 00004B1C 10410011 */  beq        $v0, $at, .L00004B64
    /* DD558C 00004B20 24010019 */   addiu     $at, $zero, 0x19
    /* DD5590 00004B24 1041000F */  beq        $v0, $at, .L00004B64
    /* DD5594 00004B28 24010015 */   addiu     $at, $zero, 0x15
    /* DD5598 00004B2C 1041000F */  beq        $v0, $at, .L00004B6C
    /* DD559C 00004B30 2401001A */   addiu     $at, $zero, 0x1A
    /* DD55A0 00004B34 1041000D */  beq        $v0, $at, .L00004B6C
    /* DD55A4 00004B38 24010016 */   addiu     $at, $zero, 0x16
    /* DD55A8 00004B3C 1041000D */  beq        $v0, $at, .L00004B74
    /* DD55AC 00004B40 2401001B */   addiu     $at, $zero, 0x1B
    /* DD55B0 00004B44 1041000B */  beq        $v0, $at, .L00004B74
    /* DD55B4 00004B48 24010017 */   addiu     $at, $zero, 0x17
    /* DD55B8 00004B4C 1041000B */  beq        $v0, $at, .L00004B7C
    /* DD55BC 00004B50 2401001C */   addiu     $at, $zero, 0x1C
    /* DD55C0 00004B54 5041000A */  beql       $v0, $at, .L00004B80
    /* DD55C4 00004B58 24030004 */   addiu     $v1, $zero, 0x4
    /* DD55C8 00004B5C 10000009 */  b          .L00004B84
    /* DD55CC 00004B60 00037900 */   sll       $t7, $v1, 4
  .L00004B64:
    /* DD55D0 00004B64 10000006 */  b          .L00004B80
    /* DD55D4 00004B68 24030001 */   addiu     $v1, $zero, 0x1
  .L00004B6C:
    /* DD55D8 00004B6C 10000004 */  b          .L00004B80
    /* DD55DC 00004B70 24030002 */   addiu     $v1, $zero, 0x2
  .L00004B74:
    /* DD55E0 00004B74 10000002 */  b          .L00004B80
    /* DD55E4 00004B78 24030003 */   addiu     $v1, $zero, 0x3
  .L00004B7C:
    /* DD55E8 00004B7C 24030004 */  addiu      $v1, $zero, 0x4
  .L00004B80:
    /* DD55EC 00004B80 00037900 */  sll        $t7, $v1, 4
  .L00004B84:
    /* DD55F0 00004B84 01F81021 */  addu       $v0, $t7, $t8
    /* DD55F4 00004B88 C44C0000 */  lwc1       $f12, 0x0($v0)
    /* DD55F8 00004B8C 4600603C */  c.lt.s     $f12, $f0
    /* DD55FC 00004B90 00000000 */  nop
    /* DD5600 00004B94 45020005 */  bc1fl      .L00004BAC
    /* DD5604 00004B98 C44E0004 */   lwc1      $f14, 0x4($v0)
    /* DD5608 00004B9C C444000C */  lwc1       $f4, 0xC($v0)
    /* DD560C 00004BA0 10000012 */  b          .L00004BEC
    /* DD5610 00004BA4 E4C4014C */   swc1      $f4, 0x14C($a2)
    /* DD5614 00004BA8 C44E0004 */  lwc1       $f14, 0x4($v0)
  .L00004BAC:
    /* DD5618 00004BAC 460E003C */  c.lt.s     $f0, $f14
    /* DD561C 00004BB0 00000000 */  nop
    /* DD5620 00004BB4 45020005 */  bc1fl      .L00004BCC
    /* DD5624 00004BB8 C442000C */   lwc1      $f2, 0xC($v0)
    /* DD5628 00004BBC C4480008 */  lwc1       $f8, 0x8($v0)
    /* DD562C 00004BC0 1000000A */  b          .L00004BEC
    /* DD5630 00004BC4 E4C8014C */   swc1      $f8, 0x14C($a2)
    /* DD5634 00004BC8 C442000C */  lwc1       $f2, 0xC($v0)
  .L00004BCC:
    /* DD5638 00004BCC C4520008 */  lwc1       $f18, 0x8($v0)
    /* DD563C 00004BD0 460E6181 */  sub.s      $f6, $f12, $f14
    /* DD5640 00004BD4 46029281 */  sub.s      $f10, $f18, $f2
    /* DD5644 00004BD8 46006201 */  sub.s      $f8, $f12, $f0
    /* DD5648 00004BDC 46065103 */  div.s      $f4, $f10, $f6
    /* DD564C 00004BE0 46082482 */  mul.s      $f18, $f4, $f8
    /* DD5650 00004BE4 46121280 */  add.s      $f10, $f2, $f18
    /* DD5654 00004BE8 E4CA014C */  swc1       $f10, 0x14C($a2)
  .L00004BEC:
    /* DD5658 00004BEC 8CC20158 */  lw         $v0, 0x158($a2)
    /* DD565C 00004BF0 2C410018 */  sltiu      $at, $v0, 0x18
    /* DD5660 00004BF4 14200026 */  bnez       $at, .L00004C90
    /* DD5664 00004BF8 2C41001D */   sltiu     $at, $v0, 0x1D
    /* DD5668 00004BFC 10200024 */  beqz       $at, .L00004C90
    /* DD566C 00004C00 00000000 */   nop
    /* DD5670 00004C04 8CC20154 */  lw         $v0, 0x154($a2)
    /* DD5674 00004C08 3C010000 */  lui        $at, (0x0 >> 16)
    /* DD5678 00004C0C C4200000 */  lwc1       $f0, 0x0($at)
    /* DD567C 00004C10 C4460318 */  lwc1       $f6, 0x318($v0)
    /* DD5680 00004C14 C444031C */  lwc1       $f4, 0x31C($v0)
    /* DD5684 00004C18 C4480320 */  lwc1       $f8, 0x320($v0)
    /* DD5688 00004C1C 46003382 */  mul.s      $f14, $f6, $f0
    /* DD568C 00004C20 27B9008C */  addiu      $t9, $sp, 0x8C
    /* DD5690 00004C24 24420318 */  addiu      $v0, $v0, 0x318
    /* DD5694 00004C28 46002082 */  mul.s      $f2, $f4, $f0
    /* DD5698 00004C2C 27A20104 */  addiu      $v0, $sp, 0x104
    /* DD569C 00004C30 27A50098 */  addiu      $a1, $sp, 0x98
    /* DD56A0 00004C34 46004302 */  mul.s      $f12, $f8, $f0
    /* DD56A4 00004C38 E7AE008C */  swc1       $f14, 0x8C($sp)
    /* DD56A8 00004C3C 3C010000 */  lui        $at, (0x0 >> 16)
    /* DD56AC 00004C40 24C40030 */  addiu      $a0, $a2, 0x30
    /* DD56B0 00004C44 E7A20090 */  swc1       $f2, 0x90($sp)
    /* DD56B4 00004C48 E7AC0094 */  swc1       $f12, 0x94($sp)
    /* DD56B8 00004C4C 8F290000 */  lw         $t1, 0x0($t9)
    /* DD56BC 00004C50 AC490000 */  sw         $t1, 0x0($v0)
    /* DD56C0 00004C54 8F280004 */  lw         $t0, 0x4($t9)
    /* DD56C4 00004C58 8C4B0000 */  lw         $t3, 0x0($v0)
    /* DD56C8 00004C5C AC480004 */  sw         $t0, 0x4($v0)
    /* DD56CC 00004C60 8F290008 */  lw         $t1, 0x8($t9)
    /* DD56D0 00004C64 ACA80004 */  sw         $t0, 0x4($a1)
    /* DD56D4 00004C68 ACAB0000 */  sw         $t3, 0x0($a1)
    /* DD56D8 00004C6C AC490008 */  sw         $t1, 0x8($v0)
    /* DD56DC 00004C70 ACA90008 */  sw         $t1, 0x8($a1)
    /* DD56E0 00004C74 C7B2009C */  lwc1       $f18, 0x9C($sp)
    /* DD56E4 00004C78 C42A0000 */  lwc1       $f10, 0x0($at)
    /* DD56E8 00004C7C AFA60120 */  sw         $a2, 0x120($sp)
    /* DD56EC 00004C80 460A9182 */  mul.s      $f6, $f18, $f10
    /* DD56F0 00004C84 0C000000 */  jal        func_00000000
    /* DD56F4 00004C88 E7A6009C */   swc1      $f6, 0x9C($sp)
    /* DD56F8 00004C8C 8FA60120 */  lw         $a2, 0x120($sp)
  .L00004C90:
    /* DD56FC 00004C90 100000CE */  b          .L00004FCC
    /* DD5700 00004C94 24C40030 */   addiu     $a0, $a2, 0x30
  .L00004C98:
    /* DD5704 00004C98 2C41001D */  sltiu      $at, $v0, 0x1D
  .L00004C9C:
    /* DD5708 00004C9C 14200002 */  bnez       $at, .L00004CA8
    /* DD570C 00004CA0 2C410022 */   sltiu     $at, $v0, 0x22
    /* DD5710 00004CA4 14200005 */  bnez       $at, .L00004CBC
  .L00004CA8:
    /* DD5714 00004CA8 2C410022 */   sltiu     $at, $v0, 0x22
    /* DD5718 00004CAC 142000C4 */  bnez       $at, .L00004FC0
    /* DD571C 00004CB0 2C410027 */   sltiu     $at, $v0, 0x27
    /* DD5720 00004CB4 502000C3 */  beql       $at, $zero, .L00004FC4
    /* DD5724 00004CB8 C4D20150 */   lwc1      $f18, 0x150($a2)
  .L00004CBC:
    /* DD5728 00004CBC 8CC200F4 */  lw         $v0, 0xF4($a2)
    /* DD572C 00004CC0 27AC0048 */  addiu      $t4, $sp, 0x48
    /* DD5730 00004CC4 27AF0080 */  addiu      $t7, $sp, 0x80
    /* DD5734 00004CC8 C44400A0 */  lwc1       $f4, 0xA0($v0)
    /* DD5738 00004CCC 24420070 */  addiu      $v0, $v0, 0x70
    /* DD573C 00004CD0 E7A40074 */  swc1       $f4, 0x74($sp)
    /* DD5740 00004CD4 C4480034 */  lwc1       $f8, 0x34($v0)
    /* DD5744 00004CD8 E7A80078 */  swc1       $f8, 0x78($sp)
    /* DD5748 00004CDC C4520038 */  lwc1       $f18, 0x38($v0)
    /* DD574C 00004CE0 C7A80074 */  lwc1       $f8, 0x74($sp)
    /* DD5750 00004CE4 27A20104 */  addiu      $v0, $sp, 0x104
    /* DD5754 00004CE8 E7B2007C */  swc1       $f18, 0x7C($sp)
    /* DD5758 00004CEC C4CA00A0 */  lwc1       $f10, 0xA0($a2)
    /* DD575C 00004CF0 E7AA0068 */  swc1       $f10, 0x68($sp)
    /* DD5760 00004CF4 C4C600A4 */  lwc1       $f6, 0xA4($a2)
    /* DD5764 00004CF8 C7B20068 */  lwc1       $f18, 0x68($sp)
    /* DD5768 00004CFC C7AA0078 */  lwc1       $f10, 0x78($sp)
    /* DD576C 00004D00 E7A6006C */  swc1       $f6, 0x6C($sp)
    /* DD5770 00004D04 C4C400A8 */  lwc1       $f4, 0xA8($a2)
    /* DD5774 00004D08 46124381 */  sub.s      $f14, $f8, $f18
    /* DD5778 00004D0C C7A6006C */  lwc1       $f6, 0x6C($sp)
    /* DD577C 00004D10 E7A40070 */  swc1       $f4, 0x70($sp)
    /* DD5780 00004D14 C7A80070 */  lwc1       $f8, 0x70($sp)
    /* DD5784 00004D18 C7A4007C */  lwc1       $f4, 0x7C($sp)
    /* DD5788 00004D1C 46065081 */  sub.s      $f2, $f10, $f6
    /* DD578C 00004D20 E7AE0048 */  swc1       $f14, 0x48($sp)
    /* DD5790 00004D24 46082301 */  sub.s      $f12, $f4, $f8
    /* DD5794 00004D28 E7A2004C */  swc1       $f2, 0x4C($sp)
    /* DD5798 00004D2C E7AC0050 */  swc1       $f12, 0x50($sp)
    /* DD579C 00004D30 8D8E0000 */  lw         $t6, 0x0($t4)
    /* DD57A0 00004D34 AC4E0000 */  sw         $t6, 0x0($v0)
    /* DD57A4 00004D38 8D8D0004 */  lw         $t5, 0x4($t4)
    /* DD57A8 00004D3C 8C590000 */  lw         $t9, 0x0($v0)
    /* DD57AC 00004D40 AC4D0004 */  sw         $t5, 0x4($v0)
    /* DD57B0 00004D44 8D8E0008 */  lw         $t6, 0x8($t4)
    /* DD57B4 00004D48 AC4E0008 */  sw         $t6, 0x8($v0)
    /* DD57B8 00004D4C ADF90000 */  sw         $t9, 0x0($t7)
    /* DD57BC 00004D50 8C580004 */  lw         $t8, 0x4($v0)
    /* DD57C0 00004D54 ADF80004 */  sw         $t8, 0x4($t7)
    /* DD57C4 00004D58 8C590008 */  lw         $t9, 0x8($v0)
    /* DD57C8 00004D5C ADF90008 */  sw         $t9, 0x8($t7)
    /* DD57CC 00004D60 C7B20080 */  lwc1       $f18, 0x80($sp)
    /* DD57D0 00004D64 C7A60084 */  lwc1       $f6, 0x84($sp)
    /* DD57D4 00004D68 AFA60120 */  sw         $a2, 0x120($sp)
    /* DD57D8 00004D6C 46129282 */  mul.s      $f10, $f18, $f18
    /* DD57DC 00004D70 C7B20088 */  lwc1       $f18, 0x88($sp)
    /* DD57E0 00004D74 46063102 */  mul.s      $f4, $f6, $f6
    /* DD57E4 00004D78 46045200 */  add.s      $f8, $f10, $f4
    /* DD57E8 00004D7C 46129182 */  mul.s      $f6, $f18, $f18
    /* DD57EC 00004D80 0C000000 */  jal        func_00000000
    /* DD57F0 00004D84 46064300 */   add.s     $f12, $f8, $f6
    /* DD57F4 00004D88 8FA60120 */  lw         $a2, 0x120($sp)
    /* DD57F8 00004D8C E7A00064 */  swc1       $f0, 0x64($sp)
    /* DD57FC 00004D90 2401001D */  addiu      $at, $zero, 0x1D
    /* DD5800 00004D94 8CC20158 */  lw         $v0, 0x158($a2)
    /* DD5804 00004D98 3C090000 */  lui        $t1, (0x0 >> 16)
    /* DD5808 00004D9C 00001825 */  or         $v1, $zero, $zero
    /* DD580C 00004DA0 1041001D */  beq        $v0, $at, .L00004E18
    /* DD5810 00004DA4 25290000 */   addiu     $t1, $t1, 0x0
    /* DD5814 00004DA8 24010022 */  addiu      $at, $zero, 0x22
    /* DD5818 00004DAC 1041001A */  beq        $v0, $at, .L00004E18
    /* DD581C 00004DB0 2401001E */   addiu     $at, $zero, 0x1E
    /* DD5820 00004DB4 10410011 */  beq        $v0, $at, .L00004DFC
    /* DD5824 00004DB8 24010023 */   addiu     $at, $zero, 0x23
    /* DD5828 00004DBC 1041000F */  beq        $v0, $at, .L00004DFC
    /* DD582C 00004DC0 2401001F */   addiu     $at, $zero, 0x1F
    /* DD5830 00004DC4 1041000F */  beq        $v0, $at, .L00004E04
    /* DD5834 00004DC8 24010024 */   addiu     $at, $zero, 0x24
    /* DD5838 00004DCC 1041000D */  beq        $v0, $at, .L00004E04
    /* DD583C 00004DD0 24010020 */   addiu     $at, $zero, 0x20
    /* DD5840 00004DD4 1041000D */  beq        $v0, $at, .L00004E0C
    /* DD5844 00004DD8 24010025 */   addiu     $at, $zero, 0x25
    /* DD5848 00004DDC 1041000B */  beq        $v0, $at, .L00004E0C
    /* DD584C 00004DE0 24010021 */   addiu     $at, $zero, 0x21
    /* DD5850 00004DE4 1041000B */  beq        $v0, $at, .L00004E14
    /* DD5854 00004DE8 24010026 */   addiu     $at, $zero, 0x26
    /* DD5858 00004DEC 5041000A */  beql       $v0, $at, .L00004E18
    /* DD585C 00004DF0 24030004 */   addiu     $v1, $zero, 0x4
    /* DD5860 00004DF4 10000009 */  b          .L00004E1C
    /* DD5864 00004DF8 000340C0 */   sll       $t0, $v1, 3
  .L00004DFC:
    /* DD5868 00004DFC 10000006 */  b          .L00004E18
    /* DD586C 00004E00 24030001 */   addiu     $v1, $zero, 0x1
  .L00004E04:
    /* DD5870 00004E04 10000004 */  b          .L00004E18
    /* DD5874 00004E08 24030002 */   addiu     $v1, $zero, 0x2
  .L00004E0C:
    /* DD5878 00004E0C 10000002 */  b          .L00004E18
    /* DD587C 00004E10 24030003 */   addiu     $v1, $zero, 0x3
  .L00004E14:
    /* DD5880 00004E14 24030004 */  addiu      $v1, $zero, 0x4
  .L00004E18:
    /* DD5884 00004E18 000340C0 */  sll        $t0, $v1, 3
  .L00004E1C:
    /* DD5888 00004E1C 01091021 */  addu       $v0, $t0, $t1
    /* DD588C 00004E20 3C014000 */  lui        $at, (0x40000000 >> 16)
    /* DD5890 00004E24 44812000 */  mtc1       $at, $f4
    /* DD5894 00004E28 C44A0004 */  lwc1       $f10, 0x4($v0)
    /* DD5898 00004E2C 3C010000 */  lui        $at, %hi(snd_calcEmitterDistPan_000003F8 + 0x15C)
    /* DD589C 00004E30 C4280554 */  lwc1       $f8, %lo(snd_calcEmitterDistPan_000003F8 + 0x15C)($at)
    /* DD58A0 00004E34 46045483 */  div.s      $f18, $f10, $f4
    /* DD58A4 00004E38 3C014334 */  lui        $at, (0x43340000 >> 16)
    /* DD58A8 00004E3C 44815000 */  mtc1       $at, $f10
    /* DD58AC 00004E40 AFA20028 */  sw         $v0, 0x28($sp)
    /* DD58B0 00004E44 AFA60120 */  sw         $a2, 0x120($sp)
    /* DD58B4 00004E48 46089182 */  mul.s      $f6, $f18, $f8
    /* DD58B8 00004E4C 460A3303 */  div.s      $f12, $f6, $f10
    /* DD58BC 00004E50 0C000000 */  jal        func_00000000
    /* DD58C0 00004E54 E7AC005C */   swc1      $f12, 0x5C($sp)
    /* DD58C4 00004E58 C7AC005C */  lwc1       $f12, 0x5C($sp)
    /* DD58C8 00004E5C 0C000000 */  jal        func_00000000
    /* DD58CC 00004E60 E7A000E8 */   swc1      $f0, 0xE8($sp)
    /* DD58D0 00004E64 8FAA0028 */  lw         $t2, 0x28($sp)
    /* DD58D4 00004E68 C7AE00E8 */  lwc1       $f14, 0xE8($sp)
    /* DD58D8 00004E6C C7B00064 */  lwc1       $f16, 0x64($sp)
    /* DD58DC 00004E70 C5440000 */  lwc1       $f4, 0x0($t2)
    /* DD58E0 00004E74 46040482 */  mul.s      $f18, $f0, $f4
    /* DD58E4 00004E78 460E9083 */  div.s      $f2, $f18, $f14
    /* DD58E8 00004E7C 46108202 */  mul.s      $f8, $f16, $f16
    /* DD58EC 00004E80 00000000 */  nop
    /* DD58F0 00004E84 46021182 */  mul.s      $f6, $f2, $f2
    /* DD58F4 00004E88 0C000000 */  jal        func_00000000
    /* DD58F8 00004E8C 46064300 */   add.s     $f12, $f8, $f6
    /* DD58FC 00004E90 C7AA0064 */  lwc1       $f10, 0x64($sp)
    /* DD5900 00004E94 0C000000 */  jal        func_00000000
    /* DD5904 00004E98 46005303 */   div.s     $f12, $f10, $f0
    /* DD5908 00004E9C 3C014000 */  lui        $at, (0x40000000 >> 16)
    /* DD590C 00004EA0 44812000 */  mtc1       $at, $f4
    /* DD5910 00004EA4 3C010000 */  lui        $at, %hi(snd_calcEmitterDistPan_000003F8 + 0x160)
    /* DD5914 00004EA8 C4280558 */  lwc1       $f8, %lo(snd_calcEmitterDistPan_000003F8 + 0x160)($at)
    /* DD5918 00004EAC 46040482 */  mul.s      $f18, $f0, $f4
    /* DD591C 00004EB0 3C014334 */  lui        $at, (0x43340000 >> 16)
    /* DD5920 00004EB4 44815000 */  mtc1       $at, $f10
    /* DD5924 00004EB8 3C010000 */  lui        $at, (0x0 >> 16)
    /* DD5928 00004EBC C42C0000 */  lwc1       $f12, 0x0($at)
    /* DD592C 00004EC0 8FA60120 */  lw         $a2, 0x120($sp)
    /* DD5930 00004EC4 3C010000 */  lui        $at, (0x0 >> 16)
    /* DD5934 00004EC8 46089183 */  div.s      $f6, $f18, $f8
    /* DD5938 00004ECC 460A3082 */  mul.s      $f2, $f6, $f10
    /* DD593C 00004ED0 460C103C */  c.lt.s     $f2, $f12
    /* DD5940 00004ED4 00000000 */  nop
    /* DD5944 00004ED8 45000003 */  bc1f       .L00004EE8
    /* DD5948 00004EDC 00000000 */   nop
    /* DD594C 00004EE0 10000009 */  b          .L00004F08
    /* DD5950 00004EE4 E4CC014C */   swc1      $f12, 0x14C($a2)
  .L00004EE8:
    /* DD5954 00004EE8 C4200000 */  lwc1       $f0, 0x0($at)
    /* DD5958 00004EEC 4602003C */  c.lt.s     $f0, $f2
    /* DD595C 00004EF0 00000000 */  nop
    /* DD5960 00004EF4 45020004 */  bc1fl      .L00004F08
    /* DD5964 00004EF8 E4C2014C */   swc1      $f2, 0x14C($a2)
    /* DD5968 00004EFC 10000002 */  b          .L00004F08
    /* DD596C 00004F00 E4C0014C */   swc1      $f0, 0x14C($a2)
    /* DD5970 00004F04 E4C2014C */  swc1       $f2, 0x14C($a2)
  .L00004F08:
    /* DD5974 00004F08 8CC20158 */  lw         $v0, 0x158($a2)
    /* DD5978 00004F0C 2C410022 */  sltiu      $at, $v0, 0x22
    /* DD597C 00004F10 14200029 */  bnez       $at, .L00004FB8
    /* DD5980 00004F14 2C410027 */   sltiu     $at, $v0, 0x27
    /* DD5984 00004F18 10200027 */  beqz       $at, .L00004FB8
    /* DD5988 00004F1C 00000000 */   nop
    /* DD598C 00004F20 8CC20154 */  lw         $v0, 0x154($a2)
    /* DD5990 00004F24 3C010000 */  lui        $at, (0x0 >> 16)
    /* DD5994 00004F28 C4200000 */  lwc1       $f0, 0x0($at)
    /* DD5998 00004F2C C4440318 */  lwc1       $f4, 0x318($v0)
    /* DD599C 00004F30 C452031C */  lwc1       $f18, 0x31C($v0)
    /* DD59A0 00004F34 C4480320 */  lwc1       $f8, 0x320($v0)
    /* DD59A4 00004F38 46002382 */  mul.s      $f14, $f4, $f0
    /* DD59A8 00004F3C 27AC002C */  addiu      $t4, $sp, 0x2C
    /* DD59AC 00004F40 27AB0104 */  addiu      $t3, $sp, 0x104
    /* DD59B0 00004F44 46009082 */  mul.s      $f2, $f18, $f0
    /* DD59B4 00004F48 27AF0104 */  addiu      $t7, $sp, 0x104
    /* DD59B8 00004F4C 27A50038 */  addiu      $a1, $sp, 0x38
    /* DD59BC 00004F50 46004302 */  mul.s      $f12, $f8, $f0
    /* DD59C0 00004F54 E7AE002C */  swc1       $f14, 0x2C($sp)
    /* DD59C4 00004F58 3C010000 */  lui        $at, (0x0 >> 16)
    /* DD59C8 00004F5C 24C40030 */  addiu      $a0, $a2, 0x30
    /* DD59CC 00004F60 E7A20030 */  swc1       $f2, 0x30($sp)
    /* DD59D0 00004F64 24420318 */  addiu      $v0, $v0, 0x318
    /* DD59D4 00004F68 E7AC0034 */  swc1       $f12, 0x34($sp)
    /* DD59D8 00004F6C 8D8E0000 */  lw         $t6, 0x0($t4)
    /* DD59DC 00004F70 AD6E0000 */  sw         $t6, 0x0($t3)
    /* DD59E0 00004F74 8D8D0004 */  lw         $t5, 0x4($t4)
    /* DD59E4 00004F78 AD6D0004 */  sw         $t5, 0x4($t3)
    /* DD59E8 00004F7C 8D8E0008 */  lw         $t6, 0x8($t4)
    /* DD59EC 00004F80 AD6E0008 */  sw         $t6, 0x8($t3)
    /* DD59F0 00004F84 8DF90000 */  lw         $t9, 0x0($t7)
    /* DD59F4 00004F88 ACB90000 */  sw         $t9, 0x0($a1)
    /* DD59F8 00004F8C 8DF80004 */  lw         $t8, 0x4($t7)
    /* DD59FC 00004F90 ACB80004 */  sw         $t8, 0x4($a1)
    /* DD5A00 00004F94 8DF90008 */  lw         $t9, 0x8($t7)
    /* DD5A04 00004F98 ACB90008 */  sw         $t9, 0x8($a1)
    /* DD5A08 00004F9C C7A6003C */  lwc1       $f6, 0x3C($sp)
    /* DD5A0C 00004FA0 C42A0000 */  lwc1       $f10, 0x0($at)
    /* DD5A10 00004FA4 AFA60120 */  sw         $a2, 0x120($sp)
    /* DD5A14 00004FA8 460A3102 */  mul.s      $f4, $f6, $f10
    /* DD5A18 00004FAC 0C000000 */  jal        func_00000000
    /* DD5A1C 00004FB0 E7A4003C */   swc1      $f4, 0x3C($sp)
    /* DD5A20 00004FB4 8FA60120 */  lw         $a2, 0x120($sp)
  .L00004FB8:
    /* DD5A24 00004FB8 10000004 */  b          .L00004FCC
    /* DD5A28 00004FBC 24C40030 */   addiu     $a0, $a2, 0x30
  .L00004FC0:
    /* DD5A2C 00004FC0 C4D20150 */  lwc1       $f18, 0x150($a2)
  .L00004FC4:
    /* DD5A30 00004FC4 24C40030 */  addiu      $a0, $a2, 0x30
    /* DD5A34 00004FC8 E4D2014C */  swc1       $f18, 0x14C($a2)
  .L00004FCC:
    /* DD5A38 00004FCC 0C000000 */  jal        func_00000000
    /* DD5A3C 00004FD0 AFA60120 */   sw        $a2, 0x120($sp)
    /* DD5A40 00004FD4 0C000000 */  jal        func_00000000
    /* DD5A44 00004FD8 8FA40120 */   lw        $a0, 0x120($sp)
    /* DD5A48 00004FDC 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD5A4C 00004FE0 27BD0120 */  addiu      $sp, $sp, 0x120
    /* DD5A50 00004FE4 03E00008 */  jr         $ra
    /* DD5A54 00004FE8 00000000 */   nop
endlabel RiderCam_update_00004948
    /* DD5A58 00004FEC 00000000 */  nop
