nonmatching Tv1Tmpl_func_00007E70, 0x58

glabel Tv1Tmpl_func_00007E70
    /* DD88DC 00007E70 27BDFFE8 */  addiu      $sp, $sp, -0x18
  alabel D_00007E74
    /* DD88E0 00007E74 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD88E4 00007E78 3C010000 */  lui        $at, (0x0 >> 16)
  alabel D_00007E7C
    /* DD88E8 00007E7C AC200000 */  sw         $zero, 0x0($at)
    /* DD88EC 00007E80 0C000000 */  jal        func_00000000
  alabel D_00007E84
    /* DD88F0 00007E84 AFA40018 */   sw        $a0, 0x18($sp)
    /* DD88F4 00007E88 3C0E0000 */  lui        $t6, (0x0 >> 16)
    /* DD88F8 00007E8C 8DCE0000 */  lw         $t6, 0x0($t6)
    /* DD88FC 00007E90 8FA40018 */  lw         $a0, 0x18($sp)
    /* DD8900 00007E94 51C00009 */  beql       $t6, $zero, .L00007EBC
  alabel D_00007E98
    /* DD8904 00007E98 8FBF0014 */   lw        $ra, 0x14($sp)
    /* DD8908 00007E9C 8C820068 */  lw         $v0, 0x68($a0)
    /* DD890C 00007EA0 24010001 */  addiu      $at, $zero, 0x1
  alabel D_00007EA4
    /* DD8910 00007EA4 24420001 */  addiu      $v0, $v0, 0x1
  alabel D_00007EA8
    /* DD8914 00007EA8 14410003 */  bne        $v0, $at, .L00007EB8
  alabel D_00007EAC
    /* DD8918 00007EAC AC820068 */   sw        $v0, 0x68($a0)
  alabel D_00007EB0
    /* DD891C 00007EB0 0C000000 */  jal        func_00000000
    /* DD8920 00007EB4 00000000 */   nop
  .L00007EB8:
    /* DD8924 00007EB8 8FBF0014 */  lw         $ra, 0x14($sp)
  .L00007EBC:
    /* DD8928 00007EBC 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD892C 00007EC0 03E00008 */  jr         $ra
  alabel D_00007EC4
    /* DD8930 00007EC4 00000000 */   nop
endlabel Tv1Tmpl_func_00007E70
