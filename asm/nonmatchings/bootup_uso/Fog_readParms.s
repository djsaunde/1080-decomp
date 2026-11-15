nonmatching Fog_readParms, 0x38

glabel Fog_readParms
    /* DD8B58 000080EC 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD8B5C 000080F0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD8B60 000080F4 AFA40018 */  sw         $a0, 0x18($sp)
    /* DD8B64 000080F8 0C000000 */  jal        func_00000000
    /* DD8B68 000080FC 2484003C */   addiu     $a0, $a0, 0x3C
    /* DD8B6C 00008100 8FA40018 */  lw         $a0, 0x18($sp)
    /* DD8B70 00008104 0C000000 */  jal        func_00000000
    /* DD8B74 00008108 24840074 */   addiu     $a0, $a0, 0x74
    /* DD8B78 0000810C 0C000000 */  jal        func_00000000
    /* DD8B7C 00008110 8FA40018 */   lw        $a0, 0x18($sp)
    /* DD8B80 00008114 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD8B84 00008118 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD8B88 0000811C 03E00008 */  jr         $ra
    /* DD8B8C 00008120 00000000 */   nop
endlabel Fog_readParms
