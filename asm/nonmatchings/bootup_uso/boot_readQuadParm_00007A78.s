nonmatching boot_readQuadParm_00007A78, 0x30

glabel boot_readQuadParm_00007A78
    /* DD84E4 00007A78 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD84E8 00007A7C AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD84EC 00007A80 AFA40020 */  sw         $a0, 0x20($sp)
    /* DD84F0 00007A84 0C0013FC */  jal        stream_readS32_00004FF0
    /* DD84F4 00007A88 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DD84F8 00007A8C 8FA40020 */  lw         $a0, 0x20($sp)
    /* DD84FC 00007A90 0C00184F */  jal        stream_readQuad_0000613C
    /* DD8500 00007A94 24840010 */   addiu     $a0, $a0, 0x10
    /* DD8504 00007A98 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD8508 00007A9C 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD850C 00007AA0 03E00008 */  jr         $ra
    /* DD8510 00007AA4 00000000 */   nop
endlabel boot_readQuadParm_00007A78
