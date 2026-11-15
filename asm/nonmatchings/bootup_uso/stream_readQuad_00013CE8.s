nonmatching stream_readQuad_00013CE8, 0x58

glabel stream_readQuad_00013CE8
    /* DE4754 00013CE8 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DE4758 00013CEC AFA40028 */  sw         $a0, 0x28($sp)
    /* DE475C 00013CF0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DE4760 00013CF4 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DE4764 00013CF8 24840000 */  addiu      $a0, $a0, 0x0
    /* DE4768 00013CFC 27A50018 */  addiu      $a1, $sp, 0x18
    /* DE476C 00013D00 0C000000 */  jal        func_00000000
    /* DE4770 00013D04 24060010 */   addiu     $a2, $zero, 0x10
    /* DE4774 00013D08 27AF0018 */  addiu      $t7, $sp, 0x18
    /* DE4778 00013D0C 8DF90000 */  lw         $t9, 0x0($t7)
    /* DE477C 00013D10 8FAE0028 */  lw         $t6, 0x28($sp)
    /* DE4780 00013D14 ADD90000 */  sw         $t9, 0x0($t6)
    /* DE4784 00013D18 8DF80004 */  lw         $t8, 0x4($t7)
    /* DE4788 00013D1C ADD80004 */  sw         $t8, 0x4($t6)
    /* DE478C 00013D20 8DF90008 */  lw         $t9, 0x8($t7)
    /* DE4790 00013D24 ADD90008 */  sw         $t9, 0x8($t6)
    /* DE4794 00013D28 8DF8000C */  lw         $t8, 0xC($t7)
    /* DE4798 00013D2C ADD8000C */  sw         $t8, 0xC($t6)
    /* DE479C 00013D30 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DE47A0 00013D34 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DE47A4 00013D38 03E00008 */  jr         $ra
    /* DE47A8 00013D3C 00000000 */   nop
endlabel stream_readQuad_00013CE8
