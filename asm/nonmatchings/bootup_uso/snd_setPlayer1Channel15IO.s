nonmatching snd_setPlayer1Channel15IO, 0x20

glabel snd_setPlayer1Channel15IO
    /* DD0E24 000003B8 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD0E28 000003BC AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD0E2C 000003C0 0C000000 */  jal        func_00000000
    /* DD0E30 000003C4 00000000 */   nop
    /* DD0E34 000003C8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD0E38 000003CC 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD0E3C 000003D0 03E00008 */  jr         $ra
    /* DD0E40 000003D4 00000000 */   nop
endlabel snd_setPlayer1Channel15IO
