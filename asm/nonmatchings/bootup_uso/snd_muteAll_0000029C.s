nonmatching snd_muteAll_0000029C, 0x20

glabel snd_muteAll_0000029C
    /* DD0D08 0000029C 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD0D0C 000002A0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD0D10 000002A4 0C000000 */  jal        func_00000000
    /* DD0D14 000002A8 00000000 */   nop
    /* DD0D18 000002AC 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD0D1C 000002B0 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD0D20 000002B4 03E00008 */  jr         $ra
    /* DD0D24 000002B8 00000000 */   nop
endlabel snd_muteAll_0000029C
