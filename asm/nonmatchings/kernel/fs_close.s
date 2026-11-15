
nonmatching fs_close, 0xC

glabel fs_close
    /* 158C 8000058C AFA40000 */  sw         $a0, 0x0($sp)
    /* 1590 80000590 03E00008 */  jr         $ra
    /* 1594 80000594 00001025 */   or        $v0, $zero, $zero
endlabel fs_close
