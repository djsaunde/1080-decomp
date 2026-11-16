nonmatching stream_readVec3_00000080, 0x70

glabel stream_readVec3_00000080
    /* DD0AEC 00000080 27BDFFB8 */  addiu      $sp, $sp, -0x48
    /* DD0AF0 00000084 00803825 */  or         $a3, $a0, $zero
    /* DD0AF4 00000088 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD0AF8 0000008C 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD0AFC 00000090 24840000 */  addiu      $a0, $a0, 0x0
    /* DD0B00 00000094 AFA70048 */  sw         $a3, 0x48($sp)
    /* DD0B04 00000098 27A50038 */  addiu      $a1, $sp, 0x38
    /* DD0B08 0000009C 0C000000 */  jal        func_00000000
    /* DD0B0C 000000A0 2406000C */   addiu     $a2, $zero, 0xC
    /* DD0B10 000000A4 27AF0038 */  addiu      $t7, $sp, 0x38
    /* DD0B14 000000A8 8DF90000 */  lw         $t9, 0x0($t7)
    /* DD0B18 000000AC 8FA70048 */  lw         $a3, 0x48($sp)
    /* DD0B1C 000000B0 27AE0024 */  addiu      $t6, $sp, 0x24
    /* DD0B20 000000B4 ADD90000 */  sw         $t9, 0x0($t6)
    /* DD0B24 000000B8 8DF80004 */  lw         $t8, 0x4($t7)
    /* DD0B28 000000BC ADD80004 */  sw         $t8, 0x4($t6)
    /* DD0B2C 000000C0 8DF90008 */  lw         $t9, 0x8($t7)
    /* DD0B30 000000C4 ADD90008 */  sw         $t9, 0x8($t6)
    /* DD0B34 000000C8 C7A40024 */  lwc1       $f4, 0x24($sp)
    /* DD0B38 000000CC E4E40000 */  swc1       $f4, 0x0($a3)
    /* DD0B3C 000000D0 C7A60028 */  lwc1       $f6, 0x28($sp)
    /* DD0B40 000000D4 E4E60004 */  swc1       $f6, 0x4($a3)
    /* DD0B44 000000D8 C7A8002C */  lwc1       $f8, 0x2C($sp)
    /* DD0B48 000000DC E4E80008 */  swc1       $f8, 0x8($a3)
    /* DD0B4C 000000E0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD0B50 000000E4 27BD0048 */  addiu      $sp, $sp, 0x48
    /* DD0B54 000000E8 03E00008 */  jr         $ra
    /* DD0B58 000000EC 00000000 */   nop
endlabel stream_readVec3_00000080
