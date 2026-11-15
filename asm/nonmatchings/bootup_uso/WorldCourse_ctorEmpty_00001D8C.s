nonmatching WorldCourse_ctorEmpty_00001D8C, 0x2C

glabel WorldCourse_ctorEmpty_00001D8C
    /* DD27F8 00001D8C 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD27FC 00001D90 14800004 */  bnez       $a0, .L00001DA4
    /* DD2800 00001D94 AFBF0014 */   sw        $ra, 0x14($sp)
    /* DD2804 00001D98 0C000000 */  jal        func_00000000
    /* DD2808 00001D9C 24040048 */   addiu     $a0, $zero, 0x48
    /* DD280C 00001DA0 00402025 */  or         $a0, $v0, $zero
  .L00001DA4:
    /* DD2810 00001DA4 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD2814 00001DA8 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD2818 00001DAC 00801025 */  or         $v0, $a0, $zero
    /* DD281C 00001DB0 03E00008 */  jr         $ra
    /* DD2820 00001DB4 00000000 */   nop
endlabel WorldCourse_ctorEmpty_00001D8C
