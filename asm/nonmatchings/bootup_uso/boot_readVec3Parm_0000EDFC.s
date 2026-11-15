nonmatching boot_readVec3Parm_0000EDFC, 0x30

glabel boot_readVec3Parm_0000EDFC
    /* DDF868 0000EDFC 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDF86C 0000EE00 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF870 0000EE04 AFA40020 */  sw         $a0, 0x20($sp)
    /* DDF874 0000EE08 0C003B20 */  jal        stream_readS32_0000EC80
    /* DDF878 0000EE0C 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DDF87C 0000EE10 8FA40020 */  lw         $a0, 0x20($sp)
    /* DDF880 0000EE14 0C003B3E */  jal        stream_readVec3_0000ECF8
    /* DDF884 0000EE18 24840010 */   addiu     $a0, $a0, 0x10
    /* DDF888 0000EE1C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF88C 0000EE20 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDF890 0000EE24 03E00008 */  jr         $ra
    /* DDF894 0000EE28 00000000 */   nop
endlabel boot_readVec3Parm_0000EDFC
