nonmatching snd_sendEvent, 0x20

glabel snd_sendEvent
    /* DD0D68 000002FC 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD0D6C 00000300 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD0D70 00000304 0C000000 */  jal        func_00000000
    /* DD0D74 00000308 00000000 */   nop
    /* DD0D78 0000030C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD0D7C 00000310 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD0D80 00000314 03E00008 */  jr         $ra
    /* DD0D84 00000318 00000000 */   nop
endlabel snd_sendEvent
