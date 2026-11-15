
nonmatching fs_skip, 0x10

glabel fs_skip
    /* 1688 80000688 8C8E0004 */  lw         $t6, 0x4($a0)
    /* 168C 8000068C 01C57821 */  addu       $t7, $t6, $a1
    /* 1690 80000690 03E00008 */  jr         $ra
    /* 1694 80000694 AC8F0004 */   sw        $t7, 0x4($a0)
endlabel fs_skip
