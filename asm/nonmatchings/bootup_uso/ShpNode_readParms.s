nonmatching ShpNode_readParms, 0x20

glabel ShpNode_readParms
    /* DDF07C 0000E610 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DDF080 0000E614 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF084 0000E618 0C000000 */  jal        func_00000000
    /* DDF088 0000E61C 24840108 */   addiu     $a0, $a0, 0x108
    /* DDF08C 0000E620 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF090 0000E624 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DDF094 0000E628 03E00008 */  jr         $ra
    /* DDF098 0000E62C 00000000 */   nop
endlabel ShpNode_readParms
