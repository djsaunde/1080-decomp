nonmatching PointsPopup_setScale, 0x14

glabel PointsPopup_setScale
    /* DE4F4C 000144E0 44856000 */  mtc1       $a1, $f12
    /* DE4F50 000144E4 44867000 */  mtc1       $a2, $f14
    /* DE4F54 000144E8 E48C0088 */  swc1       $f12, 0x88($a0)
    /* DE4F58 000144EC 03E00008 */  jr         $ra
    /* DE4F5C 000144F0 E48E008C */   swc1      $f14, 0x8C($a0)
endlabel PointsPopup_setScale
