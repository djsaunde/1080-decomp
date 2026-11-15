nonmatching Fader_start, 0x170

glabel Fader_start
    /* DE441C 000139B0 27BDFFB8 */  addiu      $sp, $sp, -0x48
    /* DE4420 000139B4 AFBF0044 */  sw         $ra, 0x44($sp)
    /* DE4424 000139B8 AFB40040 */  sw         $s4, 0x40($sp)
    /* DE4428 000139BC AFB3003C */  sw         $s3, 0x3C($sp)
    /* DE442C 000139C0 AFB20038 */  sw         $s2, 0x38($sp)
    /* DE4430 000139C4 AFB10034 */  sw         $s1, 0x34($sp)
    /* DE4434 000139C8 AFB00030 */  sw         $s0, 0x30($sp)
    /* DE4438 000139CC F7B80028 */  sdc1       $f24, 0x28($sp)
    /* DE443C 000139D0 F7B60020 */  sdc1       $f22, 0x20($sp)
    /* DE4440 000139D4 F7B40018 */  sdc1       $f20, 0x18($sp)
    /* DE4444 000139D8 00808825 */  or         $s1, $a0, $zero
    /* DE4448 000139DC 14C00006 */  bnez       $a2, .L000139F8
    /* DE444C 000139E0 AC860068 */   sw        $a2, 0x68($a0)
    /* DE4450 000139E4 240E0001 */  addiu      $t6, $zero, 0x1
    /* DE4454 000139E8 240F0003 */  addiu      $t7, $zero, 0x3
    /* DE4458 000139EC AC8E006C */  sw         $t6, 0x6C($a0)
    /* DE445C 000139F0 10000002 */  b          .L000139FC
    /* DE4460 000139F4 AC8F0070 */   sw        $t7, 0x70($a0)
  .L000139F8:
    /* DE4464 000139F8 AE20006C */  sw         $zero, 0x6C($s1)
  .L000139FC:
    /* DE4468 000139FC 8E380040 */  lw         $t8, 0x40($s1)
    /* DE446C 00013A00 24190004 */  addiu      $t9, $zero, 0x4
    /* DE4470 00013A04 00002025 */  or         $a0, $zero, $zero
    /* DE4474 00013A08 10B8003A */  beq        $a1, $t8, .L00013AF4
    /* DE4478 00013A0C 00009025 */   or        $s2, $zero, $zero
    /* DE447C 00013A10 8E220060 */  lw         $v0, 0x60($s1)
    /* DE4480 00013A14 AE250040 */  sw         $a1, 0x40($s1)
    /* DE4484 00013A18 AE390044 */  sw         $t9, 0x44($s1)
    /* DE4488 00013A1C 18400015 */  blez       $v0, .L00013A74
    /* DE448C 00013A20 AE200074 */   sw        $zero, 0x74($s1)
    /* DE4490 00013A24 4480A000 */  mtc1       $zero, $f20
    /* DE4494 00013A28 8E23005C */  lw         $v1, 0x5C($s1)
  .L00013A2C:
    /* DE4498 00013A2C 1860000D */  blez       $v1, .L00013A64
    /* DE449C 00013A30 00008025 */   or        $s0, $zero, $zero
    /* DE44A0 00013A34 00041080 */  sll        $v0, $a0, 2
    /* DE44A4 00013A38 8E280058 */  lw         $t0, 0x58($s1)
  .L00013A3C:
    /* DE44A8 00013A3C 26100001 */  addiu      $s0, $s0, 0x1
    /* DE44AC 00013A40 24840001 */  addiu      $a0, $a0, 0x1
    /* DE44B0 00013A44 01024821 */  addu       $t1, $t0, $v0
    /* DE44B4 00013A48 E5340000 */  swc1       $f20, 0x0($t1)
    /* DE44B8 00013A4C 8E23005C */  lw         $v1, 0x5C($s1)
    /* DE44BC 00013A50 24420004 */  addiu      $v0, $v0, 0x4
    /* DE44C0 00013A54 0203082A */  slt        $at, $s0, $v1
    /* DE44C4 00013A58 5420FFF8 */  bnel       $at, $zero, .L00013A3C
    /* DE44C8 00013A5C 8E280058 */   lw        $t0, 0x58($s1)
    /* DE44CC 00013A60 8E220060 */  lw         $v0, 0x60($s1)
  .L00013A64:
    /* DE44D0 00013A64 26520001 */  addiu      $s2, $s2, 0x1
    /* DE44D4 00013A68 0242082A */  slt        $at, $s2, $v0
    /* DE44D8 00013A6C 1420FFEF */  bnez       $at, .L00013A2C
    /* DE44DC 00013A70 00000000 */   nop
  .L00013A74:
    /* DE44E0 00013A74 3C010000 */  lui        $at, %hi(D_00000C64)
    /* DE44E4 00013A78 C4380C64 */  lwc1       $f24, %lo(D_00000C64)($at)
    /* DE44E8 00013A7C 3C014200 */  lui        $at, (0x42000000 >> 16)
    /* DE44EC 00013A80 4480A000 */  mtc1       $zero, $f20
    /* DE44F0 00013A84 4481B000 */  mtc1       $at, $f22
    /* DE44F4 00013A88 24140005 */  addiu      $s4, $zero, 0x5
    /* DE44F8 00013A8C 00009825 */  or         $s3, $zero, $zero
  .L00013A90:
    /* DE44FC 00013A90 0C000000 */  jal        func_00000000
    /* DE4500 00013A94 00000000 */   nop
    /* DE4504 00013A98 46160102 */  mul.s      $f4, $f0, $f22
    /* DE4508 00013A9C 4600218D */  trunc.w.s  $f6, $f4
    /* DE450C 00013AA0 44103000 */  mfc1       $s0, $f6
    /* DE4510 00013AA4 0C000000 */  jal        func_00000000
    /* DE4514 00013AA8 00000000 */   nop
    /* DE4518 00013AAC 46160202 */  mul.s      $f8, $f0, $f22
    /* DE451C 00013AB0 4600428D */  trunc.w.s  $f10, $f8
    /* DE4520 00013AB4 44125000 */  mfc1       $s2, $f10
    /* DE4524 00013AB8 0C000000 */  jal        func_00000000
    /* DE4528 00013ABC 00000000 */   nop
    /* DE452C 00013AC0 46180402 */  mul.s      $f16, $f0, $f24
    /* DE4530 00013AC4 8E2C0058 */  lw         $t4, 0x58($s1)
    /* DE4534 00013AC8 00106880 */  sll        $t5, $s0, 2
    /* DE4538 00013ACC 0012C1C0 */  sll        $t8, $s2, 7
    /* DE453C 00013AD0 26730001 */  addiu      $s3, $s3, 0x1
    /* DE4540 00013AD4 018D7021 */  addu       $t6, $t4, $t5
    /* DE4544 00013AD8 01D8C821 */  addu       $t9, $t6, $t8
    /* DE4548 00013ADC 0274082A */  slt        $at, $s3, $s4
    /* DE454C 00013AE0 1420FFEB */  bnez       $at, .L00013A90
    /* DE4550 00013AE4 E7300000 */   swc1      $f16, 0x0($t9)
    /* DE4554 00013AE8 0C000000 */  jal        func_00000000
    /* DE4558 00013AEC 02202025 */   or        $a0, $s1, $zero
    /* DE455C 00013AF0 E6340048 */  swc1       $f20, 0x48($s1)
  .L00013AF4:
    /* DE4560 00013AF4 8FBF0044 */  lw         $ra, 0x44($sp)
    /* DE4564 00013AF8 D7B40018 */  ldc1       $f20, 0x18($sp)
    /* DE4568 00013AFC D7B60020 */  ldc1       $f22, 0x20($sp)
    /* DE456C 00013B00 D7B80028 */  ldc1       $f24, 0x28($sp)
    /* DE4570 00013B04 8FB00030 */  lw         $s0, 0x30($sp)
    /* DE4574 00013B08 8FB10034 */  lw         $s1, 0x34($sp)
    /* DE4578 00013B0C 8FB20038 */  lw         $s2, 0x38($sp)
    /* DE457C 00013B10 8FB3003C */  lw         $s3, 0x3C($sp)
    /* DE4580 00013B14 8FB40040 */  lw         $s4, 0x40($sp)
    /* DE4584 00013B18 03E00008 */  jr         $ra
    /* DE4588 00013B1C 27BD0048 */   addiu     $sp, $sp, 0x48
endlabel Fader_start
