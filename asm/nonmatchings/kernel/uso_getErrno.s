
nonmatching uso_getErrno, 0x10

glabel uso_getErrno
    /* 22AC 800012AC 3C028001 */  lui        $v0, %hi(gUsoErrno)
    /* 22B0 800012B0 AFA40000 */  sw         $a0, 0x0($sp)
    /* 22B4 800012B4 03E00008 */  jr         $ra
    /* 22B8 800012B8 8C423004 */   lw        $v0, %lo(gUsoErrno)($v0)
endlabel uso_getErrno
