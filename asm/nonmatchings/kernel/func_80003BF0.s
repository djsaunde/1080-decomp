/* __osDequeueThread (libultra thread.c, compiled C at -O1) */
/* split out of func_800031F0 2026-10-24 (agent-g run 158) */
nonmatching func_80003BF0, 0x40

glabel func_80003BF0
    /* 4BF0 80003BF0 00803025 */  or         $a2, $a0, $zero
    /* 4BF4 80003BF4 8CC70000 */  lw         $a3, 0x0($a2)
    /* 4BF8 80003BF8 27BDFFF8 */  addiu      $sp, $sp, -0x8
    /* 4BFC 80003BFC 10E0000A */  beqz       $a3, .L80003C28
    /* 4C00 80003C00 00000000 */   nop
  .L80003C04:
    /* 4C04 80003C04 14E50004 */  bne        $a3, $a1, .L80003C18
    /* 4C08 80003C08 00000000 */   nop
    /* 4C0C 80003C0C 8CAE0000 */  lw         $t6, 0x0($a1)
    /* 4C10 80003C10 10000005 */  b          .L80003C28
    /* 4C14 80003C14 ACCE0000 */   sw        $t6, 0x0($a2)
  .L80003C18:
    /* 4C18 80003C18 00E03025 */  or         $a2, $a3, $zero
    /* 4C1C 80003C1C 8CC70000 */  lw         $a3, 0x0($a2)
    /* 4C20 80003C20 14E0FFF8 */  bnez       $a3, .L80003C04
    /* 4C24 80003C24 00000000 */  nop
  .L80003C28:
    /* 4C28 80003C28 03E00008 */  jr         $ra
    /* 4C2C 80003C2C 27BD0008 */   addiu     $sp, $sp, 0x8
endlabel func_80003BF0
