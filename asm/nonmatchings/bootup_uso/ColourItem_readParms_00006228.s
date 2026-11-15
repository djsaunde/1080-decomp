nonmatching ColourItem_readParms_00006228, 0x2C

glabel ColourItem_readParms_00006228
    /* DD6C94 00006228 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD6C98 0000622C AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD6C9C 00006230 AFA40018 */  sw         $a0, 0x18($sp)
    /* DD6CA0 00006234 0C00140B */  jal        stream_readS32_0000502C
    /* DD6CA4 00006238 2484003C */   addiu     $a0, $a0, 0x3C
    /* DD6CA8 0000623C 0C000000 */  jal        func_00000000
    /* DD6CAC 00006240 8FA40018 */   lw        $a0, 0x18($sp)
    /* DD6CB0 00006244 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD6CB4 00006248 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD6CB8 0000624C 03E00008 */  jr         $ra
    /* DD6CBC 00006250 00000000 */   nop
endlabel ColourItem_readParms_00006228
