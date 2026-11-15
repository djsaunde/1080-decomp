nonmatching Node_setFlag4_0000F66C, 0x58

glabel Node_setFlag4_0000F66C
    /* DE00D8 0000F66C 27BDFFF0 */  addiu      $sp, $sp, -0x10
    /* DE00DC 0000F670 24850018 */  addiu      $a1, $a0, 0x18
    /* DE00E0 0000F674 00A03025 */  or         $a2, $a1, $zero
    /* DE00E4 0000F678 24070001 */  addiu      $a3, $zero, 0x1
    /* DE00E8 0000F67C 10E00007 */  beqz       $a3, .L0000F69C
    /* DE00EC 0000F680 00000000 */   nop
    /* DE00F0 0000F684 00C04025 */  or         $t0, $a2, $zero
    /* DE00F4 0000F688 8D0E0000 */  lw         $t6, 0x0($t0)
    /* DE00F8 0000F68C 35CF0004 */  ori        $t7, $t6, 0x4
    /* DE00FC 0000F690 AD0F0000 */  sw         $t7, 0x0($t0)
    /* DE0100 0000F694 10000006 */  b          .L0000F6B0
    /* DE0104 0000F698 00000000 */   nop
  .L0000F69C:
    /* DE0108 0000F69C 00C04025 */  or         $t0, $a2, $zero
    /* DE010C 0000F6A0 8D180000 */  lw         $t8, 0x0($t0)
    /* DE0110 0000F6A4 2401FFFB */  addiu      $at, $zero, -0x5
    /* DE0114 0000F6A8 0301C824 */  and        $t9, $t8, $at
    /* DE0118 0000F6AC AD190000 */  sw         $t9, 0x0($t0)
  .L0000F6B0:
    /* DE011C 0000F6B0 10000001 */  b          .L0000F6B8
    /* DE0120 0000F6B4 00000000 */   nop
  .L0000F6B8:
    /* DE0124 0000F6B8 27BD0010 */  addiu      $sp, $sp, 0x10
    /* DE0128 0000F6BC 03E00008 */  jr         $ra
    /* DE012C 0000F6C0 00000000 */   nop
endlabel Node_setFlag4_0000F66C
