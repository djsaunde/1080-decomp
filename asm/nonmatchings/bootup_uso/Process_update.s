nonmatching Process_update, 0xA8

glabel Process_update
    /* DE0130 0000F6C4 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DE0134 0000F6C8 AFBF001C */  sw         $ra, 0x1C($sp)
    /* DE0138 0000F6CC AFA40028 */  sw         $a0, 0x28($sp)
    /* DE013C 0000F6D0 AFB00018 */  sw         $s0, 0x18($sp)
    /* DE0140 0000F6D4 8FAE0028 */  lw         $t6, 0x28($sp)
    /* DE0144 0000F6D8 8DCF002C */  lw         $t7, 0x2C($t6)
    /* DE0148 0000F6DC 15E00008 */  bnez       $t7, .L0000F700
    /* DE014C 0000F6E0 00000000 */   nop
    /* DE0150 0000F6E4 8FB80028 */  lw         $t8, 0x28($sp)
    /* DE0154 0000F6E8 8F100028 */  lw         $s0, 0x28($t8)
    /* DE0158 0000F6EC 86190068 */  lh         $t9, 0x68($s0)
    /* DE015C 0000F6F0 03382021 */  addu       $a0, $t9, $t8
    /* DE0160 0000F6F4 8E19006C */  lw         $t9, 0x6C($s0)
    /* DE0164 0000F6F8 0320F809 */  jalr       $t9
    /* DE0168 0000F6FC 00000000 */   nop
  .L0000F700:
    /* DE016C 0000F700 8FA40028 */  lw         $a0, 0x28($sp)
    /* DE0170 0000F704 0C000000 */  jal        func_00000000
    /* DE0174 0000F708 00000000 */   nop
    /* DE0178 0000F70C 8FA80028 */  lw         $t0, 0x28($sp)
    /* DE017C 0000F710 8D09002C */  lw         $t1, 0x2C($t0)
    /* DE0180 0000F714 1120000E */  beqz       $t1, .L0000F750
    /* DE0184 0000F718 00000000 */   nop
    /* DE0188 0000F71C 3C040000 */  lui        $a0, %hi(func_00000188 + 0x8)
    /* DE018C 0000F720 8C840190 */  lw         $a0, %lo(func_00000188 + 0x8)($a0)
    /* DE0190 0000F724 0C000000 */  jal        func_00000000
    /* DE0194 0000F728 00000000 */   nop
    /* DE0198 0000F72C 10400008 */  beqz       $v0, .L0000F750
    /* DE019C 0000F730 00000000 */   nop
    /* DE01A0 0000F734 8FAA0028 */  lw         $t2, 0x28($sp)
    /* DE01A4 0000F738 8D500028 */  lw         $s0, 0x28($t2)
    /* DE01A8 0000F73C 860B0060 */  lh         $t3, 0x60($s0)
    /* DE01AC 0000F740 016A2021 */  addu       $a0, $t3, $t2
    /* DE01B0 0000F744 8E190064 */  lw         $t9, 0x64($s0)
    /* DE01B4 0000F748 0320F809 */  jalr       $t9
    /* DE01B8 0000F74C 00000000 */   nop
  .L0000F750:
    /* DE01BC 0000F750 10000001 */  b          .L0000F758
    /* DE01C0 0000F754 00000000 */   nop
  .L0000F758:
    /* DE01C4 0000F758 8FB00018 */  lw         $s0, 0x18($sp)
    /* DE01C8 0000F75C 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DE01CC 0000F760 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DE01D0 0000F764 03E00008 */  jr         $ra
    /* DE01D4 0000F768 00000000 */   nop
endlabel Process_update
