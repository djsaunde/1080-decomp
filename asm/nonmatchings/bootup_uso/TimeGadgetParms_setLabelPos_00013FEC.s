nonmatching TimeGadgetParms_setLabelPos_00013FEC, 0x18

glabel TimeGadgetParms_setLabelPos_00013FEC
    /* DE4A58 00013FEC AC850074 */  sw         $a1, 0x74($a0)
    /* DE4A5C 00013FF0 AC86008C */  sw         $a2, 0x8C($a0)
    /* DE4A60 00013FF4 AC8700A4 */  sw         $a3, 0xA4($a0)
    /* DE4A64 00013FF8 8FAE0010 */  lw         $t6, 0x10($sp)
    /* DE4A68 00013FFC 03E00008 */  jr         $ra
    /* DE4A6C 00014000 AC8E00BC */   sw        $t6, 0xBC($a0)
endlabel TimeGadgetParms_setLabelPos_00013FEC
