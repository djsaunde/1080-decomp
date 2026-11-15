nonmatching snd_soundObjSetScaled_00000B2C, 0x24

glabel snd_soundObjSetScaled_00000B2C
    /* DD1598 00000B2C 8C820000 */  lw         $v0, 0x0($a0)
    /* DD159C 00000B30 3C010000 */  lui        $at, %hi(func_00000044 + 0x24)
    /* DD15A0 00000B34 44856000 */  mtc1       $a1, $f12
    /* DD15A4 00000B38 C4240068 */  lwc1       $f4, %lo(func_00000044 + 0x24)($at)
    /* DD15A8 00000B3C 10400002 */  beqz       $v0, .L00000B48
    /* DD15AC 00000B40 46046303 */   div.s     $f12, $f12, $f4
    /* DD15B0 00000B44 E44C0010 */  swc1       $f12, 0x10($v0)
  .L00000B48:
    /* DD15B4 00000B48 03E00008 */  jr         $ra
    /* DD15B8 00000B4C 00000000 */   nop
endlabel snd_soundObjSetScaled_00000B2C
