
nonmatching __osGetActiveQueue, 0xC

glabel __osGetActiveQueue
    /* A830 80009830 3C028001 */  lui        $v0, %hi(__osActiveQueue)
    /* A834 80009834 03E00008 */  jr         $ra
    /* A838 80009838 8C42A41C */   lw        $v0, %lo(__osActiveQueue)($v0)
endlabel __osGetActiveQueue
    /* A83C 8000983C 00000000 */  nop
