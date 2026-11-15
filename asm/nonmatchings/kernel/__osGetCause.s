
/* Handwritten function */
nonmatching __osGetCause, 0xC

glabel __osGetCause
    /* AAA0 80009AA0 40026800 */  mfc0       $v0, $13 /* handwritten instruction */
    /* AAA4 80009AA4 03E00008 */  jr         $ra
    /* AAA8 80009AA8 00000000 */   nop
endlabel __osGetCause
    /* AAAC 80009AAC 00000000 */  nop
