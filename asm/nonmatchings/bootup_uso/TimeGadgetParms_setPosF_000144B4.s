nonmatching TimeGadgetParms_setPosF_000144B4, 0x2C

glabel TimeGadgetParms_setPosF_000144B4
    /* DE4F20 000144B4 44852000 */  mtc1       $a1, $f4
    /* DE4F24 000144B8 44864000 */  mtc1       $a2, $f8
    /* DE4F28 000144BC 44876000 */  mtc1       $a3, $f12
    /* DE4F2C 000144C0 468021A0 */  cvt.s.w    $f6, $f4
    /* DE4F30 000144C4 E48C0088 */  swc1       $f12, 0x88($a0)
    /* DE4F34 000144C8 468042A0 */  cvt.s.w    $f10, $f8
    /* DE4F38 000144CC E4860044 */  swc1       $f6, 0x44($a0)
    /* DE4F3C 000144D0 E48A005C */  swc1       $f10, 0x5C($a0)
    /* DE4F40 000144D4 C7B00010 */  lwc1       $f16, 0x10($sp)
    /* DE4F44 000144D8 03E00008 */  jr         $ra
    /* DE4F48 000144DC E490008C */   swc1      $f16, 0x8C($a0)
endlabel TimeGadgetParms_setPosF_000144B4
