nonmatching boot_readFloatParm_00000B50, 0x30

glabel boot_readFloatParm_00000B50
    /* DD15BC 00000B50 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD15C0 00000B54 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD15C4 00000B58 AFA40020 */  sw         $a0, 0x20($sp)
    /* DD15C8 00000B5C 0C000002 */  jal        stream_readS32_00000008
    /* DD15CC 00000B60 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DD15D0 00000B64 8FA40020 */  lw         $a0, 0x20($sp)
    /* DD15D4 00000B68 0C000011 */  jal        stream_readF32_00000044
    /* DD15D8 00000B6C 24840010 */   addiu     $a0, $a0, 0x10
    /* DD15DC 00000B70 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD15E0 00000B74 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD15E4 00000B78 03E00008 */  jr         $ra
    /* DD15E8 00000B7C 00000000 */   nop
endlabel boot_readFloatParm_00000B50
