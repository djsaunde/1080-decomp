nonmatching Node_setFlag8_0000F5BC, 0x58

glabel Node_setFlag8_0000F5BC
    /* DE0028 0000F5BC 27BDFFF0 */  addiu      $sp, $sp, -0x10
    /* DE002C 0000F5C0 24850018 */  addiu      $a1, $a0, 0x18
    /* DE0030 0000F5C4 00A03025 */  or         $a2, $a1, $zero
    /* DE0034 0000F5C8 24070001 */  addiu      $a3, $zero, 0x1
    /* DE0038 0000F5CC 10E00007 */  beqz       $a3, .L0000F5EC
    /* DE003C 0000F5D0 00000000 */   nop
    /* DE0040 0000F5D4 00C04025 */  or         $t0, $a2, $zero
    /* DE0044 0000F5D8 8D0E0000 */  lw         $t6, 0x0($t0)
    /* DE0048 0000F5DC 35CF0008 */  ori        $t7, $t6, 0x8
    /* DE004C 0000F5E0 AD0F0000 */  sw         $t7, 0x0($t0)
    /* DE0050 0000F5E4 10000006 */  b          .L0000F600
    /* DE0054 0000F5E8 00000000 */   nop
  .L0000F5EC:
    /* DE0058 0000F5EC 00C04025 */  or         $t0, $a2, $zero
    /* DE005C 0000F5F0 8D180000 */  lw         $t8, 0x0($t0)
    /* DE0060 0000F5F4 2401FFF7 */  addiu      $at, $zero, -0x9
    /* DE0064 0000F5F8 0301C824 */  and        $t9, $t8, $at
    /* DE0068 0000F5FC AD190000 */  sw         $t9, 0x0($t0)
  .L0000F600:
    /* DE006C 0000F600 10000001 */  b          .L0000F608
    /* DE0070 0000F604 00000000 */   nop
  .L0000F608:
    /* DE0074 0000F608 27BD0010 */  addiu      $sp, $sp, 0x10
    /* DE0078 0000F60C 03E00008 */  jr         $ra
    /* DE007C 0000F610 00000000 */   nop
endlabel Node_setFlag8_0000F5BC
