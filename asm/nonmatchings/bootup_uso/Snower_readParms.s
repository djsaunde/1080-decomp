nonmatching Snower_readParms, 0x2C

glabel Snower_readParms
    /* DD8EE0 00008474 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD8EE4 00008478 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD8EE8 0000847C AFA40018 */  sw         $a0, 0x18($sp)
    /* DD8EEC 00008480 0C000000 */  jal        func_00000000
    /* DD8EF0 00008484 2484003C */   addiu     $a0, $a0, 0x3C
    /* DD8EF4 00008488 0C000000 */  jal        func_00000000
    /* DD8EF8 0000848C 8FA40018 */   lw        $a0, 0x18($sp)
    /* DD8EFC 00008490 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD8F00 00008494 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD8F04 00008498 03E00008 */  jr         $ra
    /* DD8F08 0000849C 00000000 */   nop
endlabel Snower_readParms
