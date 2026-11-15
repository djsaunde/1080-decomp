nonmatching Node_clearFlag4_0000F614, 0x58

glabel Node_clearFlag4_0000F614
    /* DE0080 0000F614 27BDFFF0 */  addiu      $sp, $sp, -0x10
    /* DE0084 0000F618 24850018 */  addiu      $a1, $a0, 0x18
    /* DE0088 0000F61C 00A03025 */  or         $a2, $a1, $zero
    /* DE008C 0000F620 00003825 */  or         $a3, $zero, $zero
    /* DE0090 0000F624 10E00007 */  beqz       $a3, .L0000F644
    /* DE0094 0000F628 00000000 */   nop
    /* DE0098 0000F62C 00C04025 */  or         $t0, $a2, $zero
    /* DE009C 0000F630 8D0E0000 */  lw         $t6, 0x0($t0)
    /* DE00A0 0000F634 35CF0004 */  ori        $t7, $t6, 0x4
    /* DE00A4 0000F638 AD0F0000 */  sw         $t7, 0x0($t0)
    /* DE00A8 0000F63C 10000006 */  b          .L0000F658
    /* DE00AC 0000F640 00000000 */   nop
  .L0000F644:
    /* DE00B0 0000F644 00C04025 */  or         $t0, $a2, $zero
    /* DE00B4 0000F648 8D180000 */  lw         $t8, 0x0($t0)
    /* DE00B8 0000F64C 2401FFFB */  addiu      $at, $zero, -0x5
    /* DE00BC 0000F650 0301C824 */  and        $t9, $t8, $at
    /* DE00C0 0000F654 AD190000 */  sw         $t9, 0x0($t0)
  .L0000F658:
    /* DE00C4 0000F658 10000001 */  b          .L0000F660
    /* DE00C8 0000F65C 00000000 */   nop
  .L0000F660:
    /* DE00CC 0000F660 27BD0010 */  addiu      $sp, $sp, 0x10
    /* DE00D0 0000F664 03E00008 */  jr         $ra
    /* DE00D4 0000F668 00000000 */   nop
endlabel Node_clearFlag4_0000F614
