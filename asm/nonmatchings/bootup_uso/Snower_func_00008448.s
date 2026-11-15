nonmatching Snower_func_00008448, 0x2C

glabel Snower_func_00008448
    /* DD8EB4 00008448 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD8EB8 0000844C AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD8EBC 00008450 AFA40018 */  sw         $a0, 0x18($sp)
    /* DD8EC0 00008454 0C000000 */  jal        func_00000000
    /* DD8EC4 00008458 2484003C */   addiu     $a0, $a0, 0x3C
    /* DD8EC8 0000845C 0C000000 */  jal        func_00000000
    /* DD8ECC 00008460 8FA40018 */   lw        $a0, 0x18($sp)
    /* DD8ED0 00008464 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD8ED4 00008468 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD8ED8 0000846C 03E00008 */  jr         $ra
    /* DD8EDC 00008470 00000000 */   nop
endlabel Snower_func_00008448
