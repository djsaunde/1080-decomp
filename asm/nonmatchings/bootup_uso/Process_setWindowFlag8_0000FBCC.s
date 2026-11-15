nonmatching Process_setWindowFlag8_0000FBCC, 0x5C

glabel Process_setWindowFlag8_0000FBCC
    /* DE0638 0000FBCC 27BDFFF0 */  addiu      $sp, $sp, -0x10
    /* DE063C 0000FBD0 8C850048 */  lw         $a1, 0x48($a0)
    /* DE0640 0000FBD4 24A50018 */  addiu      $a1, $a1, 0x18
    /* DE0644 0000FBD8 00A03025 */  or         $a2, $a1, $zero
    /* DE0648 0000FBDC 00003825 */  or         $a3, $zero, $zero
    /* DE064C 0000FBE0 10E00007 */  beqz       $a3, .L0000FC00
    /* DE0650 0000FBE4 00000000 */   nop
    /* DE0654 0000FBE8 00C04025 */  or         $t0, $a2, $zero
    /* DE0658 0000FBEC 8D0E0000 */  lw         $t6, 0x0($t0)
    /* DE065C 0000FBF0 35CF0008 */  ori        $t7, $t6, 0x8
    /* DE0660 0000FBF4 AD0F0000 */  sw         $t7, 0x0($t0)
    /* DE0664 0000FBF8 10000006 */  b          .L0000FC14
    /* DE0668 0000FBFC 00000000 */   nop
  .L0000FC00:
    /* DE066C 0000FC00 00C04025 */  or         $t0, $a2, $zero
    /* DE0670 0000FC04 8D180000 */  lw         $t8, 0x0($t0)
    /* DE0674 0000FC08 2401FFF7 */  addiu      $at, $zero, -0x9
    /* DE0678 0000FC0C 0301C824 */  and        $t9, $t8, $at
    /* DE067C 0000FC10 AD190000 */  sw         $t9, 0x0($t0)
  .L0000FC14:
    /* DE0680 0000FC14 10000001 */  b          .L0000FC1C
    /* DE0684 0000FC18 00000000 */   nop
  .L0000FC1C:
    /* DE0688 0000FC1C 27BD0010 */  addiu      $sp, $sp, 0x10
    /* DE068C 0000FC20 03E00008 */  jr         $ra
    /* DE0690 0000FC24 00000000 */   nop
endlabel Process_setWindowFlag8_0000FBCC
