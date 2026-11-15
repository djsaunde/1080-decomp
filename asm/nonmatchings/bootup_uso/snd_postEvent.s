nonmatching snd_postEvent, 0x20

glabel snd_postEvent
    /* DD0D48 000002DC 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD0D4C 000002E0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD0D50 000002E4 0C000000 */  jal        func_00000000
    /* DD0D54 000002E8 00000000 */   nop
    /* DD0D58 000002EC 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD0D5C 000002F0 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD0D60 000002F4 03E00008 */  jr         $ra
    /* DD0D64 000002F8 00000000 */   nop
endlabel snd_postEvent
