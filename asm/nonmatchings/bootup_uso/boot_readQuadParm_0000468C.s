nonmatching boot_readQuadParm_0000468C, 0x30

glabel boot_readQuadParm_0000468C
    /* DD50F8 0000468C 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD50FC 00004690 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD5100 00004694 AFA40020 */  sw         $a0, 0x20($sp)
    /* DD5104 00004698 0C0008B8 */  jal        stream_readS32_000022E0
    /* DD5108 0000469C 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DD510C 000046A0 8FA40020 */  lw         $a0, 0x20($sp)
    /* DD5110 000046A4 0C0008F2 */  jal        stream_readQuad_000023C8
    /* DD5114 000046A8 24840010 */   addiu     $a0, $a0, 0x10
    /* DD5118 000046AC 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD511C 000046B0 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD5120 000046B4 03E00008 */  jr         $ra
    /* DD5124 000046B8 00000000 */   nop
endlabel boot_readQuadParm_0000468C
