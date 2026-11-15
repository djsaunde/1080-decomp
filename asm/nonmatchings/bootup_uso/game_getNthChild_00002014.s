nonmatching game_getNthChild_00002014, 0x4C

glabel game_getNthChild_00002014
    /* DD2A80 00002014 8C830070 */  lw         $v1, 0x70($a0)
    /* DD2A84 00002018 18A0000F */  blez       $a1, .L00002058
    /* DD2A88 0000201C 00001025 */   or        $v0, $zero, $zero
    /* DD2A8C 00002020 30A60003 */  andi       $a2, $a1, 0x3
    /* DD2A90 00002024 10C00006 */  beqz       $a2, .L00002040
    /* DD2A94 00002028 00C02025 */   or        $a0, $a2, $zero
  .L0000202C:
    /* DD2A98 0000202C 24420001 */  addiu      $v0, $v0, 0x1
    /* DD2A9C 00002030 1482FFFE */  bne        $a0, $v0, .L0000202C
    /* DD2AA0 00002034 8C630044 */   lw        $v1, 0x44($v1)
    /* DD2AA4 00002038 10450007 */  beq        $v0, $a1, .L00002058
    /* DD2AA8 0000203C 00000000 */   nop
  .L00002040:
    /* DD2AAC 00002040 8C630044 */  lw         $v1, 0x44($v1)
    /* DD2AB0 00002044 24420004 */  addiu      $v0, $v0, 0x4
    /* DD2AB4 00002048 8C630044 */  lw         $v1, 0x44($v1)
    /* DD2AB8 0000204C 8C630044 */  lw         $v1, 0x44($v1)
    /* DD2ABC 00002050 1445FFFB */  bne        $v0, $a1, .L00002040
    /* DD2AC0 00002054 8C630044 */   lw        $v1, 0x44($v1)
  .L00002058:
    /* DD2AC4 00002058 03E00008 */  jr         $ra
    /* DD2AC8 0000205C 00601025 */   or        $v0, $v1, $zero
endlabel game_getNthChild_00002014
