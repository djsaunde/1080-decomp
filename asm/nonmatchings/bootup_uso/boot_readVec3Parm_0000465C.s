nonmatching boot_readVec3Parm_0000465C, 0x30

glabel boot_readVec3Parm_0000465C
    /* DD50C8 0000465C 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD50CC 00004660 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD50D0 00004664 AFA40020 */  sw         $a0, 0x20($sp)
    /* DD50D4 00004668 0C0008B8 */  jal        stream_readS32_000022E0
    /* DD50D8 0000466C 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DD50DC 00004670 8FA40020 */  lw         $a0, 0x20($sp)
    /* DD50E0 00004674 0C0008D6 */  jal        stream_readVec3_00002358
    /* DD50E4 00004678 24840010 */   addiu     $a0, $a0, 0x10
    /* DD50E8 0000467C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD50EC 00004680 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD50F0 00004684 03E00008 */  jr         $ra
    /* DD50F4 00004688 00000000 */   nop
endlabel boot_readVec3Parm_0000465C
