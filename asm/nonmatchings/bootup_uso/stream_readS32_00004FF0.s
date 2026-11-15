nonmatching stream_readS32_00004FF0, 0x3C

glabel stream_readS32_00004FF0
    /* DD5A5C 00004FF0 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD5A60 00004FF4 AFA40020 */  sw         $a0, 0x20($sp)
    /* DD5A64 00004FF8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD5A68 00004FFC 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD5A6C 00005000 24840000 */  addiu      $a0, $a0, 0x0
    /* DD5A70 00005004 27A50018 */  addiu      $a1, $sp, 0x18
    /* DD5A74 00005008 0C000000 */  jal        func_00000000
    /* DD5A78 0000500C 24060004 */   addiu     $a2, $zero, 0x4
    /* DD5A7C 00005010 8FAE0018 */  lw         $t6, 0x18($sp)
    /* DD5A80 00005014 8FAF0020 */  lw         $t7, 0x20($sp)
    /* DD5A84 00005018 ADEE0000 */  sw         $t6, 0x0($t7)
    /* DD5A88 0000501C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD5A8C 00005020 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD5A90 00005024 03E00008 */  jr         $ra
    /* DD5A94 00005028 00000000 */   nop
endlabel stream_readS32_00004FF0
