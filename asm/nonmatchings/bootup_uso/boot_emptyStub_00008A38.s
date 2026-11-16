nonmatching boot_emptyStub_00008A38, 0x8

glabel boot_emptyStub_00008A38
    /* DD94A4 00008A38 03E00008 */  jr         $ra
    /* DD94A8 00008A3C AFA40000 */   sw        $a0, 0x0($sp)
endlabel boot_emptyStub_00008A38
