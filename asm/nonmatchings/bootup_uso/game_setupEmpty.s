nonmatching game_setupEmpty, 0xC

glabel game_setupEmpty
    /* DDF82C 0000EDC0 AFA40000 */  sw         $a0, 0x0($sp)
    /* DDF830 0000EDC4 03E00008 */  jr         $ra
    /* DDF834 0000EDC8 AFA50004 */   sw        $a1, 0x4($sp)
endlabel game_setupEmpty
