nonmatching game_create3DWindow, 0x150

glabel game_create3DWindow
    /* DD3F54 000034E8 3C010000 */  lui        $at, %hi(func_000003F8 + 0x140)
    /* DD3F58 000034EC C4240538 */  lwc1       $f4, %lo(func_000003F8 + 0x140)($at)
    /* DD3F5C 000034F0 3C020000 */  lui        $v0, %hi(func_00000008 + 0x28)
    /* DD3F60 000034F4 24420030 */  addiu      $v0, $v0, %lo(func_00000008 + 0x28)
    /* DD3F64 000034F8 3C010000 */  lui        $at, %hi(func_000003F8 + 0x144)
    /* DD3F68 000034FC E4440000 */  swc1       $f4, 0x0($v0)
    /* DD3F6C 00003500 C426053C */  lwc1       $f6, %lo(func_000003F8 + 0x144)($at)
    /* DD3F70 00003504 27BDFFA0 */  addiu      $sp, $sp, -0x60
    /* DD3F74 00003508 3C010000 */  lui        $at, %hi(func_000003F8 + 0x148)
    /* DD3F78 0000350C E4460004 */  swc1       $f6, 0x4($v0)
    /* DD3F7C 00003510 C4280540 */  lwc1       $f8, %lo(func_000003F8 + 0x148)($at)
    /* DD3F80 00003514 44805000 */  mtc1       $zero, $f10
    /* DD3F84 00003518 AFB00058 */  sw         $s0, 0x58($sp)
    /* DD3F88 0000351C 240E0002 */  addiu      $t6, $zero, 0x2
    /* DD3F8C 00003520 240F006E */  addiu      $t7, $zero, 0x6E
    /* DD3F90 00003524 00A08025 */  or         $s0, $a1, $zero
    /* DD3F94 00003528 AFBF005C */  sw         $ra, 0x5C($sp)
    /* DD3F98 0000352C AFA40060 */  sw         $a0, 0x60($sp)
    /* DD3F9C 00003530 AFA60068 */  sw         $a2, 0x68($sp)
    /* DD3FA0 00003534 AFAF0014 */  sw         $t7, 0x14($sp)
    /* DD3FA4 00003538 AFAE0010 */  sw         $t6, 0x10($sp)
    /* DD3FA8 0000353C E4480008 */  swc1       $f8, 0x8($v0)
    /* DD3FAC 00003540 E44A000C */  swc1       $f10, 0xC($v0)
    /* DD3FB0 00003544 8E180000 */  lw         $t8, 0x0($s0)
    /* DD3FB4 00003548 2408006F */  addiu      $t0, $zero, 0x6F
    /* DD3FB8 0000354C 240B0070 */  addiu      $t3, $zero, 0x70
    /* DD3FBC 00003550 AFB80018 */  sw         $t8, 0x18($sp)
    /* DD3FC0 00003554 8E190004 */  lw         $t9, 0x4($s0)
    /* DD3FC4 00003558 AFA80020 */  sw         $t0, 0x20($sp)
    /* DD3FC8 0000355C 24180073 */  addiu      $t8, $zero, 0x73
    /* DD3FCC 00003560 AFB9001C */  sw         $t9, 0x1C($sp)
    /* DD3FD0 00003564 8E090008 */  lw         $t1, 0x8($s0)
    /* DD3FD4 00003568 240E0071 */  addiu      $t6, $zero, 0x71
    /* DD3FD8 0000356C 240F0001 */  addiu      $t7, $zero, 0x1
    /* DD3FDC 00003570 AFA90024 */  sw         $t1, 0x24($sp)
    /* DD3FE0 00003574 8E0A000C */  lw         $t2, 0xC($s0)
    /* DD3FE4 00003578 AFAB002C */  sw         $t3, 0x2C($sp)
    /* DD3FE8 0000357C 24080074 */  addiu      $t0, $zero, 0x74
    /* DD3FEC 00003580 AFAA0028 */  sw         $t2, 0x28($sp)
    /* DD3FF0 00003584 8E0C0018 */  lw         $t4, 0x18($s0)
    /* DD3FF4 00003588 AFB8003C */  sw         $t8, 0x3C($sp)
    /* DD3FF8 0000358C AFAF0038 */  sw         $t7, 0x38($sp)
    /* DD3FFC 00003590 358D0002 */  ori        $t5, $t4, 0x2
    /* DD4000 00003594 AFAD0030 */  sw         $t5, 0x30($sp)
    /* DD4004 00003598 AFAE0034 */  sw         $t6, 0x34($sp)
    /* DD4008 0000359C 8E190010 */  lw         $t9, 0x10($s0)
    /* DD400C 000035A0 AFA80044 */  sw         $t0, 0x44($sp)
    /* DD4010 000035A4 3C060000 */  lui        $a2, %hi(D_000074D0)
    /* DD4014 000035A8 AFB90040 */  sw         $t9, 0x40($sp)
    /* DD4018 000035AC 8E090014 */  lw         $t1, 0x14($s0)
    /* DD401C 000035B0 AFA0004C */  sw         $zero, 0x4C($sp)
    /* DD4020 000035B4 24C674D0 */  addiu      $a2, $a2, %lo(D_000074D0)
    /* DD4024 000035B8 00002025 */  or         $a0, $zero, $zero
    /* DD4028 000035BC 24050064 */  addiu      $a1, $zero, 0x64
    /* DD402C 000035C0 2407006D */  addiu      $a3, $zero, 0x6D
    /* DD4030 000035C4 0C000000 */  jal        func_00000000
    /* DD4034 000035C8 AFA90048 */   sw        $t1, 0x48($sp)
    /* DD4038 000035CC 8FAA0068 */  lw         $t2, 0x68($sp)
    /* DD403C 000035D0 3C040000 */  lui        $a0, %hi(func_00000008 + 0x24)
    /* DD4040 000035D4 00408025 */  or         $s0, $v0, $zero
    /* DD4044 000035D8 2484002C */  addiu      $a0, $a0, %lo(func_00000008 + 0x24)
    /* DD4048 000035DC 00402825 */  or         $a1, $v0, $zero
    /* DD404C 000035E0 0C000000 */  jal        func_00000000
    /* DD4050 000035E4 AC4A0070 */   sw        $t2, 0x70($v0)
    /* DD4054 000035E8 8FAB0060 */  lw         $t3, 0x60($sp)
    /* DD4058 000035EC 8D650084 */  lw         $a1, 0x84($t3)
    /* DD405C 000035F0 50A00004 */  beql       $a1, $zero, .L00003604
    /* DD4060 000035F4 8FAC0060 */   lw        $t4, 0x60($sp)
    /* DD4064 000035F8 0C000000 */  jal        func_00000000
    /* DD4068 000035FC 02002025 */   or        $a0, $s0, $zero
    /* DD406C 00003600 8FAC0060 */  lw         $t4, 0x60($sp)
  .L00003604:
    /* DD4070 00003604 8D850080 */  lw         $a1, 0x80($t4)
    /* DD4074 00003608 50A00004 */  beql       $a1, $zero, .L0000361C
    /* DD4078 0000360C 8FA40060 */   lw        $a0, 0x60($sp)
    /* DD407C 00003610 0C000000 */  jal        func_00000000
    /* DD4080 00003614 02002025 */   or        $a0, $s0, $zero
    /* DD4084 00003618 8FA40060 */  lw         $a0, 0x60($sp)
  .L0000361C:
    /* DD4088 0000361C 0C000000 */  jal        func_00000000
    /* DD408C 00003620 02002825 */   or        $a1, $s0, $zero
    /* DD4090 00003624 8FBF005C */  lw         $ra, 0x5C($sp)
    /* DD4094 00003628 02001025 */  or         $v0, $s0, $zero
    /* DD4098 0000362C 8FB00058 */  lw         $s0, 0x58($sp)
    /* DD409C 00003630 03E00008 */  jr         $ra
    /* DD40A0 00003634 27BD0060 */   addiu     $sp, $sp, 0x60
endlabel game_create3DWindow
