
/* Handwritten function */
nonmatching __osGetSR, 0xC

glabel __osGetSR
    /* 3DE0 80002DE0 40026000 */  mfc0       $v0, $12 /* handwritten instruction */
    /* 3DE4 80002DE4 03E00008 */  jr         $ra
    /* 3DE8 80002DE8 00000000 */   nop
endlabel __osGetSR
    /* 3DEC 80002DEC 00000000 */  nop
