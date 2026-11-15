nonmatching SnowBoarder_toggleNudge, 0x1C

glabel SnowBoarder_toggleNudge
    /* DDD538 0000CACC 8C8F0A58 */  lw         $t7, 0xA58($a0)
    /* DDD53C 0000CAD0 3C0E000F */  lui        $t6, (0xF4240 >> 16)
    /* DDD540 0000CAD4 35CE4240 */  ori        $t6, $t6, (0xF4240 & 0xFFFF)
    /* DDD544 0000CAD8 39F80100 */  xori       $t8, $t7, 0x100
    /* DDD548 0000CADC AC8E0A14 */  sw         $t6, 0xA14($a0)
    /* DDD54C 0000CAE0 03E00008 */  jr         $ra
    /* DDD550 0000CAE4 AC980A58 */   sw        $t8, 0xA58($a0)
endlabel SnowBoarder_toggleNudge
