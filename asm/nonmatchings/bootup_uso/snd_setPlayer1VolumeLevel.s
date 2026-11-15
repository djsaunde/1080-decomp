nonmatching snd_setPlayer1VolumeLevel, 0x20

glabel snd_setPlayer1VolumeLevel
    /* DD0C88 0000021C 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD0C8C 00000220 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD0C90 00000224 0C000000 */  jal        func_00000000
    /* DD0C94 00000228 00000000 */   nop
    /* DD0C98 0000022C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD0C9C 00000230 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD0CA0 00000234 03E00008 */  jr         $ra
    /* DD0CA4 00000238 00000000 */   nop
endlabel snd_setPlayer1VolumeLevel
