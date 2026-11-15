nonmatching boot_readFloatParm_0000F060, 0x30

glabel boot_readFloatParm_0000F060
    /* DDFACC 0000F060 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDFAD0 0000F064 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDFAD4 0000F068 AFA40020 */  sw         $a0, 0x20($sp)
    /* DDFAD8 0000F06C 0C003BC8 */  jal        stream_readS32_0000EF20
    /* DDFADC 0000F070 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DDFAE0 0000F074 8FA40020 */  lw         $a0, 0x20($sp)
    /* DDFAE4 0000F078 0C003BD7 */  jal        stream_readF32_0000EF5C
    /* DDFAE8 0000F07C 24840010 */   addiu     $a0, $a0, 0x10
    /* DDFAEC 0000F080 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDFAF0 0000F084 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDFAF4 0000F088 03E00008 */  jr         $ra
    /* DDFAF8 0000F08C 00000000 */   nop
endlabel boot_readFloatParm_0000F060
