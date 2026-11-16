
nonmatching osPiGetStatus, 0xC

glabel osPiGetStatus
    /* 7240 80006240 3C0EA460 */  lui        $t6, %hi(gPiStatusReg)
    /* 7244 80006244 03E00008 */  jr         $ra
    /* 7248 80006248 8DC20010 */   lw        $v0, %lo(gPiStatusReg)($t6)
endlabel osPiGetStatus
    /* 724C 8000624C 00000000 */  nop
