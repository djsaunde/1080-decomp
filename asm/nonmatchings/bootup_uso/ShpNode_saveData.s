nonmatching ShpNode_saveData, 0x20

glabel ShpNode_saveData
    /* DDF05C 0000E5F0 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DDF060 0000E5F4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF064 0000E5F8 0C000000 */  jal        func_00000000
    /* DDF068 0000E5FC 24840108 */   addiu     $a0, $a0, 0x108
    /* DDF06C 0000E600 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF070 0000E604 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DDF074 0000E608 03E00008 */  jr         $ra
    /* DDF078 0000E60C 00000000 */   nop
endlabel ShpNode_saveData
