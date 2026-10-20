
nonmatching func_800079F4, 0x148

glabel func_800079F4
    /* 89F4 800079F4 27BDFF88 */  addiu      $sp, $sp, -0x78
    /* 89F8 800079F8 AFA40078 */  sw         $a0, 0x78($sp)
    /* 89FC 800079FC 8FAE0078 */  lw         $t6, 0x78($sp)
    /* 8A00 80007A00 AFBF001C */  sw         $ra, 0x1C($sp)
    /* 8A04 80007A04 AFB00018 */  sw         $s0, 0x18($sp)
    /* 8A08 80007A08 AFAE0074 */  sw         $t6, 0x74($sp)
    /* 8A0C 80007A0C 91D00009 */  lbu        $s0, 0x9($t6)
    /* 8A10 80007A10 12000005 */  beqz       $s0, .L80007A28
    /* 8A14 80007A14 24010001 */   addiu     $at, $zero, 0x1
    /* 8A18 80007A18 12010008 */  beq        $s0, $at, .L80007A3C
    /* 8A1C 80007A1C 00000000 */   nop
    /* 8A20 80007A20 10000022 */  b          .L80007AAC
    /* 8A24 80007A24 00000000 */   nop
  .L80007A28:
    /* 8A28 80007A28 8FAF0074 */  lw         $t7, 0x74($sp)
    /* 8A2C 80007A2C 0C001E17 */  jal        func_8000785C
    /* 8A30 80007A30 8DE4000C */   lw        $a0, 0xC($t7)
    /* 8A34 80007A34 1000001F */  b          .L80007AB4
    /* 8A38 80007A38 00000000 */   nop
  .L80007A3C:
    /* 8A3C 80007A3C 0C00210C */  jal        func_80008430
    /* 8A40 80007A40 00000000 */   nop
    /* 8A44 80007A44 1040001B */  beqz       $v0, .L80007AB4
    /* 8A48 80007A48 00000000 */   nop
    /* 8A4C 80007A4C 0C002115 */  jal        func_80008454
    /* 8A50 80007A50 00000000 */   nop
    /* 8A54 80007A54 0C001AA6 */  jal        func_80006A98
    /* 8A58 80007A58 3C040408 */   lui       $a0, (0x4080000 >> 16)
    /* 8A5C 80007A5C AFA20024 */  sw         $v0, 0x24($sp)
    /* 8A60 80007A60 8FB80024 */  lw         $t8, 0x24($sp)
    /* 8A64 80007A64 13000013 */  beqz       $t8, .L80007AB4
    /* 8A68 80007A68 00000000 */   nop
    /* 8A6C 80007A6C 8FB90024 */  lw         $t9, 0x24($sp)
    /* 8A70 80007A70 3C010400 */  lui        $at, (0x4001000 >> 16)
    /* 8A74 80007A74 34211000 */  ori        $at, $at, (0x4001000 & 0xFFFF)
    /* 8A78 80007A78 2728FFFC */  addiu      $t0, $t9, -0x4
    /* 8A7C 80007A7C AFA80024 */  sw         $t0, 0x24($sp)
    /* 8A80 80007A80 01013021 */  addu       $a2, $t0, $at
    /* 8A84 80007A84 24040001 */  addiu      $a0, $zero, 0x1
    /* 8A88 80007A88 0C002263 */  jal        func_8000898C
    /* 8A8C 80007A8C 240503E8 */   addiu     $a1, $zero, 0x3E8
    /* 8A90 80007A90 30490003 */  andi       $t1, $v0, 0x3
    /* 8A94 80007A94 15200007 */  bnez       $t1, .L80007AB4
    /* 8A98 80007A98 00000000 */  nop
    /* 8A9C 80007A9C 0C002126 */  jal        func_80008498
    /* 8AA0 80007AA0 00000000 */   nop
    /* 8AA4 80007AA4 10000003 */  b          .L80007AB4
    /* 8AA8 80007AA8 00000000 */   nop
  .L80007AAC:
    /* 8AAC 80007AAC 1000001E */  b          .L80007B28
    /* 8AB0 80007AB0 2402FFFC */   addiu     $v0, $zero, -0x4
  .L80007AB4:
    /* 8AB4 80007AB4 8FAA0078 */  lw         $t2, 0x78($sp)
    /* 8AB8 80007AB8 8FAB0074 */  lw         $t3, 0x74($sp)
    /* 8ABC 80007ABC 27A60028 */  addiu      $a2, $sp, 0x28
    /* 8AC0 80007AC0 91440009 */  lbu        $a0, 0x9($t2)
    /* 8AC4 80007AC4 0C001EA6 */  jal        func_80007A98
    /* 8AC8 80007AC8 8D65000C */   lw        $a1, 0xC($t3)
    /* 8ACC 80007ACC 10400003 */  beqz       $v0, .L80007ADC
    /* 8AD0 80007AD0 00000000 */   nop
    /* 8AD4 80007AD4 10000014 */  b          .L80007B28
    /* 8AD8 80007AD8 2402FFFE */   addiu     $v0, $zero, -0x2
  .L80007ADC:
    /* 8ADC 80007ADC 8FAC0074 */  lw         $t4, 0x74($sp)
    /* 8AE0 80007AE0 27A40028 */  addiu      $a0, $sp, 0x28
    /* 8AE4 80007AE4 2405004C */  addiu      $a1, $zero, 0x4C
    /* 8AE8 80007AE8 918D0004 */  lbu        $t5, 0x4($t4)
    /* 8AEC 80007AEC A7A0002E */  sh         $zero, 0x2E($sp)
    /* 8AF0 80007AF0 24060001 */  addiu      $a2, $zero, 0x1
    /* 8AF4 80007AF4 0C001CFE */  jal        func_800073F8
    /* 8AF8 80007AF8 A3AD002C */   sb        $t5, 0x2C($sp)
    /* 8AFC 80007AFC 8FAE0034 */  lw         $t6, 0x34($sp)
    /* 8B00 80007B00 24010001 */  addiu      $at, $zero, 0x1
    /* 8B04 80007B04 15C10007 */  bne        $t6, $at, .L80007B24
    /* 8B08 80007B08 00000000 */   nop
    /* 8B0C 80007B0C 240F0004 */  addiu      $t7, $zero, 0x4
    /* 8B10 80007B10 A3AF002C */  sb         $t7, 0x2C($sp)
    /* 8B14 80007B14 27A40028 */  addiu      $a0, $sp, 0x28
    /* 8B18 80007B18 2405004C */  addiu      $a1, $zero, 0x4C
    /* 8B1C 80007B1C 0C001CFE */  jal        func_800073F8
    /* 8B20 80007B20 24060002 */   addiu     $a2, $zero, 0x2
  .L80007B24:
    /* 8B24 80007B24 00001025 */  or         $v0, $zero, $zero
  .L80007B28:
    /* 8B28 80007B28 8FBF001C */  lw         $ra, 0x1C($sp)
    /* 8B2C 80007B2C 8FB00018 */  lw         $s0, 0x18($sp)
    /* 8B30 80007B30 27BD0078 */  addiu      $sp, $sp, 0x78
    /* 8B34 80007B34 03E00008 */  jr         $ra
    /* 8B38 80007B38 00000000 */   nop
endlabel func_800079F4
