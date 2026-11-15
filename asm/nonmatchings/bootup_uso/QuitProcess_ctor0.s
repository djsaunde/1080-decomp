nonmatching QuitProcess_ctor0, 0x124

glabel QuitProcess_ctor0
    /* DE0694 0000FC28 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DE0698 0000FC2C AFBF001C */  sw         $ra, 0x1C($sp)
    /* DE069C 0000FC30 AFA40028 */  sw         $a0, 0x28($sp)
    /* DE06A0 0000FC34 AFB10018 */  sw         $s1, 0x18($sp)
    /* DE06A4 0000FC38 AFB00014 */  sw         $s0, 0x14($sp)
    /* DE06A8 0000FC3C 8FAE0028 */  lw         $t6, 0x28($sp)
    /* DE06AC 0000FC40 15C00008 */  bnez       $t6, .L0000FC64
    /* DE06B0 0000FC44 00000000 */   nop
    /* DE06B4 0000FC48 24040050 */  addiu      $a0, $zero, 0x50
    /* DE06B8 0000FC4C 0C000000 */  jal        func_00000000
    /* DE06BC 0000FC50 00000000 */   nop
    /* DE06C0 0000FC54 AFA20028 */  sw         $v0, 0x28($sp)
    /* DE06C4 0000FC58 8FAF0028 */  lw         $t7, 0x28($sp)
    /* DE06C8 0000FC5C 11E00032 */  beqz       $t7, .L0000FD28
    /* DE06CC 0000FC60 00000000 */   nop
  .L0000FC64:
    /* DE06D0 0000FC64 8FB00028 */  lw         $s0, 0x28($sp)
    /* DE06D4 0000FC68 16000007 */  bnez       $s0, .L0000FC88
    /* DE06D8 0000FC6C 00000000 */   nop
    /* DE06DC 0000FC70 24040050 */  addiu      $a0, $zero, 0x50
    /* DE06E0 0000FC74 0C000000 */  jal        func_00000000
    /* DE06E4 0000FC78 00000000 */   nop
    /* DE06E8 0000FC7C 00408025 */  or         $s0, $v0, $zero
    /* DE06EC 0000FC80 12000015 */  beqz       $s0, .L0000FCD8
    /* DE06F0 0000FC84 00000000 */   nop
  .L0000FC88:
    /* DE06F4 0000FC88 02008825 */  or         $s1, $s0, $zero
    /* DE06F8 0000FC8C 16200007 */  bnez       $s1, .L0000FCAC
    /* DE06FC 0000FC90 00000000 */   nop
    /* DE0700 0000FC94 2404002C */  addiu      $a0, $zero, 0x2C
    /* DE0704 0000FC98 0C000000 */  jal        func_00000000
    /* DE0708 0000FC9C 00000000 */   nop
    /* DE070C 0000FCA0 00408825 */  or         $s1, $v0, $zero
    /* DE0710 0000FCA4 12200009 */  beqz       $s1, .L0000FCCC
    /* DE0714 0000FCA8 00000000 */   nop
  .L0000FCAC:
    /* DE0718 0000FCAC 02202025 */  or         $a0, $s1, $zero
    /* DE071C 0000FCB0 3C050001 */  lui        $a1, %hi(sStr_Node_C58C)
    /* DE0720 0000FCB4 24A5C58C */  addiu      $a1, $a1, %lo(sStr_Node_C58C)
    /* DE0724 0000FCB8 0C000000 */  jal        func_00000000
    /* DE0728 0000FCBC 00000000 */   nop
    /* DE072C 0000FCC0 3C180000 */  lui        $t8, (0x0 >> 16)
    /* DE0730 0000FCC4 27180000 */  addiu      $t8, $t8, 0x0
    /* DE0734 0000FCC8 AE380028 */  sw         $t8, 0x28($s1)
  .L0000FCCC:
    /* DE0738 0000FCCC 3C190000 */  lui        $t9, (0x0 >> 16)
    /* DE073C 0000FCD0 27390000 */  addiu      $t9, $t9, 0x0
    /* DE0740 0000FCD4 AE190028 */  sw         $t9, 0x28($s0)
  .L0000FCD8:
    /* DE0744 0000FCD8 3C080000 */  lui        $t0, (0x0 >> 16)
    /* DE0748 0000FCDC 25080000 */  addiu      $t0, $t0, 0x0
    /* DE074C 0000FCE0 8FA90028 */  lw         $t1, 0x28($sp)
    /* DE0750 0000FCE4 AD280028 */  sw         $t0, 0x28($t1)
    /* DE0754 0000FCE8 8FB00028 */  lw         $s0, 0x28($sp)
    /* DE0758 0000FCEC 3C110001 */  lui        $s1, %hi(sStr_Quit_Process_C594)
    /* DE075C 0000FCF0 2631C594 */  addiu      $s1, $s1, %lo(sStr_Quit_Process_C594)
    /* DE0760 0000FCF4 AE11000C */  sw         $s1, 0xC($s0)
    /* DE0764 0000FCF8 8FA40028 */  lw         $a0, 0x28($sp)
    /* DE0768 0000FCFC 00002825 */  or         $a1, $zero, $zero
    /* DE076C 0000FD00 0C000000 */  jal        func_00000000
    /* DE0770 0000FD04 00000000 */   nop
    /* DE0774 0000FD08 3C010000 */  lui        $at, (0x0 >> 16)
    /* DE0778 0000FD0C AC220000 */  sw         $v0, 0x0($at)
    /* DE077C 0000FD10 8FB00028 */  lw         $s0, 0x28($sp)
    /* DE0780 0000FD14 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DE0784 0000FD18 8C840000 */  lw         $a0, 0x0($a0)
    /* DE0788 0000FD1C 02002825 */  or         $a1, $s0, $zero
    /* DE078C 0000FD20 0C000000 */  jal        func_00000000
    /* DE0790 0000FD24 00000000 */   nop
  .L0000FD28:
    /* DE0794 0000FD28 8FA20028 */  lw         $v0, 0x28($sp)
    /* DE0798 0000FD2C 10000001 */  b          .L0000FD34
    /* DE079C 0000FD30 00000000 */   nop
  .L0000FD34:
    /* DE07A0 0000FD34 8FB00014 */  lw         $s0, 0x14($sp)
    /* DE07A4 0000FD38 8FB10018 */  lw         $s1, 0x18($sp)
    /* DE07A8 0000FD3C 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DE07AC 0000FD40 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DE07B0 0000FD44 03E00008 */  jr         $ra
    /* DE07B4 0000FD48 00000000 */   nop
endlabel QuitProcess_ctor0
