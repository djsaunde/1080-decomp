nonmatching stream_readVec3_0000F2EC, 0xA4

glabel stream_readVec3_0000F2EC
    /* DDFD58 0000F2EC 27BDFFA8 */  addiu      $sp, $sp, -0x58
    /* DDFD5C 0000F2F0 AFBF0024 */  sw         $ra, 0x24($sp)
    /* DDFD60 0000F2F4 AFA40058 */  sw         $a0, 0x58($sp)
    /* DDFD64 0000F2F8 AFB30020 */  sw         $s3, 0x20($sp)
    /* DDFD68 0000F2FC AFB2001C */  sw         $s2, 0x1C($sp)
    /* DDFD6C 0000F300 AFB10018 */  sw         $s1, 0x18($sp)
    /* DDFD70 0000F304 AFB00014 */  sw         $s0, 0x14($sp)
    /* DDFD74 0000F308 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDFD78 0000F30C 24840000 */  addiu      $a0, $a0, 0x0
    /* DDFD7C 0000F310 27A50048 */  addiu      $a1, $sp, 0x48
    /* DDFD80 0000F314 2406000C */  addiu      $a2, $zero, 0xC
    /* DDFD84 0000F318 0C000000 */  jal        func_00000000
    /* DDFD88 0000F31C 00000000 */   nop
    /* DDFD8C 0000F320 8FB10058 */  lw         $s1, 0x58($sp)
    /* DDFD90 0000F324 27AE0034 */  addiu      $t6, $sp, 0x34
    /* DDFD94 0000F328 27AF0048 */  addiu      $t7, $sp, 0x48
    /* DDFD98 0000F32C 8DF90000 */  lw         $t9, 0x0($t7)
    /* DDFD9C 0000F330 ADD90000 */  sw         $t9, 0x0($t6)
    /* DDFDA0 0000F334 8DF80004 */  lw         $t8, 0x4($t7)
    /* DDFDA4 0000F338 ADD80004 */  sw         $t8, 0x4($t6)
    /* DDFDA8 0000F33C 8DF90008 */  lw         $t9, 0x8($t7)
    /* DDFDAC 0000F340 ADD90008 */  sw         $t9, 0x8($t6)
    /* DDFDB0 0000F344 02209025 */  or         $s2, $s1, $zero
    /* DDFDB4 0000F348 27B30034 */  addiu      $s3, $sp, 0x34
    /* DDFDB8 0000F34C C6640000 */  lwc1       $f4, 0x0($s3)
    /* DDFDBC 0000F350 E6440000 */  swc1       $f4, 0x0($s2)
    /* DDFDC0 0000F354 C6660004 */  lwc1       $f6, 0x4($s3)
    /* DDFDC4 0000F358 E6460004 */  swc1       $f6, 0x4($s2)
    /* DDFDC8 0000F35C C6680008 */  lwc1       $f8, 0x8($s3)
    /* DDFDCC 0000F360 E6480008 */  swc1       $f8, 0x8($s2)
    /* DDFDD0 0000F364 02208025 */  or         $s0, $s1, $zero
    /* DDFDD4 0000F368 10000001 */  b          .L0000F370
    /* DDFDD8 0000F36C 00000000 */   nop
  .L0000F370:
    /* DDFDDC 0000F370 8FB00014 */  lw         $s0, 0x14($sp)
    /* DDFDE0 0000F374 8FB10018 */  lw         $s1, 0x18($sp)
    /* DDFDE4 0000F378 8FB2001C */  lw         $s2, 0x1C($sp)
    /* DDFDE8 0000F37C 8FB30020 */  lw         $s3, 0x20($sp)
    /* DDFDEC 0000F380 8FBF0024 */  lw         $ra, 0x24($sp)
    /* DDFDF0 0000F384 27BD0058 */  addiu      $sp, $sp, 0x58
    /* DDFDF4 0000F388 03E00008 */  jr         $ra
    /* DDFDF8 0000F38C 00000000 */   nop
endlabel stream_readVec3_0000F2EC
