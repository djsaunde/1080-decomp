nonmatching boot_readVec3Parm_0000F090, 0x30

glabel boot_readVec3Parm_0000F090
    /* DDFAFC 0000F090 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDFB00 0000F094 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDFB04 0000F098 AFA40020 */  sw         $a0, 0x20($sp)
    /* DDFB08 0000F09C 0C003BC8 */  jal        stream_readS32_0000EF20
    /* DDFB0C 0000F0A0 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DDFB10 0000F0A4 8FA40020 */  lw         $a0, 0x20($sp)
    /* DDFB14 0000F0A8 0C003BE6 */  jal        stream_readVec3_0000EF98
    /* DDFB18 0000F0AC 24840010 */   addiu     $a0, $a0, 0x10
    /* DDFB1C 0000F0B0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDFB20 0000F0B4 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDFB24 0000F0B8 03E00008 */  jr         $ra
    /* DDFB28 0000F0BC 00000000 */   nop
endlabel boot_readVec3Parm_0000F090
