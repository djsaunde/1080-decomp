nonmatching Process_pause_0000F9E8, 0x100

glabel Process_pause_0000F9E8
    /* DE0454 0000F9E8 27BDFFC8 */  addiu      $sp, $sp, -0x38
    /* DE0458 0000F9EC AFBF0024 */  sw         $ra, 0x24($sp)
    /* DE045C 0000F9F0 AFA40038 */  sw         $a0, 0x38($sp)
    /* DE0460 0000F9F4 AFB30020 */  sw         $s3, 0x20($sp)
    /* DE0464 0000F9F8 AFB2001C */  sw         $s2, 0x1C($sp)
    /* DE0468 0000F9FC AFB10018 */  sw         $s1, 0x18($sp)
    /* DE046C 0000FA00 AFB00014 */  sw         $s0, 0x14($sp)
    /* DE0470 0000FA04 0C000000 */  jal        func_00000000
    /* DE0474 0000FA08 00000000 */   nop
    /* DE0478 0000FA0C 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DE047C 0000FA10 24840000 */  addiu      $a0, $a0, 0x0
    /* DE0480 0000FA14 0C000000 */  jal        func_00000000
    /* DE0484 0000FA18 00000000 */   nop
    /* DE0488 0000FA1C 8FAE0038 */  lw         $t6, 0x38($sp)
    /* DE048C 0000FA20 ADC20038 */  sw         $v0, 0x38($t6)
    /* DE0490 0000FA24 8FAF0038 */  lw         $t7, 0x38($sp)
    /* DE0494 0000FA28 8DF80048 */  lw         $t8, 0x48($t7)
    /* DE0498 0000FA2C 8F100028 */  lw         $s0, 0x28($t8)
    /* DE049C 0000FA30 86190058 */  lh         $t9, 0x58($s0)
    /* DE04A0 0000FA34 8DE80048 */  lw         $t0, 0x48($t7)
    /* DE04A4 0000FA38 03282021 */  addu       $a0, $t9, $t0
    /* DE04A8 0000FA3C 8E19005C */  lw         $t9, 0x5C($s0)
    /* DE04AC 0000FA40 0320F809 */  jalr       $t9
    /* DE04B0 0000FA44 00000000 */   nop
    /* DE04B4 0000FA48 8FA90038 */  lw         $t1, 0x38($sp)
    /* DE04B8 0000FA4C 8D2A0048 */  lw         $t2, 0x48($t1)
    /* DE04BC 0000FA50 AD40007C */  sw         $zero, 0x7C($t2)
    /* DE04C0 0000FA54 240B0001 */  addiu      $t3, $zero, 0x1
    /* DE04C4 0000FA58 8FAC0038 */  lw         $t4, 0x38($sp)
    /* DE04C8 0000FA5C AD8B0030 */  sw         $t3, 0x30($t4)
    /* DE04CC 0000FA60 8FAD0038 */  lw         $t5, 0x38($sp)
    /* DE04D0 0000FA64 ADA00034 */  sw         $zero, 0x34($t5)
    /* DE04D4 0000FA68 8FAE0038 */  lw         $t6, 0x38($sp)
    /* DE04D8 0000FA6C 8DD8004C */  lw         $t8, 0x4C($t6)
    /* DE04DC 0000FA70 13000013 */  beqz       $t8, .L0000FAC0
    /* DE04E0 0000FA74 00000000 */   nop
    /* DE04E4 0000FA78 8FAF0038 */  lw         $t7, 0x38($sp)
    /* DE04E8 0000FA7C 8DF0004C */  lw         $s0, 0x4C($t7)
    /* DE04EC 0000FA80 26100018 */  addiu      $s0, $s0, 0x18
    /* DE04F0 0000FA84 02008825 */  or         $s1, $s0, $zero
    /* DE04F4 0000FA88 00009025 */  or         $s2, $zero, $zero
    /* DE04F8 0000FA8C 12400007 */  beqz       $s2, .L0000FAAC
    /* DE04FC 0000FA90 00000000 */   nop
    /* DE0500 0000FA94 02209825 */  or         $s3, $s1, $zero
    /* DE0504 0000FA98 8E680000 */  lw         $t0, 0x0($s3)
    /* DE0508 0000FA9C 35190008 */  ori        $t9, $t0, 0x8
    /* DE050C 0000FAA0 AE790000 */  sw         $t9, 0x0($s3)
    /* DE0510 0000FAA4 10000006 */  b          .L0000FAC0
    /* DE0514 0000FAA8 00000000 */   nop
  .L0000FAAC:
    /* DE0518 0000FAAC 02209825 */  or         $s3, $s1, $zero
    /* DE051C 0000FAB0 8E690000 */  lw         $t1, 0x0($s3)
    /* DE0520 0000FAB4 2401FFF7 */  addiu      $at, $zero, -0x9
    /* DE0524 0000FAB8 01215024 */  and        $t2, $t1, $at
    /* DE0528 0000FABC AE6A0000 */  sw         $t2, 0x0($s3)
  .L0000FAC0:
    /* DE052C 0000FAC0 10000001 */  b          .L0000FAC8
    /* DE0530 0000FAC4 00000000 */   nop
  .L0000FAC8:
    /* DE0534 0000FAC8 8FB00014 */  lw         $s0, 0x14($sp)
    /* DE0538 0000FACC 8FB10018 */  lw         $s1, 0x18($sp)
    /* DE053C 0000FAD0 8FB2001C */  lw         $s2, 0x1C($sp)
    /* DE0540 0000FAD4 8FB30020 */  lw         $s3, 0x20($sp)
    /* DE0544 0000FAD8 8FBF0024 */  lw         $ra, 0x24($sp)
    /* DE0548 0000FADC 27BD0038 */  addiu      $sp, $sp, 0x38
    /* DE054C 0000FAE0 03E00008 */  jr         $ra
    /* DE0550 0000FAE4 00000000 */   nop
endlabel Process_pause_0000F9E8
