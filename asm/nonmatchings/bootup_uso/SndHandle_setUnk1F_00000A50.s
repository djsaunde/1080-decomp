nonmatching SndHandle_setUnk1F_00000A50, 0x18

glabel SndHandle_setUnk1F_00000A50
    /* DD14BC 00000A50 8C820000 */  lw         $v0, 0x0($a0)
    /* DD14C0 00000A54 10400002 */  beqz       $v0, .L00000A60
    /* DD14C4 00000A58 00000000 */   nop
    /* DD14C8 00000A5C A045001F */  sb         $a1, 0x1F($v0)
  .L00000A60:
    /* DD14CC 00000A60 03E00008 */  jr         $ra
    /* DD14D0 00000A64 00000000 */   nop
endlabel SndHandle_setUnk1F_00000A50
