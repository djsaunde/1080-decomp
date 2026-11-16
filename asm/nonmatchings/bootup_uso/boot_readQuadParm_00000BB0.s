nonmatching boot_readQuadParm_00000BB0, 0x30

glabel boot_readQuadParm_00000BB0
    /* DD161C 00000BB0 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD1620 00000BB4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD1624 00000BB8 AFA40020 */  sw         $a0, 0x20($sp)
    /* DD1628 00000BBC 0C000002 */  jal        stream_readS32_00000008
    /* DD162C 00000BC0 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DD1630 00000BC4 8FA40020 */  lw         $a0, 0x20($sp)
    /* DD1634 00000BC8 0C00003C */  jal        stream_readQuad_000000F0
    /* DD1638 00000BCC 24840010 */   addiu     $a0, $a0, 0x10
    /* DD163C 00000BD0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD1640 00000BD4 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD1644 00000BD8 03E00008 */  jr         $ra
    /* DD1648 00000BDC 00000000 */   nop
endlabel boot_readQuadParm_00000BB0
