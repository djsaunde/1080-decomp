nonmatching Node_clearFlag8_0000F564, 0x58

glabel Node_clearFlag8_0000F564
    /* DDFFD0 0000F564 27BDFFF0 */  addiu      $sp, $sp, -0x10
    /* DDFFD4 0000F568 24850018 */  addiu      $a1, $a0, 0x18
    /* DDFFD8 0000F56C 00A03025 */  or         $a2, $a1, $zero
    /* DDFFDC 0000F570 00003825 */  or         $a3, $zero, $zero
    /* DDFFE0 0000F574 10E00007 */  beqz       $a3, .L0000F594
    /* DDFFE4 0000F578 00000000 */   nop
    /* DDFFE8 0000F57C 00C04025 */  or         $t0, $a2, $zero
    /* DDFFEC 0000F580 8D0E0000 */  lw         $t6, 0x0($t0)
    /* DDFFF0 0000F584 35CF0008 */  ori        $t7, $t6, 0x8
    /* DDFFF4 0000F588 AD0F0000 */  sw         $t7, 0x0($t0)
    /* DDFFF8 0000F58C 10000006 */  b          .L0000F5A8
    /* DDFFFC 0000F590 00000000 */   nop
  .L0000F594:
    /* DE0000 0000F594 00C04025 */  or         $t0, $a2, $zero
    /* DE0004 0000F598 8D180000 */  lw         $t8, 0x0($t0)
    /* DE0008 0000F59C 2401FFF7 */  addiu      $at, $zero, -0x9
    /* DE000C 0000F5A0 0301C824 */  and        $t9, $t8, $at
    /* DE0010 0000F5A4 AD190000 */  sw         $t9, 0x0($t0)
  .L0000F5A8:
    /* DE0014 0000F5A8 10000001 */  b          .L0000F5B0
    /* DE0018 0000F5AC 00000000 */   nop
  .L0000F5B0:
    /* DE001C 0000F5B0 27BD0010 */  addiu      $sp, $sp, 0x10
    /* DE0020 0000F5B4 03E00008 */  jr         $ra
    /* DE0024 0000F5B8 00000000 */   nop
endlabel Node_clearFlag8_0000F564
