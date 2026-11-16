nonmatching stream_readS32_00000008, 0x3C

glabel stream_readS32_00000008
    /* DD0A74 00000008 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD0A78 0000000C AFA40020 */  sw         $a0, 0x20($sp)
    /* DD0A7C 00000010 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD0A80 00000014 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD0A84 00000018 24840000 */  addiu      $a0, $a0, 0x0
    /* DD0A88 0000001C 27A50018 */  addiu      $a1, $sp, 0x18
    /* DD0A8C 00000020 0C000000 */  jal        func_00000000
    /* DD0A90 00000024 24060004 */   addiu     $a2, $zero, 0x4
    /* DD0A94 00000028 8FAE0018 */  lw         $t6, 0x18($sp)
    /* DD0A98 0000002C 8FAF0020 */  lw         $t7, 0x20($sp)
    /* DD0A9C 00000030 ADEE0000 */  sw         $t6, 0x0($t7)
    /* DD0AA0 00000034 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD0AA4 00000038 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD0AA8 0000003C 03E00008 */  jr         $ra
    /* DD0AAC 00000040 00000000 */   nop
endlabel stream_readS32_00000008
