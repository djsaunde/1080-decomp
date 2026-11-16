nonmatching snd_setSoundOutputMode_000001FC, 0x20

glabel snd_setSoundOutputMode_000001FC
    /* DD0C68 000001FC 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD0C6C 00000200 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD0C70 00000204 0C000000 */  jal        func_00000000
    /* DD0C74 00000208 00000000 */   nop
    /* DD0C78 0000020C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD0C7C 00000210 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD0C80 00000214 03E00008 */  jr         $ra
    /* DD0C84 00000218 00000000 */   nop
endlabel snd_setSoundOutputMode_000001FC
