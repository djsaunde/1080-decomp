nonmatching SndHandle_setContactSurface, 0x8

glabel SndHandle_setContactSurface
    /* DD1500 00000A94 03E00008 */  jr         $ra
    /* DD1504 00000A98 AC850008 */   sw        $a1, 0x8($a0)
endlabel SndHandle_setContactSurface
