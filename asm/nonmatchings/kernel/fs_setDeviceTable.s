
nonmatching fs_setDeviceTable, 0xC

glabel fs_setDeviceTable
    /* 1474 80000474 3C018001 */  lui        $at, %hi(gFsDeviceTable)
    /* 1478 80000478 03E00008 */  jr         $ra
    /* 147C 8000047C AC24A2E0 */   sw        $a0, %lo(gFsDeviceTable)($at)
endlabel fs_setDeviceTable
