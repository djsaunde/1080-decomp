nonmatching WorldCourse_ctor_00001DB8, 0x50

glabel WorldCourse_ctor_00001DB8
    /* DD2824 00001DB8 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD2828 00001DBC AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD282C 00001DC0 AFA5001C */  sw         $a1, 0x1C($sp)
    /* DD2830 00001DC4 14800005 */  bnez       $a0, .L00001DDC
    /* DD2834 00001DC8 AFA60020 */   sw        $a2, 0x20($sp)
    /* DD2838 00001DCC 0C000000 */  jal        func_00000000
    /* DD283C 00001DD0 24040048 */   addiu     $a0, $zero, 0x48
    /* DD2840 00001DD4 10400007 */  beqz       $v0, .L00001DF4
    /* DD2844 00001DD8 00402025 */   or        $a0, $v0, $zero
  .L00001DDC:
    /* DD2848 00001DDC 8FA5001C */  lw         $a1, 0x1C($sp)
    /* DD284C 00001DE0 0C000000 */  jal        func_00000000
    /* DD2850 00001DE4 AFA40018 */   sw        $a0, 0x18($sp)
    /* DD2854 00001DE8 8FA40018 */  lw         $a0, 0x18($sp)
    /* DD2858 00001DEC 8FAE0020 */  lw         $t6, 0x20($sp)
    /* DD285C 00001DF0 AC8E0040 */  sw         $t6, 0x40($a0)
  .L00001DF4:
    /* DD2860 00001DF4 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD2864 00001DF8 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD2868 00001DFC 00801025 */  or         $v0, $a0, $zero
    /* DD286C 00001E00 03E00008 */  jr         $ra
    /* DD2870 00001E04 00000000 */   nop
endlabel WorldCourse_ctor_00001DB8
