nonmatching boot_readVec3Parm_0000EB58, 0x30

glabel boot_readVec3Parm_0000EB58
    /* DDF5C4 0000EB58 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDF5C8 0000EB5C AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF5CC 0000EB60 AFA40020 */  sw         $a0, 0x20($sp)
    /* DDF5D0 0000EB64 0C003A2F */  jal        stream_readS32_0000E8BC
    /* DDF5D4 0000EB68 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DDF5D8 0000EB6C 8FA40020 */  lw         $a0, 0x20($sp)
    /* DDF5DC 0000EB70 0C003A4D */  jal        stream_readVec3_0000E934
    /* DDF5E0 0000EB74 24840010 */   addiu     $a0, $a0, 0x10
    /* DDF5E4 0000EB78 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF5E8 0000EB7C 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDF5EC 0000EB80 03E00008 */  jr         $ra
    /* DDF5F0 0000EB84 00000000 */   nop
endlabel boot_readVec3Parm_0000EB58
