nonmatching boot_readFloatParm_00007A48, 0x30

glabel boot_readFloatParm_00007A48
    /* DD84B4 00007A48 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD84B8 00007A4C AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD84BC 00007A50 AFA40020 */  sw         $a0, 0x20($sp)
    /* DD84C0 00007A54 0C0013FC */  jal        stream_readS32_00004FF0
    /* DD84C4 00007A58 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DD84C8 00007A5C 8FA40020 */  lw         $a0, 0x20($sp)
    /* DD84CC 00007A60 0C001840 */  jal        stream_readF32_00006100
    /* DD84D0 00007A64 24840010 */   addiu     $a0, $a0, 0x10
    /* DD84D4 00007A68 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD84D8 00007A6C 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD84DC 00007A70 03E00008 */  jr         $ra
    /* DD84E0 00007A74 00000000 */   nop
endlabel boot_readFloatParm_00007A48
