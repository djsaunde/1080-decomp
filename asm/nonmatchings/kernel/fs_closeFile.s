
nonmatching fs_closeFile, 0x8

glabel fs_closeFile
    /* 2494 80001494 03E00008 */  jr         $ra
    /* 2498 80001498 AFA40000 */   sw        $a0, 0x0($sp)
endlabel fs_closeFile
