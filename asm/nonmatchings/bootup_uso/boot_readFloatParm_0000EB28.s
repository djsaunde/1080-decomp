nonmatching boot_readFloatParm_0000EB28, 0x30

glabel boot_readFloatParm_0000EB28
    /* DDF594 0000EB28 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDF598 0000EB2C AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF59C 0000EB30 AFA40020 */  sw         $a0, 0x20($sp)
    /* DDF5A0 0000EB34 0C003A2F */  jal        stream_readS32_0000E8BC
    /* DDF5A4 0000EB38 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DDF5A8 0000EB3C 8FA40020 */  lw         $a0, 0x20($sp)
    /* DDF5AC 0000EB40 0C003A3E */  jal        stream_readF32_0000E8F8
    /* DDF5B0 0000EB44 24840010 */   addiu     $a0, $a0, 0x10
    /* DDF5B4 0000EB48 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF5B8 0000EB4C 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDF5BC 0000EB50 03E00008 */  jr         $ra
    /* DDF5C0 0000EB54 00000000 */   nop
endlabel boot_readFloatParm_0000EB28
