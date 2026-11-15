nonmatching SnowBChild_readParms_0000E720, 0x20

glabel SnowBChild_readParms_0000E720
    /* DDF18C 0000E720 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DDF190 0000E724 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF194 0000E728 0C000000 */  jal        func_00000000
    /* DDF198 0000E72C 2484002C */   addiu     $a0, $a0, 0x2C
    /* DDF19C 0000E730 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF1A0 0000E734 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DDF1A4 0000E738 03E00008 */  jr         $ra
    /* DDF1A8 0000E73C 00000000 */   nop
endlabel SnowBChild_readParms_0000E720
