nonmatching snd_soundObjSetByte1D_00000B14, 0x18

glabel snd_soundObjSetByte1D_00000B14
    /* DD1580 00000B14 8C820000 */  lw         $v0, 0x0($a0)
    /* DD1584 00000B18 10400002 */  beqz       $v0, .L00000B24
    /* DD1588 00000B1C 00000000 */   nop
    /* DD158C 00000B20 A045001D */  sb         $a1, 0x1D($v0)
  .L00000B24:
    /* DD1590 00000B24 03E00008 */  jr         $ra
    /* DD1594 00000B28 00000000 */   nop
endlabel snd_soundObjSetByte1D_00000B14
