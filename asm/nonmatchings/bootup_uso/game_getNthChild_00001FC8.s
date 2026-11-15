nonmatching game_getNthChild_00001FC8, 0x4C

glabel game_getNthChild_00001FC8
    /* DD2A34 00001FC8 8C830070 */  lw         $v1, 0x70($a0)
    /* DD2A38 00001FCC 18A0000F */  blez       $a1, .L0000200C
    /* DD2A3C 00001FD0 00001025 */   or        $v0, $zero, $zero
    /* DD2A40 00001FD4 30A60003 */  andi       $a2, $a1, 0x3
    /* DD2A44 00001FD8 10C00006 */  beqz       $a2, .L00001FF4
    /* DD2A48 00001FDC 00C02025 */   or        $a0, $a2, $zero
  .L00001FE0:
    /* DD2A4C 00001FE0 24420001 */  addiu      $v0, $v0, 0x1
    /* DD2A50 00001FE4 1482FFFE */  bne        $a0, $v0, .L00001FE0
    /* DD2A54 00001FE8 8C630044 */   lw        $v1, 0x44($v1)
    /* DD2A58 00001FEC 10450007 */  beq        $v0, $a1, .L0000200C
    /* DD2A5C 00001FF0 00000000 */   nop
  .L00001FF4:
    /* DD2A60 00001FF4 8C630044 */  lw         $v1, 0x44($v1)
    /* DD2A64 00001FF8 24420004 */  addiu      $v0, $v0, 0x4
    /* DD2A68 00001FFC 8C630044 */  lw         $v1, 0x44($v1)
    /* DD2A6C 00002000 8C630044 */  lw         $v1, 0x44($v1)
    /* DD2A70 00002004 1445FFFB */  bne        $v0, $a1, .L00001FF4
    /* DD2A74 00002008 8C630044 */   lw        $v1, 0x44($v1)
  .L0000200C:
    /* DD2A78 0000200C 03E00008 */  jr         $ra
    /* DD2A7C 00002010 00601025 */   or        $v0, $v1, $zero
endlabel game_getNthChild_00001FC8
