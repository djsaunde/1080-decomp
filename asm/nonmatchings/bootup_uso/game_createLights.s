nonmatching game_createLights, 0x228

glabel game_createLights
    /* DD39FC 00002F90 27BDFFA0 */  addiu      $sp, $sp, -0x60
    /* DD3A00 00002F94 AFB10020 */  sw         $s1, 0x20($sp)
    /* DD3A04 00002F98 00808825 */  or         $s1, $a0, $zero
    /* DD3A08 00002F9C AFBF0024 */  sw         $ra, 0x24($sp)
    /* DD3A0C 00002FA0 3C040000 */  lui        $a0, %hi(D_0000741C)
    /* DD3A10 00002FA4 AFB0001C */  sw         $s0, 0x1C($sp)
    /* DD3A14 00002FA8 2484741C */  addiu      $a0, $a0, %lo(D_0000741C)
    /* DD3A18 00002FAC 0C000000 */  jal        func_00000000
    /* DD3A1C 00002FB0 00002825 */   or        $a1, $zero, $zero
    /* DD3A20 00002FB4 3C050000 */  lui        $a1, %hi(D_00007424)
    /* DD3A24 00002FB8 24A57424 */  addiu      $a1, $a1, %lo(D_00007424)
    /* DD3A28 00002FBC 0C000000 */  jal        func_00000000
    /* DD3A2C 00002FC0 00002025 */   or        $a0, $zero, $zero
    /* DD3A30 00002FC4 3C010000 */  lui        $at, %hi(func_000003F8 + 0x12C)
    /* DD3A34 00002FC8 C4200524 */  lwc1       $f0, %lo(func_000003F8 + 0x12C)($at)
    /* DD3A38 00002FCC AE220090 */  sw         $v0, 0x90($s1)
    /* DD3A3C 00002FD0 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DD3A40 00002FD4 44812000 */  mtc1       $at, $f4
    /* DD3A44 00002FD8 E440012C */  swc1       $f0, 0x12C($v0)
    /* DD3A48 00002FDC E4400128 */  swc1       $f0, 0x128($v0)
    /* DD3A4C 00002FE0 E4400124 */  swc1       $f0, 0x124($v0)
    /* DD3A50 00002FE4 E4440130 */  swc1       $f4, 0x130($v0)
    /* DD3A54 00002FE8 8E260090 */  lw         $a2, 0x90($s1)
    /* DD3A58 00002FEC 240E0001 */  addiu      $t6, $zero, 0x1
    /* DD3A5C 00002FF0 3C050000 */  lui        $a1, %hi(D_00007434)
    /* DD3A60 00002FF4 ACCE00D0 */  sw         $t6, 0xD0($a2)
    /* DD3A64 00002FF8 8E260090 */  lw         $a2, 0x90($s1)
    /* DD3A68 00002FFC 24A57434 */  addiu      $a1, $a1, %lo(D_00007434)
    /* DD3A6C 00003000 00002025 */  or         $a0, $zero, $zero
    /* DD3A70 00003004 8CCF00C4 */  lw         $t7, 0xC4($a2)
    /* DD3A74 00003008 24C600C4 */  addiu      $a2, $a2, 0xC4
    /* DD3A78 0000300C 35F80040 */  ori        $t8, $t7, 0x40
    /* DD3A7C 00003010 ACD80000 */  sw         $t8, 0x0($a2)
    /* DD3A80 00003014 8E260090 */  lw         $a2, 0x90($s1)
    /* DD3A84 00003018 8CD900C4 */  lw         $t9, 0xC4($a2)
    /* DD3A88 0000301C 24C600C4 */  addiu      $a2, $a2, 0xC4
    /* DD3A8C 00003020 37280080 */  ori        $t0, $t9, 0x80
    /* DD3A90 00003024 0C000000 */  jal        func_00000000
    /* DD3A94 00003028 ACC80000 */   sw        $t0, 0x0($a2)
    /* DD3A98 0000302C 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DD3A9C 00003030 44810000 */  mtc1       $at, $f0
    /* DD3AA0 00003034 AE220094 */  sw         $v0, 0x94($s1)
    /* DD3AA4 00003038 24090002 */  addiu      $t1, $zero, 0x2
    /* DD3AA8 0000303C E4400130 */  swc1       $f0, 0x130($v0)
    /* DD3AAC 00003040 E440012C */  swc1       $f0, 0x12C($v0)
    /* DD3AB0 00003044 E4400128 */  swc1       $f0, 0x128($v0)
    /* DD3AB4 00003048 E4400124 */  swc1       $f0, 0x124($v0)
    /* DD3AB8 0000304C 8E260094 */  lw         $a2, 0x94($s1)
    /* DD3ABC 00003050 3C050000 */  lui        $a1, %hi(D_00007444)
    /* DD3AC0 00003054 24A57444 */  addiu      $a1, $a1, %lo(D_00007444)
    /* DD3AC4 00003058 ACC900D0 */  sw         $t1, 0xD0($a2)
    /* DD3AC8 0000305C 8E260094 */  lw         $a2, 0x94($s1)
    /* DD3ACC 00003060 00002025 */  or         $a0, $zero, $zero
    /* DD3AD0 00003064 8CCA00C4 */  lw         $t2, 0xC4($a2)
    /* DD3AD4 00003068 24C600C4 */  addiu      $a2, $a2, 0xC4
    /* DD3AD8 0000306C 354B0040 */  ori        $t3, $t2, 0x40
    /* DD3ADC 00003070 0C000000 */  jal        func_00000000
    /* DD3AE0 00003074 ACCB0000 */   sw        $t3, 0x0($a2)
    /* DD3AE4 00003078 44800000 */  mtc1       $zero, $f0
    /* DD3AE8 0000307C 3C010000 */  lui        $at, %hi(func_000003F8 + 0x130)
    /* DD3AEC 00003080 C4260528 */  lwc1       $f6, %lo(func_000003F8 + 0x130)($at)
    /* DD3AF0 00003084 00408025 */  or         $s0, $v0, $zero
    /* DD3AF4 00003088 24440070 */  addiu      $a0, $v0, 0x70
    /* DD3AF8 0000308C 27A5003C */  addiu      $a1, $sp, 0x3C
    /* DD3AFC 00003090 E7A0003C */  swc1       $f0, 0x3C($sp)
    /* DD3B00 00003094 E7A00044 */  swc1       $f0, 0x44($sp)
    /* DD3B04 00003098 0C000000 */  jal        func_00000000
    /* DD3B08 0000309C E7A60040 */   swc1      $f6, 0x40($sp)
    /* DD3B0C 000030A0 260200C4 */  addiu      $v0, $s0, 0xC4
    /* DD3B10 000030A4 8C4C0000 */  lw         $t4, 0x0($v0)
    /* DD3B14 000030A8 3C100000 */  lui        $s0, (0x0 >> 16)
    /* DD3B18 000030AC 26100000 */  addiu      $s0, $s0, 0x0
    /* DD3B1C 000030B0 358D0040 */  ori        $t5, $t4, 0x40
    /* DD3B20 000030B4 AC4D0000 */  sw         $t5, 0x0($v0)
    /* DD3B24 000030B8 8E250094 */  lw         $a1, 0x94($s1)
    /* DD3B28 000030BC 0C000000 */  jal        func_00000000
    /* DD3B2C 000030C0 02002025 */   or        $a0, $s0, $zero
    /* DD3B30 000030C4 02002025 */  or         $a0, $s0, $zero
    /* DD3B34 000030C8 0C000000 */  jal        func_00000000
    /* DD3B38 000030CC 8E250090 */   lw        $a1, 0x90($s1)
    /* DD3B3C 000030D0 02002025 */  or         $a0, $s0, $zero
    /* DD3B40 000030D4 0C000000 */  jal        func_00000000
    /* DD3B44 000030D8 8E250094 */   lw        $a1, 0x94($s1)
    /* DD3B48 000030DC 8E250090 */  lw         $a1, 0x90($s1)
    /* DD3B4C 000030E0 8E300084 */  lw         $s0, 0x84($s1)
    /* DD3B50 000030E4 AFA50058 */  sw         $a1, 0x58($sp)
    /* DD3B54 000030E8 0C000000 */  jal        func_00000000
    /* DD3B58 000030EC 26040010 */   addiu     $a0, $s0, 0x10
    /* DD3B5C 000030F0 8FA60058 */  lw         $a2, 0x58($sp)
    /* DD3B60 000030F4 240F0001 */  addiu      $t7, $zero, 0x1
    /* DD3B64 000030F8 2404016C */  addiu      $a0, $zero, 0x16C
    /* DD3B68 000030FC 8CCE0014 */  lw         $t6, 0x14($a2)
    /* DD3B6C 00003100 11C00002 */  beqz       $t6, .L0000310C
    /* DD3B70 00003104 00000000 */   nop
    /* DD3B74 00003108 ACCF0004 */  sw         $t7, 0x4($a2)
  .L0000310C:
    /* DD3B78 0000310C 0C000000 */  jal        func_00000000
    /* DD3B7C 00003110 ACD00014 */   sw        $s0, 0x14($a2)
    /* DD3B80 00003114 10400016 */  beqz       $v0, .L00003170
    /* DD3B84 00003118 00408025 */   or        $s0, $v0, $zero
    /* DD3B88 0000311C 44800000 */  mtc1       $zero, $f0
    /* DD3B8C 00003120 3C050000 */  lui        $a1, %hi(D_00007454)
    /* DD3B90 00003124 24A57454 */  addiu      $a1, $a1, %lo(D_00007454)
    /* DD3B94 00003128 44060000 */  mfc1       $a2, $f0
    /* DD3B98 0000312C 44070000 */  mfc1       $a3, $f0
    /* DD3B9C 00003130 00402025 */  or         $a0, $v0, $zero
    /* DD3BA0 00003134 0C000000 */  jal        func_00000000
    /* DD3BA4 00003138 E7A00010 */   swc1      $f0, 0x10($sp)
    /* DD3BA8 0000313C 3C010000 */  lui        $at, %hi(func_000003F8 + 0x134)
    /* DD3BAC 00003140 C420052C */  lwc1       $f0, %lo(func_000003F8 + 0x134)($at)
    /* DD3BB0 00003144 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DD3BB4 00003148 44814000 */  mtc1       $at, $f8
    /* DD3BB8 0000314C 3C180000 */  lui        $t8, (0x0 >> 16)
    /* DD3BBC 00003150 27180000 */  addiu      $t8, $t8, 0x0
    /* DD3BC0 00003154 3419FFFF */  ori        $t9, $zero, 0xFFFF
    /* DD3BC4 00003158 AE190120 */  sw         $t9, 0x120($s0)
    /* DD3BC8 0000315C AE180028 */  sw         $t8, 0x28($s0)
    /* DD3BCC 00003160 E6000108 */  swc1       $f0, 0x108($s0)
    /* DD3BD0 00003164 E600010C */  swc1       $f0, 0x10C($s0)
    /* DD3BD4 00003168 E6000110 */  swc1       $f0, 0x110($s0)
    /* DD3BD8 0000316C E6080124 */  swc1       $f8, 0x124($s0)
  .L00003170:
    /* DD3BDC 00003170 8E220084 */  lw         $v0, 0x84($s1)
    /* DD3BE0 00003174 02002825 */  or         $a1, $s0, $zero
    /* DD3BE4 00003178 24440010 */  addiu      $a0, $v0, 0x10
    /* DD3BE8 0000317C 0C000000 */  jal        func_00000000
    /* DD3BEC 00003180 AFA20048 */   sw        $v0, 0x48($sp)
    /* DD3BF0 00003184 8E080014 */  lw         $t0, 0x14($s0)
    /* DD3BF4 00003188 8FA20048 */  lw         $v0, 0x48($sp)
    /* DD3BF8 0000318C 24090001 */  addiu      $t1, $zero, 0x1
    /* DD3BFC 00003190 11000002 */  beqz       $t0, .L0000319C
    /* DD3C00 00003194 00000000 */   nop
    /* DD3C04 00003198 AE090004 */  sw         $t1, 0x4($s0)
  .L0000319C:
    /* DD3C08 0000319C 0C000000 */  jal        func_00000000
    /* DD3C0C 000031A0 AE020014 */   sw        $v0, 0x14($s0)
    /* DD3C10 000031A4 8FBF0024 */  lw         $ra, 0x24($sp)
    /* DD3C14 000031A8 8FB0001C */  lw         $s0, 0x1C($sp)
    /* DD3C18 000031AC 8FB10020 */  lw         $s1, 0x20($sp)
    /* DD3C1C 000031B0 03E00008 */  jr         $ra
    /* DD3C20 000031B4 27BD0060 */   addiu     $sp, $sp, 0x60
endlabel game_createLights
