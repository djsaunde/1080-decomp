nonmatching Fog_func_00008078, 0x3C

glabel Fog_func_00008078
    /* DD8AE4 00008078 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD8AE8 0000807C AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD8AEC 00008080 AFA40018 */  sw         $a0, 0x18($sp)
    /* DD8AF0 00008084 2484003C */  addiu      $a0, $a0, 0x3C
    /* DD8AF4 00008088 0C000000 */  jal        func_00000000
    /* DD8AF8 0000808C 00002825 */   or        $a1, $zero, $zero
    /* DD8AFC 00008090 8FA40018 */  lw         $a0, 0x18($sp)
    /* DD8B00 00008094 0C000000 */  jal        func_00000000
    /* DD8B04 00008098 24840074 */   addiu     $a0, $a0, 0x74
    /* DD8B08 0000809C 0C000000 */  jal        func_00000000
    /* DD8B0C 000080A0 8FA40018 */   lw        $a0, 0x18($sp)
    /* DD8B10 000080A4 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD8B14 000080A8 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD8B18 000080AC 03E00008 */  jr         $ra
    /* DD8B1C 000080B0 00000000 */   nop
endlabel Fog_func_00008078
