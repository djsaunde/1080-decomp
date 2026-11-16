nonmatching ColourItem_create_00000C10, 0x90

glabel ColourItem_create_00000C10
    /* DD167C 00000C10 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DD1680 00000C14 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD1684 00000C18 AFA40028 */  sw         $a0, 0x28($sp)
    /* DD1688 00000C1C 0C000000 */  jal        func_00000000
    /* DD168C 00000C20 24040040 */   addiu     $a0, $zero, 0x40
    /* DD1690 00000C24 10400008 */  beqz       $v0, .L00000C48
    /* DD1694 00000C28 00402025 */   or        $a0, $v0, $zero
    /* DD1698 00000C2C 0C000000 */  jal        func_00000000
    /* DD169C 00000C30 AFA2001C */   sw        $v0, 0x1C($sp)
    /* DD16A0 00000C34 8FA4001C */  lw         $a0, 0x1C($sp)
    /* DD16A4 00000C38 3C0E0000 */  lui        $t6, (0x0 >> 16)
    /* DD16A8 00000C3C 25CE0000 */  addiu      $t6, $t6, 0x0
    /* DD16AC 00000C40 AC8E0028 */  sw         $t6, 0x28($a0)
    /* DD16B0 00000C44 AC80003C */  sw         $zero, 0x3C($a0)
  .L00000C48:
    /* DD16B4 00000C48 8FAF0028 */  lw         $t7, 0x28($sp)
    /* DD16B8 00000C4C 00801825 */  or         $v1, $a0, $zero
  alabel kRo_Fader_display_C50
    /* DD16BC 00000C50 24840010 */  addiu      $a0, $a0, 0x10
  alabel kRo_Fader_display_C54
    /* DD16C0 00000C54 8DE20040 */  lw         $v0, 0x40($t7)
  alabel kRo_Fader_display_C58
    /* DD16C4 00000C58 1040000C */  beqz       $v0, .L00000C8C
  alabel kRo_Fader_display_C5C
    /* DD16C8 00000C5C 00402825 */   or        $a1, $v0, $zero
  alabel kRo_Fader_display_C60
    /* DD16CC 00000C60 AFA30024 */  sw         $v1, 0x24($sp)
  alabel kRo_Fader_start_C64
    /* DD16D0 00000C64 0C000000 */  jal        func_00000000
    /* DD16D4 00000C68 AFA20020 */   sw        $v0, 0x20($sp)
    /* DD16D8 00000C6C 8FA50020 */  lw         $a1, 0x20($sp)
    /* DD16DC 00000C70 8FA30024 */  lw         $v1, 0x24($sp)
    /* DD16E0 00000C74 24190001 */  addiu      $t9, $zero, 0x1
    /* DD16E4 00000C78 8CB80014 */  lw         $t8, 0x14($a1)
    /* DD16E8 00000C7C 53000003 */  beql       $t8, $zero, .L00000C8C
    /* DD16EC 00000C80 ACA30014 */   sw        $v1, 0x14($a1)
    /* DD16F0 00000C84 ACB90004 */  sw         $t9, 0x4($a1)
    /* DD16F4 00000C88 ACA30014 */  sw         $v1, 0x14($a1)
  .L00000C8C:
    /* DD16F8 00000C8C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD16FC 00000C90 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DD1700 00000C94 00601025 */  or         $v0, $v1, $zero
    /* DD1704 00000C98 03E00008 */  jr         $ra
    /* DD1708 00000C9C 00000000 */   nop
endlabel ColourItem_create_00000C10
