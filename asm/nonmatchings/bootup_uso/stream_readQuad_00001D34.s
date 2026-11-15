nonmatching stream_readQuad_00001D34, 0x58

glabel stream_readQuad_00001D34
    /* DD27A0 00001D34 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DD27A4 00001D38 AFA40028 */  sw         $a0, 0x28($sp)
    /* DD27A8 00001D3C AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD27AC 00001D40 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD27B0 00001D44 24840000 */  addiu      $a0, $a0, 0x0
    /* DD27B4 00001D48 27A50018 */  addiu      $a1, $sp, 0x18
    /* DD27B8 00001D4C 0C000000 */  jal        func_00000000
    /* DD27BC 00001D50 24060010 */   addiu     $a2, $zero, 0x10
    /* DD27C0 00001D54 27AF0018 */  addiu      $t7, $sp, 0x18
    /* DD27C4 00001D58 8DF90000 */  lw         $t9, 0x0($t7)
    /* DD27C8 00001D5C 8FAE0028 */  lw         $t6, 0x28($sp)
    /* DD27CC 00001D60 ADD90000 */  sw         $t9, 0x0($t6)
    /* DD27D0 00001D64 8DF80004 */  lw         $t8, 0x4($t7)
    /* DD27D4 00001D68 ADD80004 */  sw         $t8, 0x4($t6)
    /* DD27D8 00001D6C 8DF90008 */  lw         $t9, 0x8($t7)
    /* DD27DC 00001D70 ADD90008 */  sw         $t9, 0x8($t6)
    /* DD27E0 00001D74 8DF8000C */  lw         $t8, 0xC($t7)
    /* DD27E4 00001D78 ADD8000C */  sw         $t8, 0xC($t6)
    /* DD27E8 00001D7C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD27EC 00001D80 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DD27F0 00001D84 03E00008 */  jr         $ra
    /* DD27F4 00001D88 00000000 */   nop
endlabel stream_readQuad_00001D34
