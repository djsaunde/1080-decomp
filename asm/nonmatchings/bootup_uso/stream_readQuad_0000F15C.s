nonmatching stream_readQuad_0000F15C, 0x58

glabel stream_readQuad_0000F15C
    /* DDFBC8 0000F15C 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DDFBCC 0000F160 AFA40028 */  sw         $a0, 0x28($sp)
    /* DDFBD0 0000F164 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDFBD4 0000F168 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDFBD8 0000F16C 24840000 */  addiu      $a0, $a0, 0x0
    /* DDFBDC 0000F170 27A50018 */  addiu      $a1, $sp, 0x18
    /* DDFBE0 0000F174 0C000000 */  jal        func_00000000
    /* DDFBE4 0000F178 24060010 */   addiu     $a2, $zero, 0x10
    /* DDFBE8 0000F17C 27AF0018 */  addiu      $t7, $sp, 0x18
    /* DDFBEC 0000F180 8DF90000 */  lw         $t9, 0x0($t7)
    /* DDFBF0 0000F184 8FAE0028 */  lw         $t6, 0x28($sp)
    /* DDFBF4 0000F188 ADD90000 */  sw         $t9, 0x0($t6)
    /* DDFBF8 0000F18C 8DF80004 */  lw         $t8, 0x4($t7)
    /* DDFBFC 0000F190 ADD80004 */  sw         $t8, 0x4($t6)
    /* DDFC00 0000F194 8DF90008 */  lw         $t9, 0x8($t7)
    /* DDFC04 0000F198 ADD90008 */  sw         $t9, 0x8($t6)
    /* DDFC08 0000F19C 8DF8000C */  lw         $t8, 0xC($t7)
    /* DDFC0C 0000F1A0 ADD8000C */  sw         $t8, 0xC($t6)
    /* DDFC10 0000F1A4 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDFC14 0000F1A8 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DDFC18 0000F1AC 03E00008 */  jr         $ra
    /* DDFC1C 0000F1B0 00000000 */   nop
endlabel stream_readQuad_0000F15C
