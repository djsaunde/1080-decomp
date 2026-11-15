nonmatching Fog_func_000080B4, 0x38

glabel Fog_func_000080B4
    /* DD8B20 000080B4 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD8B24 000080B8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD8B28 000080BC AFA40018 */  sw         $a0, 0x18($sp)
    /* DD8B2C 000080C0 0C000000 */  jal        func_00000000
    /* DD8B30 000080C4 2484003C */   addiu     $a0, $a0, 0x3C
    /* DD8B34 000080C8 8FA40018 */  lw         $a0, 0x18($sp)
    /* DD8B38 000080CC 0C000000 */  jal        func_00000000
    /* DD8B3C 000080D0 24840074 */   addiu     $a0, $a0, 0x74
    /* DD8B40 000080D4 0C000000 */  jal        func_00000000
    /* DD8B44 000080D8 8FA40018 */   lw        $a0, 0x18($sp)
    /* DD8B48 000080DC 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD8B4C 000080E0 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD8B50 000080E4 03E00008 */  jr         $ra
    /* DD8B54 000080E8 00000000 */   nop
endlabel Fog_func_000080B4
