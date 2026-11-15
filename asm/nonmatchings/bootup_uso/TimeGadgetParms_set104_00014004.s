nonmatching TimeGadgetParms_set104_00014004, 0xC

glabel TimeGadgetParms_set104_00014004
    /* DE4A70 00014004 AC850104 */  sw         $a1, 0x104($a0)
    /* DE4A74 00014008 03E00008 */  jr         $ra
    /* DE4A78 0001400C AC860108 */   sw        $a2, 0x108($a0)
endlabel TimeGadgetParms_set104_00014004
