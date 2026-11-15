nonmatching snd_setPlayer3FreqScaleFromStick, 0x20

glabel snd_setPlayer3FreqScaleFromStick
    /* DD0E44 000003D8 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD0E48 000003DC AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD0E4C 000003E0 0C000000 */  jal        func_00000000
    /* DD0E50 000003E4 00000000 */   nop
    /* DD0E54 000003E8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD0E58 000003EC 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD0E5C 000003F0 03E00008 */  jr         $ra
    /* DD0E60 000003F4 00000000 */   nop
endlabel snd_setPlayer3FreqScaleFromStick
