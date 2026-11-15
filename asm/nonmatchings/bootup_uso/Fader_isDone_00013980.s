nonmatching Fader_isDone_00013980, 0x30

glabel Fader_isDone_00013980
    /* DE43EC 00013980 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DE43F0 00013984 44812000 */  mtc1       $at, $f4
    /* DE43F4 00013988 C4860048 */  lwc1       $f6, 0x48($a0)
    /* DE43F8 0001398C 00001025 */  or         $v0, $zero, $zero
    /* DE43FC 00013990 46062032 */  c.eq.s     $f4, $f6
    /* DE4400 00013994 00000000 */  nop
    /* DE4404 00013998 45000003 */  bc1f       .L000139A8
    /* DE4408 0001399C 00000000 */   nop
    /* DE440C 000139A0 03E00008 */  jr         $ra
    /* DE4410 000139A4 24020001 */   addiu     $v0, $zero, 0x1
  .L000139A8:
    /* DE4414 000139A8 03E00008 */  jr         $ra
    /* DE4418 000139AC 00000000 */   nop
endlabel Fader_isDone_00013980
