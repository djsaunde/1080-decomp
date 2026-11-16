nonmatching snd_startPlayer2Seq_0000027C, 0x20

glabel snd_startPlayer2Seq_0000027C
    /* DD0CE8 0000027C 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD0CEC 00000280 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD0CF0 00000284 0C000000 */  jal        func_00000000
    /* DD0CF4 00000288 00000000 */   nop
    /* DD0CF8 0000028C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD0CFC 00000290 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD0D00 00000294 03E00008 */  jr         $ra
    /* DD0D04 00000298 00000000 */   nop
endlabel snd_startPlayer2Seq_0000027C
