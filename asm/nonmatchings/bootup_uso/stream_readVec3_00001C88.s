nonmatching stream_readVec3_00001C88, 0x70

glabel stream_readVec3_00001C88
    /* DD26F4 00001C88 27BDFFB8 */  addiu      $sp, $sp, -0x48
    /* DD26F8 00001C8C 00803825 */  or         $a3, $a0, $zero
    /* DD26FC 00001C90 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD2700 00001C94 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD2704 00001C98 24840000 */  addiu      $a0, $a0, 0x0
    /* DD2708 00001C9C AFA70048 */  sw         $a3, 0x48($sp)
    /* DD270C 00001CA0 27A50038 */  addiu      $a1, $sp, 0x38
    /* DD2710 00001CA4 0C000000 */  jal        func_00000000
    /* DD2714 00001CA8 2406000C */   addiu     $a2, $zero, 0xC
    /* DD2718 00001CAC 27AF0038 */  addiu      $t7, $sp, 0x38
    /* DD271C 00001CB0 8DF90000 */  lw         $t9, 0x0($t7)
    /* DD2720 00001CB4 8FA70048 */  lw         $a3, 0x48($sp)
    /* DD2724 00001CB8 27AE0024 */  addiu      $t6, $sp, 0x24
    /* DD2728 00001CBC ADD90000 */  sw         $t9, 0x0($t6)
    /* DD272C 00001CC0 8DF80004 */  lw         $t8, 0x4($t7)
    /* DD2730 00001CC4 ADD80004 */  sw         $t8, 0x4($t6)
    /* DD2734 00001CC8 8DF90008 */  lw         $t9, 0x8($t7)
    /* DD2738 00001CCC ADD90008 */  sw         $t9, 0x8($t6)
    /* DD273C 00001CD0 C7A40024 */  lwc1       $f4, 0x24($sp)
    /* DD2740 00001CD4 E4E40000 */  swc1       $f4, 0x0($a3)
    /* DD2744 00001CD8 C7A60028 */  lwc1       $f6, 0x28($sp)
    /* DD2748 00001CDC E4E60004 */  swc1       $f6, 0x4($a3)
    /* DD274C 00001CE0 C7A8002C */  lwc1       $f8, 0x2C($sp)
    /* DD2750 00001CE4 E4E80008 */  swc1       $f8, 0x8($a3)
    /* DD2754 00001CE8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD2758 00001CEC 27BD0048 */  addiu      $sp, $sp, 0x48
    /* DD275C 00001CF0 03E00008 */  jr         $ra
    /* DD2760 00001CF4 00000000 */   nop
endlabel stream_readVec3_00001C88
