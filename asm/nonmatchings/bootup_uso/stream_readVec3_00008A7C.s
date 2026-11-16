nonmatching stream_readVec3_00008A7C, 0x70

glabel stream_readVec3_00008A7C
    /* DD94E8 00008A7C 27BDFFB8 */  addiu      $sp, $sp, -0x48
    /* DD94EC 00008A80 00803825 */  or         $a3, $a0, $zero
    /* DD94F0 00008A84 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD94F4 00008A88 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD94F8 00008A8C 24840000 */  addiu      $a0, $a0, 0x0
    /* DD94FC 00008A90 AFA70048 */  sw         $a3, 0x48($sp)
    /* DD9500 00008A94 27A50038 */  addiu      $a1, $sp, 0x38
    /* DD9504 00008A98 0C000000 */  jal        func_00000000
    /* DD9508 00008A9C 2406000C */   addiu     $a2, $zero, 0xC
    /* DD950C 00008AA0 27AF0038 */  addiu      $t7, $sp, 0x38
    /* DD9510 00008AA4 8DF90000 */  lw         $t9, 0x0($t7)
    /* DD9514 00008AA8 8FA70048 */  lw         $a3, 0x48($sp)
    /* DD9518 00008AAC 27AE0024 */  addiu      $t6, $sp, 0x24
    /* DD951C 00008AB0 ADD90000 */  sw         $t9, 0x0($t6)
    /* DD9520 00008AB4 8DF80004 */  lw         $t8, 0x4($t7)
    /* DD9524 00008AB8 ADD80004 */  sw         $t8, 0x4($t6)
    /* DD9528 00008ABC 8DF90008 */  lw         $t9, 0x8($t7)
    /* DD952C 00008AC0 ADD90008 */  sw         $t9, 0x8($t6)
    /* DD9530 00008AC4 C7A40024 */  lwc1       $f4, 0x24($sp)
    /* DD9534 00008AC8 E4E40000 */  swc1       $f4, 0x0($a3)
    /* DD9538 00008ACC C7A60028 */  lwc1       $f6, 0x28($sp)
    /* DD953C 00008AD0 E4E60004 */  swc1       $f6, 0x4($a3)
    /* DD9540 00008AD4 C7A8002C */  lwc1       $f8, 0x2C($sp)
    /* DD9544 00008AD8 E4E80008 */  swc1       $f8, 0x8($a3)
    /* DD9548 00008ADC 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD954C 00008AE0 27BD0048 */  addiu      $sp, $sp, 0x48
    /* DD9550 00008AE4 03E00008 */  jr         $ra
    /* DD9554 00008AE8 00000000 */   nop
endlabel stream_readVec3_00008A7C
