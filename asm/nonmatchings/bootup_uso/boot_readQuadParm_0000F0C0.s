nonmatching boot_readQuadParm_0000F0C0, 0x30

glabel boot_readQuadParm_0000F0C0
    /* DDFB2C 0000F0C0 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDFB30 0000F0C4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDFB34 0000F0C8 AFA40020 */  sw         $a0, 0x20($sp)
    /* DDFB38 0000F0CC 0C003BC8 */  jal        stream_readS32_0000EF20
    /* DDFB3C 0000F0D0 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DDFB40 0000F0D4 8FA40020 */  lw         $a0, 0x20($sp)
    /* DDFB44 0000F0D8 0C003C02 */  jal        stream_readQuad_0000F008
    /* DDFB48 0000F0DC 24840010 */   addiu     $a0, $a0, 0x10
    /* DDFB4C 0000F0E0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDFB50 0000F0E4 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDFB54 0000F0E8 03E00008 */  jr         $ra
    /* DDFB58 0000F0EC 00000000 */   nop
endlabel boot_readQuadParm_0000F0C0
