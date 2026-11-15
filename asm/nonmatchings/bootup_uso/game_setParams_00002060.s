nonmatching game_setParams_00002060, 0x20

glabel game_setParams_00002060
    /* DD2ACC 00002060 AC850080 */  sw         $a1, 0x80($a0)
    /* DD2AD0 00002064 AC860084 */  sw         $a2, 0x84($a0)
    /* DD2AD4 00002068 8FAE0010 */  lw         $t6, 0x10($sp)
    /* DD2AD8 0000206C AC8E0088 */  sw         $t6, 0x88($a0)
    /* DD2ADC 00002070 8FAF0014 */  lw         $t7, 0x14($sp)
    /* DD2AE0 00002074 AC87008C */  sw         $a3, 0x8C($a0)
    /* DD2AE4 00002078 03E00008 */  jr         $ra
    /* DD2AE8 0000207C AC8F007C */   sw        $t7, 0x7C($a0)
endlabel game_setParams_00002060
