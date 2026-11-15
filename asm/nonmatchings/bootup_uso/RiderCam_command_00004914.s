nonmatching RiderCam_command_00004914, 0x34

glabel RiderCam_command_00004914
    /* DD5380 00004914 C4C40030 */  lwc1       $f4, 0x30($a2)
    /* DD5384 00004918 3C0142C8 */  lui        $at, (0x42C80000 >> 16)
    /* DD5388 0000491C 44818000 */  mtc1       $at, $f16
    /* DD538C 00004920 E4840060 */  swc1       $f4, 0x60($a0)
    /* DD5390 00004924 C4C60034 */  lwc1       $f6, 0x34($a2)
    /* DD5394 00004928 E4860064 */  swc1       $f6, 0x64($a0)
    /* DD5398 0000492C C48A0064 */  lwc1       $f10, 0x64($a0)
    /* DD539C 00004930 C4C80038 */  lwc1       $f8, 0x38($a2)
    /* DD53A0 00004934 AC850158 */  sw         $a1, 0x158($a0)
    /* DD53A4 00004938 46105480 */  add.s      $f18, $f10, $f16
    /* DD53A8 0000493C E4880068 */  swc1       $f8, 0x68($a0)
    /* DD53AC 00004940 03E00008 */  jr         $ra
    /* DD53B0 00004944 E4920064 */   swc1      $f18, 0x64($a0)
endlabel RiderCam_command_00004914
