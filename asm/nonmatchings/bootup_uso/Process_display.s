nonmatching Process_display, 0x64

glabel Process_display
    /* DE01D8 0000F76C 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DE01DC 0000F770 AFBF001C */  sw         $ra, 0x1C($sp)
    /* DE01E0 0000F774 AFA40028 */  sw         $a0, 0x28($sp)
    /* DE01E4 0000F778 AFB00018 */  sw         $s0, 0x18($sp)
    /* DE01E8 0000F77C 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DE01EC 0000F780 24840000 */  addiu      $a0, $a0, 0x0
    /* DE01F0 0000F784 0C000000 */  jal        func_00000000
    /* DE01F4 0000F788 00000000 */   nop
    /* DE01F8 0000F78C 8FAE0028 */  lw         $t6, 0x28($sp)
    /* DE01FC 0000F790 8DD00028 */  lw         $s0, 0x28($t6)
    /* DE0200 0000F794 860F0070 */  lh         $t7, 0x70($s0)
    /* DE0204 0000F798 01EE2021 */  addu       $a0, $t7, $t6
    /* DE0208 0000F79C 8E190074 */  lw         $t9, 0x74($s0)
    /* DE020C 0000F7A0 0320F809 */  jalr       $t9
    /* DE0210 0000F7A4 00000000 */   nop
    /* DE0214 0000F7A8 8FA40028 */  lw         $a0, 0x28($sp)
    /* DE0218 0000F7AC 0C000000 */  jal        func_00000000
    /* DE021C 0000F7B0 00000000 */   nop
    /* DE0220 0000F7B4 10000001 */  b          .L0000F7BC
    /* DE0224 0000F7B8 00000000 */   nop
  .L0000F7BC:
    /* DE0228 0000F7BC 8FB00018 */  lw         $s0, 0x18($sp)
    /* DE022C 0000F7C0 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DE0230 0000F7C4 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DE0234 0000F7C8 03E00008 */  jr         $ra
    /* DE0238 0000F7CC 00000000 */   nop
endlabel Process_display
