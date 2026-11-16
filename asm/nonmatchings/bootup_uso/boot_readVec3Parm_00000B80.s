nonmatching boot_readVec3Parm_00000B80, 0x30

glabel boot_readVec3Parm_00000B80
    /* DD15EC 00000B80 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD15F0 00000B84 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD15F4 00000B88 AFA40020 */  sw         $a0, 0x20($sp)
    /* DD15F8 00000B8C 0C000002 */  jal        stream_readS32_00000008
    /* DD15FC 00000B90 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DD1600 00000B94 8FA40020 */  lw         $a0, 0x20($sp)
    /* DD1604 00000B98 0C000020 */  jal        stream_readVec3_00000080
    /* DD1608 00000B9C 24840010 */   addiu     $a0, $a0, 0x10
    /* DD160C 00000BA0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD1610 00000BA4 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD1614 00000BA8 03E00008 */  jr         $ra
    /* DD1618 00000BAC 00000000 */   nop
endlabel boot_readVec3Parm_00000B80
