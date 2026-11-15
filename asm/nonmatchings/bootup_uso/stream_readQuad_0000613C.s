nonmatching stream_readQuad_0000613C, 0x58

glabel stream_readQuad_0000613C
    /* DD6BA8 0000613C 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DD6BAC 00006140 AFA40028 */  sw         $a0, 0x28($sp)
    /* DD6BB0 00006144 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD6BB4 00006148 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD6BB8 0000614C 24840000 */  addiu      $a0, $a0, 0x0
    /* DD6BBC 00006150 27A50018 */  addiu      $a1, $sp, 0x18
    /* DD6BC0 00006154 0C000000 */  jal        func_00000000
    /* DD6BC4 00006158 24060010 */   addiu     $a2, $zero, 0x10
    /* DD6BC8 0000615C 27AF0018 */  addiu      $t7, $sp, 0x18
    /* DD6BCC 00006160 8DF90000 */  lw         $t9, 0x0($t7)
    /* DD6BD0 00006164 8FAE0028 */  lw         $t6, 0x28($sp)
    /* DD6BD4 00006168 ADD90000 */  sw         $t9, 0x0($t6)
    /* DD6BD8 0000616C 8DF80004 */  lw         $t8, 0x4($t7)
    /* DD6BDC 00006170 ADD80004 */  sw         $t8, 0x4($t6)
    /* DD6BE0 00006174 8DF90008 */  lw         $t9, 0x8($t7)
    /* DD6BE4 00006178 ADD90008 */  sw         $t9, 0x8($t6)
    /* DD6BE8 0000617C 8DF8000C */  lw         $t8, 0xC($t7)
    /* DD6BEC 00006180 ADD8000C */  sw         $t8, 0xC($t6)
    /* DD6BF0 00006184 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD6BF4 00006188 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DD6BF8 0000618C 03E00008 */  jr         $ra
    /* DD6BFC 00006190 00000000 */   nop
endlabel stream_readQuad_0000613C
