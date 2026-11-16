nonmatching stream_readQuad_000000F0, 0x58

glabel stream_readQuad_000000F0
    /* DD0B5C 000000F0 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DD0B60 000000F4 AFA40028 */  sw         $a0, 0x28($sp)
    /* DD0B64 000000F8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD0B68 000000FC 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD0B6C 00000100 24840000 */  addiu      $a0, $a0, 0x0
    /* DD0B70 00000104 27A50018 */  addiu      $a1, $sp, 0x18
    /* DD0B74 00000108 0C000000 */  jal        func_00000000
    /* DD0B78 0000010C 24060010 */   addiu     $a2, $zero, 0x10
    /* DD0B7C 00000110 27AF0018 */  addiu      $t7, $sp, 0x18
    /* DD0B80 00000114 8DF90000 */  lw         $t9, 0x0($t7)
    /* DD0B84 00000118 8FAE0028 */  lw         $t6, 0x28($sp)
    /* DD0B88 0000011C ADD90000 */  sw         $t9, 0x0($t6)
    /* DD0B8C 00000120 8DF80004 */  lw         $t8, 0x4($t7)
    /* DD0B90 00000124 ADD80004 */  sw         $t8, 0x4($t6)
    /* DD0B94 00000128 8DF90008 */  lw         $t9, 0x8($t7)
    /* DD0B98 0000012C ADD90008 */  sw         $t9, 0x8($t6)
    /* DD0B9C 00000130 8DF8000C */  lw         $t8, 0xC($t7)
    /* DD0BA0 00000134 ADD8000C */  sw         $t8, 0xC($t6)
    /* DD0BA4 00000138 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD0BA8 0000013C 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DD0BAC 00000140 03E00008 */  jr         $ra
    /* DD0BB0 00000144 00000000 */   nop
endlabel stream_readQuad_000000F0
